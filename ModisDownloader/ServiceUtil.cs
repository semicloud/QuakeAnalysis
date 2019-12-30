using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Threading;
using System.Threading.Tasks;
using System.Xml.Serialization;
using Antlr4.StringTemplate;
using NLog;
using Timer = System.Timers.Timer;

namespace ModisDownloader
{
    public static class ServiceUtil
    {
        private static Logger g = LogManager.GetCurrentClassLogger();

        /// <summary>
        /// 该Logger用于在界面上向用户展示进度
        /// </summary>
        private static Logger uiLogger = LogManager.GetLogger("ui");

        private static HttpClient client = new HttpClient();

        #region 用于下载速度的单位转换

        private static readonly string[] SizeSuffixes =
             { "bytes", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB" };

        private static string SizeSuffix(Int64 value, int decimalPlaces = 1)
        {
            if (decimalPlaces < 0) { throw new ArgumentOutOfRangeException("decimalPlaces"); }
            if (value < 0) { return "-" + SizeSuffix(-value); }
            if (value == 0) { return string.Format("{0:n" + decimalPlaces + "} bytes", 0); }

            // mag is 0 for bytes, 1 for KB, 2, for MB, etc.
            int mag = (int)Math.Log(value, 1024);

            // 1L << (mag * 10) == 2 ^ (10 * mag) 
            // [i.e. the number of bytes in the unit corresponding to mag]
            decimal adjustedSize = (decimal)value / (1L << (mag * 10));

            // make adjustment when the value is large enough that
            // it would round up to 1000 or more
            if (Math.Round(adjustedSize, decimalPlaces) >= 1000)
            {
                mag += 1;
                adjustedSize /= 1024;
            }

            return string.Format("{0:n" + decimalPlaces + "} {1}/s",
                adjustedSize,
                SizeSuffixes[mag]);
        }

        #endregion


        #region 下载文件

        /// <summary>
        /// 下载Modis文件
        /// </summary>
        /// <param name="filesToDownload">待下载的文件对象</param>
        /// <param name="directoryToSave">下载存放目录</param>
        /// <param name="currentDownloadFileIndex">当前下载的文件索引</param>
        /// <param name="currentDownloadFilePercentage"></param>
        /// <param name="downloadSpeedInfo"></param>
        /// <param name="cancellationToken"></param>
        /// <returns></returns>
        public static async Task DownloadFileTaskAsync(List<DownloadFile> filesToDownload, string directoryToSave,
            IProgress<int> currentDownloadFileIndex,
            IProgress<int> currentDownloadFilePercentage,
            IProgress<string> downloadSpeedInfo, CancellationToken cancellationToken)
        {
            for (int i = 0; i < filesToDownload.Count; i++)
            {
                if (cancellationToken.IsCancellationRequested)
                {
                    g.Debug("user cancelled all download work..");
                    break;
                }

                var downloadFile = filesToDownload[i];
                var destFilePath = directoryToSave + downloadFile.Name;
                uiLogger.Info("开始下载{0}..", downloadFile.Name);
                g.Debug("begin downlaod {0}, with appkey {1}", downloadFile, Global.Config.AppKey);

                using (var webClient = new WebClient())
                {
                    // 这两个参数用于计算下载速度
                    long currReceived = 0;
                    long prevReceived = 0;
                    // 定义一个计时器，用于计算下载速度
                    var timer = new Timer(1000);
                    timer.Elapsed += (sender, args) =>
                    {
                        // 如果用户取消下载，则不再报告下载速度
                        if (cancellationToken.IsCancellationRequested)
                        {
                            downloadSpeedInfo.Report(string.Empty);
                            return;
                        }
                        downloadSpeedInfo.Report(SizeSuffix(currReceived - prevReceived));
                        prevReceived = currReceived;
                    };
                    // Header必须
                    webClient.Headers.Add("User-Agent", Global.Config.UserAgent);
                    // 不加AppKey也能下载了，接口留在这里，如果需要AppKey的话可以直接加上
                    webClient.Headers.Add("Authorization", $@"Bearer {Global.Config.AppKey}");
                    webClient.DownloadProgressChanged += (sender, args) =>
                    {
                        // 如果用户取消下载，则不再报告下载进度
                        if (cancellationToken.IsCancellationRequested)
                        {
                            //g.Debug("user cancel downlaod, not longer report file download progress..");
                            currentDownloadFilePercentage.Report(100);
                            return;
                        }
                        currentDownloadFilePercentage.Report(args.ProgressPercentage);
                        currReceived = args.BytesReceived;
                    };

                    webClient.DownloadDataCompleted += async (sender, args) =>
                    {
                        // 如果用户取消下载，则直接退出，不再保存文件
                        if (cancellationToken.IsCancellationRequested)
                        {
                            timer.Stop();
                            timer.Dispose();
                            g.Debug("user cancel downlaod, not file will create..");
                            return;
                        }

                        if (args.Error != null)
                        {
                            timer.Stop();
                            timer.Dispose();
                            g.Error("error occured during download.. message: {0}", args.Error.Message);
                            throw args.Error;
                        }

                        var downloadData = args.Result;
                        timer.Stop();
                        timer.Dispose();
                        // TODO CheckSum
                        using (var fileStream = new FileStream(destFilePath, FileMode.Create))
                        {
                            await fileStream.WriteAsync(downloadData, 0, downloadData.Length);
                            uiLogger.Info("文件已下载至：{0}", destFilePath);
                        }
                    };

                    currentDownloadFileIndex.Report(i);

                    timer.Start();

                    // DownloadDataTaskAsync方法无法通过webClient.CancelAsync()取消
                    await webClient.DownloadDataTaskAsync(downloadFile.Url);
                }
            }
        }

        /// <summary>
        /// 下载Modis文件
        /// </summary>
        /// <param name="downloadFiles">待下载的文件对象</param>
        /// <param name="directory">下载到的目录，注意该字符串最后应为\，例如E:\Data\而不能是E:\Data </param>
        /// <param name="totalProgress">全部进度对象</param>
        /// <param name="fileProgress">单文件下载进度对象</param>
        /// <returns></returns>
        [Obsolete]
        public static async Task DownloadFile(List<DownloadFile> downloadFiles, string directory,
            IProgress<Tuple<int, string>> totalProgress, IProgress<int> fileProgress)
        {
            for (int i = 0; i < downloadFiles.Count; i++)
            {
                var downloadFile = downloadFiles[i];
                var destFilePath = directory + downloadFile.Name;
                using (var webClient = new WebClient())
                {
                    // Header必须
                    webClient.Headers.Add("User-Agent", Global.Config.UserAgent);
                    webClient.Headers.Add("Authorization", $@"Bearer {Global.Config.AppKey}");

                    webClient.DownloadProgressChanged += (sender, args) =>
                    {
                        fileProgress.Report(args.ProgressPercentage);
                    };

                    webClient.DownloadDataCompleted += async (sender, args) =>
                    {
                        if (args.Error != null)
                        {
                            uiLogger.Error($"@下载文件{downloadFile.Name}时出现错误：${args.Error.Message}！");
                            throw args.Error;
                        }

                        var downloadData = args.Result;
                        // TODO CheckSum
                        using (var fileStream = new FileStream(destFilePath, FileMode.Create))
                        {
                            await fileStream.WriteAsync(downloadData, 0, downloadData.Length);
                        }
                    };

                    totalProgress.Report(new Tuple<int, string>(i + 1, downloadFile.Name));

                    await webClient.DownloadDataTaskAsync(new Uri(downloadFile.Url));
                    uiLogger.Info(destFilePath + "下载完成..");
                }
            }
        }

        #endregion

        #region 生成查询URL

        /// <summary>
        /// 获取一组查询FileId的URL
        /// </summary>
        /// <param name="query"></param>
        /// <returns></returns>
        public static List<string> GetFileIdServiceUrls(Query query)
        {
            return query.ProductNames.Select(pn => GetFileIdServiceUrl(pn, query.DateStart, query.DateStop,
                query.MaxLat, query.MinLat, query.MinLng, query.MaxLng,
                query.DnbType)).ToList();
        }

        /// <summary>
        /// 获取一个查询FileId的Service Url
        /// </summary>
        /// <param name="productName">产品名</param>
        /// <param name="start">开始日期</param>
        /// <param name="stop">结束日期</param>
        /// <param name="north">北</param>
        /// <param name="south">南</param>
        /// <param name="west">西</param>
        /// <param name="east">东</param>
        /// <param name="dayNightBoth">dayNightBoth字符串</param>
        /// <returns>查询FileId的Service Url</returns>
        /// <remarks>
        /// 注意：在该方法中修改了MOD11和MYD11的下载路径
        /// </remarks>
        private static string GetFileIdServiceUrl(String productName, DateTime start, DateTime stop, double north,
            double south,
            double west, double east, string dayNightBoth)
        {
            var cfg = ModisDownloaderConfig.CreateInstance();
            var template = new Template(cfg.FileIdService);
            template.Add("product_name", productName);
            template.Add("start", start.Date.ToString("yyyy-MM-dd"));
            template.Add("stop", stop.Date.ToString("yyyy-MM-dd"));
            template.Add("north", north);
            template.Add("south", south);
            template.Add("west", west);
            template.Add("east", east);
            template.Add("day_night_both", dayNightBoth);
            var service = template.Render();
            //在这里修改了对MOD11A1、MYD11A1数据集的URL，这两个数据用使用的是6版本，而非61版本
            if (productName.Contains("MOD11") || productName.Contains("MYD11"))
            {
                service = service.Replace("collection=61", "collection=6");
            }

            g.Debug("file-id-service: {0}", service);
            return service;
        }

        /// <summary>
        /// 通过文件Id构建文件下载地址查询Service URL
        /// </summary>
        /// <param name="fileIdList"></param>
        /// <returns></returns>
        private static string GetFileUrlsServiceUrl(List<string> fileIdList)
        {
            var cfg = ModisDownloaderConfig.CreateInstance();
            var template = new Template(cfg.FileUrlService);
            template.Add("file_ids", string.Join(",", fileIdList));
            var service = template.Render();
            g.Debug("file-url-service: {0}", service);
            return service;
        }

        /// <summary>
        /// 通过文件Id构建文件属性查询Service URL
        /// </summary>
        /// <param name="fileIdList"></param>
        /// <returns></returns>
        private static string GetFilePropertiesServiceUrl(List<string> fileIdList)
        {
            var cfg = ModisDownloaderConfig.CreateInstance();
            var template = new Template(cfg.FilePropertyService);
            template.Add("file_ids", string.Join(",", fileIdList));
            var service = template.Render();
            g.Debug("file-properties-service: {0}", service);
            return service;
        }

        #endregion

        #region 调用WebService进行文件属性和下载地址的查询

        /// <summary>
        /// 通过Service获取文件Id
        /// </summary>
        /// <param name="urls">Service Url</param>
        /// <param name="progress">过程对象</param>
        /// <returns></returns>
        public static async Task<List<string>> GetFileIdsAsync(List<string> urls, IProgress<int> progress)
        {
            var allFileIdList = new List<string>();
            for (int i = 0; i < urls.Count; i++)
            {
                var respStr = await client.GetStringAsync(urls[i]);
                using (var reader = new StringReader(respStr))
                {
                    var serializer = new XmlSerializer(typeof(searchForFilesResponse));
                    var searchForFilesResponse = (searchForFilesResponse)serializer.Deserialize(reader);
                    var fileIdList = searchForFilesResponse.Items.ToList().Select(x => x.Value).ToList();
                    var count1 = fileIdList.Count;
                    long t;
                    var errFileIdList = fileIdList.FindAll(fid => !long.TryParse(fid, out t));
                    if (errFileIdList.Any())
                    {
                        uiLogger.Warn($"在[{urls[i]}]未查询到相关文件..");
                        g.Warn("err file id found in [{0}]", urls[i]);
                        errFileIdList.ForEach(g.Warn);
                        errFileIdList.ForEach(f => fileIdList.Remove(f));
                        g.Debug($"remove file ids from list, [{count1}->{fileIdList.Count}]");
                    }

                    allFileIdList.AddRange(fileIdList);
                    g.Debug("fetch [{0}] file id from service [{1}]", fileIdList.Count, urls[i]);
                }

                int iProgress = (int)Math.Round((double)(i + 1) / urls.Count * 100);
                progress.Report(iProgress);
            }

            g.Debug("fetch [{0}] file id via [{1}] urls", allFileIdList.Count, urls.Count);
            return allFileIdList;
        }

        /// <summary>
        /// 通过文件Id获取文件的属性
        /// </summary>
        /// <param name="fileIdList"></param>
        /// <param name="progress"></param>
        /// <returns></returns>
        public static async Task<List<getFilePropertiesResponseReturn>> GetFilePropertiesAsync(List<string> fileIdList,
            IProgress<int> progress)
        {
            var ans = new List<getFilePropertiesResponseReturn>();
            g.Debug("there is [{0}] file to get properties..");
            var listOfFileNameList = fileIdList.Split();
            g.Debug("List Count: {0}, Per Count:{1}", listOfFileNameList.Count,
                string.Join(", ", listOfFileNameList.Select(l => l.Count)));
            for (int i = 0; i < listOfFileNameList.Count; i++)
            {
                var serviceUrl = GetFilePropertiesServiceUrl(listOfFileNameList[i]);
                var resp = await client.GetStringAsync(serviceUrl);
                using (var reader = new StringReader(resp))
                {
                    var serializer = new XmlSerializer(typeof(getFilePropertiesResponse));
                    var getFilePropertiesResponse = (getFilePropertiesResponse)serializer.Deserialize(reader);
                    ans.AddRange(getFilePropertiesResponse.Items.ToList());
                }
                int iProgress = (int)Math.Round((double)(i + 1) / listOfFileNameList.Count * 100);
                progress.Report(iProgress);
            }

            return ans;
        }

        /// <summary>
        /// 通过文件Id获取文件下载Url
        /// </summary>
        /// <param name="fileIds">文件Id列表</param>
        /// <param name="progress">进度对象</param>
        /// <returns></returns>
        public static async Task<List<string>> GetFileUrlsAsync(List<string> fileIds, IProgress<int> progress)
        {
            var ans = new List<string>();
            var listOfFileIdList = fileIds.Split();
            for (int i = 0; i < listOfFileIdList.Count; i++)
            {
                var serviceUrl = GetFileUrlsServiceUrl(listOfFileIdList[i]);
                var resp = await client.GetStringAsync(serviceUrl);
                using (var reader = new StringReader(resp))
                {
                    // I really love xsd.exe
                    var serializer = new XmlSerializer(typeof(getFileUrlsResponse));
                    var getFileUrlsResponse = (getFileUrlsResponse)serializer.Deserialize(reader);
                    ans.AddRange(getFileUrlsResponse.Items.Select(x => x.Value));
                }

                int iProgress = (int)Math.Round((double)(i + 1) / listOfFileIdList.Count * 100);
                progress.Report(iProgress);
            }

            return ans;
        }

        #endregion

        //public static List<string> GetFileNames(List<string> serviceUrls)
        //{
        //    var stopwatch = new Stopwatch();
        //    stopwatch.Start();
        //    var fileNames = new List<string>();
        //    Task.WaitAll(serviceUrls.Select(url =>
        //    {
        //        return client.GetStringAsync(url).ContinueWith(resp =>
        //       {
        //           using (var reader = new StringReader(resp.Result))
        //           {
        //               var xmlSerializer = new XmlSerializer(typeof(searchForFilesResponse));
        //               var deserialize = (searchForFilesResponse)xmlSerializer.Deserialize(reader);
        //               deserialize.Items.ToList().ForEach(i => fileNames.Add(i.Value));
        //           }
        //       });
        //    }).ToArray());
        //    g.Debug("retrieve {0} filenames via {1} urls.", fileNames.Count, serviceUrls.Count);
        //    g.Debug("cost {0}s", stopwatch.Elapsed.Seconds);
        //    return fileNames;
        //}
    }
}
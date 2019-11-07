using NUnit.Framework;
using System;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using OSGeo.GDAL;
using OSGeo.OGR;
using OSGeo.OSR;
using QuakeAnalysis.Entity;
using QuakeAnalysis.Method.Modis;
using YamlDotNet.Serialization;
using YamlDotNet.Serialization.NamingConventions;

namespace QuakeAnalysis.Test
{
    public static class MyTest
    {
        [Test]
        public static void TestGenerateWaterVaporPreprocessInputFile()
        {
            // 生成亮温预处理的InputFile
            string workspaceDir = @"E:\modis_workspace\";
            string type = "MOD";
            string dateStr = "2017353";

            string year = dateStr.Substring(0, 4);
            string day = dateStr.Substring(4, 3);
            string aerosolHdfDir = $@"{workspaceDir}{type}05_L2\{year}\{day}\";

            var directoryInfo = new DirectoryInfo(aerosolHdfDir);
            var sb = new StringBuilder();
            foreach (System.IO.FileInfo file in directoryInfo.EnumerateFiles())
            {
                sb.AppendLine(file.FullName);
            }

            File.WriteAllText(@"E:\CodeWorld\VSProjects\QuakeAnalysis\QuakeAnalysis\ymlsamples\proc_MxD05_L2_hdf_list.txt", sb.ToString());
        }

        [Test]
        public static void TestGenerateAerosolPreprocessInputFile()
        {
            // 生成亮温预处理的InputFile
            string workspaceDir = @"E:\modis_workspace\";
            string type = "MOD";
            string dateStr = "2016353";

            string year = dateStr.Substring(0, 4);
            string day = dateStr.Substring(4, 3);
            string aerosolHdfDir = $@"{workspaceDir}{type}04_3k\{year}\{day}\";

            var directoryInfo = new DirectoryInfo(aerosolHdfDir);
            var sb = new StringBuilder();
            foreach (System.IO.FileInfo file in directoryInfo.EnumerateFiles())
            {
                sb.AppendLine(file.FullName);
            }

            File.WriteAllText(@"D:\proc_MOD04_hdf_list.txt", sb.ToString());
        }

        [Test]
        public static void TestGenerateBtPreprocessInputFile()
        {
            // 生成亮温预处理的InputFile
            string workspaceDir = @"E:\modis_workspace\";
            string type = "MOD";
            string dateStr = "2016352";

            string year = dateStr.Substring(0, 4);
            string day = dateStr.Substring(4, 3);
            string btHdfDir = $@"{workspaceDir}{type}021KM\{year}\{day}\";
            string szaHdfDir = $@"{workspaceDir}{type}03\{year}\{day}\";
            string cmHdfDir = $@"{workspaceDir}{type}35_L2\{year}\{day}\";

            var directoryInfo = new DirectoryInfo(btHdfDir);
            var sb = new StringBuilder();
            foreach (System.IO.FileInfo file in directoryInfo.EnumerateFiles())
            {
                string searchStr = file.Name.Substring(9, 13);
                Console.WriteLine(searchStr);
                var szaFiles = Directory.EnumerateFiles(szaHdfDir).ToList();
                string szaFileName = szaFiles.Find(s => s.Contains(searchStr));
                var cmFiles = Directory.EnumerateFiles(cmHdfDir).ToList();
                string cmFileName = cmFiles.Find(s => s.Contains(searchStr));
                if (!string.IsNullOrEmpty(szaFileName) && !string.IsNullOrEmpty(cmFileName))
                {
                    sb.AppendLine("#"); //#开头
                    sb.AppendLine(file.FullName);
                    sb.AppendLine(szaFileName);
                    sb.AppendLine(cmFileName);
                }
                else
                {
                    Console.WriteLine("不存在的Sza或CM文件！");
                }
            }

            File.WriteAllText(@"D:\proc_MxD021km_hdf_list.txt", sb.ToString());
        }

        /// <summary>
        /// 预处理MXD02
        /// </summary>
        [Test]
        public static void TestPreProcessMXD02()
        {
            ModisPreprocess.PreprocessMXD02(
                @"e:\CodeWorld\VSProjects\QuakeAnalysis\QuakeAnalysis\QuakeAnalysis\bin\Debug\modis_workspace\MXD02\MYD02\MYD021KM.A2018305.0630.061.2018305193520.hdf");

        }

        /// <summary>
        /// 调用gdal_translate.exe转换数据格式
        /// </summary>
        [Test]
        public static void TestGdalTranslate()
        {
            var dataset =
                Gdal.OpenShared(
                    @"e:\CodeWorld\VSProjects\QuakeAnalysis\QuakeAnalysis\QuakeAnalysis\bin\Debug\modis_workspace\Tmp\MXD02_EV_1KM_Emissive_b0.tif",
                    Access.GA_ReadOnly);
            Gdal.wrapper_GDALTranslate(@"d:\ok.tif", dataset, new GDALTranslateOptions(new[] { "-ot", "Float32" }), null,
                null);
            dataset.Dispose();
        }

        /// <summary>
        /// 调用GDAL生成等值线Contour图
        /// </summary>
        [Test]
        public static void TestGdalContour()
        {
            //GdalConfiguration.ConfigureGdal();
            //GdalConfiguration.ConfigureOgr();
            //Gdal.AllRegister();

            var dataset =
                Gdal.OpenShared(
                    @"e:\CodeWorld\VSProjects\QuakeAnalysis\QuakeAnalysis\QuakeAnalysis\bin\Debug\modis_workspace\Tmp\MXD02_EV_1KM_Emissive_b0_Float.tif",
                    Access.GA_ReadOnly);
            var band = dataset.GetRasterBand(1);

            var output =
                @"e:\CodeWorld\VSProjects\QuakeAnalysis\QuakeAnalysis\QuakeAnalysis\bin\Debug\modis_workspace\Tmp\MXD02_EV_1KM_Emissive_b0_Float.shp";
            var shpDriver = Ogr.GetDriverByName("ESRI Shapefile");
            var dataSource = shpDriver.CreateDataSource(output, null);
            SpatialReference spatialReference = new SpatialReference(dataset.GetProjectionRef());
            var layer = dataSource.CreateLayer("Elevation", spatialReference, wkbGeometryType.wkbLineString, null);
            FieldDefn fieldDefn = new FieldDefn("Elevation", FieldType.OFTInteger);
            layer.CreateField(fieldDefn, 0);

            Gdal.ContourGenerate(band, 3, 0, 0, null, 0, 0, layer, -1, 0, null, null);

            band.Dispose();
            layer.Dispose();
            dataset.Dispose();
            dataSource.Dispose();
            shpDriver.Dispose();
        }

        /// <summary>
        /// 二进制转换及比较
        /// </summary>
        [Test]
        public static void TestBinaryConvert()
        {
            var bVal = Convert.ToString(185, 2);
            var iVal = Convert.ToInt32(bVal, 2);
            Console.WriteLine(bVal);
            Console.WriteLine(iVal);
            Console.WriteLine(Convert.ToInt32("00000110", 2));
            Console.WriteLine(255 & 6);
        }

        /// <summary>
        /// 从GTIFF文件中读取波段DN值
        /// </summary>
        [Test]
        public static void TestReadDNValueFromGTiff()
        {
            ModisPreprocess.GetDNMatrix(
                @"e:\CodeWorld\VSProjects\QuakeAnalysis\QuakeAnalysis\QuakeAnalysis\bin\Debug\modis_workspace\Tmp\MXD02_EV_1KM_Emissive_b0.tif",
                1);
        }

        [Test]
        public static void TestGetArchiveFilePath()
        {
            //HdfFile file = new HdfFile(@"e:\Data\MOD35_L2.A2018352.0410.061.2018352131617.hdf");
            //Assert.AreEqual(
            //    @"e:\CodeWorld\VSProjects\QuakeAnalysis\QuakeAnalysis\QuakeAnalysis\bin\Debug\modis_workspace\MXD35\MOD35\MOD35_L2.A2018352.0410.061.2018352131617.hdf"
            //        .ToLower(),
            //    file.GetArchiveFilePath().ToLower());

            //HdfFile file2 = new HdfFile(@"e:\Data\MYD021KM.A2018354.1915.061.2018355165601.hdf");
            //Assert.AreEqual(
            //    @"e:\CodeWorld\VSProjects\QuakeAnalysis\QuakeAnalysis\QuakeAnalysis\bin\Debug\modis_workspace\MXD02\MYD02\MYD021KM.A2018354.1915.061.2018355165601.hdf"
            //        .ToLower()
            //    , file2.GetArchiveFilePath().ToLower());

        }

        [OneTimeSetUp]
        public static void RunBeforeAnyTests()
        {
            Console.WriteLine("I am here...");
            GdalConfiguration.ConfigureGdal();
            GdalConfiguration.ConfigureOgr();
            Gdal.AllRegister();

            var dir = Path.GetDirectoryName(typeof(ConfigTest).Assembly.Location);
            if (dir != null)
            {
                Environment.CurrentDirectory = dir;
                Directory.SetCurrentDirectory(dir);
            }
            else
                throw new Exception("Path.GetDirectoryName(typeof(TestingWithReferencedFiles).Assembly.Location) returned null");
        }

        /// <summary>
        /// 读取yaml配置文件
        /// </summary>
        [Test]
        public static void TestCConfig()
        {
            var cfg = QuakeAnalysisConfig.CreateInstance();
            //Console.WriteLine(cconfig.DataDirectory.Hdf.Mod02);
            Console.WriteLine(cfg.Version);
            Console.WriteLine(cfg.DataDirectory.Hdf.Mod02);
            Console.WriteLine(cfg.DataDirectory.Hdf.Myd35);
            Console.WriteLine(cfg.DataDirectory.Standard.Mod02);
            Console.WriteLine(cfg.DataDirectory.StandardMap.Mod02);
            Console.WriteLine(cfg.DataDirectory.StandardMap.Myd02);

            Console.WriteLine(cfg.DefaultParameter.Mxd02Sds);
            Console.WriteLine(cfg.DefaultParameter.Mxd03Sds);
            Console.WriteLine(cfg.DefaultParameter.Mxd35Sds);

            Console.WriteLine(cfg.DefaultParameter.UlcLng);
            Console.WriteLine(cfg.DefaultParameter.UlcLat);
            Console.WriteLine(cfg.DefaultParameter.LrcLng);
            Console.WriteLine(cfg.DefaultParameter.LrcLat);


            Console.WriteLine(Environment.CurrentDirectory + "\\" + cfg.DataDirectory.Hdf.Myd02 +
                              "MYD021KM.A2018305.0630.061.2018305193520.hdf");
            Console.WriteLine(File.Exists(Environment.CurrentDirectory + "\\" + cfg.DataDirectory.Hdf.Myd02 +
                                          "MYD021KM.A2018305.0630.061.2018305193520.hdf"));


            cfg.Version = "xxxxxxxxx";
            cfg.SaveToFile();

            cfg = QuakeAnalysisConfig.CreateInstance();
            Assert.AreEqual("xxxxxxxxx", cfg.Version);

        }

        [Test]
        public static void TestDwonloadHdf()
        {
            using (var client = new WebClient())
            {
                ServicePointManager.SecurityProtocol =
                    SecurityProtocolType.Tls12 | SecurityProtocolType.Tls | SecurityProtocolType.Tls11;
                // Header必须
                client.Headers.Add("User-Agent",
                    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.140 Safari/537.36 Edge/18.17763");
                client.Headers.Add("Authorization", "Bearer 85A5F00E-FEB0-11E8-BD8E-CCB570C49BBF");
                client.DownloadProgressChanged += (sender, args) => { Console.WriteLine(args.ProgressPercentage); };

                client.DownloadFileCompleted += (sender, args) => { Console.WriteLine("我下载完了。。"); };
                client.DownloadFile(
                    new Uri(
                        @"https://ladsweb.modaps.eosdis.nasa.gov/archive/orders/501290447/MOD021KM.A2018305.0110.061.2018305133918.hdf"),
                    @"D:\ddd.hdf");
            }

            Assert.IsTrue(File.Exists(@"D:\ddd.hdf"));
        }

    }
}

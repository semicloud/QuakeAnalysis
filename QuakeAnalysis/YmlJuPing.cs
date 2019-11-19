using QuakeAnalysis.Cfg;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace QuakeAnalysis
{
    public partial class YmlGenerator
    {
        // 距平的yml生成法

        public static string GetJuPingScripts(string product, DateTime start, DateTime end)
        {
            StringBuilder sb = new StringBuilder();
            for (DateTime d = start; d <= end; d = d.AddMonths(1))
            {
                sb.AppendLine(GetJuPingScript(product, d));
            }
            return sb.ToString();
        }

        private static string GetJuPingScript(string product, DateTime date)
        {
            string anoScript = $"{GlobalModisMain.Config.ModisGeneralAno} -y {GetJuPingYmlStr(product, date)}";
            string plotAnoScript = $"{GlobalModisMain.Config.ModisPlot} -y {GetPlotJuPingYmlStr(product, date)}";
            var stringBuilder = new StringBuilder();
            stringBuilder.AppendLine(anoScript);
            stringBuilder.AppendLine(plotAnoScript);
            return stringBuilder.ToString();
        }

        private static string GetJuPingYmlStr(string product, DateTime date)
        {
            string type = product.Substring(0, 3);
            string code = product.Substring(3, 2);
            string prodName = GetProductName(code);

            string tifListStr = GetJuPingTifFilePathStr(product, date);
            string refTifListStr = GetJuPingRefTifFilePathStr(product, date);
            string tifListFilePath = $"{GlobalModisMain.Config.ScriptDir}\\jp_{product}_{date.Year}_{date.Month}_tiflist.txt";
            string refTifListFilePath = $"{GlobalModisMain.Config.ScriptDir}\\jp_{product}_{date.Year}_{date.Month}_ref_tif_list.txt";
            File.WriteAllText(tifListFilePath, tifListStr);
            File.WriteAllText(refTifListFilePath, refTifListStr);

            string outputTifPath =
                $"{GlobalModisMain.Config.WorkspaceDir}\\Ano\\JP_{type}_{prodName}\\{date.Year}\\jp_{prodName.ToLower()}_{date.Month}.tif";

            string ymlStr = $"MonthListFile: {tifListFilePath}\n" +
                            $"RefListFile: {refTifListFilePath}\n" +
                            $"OutputImageFile: {outputTifPath}\n" +
                            $"TmpPath: {GlobalModisMain.Config.TmpDir}";
            string ymlPath = $"{GlobalModisMain.Config.ScriptDir}\\jp_{product}_{date.Year}_{date.Month}.yml";

            File.WriteAllText(ymlPath, ymlStr);

            return ymlPath;
        }

        private static string GetPlotJuPingYmlStr(string product, DateTime date)
        {
            string type = product.Substring(0, 3);
            string code = product.Substring(3, 2);
            string prodName = GetProductName(code);

            var cfg = GlobalModisMain.Config;
            string plotExtent = cfg.PlotStdExtent.Replace(',', ' ');
            string inputFile = $"{GlobalModisMain.Config.WorkspaceDir}\\Ano\\JP_{type}_{prodName}\\{date.Year}\\jp_{prodName.ToLower()}_{date.Month}.tif";
            string outputFile = $"{cfg.WorkspaceDir}\\Ano_Map\\JP\\{type}_{prodName}\\{date.Year}\\{prodName.ToLower()}_{date.Year}_{date.Month}.png";
            string ymlStr = $"PlotTitle: JP_{prodName.ToUpper()}_{date.Year}_{date.Month}\n" +
                            $"PlotTitleSize: {cfg.PlotStdTitleSize}\n" +
                            $"PlotExtent: {plotExtent}\n" +
                            $"BarName: {cfg.PlotStdBarName}\n" +
                            $"BarTitle: bar title\n" +
                            $"BarTitleSize: {cfg.PlotStdBarTitleSize}\n" +
                            $"ShpBoundary: {cfg.PlotShpBoundary}\n" +
                            $"ShpFault: {cfg.PlotShpFault}\n" +
                            $"ShpCity: {cfg.PlotShpCity}\n" +
                            $"QuakeRecord: {cfg.PlotQuakeRecord}\n" +
                            $"OutputDpi: {cfg.plotOutputDpi}\n" +
                            $"OutputSize: {cfg.PlotOutputSize}\n" +
                            $"InputFile: {inputFile}\n" +
                            $"OutputFile: {outputFile}";
            string ymlPath = $"{GlobalModisMain.Config.ScriptDir}\\plot_jp_{product}_{date.Year}_{date.Month}.yml";
            File.WriteAllText(ymlPath, ymlStr);
            return ymlPath;
        }

        /// <summary>
        /// 获取距平tif输入文件列表
        /// </summary>
        /// <param name="product"></param>
        /// <param name="date"></param>
        /// <returns></returns>
        private static string GetJuPingTifFilePathStr(string product, DateTime date)
        {
            // 距平按照月份处理
            string type = product.Substring(0, 3);
            string code = product.Substring(3, 2);
            string prodName = GetProductName(code);
            // D:\modis\Standard\MOD_BT\2019\bt_yyyy_doy.tif
            string dataDir = $"{GlobalModisMain.Config.StdDir}\\{type}_{prodName}\\{date.Year}";
            List<string> paths = new List<string>();
            foreach (string path in Directory.EnumerateFiles(dataDir, "*.tif"))
            {
                string fileNameNoExt = Path.GetFileNameWithoutExtension(path);
                var strList = fileNameNoExt.Split('_');
                int fileYear = Convert.ToInt32(strList[1]);
                int fileDoy = Convert.ToInt32(strList[2]);
                DateTime fileDate = new DateTime(fileYear, 1, 1).AddDays(fileDoy);
                if (fileDate.Month == date.Month)
                {
                    paths.Add(path);
                }
            }

            StringBuilder sb = new StringBuilder();
            paths.ForEach(s => sb.AppendLine(s));
            return sb.ToString();
        }

        /// <summary>
        /// 获取距平背景场tif文件列表字符串
        /// </summary>
        /// <param name="product"></param>
        /// <param name="date"></param>
        /// <returns></returns>
        private static string GetJuPingRefTifFilePathStr(string product, DateTime date)
        {
            // 距平按照月份处理
            string type = product.Substring(0, 3);
            string code = product.Substring(3, 2);
            string prodName = GetProductName(code);
            // D:\modis\Standard\MOD_BT\2019\bt_yyyy_doy.tif
            string dataDir = $"{GlobalModisMain.Config.StdDir}\\{type}_{prodName}\\";
            List<string> paths = new List<string>();
            foreach (string path in Directory.EnumerateFiles($"{GlobalModisMain.Config.StdDir}\\{type}_{prodName}", "*.tif", SearchOption.AllDirectories))
            {
                string fileNameNoExt = Path.GetFileNameWithoutExtension(path);
                var strList = fileNameNoExt.Split('_');
                int fileYear = Convert.ToInt32(strList[1]);
                int fileDoy = Convert.ToInt32(strList[2]);
                DateTime fileDate = new DateTime(fileYear, 1, 1).AddDays(fileDoy);
                if (fileDate.Month == date.Month && fileDate.Year < date.Year)
                {
                    paths.Add(path);
                }
            }

            StringBuilder sb = new StringBuilder();
            paths.ForEach(s => sb.AppendLine(s));
            return sb.ToString();
        }
    }
}

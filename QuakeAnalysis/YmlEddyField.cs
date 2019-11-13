using QuakeAnalysis.Cfg;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace QuakeAnalysis
{
    public partial class YmlGenerator
    {
        public static string GetEddyScript(string product, DateTime start, DateTime end)
        {
            StringBuilder sb = new StringBuilder();
            for (DateTime d = start; d <= end; d = d.AddDays(1))
            {
                sb.AppendLine(GetEddyScript(product, d));
            }

            return sb.ToString();
        }


        public static string GetEddyScript(string product, DateTime date)
        {
            StringBuilder builder = new StringBuilder();

            if (GlobalModisMain.Config.UseEddyMode1)
            {
                string ymlPath = $"{GlobalModisMain.Config.ScriptDir}\\eddy_mode1_{product.ToLower()}_{date.Year}_{date.DayOfYear}.yml";
                File.WriteAllText(ymlPath, GetEddyYmlStr(product, date, 1));
                builder.AppendLine($"{GlobalModisMain.Config.ModisEddyField} -y {ymlPath}");

                string plotYmlPath = $"{GlobalModisMain.Config.ScriptDir}\\plot_eddy_mode1_{product.ToLower()}_{date.Year}_{date.DayOfYear}.yml";
                File.WriteAllText(plotYmlPath, GetPlotEddyYmlStr(product, date, 1));
                builder.AppendLine($"{GlobalModisMain.Config.ModisPlot} -y {plotYmlPath}");

            }
            if (GlobalModisMain.Config.UseEddyMode2)
            {
                string ymlPath = $"{GlobalModisMain.Config.ScriptDir}\\eddy_mode2_{product.ToLower()}_{date.Year}_{date.DayOfYear}.yml";
                File.WriteAllText(ymlPath, GetEddyYmlStr(product, date, 2));
                builder.AppendLine($"{GlobalModisMain.Config.ModisEddyField} -y {ymlPath}");

                string plotYmlPath = $"{GlobalModisMain.Config.ScriptDir}\\plot_eddy_mode2_{product.ToLower()}_{date.Year}_{date.DayOfYear}.yml";
                File.WriteAllText(plotYmlPath, GetPlotEddyYmlStr(product, date, 2));
                builder.AppendLine($"{GlobalModisMain.Config.ModisPlot} -y {plotYmlPath}");
            }

            return builder.ToString();
        }

        private static string GetEddyRefImagePath(string product, DateTime date, int mode)
        {
            string type = product.Substring(0, 3);
            string code = product.Substring(3, 2);
            string prodName = GetProductName(code);
            return
                $"{GlobalModisMain.Config.WorkspaceDir}\\Ref\\EF{mode}_{type}_{prodName}\\ef{mode}_{prodName.ToLower()}_{date.Year}_{date.DayOfYear}_ref.tif";
        }

        private static string GetEddyYmlStr(string product, DateTime date, int mode)
        {
            string type = product.Substring(0, 3);
            string code = product.Substring(3, 2);
            string stdImagePath = GetStdImagePath(product, date);
            string refImagePath = GetEddyRefImagePath(product, date, mode);
            string tifListFiles = GetEddyRefTifListFileStr(product, date);
            string refTifListFilePath =
                $"{GlobalModisMain.Config.ScriptDir}\\eddy_mode{mode}_{product.ToLower()}_{date.Year}_{date.DayOfYear}.txt";
            File.WriteAllText(refTifListFilePath, tifListFiles);
            string outputAnoFilePath =
                $"{GlobalModisMain.Config.WorkspaceDir}\\Ano\\EF{mode}_{type}_{GetProductName(code)}\\ef{mode}_{GetProductName(code).ToLower()}_{date.Year}_{date.DayOfYear}.tif";

            string ymlStr = $"Workspace: {GlobalModisMain.Config.WorkspaceDir}\n" +
                            $"CalcRef: true\n" +
                            $"CalcAno: true\n" +
                            $"AnoMethod: {mode}\n" +
                            $"InputImageFile: {GetStdImagePath(product, date)}\n" +
                            $"RefListFile: {refTifListFilePath}\n" +
                            $"RefImageFile: {refImagePath}\n" +
                            $"OutputAnoFile: {outputAnoFilePath}";
            return ymlStr;
        }

        private static string GetPlotEddyYmlStr(string product, DateTime date, int mode)
        {
            var cfg = GlobalModisMain.Config;
            string type = product.Substring(0, 3);
            string code = product.Substring(3, 2);
            string prodName = GetProductName(code);

            string plotExtent = cfg.PlotStdExtent.Replace(',', ' ');
            string inputFile = GetStdImagePath(product, date);
            // ../Ano_Map/EF1/MYD_AOD/aod_2019_274.png
            string outputFile =
                $"{GlobalModisMain.Config.WorkspaceDir}\\Ano_Map\\EF{mode}\\{type}_{product}\\{prodName}_{date.Year}_{date.DayOfYear}.png";
            string ymlStr = $"PlotTitle: EddyField\n" +
                            $"PlotTitleSize: {cfg.PlotStdTitleSize}\n" +
                            $"PlotExtent: {plotExtent}\n" +
                            $"BarName: {cfg.PlotStdBarName}\n" +
                            $"BarTitle: {cfg.PlotBarTitle02}\n" +
                            $"BarTitleSize: {cfg.PlotStdBarTitleSize}\n" +
                            $"ShpBoundary: {cfg.PlotShpBoundary}\n" +
                            $"ShpFault: {cfg.PlotShpFault}\n" +
                            $"ShpCity: {cfg.PlotShpCity}\n" +
                            $"QuakeRecord: {cfg.PlotQuakeRecord}\n" +
                            $"OutputDpi: {cfg.plotOutputDpi}\n" +
                            $"OutputSize: {cfg.PlotOutputSize}\n" +
                            $"InputFile: {inputFile}\n" +
                            $"OutputFile: {outputFile}";
            return ymlStr;
        }

        private static string GetEddyRefTifListFileStr(string product, DateTime date)
        {
            string type = product.Substring(0, 3);
            string code = product.Substring(3, 2);
            string dataFolder = GetStdFolder(type, code);
            StringBuilder builder = new StringBuilder();
            foreach (string file in Directory.EnumerateFiles(dataFolder, "*.tif"))
            {
                FileInfo fileInfo = new FileInfo(file);
                int fileYear = Convert.ToInt32(GetYearFromFileName(fileInfo.Name));
                int fileDayOfYear = Convert.ToInt32(GetDayOfYearFromFileName(Path.GetFileNameWithoutExtension(fileInfo.FullName)));
                if (fileDayOfYear == date.DayOfYear && fileYear <= date.Year)
                {
                    builder.AppendLine(fileInfo.FullName);
                }
            }

            return builder.ToString();
        }

        private static string GetYearFromFileName(string fileName)
        {
            return fileName.Split('_')[1];
        }

        private static string GetDayOfYearFromFileName(string fileName)
        {
            return fileName.Split('_')[2];
        }

        private static string GetStdFolder(string type, string code)
        {
            return $"{GlobalModisMain.Config.StdDir}\\{type}_{GetProductName(code)}";
        }

        /// <summary>
        /// 根据产品代码获取产品名称
        /// </summary>
        /// <param name="code"></param>
        /// <returns></returns>
        private static string GetProductName(string code)
        {
            Dictionary<string, string> mapping = new Dictionary<string, string>();
            mapping.Add("02", "BT");
            mapping.Add("04", "AOD");
            mapping.Add("05", "TPW");
            mapping.Add("11", "LST");
            return mapping[code];
        }

        private static string GetStdImagePath(string product, DateTime date)
        {
            string type = product.Substring(0, 3);
            string code = product.Substring(3, 2);
            string doyStr = $"{date.Year}_{date.DayOfYear}";
            switch (code)
            {
                case "02":
                    return GlobalModisMain.Config.Std02TifPath(type, date);
                case "04":
                    return GlobalModisMain.Config.Std04TifPath(type, date);
                case "05":
                    return GlobalModisMain.Config.Std05TifPath(type, date);
                case "11":
                    return GlobalModisMain.Config.Std11TifPath(type, date);
                default:
                    throw new ArgumentException("Wrong product code!");
            }
        }
    }
}

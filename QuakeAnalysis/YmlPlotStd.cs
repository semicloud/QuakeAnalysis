using QuakeAnalysis.Cfg;
using System;
using System.IO;
using System.Text;

namespace QuakeAnalysis
{
    // 标准数据出图部分
    public partial class YmlGenerator
    {
        /// <summary>
        /// 获取标准数据出图的运行脚本
        /// </summary>
        /// <param name="product"></param>
        /// <param name="start"></param>
        /// <param name="end"></param>
        /// <param name="ymlGenerator"></param>
        /// <returns></returns>
        public static string GetStdPlotScript(string product, DateTime start, DateTime end,
            Func<string, DateTime, string> ymlGenerator)
        {
            StringBuilder sb = new StringBuilder();
            sb.AppendLine($"cd {new FileInfo(GlobalModisMain.Config.ModisPlot).DirectoryName}");
            for (DateTime date = start; date <= end; date = date.AddDays(1))
            {
                string ymlPath = ymlGenerator(product.Substring(0, 3), date);
                sb.AppendLine($@"{GlobalModisMain.Config.ModisPlot} -y {ymlPath}");
            }
            return sb.ToString();
        }

        public static string Generate02PlotStdYml(string type, DateTime date)
        {
            var cfg = GlobalModisMain.Config;
            string plotExtent = cfg.PlotStdExtent.Replace(',', ' ');
            string inputFile = cfg.Std02TifPath(type, date);
            string outputFile = $"{cfg.StdMapDir}\\{type}_BT\\bt_{date.Year}_{date.DayOfYear}.png";
            string ymlStr = $"PlotTitle: {cfg.GetStdPlotTitle($"{type}02", date)}\n" +
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
            string ymlPath = $"{cfg.ScriptDir}\\plot_std_{type}02_{date.Year}{date.DayOfYear}.yml";
            File.WriteAllText(ymlPath, ymlStr);
            return ymlPath;
        }

        public static string Generate04PlotStdYml(string type, DateTime date)
        {
            var cfg = GlobalModisMain.Config;
            string plotExtent = cfg.PlotStdExtent.Replace(',', ' ');
            string inputFile = cfg.Std04TifPath(type, date);  //! Here
            string outputFile = $"{cfg.StdMapDir}\\{type}_AOD\\aod_{date.Year}_{date.DayOfYear}.png";  //! Here
            string ymlStr = $"PlotTitle: {cfg.GetStdPlotTitle($"{type}04", date)}\n" +  //! Here
                            $"PlotTitleSize: {cfg.PlotStdTitleSize}\n" +
                            $"PlotExtent: {plotExtent}\n" +
                            $"BarName: {cfg.PlotStdBarName}\n" +
                            $"BarTitle: {cfg.PlotBarTitle04}\n" +   //! Here
                            $"BarTitleSize: {cfg.PlotStdBarTitleSize}\n" +
                            $"ShpBoundary: {cfg.PlotShpBoundary}\n" +
                            $"ShpFault: {cfg.PlotShpFault}\n" +
                            $"ShpCity: {cfg.PlotShpCity}\n" +
                            $"QuakeRecord: {cfg.PlotQuakeRecord}\n" +
                            $"OutputDpi: {cfg.plotOutputDpi}\n" +
                            $"OutputSize: {cfg.PlotOutputSize}\n" +
                            $"InputFile: {inputFile}\n" +
                            $"OutputFile: {outputFile}";
            string ymlPath = $"{cfg.ScriptDir}\\plot_std_{type}04_{date.Year}{date.DayOfYear}.yml";  //! Here
            File.WriteAllText(ymlPath, ymlStr);
            return ymlPath;
        }

        public static string Generate05PlotStdYml(string type, DateTime date)
        {
            var cfg = GlobalModisMain.Config;
            string plotExtent = cfg.PlotStdExtent.Replace(',', ' ');
            string inputFile = cfg.Std05TifPath(type, date);  //! Here
            string outputFile = $"{cfg.StdMapDir}\\{type}_TPW\\tpw_{date.Year}_{date.DayOfYear}.png";  //! Here
            string ymlStr = $"PlotTitle: {cfg.GetStdPlotTitle($"{type}05", date)}\n" +  //! Here
                            $"PlotTitleSize: {cfg.PlotStdTitleSize}\n" +
                            $"PlotExtent: {plotExtent}\n" +
                            $"BarName: {cfg.PlotStdBarName}\n" +
                            $"BarTitle: {cfg.PlotBarTitle05}\n" +   //! Here
                            $"BarTitleSize: {cfg.PlotStdBarTitleSize}\n" +
                            $"ShpBoundary: {cfg.PlotShpBoundary}\n" +
                            $"ShpFault: {cfg.PlotShpFault}\n" +
                            $"ShpCity: {cfg.PlotShpCity}\n" +
                            $"QuakeRecord: {cfg.PlotQuakeRecord}\n" +
                            $"OutputDpi: {cfg.plotOutputDpi}\n" +
                            $"OutputSize: {cfg.PlotOutputSize}\n" +
                            $"InputFile: {inputFile}\n" +
                            $"OutputFile: {outputFile}";
            string ymlPath = $"{cfg.ScriptDir}\\plot_std_{type}05_{date.Year}{date.DayOfYear}.yml";  //! Here
            File.WriteAllText(ymlPath, ymlStr);
            return ymlPath;
        }
    }
}

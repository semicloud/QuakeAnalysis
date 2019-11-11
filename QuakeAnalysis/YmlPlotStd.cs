using QuakeAnalysis.Cfg;
using System;
using System.IO;

namespace QuakeAnalysis
{
    public partial class YmlGenerator
    {
        // 标准数据出图部分

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

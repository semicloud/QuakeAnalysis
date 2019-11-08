using System;
using System.Configuration;
using NLog;

namespace QuakeAnalysis.Cfg
{
public static     class Config
{
    private static Logger g = LogManager.GetCurrentClassLogger();

        public static string GetMrtSwathPath()
        {
            var path = ConfigurationManager.AppSettings["mrt_swath_path"];
            return path;
        }

        public static string GetModisTmpDir()
        {
            var tmpDir = ConfigurationManager.AppSettings["modis_tmp_dir"];
            tmpDir = Environment.CurrentDirectory + "\\" + tmpDir;
            g.Debug("get modis tmp dir: " + tmpDir);
            return tmpDir;
        }
    }
}

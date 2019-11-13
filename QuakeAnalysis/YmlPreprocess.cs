using QuakeAnalysis.Cfg;
using QuakeAnalysis.Properties;
using System;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace QuakeAnalysis
{
    public partial class YmlGenerator
    {
        // 预处理部分Yml文件的自动生成

        #region 亮温

        /// <summary>
        /// 获取亮温的bat运行脚本
        /// </summary>
        /// <param name="product"></param>
        /// <param name="start"></param>
        /// <param name="end"></param>
        /// <returns></returns>
        public static string GetPrep02Scripts(string product, DateTime start, DateTime end)
        {
            StringBuilder sb = new StringBuilder();
            sb.AppendLine($"cd {new FileInfo(GlobalModisMain.Config.ModisProc02).DirectoryName}");
            for (DateTime date = start; date <= end; date = date.AddDays(1))
            {
                string ymlPath = Generate02PreprocessYml(product.Substring(0, 3), date);
                sb.AppendLine($@"{GlobalModisMain.Config.ModisProc02} -y {ymlPath}");
            }

            return sb.ToString();
        }

        public static string Generate02PreprocessYml(string type, DateTime date)
        {
            var cfg = GlobalModisMain.Config;
            var hdfListFile = Generate02HdfListFile(type, date);
            StringBuilder sb = new StringBuilder($"HDFListFile: {hdfListFile}\n");
            sb.AppendLine($@"TmpPath: {cfg.WorkspaceDir}\tmp");
            sb.AppendLine($"MinLon: {cfg.PrepMinLon}");
            sb.AppendLine($"MaxLon: {cfg.PrepMaxLon}");
            sb.AppendLine($"MinLat: {cfg.PrepMinLat}");
            sb.AppendLine($"MaxLat: {cfg.PrepMaxLat}");
            sb.AppendLine($"Band: {cfg.Prep02Band}");
            sb.AppendLine($"MRTKernelType: {cfg.Prep02MrtKernelType}");
            sb.AppendLine($"MRTProjectionType: {cfg.Prep02MrtProjType}");
            sb.AppendLine($"MRTProjectionArgs: {cfg.Prep02MrtProjArgs}");
            sb.AppendLine($"MRTPixelSize: {cfg.Prep02MrtPixelSize}");
            sb.AppendLine($@"OutputImageFile: {cfg.Std02TifPath(type, date)}");
            string ymlPath = $@"{cfg.Script02YmlPath(type, date)}";
            if (File.Exists(ymlPath)) File.Delete(ymlPath);
            File.WriteAllText(ymlPath, sb.ToString());
            return ymlPath;
        }

        public static string Generate02HdfListFile(string type, DateTime date)
        {
            var cfg = GlobalModisMain.Config;
            string year = date.Year.ToString();
            string day = date.DayOfYear.ToString();
            //string btHdfDir = $@"{cfg.WorkspaceDir}\{type}021KM\{year}\{day}\";
            //string szaHdfDir = $@"{cfg.WorkspaceDir}\{type}03\{year}\{day}\";
            //string cmHdfDir = $@"{cfg.WorkspaceDir}\{type}35_L2\{year}\{day}\";
            DirectoryInfo btHdfDirInfo = new DirectoryInfo(GlobalModisMain.Config.Hdf02Dir(type, date));
            StringBuilder sb = new StringBuilder();
            foreach (FileInfo file in btHdfDirInfo.EnumerateFiles("*.hdf"))
            {
                string searchStr = file.Name.Substring(9, 13);
                var szaFiles = Directory.EnumerateFiles(GlobalModisMain.Config.Hdf03Dir(type, date), "*.hdf").ToList();
                string szaFileName = szaFiles.Find(s => s.Contains(searchStr));
                var cmFiles = Directory.EnumerateFiles(GlobalModisMain.Config.Hdf35Dir(type, date), "*.hdf").ToList();
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
                    MessageBox.Show($"不存在{date}的云掩膜或太阳天顶角文件！", Settings.Default.DT,
                        MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return "";
                }
            }

            string hdfListPath = GlobalModisMain.Config.Script02HdfListPath(type, date);
            if (File.Exists(hdfListPath)) File.Delete(hdfListPath);
            File.WriteAllText(hdfListPath, sb.ToString());
            return hdfListPath;
        }

        #endregion 亮温

        #region 气溶胶

        /// <summary>
        /// 获取气溶胶bat运行脚本
        /// </summary>
        /// <param name="product"></param>
        /// <param name="start"></param>
        /// <param name="end"></param>
        /// <returns></returns>
        public static string GetPrep04Scripts(string product, DateTime start, DateTime end)
        {
            StringBuilder sb = new StringBuilder();
            sb.AppendLine($"cd {new FileInfo(GlobalModisMain.Config.ModisProc04).DirectoryName}");
            for (DateTime date = start; date <= end; date = date.AddDays(1))
            {
                string ymlPath = Generate04PreprocessYml(product.Substring(0, 3), date);
                sb.AppendLine($@"{GlobalModisMain.Config.ModisProc04} -y {ymlPath}");
            }

            return sb.ToString();
        }

        public static string Generate04PreprocessYml(string type, DateTime date)
        {
            string hdfListFile = Generate04HdfListFile(type, date);
            string ymlStr = $"HDFListFile: {hdfListFile}\n" +
                            $"MinLon: {GlobalModisMain.Config.PrepMinLon}\n" +
                            $"MaxLon: {GlobalModisMain.Config.PrepMaxLon}\n" +
                            $"MinLat: {GlobalModisMain.Config.PrepMinLat}\n" +
                            $"MaxLat: {GlobalModisMain.Config.PrepMaxLat}\n" +
                            $"ResamplingType: {GlobalModisMain.Config.Prep04ResampType}\n" +
                            $"OutputProjectionType: {GlobalModisMain.Config.Prep04ProjType}\n" +
                            $"OutputProjectionParameters: {GlobalModisMain.Config.Prep04ProjParameters}\n" +
                            $"OutputImageFile: {GlobalModisMain.Config.Std04TifPath(type, date)}\n" +
                            $"TmpPath: {GlobalModisMain.Config.TmpDir}";
            string ymlPath = $"{GlobalModisMain.Config.Script04YmlPath(type, date)}";
            if (File.Exists(ymlPath)) File.Delete(ymlPath);
            File.WriteAllText(ymlPath, ymlStr);
            return ymlPath;
        }

        public static string Generate04HdfListFile(string type, DateTime date)
        {
            var cfg = GlobalModisMain.Config;
            string year = date.Year.ToString();
            string day = date.DayOfYear.ToString();
            DirectoryInfo directoryInfo = new DirectoryInfo(GlobalModisMain.Config.Hdf04Dir(type, date));
            StringBuilder sb = new StringBuilder();
            //TODO 在生成脚本前应该先检查合法性
            foreach (FileInfo file in directoryInfo.EnumerateFiles("*.hdf"))
            {
                sb.AppendLine(file.FullName);
            }

            string hdfListPath = GlobalModisMain.Config.Script04HdfListPath(type, date);
            if (File.Exists(hdfListPath)) File.Delete(hdfListPath);
            File.WriteAllText(hdfListPath, sb.ToString());
            return hdfListPath;
        }

        #endregion 气溶胶

        #region 水汽

        /// <summary>
        /// 获取水汽bat运行脚本
        /// </summary>
        /// <param name="product"></param>
        /// <param name="start"></param>
        /// <param name="end"></param>
        /// <returns></returns>
        public static string GetPrep05Scripts(string product, DateTime start, DateTime end)
        {
            StringBuilder sb = new StringBuilder();
            sb.AppendLine($"cd {new FileInfo(GlobalModisMain.Config.ModisProc05).DirectoryName}");
            for (DateTime date = start; date <= end; date = date.AddDays(1))
            {
                string ymlPath = Generate05PreprocessYml(product.Substring(0, 3), date);
                sb.AppendLine($@"{GlobalModisMain.Config.ModisProc05} -y {ymlPath}");
            }

            return sb.ToString();
        }


        public static string Generate05PreprocessYml(string type, DateTime date)
        {
            string hdfListFile = Generate05HdfListFile(type, date);
            string ymlStr = $"HDFListFile: {hdfListFile}\n" +
                            $"MinLon: {GlobalModisMain.Config.PrepMinLon}\n" +
                            $"MaxLon: {GlobalModisMain.Config.PrepMaxLon}\n" +
                            $"MinLat: {GlobalModisMain.Config.PrepMinLat}\n" +
                            $"MaxLat: {GlobalModisMain.Config.PrepMaxLat}\n" +
                            $"ResamplingType: {GlobalModisMain.Config.Prep05ResampType}\n" +
                            $"OutputProjectionType: {GlobalModisMain.Config.Prep05ProjType}\n" +
                            $"OutputProjectionParameters: {GlobalModisMain.Config.Prep05ProjParameters}\n" +
                            $"OutputImageFile: {GlobalModisMain.Config.Std05TifPath(type, date)}\n" +
                            $"TmpPath: {GlobalModisMain.Config.TmpDir}";
            string ymlPath = $"{GlobalModisMain.Config.Script05YmlPath(type, date)}";
            if (File.Exists(ymlPath)) File.Delete(ymlPath);
            File.WriteAllText(ymlPath, ymlStr);
            return ymlPath;
        }

        public static string Generate05HdfListFile(string type, DateTime date)
        {
            var cfg = GlobalModisMain.Config;
            string year = date.Year.ToString();
            string day = date.DayOfYear.ToString();
            DirectoryInfo directoryInfo = new DirectoryInfo(GlobalModisMain.Config.Hdf05Dir(type, date));
            StringBuilder sb = new StringBuilder();
            //TODO 在生成脚本前应该先检查合法性
            foreach (FileInfo file in directoryInfo.EnumerateFiles("*.hdf"))
            {
                sb.AppendLine(file.FullName);
            }
            string hdfListPath = GlobalModisMain.Config.Script05HdfListPath(type, date);
            if (File.Exists(hdfListPath)) File.Delete(hdfListPath);
            File.WriteAllText(hdfListPath, sb.ToString());
            return hdfListPath;
        }

        #endregion 水汽
    }
}
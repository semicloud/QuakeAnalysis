using System.IO;
using Antlr4.StringTemplate;
using NLog;

namespace QuakeAnalysis.Entity
{
    /// <summary>
    /// 用于HDF2GTIFF的CMD脚本
    /// </summary>
    public class PrmBatFile
    {
        private string _mrtSwathPath;
        private string _prmFilePath;

        private static Logger g = LogManager.GetCurrentClassLogger();

        /// <summary>
        /// 本地安装的MRT路径，例如C:\DevLib\MRTSwath
        /// </summary>
        public string MrtSwathPath => _mrtSwathPath;

        /// <summary>
        /// prm文件路径
        /// </summary>
        public string PrmFilePath => _prmFilePath;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="mrtSwathPath">本地安装的MRT路径，例如C:\DevLib\MRTSwath</param>
        /// <param name="prmFilePath">prm文件路径</param>
        public PrmBatFile(string mrtSwathPath, string prmFilePath)
        {
            _mrtSwathPath = mrtSwathPath;
            _prmFilePath = prmFilePath;
        }

        /// <summary>
        /// 获取运行Prm文件的bat脚本内容
        /// </summary>
        /// <returns></returns>
        public string GetPrmBatContent()
        {
            var template = new Template(File.ReadAllText(@"Template/Prm/run_prm.st"));
            template.Add("mrt_swath_path", QuakeAnalysis.Config.GetMrtSwathPath());
            template.Add("prm_file_path", _prmFilePath);
            var content = template.Render();
            g.Debug("Prm Bat Content:");
            g.Debug(content);
            return content;
        }

        /// <summary>
        /// 将运行Prm文件的bat脚本保存到指定位置
        /// </summary>
        public void ExportPrmBatFile(string dest)
        {
            var content = GetPrmBatContent();
            //TODO 已存在相关文件的情况的处理
            File.WriteAllText(dest, content);
            g.Debug("save prm bat to {0}", dest);
        }
    }
}

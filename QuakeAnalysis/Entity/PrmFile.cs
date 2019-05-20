using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Antlr4.StringTemplate;
using NLog;

namespace QuakeAnalysis.Entity
{
    /// <summary>
    /// 该类主要用于处理swath2grid.exe使用的prm配置文件
    /// 包括生成Prm文件字符串、导出Prm文件至指定目录等
    /// </summary>
   public  class PrmFile
    {
        private PrmSetting _prmSetting;
        private static Logger g = LogManager.GetCurrentClassLogger();

        /// <summary>
        /// 使用PrmSetting对象创建一个PrmFile对象
        /// </summary>
        /// <param name="prmSetting"></param>
        public PrmFile(PrmSetting prmSetting)
        {
            _prmSetting = prmSetting;
        }

        /// <summary>
        /// 获取Prm文件内容
        /// </summary>
        /// <returns></returns>
        public string GetPrmFileContent()
        {
            var template = new Template(File.ReadAllText(@"Template/Prm/prm_demo.st"));
            template.Add("input_hdf_file", _prmSetting.InputHdfFile);
            template.Add("geo_loc_hdf_file", _prmSetting.GeoLocHdfFile);
            template.Add("sds_str", _prmSetting.SdsStr);
            template.Add("ulc_lng", _prmSetting.UlcLng);
            template.Add("ulc_lat", _prmSetting.UlcLat);
            template.Add("lrc_lng", _prmSetting.LrcLng);
            template.Add("lrc_lat", _prmSetting.LrcLat);
            template.Add("output_gtiff_file", _prmSetting.OutputGTiffFile);
            var str = template.Render();
            g.Debug("Prm Str:");
            g.Debug(str);
            return str;
        }

        /// <summary>
        /// 将Prm文件保存至指定目录
        /// </summary>
        /// <param name="dest"></param>
        public void ExportPrmFile(string dest)
        {
            var content = GetPrmFileContent();
            //TODO 已存在相关文件的情况的处理
            File.WriteAllText(dest, content);
            g.Debug("save prm content to {0}", dest);
        }
    }
}

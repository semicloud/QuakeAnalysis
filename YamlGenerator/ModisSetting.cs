using System.Collections.Generic;

namespace YamlGenerator
{
    /// <summary>
    /// 主设置类
    /// </summary>
    public class ModisSetting
    {
        /// <summary>
        /// 工作空间目录
        /// </summary>
        public string WorkSpaceFolder { get; set; }
        public List<string> SelectedProducts { get; set; }
        public string StartDate { get; set; }
        public string EndDate { get; set; }
        public ArchiveSetting ArchiveSetting { get; set; }
        public PreprocessSetting PreprocessSetting { get; set; }
    }
}

using YamlDotNet.Serialization;

namespace QuakeAnalysis.Cfg
{
    /// <summary>
    /// 涡度的相关配置
    /// </summary>
    public partial class ModisMainConfig
    {
        /// <summary>
        /// # 是否使用模式1
        /// </summary>
        [YamlMember(Alias = "eddy-mode1", ApplyNamingConventions = false)]
        public bool UseEddyMode1 { get; set; }

        /// <summary>
        /// 是否使用模式2
        /// </summary>
        [YamlMember(Alias = "eddy-mode2", ApplyNamingConventions = false)]
        public bool UseEddyMode2 { get; set; }

        /// <summary>
        /// 是否生成背景场
        /// </summary>
        [YamlMember(Alias = "eddy-bg", ApplyNamingConventions = false)]
        public bool CalcEddyBg { get; set; }

        /// <summary>
        /// 背景场是否出图
        /// </summary>
        [YamlMember(Alias = "eddy-plot-bg", ApplyNamingConventions = false)]
        public bool IsPlotEddyBg { get; set; }

        /// <summary>
        /// 异常是否出图
        /// </summary>
        [YamlMember(Alias = "eddy-plot-ano", ApplyNamingConventions = false)]
        public bool IsPlotEddyAno { get; set; }

        /// <summary>
        /// 是否绘制等值线
        /// </summary>
        [YamlMember(Alias = "eddy-contour", ApplyNamingConventions = false)]
        public bool IsPlotEddyContour { get; set; }
    }
}

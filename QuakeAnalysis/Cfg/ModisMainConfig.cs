using System;
using System.IO;
using YamlDotNet.Serialization;
using YamlDotNet.Serialization.NamingConventions;

namespace QuakeAnalysis.Cfg
{
    public class ModisMainConfig
    {
        private const string CONFIG_FILE = "Yml\\modis_main.yml";

        [YamlMember(Alias = "font-size", ApplyNamingConventions = false)]
        public float FontSize { get; set; }

        [YamlMember(Alias = "test-attr", ApplyNamingConventions = false)]
        public float TestAttr { get; set; }

        [YamlMember(Alias = "workspace-dir", ApplyNamingConventions = false)]
        public string WorkspaceDir { get; set; }

        [YamlMember(Alias = "script-dir", ApplyNamingConventions = false)]
        public string ScriptDir { get; set; }

        [YamlMember(Alias = "tmp-dir", ApplyNamingConventions = false)]
        public string TmpDir { get; set; }

        [YamlMember(Alias = "stdmap-dir")]
        public string StdMapDir { get; set; }

        [YamlMember(Alias = "data-dir", ApplyNamingConventions = false)]
        public string DataDir { get; set; }

        #region Modis程序路径

        [YamlMember(Alias = "modis-archive", ApplyNamingConventions = false)]
        public string ModisArchive { get; set; }

        [YamlMember(Alias = "modis-proc_mxd021km", ApplyNamingConventions = false)]
        public string ModisProc02 { get; set; }

        [YamlMember(Alias = "modis-proc_mxd04_3k", ApplyNamingConventions = false)]
        public string ModisProc04 { get; set; }

        [YamlMember(Alias = "modis-proc_mxd05_l2", ApplyNamingConventions = false)]
        public string ModisProc05 { get; set; }

        [YamlMember(Alias = "modis-proc_mxd11a1", ApplyNamingConventions = false)]
        public string ModisProc11 { get; set; }

        [YamlMember(Alias = "modis-eddyfield", ApplyNamingConventions = false)]
        public string ModisEddyField { get; set; }

        [YamlMember(Alias = "modis-generalano", ApplyNamingConventions = false)]
        public string ModisGeneralAno { get; set; }

        [YamlMember(Alias = "modis-rsten", ApplyNamingConventions = false)]
        public string ModisRstEn { get; set; }

        [YamlMember(Alias = "modis-plot", ApplyNamingConventions = false)]
        public string ModisPlot { get; set; }

        #endregion

        #region 输出路径和文件名规则

        public string Hdf02Dir(string type, DateTime date)
        {
            return $"{WorkspaceDir}\\{type}021KM\\{date.Year}\\{date.DayOfYear}";
        }

        public string Hdf03Dir(string type, DateTime date)
        {
            return $"{WorkspaceDir}\\{type}03\\{date.Year}\\{date.DayOfYear}";
        }

        public string Hdf35Dir(string type, DateTime date)
        {
            return $"{WorkspaceDir}\\{type}35_L2\\{date.Year}\\{date.DayOfYear}";
        }

        public string Hdf04Dir(string type, DateTime date)
        {
            return $"{WorkspaceDir}\\{type}04_3K\\{date.Year}\\{date.DayOfYear}";
        }

        public string Hdf05Dir(string type, DateTime date)
        {
            return $"{WorkspaceDir}\\{type}05_L2\\{date.Year}\\{date.DayOfYear}";
        }

        /// <summary>
        /// 亮温的预处理文件的存放目录
        /// </summary>
        /// <param name="type">类型</param>
        /// <returns></returns>
        public string Std02Dir(string type)
        {
            return $"{WorkspaceDir}\\Standard\\{type}_BT";
        }

        public string Std04Dir(string type)
        {
            return $"{WorkspaceDir}\\Standard\\{type}_AOD";
        }

        public string Std05Dir(string type)
        {
            return $"{WorkspaceDir}\\Standard\\{type}_TPW";
        }

        public string Std11Dir(string type)
        {
            return $"{WorkspaceDir}\\Standard\\{type}_LST";
        }

        public string Std02TifPath(string type, DateTime d)
        {
            return $"{Std02Dir(type)}\\bt_{d.Year}_{d.DayOfYear}.tif";
        }

        public string Std04TifPath(string type, DateTime d)
        {
            return $"{Std04Dir(type)}\\aod_{d.Year}_{d.DayOfYear}.tif";
        }

        public string Std05TifPath(string type, DateTime d)
        {
            return $"{Std05Dir(type)}\\tpw_{d.Year}_{d.DayOfYear}.tif";
        }

        public string Std11TifPath(string type, DateTime d)
        {
            return $"{Std11Dir(type)}\\lst_{d.Year}_{d.DayOfYear}.tif";
        }

        public string Script02HdfListPath(string type, DateTime d)
        {
            return $"{WorkspaceDir}\\Scripts\\{type}021KM_{d.Year}_{d.DayOfYear}_hdflist.txt";
        }

        public string Script02YmlPath(string type, DateTime d)
        {
            return $"{WorkspaceDir}\\Scripts\\{type}021KM_{d.Year}_{d.DayOfYear}.yml";
        }

        public string Script04HdfListPath(string type, DateTime d)
        {
            return $"{WorkspaceDir}\\Scripts\\{type}04_3K_{d.Year}_{d.DayOfYear}_hdflist.txt";
        }

        public string Script04YmlPath(string type, DateTime d)
        {
            return $"{WorkspaceDir}\\Scripts\\{type}04_3K_{d.Year}_{d.DayOfYear}.yml";
        }

        public string Script05HdfListPath(string type, DateTime d)
        {
            return $"{WorkspaceDir}\\Scripts\\{type}05_L2_{d.Year}_{d.DayOfYear}_hdflist.txt";
        }

        public string Script05YmlPath(string type, DateTime d)
        {
            return $"{WorkspaceDir}\\Scripts\\{type}05_L2_{d.Year}_{d.DayOfYear}.yml";
        }

        public string Script11HdfListPath(string type, DateTime d)
        {
            return $"{WorkspaceDir}\\Scripts\\{type}11_{d.Year}_{d.DayOfYear}_hdflist.txt";
        }

        public string Script11YmlPath(string type, DateTime d)
        {
            return $"{WorkspaceDir}\\Scripts\\{type}11_{d.Year}_{d.DayOfYear}.yml";
        }

        public string ScriptBatPath(string fileName)
        {
            return $"{WorkspaceDir}\\Scripts\\{fileName}.bat";
        }

        #endregion

        #region 预处理配置

        #region 经纬度

        [YamlMember(Alias = "prep-min-lon", ApplyNamingConventions = false)]
        public double PrepMinLon { get; set; }

        [YamlMember(Alias = "prep-max-lon", ApplyNamingConventions = false)]
        public double PrepMaxLon { get; set; }

        [YamlMember(Alias = "prep-min-lat", ApplyNamingConventions = false)]
        public double PrepMinLat { get; set; }

        [YamlMember(Alias = "prep-max-lat", ApplyNamingConventions = false)]
        public double PrepMaxLat { get; set; }

        #endregion

        #region 亮温

        [YamlMember(Alias = "prep02-band", ApplyNamingConventions = false)]
        public int Prep02Band { get; set; }

        [YamlMember(Alias = "prep02-mrtkerneltype", ApplyNamingConventions = false)]
        public string Prep02MrtKernelType { get; set; }

        [YamlMember(Alias = "prep02-mrtprojtype", ApplyNamingConventions = false)]
        public string Prep02MrtProjType { get; set; }

        [YamlMember(Alias = "prep02-mrtprojargs", ApplyNamingConventions = false)]
        public string Prep02MrtProjArgs { get; set; }

        [YamlMember(Alias = "prep02-mrtpixelsize", ApplyNamingConventions = false)]
        public double Prep02MrtPixelSize { get; set; }



        #endregion

        #region 气溶胶、水汽

        [YamlMember(Alias = "prep04-resamptype", ApplyNamingConventions = false)]
        public string Prep04ResampType { get; set; }

        [YamlMember(Alias = "prep04-projtype", ApplyNamingConventions = false)]
        public string Prep04ProjType { get; set; }

        [YamlMember(Alias = "prep04-projparameters", ApplyNamingConventions = false)]
        public string Prep04ProjParameters { get; set; }

        [YamlMember(Alias = "prep05-resamptype", ApplyNamingConventions = false)]
        public string Prep05ResampType { get; set; }

        [YamlMember(Alias = "prep05-projtype", ApplyNamingConventions = false)]
        public string Prep05ProjType { get; set; }

        [YamlMember(Alias = "prep05-projparameters", ApplyNamingConventions = false)]
        public string Prep05ProjParameters { get; set; }

        #endregion

        #region 地表温度

        //[YamlMember(Alias = "prep11-resamptype", ApplyNamingConventions = false)]
        //public string Prep11SubsetType { get; set; }

        //[YamlMember(Alias = "prep11-resamptype", ApplyNamingConventions = false)]
        //public string Prep11ResampType { get; set; }

        //[YamlMember(Alias = "prep11-projtype", ApplyNamingConventions = false)]
        //public string Prep11ProjType { get; set; }

        //[YamlMember(Alias = "prep11-projparameters", ApplyNamingConventions = false)]
        //public string Prep11ProjParameters { get; set; }

        //[YamlMember(Alias = "prep11-projparameters", ApplyNamingConventions = false)]
        //public string Prep11Datum { get; set; }

        //[YamlMember(Alias = "prep11-pixelsize", ApplyNamingConventions = false)]
        //public double Prep11PixelSize { get; set; }

        #endregion


        #endregion

        public static ModisMainConfig CreateInstance()
        {
            var deserializer = new DeserializerBuilder().
                WithNamingConvention(CamelCaseNamingConvention.Instance).Build();
            var configFileText = File.ReadAllText(CONFIG_FILE);
            var cfg = deserializer.Deserialize<ModisMainConfig>(configFileText);
            return cfg;
        }

        /// <summary>
        /// 将目前的配置写回到配置文件
        /// </summary>
        public void SaveToFile()
        {
            var serializer = new Serializer();
            var serialize = serializer.Serialize(this);
            File.WriteAllText(CONFIG_FILE, serialize);
        }

        public void Reload()
        {
            GlobalModisMain.Config = CreateInstance();
        }
    }
}

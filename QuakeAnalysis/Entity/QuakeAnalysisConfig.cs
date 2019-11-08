using System.IO;
using YamlDotNet.Serialization;
using YamlDotNet.Serialization.NamingConventions;

namespace QuakeAnalysis.Entity
{
    public class QuakeAnalysisConfig
    {
        private const string CONFIG_FILE = "config.yml";

        [YamlMember(Alias = "version", ApplyNamingConventions = false)]
        public string Version { get; set; }

        [YamlMember(Alias = "data-directory", ApplyNamingConventions = false)]
        public DataDirectory DataDirectory { get; set; }

        [YamlMember(Alias = "default-parameter", ApplyNamingConventions = false)]
        public DefaultParameter DefaultParameter { get; set; }

        public static QuakeAnalysisConfig CreateInstance()
        {
            var deserializer = new DeserializerBuilder().WithNamingConvention(CamelCaseNamingConvention.Instance).Build();
            var cfg =
                deserializer.Deserialize<QuakeAnalysisConfig>(File.ReadAllText(CONFIG_FILE));
            return cfg;
        }

        public void SaveToFile()
        {
            var serializer = new Serializer();
            var serialize = serializer.Serialize(this);
            File.WriteAllText(CONFIG_FILE, serialize);
        }
    }

    public class DefaultParameter
    {
        [YamlMember(Alias = "mxd02-sds", ApplyNamingConventions = false)]
        public string Mxd02Sds { get; set; }

        [YamlMember(Alias = "mxd03-sds", ApplyNamingConventions = false)]
        public string Mxd03Sds { get; set; }

        [YamlMember(Alias = "mxd35-sds", ApplyNamingConventions = false)]
        public string Mxd35Sds { get; set; }

        [YamlMember(Alias = "ulc-lng", ApplyNamingConventions = false)]
        public double UlcLng { get; set; }

        [YamlMember(Alias = "ulc-lat", ApplyNamingConventions = false)]
        public double UlcLat { get; set; }

        [YamlMember(Alias = "lrc-lng", ApplyNamingConventions = false)]
        public double LrcLng { get; set; }

        [YamlMember(Alias = "lrc-lat", ApplyNamingConventions = false)]
        public double LrcLat { get; set; }
    }

    public class DataDirectory
    {
        [YamlMember(Alias = "hdf", ApplyNamingConventions = false)]
        public HdfDirectory Hdf { get; set; }

        [YamlMember(Alias = "standard", ApplyNamingConventions = false)]
        public StandardDirectory Standard { get; set; }

        [YamlMember(Alias = "std-map", ApplyNamingConventions = false)]
        public StandardMapDirectory StandardMap { get; set; }
    }

    public class HdfDirectory
    {
        [YamlMember(Alias = "mod02", ApplyNamingConventions = false)]
        public string Mod02 { get; set; }
        [YamlMember(Alias = "myd02", ApplyNamingConventions = false)]
        public string Myd02 { get; set; }

        [YamlMember(Alias = "mod03", ApplyNamingConventions = false)]
        public string Mod03 { get; set; }
        [YamlMember(Alias = "myd03", ApplyNamingConventions = false)]
        public string Myd03 { get; set; }
        [YamlMember(Alias = "mod04", ApplyNamingConventions = false)]
        public string Mod04 { get; set; }
        [YamlMember(Alias = "myd04", ApplyNamingConventions = false)]
        public string Myd04 { get; set; }
        [YamlMember(Alias = "mod05", ApplyNamingConventions = false)]
        public string Mod05 { get; set; }
        [YamlMember(Alias = "myd05", ApplyNamingConventions = false)]
        public string Myd05 { get; set; }
        [YamlMember(Alias = "mod11", ApplyNamingConventions = false)]
        public string Mod11 { get; set; }
        [YamlMember(Alias = "myd11", ApplyNamingConventions = false)]
        public string Myd11 { get; set; }

        [YamlMember(Alias = "mod35", ApplyNamingConventions = false)]
        public string Mod35 { get; set; }

        [YamlMember(Alias = "myd35", ApplyNamingConventions = false)]
        public string Myd35 { get; set; }
    }

    public class StandardDirectory
    {
        [YamlMember(Alias = "mod02", ApplyNamingConventions = false)]
        public string Mod02 { get; set; }
        [YamlMember(Alias = "myd02", ApplyNamingConventions = false)]
        public string Myd02 { get; set; }
    }

    public class StandardMapDirectory
    {
        [YamlMember(Alias = "mod02", ApplyNamingConventions = false)]
        public string Mod02 { get; set; }
        [YamlMember(Alias = "myd02", ApplyNamingConventions = false)]
        public string Myd02 { get; set; }

    }
}

using System.IO;
using YamlDotNet.Serialization;
using YamlDotNet.Serialization.NamingConventions;

namespace ModisDownloader
{
    /// <summary>
    /// ModisDownloader配置类
    /// </summary>
    internal class ModisDownloaderConfig
    {
        private const string CONFIG_FILE = "modis_downloader_config.yml";

        [YamlMember(Alias = "app-key", ApplyNamingConventions = false)]
        public string AppKey { get; set; }

        [YamlMember(Alias = "file-id-service", ApplyNamingConventions = false)]
        public string FileIdService { get; set; }

        [YamlMember(Alias = "file-property-service", ApplyNamingConventions = false)]
        public string FilePropertyService { get; set; }

        [YamlMember(Alias = "file-url-service", ApplyNamingConventions = false)]
        public string FileUrlService { get; set; }

        [YamlMember(Alias = "user-agent", ApplyNamingConventions = false)]
        public string UserAgent { get; set; }

        [YamlMember(Alias = "font-size", ApplyNamingConventions = false)]
        public float FontSize { get; set; }

        public static ModisDownloaderConfig CreateInstance()
        {
            var deserializer = new DeserializerBuilder().WithNamingConvention(new CamelCaseNamingConvention()).Build();
            var cfg =
                deserializer.Deserialize<ModisDownloaderConfig>(File.ReadAllText(CONFIG_FILE));
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

        /// <summary>
        /// 配置文件重新加载
        /// </summary>
        public void Reload()
        {
            Global.Config = CreateInstance();
        }
    }
}

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using YamlDotNet.Serialization;
using YamlDotNet.Serialization.NamingConventions;

namespace QuakeAnalysis
{
    public class ModisMainConfig
    {
        private const string CONFIG_FILE = "modis_main.yml";

        [YamlMember(Alias = "font-size", ApplyNamingConventions = false)]
        public float FontSize { get; set; }

        [YamlMember(Alias = "test-attr", ApplyNamingConventions = false)]
        public float TestAttr { get; set; }

        public static ModisMainConfig CreateInstance()
        {
            var deserializer = new DeserializerBuilder().
                WithNamingConvention(new CamelCaseNamingConvention()).Build();
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

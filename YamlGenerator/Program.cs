using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using YamlDotNet.Serialization;

namespace YamlGenerator
{
    class Program
    {
        static void Main(string[] args)
        {
            var modisSetting = new ModisSetting();
            modisSetting.WorkSpaceFolder = @"E:\modis_workspace\";
            modisSetting.SelectedProducts = new List<string> { "MOD021km", "MYD04", "MOD35", "MYD35" };
            modisSetting.StartDate = "2018351";
            modisSetting.EndDate = "2018359";
            modisSetting.ArchiveSetting = new ArchiveSetting
            {
                ModisDataFolder = @"E:\Data\"
            };
            modisSetting.PreprocessSetting = new PreprocessSetting
            {
                PreprocessBt = new PreprocessBt()
            };

            modisSetting.PreprocessSetting.PreprocessBt.MinLon = 113;
            modisSetting.PreprocessSetting.PreprocessBt.MaxLon = 115;
            modisSetting.PreprocessSetting.PreprocessBt.MinLat = 39;
            modisSetting.PreprocessSetting.PreprocessBt.MaxLat = 42;
            modisSetting.PreprocessSetting.PreprocessBt.Band = "Band31";
            modisSetting.PreprocessSetting.PreprocessBt.MRTKernelType = "NN";
            modisSetting.PreprocessSetting.PreprocessBt.MRTProjectionType = "EQRECT";
            modisSetting.PreprocessSetting.PreprocessBt.MRTProjectionArgs =
                "0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0";
            modisSetting.PreprocessSetting.PreprocessBt.MRTPixelSize = 1000;

            var serializer = new Serializer();
            var str = serializer.Serialize(modisSetting);
            File.WriteAllText("modis.yml", str);
            var deserialize = new DeserializerBuilder().Build()
                .Deserialize<ModisSetting>(File.ReadAllText("modis.yml"));


            Generator.Generate(modisSetting);

            Console.ReadLine();
        }
    }
}

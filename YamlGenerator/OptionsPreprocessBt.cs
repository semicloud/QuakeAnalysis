using System.IO;
using YamlDotNet.Serialization;

namespace YamlGenerator
{
    public class OptionsPreprocessBt
    {
        public bool NeedArchive { get; set; }
        public string WorkspacePath { get; set; }
        public string MODISDataPath { get; set; }
        public bool NeedPreprocess { get; set; }
        public string Type { get; set; }
        public string Date { get; set; }

        public double MinLon { get; set; }

        public double MaxLon { get; set; }

        public double MinLat { get; set; }

        public double MaxLat { get; set; }

        public string Band { get; set; }

        public string MRTKernelType { get; set; }

        public string MRTProjectionType { get; set; }

        public string MRTProjectionArgs { get; set; }

        public double MRTPixelSize { get; set; }

        public string ResultDataSetPath { get; set; }

        public void SaveTo(string path)
        {
            var serializer = new SerializerBuilder().EmitDefaults().Build();
            var serialize = serializer.Serialize(this);
            File.WriteAllText(path, serialize);
        }
    }
}

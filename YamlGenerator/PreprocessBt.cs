using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using YamlDotNet.Serialization;

namespace YamlGenerator
{
    public class PreprocessBt
    {
       
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

    }
}

using System;
using System.Linq;
using Antlr4.StringTemplate;
using MathNet.Numerics.LinearAlgebra;
using NLog;
using NUnit.Framework;
using OSGeo.GDAL;

namespace QuakeAnalysis.Method.Gdal
{
    public static class GdalHelper
    {
        private static Logger g = LogManager.GetCurrentClassLogger();

        /// <summary>
        /// 读取MXD02XXX.HDF文件中的radiance_scales和radiance_offsets
        /// 返回值是2*16矩阵，该矩阵的第一行是radiance_scales的值，第二行是radiance_offsets的值
        /// </summary>
        /// <param name="hdfEosFilePath"></param>
        /// <returns>2*16矩阵，该矩阵的第一行是radiance_scales的值，第二行是radiance_offsets的值</returns>
        public static Matrix<double> ReadRadianceScalesAndOffsets(string hdfEosFilePath)
        {
            var realPath = @"HDF4_EOS:EOS_SWATH:""<hdf_eos_file_path>"":MODIS_SWATH_Type_L1B:EV_1KM_Emissive";
            var template = new Template(realPath);
            template.Add("hdf_eos_file_path", hdfEosFilePath);
            realPath = template.Render();
            g.Debug("read radiance_scales, radiance_offsets from {0}", realPath);
            var dataset = OSGeo.GDAL.Gdal.OpenShared(realPath, Access.GA_ReadOnly);
            var scales = dataset.GetMetadataItem("radiance_scales", "");
            var offsets = dataset.GetMetadataItem("radiance_offsets", "");
            dataset.Dispose();

            //TODO 合法性检查，长度相等检查
            var dScales = scales.Split(new[] { "," }, StringSplitOptions.RemoveEmptyEntries).Select(Convert.ToDouble)
                .ToArray();
            var dOffsets = offsets.Split(new[] { "," }, StringSplitOptions.RemoveEmptyEntries).Select(Convert.ToDouble)
                .ToArray();
            //g.Debug("read scales: {0}", string.Join(", ", dScales));
            //g.Debug("read offsets: {0}", string.Join(", ", dOffsets));

            var matrix = Matrix<double>.Build.Dense(2, dScales.Length);
            matrix.SetRow(0, dScales);
            matrix.SetRow(1, dOffsets);
            g.Debug("radiance scales and offsets:");
            g.Debug(matrix);
            return matrix;
        }

        [Test]
        public static void Test2()
        {
            GdalConfiguration.ConfigureGdal();
            OSGeo.GDAL.Gdal.AllRegister();
            var file = @"e:\Data\MODIS\Preprocess\MRT\MYD021KM.A2018305.0630.061.2018305193520.hdf";
            ReadRadianceScalesAndOffsets(file);
        }

        [Test]
        public static void Test1()
        {
            GdalConfiguration.ConfigureGdal();
            GdalConfiguration.ConfigureOgr();
            OSGeo.GDAL.Gdal.AllRegister();
            var s = @"e:\Data\MODIS\Preprocess\MRT\MYD021KM.A2018305.0630.061.2018305193520.hdf";
            var s2 =
                @"HDF4_EOS:EOS_SWATH:""e:\Data\MODIS\Preprocess\MRT\MYD021KM.A2018305.0630.061.2018305193520.hdf"":MODIS_SWATH_Type_L1B:EV_1KM_Emissive";
            var dataset = OSGeo.GDAL.Gdal.OpenShared(s2, Access.GA_ReadOnly);
            Console.WriteLine(dataset.GetDriver().LongName);
            string[] fileList = dataset.GetFileList();
            string[] metadata = dataset.GetMetadata("");

            foreach (var s1 in metadata)
            {
                Console.WriteLine(s1);
                Console.WriteLine("*******");
            }

            Console.WriteLine("--------------------------我是华丽分割线---------------------");

            var scales = dataset.GetMetadataItem("radiance_scales", "");
            Console.WriteLine(scales);

            Console.WriteLine("--------------------------我是华丽分割线---------------------");
            var offsets = dataset.GetMetadataItem("radiance_offsets", "");
            Console.WriteLine(offsets);

            dataset.Dispose();

            Console.WriteLine(s);
        }


    }
}

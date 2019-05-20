using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NUnit.Framework;
using QuakeAnalysis.Entity;

namespace QuakeAnalysis.Method.Modis
{
   public static  class Hdf2GTiffTest
    {
        [Test]
        public static void TestHdf2GTiff()
        {
            var prmSetting = new PrmSetting();
            prmSetting.InputHdfFile =
                @"e:\CodeWorld\VSProjects\QuakeAnalysis\QuakeAnalysis\QuakeAnalysis\bin\Debug\modis_workspace\MXD02\MYD02\MYD021KM.A2018305.0630.061.2018305193520.hdf";
            prmSetting.GeoLocHdfFile =
                @"e:\CodeWorld\VSProjects\QuakeAnalysis\QuakeAnalysis\QuakeAnalysis\bin\Debug\modis_workspace\MXD03\MYD03\MYD03.A2018305.0630.061.2018305155109.hdf";
            prmSetting.SdsStr = "EV_1KM_Emissive, 1";
            prmSetting.UlcLng = 80.0;
            prmSetting.UlcLat = 40.0;
            prmSetting.LrcLng = 110.0;
            prmSetting.LrcLat = 20.0;
            prmSetting.OutputGTiffFile = Config.GetModisTmpDir() + @"\MXD02.tiff";

            Hdf2GTiff hdf2GTiff = new Hdf2GTiff(prmSetting, Config.GetModisTmpDir());
            hdf2GTiff.DoConvert();
        }


        [OneTimeSetUp]
        public static void RunBeforeAnyTests()
        {
            var dir = Path.GetDirectoryName(typeof(Hdf2GTiffTest).Assembly.Location);
            if (dir != null)
            {
                Environment.CurrentDirectory = dir;
                Directory.SetCurrentDirectory(dir);
            }
            else
                throw new Exception("Path.GetDirectoryName(typeof(TestingWithReferencedFiles).Assembly.Location) returned null");
        }

    }
}

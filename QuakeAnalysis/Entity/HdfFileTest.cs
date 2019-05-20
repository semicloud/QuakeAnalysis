using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NUnit.Framework;

namespace QuakeAnalysis.Entity
{
   public  class HdfFileTest
    {
        [Test]
        public static void TestHdfFileNameParsing()
        {
            var fileName =
                @"e:\CodeWorld\VSProjects\QuakeAnalysis\QuakeAnalysis\QuakeAnalysis\bin\Debug\modis_workspace\MXD35\MYD35\MYD35_L2.A2018305.0630.061.2018305193653.hdf";
            var hdfFile = new HdfFile(fileName);
            Assert.AreEqual("MYD35_L2.A2018305.0630.061.2018305193653.hdf", hdfFile.RealFileName);
            Assert.AreEqual("MYD35_L2", hdfFile.MainName);
            Assert.AreEqual("A2018305.0630", hdfFile.DateTimeStr);
        }
    }
}

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NUnit.Framework;

namespace QuakeAnalysis
{
  public static   class ConfigTest
    {
        [Test]
        public static void TestConfig()
        {
            TestContext.Out.WriteLine(Environment.CurrentDirectory);
            TestContext.Out.WriteLine(Config.GetMrtSwathPath());
            TestContext.Out.WriteLine(Config.GetModisTmpDir());
        }


        [OneTimeSetUp]
        public static  void RunBeforeAnyTests()
        {
            var dir = Path.GetDirectoryName(typeof(ConfigTest).Assembly.Location);
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

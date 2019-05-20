using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Antlr4.StringTemplate;
using NUnit.Framework;

namespace QuakeAnalysis
{
    public class StringTemplateTest
    {
        [Test]
        public static void TestStringTemplate()
        {
            Template tt = new Template(File.ReadAllText(@"Template/Prm/prm_demo.st"));
            tt.Add("input_file_name", @"E:\workwork");
            Console.WriteLine(tt.Render(CultureInfo.CurrentCulture));
        }

        [OneTimeSetUp]
        public void RunBeforeAnyTests()
        {
            var dir = Path.GetDirectoryName(typeof(StringTemplateTest).Assembly.Location);
            if (dir != null)
            {
                Environment.CurrentDirectory = dir;
                Directory.SetCurrentDirectory(dir);
                Console.WriteLine("Path Set.");
            }
            else
                throw new Exception("Path.GetDirectoryName(typeof(TestingWithReferencedFiles).Assembly.Location) returned null");
        }
    }
}

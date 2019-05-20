using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using NUnit.Framework;

namespace QuakeAnalysis.Entity
{

    public static class CODGTest
    {
        [Test]
        public static void TestLoadCODG()
        {
            var path = new Uri(Path.GetDirectoryName(Assembly.GetExecutingAssembly().CodeBase) +
                               "\\Data\\CODG1950.17I").AbsolutePath;
            Console.WriteLine(path);

            var allString = File.ReadAllLines(path).ToList();
            Console.WriteLine(allString.Count);

            var startIndexes = allString.FindAll(s => s.Contains("START OF TEC MAP"))
                .Select(s => allString.IndexOf(s)).ToList();
            var endIndexes = allString.FindAll(s => s.Contains("END OF TEC MAP"))
                .Select(s => allString.IndexOf(s)).ToList();
            Console.WriteLine("Start Index:" + string.Join(", ", startIndexes.ToArray()));
            Console.WriteLine("End Index:" + string.Join(", ", endIndexes.ToArray()));

            if (startIndexes.Count != endIndexes.Count)
                Console.WriteLine("Length ERROR");

            var indexes = from i in Enumerable.Range(0, startIndexes.Count)
                select new {StartIndex = startIndexes[i], EndIndex = endIndexes[i]};

            var index = indexes.First();

            var section = allString.GetRange(index.StartIndex, index.EndIndex - index.StartIndex + 1).ToList();

            //Console.WriteLine("");
            //section.ForEach(Console.WriteLine);

            var dataIndexes = section.FindAll(s => s.Contains("LAT/LON1/LON2/DLON/H"))
                .Select(s => section.IndexOf(s))
                .ToList();
            Console.WriteLine("Data Index:" + string.Join(",", dataIndexes));

            var dataIndex = 2;
           
                var strList = section.GetRange(dataIndex + 1, 5);
            var q = strList.SelectMany(s => s.Split(new[] {"   "}, StringSplitOptions.None))
                .ToList();
            q.RemoveAll(s => s.Equals(string.Empty));
            var doubles = q.Select(Convert.ToDouble).ToList();

            Console.WriteLine(doubles.Count);
        }
    }
}

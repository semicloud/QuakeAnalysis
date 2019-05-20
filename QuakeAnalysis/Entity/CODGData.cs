using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using DevExpress.Utils.Extensions;
using Newtonsoft.Json;
using NLog;
using NUnit.Framework;

namespace QuakeAnalysis.Entity
{
    /// <summary>
    /// CODG数据，数据文件的后缀名为17I
    /// </summary>
    public class CODGData
    {
        private DateTime _firstEpoch;
        private DateTime _lastEpoch;
        private List<CODGDataItem> _dataItems;

        private const string EPOCH_FIRST = "EPOCH OF FIRST MAP";
        private const string EPOCH_LAST = "EPOCH OF LAST MAP";
        private const string TEC_START = "START OF TEC MAP";
        private const string TEC_END = "END OF TEC MAP";

        private static Logger g = LogManager.GetCurrentClassLogger();

        public DateTime FirstEpoch
        {
            get { return _firstEpoch; }
            set { _firstEpoch = value; }
        }

        public DateTime LastEpoch
        {
            get { return _lastEpoch; }
            set { _lastEpoch = value; }
        }

        public List<CODGDataItem> DataItems
        {
            get { return _dataItems; }
            set { _dataItems = value; }
        }

        [Test]
        public static void TestLoad()
        {
            var path = new Uri(Path.GetDirectoryName(Assembly.GetExecutingAssembly().CodeBase) +
                               "\\Data\\CODG1950.17I").AbsolutePath;
            Console.WriteLine(path);

            var data = LoadFromFile(path);
            Console.WriteLine(JsonConvert.SerializeObject(data));
        }

        public static CODGData LoadFromFile(string codgFilePath)
        {
            if (!File.Exists(codgFilePath))
                throw new FileNotFoundException("file " + codgFilePath + " not found!");

            var codgData = new CODGData();
            var allString = File.ReadAllLines(codgFilePath).ToList();
            g.Debug("read {0} lines from {1}", allString.Count, codgFilePath);

            codgData.FirstEpoch = ParseEpoch(allString.Find(s => s.Contains(EPOCH_FIRST)));
            codgData.LastEpoch = ParseEpoch(allString.Find(s => s.Contains(EPOCH_LAST)));

            var startIndexes = Enumerable.Range(0, allString.Count)
                .Where(i => allString[i].Contains(TEC_START)).ToList();
            var endIndexes = Enumerable.Range(0, allString.Count)
                .Where(i => allString[i].Contains(TEC_END)).ToList();
            // 此处应有两个列表大小相等的测试

            var dataIndexes = (from i in Enumerable.Range(0, startIndexes.Count)
                              select new { StartIndex = startIndexes[i], EndIndex = endIndexes[i] }).ToList();
            var dataStrLists =
                dataIndexes.Select(dix => allString.GetRange(dix.StartIndex, dix.EndIndex - dix.StartIndex + 1));
            var dataItems = dataStrLists.Select(CODGDataItem.Parse).ToList();

            codgData.DataItems = dataItems;

            return codgData;
        }

        private static DateTime ParseEpoch(string epochStr)
        {
            var array = epochStr.Split(new[] { " " }, StringSplitOptions.RemoveEmptyEntries).ToList();
            array.RemoveAll(s => s.Equals(string.Empty));
            array.RemoveAll(s => !Regex.IsMatch(s, "[^a-zA-Z]")); // 此处应有保护代码
            var numbers = array.Select(s => Convert.ToInt32(s)).ToList();
            return new DateTime(numbers[0], numbers[1], numbers[2], 0, 0, 0);
        }

    }
}

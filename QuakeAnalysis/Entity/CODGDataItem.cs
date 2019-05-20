using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using DevExpress.XtraPrinting.Native;

namespace QuakeAnalysis.Entity
{
    /// <summary>
    /// CODG中的数据项
    /// 一个数据项包括：
    ///     数据项的索引值，即17I文件中START OF TEC MAP
    ///     数据项的采集时间，即17I文件中的EPOCH OF CURRENT MAP

    /// </summary>
    public class CODGDataItem
    {
        private int _index;
        private DateTime _dateTime;
        private List<CODGDataElement> _elements;

       
        private CODGDataItem()
        {

        }

        private const string INDEX = "START OF TEC MAP";
        private const string EPOCH = "EPOCH OF CURRENT MAP";
        private const string DATA_START = "LAT/LON1/LON2/DLON/H";

        public static CODGDataItem Parse(List<string> strList)
        {
            var codgDataItem = new CODGDataItem();
            codgDataItem.Index = ParseIndex(strList);
            codgDataItem.DateTime = ParseEpoch(strList);
            codgDataItem.Elements = ParseDataElements(strList);
            return codgDataItem;
        }

        private static int ParseIndex(List<string> strList)
        {
            var indexStr = strList.Find(s => s.Contains(INDEX));
            var strings = indexStr.Split(new[] {" "}, StringSplitOptions.RemoveEmptyEntries);
            return Convert.ToInt32(strings.First());
        }

        private static DateTime ParseEpoch(List<string> strList)
        {
            var epochStr = strList.Find(s => s.Contains(EPOCH));
            var strs = epochStr.Split(new[] {" "}, StringSplitOptions.RemoveEmptyEntries).ToList();
            //TODO 修改此处的匹配表达式，将不是数字的元素除去
            strs.RemoveAll(s => Regex.IsMatch(s, "^[a-zA-Z]"));
            var numbers = strs.Select(s => Convert.ToInt32(s)).ToList();
            return new DateTime(numbers[0], numbers[1], numbers[2], numbers[3], 0, 0);
        }

        private static List<CODGDataElement> ParseDataElements(List<string> strList)
        {
            var elements = new List<CODGDataElement>();

            var elementIndex = Enumerable.Range(0, strList.Count)
                .Where(i => strList[i].Contains(DATA_START)).ToList();
            var elementStrList = elementIndex.Select(idx => strList.GetRange(idx, 6)).ToList();

            elements = elementStrList.Select(CODGDataElement.Parse).ToList();
            return elements;
        }

        public int Index
        {
            get { return _index; }
            set { _index = value; }
        }

        public DateTime DateTime
        {
            get { return _dateTime; }
            set { _dateTime = value; }
        }

        public List<CODGDataElement> Elements
        {
            get { return _elements; }
            set { _elements = value; }
        }

    }
}

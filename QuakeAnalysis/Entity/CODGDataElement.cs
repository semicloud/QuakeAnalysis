using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace QuakeAnalysis.Entity
{
    /// <summary>
    ///     采集数据，共73个
    /// </summary>
    public class CODGDataElement
   {
       private double _latitude;
       private List<double> _values;

       private const int LATITUDE_POSITION_INDEX = 8;

       public static CODGDataElement Parse(List<string> strList)
       {
           return new CODGDataElement
           {
               Latitude = ParseLatitude(strList),
               Values = ParseValues(strList)
           };
       }

       public double Latitude
       {
           get { return _latitude; }
           set { _latitude = value; }
       }

       public List<double> Values
       {
           get { return _values; }
           set { _values = value; }
       }

       private static double ParseLatitude(List<string> strList)
       {
           return Convert.ToDouble(strList.First().Substring(0, LATITUDE_POSITION_INDEX).Trim());
       }

       private static List<double> ParseValues(List<string> strList)
       {
           // 从第2行开始，共取4行数据
           var dataList = strList.GetRange(1, 5)
               .SelectMany(s => s.Split(new[] { " " }, StringSplitOptions.RemoveEmptyEntries))
               .Select(Convert.ToDouble).ToList();
           return dataList;
       }
    }
}

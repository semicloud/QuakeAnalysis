using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NLog;

namespace ModisEntity
{
    /// <summary>
    /// HDF文件
    /// </summary>
    public class HdfFile
    {
        private static Logger g = LogManager.GetCurrentClassLogger();

        private string _filePath;
        private string _realFileName;
        private string _mainName;
        private string _dateTimeStr;
        private string _year;
        private string _dayOfYear;
        private string _remainder;
        private ModisType _modisType;

        public HdfFile(string filePath)
        {
            _filePath = filePath;
            _realFileName = Path.GetFileName(_filePath);

            var fileName = Path.GetFileNameWithoutExtension(_filePath);
            //g.Debug("parse hdf file: {0}", _filePath);
            //g.Debug("real file name: {0}", _realFileName);
            //g.Debug("file name: {0}", fileName);

            var strs = fileName.Split(new[] { "." }, StringSplitOptions.None);
            //g.Debug("file name split: {0}", string.Join(", ", strs));

            //TODO 需检查strs的合法性

            _mainName = strs[0];
            _dateTimeStr = strs[1] + "." + strs[2];
            _remainder = strs[3] + strs[4];

            var dateStr = strs[1];
            dateStr = dateStr.Substring(1, dateStr.Length - 1);

            _year = dateStr.Substring(0, 4);
            _dayOfYear = dateStr.Substring(4, 3);
            g.Debug($"年份：{_year}，年积日：{_dayOfYear}");


            if (_filePath.Contains("MOD"))
                _modisType = ModisType.MOD;
            else if (_filePath.Contains("MYD"))
                _modisType = ModisType.MYD;
            else
                throw new ModisTypeNotSupportedException("除MOD、MYD以外不支持的MODIS数据！");

            g.Debug("main name, datetime str, remainder, MODIS: {0}, {1}, {2}, {3}",
                _mainName, _dateTimeStr, _remainder, _modisType);
        }

        /// <summary>
        /// HDF文件所在的路径
        /// </summary>
        public string FilePath
        {
            get => _filePath;
            set => _filePath = value;
        }

        /// <summary>
        /// HDF文件名（带扩展名）
        /// </summary>
        public string RealFileName => _realFileName;

        /// <summary>
        /// 主名称，即文件名第一个“.”前面的部分
        /// </summary>
        public string MainName => _mainName;

        /// <summary>
        /// 日期及时间字符串，用于定位同组HDF文件
        /// </summary>
        public string DateTimeStr => _dateTimeStr;

        /// <summary>
        /// 该文件的日期，就是Ayyyymmdd里面的这个日期
        /// </summary>
        public string Year => _year;

        /// <summary>
        /// 该文件的观测日
        /// </summary>
        public string DayOfYear => _dayOfYear;

        /// <summary>
        /// 其余部分，不重要
        /// </summary>
        public string Remainder => _remainder;

        /// <summary>
        /// 数据集类型，MOD或MYD
        /// </summary>
        public ModisType ModisType => _modisType;
    }

    /// <summary>
    /// 不支持的MODIS数据
    /// </summary>
    public class ModisTypeNotSupportedException : Exception
    {
        public ModisTypeNotSupportedException()
        {
        }

        public ModisTypeNotSupportedException(string message) : base(message)
        {
        }

        public ModisTypeNotSupportedException(string message, Exception innerException) : base(message, innerException)
        {
        }
    }

    /// <summary>
    /// HDF文件名解析异常
    /// </summary>
    [Serializable]
    public class HdfFileNameParseError : Exception
    {
        public HdfFileNameParseError()
        {
        }

        public HdfFileNameParseError(string message) : base(message)
        {
        }

        public HdfFileNameParseError(string message, Exception innerException) : base(message, innerException)
        {
        }
    }
}
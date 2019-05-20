using System;
using System.IO;
using System.Threading.Tasks;
using NLog;

namespace QuakeAnalysis.Entity
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
        /// 其余部分，不重要
        /// </summary>
        public string Remainder => _remainder;

        /// <summary>
        /// 数据集类型，MOD或MYD
        /// </summary>
        public ModisType ModisType => _modisType;

        /// <summary>
        /// 归档
        /// </summary>
        public async Task Archive()
        {
            var archivedFilePath = GetArchiveFilePath();
            using (Stream source = File.Open(_filePath, FileMode.Open))
            {
                using (var dest = File.Create(archivedFilePath))
                {
                    await source.CopyToAsync(dest);
                }
            }
            //File.Copy(_filePath, archivedFilePath);
            g.Debug($"archive [{_filePath}] to [${archivedFilePath}] success!");
        }

        /// <summary>
        /// 获取该HDF文件的应归档路径
        /// </summary>
        /// <returns>该HDF文件的应归档路径</returns>
        private string GetArchiveFilePath()
        {
            var directories = QuakeAnalysisConfig.CreateInstance().DataDirectory.Hdf;
            var directory = string.Empty;
            switch (_mainName)
            {
                // TODO Oh, Magic Strings..
                case "MOD021KM":
                    directory = directories.Mod02;
                    break;
                case "MYD021KM":
                    directory = directories.Myd02;
                    break;
                case "MOD03":
                    directory = directories.Mod03;
                    break;
                case "MYD03":
                    directory = directories.Myd03;
                    break;
                case "MOD04_3K":
                    directory = directories.Mod04;
                    break;
                case "MYD04_3K":
                    directory = directories.Myd04;
                    break;
                case "MOD05_L2":
                    directory = directories.Mod05;
                    break;
                case "MYD05_L2":
                    directory = directories.Myd05;
                    break;
                case "MOD11A1":
                    directory = directories.Mod11;
                    break;
                case "MYD11A1":
                    directory = directories.Myd11;
                    break;
                case "MOD35_L2":
                    directory = directories.Mod35;
                    break;
                case "MYD35_L2":
                    directory = directories.Myd35;
                    break;
                default:
                    throw new ModisTypeNotSupportedException("Unsupported modis data type: " + _mainName);
            }

            var archivedFilePath = Environment.CurrentDirectory + @"\" + directory + _realFileName;
            g.Debug($"get modis archive file path {_filePath} --> {archivedFilePath}");
            return archivedFilePath;
        }
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

namespace ModisEntity
{
    /// <summary>
    /// SWATH2GRID.EXE Prm文件参数设置
    /// </summary>
    public class PrmSetting
    {
        private string _inputHdfFile;
        private string _geoLocHdfFile;
        private string _sdsStr;
        private double _ulcLng;
        private double _ulcLat;
        private double _lrcLng;
        private double _lrcLat;
        private string _outputGTiffFile;

        /// <summary>
        /// 输入HDF文件（全路径）
        /// </summary>
        public string InputHdfFile
        {
            get => _inputHdfFile;
            set => _inputHdfFile = value;
        }

        /// <summary>
        /// GeoLocation HDF文件
        /// </summary>
        public string GeoLocHdfFile
        {
            get => _geoLocHdfFile;
            set => _geoLocHdfFile = value;
        }

        /// <summary>
        /// 要提取的波段信息
        /// </summary>
        public string SdsStr
        {
            get => _sdsStr;
            set => _sdsStr = value;
        }

        /// <summary>
        /// 提取范围 Upper Left Corner Longitude 
        /// </summary>
        public double UlcLng
        {
            get => _ulcLng;
            set => _ulcLng = value;
        }

        /// <summary>
        /// 提取范围 Upper Left Corner Latitude
        /// </summary>
        public double UlcLat
        {
            get => _ulcLat;
            set => _ulcLat = value;
        }

        /// <summary>
        /// 提取范围 Lower Right Corner Longitude
        /// </summary>
        public double LrcLng
        {
            get => _lrcLng;
            set => _lrcLng = value;
        }

        /// <summary>
        /// 提取范围 Lower Right Corner Latitude
        /// </summary>
        public double LrcLat
        {
            get => _lrcLat;
            set => _lrcLat = value;
        }

        /// <summary>
        /// 输出GTiff文件路径
        /// </summary>
        public string OutputGTiffFile
        {
            get => _outputGTiffFile;
            set => _outputGTiffFile = value;
        }
    }
}

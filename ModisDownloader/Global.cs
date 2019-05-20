namespace ModisDownloader
{
    /// <summary>
    /// 工具全局设置
    /// </summary>
    static class Global
    {
        private static ModisDownloaderConfig _config;

        /// <summary>
        /// 软件配置
        /// </summary>
        public static ModisDownloaderConfig Config
        {
            get
            {
                if (_config != null) return _config;
                _config = ModisDownloaderConfig.CreateInstance();
                return _config;
            }

            set => _config = value;
        }
    }
}

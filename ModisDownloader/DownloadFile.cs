using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.ComponentModel.DataAnnotations;
using System.Diagnostics.Contracts;
using System.Linq;

namespace ModisDownloader
{
    public class DownloadFile
    {
        private string _id;
        private string _name;
        private long _size;
        private double _sizeMb;
        private string _type;
        private DateTime _ingestTime;
        private DateTime _startTime;
        private bool _onLine;
        private string _checkSum;
        private string _url;

        [DisplayName("文件编号")]
        [Browsable(false)]
        public string Id
        {
            get => _id;
            set => _id = value;
        }

        [DisplayName("文件名")]
        public string Name
        {
            get => _name;
            set => _name = value;
        }

        [Browsable(false)]
        public long Size
        {
            get => _size;
            set => _size = value;
        }

        [DisplayName("文件类型")]
        [Browsable(false)]
        public string Type
        {
            get => _type;
            set => _type = value;
        }

        [Browsable(false)]
        public DateTime IngestTime
        {
            get => _ingestTime;
            set => _ingestTime = value;
        }

        [Browsable(false)]
        public DateTime StartTime
        {
            get => _startTime;
            set => _startTime = value;
        }

        [DisplayName("是否在线")]
        [Browsable(false)]
        public bool OnLine
        {
            get => _onLine;
            set => _onLine = value;
        }

        [DisplayName("校验和")]
        [Browsable(true)]
        public string CheckSum
        {
            get => _checkSum;
            set => _checkSum = value;
        }

        [Browsable(false)]
        public string Url
        {
            get => _url;
            set => _url = value;
        }

        [DisplayName("文件大小(MB)")]
        public double SizeMb { get => _sizeMb; }

        public DownloadFile(string id, string name, long size, string type, DateTime ingestTime, DateTime startTime, bool onLine, string checkSum, string url)
        {
            _id = id;
            _name = name;
            _size = size;
            _sizeMb = Math.Round((double)_size / 1048576, 2);
            _type = type;
            _ingestTime = ingestTime;
            _startTime = startTime;
            _onLine = onLine;
            _checkSum = checkSum;
            _url = url;
        }

        public static List<DownloadFile> LoadDownloadFiles(List<getFilePropertiesResponseReturn> fileProperties,
            List<string> fileUrls)
        {
            Contract.Assert(fileProperties.Count == fileUrls.Count, "文件属性列表与文件地址列表长度应相等！");
            var downloadFiles = new List<DownloadFile>();
            foreach (var fileProperty in fileProperties)
            {
                var downloadFile = new DownloadFile(fileProperty.fileId,
                    fileProperty.fileName,
                    Convert.ToInt32(fileProperty.fileSizeBytes),
                    fileProperty.fileType, DateTime.Parse(fileProperty.ingestTime),
                    DateTime.Parse(fileProperty.startTime), fileProperty.online.Equals("true"), fileProperty.checksum,
                    // TODO 到不到文件下载地址怎么办
                    fileUrls.Find(u => u.Contains(fileProperty.fileName))
                );
                downloadFiles.Add(downloadFile);
            }

            return downloadFiles.OrderBy(d => d.Type).ThenBy(d => d.StartTime).ToList();
        }
    }
}

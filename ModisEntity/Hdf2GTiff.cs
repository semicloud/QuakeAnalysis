using System;
using System.IO;
using NLog;

namespace ModisEntity
{
    public class Hdf2GTiff
    {
        private PrmSetting _prmSetting;
        private PrmFile _prmFile;
        private PrmBatFile _prmBatFile;
        private readonly string _convertAt;
        private static Logger g = LogManager.GetCurrentClassLogger();

        /// <summary>
        /// Hdf2GTiff工具
        /// </summary>
        /// <param name="prmSetting">Prm文件设置</param>
        /// <param name="path">进行转换的目录，转换需要的Prm文件以及Bat脚本将保存在这个目录，一般应设置为workspace/Tmp目录</param>
        public Hdf2GTiff(PrmSetting prmSetting, string convertAt)
        {
            _prmSetting = prmSetting;
            _prmFile = new PrmFile(_prmSetting);
            _convertAt = convertAt;
        }

        private bool CheckData()
        {
            throw new NotImplementedException();
        }

        public void DoConvert()
        {
            //TODO 运行前的运行环境清理
            g.Debug("-----------------------------------------------------------------");
            g.Debug("convert {0} to {1} ..", Path.GetFileName(_prmSetting.InputHdfFile),
                Path.GetFileName(_prmSetting.OutputGTiffFile));

            var prmFilePath = _convertAt + @"hdf2tiff.prm";
            _prmFile.ExportPrmFile(prmFilePath);

            var prmBatFilePath = _convertAt + @"\run.bat";
            // TODO SWATH PATH
            _prmBatFile = new PrmBatFile($@"{Environment.CurrentDirectory}\MRTSwath", prmFilePath);
            _prmBatFile.ExportPrmBatFile(prmBatFilePath);
            // TODO 可以优化
            var task = CmdRunner.RunProcessAsync(prmBatFilePath);
            task.Wait();
            if (task.IsCompleted)
                g.Debug("invoke swath2grid.exe ok, return code: " + task.Result);

            g.Debug("-----------------------------------------------------------------");
        }
    }
}
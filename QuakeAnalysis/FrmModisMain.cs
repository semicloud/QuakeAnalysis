using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using NLog;
using QuakeAnalysis.Entity;

namespace QuakeAnalysis
{
    public partial class FrmModisMain : Form
    {
        private string _dataFolder;
        private static Logger g = LogManager.GetCurrentClassLogger();

        public FrmModisMain()
        {
            InitializeComponent();
            StartPosition = FormStartPosition.CenterScreen;
        }

        private void FrmModisMain_Load(object sender, EventArgs e)
        {
        }

        private void button4_Click(object sender, EventArgs e)
        {

        }

        private void btnArchive_Click(object sender, EventArgs e)
        {
            var frmModisArchive = new FrmModisArchive();
            frmModisArchive.StartPosition = FormStartPosition.CenterParent;
            if (frmModisArchive.ShowDialog() == DialogResult.OK)
            {
                _dataFolder = frmModisArchive.GetDataFolder();
            }
        }

        private async void btnRun_Click(object sender, EventArgs e)
        {
            await ArchiveFiles(_dataFolder);
        }

        /// <summary>
        /// 将dataFolder目录下的数据文件进行归档
        /// </summary>
        /// <param name="dataFolder"></param>
        private static async Task ArchiveFiles(string dataFolder)
        {
            g.Debug($"archive modis files from [{dataFolder}]");

            var files = Directory.EnumerateFiles(dataFolder).ToList();
            var hdfFiles = files.ToList().FindAll(f => f.EndsWith("hdf") | f.EndsWith("HDF"));
            g.Debug($"{hdfFiles.Count} .hdf files ({files.Count()} totally) found in directory [{dataFolder}]");

            foreach (var file in hdfFiles)
            {
                var hdfFile = new HdfFile(file);
                await hdfFile.Archive();
            }

            g.Debug($"archive modis files complete.");
            g.Info($"arichve modis files from directory [{dataFolder}]. complete.");
        }

        public List<string> GetCheckedProducts()
        {
            return null;
        }

        private void button8_Click(object sender, EventArgs e)
        {

        }
    }
}

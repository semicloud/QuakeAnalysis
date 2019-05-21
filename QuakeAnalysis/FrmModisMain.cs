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

        private static readonly float[] SUPPORT_FONT_SIZES =
            {9, 10, 11, 12, 13, 14, 15};

        private void cboxFontSize_SelectedIndexChanged(object sender, EventArgs e)
        {
            float fontSize = Convert.ToSingle(cboxFontSize.SelectedValue);
            this.Font = this.Font.ChangeFontSize(fontSize);
            GlobalModisMain.Config.FontSize = fontSize;
            GlobalModisMain.Config.SaveToFile();
        }

        public FrmModisMain()
        {
            InitializeComponent();
            this.Font = this.Font.ChangeFontSize(GlobalModisMain.Config.FontSize);
            StartPosition = FormStartPosition.CenterScreen;
        }

        private void FrmModisMain_Load(object sender, EventArgs e)
        {
            cboxFontSize.DataSource = SUPPORT_FONT_SIZES;
            cboxFontSize.SelectedIndexChanged += cboxFontSize_SelectedIndexChanged;
            cboxFontSize.SelectedIndex = SUPPORT_FONT_SIZES.ToList()
                .IndexOf(GlobalModisMain.Config.FontSize);
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
            List<string> ans = new List<string>();
            foreach (Control control in gboxData.Controls)
            {
                if (control is CheckBox)
                {
                    CheckBox checkBox = (CheckBox)control;
                    if (checkBox.Checked)
                    {
                        ans.Add(checkBox.Text);
                    }
                }
            }
            ans.Sort();
            g.Debug($"Checked products:{string.Join(",", ans)}");

            return ans;
        }

        private void btnWSFolder_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog folderBrowserDialog =
                new FolderBrowserDialog();
            if (folderBrowserDialog.ShowDialog() == DialogResult.OK)
            {
                txtWSFolder.Text = folderBrowserDialog.SelectedPath;
            }
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            GetCheckedProducts();
        }
    }
}

using NLog;
using QuakeAnalysis.Entity;
using QuakeAnalysis.Properties;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace QuakeAnalysis
{
    public partial class FrmModisMain : Form
    {
        /// <summary>
        /// 数据目录
        /// </summary>
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
            rbtnYMD.Checked = true;

            txtWSFolder.Text = GlobalModisMain.Config.WorkspaceDir;
            dtpStart.Value = new DateTime(2019, 10, 1);
            dtpEnd.Value = new DateTime(2019, 10, 3);
        }

        private void button4_Click(object sender, EventArgs e)
        {

        }

        private void btnArchive_Click(object sender, EventArgs e)
        {
            var frmModisArchive = new FrmDataArchive();
            frmModisArchive.StartPosition = FormStartPosition.CenterParent;
            if (frmModisArchive.ShowDialog() == DialogResult.OK)
            {
                _dataFolder = frmModisArchive.DataFolder;
            }
        }

        private async void btnRun_Click(object sender, EventArgs e)
        {
            //await ArchiveFiles(_dataFolder);
            //if (ckBoxArchive.Checked)
            //{
            //    if (String.IsNullOrEmpty(_dataFolder))
            //    {
            //        MessageBox.Show("未设置数据目录，无法归档！", Settings.Default.DT, MessageBoxButtons.OK, MessageBoxIcon.Error);
            //        return;
            //    }

            //    if (!Directory.Exists(_dataFolder))
            //    {
            //        MessageBox.Show($"数据目录{_dataFolder}不存在，无法归档！", Settings.Default.DT, MessageBoxButtons.OK,
            //            MessageBoxIcon.Error);
            //        return;
            //    }

            //    await CmdRunner.RunProcessAsync("cmd.exe",
            //        $@"/c {GlobalModisMain.Config.ModisArchive} -w {txtWSFolder.Text.Trim()} -a {_dataFolder} -c");

            //    MessageBox.Show("归档完成");
            //}

            //// 预处理
            //if (ckBoxPreprocess.Checked)
            //{

            //}

            await CmdRunner.RunProcessAsync("cmd.exe", $@"/c {GlobalModisMain.Config.WorkspaceDir}\Scripts\a.bat");
            MessageBox.Show("运行完成！", Settings.Default.DT, MessageBoxButtons.OK, MessageBoxIcon.Information);
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

        private List<string> GetCheckedProducts()
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
            folderBrowserDialog.SelectedPath = GlobalModisMain.Config.WorkspaceDir;
            if (folderBrowserDialog.ShowDialog() == DialogResult.OK)
            {
                txtWSFolder.Text = folderBrowserDialog.SelectedPath;
                GlobalModisMain.Config.WorkspaceDir = folderBrowserDialog.SelectedPath;
                GlobalModisMain.Config.SaveToFile();
            }
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            var checkedProducts = GetCheckedProducts();
            if (checkedProducts.Count == 0)
            {
                MessageBox.Show("未选择数据！", Settings.Default.DT,
                    MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            if (dtpStart.Value >= dtpEnd.Value)
            {
                MessageBox.Show("开始时间必须小于结束时间！", Settings.Default.DT,
                    MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            if (MessageBox.Show("确定保存配置吗？", Settings.Default.DT, MessageBoxButtons.OKCancel, MessageBoxIcon.Question) ==
                DialogResult.OK)
            {
                GlobalModisMain.Config.SaveToFile();
                GenerateScripts();
                MessageBox.Show("配置已保存！", Settings.Default.DT, MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        private void GenerateScripts()
        {
            StringBuilder sb = new StringBuilder();
            var cfg = GlobalModisMain.Config;
            if (ckBoxArchive.Checked)
            {
                sb.AppendLine($"{cfg.ModisArchive} -w {cfg.WorkspaceDir} -a {cfg.DataDir}");
            }

            if (ckBoxPreprocess.Checked)
            {
                foreach (var product in GetCheckedProducts())
                {
                    switch (product)
                    {
                        case "MOD02":
                        case "MYD02":
                            sb.AppendLine($@"cd {new FileInfo(cfg.ModisProc02).DirectoryName}");
                            sb.AppendLine(Get02PreprocessScript(product, dtpStart.Value, dtpEnd.Value));
                            break;
                        case "MOD04":
                        case "MYD04":

                            break;

                        case "MOD05":
                        case "MYD05":
                            break;
                        case "MOD11":
                        case "MYD11":
                            break;
                    }
                }
            }

            File.WriteAllText($@"{GlobalModisMain.Config.WorkspaceDir}\Scripts\a.bat", sb.ToString());
        }

        public static string Get02PreprocessScript(string product, DateTime start, DateTime end)
        {
            StringBuilder sb = new StringBuilder();
            for (DateTime date = start; date <= end; date = date.AddDays(1))
            {
                string ymlPath = Generate02PreprocessYml(product.Substring(0, 3), date);
                sb.AppendLine($@"{GlobalModisMain.Config.ModisProc02} -y {ymlPath}");
            }
            return sb.ToString();
        }

        public static string Generate02PreprocessYml(string type, DateTime date)
        {
            var cfg = GlobalModisMain.Config;
            var hdfListFile = Generate02HdfListFile(type, date);
            StringBuilder sb = new StringBuilder($"HDFListFile: {hdfListFile}\n");
            sb.AppendLine($@"TmpPath: {cfg.WorkspaceDir}\tmp");
            sb.AppendLine($"MinLon: {cfg.PrepMinLon}");
            sb.AppendLine($"MaxLon: {cfg.PrepMaxLon}");
            sb.AppendLine($"MinLat: {cfg.PrepMinLat}");
            sb.AppendLine($"MaxLat: {cfg.PrepMaxLat}");
            sb.AppendLine($"Band: {cfg.Prep02Band}");
            sb.AppendLine($"MRTKernelType: {cfg.Prep02MrtKernelType}");
            sb.AppendLine($"MRTProjectionType: {cfg.Prep02MrtProjType}");
            sb.AppendLine($"MRTProjectionArgs: {cfg.Prep02MrtProjArgs}");
            sb.AppendLine($"MRTPixelSize: {cfg.Prep02MrtPixelSize}");
            sb.AppendLine($@"OutputImageFile: {cfg.WorkspaceDir}\Standard\{type}_BT\bt_{date.Year}_{date.DayOfYear}.tif");
            string outputPath = $@"{cfg.WorkspaceDir}\Scripts\{type}021KM_{date.Year}_{date.DayOfYear}.yml";
            if (File.Exists(outputPath)) File.Delete(outputPath);
            File.WriteAllText(outputPath, sb.ToString());
            return outputPath;
        }

        public static string Generate02HdfListFile(string type, DateTime date)
        {
            var cfg = GlobalModisMain.Config;
            string year = date.Year.ToString();
            string day = date.DayOfYear.ToString();
            string btHdfDir = $@"{cfg.WorkspaceDir}\{type}021KM\{year}\{day}\";
            string szaHdfDir = $@"{cfg.WorkspaceDir}\{type}03\{year}\{day}\";
            string cmHdfDir = $@"{cfg.WorkspaceDir}\{type}35_L2\{year}\{day}\";
            DirectoryInfo directoryInfo = new DirectoryInfo(btHdfDir);
            StringBuilder sb = new StringBuilder();
            foreach (FileInfo file in directoryInfo.EnumerateFiles("*.hdf"))
            {
                string searchStr = file.Name.Substring(9, 13);
                var szaFiles = Directory.EnumerateFiles(szaHdfDir, "*.hdf").ToList();
                string szaFileName = szaFiles.Find(s => s.Contains(searchStr));
                var cmFiles = Directory.EnumerateFiles(cmHdfDir, "*.hdf").ToList();
                string cmFileName = cmFiles.Find(s => s.Contains(searchStr));
                if (!string.IsNullOrEmpty(szaFileName) && !string.IsNullOrEmpty(cmFileName))
                {
                    sb.AppendLine("#"); //#开头
                    sb.AppendLine(file.FullName);
                    sb.AppendLine(szaFileName);
                    sb.AppendLine(cmFileName);
                }
                else
                {
                    MessageBox.Show($"不存在{date}的云掩膜或太阳天顶角文件！", Settings.Default.DT,
                        MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return "";
                }
            }
            string path = $@"{cfg.WorkspaceDir}\Scripts\{type}021KM_{year}_{day}_hdflist.txt";
            if (File.Exists(path)) File.Delete(path);
            File.WriteAllText(path, sb.ToString());
            return path;
        }

        public static string Get04PreprocessScript(string product, DateTime start, DateTime end)
        {
            return "";

        }

        public static string Generate04PreprocessYml(string type, DateTime date)
        {
            return "";

        }

        public static string Generate04HdfListFile(string product, DateTime date)
        {
            return "";

        }

        public static string Get05PreprocessScript(string product, DateTime start, DateTime end)
        {
            return "";

        }

        public static string Generate05PreprocessYml(string type, DateTime date)
        {
            return "";

        }

        public static string Generate05HdfListFile(string product, DateTime date)
        {
            return "";

        }

        public static string Get11PreprocessScript(string product, DateTime start, DateTime end)
        {
            return "";

        }

        public static string Generate11PreprocessYml(string type, DateTime date)
        {
            return "";

        }

        public static string Generate11HdfListFile(string product, DateTime date)
        {
            return "";
        }



        private void GeneratePreprocessScripts()
        {
            var start = dtpStart.Value;
            var end = dtpEnd.Value;
            var type = "MOD";
            var cfg = GlobalModisMain.Config;
            for (DateTime date = start; date != end;)
            {
                string year = date.Year.ToString();
                string day = date.DayOfYear.ToString();
                string btHdfDir = $@"{cfg.WorkspaceDir}\{type}021KM\{year}\{day}\";
                string szaHdfDir = $@"{cfg.WorkspaceDir}\{type}03\{year}\{day}\";
                string cmHdfDir = $@"{cfg.WorkspaceDir}\{type}35_L2\{year}\{day}\";
                DirectoryInfo directoryInfo = new DirectoryInfo(btHdfDir);
                StringBuilder sb = new StringBuilder();
                foreach (FileInfo file in directoryInfo.EnumerateFiles("*.hdf"))
                {
                    string searchStr = file.Name.Substring(9, 13);
                    var szaFiles = Directory.EnumerateFiles(szaHdfDir, "*.hdf").ToList();
                    string szaFileName = szaFiles.Find(s => s.Contains(searchStr));
                    var cmFiles = Directory.EnumerateFiles(cmHdfDir, "*.hdf").ToList();
                    string cmFileName = cmFiles.Find(s => s.Contains(searchStr));
                    if (!string.IsNullOrEmpty(szaFileName) && !string.IsNullOrEmpty(cmFileName))
                    {
                        sb.AppendLine("#"); //#开头
                        sb.AppendLine(file.FullName);
                        sb.AppendLine(szaFileName);
                        sb.AppendLine(cmFileName);
                    }
                    else
                    {
                        Console.WriteLine("不存在的Sza或CM文件！");
                    }
                }

                date = date.AddDays(1);
            }
        }

        private void rbtnYMD_CheckedChanged(object sender, EventArgs e)
        {
            dtpStart.Enabled = rbtnYMD.Checked;
            dtpEnd.Enabled = rbtnYMD.Checked;
        }

        private void rbtnDOY_CheckedChanged(object sender, EventArgs e)
        {
            txtStartYear.Enabled = rbtnDOY.Checked;
            txtEndYear.Enabled = rbtnDOY.Checked;
            txtStartDay.Enabled = rbtnDOY.Checked;
            txtEndDay.Enabled = rbtnDOY.Checked;
        }

        private void ckBoxArchive_CheckedChanged(object sender, EventArgs e)
        {
            if (String.IsNullOrEmpty(txtWSFolder.Text))
            {
                MessageBox.Show("还没有选择工作空间目录！", Settings.Default.DT, MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return;
            }

            if (!Directory.Exists(txtWSFolder.Text))
            {
                MessageBox.Show("工作空间目录不存在！", Settings.Default.DT, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            btnArchive.Enabled = ckBoxArchive.Checked;
        }

        private void ckBoxPreprocess_CheckedChanged(object sender, EventArgs e)
        {
            btnPreprocess.Enabled = ckBoxPreprocess.Checked;
        }

        private void btnPreprocess_Click(object sender, EventArgs e)
        {
            FrmModisPreprocess frmModisPreprocess =
                new FrmModisPreprocess(GetCheckedProducts());
            frmModisPreprocess.StartPosition = FormStartPosition.CenterScreen;
            frmModisPreprocess.ShowDialog();
        }

        private void tableLayoutPanel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            FrmEddySetting frmEddySetting = new FrmEddySetting();
            frmEddySetting.ShowDialog();
        }

        private void btnPreprocess_Click_1(object sender, EventArgs e)
        {
            var checkedProducts = GetCheckedProducts();
            FrmModisPreprocess frmModisPreprocess =
                new FrmModisPreprocess(checkedProducts);
            frmModisPreprocess.ShowDialog();
        }

    }
}

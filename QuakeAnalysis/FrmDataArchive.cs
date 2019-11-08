using QuakeAnalysis.Properties;
using System;
using System.Windows.Forms;
using QuakeAnalysis.Cfg;

namespace QuakeAnalysis
{
    public partial class FrmDataArchive : Form
    {
        public FrmDataArchive()
        {
            InitializeComponent();
        }

        public string DataFolder { get; set; }

        private void FrmDataArchive_Load(object sender, EventArgs e)
        {
            this.Font = Font.ChangeFontSize(GlobalModisMain.Config.FontSize);
            this.txtDataFolder.Text = GlobalModisMain.Config.DataDir;
        }

        private void btnOk_Click(object sender, EventArgs e)
        {
            if (txtDataFolder.Text.Trim() == string.Empty)
            {
                MessageBox.Show("数据目录不可为空！", Settings.Default.DT,
                    MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            this.DataFolder = txtDataFolder.Text.Trim();
            GlobalModisMain.Config.DataDir = txtDataFolder.Text.Trim();
            GlobalModisMain.Config.SaveToFile();
            this.DialogResult = DialogResult.OK;
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
        }

        private void btnFolder_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog folderBrowserDialog =
                new FolderBrowserDialog();
            folderBrowserDialog.SelectedPath = GlobalModisMain.Config.DataDir;
            if (folderBrowserDialog.ShowDialog() == DialogResult.OK)
            {
                txtDataFolder.Text = folderBrowserDialog.SelectedPath;
                GlobalModisMain.Config.DataDir = folderBrowserDialog.SelectedPath;
                GlobalModisMain.Config.SaveToFile();
            }
        }
    }
}

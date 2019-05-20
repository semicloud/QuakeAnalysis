using System.IO;
using System.Windows.Forms;
using NLog;
using QuakeAnalysis.Properties;

namespace QuakeAnalysis
{
    public partial class FrmModisArchive : Form
    {
        private static Logger g = LogManager.GetCurrentClassLogger();

        public FrmModisArchive()
        {
            InitializeComponent();
        }

        public string GetDataFolder()
        {
            if (!string.IsNullOrEmpty(txtDataFolder.Text))
            {
                g.Debug($"archive data folder:{txtDataFolder.Text.Trim()}");
                return txtDataFolder.Text.Trim();
            }

            g.Error("archive data directory is empty!");
            throw new DirectoryNotFoundException("directory is empty!");
        }

        private void btnBrowse_Click(object sender, System.EventArgs e)
        {
            var folderBrowserDialog = new FolderBrowserDialog();
            if (folderBrowserDialog.ShowDialog() == DialogResult.OK)
            {
                txtDataFolder.Text = folderBrowserDialog.SelectedPath;
                if (!txtDataFolder.Text.EndsWith(@"\"))
                    txtDataFolder.Text += @"\";
            }
        }

        private void FrmModisArchive_FormClosing(object sender, FormClosingEventArgs e)
        {
            //if (string.IsNullOrEmpty(txtDataFolder.Text.Trim()))
            //{
            //    MessageBox.Show("必须设置数据目录！", Settings.Default.DT, MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            //    e.Cancel = true;
            //}
        }

        private void btnOK_Click(object sender, System.EventArgs e)
        {
            if (string.IsNullOrEmpty(txtDataFolder.Text.Trim()))
            {
                MessageBox.Show("必须设置数据目录！", Settings.Default.DT, MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return;
            }

            DialogResult = DialogResult.OK;
        }
      
    }
}

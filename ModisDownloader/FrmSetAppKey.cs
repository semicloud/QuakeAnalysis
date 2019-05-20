using System;
using System.Windows.Forms;
using ModisDownloader.Properties;

namespace ModisDownloader
{
    public partial class FrmSetAppKey : Form
    {
        public FrmSetAppKey()
        {
            InitializeComponent();
        }

        private void FrmSetAppKey_Load(object sender, EventArgs e)
        {
            this.Font = this.Font.ChangeFontSize(Global.Config.FontSize);
            this.txtAppKey.Text = Global.Config.AppKey;

            MaximizeBox = false;
        }

        private void btnSaveConfig_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(txtAppKey.Text.Trim()))
            {
                MessageBox.Show("AppKey不可为空！", Settings.Default.DialogTitle, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            Global.Config.AppKey = txtAppKey.Text.Trim();
            // 将配置保存到文件、重新加载配置
            Global.Config.SaveToFile();
            Global.Config.Reload();
            DialogResult = DialogResult.OK;
        }
    }
}

using QuakeAnalysis.Properties;
using System;
using System.Collections.Generic;
using System.Windows.Forms;
using QuakeAnalysis.Cfg;

namespace QuakeAnalysis
{
    public partial class FrmModisPreprocess : Form
    {
        private List<string> _checkedProducts;

        public FrmModisPreprocess(List<string> checkedProducts)
        {
            InitializeComponent();
            _checkedProducts = checkedProducts;
        }

        private void FrmModisPreprocess_Load(object sender, EventArgs e)
        {
            this.Font = this.Font.ChangeFontSize(GlobalModisMain.Config.FontSize);
            tabPage02.Enabled = false;
            tabPage04.Enabled = false;
            tabPage05.Enabled = false;
            tabPage11.Enabled = false;
            Settings settings = Settings.Default;
            //不选择的数据不显示预处理界面
            if (_checkedProducts.Contains(settings.MOD02) || _checkedProducts.Contains(settings.MYD02)) tabPage02.Enabled = true;
            if (_checkedProducts.Contains(settings.MOD04) || _checkedProducts.Contains(settings.MYD04)) tabPage04.Enabled = true;
            if (_checkedProducts.Contains(settings.MOD05) || _checkedProducts.Contains(settings.MYD05)) tabPage05.Enabled = true;
            if (_checkedProducts.Contains(settings.MOD11) || _checkedProducts.Contains(settings.MYD11)) tabPage11.Enabled = true;

            txt02Band.Text = GlobalModisMain.Config.Prep02Band.ToString();
            txt02KernelType.Text = GlobalModisMain.Config.Prep02MrtKernelType;
            txt02ProjType.Text = GlobalModisMain.Config.Prep02MrtProjType;
            txt02ProjArgs.Text = GlobalModisMain.Config.Prep02MrtProjArgs;
            txt02PixelSize.Text = GlobalModisMain.Config.Prep02MrtPixelSize.ToString();

            txt04ResampType.Text = GlobalModisMain.Config.Prep04ResampType;
            txt04ProjType.Text = GlobalModisMain.Config.Prep04ProjType;
            txt04ProjParameters.Text = GlobalModisMain.Config.Prep04ProjParameters;

            txt05ResampType.Text = GlobalModisMain.Config.Prep05ResampType;
            txt05ProjType.Text = GlobalModisMain.Config.Prep05ProjType;
            txt05ProjParameters.Text = GlobalModisMain.Config.Prep05ProjParameters;

        }

        private void btnOk_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show("保存预处理配置吗？", Settings.Default.DT, MessageBoxButtons.OKCancel,
                    MessageBoxIcon.Question) == DialogResult.OK)
            {
                GlobalModisMain.Config.Prep02Band = Convert.ToInt32(txt02Band.Text);
                GlobalModisMain.Config.Prep02MrtKernelType = txt02KernelType.Text;
                GlobalModisMain.Config.Prep02MrtProjType = txt02ProjType.Text;
                GlobalModisMain.Config.Prep02MrtProjArgs = txt02ProjArgs.Text;
                GlobalModisMain.Config.Prep02MrtPixelSize = Convert.ToDouble(txt02PixelSize.Text);

                GlobalModisMain.Config.Prep04ResampType = txt04ResampType.Text;
                GlobalModisMain.Config.Prep04ProjType = txt04ProjType.Text;
                GlobalModisMain.Config.Prep04ProjParameters = txt04ProjParameters.Text;

                GlobalModisMain.Config.Prep05ResampType = txt05ResampType.Text;
                GlobalModisMain.Config.Prep05ProjType = txt05ProjType.Text;
                GlobalModisMain.Config.Prep05ProjParameters = txt05ProjParameters.Text;

                GlobalModisMain.Config.SaveToFile();

                MessageBox.Show("预处理配置已保存！", Settings.Default.DT, MessageBoxButtons.OK, MessageBoxIcon.Information);

                Close();
            }
            else
            {
                Close();
            }
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}

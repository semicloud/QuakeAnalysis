using QuakeAnalysis.Cfg;
using QuakeAnalysis.Properties;
using System;
using System.Drawing;
using System.Windows.Forms;

namespace QuakeAnalysis
{
    public partial class FrmEddySetting : Form
    {
        public FrmEddySetting()
        {
            InitializeComponent();
        }

        private void FrmEddySetting_Load(object sender, EventArgs e)
        {
            this.Font = new Font(FontFamily.GenericSerif, GlobalModisMain.Config.FontSize);
            ckBoxEddyM1.Checked = GlobalModisMain.Config.UseEddyMode1;
            ckBoxEddyM2.Checked = GlobalModisMain.Config.UseEddyMode2;
            ckBoxEddyBg.Checked = GlobalModisMain.Config.CalcEddyBg;
            ckBoxEddyPlotBg.Checked = GlobalModisMain.Config.CalcEddyBg;
            ckBoxEddyPlotAno.Checked = GlobalModisMain.Config.IsPlotEddyAno;
            ckBoxEddyPlotCont.Checked = GlobalModisMain.Config.IsPlotEddyContour;
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            gboxPlottingCommon.Enabled = ckBoxEddyBg.Checked;
        }

        private void checkBox5_CheckedChanged(object sender, EventArgs e)
        {
        }

        private void checkBox3_CheckedChanged(object sender, EventArgs e)
        {
            gboxPlottingEddyField.Enabled = ckBoxEddyPlotAno.Checked;
        }

        private void checkBox2_CheckedChanged(object sender, EventArgs e)
        {
            gboxPlottingBackground.Enabled = ckBoxEddyPlotBg.Checked;
        }

        private void btnOk_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show("确定保存配置吗？", Settings.Default.DT, MessageBoxButtons.OKCancel, MessageBoxIcon.Question) ==
                DialogResult.OK)
            {
                GlobalModisMain.Config.UseEddyMode1 = ckBoxEddyM1.Checked;
                GlobalModisMain.Config.UseEddyMode2 = ckBoxEddyM2.Checked;
                GlobalModisMain.Config.CalcEddyBg = ckBoxEddyBg.Checked;
                GlobalModisMain.Config.CalcEddyBg = ckBoxEddyPlotBg.Checked;
                GlobalModisMain.Config.IsPlotEddyAno = ckBoxEddyPlotAno.Checked;
                GlobalModisMain.Config.IsPlotEddyContour = ckBoxEddyPlotCont.Checked;
                GlobalModisMain.Config.SaveToFile();
                if (MessageBox.Show("配置已保存！", Settings.Default.DT, MessageBoxButtons.OK, MessageBoxIcon.Information) ==
                    DialogResult.OK)
                {
                    this.DialogResult = DialogResult.OK;
                }
            }
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
        }
    }
}

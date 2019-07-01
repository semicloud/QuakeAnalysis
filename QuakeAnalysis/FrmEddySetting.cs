using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
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
            tabPage1.AutoScroll = false;
            tabPage1.HorizontalScroll.Enabled = false;
            tabPage1.HorizontalScroll.Visible = false;
            tabPage1.HorizontalScroll.Maximum = 0;
            tabPage1.AutoScroll = true;
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            gboxPlottingCommon.Enabled = checkBox1.Checked;
        }

        private void checkBox5_CheckedChanged(object sender, EventArgs e)
        {
            gboxContour.Enabled = checkBox5.Checked;
        }

        private void checkBox3_CheckedChanged(object sender, EventArgs e)
        {
            gboxPlottingEddyField.Enabled = checkBox3.Checked;
        }

        private void checkBox2_CheckedChanged(object sender, EventArgs e)
        {
            gboxPlottingBackground.Enabled = checkBox2.Checked;
        }
    }
}

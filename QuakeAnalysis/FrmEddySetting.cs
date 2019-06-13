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
    }
}

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using DevExpress.XtraEditors;

namespace QuakeAnalysis
{
    public partial class Form4 : XtraForm
    {
        public Form4()
        {
            InitializeComponent();
        }

        private void simpleButton1_Click(object sender, EventArgs e)
        {
            var form2 = new Form2();
            form2.StartPosition = FormStartPosition.CenterScreen;
            form2.Show();
        }

        private void simpleButton2_Click(object sender, EventArgs e)
        {
            var form3 = new Form3();
            form3.StartPosition = FormStartPosition.CenterScreen;
            form3.Show();
        }

        private void simpleButton3_Click(object sender, EventArgs e)
        {
            var form5 = new Form5 {StartPosition = FormStartPosition.CenterScreen};
            form5.Show();
        }

        private void Form4_Load(object sender, EventArgs e)
        {
           
        }
    }
}

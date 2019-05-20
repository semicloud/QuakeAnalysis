using System;
using System.Windows.Forms;
using DevExpress.XtraEditors;

namespace QuakeAnalysis
{
    /// <summary>
    /// 调用自定义反演程序
    /// </summary>
    public partial class Form5 : XtraForm
    {
        public Form5()
        {
            InitializeComponent();
        }

        private void simpleButton1_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.ShowDialog();
        }
    }
}

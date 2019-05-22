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
            //不选择的数据不显示预处理界面
            foreach (TabPage page in tabControl1.TabPages)
            {
                if (!_checkedProducts.Contains(page.Text))
                    tabControl1.TabPages.Remove(page);
            }
        }
    }
}

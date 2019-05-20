using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using DevExpress.XtraEditors;

namespace QuakeAnalysis
{
    /// <summary>
    /// 工具窗体的公共父类，工具窗体是指无法最大化的窗体
    /// </summary>
    public class ToolForm : XtraForm
    {
        public ToolForm()
        {
            FormBorderStyle = FormBorderStyle.FixedDialog;
            StartPosition = FormStartPosition.CenterParent;
            MaximizeBox = false;
        }
    }
}

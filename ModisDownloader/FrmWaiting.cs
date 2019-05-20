using System;
using System.Windows.Forms;

namespace ModisDownloader
{
    public partial class FrmWaiting : Form
    {
        public FrmWaiting()
        {
            InitializeComponent();
        }

        private void FrmWaiting_Load(object sender, EventArgs e)
        {
            MaximizeBox = false;
            MinimizeBox = false;
            this.Font = Font.ChangeFontSize(Global.Config.FontSize);
        }

        private void FrmWaiting_Activated(object sender, EventArgs e)
        {
            
        }

        private void FrmWaiting_Deactivate(object sender, EventArgs e)
        {
           
        }
    }
}

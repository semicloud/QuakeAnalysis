using System.Windows.Forms;

namespace ModisDownloader
{
    public partial class FrmProgress : Form
    {
        public FrmProgress()
        {
            InitializeComponent();
            ControlBox = false;
            MaximizeBox = false;
            StartPosition = FormStartPosition.CenterParent;
        }

        private void FrmProgress_Load(object sender, System.EventArgs e)
        {
            this.Font = Font.ChangeFontSize(Global.Config.FontSize);
        }
    }
}

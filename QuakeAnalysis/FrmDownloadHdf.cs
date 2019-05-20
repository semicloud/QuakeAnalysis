using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using DevExpress.XtraEditors;

namespace QuakeAnalysis
{
    public partial class FrmDownloadHdf : XtraForm
    {
        public FrmDownloadHdf()
        {
            InitializeComponent();
        }

        private void simpleButton1_Click(object sender, EventArgs e)
        {
            using (var client = new WebClient())
            {
                ServicePointManager.SecurityProtocol =
                    SecurityProtocolType.Tls12 | SecurityProtocolType.Tls | SecurityProtocolType.Tls11;
                // Header必须
                client.Headers.Add("User-Agent",
                    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.140 Safari/537.36 Edge/18.17763");
                client.Headers.Add("Authorization", "Bearer 85A5F00E-FEB0-11E8-BD8E-CCB570C49BBF");
                client.DownloadProgressChanged += (o, args) =>
                {
                    //Console.WriteLine(args.ProgressPercentage);
                    progressBarControl1.EditValue = args.ProgressPercentage;
                };
                client.DownloadFileCompleted += (o, args) => { XtraMessageBox.Show("下载完鸟。"); };
                client.DownloadFileAsync(new Uri(textEdit1.EditValue.ToString()), @"D:\ddd.hdf");
            }
        }
    }
}

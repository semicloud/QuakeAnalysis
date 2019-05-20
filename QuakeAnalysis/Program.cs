using System;
using System.Globalization;
using System.Threading;
using System.Windows.Forms;
using DevExpress.Skins;
using NLog;

namespace QuakeAnalysis
{
    static class Program
    {
        private static Logger g = LogManager.GetCurrentClassLogger();

        /// <summary>
        /// 应用程序的主入口点。
        /// </summary>
        [STAThread]
        static void Main()
        {
            // 新版本的DevExpress将汉化文件夹改为了zh-Hans
            Thread.CurrentThread.CurrentUICulture = new CultureInfo("zh-Hans");
            Thread.CurrentThread.CurrentCulture = new CultureInfo("zh-Hans");
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            DevExpress.UserSkins.BonusSkins.Register();
            SkinManager.EnableFormSkins();
            SkinManager.EnableMdiFormSkins();


            //var data = new float[] { 5.56f, 9.58f };
            //var ans = new float[2];
            //Rad2Bt.rad2bt(2, data, 31, ans);
            //Console.WriteLine(string.Join(",", ans));

            //Application.Run(new FrmDownloadHdf() { StartPosition = FormStartPosition.CenterScreen });
            Application.Run(new FrmModisMain());
        }
    }
}

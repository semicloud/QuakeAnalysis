using System;
using System.Collections.Generic;
using System.Configuration;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net;
using System.Threading;
using System.Windows.Forms;
using ModisDownloader.Properties;
using Newtonsoft.Json;
using NLog;
using NLog.Windows.Forms;

namespace ModisDownloader
{
    public partial class FrmDownload : Form
    {
        private static Logger g = LogManager.GetCurrentClassLogger();
        private static Logger uiLogger = LogManager.GetLogger("ui");

        private CancellationTokenSource _cancellationTokenSource;

        /// <summary>
        /// 界面展示支持的字号
        /// </summary>
        private static readonly float[] _supportFontSize = { 9, 10, 11, 12, 13, 14, 15 };

        private FrmWaiting _frmWaiting;

        public FrmDownload()
        {
            InitializeComponent();

            Font = Font.ChangeFontSize(Global.Config.FontSize);

            RichTextBoxTarget.ReInitializeAllTextboxes(this);

            StartPosition = FormStartPosition.CenterScreen;

            dataGridView1.AutoGenerateColumns = true;
            dataGridView1.RowHeadersVisible = false;
            dataGridView1.SelectionMode = DataGridViewSelectionMode.FullRowSelect;
        }

        #region 窗体相关方法

        private void FrmDownload_Load(object sender, EventArgs e)
        {
            LoadUiState();

            //dateTimePicker1.Value = new DateTime(2018, 12, 18);
            //dateTimePicker2.Value = new DateTime(2018, 12, 20);
            //txtMinLng.Text = "113";
            //txtMaxLng.Text = "115";
            //txtMinLat.Text = "39";
            //txtMaxLat.Text = "42";
            //txtDownloadFolder.Text = @"C:\";

            if (string.IsNullOrEmpty(Global.Config.AppKey))
            {
                uiLogger.Fatal("软件未设置AppKey！");
            }

            // 这句代码触发后，立即触发SelectedIndexChanged的事件
            // 而此时的SelectedValue为9，所以会把配置文件中的字号改为9
            // 因此造成软件启动时，就会把字号改成9
            // 一种可能的修改方案是，在设置DataSource的时候不触发SelectedIndexChanged的事件
            // 因此，在绑定数据源后，再指定EventHandler
            cboxFontSize.DataSource = _supportFontSize;
            cboxFontSize.SelectedIndexChanged += cboxFontSize_SelectedIndexChanged;
            cboxFontSize.SelectedIndex = _supportFontSize.ToList().IndexOf(Global.Config.FontSize);
        }

        private void FrmDownload_FormClosed(object sender, FormClosedEventArgs e)
        {
            SaveUiState();
        }

        private void FrmDownload_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (MessageBox.Show("确定退出吗？", Settings.Default.DialogTitle, MessageBoxButtons.OKCancel,
                    MessageBoxIcon.Question) != DialogResult.OK)
                e.Cancel = true;
        }

        #endregion

        #region 其他控件相关方法

        private void cboxFontSize_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.Font = this.Font.ChangeFontSize(Convert.ToSingle(cboxFontSize.SelectedValue));
            Global.Config.FontSize = Convert.ToInt32(cboxFontSize.SelectedValue);
            Global.Config.SaveToFile();
            Global.Config.Reload();
        }

        #endregion

        #region 按钮Click事件

        private void btnCheckAllProducts_Click(object sender, EventArgs e)
        {
            Action<Control> setChecked = control =>
            {
                if (control is CheckBox checkBox)
                    checkBox.Checked = true;
            };
            groupBox1.Controls.Cast<Control>().ToList().ForEach(setChecked);
        }

        private void btnUncheckAllProducts_Click(object sender, EventArgs e)
        {
            Action<Control> setUnChecked = control =>
            {
                if (control is CheckBox checkBox)
                    checkBox.Checked = false;
            };
            groupBox1.Controls.Cast<Control>().ToList().ForEach(setUnChecked);
        }

        private void btnInvertCheckedProducts_Click(object sender, EventArgs e)
        {
            Action<Control> inverse = control =>
            {
                if (control is CheckBox checkBox)
                    checkBox.Checked = !checkBox.Checked;
            };
            groupBox1.Controls.Cast<Control>().ToList().ForEach(inverse);
        }

        private void btnBrowse_Click(object sender, EventArgs e)
        {
            var dialog = new FolderBrowserDialog();
            if (!string.IsNullOrEmpty(txtDownloadFolder.Text))
            {
                dialog.SelectedPath = txtDownloadFolder.Text.Trim();
            }

            if (dialog.ShowDialog() == DialogResult.OK)
            {
                g.Debug($"user select path: [{dialog.SelectedPath}]");
                if (!dialog.SelectedPath.EndsWith(@"\"))
                {
                    txtDownloadFolder.Text = dialog.SelectedPath + @"\";
                    return;
                }

                txtDownloadFolder.Text = dialog.SelectedPath;
            }
        }

        /// <summary>
        /// 取消按钮事件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnCancel_Click(object sender, EventArgs e)
        {
            Close();
        }

        /// <summary>
        /// 查询按钮事件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private async void btnQuery_Click(object sender, EventArgs e)
        {
            var checkedProductNames = GetCheckedProductNames();
            var checkedDnbs = GetCheckedDnbType();

            #region 防御代码、解析经纬度

            if (string.IsNullOrEmpty(Global.Config.AppKey))
            {
                MessageBox.Show("未设置AppKey！", Settings.Default.DialogTitle, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            if (!checkedProductNames.Any())
            {
                MessageBox.Show("请选择产品！", Settings.Default.DialogTitle, MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return;
            }

            if (string.IsNullOrEmpty(checkedDnbs))
            {
                MessageBox.Show("请选择DNB！", Settings.Default.DialogTitle, MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return;
            }

            if (dateTimePicker1.Value > dateTimePicker2.Value)
            {
                MessageBox.Show("开始日期不能大于结束日期！", Settings.Default.DialogTitle, MessageBoxButtons.OK,
                    MessageBoxIcon.Exclamation);
                return;
            }

            if ((dateTimePicker2.Value - dateTimePicker1.Value).Days > 365)
            {
                MessageBox.Show("开始日期与结束日期相距不可超过1年！", Settings.Default.DialogTitle, MessageBoxButtons.OK,
                    MessageBoxIcon.Exclamation);
                return;
            }

            double minLng, maxLng, minLat, maxLat;

            if (!double.TryParse(txtMinLng.Text.Trim(), out minLng))
            {
                MessageBox.Show("最小经度格式错误！", Settings.Default.DialogTitle, MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                txtMinLng.Focus();
                return;
            }

            if (minLng < -180.0 || minLng > 180.0)
            {
                MessageBox.Show("最小经度范围错误！有效经度范围(-180,180)!", Settings.Default.DialogTitle, MessageBoxButtons.OK,
                    MessageBoxIcon.Exclamation);
                txtMinLng.Focus();
                return;
            }

            if (!double.TryParse(txtMaxLng.Text.Trim(), out maxLng))
            {
                MessageBox.Show("最大经度格式错误！", Settings.Default.DialogTitle, MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                txtMaxLng.Focus();
                return;
            }

            if (maxLng < -180.0 || maxLng > 180.0)
            {
                MessageBox.Show("最大经度范围错误！有效经度范围(-180,180)!", Settings.Default.DialogTitle, MessageBoxButtons.OK,
                    MessageBoxIcon.Exclamation);
                txtMaxLng.Focus();
                return;
            }

            if (!double.TryParse(txtMinLat.Text.Trim(), out minLat))
            {
                MessageBox.Show("最小纬度格式错误！", Settings.Default.DialogTitle, MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                txtMinLat.Focus();
                return;
            }

            if (minLat < -90.0 || minLat > 90.0)
            {
                MessageBox.Show("最小纬度范围错误！有效纬度范围(-90,90)!", Settings.Default.DialogTitle, MessageBoxButtons.OK,
                    MessageBoxIcon.Exclamation);
                txtMinLat.Focus();
                return;
            }

            if (!double.TryParse(txtMaxLat.Text.Trim(), out maxLat))
            {
                MessageBox.Show("最大纬度格式错误！", Settings.Default.DialogTitle, MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                txtMaxLat.Focus();
                return;
            }

            if (maxLat < -90.0 || maxLat > 90.0)
            {
                MessageBox.Show("最大纬度范围错误！有效纬度范围(-90,90)!", Settings.Default.DialogTitle, MessageBoxButtons.OK,
                    MessageBoxIcon.Exclamation);
                txtMaxLat.Focus();
                return;
            }

            if (string.IsNullOrEmpty(txtDownloadFolder.Text.Trim()))
            {
                MessageBox.Show("请选择下载文件夹！", Settings.Default.DialogTitle, MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                btnBrowse.Focus();
                return;
            }

            #endregion

            DisableControlsDuringQuery();

            var query = new Query
            {
                DateStart = dateTimePicker1.Value,
                DateStop = dateTimePicker2.Value,
                DnbType = checkedDnbs,
                MinLat = minLat,
                MaxLat = maxLat,
                MinLng = minLng,
                MaxLng = maxLng,
                ProductNames = checkedProductNames
            };

            var urls = ServiceUtil.GetFileIdServiceUrls(query);

            //var frmProgress = new FrmProgress { StartPosition = FormStartPosition.CenterParent };
            //frmProgress.label1.Text = "正在查询文件名...";
            //BeginInvoke(new Action(() => frmProgress.ShowDialog()));

            var progress = new Progress<int>();
            progress.ProgressChanged += (o, i) => { progressBar1.Value = i; };

            List<string> allFileIdList;

            try
            {
                allFileIdList = await ServiceUtil.GetFileIdsAsync(urls, progress);
            }
            catch (Exception exception)
            {
                uiLogger.Error("网络异常：" + exception.Message + "请检查网络后重新查询！");
                EnableControlsAfterQuery();
                return;
            }

            //long tmp = 0;
            //var legalFileNames = allFileNames.FindAll(a => long.TryParse(a, out tmp));
            //var unLegalFileNames = allFileNames.FindAll(a => !long.TryParse(a, out tmp));

            //g.Debug("all file name count: [{0}], legal file name count: [{1}]", allFileNames.Count,
            //    legalFileNames.Count);
            //if (unLegalFileNames.Any())
            //{
            //    uiLogger.Warn("没有查询到相关文件" + string.Join(", ", unLegalFileNames));
            //}

            uiLogger.Info("共查询到{0}个文件，正在获取文件属性数据..", allFileIdList.Count);
            //frmProgress.label1.Text = string.Format("共查询到{0}个文件，正在获取文件属性数据..", legalFileNames.Count);

            var fileProperties = await ServiceUtil.GetFilePropertiesAsync(allFileIdList, progress);
            Console.WriteLine(fileProperties.Count);

            uiLogger.Info("获取文件属性数据完毕，正在获取文件下载地址..", allFileIdList.Count);
            //frmProgress.label1.Text = string.Format("获取文件属性数据完毕，正在获取文件下载地址..", allFileNames.Count);

            var fileUrls = await ServiceUtil.GetFileUrlsAsync(allFileIdList, progress);
            uiLogger.Info("待下载数据准备完毕..");
            //frmProgress.label1.Text = "完成..";

            var downloadFiles = DownloadFile.LoadDownloadFiles(fileProperties, fileUrls);

            //frmProgress.Close();

            BindDataGridview(downloadFiles);

            var totalSizeGb = Math.Round(downloadFiles.Sum(d => d.SizeMb) / 1024, 2);

            uiLogger.Info($"共查询到{downloadFiles.Count}个数据集，合计大小约{totalSizeGb}GB。");

            EnableControlsAfterQuery();

            MessageBox.Show($@"共查询到{downloadFiles.Count}个数据集，合计大小约{totalSizeGb}GB。",
                Settings.Default.DialogTitle, MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        /// <summary>
        /// 下载按钮
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private async void btnDownload_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(Global.Config.AppKey))
            {
                MessageBox.Show("未设置AppKey！", Settings.Default.DialogTitle, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            if (dataGridView1.DataSource == null)
            {
                MessageBox.Show("没有要下载的文件！", Settings.Default.DialogTitle, MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return;
            }

            var downloadFiles = (List<DownloadFile>)dataGridView1.DataSource;
            var totalSizeGb = Math.Round(downloadFiles.Sum(d => d.SizeMb) / 1024, 2);

            if (MessageBox.Show($@"文件个数：{downloadFiles.Count}，大小：约{totalSizeGb}GB，确定下载吗？",
                    Settings.Default.DialogTitle, MessageBoxButtons.OKCancel, MessageBoxIcon.Question) ==
                DialogResult.Cancel)
                return;

            DisableControlsDuringDownload();

            // 当前下载文件索引报告
            var currentDownloadFileIndex = new Progress<int>();
            currentDownloadFileIndex.ProgressChanged += (o, i) =>
            {
                lblIndex.Text = $@"({i + 1}/{downloadFiles.Count})正在下载{downloadFiles[i].Name}..";
            };

            // 当前下载文件进度报告
            var currentDownloadFilePercentage = new Progress<int>();
            currentDownloadFilePercentage.ProgressChanged += (o, i) => { progressBar1.Value = i; };

            // 当前文件下载速度报告
            var currentDownloadSpeed = new Progress<string>();
            currentDownloadSpeed.ProgressChanged += (o, i) => { lblSpeed.Text = i; };

            // 取消标志
            _cancellationTokenSource = new CancellationTokenSource();

            uiLogger.Info("共需下载{0}个文件，开始下载..", downloadFiles.Count);

            await ServiceUtil.DownloadFileTaskAsync(downloadFiles, txtDownloadFolder.Text, currentDownloadFileIndex,
                currentDownloadFilePercentage, currentDownloadSpeed, _cancellationTokenSource.Token);

            if (_cancellationTokenSource.IsCancellationRequested)
            {
                _frmWaiting?.Close();
                uiLogger.Info("用户停止下载...");
                MessageBox.Show("已停止下载..", Settings.Default.DialogTitle, MessageBoxButtons.OK,
                    MessageBoxIcon.Information);
                _cancellationTokenSource = null;
                EnableControlsAfterDownload();
                WriteLogFileToDestFolder();
                return;
            }

            EnableControlsAfterDownload();
            uiLogger.Info("下载完成..");

            WriteLogFileToDestFolder();

            MessageBox.Show("下载完成！", Settings.Default.DialogTitle, MessageBoxButtons.OK, MessageBoxIcon.Information);


            dataGridView1.DataSource = null;
            _cancellationTokenSource = null;

        }

        private void btnCancelDownload_Click(object sender, EventArgs e)
        {
            if (dataGridView1.DataSource == null || _cancellationTokenSource == null)
            {
                MessageBox.Show("当前无下载任务！", Settings.Default.DialogTitle, MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return;
            }

            if (MessageBox.Show("确定停止下载吗？", Settings.Default.DialogTitle, MessageBoxButtons.OKCancel,
                    MessageBoxIcon.Question) == DialogResult.Cancel)
                return;


            _cancellationTokenSource?.Cancel();

            _frmWaiting = new FrmWaiting();
            _frmWaiting.StartPosition = FormStartPosition.CenterParent;
            _frmWaiting.label1.Text = "正在停止，请稍候..";
            BeginInvoke(new Action(() => _frmWaiting.ShowDialog()));

            //_frmWaiting = new FrmWaiting();
            //_frmWaiting.StartPosition = FormStartPosition.Manual;
            //_frmWaiting.label1.Text = "正在停止下载，请稍候..";
            //_frmWaiting.Location = new Point(Location.X + (Width - _frmWaiting.Width) / 2,
            //    Location.Y + (Height - _frmWaiting.Height) / 2);
            //_frmWaiting.Show(this);
        }

        private void btnClearLog_Click(object sender, EventArgs e)
        {
            richTextBox1.Clear();
        }

        private void btnSetAppKey_Click(object sender, EventArgs e)
        {
            var frmSetAppKey = new FrmSetAppKey { StartPosition = FormStartPosition.CenterParent };
            if (frmSetAppKey.ShowDialog() == DialogResult.OK)
            {
                MessageBox.Show("设置已保存！", Settings.Default.DialogTitle, MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        #region 私有方法

        private List<string> GetCheckedProductNames()
        {
            var checkedProductNames = new List<string>();
            Action<Control> getCheckedText = control =>
            {
                if (control is CheckBox checkBox && checkBox.Checked)
                {
                    checkedProductNames.Add(checkBox.Text);
                }
            };
            groupBox1.Controls.Cast<Control>().ToList().ForEach(getCheckedText);
            g.Debug("Checked product names: {0}", string.Join(", ", checkedProductNames));
            return checkedProductNames;
        }

        private string GetCheckedDnbType()
        {
            var checkedDnbTypes = new List<string>();
            Action<Control> getCheckedTag = control =>
            {
                if (control is CheckBox checkBox && checkBox.Checked)
                {
                    checkedDnbTypes.Add(checkBox.Tag.ToString());
                }
            };
            groupBox4.Controls.Cast<Control>().ToList().ForEach(getCheckedTag);
            var ans = string.Join("", checkedDnbTypes);
            g.Debug("Checked Dnb type: " + ans);
            return ans;
        }

        private void BindDataGridview(List<DownloadFile> downloadFiles)
        {
            dataGridView1.DataSource = null;
            dataGridView1.DataSource = downloadFiles;
            foreach (DataGridViewColumn column in dataGridView1.Columns)
            {
                column.SortMode = DataGridViewColumnSortMode.NotSortable;
                column.ReadOnly = true;
            }
            //dataGridView1.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.Fill;
            SetDatagridViewColumn("Name", 0, DataGridViewAutoSizeColumnMode.Fill, 70);
            SetDatagridViewColumn("SizeMb", 1, DataGridViewAutoSizeColumnMode.Fill, 15);
            SetDatagridViewColumn("CheckSum", 2, DataGridViewAutoSizeColumnMode.Fill, 15);
        }

        private void SetDatagridViewColumn(string colName, int dspIndex, DataGridViewAutoSizeColumnMode mode, int fillWeight)
        {
            if (dataGridView1.Columns[colName] != null)
            {
                dataGridView1.Columns[colName].DisplayIndex = dspIndex;
                dataGridView1.Columns[colName].AutoSizeMode = mode;
                dataGridView1.Columns[colName].FillWeight = fillWeight;
            }
        }

        private void DisableControlsDuringDownload()
        {
            //下载期间禁用查询、取消、浏览按钮，禁止切换Page
            btnQuery.Enabled = false;
            btnCancel.Enabled = false;
            btnBrowse.Enabled = false;
            btnDownload.Enabled = false;
            btnSaveUrl.Enabled = false;
            ControlBox = false;
        }

        private void EnableControlsAfterDownload()
        {
            btnQuery.Enabled = true;
            btnCancel.Enabled = true;
            btnBrowse.Enabled = true;
            btnDownload.Enabled = true;
            btnSaveUrl.Enabled = true;
            ControlBox = true;
        }

        private void DisableControlsDuringQuery()
        {
            groupBox1.Enabled = false;
            groupBox2.Enabled = false;
            groupBox3.Enabled = false;
            groupBox4.Enabled = false;
            btnDownload.Enabled = false;
            btnCancelDownload.Enabled = false;
            btnCancel.Enabled = false;
            btnQuery.Enabled = false;
            btnSaveUrl.Enabled = false;
            ControlBox = false;
        }

        private void EnableControlsAfterQuery()
        {
            groupBox1.Enabled = true;
            groupBox2.Enabled = true;
            groupBox3.Enabled = true;
            groupBox4.Enabled = true;
            btnDownload.Enabled = true;
            btnCancelDownload.Enabled = true;
            btnCancel.Enabled = true;
            btnSaveUrl.Enabled = true;
            btnQuery.Enabled = true;
            ControlBox = true;
        }

        /// <summary>
        /// 保存下载日志
        /// </summary>
        private void WriteLogFileToDestFolder()
        {
            File.WriteAllText(txtDownloadFolder.Text + $"{DateTime.Now.ToString("yyyy_MM_dd_hh_mm_ss")}.log",
                richTextBox1.Text);
        }

        private void SaveUiState()
        {
            Settings.Default.MOD02Checked = checkBox1.Checked;
            Settings.Default.MYD02Checked = checkBox12.Checked;
            Settings.Default.MOD03Checked = checkBox2.Checked;
            Settings.Default.MYD03Checked = checkBox11.Checked;
            Settings.Default.MOD04Checked = checkBox3.Checked;
            Settings.Default.MYD04Checked = checkBox10.Checked;
            Settings.Default.MOD05Checked = checkBox4.Checked;
            Settings.Default.MYD05Checked = checkBox9.Checked;
            Settings.Default.MOD11Checked = checkBox5.Checked;
            Settings.Default.MYD11Checked = checkBox8.Checked;
            Settings.Default.MOD35Checked = checkBox6.Checked;
            Settings.Default.MYD35Checked = checkBox7.Checked;

            Settings.Default.StartDate = dateTimePicker1.Value;
            Settings.Default.StopDate = dateTimePicker2.Value;

            Settings.Default.MinLng = txtMinLng.Text.Trim();
            Settings.Default.MaxLng = txtMaxLng.Text.Trim();
            Settings.Default.MinLat = txtMinLat.Text.Trim();
            Settings.Default.MaxLat = txtMaxLat.Text.Trim();

            Settings.Default.DayChecked = checkBox15.Checked;
            Settings.Default.NightChecked = checkBox14.Checked;
            Settings.Default.DayNightBoundaryChecked = checkBox13.Checked;

            Settings.Default.DownloadFolder = txtDownloadFolder.Text;

            Settings.Default.Save();
            g.Debug("save ui state success..");
        }

        private void LoadUiState()
        {
            checkBox1.Checked = Settings.Default.MOD02Checked;
            checkBox12.Checked = Settings.Default.MYD02Checked;
            checkBox2.Checked = Settings.Default.MOD03Checked;
            checkBox11.Checked = Settings.Default.MYD03Checked;
            checkBox3.Checked = Settings.Default.MOD04Checked;
            checkBox10.Checked = Settings.Default.MYD04Checked;
            checkBox4.Checked = Settings.Default.MOD05Checked;
            checkBox9.Checked = Settings.Default.MYD05Checked;
            checkBox5.Checked = Settings.Default.MOD11Checked;
            checkBox8.Checked = Settings.Default.MYD11Checked;
            checkBox6.Checked = Settings.Default.MOD35Checked;
            checkBox7.Checked = Settings.Default.MYD35Checked;
            dateTimePicker1.Value = Settings.Default.StartDate == DateTime.MinValue
                ? DateTime.Now
                : Settings.Default.StartDate;
            dateTimePicker2.Value = Settings.Default.StopDate == DateTime.MinValue
                ? DateTime.Now
                : Settings.Default.StopDate;
            txtMinLng.Text = Settings.Default.MinLng;
            txtMaxLng.Text = Settings.Default.MaxLng;
            txtMinLat.Text = Settings.Default.MinLat;
            txtMaxLat.Text = Settings.Default.MaxLat;

            checkBox15.Checked = Settings.Default.DayChecked;
            checkBox14.Checked = Settings.Default.NightChecked;
            checkBox13.Checked = Settings.Default.DayNightBoundaryChecked;
            txtDownloadFolder.Text = Settings.Default.DownloadFolder;

            g.Debug("load ui state success..");

        }

        #endregion

        #endregion

        /// <summary>
        /// 保存地址按钮
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnSaveUrl_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(Global.Config.AppKey))
            {
                MessageBox.Show("未设置AppKey！", Settings.Default.DialogTitle, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            if (dataGridView1.DataSource == null)
            {
                MessageBox.Show("没有要下载的文件！", Settings.Default.DialogTitle, MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return;
            }

            var saveFileDialog = new SaveFileDialog
            {
                Filter = "文本文件(*.txt)|*.txt",
                RestoreDirectory = true,
                FileName = "MODIS_Download_URL_" + DateTime.Now.ToString("yyyyMMddHHmmss") + ".txt"
            };
            if (saveFileDialog.ShowDialog() == DialogResult.OK)
            {
                var txtPath = saveFileDialog.FileName;
                var urls = (dataGridView1.DataSource as List<DownloadFile>).Select(d => d.Url).ToList();
                File.WriteAllLines(txtPath, urls);
                MessageBox.Show("下载路径已保存至" + txtPath + "。", Settings.Default.DialogTitle, MessageBoxButtons.OK,
                    MessageBoxIcon.Information);
            }
        }

        private void FrmDownload_Resize(object sender, EventArgs e)
        {
            if (WindowState == FormWindowState.Minimized)
            {
                Hide();
                ShowInTaskbar = false;
                notifyIcon1.Visible = true;
            }
        }

        private void notifyIcon1_DoubleClick(object sender, EventArgs e)
        {
            if (WindowState == FormWindowState.Minimized)
            {
                Show();
                WindowState = FormWindowState.Normal;
                notifyIcon1.Visible = false;
                ShowInTaskbar = true;
            }
        }
    }
}

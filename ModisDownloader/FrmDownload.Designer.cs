namespace ModisDownloader
{
    sealed partial class FrmDownload
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle2 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle3 = new System.Windows.Forms.DataGridViewCellStyle();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FrmDownload));
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.btnInvertCheckedProducts = new System.Windows.Forms.Button();
            this.btnUncheckAllProducts = new System.Windows.Forms.Button();
            this.btnCheckAllProducts = new System.Windows.Forms.Button();
            this.checkBox7 = new System.Windows.Forms.CheckBox();
            this.checkBox8 = new System.Windows.Forms.CheckBox();
            this.checkBox9 = new System.Windows.Forms.CheckBox();
            this.checkBox10 = new System.Windows.Forms.CheckBox();
            this.checkBox11 = new System.Windows.Forms.CheckBox();
            this.checkBox12 = new System.Windows.Forms.CheckBox();
            this.checkBox6 = new System.Windows.Forms.CheckBox();
            this.checkBox5 = new System.Windows.Forms.CheckBox();
            this.checkBox4 = new System.Windows.Forms.CheckBox();
            this.checkBox3 = new System.Windows.Forms.CheckBox();
            this.checkBox2 = new System.Windows.Forms.CheckBox();
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.dateTimePicker2 = new System.Windows.Forms.DateTimePicker();
            this.label2 = new System.Windows.Forms.Label();
            this.dateTimePicker1 = new System.Windows.Forms.DateTimePicker();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.label7 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.txtMaxLng = new System.Windows.Forms.TextBox();
            this.txtMinLng = new System.Windows.Forms.TextBox();
            this.txtMinLat = new System.Windows.Forms.TextBox();
            this.txtMaxLat = new System.Windows.Forms.TextBox();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.checkBox13 = new System.Windows.Forms.CheckBox();
            this.checkBox14 = new System.Windows.Forms.CheckBox();
            this.checkBox15 = new System.Windows.Forms.CheckBox();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.btnBrowse = new System.Windows.Forms.Button();
            this.txtDownloadFolder = new System.Windows.Forms.TextBox();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.groupBox7 = new System.Windows.Forms.GroupBox();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.progressBar1 = new System.Windows.Forms.ProgressBar();
            this.panel1 = new System.Windows.Forms.Panel();
            this.lblSpeed = new System.Windows.Forms.Label();
            this.lblIndex = new System.Windows.Forms.Label();
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            this.btnSetAppKey = new System.Windows.Forms.Button();
            this.btnQuery = new System.Windows.Forms.Button();
            this.btnDownload = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            this.btnCancelDownload = new System.Windows.Forms.Button();
            this.btnClearLog = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.cboxFontSize = new System.Windows.Forms.ComboBox();
            this.btnSaveUrl = new System.Windows.Forms.Button();
            this.notifyIcon1 = new System.Windows.Forms.NotifyIcon();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.groupBox5.SuspendLayout();
            this.groupBox6.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            this.groupBox7.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.btnInvertCheckedProducts);
            this.groupBox1.Controls.Add(this.btnUncheckAllProducts);
            this.groupBox1.Controls.Add(this.btnCheckAllProducts);
            this.groupBox1.Controls.Add(this.checkBox7);
            this.groupBox1.Controls.Add(this.checkBox8);
            this.groupBox1.Controls.Add(this.checkBox9);
            this.groupBox1.Controls.Add(this.checkBox10);
            this.groupBox1.Controls.Add(this.checkBox11);
            this.groupBox1.Controls.Add(this.checkBox12);
            this.groupBox1.Controls.Add(this.checkBox6);
            this.groupBox1.Controls.Add(this.checkBox5);
            this.groupBox1.Controls.Add(this.checkBox4);
            this.groupBox1.Controls.Add(this.checkBox3);
            this.groupBox1.Controls.Add(this.checkBox2);
            this.groupBox1.Controls.Add(this.checkBox1);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(435, 86);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "选择产品";
            // 
            // btnInvertCheckedProducts
            // 
            this.btnInvertCheckedProducts.Location = new System.Drawing.Point(144, 59);
            this.btnInvertCheckedProducts.Name = "btnInvertCheckedProducts";
            this.btnInvertCheckedProducts.Size = new System.Drawing.Size(64, 20);
            this.btnInvertCheckedProducts.TabIndex = 14;
            this.btnInvertCheckedProducts.Text = "反选";
            this.btnInvertCheckedProducts.UseVisualStyleBackColor = true;
            this.btnInvertCheckedProducts.Click += new System.EventHandler(this.btnInvertCheckedProducts_Click);
            // 
            // btnUncheckAllProducts
            // 
            this.btnUncheckAllProducts.Location = new System.Drawing.Point(75, 59);
            this.btnUncheckAllProducts.Name = "btnUncheckAllProducts";
            this.btnUncheckAllProducts.Size = new System.Drawing.Size(64, 20);
            this.btnUncheckAllProducts.TabIndex = 13;
            this.btnUncheckAllProducts.Text = "全不选";
            this.btnUncheckAllProducts.UseVisualStyleBackColor = true;
            this.btnUncheckAllProducts.Click += new System.EventHandler(this.btnUncheckAllProducts_Click);
            // 
            // btnCheckAllProducts
            // 
            this.btnCheckAllProducts.Location = new System.Drawing.Point(5, 59);
            this.btnCheckAllProducts.Name = "btnCheckAllProducts";
            this.btnCheckAllProducts.Size = new System.Drawing.Size(64, 20);
            this.btnCheckAllProducts.TabIndex = 12;
            this.btnCheckAllProducts.Text = "全选";
            this.btnCheckAllProducts.UseVisualStyleBackColor = true;
            this.btnCheckAllProducts.Click += new System.EventHandler(this.btnCheckAllProducts_Click);
            // 
            // checkBox7
            // 
            this.checkBox7.AutoSize = true;
            this.checkBox7.Location = new System.Drawing.Point(362, 39);
            this.checkBox7.Name = "checkBox7";
            this.checkBox7.Size = new System.Drawing.Size(72, 16);
            this.checkBox7.TabIndex = 11;
            this.checkBox7.Text = "MYD35_L2";
            this.checkBox7.UseVisualStyleBackColor = true;
            // 
            // checkBox8
            // 
            this.checkBox8.AutoSize = true;
            this.checkBox8.Location = new System.Drawing.Point(300, 39);
            this.checkBox8.Name = "checkBox8";
            this.checkBox8.Size = new System.Drawing.Size(66, 16);
            this.checkBox8.TabIndex = 10;
            this.checkBox8.Text = "MYD11A1";
            this.checkBox8.UseVisualStyleBackColor = true;
            // 
            // checkBox9
            // 
            this.checkBox9.AutoSize = true;
            this.checkBox9.Location = new System.Drawing.Point(221, 39);
            this.checkBox9.Name = "checkBox9";
            this.checkBox9.Size = new System.Drawing.Size(72, 16);
            this.checkBox9.TabIndex = 9;
            this.checkBox9.Text = "MYD05_L2";
            this.checkBox9.UseVisualStyleBackColor = true;
            // 
            // checkBox10
            // 
            this.checkBox10.AutoSize = true;
            this.checkBox10.Location = new System.Drawing.Point(141, 39);
            this.checkBox10.Name = "checkBox10";
            this.checkBox10.Size = new System.Drawing.Size(72, 16);
            this.checkBox10.TabIndex = 8;
            this.checkBox10.Text = "MYD04_3K";
            this.checkBox10.UseVisualStyleBackColor = true;
            // 
            // checkBox11
            // 
            this.checkBox11.AutoSize = true;
            this.checkBox11.Location = new System.Drawing.Point(80, 39);
            this.checkBox11.Name = "checkBox11";
            this.checkBox11.Size = new System.Drawing.Size(54, 16);
            this.checkBox11.TabIndex = 7;
            this.checkBox11.Text = "MYD03";
            this.checkBox11.UseVisualStyleBackColor = true;
            // 
            // checkBox12
            // 
            this.checkBox12.AutoSize = true;
            this.checkBox12.Location = new System.Drawing.Point(5, 39);
            this.checkBox12.Name = "checkBox12";
            this.checkBox12.Size = new System.Drawing.Size(72, 16);
            this.checkBox12.TabIndex = 6;
            this.checkBox12.Text = "MYD021KM";
            this.checkBox12.UseVisualStyleBackColor = true;
            // 
            // checkBox6
            // 
            this.checkBox6.AutoSize = true;
            this.checkBox6.Location = new System.Drawing.Point(362, 18);
            this.checkBox6.Name = "checkBox6";
            this.checkBox6.Size = new System.Drawing.Size(72, 16);
            this.checkBox6.TabIndex = 5;
            this.checkBox6.Text = "MOD35_L2";
            this.checkBox6.UseVisualStyleBackColor = true;
            // 
            // checkBox5
            // 
            this.checkBox5.AutoSize = true;
            this.checkBox5.Location = new System.Drawing.Point(300, 18);
            this.checkBox5.Name = "checkBox5";
            this.checkBox5.Size = new System.Drawing.Size(66, 16);
            this.checkBox5.TabIndex = 4;
            this.checkBox5.Text = "MOD11A1";
            this.checkBox5.UseVisualStyleBackColor = true;
            // 
            // checkBox4
            // 
            this.checkBox4.AutoSize = true;
            this.checkBox4.Location = new System.Drawing.Point(221, 18);
            this.checkBox4.Name = "checkBox4";
            this.checkBox4.Size = new System.Drawing.Size(72, 16);
            this.checkBox4.TabIndex = 3;
            this.checkBox4.Text = "MOD05_L2";
            this.checkBox4.UseVisualStyleBackColor = true;
            // 
            // checkBox3
            // 
            this.checkBox3.AutoSize = true;
            this.checkBox3.Location = new System.Drawing.Point(141, 18);
            this.checkBox3.Name = "checkBox3";
            this.checkBox3.Size = new System.Drawing.Size(72, 16);
            this.checkBox3.TabIndex = 2;
            this.checkBox3.Text = "MOD04_3K";
            this.checkBox3.UseVisualStyleBackColor = true;
            // 
            // checkBox2
            // 
            this.checkBox2.AutoSize = true;
            this.checkBox2.Location = new System.Drawing.Point(80, 18);
            this.checkBox2.Name = "checkBox2";
            this.checkBox2.Size = new System.Drawing.Size(54, 16);
            this.checkBox2.TabIndex = 1;
            this.checkBox2.Text = "MOD03";
            this.checkBox2.UseVisualStyleBackColor = true;
            // 
            // checkBox1
            // 
            this.checkBox1.AutoSize = true;
            this.checkBox1.Location = new System.Drawing.Point(5, 18);
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.Size = new System.Drawing.Size(72, 16);
            this.checkBox1.TabIndex = 0;
            this.checkBox1.Text = "MOD021KM";
            this.checkBox1.UseVisualStyleBackColor = true;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.dateTimePicker2);
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.Controls.Add(this.dateTimePicker1);
            this.groupBox2.Controls.Add(this.label1);
            this.groupBox2.Location = new System.Drawing.Point(12, 104);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(435, 59);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "日期范围";
            // 
            // dateTimePicker2
            // 
            this.dateTimePicker2.Location = new System.Drawing.Point(276, 23);
            this.dateTimePicker2.Name = "dateTimePicker2";
            this.dateTimePicker2.Size = new System.Drawing.Size(144, 21);
            this.dateTimePicker2.TabIndex = 3;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(219, 27);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(65, 12);
            this.label2.TabIndex = 2;
            this.label2.Text = "结束日期：";
            // 
            // dateTimePicker1
            // 
            this.dateTimePicker1.Location = new System.Drawing.Point(64, 23);
            this.dateTimePicker1.Name = "dateTimePicker1";
            this.dateTimePicker1.Size = new System.Drawing.Size(144, 21);
            this.dateTimePicker1.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(5, 27);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(65, 12);
            this.label1.TabIndex = 0;
            this.label1.Text = "开始日期：";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.label7);
            this.groupBox3.Controls.Add(this.label6);
            this.groupBox3.Controls.Add(this.label5);
            this.groupBox3.Controls.Add(this.label4);
            this.groupBox3.Controls.Add(this.txtMaxLng);
            this.groupBox3.Controls.Add(this.txtMinLng);
            this.groupBox3.Controls.Add(this.txtMinLat);
            this.groupBox3.Controls.Add(this.txtMaxLat);
            this.groupBox3.Location = new System.Drawing.Point(11, 169);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(435, 109);
            this.groupBox3.TabIndex = 2;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "地理范围";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(178, 86);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(53, 12);
            this.label7.TabIndex = 7;
            this.label7.Text = "最小纬度";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(45, 48);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(53, 12);
            this.label6.TabIndex = 6;
            this.label6.Text = "最小经度";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(178, 13);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(53, 12);
            this.label5.TabIndex = 5;
            this.label5.Text = "最大纬度";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(310, 48);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(53, 12);
            this.label4.TabIndex = 4;
            this.label4.Text = "最大经度";
            // 
            // txtMaxLng
            // 
            this.txtMaxLng.Location = new System.Drawing.Point(250, 45);
            this.txtMaxLng.Name = "txtMaxLng";
            this.txtMaxLng.Size = new System.Drawing.Size(54, 21);
            this.txtMaxLng.TabIndex = 3;
            // 
            // txtMinLng
            // 
            this.txtMinLng.Location = new System.Drawing.Point(104, 45);
            this.txtMinLng.Name = "txtMinLng";
            this.txtMinLng.Size = new System.Drawing.Size(54, 21);
            this.txtMinLng.TabIndex = 2;
            // 
            // txtMinLat
            // 
            this.txtMinLat.Location = new System.Drawing.Point(177, 62);
            this.txtMinLat.Name = "txtMinLat";
            this.txtMinLat.Size = new System.Drawing.Size(54, 21);
            this.txtMinLat.TabIndex = 1;
            // 
            // txtMaxLat
            // 
            this.txtMaxLat.Location = new System.Drawing.Point(177, 31);
            this.txtMaxLat.Name = "txtMaxLat";
            this.txtMaxLat.Size = new System.Drawing.Size(54, 21);
            this.txtMaxLat.TabIndex = 0;
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.checkBox13);
            this.groupBox4.Controls.Add(this.checkBox14);
            this.groupBox4.Controls.Add(this.checkBox15);
            this.groupBox4.Location = new System.Drawing.Point(11, 284);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(435, 58);
            this.groupBox4.TabIndex = 3;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "DNB设置";
            // 
            // checkBox13
            // 
            this.checkBox13.AutoSize = true;
            this.checkBox13.Checked = true;
            this.checkBox13.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox13.Location = new System.Drawing.Point(122, 28);
            this.checkBox13.Name = "checkBox13";
            this.checkBox13.Size = new System.Drawing.Size(132, 16);
            this.checkBox13.TabIndex = 16;
            this.checkBox13.Tag = "B";
            this.checkBox13.Text = "Day-Night Boundary";
            this.checkBox13.UseVisualStyleBackColor = true;
            // 
            // checkBox14
            // 
            this.checkBox14.AutoSize = true;
            this.checkBox14.Checked = true;
            this.checkBox14.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox14.Location = new System.Drawing.Point(62, 28);
            this.checkBox14.Name = "checkBox14";
            this.checkBox14.Size = new System.Drawing.Size(54, 16);
            this.checkBox14.TabIndex = 15;
            this.checkBox14.Tag = "N";
            this.checkBox14.Text = "Night";
            this.checkBox14.UseVisualStyleBackColor = true;
            // 
            // checkBox15
            // 
            this.checkBox15.AutoSize = true;
            this.checkBox15.Checked = true;
            this.checkBox15.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox15.Location = new System.Drawing.Point(14, 28);
            this.checkBox15.Name = "checkBox15";
            this.checkBox15.Size = new System.Drawing.Size(42, 16);
            this.checkBox15.TabIndex = 14;
            this.checkBox15.Tag = "D";
            this.checkBox15.Text = "Day";
            this.checkBox15.UseVisualStyleBackColor = true;
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.btnBrowse);
            this.groupBox5.Controls.Add(this.txtDownloadFolder);
            this.groupBox5.Location = new System.Drawing.Point(12, 352);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(435, 94);
            this.groupBox5.TabIndex = 4;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "下载目录";
            // 
            // btnBrowse
            // 
            this.btnBrowse.Location = new System.Drawing.Point(381, 38);
            this.btnBrowse.Name = "btnBrowse";
            this.btnBrowse.Size = new System.Drawing.Size(49, 20);
            this.btnBrowse.TabIndex = 1;
            this.btnBrowse.Text = "浏览..";
            this.btnBrowse.UseVisualStyleBackColor = true;
            this.btnBrowse.Click += new System.EventHandler(this.btnBrowse_Click);
            // 
            // txtDownloadFolder
            // 
            this.txtDownloadFolder.Enabled = false;
            this.txtDownloadFolder.Location = new System.Drawing.Point(11, 38);
            this.txtDownloadFolder.Name = "txtDownloadFolder";
            this.txtDownloadFolder.Size = new System.Drawing.Size(365, 21);
            this.txtDownloadFolder.TabIndex = 0;
            // 
            // groupBox6
            // 
            this.groupBox6.Controls.Add(this.dataGridView1);
            this.groupBox6.Location = new System.Drawing.Point(453, 14);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(510, 243);
            this.groupBox6.TabIndex = 8;
            this.groupBox6.TabStop = false;
            this.groupBox6.Text = "待下载文件列表";
            // 
            // dataGridView1
            // 
            this.dataGridView1.AllowUserToAddRows = false;
            this.dataGridView1.AllowUserToDeleteRows = false;
            this.dataGridView1.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.Fill;
            dataGridViewCellStyle1.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle1.BackColor = System.Drawing.SystemColors.Control;
            dataGridViewCellStyle1.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            dataGridViewCellStyle1.ForeColor = System.Drawing.SystemColors.WindowText;
            dataGridViewCellStyle1.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle1.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle1.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.dataGridView1.ColumnHeadersDefaultCellStyle = dataGridViewCellStyle1;
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            dataGridViewCellStyle2.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle2.BackColor = System.Drawing.SystemColors.Window;
            dataGridViewCellStyle2.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            dataGridViewCellStyle2.ForeColor = System.Drawing.SystemColors.ControlText;
            dataGridViewCellStyle2.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle2.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle2.WrapMode = System.Windows.Forms.DataGridViewTriState.False;
            this.dataGridView1.DefaultCellStyle = dataGridViewCellStyle2;
            this.dataGridView1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dataGridView1.Location = new System.Drawing.Point(3, 17);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.ReadOnly = true;
            dataGridViewCellStyle3.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle3.BackColor = System.Drawing.SystemColors.Control;
            dataGridViewCellStyle3.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            dataGridViewCellStyle3.ForeColor = System.Drawing.SystemColors.WindowText;
            dataGridViewCellStyle3.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle3.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle3.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.dataGridView1.RowHeadersDefaultCellStyle = dataGridViewCellStyle3;
            this.dataGridView1.RowTemplate.Height = 23;
            this.dataGridView1.Size = new System.Drawing.Size(504, 223);
            this.dataGridView1.TabIndex = 0;
            // 
            // groupBox7
            // 
            this.groupBox7.Controls.Add(this.tableLayoutPanel1);
            this.groupBox7.Location = new System.Drawing.Point(456, 267);
            this.groupBox7.Name = "groupBox7";
            this.groupBox7.Size = new System.Drawing.Size(507, 179);
            this.groupBox7.TabIndex = 9;
            this.groupBox7.TabStop = false;
            this.groupBox7.Text = "下载进度";
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.progressBar1, 0, 2);
            this.tableLayoutPanel1.Controls.Add(this.panel1, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.richTextBox1, 0, 1);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(3, 17);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 3;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 21.32353F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 78.67648F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(501, 159);
            this.tableLayoutPanel1.TabIndex = 0;
            // 
            // progressBar1
            // 
            this.progressBar1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.progressBar1.Location = new System.Drawing.Point(3, 141);
            this.progressBar1.Name = "progressBar1";
            this.progressBar1.Size = new System.Drawing.Size(495, 15);
            this.progressBar1.TabIndex = 4;
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.lblSpeed);
            this.panel1.Controls.Add(this.lblIndex);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(3, 3);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(495, 23);
            this.panel1.TabIndex = 1;
            // 
            // lblSpeed
            // 
            this.lblSpeed.AutoSize = true;
            this.lblSpeed.Location = new System.Drawing.Point(389, 5);
            this.lblSpeed.Name = "lblSpeed";
            this.lblSpeed.Size = new System.Drawing.Size(23, 12);
            this.lblSpeed.TabIndex = 3;
            this.lblSpeed.Text = "   ";
            // 
            // lblIndex
            // 
            this.lblIndex.AutoSize = true;
            this.lblIndex.Location = new System.Drawing.Point(3, 5);
            this.lblIndex.Name = "lblIndex";
            this.lblIndex.Size = new System.Drawing.Size(0, 12);
            this.lblIndex.TabIndex = 1;
            // 
            // richTextBox1
            // 
            this.richTextBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.richTextBox1.Location = new System.Drawing.Point(3, 32);
            this.richTextBox1.Name = "richTextBox1";
            this.richTextBox1.ReadOnly = true;
            this.richTextBox1.Size = new System.Drawing.Size(495, 103);
            this.richTextBox1.TabIndex = 2;
            this.richTextBox1.Text = "";
            // 
            // btnSetAppKey
            // 
            this.btnSetAppKey.Location = new System.Drawing.Point(119, 454);
            this.btnSetAppKey.Name = "btnSetAppKey";
            this.btnSetAppKey.Size = new System.Drawing.Size(70, 20);
            this.btnSetAppKey.TabIndex = 13;
            this.btnSetAppKey.Text = "App Key";
            this.btnSetAppKey.UseVisualStyleBackColor = true;
            this.btnSetAppKey.Visible = false;
            this.btnSetAppKey.Click += new System.EventHandler(this.btnSetAppKey_Click);
            // 
            // btnQuery
            // 
            this.btnQuery.Location = new System.Drawing.Point(618, 452);
            this.btnQuery.Name = "btnQuery";
            this.btnQuery.Size = new System.Drawing.Size(64, 20);
            this.btnQuery.TabIndex = 5;
            this.btnQuery.Text = "查询";
            this.btnQuery.UseVisualStyleBackColor = true;
            this.btnQuery.Click += new System.EventHandler(this.btnQuery_Click);
            // 
            // btnDownload
            // 
            this.btnDownload.Location = new System.Drawing.Point(758, 452);
            this.btnDownload.Name = "btnDownload";
            this.btnDownload.Size = new System.Drawing.Size(64, 20);
            this.btnDownload.TabIndex = 6;
            this.btnDownload.Text = "下载";
            this.btnDownload.UseVisualStyleBackColor = true;
            this.btnDownload.Click += new System.EventHandler(this.btnDownload_Click);
            // 
            // btnCancel
            // 
            this.btnCancel.Location = new System.Drawing.Point(899, 452);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(64, 20);
            this.btnCancel.TabIndex = 7;
            this.btnCancel.Text = "退出";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // btnCancelDownload
            // 
            this.btnCancelDownload.Location = new System.Drawing.Point(828, 452);
            this.btnCancelDownload.Name = "btnCancelDownload";
            this.btnCancelDownload.Size = new System.Drawing.Size(65, 20);
            this.btnCancelDownload.TabIndex = 10;
            this.btnCancelDownload.Text = "停止下载";
            this.btnCancelDownload.UseVisualStyleBackColor = true;
            this.btnCancelDownload.Click += new System.EventHandler(this.btnCancelDownload_Click);
            // 
            // btnClearLog
            // 
            this.btnClearLog.Location = new System.Drawing.Point(459, 452);
            this.btnClearLog.Name = "btnClearLog";
            this.btnClearLog.Size = new System.Drawing.Size(64, 20);
            this.btnClearLog.TabIndex = 11;
            this.btnClearLog.Text = "清空日志";
            this.btnClearLog.UseVisualStyleBackColor = true;
            this.btnClearLog.Click += new System.EventHandler(this.btnClearLog_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(11, 458);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(41, 12);
            this.label3.TabIndex = 14;
            this.label3.Text = "字号：";
            // 
            // cboxFontSize
            // 
            this.cboxFontSize.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboxFontSize.FormattingEnabled = true;
            this.cboxFontSize.Location = new System.Drawing.Point(58, 454);
            this.cboxFontSize.Name = "cboxFontSize";
            this.cboxFontSize.Size = new System.Drawing.Size(55, 20);
            this.cboxFontSize.TabIndex = 15;
            // 
            // btnSaveUrl
            // 
            this.btnSaveUrl.Location = new System.Drawing.Point(688, 452);
            this.btnSaveUrl.Name = "btnSaveUrl";
            this.btnSaveUrl.Size = new System.Drawing.Size(64, 20);
            this.btnSaveUrl.TabIndex = 16;
            this.btnSaveUrl.Text = "保存地址";
            this.btnSaveUrl.UseVisualStyleBackColor = true;
            this.btnSaveUrl.Click += new System.EventHandler(this.btnSaveUrl_Click);
            // 
            // notifyIcon1
            // 
            this.notifyIcon1.Icon = ((System.Drawing.Icon)(resources.GetObject("notifyIcon1.Icon")));
            this.notifyIcon1.Text = "MODIS 数据下载器 V1.0";
            this.notifyIcon1.Visible = true;
            this.notifyIcon1.DoubleClick += new System.EventHandler(this.notifyIcon1_DoubleClick);
            // 
            // FrmDownload
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.ClientSize = new System.Drawing.Size(975, 482);
            this.Controls.Add(this.btnSaveUrl);
            this.Controls.Add(this.cboxFontSize);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.btnClearLog);
            this.Controls.Add(this.btnCancelDownload);
            this.Controls.Add(this.btnSetAppKey);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.groupBox7);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.btnDownload);
            this.Controls.Add(this.groupBox6);
            this.Controls.Add(this.btnQuery);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox5);
            this.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "FrmDownload";
            this.Text = "MODIS 数据下载器 V1.0";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FrmDownload_FormClosing);
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.FrmDownload_FormClosed);
            this.Load += new System.EventHandler(this.FrmDownload_Load);
            this.Resize += new System.EventHandler(this.FrmDownload_Resize);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            this.groupBox6.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            this.groupBox7.ResumeLayout(false);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button btnUncheckAllProducts;
        private System.Windows.Forms.Button btnCheckAllProducts;
        private System.Windows.Forms.CheckBox checkBox7;
        private System.Windows.Forms.CheckBox checkBox8;
        private System.Windows.Forms.CheckBox checkBox9;
        private System.Windows.Forms.CheckBox checkBox10;
        private System.Windows.Forms.CheckBox checkBox11;
        private System.Windows.Forms.CheckBox checkBox12;
        private System.Windows.Forms.CheckBox checkBox6;
        private System.Windows.Forms.CheckBox checkBox5;
        private System.Windows.Forms.CheckBox checkBox4;
        private System.Windows.Forms.CheckBox checkBox3;
        private System.Windows.Forms.CheckBox checkBox2;
        private System.Windows.Forms.CheckBox checkBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.DateTimePicker dateTimePicker2;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.DateTimePicker dateTimePicker1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox txtMaxLng;
        private System.Windows.Forms.TextBox txtMinLng;
        private System.Windows.Forms.TextBox txtMinLat;
        private System.Windows.Forms.TextBox txtMaxLat;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.CheckBox checkBox13;
        private System.Windows.Forms.CheckBox checkBox14;
        private System.Windows.Forms.CheckBox checkBox15;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.Button btnBrowse;
        private System.Windows.Forms.TextBox txtDownloadFolder;
        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.GroupBox groupBox7;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label lblSpeed;
        private System.Windows.Forms.Label lblIndex;
        private System.Windows.Forms.RichTextBox richTextBox1;
        private System.Windows.Forms.Button btnSetAppKey;
        private System.Windows.Forms.Button btnClearLog;
        private System.Windows.Forms.Button btnCancelDownload;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.Button btnDownload;
        private System.Windows.Forms.Button btnQuery;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ComboBox cboxFontSize;
        private System.Windows.Forms.ProgressBar progressBar1;
        private System.Windows.Forms.Button btnInvertCheckedProducts;
        private System.Windows.Forms.Button btnSaveUrl;
        private System.Windows.Forms.NotifyIcon notifyIcon1;
    }
}
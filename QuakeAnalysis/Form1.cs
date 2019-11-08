using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Windows.Forms;
using MathNet.Numerics.LinearAlgebra.Double;
using OSGeo.GDAL;
using QuakeAnalysis.Entity;
using QuakeAnalysis.Method.Gdal;

namespace QuakeAnalysis
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            GdalConfiguration.ConfigureGdal();
            GdalConfiguration.ConfigureOgr();
            Gdal.AllRegister();

            var path = Environment.CurrentDirectory + "//Data//CODG1950.17I";
            var codgData = CODGData.LoadFromFile(path);

            var item = codgData.DataItems.Find(i => i.DateTime.Hour == 20);
            item.Elements.Sort((x1, x2) => x1.Latitude.CompareTo(x2.Latitude));

            //item.Elements.ForEach(x => Console.WriteLine(x.Values.Count));

            var matrix = DenseMatrix.Build.Dense(item.Elements.Count, 73);

            Enumerable.Range(0, item.Elements.Count).ToList()
                .ForEach(i => matrix.SetRow(i, item.Elements[i].Values.ToArray()));

            var tiffPath = "demo.tif";

            Driver driver = Gdal.GetDriverByName("GTiff");
            Console.WriteLine("Using Driver:" + driver);

            var array = matrix.ToRowMajorArray();
            var min = (int)array.Min();
            var max = (int)array.Max();
            Console.WriteLine("Min:" + matrix.ToRowMajorArray().Min());
            Console.WriteLine("Max:" + matrix.ToRowMajorArray().Max());

            if (File.Exists("demo.tif"))
                File.Delete("demo.tif");
            Dataset ds = driver.Create(tiffPath, matrix.RowCount, matrix.ColumnCount, 1, DataType.GDT_Byte, null);

            GCP[] GCPs = new GCP[]
            {
                new GCP(44.5, 27.5, 0, 0, 0, "info0", "id0"),
                new GCP(45.5, 27.5, 0, 100, 0, "info1", "id1"),
                new GCP(44.5, 26.5, 0, 0, 100, "info2", "id2"),
                new GCP(45.5, 26.5, 0, 100, 100, "info3", "id3")
            };
            ds.SetGCPs(GCPs, "");

            Band ba = ds.GetRasterBand(1);
            // add color to dataset
            var colorTable = new ColorTable(PaletteInterp.GPI_RGB);

            Enumerable.Range(min, max).ToList().ForEach(i =>
                colorTable.SetColorEntry(i, GetColorEntry(Color.Blue, Color.Red, t: (float)i / max)));

            ba.SetColorTable(colorTable);

            Console.WriteLine("Row Count:{0}, Column Count:{1}", matrix.RowCount, matrix.ColumnCount);

            double[] buffer = matrix.ToRowMajorArray().Select(Convert.ToDouble).ToArray();

            ba.WriteRaster(0, 0, matrix.RowCount, matrix.ColumnCount, buffer, matrix.RowCount, matrix.ColumnCount, 0,
                0);

            ba.FlushCache();
            ds.FlushCache();
            ba.Dispose();
            ds.Dispose();
            driver.Dispose();
        }

        public static ColorEntry GetColorEntry(Color a, Color b, float t)
        {
            return new ColorEntry
            {
                c1 = (short)(a.R + (b.R - a.R) * t),
                c2 = (short)(a.G + (b.G - a.G) * t),
                c3 = (short)(a.R + (b.R - a.R) * t),
                c4 = (short)(a.A + (b.A - a.A) * t)
            };
        }

        /// <summary>  
        /// GDAL栅格转换为位图  
        /// </summary>  
        /// <param name="ds">GDAL Dataset</param>  
        /// <param name="showRect">显示区域</param>  
        /// <param name="bandlist">需要显示的波段列表</param>  
        /// <returns>返回Bitmap对象</returns>  
        public Bitmap GetImage(Dataset ds, Rectangle showRect, int[] bandlist)
        {
            int imgWidth = ds.RasterXSize; //影像宽  
            int imgHeight = ds.RasterYSize; //影像高  

            float ImgRatio = imgWidth / (float)imgHeight; //影像宽高比  

            //获取显示控件大小  
            int BoxWidth = showRect.Width;
            int BoxHeight = showRect.Height;

            float BoxRatio = imgWidth / (float)imgHeight; //显示控件宽高比  

            //计算实际显示区域大小，防止影像畸变显示  
            int BufferWidth, BufferHeight;
            if (BoxRatio >= ImgRatio)
            {
                BufferHeight = BoxHeight;
                BufferWidth = (int)(BoxHeight * ImgRatio);
            }
            else
            {
                BufferWidth = BoxWidth;
                BufferHeight = (int)(BoxWidth / ImgRatio);
            }

            //构建位图  
            Bitmap bitmap = new Bitmap(BufferWidth, BufferHeight,
                System.Drawing.Imaging.PixelFormat.Format24bppRgb);

            if (bandlist.Length == 3) //RGB显示  
            {
                int[] r = new int[BufferWidth * BufferHeight];
                Band band1 = ds.GetRasterBand(bandlist[0]);
                band1.ReadRaster(0, 0, imgWidth, imgHeight, r, BufferWidth, BufferHeight, 0, 0); //读取图像到内存  

                //为了显示好看，进行最大最小值拉伸显示  
                double[] maxandmin1 = { 0, 0 };
                band1.ComputeRasterMinMax(maxandmin1, 0);

                int[] g = new int[BufferWidth * BufferHeight];
                Band band2 = ds.GetRasterBand(bandlist[1]);
                band2.ReadRaster(0, 0, imgWidth, imgHeight, g, BufferWidth, BufferHeight, 0, 0);

                double[] maxandmin2 = { 0, 0 };
                band2.ComputeRasterMinMax(maxandmin2, 0);

                int[] b = new int[BufferWidth * BufferHeight];
                Band band3 = ds.GetRasterBand(bandlist[2]);
                band3.ReadRaster(0, 0, imgWidth, imgHeight, b, BufferWidth, BufferHeight, 0, 0);

                double[] maxandmin3 = { 0, 0 };
                band3.ComputeRasterMinMax(maxandmin3, 0);

                int i, j;
                for (i = 0; i < BufferWidth; i++)
                {
                    for (j = 0; j < BufferHeight; j++)
                    {
                        int rVal = Convert.ToInt32(r[i + j * BufferWidth]);
                        rVal = (int)((rVal - maxandmin1[0]) / (maxandmin1[1] - maxandmin1[0]) * 255);

                        int gVal = Convert.ToInt32(g[i + j * BufferWidth]);
                        gVal = (int)((gVal - maxandmin2[0]) / (maxandmin2[1] - maxandmin2[0]) * 255);

                        int bVal = Convert.ToInt32(b[i + j * BufferWidth]);
                        bVal = (int)((bVal - maxandmin3[0]) / (maxandmin3[1] - maxandmin3[0]) * 255);

                        Color newColor = Color.FromArgb(rVal, gVal, bVal);
                        bitmap.SetPixel(i, j, newColor);
                    }
                }
            }
            else //灰度显示  
            {
                int[] r = new int[BufferWidth * BufferHeight];
                Band band1 = ds.GetRasterBand(bandlist[0]);
                band1.ReadRaster(0, 0, imgWidth, imgHeight, r, BufferWidth, BufferHeight, 0, 0);

                double[] maxandmin1 = { 0, 0 };
                band1.ComputeRasterMinMax(maxandmin1, 0);

                int i, j;
                for (i = 0; i < BufferWidth; i++)
                {
                    for (j = 0; j < BufferHeight; j++)
                    {
                        int rVal = Convert.ToInt32(r[i + j * BufferWidth]);
                        rVal = (int)((rVal - maxandmin1[0]) / (maxandmin1[1] - maxandmin1[0]) * 255);

                        Color newColor = Color.FromArgb(rVal, rVal, rVal);
                        bitmap.SetPixel(i, j, newColor);
                    }
                }
            }

            return bitmap;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string filename = "";
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "Tiff文件|*.tif|Erdas img文件|*.img|Bmp文件|*.bmp|jpeg文件|*.jpg|所有文件|*.*";
            if (dlg.ShowDialog() == DialogResult.OK)
            {
                filename = dlg.FileName;
            }

            if (filename == "")
            {
                MessageBox.Show("影像路径不能为空");
                return;
            }
            OSGeo.GDAL.Dataset ds = Gdal.Open(filename, Access.GA_ReadOnly);
            if (ds == null)
            {
                MessageBox.Show("影像打开失败");
                return;
            }
            Rectangle pictureRect = new Rectangle();
            pictureRect.X = 0;
            pictureRect.Y = 0;
            pictureRect.Width = this.pictureBox1.Width;
            pictureRect.Height = this.pictureBox1.Height;

            int[] disband = { 3, 2, 1 };

            Bitmap bitmap = GetImage(ds, pictureRect, disband);   //遥感影像构建位图  
            pictureBox1.Image = bitmap;                   //将位图传递给PictureBox控件进行显示  
        }





    }


}

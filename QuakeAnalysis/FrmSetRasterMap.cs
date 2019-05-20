using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Linq;
using DevExpress.Utils;
using DevExpress.XtraEditors;
using DevExpress.XtraEditors.Controls;
using DevExpress.XtraPrinting.Native;

namespace QuakeAnalysis
{
    public partial class FrmSetRasterMap : XtraForm
    {
        public FrmSetRasterMap()
        {
            InitializeComponent();
        }

        private void FrmSetRasterMap_Load(object sender, EventArgs e)
        {
            // 色带的宽度和高度
            var beltWidth = 200;
            var beltHeight = 16;
            // 需要在这里设置ImageSize
            var imageCollection = new ImageCollection { ImageSize = new Size(beltWidth, beltHeight) };
            var gradientColors = new List<GradientColor>
            {
                new GradientColor {Name = "Belt1", StartColor = Color.White, EndColor = Color.Black},
                new GradientColor {Name = "Belt2", StartColor = Color.LawnGreen, EndColor = Color.Red},
                new GradientColor {Name = "Belt3", StartColor = Color.Blue, EndColor = Color.Purple},
                new GradientColor {Name = "Belt3", StartColor = Color.LightGoldenrodYellow, EndColor = Color.Chocolate}
            };
            Enumerable.Range(0, gradientColors.Count).ForEach(i =>
                imageComboBoxEdit1.Properties.Items.Add(
                    new ImageComboBoxItem(gradientColors[i].Name, gradientColors[i])
                ));
            imageComboBoxEdit1.Properties.LargeImages = imageCollection;

            Enumerable.Range(0, gradientColors.Count).ForEach(i =>
                imageComboBoxEdit2.Properties.Items.Add(
                    new ImageComboBoxItem(gradientColors[i].Name, gradientColors[i])
                ));
            imageComboBoxEdit2.Properties.LargeImages = imageCollection;

            Func<Rectangle, Color, Color, LinearGradientBrush> getLgBrush = (r, c1, c2) =>
                new LinearGradientBrush(r, c1, c2, LinearGradientMode.Horizontal);

            Action<ImageComboBoxItem> drawRectangle = item =>
            {
                int iWidth = beltWidth;
                int iHeight = beltHeight;
                var bitmap = new Bitmap(iWidth, iHeight);
                var gc = item.Value as GradientColor;
                using (var g = Graphics.FromImage(bitmap))
                {
                    g.DrawRectangle(Pens.Black, 0, 0, iWidth, iHeight);
                    var rectangle = new Rectangle(1, 1, iWidth - 2, iHeight - 2);
                    var gradientBrush = getLgBrush(rectangle, gc.StartColor, gc.EndColor);
                    g.FillRectangle(gradientBrush, rectangle);

                    imageCollection.Images.Add(bitmap);
                    item.ImageIndex = imageCollection.Images.Count - 1;
                }
            };

            imageComboBoxEdit1.Properties.Items.ForEach(drawRectangle);
            imageComboBoxEdit2.Properties.Items.ForEach(drawRectangle);


            textEdit1.EditValue = "XX地区栅格专题图";
            imageComboBoxEdit1.SelectedIndex = 1;
            imageComboBoxEdit2.SelectedIndex = 2;
            textEdit4.EditValue = 101.0;
            textEdit5.EditValue = 80;
            textEdit6.EditValue = 108.7;
            textEdit2.EditValue = 95.0;

            textEdit3.EditValue = @"D:\Data\City.shp";
            textEdit7.EditValue = @"D:\Data\Earthquake\Quake.csv";

            textEdit8.EditValue = "1.0, 2.0, 3.0";
            textEdit9.EditValue = "4.0, 5.0, 6.0";
        }

    }


    public class GradientColor
    {
        private string _name;
        private Color _startColor;
        private Color _endColor;

        public string Name
        {
            get { return _name; }
            set { _name = value; }
        }

        public Color StartColor
        {
            get { return _startColor; }
            set { _startColor = value; }
        }

        public Color EndColor
        {
            get { return _endColor; }
            set { _endColor = value; }
        }
    }
}

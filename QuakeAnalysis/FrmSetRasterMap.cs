using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Linq;
using System.Windows.Forms;

namespace QuakeAnalysis
{
    public partial class FrmSetRasterMap : Form
    {
        public FrmSetRasterMap()
        {
            InitializeComponent();
        }

        private void FrmSetRasterMap_Load(object sender, EventArgs e)
        {

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

using MathNet.Numerics.LinearAlgebra;
using MathNet.Numerics.Statistics;
using System;
using System.Drawing;

namespace QuakeAnalysis
{
    /// <summary>
    /// 工程用扩展方法
    /// </summary>
    public static class ExtendMethod
    {
        public static string Description(this Matrix<double> matrix)
        {
            var enumerable = matrix.Enumerate();
            const int rbit = 2;
            return string.Format("MAT INFO [X:{0}, Y:{1}, MIN:{2}, MAX:{3}, MEAN:{4}, STD_DEV:{5}]", matrix.RowCount,
                matrix.ColumnCount,
                enumerable.Minimum(), enumerable.Maximum(), Math.Round(enumerable.Mean(), rbit),
                Math.Round(enumerable.StandardDeviation(), rbit));
        }

        public static Font ChangeFontSize(this Font font, float fontSize)
        {
            if (font != null)
            {
                float currentSize = font.Size;
                if (currentSize != fontSize)
                {
                    font = new Font(font.Name, fontSize, font.Style,
                        font.Unit, font.GdiCharSet, font.GdiVerticalFont);
                }
            }

            return font;
        }
    }
}

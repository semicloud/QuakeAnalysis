using MathNet.Numerics.LinearAlgebra;
using MathNet.Numerics.Statistics;
using System;

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

        
    }
}

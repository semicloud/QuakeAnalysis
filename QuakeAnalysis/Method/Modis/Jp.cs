using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MathNet.Numerics.LinearAlgebra;

namespace QuakeAnalysis.Method.Modis
{
    /// <summary>
    /// Modis异常处理算法距平法
    /// </summary>
    public static  class Jp
    {
        public static Matrix<double> GetJp(Matrix<double> matrix, Matrix<double> meanedMatrix)
        {
            Contract.Requires(
                matrix.RowCount == meanedMatrix.RowCount && matrix.ColumnCount == meanedMatrix.ColumnCount);
            return matrix - meanedMatrix;
        }

    }
}

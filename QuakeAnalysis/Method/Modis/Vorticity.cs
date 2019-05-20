using MathNet.Numerics.LinearAlgebra;

namespace QuakeAnalysis.Method.Modis
{
    /// <summary>
    /// 涡度处理类
    /// </summary>
    public static class Vorticity
    {
        /// <summary>
        /// 计算4点涡度值
        /// </summary>
        /// <param name="matrix">待求涡度的矩阵</param>
        /// <param name="defaultValue">默认跳过的值（一般为NoDataValue？）</param>
        /// <returns></returns>
        public static Matrix<double> Get4PVorticity(Matrix<double> matrix, double defaultValue)
        {
            int iRow = matrix.RowCount, iCol = matrix.ColumnCount;
            var ans = Matrix<double>.Build.Dense(iRow, iCol, 0);
            // Set Border
            ans.SetRow(0, matrix.Row(0));
            ans.SetRow(iRow - 1, matrix.Row(iRow - 1));
            ans.SetColumn(0, matrix.Column(0));
            ans.SetColumn(iCol - 1, matrix.Column(iCol));

            for (int i = 1; i < iRow - 1; i++)
            {
                for (int j = 1; j < iCol - 1; j++)
                {
                    if (matrix[i, j] != defaultValue &&
                        matrix[i, j - 1] != defaultValue && matrix[i, j + 1] != defaultValue &&
                        matrix[i - 1, j] != defaultValue && matrix[i + 1, j] != defaultValue)
                    {
                        ans[i, j] = 4 * matrix[i, j] - matrix[i, j - 1] - matrix[i, j + 1] - matrix[i - 1, j] -
                                    matrix[i + 1, j];
                    }
                    else
                    {
                        ans[i, j] = matrix[i, j];
                    }
                }
            }

            return ans;
        }

        /// <summary>
        /// 计算8点涡度值
        /// </summary>
        /// <param name="matrix">待求涡度的矩阵</param>
        /// <param name="defaultValue">默认跳过的值（一般为NoDataValue？）</param>
        /// <returns></returns>
        public static Matrix<double> Get8PVorticity(Matrix<double> matrix, double defaultValue)
        {
            int iRow = matrix.RowCount, iCol = matrix.ColumnCount;
            var ans = Matrix<double>.Build.Dense(iRow, iCol, 0);
            // Set Border
            ans.SetRow(0, matrix.Row(0));
            ans.SetRow(iRow - 1, matrix.Row(iRow - 1));
            ans.SetColumn(0, matrix.Column(0));
            ans.SetColumn(iCol - 1, matrix.Column(iCol));

            for (int i = 1; i < iRow - 1; i++)
            {
                for (int j = 1; j < iCol - 1; j++)
                {
                    if (matrix[i, j] != defaultValue &&
                        matrix[i, j - 1] != defaultValue && matrix[i, j + 1] != defaultValue &&
                        matrix[i - 1, j] != defaultValue && matrix[i + 1, j] != defaultValue &&
                        matrix[i - 1, j - 1] != defaultValue && matrix[i + 1, j - 1] != defaultValue &&
                        matrix[i - 1, j - 1] != defaultValue && matrix[i + 1, j + 1] != defaultValue)
                    {
                        ans[i, j] = 8 * matrix[i, j] - matrix[i, j - 1] - matrix[i, j + 1] - matrix[i - 1, j] -
                                    matrix[i + 1, j]
                                    - matrix[i - 1, j - 1] - matrix[i + 1, j - 1] - matrix[i - 1, j - 1] -
                                    matrix[i + 1, j + 1];
                    }
                    else
                    {
                        ans[i, j] = matrix[i, j];
                    }
                }
            }

            return ans;
        }
    }
}

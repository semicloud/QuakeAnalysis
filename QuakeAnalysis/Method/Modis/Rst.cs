using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MathNet.Numerics.LinearAlgebra;
using MathNet.Numerics.Statistics;

namespace QuakeAnalysis.Method.Modis
{
    /// <summary>
    /// Modis异常提取算法--RST(Robust Satellite Techniques)法
    /// </summary>
    public static class Rst
    {
        /// <summary>
        /// Modis异常识别方法 -- RST法
        /// </summary>
        /// <param name="matrix">待识别的矩阵</param>
        /// <param name="defaultValue">需要忽略的值</param>
        /// <returns></returns>
        public static  Matrix<double> GetRst(Matrix<double> matrix, double defaultValue)
        {
            //TODO 是否需要跳过NoDataValue，以及下面标准化的处理
            var array = matrix.Enumerate().ToList().FindAll(d => d != defaultValue).ToArray();
            double mean = array.Mean(), sd = array.StandardDeviation();
            Func<double, double> normalize = d => d != defaultValue ? (d - mean) / sd : d;
            matrix.MapInplace(normalize);
            return matrix;
        }
    }
}

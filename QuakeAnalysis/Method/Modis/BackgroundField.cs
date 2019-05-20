using System;
using System.Collections.Generic;
using MathNet.Numerics.LinearAlgebra;

namespace QuakeAnalysis.Method.Modis
{
    /// <summary>
    /// 背景场计算
    /// 背景场计算应该分为两步，首先检查当前数据集是否存在背景场数据，如果存在背景场数据则直接使用
    /// 如果不存在背景场数据，则需要重新创建背景场数据。创建背景场数据的步骤如下：找到多个（至少一个）历史
    /// 同期数据，然后将这些历史同期数据取平均值，得到背景场数据，如果找不到历史同期数据，则应报错，提示
    /// 无法形成背景场数据
    /// 如何找到历史同期数据？
    /// </summary>
   public static  class BackgroundField
    {

        public static Matrix<double> GetBackgroundField(List<Matrix<double>> matrices)
        {
            throw new NotImplementedException();
        }
    }
}

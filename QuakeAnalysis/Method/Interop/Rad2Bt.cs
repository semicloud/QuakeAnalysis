using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using NUnit.Framework;

namespace QuakeAnalysis.Method.Interop
{

    public static class Rad2Bt
    {
        /// <summary>
        /// 辐亮度转换为亮温，此函数实际为C++编写的dll
        /// </summary>
        /// <param name="length">数组长度</param>
        /// <param name="rads">待转换的辐亮度</param>
        /// <param name="band">波段值，取固定值31</param>
        /// <param name="ans">返回数组</param>
        [DllImport("ModisCppLib.dll", EntryPoint = "rad2bt", SetLastError = true,
            CallingConvention = CallingConvention.StdCall)]
        public static extern void rad2bt(int length, float[] rads, int band,
            [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 1)]
            float[] ans);
    }
}

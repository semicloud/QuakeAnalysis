using System;
using MathNet.Numerics.LinearAlgebra;
using QuakeAnalysis.Entity;

namespace QuakeAnalysis.Method.Modis
{
    public static class BTProcess
    {
        public static void Preprocess(string directory, string dateStr)
        {

        }

        // 辐亮度转换为亮温
        public static Matrix<double> ConvertToBT(Matrix<double> matrix)
        {
            throw new NotImplementedException();
        }

        public static bool IsDataPrepared(string dateStr)
        {
            throw new NotImplementedException();
        }

        // 调用MRT，HDF转换为GTIFF
        public static void ConvertHDF2GTiff(PrmSetting setting, out string gTiffFilePath)
        {
            throw new NotImplementedException();
        }

        // 
        public static double[] GetRadianceScaleAndOffset(string hdfFilePath)
        {
            throw new NotImplementedException();
        }

        public static Matrix<double> GetSolarZenithMatrix(string mxd03HdfFilePath)
        {
            throw new NotImplementedException();
        }

        public static double GetMxd03Scale(string mxd03HdfFilePath)
        {
            throw new NotImplementedException();
        }

        public static Matrix<double> GetCloudlessMatrix(string mxd35HdfFilePath)
        {
            throw new NotImplementedException();
        }

        public static double GetMxd35Scale(string mxd35HdfFilePath)
        {
            throw new NotImplementedException();
        }

        public static Matrix<double> ReadValuesFromGTiff(string gTiffFilePath)
        {
            throw new NotImplementedException();
        }

        public static void SaveGTiffFileWithWrapper(string srcGTiff, Matrix<double> values, out string targetGTiff)
        {
            throw new NotImplementedException();
        }

        // 转换GTIFF文件的数据类型转换为Float32
        public static void ConvertGTiffDataTypeToFloat32(string srcGTiff, out string targetGTiff)
        {
            throw new NotImplementedException();
        }
    }
}

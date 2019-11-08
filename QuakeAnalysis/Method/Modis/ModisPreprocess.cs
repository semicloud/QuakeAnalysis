using MathNet.Numerics.LinearAlgebra;
using NLog;
using OSGeo.GDAL;
using QuakeAnalysis.Entity;
using QuakeAnalysis.Method.Gdal;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using QuakeAnalysis.Cfg;

namespace QuakeAnalysis.Method.Modis
{
    /// <summary>
    /// MODIS数据集预处理方法
    /// </summary>
    public static class ModisPreprocess
    {
        private static Logger g = LogManager.GetCurrentClassLogger();

        /// <summary>
        /// 检查预处理需要的数据是否齐全
        /// </summary>
        /// <returns></returns>
        public static bool CheckData()
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// 从指定的目录中找到含有findStr的文件，如果找到返回该文件的路径，如果找不到则抛出FileNotFoundException
        /// </summary>
        /// <param name="sDir"></param>
        /// <param name="findStr"></param>
        /// <returns></returns>
        public static List<string> FindFiles(string sDir, string findStr)
        {
            //TODO 目录合法性检查
            var directory = new DirectoryInfo(sDir);
            if (directory.Exists)
            {
                var fileInfos = directory.GetFiles("*" + findStr + "*", SearchOption.TopDirectoryOnly);
                var fileNames = fileInfos.Select(i => i.FullName).ToList();
                return fileNames;
            }

            throw new DirectoryNotFoundException();
        }

        // 找到目录下用于预处理的数据集
        public static string FindDatasetForPreprocess(string sDir, string findStr)
        {
            var findFiles = FindFiles(sDir, findStr);
            if (!findFiles.Any()) throw new FileNotFoundException();
            if (findFiles.Count > 1) throw new Exception("more than 1 files found at " + sDir);
            return findFiles.First();
        }

        /// <summary>
        /// 检查运行环境是否OK
        /// </summary>
        /// <returns></returns>
        public static bool CheckEnvironment()
        {
            throw new NotImplementedException();
        }


        public static void PreprocessBT(string sDateTime)
        {

        }

        // 预处理MXD02数据集
        public static void PreprocessMXD02(string hdfFilePath)
        {
            HdfFile hdfFile = new HdfFile(hdfFilePath);
            if (hdfFile.ModisType == ModisType.MOD)
            {

            }

            if (hdfFile.ModisType == ModisType.MYD)
            {
                var myd03File = FindDatasetForPreprocess(
                    Environment.CurrentDirectory + @"\modis_workspace\MXD03\MYD03\",
                    hdfFile.DateTimeStr);
                var myd35File = FindDatasetForPreprocess(
                    Environment.CurrentDirectory + @"\modis_workspace\MXD35\MYD35\",
                    hdfFile.DateTimeStr);

                Console.WriteLine(myd03File);
                Console.WriteLine(myd35File);

                var myd02PrmSetting = new PrmSetting();
                myd02PrmSetting.InputHdfFile = hdfFilePath;
                myd02PrmSetting.GeoLocHdfFile = myd03File;
                myd02PrmSetting.SdsStr = "EV_1KM_Emissive, 1";
                myd02PrmSetting.UlcLng = 80.0;
                myd02PrmSetting.UlcLat = 40.0;
                myd02PrmSetting.LrcLng = 110.0;
                myd02PrmSetting.LrcLat = 20.0;
                myd02PrmSetting.OutputGTiffFile = Config.GetModisTmpDir() + @"\MXD02.tiff";

                Hdf2GTiff hdf2GTiff = new Hdf2GTiff(myd02PrmSetting, Config.GetModisTmpDir());
                hdf2GTiff.DoConvert();

                var myd03PrmSetting = new PrmSetting();
                myd03PrmSetting.InputHdfFile = myd03File;
                myd03PrmSetting.GeoLocHdfFile = myd03File;
                myd03PrmSetting.SdsStr = "SolarZenith";
                myd03PrmSetting.UlcLng = 80.0;
                myd03PrmSetting.UlcLat = 40.0;
                myd03PrmSetting.LrcLng = 110.0;
                myd03PrmSetting.LrcLat = 20.0;
                myd03PrmSetting.OutputGTiffFile = Config.GetModisTmpDir() + @"\MXD03.tiff";
                Hdf2GTiff myd03Convert = new Hdf2GTiff(myd03PrmSetting, Config.GetModisTmpDir());
                myd03Convert.DoConvert();

                var myd35PrmSetting = new PrmSetting();
                myd35PrmSetting.InputHdfFile = myd35File;
                myd35PrmSetting.GeoLocHdfFile = myd03File;
                myd35PrmSetting.SdsStr = "Cloud_Mask, 1";
                myd35PrmSetting.UlcLng = 80.0;
                myd35PrmSetting.UlcLat = 40.0;
                myd35PrmSetting.LrcLng = 110.0;
                myd35PrmSetting.LrcLat = 20.0;
                myd35PrmSetting.OutputGTiffFile = Config.GetModisTmpDir() + @"\MXD35.tiff";
                Hdf2GTiff myd35Convert = new Hdf2GTiff(myd35PrmSetting, Config.GetModisTmpDir());
                myd35Convert.DoConvert();

                var tiff02Path =
                    @"e:\CodeWorld\VSProjects\QuakeAnalysis\QuakeAnalysis\QuakeAnalysis\bin\Debug\modis_workspace\Tmp\MXD02_EV_1KM_Emissive_b0.tif";
                var tiff03Path =
                    @"e:\CodeWorld\VSProjects\QuakeAnalysis\QuakeAnalysis\QuakeAnalysis\bin\Debug\modis_workspace\Tmp\MXD03_SolarZenith.tif";
                var tiff35Path =
                    @"e:\CodeWorld\VSProjects\QuakeAnalysis\QuakeAnalysis\QuakeAnalysis\bin\Debug\modis_workspace\Tmp\MXD35_Cloud_Mask_b0.tif";

                Matrix<double> mod02 = GetDNMatrix(tiff02Path, 1);
                Matrix<double> mod03 = GetDNMatrix(tiff03Path, 1);
                Matrix<double> mod35 = GetDNMatrix(tiff35Path, 1);

                g.Debug("mod02 " + mod02.Description());
                g.Debug("mod03 " + mod03.Description());
                g.Debug("mod35 " + mod35.Description());

                // 02数据集处理
                // TODO Magic number should removed
                Matrix<double> mxd02coeff = GdalHelper.ReadRadianceScalesAndOffsets(hdfFilePath);
                double scale = mxd02coeff.Column(10)[0];
                double offset = mxd02coeff.Column(10)[1];
                g.Debug("scale: {0}, offset: {1}", scale, offset);
                Func<double, double> _02calibrate =
                    d => d != 65535 ? (d - offset) * scale : 0;
                mod02.MapInplace(_02calibrate);
                g.Debug("after calibrate, mod02: " + mod02.Description());

                // 03数据集处理
                Func<double, double> _03calibrate = d => d != -32767 ? d * 0.01 : 0;
                Func<double, double> _03Predicate = d => d > 30 ? 1 : 0; // TODO 应为85
                mod03.MapInplace(_03calibrate);
                g.Debug("after calibrate, mod03: " + mod03.Description());
                mod03.MapInplace(_03Predicate);
                g.Debug("after select value greater than 85, mod03: " + mod03.Description());

                // 35数据集处理
                mod35.MapInplace(d => ((int)d & 6) == 6 ? 1 : 0);
                g.Debug("after and opertion, mod35: " + mod35.Description());

                // 最终计算结果
                Matrix<double> resultMatrix = mod02.PointwiseMultiply(mod03).PointwiseMultiply(mod35);
                g.Debug("result matrix: " + resultMatrix.Description());


                //调用GDAL将最终计算结果保存到GTiff
                var tiff02FloatPath =
                    @"e:\CodeWorld\VSProjects\QuakeAnalysis\QuakeAnalysis\QuakeAnalysis\bin\Debug\modis_workspace\Tmp\MXD02_EV_1KM_Emissive_b0_Float.tif";
                var dataSet02 = OSGeo.GDAL.Gdal.OpenShared(tiff02Path, Access.GA_ReadOnly);
                OSGeo.GDAL.Gdal.wrapper_GDALTranslate(tiff02FloatPath, dataSet02,
                    new GDALTranslateOptions(new[] { "-ot", "Float32" }), null, null);
                dataSet02.Dispose();

                var dataSet02Float = OSGeo.GDAL.Gdal.OpenShared(tiff02FloatPath, Access.GA_Update);
                dataSet02Float.GetRasterBand(1).WriteRaster(0, 0, resultMatrix.RowCount, resultMatrix.ColumnCount,
                    resultMatrix.Enumerate().ToArray(), resultMatrix.RowCount, resultMatrix.ColumnCount, 0, 0);
                dataSet02Float.FlushCache();
                dataSet02.Dispose();

                // 将预处理后的数据集复制到相关目录
                // 清理Tmp目录

                g.Debug("done...");
            }
        }

        /// <summary>
        /// 从gTiff中读取某个波段的DN值，以矩阵的形式返回
        /// </summary>
        /// <param name="gTiffPath"></param>
        /// <param name="bandNum"></param>
        /// <returns></returns>
        public static Matrix<double> GetDNMatrix(string gTiffPath, int bandNum)
        {
            if (!File.Exists(gTiffPath))
                throw new FileNotFoundException("读取GTiff文件错误，不存在的GTiff文件：\n" + gTiffPath);
            g.Debug("read value from {0} at band {1}", gTiffPath, bandNum);

            var dataSet = OSGeo.GDAL.Gdal.OpenShared(gTiffPath, Access.GA_ReadOnly);
            g.Debug("file {0} has {1} bands", Path.GetFileName(gTiffPath), dataSet.RasterCount);

            var band = dataSet.GetRasterBand(1);
            var xSize = band.XSize;
            var ySize = band.YSize;
            g.Debug("band xSize is {0}, and ySize is {1}", xSize, ySize);

            var bandData = new double[xSize * ySize];
            band.ReadRaster(0, 0, xSize, ySize, bandData, xSize, ySize, 0, 0);
            var matrix = Matrix<double>.Build.Dense(xSize, ySize, bandData);
            g.Debug("read data, " + matrix.Description());

            dataSet.Dispose();
            g.Debug("close dataset {0}, read value done.", Path.GetFileName(gTiffPath));
            return matrix;
        }
    }
}

echo "..copy openblas libraries.."
if not exist $(TargetDir)libgcc_s_seh-1.dll copy C:\SDK\OpenBLAS-0.3.6-x64\bin\libgcc_s_seh-1.dll $(TargetDir)libgcc_s_seh-1.dll
if not exist $(TargetDir)libgfortran-3.dll copy C:\SDK\OpenBLAS-0.3.6-x64\bin\libgfortran-3.dll $(TargetDir)libgfortran-3.dll
if not exist $(TargetDir)libopenblas.dll copy C:\SDK\OpenBLAS-0.3.6-x64\bin\libopenblas.dll $(TargetDir)libopenblas.dll
if not exist $(TargetDir)libquadmath-0.dll copy C:\SDK\OpenBLAS-0.3.6-x64\bin\libquadmath-0.dll $(TargetDir)libquadmath-0.dll

echo "..copy lapack libraries.."
if not exist $(TargetDir)liblapack.dll copy C:\SDK\lapack-3.7.0-x64\lib\liblapack.dll $(TargetDir)liblapack.dll

echo "..copy gsl libraries.."
if not exist $(TargetDir)gslcblas.dll copy C:\SDK\gsl_x64-windows\bin\gslcblas.dll $(TargetDir)gslcblas.dll
if not exist $(TargetDir)gsl.dll copy C:\SDK\gsl_x64-windows\bin\gslcblas.dll $(TargetDir)gsl.dll

echo "..copy boost libraries.."
if not exist $(TargetDir)boost_program_options-vc141-mt-x64-1_69.dll copy C:\SDK\boost_1_69_0\bin\vc14.1\lib\boost_program_options-vc141-mt-x64-1_69.dll $(TargetDir)boost_program_options-vc141-mt-x64-1_69.dll
if not exist $(TargetDir)boost_filesystem-vc141-mt-x64-1_69.dll copy C:\SDK\boost_1_69_0\bin\vc14.1\lib\boost_filesystem-vc141-mt-x64-1_69.dll $(TargetDir)boost_filesystem-vc141-mt-x64-1_69.dll
if not exist $(TargetDir)boost_log-vc141-mt-x64-1_69.dll copy C:\SDK\boost_1_69_0\bin\vc14.1\lib\boost_log-vc141-mt-x64-1_69.dll $(TargetDir)\boost_log-vc141-mt-x64-1_69.dll
if not exist $(TargetDir)boost_thread-vc141-mt-x64-1_69.dll copy C:\SDK\boost_1_69_0\bin\vc14.1\lib\boost_thread-vc141-mt-x64-1_69.dll $(TargetDir)boost_thread-vc141-mt-x64-1_69.dll
if not exist $(TargetDir)boost_date_time-vc141-mt-x64-1_69.dll copy C:\SDK\boost_1_69_0\bin\vc14.1\lib\boost_date_time-vc141-mt-x64-1_69.dll $(TargetDir)boost_date_time-vc141-mt-x64-1_69.dll

echo "..copy MRTSwath files.."
if not exist $(TargetDir)MRTSWath\bin xcopy C:\DevLib\MRTSwath\bin\* $(TargetDir)MRTSWath\bin /y /e /i
if not exist $(TargetDir)MRTSWath\data xcopy C:\DevLib\MRTSwath\data\* $(TargetDir)MRTSWath\data /y /e /i

echo "..copy gdal libraries.."
if not exist $(TargetDir)gdal_translate.exe copy C:\SDK\gdal\modis_library\gdal_translate.exe $(TargetDir)gdal_translate.exe
if not exist $(TargetDir)gdal203.dll copy C:\SDK\gdal\modis_library\gdal203.dll $(TargetDir)gdal203.dll
if not exist $(TargetDir)geos_c.dll copy C:\SDK\gdal\modis_library\geos_c.dll $(TargetDir)geos_c.dll
if not exist $(TargetDir)hdf.dll copy C:\SDK\gdal\modis_library\hdf.dll $(TargetDir)hdf.dll
if not exist $(TargetDir)hdf5.dll copy C:\SDK\gdal\modis_library\hdf5.dll $(TargetDir)hdf5.dll
if not exist $(TargetDir)hdf5_hl.dll copy C:\SDK\gdal\modis_library\hdf5_hl.dll $(TargetDir)hdf5_hl.dll
if not exist $(TargetDir)libcurl.dll copy C:\SDK\gdal\modis_library\libcurl.dll $(TargetDir)libcurl.dll
if not exist $(TargetDir)mfhdf.dll copy C:\SDK\gdal\modis_library\mfhdf.dll $(TargetDir)mfhdf.dll
if not exist $(TargetDir)netcdf.dll copy C:\SDK\gdal\modis_library\netcdf.dll $(TargetDir)netcdf.dll
if not exist $(TargetDir)xdr.dll copy C:\SDK\gdal\modis_library\xdr.dll $(TargetDir)xdr.dll
if not exist $(TargetDir)zlib1.dll copy C:\SDK\gdal\modis_library\zlib1.dll $(TargetDir)zlib1.dll

echo "..copy HEG libraries.."
if not exist $(TargetDir)HEG_Win\bin xcopy C:\SDK\HEG_Win\bin\* $(TargetDir)HEG_Win\bin /y /e /i
if not exist $(TargetDir)HEG_Win\data xcopy C:\SDK\HEG_Win\data\* $(TargetDir)HEG_Win\data /y /e /i
if not exist $(TargetDir)HEG_Win\TOOLKIT_MTD xcopy C:\SDK\HEG_Win\TOOLKIT_MTD\* $(TargetDir)HEG_Win\TOOLKIT_MTD /y /e /i

echo "..copy MRT files.."
if not exist $(TargetDir)MRT\bin xcopy C:\SDK\MRT_Win\bin\* $(TargetDir)MRT\bin /y /e /i
if not exist $(TargetDir)MRT\data xcopy C:\SDK\MRT_Win\data\* $(TargetDir)MRT\data /y /e /i

echo "..copy ymlsamples and templates.."
if not exist $(TargetDir)ymlsamples xcopy $(SolutionDir)ymlsamples\* $(TargetDir)ymlsamples /y /e /i
if not exist $(TargetDir)templates xcopy $(SolutionDir)templates\* $(TargetDir)templates /y /e /i

echo "..copy external executables.."
if not exist $(TargetDir)dos2unix.exe copy $(SolutionDir)support\dos2unix.exe $(TargetDir)dos2unix.exe
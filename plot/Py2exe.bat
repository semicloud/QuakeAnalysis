rem Use this bat file in 适用于 VS 2017 的 x64 本机工具命令提示, require vs 2017

"C:\Program Files\Python36\python.exe" "C:\Program Files\Python36\Lib\site-packages\cython.py" modis_plot.py --embed
cl.exe  /nologo /Ox /MD /W3 /GS- /DNDEBUG -I"C:\Program Files\Python36\include" -I"C:\Program Files\Python36\PC" /Tcmodis_plot.c /link /OUT:"modis_plot.exe" /SUBSYSTEM:CONSOLE /MACHINE:X64 /LIBPATH:"C:\Program Files\Python36\libs" /LIBPATH:"C:\Program Files\Python36\PCbuild"

rem clear temp files
del *.c
del *.exp
del *.obj
del *.lib
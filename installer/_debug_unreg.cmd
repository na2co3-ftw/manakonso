pushd %~dp0
regsvr32 /s /u ..\Win32\Debug\immanatip.dll
regsvr32 /s /u ..\x64\Debug\immanatip.dll
popd

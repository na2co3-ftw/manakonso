pushd %~dp0
regsvr32 /s ..\Win32\Debug\immanatip.dll
regsvr32 /s ..\x64\Debug\immanatip.dll
popd

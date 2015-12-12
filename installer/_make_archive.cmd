@echo off
setlocal

pushd %~dp0

call _version.cmd

del "%TARGETDIR%\config-sample.zip"

pushd config-sample
"C:\Program Files\7-Zip\7z.exe" a -tzip -mtc=off "..\%TARGETDIR%\config-sample.zip" *.xml *.txt
popd

del "%TARGETDIR%\manakonso-%VERSION%.zip"

pushd "%TARGETDIR%"
"C:\Program Files\7-Zip\7z.exe" a -tzip -mtc=off manakonso-%VERSION%.zip manakonso-%VERSION%-x86.msi manakonso-%VERSION%-x64.msi ..\README.TXT ..\..\LICENSE.TXT config-sample.zip
popd

popd

endlocal

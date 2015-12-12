@echo off
setlocal

pushd %~dp0

call _version.cmd

"%WIX%bin\candle.exe" manakonso-x64.wxs -nologo -out "%TARGETDIR%\manakonso-%VERSION%-x64.wixobj"
"%WIX%bin\light.exe" "%TARGETDIR%\manakonso-%VERSION%-x64.wixobj" -nologo -out "%TARGETDIR%\manakonso-%VERSION%-x64.msi" -ext bin\WixUIExtension.dll -sw1056

popd

endlocal

@echo off
setlocal

pushd "%~dp0"

call _version.cmd

"%WIX%bin\candle.exe" installer-bundle.wxs -nologo -out "%TARGETDIR%\manakonso-%VERSION%.wixobj" -ext WixBalExtension -ext WixUtilExtension
"%WIX%bin\light.exe" "%TARGETDIR%\manakonso-%VERSION%.wixobj" -nologo -out "%TARGETDIR%\manakonso-%VERSION%.exe" -ext WixBalExtension -ext WixUtilExtension

popd

endlocal

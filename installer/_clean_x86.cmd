@echo off
setlocal

pushd %~dp0

call _version.cmd

del "%TARGETDIR%\manakonso-%VERSION%-x86.*"

popd

endlocal

@echo off
setlocal

pushd %~dp0

set DIGESTALGORITHM=%1
set TIMESTAMPFLAG=%2
set TIMESTAMPSERVER=%3
set PFXFILE=%4
set PASSWORD=%5

call "%VS140COMNTOOLS%VsDevCmd.bat"

call _version.cmd

set BINFILES="..\Win32\Release\*.dll" "..\Win32\Release\*.exe" "..\x64\Release\*.dll" "..\x64\Release\*.exe"
set MSIFILES="build\manakonso-%VERSION%-x86.msi" "build\manakonso-%VERSION%-x64.msi"

call _clean.cmd

set SIGNCOMMAND=signtool sign /v /d "manakonso" /fd %DIGESTALGORITHM% /f %PFXFILE% /p %PASSWORD%

if "%TIMESTAMPFLAG%"=="t" (
  set SIGNCOMMAND=%SIGNCOMMAND% /t %TIMESTAMPSERVER%
) else if "%TIMESTAMPFLAG%"=="tr" (
  set SIGNCOMMAND=%SIGNCOMMAND% /tr %TIMESTAMPSERVER% /td %DIGESTALGORITHM%
)

%SIGNCOMMAND% %BINFILES%

call _build_x86.cmd
call _build_x64.cmd

%SIGNCOMMAND% %MSIFILES%

popd

endlocal

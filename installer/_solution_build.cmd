@echo off
setlocal

pushd "%~dp0"

call "%VS140COMNTOOLS%VsDevCmd.bat"

msbuild "..\manakonso.sln" /nologo /maxcpucount /verbosity:normal /target:Build /property:Configuration=Release,Platform=Win32
msbuild "..\manakonso.sln" /nologo /maxcpucount /verbosity:normal /target:Build /property:Configuration=Release,Platform=x64

popd

endlocal

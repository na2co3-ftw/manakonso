@echo off
setlocal

pushd "%~dp0"

call _version.cmd

if not exist "%TARGETDIR%" mkdir "%TARGETDIR%"

pushd ..

set DESCRIPTION=京極入致

"%LocalAppData%\Pandoc\pandoc.exe" -s -f markdown_github-ascii_identifiers -t html5 -V lang:"ja" -V title-prefix:"%DESCRIPTION%" -V pagetitle:"使用説明書" -V title:"%DESCRIPTION% 使用説明書" --self-contained -c "installer\resource-md\markdown.css" --columns=1024 --toc -o "installer\%TARGETDIR%\README.html" README.md

popd

copy /y /b "..\LICENSE.TXT" "%TARGETDIR%\LICENSE.txt" > nul
copy /y /b "config-share\config.xml" "%TARGETDIR%\config.xml" > nul

pushd "%TARGETDIR%"

copy /b LICENSE.txt + > nul
copy /b config.xml + > nul

popd

popd

endlocal

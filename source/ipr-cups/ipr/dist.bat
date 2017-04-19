@echo off
rem copy binaries to the distribution dir
set DIST=..\dist\win32
mkdir %DIST%
for %%f in (release\ipr.exe install.bat readme.txt license.txt ipp-template ipp-narrowjobs) do xcopy /y %%f %DIST%

@echo off
rem installation program for IPR utilities
if %1. == . goto usage
if not exist %1 mkdir %1
echo -- Copying executables to %1
echo ipr.exe
copy ipr.exe %1 > nul
echo ipq.exe
copy ipr.exe %1\ipq.exe > nul
echo ipc.exe
copy ipr.exe %1\ipc.exe > nul
echo iprm.exe
copy ipr.exe %1\iprm.exe > nul
echo -- Copying executables done
echo -- Copying template files to %1. 
if not exist %1\.ipp-template goto hop
if not %2. == . goto hop
echo *** %1\ipp-template exists. To overwrite it and other template files, run:
echo  %0 %1 -o
goto end
:hop
for %%f in (ipp-*) do copy %%f "%HOMEDRIVE%%HOMEPATH%\.%%f"
echo -- Installation finished
echo For the ipr, ipq, iprm, ipc programs to be accessible, you
echo have to add %1 to your path.
echo -- 
rem this applies to cygwin and HOME env. var.:
rem echo For the IPR programs to access ipp-template files, please set
rem echo your HOME environment variable to point to %1 or move ipp-template*
rem echo files to a directory your HOME points to.
pause
goto end
:usage
echo usage %0 install_directory
:end

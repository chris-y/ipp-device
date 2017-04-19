echo building release
nmake -f ipr.mak CFG="ipr - Win32 Release" RECURSE=1 
cd Release
copy ipr.exe ipq.exe
copy ipr.exe iprm.exe
copy ipr.exe ipc.exe
cd ..

rem nmake -f ipr.mak CFG="fvdlfilter - Win32 Debug" RECURSE=1 


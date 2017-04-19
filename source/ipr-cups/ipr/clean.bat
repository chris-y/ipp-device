nmake -f ipr.mak CFG="ipr - Win32 Release" RECURSE=1 clean
nmake -f ipr.mak CFG="ipr - Win32 Debug" RECURSE=1 clean
rd /s /q Debug
rd /s /q Release
rd /s /q ..\cups\Debug
rd /s /q ..\cups\Release
rem remove packaged product
rd /s /q ..\dist
rem delete misc. msvc stuff
del ipr.plg
rem rd /s /q ipr
rem del ipr.zip
del ipr.ncb
del ipr.opt
del ipr.plg

# Microsoft Developer Studio Generated NMAKE File, Based on cups.dsp
!IF "$(CFG)" == ""
CFG=cups - Win32 Debug
!MESSAGE No configuration specified. Defaulting to cups - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "cups - Win32 Release" && "$(CFG)" != "cups - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cups.mak" CFG="cups - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cups - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "cups - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "cups - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : ".\cups.lib"


CLEAN :
	-@erase "$(INTDIR)\auth.obj"
	-@erase "$(INTDIR)\dest.obj"
	-@erase "$(INTDIR)\emit.obj"
	-@erase "$(INTDIR)\encode.obj"
	-@erase "$(INTDIR)\getputfile.obj"
	-@erase "$(INTDIR)\http-addr.obj"
	-@erase "$(INTDIR)\http-support.obj"
	-@erase "$(INTDIR)\http.obj"
	-@erase "$(INTDIR)\ipp-support.obj"
	-@erase "$(INTDIR)\ipp.obj"
	-@erase "$(INTDIR)\language.obj"
	-@erase "$(INTDIR)\mark.obj"
	-@erase "$(INTDIR)\md5.obj"
	-@erase "$(INTDIR)\md5passwd.obj"
	-@erase "$(INTDIR)\options.obj"
	-@erase "$(INTDIR)\page.obj"
	-@erase "$(INTDIR)\ppd.obj"
	-@erase "$(INTDIR)\snprintf.obj"
	-@erase "$(INTDIR)\string.obj"
	-@erase "$(INTDIR)\tempfile.obj"
	-@erase "$(INTDIR)\usersys.obj"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase ".\cups.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "../visualc" /I ".." /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\cups.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\cups.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"cups.lib" 
LIB32_OBJS= \
	"$(INTDIR)\auth.obj" \
	"$(INTDIR)\dest.obj" \
	"$(INTDIR)\emit.obj" \
	"$(INTDIR)\encode.obj" \
	"$(INTDIR)\getputfile.obj" \
	"$(INTDIR)\http-addr.obj" \
	"$(INTDIR)\http-support.obj" \
	"$(INTDIR)\http.obj" \
	"$(INTDIR)\ipp-support.obj" \
	"$(INTDIR)\ipp.obj" \
	"$(INTDIR)\language.obj" \
	"$(INTDIR)\mark.obj" \
	"$(INTDIR)\md5.obj" \
	"$(INTDIR)\md5passwd.obj" \
	"$(INTDIR)\options.obj" \
	"$(INTDIR)\page.obj" \
	"$(INTDIR)\ppd.obj" \
	"$(INTDIR)\snprintf.obj" \
	"$(INTDIR)\string.obj" \
	"$(INTDIR)\tempfile.obj" \
	"$(INTDIR)\usersys.obj" \
	"$(INTDIR)\util.obj"

".\cups.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "cups - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : ".\cupsd.lib"


CLEAN :
	-@erase "$(INTDIR)\auth.obj"
	-@erase "$(INTDIR)\dest.obj"
	-@erase "$(INTDIR)\emit.obj"
	-@erase "$(INTDIR)\encode.obj"
	-@erase "$(INTDIR)\getputfile.obj"
	-@erase "$(INTDIR)\http-addr.obj"
	-@erase "$(INTDIR)\http-support.obj"
	-@erase "$(INTDIR)\http.obj"
	-@erase "$(INTDIR)\ipp-support.obj"
	-@erase "$(INTDIR)\ipp.obj"
	-@erase "$(INTDIR)\language.obj"
	-@erase "$(INTDIR)\mark.obj"
	-@erase "$(INTDIR)\md5.obj"
	-@erase "$(INTDIR)\md5passwd.obj"
	-@erase "$(INTDIR)\options.obj"
	-@erase "$(INTDIR)\page.obj"
	-@erase "$(INTDIR)\ppd.obj"
	-@erase "$(INTDIR)\snprintf.obj"
	-@erase "$(INTDIR)\string.obj"
	-@erase "$(INTDIR)\tempfile.obj"
	-@erase "$(INTDIR)\usersys.obj"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase ".\cupsd.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /Gm /GX /Zi /Od /I "../visualc" /I ".." /D "DEBUG" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\cups.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\cups.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"cupsd.lib" 
LIB32_OBJS= \
	"$(INTDIR)\auth.obj" \
	"$(INTDIR)\dest.obj" \
	"$(INTDIR)\emit.obj" \
	"$(INTDIR)\encode.obj" \
	"$(INTDIR)\getputfile.obj" \
	"$(INTDIR)\http-addr.obj" \
	"$(INTDIR)\http-support.obj" \
	"$(INTDIR)\http.obj" \
	"$(INTDIR)\ipp-support.obj" \
	"$(INTDIR)\ipp.obj" \
	"$(INTDIR)\language.obj" \
	"$(INTDIR)\mark.obj" \
	"$(INTDIR)\md5.obj" \
	"$(INTDIR)\md5passwd.obj" \
	"$(INTDIR)\options.obj" \
	"$(INTDIR)\page.obj" \
	"$(INTDIR)\ppd.obj" \
	"$(INTDIR)\snprintf.obj" \
	"$(INTDIR)\string.obj" \
	"$(INTDIR)\tempfile.obj" \
	"$(INTDIR)\usersys.obj" \
	"$(INTDIR)\util.obj"

".\cupsd.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("cups.dep")
!INCLUDE "cups.dep"
!ELSE 
!MESSAGE Warning: cannot find "cups.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "cups - Win32 Release" || "$(CFG)" == "cups - Win32 Debug"
SOURCE=.\auth.c

"$(INTDIR)\auth.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\dest.c

"$(INTDIR)\dest.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\emit.c

"$(INTDIR)\emit.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\encode.c

"$(INTDIR)\encode.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\getputfile.c

"$(INTDIR)\getputfile.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=".\http-addr.c"

"$(INTDIR)\http-addr.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=".\http-support.c"

"$(INTDIR)\http-support.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\http.c

"$(INTDIR)\http.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=".\ipp-support.c"

"$(INTDIR)\ipp-support.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ipp.c

"$(INTDIR)\ipp.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\language.c

"$(INTDIR)\language.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\mark.c

"$(INTDIR)\mark.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\md5.c

"$(INTDIR)\md5.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\md5passwd.c

"$(INTDIR)\md5passwd.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\options.c

"$(INTDIR)\options.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\page.c

"$(INTDIR)\page.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ppd.c

"$(INTDIR)\ppd.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\snprintf.c

"$(INTDIR)\snprintf.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\string.c

"$(INTDIR)\string.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tempfile.c

"$(INTDIR)\tempfile.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\usersys.c

"$(INTDIR)\usersys.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\util.c

"$(INTDIR)\util.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 


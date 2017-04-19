# Microsoft Developer Studio Generated NMAKE File, Based on ipr.dsp
!IF "$(CFG)" == ""
CFG=ipr - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ipr - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ipr - Win32 Release" && "$(CFG)" != "ipr - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ipr.mak" CFG="ipr - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ipr - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "ipr - Win32 Debug" (based on "Win32 (x86) Console Application")
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

!IF  "$(CFG)" == "ipr - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\ipr.exe"

!ELSE 

ALL : "cups - Win32 Release" "$(OUTDIR)\ipr.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"cups - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\AttrGroups.obj"
	-@erase "$(INTDIR)\ipr.obj"
	-@erase "$(INTDIR)\iprutil.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\ipr.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "../visualc" /I ".." /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\ipr.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ipr.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\ipr.pdb" /machine:I386 /out:"$(OUTDIR)\ipr.exe" 
LINK32_OBJS= \
	"$(INTDIR)\AttrGroups.obj" \
	"$(INTDIR)\ipr.obj" \
	"$(INTDIR)\iprutil.obj" \
	"..\cups\cups.lib"

"$(OUTDIR)\ipr.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ipr - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\ipr.exe" "$(OUTDIR)\ipr.bsc"

!ELSE 

ALL : "cups - Win32 Debug" "$(OUTDIR)\ipr.exe" "$(OUTDIR)\ipr.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"cups - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\AttrGroups.obj"
	-@erase "$(INTDIR)\AttrGroups.sbr"
	-@erase "$(INTDIR)\ipr.obj"
	-@erase "$(INTDIR)\ipr.sbr"
	-@erase "$(INTDIR)\iprutil.obj"
	-@erase "$(INTDIR)\iprutil.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\ipr.bsc"
	-@erase "$(OUTDIR)\ipr.exe"
	-@erase "$(OUTDIR)\ipr.ilk"
	-@erase "$(OUTDIR)\ipr.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /Gm /GX /ZI /Od /I "../visualc" /I ".." /D "DEBUG" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ipr.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ipr.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AttrGroups.sbr" \
	"$(INTDIR)\ipr.sbr" \
	"$(INTDIR)\iprutil.sbr"

"$(OUTDIR)\ipr.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\ipr.pdb" /debug /machine:I386 /out:"$(OUTDIR)\ipr.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\AttrGroups.obj" \
	"$(INTDIR)\ipr.obj" \
	"$(INTDIR)\iprutil.obj" \
	"..\cups\cupsd.lib"

"$(OUTDIR)\ipr.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
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
!IF EXISTS("ipr.dep")
!INCLUDE "ipr.dep"
!ELSE 
!MESSAGE Warning: cannot find "ipr.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ipr - Win32 Release" || "$(CFG)" == "ipr - Win32 Debug"
SOURCE=.\AttrGroups.cpp

!IF  "$(CFG)" == "ipr - Win32 Release"


"$(INTDIR)\AttrGroups.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipr - Win32 Debug"


"$(INTDIR)\AttrGroups.obj"	"$(INTDIR)\AttrGroups.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ipr.cpp

!IF  "$(CFG)" == "ipr - Win32 Release"


"$(INTDIR)\ipr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipr - Win32 Debug"


"$(INTDIR)\ipr.obj"	"$(INTDIR)\ipr.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\iprutil.cpp

!IF  "$(CFG)" == "ipr - Win32 Release"


"$(INTDIR)\iprutil.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipr - Win32 Debug"


"$(INTDIR)\iprutil.obj"	"$(INTDIR)\iprutil.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

!IF  "$(CFG)" == "ipr - Win32 Release"

"cups - Win32 Release" : 
   cd "..\cups"
   $(MAKE) /$(MAKEFLAGS) /F ".\cups.mak" CFG="cups - Win32 Release" 
   cd "..\ipr"

"cups - Win32 ReleaseCLEAN" : 
   cd "..\cups"
   $(MAKE) /$(MAKEFLAGS) /F ".\cups.mak" CFG="cups - Win32 Release" RECURSE=1 CLEAN 
   cd "..\ipr"

!ELSEIF  "$(CFG)" == "ipr - Win32 Debug"

"cups - Win32 Debug" : 
   cd "..\cups"
   $(MAKE) /$(MAKEFLAGS) /F ".\cups.mak" CFG="cups - Win32 Debug" 
   cd "..\ipr"

"cups - Win32 DebugCLEAN" : 
   cd "..\cups"
   $(MAKE) /$(MAKEFLAGS) /F ".\cups.mak" CFG="cups - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\ipr"

!ENDIF 


!ENDIF 


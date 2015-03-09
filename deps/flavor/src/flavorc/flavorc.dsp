# Microsoft Developer Studio Project File - Name="flavorc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=flavorc - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "flavorc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "flavorc.mak" CFG="flavorc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "flavorc - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "flavorc - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "flavorc - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "../../include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# Begin Special Build Tool
ProjDir=.
SOURCE="$(InputPath)"
PostBuild_Cmds=copy $(ProjDir)\Release\flavorc.exe ..\..\bin
# End Special Build Tool

!ELSEIF  "$(CFG)" == "flavorc - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "flavorc___Win32_Debug"
# PROP BASE Intermediate_Dir "flavorc___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# Begin Special Build Tool
ProjDir=.
SOURCE="$(InputPath)"
PostBuild_Cmds=copy $(ProjDir)\Debug\flavorc.exe ..\..\bin
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "flavorc - Win32 Release"
# Name "flavorc - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\carray.c
# End Source File
# Begin Source File

SOURCE=.\cbac.c
# End Source File
# Begin Source File

SOURCE=.\cclass.c
# End Source File
# Begin Source File

SOURCE=.\cexpr.c
# End Source File
# Begin Source File

SOURCE=.\cgetput.c
# End Source File
# Begin Source File

SOURCE=.\check.c
# End Source File
# Begin Source File

SOURCE=.\cid.c
# End Source File
# Begin Source File

SOURCE=.\cmap.c
# End Source File
# Begin Source File

SOURCE=.\compile.c
# End Source File
# Begin Source File

SOURCE=.\cout.c
# End Source File
# Begin Source File

SOURCE=.\cpragma.c
# End Source File
# Begin Source File

SOURCE=.\ctrace.c
# End Source File
# Begin Source File

SOURCE=.\cvar.c
# End Source File
# Begin Source File

SOURCE=.\cverbatim.c
# End Source File
# Begin Source File

SOURCE=.\cxml.c
# End Source File
# Begin Source File

SOURCE=.\error.c
# End Source File
# Begin Source File

SOURCE=.\eval.c
# End Source File
# Begin Source File

SOURCE=.\lex.yy.c
# End Source File
# Begin Source File

SOURCE=.\main.c
# End Source File
# Begin Source File

SOURCE=.\ptree.c
# End Source File
# Begin Source File

SOURCE=.\symbol.c
# End Source File
# Begin Source File

SOURCE=.\util.c
# End Source File
# Begin Source File

SOURCE=.\y.tab.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\carray.h
# End Source File
# Begin Source File

SOURCE=.\cbac.h
# End Source File
# Begin Source File

SOURCE=.\cclass.h
# End Source File
# Begin Source File

SOURCE=.\cdef.h
# End Source File
# Begin Source File

SOURCE=.\cexpr.h
# End Source File
# Begin Source File

SOURCE=.\cgetput.h
# End Source File
# Begin Source File

SOURCE=.\check.h
# End Source File
# Begin Source File

SOURCE=.\cid.h
# End Source File
# Begin Source File

SOURCE=.\cmap.h
# End Source File
# Begin Source File

SOURCE=.\compile.h
# End Source File
# Begin Source File

SOURCE=.\cout.h
# End Source File
# Begin Source File

SOURCE=.\cpragma.h
# End Source File
# Begin Source File

SOURCE=.\ctrace.h
# End Source File
# Begin Source File

SOURCE=.\cvar.h
# End Source File
# Begin Source File

SOURCE=.\cverbatim.h
# End Source File
# Begin Source File

SOURCE=.\cxml.h
# End Source File
# Begin Source File

SOURCE=.\error.h
# End Source File
# Begin Source File

SOURCE=.\eval.h
# End Source File
# Begin Source File

SOURCE=.\globals.h
# End Source File
# Begin Source File

SOURCE=.\ptree.h
# End Source File
# Begin Source File

SOURCE=.\symbol.h
# End Source File
# Begin Source File

SOURCE=.\util.h
# End Source File
# Begin Source File

SOURCE=.\y.tab.h
# End Source File
# End Group
# End Target
# End Project

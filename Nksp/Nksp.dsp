# Microsoft Developer Studio Project File - Name="Nksp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Nksp - Win32 FinalDebug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Nksp.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Nksp.mak" CFG="Nksp - Win32 FinalDebug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Nksp - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Nksp - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "Nksp - Win32 FinalRelease" (based on "Win32 (x86) Application")
!MESSAGE "Nksp - Win32 FinalDebug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/aboard_WildPet_UI_USA/All/Nksp", EULBAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Nksp - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"StdH.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib EntitiesMP.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib ws2_32.lib /nologo /subsystem:windows /profile /map /debug /machine:I386 /nodefaultlib:"game.lib" /nodefaultlib:"gamemp.lib"
# Begin Custom Build - Copying $(InputName) binaries to $(a60911_dir)\Bin
InputPath=.\Release\Nksp.exe
InputName=Nksp
SOURCE="$(InputPath)"

"$(a60911_dir)\Bin\$(InputName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy Release\$(InputName).exe $(a60911_dir)\Bin >nul 
	copy Release\$(InputName).map $(a60911_dir)\Bin >nul 
	copy Release\$(InputName).pdb $(a60911_dir)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Nksp - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /G5 /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_WINDLL" /D "_USRDLL" /Fr /Yu"StdH.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib Winmm.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"gamed.lib" /nodefaultlib:"gamempd.lib" /pdbtype:sept
# SUBTRACT LINK32 /profile /pdb:none /incremental:no
# Begin Custom Build - Copying $(InputName) binaries to $(a60911_dir)\Bin\Debug
InputPath=.\Debug\Nksp.exe
InputName=Nksp
SOURCE="$(InputPath)"

"$(a60911_dir)\Bin\Debug\$(InputName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy Debug\$(InputName).exe $(a60911_dir)\Bin\Debug >nul 
	copy Debug\$(InputName).map $(a60911_dir)\Bin\Debug >nul 
	copy Debug\$(InputName).pdb $(a60911_dir)\Bin\Debug >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Nksp - Win32 FinalRelease"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Nksp___Win32_FinalRelease"
# PROP BASE Intermediate_Dir "Nksp___Win32_FinalRelease"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "FinalRelease"
# PROP Intermediate_Dir "FinalRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"StdH.h" /FD /c
# ADD CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "FINALVERSION" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"StdH.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"game.lib gamemp.lib"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"game.lib gamemp.lib"
# Begin Custom Build - Copying $(InputName) binaries to $(a60911_dir)\Bin
InputPath=.\FinalRelease\Nksp.exe
InputName=Nksp
SOURCE="$(InputPath)"

"$(a60911_dir)\Bin\$(InputName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy FinalRelease\$(InputName).exe $(a60911_dir)\Bin >nul 
	copy FinalRelease\$(InputName).map $(a60911_dir)\Bin >nul 
	copy FinalRelease\$(InputName).pdb $(a60911_dir)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Nksp - Win32 FinalDebug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Nksp___Win32_FinalDebug"
# PROP BASE Intermediate_Dir "Nksp___Win32_FinalDebug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "FinalDebug"
# PROP Intermediate_Dir "FinalDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_WINDLL" /D "_USRDLL" /Fr /Yu"StdH.h" /FD /c
# ADD CPP /nologo /G5 /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /D "FINALVERSION" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_WINDLL" /D "_USRDLL" /Fr /Yu"StdH.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"gamed.lib gamempd.lib" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none /incremental:no
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"gamed.lib gamempd.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none /incremental:no
# Begin Custom Build - Copying $(InputName) binaries to $(a60911_dir)\Bin\Debug
InputPath=.\FinalDebug\Nksp.exe
InputName=Nksp
SOURCE="$(InputPath)"

"$(a60911_dir)\Bin\Debug\$(InputName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy FinalDebug\$(InputName).exe $(a60911_dir)\Bin\Debug >nul 
	copy FinalDebug\$(InputName).map $(a60911_dir)\Bin\Debug >nul 
	copy FinalDebug\$(InputName).pdb $(a60911_dir)\Bin\Debug >nul 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "Nksp - Win32 Release"
# Name "Nksp - Win32 Debug"
# Name "Nksp - Win32 FinalRelease"
# Name "Nksp - Win32 FinalDebug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CmdLine.cpp
# End Source File
# Begin Source File

SOURCE=.\GLSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\MainWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\Nksp.cpp
# End Source File
# Begin Source File

SOURCE=.\SplashScreen.cpp
# End Source File
# Begin Source File

SOURCE=.\StdH.cpp
# ADD CPP /Yc"StdH.h"
# End Source File
# Begin Source File

SOURCE=.\VarList.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CDCheck.h
# End Source File
# Begin Source File

SOURCE=.\CmdLine.h
# End Source File
# Begin Source File

SOURCE=.\GLSettings.h
# End Source File
# Begin Source File

SOURCE=.\MainWindow.h
# End Source File
# Begin Source File

SOURCE=.\Nksp.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\SplashScreen.h
# End Source File
# Begin Source File

SOURCE=.\StdH.h
# End Source File
# Begin Source File

SOURCE=.\VarList.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\cur_00.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur_01.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur_02.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur_03.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur_04.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur_05.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur_06.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur_07.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur_08.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur_09.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur_10.cur
# End Source File
# Begin Source File

SOURCE=.\res\lastwar.ico
# End Source File
# Begin Source File

SOURCE=.\res\Nksp.ico
# End Source File
# Begin Source File

SOURCE=.\Nksp.rc
# End Source File
# Begin Source File

SOURCE=.\nocursor.cur
# End Source File
# Begin Source File

SOURCE=.\Splash.bmp
# End Source File
# Begin Source File

SOURCE=.\SplashMask.bmp
# End Source File
# End Group
# Begin Group "Scripts"

# PROP Default_Filter ""
# End Group
# Begin Group "Texts"

# PROP Default_Filter ""
# End Group
# Begin Source File

SOURCE=.\Nksp.exe.manifest
# End Source File
# End Target
# End Project

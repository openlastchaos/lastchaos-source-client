# Microsoft Developer Studio Project File - Name="GameMP" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=GameMP - Win32 FinalDebug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GameMP.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GameMP.mak" CFG="GameMP - Win32 FinalDebug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GameMP - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameMP - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameMP - Win32 FinalRelease" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GameMP - Win32 FinalDebug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/aboard_WildPet_UI_USA/All/GameMP", NTLBAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GameMP - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "GAME_EXPORTS" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Oa /Ow /Gf /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 user32.lib EntitiesMP.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(a60911_dir)\Bin
InputPath=.\Release\GameMP.dll
InputName=GameMP
SOURCE="$(InputPath)"

"$(a60911_dir)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy Release\$(InputName).dll $(a60911_dir)\Bin >nul 
	copy Release\$(InputName).map $(a60911_dir)\Bin >nul 
	copy Release\$(InputName).lib $(a60911_dir)\Bin >nul 
	copy Release\$(InputName).pdb $(a60911_dir)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameMP - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /G5 /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "GAME_EXPORTS" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 user32.lib /nologo /subsystem:windows /dll /map /debug /machine:I386 /out:"Debug/GameMPD.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Copying $(InputName) binaries to $(a60911_dir)\Bin\Debug
InputPath=.\Debug\GameMPD.dll
InputName=GameMPD
SOURCE="$(InputPath)"

"$(a60911_dir)\Bin\Debug\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy Debug\$(InputName).dll $(a60911_dir)\Bin\Debug >nul 
	copy Debug\$(InputName).map $(a60911_dir)\Bin\Debug >nul 
	copy Debug\$(InputName).lib $(a60911_dir)\Bin\Debug >nul 
	copy Debug\$(InputName).pdb $(a60911_dir)\Bin\Debug >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameMP - Win32 FinalRelease"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameMP___Win32_FinalRelease"
# PROP BASE Intermediate_Dir "GameMP___Win32_FinalRelease"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "FinalRelease"
# PROP Intermediate_Dir "FinalRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "GAME_EXPORTS" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Oa /Ow /Gf /Fr
# ADD CPP /nologo /G5 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "FINALVERSION" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "GAME_EXPORTS" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Oa /Ow /Gf /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib /nologo /subsystem:windows /dll /map /debug /machine:I386 /pdbtype:sept
# SUBTRACT BASE LINK32 /profile /incremental:yes
# ADD LINK32 user32.lib /nologo /subsystem:windows /dll /map /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /profile /incremental:yes
# Begin Custom Build - Copying $(InputName) binaries to $(a60911_dir)\Bin
InputPath=.\FinalRelease\GameMP.dll
InputName=GameMP
SOURCE="$(InputPath)"

"$(a60911_dir)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy FinalRelease\$(InputName).dll $(a60911_dir)\Bin >nul 
	copy FinalRelease\$(InputName).map $(a60911_dir)\Bin >nul 
	copy FinalRelease\$(InputName).lib $(a60911_dir)\Bin >nul 
	copy FinalRelease\$(InputName).pdb $(a60911_dir)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "GameMP - Win32 FinalDebug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "GameMP___Win32_FinalDebug"
# PROP BASE Intermediate_Dir "GameMP___Win32_FinalDebug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "FinalDebug"
# PROP Intermediate_Dir "FinalDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "GAME_EXPORTS" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G5 /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /D "FINALVERSION" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "GAME_EXPORTS" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib /nologo /subsystem:windows /dll /map /debug /machine:I386 /out:"Debug/GameMPD.dll" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 user32.lib /nologo /subsystem:windows /dll /map /debug /machine:I386 /out:"FinalDebug/GameMPD.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Copying $(InputName) binaries to $(a60911_dir)\Bin\Debug
InputPath=.\FinalDebug\GameMPD.dll
InputName=GameMPD
SOURCE="$(InputPath)"

"$(a60911_dir)\Bin\Debug\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy FinalDebug\$(InputName).dll $(a60911_dir)\Bin\Debug >nul 
	copy FinalDebug\$(InputName).map $(a60911_dir)\Bin\Debug >nul 
	copy FinalDebug\$(InputName).lib $(a60911_dir)\Bin\Debug >nul 
	copy FinalDebug\$(InputName).pdb $(a60911_dir)\Bin\Debug >nul 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "GameMP - Win32 Release"
# Name "GameMP - Win32 Debug"
# Name "GameMP - Win32 FinalRelease"
# Name "GameMP - Win32 FinalDebug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\Console.cpp
# End Source File
# Begin Source File

SOURCE=.\Controls.cpp
# End Source File
# Begin Source File

SOURCE=.\Game.cpp
# End Source File
# Begin Source File

SOURCE=.\LCDDrawing.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadingHook.cpp
# End Source File
# Begin Source File

SOURCE=.\Map.cpp
# End Source File
# Begin Source File

SOURCE=.\SessionProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\WEDInterface.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\Game.h
# End Source File
# Begin Source File

SOURCE=.\LCDDrawing.h
# End Source File
# Begin Source File

SOURCE=.\PlayerSettings.h
# End Source File
# Begin Source File

SOURCE=.\SEColors.h
# End Source File
# Begin Source File

SOURCE=.\SessionProperties.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# End Target
# End Project

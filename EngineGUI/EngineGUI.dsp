# Microsoft Developer Studio Project File - Name="EngineGUI" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=EngineGUI - Win32 FinalDebug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "EngineGUI.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "EngineGUI.mak" CFG="EngineGUI - Win32 FinalDebug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "EngineGUI - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "EngineGUI - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "EngineGUI - Win32 FinalRelease" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "EngineGUI - Win32 FinalDebug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/aboard_WildPet_UI_USA/All/EngineGUI", JQNBAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "EngineGUI - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ENGINEGUI_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G6 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ENGINEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"StdH.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(a60911_dir)\Bin
InputPath=.\Release\EngineGUI.dll
InputName=EngineGUI
SOURCE="$(InputPath)"

"$(a60911_dir)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy Release\$(InputName).dll $(a60911_dir)\Bin >nul 
	copy Release\$(InputName).map $(a60911_dir)\Bin >nul 
	copy Release\$(InputName).lib $(a60911_dir)\Bin >nul 
	copy Release\$(InputName).pdb $(a60911_dir)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EngineGUI - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ENGINEGUI_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "ENGINEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"StdH.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /dll /map /debug /machine:I386 /out:"Debug/EngineGUID.dll" /pdbtype:sept
# SUBTRACT LINK32 /profile /incremental:no
# Begin Custom Build - Copying $(InputName) binaries to $(a60911_dir)\Bin\Debug
InputPath=.\Debug\EngineGUID.dll
InputName=EngineGUID
SOURCE="$(InputPath)"

"$(a60911_dir)\Bin\Debug\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy Debug\$(InputName).dll $(a60911_dir)\Bin\Debug >nul 
	copy Debug\$(InputName).map $(a60911_dir)\Bin\Debug >nul 
	copy Debug\$(InputName).lib $(a60911_dir)\Bin\Debug >nul 
	copy Debug\$(InputName).pdb $(a60911_dir)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EngineGUI - Win32 FinalRelease"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "EngineGUI___Win32_FinalRelease"
# PROP BASE Intermediate_Dir "EngineGUI___Win32_FinalRelease"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "FinalRelease"
# PROP Intermediate_Dir "FinalRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ENGINEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"StdH.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G6 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "FINALVERSION" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ENGINEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"StdH.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /map /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /dll /map /debug /machine:I386 /pdbtype:sept
# Begin Custom Build - Copying $(InputName) binaries to $(a60911_dir)\Bin
InputPath=.\FinalRelease\EngineGUI.dll
InputName=EngineGUI
SOURCE="$(InputPath)"

"$(a60911_dir)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy FinalRelease\$(InputName).dll $(a60911_dir)\Bin >nul 
	copy FinalRelease\$(InputName).map $(a60911_dir)\Bin >nul 
	copy FinalRelease\$(InputName).lib $(a60911_dir)\Bin >nul 
	copy FinalRelease\$(InputName).pdb $(a60911_dir)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EngineGUI - Win32 FinalDebug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "EngineGUI___Win32_FinalDebug"
# PROP BASE Intermediate_Dir "EngineGUI___Win32_FinalDebug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "FinalDebug"
# PROP Intermediate_Dir "FinalDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "ENGINEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"StdH.h" /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /D "FINALVERSION" /D "WIN32" /D "_DEBUG" /D "ENGINEGUI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"StdH.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /map /debug /machine:I386 /out:"Debug/EngineGUID.dll" /pdbtype:sept
# ADD LINK32 /nologo /dll /map /debug /machine:I386 /out:"FinalDebug/EngineGUID.dll" /pdbtype:sept
# Begin Custom Build - Copying $(InputName) binaries to $(a60911_dir)\Bin\Debug
InputPath=.\FinalDebug\EngineGUID.dll
InputName=EngineGUID
SOURCE="$(InputPath)"

"$(a60911_dir)\Bin\Debug\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy FinalDebug\$(InputName).dll $(a60911_dir)\Bin\Debug >nul 
	copy FinalDebug\$(InputName).map $(a60911_dir)\Bin\Debug >nul 
	copy FinalDebug\$(InputName).lib $(a60911_dir)\Bin\Debug >nul 
	copy FinalDebug\$(InputName).pdb $(a60911_dir)\Bin >nul 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "EngineGUI - Win32 Release"
# Name "EngineGUI - Win32 Debug"
# Name "EngineGUI - Win32 FinalRelease"
# Name "EngineGUI - Win32 FinalDebug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DlgChooseTextureType.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCreateAnimatedTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCreateEffectTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCreateNormalTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRecursiveFileBrowser.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSelectMode.cpp
# End Source File
# Begin Source File

SOURCE=.\EngineGUI.cpp
# End Source File
# Begin Source File

SOURCE=.\FileRequester.cpp
# End Source File
# Begin Source File

SOURCE=.\FolderBrowser.cpp
# End Source File
# Begin Source File

SOURCE=.\RecursiveFileBrowser.cpp
# End Source File
# Begin Source File

SOURCE=.\StdH.cpp
# ADD CPP /Yc"StdH.h"
# End Source File
# Begin Source File

SOURCE=.\WndDisplayTexture.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DlgChooseTextureType.h
# End Source File
# Begin Source File

SOURCE=.\DlgCreateAnimatedTexture.h
# End Source File
# Begin Source File

SOURCE=.\DlgCreateEffectTexture.h
# End Source File
# Begin Source File

SOURCE=.\DlgCreateNormalTexture.h
# End Source File
# Begin Source File

SOURCE=.\DlgRecursiveFileBrowser.h
# End Source File
# Begin Source File

SOURCE=.\DlgSelectMode.h
# End Source File
# Begin Source File

SOURCE=.\EngineGUI.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\StdH.h
# End Source File
# Begin Source File

SOURCE=.\WndDisplayTexture.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Graphics.rc
# End Source File
# Begin Source File

SOURCE=.\mode_bro.bmp
# End Source File
# End Group
# Begin Group "Wizard Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Graphics.clw
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

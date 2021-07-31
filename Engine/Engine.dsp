# Microsoft Developer Studio Project File - Name="Engine" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Engine - Win32 FinalDebug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Engine.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Engine.mak" CFG="Engine - Win32 FinalDebug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Engine - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Engine - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Engine - Win32 FinalRelease" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Engine - Win32 FinalDebug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/aboard_WildPet_UI_USA/All/Engine", PDMBAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Engine - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ENGINE_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G6 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ENGINE_EXPORTS" /Yu"StdH.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 wininet.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib winmm.lib /nologo /dll /profile /map /debug /machine:I386 /nodefaultlib:"LIBCMT" /nodefaultlib:"libc"
# Begin Custom Build - Copying $(InputName) binaries to $(a60911_dir)\Bin
InputPath=.\Release\Engine.dll
InputName=Engine
SOURCE="$(InputPath)"

"$(a60911_dir)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy Release\$(InputName).dll $(a60911_dir)\Bin >nul 
	copy Release\$(InputName).map $(a60911_dir)\Bin >nul 
	copy Release\$(InputName).lib $(a60911_dir)\Bin >nul 
	copy Release\$(InputName).pdb $(a60911_dir)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ENGINE_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "ENGINE_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Fr /Yu"StdH.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 wininet.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib NPGameLib.lib winmm.lib /nologo /dll /incremental:no /map /debug /machine:I386 /nodefaultlib:"LIBCMT" /out:"Debug/EngineD.dll" /pdbtype:sept
# SUBTRACT LINK32 /profile /pdb:none /nodefaultlib
# Begin Custom Build - Copying $(InputName) binaries to $(a60911_dir)\Bin\Debug
InputPath=.\Debug\EngineD.dll
InputName=EngineD
SOURCE="$(InputPath)"

"$(a60911_dir)\Bin\Debug\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy Debug\$(InputName).dll $(a60911_dir)\Bin\Debug >nul 
	copy Debug\$(InputName).map $(a60911_dir)\Bin\Debug >nul 
	copy Debug\$(InputName).lib $(a60911_dir)\Bin\Debug >nul 
	copy Debug\$(InputName).pdb $(a60911_dir)\Bin\Debug >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Engine___Win32_FinalRelease"
# PROP BASE Intermediate_Dir "Engine___Win32_FinalRelease"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "FinalRelease"
# PROP Intermediate_Dir "FinalRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ENGINE_EXPORTS" /Yu"StdH.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G6 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "FINALVERSION" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ENGINE_EXPORTS" /Yu"StdH.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /map /debug /machine:I386 /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /map /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Copying $(InputName) binaries to $(a60911_dir)\Bin
InputPath=.\FinalRelease\Engine.dll
InputName=Engine
SOURCE="$(InputPath)"

"$(a60911_dir)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy FinalRelease\$(InputName).dll $(a60911_dir)\Bin >nul 
	copy FinalRelease\$(InputName).map $(a60911_dir)\Bin >nul 
	copy FinalRelease\$(InputName).lib $(a60911_dir)\Bin >nul 
	copy FinalRelease\$(InputName).pdb $(a60911_dir)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Engine___Win32_FinalDebug"
# PROP BASE Intermediate_Dir "Engine___Win32_FinalDebug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "FinalDebug"
# PROP Intermediate_Dir "FinalDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "ENGINE_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Fr /Yu"StdH.h" /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "ENGINE_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Fr /Yu"StdH.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /map /debug /machine:I386 /out:"Debug/EngineD.dll" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none /nodefaultlib
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib Wininet.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /map /debug /machine:I386 /out:"FinalDebug/EngineD.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Copying $(InputName) binaries to $(a60911_dir)\Bin\Debug
InputPath=.\FinalDebug\EngineD.dll
InputName=EngineD
SOURCE="$(InputPath)"

"$(a60911_dir)\Bin\Debug\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy FinalDebug\$(InputName).dll $(a60911_dir)\Bin\Debug >nul 
	copy FinalDebug\$(InputName).map $(a60911_dir)\Bin\Debug >nul 
	copy FinalDebug\$(InputName).lib $(a60911_dir)\Bin\Debug >nul 
	copy FinalDebug\$(InputName).pdb $(a60911_dir)\Bin\Debug >nul 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "Engine - Win32 Release"
# Name "Engine - Win32 Debug"
# Name "Engine - Win32 FinalRelease"
# Name "Engine - Win32 FinalDebug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Math"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Math\Float.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Math\Functions.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Math\Geometry.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Math\Geometry_DOUBLE.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Math\Object3D.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Math\Object3D_CSG.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Math\Object3D_IO.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Math\ObjectSector.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Math\Placement.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Math\Projection.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Math\Projection_Isometric.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Math\Projection_Parallel.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Math\Projection_Perspective.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Math\Projection_Simple.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Math\Projection_Simple_DOUBLE.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Math\TextureMapping.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# End Group
# Begin Group "Graphics"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Graphics\Adapter.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\Benchmark.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\Color.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\DepthCheck.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\DisplayMode.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\DrawPort.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\DrawPort_Particles.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\DrawPort_RenderScene.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\DXTC.cpp
# End Source File
# Begin Source File

SOURCE=.\Graphics\Fog.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\Font.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\Gfx_Direct3D.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\Gfx_Direct3D_Colors.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\Gfx_Direct3D_Textures.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\Gfx_Direct3D_XBox.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\Gfx_OpenGL.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\Gfx_OpenGL_Textures.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\Gfx_wrapper.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\Gfx_wrapper_Direct3D.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\Gfx_wrapper_OpenGL.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\GfxLibrary.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\Graphics.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\ImageInfo.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\MultiMonitor.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\Raster.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\Shader.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\ShadowMap.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\Stereo.cpp
# End Source File
# Begin Source File

SOURCE=.\Graphics\Texture.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\TextureEffects.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics\TextureRender.cpp
# End Source File
# Begin Source File

SOURCE=.\Graphics\ViewPort.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# End Group
# Begin Group "Sound"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Sound\SoundData.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sound\SoundDecoder.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sound\SoundLibrary.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sound\SoundMixer.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sound\SoundObject.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# End Group
# Begin Group "Models"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Models\EditModel.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Models\MipMaker.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Models\Model.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Models\Normals.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Models\RenderModel.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Models\RenderModel_Mask.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Models\RenderModel_View.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Models\VertexGetting.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# End Group
# Begin Group "World"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\World\World.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\World\WorldCollision.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\World\WorldCollisionGrid.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\World\WorldCSG.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\World\WorldEditingProfile.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\World\WorldEntityHashing.cpp
# End Source File
# Begin Source File

SOURCE=.\World\WorldIO.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\World\WorldRayCasting.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# End Group
# Begin Group "Templates"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Templates\AllocationArray.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\BSP.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\DynamicArray.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\DynamicContainer.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\DynamicStackArray.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\HashTableTemplate.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\LinearAllocator.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\NameTable.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\NameTable_CTFileName.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\NameTable_CTranslationPair.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\ReusableContainer.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\Selection.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\StaticArray.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\StaticStackArray.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\Stock.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CAnimData.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CAnimSet.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CEntityClass.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CFontData.cpp
# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CMesh.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CModelData.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CModelInstance.cpp
# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CShader.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CSkeleton.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CSoundData.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CTextureData.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# End Group
# Begin Group "Network"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Network\ActionBuffer.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Network\Buffer.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\CharacterTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\ChatMsgBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\ClientInterface.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Network\cmd.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\CNetwork.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\CommunicationInterface.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Network\Compression.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Network\CPacket.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\Diff.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Network\EMsgBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\EntityHashing.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\InputBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\ItemTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MessageDispatcher.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Network\MobTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MultiTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\NetworkMessage.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Network\NetworkProfile.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Network\PetTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\PlayerSource.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Network\PlayerTarget.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Network\Server.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Network\SessionState.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Network\SessionStateExten.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\SlaveTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\TcpIpConnection.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\ThreadWrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\TxtQueue.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\Web.cpp
# End Source File
# End Group
# Begin Group "Rendering"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Rendering\RenCache.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Rendering\RendASER.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Rendering\RendClip.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Rendering\Render.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Rendering\RenderAdding.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Rendering\RenderBloom.cpp
# End Source File
# Begin Source File

SOURCE=.\Rendering\RenderBrushes.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Rendering\RenderModels.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Rendering\RendMisc.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Rendering\SelectOnRender.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# End Group
# Begin Group "Entities"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Entities\Action.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\CashShopData.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\Entity.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Entities\EntityClass.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Entities\EntityCollision.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Entities\EntityCopying.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Entities\EntityProperties.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Entities\FieldBSPTesting.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Entities\ItemData.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\ItemEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\Items.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\LastPositions.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Entities\MissionCase.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\MobData.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\NearestPolygon.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Entities\NpcHelp.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\OptionData.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\PlayerCharacter.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Entities\QuestSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\ShopData.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\Skill.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\SmcInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\SmcParser.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\SpecialSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\StatusEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\TargetInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Entities\WildPetData.cpp
# End Source File
# End Group
# Begin Group "Light"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Light\LayerMaker.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Light\LayerMixer.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Light\LightSource.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# End Group
# Begin Group "zlib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\zlib\adler32.c

!IF  "$(CFG)" == "Engine - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# SUBTRACT BASE CPP /YX /Yc /Yu
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /Zi
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\zlib\compress.c

!IF  "$(CFG)" == "Engine - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# SUBTRACT BASE CPP /YX /Yc /Yu
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /Zi
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\zlib\deflate.c

!IF  "$(CFG)" == "Engine - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# SUBTRACT BASE CPP /YX /Yc /Yu
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /Zi
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\zlib\infblock.c

!IF  "$(CFG)" == "Engine - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# SUBTRACT BASE CPP /YX /Yc /Yu
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /Zi
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\zlib\infcodes.c

!IF  "$(CFG)" == "Engine - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# SUBTRACT BASE CPP /YX /Yc /Yu
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /Zi
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\zlib\inffast.c

!IF  "$(CFG)" == "Engine - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# SUBTRACT BASE CPP /YX /Yc /Yu
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /Zi
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\zlib\inflate.c

!IF  "$(CFG)" == "Engine - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# SUBTRACT BASE CPP /YX /Yc /Yu
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /Zi
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\zlib\inftrees.c

!IF  "$(CFG)" == "Engine - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# SUBTRACT BASE CPP /YX /Yc /Yu
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /Zi
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\zlib\infutil.c

!IF  "$(CFG)" == "Engine - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# SUBTRACT BASE CPP /YX /Yc /Yu
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /Zi
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\zlib\trees.c

!IF  "$(CFG)" == "Engine - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# SUBTRACT BASE CPP /YX /Yc /Yu
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /Zi
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\zlib\uncompr.c

!IF  "$(CFG)" == "Engine - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# SUBTRACT BASE CPP /YX /Yc /Yu
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /Zi
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\zlib\zutil.c

!IF  "$(CFG)" == "Engine - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# SUBTRACT BASE CPP /YX /Yc /Yu
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /Zi
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# End Group
# Begin Group "Ska"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Ska\AnimSet.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Ska\Mesh.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Ska\ModelInstance.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Ska\NmFileIO.cpp
# End Source File
# Begin Source File

SOURCE=.\Ska\RMMisc.cpp
# End Source File
# Begin Source File

SOURCE=.\Ska\RMRender.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Ska\RMRenderMask.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Ska\Skeleton.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Ska\StringTable.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# End Group
# Begin Group "Terrain"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Terrain\Terrain.cpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TerrainArchive.cpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TerrainInt.cpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TerrainTile.cpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TREditing.cpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TRGenerate.cpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TRLayer.cpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TRLayerImp.cpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TRMisc.cpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TRRayCast.cpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TRRender.cpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TRShader_1TRL.cpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TRShader_2TRL.cpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TRShader_Base.cpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TRShader_Fog.cpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TRShader_Shadow.cpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TRShadows.cpp
# End Source File
# End Group
# Begin Group "XBox"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\XBox\IFeel_X.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\XBox\Input_X.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\XBox\SoundDecoder_X.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\XBox\SymbolLocator_X.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\XBox\VideoDecoder_X.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "Effect"

# PROP Default_Filter ""
# Begin Group "Effects"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Effect\CCameraEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CEffectControl.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CEffectGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CEffectGroupManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CEffectManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CEntityEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CLightEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CMdlEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\COrbitEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticleEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CShockWaveEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CSkaEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CSoundEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CSplineBillboardEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CSplinePathEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CTerrainEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CTraceEffect.cpp
# End Source File
# End Group
# Begin Group "Particle"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Effect\CParticle.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticleGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticleGroupManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticles.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesAbsorption.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesAbsorptionDefault.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesAbsorptionSphere.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesCommonProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesEmitter.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesEmitterCone.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesEmitterCylinder.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesEmitterSphere.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesProcessControl.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesProcessDynamicState.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesProcessForce.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesProcessPointGoal.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesProcessVelocity.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CRandomTable.cpp
# End Source File
# End Group
# Begin Group "Tags"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Effect\CEntityTag.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CGroupTag.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CSkaTag.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CTag.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CTagManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\CWorldTag.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\Effect\CManager.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Effect\CNiceWater.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect\EffectCommon.cpp
# End Source File
# End Group
# Begin Group "Interface"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Interface\UIAuction.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIAutoHelp.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIBilling.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIBillItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIBingoBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIBuff.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIButton.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIButtonEx.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UICashShop.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIChangeWeapon.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UICharacterInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIChatting.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UICheckButton.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UICollectBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UICompound.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UICreateChar.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIDrawFigure.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIEditBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIExchange.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIFiltering.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIFlowerTree.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIGamble.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIGuild.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIGuildBattle.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIGuildNotice.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIGuildStash.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIGuildWarPortal.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIGWMix.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIHelp.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIHelpIcon.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIHelpOld.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIImageBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIIME.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIInitJob.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIInventory.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIListBoxEx.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UILogin.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMap.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMessageBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMessageBoxL.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMessenger.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMinigame.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMix.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMixNew.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMonsterCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMultiEditBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMultList.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UINotice.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UINpcHelp.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIOption.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIOXQuizEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIParty.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIPartyAuto.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIPersonalShop.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIPetFree.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIPetInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIPetItemMix.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIPetTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIPetTraining.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIPlayerInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIPortal.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIProcessNPC.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIProduct.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIQuest.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIQuestBook.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIQuickSlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIQuiz.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIRadar.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIRanking.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIRectString.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIRefine.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIRemission.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIScrollBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UISecurity.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UISelChar.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UISelectList.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UISelectResource.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UISelectServer.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UISelectWord.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIShop.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UISiegeWarfare.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UISiegeWarfareDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UISiegeWarfareNew.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UISignBoard.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UISimplePop.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UISingleBattle.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UISkillLearn.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UISlideBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UISpinButton.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UISummon.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UISystemMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UITalk.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UITargetInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UITatoo.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UITeleport.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UITextureManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UITrackPopup.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIWareHouse.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIWebBoard.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\UIWildPetInfo.cpp
# End Source File
# End Group
# Begin Group "Base"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Base\Anim.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Intermediate_Dir "Debug"
# ADD CPP /Zi /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Intermediate_Dir "Debug"
# PROP Intermediate_Dir "FinalDebug"
# ADD BASE CPP /Zi /Yu"StdH.h"
# ADD CPP /Zi /Fr /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\Changeable.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"
# ADD BASE CPP /Zi
# ADD CPP /Zi /Fr

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\Console.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Intermediate_Dir "Debug"
# ADD CPP /Zi /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Intermediate_Dir "Debug"
# PROP Intermediate_Dir "FinalDebug"
# ADD BASE CPP /Zi /Yu"StdH.h"
# ADD CPP /Zi /Fr /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\CRC.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"
# ADD BASE CPP /Zi
# ADD CPP /Zi /Fr

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\CRCTable.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"
# ADD BASE CPP /Zi
# ADD CPP /Zi /Fr

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\CTString.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"
# ADD BASE CPP /Zi
# ADD CPP /Zi /Fr

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\CTWString.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"
# ADD CPP /Fr

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\Directory.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"
# ADD BASE CPP /Zi
# ADD CPP /Zi /Fr

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\ErrorReporting.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Intermediate_Dir "Debug"
# ADD CPP /Zi /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Intermediate_Dir "Debug"
# PROP Intermediate_Dir "FinalDebug"
# ADD BASE CPP /Zi /Yu"StdH.h"
# ADD CPP /Zi /Fr /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\FileName.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"
# ADD BASE CPP /Zi
# ADD CPP /Zi /Fr

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\IFeel.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"
# ADD BASE CPP /Zi
# ADD CPP /Zi /Fr

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\Input.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Intermediate_Dir "Debug"
# ADD CPP /Zi /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Intermediate_Dir "Debug"
# PROP Intermediate_Dir "FinalDebug"
# ADD BASE CPP /Zi /Yu"StdH.h"
# ADD CPP /Zi /Fr /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\Lists.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Intermediate_Dir "Debug"
# ADD CPP /Zi /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Intermediate_Dir "Debug"
# PROP Intermediate_Dir "FinalDebug"
# ADD BASE CPP /Zi /Yu"StdH.h"
# ADD CPP /Zi /Fr /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\Memory.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Intermediate_Dir "Debug"
# ADD CPP /Zi /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Intermediate_Dir "Debug"
# PROP Intermediate_Dir "FinalDebug"
# ADD BASE CPP /Zi /Yu"StdH.h"
# ADD CPP /Zi /Fr /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\MemoryTracking.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"
# ADD CPP /Fr"FinalFinalDebug/"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\Profiling.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Intermediate_Dir "Debug"
# ADD CPP /Zi /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Intermediate_Dir "Debug"
# PROP Intermediate_Dir "FinalDebug"
# ADD BASE CPP /Zi /Yu"StdH.h"
# ADD CPP /Zi /Fr /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\ProgressHook.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"
# ADD BASE CPP /Zi
# ADD CPP /Zi /Fr

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\Protection.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# SUBTRACT BASE CPP /YX /Yc /Yu
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"
# ADD BASE CPP /Zi
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /Zi /Fr
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\Registry.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"
# ADD BASE CPP /Zi
# ADD CPP /Zi /Fr

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\Relations.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Intermediate_Dir "Debug"
# ADD CPP /Zi /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Intermediate_Dir "Debug"
# PROP Intermediate_Dir "FinalDebug"
# ADD BASE CPP /Zi /Yu"StdH.h"
# ADD CPP /Zi /Fr /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\ReplaceFile.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"
# ADD BASE CPP /Zi
# ADD CPP /Zi /Fr

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\Serial.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Intermediate_Dir "Debug"
# ADD CPP /Zi /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Intermediate_Dir "Debug"
# PROP Intermediate_Dir "FinalDebug"
# ADD BASE CPP /Zi /Yu"StdH.h"
# ADD CPP /Zi /Fr /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\Shell.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Intermediate_Dir "Debug"
# ADD CPP /Zi /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Intermediate_Dir "Debug"
# PROP Intermediate_Dir "FinalDebug"
# ADD BASE CPP /Zi /Yu"StdH.h"
# ADD CPP /Zi /Fr /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\ShellTypes.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"
# ADD BASE CPP /Zi
# ADD CPP /Zi /Fr

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\SimpleHashTable.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"
# ADD CPP /Fr

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\StackDump.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Intermediate_Dir "Debug"
# ADD CPP /Zi /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Intermediate_Dir "Debug"
# PROP Intermediate_Dir "FinalDebug"
# ADD BASE CPP /Zi /Yu"StdH.h"
# ADD CPP /Zi /Fr /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\Statistics.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Intermediate_Dir "Debug"
# ADD CPP /Zi /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Intermediate_Dir "Debug"
# PROP Intermediate_Dir "FinalDebug"
# ADD BASE CPP /Zi /Yu"StdH.h"
# ADD CPP /Zi /Fr /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\Stream.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Intermediate_Dir "Debug"
# ADD CPP /Zi /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Intermediate_Dir "Debug"
# PROP Intermediate_Dir "FinalDebug"
# ADD BASE CPP /Zi /Yu"StdH.h"
# ADD CPP /Zi /Fr /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\Synchronization.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"
# ADD BASE CPP /Zi
# ADD CPP /Zi /Fr

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\Timer.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP Intermediate_Dir "Debug"
# ADD CPP /Zi /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP BASE Intermediate_Dir "Debug"
# PROP Intermediate_Dir "FinalDebug"
# ADD BASE CPP /Zi /Yu"StdH.h"
# ADD CPP /Zi /Fr /Yu"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\Translation.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"
# ADD BASE CPP /Zi
# ADD CPP /Zi /Fr

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\Unzip.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"
# ADD BASE CPP /Zi
# ADD CPP /Zi /Fr

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\Updateable.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"
# ADD BASE CPP /Zi
# ADD CPP /Zi /Fr

!ENDIF 

# End Source File
# End Group
# Begin Group "Brushes"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Brushes\Brush.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Brushes\BrushArchive.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Brushes\BrushExport.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Brushes\BrushImport.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Brushes\BrushIO.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Brushes\BrushMip.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Brushes\BrushPolygon.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Brushes\BrushSector.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Brushes\BrushShadows.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Brushes\BrushTriangularize.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\Engine.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# ADD BASE CPP /Yu"StdH.h"
# ADD CPP /Yu"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi
# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GameState.cpp
# End Source File
# Begin Source File

SOURCE=.\JobInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\MiniDump.cpp
# End Source File
# Begin Source File

SOURCE=.\PetInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\SlaveInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\StdH.cpp

!IF  "$(CFG)" == "Engine - Win32 Release"

# ADD CPP /Yc"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# ADD CPP /Zi /Yc"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# ADD BASE CPP /Yc"StdH.h"
# ADD CPP /Yc"StdH.h"

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# ADD BASE CPP /Zi /Yc"StdH.h"
# ADD CPP /Zi /Yc"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TransformInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Version.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;inl;hxx"
# Begin Group "Base Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Base\Anim.h
# End Source File
# Begin Source File

SOURCE=.\Base\Assert.h
# End Source File
# Begin Source File

SOURCE=.\Base\Base.h
# End Source File
# Begin Source File

SOURCE=.\Base\Changeable.h
# End Source File
# Begin Source File

SOURCE=.\Base\ChangeableRT.h
# End Source File
# Begin Source File

SOURCE=.\Base\Console.h
# End Source File
# Begin Source File

SOURCE=.\Base\Console_internal.h
# End Source File
# Begin Source File

SOURCE=.\Base\CRC.h
# End Source File
# Begin Source File

SOURCE=.\Base\CRCTable.h
# End Source File
# Begin Source File

SOURCE=.\Base\CTString.h
# End Source File
# Begin Source File

SOURCE=.\Base\CTString.inl
# End Source File
# Begin Source File

SOURCE=.\Base\CTWString.h
# End Source File
# Begin Source File

SOURCE=.\Base\ErrorReporting.h
# End Source File
# Begin Source File

SOURCE=.\Base\ErrorTable.h
# End Source File
# Begin Source File

SOURCE=.\Base\FileName.h
# End Source File
# Begin Source File

SOURCE=.\Base\GroupFile.h
# End Source File
# Begin Source File

SOURCE=.\Base\IFeel.h
# End Source File
# Begin Source File

SOURCE=.\Base\Input.h
# End Source File
# Begin Source File

SOURCE=.\Base\KeyNames.h
# End Source File
# Begin Source File

SOURCE=.\Base\ListIterator.inl
# End Source File
# Begin Source File

SOURCE=.\Base\Lists.h
# End Source File
# Begin Source File

SOURCE=.\Base\Lists.inl
# End Source File
# Begin Source File

SOURCE=.\Base\Memory.h
# End Source File
# Begin Source File

SOURCE=.\Base\MemoryTracking.h
# End Source File
# Begin Source File

SOURCE=.\Base\ObjectRestore.h
# End Source File
# Begin Source File

SOURCE=.\Base\ParsingSymbols.h
# End Source File
# Begin Source File

SOURCE=.\Base\PersistentSymbolsEncode.h
# End Source File
# Begin Source File

SOURCE=.\Base\Priority.inl
# End Source File
# Begin Source File

SOURCE=.\Base\Profiling.h
# End Source File
# Begin Source File

SOURCE=.\Base\ProfilingEnabled.h
# End Source File
# Begin Source File

SOURCE=.\Base\ProgressHook.h
# End Source File
# Begin Source File

SOURCE=.\Base\Protection.h
# End Source File
# Begin Source File

SOURCE=.\Base\Rect.h
# End Source File
# Begin Source File

SOURCE=.\Base\Registry.h
# End Source File
# Begin Source File

SOURCE=.\Base\Relations.h
# End Source File
# Begin Source File

SOURCE=.\Base\ReplaceFile.h
# End Source File
# Begin Source File

SOURCE=.\Base\Serial.h
# End Source File
# Begin Source File

SOURCE=.\Base\Shell.h
# End Source File
# Begin Source File

SOURCE=.\Base\Shell_internal.h
# End Source File
# Begin Source File

SOURCE=.\Base\SimpleHashTable.h
# End Source File
# Begin Source File

SOURCE=.\Base\StackDump.h
# End Source File
# Begin Source File

SOURCE=.\Base\Statistics.h
# End Source File
# Begin Source File

SOURCE=.\Base\Statistics_Internal.h
# End Source File
# Begin Source File

SOURCE=.\Base\Stream.h
# End Source File
# Begin Source File

SOURCE=.\Base\Synchronization.h
# End Source File
# Begin Source File

SOURCE=.\Base\Timer.h
# End Source File
# Begin Source File

SOURCE=.\Base\Timer.inl
# End Source File
# Begin Source File

SOURCE=.\Base\Translation.h
# End Source File
# Begin Source File

SOURCE=.\Base\TranslationPair.h
# End Source File
# Begin Source File

SOURCE=.\Base\Types.h
# End Source File
# Begin Source File

SOURCE=.\Base\Unzip.h
# End Source File
# Begin Source File

SOURCE=.\Base\Updateable.h
# End Source File
# Begin Source File

SOURCE=.\Base\UpdateableRT.h
# End Source File
# End Group
# Begin Group "Math Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Math\AABBox.h
# End Source File
# Begin Source File

SOURCE=.\Math\AdditionalFunction.h
# End Source File
# Begin Source File

SOURCE=.\Math\Clipping.inl
# End Source File
# Begin Source File

SOURCE=.\Math\ExtensionMath.h
# End Source File
# Begin Source File

SOURCE=.\Math\FixInt.h
# End Source File
# Begin Source File

SOURCE=.\Math\Float.h
# End Source File
# Begin Source File

SOURCE=.\Math\Functions.h
# End Source File
# Begin Source File

SOURCE=.\Math\Geometry.h
# End Source File
# Begin Source File

SOURCE=.\Math\Geometry.inl
# End Source File
# Begin Source File

SOURCE=.\Math\Matrix.h
# End Source File
# Begin Source File

SOURCE=.\Math\Matrix12.h
# End Source File
# Begin Source File

SOURCE=.\Math\OBBox.h
# End Source File
# Begin Source File

SOURCE=.\Math\Object3D.h
# End Source File
# Begin Source File

SOURCE=.\Math\Placement.h
# End Source File
# Begin Source File

SOURCE=.\Math\Plane.h
# End Source File
# Begin Source File

SOURCE=.\Math\Projection.h
# End Source File
# Begin Source File

SOURCE=.\Math\Projection_DOUBLE.h
# End Source File
# Begin Source File

SOURCE=.\Math\Quaternion.h
# End Source File
# Begin Source File

SOURCE=.\Math\TextureMapping.h
# End Source File
# Begin Source File

SOURCE=.\Math\Vector.h
# End Source File
# End Group
# Begin Group "Graphics Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Graphics\Adapter.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\Color.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\DisplayMode.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\DrawPort.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\dxtlib.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\Fog.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\Fog_internal.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\Font.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\Gfx_wrapper.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\GfxLibrary.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\gl_functions.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\gl_types.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\ImageInfo.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\MultiMonitor.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\OpenGL.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\Raster.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\RenderPoly.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\RenderScene.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\Shader.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\ShaderCode.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\ShadowMap.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\Stereo.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\Texture.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\TextureEffects.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\Vertex.h
# End Source File
# Begin Source File

SOURCE=.\Graphics\ViewPort.h
# End Source File
# End Group
# Begin Group "Sound Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Sound\al_functions.h
# End Source File
# Begin Source File

SOURCE=.\Sound\eax.h
# End Source File
# Begin Source File

SOURCE=.\Sound\SoundData.h
# End Source File
# Begin Source File

SOURCE=.\Sound\SoundDecoder.h
# End Source File
# Begin Source File

SOURCE=.\Sound\SoundLibrary.h
# End Source File
# Begin Source File

SOURCE=.\Sound\SoundObject.h
# End Source File
# End Group
# Begin Group "Models Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Models\EditModel.h
# End Source File
# Begin Source File

SOURCE=.\Models\MipMaker.h
# End Source File
# Begin Source File

SOURCE=.\Models\Model.h
# End Source File
# Begin Source File

SOURCE=.\Models\Model_internal.h
# End Source File
# Begin Source File

SOURCE=.\Models\ModelData.h
# End Source File
# Begin Source File

SOURCE=.\Models\ModelObject.h
# End Source File
# Begin Source File

SOURCE=.\Models\Normals.h
# End Source File
# Begin Source File

SOURCE=.\Models\RenderModel.h
# End Source File
# Begin Source File

SOURCE=.\Models\RenderModel_internal.h
# End Source File
# End Group
# Begin Group "Templates Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Templates\AllocationArray.h
# End Source File
# Begin Source File

SOURCE=.\Templates\BSP.h
# End Source File
# Begin Source File

SOURCE=.\Templates\BSP_internal.h
# End Source File
# Begin Source File

SOURCE=.\Templates\DynamicArray.h
# End Source File
# Begin Source File

SOURCE=.\Templates\DynamicContainer.h
# End Source File
# Begin Source File

SOURCE=.\Templates\DynamicStackArray.h
# End Source File
# Begin Source File

SOURCE=.\Templates\HashTableTemplate.h
# End Source File
# Begin Source File

SOURCE=.\Templates\LinearAllocator.h
# End Source File
# Begin Source File

SOURCE=.\Templates\MapOfData.h
# End Source File
# Begin Source File

SOURCE=.\Templates\NameTable.h
# End Source File
# Begin Source File

SOURCE=.\Templates\NameTable_CTFileName.h
# End Source File
# Begin Source File

SOURCE=.\Templates\NameTable_CTranslationPair.h
# End Source File
# Begin Source File

SOURCE=.\Templates\ReusableContainer.h
# End Source File
# Begin Source File

SOURCE=.\Templates\Selection.h
# End Source File
# Begin Source File

SOURCE=.\Templates\StaticArray.h
# End Source File
# Begin Source File

SOURCE=.\Templates\StaticStackArray.h
# End Source File
# Begin Source File

SOURCE=.\Templates\Stock.h
# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CAnimData.h
# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CAnimSet.h
# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CEntityClass.h
# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CFontData.h
# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CMesh.h
# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CModelData.h
# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CModelInstance.h
# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CShader.h
# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CSkeleton.h
# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CSoundData.h
# End Source File
# Begin Source File

SOURCE=.\Templates\Stock_CTextureData.h
# End Source File
# End Group
# Begin Group "Rendering Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Rendering\Render.h
# End Source File
# Begin Source File

SOURCE=.\Rendering\Render_internal.h
# End Source File
# End Group
# Begin Group "Entities Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Entities\Action.h
# End Source File
# Begin Source File

SOURCE=.\Entities\CashShopData.h
# End Source File
# Begin Source File

SOURCE=.\Entities\CharacterAnimation.h
# End Source File
# Begin Source File

SOURCE=.\Entities\Entity.h
# End Source File
# Begin Source File

SOURCE=.\Entities\EntityClass.h
# End Source File
# Begin Source File

SOURCE=.\Entities\EntityCollision.h
# End Source File
# Begin Source File

SOURCE=.\Entities\EntityEvent.h
# End Source File
# Begin Source File

SOURCE=.\Entities\EntityPointer.h
# End Source File
# Begin Source File

SOURCE=.\Entities\EntityProperties.h
# End Source File
# Begin Source File

SOURCE=.\Entities\FieldSettings.h
# End Source File
# Begin Source File

SOURCE=.\Entities\InternalClasses.h
# End Source File
# Begin Source File

SOURCE=.\Entities\ItemData.h
# End Source File
# Begin Source File

SOURCE=.\Entities\ItemEffect.h
# End Source File
# Begin Source File

SOURCE=.\Entities\Items.h
# End Source File
# Begin Source File

SOURCE=.\Entities\LastPositions.h
# End Source File
# Begin Source File

SOURCE=.\Entities\MissionCase.h
# End Source File
# Begin Source File

SOURCE=.\Entities\MobData.h
# End Source File
# Begin Source File

SOURCE=.\Entities\NpcHelp.h
# End Source File
# Begin Source File

SOURCE=.\Entities\OptionData.h
# End Source File
# Begin Source File

SOURCE=.\Entities\PlayerCharacter.h
# End Source File
# Begin Source File

SOURCE=.\Entities\Precaching.h
# End Source File
# Begin Source File

SOURCE=.\Entities\QuestSystem.h
# End Source File
# Begin Source File

SOURCE=.\Entities\ShadingInfo.h
# End Source File
# Begin Source File

SOURCE=.\Entities\ShopData.h
# End Source File
# Begin Source File

SOURCE=.\Entities\Skill.h
# End Source File
# Begin Source File

SOURCE=.\Entities\SmcInfo.h
# End Source File
# Begin Source File

SOURCE=.\Entities\SmcParser.h
# End Source File
# Begin Source File

SOURCE=.\Entities\SpecialSkill.h
# End Source File
# Begin Source File

SOURCE=.\Entities\StatusEffect.h
# End Source File
# Begin Source File

SOURCE=.\Entities\TargetInfo.h
# End Source File
# Begin Source File

SOURCE=.\Entities\WildPetData.h
# End Source File
# End Group
# Begin Group "Brushes Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Brushes\Brush.h
# End Source File
# Begin Source File

SOURCE=.\Brushes\BrushArchive.h
# End Source File
# Begin Source File

SOURCE=.\Brushes\BrushBase.h
# End Source File
# Begin Source File

SOURCE=.\Brushes\BrushTransformed.h
# End Source File
# End Group
# Begin Group "Network Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Network\ActionBuffer.h
# End Source File
# Begin Source File

SOURCE=.\Network\Buffer.h
# End Source File
# Begin Source File

SOURCE=.\Network\CharacterTarget.h
# End Source File
# Begin Source File

SOURCE=.\Network\ChatMsgBuffer.h
# End Source File
# Begin Source File

SOURCE=.\Network\ClientInterface.h
# End Source File
# Begin Source File

SOURCE=.\Network\cmd.h
# End Source File
# Begin Source File

SOURCE=.\Network\CNetwork.h
# End Source File
# Begin Source File

SOURCE=.\Network\Common.h
# End Source File
# Begin Source File

SOURCE=.\Network\CommunicationInterface.h
# End Source File
# Begin Source File

SOURCE=.\Network\Compression.h
# End Source File
# Begin Source File

SOURCE=.\Network\CPacket.h
# End Source File
# Begin Source File

SOURCE=.\Network\CryptNetMsg.h
# End Source File
# Begin Source File

SOURCE=.\Network\CryptNetMsg_TransTable
# End Source File
# Begin Source File

SOURCE=.\Network\Diff.h
# End Source File
# Begin Source File

SOURCE=.\Network\EMsgBuffer.h
# End Source File
# Begin Source File

SOURCE=.\Network\EntityHashing.h
# End Source File
# Begin Source File

SOURCE=.\Network\InputBuffer.h
# End Source File
# Begin Source File

SOURCE=.\Network\ItemTarget.h
# End Source File
# Begin Source File

SOURCE=.\Network\LevelChange.h
# End Source File
# Begin Source File

SOURCE=.\Network\MessageDefine.h
# End Source File
# Begin Source File

SOURCE=.\Network\MessageDispatcher.h
# End Source File
# Begin Source File

SOURCE=.\Network\MobTarget.h
# End Source File
# Begin Source File

SOURCE=.\Network\MultiTarget.h
# End Source File
# Begin Source File

SOURCE=.\Network\NetworkMessage.h
# End Source File
# Begin Source File

SOURCE=.\Network\NetworkProfile.h
# End Source File
# Begin Source File

SOURCE=.\Network\PetTarget.h
# End Source File
# Begin Source File

SOURCE=.\Network\PlayerSource.h
# End Source File
# Begin Source File

SOURCE=.\Network\PlayerTarget.h
# End Source File
# Begin Source File

SOURCE=.\Network\Server.h
# End Source File
# Begin Source File

SOURCE=.\Network\SessionSocket.h
# End Source File
# Begin Source File

SOURCE=.\Network\SessionState.h
# End Source File
# Begin Source File

SOURCE=.\Light\Shadows_internal.h
# End Source File
# Begin Source File

SOURCE=.\Network\SlaveTarget.h
# End Source File
# Begin Source File

SOURCE=.\Network\TcpIpConnection.h
# End Source File
# Begin Source File

SOURCE=.\Network\ThreadWrapper.h
# End Source File
# Begin Source File

SOURCE=.\Network\TxtQueue.h
# End Source File
# Begin Source File

SOURCE=.\Network\Web.h
# End Source File
# End Group
# Begin Group "Light Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Light\Gradient.h
# End Source File
# Begin Source File

SOURCE=.\Light\LensFlares.h
# End Source File
# Begin Source File

SOURCE=.\Light\LightSource.h
# End Source File
# End Group
# Begin Group "zlib Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\zlib\deflate.h
# End Source File
# Begin Source File

SOURCE=.\zlib\infblock.h
# End Source File
# Begin Source File

SOURCE=.\zlib\infcodes.h
# End Source File
# Begin Source File

SOURCE=.\zlib\inffast.h
# End Source File
# Begin Source File

SOURCE=.\zlib\inffixed.h
# End Source File
# Begin Source File

SOURCE=.\zlib\inftrees.h
# End Source File
# Begin Source File

SOURCE=.\zlib\infutil.h
# End Source File
# Begin Source File

SOURCE=.\zlib\trees.h
# End Source File
# Begin Source File

SOURCE=.\zlib\zconf.h
# End Source File
# Begin Source File

SOURCE=.\zlib\zlib.h
# End Source File
# Begin Source File

SOURCE=.\zlib\zutil.h
# End Source File
# End Group
# Begin Group "exploration3d"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\exploration3d\E3BASE.H
# End Source File
# Begin Source File

SOURCE=.\exploration3d\E3CALL.H
# End Source File
# Begin Source File

SOURCE=.\exploration3d\E3EXT.H
# End Source File
# Begin Source File

SOURCE=.\exploration3d\E3TYPES.H
# End Source File
# Begin Source File

SOURCE=.\exploration3d\EXPLOR3D.H
# End Source File
# End Group
# Begin Group "Ska Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Ska\AnimSet.h
# End Source File
# Begin Source File

SOURCE=.\Ska\Mesh.h
# End Source File
# Begin Source File

SOURCE=.\Ska\ModelInstance.h
# End Source File
# Begin Source File

SOURCE=.\Ska\NmFileIO.h
# End Source File
# Begin Source File

SOURCE=.\Ska\ParsingSmbs.h
# End Source File
# Begin Source File

SOURCE=.\Ska\Render.h
# End Source File
# Begin Source File

SOURCE=.\Ska\SkaAssert.h
# End Source File
# Begin Source File

SOURCE=.\Ska\Skeleton.h
# End Source File
# Begin Source File

SOURCE=.\Ska\StringTable.h
# End Source File
# End Group
# Begin Group "World Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\World\World.h
# End Source File
# Begin Source File

SOURCE=.\World\WorldCollision.h
# End Source File
# Begin Source File

SOURCE=.\World\WorldEditingProfile.h
# End Source File
# Begin Source File

SOURCE=.\World\WorldEntityHashing.h
# End Source File
# Begin Source File

SOURCE=.\World\WorldRayCasting.h
# End Source File
# Begin Source File

SOURCE=.\World\WorldSettings.h
# End Source File
# End Group
# Begin Group "Terrain Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Terrain\Terrain.h
# End Source File
# Begin Source File

SOURCE=.\Terrain\Terrain.hpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TerrainArchive.h
# End Source File
# Begin Source File

SOURCE=.\Terrain\TerrainTile.h
# End Source File
# Begin Source File

SOURCE=.\Terrain\TerrainTile.hpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TRDataArray.hpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TREditing.hpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TRGenerate.hpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TRLayer.h
# End Source File
# Begin Source File

SOURCE=.\Terrain\TRLayer.hpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TRMisc.hpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TRRayCast.hpp
# End Source File
# Begin Source File

SOURCE=.\Terrain\TRShadows.hpp
# End Source File
# End Group
# Begin Group "XBox Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\XBox\SymbolLocator_X.h
# End Source File
# Begin Source File

SOURCE=.\XBox\VideoDecoder_X.h
# End Source File
# Begin Source File

SOURCE=.\XBox\XKeyboard.h
# End Source File
# End Group
# Begin Group "Effect Headers"

# PROP Default_Filter ""
# Begin Group "Effects Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Effect\CCameraEffect.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CEffect.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CEffectControl.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CEffectGroup.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CEffectGroupManager.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CEffectManager.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CEntityEffect.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CLightEffect.h
# End Source File
# Begin Source File

SOURCE=..\Engine/Effect/CMdlEffect.h
# End Source File
# Begin Source File

SOURCE=.\Effect\COrbitEffect.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticleEffect.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CShockWaveEffect.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CSkaEffect.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CSoundEffect.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CSplineBillboardEffect.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CSplinePathEffect.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CTerrainEffect.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CTraceEffect.h
# End Source File
# End Group
# Begin Group "Particle Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Effect\CConeSpace.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CCylinderSpace.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CForce.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticle.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticleGroup.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticleGroupManager.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticles.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesAbsorption.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesAbsorptionDefault.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesAbsorptionSphere.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesCommonProcess.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesEmitter.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesEmitterCone.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesEmitterCylinder.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesEmitterSphere.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesProcessControl.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesProcessDynamicState.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesProcessForce.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesProcessPointGoal.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CParticlesProcessVelocity.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CRandomTable.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CSphereSpace.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CVelocity.h
# End Source File
# End Group
# Begin Group "Tags Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Effect\CEntityTag.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CGroupTag.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CRefCountPtr.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CSkaTag.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CTag.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CTagManager.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CWorldTag.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Effect\CManager.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CNiceWater.h
# End Source File
# Begin Source File

SOURCE=.\Effect\CSampleSpline.h
# End Source File
# Begin Source File

SOURCE=.\Effect\EffectCommon.h
# End Source File
# Begin Source File

SOURCE=.\Effect\FreeMemStack.h
# End Source File
# End Group
# Begin Group "Interface Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Interface\UIAuction.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIAutoHelp.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIBilling.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIBillItem.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIBingoBox.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIBuff.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIButton.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIButtonEx.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UICashShop.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIChangeWeapon.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UICharacterInfo.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIChatting.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UICheckButton.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UICollectBox.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIComboBox.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UICompound.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UICreateChar.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIDrawFigure.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIEditBox.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIExchange.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIFiltering.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIFlowerTree.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIGamble.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIGroup.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIGuild.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIGuildBattle.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIGuildNotice.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIGuildStash.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIGuildWarPortal.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIGWMix.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIHelp.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIHelper.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIHelpIcon.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIHelpOld.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIImageBox.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIIME.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIInitJob.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIInternalClasses.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIInventory.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIListBox.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIListBoxEx.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UILogin.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIManager.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMap.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMessageBox.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMessageBoxL.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMessenger.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMinigame.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMix.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMixNew.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMonsterCombo.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMouseCursor.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMultiEditBox.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIMultList.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UINotice.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UINpcHelp.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIOption.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIOXQuizEvent.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIParty.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIPartyAuto.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIPersonalShop.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIPetFree.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIPetInfo.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIPetItemMix.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIPetTarget.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIPetTraining.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIPlayerInfo.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIPortal.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIProcess.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIProcessNPC.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIProduct.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIQuest.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIQuestBook.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIQuickSlot.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIQuiz.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIRadar.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIRanking.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIRectString.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIRefine.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIRemission.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIScrollBar.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UISecurity.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UISelChar.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UISelectList.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UISelectResource.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UISelectServer.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UISelectWord.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIShop.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UISiegeWarfare.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UISiegeWarfareDoc.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UISiegeWarfareNew.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UISignBoard.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UISimplePop.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UISingleBattle.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UISkillLearn.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UISlideBar.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UISpinButton.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UISummon.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UISystemMenu.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UITalk.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UITargetInfo.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UITatoo.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UITeleport.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UITextureManager.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UITrackPopup.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIWareHouse.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIWebBoard.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIWildPetInfo.h
# End Source File
# Begin Source File

SOURCE=.\Interface\UIWindow.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Build.h
# End Source File
# Begin Source File

SOURCE=.\CurrentVersion.h
# End Source File
# Begin Source File

SOURCE=.\Engine.h
# End Source File
# Begin Source File

SOURCE=.\GameShell.h
# End Source File
# Begin Source File

SOURCE=.\GameState.h
# End Source File
# Begin Source File

SOURCE=.\GlobalDefinition.h
# End Source File
# Begin Source File

SOURCE=.\JobInfo.h
# End Source File
# Begin Source File

SOURCE=.\LocalDefine.h
# End Source File
# Begin Source File

SOURCE=.\MiniDump.h
# End Source File
# Begin Source File

SOURCE=.\Sound\ov_functions.h
# End Source File
# Begin Source File

SOURCE=.\PetInfo.h
# End Source File
# Begin Source File

SOURCE=.\SlaveInfo.h
# End Source File
# Begin Source File

SOURCE=.\StdH.h
# End Source File
# Begin Source File

SOURCE=.\TransformInfo.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Grammar Files"

# PROP Default_Filter "y;l;skl"
# Begin Source File

SOURCE=.\Base\FLEX.SKL
# End Source File
# Begin Source File

SOURCE=.\Base\Parser.y

!IF  "$(CFG)" == "Engine - Win32 Release"

# Begin Custom Build - Generating shell parser source
InputDir=.\Base
InputPath=.\Base\Parser.y

BuildCmds= \
	bison -o$(InputDir)/Parser.c $(InputDir)/Parser.y -d \
	copy $(InputDir)\Parser.c $(InputDir)\Parser.cpp \
	del $(InputDir)\Parser.c /q \
	

"$(InputDir)/Parser.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)/Parser.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# Begin Custom Build - Generating shell parser source
InputDir=.\Base
InputPath=.\Base\Parser.y

BuildCmds= \
	bison -o$(InputDir)/Parser.c $(InputDir)/Parser.y -d \
	copy $(InputDir)\Parser.c $(InputDir)\Parser.cpp \
	del $(InputDir)\Parser.c /q \
	

"$(InputDir)/Parser.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)/Parser.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# Begin Custom Build - Generating shell parser source
InputDir=.\Base
InputPath=.\Base\Parser.y

BuildCmds= \
	bison -o$(InputDir)/Parser.c $(InputDir)/Parser.y -d \
	copy $(InputDir)\Parser.c $(InputDir)\Parser.cpp \
	del $(InputDir)\Parser.c /q \
	

"$(InputDir)/Parser.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)/Parser.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# Begin Custom Build - Generating shell parser source
InputDir=.\Base
InputPath=.\Base\Parser.y

BuildCmds= \
	bison -o$(InputDir)/Parser.c $(InputDir)/Parser.y -d \
	copy $(InputDir)\Parser.c $(InputDir)\Parser.cpp \
	del $(InputDir)\Parser.c /q \
	

"$(InputDir)/Parser.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)/Parser.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base\Scanner.l

!IF  "$(CFG)" == "Engine - Win32 Release"

# Begin Custom Build - Generating shell scanner source
InputDir=.\Base
InputPath=.\Base\Scanner.l

"$(InputDir)/Scanner.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	flex -o$(InputDir)\Scanner.cpp -S$(InputDir)\Flex.skl $(InputDir)\Scanner.l

# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# Begin Custom Build - Generating shell scanner source
InputDir=.\Base
InputPath=.\Base\Scanner.l

"$(InputDir)/Scanner.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	flex -o$(InputDir)\Scanner.cpp -S$(InputDir)\Flex.skl $(InputDir)\Scanner.l

# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# Begin Custom Build - Generating shell scanner source
InputDir=.\Base
InputPath=.\Base\Scanner.l

"$(InputDir)/Scanner.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	flex -o$(InputDir)\Scanner.cpp -S$(InputDir)\Flex.skl $(InputDir)\Scanner.l

# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# Begin Custom Build - Generating shell scanner source
InputDir=.\Base
InputPath=.\Base\Scanner.l

"$(InputDir)/Scanner.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	flex -o$(InputDir)\Scanner.cpp -S$(InputDir)\Flex.skl $(InputDir)\Scanner.l

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Ska\smcFlex.skl
# End Source File
# Begin Source File

SOURCE=.\Ska\smcPars.y

!IF  "$(CFG)" == "Engine - Win32 Release"

# Begin Custom Build
InputDir=.\Ska
InputPath=.\Ska\smcPars.y

BuildCmds= \
	bison -o$(InputDir)/smcPars.c ./Ska/smcPars.y -d  -p syy \
	copy $(InputDir)\smcPars.c $(InputDir)\smcPars.cpp \
	del $(InputDir)\smcPars.c /q \
	

"$(InputDir)/smcPars.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)/smcPars.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# Begin Custom Build
InputDir=.\Ska
InputPath=.\Ska\smcPars.y

BuildCmds= \
	bison -o$(InputDir)/smcPars.c ./Ska/smcPars.y -d  -p syy \
	copy $(InputDir)\smcPars.c $(InputDir)\smcPars.cpp \
	del $(InputDir)\smcPars.c /q \
	

"$(InputDir)/smcPars.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)/smcPars.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# Begin Custom Build
InputDir=.\Ska
InputPath=.\Ska\smcPars.y

BuildCmds= \
	bison -o$(InputDir)/smcPars.c ./Ska/smcPars.y -d  -p syy \
	copy $(InputDir)\smcPars.c $(InputDir)\smcPars.cpp \
	del $(InputDir)\smcPars.c /q \
	

"$(InputDir)/smcPars.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)/smcPars.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# Begin Custom Build
InputDir=.\Ska
InputPath=.\Ska\smcPars.y

BuildCmds= \
	bison -o$(InputDir)/smcPars.c ./Ska/smcPars.y -d  -p syy \
	copy $(InputDir)\smcPars.c $(InputDir)\smcPars.cpp \
	del $(InputDir)\smcPars.c /q \
	

"$(InputDir)/smcPars.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)/smcPars.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Ska\smcScan.l

!IF  "$(CFG)" == "Engine - Win32 Release"

# Begin Custom Build
InputDir=.\Ska
InputPath=.\Ska\smcScan.l

"$(InputDir)/smcScan.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	flex -o$(InputDir)\smcScan.cpp -S$(InputDir)\smcFlex.skl $(InputDir)\smcScan.l

# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# Begin Custom Build
InputDir=.\Ska
InputPath=.\Ska\smcScan.l

"$(InputDir)/smcScan.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	flex -o$(InputDir)\smcScan.cpp -S$(InputDir)\smcFlex.skl $(InputDir)\smcScan.l

# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# Begin Custom Build
InputDir=.\Ska
InputPath=.\Ska\smcScan.l

"$(InputDir)/smcScan.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	flex -o$(InputDir)\smcScan.cpp -S$(InputDir)\smcFlex.skl $(InputDir)\smcScan.l

# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# Begin Custom Build
InputDir=.\Ska
InputPath=.\Ska\smcScan.l

"$(InputDir)/smcScan.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	flex -o$(InputDir)\smcScan.cpp -S$(InputDir)\smcFlex.skl $(InputDir)\smcScan.l

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Generated Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Classes\BaseEvents.cpp
# ADD CPP /Yu
# End Source File
# Begin Source File

SOURCE=.\Classes\MovableBrushEntity.cpp
# End Source File
# Begin Source File

SOURCE=.\Classes\MovableEntity.cpp
# End Source File
# Begin Source File

SOURCE=.\Classes\MovableModelEntity.cpp
# End Source File
# Begin Source File

SOURCE=.\Base\Parser.cpp
# End Source File
# Begin Source File

SOURCE=.\Classes\PlayerEntity.cpp
# End Source File
# Begin Source File

SOURCE=.\Base\SCANNER.CPP
# End Source File
# Begin Source File

SOURCE=.\Ska\smcPars.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\Ska\SMCSCAN.CPP
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Group
# Begin Group "Entity Class Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Classes\BaseEvents.es

!IF  "$(CFG)" == "Engine - Win32 Release"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\BaseEvents.es
InputName=BaseEvents

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd                                                                                                                                                                                                                                                                                                                                          ..\  
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\BaseEvents.es
InputName=BaseEvents

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd                                                                                                                                                                                                                                                                                                                                          ..\  
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\BaseEvents.es
InputName=BaseEvents

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd                                                                                                                                                                                                                                                                                                                                          ..\  
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\BaseEvents.es
InputName=BaseEvents

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd                                                                                                                                                                                                                                                                                                                                          ..\  
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Classes\MovableBrushEntity.es

!IF  "$(CFG)" == "Engine - Win32 Release"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableBrushEntity.es
InputName=MovableBrushEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd                                                                                                                                                                                                                                                                                                                                          ..\  
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableBrushEntity.es
InputName=MovableBrushEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd                                                                                                                                                                                                                                                                                                                                          ..\  
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableBrushEntity.es
InputName=MovableBrushEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd                                                                                                                                                                                                                                                                                                                                          ..\  
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableBrushEntity.es
InputName=MovableBrushEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd                                                                                                                                                                                                                                                                                                                                          ..\  
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Classes\MovableEntity.es

!IF  "$(CFG)" == "Engine - Win32 Release"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableEntity.es
InputName=MovableEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd                                                                                                                                                                                                                                                                                                                                          ..\  
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableEntity.es
InputName=MovableEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd                                                                                                                                                                                                                                                                                                                                          ..\  
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableEntity.es
InputName=MovableEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd                                                                                                                                                                                                                                                                                                                                          ..\  
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableEntity.es
InputName=MovableEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd                                                                                                                                                                                                                                                                                                                                          ..\  
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Classes\MovableModelEntity.es

!IF  "$(CFG)" == "Engine - Win32 Release"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableModelEntity.es
InputName=MovableModelEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd                                                                                                                                                                                                                                                                                                                                          ..\  
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableModelEntity.es
InputName=MovableModelEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd                                                                                                                                                                                                                                                                                                                                          ..\  
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableModelEntity.es
InputName=MovableModelEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd                                                                                                                                                                                                                                                                                                                                          ..\  
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\MovableModelEntity.es
InputName=MovableModelEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd                                                                                                                                                                                                                                                                                                                                          ..\  
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Classes\PlayerEntity.es

!IF  "$(CFG)" == "Engine - Win32 Release"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\PlayerEntity.es
InputName=PlayerEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd                                                                                                                                                                                                                                                                                                                                          ..\  
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\PlayerEntity.es
InputName=PlayerEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd                                                                                                                                                                                                                                                                                                                                          ..\  
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalRelease"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\PlayerEntity.es
InputName=PlayerEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd                                                                                                                                                                                                                                                                                                                                          ..\  
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Engine - Win32 FinalDebug"

# Begin Custom Build - Engine/Classes/$(InputName).es
InputPath=.\Classes\PlayerEntity.es
InputName=PlayerEntity

"Classes/$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd                                                                                                                                                                                                                                                                                                                                          ..\  
	ecc Engine/Classes/$(InputName).es 
	
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\ChangeLog.txt
# End Source File
# End Target
# End Project

# Microsoft Developer Studio Project File - Name="Shaders" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Shaders - Win32 FinalDebug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Shaders.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Shaders.mak" CFG="Shaders - Win32 FinalDebug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Shaders - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Shaders - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Shaders - Win32 FinalRelease" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Shaders - Win32 FinalDebug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/aboard_WildPet_UI_USA/All/Shaders", PVLBAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Shaders - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHADERS_EXPORTS" /YX /FD /c
# ADD CPP /nologo /G6 /MD /W3 /GX /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHADERS_EXPORTS" /Yu"StdH.h" /FD /c
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
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386
# Begin Custom Build - Copying $(InputName) binaries to $(a60911_dir)\Bin
InputPath=.\Release\Shaders.dll
InputName=Shaders
SOURCE="$(InputPath)"

"$(a60911_dir)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy Release\$(InputName).dll $(a60911_dir)\Bin >nul 
	copy Release\$(InputName).map $(a60911_dir)\Bin >nul 
	copy Release\$(InputName).lib $(a60911_dir)\Bin >nul 
	copy Release\$(InputName).pdb $(a60911_dir)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Shaders - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHADERS_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHADERS_EXPORTS" /Yu"StdH.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /dll /map /debug /machine:I386 /out:"Debug/ShadersD.dll" /pdbtype:sept
# SUBTRACT LINK32 /profile /incremental:no
# Begin Custom Build - Copying $(InputName) binaries to $(a60911_dir)\Bin\Debug
InputPath=.\Debug\ShadersD.dll
InputName=ShadersD
SOURCE="$(InputPath)"

"$(a60911_dir)\Bin\Debug\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy Debug\$(InputName).dll $(a60911_dir)\Bin\Debug >nul 
	copy Debug\$(InputName).map $(a60911_dir)\Bin\Debug >nul 
	copy Debug\$(InputName).lib $(a60911_dir)\Bin\Debug >nul 
	copy Debug\$(InputName).pdb $(a60911_dir)\Bin\Debug >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalRelease"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Shaders___Win32_FinalRelease"
# PROP BASE Intermediate_Dir "Shaders___Win32_FinalRelease"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "FinalRelease"
# PROP Intermediate_Dir "FinalRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GX /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHADERS_EXPORTS" /Yu"StdH.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G6 /MD /W3 /GX /Ox /Ot /Og /Oi /Oy- /I "..\\" /D "FINALVERSION" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHADERS_EXPORTS" /Yu"StdH.h" /FD /c
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
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /map /debug /machine:I386 /pdbtype:sept
# Begin Custom Build - Copying $(InputName) binaries to $(a60911_dir)\Bin
InputPath=.\FinalRelease\Shaders.dll
InputName=Shaders
SOURCE="$(InputPath)"

"$(a60911_dir)\Bin\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy FinalRelease\$(InputName).dll $(a60911_dir)\Bin >nul 
	copy FinalRelease\$(InputName).map $(a60911_dir)\Bin >nul 
	copy FinalRelease\$(InputName).lib $(a60911_dir)\Bin >nul 
	copy FinalRelease\$(InputName).pdb $(a60911_dir)\Bin >nul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalDebug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Shaders___Win32_FinalDebug"
# PROP BASE Intermediate_Dir "Shaders___Win32_FinalDebug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "FinalDebug"
# PROP Intermediate_Dir "FinalDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHADERS_EXPORTS" /Yu"StdH.h" /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /D "FINALVERSION" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SHADERS_EXPORTS" /Yu"StdH.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /map /debug /machine:I386 /out:"Debug/ShadersD.dll" /pdbtype:sept
# ADD LINK32 /nologo /dll /map /debug /machine:I386 /out:"Debug/ShadersD.dll" /pdbtype:sept
# Begin Custom Build - Copying $(InputName) binaries to $(a60911_dir)\Bin\Debug
InputPath=.\Debug\ShadersD.dll
InputName=ShadersD
SOURCE="$(InputPath)"

"$(a60911_dir)\Bin\Debug\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy FinalDebug\$(InputName).dll $(a60911_dir)\Bin\Debug >nul 
	copy FinalDebug\$(InputName).map $(a60911_dir)\Bin\Debug >nul 
	copy FinalDebug\$(InputName).lib $(a60911_dir)\Bin\Debug >nul 
	copy FinalDebug\$(InputName).pdb $(a60911_dir)\Bin\Debug >nul 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "Shaders - Win32 Release"
# Name "Shaders - Win32 Debug"
# Name "Shaders - Win32 FinalRelease"
# Name "Shaders - Win32 FinalDebug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AddShader.cpp

!IF  "$(CFG)" == "Shaders - Win32 Release"

!ELSEIF  "$(CFG)" == "Shaders - Win32 Debug"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\BaseShader.cpp

!IF  "$(CFG)" == "Shaders - Win32 Release"

!ELSEIF  "$(CFG)" == "Shaders - Win32 Debug"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\BaseShaderPPL_Abs.cpp

!IF  "$(CFG)" == "Shaders - Win32 Release"

!ELSEIF  "$(CFG)" == "Shaders - Win32 Debug"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\BaseTransparent.cpp

!IF  "$(CFG)" == "Shaders - Win32 Release"

!ELSEIF  "$(CFG)" == "Shaders - Win32 Debug"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ColorShader.cpp

!IF  "$(CFG)" == "Shaders - Win32 Release"

!ELSEIF  "$(CFG)" == "Shaders - Win32 Debug"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Common.cpp

!IF  "$(CFG)" == "Shaders - Win32 Release"

!ELSEIF  "$(CFG)" == "Shaders - Win32 Debug"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DefaultShader.cpp

!IF  "$(CFG)" == "Shaders - Win32 Release"

!ELSEIF  "$(CFG)" == "Shaders - Win32 Debug"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Detail_SpecularShader.cpp

!IF  "$(CFG)" == "Shaders - Win32 Release"

!ELSEIF  "$(CFG)" == "Shaders - Win32 Debug"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DetailShader.cpp

!IF  "$(CFG)" == "Shaders - Win32 Release"

!ELSEIF  "$(CFG)" == "Shaders - Win32 Debug"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DisplaceShader.cpp

!IF  "$(CFG)" == "Shaders - Win32 Release"

!ELSEIF  "$(CFG)" == "Shaders - Win32 Debug"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GlassShader.cpp

!IF  "$(CFG)" == "Shaders - Win32 Release"

!ELSEIF  "$(CFG)" == "Shaders - Win32 Debug"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\InvisibleShader.cpp

!IF  "$(CFG)" == "Shaders - Win32 Release"

!ELSEIF  "$(CFG)" == "Shaders - Win32 Debug"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MultiLayerShader.cpp

!IF  "$(CFG)" == "Shaders - Win32 Release"

!ELSEIF  "$(CFG)" == "Shaders - Win32 Debug"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NormalMapShader.cpp

!IF  "$(CFG)" == "Shaders - Win32 Release"

!ELSEIF  "$(CFG)" == "Shaders - Win32 Debug"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NoShadowShader.cpp

!IF  "$(CFG)" == "Shaders - Win32 Release"

!ELSEIF  "$(CFG)" == "Shaders - Win32 Debug"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Reflection.cpp

!IF  "$(CFG)" == "Shaders - Win32 Release"

!ELSEIF  "$(CFG)" == "Shaders - Win32 Debug"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ReftectionAndSpecular.cpp

!IF  "$(CFG)" == "Shaders - Win32 Release"

!ELSEIF  "$(CFG)" == "Shaders - Win32 Debug"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ShadowShader.cpp

!IF  "$(CFG)" == "Shaders - Win32 Release"

!ELSEIF  "$(CFG)" == "Shaders - Win32 Debug"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Specular.cpp

!IF  "$(CFG)" == "Shaders - Win32 Release"

!ELSEIF  "$(CFG)" == "Shaders - Win32 Debug"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\StdH.cpp

!IF  "$(CFG)" == "Shaders - Win32 Release"

# ADD CPP /Yc"StdH.h"

!ELSEIF  "$(CFG)" == "Shaders - Win32 Debug"

# ADD CPP /Yc"StdH.h"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalRelease"

# ADD CPP /Yc"StdH.h"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"
# ADD CPP /Yc"StdH.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Translucent.cpp

!IF  "$(CFG)" == "Shaders - Win32 Release"

!ELSEIF  "$(CFG)" == "Shaders - Win32 Debug"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\WaterShader.cpp

!IF  "$(CFG)" == "Shaders - Win32 Release"

!ELSEIF  "$(CFG)" == "Shaders - Win32 Debug"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Common.h

!IF  "$(CFG)" == "Shaders - Win32 Release"

!ELSEIF  "$(CFG)" == "Shaders - Win32 Debug"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\StdH.h

!IF  "$(CFG)" == "Shaders - Win32 Release"

!ELSEIF  "$(CFG)" == "Shaders - Win32 Debug"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalRelease"

!ELSEIF  "$(CFG)" == "Shaders - Win32 FinalDebug"

# PROP Intermediate_Dir "FinalDebug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project

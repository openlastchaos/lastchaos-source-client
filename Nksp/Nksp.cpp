

/*
 *  Last Chaos
 *  Copyright (c) 1998, CroTeam. All rights reserved.
 *
 */


#include "StdH.h"
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <process.h>
#include <Engine/CurrentVersion.h>
#include <Engine/Templates/Stock_CEntityClass.h>
#include <Engine/Templates/Stock_CFontData.h>
#include <Engine/Interface/UIManager.h>				// yjpark
#include <Engine/Interface/UIMouseCursor.h>
#include <Engine/GameState.h>
#include <Engine/GlobalDefinition.h>
#define DECL_DLL
#include <EntitiesMP/Global.h>
#include "resource.h"
//#include "SplashScreen.h"
#include "MainWindow.h"
#include "GlSettings.h"
//#include "LevelInfo.h"
//#include "LCDDrawing.h"
#include "CmdLine.h"
//#include "Credits.h"
#include <Engine/Base/Protection.h>
#if COPY_PROTECTION
	#include <Engine/Base/Protection.cpp>
#endif
#include <Engine/Network/Web.h>
#include <TlHelp32.h>

ENGINE_API extern cWeb g_web;
extern ENGINE_API char *g_szExitError;

//CTString _fnmForCDCheck = "Bin\\SeriousSam.exe";

// application state variables by NSP
extern BOOL g_bTcp = TRUE;
// end of ..

extern CGame		*_pGame = NULL;

// application state variables
//extern BOOL _bRunning = TRUE;
//extern BOOL _bQuitScreen = TRUE;
//extern BOOL _bMenuActive = FALSE;
//extern BOOL _bMenuRendering = FALSE;



static PIX _apixRes[][2] = {	 
	640, 480,	 
		800, 500,
		800, 600,	 
		1024, 768,
		1152, 864,	
		1280, 800,
		1280, 960,	
		1280, 1024,
		1600,1200,    	
};

// helper for limiting window size when changing mode out of full screen
extern void FindMaxResolution( PIX &pixSizeI, PIX &pixSizeJ)
{
	const INDEX ctResolutions = ARRAYCOUNT(_apixRes);
	for( INDEX iRes=(ctResolutions-1); iRes>=0; iRes--) 
	{
		if( pixSizeI>_apixRes[iRes][0] && pixSizeJ>_apixRes[iRes][1]) 
		{
			pixSizeI = _apixRes[iRes][0];
			pixSizeJ = _apixRes[iRes][1];
			return;
		}
	} // should not end up here
	ASSERTALWAYS( "Desktop resolution too low!");
	pixSizeI = _apixRes[0][0];
	pixSizeJ = _apixRes[0][1];
}

//extern BOOL _bDefiningKey;		// 원본.
extern BOOL _bDefiningKey = FALSE;
//static BOOL _bReconsiderInput = FALSE;
extern PIX  _pixDesktopWidth  = 0;
extern PIX  _pixDesktopHeight = 0;

static INDEX sam_iMaxFPSActive   = 500;
static INDEX sam_iMaxFPSInactive = 10;
static INDEX sam_bPauseOnMinimize = TRUE; // auto-pause when window has been minimized
extern ENGINE_API INDEX	sam_bWideScreen;		// yjpark
extern FLOAT sam_fPlayerOffset = 0.0f;

// Window handle
extern ENGINE_API HWND	_hwndMain;

// display mode settings
extern ENGINE_API BOOL	_bWindowChanging;				// yjpark |<--
extern ENGINE_API INDEX sam_bFullScreenActive;
extern ENGINE_API INDEX sam_iScreenSizeI;
extern ENGINE_API INDEX sam_iScreenSizeJ;
extern ENGINE_API INDEX sam_iDisplayDepth;
extern ENGINE_API INDEX sam_iDisplayAdapter;
extern ENGINE_API INDEX sam_iGfxAPI;	// 0==OpenGL	// yjpark     -->|
extern INDEX sam_bFirstStarted = FALSE;
extern FLOAT sam_tmDisplayModeReport = 5.0f;
//extern INDEX sam_bShowAllLevels = FALSE;
//extern INDEX sam_bMentalActivated = FALSE;

// network settings
extern CTString sam_strNetworkSettings = "";
// command line
extern CTString sam_strCommandLine = "";

// 0...app started for the first time
// 1...all ok
// 2...automatic fallback
static INDEX _iDisplayModeChangeFlag	= 0;
static TIME _tmDisplayModeChanged		= 100.0f; // when display mode was last changed

// rendering preferences for automatic settings
extern INDEX sam_iVideoSetup = 1;  // 0==speed, 1==normal, 2==quality, 3==custom
// automatic adjustment of audio quality
extern BOOL sam_bAutoAdjustAudio	= TRUE;

extern INDEX sam_bAutoPlayDemos		= TRUE;
static INDEX _bInAutoPlayLoop		= TRUE;

// menu calling
/*
extern INDEX sam_bMenuSave     = FALSE;
extern INDEX sam_bMenuLoad     = FALSE;
extern INDEX sam_bMenuControls = FALSE;
extern INDEX sam_bMenuHiScore  = FALSE;
*/
extern INDEX sam_bToggleConsole		= FALSE;
//extern INDEX sam_iStartCredits		= FALSE;

// for mod re-loading
extern CTFileName _fnmModToLoad		= CTString("");
extern CTString _strModServerJoin	= CTString("");
extern CTString _strURLToVisit		= CTString("");


// state variables fo addon execution
// 0 - nothing
// 1 - start (invoke console)
// 2 - console invoked, waiting for one redraw
extern INDEX _iAddonExecState		= 0;
extern CTFileName _fnmAddonToExec	= CTString("");

// logo textures
//static CTextureObject  _toLogoCT;
//static CTextureObject  _toLogoODI;
//static CTextureObject  _toLogoGSpy;
//static CTextureObject  _toLogoEAX;
//extern CTextureObject *_ptoLogoCT  = NULL;
//extern CTextureObject *_ptoLogoODI = NULL;
//extern CTextureObject *_ptoLogoGSpy = NULL;
//extern CTextureObject *_ptoLogoEAX = NULL;

extern CTString sam_strVersion = "0.5";
//extern CTString sam_strModName = TRANS("Prologue");
/*
#if _SE_DEMO
extern CTString sam_strFirstLevel = "Levels\\KarnakDemo.wld";
#else
extern CTString sam_strFirstLevel = "Levels\\01_Hatshepsut.wld";
#endif
*/
extern CTString sam_strIntroLevel = "Levels\\Intro.wld";
extern CTString sam_strGameName = "serioussam";

//extern CTString sam_strTechTestLevel = "Levels\\TechTest.wld";
//extern CTString sam_strTrainingLevel = "Levels\\KarnakDemo.wld";

ENGINE_API extern INDEX snd_iFormat;


// main window canvas
/*CDrawPort *pdp;
CDrawPort *pdpNormal;
CDrawPort *pdpWideScreen;
CViewPort *pvpViewPort;
HINSTANCE _hInstance;*/		// ypark

// main window canvas		// yjpark
extern ENGINE_API CDrawPort	*_pdpMain;
extern ENGINE_API CDrawPort	*_pdpNormalMain;
extern ENGINE_API CDrawPort	*_pdpWideScreenMain;
extern ENGINE_API CViewPort	*_pvpViewPortMain;
extern ENGINE_API HINSTANCE	_hInstanceMain;

/*
static void PlayDemo(const CTString &strDemoFilename)
{
	// 쓰이지 않음.
	_gmMenuGameMode = GM_DEMO;
	CTFileName fnDemo = "demos\\" + strDemoFilename + ".dem";
	extern BOOL LSLoadDemo(const CTFileName &fnm);
	LSLoadDemo(fnDemo);
}
	*/
ENGINE_API extern INDEX g_iCountry;

// WSS_NPROTECT 070402 -------------------------->>
#ifndef NO_GAMEGUARD
	extern ENGINE_API CTString g_szHackMsg;
#endif
// ----------------------------------------------<<

void GetNM(LPSTR lpCmdLine)
{
	
	// wooss 060209 NetMable_JP 
	extern BOOL g_bAutoLogin;
	extern CTString g_nmVER;
	extern CTString g_nmCID;
	extern CTString g_nmID;
	extern CTString g_nmPW;
	
	// wooss NetMable_JP Auto login 060209
	CTString strTemp((CHAR*)lpCmdLine);

	for(INDEX ii = 0; ii < 4; ii++)
	{
		INDEX tempPos = strTemp.FindSubstr(" ");

		CTString strInsert = strTemp;

		strInsert.TrimRight(tempPos);

		switch(ii)
		{
			case 0 :
				g_nmVER.PrintF("%s", strInsert);
				break;
			case 1 :
				g_nmCID.PrintF("%s", strInsert);
				break;
			case 2 :
				g_nmID.PrintF("%s", strInsert);
				break;
			case 3 :
				g_nmPW.PrintF("%s", strTemp);
				break;
		}

		if(ii < 3)
			strTemp.DeleteChars(0, tempPos + 1);
	}

	if(g_nmID.Length()!=0 && g_nmPW.Length()!=0 ) {
			switch(g_iCountry)
			{
			case KOREA :
				if(g_nmCID.IsEqualCaseSensitive("em"))
					g_bAutoLogin = TRUE;
				break;
			case TAIWAN :
				break;
			case CHINA :
				break;
			case THAILAND :
				break;
			case TAIWAN2:
				break;
			case JAPAN :
				g_bAutoLogin = TRUE;
				break;
			case MALAYSIA :
				break;
			case USA:
				break;
			case BRAZIL:
				break;
			case HONGKONG:
				break;
			}
			
	}

	
}

/***********************************************************************************************
* IsRunning()
* 중복 실행 방지 함수
* 현재 실행 중인 프로세스를 검색하여 Nksp.exe가 실행되어 있는 것이 있는지 확인하여 TRUE, FALSE를
* 리턴 한다.
*************************************************************************************************/
BOOL IsRunning(void)
{
	PROCESSENTRY32 peNext;

	// 접근 프로세스 핸들을 생성한다.
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnapShot == INVALID_HANDLE_VALUE )
	{
		MessageBox(NULL, "Fail Execute LastChaos!", "LastChaos Run", MB_OK);
		return TRUE;
	}

	peNext.dwSize = sizeof(PROCESSENTRY32);

	int nFindNum = 0;
	int nSize = 0;
	DWORD dwTemp = GetCurrentProcessId();
	BOOL bCurrentProcess = FALSE;

	Process32First(hSnapShot, &peNext); // 첫번째 프로세스를 가져온다.

	while(Process32Next(hSnapShot, &peNext)) // 모든 프로세스를 검색한다.
	{
		if (peNext.th32ProcessID == dwTemp)
		{
			bCurrentProcess = TRUE;
			continue;
		}

		nSize = strlen(peNext.szExeFile);

		if ( nSize > 8 ) { nSize -= 8; }
		else { nSize = 0; }
		
		if (stricmp(peNext.szExeFile+(nSize), "Nksp.exe") == 0)
		{
			nFindNum++;

			if (nFindNum > 0)
			{
				CloseHandle(hSnapShot);
				return TRUE;
			}
		}
	}

	CloseHandle(hSnapShot);

	if (!bCurrentProcess)
	{
		MessageBox(NULL, "Fail Execute LastChaos!", "LastChaos Run", MB_OK);		
		return TRUE;
	}

	return FALSE;
}

static void ApplyRenderingPreferences(void)
{
	ApplyGLSettings(TRUE);
}

/*
extern void ApplyVideoMode(void)
{
	StartNewMode( (GfxAPIType)sam_iGfxAPI, sam_iDisplayAdapter, sam_iScreenSizeI, sam_iScreenSizeJ,
								(enum DisplayDepth)sam_iDisplayDepth, IsFullScreen( sam_bFullScreenActive) );
}
*/

static void BenchMark(void)
{
	_pGfx->Benchmark(_pvpViewPortMain, _pdpMain);
}


static void QuitGame(void)
{
	_pGameState->Running()		= FALSE;
	_pGameState->QuitScreen()	= FALSE;
}

// check if another app is already running
static HANDLE _hLock = NULL;
static CTFileName _fnmLock;
static void DirectoryLockOn(void)
{
	// create lock filename
	_fnmLock = _fnmApplicationPath+"nksp.loc";
	// try to open lock file
	_hLock = CreateFile(
		_fnmLock, 
		GENERIC_WRITE,
		0/*no sharing*/,
		NULL, // pointer to security attributes
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL|FILE_FLAG_DELETE_ON_CLOSE,  // file attributes
		NULL);
	// if failed
	if (_hLock==NULL || GetLastError()!=0) {
		// report warning
		CPrintF(TRANS("WARNING: Last Chaos didn't shut down properly last time!\n"));
	}
}
static void DirectoryLockOff(void)
{
	// if lock is open
	if (_hLock!=NULL) {
		// close it
		CloseHandle(_hLock);
	}
}

//강동민 수정 시작 시스템 마우스 작업	09.09
// NOTE : 애니메이션 커서를 리소스로부터 읽어옴.
static HCURSOR LoadAnimationCursor(HINSTANCE hInstance, UINT nID)
{
	HRSRC hRes		= FindResource(hInstance, MAKEINTRESOURCE(nID), "CURSORS");
	DWORD dwSize	= SizeofResource(hInstance, hRes);
	HGLOBAL hGlob	= LoadResource(hInstance, hRes);
	LPBYTE pBytes	= (LPBYTE)LockResource(hGlob);
	return (HCURSOR)CreateIconFromResource(pBytes, dwSize, FALSE, 0x00030000);	
}
//강동민 수정 끝 시스템 마우스 작업		09.09

void End(void);


// automaticaly manage input enable/disable toggling
//static BOOL _bInputEnabled = FALSE;
/*void UpdateInputEnabledState(void)
{
	// do nothing if window is invalid
	if( _hwndMain==NULL) return;

	// input should be enabled if application is active
	// and no menu is active and no console is active
	BOOL bShouldBeEnabled = ( !IsIconic(_hwndMain) && !_pGameState->m_bMenuActive && _pGame->gm_csConsoleState==CS_OFF ) || _bDefiningKey;

	// if should be turned off
	if( (!bShouldBeEnabled && _bInputEnabled) || _bReconsiderInput) {
		// disable it and remember new state
		_pInput->DisableInput();
		_bInputEnabled = FALSE;
	}
	// if should be turned on
	if( bShouldBeEnabled && !_bInputEnabled) {
		// enable it and remember new state
		_pInput->EnableInput(_hwndMain);
		_bInputEnabled = TRUE;
	}
	_bReconsiderInput = FALSE;
}*/


// automaticaly manage pause toggling
void UpdatePauseState(void)
{
	// 쓰이지 않는 부분.
	/*
	BOOL bShouldPause = ( _gmRunningGameMode == GM_SINGLE_PLAYER ) &&
						( _pGameState->m_bMenuActive || _pGame->gm_csConsoleState == CS_ON ||
						 _pGame->gm_csConsoleState == CS_TURNINGON || _pGame->gm_csConsoleState == CS_TURNINGOFF );
	_pNetwork->SetLocalPause(bShouldPause);
	*/
}


// limit current frame rate if neeeded
void LimitFrameRate(void)
{
	// measure passed time for each loop
	static CTimerValue tvLast(-1.0f);
	CTimerValue tvNow   = _pTimer->GetHighPrecisionTimer();
	TIME tmCurrentDelta = (tvNow-tvLast).GetSeconds();

	// limit maximum frame rate
	sam_iMaxFPSActive   = ClampDn( (INDEX)sam_iMaxFPSActive,   1L);
	sam_iMaxFPSInactive = ClampDn( (INDEX)sam_iMaxFPSInactive, 1L);
	INDEX iMaxFPS = sam_iMaxFPSActive;
	if( IsIconic(_hwndMain)) iMaxFPS = sam_iMaxFPSInactive;

	/*
	if(_pGame->gm_CurrentSplitScreenCfg==CGame::SSC_DEDICATED) 
	{
		iMaxFPS = ClampDn(iMaxFPS, 60L); // never go very slow if dedicated server
	}
	*/
	TIME tmWantedDelta = 1.0f / iMaxFPS;
	if( tmCurrentDelta<tmWantedDelta) Sleep( (tmWantedDelta-tmCurrentDelta)*1000.0f);
	
	// remember new time
	tvLast = _pTimer->GetHighPrecisionTimer();
}

/*
// load first demo
void StartNextDemo(void)
{
	// 쓸모없는 부분이므로 주석처리됨.
	if (!sam_bAutoPlayDemos || !_bInAutoPlayLoop) {
		_bInAutoPlayLoop = FALSE;
		return;
	}

	// skip if no demos
	if(_lhAutoDemos.IsEmpty()) {
		_bInAutoPlayLoop = FALSE;
		return;
	}

	// get first demo level and cycle the list
	CLevelInfo *pli = LIST_HEAD(_lhAutoDemos, CLevelInfo, li_lnNode);
	pli->li_lnNode.Remove();
	_lhAutoDemos.AddTail(pli->li_lnNode);

	// if intro
	if (pli->li_fnLevel==sam_strIntroLevel) {
		// start intro
		_gmRunningGameMode = GM_NONE;
		_pGame->gm_aiStartLocalPlayers[0] = 0;
		_pGame->gm_aiStartLocalPlayers[1] = -1;
		_pGame->gm_aiStartLocalPlayers[2] = -1;
		_pGame->gm_aiStartLocalPlayers[3] = -1;
		_pGame->gm_strNetworkProvider = "Local";
		_pGame->gm_StartSplitScreenCfg = CGame::SSC_PLAY1;

		_pShell->SetINDEX("gam_iStartDifficulty", CSessionProperties::GD_NORMAL);
		_pShell->SetINDEX("gam_iStartMode", CSessionProperties::GM_FLYOVER);

		CUniversalSessionProperties sp;
		_pGame->SetSinglePlayerSession(sp);

		_pGame->gm_bFirstLoading = TRUE;

		if ( _pGame->PreNewGame() && _pGame->NewGame( sam_strIntroLevel, sam_strIntroLevel, sp)) {
			_gmRunningGameMode = GM_INTRO;
		}
	// if not intro
	} else {
		// start the demo
		_pGame->gm_StartSplitScreenCfg = CGame::SSC_OBSERVER;
		_pGame->gm_aiStartLocalPlayers[0] = -1;
		_pGame->gm_aiStartLocalPlayers[1] = -1;
		_pGame->gm_aiStartLocalPlayers[2] = -1;
		_pGame->gm_aiStartLocalPlayers[3] = -1;
		// play the demo
		_pGame->gm_strNetworkProvider = "Local";
		_gmRunningGameMode = GM_NONE;
		if( _pGame->StartDemoPlay( pli->li_fnLevel)) {
			_gmRunningGameMode = GM_DEMO;
			CON_DiscardLastLineTimes();
		}
	}

	if (_gmRunningGameMode==GM_NONE) {
		_bInAutoPlayLoop = FALSE;
	}
}

// set registry key for GameSpy
void RegisterGameSpy(void)
{
	char strExePath[MAX_PATH+1] = "";
	// get full path to the exe
	GetModuleFileName( NULL, strExePath, sizeof(strExePath)-1);

	// create the registry key
	HKEY hkey;
	DWORD dwDisposition;
	LONG lRes = RegCreateKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\GameSpy\\games",0,
		"", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, &dwDisposition);
	if (lRes!=ERROR_SUCCESS) {
		return;
	}

	// set the value to current application
#if !TECHTESTONLY
#if _SE_DEMO
		RegSetValueEx(hkey, "serioussamdemo", 0, REG_SZ, (UBYTE*)strExePath, strlen(strExePath));
#else
		RegSetValueEx(hkey, sam_strGameName, 0, REG_SZ, (UBYTE*)strExePath, strlen(strExePath));
#endif
#endif

	// close the key
	RegCloseKey(hkey);
}
*/

BOOL _bCDPathFound = FALSE;

BOOL FileExistsOnHD(const CTString &strFile)
{
	FILE *f = fopen(_fnmApplicationPath+strFile, "rb");
	if (f!=NULL) {
		fclose(f);
		return TRUE;
	} else {
		return FALSE;
	}
}
	

/*
// find possible cd path
BOOL FindCDPath(void)
{
	// if no specified cdpath
	if (_fnmCDPath=="") 
	{
		// use default one
		_fnmCDPath = CTString("C:\\Install\\");
	}

	// for each drive
	for(INDEX chDrive='C'; chDrive<='Z'; chDrive++) 
	{
		char strDrive[4];
		strDrive[0] = chDrive;
		strDrive[1] = ':';
		strDrive[2] = '\\';
		strDrive[3] = 0;
		// if not cdrom
		if (GetDriveType(strDrive) != DRIVE_CDROM) 
		{
			// skip it
			continue;
		}
		// substitute the drive letter
		((char*)(const char*)_fnmCDPath)[0] = chDrive;
		// if the path is valid
		FILE *f = fopen(_fnmCDPath+_fnmForCDCheck, "rb");
		_bCDPathFound = (BOOL)f;
		if (f!=NULL) 
		{
			// we have found the cd
			fclose(f);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL IsCDInDrive(void)
{
	CTFileName fnmExpanded;
	INDEX iType = ExpandFilePath(EFP_READ, CTString("Setup.exe"), fnmExpanded);
	if (iType!=EFP_FILE || fnmExpanded.HasPrefix(_fnmCDPath)) 
	{
		return FALSE;
	}

	fnmExpanded = fnmExpanded.FileDir()+_fnmForCDCheck;
	FILE *f = fopen(fnmExpanded, "rb");
	if (f==NULL) {
		return FALSE;
	}
	fclose(f);

	return TRUE;
}
*/

void TrimString(char *str)
{
	int i = strlen(str);
	if (str[i-1]=='\n' || str[i-1]=='\r') {
		str[i-1]=0;
	}
}

/*
BOOL PerformCDCheck(void)
{
	// default message
	char strTitle[256] = "CD check";
	char strMessage[256] = "Please insert the game CD";

	// first get the application filename
	char strTmpPath[MAX_PATH] = "";
	GetModuleFileName( NULL, strTmpPath, sizeof(strTmpPath)-1);
	int iLen = strlen(strTmpPath);
	// generate 'insert-cd' text filename
	strTmpPath[iLen-3] = 'c';
	strTmpPath[iLen-2] = 'd';
	strTmpPath[iLen-1] = 0;

	// try to load the file
	FILE *f = fopen(strTmpPath, "rt");
	if (f!=NULL) 
	{
		fgets(strTitle, sizeof(strTitle)-1, f);       TrimString(strTitle);
		fgets(strMessage, sizeof(strMessage)-1, f);   TrimString(strMessage);
		fclose(f);
	}

	// repeat
	FOREVER{
		// if cd path found
		if (FindCDPath()) 
		{
			// ok
			return TRUE;
		}
#if !CD_CHECK || _SE_DEMO || TECHTESTONLY
			return TRUE;
#endif

		// ask user to insert CD
		int iResult = MessageBox(_hwndMain, strMessage, strTitle, MB_ICONEXCLAMATION|MB_RETRYCANCEL);

		if (iResult!=IDRETRY) 
		{
			return FALSE;
		}
		Sleep(1000);
	}
}

// run web browser and view an url
void RunBrowser(const char *strUrl)
{
	int iResult = (int)ShellExecute( _hwndMain, "OPEN", strUrl, NULL, NULL, SW_SHOWMAXIMIZED);
	if (iResult<32) {
		// should report error?
		NOTHING;
	}
}
*/

void LoadAndForceTexture(CTextureObject &to, CTextureObject *&pto, const CTFileName &fnm)
{
	try 
	{
		to.SetData_t(fnm);
		CTextureData *ptd = (CTextureData*)to.GetData();
		ptd->Force( TEX_CONSTANT);
		ptd = ptd->td_ptdBaseTexture;
		if( ptd!=NULL) ptd->Force( TEX_CONSTANT);
		pto = &to;
	} 
	catch( char *pchrError) 
	{
		(void*)pchrError;
		pto = NULL;
	}
}

//안태훈 수정 시작	//(DevPartner Bug Fix)(2005-01-14)
HMODULE g_hGame = NULL;
//안태훈 수정 끝	//(DevPartner Bug Fix)(2005-01-14)
void InitializeGame(void)
{
	try {
#ifndef NDEBUG 
#define GAMEDLL ("Bin\\Debug\\Game"+_strModExt+"D.dll")
#else
#define GAMEDLL ("Bin\\Game"+_strModExt+".dll")
#endif
		CTFileName fnmExpanded;
		ExpandFilePath(EFP_READ, CTString(GAMEDLL), fnmExpanded);

		CPrintF(TRANS("Loading game library '%s'...\n"), (const char *)fnmExpanded);
//안태훈 수정 시작	//(DevPartner Bug Fix)(2005-01-14)
		g_hGame = LoadLibrary(fnmExpanded);
		if (g_hGame==NULL) 
		{
			ThrowF_t("%s", GetWindowsError(GetLastError()));
		}
		CGame* (*GAME_Create)(void) = (CGame* (*)(void))GetProcAddress(g_hGame, "GAME_Create");
		if (GAME_Create==NULL) 
		{
			ThrowF_t("%s", GetWindowsError(GetLastError()));
		}
//안태훈 수정 끝	//(DevPartner Bug Fix)(2005-01-14)
		_pGame = GAME_Create();		
	} 
	catch (char *strError) 
	{
		FatalError("%s", strError);
	}
	// init game - this will load persistent symbols
	_pGame->Initialize(CTString("Data\\nksp.gms"));

//		ENGINE_API extern float g_fGWTimeMul;
//		ENGINE_API extern float g_fGWTime;			// Game world time
//		g_fGWTimeMul	= 0.0f;
//		g_fGWTime		= 100.0f;
}

static char strExePath[MAX_PATH] = "";
static char strDirPath[MAX_PATH] = "";
CEntityClass *g_pPlayer, *g_pWorldBase;
ENGINE_API void CheckEngineVersion();

BOOL Init( HINSTANCE hInstance, int nCmdShow, CTString strCmdLine)
{

	// 새로운 버전의 Launcher가 있는가 확인하고 있다면, Laucher를 업데이트한다.
	// by seo - 40727
	// get full path to the exe
	char strTmpPath[MAX_PATH] = "";
	GetModuleFileName( NULL, strExePath, sizeof(strExePath)-1);

	for (int i=0; i < 2; i++) {
		for (int j=strlen(strExePath)-1; j > 0; j--) {
			if (strExePath[j] == '\\') {
				strExePath[j] = '\0';
				break;
			}
		}
	}
	strcat(strExePath, "\\");

	CTFileName fnmLC = strExePath;
	CTFileName fnmLCNew = strExePath;
	CTFileName fnmLW = strExePath;
	CTFileName fnmLWNew = strExePath;

	fnmLC = fnmLC + "LC.exe";
	fnmLCNew = fnmLCNew + "LCNew.exe";

	// 천하대란용.
	fnmLW = fnmLW + "LW.exe";
	fnmLWNew = fnmLWNew + "LWNew.exe";



	FILE *fp = fopen (fnmLCNew, "rb");
	if (fp) {
		fclose(fp);
		Sleep(1000);
		remove (fnmLC);
		Sleep(1000);
		int ret = rename(fnmLCNew, fnmLC);
		// Date : 2005-05-17(오후 4:44:15), By Lee Ki-hwan 추가
		if (!ret) {
			ShowCursor(TRUE);
			if( GetSystemDefaultLangID() == 0x0804 ) // 중국 로컬의 경우 수정
			{
				MessageBox(NULL, "헝듐샌횅땍，路劤폘땡질산。", "질산", MB_OK); 
			}
			else 
			{
				MessageBox(NULL, "Update is completed.\nPlease, restart game.", "Update completed", MB_OK);
			}
			return 0;
		}
	}

	fp = NULL;
	fp = fopen (fnmLWNew, "rb");
	if (fp) {
		fclose(fp);
		Sleep(1000);
		remove (fnmLW);
		Sleep(1000);
		int ret = rename(fnmLWNew, fnmLW);
		// Date : 2005-05-17(오후 4:44:15), By Lee Ki-hwan 추가
		if (!ret) {
			ShowCursor(TRUE);
			if( GetSystemDefaultLangID() == 0x0804 ) // 중국 로컬의 경우 수정
			{
				MessageBox(NULL, "헝듐샌횅땍，路劤폘땡질산。", "질산", MB_OK); 
			}
			else 
			{
				MessageBox(NULL, "Update is completed.\nPlease, restart game.", "Update completed", MB_OK);
			}
			return 0;
		}
	}
	// end.

	_hInstanceMain = hInstance;
	//0214 스플래시 안띄우기.
	//  ShowSplashScreen(hInstance);

	// set registry key for GameSpy
	// deleted by seo - 40629
	//RegisterGameSpy();

	// remember desktop dimensions
	_pixDesktopWidth  = ::GetSystemMetrics(SM_CXSCREEN);
	_pixDesktopHeight = ::GetSystemMetrics(SM_CYSCREEN);

	if(strCmdLine.IsEqualCaseSensitive(CTString("6574"))) ;
	else
	{
		// WSS_NPROTECT 070405 ------------------>><<	
		// 홍콩의 경우 nProtect로 중복 검출
#ifdef NO_GAMEGUARD
		if (IsRunning())
		{
			MessageBox(NULL, "LastChaos is already running on your system", "LastChaos Run", MB_OK);
			return FALSE;
		}		
#endif
		const CTString CmpStr("dlwltnr");
		char* strPos = '\0';

		strPos = strstr(strCmdLine.str_String, CmpStr.str_String);

		if (!strPos || !CmpStr.IsEqualCaseSensitive(strPos))
		{
			MessageBox(NULL, "This program could not be run itself", "LastChaos", MB_OK);
			return FALSE;
		}
	}
	// prepare main window

	MainWindow_Init();
	OpenMainWindowInvisible();

	// parse command line before initializing engine
	ParseCommandLine(strCmdLine);

	/*#if !_SE_DEMO
	if (!PerformCDCheck()) {
		return FALSE;
	}
#endif*/

	// initialize engine
	SE_InitEngine(sam_strGameName);

	extern BOOL g_bNasTrans;
	g_bNasTrans = FALSE;
	switch(g_iCountry)
	{
		case BRAZIL:
			SetClassLong( _hwndMain, GCL_HICON, (LONG)LoadIcon( _hInstanceMain, (LPCTSTR)IDI_GUNSOFT ) );
			g_bNasTrans = TRUE;
			break;

	}

	// recheck for proper CD now
/*
	if (FileExistsOnHD("Bin\\EntitiesMP.dll")) {
		_fnmForCDCheck = "Bin\\EntitiesMP.dll";
	} else if (FileExistsOnHD("Bin\\Entities.dll")) {
		_fnmForCDCheck = "Bin\\Entities.dll";
	}
*/
	/*#if !_SE_DEMO
	if (!PerformCDCheck()) {
		return FALSE;
	}
#endif*/

#if COPY_PROTECTION

	// get full path to the exe
	char strExePath[MAX_PATH] = "";
	GetModuleFileName( NULL, strExePath, sizeof(strExePath)-1);
	char strDrive[] = "C:\\";
	strDrive[0] = strExePath[0];
	DWORD dwSerial;
	GetVolumeInformation(strDrive, NULL, 0, &dwSerial, NULL, NULL, NULL, 0);

	CTFileName fnmKey = CTString("TSE.key");
	INDEX iSerial = dwSerial;

	// init encryption table two times
	BLOWFISH_CTX ctx1;
	BLOWFISH_CTX ctx2;
	CTString strEnKey = _strEngineBuild;
	strEnKey.TrimLeft(16);
	char aubKey1[16];
	char aubKey2[16];
	for(INDEX i=0; i<16; i++) {
		aubKey1[i] = strEnKey[i];
		aubKey2[i] = strEnKey[16-1-i];
	}
	Blowfish_Init (&ctx1, (unsigned char*)aubKey1, 16);
	Blowfish_Init (&ctx2, (unsigned char*)aubKey2, 16);

	// if no key
	if (!FileExists(fnmKey)) {
		// just bail out
		FatalError(TRANS("No registration key!\nPlease request registration key for code:\n\n%08X"), iSerial);
		return FALSE;
	// if key is here
	} else {
		// load it
		CTString strKey;
		LoadStringVar(fnmKey, strKey);
		__int64 llCodeRead = 0;
		strKey.ScanF("%I64x", &llCodeRead);
		__int64 llCode1 = llCodeRead;
		__int64 llCode2 = llCodeRead;

		// decrypt the code
		Blowfish_Decrypt(&ctx1, (ULONG*)&llCode1+1, ((ULONG*)&llCode1));
		Blowfish_Decrypt(&ctx2, (ULONG*)&llCode2+1, ((ULONG*)&llCode2));
		// if not valid
		if (llCode1!=iSerial && llCode2!=iSerial) {
			// bail out
			FatalError(TRANS("Registration key is invalid!"));
			return FALSE;
		}
	}
#endif

	// if cd check didn't pass
	/*  
	if (!_bCDPathFound) {
		// check current time
		SYSTEMTIME st;
		GetSystemTime(&st);
		// if after 30.3.2001, fail
		if (st.wYear>2001) {
			return FALSE;
		} else if (st.wYear==2001) {
			if (st.wMonth>3) {
				return FALSE;
			} else if (st.wMonth==3) {
				if (st.wDay>30) {
					return FALSE;
				}
			}
		}
	}
	*/

	SE_LoadDefaultFonts();
	// now print the output of command line parsing
	// deleted by seo
	//CPrintF("%s", cmd_strOutput);

	// lock the directory
	DirectoryLockOn();

	// load all translation tables
	InitTranslation();
	try {
		AddTranslationTablesDir_t(CTString("Data\\Translations\\"), CTString("*.txt"));
		FinishTranslationTable();
	} catch (char *strError) {
		FatalError("%s", strError);
	}

	// always disable all warnings when in serious sam
	_pShell->Execute( "con_bNoWarnings=1;");

	// declare shell symbols
//	_pShell->DeclareSymbol("user void PlayDemo(CTString);", &PlayDemo);
	_pShell->DeclareSymbol("persistent INDEX sam_bFullScreen;",   &sam_bFullScreenActive);
	
	// 기본의 원도우 모드 시에 전체 모드로 변경 
	/*
		if( OLD_WINDOW_MODE != WINDOW_MODE )
		if ( sam_bFullScreenActive == OLD_WINDOW_MODE ) sam_bFullScreenActive = FULLSCREEN_MODE;
	*/
	// Date : 2005-09-13(오후 2:58:13), By Lee Ki-hwan
	// 대만에서 기본 원도우 모드를 지원하면서 필요없어짐

	_pShell->DeclareSymbol("persistent INDEX sam_iScreenSizeI;",  &sam_iScreenSizeI);
	_pShell->DeclareSymbol("persistent INDEX sam_iScreenSizeJ;",  &sam_iScreenSizeJ);
	_pShell->DeclareSymbol("persistent INDEX sam_iDisplayDepth;", &sam_iDisplayDepth);
	_pShell->DeclareSymbol("persistent INDEX sam_iDisplayAdapter;", &sam_iDisplayAdapter);
	_pShell->DeclareSymbol("persistent INDEX sam_iGfxAPI;",         &sam_iGfxAPI);
	//_pShell->DeclareSymbol("persistent INDEX sam_bFirstStarted;", &sam_bFirstStarted);
	_pShell->DeclareSymbol("persistent INDEX sam_bAutoAdjustAudio;", &sam_bAutoAdjustAudio);
	_pShell->DeclareSymbol("persistent user INDEX sam_bWideScreen;", &sam_bWideScreen);
	_pShell->DeclareSymbol("persistent user FLOAT sam_fPlayerOffset;",  &sam_fPlayerOffset);
	_pShell->DeclareSymbol("persistent user INDEX sam_bAutoPlayDemos;", &sam_bAutoPlayDemos);
	_pShell->DeclareSymbol("persistent user INDEX sam_iMaxFPSActive;",    &sam_iMaxFPSActive);
	_pShell->DeclareSymbol("persistent user INDEX sam_iMaxFPSInactive;",  &sam_iMaxFPSInactive);
	_pShell->DeclareSymbol("persistent user INDEX sam_bPauseOnMinimize;", &sam_bPauseOnMinimize);
	_pShell->DeclareSymbol("persistent user FLOAT sam_tmDisplayModeReport;",   &sam_tmDisplayModeReport);
	_pShell->DeclareSymbol("persistent user CTString sam_strNetworkSettings;", &sam_strNetworkSettings);
	_pShell->DeclareSymbol("persistent user CTString sam_strIntroLevel;",      &sam_strIntroLevel);
	_pShell->DeclareSymbol("persistent user CTString sam_strGameName;",      &sam_strGameName);
	_pShell->DeclareSymbol("user CTString sam_strVersion;",    &sam_strVersion);
	//_pShell->DeclareSymbol("user CTString sam_strFirstLevel;", &sam_strFirstLevel);
	//_pShell->DeclareSymbol("user CTString sam_strModName;", &sam_strModName);
//	_pShell->DeclareSymbol("persistent INDEX sam_bShowAllLevels;", &sam_bShowAllLevels);
//	_pShell->DeclareSymbol("persistent INDEX sam_bMentalActivated;", &sam_bMentalActivated);

	//_pShell->DeclareSymbol("user CTString sam_strTechTestLevel;", &sam_strTechTestLevel);
	//_pShell->DeclareSymbol("user CTString sam_strTrainingLevel;", &sam_strTrainingLevel);
	
	_pShell->DeclareSymbol("user void Quit(void);", &QuitGame);

	_pShell->DeclareSymbol("persistent user INDEX sam_iVideoSetup;",     &sam_iVideoSetup);
	_pShell->DeclareSymbol("user void ApplyRenderingPreferences(void);", &ApplyRenderingPreferences);
	//_pShell->DeclareSymbol("user void ApplyVideoMode(void);",            &ApplyVideoMode);
	_pShell->DeclareSymbol("user void Benchmark(void);", &BenchMark);

	/*
	_pShell->DeclareSymbol("user INDEX sam_bMenuSave;",     &sam_bMenuSave);
	_pShell->DeclareSymbol("user INDEX sam_bMenuLoad;",     &sam_bMenuLoad);
	_pShell->DeclareSymbol("user INDEX sam_bMenuControls;", &sam_bMenuControls);
	_pShell->DeclareSymbol("user INDEX sam_bMenuHiScore;",  &sam_bMenuHiScore);
	*/
	_pShell->DeclareSymbol("user INDEX sam_bToggleConsole;",&sam_bToggleConsole);
//	_pShell->DeclareSymbol("INDEX sam_iStartCredits;", &sam_iStartCredits);

	// Obtain player and world base class without releasing it (this will never be released from stock)
	g_pPlayer = _pEntityClassStock->Obtain_t(CTString("Classes\\Player.ecl"));  // this must not be a dependency!
	g_pWorldBase = _pEntityClassStock->Obtain_t(CTString("Classes\\WorldBase.ecl"));  // this must not be a dependency!

	InitializeGame();
	_pNetwork->md_strGameID = sam_strGameName;

	//LCDInit();

	/*if( sam_bFirstStarted) {
		InfoMessage("%s", TRANS(
			"SeriousSam is starting for the first time.\n"
			"If you experience any problems, please consult\n"
			"ReadMe file for troubleshooting information."));
	}*/

	// initialize sound library
	snd_iFormat = Clamp( snd_iFormat, (INDEX)CSoundLibrary::SF_NONE, (INDEX)CSoundLibrary::SF_44100_16);
	_pSound->SetFormat( (enum CSoundLibrary::SoundFormat)snd_iFormat);

	if (sam_bAutoAdjustAudio) 
	{
		_pShell->Execute("include \"Scripts\\Addons\\SFX-AutoAdjust.ini\"");
	}

	/*
	// execute script given on command line
	if (cmd_strScript!="") 
	{
		CPrintF("Command line script: '%s'\n", cmd_strScript);
		CTString strCmd;
		strCmd.PrintF("include \"%s\"", cmd_strScript);
		_pShell->Execute(strCmd);
	}
	*/
	
	// load logo textures
	//LoadAndForceTexture(_toLogoCT,   _ptoLogoCT,   CTFILENAME("Textures\\Logo\\LogoCT.tex"));
	//LoadAndForceTexture(_toLogoODI,  _ptoLogoODI,  CTFILENAME("Textures\\Logo\\GodGamesLogo.tex"));
	//LoadAndForceTexture(_toLogoGSpy, _ptoLogoGSpy, CTFILENAME("Textures\\Logo\\LogoGSpy.tex"));
	//LoadAndForceTexture(_toLogoEAX,  _ptoLogoEAX,  CTFILENAME("Textures\\Logo\\LogoEAX.tex"));

	//LoadStringVar(CTString("Data\\Var\\Sam_Version.var"), sam_strVersion);
	//LoadStringVar(CTString("Data\\Var\\ModName.var"), sam_strModName);
	CPrintF(TRANS("Nksp version: %s\n"), sam_strVersion);
	//CPrintF(TRANS("Active mod: %s\n"), sam_strModName);
	//InitializeMenus();		// 원본.
	
	// if there is a mod
	/*if (_fnmMod!="") {
		// execute the mod startup script
		_pShell->Execute(CTString("include \"Scripts\\Mod_startup.ini\";"));
	}*/

	// init gl settings module
	// deleted by seo
	//InitGLSettings();

	// init level-info subsystem
	// deleted by seo
	//LoadLevelsList();
	//LoadDemosList();

	// apply application mode
	StartNewMode( GAT_D3D, sam_iDisplayAdapter, sam_iScreenSizeI, sam_iScreenSizeJ,
								(enum DisplayDepth)sam_iDisplayDepth, IsFullScreen( sam_bFullScreenActive) );

	_pGameState = new CGameState;
	_pUIMgr = new CUIManager;				// yjpark
	_pUIMgr->Create();						// yjpark

	//wooss 050822 add test ui version
	// TEST UI를 위해 추가
	// TEST 환경이 아닐 경우 이부분을 FALSE로 처리 
	// TEST 하고자 하는 UI 변경시 TYPE 부분을 고쳐 준다 
	//------------------------------------------------------------------------------>>
	
//		_pUIMgr->m_testUI_MODE = TRUE;
		_pUIMgr->m_testUI_MODE = FALSE;
		_pUIMgr->m_testUI_TYPE = UI_SELECTLIST;		//wooss  UI test variable
	
	//<<------------------------------------------------------------------------------

	if(_pUIMgr->m_testUI_MODE) _pUIMgr->SetUIGameState( UGS_UI_TEST);
	else _pUIMgr->SetUIGameState( UGS_LOGIN );	// yjpark
	_pUIMgr->GetOption()->SetDesktopSize( _pixDesktopWidth, _pixDesktopHeight );
	_pUIMgr->SetGameHandle(_pGame);
	_pUIMgr->ResetUIPos( _pdpMain );

//강동민 수정 시작 시스템 마우스 작업	09.09
//#define ANIMATION_CURSOR
#ifdef ANIMATION_CURSOR
	_pUIMgr->GetMouseCursor()->SetCursorHandle( UMCT_NORMAL, LoadAnimationCursor( _hInstanceMain, IDC_NORMAL_CURSOR) );
	_pUIMgr->GetMouseCursor()->SetCursorHandle( UMCT_ATT_NORMAL, LoadAnimationCursor( _hInstanceMain, IDC_ATTACK_CURSOR) );
	_pUIMgr->GetMouseCursor()->SetCursorHandle( UMCT_ATT_SKILL, LoadAnimationCursor( _hInstanceMain, IDC_SKILL_CURSOR) );
	_pUIMgr->GetMouseCursor()->SetCursorHandle( UMCT_ATT_MAGIC, LoadAnimationCursor( _hInstanceMain, IDC_MAGIC_CURSOR) );
	_pUIMgr->GetMouseCursor()->SetCursorHandle( UMCT_ATT_BOW, LoadAnimationCursor( _hInstanceMain, IDC_BOW_CURSOR) );
	_pUIMgr->GetMouseCursor()->SetCursorHandle( UMCT_PICK, LoadAnimationCursor( _hInstanceMain, IDC_PICK_CURSOR) );
	_pUIMgr->GetMouseCursor()->SetCursorHandle( UMCT_TALK, LoadAnimationCursor( _hInstanceMain, IDC_TALK_CURSOR) );
	_pUIMgr->GetMouseCursor()->SetCursorHandle( UMCT_PRODUCE, LoadAnimationCursor( _hInstanceMain, IDC_PRODUCE_CURSOR) );
	_pUIMgr->GetMouseCursor()->SetCursorHandle( UMCT_SIZE, LoadAnimationCursor( _hInstanceMain, IDC_SIZE_CURSOR) );
	_pUIMgr->GetMouseCursor()->SetCursorHandle( UMCT_ZOOMIN, LoadAnimationCursor( _hInstanceMain, IDC_ZOOMIN_CURSOR) );
	_pUIMgr->GetMouseCursor()->SetCursorHandle( UMCT_SIGNAL, LoadAnimationCursor( _hInstanceMain, IDC_SIGNAL_CURSOR) );
#else
	// FIXME : Engine의 Resource에 *.cur 화일을 넣어줬을때, 해당 커서를 얻어오지 못해서 이런식으로 처리함.	
	_pUIMgr->GetMouseCursor()->SetCursorHandle( UMCT_NORMAL, LoadCursor( _hInstanceMain, MAKEINTRESOURCE(IDC_NORMAL_CURSOR) ) );
	_pUIMgr->GetMouseCursor()->SetCursorHandle( UMCT_ATT_NORMAL, LoadCursor( _hInstanceMain, MAKEINTRESOURCE(IDC_ATTACK_CURSOR) ) );
	_pUIMgr->GetMouseCursor()->SetCursorHandle( UMCT_ATT_SKILL, LoadCursor( _hInstanceMain, MAKEINTRESOURCE(IDC_SKILL_CURSOR) ) );
	_pUIMgr->GetMouseCursor()->SetCursorHandle( UMCT_ATT_MAGIC, LoadCursor( _hInstanceMain, MAKEINTRESOURCE(IDC_MAGIC_CURSOR) ) );
	_pUIMgr->GetMouseCursor()->SetCursorHandle( UMCT_ATT_BOW, LoadCursor( _hInstanceMain, MAKEINTRESOURCE(IDC_BOW_CURSOR) ) );
	_pUIMgr->GetMouseCursor()->SetCursorHandle( UMCT_PICK, LoadCursor( _hInstanceMain, MAKEINTRESOURCE(IDC_PICK_CURSOR) ) );
	_pUIMgr->GetMouseCursor()->SetCursorHandle( UMCT_TALK, LoadCursor( _hInstanceMain, MAKEINTRESOURCE(IDC_TALK_CURSOR) ) );
	_pUIMgr->GetMouseCursor()->SetCursorHandle( UMCT_PRODUCE, LoadCursor( _hInstanceMain, MAKEINTRESOURCE(IDC_PRODUCE_CURSOR) ) );
	_pUIMgr->GetMouseCursor()->SetCursorHandle( UMCT_SIZE, LoadCursor( _hInstanceMain, MAKEINTRESOURCE(IDC_SIZE_CURSOR) ) );
	_pUIMgr->GetMouseCursor()->SetCursorHandle( UMCT_ZOOMIN, LoadCursor( _hInstanceMain, MAKEINTRESOURCE(IDC_ZOOMIN_CURSOR) ) );
	_pUIMgr->GetMouseCursor()->SetCursorHandle( UMCT_SIGNAL, LoadCursor( _hInstanceMain, MAKEINTRESOURCE(IDC_SIGNAL_CURSOR) ) );
	
#endif
//강동민 수정 끝 시스템 마우스 작업		09.09
	// set default mode reporting
	/*if( sam_bFirstStarted) {
		_iDisplayModeChangeFlag = 0;
		sam_bFirstStarted = FALSE;
	}*/
	
	//0214 스플래시 숨기기
	//  HideSplashScreen();

	/*
	if (cmd_strPassword!="") {
		_pShell->SetString("net_strConnectPassword", cmd_strPassword);
	}

	  #if TECHTESTONLY
	cmd_strWorld = CTString("Levels\\TechTestElsa.wld");
	  #endif

		if (cmd_strPlayer!="") 
		{
		int iPlayer = atoi(cmd_strPlayer);
		_pGame->gm_aiMenuLocalPlayers[0] = Clamp(iPlayer, 0, 7);
	}
	*/
	//0105 42line 지우기
	/* 
	// if connecting to server from command line
	if (cmd_strServer!="") {
		CTString strPort = "";
		if (cmd_iPort>0) {
			_pShell->SetINDEX("net_iPort", cmd_iPort);
			strPort.PrintF(":%d", cmd_iPort);
		}
		CPrintF(TRANS("Command line connection: '%s%s'\n"), cmd_strServer, strPort);
		// go to join menu
		_pGame->gam_strJoinAddress = cmd_strServer;
		if (cmd_bQuickJoin) {
			extern void JoinNetworkGame(void);
			JoinNetworkGame();
		} else {
			StartMenus("join");
		}
	// if starting world from command line
	} else if (cmd_strWorld!="") {
		CPrintF(TRANS("Command line world: '%s'\n"), cmd_strWorld);
		// try to start the game with that level
		try {
			if (cmd_iGoToMarker>=0) {
				CPrintF(TRANS("Command line marker: %d\n"), cmd_iGoToMarker);
				CTString strCommand;
				strCommand.PrintF("cht_iGoToMarker = %d;", cmd_iGoToMarker);
				_pShell->Execute(strCommand);
			}
			_pGame->gam_strCustomLevel = cmd_strWorld;
			if (cmd_bServer) {
				extern void StartNetworkGame(void);
				StartNetworkGame();
			} else {
				extern void StartSinglePlayerGame(void);
				StartSinglePlayerGame();
			}
		} catch (char *strError) {
			CPrintF(TRANS("Cannot start '%s': '%s'\n"), cmd_strWorld, strError);
		}
	// if no relevant starting at command line
	} else {
		StartNextDemo();
	}
	*/
	//0105
	//_pGame->gam_strJoinAddress = "211.56.73.22";
	//_pGame->gm_strNetworkProvider = "TCP/IP Client";
	/*
	_pGame->gam_strJoinAddress = "211.56.73.12";		// by seo 40225
	_pGame->gm_strNetworkProvider = "TCP/IP Mmo";
	 
	if (_pGame->JoinGame( CNetworkSession( _pGame->gam_strJoinAddress)))
	{
	 _gmRunningGameMode = GM_NETWORK;
	}
	  //..
	*/
	//_pGameState->m_BackGroundWorld.Load_t(LOGIN_WORLD);
	{
		/*
		_pGame->gm_strNetworkProvider = "Local";
		_pGame->gm_aiStartLocalPlayers[0] = 0;
		_pGame->gm_CurrentSplitScreenCfg = CGame::SSC_PLAY1;

		CSessionProperties sp;
		_pGame->SetQuickStartSession(sp);
		CPlayerCharacter &pc = _pGame->gm_apcPlayers[0];
		pc.pc_iPlayerIndex = 0;
		if(!_pGame->PreNewGame() || !_pGame->NewGame( LOGIN_WORLD, LOGIN_WORLD, sp))
		{
			//DisableLoadingHook();
		}
		//_pNetwork->ga_World;
		//_pGameState->m_BackGroundWorld.Load_t(LOGIN_WORLD);
		_pUIMgr->SetBackgroundWorld(&_pNetwork->ga_World);	
		*/

		/*
		while(TRUE)
		{
			GameMainLoop();			
			pdp->SetAsCurrent();
			pdp->Fill(C_BLACK|CT_OPAQUE);
			pdp->FillZBuffer(ZBUF_BACK);
			GameRedrawView(pdp, GRV_SHOWEXTRAS);
			pvp->SwapBuffers();			
		};

		StopGame();
		DisableLoadingHook();
		*/
	}

//안태훈 수정 시작	//(5th Closed beta)(0.2)
	WebAddressInit();
	g_web.Begin();
//안태훈 수정 끝	//(5th Closed beta)(0.2)

	// 051121 nksp.exe version check on/off  
	if(strCmdLine.IsEqualCaseSensitive(CTString("6574"))) ;
	else { 
		CheckEngineVersion();
		CheckEntityVersion();
	}

	return TRUE;
}

void End(void)
{
//안태훈 수정 시작	//(5th Closed beta)(0.2)
	g_web.End();
//안태훈 수정 끝	//(5th Closed beta)(0.2)
	_pGame->DisableLoadingHook();
	// cleanup level-info subsystem
	//ClearLevelsList();		// 원본.
	//ClearDemosList();

	_pFontStock->Release(_pfdDisplayFont);
	_pfdDisplayFont = NULL;
	_pFontStock->Release(_pfdConsoleFont);
	_pfdConsoleFont = NULL;

	_pEntityClassStock->Release(g_pPlayer);
	g_pPlayer = NULL;
	_pEntityClassStock->Release(g_pWorldBase);
	g_pWorldBase = NULL;
	
	// destroy the main window and its canvas
	if (_pvpViewPortMain!=NULL) 
	{
 		_pGfx->DestroyWindowCanvas( _pvpViewPortMain);
		_pvpViewPortMain = NULL;
		_pdpNormalMain   = NULL;
	}
	CloseMainWindow();
	MainWindow_End();
	//DestroyMenus();		// 원본.
	_pGame->End();

	//LCDEnd();
	// unlock the directory
	DirectoryLockOff();
	SE_EndEngine();
	
	void (*GAME_Destroy)(void) = (void (*)(void))GetProcAddress(g_hGame, "GAME_Destroy");
	if (GAME_Destroy==NULL) 
	{
		ThrowF_t("%s", GetWindowsError(GetLastError()));
	}
	GAME_Destroy();
//안태훈 수정 시작	//(DevPartner Bug Fix)(2005-01-14)
	if(g_hGame) FreeLibrary(g_hGame);
//안태훈 수정 끝	//(DevPartner Bug Fix)(2005-01-14)
}


// print display mode info if needed
void PrintDisplayModeInfo(void)
{
	// skip if timed out
	if( _pTimer->GetRealTimeTick() > (_tmDisplayModeChanged+sam_tmDisplayModeReport)) return;

	// cache some general vars
	SLONG slDPWidth  = _pdpMain->GetWidth();
	SLONG slDPHeight = _pdpMain->GetHeight();
	if( _pdpMain->IsDualHead())   slDPWidth/=2;
	if( _pdpMain->IsTripleHead()) slDPWidth/=3;

	CDisplayMode dm;
	dm.dm_pixSizeI = slDPWidth;
	dm.dm_pixSizeJ = slDPHeight;
	// determine proper text scale for statistics display
	FLOAT fTextScale = (FLOAT)slDPHeight/480.0f/2;

	// get resolution
	CTString strRes;
	extern CTString _strPreferencesDescription;
	strRes.PrintF( "%dx%dx%s", slDPWidth, slDPHeight, _pGfx->gl_dmCurrentDisplayMode.DepthString());
	if( dm.IsDualHead())   strRes += TRANS(" DualMonitor");
	if( dm.IsTripleHead()) strRes += TRANS(" TripleMonitor");
	if( dm.IsWideScreen()) strRes += TRANS(" WideScreen");
			 if( _pGfx->gl_eCurrentAPI==GAT_OGL) strRes += " (OpenGL)";
	else 
		if( _pGfx->gl_eCurrentAPI==GAT_D3D) strRes += " (Direct3D)";

	CTString strDescr;
	strDescr.PrintF("\n%s (%s)\n", _strPreferencesDescription, RenderingPreferencesDescription(sam_iVideoSetup));
	strRes += strDescr;
	// tell if application is started for the first time, or failed to set mode
		if( _iDisplayModeChangeFlag==0) 
		{
		strRes += TRANS("Display mode set by default!");
		} 
		else if( _iDisplayModeChangeFlag==2) 
		{
		strRes += TRANS("Last mode set failed!");
	}

	// print it all
	_pdpMain->SetFont( _pfdDisplayFont);
	_pdpMain->SetTextScaling( fTextScale);
	_pdpMain->SetTextShadow( +2);
	_pdpMain->SetTextAspect( 1.0f);
	const FLOAT fPosMul = _pdpMain->IsTripleHead() ? 1.05f : 0.05f;
	//pdp->PutText( strRes, slDPWidth*fPosMul, slDPHeight*0.85f, LCDGetColor(C_GREEN|255, "display mode"));		// 원본.
	_pdpMain->PutText( strRes, slDPWidth*fPosMul, slDPHeight*0.85f);
}

// do the main game loop and render screen
void DoGame(void)
{
	// set flag if not in game
	if( !_pGame->gm_bGameOn) 
		//_gmRunningGameMode = GM_NONE;		// 원본.		
		_pGameState->GetGameMode() = CGameState::GM_NONE;

	// 게임 모드가 데모모드일때...
	/*
	if( _gmRunningGameMode==GM_DEMO  && _pNetwork->IsDemoPlayFinished()
		||_gmRunningGameMode==GM_INTRO && _pNetwork->IsGameFinished()) 
	{
		_pGame->StopGame();
		_gmRunningGameMode = GM_NONE;

		// load next demo
		StartNextDemo();
	  if (!_bInAutoPlayLoop) {
			// start menu
			StartMenus();
		}
	}
	*/
	/*
	// do the main game loop
	if( _gmRunningGameMode == GM_LOGIN){
	_pNetwork->
	}
	else*/


	// 게임 실행중일때 상태!
	//_pGameState->m_bMenuActive	0
	//_pGameState->m_bMenuRendering	1
	//_gmMenuGameMode	GM_NONE
	//_gmRunningGameMode	GM_NETWORK
	//_pGame->gm_bGameOn	1
	//_pGame->gm_bMenuOn	0

	// 게임이 실행중일때만 호출되는 부분.
	//if( _gmRunningGameMode != GM_NONE) 
	//if( _gmRunningGameMode != GM_NONE)		// 원본.
	if( _pGameState->GetGameMode() != CGameState::GM_NONE)		// 원본.
	{
		_pGame->GameMainLoop();
		//0215
		//	 CSoundObject soMusic;  
		//     soMusic.Play_t(CTFILENAME("Music\\Credits.mp3"), SOF_NONGAME|SOF_MUSIC|SOF_LOOP);
		//
	// if game is not started
	} 
	else 
	{
		// just handle broadcast messages
		_pNetwork->GameInactive();
	}

	// redraw the view
	if( !IsIconic(_hwndMain) && _pdpMain!=NULL && _pdpMain->Lock())
	{
		// 메뉴가 비활성화 되었고, 게임 실행중일때???
		//if( _gmRunningGameMode!=GM_NONE && !_pGameState->m_bMenuActive ) 
		//if( _gmRunningGameMode!=GM_NONE && !_pGameState->m_bMenuActive )		// 원본.
		int iState = _pGameState->GetGameMode();
		//BOOL bState = _pGameState->m_bMenuActive;
		if( _pGameState->GetGameMode() != CGameState::GM_NONE)		// 원본.
		{
			// handle pretouching of textures and shadowmaps
			_pdpMain->Unlock();

			// 결국은 RenderView()를 호출함.
			_pGame->GameRedrawView( _pdpMain, (_pGame->gm_csConsoleState!=CS_OFF)?0:GRV_SHOWEXTRAS);

			_pdpMain->Lock();			
		} 
		else 
		{
			//pdp->Fill( LCDGetColor(C_dGREEN|CT_OPAQUE, "bcg fill"));	// 원본.
			_pdpMain->Fill(C_dGREEN|CT_OPAQUE);
		}

		// do menu
		/*
		if( _pGameState->m_bMenuRendering) {
			// clear z-buffer
			pdp->FillZBuffer( ZBUF_BACK);
			// remember if we should render menus next tick
			_pGameState->m_bMenuRendering = DoMenu(pdp);
		}
		*/
		//_pUIMgr->SetBackgroundWorld(&_pGameState->m_BackGroundWorld);
		//pdp->FillZBuffer( ZBUF_BACK);

		/*
		TIME tmTickNow = _pTimer->GetRealTimeTick();
		
		while( _tmMenuLastTickDone < tmTickNow)
		{
			_pTimer->SetCurrentTick(_tmMenuLastTickDone);
			_tmMenuLastTickDone += _pTimer->TickQuantum;
		}
			
		SetMenuLerping();
		*/

		//if(_pGameState->m_bMenuRendering)
		// Render User interface
		//_pUIMgr->Render( _pdpMain );			// yjpark

		//0609 kwon
		_pGame->GameRedrawCursor( _pdpMain, (_pGame->gm_csConsoleState!=CS_OFF)?0:GRV_SHOWEXTRAS);

		// print display mode info if needed
		//PrintDisplayModeInfo();

		// render console
		// 콘솔창 렌더링.
		_pGame->ConsoleRender(_pdpMain);

		// done with all
		_pdpMain->Unlock();

		// clear upper and lower parts of screen if in wide screen mode
/*
		if( _pdpMain==_pdpWideScreenMain && _pdpNormalMain->Lock()) {
			const PIX pixWidth  = _pdpWideScreenMain->GetWidth();
			const PIX pixHeight = (_pdpNormalMain->GetHeight() - _pdpWideScreenMain->GetHeight()) /2;
			const PIX pixJOfs   = pixHeight + _pdpWideScreenMain->GetHeight()-1;
			_pdpNormalMain->Fill( 0, 0,       pixWidth, pixHeight, C_BLACK|CT_OPAQUE);
			_pdpNormalMain->Fill( 0, pixJOfs, pixWidth, pixHeight, C_BLACK|CT_OPAQUE);
			_pdpNormalMain->Unlock();
		}
*/
		// show
		_pvpViewPortMain->SwapBuffers();
	}
}


/*
void TeleportPlayer(int iPosition)
{
CTString strCommand;
strCommand.PrintF( "cht_iGoToMarker = %d;", iPosition);
_pShell->Execute(strCommand);
}
*/


CTextureObject _toStarField;
static FLOAT _fLastVolume = 1.0f;
void RenderStarfield(CDrawPort *pdp, FLOAT fStrength)
{
	CTextureData *ptd = (CTextureData *)_toStarField.GetData();
	// skip if no texture
	if(ptd==NULL) return;

	PIX pixSizeI = pdp->GetWidth();
	PIX pixSizeJ = pdp->GetHeight();
	FLOAT fStretch = pixSizeJ / 480.0f;
	fStretch *= FLOAT(ptd->GetPixWidth()) / ptd->GetWidth();

	PIXaabbox2D boxScreen(PIX2D(0,0), PIX2D(pixSizeI, pixSizeJ));
	MEXaabbox2D boxTexture(MEX2D(0, 0), MEX2D(pixSizeI/fStretch, pixSizeJ/fStretch));
	pdp->PutTexture(&_toStarField, boxScreen, boxTexture, LerpColor(C_BLACK, C_WHITE, fStrength)|CT_OPAQUE);
}


FLOAT RenderQuitScreen(CDrawPort *pdp, CViewPort *pvp)
{
	CDrawPort dpQuit(pdp, 0);
	CDrawPort dpWide;
	dpQuit.MakeWideScreen(&dpWide);
	// redraw the view
	if (!dpWide.Lock()) {
		return 0;
	}

	dpWide.Fill(C_BLACK|CT_OPAQUE);
	RenderStarfield(&dpWide, _fLastVolume);
	
//	FLOAT fVolume = Credits_Render(&dpWide);
//	_fLastVolume = fVolume;
	_fLastVolume	= 1.0f;

	dpWide.Unlock();
	pvp->SwapBuffers();

	return _fLastVolume;
}

// 종료시 Credit 보여주는 부분.
void QuitScreenLoop(void)
{
//	Credits_On(3);
	CSoundObject soMusic;
	try 
	{
		_toStarField.SetData_t(CTFILENAME("Textures\\Background\\Night01\\Stars01.tex"));
		//0603 kwon 삭제.
		//		soMusic.Play_t(CTFILENAME("Music\\Credits.mp3"), SOF_NONGAME|SOF_MUSIC|SOF_LOOP);
	} 
	catch (char *strError) 
	{
		CPrintF("%s\n", strError);
	}
	// while it is still running
	FOREVER 
	{
		FLOAT fVolume = RenderQuitScreen(_pdpMain, _pvpViewPortMain);
		if (fVolume<=0) 
		{
			return;
		}
		// assure we can listen to non-3d sounds
		soMusic.SetVolume(fVolume, fVolume);
		_pSound->UpdateSounds();
		// while there are any messages in the message queue
		MSG msg;
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{
			// if it is not a keyboard or mouse message
			if(msg.message==WM_LBUTTONDOWN||
				 msg.message==WM_RBUTTONDOWN||
				 msg.message==WM_KEYDOWN) 
			{
				return;
			}
		}
		//Sleep(5);
	}
}

int SubMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	(void)hPrevInstance;
	
	if (strlen(lpCmdLine) <= 0) {
		MessageBox(NULL, "This program could not be run itself", "LastChaos", MB_OK);
		return TRUE;
	}

	GetNM(lpCmdLine);
	
	ShowCursor(FALSE);
	
	if( !Init( hInstance, nCmdShow, lpCmdLine )) return FALSE;

	BOOL bInit = TRUE;
	
	// WSS_NPROTECT 070402 ------------------------------->>
#ifndef NO_GAMEGUARD
//	CPrintF("[ ---->> GameGuard Init...Start <<---- ] - %lu\n",timeGetTime());
	bInit = _pNetwork->Init_nProtect();
	_pGameState->Running() = bInit;
//	CPrintF("[ ---->> GameGuard Init...End <<---- ] - %lu\n",timeGetTime());
#endif
	// ---------------------------------------------------<<
	
	// initialy, application is running and active, console and menu are off
	//_bRunning    = TRUE;
	//_bQuitScreen = TRUE;
	if(g_szExitError == NULL && bInit)
	{
		_pGameState->Running()	= TRUE;
		_pGameState->QuitScreen() = TRUE;
	}
	_pGame->gm_csConsoleState  = CS_OFF;
	//_pGame->gm_csComputerState = CS_OFF;
	//  _pGameState->m_bMenuActive    = FALSE;
	//_pGameState->m_bMenuRendering = FALSE;
	// while it is still running
	
	//_pGame->QuickTest(LOGIN_WORLD, pdp, pvpViewPort);
//#define AUTO_LOGIN
#ifndef AUTO_LOGIN
	if(_pGameState->Running())
	{
		_pGame->LoginGame(LOGIN_WORLD);
	}
#else	
	if(_pGameState->Running())
	{
		g_slZone = 0;
		_pGame->LoginGame(START_WORLD);
	}
#endif
	
	ShowCursor(TRUE);
	
	// 이기환 수정 시작 (11. 15) : IME 상태 창 제거 
	SendMessage ( _hwndMain, WM_IME_NOTIFY, IMN_CLOSESTATUSWINDOW, NULL );
	// 이기환 수정 끝 


//안태훈 수정 시작	//(5th Closed beta)(0.2)
	FLOAT tmPrev = 0;
	FLOAT tmNow = 0;
	INDEX cntFrame = 0;
	FLOAT tmLastSndMute = 0;
//안태훈 수정 끝	//(5th Closed beta)(0.2)

	// WSS_NPROTECT 070402 ------------------------------->>
	// 윈도우 핸들 생성 이후 모든 그래픽 UI관련 초기화가 끝난 후 SetHwnd() 호출
	// 이 함수로 부터 CallBack 함수가 호출 되기 시작함...
#ifndef NO_GAMEGUARD
//	CPrintF("[ ---->> GameGuard SetHwnd...Start <<---- ] - %lu\n",timeGetTime());
	_pNetwork->SetHwnd_nProtect(_hwndMain);
//	CPrintF("[ ---->> GameGuard SetHwnd...End <<---- ] - %lu\n",timeGetTime());
#endif
	// ---------------------------------------------------<<

	//while( _bRunning && _fnmModToLoad=="")
	while( _pGameState->Running() && _fnmModToLoad=="")
	{
		// while there are any messages in the message queue
		MSG msg;
		while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE)) 
		{
			// if it is not a mouse message
			if( !(msg.message>=WM_MOUSEFIRST && msg.message<=WM_MOUSELAST) ) 
			{
				// if not system key messages
				if( !(msg.message==WM_KEYDOWN&& msg.wParam==VK_F10
					||msg.message==WM_SYSKEYDOWN) ) 
				{
					if(g_iCountry == JAPAN && msg.message ==WM_IME_COMPOSITION) ;
					else {
						// dispatch it
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
				}
				// 이기환 수정 시작 (11. 15) : 국가 설정 변경시 처리 (에러출력, 변경 막음)
				/*if ( msg.message == WM_INPUTLANGCHANGE || msg.message == WM_INPUTLANGCHANGEREQUEST )
				{
					_pUIMgr->MsgProc( &msg, &_bIMEProc ); 
				}*/
				// 이기환 수정 끝 
			}
			
			// system commands (also send by the application itself)
			if( msg.message==WM_SYSCOMMAND)
			{
				switch( msg.wParam & ~0x0F) 
				{
					// if should minimize
				case SC_MINIMIZE:
					if( _bWindowChanging) break;
					_bWindowChanging  = TRUE;
					//_bReconsiderInput = TRUE;
					// 쓰이지 않는 부분.
					/*
					// if allowed, not already paused and only in single player game mode
					if( sam_bPauseOnMinimize && !_pNetwork->IsPaused() && _gmRunningGameMode==GM_SINGLE_PLAYER) 
					{
					// pause game
					_pNetwork->TogglePause();
					}
					*/
					// if in full screen
					if( IsFullScreen( sam_bFullScreenActive) ) 
					{
						// reset display mode and minimize window
						_pGfx->ResetDisplayMode();
						ShowWindow(_hwndMain, SW_MINIMIZE);
					}
					// if not in full screen
					else 
					{
						// just minimize the window
						ShowWindow(_hwndMain, SW_MINIMIZE);
					}
					break;
					// if should restore
				case SC_RESTORE:
					if( _bWindowChanging) break;
					_bWindowChanging  = TRUE;
					//_bReconsiderInput = TRUE;
					// if in full screen
					if( IsFullScreen( sam_bFullScreenActive) ) 
					{
						ShowWindow(_hwndMain, SW_SHOWNORMAL);
						// set the display mode once again
						StartNewMode( (GfxAPIType)sam_iGfxAPI, sam_iDisplayAdapter, sam_iScreenSizeI, sam_iScreenSizeJ,
							(enum DisplayDepth)sam_iDisplayDepth, IsFullScreen( sam_bFullScreenActive));
						// if not in full screen
					} else {
						// restore window
						ShowWindow(_hwndMain, SW_SHOWNORMAL);
					}
					break;
					// if should maximize
				case SC_MAXIMIZE:
					if( _bWindowChanging) break;
					_bWindowChanging  = TRUE;
					//_bReconsiderInput = TRUE;
					// go to full screen
					StartNewMode( (GfxAPIType)sam_iGfxAPI, sam_iDisplayAdapter, sam_iScreenSizeI, sam_iScreenSizeJ,
						(enum DisplayDepth)sam_iDisplayDepth, TRUE);
					ShowWindow( _hwndMain, SW_SHOWNORMAL);
					break;
				}
			}
			
			// toggle full-screen on alt-enter
			/*if( msg.message==WM_SYSKEYDOWN && msg.wParam==VK_RETURN && !IsIconic(_hwndMain)) {
			StartNewMode( (GfxAPIType)sam_iGfxAPI, sam_iDisplayAdapter, sam_iScreenSizeI, sam_iScreenSizeJ,
			(enum DisplayDepth)sam_iDisplayDepth, !IsFullScreen( sam_bFullScreenActive));
			}*/
			
			// if application should stop
			if( msg.message==WM_QUIT || msg.message==WM_CLOSE) 
			{				
				QuitGame();
			}
			
			// if application is deactivated or minimized
			if( (msg.message==WM_ACTIVATE && (LOWORD(msg.wParam)==WA_INACTIVE || HIWORD(msg.wParam)))
				||  msg.message==WM_CANCELMODE
				||  msg.message==WM_KILLFOCUS
				|| (msg.message==WM_ACTIVATEAPP && !msg.wParam)) 
			{
				// 이기환 수정 시작 (11. 15) : IME 상태 창 제거 
				SendMessage ( _hwndMain, WM_IME_NOTIFY, IMN_CLOSESTATUSWINDOW, NULL );
				// 이기환 수정 끝 

				// if application is running and in full screen mode
				if( !_bWindowChanging && _pGameState->Running()) 
				{
					// minimize if in full screen 
					//if( IsFullScreen( sam_bFullScreenActive)) PostMessage(NULL, WM_SYSCOMMAND, SC_MINIMIZE, 0);
					// just disable input if not in full screen 
					//else _pInput->DisableInput();
				}
			}
			// if application is activated or minimized
			else if( (msg.message==WM_ACTIVATE && (LOWORD(msg.wParam)==WA_ACTIVE || LOWORD(msg.wParam)==WA_CLICKACTIVE))
				||  msg.message==WM_SETFOCUS
				|| (msg.message==WM_ACTIVATEAPP && msg.wParam)) 
			{
				if (msg.message == WM_SETFOCUS)
					msg = msg;
				// 이기환 수정 시작 (11. 15) : IME 상태 창 제거 
				SendMessage ( _hwndMain, WM_IME_NOTIFY, IMN_CLOSESTATUSWINDOW, NULL );
				// 이기환 수정 끝 
				
				// enable input back again if needed
				//_bReconsiderInput = TRUE;
			}
	
			// UI message
			_pUIMgr->MsgProc( &msg, &_bIMEProc );	// yjpark // 이기환 수정 11.12 : IME 처리 추가
	
			
			BOOL bMenuForced = 
				//(_gmRunningGameMode==GM_NONE &&		// 원본.
				(_pGameState->GetGameMode() == CGameState::GM_NONE &&
				(_pGame->gm_csConsoleState==CS_OFF || 
				_pGame->gm_csConsoleState==CS_TURNINGOFF));
			
			// interpret console key presses
			// 콘솔창에 키입력받는 부분.
			if (_iAddonExecState==0) 
			{				
				if (msg.message==WM_KEYDOWN) 
				{
					_pGame->ConsoleKeyDown(msg);
					}/* else if (msg.message==WM_KEYUP) {		// yjpark
					 // special handler for talk (not to invoke return key bind)
					 if( msg.wParam==VK_RETURN && _pGame->gm_csConsoleState==CS_TALK) _pGame->gm_csConsoleState = CS_OFF;
			}*/ else if (msg.message==WM_CHAR) 
				{
					_pGame->ConsoleChar(msg);
				}
			}
			
			// if toggling console
			// 콘솔 키를 눌렀을때~!!!
			BOOL bConsoleKey = sam_bToggleConsole || msg.message==WM_KEYDOWN && _pNetwork->m_ubGMLevel > 0 &&
				( MapVirtualKey( msg.wParam, 0 ) == 41 /* scan code for '~' */ );
			if(bConsoleKey && !_bDefiningKey)
			{
				sam_bToggleConsole = FALSE;
				if( _iAddonExecState==3) _iAddonExecState = 0;
				// if it is up, or pulling up
				if( _pGame->gm_csConsoleState==CS_OFF || _pGame->gm_csConsoleState==CS_TURNINGOFF) 
				{
					// start it moving down and disable menu
					_pGame->gm_csConsoleState = CS_TURNINGON;
					// stop all IFeel effects
					IFeel_StopEffect(NULL);
					
					// 메뉴가 활성화되어있다면, 메뉴를 비활성화함.	
					//_pGameState->m_bMenuActive = FALSE;
					//if(_pGame->gm_bGameOn)
					// 게임이 실행중일때만 호출되어야함.
					//	_pUIMgr->SetUIGameState( UGS_GAMEON );
					/*
					if( _pGameState->m_bMenuActive) 
					{
					StopMenus(FALSE);
					}
					*/
					// if it is down, or dropping down
				}
				
				// 콘솔창을 닫으려할때...
				// 즉, 콘솔창이 열린상태에서....
				else if( _pGame->gm_csConsoleState==CS_ON || _pGame->gm_csConsoleState==CS_TURNINGON) 
				{
					// start it moving up
					_pGame->gm_csConsoleState = CS_TURNINGOFF;
				}
			}
			
			/*if (_pShell->GetINDEX("con_bTalk") && _pGame->gm_csConsoleState==CS_OFF) {
			_pShell->SetINDEX("con_bTalk", FALSE);
			_pGame->gm_csConsoleState = CS_TALK;
			}*/		// yjpark
			
			/*
			// if pause pressed// 메뉴가 활성화된 상태에서 PAUSE키를 눌렀을때...(필요없음)
			if (msg.message==WM_KEYDOWN && msg.wParam==VK_PAUSE) 
			{
			// toggle pause
			_pNetwork->TogglePause();
			}
			*/
			
		} // loop while there are messages
		
		// when all messages are removed, window has surely changed
		_bWindowChanging = FALSE;
		
		// get real cursor position
		/*if( _pGame->gm_csComputerState!=CS_OFF && _pGame->gm_csComputerState!=CS_ONINBACKGROUND) {
		POINT pt;
		::GetCursorPos(&pt);
		::ScreenToClient(_hwndMain, &pt);
		_pGame->ComputerMouseMove(pt.x, pt.y);
			}*/		// yjpark
		
		// if addon is to be executed
		if (_iAddonExecState==1) {
			// print header and start console
			CPrintF(TRANS("---- Executing addon: '%s'\n"), (const char*)_fnmAddonToExec);
			sam_bToggleConsole = TRUE;
			_iAddonExecState = 2;
			// if addon is ready for execution
		} else if (_iAddonExecState==2 && _pGame->gm_csConsoleState == CS_ON) {
			// execute it
			CTString strCmd;
			strCmd.PrintF("include \"%s\"", (const char*)_fnmAddonToExec);
			_pShell->Execute(strCmd);
			CPrintF(TRANS("Addon done, press Escape to close console\n"));
			_iAddonExecState = 3;
		}
		
		// automaticaly manage input enable/disable toggling
		//UpdateInputEnabledState();
		// automaticaly manage pause toggling
		UpdatePauseState();
		// notify game whether menu is active
		//_pGame->gm_bMenuOn = _pGameState->m_bMenuActive;
		
		// do the main game loop and render screen
		DoGame();
	
//안태훈 수정 시작	//(5th Closed beta)(0.2)
		extern ENGINE_API FLOAT g_fFramePerSecond;
		extern ENGINE_API BOOL g_bNoPlaySnd;
		++cntFrame;
		tmNow = (FLOAT)_pTimer->GetHighPrecisionTimer().GetSeconds();
		static bool bForDebug = FALSE;
		if(!bForDebug)
		{
			if(g_fFramePerSecond == FLT_MAX || (tmNow - tmPrev) >= 1.0f)	//1초에 한번 처리.
			{
				g_fFramePerSecond = FLOAT( cntFrame / (tmNow - tmPrev) );
				if(g_fFramePerSecond < 6.1f)
				{
					tmLastSndMute = tmNow;
					g_bNoPlaySnd = TRUE;
				}
				else if( tmLastSndMute != 0 && (tmNow - tmLastSndMute) > 1.f )
				{
					tmLastSndMute = 0;
					g_bNoPlaySnd = FALSE;
				}
				cntFrame = 0;
				tmPrev = tmNow;

				// WSS_NPROTECT 070402 ------------------------------->>
#ifndef NO_GAMEGUARD
				_pNetwork->Check_nProtect();
#endif
				// ---------------------------------------------------<<
			}
		}
//안태훈 수정 끝	//(5th Closed beta)(0.2)
		// limit current frame rate if neeeded
		//LimitFrameRate();
		
	} // end of main application loop
	
	//_pInput->DisableInput();
	_pGame->StopGame();

	// WSS_NPROTECT 070402 ------------------------------->>
#ifndef NO_GAMEGUARD
	if(g_szHackMsg.Length()>0)
	{
		MessageBox(_hwndMain,g_szHackMsg.str_String,"nProtect GameGuard",MB_OK);
	}	
#endif
	// ---------------------------------------------------<<
	
	
	// invoke quit screen if needed
	// 게임 종료시 호출되는 부분.
	//if( _bQuitScreen && _fnmModToLoad=="") QuitScreenLoop();
	
	End();
	
	return TRUE;
}

void CheckModReload(void)
{
	if (_fnmModToLoad!="") {
#ifndef NDEBUG
		CTString strDebug = "Debug\\";
#else
		CTString strDebug = "";
#endif
		CTString strCommand = _fnmApplicationPath+"Bin\\"+strDebug+"SeriousSam.exe";
		//+mod "+_fnmModToLoad.FileName()+"\"";
		CTString strMod = _fnmModToLoad.FileName();
		const char *argv[7];
		argv[0] = strCommand;
		argv[1] = "+game";
		argv[2] = strMod;
		argv[3] = NULL;
		if (_strModServerJoin!="") {
			argv[3] = "+connect";
			argv[4] = _strModServerJoin;
			argv[5] = "+quickjoin";
			argv[6] = NULL;
		}
		_execv(strCommand, argv);
	}
}

/*
void CheckTeaser(void)
{
	CTFileName fnmTeaser = _fnmApplicationPath+CTString("Bin\\AfterSam.exe");
	if (fopen(fnmTeaser, "r")!=NULL) {
		Sleep(500);
		_execl(fnmTeaser, "\""+fnmTeaser+"\"", NULL);
	}
}

void CheckBrowser(void)
{
	if (_strURLToVisit!="") {
		RunBrowser(_strURLToVisit);
	}
}
*/
#ifdef   _DEBUG
#define  SET_CRT_DEBUG_FIELD(a) \
            _CrtSetDbgFlag((a) | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#define  CLEAR_CRT_DEBUG_FIELD(a) \
            _CrtSetDbgFlag(~(a) & _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#else
#define  SET_CRT_DEBUG_FIELD(a)   ((void) 0)
#define  CLEAR_CRT_DEBUG_FIELD(a) ((void) 0)
#endif
int PASCAL WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
			LPSTR lpCmdLine, int nCmdShow)
{
/*
	HANDLE hMutex = CreateMutex(NULL, TRUE, "LastChaosIsOnlyOne!");
	if(hMutex == NULL || GetLastError() == ERROR_ALREADY_EXISTS)
	{
		return 1;
	}
*/

	int iResult;
	CTSTREAM_BEGIN {
		iResult = SubMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	} CTSTREAM_END;
	CTStream::ClearStreamHandling();

	CheckModReload();

/*
	CloseHandle(hMutex);
*/
	if(g_szExitError != NULL)
	{
		::MessageBox(NULL, g_szExitError, "Error", MB_OK);
		delete[] g_szExitError;
	}

	SET_CRT_DEBUG_FIELD( _CRTDBG_LEAK_CHECK_DF );
	return iResult;
}


// try to start a new display mode
BOOL TryToSetDisplayMode( enum GfxAPIType eGfxAPI, INDEX iAdapter, PIX pixSizeI, PIX pixSizeJ,
													enum DisplayDepth eColorDepth, BOOL bFullScreenMode)
{
	CDisplayMode dmTmp;
	dmTmp.dm_ddDepth = eColorDepth;
	CPrintF( TRANS("  Starting display mode: %dx%dx%s (%s)\n"),
					 pixSizeI, pixSizeJ, dmTmp.DepthString(),
					 bFullScreenMode ? TRANS("fullscreen") : TRANS("window"));

	// mark to start ignoring window size/position messages until settled down
	_bWindowChanging = TRUE;
	
	// destroy canvas if existing
	_pGame->DisableLoadingHook();
	if( _pvpViewPortMain!=NULL) {
 	  _pGfx->DestroyWindowCanvas( _pvpViewPortMain);
		_pvpViewPortMain = NULL;
		_pdpNormalMain = NULL;
	}

	// close the application window
	CloseMainWindow();

	// try to set new display mode
	BOOL bSuccess;
	if( bFullScreenMode) {
		if( eGfxAPI==GAT_D3D) OpenMainWindowFullScreen( pixSizeI, pixSizeJ);
		bSuccess = _pGfx->SetDisplayMode( eGfxAPI, iAdapter, pixSizeI, pixSizeJ, eColorDepth);
		if( bSuccess && eGfxAPI==GAT_OGL) OpenMainWindowFullScreen( pixSizeI, pixSizeJ);
	} else {
		if( eGfxAPI==GAT_D3D) OpenMainWindowNormal( pixSizeI, pixSizeJ);
		bSuccess = _pGfx->ResetDisplayMode( eGfxAPI);
		if( bSuccess && eGfxAPI==GAT_OGL) OpenMainWindowNormal( pixSizeI, pixSizeJ);
		if( bSuccess && eGfxAPI==GAT_D3D) ResetMainWindowNormal();
	}

	// if new mode was set
	if( bSuccess) {
		// create canvas
		ASSERT( _pvpViewPortMain==NULL);
		ASSERT( _pdpNormalMain==NULL);
		if(_pdpNormalMain) delete _pdpNormalMain;
 	  _pGfx->CreateWindowCanvas( _hwndMain, &_pvpViewPortMain, &_pdpNormalMain);

	    // 윈도우 포커스 조정
	    SetFocus(_hwndMain);

		// erase context of both buffers (for the sake of wide-screen)
		_pdpMain = _pdpNormalMain;
		if( _pdpMain!=NULL && _pdpMain->Lock()) {
			_pdpMain->Fill(C_BLACK|CT_OPAQUE);
			_pdpMain->Unlock();
			_pvpViewPortMain->SwapBuffers();
			_pdpMain->Lock();
			_pdpMain->Fill(C_BLACK|CT_OPAQUE);
			_pdpMain->Unlock();
			_pvpViewPortMain->SwapBuffers();
		}

		// lets try some wide screen screaming :)
		const PIX pixYBegAdj = _pdpMain->GetHeight() * 21/24;
		const PIX pixYEndAdj = _pdpMain->GetHeight() * 3/24;
		const PIX pixXEnd    = _pdpMain->GetWidth();
		//if(_pdpWideScreenMain) delete _pdpWideScreenMain;
		//_pdpWideScreenMain = new CDrawPort( _pdpMain, PIXaabbox2D( PIX2D(0,pixYBegAdj), PIX2D(pixXEnd, pixYEndAdj)));
		//_pdpWideScreenMain->dp_fWideAdjustment = 9.0f / 12.0f;
		//if( sam_bWideScreen) _pdpMain = _pdpWideScreenMain;

		// initial screen fill and swap, just to get context running
		BOOL bSuccess = FALSE;
		if( _pdpMain!=NULL && _pdpMain->Lock()) 
		{
			//pdp->Fill( LCDGetColor( C_dGREEN|CT_OPAQUE, "bcg fill"));		// 원본.
			_pdpMain->Fill( C_BLACK|CT_OPAQUE);				// maybe 동민
			_pdpMain->Unlock();
			_pvpViewPortMain->SwapBuffers();
			bSuccess = TRUE;
		}
		_pGame->EnableLoadingHook(_pdpMain);

		// if the mode is not working, or is not accelerated
		if( !bSuccess || !_pGfx->IsCurrentModeAccelerated())
		{ // report error
			CPrintF( TRANS("This mode does not support hardware acceleration.\n"));
			// destroy canvas if existing
			if( _pvpViewPortMain!=NULL) {
				_pGame->DisableLoadingHook();
 				_pGfx->DestroyWindowCanvas( _pvpViewPortMain);
				_pvpViewPortMain = NULL;
				_pdpNormalMain = NULL;
			}
			// close the application window
			CloseMainWindow();
			// report failure
			return FALSE;
		}

		// Compressed texture
		if( eGfxAPI == GAT_D3D )
		{
			HRESULT	hrDXT1 = _pGfx->gl_pD3D->CheckDeviceFormat( iAdapter,
																D3DDEVTYPE_HAL,
																_pGfx->gl_d3dColorFormat,
									                            0,
																D3DRTYPE_TEXTURE,
																D3DFMT_DXT1 );
			HRESULT	hrDXT3 = _pGfx->gl_pD3D->CheckDeviceFormat( iAdapter,
																D3DDEVTYPE_HAL,
																_pGfx->gl_d3dColorFormat,
									                            0,
																D3DRTYPE_TEXTURE,
																D3DFMT_DXT3 );
			HRESULT	hrDXT5 = _pGfx->gl_pD3D->CheckDeviceFormat( iAdapter,
																D3DDEVTYPE_HAL,
																_pGfx->gl_d3dColorFormat,
									                            0,
																D3DRTYPE_TEXTURE,
																D3DFMT_DXT5 );

			_pGfx->gl_bCompressedTexture = SUCCEEDED( hrDXT1 ) && SUCCEEDED( hrDXT3 ) && SUCCEEDED( hrDXT5 );
		}

		// remember new settings
		sam_bFullScreenActive = (bFullScreenMode==FULLSCREEN_MODE)?FULLSCREEN_MODE:OLD_WINDOW_MODE;
		sam_iScreenSizeI = pixSizeI;
		sam_iScreenSizeJ = pixSizeJ;
		sam_iDisplayDepth = eColorDepth;
		sam_iDisplayAdapter = iAdapter;
		sam_iGfxAPI = eGfxAPI;

		// report success
		return TRUE;
	}

	// if couldn't set new mode
	else {
		// close the application window
		CloseMainWindow();
		// report failure
		return FALSE;
	}
}


// list of possible display modes for recovery 
const INDEX aDefaultModes[][3] =
{ // color, API, adapter
	{ DD_DEFAULT, GAT_OGL, 0},
	{ DD_16BIT,   GAT_OGL, 0},
	{ DD_16BIT,   GAT_OGL, 1}, // 3dfx Voodoo2
	{ DD_DEFAULT, GAT_D3D, 0},
	{ DD_16BIT,   GAT_D3D, 0},
	{ DD_16BIT,   GAT_D3D, 1},
};
const INDEX ctDefaultModes = ARRAYCOUNT(aDefaultModes);

// start new display mode
void StartNewMode( enum GfxAPIType eGfxAPI, INDEX iAdapter, PIX pixSizeI, PIX pixSizeJ,
									 enum DisplayDepth eColorDepth, BOOL bFullScreenMode)
{
	CPrintF( TRANS("\n* START NEW DISPLAY MODE ...\n"));

	// clamp window size to screen size if in window mode
	if( !bFullScreenMode) {
		PIX pixMaxSizeI = _pixDesktopWidth  - 1; // need 1st lower resolution than desktop
		PIX pixMaxSizeJ = _pixDesktopHeight - 1;
		if( pixSizeI>pixMaxSizeI || pixSizeJ>pixMaxSizeJ) {
			extern void FindMaxResolution( PIX &pixSizeI, PIX &pixSizeJ);
			FindMaxResolution( pixMaxSizeI, pixMaxSizeJ);
			pixSizeI = pixMaxSizeI;
			pixSizeJ = pixMaxSizeJ;
		}
	}

	// try to set the mode
	BOOL bSuccess = TryToSetDisplayMode( eGfxAPI, iAdapter, pixSizeI, pixSizeJ, eColorDepth, bFullScreenMode);

	// if failed
	if( !bSuccess)
	{
		// report failure and reset to default resolution
		_iDisplayModeChangeFlag = 2;  // failure
		CPrintF( TRANS("Requested display mode could not be set!\n"));
		pixSizeI = 640;
		pixSizeJ = 480;
		bFullScreenMode = TRUE;
		// try to revert to one of recovery modes
		for( INDEX iMode=0; iMode<ctDefaultModes; iMode++) {
			eColorDepth = (DisplayDepth)aDefaultModes[iMode][0];
			eGfxAPI     = (GfxAPIType)  aDefaultModes[iMode][1];
			iAdapter    =               aDefaultModes[iMode][2];
			CPrintF(TRANS("\nTrying recovery mode %d...\n"), iMode);
			bSuccess = TryToSetDisplayMode( eGfxAPI, iAdapter, pixSizeI, pixSizeJ, eColorDepth, bFullScreenMode);
			if( bSuccess) break;
		}
		// if all failed
		if( !bSuccess) {
			FatalError(TRANS(
				"Cannot set display mode!\n"
				"Last Chaos was unable to find display mode with hardware acceleration.\n"
				"Make sure you install proper drivers for your video card as recommended\n"
				"in documentation and set your desktop to 16 bit (65536 colors).\n"
				"Please see ReadMe file for troubleshooting information.\n"));
		}

	// if succeeded
	} else {
		_iDisplayModeChangeFlag = 1;  // all ok
	}

	// apply 3D-acc settings
	ApplyGLSettings(FALSE);

	// remember time of mode setting
	_tmDisplayModeChanged = _pTimer->GetRealTimeTick();
}

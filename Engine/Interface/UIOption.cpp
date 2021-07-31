#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <vector>
#include "UIOption.h"
#include <Engine/Engine.h>
#include <Engine/Network/Web.h>
#include <Engine/Graphics/Adapter.h>
#include <Engine/GameStageManager/StageMgr.h>

//2013/04/15 jeil 런처에 해상도 변경 추가
#include <fstream>

//안태훈 수정 시작	//(Open beta)(2004-12-30)
extern FLOAT g_fSndVol;
extern FLOAT g_fMscVol;
//안태훈 수정 끝	//(Open beta)(2004-12-30)
extern FLOAT		gfx_fBrightness;
extern FLOAT		gfx_fContrast;
extern FLOAT		gap_fTextureLODBias;
extern INDEX		g_bRenderDecoration;
extern FLOAT		ter_fLODMul;
extern FLOAT		ska_fLODMul;
extern FLOAT		mdl_fLODMul;
extern INDEX		g_iUseBloom;
extern INDEX		g_iShadowDetail;
extern INDEX		g_iReflectionDetail;
extern INDEX		g_iEnterChat;
extern INDEX		g_iAutoAttack;
extern INDEX		g_iShowName;
extern INDEX		g_iShowNameItem;

// wooss 070401 ------------------->><<
// kw : WSS_HELP_SYSTEM_1
extern INDEX		g_iShowHelp1Icon;

extern INDEX		g_iRejectExchange;
extern INDEX		g_iRejectParty;

extern INDEX		g_iQuestBookOpen;

extern BOOL			_bWindowChanging;
extern HWND			_hwndMain;
extern HWND			_hDlgWeb;
extern cWeb			g_web;
extern CDrawPort	*_pdpMain;
extern CDrawPort	*_pdpNormalMain;
extern CDrawPort	*_pdpWideScreenMain;
extern CViewPort	*_pvpViewPortMain;
extern HINSTANCE	_hInstanceMain;
extern INDEX		sam_bFullScreenActive;
extern INDEX		sam_iScreenSizeI;
extern INDEX		sam_iScreenSizeJ;
extern INDEX		sam_iDisplayDepth;
extern INDEX		sam_iDisplayAdapter;
extern INDEX		sam_iGfxAPI;
extern INDEX		sam_bWideScreen;
extern INDEX		cmd_iWindowLeft;
extern INDEX		cmd_iWindowTop;
extern INDEX		g_bSlaveNoTarget;			// UI_REFORM :Su-won
// MBCS -> WBCS전환 [12/21/2011 rumist]
static WCHAR		wchWindowTitle[256];
//static char			achWindowTitle[256];
static PIX			_pixLastSizeI, _pixLastSizeJ;
extern ENGINE_API INDEX sam_bFullScreenActive;

int	g_nCurSelResolution;
extern INDEX	g_iCountry;

#ifdef KALYDO
extern ENGINE_API HWND	g_KalydoWindowHandle;
#endif

// ----------------------------------------------------------------------------
// Name : CUIOption()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIOption::CUIOption()
	: m_pTabMain(NULL)
	, m_pSlideShowName(NULL)
	, m_pSlideShowItem(NULL)
	, m_pChkEnterChat(NULL)
	, m_pChkMovePermit(NULL)
	, m_pChkAutoAttack(NULL)
	, m_pChkHelp1Icon(NULL)
	, m_pChkPopQbook(NULL)
	, m_pChkDenyTrade(NULL)
	, m_pChkDenyParty(NULL)
	, m_pComboItemPlusEffect(NULL)
	, m_pComboSelectTarget(NULL)
	, m_pComboResolution(NULL)
	, m_pChkFullscreen(NULL)
	, m_pTxtBright(NULL)
	, m_pTxtContrast(NULL)
	, m_pSlideBright(NULL)
	, m_pSlideContrast(NULL)
	, m_pSlideTexture(NULL)
	, m_pSlideShadow(NULL)
	, m_pSlideCharacter(NULL)
	, m_pSlideTerrain(NULL)	
	, m_pSlideReflection(NULL)
	, m_pSlideFiltering(NULL)
	, m_pChkBackground(NULL)
	, m_pSlideBGM(NULL)
	, m_pSlideFXSound(NULL)
{
	setInherit(false);

	int		i;
	m_nCurOptionTab = 0;
	m_bPixelShader = FALSE;

	for( i = 0; i < 11; i++ )
	{
		m_afBrightness[i] = 0.9f * ( ( i - 5 ) * 0.05f );
		m_afContrast[i] = 1.0f + 0.9f * ( ( i - 5 ) * 0.05f );
	}
	for( i = 0; i < 3; i++ )
	{
		m_afTextureQuality[i] = 0.5f - i * 0.5f;
	}
	for( i = 0; i < 5; i++ )
	{
		m_afViewTerrain[i] = 0.7f + i * 0.075;//1.0f + 0.5f * ( ( i - 4 ) / 5.0f );
	}

	m_pixDesktopWidth = 0;
	m_pixDesktopHeight = 0;

	for (i = 0; i < OPTIONTAB_TOTAL; ++i)
		m_pImgArrayTab[i] = NULL;
}

// ----------------------------------------------------------------------------
// Name : ~CUIOption()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIOption::~CUIOption()
{
	Destroy();

	if( _pvpViewPortMain != NULL )
	{
		_pGfx->DestroyWindowCanvas( _pvpViewPortMain );
		_pvpViewPortMain = NULL;
		_pdpNormalMain = NULL;
	}
}

// ----------------------------------------------------------------------------
// Name : InitOption()
// Desc :
// ----------------------------------------------------------------------------
void CUIOption::InitOption()
{
	m_pChkFullscreen->SetEnable( TRUE );
	m_pChkFullscreen->SetCheck( IsFullScreen( sam_bFullScreenActive) );

// [2011/04/07 : Sora] DX9.0 이상 버전에서는 지원하지 않으므로 비활성화
	if (IsGamigo(g_iCountry) == TRUE || g_iCountry == RUSSIA)
	{
		m_pSlideBright->SetEnable( FALSE );
		m_pSlideContrast->SetEnable( FALSE );
	}
	else
	{
		m_pSlideBright->SetEnable( IsFullScreen( sam_bFullScreenActive ) );
		m_pSlideContrast->SetEnable( IsFullScreen( sam_bFullScreenActive ) );
	}

	if (m_pSlideBright->IsEnabled() == TRUE)
	{
		m_pTxtBright->setFontColor(DEF_UI_FONT_COLOR);
		m_pTxtContrast->setFontColor(DEF_UI_FONT_COLOR);
	}
	else
	{
		m_pTxtBright->setFontColor(DEF_UI_FONT_DISABLE);
		m_pTxtContrast->setFontColor(DEF_UI_FONT_DISABLE);
	}

	// Pixel shader
	m_bPixelShader = TRUE;
	if( !( ( _pGfx->gl_ulFlags & GLF_VERTEXPROGRAM ) && ( _pGfx->gl_ulFlags & GLF_PIXELPROGRAM ) &&
		( _pGfx->gl_ctTextureUnits >= 4 ) ) )
	{
		m_bPixelShader = FALSE;
		m_nReflectionQuality = 0;
		m_pSlideReflection->SetCurPos( 0 );
		m_pSlideReflection->SetEnable( FALSE );
		m_nFiltering = 0;
		m_pSlideFiltering->SetCurPos( 0 );
		m_pSlideFiltering->SetEnable( FALSE );
	}

	CreateResolutionList();

	m_nBrightness = -1;
	m_nContrast = -1;
	int		i;
	for( i = 0; i < 11; i++ )
	{
		if( m_afBrightness[i] == gfx_fBrightness )
		{
			m_pSlideBright->SetCurPos( i );
			m_nBrightness = i;
		}

		if( m_afContrast[i] == gfx_fContrast )
		{
		m_pSlideContrast->SetCurPos( i );
			m_nContrast = i;
		}
	}
	if( m_nBrightness == -1 )
	{
		m_nBrightness = 5;
		m_pSlideBright->SetCurPos( m_nBrightness );
	}
	if( m_nContrast == -1 )
	{
		m_nContrast = 5;
		m_pSlideContrast->SetCurPos( m_nContrast );
	}

	m_nTextureQuality = -1;
	for( i = 0; i < 3; i++ )
	{
		if( m_afTextureQuality[i] == gap_fTextureLODBias )
		{
			m_pSlideTexture->SetCurPos( i );
			m_nTextureQuality = i;
		}
	}
	if( m_nTextureQuality == -1 )
	{
		m_nTextureQuality = 0;
		m_pSlideTexture->SetCurPos( m_nTextureQuality );
	}

	m_pChkBackground->SetCheck( g_bRenderDecoration != 0 );

	m_nViewTerrain = -1;

	float fScale = CZoneInfo::getSingleton()->GetTerMul(g_slZone);
	float fTmpView = 0.f;

	for( i = 0; i < 5; i++ )
	{
		fTmpView = m_afViewTerrain[i] * fScale;

		if( fTmpView == ter_fLODMul )
		{
			m_pSlideTerrain->SetCurPos( i );
			m_nViewTerrain = i;
			break;
		}
	}
	if( m_nViewTerrain == -1 )
	{
		m_nViewTerrain = 0;
		m_pSlideTerrain->SetCurPos( m_nViewTerrain );
	}

	m_pSlideFiltering->SetCurPos( g_iUseBloom );
	m_nFiltering = g_iUseBloom;

	m_pSlideShadow->SetCurPos( g_iShadowDetail );
	m_nShadowQuality = g_iShadowDetail;

	m_pSlideReflection->SetCurPos( g_iReflectionDetail );
	m_nReflectionQuality = g_iReflectionDetail;

//안태훈 수정 시작	//(Open beta)(2004-12-30)
	m_pSlideBGM->SetCurPos( g_fMscVol * 100 );
	m_nBGMVolume = m_pSlideBGM->GetCurPos();
	m_pSlideFXSound->SetCurPos( g_fSndVol * 100 );
	m_nFXVolume = m_pSlideFXSound->GetCurPos();
//안태훈 수정 끝	//(Open beta)(2004-12-30)

	// default option change to enter chat. [9/22/2009 rumist]
	m_pChkEnterChat->SetCheck( g_iEnterChat != 0 );
	m_bEnterChat = g_iEnterChat != 0;

	m_pChkAutoAttack->SetCheck( g_iAutoAttack != 0 );
	m_bAutoAttack = g_iAutoAttack != 0;

	//TEMP:명성공개
//	m_cbtnFameOpen.SetCheck( _pNetwork->MyCharacterInfo.bFameOpen != 0 );

	m_pSlideShowName->SetCurPos( g_iShowName );
	m_nShowName = g_iShowName;

	m_pSlideShowItem->SetCurPos( g_iShowNameItem );
	m_nShowNameItem = g_iShowNameItem;

	//wooss 050809
	UBYTE tv_byte = _pNetwork->MyCharacterInfo.sbItemEffectOption;

	m_pComboItemPlusEffect->SetCurSel(tv_byte & 0x01);

	//wooss 050809
	//default true
		
	m_pChkMovePermit->SetCheck(!(tv_byte & (0x01 << 1)));
	m_pChkMovePermit->SetEnable(STAGEMGR()->GetCurStage() == eSTAGE_GAMEPLAY);

	// wooss 070401 ------------------------------------->>
	// kw : WSS_HELP_SYSTEM_1
	m_pChkHelp1Icon->SetCheck( g_iShowHelp1Icon != 0 );
	m_iShowHelp1Icon = g_iShowHelp1Icon != 0;
	// --------------------------------------------------<<

	// [7/9/2009 rumist] rejection.
	m_pChkDenyTrade->SetCheck( g_iRejectExchange != 0 );
	m_bRejectExchange = g_iRejectExchange != 0;

	// [7/9/2009 rumist] rejection.
	m_pChkDenyParty->SetCheck( g_iRejectParty != 0 );
	m_bRejectParty = g_iRejectParty != 0;

	// 1106 UI 개편 - 퀘스트북 열기 [09/05/11 trylord]
	m_pChkPopQbook->SetCheck(g_iQuestBookOpen != 0 );
	m_bQuestBookOpen	= g_iQuestBookOpen != 0;

	// UI_REFORM :Su-won
	m_pComboSelectTarget->SetCurSel(g_bSlaveNoTarget);
}

// ----------------------------------------------------------------------------
// Name : CreateResolutionList()
// Desc :
// ----------------------------------------------------------------------------
void CUIOption::CreateResolutionList()
{
	m_pComboResolution->ResetStrings();
	m_vecPixWidth.clear();
	m_vecPixHeight.clear();
	m_vecPixDepth.clear();

	CDisplayAdapter	&daDisplayAdapter = _pGfx->gl_gaAPI[_pGfx->gl_eCurrentAPI].ga_adaAdapter[_pGfx->gl_iCurrentAdapter];
	INDEX			ctDisplayMode = daDisplayAdapter.da_ctDisplayModes;
	INDEX			ctDisplayCount = 0;

	PIX	pixDepth = 0;
	if( _pGfx->gl_dmCurrentDisplayMode.dm_ddDepth == DISPD_16BIT )
		pixDepth = 16;
	else if( _pGfx->gl_dmCurrentDisplayMode.dm_ddDepth == DISPD_32BIT )
		pixDepth = 32;
	else if( _pGfx->gl_dmCurrentDisplayMode.dm_ddDepth == DISPD_DEFAULT )
	{
		if( _pGfx->gl_d3dColorFormat == D3DFMT_X8R8G8B8 ||
			_pGfx->gl_d3dColorFormat == D3DFMT_A8R8G8B8 )
			pixDepth = 32;
		else if( _pGfx->gl_d3dColorFormat == D3DFMT_R5G6B5 ||
			_pGfx->gl_d3dColorFormat == D3DFMT_X1R5G5B5 ||
			_pGfx->gl_d3dColorFormat == D3DFMT_A1R5G5B5 ||
			_pGfx->gl_d3dColorFormat == D3DFMT_X4R4G4B4 ||
			_pGfx->gl_d3dColorFormat == D3DFMT_A4R4G4B4 )
			pixDepth = 16;
	}

	for( INDEX idm = 0; idm < ctDisplayMode; idm++ )
	{
		CDisplayMode	&dmDisplayMode = daDisplayAdapter.da_admDisplayModes[idm];
		if( dmDisplayMode.dm_pixSizeI >= 800 && dmDisplayMode.dm_pixSizeJ >= 600 )
		{
			if( !m_pChkFullscreen->IsChecked() &&
				( dmDisplayMode.dm_pixSizeI > m_pixDesktopWidth || dmDisplayMode.dm_pixSizeJ > m_pixDesktopHeight ) )
				continue;

			m_vecPixWidth.push_back( dmDisplayMode.dm_pixSizeI );
			m_vecPixHeight.push_back( dmDisplayMode.dm_pixSizeJ );
			if( m_pChkFullscreen->IsChecked() )
				m_vecPixDepth.push_back( 16 );
			else
				m_vecPixDepth.push_back( pixDepth );
			ctDisplayCount++;
		}
	}

	if( ctDisplayCount == 0 )
	{
		m_vecPixWidth.push_back( 800 );
		m_vecPixHeight.push_back( 600 );
		if( m_pChkFullscreen->IsChecked() )
			m_vecPixDepth.push_back( 16 );
		else
			m_vecPixDepth.push_back( pixDepth );
		ctDisplayCount++;
	}

	if( m_pChkFullscreen->IsChecked() )
	{
		for( INDEX iRes = 0; iRes < ctDisplayCount; iRes++ )
		{
			m_vecPixWidth.push_back( m_vecPixWidth[iRes] );
			m_vecPixHeight.push_back( m_vecPixHeight[iRes] );
			m_vecPixDepth.push_back( 32 );
		}
	}

	CTString	strDisplay;
	int res_cnt = m_vecPixWidth.size();	
	m_pComboResolution->setMaxLine(res_cnt);

	for( INDEX iRes = 0; iRes < res_cnt; iRes++ )
	{
		if( m_pChkFullscreen->IsChecked() )
			strDisplay.PrintF( "%4dx%-4d %2dbit", m_vecPixWidth[iRes], m_vecPixHeight[iRes], m_vecPixDepth[iRes] );
		else
			strDisplay.PrintF( "%4dx%-4d", m_vecPixWidth[iRes], m_vecPixHeight[iRes] );

		m_pComboResolution->AddString( strDisplay );

		if( m_vecPixWidth[iRes] == sam_iScreenSizeI &&
			m_vecPixHeight[iRes] == sam_iScreenSizeJ &&
			m_vecPixDepth[iRes] == pixDepth )
		{
			m_pComboResolution->SetCurSel( iRes );
		}
	}

	m_pComboResolution->setShowLine(res_cnt);
	m_pComboResolution->updateDropRect();	
}

// ----------------------------------------------------------------------------
// Name : OpenOption()
// Desc :
// ----------------------------------------------------------------------------
void CUIOption::OpenOption()
{
	if( IsVisible() == TRUE )
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

//	m_cbtnFameOpen.SetEnable( pUIManager->GetUIGameState() == UGS_GAMEON );
	m_pComboItemPlusEffect->SetEnable( STAGEMGR()->GetCurStage() == eSTAGE_GAMEPLAY );

	// Set default option
	InitOption();

	// Store current selected resolution
	g_nCurSelResolution = m_pComboResolution->GetCurSel();

	// Set visible
	Hide(FALSE);
	pUIManager->RearrangeOrder( UI_OPTION, TRUE );
}

// toggle option UI [7/19/2010 rumist]
// ----------------------------------------------------------------------------
// Name : ToggleOption()
// Desc :
// ----------------------------------------------------------------------------
void	CUIOption::ToggleOption()
{
	if( IsVisible() )
		CancelOption();
	else
		OpenOption();
}

// ----------------------------------------------------------------------------
// Name : CloseMainWindow()
// Desc : close the main application window
// ----------------------------------------------------------------------------
static void CloseMainWindow(void)
{
	if (g_web.GetWebHandle() != NULL)
	{
		HWND hTmpDlg = g_web.GetWebHandle();
		g_web.CloseWebPage(hTmpDlg);
		DestroyWindow(hTmpDlg);
	}

	CUIManager::getSingleton()->DestroyRenderTarget();
		
	// if window exists
	//	김영환 클라이언트의 경우 메인 핸들 삭제 안함.
	if(_bClientApp == FALSE)
	{
		if( _hwndMain != NULL )
		{
			// destroy it
			DestroyWindow( _hwndMain );
			_hwndMain = NULL;
		}
	}
}


// ----------------------------------------------------------------------------
// Name : CloseMainWindow()
// Desc : open the main application window for fullscreen mode
// ----------------------------------------------------------------------------
static void OpenMainWindowFullScreen( PIX pixSizeI, PIX pixSizeJ )
{
	//	김영환 : 클라이언트 아닌 겨우에만. 생성.
	if(_bClientApp == FALSE)
	{
		ASSERT( _hwndMain == NULL );
		// create a window, invisible initially
		_hwndMain = CreateWindowEx(	WS_EX_TOPMOST | WS_EX_APPWINDOW,
									"Nksp",
									"",   // title
									WS_POPUP,
									0, 0,
									pixSizeI, pixSizeJ,  // window size
									NULL,
									NULL,
									_hInstanceMain,
									NULL );
		// didn't make it?
		if( _hwndMain == NULL ) FatalError( TRANS( "Cannot open main window!" ) );
	}
	else
	{
		//	김영환 : 윈도우 메인 핸들 최상의 팝업 윈도우 설정.
		SetWindowLong(_hwndMain,GWL_EXSTYLE,WS_EX_TOPMOST | WS_EX_APPWINDOW);
		SetWindowLong(_hwndMain,GWL_STYLE,WS_POPUP);
	}

	_hDlgWeb = CreateDialog(_hInstanceMain, MAKEINTRESOURCE(g_web.GetWebDlgID()), _hwndMain, g_web.GetWebDlgCallBack());

	if( _hDlgWeb==NULL) FatalError(TRANS("Cannot open Web Dialog!"));

	SE_UpdateWindowHandle( _hwndMain, _hDlgWeb);

	// set window title and show it
	// ITS # 6311 : 러시아 스트링 변경 요청. [12/15/2011 rumist]
	if (g_iCountry == RUSSIA)
		swprintf( wchWindowTitle, L"Загрузка" );
	else
		swprintf( wchWindowTitle, L"Loading..." );

	SetWindowTextW( _hwndMain, wchWindowTitle );
}

// ----------------------------------------------------------------------------
// Name : ResetMainWindowNormal()
// Desc : try to start a new display mode
// ----------------------------------------------------------------------------
static void ResetMainWindowNormal(void)
{
// screen change bug in fullscreen. [9/27/2011 rumist]
	if (sam_bFullScreenActive)
	{
		HDC hDC = GetDC( GetDesktopWindow() );
		int iDesktopBitsPixel = GetDeviceCaps( hDC, BITSPIXEL );
		int iDesktopWidth = GetDeviceCaps( hDC, HORZRES );
		int iDesktopHeight = GetDeviceCaps( hDC, VERTRES );
		ReleaseDC( GetDesktopWindow(), hDC );

		DEVMODE dm;
		memset( &dm, 0, sizeof( dm ) );
		dm.dmSize = sizeof( dm );
		dm.dmPelsWidth  = _pixLastSizeI;
		dm.dmPelsHeight = _pixLastSizeJ;
		dm.dmFields     = DM_PELSWIDTH | DM_PELSHEIGHT;
		ChangeDisplaySettings( &dm, CDS_FULLSCREEN );
	}

	int	pixPosX,pixPosY;
	PIX	pixWidth, pixHeight;

	if( sam_bFullScreenActive )
	{
		pixPosX = 0;
		pixPosY = 0;
		pixWidth = _pixLastSizeI;
		pixHeight = _pixLastSizeJ;
	}
	else 
	{
		// add edges and title bar to window size so client area would have size that we requested
		RECT	rWindow, rClient;
		GetClientRect( _hwndMain, &rClient );
		GetWindowRect( _hwndMain, &rWindow );
		pixWidth  = _pixLastSizeI + (rWindow.right-rWindow.left) - (rClient.right-rClient.left);
		pixHeight = _pixLastSizeJ + (rWindow.bottom-rWindow.top) - (rClient.bottom-rClient.top);
			
		if( cmd_iWindowLeft > 0 )
		{    
			pixPosX = cmd_iWindowLeft > (::GetSystemMetrics(SM_CXSCREEN)) ? (::GetSystemMetrics(SM_CXSCREEN)) : cmd_iWindowLeft;
			cmd_iWindowLeft = -1;
		}
		else
		{
			pixPosX = (::GetSystemMetrics(SM_CXSCREEN) - pixWidth ) / 2;
		}
		if( cmd_iWindowTop > 0 )
		{
			pixPosY = cmd_iWindowTop > (::GetSystemMetrics(SM_CYSCREEN)) ? (::GetSystemMetrics(SM_CYSCREEN)) : cmd_iWindowTop;
			cmd_iWindowTop = -1;
		}
		else
		{
			pixPosY = (::GetSystemMetrics(SM_CYSCREEN) - pixHeight ) / 2;
		}
	}

	// set new window size and show it
	// ITS # 6311 : 러시아 스트링 변경 요청. [12/15/2011 rumist]
	if (g_iCountry == RUSSIA)
		swprintf( wchWindowTitle, L"Загрузка" );
	else
		swprintf( wchWindowTitle, L"Loading..." );

	SetWindowTextW( _hwndMain, wchWindowTitle );
	if (sam_bFullScreenActive)
		SetWindowPos( _hwndMain, HWND_TOP, pixPosX,pixPosY, pixWidth,pixHeight, SWP_NOZORDER);
	else
		SetWindowPos(_hwndMain, HWND_NOTOPMOST, pixPosX,pixPosY, pixWidth,pixHeight, SWP_SHOWWINDOW | SWP_FRAMECHANGED);
}

// ----------------------------------------------------------------------------
// Name : OpenMainWindowNormal()
// Desc : open the main application window for windowed mode
// ----------------------------------------------------------------------------
static void OpenMainWindowNormal( PIX pixSizeI, PIX pixSizeJ )
{
	_pixLastSizeI = pixSizeI;
	_pixLastSizeJ = pixSizeJ;

	//	김영환 : 클라이어트의 경우에는 윈도우 생성 안함.
	if(_bClientApp == FALSE)
	{
		ASSERT( _hwndMain == NULL );
	
		if (sam_bFullScreenActive)
		{
			// create a window, invisible initially
			_hwndMain = CreateWindowEx(
				WS_EX_TOPMOST | WS_EX_APPWINDOW,
				"Nksp",
				"",   // title
				WS_POPUP,//WS_OVERLAPPED|WS_CAPTION|WS_MINIMIZEBOX|WS_SYSMENU,
				10,10,
				100,100,  // window size
				NULL,
				NULL,
				_hInstanceMain,
				NULL);
		}
		else
		{
			// create a window, invisible initially
			_hwndMain = CreateWindowEx(
				WS_EX_APPWINDOW,
				"Nksp",
				"",   // title
				WS_OVERLAPPED|WS_CAPTION|WS_MINIMIZEBOX|WS_SYSMENU,
				10,10,
				100,100,  // window size
				NULL,
				NULL,
				_hInstanceMain,
				NULL);
		}

		// didn't make it?
		if( _hwndMain == NULL ) FatalError( TRANS( "Cannot open main window!" ) );
	}
	else
	{
		ShowWindow(_hwndMain, SW_HIDE);
		//	김영환 : 메인 윈도우 스타일만 변경.
		if (sam_bFullScreenActive)
		{
			//	풀모드
			SetWindowLong(_hwndMain,GWL_EXSTYLE,WS_EX_TOPMOST | WS_EX_APPWINDOW);
			SetWindowLong(_hwndMain,GWL_STYLE,WS_POPUP);
		}
		else
		{
			//	창모드
			SetWindowLong(_hwndMain,GWL_EXSTYLE,WS_EX_APPWINDOW);
			SetWindowLong(_hwndMain,GWL_STYLE, (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX));
		}
	}

	_hDlgWeb = CreateDialog(_hInstanceMain, MAKEINTRESOURCE(g_web.GetWebDlgID()), _hwndMain, g_web.GetWebDlgCallBack());

	if( _hDlgWeb==NULL) FatalError(TRANS("Cannot open Web Dialog!"));


	SE_UpdateWindowHandle( _hwndMain, _hDlgWeb);

	// set window title
	// ITS # 6311 : 러시아 스트링 변경 요청. [12/15/2011 rumist]
	if (g_iCountry == RUSSIA)
		swprintf( wchWindowTitle, L"Загрузка" );
	else
		swprintf( wchWindowTitle, L"Loading..." );

	SetWindowTextW( _hwndMain, wchWindowTitle );
	//ResetMainWindowNormal();
	CUIManager::getSingleton()->DestroyRenderTarget();
}

// ----------------------------------------------------------------------------
// Name : TryToSetDisplayMode()
// Desc : try to start a new display mode
// ----------------------------------------------------------------------------
BOOL TryToSetDisplayMode( enum GfxAPIType eGfxAPI, INDEX iAdapter, PIX pixSizeI, PIX pixSizeJ,
							enum DisplayDepth eColorDepth, BOOL bFullScreenMode)
{
	CDisplayMode dmTmp;
	dmTmp.dm_ddDepth = eColorDepth;

	// mark to start ignoring window size/position messages until settled down
	_bWindowChanging = TRUE;
	// remember new settings

	CUIManager* pUIManager = CUIManager::getSingleton();

	// destroy canvas if existing
	if( _pvpViewPortMain != NULL )
	{
		_pGfx->DestroyWindowCanvas( _pvpViewPortMain );
		_pvpViewPortMain = NULL;
		_pdpNormalMain = NULL;
	}

	// close the application window
	CloseMainWindow();
	extern void FinalizeShaders(void);
	FinalizeShaders();	// Effect shader handle 삭제

	if (bFullScreenMode && !sam_bFullScreenActive)
	{
		DEVMODE dm;
		memset( &dm, 0, sizeof( dm ) );
		dm.dmSize = sizeof( dm );
		dm.dmPelsWidth  = pixSizeI;
		dm.dmPelsHeight = pixSizeJ;
		dm.dmFields     = DM_PELSWIDTH | DM_PELSHEIGHT;
		ChangeDisplaySettings( &dm, CDS_FULLSCREEN );
	}
	else if (!bFullScreenMode && sam_bFullScreenActive)
	{
		//ChangeDisplaySettings(&_pGfx->gl_devmode, CDS_RESET);
		ChangeDisplaySettings(NULL, 0);
	}

	sam_bFullScreenActive = (bFullScreenMode==FULLSCREEN_MODE)?FULLSCREEN_MODE:OLD_WINDOW_MODE;

	// try to set new display mode
	BOOL bSuccess;

	if( bFullScreenMode) 
	{
		if( eGfxAPI==GAT_D3D) OpenMainWindowFullScreen( pixSizeI, pixSizeJ);
		bSuccess = _pGfx->SetDisplayMode( eGfxAPI, iAdapter, pixSizeI, pixSizeJ, eColorDepth);
		if( bSuccess && eGfxAPI==GAT_OGL) OpenMainWindowFullScreen( pixSizeI, pixSizeJ);
	} 
	else 
	{
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
		_pGfx->CreateWindowCanvas( _hwndMain, &_pvpViewPortMain, &_pdpNormalMain);

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

		// Win7 듀얼 모니터 환경에서 
		// 전체화면 변경 시 서브 모니터에서 전체 화면이 풀린다.
		SetFocus(_hwndMain);

		// if the mode is not working, or is not accelerated
		if( !_pGfx->IsCurrentModeAccelerated() )
		{ // report error
#ifdef _DEBUG
			CPrintF( TRANS("This mode does not support hardware acceleration.\n"));
#endif
			// destroy canvas if existing
			if( _pvpViewPortMain!=NULL) {
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

		sam_iScreenSizeI = pixSizeI;
		sam_iScreenSizeJ = pixSizeJ;
		sam_iDisplayDepth = eColorDepth;
		sam_iDisplayAdapter = iAdapter;
		sam_iGfxAPI = eGfxAPI;

		// Date : 2005-09-22(오후 8:26:30), By Lee Ki-hwan
		// 타이틀 바 재 설졍 
		pUIManager->SetTitleName( sam_bFullScreenActive, sam_iScreenSizeI, sam_iScreenSizeJ );

		pUIManager->DestroyRenderTarget();
		pUIManager->InitRenderTarget();

		extern void InitializeShaders(void);
		InitializeShaders(); // Effect shader handle 설정			
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

#ifdef KALYDO
const BOOL CUIOption::ToggleFullscreenMode(const BOOL _bEnableFullScreenMode /* = FALSE  */)
{
	// already apply this option.
	if( _bEnableFullScreenMode == m_bFullscreen )
	{
		return FALSE;
	}	

	if (!m_bFullscreen)
	{
		m_pChkFullscreen->SetCheck(TRUE);
		m_bFullscreen = TRUE;
	}
	else
	{
		// We need to remember the fullscreen state
		m_pChkFullscreen->SetCheck(FALSE);
		m_bFullscreen = FALSE;
	}

	return TRUE;
}
#endif

// ----------------------------------------------------------------------------
// Name : ApplyOption()
// Desc :
// ----------------------------------------------------------------------------
void CUIOption::ApplyOption()
{
	// Save option
	m_bEnterChat = m_pChkEnterChat->IsChecked();
	m_bAutoAttack = m_pChkAutoAttack->IsChecked();
	m_nShowName = m_pSlideShowName->GetCurPos();
	m_nShowNameItem = m_pSlideShowItem->GetCurPos();

	m_iShowHelp1Icon = (INDEX)m_pChkHelp1Icon->IsChecked();    // wooss 070401 WSS_HELP_SYSTEM_1

	m_bRejectExchange = m_pChkDenyTrade->IsChecked();		// [7/9/2009 rumist] rejection 
	m_bRejectParty = m_pChkDenyParty->IsChecked();	

	m_bQuestBookOpen	= m_pChkPopQbook->IsChecked();

	m_nBrightness = m_pSlideBright->GetCurPos();
	m_nContrast = m_pSlideContrast->GetCurPos();

	m_nTextureQuality = m_pSlideTexture->GetCurPos();
	m_nShadowQuality = m_pSlideShadow->GetCurPos();
	m_nCharacterQuality = m_pSlideCharacter->GetCurPos();
	m_nViewTerrain = m_pSlideTerrain->GetCurPos();
	m_nReflectionQuality = m_pSlideReflection->GetCurPos();
	m_nFiltering = m_pSlideFiltering->GetCurPos();
	m_bBackground = m_pChkBackground->IsChecked();

	m_nBGMVolume = m_pSlideBGM->GetCurPos();
	m_nFXVolume = m_pSlideFXSound->GetCurPos();

	// If resolution is changed or window state is toggled
	PIX	pixDepth = 0;
	if( _pGfx->gl_dmCurrentDisplayMode.dm_ddDepth == DISPD_16BIT )
		pixDepth = 16;
	else if( _pGfx->gl_dmCurrentDisplayMode.dm_ddDepth == DISPD_32BIT )
		pixDepth = 32;
	else if( _pGfx->gl_dmCurrentDisplayMode.dm_ddDepth == DISPD_DEFAULT )
	{
		if( _pGfx->gl_d3dColorFormat == D3DFMT_X8R8G8B8 ||
			_pGfx->gl_d3dColorFormat == D3DFMT_A8R8G8B8 )
			pixDepth = 32;
		else if( _pGfx->gl_d3dColorFormat == D3DFMT_R5G6B5 ||
			_pGfx->gl_d3dColorFormat == D3DFMT_X1R5G5B5 ||
			_pGfx->gl_d3dColorFormat == D3DFMT_A1R5G5B5 ||
			_pGfx->gl_d3dColorFormat == D3DFMT_X4R4G4B4 ||
			_pGfx->gl_d3dColorFormat == D3DFMT_A4R4G4B4 )
			pixDepth = 16;
	}
#ifdef KALYDO
	// 만약 스크린이 변경되었으면 아래 루틴은 우회한다.
	if( ToggleFullscreenMode() )
		return;
#endif
	int	iRes = m_pComboResolution->GetCurSel();
	if( iRes >= 0 )
	{
		if( g_nCurSelResolution != iRes ||
			m_vecPixWidth[iRes] != sam_iScreenSizeI ||
			m_vecPixHeight[iRes] != sam_iScreenSizeJ ||
			( IsFullScreen( sam_bFullScreenActive ) && m_vecPixDepth[iRes] != pixDepth ) ||
			m_pChkFullscreen->IsChecked() != IsFullScreen( sam_bFullScreenActive ) )
		{
			DisplayDepth	eDD = DISPD_DEFAULT;
			if( m_pChkFullscreen->IsChecked() )
			{
				if( m_vecPixDepth[iRes] == 0 )
					eDD = DISPD_DEFAULT;
				else if( m_vecPixDepth[iRes] == 16 )
					eDD = DISPD_16BIT;
				else if( m_vecPixDepth[iRes] == 32 )
					eDD = DISPD_32BIT;
			}

			TryToSetDisplayMode( (GfxAPIType)sam_iGfxAPI, sam_iDisplayAdapter,
									m_vecPixWidth[iRes], m_vecPixHeight[iRes], eDD, m_pChkFullscreen->IsChecked() );
			CreateResolutionList();

			// 해상도가 변경되었다면, UIManager 크기를 갱신해야 한다.
			SE_Get_UIManagerPtr()->InitPos(0, 0, m_vecPixWidth[iRes], m_vecPixHeight[iRes]);
			// Adjust position of UIs
			SE_Get_UIManagerPtr()->AdjustUIPos( _pdpMain );
		}
	}

	// fame opn option
//	BOOL bFameOpen = m_cbtnFameOpen.IsChecked();
	//TEMP:명성공개
/*
	if(_pNetwork->MyCharacterInfo.bFameOpen != bFameOpen)
	{
		_pNetwork->FameOpenReq( bFameOpen );
	}
*/

	// Item plus effect
//	SBYTE	sbItemEffectOption = m_pComboItemPlusEffect->GetCurSel();
//	if( sbItemEffectOption >= 0 &&
//		_pNetwork->MyCharacterInfo.sbItemEffectOption != sbItemEffectOption )
//	{
//		_pNetwork->ItemPlusEffectReq( sbItemEffectOption );
//	}

	// wooss 050809
	// move, summon permission check + // Item plus effect
	// 기존 sbItemEffectOption을 비트 단위로 처리
	// 0x00		Effect	x	Warp	o
	// 0x01				o			o
	// 0x10				x			x
	// 0x11				o			x

	SBYTE	bItemEffectOption = m_pComboItemPlusEffect->GetCurSel();
	BOOL	bMove = m_pChkMovePermit->IsChecked();
	SBYTE	tv_byte=0x0;
	if(bItemEffectOption) tv_byte = 0x01;  
	if(!bMove) tv_byte |= (0x01 << 1); 	
	
	_pNetwork->ItemPlusEffectReq( tv_byte );
		
	// UI_REFORM :Su-won
	g_bSlaveNoTarget =m_pComboSelectTarget->GetCurSel();

	// Set invisible
	CUIManager::getSingleton()->RearrangeOrder( UI_OPTION, FALSE );
}

void CUIOption::ResetDisplay(void)
{
	TryToSetDisplayMode( (GfxAPIType)sam_iGfxAPI, sam_iDisplayAdapter,
							sam_iScreenSizeI, sam_iScreenSizeJ, (enum DisplayDepth)sam_iDisplayDepth, IsFullScreen( sam_bFullScreenActive) );
}

// ----------------------------------------------------------------------------
// Name : CancelOption()
// Desc :
// ----------------------------------------------------------------------------
void CUIOption::CancelOption()
{
	g_iEnterChat = m_bEnterChat;
	g_iAutoAttack = m_bAutoAttack ? 1 : 0;
	g_iShowName = m_nShowName;
	g_iShowNameItem = m_nShowNameItem;

	g_iShowHelp1Icon = m_iShowHelp1Icon ? 1 : 0;			// wooss 070401 WSS_HELP_SYSTEM_1

	g_iRejectExchange = m_bRejectExchange ? 1 : 0;					// [7/9/2009 rumist] rejection 
	g_iRejectParty	= m_bRejectParty ? 1 : 0;

	g_iQuestBookOpen	= m_bQuestBookOpen ? 1 : 0;

	gfx_fBrightness = m_afBrightness[m_nBrightness];
	gfx_fContrast = m_afContrast[m_nContrast];

	gap_fTextureLODBias = m_afTextureQuality[m_nTextureQuality];
	g_iShadowDetail = m_nShadowQuality;

	float fScale = CZoneInfo::getSingleton()->GetTerMul(g_slZone);

	ter_fLODMul = ska_fLODMul = mdl_fLODMul = (m_afViewTerrain[m_nViewTerrain] * fScale);
	g_iReflectionDetail = m_nReflectionQuality;
	g_iUseBloom = m_nFiltering;
	g_bRenderDecoration = m_bBackground ? 1 : 0;

//안태훈 수정 시작	//(5th Closed beta)(0.2)
	g_fMscVol = m_nBGMVolume / 100.0f;
	g_fSndVol = m_nFXVolume / 100.0f;
//안태훈 수정 끝	//(5th Closed beta)(0.2)

	// Set invisible
	Hide(TRUE);
	CUIManager::getSingleton()->RearrangeOrder( UI_OPTION, FALSE );

	if (m_pComboItemPlusEffect != NULL)
		 CUIFocus::getSingleton()->killFocus(m_pComboItemPlusEffect);

	if (m_pComboSelectTarget != NULL)
		CUIFocus::getSingleton()->killFocus(m_pComboSelectTarget);

	if (m_pComboResolution != NULL)
		CUIFocus::getSingleton()->killFocus(m_pComboResolution);
}

// ----------------------------------------------------------------------------
// Name : InitInterface()
// Desc :
// ----------------------------------------------------------------------------
void CUIOption::InitInterface()
{
	CUIManager::getSingleton()->ResetUIPos( _pdpMain );
	CUIManager::getSingleton()->ResetUISavePos( _pdpMain );
}

void CUIOption::ChangeWindowMode ()
{	
	if ( IsFullScreen( sam_bFullScreenActive) )
	{
		m_pChkFullscreen->SetCheck(FALSE);	
		TryToSetDisplayMode( (GfxAPIType)sam_iGfxAPI, sam_iDisplayAdapter,
									sam_iScreenSizeI,sam_iScreenSizeJ, DISPD_DEFAULT, m_pChkFullscreen->IsChecked() );	
	}	
}

void CUIOption::UpdateTerMul()
{
	float fScale = CZoneInfo::getSingleton()->GetTerMul(g_slZone);
	ter_fLODMul = ska_fLODMul = mdl_fLODMul = (m_afViewTerrain[m_pSlideTerrain->GetCurPos()] * fScale);
}

void CUIOption::initialize()
{	
	CUIText* pTxt = (CUIText*)findUI("txt_title");

	if (pTxt != NULL)
	{
		int l, t, r, b;
		l = pTxt->GetPosX();
		t = pTxt->GetPosY();
		r = l + pTxt->GetWidth();
		b = t + pTxt->GetHeight();
		setTitleRect(l, t, r, b);
	}

	pTxt = (CUIText*)findUI("txt_char_qual");

	if (pTxt != NULL)
		pTxt->setFontColor(DEF_UI_FONT_DISABLE);

	m_pTabMain = (CUITab*)findUI("tab_kind");

	m_pImgArrayTab[0] = (CUIImageArray*)findUI("tab_img_game");
	m_pImgArrayTab[1] = (CUIImageArray*)findUI("tab_img_display");
	m_pImgArrayTab[2] = (CUIImageArray*)findUI("tab_img_graphic");
	m_pImgArrayTab[3] = (CUIImageArray*)findUI("tab_img_audio");

	CUIButton* pBtn;

	m_pSlideShowName =	(CUISlideBar*)findUI("slide_name");
	m_pSlideShowItem =	(CUISlideBar*)findUI("slide_item");
	m_pChkEnterChat =	(CUICheckButton*)findUI("chk_enter_chat");
	m_pChkMovePermit =	(CUICheckButton*)findUI("chk_move_permit");
	m_pChkAutoAttack =	(CUICheckButton*)findUI("chk_cont_att");
	m_pChkHelp1Icon =	(CUICheckButton*)findUI("chk_help1icon");
	m_pChkPopQbook =	(CUICheckButton*)findUI("chk_pop_qbook");
	m_pChkDenyTrade =	(CUICheckButton*)findUI("chk_deny_trade");
	m_pChkDenyParty =	(CUICheckButton*)findUI("chk_deny_party");
	m_pComboItemPlusEffect =	(CUIComboBox*)findUI("combo_eff_item");

	if (m_pComboItemPlusEffect != NULL)
	{
		m_pComboItemPlusEffect->AddString(_S(1031, "내 효과 비공개"));
		m_pComboItemPlusEffect->AddString(_S(1032, "내 효과 공개"));
	}

	m_pComboSelectTarget =	(CUIComboBox*)findUI("combo_select_target");

	if (m_pComboSelectTarget != NULL)
	{
		m_pComboSelectTarget->AddString(_S(4163, "모두 허가"));
		m_pComboSelectTarget->AddString(_S(4164, "소환수&펫 류 제외"));
		m_pComboSelectTarget->AddString(_S(4386, "캐릭터,소환수&펫 제외"));
	}

	m_pComboResolution =	(CUIComboBox*)findUI("combo_res");
	m_pChkFullscreen =	(CUICheckButton*)findUI("chk_fullscreen");
	m_pTxtBright = (CUIText*)findUI("txt_bright");
	m_pTxtContrast = (CUIText*)findUI("txt_contrast");
	m_pSlideBright =	(CUISlideBar*)findUI("slide_bright");
	m_pSlideContrast =	(CUISlideBar*)findUI("slide_contrast");

	m_pSlideTexture =	(CUISlideBar*)findUI("slide_texture");
	m_pSlideShadow =	(CUISlideBar*)findUI("slide_shadow");
	m_pSlideCharacter =	(CUISlideBar*)findUI("slide_character");

	if (m_pSlideCharacter != NULL)
		m_pSlideCharacter->SetEnable(FALSE);

	m_pSlideTerrain =	(CUISlideBar*)findUI("slide_terrain");	
	m_pSlideReflection =	(CUISlideBar*)findUI("slide_reflect");
	m_pSlideFiltering =	(CUISlideBar*)findUI("slide_filtering");
	m_pChkBackground =	(CUICheckButton*)findUI("chk_eff_bg");

	m_pSlideBGM =	(CUISlideBar*)findUI("slide_bgm");
	m_pSlideFXSound =	(CUISlideBar*)findUI("slide_sound_fx");	

#ifndef		WORLD_EDITOR	

	if (m_pTabMain != NULL)
		m_pTabMain->SetCommandFChange(boost::bind(&CUIOption::_change_tab, this));

	if (m_pSlideShowName != NULL)
		m_pSlideShowName->SetCommandFChange(boost::bind(&CUIOption::_change_show_name, this));

	if (m_pSlideShowItem != NULL)
		m_pSlideShowItem->SetCommandFChange(boost::bind(&CUIOption::_change_show_item, this));

	if (m_pChkEnterChat != NULL)
		m_pChkEnterChat->SetCommandF(boost::bind(&CUIOption::_change_enter_chat, this));

	if (m_pChkAutoAttack != NULL)
		m_pChkAutoAttack->SetCommandF(boost::bind(&CUIOption::_change_auto_attack, this));

	if (m_pChkHelp1Icon != NULL)
		m_pChkHelp1Icon->SetCommandF(boost::bind(&CUIOption::_change_help_icon, this));

	if (m_pChkPopQbook != NULL)
		m_pChkPopQbook->SetCommandF(boost::bind(&CUIOption::_change_pop_qbook, this));

	if (m_pChkDenyTrade != NULL)
		m_pChkDenyTrade->SetCommandF(boost::bind(&CUIOption::_change_deny_trade, this));

	if (m_pChkDenyParty != NULL)
		m_pChkDenyParty->SetCommandF(boost::bind(&CUIOption::_change_deny_party, this));

	//------------------------------------------------------------------------

	if (m_pChkFullscreen != NULL)
		m_pChkFullscreen->SetCommandF(boost::bind(&CUIOption::_change_fullscreen, this));

	if (m_pSlideBright != NULL)
		m_pSlideBright->SetCommandFChange(boost::bind(&CUIOption::_change_bright, this));

	if (m_pSlideContrast != NULL)
		m_pSlideContrast->SetCommandFChange(boost::bind(&CUIOption::_change_contrast, this));

	//------------------------------------------------------------------------

	if (m_pSlideTexture != NULL)
		m_pSlideTexture->SetCommandFChange(boost::bind(&CUIOption::_change_texture, this));
	
	if (m_pSlideShadow != NULL)
		m_pSlideShadow->SetCommandFChange(boost::bind(&CUIOption::_change_shadow, this));
	
	if (m_pSlideCharacter != NULL)
		m_pSlideCharacter->SetCommandFChange(boost::bind(&CUIOption::_change_character, this));
	
	if (m_pSlideTerrain != NULL)
		m_pSlideTerrain->SetCommandFChange(boost::bind(&CUIOption::_change_terrain, this));
	
	if (m_pSlideReflection != NULL)
		m_pSlideReflection->SetCommandFChange(boost::bind(&CUIOption::_change_reflection, this));
	
	if (m_pSlideFiltering != NULL)
		m_pSlideFiltering->SetCommandFChange(boost::bind(&CUIOption::_change_filtering, this));

	if (m_pChkBackground != NULL)
		m_pChkBackground->SetCommandF(boost::bind(&CUIOption::_change_eff_bg, this));

	//------------------------------------------------------------------------

	if (m_pSlideBGM != NULL)
		m_pSlideBGM->SetCommandFChange(boost::bind(&CUIOption::_change_bgm, this));

	if (m_pSlideFXSound != NULL)
		m_pSlideFXSound->SetCommandFChange(boost::bind(&CUIOption::_change_fxsound, this));

	pBtn = (CUIButton*)findUI("btn_close");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&CUIOption::CancelOption, this));

	pBtn = (CUIButton*)findUI("btn_ok");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&CUIOption::ApplyOption, this));

	pBtn = (CUIButton*)findUI("btn_cancel");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&CUIOption::CancelOption, this));

	pBtn = (CUIButton*)findUI("btn_init");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&CUIOption::InitInterface, this));

#endif		// WORLD_EDITOR

	// tab image init
	_change_tab();
}

// ------------------------------------------------------------------

void CUIOption::_change_tab()
{
	if (m_pTabMain == NULL)
		return;

	int		i;
	int nCursel = m_pTabMain->getCurSelTab();

	for (i = 0; i < OPTIONTAB_TOTAL; ++i)
	{
		if (nCursel == i)
			m_pImgArrayTab[i]->SetRenderIdx(0);
		else
			m_pImgArrayTab[i]->SetRenderIdx(1);
	}
}

void CUIOption::_change_show_name()
{
	if (m_pSlideShowName != NULL)
		g_iShowName = m_pSlideShowName->GetCurPos();
}

void CUIOption::_change_show_item()
{
	if (m_pSlideShowItem != NULL)
		g_iShowNameItem = m_pSlideShowItem->GetCurPos();
}

void CUIOption::_change_enter_chat()
{
	if (m_pChkEnterChat != NULL)
		g_iEnterChat = m_pChkEnterChat->IsChecked() ? 1 : 0;
}

void CUIOption::_change_auto_attack()
{
	if (m_pChkAutoAttack != NULL)
		g_iAutoAttack = m_pChkAutoAttack->IsChecked() ? 1 : 0;
}

void CUIOption::_change_help_icon()
{
	if (m_pChkHelp1Icon != NULL)
		g_iShowHelp1Icon = m_pChkHelp1Icon->IsChecked() ? 1 : 0;
}

void CUIOption::_change_pop_qbook()
{
	if (m_pChkPopQbook != NULL)
		g_iQuestBookOpen = m_pChkPopQbook->IsChecked() ? 1 : 0;
}

void CUIOption::_change_deny_trade()
{
	if (m_pChkDenyTrade != NULL)
		g_iRejectExchange = m_pChkDenyTrade->IsChecked() ? 1 : 0;
}

void CUIOption::_change_deny_party()
{
	if (m_pChkDenyParty != NULL)
		g_iRejectParty = m_pChkDenyParty->IsChecked() ? 1 : 0;
}

void CUIOption::_change_fullscreen()
{
	CreateResolutionList();	
}

void CUIOption::_change_bright()
{
	if (m_pSlideBright != NULL)
		gfx_fBrightness = m_afBrightness[m_pSlideBright->GetCurPos()];	
}

void CUIOption::_change_contrast()
{
	if (m_pSlideContrast != NULL)
		gfx_fContrast = m_afContrast[m_pSlideContrast->GetCurPos()];
}

void CUIOption::_change_texture()
{
	if (m_pSlideTexture != NULL)
		gap_fTextureLODBias = m_afTextureQuality[m_pSlideTexture->GetCurPos()];
}

void CUIOption::_change_shadow()
{
	if (m_pSlideShadow != NULL)
		g_iShadowDetail = m_pSlideShadow->GetCurPos();
}

void CUIOption::_change_character()
{
	//g_iCharacterDetail = m_pSlideCharacter->GetCurPos();
}

void CUIOption::_change_terrain()
{
	UpdateTerMul();
}

void CUIOption::_change_reflection()
{
	if (m_pSlideReflection != NULL)
		g_iReflectionDetail = m_pSlideReflection->GetCurPos();
}

void CUIOption::_change_filtering()
{
	if (m_pSlideFiltering != NULL)
		g_iUseBloom = m_pSlideFiltering->GetCurPos();
}

void CUIOption::_change_eff_bg()
{
	if (m_pChkBackground != NULL)
		g_bRenderDecoration = m_pChkBackground->IsChecked() ? 1 : 0;
}

void CUIOption::_change_bgm()
{
	if (m_pSlideBGM != NULL)
		g_fMscVol = m_pSlideBGM->GetCurPos() / 100.0f;
}

void CUIOption::_change_fxsound()
{
	if (m_pSlideFXSound != NULL)
		g_fSndVol = m_pSlideFXSound->GetCurPos() / 100.0f;
}

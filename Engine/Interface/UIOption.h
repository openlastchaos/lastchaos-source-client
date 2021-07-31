// ----------------------------------------------------------------------------
//  File : UIOption.h
//  Desc : Created by dongmin
// ----------------------------------------------------------------------------

#ifndef	UIOption_H_
#define	UIOption_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


class CUIButton;
class CUIComboBox;
class CUICheckButton;
class CUISlideBar;
class CUITab;
class CUIImageArray;


// Define tabs
enum OptionTab
{
	OPTIONTAB_GAME =  0,
	OPTIONTAB_DISPLAY,
	OPTIONTAB_GRAPHIC,
	OPTIONTAB_AUDIO,
	OPTIONTAB_TOTAL,
};

// Define distance of showing name
#define	OPTION_NAME_DISTANCE		15.0f

// ----------------------------------------------------------------------------
// Name : CUIOption
// Desc :
// ----------------------------------------------------------------------------
class ENGINE_API CUIOption : public CUIWindow
{
protected:
	int					m_nCurOptionTab;					// Current tab of option ( game, video, audio )

	// Selected option information
	BOOL				m_bPixelShader;
	BOOL				m_bEnterChat;
	BOOL				m_bAutoAttack;
	INDEX				m_iShowHelp1Icon;					// wooss 070401 kw : WSS_HELP_SYSTEM_1
	BOOL				m_bFameOpen;
	BOOL				m_bRejectExchange;						// [7/9/2009 rumist] rejection options. 
	BOOL				m_bRejectParty;
	BOOL				m_bQuestBookOpen;
	int					m_nShowName;
	int					m_nShowNameItem;
	int					m_nBrightness;
	int					m_nContrast;
	int					m_nTextureQuality;
	int					m_nShadowQuality;
	int					m_nCharacterQuality;
	int					m_nViewTerrain;
	int					m_nViewCharacter;
	int					m_nReflectionQuality;
	int					m_nFiltering;
	BOOL				m_bBackground;
	int					m_nBGMVolume;
	int					m_nFXVolume;

	// Preset option data
	FLOAT				m_afBrightness[11];
	FLOAT				m_afContrast[11];
	FLOAT				m_afTextureQuality[3];
	FLOAT				m_afViewTerrain[5];
	FLOAT				m_afViewCharacter[5];

	// Display information
	PIX					m_pixDesktopWidth;
	PIX					m_pixDesktopHeight;
	std::vector<PIX>	m_vecPixWidth;
	std::vector<PIX>	m_vecPixHeight;
	std::vector<PIX>	m_vecPixDepth;

protected:
	// Internal functions
	void	CreateResolutionList();
	void	ShowToolTip( BOOL bShow, int nToolTipID = -1 );

public:
	CUIOption();
	~CUIOption();

	void	initialize();

	// Open option
	void	OpenOption();
	// toggle support. [7/19/2010 rumist]
	// Toggle option
	void	ToggleOption();

	// Init option
	void	InitOption();

	// Init interface
	void	InitInterface();

	// Apply & cancel option
	void	ApplyOption();
	void	CancelOption();

	// Set desktop size
	void	SetDesktopSize( PIX pixWidth, PIX pixHeigth )
	{
		m_pixDesktopWidth = pixWidth;
		m_pixDesktopHeight = pixHeigth;
	}
	
	void	ResetDisplay(void);

	// Get texture quality
	FLOAT	GetTextureQuality( int nStep ) const { return m_afTextureQuality[nStep]; }

	//윈도우 모드로 강제 변경 added by sam 11/01/17
	void ChangeWindowMode ();

	void UpdateTerMul();
#ifdef KALYDO
	const BOOL	ToggleFullscreenMode(const BOOL _bEnableFullScreenMode = FALSE );
	BOOL	m_bFullscreen;
#endif

private:
	void _change_tab();

	void _change_show_name();
	void _change_show_item();
	void _change_enter_chat();
	void _change_auto_attack();
	void _change_help_icon();
	void _change_pop_qbook();
	void _change_deny_trade();
	void _change_deny_party();
	
	void _change_fullscreen();
	void _change_bright();
	void _change_contrast();

	void _change_texture();
	void _change_shadow();
	void _change_character();
	void _change_terrain();
	void _change_reflection();
	void _change_filtering();
	void _change_eff_bg();

	void _change_bgm();
	void _change_fxsound();

	CUITab*				m_pTabMain;
	CUIImageArray*		m_pImgArrayTab[OPTIONTAB_TOTAL];

	// GAME
	CUISlideBar*		m_pSlideShowName;
	CUISlideBar*		m_pSlideShowItem;
	CUICheckButton*		m_pChkEnterChat;
	CUICheckButton*		m_pChkMovePermit;
	CUICheckButton*		m_pChkAutoAttack;
	CUICheckButton*		m_pChkHelp1Icon;
	CUICheckButton*		m_pChkPopQbook;
	CUICheckButton*		m_pChkDenyTrade;
	CUICheckButton*		m_pChkDenyParty;
	CUIComboBox*		m_pComboItemPlusEffect;
	CUIComboBox*		m_pComboSelectTarget;

	// DISPLAY
	CUIComboBox*		m_pComboResolution;
	CUICheckButton*		m_pChkFullscreen;
	CUIText*			m_pTxtBright;
	CUIText*			m_pTxtContrast;
	CUISlideBar*		m_pSlideBright;
	CUISlideBar*		m_pSlideContrast;

	// GRAPHIC
	CUISlideBar*		m_pSlideTexture;
	CUISlideBar*		m_pSlideShadow;
	CUISlideBar*		m_pSlideCharacter;
	CUISlideBar*		m_pSlideTerrain;	
	CUISlideBar*		m_pSlideReflection;
	CUISlideBar*		m_pSlideFiltering;
	CUICheckButton*		m_pChkBackground;
	
	// AUDIO
	CUISlideBar*		m_pSlideBGM;
	CUISlideBar*		m_pSlideFXSound;
};

#endif // UISELCHAR_H_



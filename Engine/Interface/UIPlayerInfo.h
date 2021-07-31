// ----------------------------------------------------------------------------
//  File : UIPlayerInfo.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UIPLAYERINFO_H_
#define	UIPLAYERINFO_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


#include <Engine/Interface/UIButton.h>
#include <Engine/GlobalDefinition.h>

// Define position
/****************************************
#define	PLAYERINFO_LEVEL_CX			22
#define	PLAYERINFO_LEVEL_SY			5
#define	PLAYERINFO_NAME_SX			33
#define	PLAYERINFO_NAME_SY			5

#define	PLAYERINFO_HP_SX			28
#define	PLAYERINFO_HP_CX			91
#define	PLAYERINFO_HP_SY			53
#define	PLAYERINFO_MP_SY			67
#define	PLAYERINFO_EXP_SY			81


// Define size of player information
#define	PLAYERINFO_WIDTH			140
#define	PLAYERINFO_HEIGHT			98
#define	PLAYERINFO_BAR_WIDTH		120
******************************************/

///////////////////////////////////////////////////////
// UI_REFORM :Su-won
#define	PLAYERINFO_LEVEL_CX			83
#define	PLAYERINFO_LEVEL_SY			71
#define	PLAYERINFO_NAME_SX			120
#define	PLAYERINFO_NAME_SY			5

#define	PLAYERINFO_HP_SX			114
#define	PLAYERINFO_HP_CX			180
#define	PLAYERINFO_HP_SY			29
#define	PLAYERINFO_MP_SY			42
#define	PLAYERINFO_EXP_SY			55


// Define size of player information
#define	PLAYERINFO_WIDTH			275
#define	PLAYERINFO_HEIGHT			90
#define	PLAYERINFO_BAR_WIDTH		120
// UI_REFORM :Su-won
///////////////////////////////////////////////////////



// ----------------------------------------------------------------------------
// Name : CUIPlayerInfo
// Desc :
// ----------------------------------------------------------------------------
class CUIPlayerInfo : public CUIWindow
{
protected:
	// Controls
	CUIButton			m_btnBoard;					// Button of web board
	CUIButton			m_btnMessanger;				// Button of messanger
	CUIButton			m_btnSysMenu;				// Button of system menu
	CUIButton			m_btnInven;					// Button of inventory
	CUIButton			m_btnCharInfo;				// Button of character info

	// UI_REFORM :Su-won
	CUIButton			m_btnQuickMenu[9];			//퀵메뉴 버튼


	BOOL				m_bShowToolTip;				// If tool tip is shown or not
	CTString			m_strToolTip;				// String of tool tip

	// Information
	CTString			m_strLevel;					// Level
	CTString			m_strHP;					// HP
	CTString			m_strMP;					// MP
	CTString			m_strEXP;					// Exp
	FLOAT				m_fHPRatio;					// Ratio of HP gauge
	FLOAT				m_fMPRatio;					// Ratio of MP gauge
	FLOAT				m_fEXPRatio;				// Ratio of EXP gauge

	// Region of each part
	UIRect				m_rcHP;						// Region of HP
	UIRect				m_rcMP;						// Region of MP
	UIRect				m_rcEXP;					// Region of Exp
	UIRect				m_rcToolTip;				// Region of tool tip

	// UI_REFORM :Su-won
	UIRect				m_rcQuickMenu;				//퀵메뉴 영역
	UIRect				m_rcFace;					//캐릭별 일러스트 표시 영역

	// UV of each part
	UIRectUV			m_rtBackground;				// UV of background
	UIRectUV			m_rtHP;						// UV of HP
	UIRectUV			m_rtMP;						// UV of MP
	UIRectUV			m_rtEXP;					// UV of Exp
	UIRectUV			m_rtToolTipL;				// UV of left region of tool tip
	UIRectUV			m_rtToolTipM;				// UV of middle region of tool tip
	UIRectUV			m_rtToolTipR;				// UV of right region of tool tip

	// UI_REFORM :Su-won
	UIRectUV			m_rtQuickMenu;				//퀵메뉴 영역 텍스쳐 좌표
	UIRectUV			m_rtFace[TOTAL_JOB+1];					//캐릭별 일러스트 표시 영역

	// UI_REFORM :Su-won

protected:
	// Internal functinos
	void	ToggleUIWebBoard();
	void	ToggleUIMessanger();
	void	ToggleUISysMenu();
	void	ToggleUIInventory();
	void	ToggleUICharacterInfo();
	void	ShowToolTip( BOOL bShow, int nToolTipID = -1 );

public:
	CUIPlayerInfo();
	~CUIPlayerInfo();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Get information
	void	GetRatioOfGauges( FLOAT &fHP, FLOAT &fMP )
	{
		fHP = m_fHPRatio;	fMP = m_fMPRatio;
	}
	CTString	&GetStringOfLevel() { return m_strLevel; }
	CTString	&GetStringOfHP() { return m_strHP; }
	CTString	&GetStringOfMP() { return m_strMP; }

	// Update player information
	ENGINE_API void	UpdateAllInfos();
	ENGINE_API void	UpdateHPInfo();
	ENGINE_API void UpdateEXPInfo();

	// Process short cut of UIs
	BOOL	ProcessShortCut( MSG *pMsg );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// UI_REFORM :Su-won
	void ResetQuickMenuPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void ShowQuickMenuToolTip(BOOL bShow, int nToolTipID = -1);
	void OpenQuickMenu(int nMenu);
	
	// 2009. 06. 02 김정래
	// 전체화면에 표시할 퀘스트 내용 랜더
	void RenderSelectedQuest();

	// 2009. 06. 08 김정래
	// 전제화면에 표시할 타임어택 남은시간 랜더
	void RenderTimeAttackRemainTime();
};


#endif	// UIPLAYERINFO_H_


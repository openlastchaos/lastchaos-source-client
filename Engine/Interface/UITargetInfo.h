// ----------------------------------------------------------------------------
//  File : UITargetInfo.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UITARGETINFO_H_
#define	UITARGETINFO_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


#include <Engine/Interface/UIWindow.h>


// Define position
//#define	TARGETINFO_NAME_SY				5
#define	TARGETINFO_NAME_SY				2


// Define size of player information
//#define	TARGETINFO_WIDTH				140
//#define	TARGETINFO_HEIGHT				43
#define	TARGETINFO_WIDTH				187
#define	TARGETINFO_HEIGHT			57
#define	TARGETINFO_BAR_WIDTH			120

#define TARGETINFO_MAX_INFO_LINE 5


// ----------------------------------------------------------------------------
// Name : CUITargetInfo
// Desc :
// ----------------------------------------------------------------------------
class CUITargetInfo : public CUIWindow
{
protected:
	// Information
	COLOR				m_colNameColor[11];				// Colors of target name
	COLOR				m_colCurNameColor;				// Current color of target name
	BOOL				m_bShowBuff;					// If buff of target is shown or not
	BOOL				m_bShowPkHp;					// WSS_PK_TARGET_HP 070726

	// Region of each part
	UIRect				m_rcTitle;						// Title
	UIRect				m_rcHP;							// HP
	UIRect				m_rcHPBack;						// Background of HP
	UIRect				m_rcInfoMark;
	UIRect				m_rcLv;							// Level mark

	// UV of each part
	UIRectUV			m_rtBackground;					// Background
	UIRectUV			m_rtBackground_new_L;			// Background
	UIRectUV			m_rtBackground_new_M;			// Background
	UIRectUV			m_rtBackground_new_R;			// Background
	UIRectUV			m_rtNPCBackground;
	UIRectUV			m_rtMobBackground;
	UIRectUV			m_rtHP;							// HP
	UIRectUV			m_rtHPBack;						// Background of HP
	UIRectUV			m_rtInfoMarkUV;
	UIRectUV			m_rtLvUV;

	// Tool Tip	
	UIRectUV				m_rtInfoUL;								// UV of upper left region of information
	UIRectUV				m_rtInfoUM;								// UV of upper middle region of information
	UIRectUV				m_rtInfoUR;								// UV of upper right region of information
	UIRectUV				m_rtInfoML;								// UV of middle left region of information
	UIRectUV				m_rtInfoMM;								// UV of middle middle region of information
	UIRectUV				m_rtInfoMR;								// UV of middle right region of information
	UIRectUV				m_rtInfoLL;								// UV of lower left region of information
	UIRectUV				m_rtInfoLM;								// UV of lower middle region of information
	UIRectUV				m_rtInfoLR;								// UV of lower right region of information
	

	BOOL					m_bShowInfo;

	UIRect					m_rcInfo;
	int						m_nCurInfoLines;

	CTString				m_strInfo[TARGETINFO_MAX_INFO_LINE];		// information string
	COLOR					m_colInfo[TARGETINFO_MAX_INFO_LINE];		// Color of information string
protected:
	// Update info of target
	void	UpdateHPInfo();

public:
	CUITargetInfo();
	~CUITargetInfo();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();
	void	RenderInfo();

	// Buff
	void	SetTargetBuff( BOOL bMe );
	// WSS_PK_TARGET_HP 070726--------------------->>
	// Show Target Character HP
	inline void	ShowPKTargetHP( BOOL bOn) { m_bShowPkHp = bOn; }
	// --------------------------------------------<<

	// Get color of name
	COLOR	&GetNameColor( int nIndex ) { return m_colNameColor[nIndex]; }

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
};


#endif	// UITARGETINFO_H_


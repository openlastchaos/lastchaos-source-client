// ----------------------------------------------------------------------------
//  File : UIInitJob.h
//  Desc : Created by Dongmin
// ----------------------------------------------------------------------------

#ifndef	UIINITJOB_H_
#define	UIINITJOB_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Interface/UIButton.h>
#include <Engine/Interface/UIButtonEx.h>

// Define size of refine
#define	INITJOB_WIDTH					216
#define	INITJOB_HEIGHT					251

// Define max char and line of strings
#define	MAX_INITJOB_STRING				11

// ----------------------------------------------------------------------------
// Name : CUIInitJob
// Desc :
// ----------------------------------------------------------------------------
class CUIInitJob : public CUIWindow
{
protected:
	// InitJob State
	enum eInitJobState
	{
		INITJOB_REQ,		
		INITJOB_STAT,		// 생성
		INITJOB_GIVEUP,		// 승급
	};

	// Controls
	CUIButton			m_btnClose;								// Close button
	CUIButton			m_btnOK;								// OK button
	CUIButton			m_btnCancel;							// Cancel button

	// Strings
	int					m_nStringCount;							// Count of string
	CTString			m_strInitJobDesc[MAX_INITJOB_STRING];		// Strings of refine description
	CTString			m_strInitJobMoney;						// String of refine money
	CTString			m_strGivePrice;
	int					m_iRestoreStatPoint;					// 환원 포인트.
	CTString			m_strRestoreStatPoint;
	SQUAD				m_llInitJobMoney;						// InitJob money

	CTString			m_strStrength;
	CTString			m_strDexterity;
	CTString			m_strIntelligence;
	CTString			m_strConstitution;

	int					m_aswTemp[4];
	CTString			m_astrTemp[4];	
	int					m_aswNew[4];
	CTString			m_astrNew[4];

	int					m_nMoneyPosY;							// Position y of refine money text
	int					m_nTextRegionHeight;					// Height of text region

	// Position of target npc
	FLOAT				m_fNpcX, m_fNpcZ;

	// Region of each part
	UIRect				m_rcTitle;								// Region of title
	UIRect				m_rcItemSlot;							// Region of item slot

	// UV of each part
	UIRectUV			m_rtTop;								// UV of top background
	UIRectUV			m_rtMiddle1;							// UV of middle background
	UIRectUV			m_rtMiddle2;							// UV of middle background
	UIRectUV			m_rtMiddle3;							// UV of middle background
	UIRectUV			m_rtBottom;								// UV of bottom background

	UIRectUV			m_rtInputBoxL;							// UV of left region of input box
	UIRectUV			m_rtInputBoxM;							// UV of middle region of input box
	UIRectUV			m_rtInputBoxR;							// UV of right region of input box

	CUIButton			m_btnDPStrength;						// Button of strength decrease point
	CUIButton			m_btnDPDexterity;						// Button of dexterity decrease point
	CUIButton			m_btnDPIntelligence;					// Button of intelligence decrease point
	CUIButton			m_btnDPConstitution;					// Button of constitution decrease point

	eInitJobState		m_eInitJobState;

protected:
	// Internal functions
	void	AddString( CTString &strDesc );

	// Command functions
	void	SetInitJobItem();

public:
	CUIInitJob();
	~CUIInitJob();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Open & close refine
	ENGINE_API	void	OpenInitJob(int iMobIndex, BOOL bHasQuest, FLOAT fX, FLOAT fZ );
	void	CloseInitJob();

	// Calculate Price
	SQUAD	CalculateGiveUpPrice( int iTotalStat, int iCharLevel );
	SQUAD	CalculateStatPrice( int iStatPoint );

	void	RestoreStatPoint( SBYTE sbStatType );

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	MsgBoxLCommand( int nCommandCode, int nResult );
};


#endif	// UIINITJOB_H_


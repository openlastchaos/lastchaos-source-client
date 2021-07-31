// ----------------------------------------------------------------------------
//  File : UIMixNew.h
//  Desc : Created by wooss
// ----------------------------------------------------------------------------

#ifndef	UIMIXNEW_H_
#define	UIMIXNEW_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


#include <Engine/Interface/UIButton.h>
#include <Engine/Interface/UIButtonEx.h>
#include <Engine/Interface/UIInventory.h>

// Define max char and line of strings
//#define	MAX_MIXNEW_STRING				13
#define	MAX_MIXNEW_STRING				20


// Define text position
#define	MIXNEW_TITLE_TEXT_OFFSETX		25
#define	MIXNEW_TITLE_TEXT_OFFSETY		5
#define	MIXNEW_DESC_TEXT_SX			15
#define	MIXNEW_DESC_TEXT_SY			85


// Define size of refine
#define	MIXNEW_TOP_HEIGHT				26
#define	MIXNEW_BOTTOM_HEIGHT			7
#define	MIXNEW_WIDTH					216
#define	MIXNEW_HEIGHT					490

#define MIXNEW_ITEM_SLOT_SIZE			34
#define MIXNEW_ITEM_SLOT_COUNT			2


// ----------------------------------------------------------------------------
// Name : CUIMixNew
// Desc :
// ----------------------------------------------------------------------------
class CUIMixNew : public CUIWindow
{
protected:
	// Controls
	CUIButton			m_btnClose;							// Close button
	CUIButton			m_btnOK;							// OK button
	CUIButton			m_btnCancel;						// Cancel button
	CUIButtonEx			m_btnItemSlot[MIXNEW_ITEM_SLOT_COUNT];					// Slot item button
	BOOL				m_bWaitMixResult;					// If UI wait result from server or not

	// Strings
	int					m_nStringCount;						// Count of string
	CTString			m_strMixDesc[MAX_MIXNEW_STRING];		// Strings of refine description
	int					m_nTextRegionHeight;				// Height of text region

	// Region of each part
	UIRect				m_rcTitle;								// Region of title
	UIRect				m_rcItemSlot[MIXNEW_ITEM_SLOT_COUNT];	// Region of item slot
	UIRect				m_rcItemSlot_UN;						// Region of item slot
	UIRect				m_rcInsertItem;							// Region of inserting item

	// UV of each part
	UIRectUV			m_rtTop;							// UV of top background
	UIRectUV			m_rtMiddle1;						// UV of middle background
	UIRectUV			m_rtMiddle2;						// UV of middle background
	UIRectUV			m_rtBottom;							// UV of bottom background
	UIRectUV			m_rtItemSlot;						// UV of item slot
	
	int					m_nCurItemSlot;
	int					m_nCurItemCount;

	int					m_nTextItemIndex;					// 조합문서의 정보
	int					m_nTextRow;
	int					m_nTextCol;

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

	BOOL					m_bShowItemInfo;

	UIRect					m_rcItemInfo;
	int						m_nCurInfoLines;

	CTString				m_strItemInfo[MAX_ITEMINFO_LINE];		// Item information string
	COLOR					m_colItemInfo[MAX_ITEMINFO_LINE];		// Color of item information string
	BOOL					m_bDetailItemInfo;

	CTString 				m_strItemSlot[MIXNEW_ITEM_SLOT_COUNT];

	BOOL					m_bIsMix;

	// Internal functions
	void	AddString( CTString &strDesc );

	// Network message functions ( send )
	void	SendMixNewReq();

public:
	CUIMixNew();
	~CUIMixNew();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Open & close refine
	void	OpenMixNew(BOOL bMix);
	void	CloseMixNew();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Network message functions
	void	MixNewRep( SBYTE sbResult );

	// Command functions
	void	SetMixItem ( int nSlotIndex = -1 );
	void	ResetMixItem ();
	int		NearPosition ( int nX, int nY );
	void	SetMixEnable(BOOL bMix) { m_bIsMix = bMix; }
	BOOL	GetMixEnable() { return m_bIsMix; }

	// RenderItemInfo ...
	void	AddItemInfoString( CTString &strItemInfo, COLOR colItemInfo = 0xF2F2F2FF );
	BOOL	GetItemInfo( int nItemIndex, int &nInfoWidth, int &nInfoHeight );
	void	ShowItemInfo( BOOL bShowInfo, int nItemIndex, BOOL bRenew = FALSE );
	void	RenderItemInfo ();

	CUIButtonEx* GetItemSlot ( int nIndex )
	{
		if ( nIndex < 0 && nIndex >= MIXNEW_ITEM_SLOT_COUNT )
		{
			return NULL;
		}

		return &m_btnItemSlot[nIndex];		
	}
};


#endif	// UIMIX_H_


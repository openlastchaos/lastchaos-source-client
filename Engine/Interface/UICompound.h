// ----------------------------------------------------------------------------
//  File : UICompound.h
//  Desc : // Date : 2005-01-12,   By Lee Ki-hwan
// ----------------------------------------------------------------------------

#ifndef	UICOMPOUND_H_
#define	UICOMPOUND_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Interface/UIButton.h>
#include <Engine/Interface/UIButtonEx.h>
#include <Engine/Interface/UIInventory.h>

// Define max char and line of strings
#define	MAX_COMPOUND_STRING				4

// Define text position
#define	COMPOUND_TITLE_TEXT_OFFSETX		25
#define	COMPOUND_TITLE_TEXT_OFFSETY		5
#define	COMPOUND_DESC_TEXT_SX			15
#define	COMPOUND_DESC_TEXT_SY			33

// Define size of refine
#define	COMPOUND_TOP_HEIGHT				26
#define	COMPOUND_BOTTOM_HEIGHT			7
#define	COMPOUND_WIDTH					216
#define	COMPOUND_HEIGHT					190

#define COMPOUND_ITEM_SLOT_SIZE			34

#define ITEM_ARCANE_MATERIAL_UPGRADE		0 // 비밀의 상자 재료 0번 제련석 
#define ITEM_ARCANE_MATERIAL_ACCESSORY		1 // 비밀의 상자 재료 1번 악세사리 
#define ITEM_ARCANE_MATERIAL_SAMPLE			2 // 비밀의 상자 재료 2번 시료 
#define COMPOUND_ITEM_SLOT_COUNT			3


// ----------------------------------------------------------------------------
// Name : CUICompound
// Desc :
// ----------------------------------------------------------------------------
class CUICompound : public CUIWindow
{
protected:
	// Controls
	CUIButton			m_btnClose;							// Close button
	CUIButton			m_btnOK;							// OK button
	CUIButton			m_btnCancel;						// Cancel button
	CUIButtonEx			m_btnItemSlot[3];					// Slot item button
	BOOL				m_bWaitResult;					// If UI wait result from server or not

	// Strings
	int					m_nStringCount;						// Count of string
	CTString			m_strDesc[MAX_COMPOUND_STRING];		// Strings of refine description
	int					m_nTextRegionHeight;				// Height of text region

	// Region of each part
	UIRect				m_rcTitle;							// Region of title
	UIRect				m_rcItemSlot[3];						// Region of item slot
	UIRect				m_rcInsertItem;						// Region of inserting item

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

protected:
	// Internal functions
	void	AddString( CTString &strDesc );

	// Network message functions ( send )
	void	SendCompoundReq();

public:
	CUICompound();
	~CUICompound();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Open & close refine
	ENGINE_API	void	OpenCompound( int nItemIndex, int nRow, int nCol );
	void	CloseCompound();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Network message functions
	void	CompoundRep( SBYTE sbResult );

	// Command functions
	void	SetCompoundItem ( int nSlotIndex = -1 );
	void	ResetCompoundItem ();
	int		NearPosition ( int nX, int nY );

	// RenderItemInfo ...
	void	AddItemInfoString( CTString &strItemInfo, COLOR colItemInfo = 0xF2F2F2FF );
	BOOL	GetItemInfo( int nItemIndex, int &nInfoWidth, int &nInfoHeight );
	void	ShowItemInfo( BOOL bShowInfo, int nItemIndex, BOOL bRenew = FALSE );
	void	RenderItemInfo ();

	CUIButtonEx* GetItemSlot ( int nIndex )
	{
		if ( nIndex < 0 && nIndex >= COMPOUND_ITEM_SLOT_COUNT )
		{
			return NULL;
		}

		return &m_btnItemSlot[nIndex];		
	}
};


#endif	// UIMIX_H_


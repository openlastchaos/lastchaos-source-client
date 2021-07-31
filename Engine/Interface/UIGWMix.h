// ----------------------------------------------------------------------------
//  File : UIMix.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UIGWMIX_H_
#define	UIGWMIX_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Interface/UIButton.h>
#include <Engine/Interface/UIButtonEx.h>
#include <Engine/Interface/UIInventory.h>

// Define max char and line of strings
#define	MAX_GW_MIX_STRING				4
#define GW_MIX_ITEM_SLOT_COUNT			3

// ----------------------------------------------------------------------------
// Name : CUIGWMix
// Desc :
// ----------------------------------------------------------------------------
class CUIGWMix : public CUIWindow
{
protected:
	// Controls
	CUIButton			m_btnClose;							// Close button
	CUIButton			m_btnOK;							// OK button
	CUIButton			m_btnCancel;						// Cancel button
	CUIButtonEx			m_btnItemSlot[3];					// Slot item button
	BOOL				m_bWaitMixResult;					// If UI wait result from server or not

	// Strings
	int					m_nStringCount;						// Count of string
	CTString			m_strMixDesc[MAX_GW_MIX_STRING];	// Strings of refine description
	int					m_nTextRegionHeight;				// Height of text region

	// Region of each part
	UIRect				m_rcTitle;							// Region of title
	UIRect				m_rcItemSlot[3];					// Region of item slot
	UIRect				m_rcInsertItem;						// Region of inserting item

	// UV of each part
	UIRectUV			m_rtTop;							// UV of top background
	UIRectUV			m_rtMiddle1;						// UV of middle background
	UIRectUV			m_rtMiddle2;						// UV of middle background
	UIRectUV			m_rtBottom;							// UV of bottom background
	UIRectUV			m_rtItemSlot;						// UV of item slot
	
	int					m_nCurItemSlot;
	int					m_nCurItemCount;

	// Tool Tip	
	UIRectUV				m_rtInfoUL;						// UV of upper left region of information
	UIRectUV				m_rtInfoUM;						// UV of upper middle region of information
	UIRectUV				m_rtInfoUR;						// UV of upper right region of information
	UIRectUV				m_rtInfoML;						// UV of middle left region of information
	UIRectUV				m_rtInfoMM;						// UV of middle middle region of information
	UIRectUV				m_rtInfoMR;						// UV of middle right region of information
	UIRectUV				m_rtInfoLL;						// UV of lower left region of information
	UIRectUV				m_rtInfoLM;						// UV of lower middle region of information
	UIRectUV				m_rtInfoLR;						// UV of lower right region of information

	BOOL					m_bShowItemInfo;

	UIRect					m_rcItemInfo;
	int						m_nCurInfoLines;

	CTString				m_strItemInfo[MAX_ITEMINFO_LINE];	// Item information string
	COLOR					m_colItemInfo[MAX_ITEMINFO_LINE];	// Color of item information string
	BOOL					m_bDetailItemInfo;

protected:
	// Internal functions
	void	AddString( CTString &strDesc );

	// Network message functions ( send )
	void	SendMixReq();

public:
	CUIGWMix();
	~CUIGWMix();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Open & close refine
	ENGINE_API	void	OpenGWMix();
	void	CloseGWMix();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Network message functions
	void	MixRep( SBYTE sbResult );

	// Command functions
	void	SetMixItem ( int nSlotIndex = -1 );
	void	ResetMixItem ();
	int		NearPosition ( int nX, int nY );

	// RenderItemInfo ...
	void	AddItemInfoString( CTString &strItemInfo, COLOR colItemInfo = 0xF2F2F2FF );
	BOOL	GetItemInfo( int nItemIndex, int &nInfoWidth, int &nInfoHeight );
	void	ShowItemInfo( BOOL bShowInfo, int nItemIndex, BOOL bRenew = FALSE );
	void	RenderItemInfo ();

	CUIButtonEx* GetItemSlot ( int nIndex )
	{
		if ( nIndex < 0 && nIndex >= GW_MIX_ITEM_SLOT_COUNT )
		{
			return NULL;
		}

		return &m_btnItemSlot[nIndex];		
	}
};


#endif	// UIGWMIX_H_


// ----------------------------------------------------------------------------
//  File : UIExchange.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UIEXCHANGE_H_
#define	UIEXCHANGE_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


#include <Engine/Interface/UIInventory.h>


// Exchange request type
enum ExchangeReq
{
	EXCHREQ_REQ				= 0,
	EXCHREQ_REP				= 1,
	EXCHREQ_REJECT			= 2,
	EXCHREQ_REJECT_SRC		= 3,
	EXCHREQ_REJECT_DEST		= 4,
	EXCHREQ_READY			= 5,
	EXCHREQ_READY_SRC		= 6,
	EXCHREQ_READY_DST		= 7,
	EXCHREQ_FULL_SRC		= 8,
	EXCHREQ_FULL_DEST		= 9,
	EXCHREQ_REQ_OK			= 10,
	EXCHREQ_REQ_OK_SRC		= 11,
	EXCHREQ_REQ_OK_DEST		= 12
};


// Exchange item action type
enum ExchangeItemAction
{
	EXCHITEM_ADD			= 0,
	EXCHITEM_ADD_SRC		= 1,
	EXCHITEM_ADD_DEST		= 2,
	EXCHITEM_DEL			= 3,
	EXCHITEM_DEL_SRC		= 4,
	EXCHITEM_DEL_DEST		= 5,
	EXCHITEM_UPDATE_SRC		= 6,
	EXCHITEM_UPDATE_DEST	= 7
};


// Define exchange item slot
#define	EXCH_EXCH_SLOT_COL			5
#define	EXCH_EXCH_SLOT_TOTAL		10
#define	EXCH_INVEN_SLOT_COL			5
#define	EXCH_INVEN_SLOT_ROW			4		
#define	EXCH_INVEN_SLOT_ROW_TOTAL	20
#define	EXCH_SLOT_SX				14
#define	EXCH_OPP_SLOT_SY			51
#define	EXCH_MY_SLOT_SY				150
#define	EXCH_INVEN_SLOT_SY			249


// Define text position
#define	EXCH_TITLE_TEXT_OFFSETX		25
#define	EXCH_TITLE_TEXT_OFFSETY		5
#define	EXCH_OPP_NAME_OFFSETX		58
#define	EXCH_OPP_NAME_OFFSETY		33
#define	EXCH_MONEY_OFFSETX			180
#define	EXCH_OPP_MONEY_OFFSETY		124
#define	EXCH_MY_MONEY_OFFSETY		223
#define	EXCH_MYCUR_MONEY_OFFSETY	392


// Define size of exchange
#define	EXCHANGE_WIDTH				216
#define	EXCHANGE_HEIGHT				452


// ----------------------------------------------------------------------------
// Name : CUIExchange
// Desc :
// ----------------------------------------------------------------------------
class CUIExchange : public CUIWindow
{
protected:
	// Controls
	CUIButton			m_btnClose;								// Close button
	CUIButton			m_btnReady;								// Ready button
	CUIButton			m_btnExchange;							// Exchange button
	CUIButton			m_btnCancel;							// Cancel button
	CUIScrollBar		m_sbScrollBar;							// Scroll bar

	// Exchange items
	CUIButtonEx			m_abtnOppItems[EXCH_EXCH_SLOT_TOTAL];	// Items of opposite slot
	CUIButtonEx			m_abtnMyItems[EXCH_EXCH_SLOT_TOTAL];	// Items of my slot
	CUIButtonEx			m_abtnInvenItems[EXCH_INVEN_SLOT_ROW_TOTAL][EXCH_INVEN_SLOT_COL];	// Items of inventory slot

	// Item information
	int					m_nSelMyItemID;							// Selected item ID of my slot
	int					m_nSelInvenItemID;						// Selected item ID of inventory slot
	BOOL				m_bShowItemInfo;						// If item tool tip is shown or not
	BOOL				m_bDetailItemInfo;						// If item informaion is shown in detail or not
	int					m_nCurInfoLines;						// Count of current item information lines
	CTString			m_strItemInfo[MAX_ITEMINFO_LINE];		// Item information string
	COLOR				m_colItemInfo[MAX_ITEMINFO_LINE];		// Color of item information string
	UIRect				m_rcItemInfo;							// Item information region

	BOOL				m_bRareItem;
	int					m_iRareGrade;

	// Exchange information
	BOOL				m_bAmISrc;								// If I am source or not
	int					m_nSrcIndex;							// Source index
	int					m_nDestIndex;							// Destination index
	CTString			m_strSrcName;							// Source name
	CTString			m_strDestName;							// Destination name
	BOOL				m_bExchReady;							// If exchange is ready or not
	BOOL				m_bExchReadyDest;						// If exchange of target is ready or not
	CTString			m_strMyCurMoney;						// Current money in inventory
	CTString			m_strOppExchMoney;						// Exchange money of opposite
	CTString			m_strMyExchMoney;						// Exchange money of me

	// Region of each part
	UIRect				m_rcTitle;								// Region of title
	UIRect				m_rcOppSlot;							// Region of opposite slot
	UIRect				m_rcMySlot;								// Region of my slot
	UIRect				m_rcInvenSlot;							// Region of inventory slot

	// UV of each part
	UIRectUV			m_rtBackground;							// UV of background
	UIRectUV			m_rtInfoL;								// UV of left region of information
	UIRectUV			m_rtInfoM;								// UV of middle region of information
	UIRectUV			m_rtInfoR;								// UV of right region of information
	UIRectUV			m_rtInfoUL;								// UV of upper left region of information
	UIRectUV			m_rtInfoUM;								// UV of upper middle region of information
	UIRectUV			m_rtInfoUR;								// UV of upper right region of information
	UIRectUV			m_rtInfoML;								// UV of middle left region of information
	UIRectUV			m_rtInfoMM;								// UV of middle middle region of information
	UIRectUV			m_rtInfoMR;								// UV of middle right region of information
	UIRectUV			m_rtInfoLL;								// UV of lower left region of information
	UIRectUV			m_rtInfoLM;								// UV of lower middle region of information
	UIRectUV			m_rtInfoLR;								// UV of lower right region of information
	UIRectUV			m_rtWearingOutline;						// UV of outline of wearing items
	UIRectUV			m_rtSelectOutline;						// UV of outline of selected items

protected:
	// Internal functions
	void	PrepareExchange();	
	void	AddItemInfoString( CTString &strItemInfo, COLOR colItemInfo = 0xF2F2F2FF );
	void	GetItemInfo( int nWhichSlot, int &nInfoWidth, int &nInfoHeight,
							int nOppItem = -1, int nMyItem = -1, int nRow = -1, int nCol = -1 );
	void	ShowItemInfo( BOOL bShowInfo, BOOL bRenew = FALSE, int nOppItem = -1, int nMyItem = -1,
							int nRow = -1, int nCol = -1 );
	void	RenderItems();

	// Command functions
	void	AddExchItem( int nRow, int nCol, int nUniIndex, SQUAD llCount );
	void	DelExchItem( int nRow, int nCol, int nUniIndex, SQUAD llCount );

	// Network message functions ( send )
	void	SendExchangeReq_Rep();
	void	SendExchangeReq_Rej();
	void	SendExchangeReq_Ready();
	void	SendExchangeReq_Ok();
	void	SendExchangeItem_Add( int nUniIndex, SQUAD llCount );
	void	SendExchangeItem_Del( int nUniIndex, SQUAD llCount );

public:
	CUIExchange();
	~CUIExchange();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Network message functions ( send )
	void	SendExchangeReq_Req( CTString &strSrcName );

	// Network message functions ( receive )
	void	ExchangeReq_Req( int nSrcIndex, CTString &strSrcName,
								int nDestIndex, CTString &strDestName );
	void	ExchangeReq_Rep();
	void	ExchangeReq_Rej( BOOL bSrcReject );
	void	ExchangeReq_Ready( BOOL bSrcReady );
	void	ExchangeReq_BothReady();
	void	ExchangeReq_Full( BOOL bSrcFull );
	void	ExchangeReq_Ok( BOOL bSrcOk );
	void	ExchangeReq_BothOk();
	void	ExchangeReq_Pet_Err(BOOL petErr);

	void	ExchangeItem_Add( BOOL bSrcAdd, int nUniIndex, SQUAD llCount,
								int nIndex, ULONG ulPlus, ULONG ulFlag, LONG ulUsed, LONG ulUsed2, LONG lRareIndex =0 );
	void	ExchangeItem_SetOptionData( BOOL bSrcAdd, int nUniIndex, SBYTE sbOption,
										SBYTE sbOptionType, SBYTE sbOptionLevel, LONG lRareIndex =0 );
	void	ExchangeItem_SetRareOption( CNetworkMessage *istr, BOOL bSrcAdd, int nUniIndex);
	void	ExchangeItem_Del( BOOL bSrcDel, int nUniIndex );
	void	ExchangeItem_Update( BOOL bSrcUpdate, int nUniIndex, SQUAD llCount );

	void	ResetExchange();

	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
};


#endif	// UIINVENTORY_H_


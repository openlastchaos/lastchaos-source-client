// ----------------------------------------------------------------------------
//  File : UIWareHouse.h
//  Desc : Created by dongmin
// ----------------------------------------------------------------------------

#ifndef	UIWAREHOUSE_H_
#define	UIWAREHOUSE_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


#include <Engine/Interface/UIScrollBar.h>
#include <Engine/Interface/UIButtonEx.h>
#include <Engine/Interface/UIInventory.h>
#include <vector>
class CItems;


// Column & Row
#define WAREHOUSE_WAREHOUSE_SLOT_COL		5
#define WAREHOUSE_WAREHOUSE_SLOT_ROW		5
#define WAREHOUSE_WAREHOUSE_SLOT_ROW_TOTAL	20	 // wooss 050817 10 -> 20 050917 20 -> 10 유료화시 복구
												 // 051208 -> 유료화 작업으로 
#define WAREHOUSE_TRADE_SLOT_COL			5
#define	WAREHOUSE_TRADE_SLOT_TOTAL			10


// Define text position
#define	WAREHOUSE_TITLE_TEXT_OFFSETX		25
#define	WAREHOUSE_TITLE_TEXT_OFFSETY		5
#define	WAREHOUSE_MYMONEY_POSX				95
#define	WAREHOUSE_MYMONEY_POSY				310		// wooss 050811 283 -> 310
#define	WAREHOUSE_TRADEPRICE_POSX			177
#define	WAREHOUSE_BUY_TRADEPRICE_POSY		312		// wooss 050811 285 -> 312
#define	WAREHOUSE_SELL_TRADEPRICE_POSY		101
#define	WAREHOUSE_EXTEND_TEXT_POSX			15		// wooss 
#define	WAREHOUSE_EXTEND_TEXT_POSY			26		// wooss 
#define	WAREHOUSE_EXTEND_DAYS_POSX			180		// wooss 

// Define size of warehouse
#define WAREHOUSE_WIDTH						216
#define	WAREHOUSE_HEIGHT					369		// wooss 050811 338 -> 369


// ----------------------------------------------------------------------------
// Name : CUIWareHouse
// Desc :
// ----------------------------------------------------------------------------
class CUIWareHouse : public CUIWindow
{
protected:
	BOOL				m_bStorageWareHouse;
	BOOL				m_bSealed;
	BOOL				m_bHasPassword;
	int					m_nCurRow;
	int					m_nNpcIndex;				// For showing name of npc
	BOOL				m_bHasQuest;

	int					m_nSelWareHouseItemID;
	int					m_nSelTradeItemID;
	int					m_nSelectedWareHouseID;		// WareHouse ID
	int					m_nTex;
	
	BOOL				m_bRareItem;
	int					m_iRareGrade;
	
	CTString			m_strTotalPrice;
	CTString			m_strPW;

	__int64				m_nTotalPrice;
	int					m_nNumOfItem;
	
	//cash item remain time
	int					m_useTime;				// wooss 050817	
	// Region
	UIRect				m_rcMainTitle;	
	UIRect				m_rcTop;				// WareHouse
	UIRect				m_rcBottom;				// Trade
	UIRect				m_rcExInvenRect;		// wooss 050811

	// Background
	UIRectUV			m_rtBackTop;
	UIRectUV			m_rtBackMiddle;
	UIRectUV			m_rtBackBottomLine;
	UIRectUV			m_rtBackBottom;

	// wooss 050811
	// extand inventroy remain days
	UIRectUV			m_rtExInven;


	// Position of target npc
	FLOAT				m_fNpcX, m_fNpcZ;

	// Inventory
	UIRectUV			m_rtTopTopInven;
	UIRectUV			m_rtTopMiddleInven;
	UIRectUV			m_rtTopBottomInven;
	UIRectUV			m_rtBottomInven;
	UIRectUV			m_rtSeperatorInven;
	UIRectUV			m_rtBlankInven;
	UIRectUV			m_rtPriceInven;

	// Item Info
	UIRectUV			m_rtInfoUL;								// UV of upper left region of information
	UIRectUV			m_rtInfoUM;								// UV of upper middle region of information
	UIRectUV			m_rtInfoUR;								// UV of upper right region of information
	UIRectUV			m_rtInfoML;								// UV of middle left region of information
	UIRectUV			m_rtInfoMM;								// UV of middle middle region of information
	UIRectUV			m_rtInfoMR;								// UV of middle right region of information
	UIRectUV			m_rtInfoLL;								// UV of lower left region of information
	UIRectUV			m_rtInfoLM;								// UV of lower middle region of information
	UIRectUV			m_rtInfoLR;								// UV of lower right region of information
	UIRectUV			m_rtUnmovableOutline;					// UV of outline of unmovable items
	UIRectUV			m_rtSelectOutline;						// UV of outline of selected items

	BOOL				m_bShowItemInfo;						// If item tool tip is shown or not
	BOOL				m_bDetailItemInfo;						// If item informaion is shown in detail or not
	int					m_nCurInfoLines;						// Count of current item information lines
	CTString			m_strItemInfo[MAX_ITEMINFO_LINE];		// Item information string
	COLOR				m_colItemInfo[MAX_ITEMINFO_LINE];		// Color of item information string
	UIRect				m_rcItemInfo;							// Item information region	

	// Buttons
	CUIButton			m_btnClose;								// Close button of WareHouse
	CUIButton			m_btnWareHouseKeep;						// Keep button of WareHouse
	CUIButton			m_btnWareHouseTake;						// Take button of WareHouse
	CUIButton			m_btnWareHouseCancel;					// Cancel button of WareHouse	
	CUIScrollBar		m_sbTopScrollBar;						// Scroll bar of warehouse

	// Storage Items
	std::vector<CItems>	m_vectorStorageItems;

	// Items	
	CUIButtonEx			m_abtnTradeItems[WAREHOUSE_TRADE_SLOT_TOTAL];								// Player Slot items
	CUIButtonEx			m_abtnWareHouseItems[WAREHOUSE_WAREHOUSE_SLOT_ROW_TOTAL][WAREHOUSE_WAREHOUSE_SLOT_COL];			// WareHouse Slot items

public:
	CUIWareHouse();
	~CUIWareHouse();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void	OpenWareHouse( SBYTE sbSetPW );
	void	CheckHasPassWord( int iMobIndex, BOOL bHasQuest, FLOAT fX, FLOAT fZ );

	// Add & Remove Item
	void	AddItemToList( int iUniIndex, SQUAD llCount );
	void	DelItemFromList( int iUniIndex, SQUAD llCount );

	inline BOOL HasPassword() const
	{	return m_bHasPassword; }

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	
	// Clear
	void	ClearItems();
	void	ResetWareHouse();

	// WareHouse -> Inventory
	void	DelWareHouseItemToInventory( );

	// Refresh
	void	RefreshWareHouse();
	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	MsgBoxLCommand( int nCommandCode, int nResult );

	// Network functions
	void	ReceiveWareHouseListRep(CNetworkMessage *istr);

	// set,get use time
	void SetUseTime(int t){m_useTime=t;}
	int GetUseTime(){return m_useTime;}


protected:
	// Internal functions
	void	AddItemInfoString( CTString &strItemInfo, COLOR colItemInfo = 0xF2F2F2FF );
	BOOL	GetItemInfo( int nWhichSlot, int &nInfoWidth, int &nInfoHeight,
							int nTradeItem = -1, int nRow = -1, int nCol = -1 );
	void	ShowItemInfo( BOOL bShowInfo, BOOL bRenew = FALSE, int nTradeItem = -1, int nRow = -1, int nCol = -1 );
	void	RenderWareHouseItems();

	void	PrepareWareHouse();
	void	TradeToWareHouse( SQUAD llCount, ULONG ulPlus, ULONG ulFlag, LONG lUsed );
	void	WareHouseToTrade( SQUAD llCount, ULONG ulPlus, ULONG ulFlag, LONG lUsed, LONG lRareIndex);
	void	PrepareBuyWareHouse();
	void	PrepareSellWareHouse();	
	SQUAD	CalculateStoragePrice();

	// Command functions
	void	AddWareHouseItem( int nRow, int nCol, int nUniIndex, SQUAD llCount );
	void	DelWareHouseItem( int nRow, int nCol, int nUniIndex, SQUAD llCount, int nTradeItemID );
	void	AddWareHouseItemFromInventory( );	
	void	KeepItems();
	void	TakeItems();

	// Network functions
// 강동민 수정 시작		// 창고
	void	SendWareHouseIsSetPassword();
	void	SendWareHouseListReq( const CTString& strPW );
	void	SendWareHouseKeepReq();
	void	SendWareHouseTakeReq();
// 강동민 수정 끝		// 창고


	
};

#endif // UIWAREHOUSE_H_


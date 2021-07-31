// ----------------------------------------------------------------------------
//  File : UIShop.h
//  Desc : Created by dongmin
// ----------------------------------------------------------------------------

#ifndef	UISHOP_H_
#define	UISHOP_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


#include <Engine/Interface/UIScrollBar.h>
#include <Engine/Interface/UIButtonEx.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Entities/ItemData.h>


// Define item slot
#define SHOP_SHOP_WIDTH					216
#define SHOP_SHOP_HEIGHT				151
#define SHOP_TRADE_HEIGHT				98
#define SHOP_LEASE_HEIGHT				77

#define SHOP_BUY_TOP_SLOT_SX			11		// Buy Mode(Top)
#define SHOP_BUY_TOP_SLOT_SY			28
#define SHOP_BUY_BOTTOM_SLOT_SX			11		// (Bottom)
#define SHOP_BUY_BOTTOM_SLOT_SY			180

#define SHOP_SELL_TOP_SLOT_SX			11		// Sell Mode(Top)
#define SHOP_SELL_TOP_SLOT_SY			28
#define SHOP_SELL_BOTTOM_SLOT_SX		11		// (Bottom)
#define SHOP_SELL_BOTTOM_SLOT_SY		127


// Column & Row
#define SHOP_SHOP_SLOT_COL				5
#define SHOP_SHOP_SLOT_ROW				4
#define SHOP_SHOP_SLOT_ROW_TOTAL		50
#define SHOP_TRADE_SLOT_COL				5
#define	SHOP_TRADE_SLOT_TOTAL			10


// Define text position
#define	SHOP_TITLE_TEXT_OFFSETX			25
#define	SHOP_TITLE_TEXT_OFFSETY			5
#define	SHOP_MYMONEY_POSX				95
#define	SHOP_MYMONEY_POSY				283
#define	SHOP_TRADEPRICE_POSX			177
#define	SHOP_BUY_TRADEPRICE_POSY		253
#define	SHOP_SELL_TRADEPRICE_POSY		101


// Define size of shop
#define SHOP_MAIN_WIDTH					216
#define	SHOP_MAIN_HEIGHT				305

class CItemData;


// ----------------------------------------------------------------------------
// Name : CUIShop
// Desc :
// ----------------------------------------------------------------------------
class CUIShop : public CUIWindow
{
protected:
	BOOL				m_bBuyShop;
	int					m_nCurRow;

	int					m_nSelShopItemID;
	int					m_nSelTradeItemID;
	int					m_nSelectedShopID;		// Shop ID

	CTString			m_strPlayerMoney;
	CTString			m_strTotalPrice;

	__int64				m_nTotalPrice;
	int					m_nNumOfItem;
	// Region
	UIRect				m_rcMainTitle;
	UIRect				m_rcBuyShopSlot;		// Buy Mode
	UIRect				m_rcBuyTradeSlot;
	UIRect				m_rcSellShopSlot;		// Sell Mode
	UIRect				m_rcSellTradeSlot;
	UIRect				m_rcTop;				// Shop
	UIRect				m_rcBottom;				// Trade

	// Background
	UIRectUV			m_rtBackTop;
	UIRectUV			m_rtBackMiddle;
	UIRectUV			m_rtBackBottom;

	// Position of target npc
	FLOAT				m_fNpcX, m_fNpcZ;

	// Inventory
	UIRectUV			m_rtTopInven;
	UIRectUV			m_rtBottomInven;
	UIRectUV			m_rtSeperatorInven;

	// for lease 
	UIRect				m_rcBottom1;
	UIRect				m_rcBottom2;
	UIRectUV			m_rtBottom1;
	UIRectUV			m_rtBottom2;
	UIRectUV			m_rtBottom3;
	UIRectUV			m_rtleaseMark;

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
	CUIButton			m_btnClose;								// Close button of Shop
	CUIButton			m_btnShopBuy;							// Buy button of Shop
	CUIButton			m_btnShopSell;							// Sell button of Shop
	CUIButton			m_btnShopCancel;						// Cancel button of Shop
	CUIScrollBar		m_sbTopScrollBar;						// Scroll bar of shop
	CUIScrollBar		m_sbBottomScrollBar;					// Scroll bar of inventory

	// etc
	BOOL				m_bIsLease;
	BOOL				m_bIsFieldShop;							// ttos : 잡화상 이용 주문서 사용시 체크
	// event : 가정의 달 이벤트
	CUIButton			m_btnEvent_weapon;						// ttos : 무기류
	CUIButton			m_btnEvent_shield;						// ttos : 방어구류
	BOOL				m_bIsEvent;								// ttos : 2007 가정의 달 이벤트 
	int					m_iEventjob;

public:
	// Items	
	CUIButtonEx			m_abtnTradeItems[SHOP_TRADE_SLOT_TOTAL];								// Trade Slot items
	CUIButtonEx			m_abtnShopItems[SHOP_SHOP_SLOT_ROW_TOTAL][SHOP_SHOP_SLOT_COL];			// Shop Slot items

public:
	CUIShop();
	~CUIShop();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void	OpenShop( int iMobIndex, BOOL bHasQuest, FLOAT fX, FLOAT fZ );
	void	EventOpenShop( int iMobIndex, BOOL bHasQuest, FLOAT fX, FLOAT fZ );

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	
	// Clear
	void	ClearItems();
	void	RefreshPlayerItem();
	void	ResetShop();
	void	CloseShop(); //WSS_SHOP_BUGFIX 070530

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	MsgBoxLCommand( int nCommandCode, int nResult );
	inline void SetFieldShopChack ( BOOL bChack) {m_bIsFieldShop = bChack;}

	void	CreateCubeStateMsgBox(CNetworkMessage *istr, BOOL bGuild);

protected:
	// Internal functions
	void	AddItemInfoString( CTString &strItemInfo, COLOR colItemInfo = 0xF2F2F2FF );
	BOOL	GetItemInfo( int nWhichSlot, int &nInfoWidth, int &nInfoHeight,
							int nTradeItem = -1, int nRow = -1, int nCol = -1 );
	void	ShowItemInfo( BOOL bShowInfo, BOOL bRenew = FALSE, int nTradeItem = -1, int nRow = -1, int nCol = -1 );
	void	RenderShopItems();

	void	TradeToShop( SQUAD llCount );
	void	ShopToTrade( SQUAD llCount );
	__int64	CalculateTotalPrice(int iShopID, int& iCount, BOOL bSell);
	__int64	CalculateItemPrice(int iShopID, const CItemData &ID, int iNumOfItem, BOOL bSell);		
	void	PrepareBuyShop();
	void	PrepareSellShop();	
	void	PrepareLeaseShop(int jobIdx, int itype = CItemData::ITEM_WEAPON);	

	// Command functions
	void	AddShopItem( int nRow, int nCol, int nUniIndex, SQUAD llCount );
	void	DelShopItem( int nRow, int nCol, int nUniIndex, SQUAD llCount, int nTradeItemID );
	void	BuyItems();
	void	SellItems();

};

#endif // UISHOP_H_


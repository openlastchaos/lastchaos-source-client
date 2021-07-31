// ----------------------------------------------------------------------------
//  File : UIPersonalShop.h
//  Desc : Created by dongmin
// ----------------------------------------------------------------------------

#ifndef	UIPERSONALSHOP_H_
#define	UIPERSONALSHOP_H_
#ifdef	PRAGMA_ONCE
#pragma once
#endif

#include <Engine/Interface/UIEditBox.h>

class CItems;
class CItemData;
class CUIEditBox;
class CUIArray;

// SHOP CONF
#define PS_MAX_SHOPNAME		100				// 상점 이름 최대 길이 100 바이트
#define PS_MAX_NORMAL_ITEM	10				// 일반 아이템 거래 최대 수
#define PS_MAX_PACKAGE_ITEM	5				// 패키지 판매 거래 최대 수

// PERSONAL SHOP TYPE
#define PST_NOSHOP			0
#define PST_SELL			(1 << 0)		// 판매
#define PST_BUY				(1 << 1)		// 구매
#define PST_PREMIUM			(1 << 2)		// 프리미엄
#define PST_PACKAGE			(1 << 3)		// 패키지

// Define item slot
// #define PERSONAL_SHOP_WIDTH					216
// #define PERSONAL_SHOP_HEIGHT				151
// #define PERSONAL_TRADE_HEIGHT				77		// 98

// Column & Row
#define PERSONAL_SHOP_SLOT_COL				5
#define PERSONAL_SHOP_SLOT_ROW				4
#define PERSONAL_SHOP_SLOT_ROW_TOTAL		20
#define	PERSONAL_SHOP_SLOT_MAX				(ITEM_COUNT_IN_INVENTORY_NORMAL + ITEM_COUNT_IN_INVENTORY_CASH_1 + ITEM_COUNT_IN_INVENTORY_CASH_2)
#define PERSONAL_TRADE_SLOT_COL				5
#define	PERSONAL_TRADE_SLOT_TOTAL			10
#define PERSONAL_PACKAGE_SLOT_COL			5

// ----------------------------------------------------------------------------
// Name : CUIPersonalShop
// Desc :
// ----------------------------------------------------------------------------
class CUIPersonalshopNew : public CUIWindow
{
public:
	enum eSlotType
	{
		SLOT_PACKAGE	= 0,
		SLOT_TRADE		= 1,
		SLOT_SHOP		= 2,
	};

	enum eCMD_TYPE
	{
		CMD_NONE = -1,
		CMD_DBLCLICK = 0,
		CMD_LBUTTONUP,

	};

	CUIPersonalshopNew();
	~CUIPersonalshopNew();

	// Create
	//void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void initialize();
	void	OpenPersonalShop( bool bSellShop );			// 개인 상점 개설 ( TRUE -> 판매, FALSE -> 구매 )
	ENGINE_API void	TradePersonalShop( INDEX iChaIndex, FLOAT fX, FLOAT fZ, bool bBuy );				// 개인 상점에서 아이템 사기&팔기

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Clear
	void	ClearItems();
	void	RefreshPlayerItem();
	void	RefreshUserItem();
	ENGINE_API void	ResetShop(bool bOpen=FALSE);

	bool	IsBuyShop() const { return m_bBuyShop; }
	void	ChangeShopState(bool bShopStart);
	bool	EndBuyShop(int nChaID);
	//void	ToggleVisible();

	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );

	// Set focus
	void	SetFocus( bool bVisible );

	// Edit box focus
//	bool	IsEditBoxFocused() { return m_ebPackagePrice.IsFocused() || m_ebShopName.IsFocused(); }
	void	KillFocusEditBox()
	{
//		m_ebPackagePrice.SetFocus( FALSE );
//		m_ebShopName.SetFocus( FALSE );
	}

	// Network Message 
	void	SendPersonalShopStart();
	void	SendPersonalShopStop();
	void	SendPersonalShopBuy();
	void	SendPersonalOpenShop(int iChaIndex);
	void	ReceivePersonalShopItemList(int iChaIndex, CNetworkMessage *istr);
	void	ReceivePersonalShopItemUpdate(CNetworkMessage *istr);

	// 판매대행 상인
	bool	GetCashPersonShop() {return m_bCashPersonShop;}
	void	SetCashPersonShop(bool bPersonShop) { m_bCashPersonShop = bPersonShop;}
	void	SendCashPersonOpenShop(int iChaIndex);
	void	SendCashPersonShopBuy();

	void	AddItemCallback();
	void	DelItemCallback();

	BOOL	IsEditBoxFocused();
	
	void	proc_command(eCMD_TYPE eCmd, eSlotType eSlot);

	void	start_shop();
	
	void	cancel();
	void	buy();

	void	check_premium();

	void	package_ok();
	void	package_cancel();

protected:
	// Internal functions
	void	RenderShopItems();

	void	TradeToShop( SQUAD llCount, int iSlot );				// Trade -> Shop
	void	ShopToTrade( SQUAD llCount, SQUAD llPrice, int iSlot);	// Shop -> Trade
	void	PackageToShop( bool bAdd );

	void	FindShopSlot( int& nIdx, int iIndex, ULONG ulFlag);
	void	FindShopSlot( int& nIdx, int iVirtualIdx );
	void	FindEmptySlot( int& nIdx );

	__int64	CalculateTotalPrice(int& iCount);
	__int64	CalculateItemPrice(int iShopID, const CItemData &ID, int iNumOfItem, bool bSell);		
	void	PrepareBuyShop();
	void	PrepareSellShop();	

	// Command functions
	void	AddShopItem( int nIdx, int nUniIndex, SQUAD llCount, int nWhichSlot );
	void	AskQuantity();
	void	DelShopItem( int nTab, int nIdx, int nUniIndex, SQUAD llCount, int nTradeItemID, int nPackageItemID, int nWhichSlot );
	void	BuyItems();	

	void	ClearPackageSlot();

	WMSG_RESULT	LButtonUp(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);
	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);

	void	proc_dblclick(eSlotType type);
	void	proc_lbutton_up(eSlotType type);

private:
	void	clearContainer();
	void	CloseMsg();

	typedef		std::vector< CItems* >		vec_Items;
	typedef		vec_Items::iterator			vec_Items_iter;	

	bool				m_bBuyShop;				// 판매 상점인가? 구매 창인가?
	bool				m_bShopStart;			// 판매 개시했는가?
	BOOL				m_bPremium;				// 프리미엄 서비스 사용.
	SQUAD				m_llPackagePrice;
	bool				m_bPackageBuy;

	int					m_nSelectedChaID;	// Character ID

	__int64				m_nTotalPrice;	
	int					m_nNumOfItem;

	UINT16				ox, oy;

	CTString			m_strShopName;

	vec_Items			m_vectorSellItemList;					// 판매 물품에 등록.(네트워크에서 받아와서...)
	vec_Items			m_vectorSellPackageList;				// 판매 패키지 물품에 등록.(네트워크에서 받아와서...)

	bool				m_bCashPersonShop;						// 판매대행 상인
	bool				m_bCashPersonShop_open;					// 판매 대행 상인 거래중

	bool				m_bTitleClick;

	CUIArray*			m_pArrTrade;
	CUIArray*			m_pArrPackage;
	CUIArray*			m_pArrShop;

	// Items	
	CUIIcon*			m_pIconsTradeItem[PERSONAL_TRADE_SLOT_TOTAL];			// Player Slot items
	CUIIcon*			m_pIconsShopItem[PERSONAL_SHOP_SLOT_MAX];				// Shop Slot items
	CUIIcon*			m_pIconsPackageItem[PERSONAL_PACKAGE_SLOT_COL];			// Package Slot items

	CItems*				m_pTempItems;

	CUICheckButton*		m_pChkPremium;
	CUIEditBox*			m_pEditPackagePrice;
	CUIButton*			m_pBtnPackOk;
	CUIButton*			m_pBtnPackCancel;

	CUIEditBox*			m_pEditName;

	
	CUIButton*			m_pBtnCancel;
	CUIButton*			m_pBtnBuy;

	CUIButton*			m_pBtnStart;		// 판매 시작 / 중지

	CUIText*			m_pTxtNas;
	CUIText*			m_pTxtTotal;
	CUIText*			m_pTxtPackagePrice;
	CUIText*			m_pTxtTitle;

	CUIBase*			m_pGroupSell;
	CUIBase*			m_pGroupBuy;

	CUIImage*			m_pImgUnmove;
	CUIImage*			m_pImgSelect;
};

#endif // UIPERSONALSHOP_H_


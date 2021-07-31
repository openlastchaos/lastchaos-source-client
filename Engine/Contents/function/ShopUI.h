#ifndef	SHOP_UI_H_
#define	SHOP_UI_H_

#include <Engine/Interface/UIWindow.h>

enum eSHOP_MODE
{
	eSHOP_NONE = -1,
	eSHOP_BUY = 0,
	eSHOP_SELL,
	eSHOP_LEASE
};

class CUIButton;
class CUIText;
class CUIImage;
class CUIArray;

typedef std::vector< CItems* > vec_items;

class CShopUI : public CUIWindow
{
public:
	CShopUI();
	~CShopUI();

	void PressOK();
	
	void CheckLease();
	void BuyItems();
	void SellItems();

	void AddShopItem( int invenIdx, int nUniIndex, SQUAD llCount );
	void DelShopItem( int invenIdx, int nUniIndex, SQUAD llCount, int nTradeItemID );

	void AddItemCallback();
	void DelItemCallback();

	void TradeToShop( SQUAD llCount );
	void ShopToTrade( SQUAD llCount );
	
	void openUI(int iMobIndex, int iMobVirIdx, FLOAT fX, FLOAT fZ, int nUIType);
	void closeUI();
	void resetUI();
	void updateUI();

	void initialize();

	void OnUpdate( float fDeltaTime, ULONG ElapsedTime );

	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonDBLClick(UINT16 x, UINT16 y);

	inline void SetFieldShopChack ( BOOL bChack)	{ m_bIsFieldShop = bChack; }
	void SetLeaseJob(int nJob)						{ m_nLeaseJob = nJob; }

	__int64	CalculateItemPrice(int iShopID, const CItemData &ID, int iNumOfItem, BOOL bSell);
	CItems*	GetTradeItem(int idx);
	int GetShopID() { return m_nSelectShopID; }
	BOOL IsBuyShop();
private:
	void _setPosUI();
	void _setNas(int nNasType, SQUAD llNas); // Nas Type : eTEXT_MY_NAS = 0 eTEXT_TRADE_NAS = 1 eTEXT_LEASE_NAS = 2,
	void _setText(int nTextType, CTString strContents);
	void _setSlotSelectIndex(int nSlotType, int nIndex);
	void _setBtnString(int nBtnType, CTString strString);
	void _setLeaseInfo(int nSlotIndex);

	void _updateBuy();
	void _updateSell();
	void _updateLease();
	
	void _resetSlot(int nSlotType); // Slot Type : eARRY_TRADE = 0,	eARRY_SELECT = 1,
	void _resetLeaseInfo();
	void clearTrade();

	CUIIcon* _getTradeItemSlot(int nSlotPos);
	CUIIcon* _getMyItemSlot(int nInvenIdx);
	int _getSlotSelectIndex(int nSlotType);

	bool _checkSendCondition();

	__int64	CalculateTotalPrice(int iShopID, int& iCount, BOOL bSell, BOOL bRate = TRUE);
	
	enum
	{
		eBTN_CLOSE = 0,
		eBTN_OK,
		eBTN_CANCEL,
		eBTN_MAX
	};

	CUIButton* m_pBtn[eBTN_MAX];

	enum
	{
		eIMG_TITLE = 0,
		eIMG_TRADE_SLOT,
		eIMG_SELECT_SLOT,
		eIMG_LEASE_INFO,
		eIMG_BOTTOM,
		eIMG_MAX
	};

	CUIImage* m_pImg[eIMG_MAX];

	enum
	{
		eARRY_TRADE = 0,
		eARRY_SELECT,
		eARRY_MAX
	};

	CUIArray* m_pArry[eARRY_MAX];

	enum
	{
		eTEXT_MY_NAS = 0,
		eTEXT_TRADE_NAS,
		eTEXT_LEASE_NAS,
		eTEXT_ITEM_NAME_TITLE,
		eTEXT_ITEM_NAME,
		eTEXT_ITEM_LEVEL,
		eTEXT_ITEM_JOB,
		eTEXT_ITEM_ATT,
		eTEXT_ITEM_MAGIC,
		eTEXT_ITEM_TIME,
		eTEXT_MAX
	};

	CUIText* m_pText[eTEXT_MAX];

	CUIScrollBar* m_pSelectScroll;

	enum
	{
		ePARTS_TOP = 0,
		ePARTS_BOTTOM,
		ePARTS_HIDE,
		ePARTS_MAX
	};

	vec_items m_vecTrade;

	FLOAT m_fNpcX, m_fNpcZ;
	int m_nUIParts[ePARTS_MAX];
	int m_iMobIdx;
	int m_iMobVirIdx;
	int m_nUIType;
	int m_nLeaseJob;
	int m_nSelectShopID;
	int	m_nSelShopItemID;
	int	m_nSelTradeItemID;
	int m_nNumOfItem;
	__int64 m_nTotalPrice;
	__int64 m_nSendTotalPrice;

	BOOL m_bIsFieldShop; // 잡화상 이용 주문서 사용시 체크
};

#endif	// SHOP_UI_H_


#ifndef	WARE_HOUSE_UI_H_
#define	WARE_HOUSE_UI_H_

#include <Engine/Interface/UIWindow.h>

class CUIButton;
class CUIImage;
class CUIIcon;
class CUIText;
class CUITextBox;

class CWareHouseUI : public CUIWindow
{
public:
	CWareHouseUI();
	~CWareHouseUI();

	void AddItemCallback();
	void DelItemCallback();
	void AddWareHouseItem(int nIndex);
	void DelWareHouseItem();

	// Command Link
	void PressFirstBtn();
	void PressSecondBtn();
	void WareIconMouseLDownEvent(int nSelIdx);
	void TradeIconMouseLDownEvent(int nSelIdx);
	void WareIconMouseDBLCLKEvent(int nSelIdx);
	void TradeIconMouseDBLCLKEvent(int nSelIdx);
	void WareSlotMouseLUPEvent();	
	void TradeSlotMouseLUPEvent();


	// WareHouse -> Inventory
	void AddWareHouseItemFromInventory();
	void DelWareHouseItemToInventory();

	void OpenWareHouse( SBYTE sbSetPW, bool bCashRemoteOpen = false );
	void openUI();
	void closeUI();
	void ResetUI();
	
	void initialize();

	void OnUpdate( float fDeltaTime, ULONG ElapsedTime );

	void MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void MsgBoxLCommand( int nCommandCode, int nResult );

	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);
	BOOL CloseWindowByEsc()	{ closeUI(); return TRUE; }

	// Set
	void SetNPCPos(int nNPCIdx, FLOAT fX, FLOAT fZ);

	void SetUseTime(int t)			{ m_useTime = t; updateUseTime(); }
	void SetHasPassword(bool bVal)	{ m_bHasPassword = bVal; }

	// Get
	int	 GetWareHouseItemIndex( int nInvenIdx );
	LONGLONG GetWareHouseItemCount( int nInvenIdx );

	bool GetUseCashRemote()			{ return m_bCashRemote; }
	float GetNpcPosX()				{ return m_fNpcPosX; }
	float GetNpcPosZ()				{ return m_fNpcPosZ; }
	inline bool HasPassword() const	{ return m_bHasPassword; }
	
	// Network functions
	void SendListReq(CTString& strPW);

	void ReceiveNas(CNetworkMessage *istr);
	void ReceiveList(CNetworkMessage *istr);

private:
	void KeepItems();
	void TakeItems();	
	void InNas();
	void OutNas();

	void updateUseTime();
	void PrepareWareHouse();
	void RefreshWareHouse();
	void TradeToWareHouse(SQUAD llCount = 1);
	void WareHouseToTrade(SQUAD llCount = 1);
	void ClearItems();
	void WareHouseCloseMessageBox();
	SQUAD CalculateStoragePrice();
	// Err Msg
	void ErrMessage( int nErr );	

	void SendWareHouseKeepReq();
	void SendWareHouseTakeReq();

	void SetUIString(int nStringType, CTString strContents, COLOR col = 0xF2F2F2FF);
	void SetUI();
	CUIIcon* GetTradeItemIcon(int nIdx);
	CUIIcon* GetWareHouseItemIcon(int nIdx);

	enum
	{
		eBTN_FIRST = 0,
		eBTN_SECOND,
		eBTN_CANCEL,
		eBTN_CLOSE,
		eBTN_MAX
	};

	CUIButton* m_pBtn[eBTN_MAX];

	enum
	{
		eTEXT_USETIME = 0,
		eTEXT_SLOTINFO,
		eTEXT_NASTITLE1,
		eTEXT_NASTITLE2,
		eTEXT_NAS1,
		eTEXT_NAS2,
		eTEXT_MAX
	};

	CUIText* m_pText[eTEXT_MAX];

	enum
	{
		eARRAY_WAREHOUSE_ITEM = 0,
		eARRAY_TRADE_ITEM,
		eARRAY_MAX
	};

	CUIArray* m_pArray[eARRAY_MAX];

	// drag
	CUIImage* m_pMoveArea;

	bool	m_bDrag;
	int		m_nOriX, m_nOriY;
	int		m_nMouseX;
	int		m_nMouseY;
	float	m_fNpcPosX, m_fNpcPosZ;
	int		m_nNpcIndex;
	int		m_nNPCVIdx;

	// Storage Items
	typedef		std::vector< CItems* >		vec_Items;
	typedef		vec_Items::iterator			vec_Items_iter;

	vec_Items m_vectorStorageItems;
	vec_Items m_vecTradeItems;
	CItems*	m_pItemsTemp;

	int		m_nTempStorageIdx;
	int		m_nTempTradeIdx;
	SQUAD	m_nTempMax;
	int		m_nSelTradeItemID;
	int		m_nSelWareHouseItemID;
	BOOL	m_bStorageWareHouse;
	bool	m_bCashRemote;		// 창고 원격 사용
	bool	m_bHasPassword;
	int		m_useTime; 			// cash item remain time
	__int64	m_nTotalPrice;
	__int64	m_nTotalNas;
	__int64	m_nInNas;			// 입금나스
	__int64	m_nOutNas;			// 출금나스
	CTString m_strPW;
};

#endif	// WARE_HOUSE_UI_H_


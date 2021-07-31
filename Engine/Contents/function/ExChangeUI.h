#ifndef __EXCHANGE_UI_H__
#define __EXCHANGE_UI_H__

#define	EXCH_EXCH_SLOT_TOTAL		(10)
#define	DEF_INVENTORY_MAX			(ITEM_COUNT_IN_INVENTORY_NORMAL + ITEM_COUNT_IN_INVENTORY_CASH_1 + ITEM_COUNT_IN_INVENTORY_CASH_2)

class CExchangeUI : public CUIWindow
{
public:
	CExchangeUI();
	~CExchangeUI();

	void initialize();
	void ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	void SendExchangeReq_Req( CTString &strSrcName );

	// Network message functions ( receive )
	void ExchangeReq_Req( int nSrcIndex, CTString &strSrcName, int nDestIndex, CTString &strDestName );
	void ExchangeReq_Rep();
	void ExchangeReq_Rej( BOOL bSrcReject );
	void ExchangeReq_Ready( BOOL bSrcReady );
	void ExchangeReq_BothReady();
	void ExchangeReq_Full( BOOL bSrcFull );
	void ExchangeReq_Ok( BOOL bSrcOk );
	void ExchangeReq_BothOk();
	void ExchangeReq_Pet_Err(BOOL petErr);

	void ExchangeItem_Add( BOOL bSrcAdd, int nUniIndex, SQUAD llCount,
		int nIndex, ULONG ulPlus, ULONG ulFlag, LONG ulUsed, LONG ulUsed2, LONG lRareIndex =0 );
	void SetDurability( BOOL bSrcAdd, int nUniIndex, LONG nDuraNow, LONG nDuraMax );
	void ExchangeItem_SetOptionData( BOOL bSrcAdd, int nUniIndex, SBYTE sbOption,
		SBYTE sbOptionType, LONG lOptionLevel, LONG lRareIndex, LONG lOriginOptionVar );
	void ExchangeItem_SetRareOption( CNetworkMessage *istr, BOOL bSrcAdd, int nUniIndex);
	void ExchangeItem_Del( BOOL bSrcDel, int nUniIndex );
	// by jeil Delete nas item
	void ExchangeItem_Update( BOOL bSrcUpdate, int nUniIndex, SQUAD llCount , SQUAD llNas = 0);

	void ResetExchange();

	void ExchangeItem_SetSkill( CNetworkMessage *istr, BOOL bSrcAdd, int nUniIndex);
	void ExchangeItem_InitSocket( BOOL bSrcAdd, int nUniIndex );
	void ExchangeItem_SetSocketData( BOOL bSrcAdd, int nUniIndex, SBYTE sbSocketCreateCount,
		SBYTE sbSlotIndex, LONG lJewelIndex );
	void ExchangeItem_SetPlus2( BOOL bSrcAdd, int nUniIndex, LONG plus2 );

	// Command functions
	void MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );

	//2013/04/03 jeil 나스 아이템 제거
	void ExchangeNas();

	void AddItemCallback();
	void OnUpdate( float fDeltaTime, ULONG ElapsedTime );

	void AddExchItem( int nTab, int inven_idx, int nUniIndex, SQUAD llCount );
	void DragItem();

	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);
private:
	void InitArrayItem(CUIArray* pArr, CUIIcon** pPtr, int nRow, int nCol);
	// Internal functions
	void PrepareExchange();
	void RenderItems();

	// Network message functions ( send )
	void SendExchangeReq_Rep();
	void SendExchangeReq_Rej();
	void SendExchangeReq_Ready();
	void SendExchangeReq_Ok();
	void SetNoExchangeImg(CUIIcon** ptr, int nCnt);

	//2013/04/09 jeil 나스 아이템 제거 
	void SendExchangeItem_Add( int nUniIndex, SQUAD llCount = 0 );

	CUIArray*	m_pArrUser;
	CUIArray*	m_pArrMy;
	CUIArray*	m_pArrInven;

	CUIButton*	m_pBtnReady;								// Ready button
	CUIButton*	m_pBtnExchange;							// Exchange button

	// Exchange items
	CUIIcon*	m_pIconsOppItems[EXCH_EXCH_SLOT_TOTAL];	// Items of opposite slot
	CUIIcon*	m_pIconsMyItems[EXCH_EXCH_SLOT_TOTAL];	// Items of my slot
	CUIIcon*	m_pIconsInvenItems[DEF_INVENTORY_MAX];	// Items of inventory slot

	CUIText*	m_pTxtUserGold;
	CUIText*	m_pTxtMyGold;
	CUIText*	m_pTxtInvenGold;
	CUIText*	m_pTxtTargetName;

	CUIImage*	m_pImgNoExchange;

	CTString	m_strSrcName;							// Source name
	CTString	m_strDestName;							// Destination name

	// Item information
	int	m_nSelMyItemID;							// Selected item ID of my slot
	int	m_nSelITab;
	int	m_nSelInvenItemID;						// Selected item ID of inventory slot
	int	m_nCurInfoLines;						// Count of current item information lines
	int	m_iRareGrade;
	int	m_nSrcIndex;							// Source index
	int	m_nDestIndex;							// Destination index
	__int64	m_nMyNas;
	__int64	m_nTradeNas;

	// Exchange information
	BOOL m_bAmISrc;								// If I am source or not
	BOOL m_bRareItem;
	BOOL m_bShowItemInfo;						// If item tool tip is shown or not
	BOOL m_bDetailItemInfo;						// If item informaion is shown in detail or not
	BOOL m_bExchReady;							// If exchange is ready or not
	BOOL m_bExchReadyDest;						// If exchange of target is ready or not
};
#endif // __EXCHANGE_UI_H__
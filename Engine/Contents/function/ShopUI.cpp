#include "stdh.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Contents/Base/UIMsgBoxNumeric_only.h>
#include "ShopUI.h"
#include <Engine/info/MyInfo.h>

class CmdShopAddItem : public Command
{
public:
	CmdShopAddItem() : m_pWnd(NULL)	{}
	void setData(CShopUI* pWnd)	{ m_pWnd = pWnd;	}
	void execute() 
	{
		if (m_pWnd != NULL)
			m_pWnd->AddItemCallback();
	}
private:
	CShopUI* m_pWnd;
};

class CmdShopDelItem : public Command
{
public:
	CmdShopDelItem() : m_pWnd(NULL)	{}
	void setData(CShopUI* pWnd)	{ m_pWnd = pWnd;	}
	void execute() 
	{
		if (m_pWnd != NULL)
			m_pWnd->DelItemCallback();
	}
private:
	CShopUI* m_pWnd;
};

extern INDEX g_iShowHelp1Icon;
extern INDEX g_iCountry;

#define SHOP_SLOT_SELL_MAX				(ITEM_COUNT_IN_INVENTORY_NORMAL + ITEM_COUNT_IN_INVENTORY_CASH_1 + ITEM_COUNT_IN_INVENTORY_CASH_2)
#define	SHOP_TRADE_SLOT_TOTAL			10
#define SHOP_SLOT_COL					5
#define SHOP_SLOT_ROW					4

CShopUI::CShopUI()
	: m_pSelectScroll(NULL)
	, m_nSelectShopID(-1)
	, m_nSelTradeItemID(-1)
	, m_nSelShopItemID(-1)
	, m_nTotalPrice(0)
	, m_nSendTotalPrice(0)
	, m_nNumOfItem(0)
	, m_bIsFieldShop(FALSE)
{
	int i;
	for(i = 0; i < eBTN_MAX; ++i)
		m_pBtn[i] = NULL;

	for(i = 0; i < eTEXT_MAX; ++i)
		m_pText[i] = NULL;
	
	for(i = 0; i < eIMG_MAX; ++i)
		m_pImg[i] = NULL;
	
	for(i = 0; i < eARRY_MAX; ++i)
		m_pArry[i] = NULL;

	setInherit(false);
}

CShopUI::~CShopUI()
{
	clearTrade();
}

static int		nTempIndex;
static int		nTempUniIndex;
static int		nTempTab;
static int		nTempIdx;
static SQUAD	llTempCount;
static int		nTempTradeItemID;

void CShopUI::BuyItems()
{
	// Buy nothing
	if( m_nSendTotalPrice < 0 )
		return;

	// Not enough money
	if(_pNetwork->MyCharacterInfo.money < m_nTotalPrice )
	{
		// Add system message
		CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( _S( 266, "소지금이 부족합니다." ), SYSMSG_ERROR );
		return;
	}

	if(m_nUIType == eSHOP_LEASE)
	{
		CUIIcon* pIcon = NULL;
		pIcon = _getMyItemSlot(m_nSelShopItemID);

		if (pIcon != NULL)
			_pNetwork->SendLeaseItem( pIcon->getIndex());
	}
	else
	{
		if(m_bIsFieldShop)
		{
			_pNetwork->FieldShopBuyItem(m_nNumOfItem, m_nSendTotalPrice);
			m_bIsFieldShop = FALSE;
		}
		else
			_pNetwork->BuyItem( m_nSelectShopID, m_nNumOfItem, m_nSendTotalPrice );
	}
}

void CShopUI::SellItems()
{
	// Sell nothing
	if( m_nSendTotalPrice < 0 )
		return;

	if (m_nNumOfItem <= 0)
		return;

	if(m_bIsFieldShop)
	{
		_pNetwork->FieldShopSellItem(m_nNumOfItem, m_nSendTotalPrice);
		m_bIsFieldShop = FALSE;
	}else
	{
		_pNetwork->SellItem( m_nSelectShopID, m_nNumOfItem, m_nSendTotalPrice );
	}
}

void CShopUI::PressOK()
{
	switch(m_nUIType)
	{
	case eSHOP_BUY:
		{
			if (_checkSendCondition() == true)
			{
				BuyItems();
				closeUI();
			}
		}		
		break;
	case eSHOP_LEASE:
		CheckLease();
		break;
	case eSHOP_SELL:
		{
			if (_checkSendCondition() == true)
			{
				SellItems();
				closeUI();
			}
		}
		break;
	}
}

void CShopUI::CheckLease()
{
	if(m_nSelShopItemID >= 0)
	{
		CUIManager* pUIMgr = UIMGR();
		CTString	strMessage;
		CUIMsgBox_Info	MsgBoxInfo;
		if (pUIMgr->DoesMessageBoxExist(MSGCMD_SHOP_LEASE_ITEM))
			pUIMgr->CloseMessageBox(MSGCMD_SHOP_LEASE_ITEM);

		MsgBoxInfo.SetMsgBoxInfo( _S( 263, "상점" ), UMBS_YESNO,	UI_SHOP, MSGCMD_SHOP_LEASE_ITEM );
		strMessage = _S(3080, "대여한 무기는 대여한 날로부터 1일간의 대여기간이 만료되면 자동으로 소멸됩니다.")
			+ _S(3081,"선택하신 무기를 대여하시겠습니까?");

		MsgBoxInfo.AddString( strMessage );
		pUIMgr->CreateMessageBox( MsgBoxInfo );	
	}
}

void CShopUI::AddShopItem( int invenIdx, int nUniIndex, SQUAD llCount )
{
	BOOL bBuyShop = IsBuyShop();

	if (bBuyShop == FALSE)
	{
		if (invenIdx >= (ITEM_COUNT_IN_INVENTORY_NORMAL + ITEM_COUNT_IN_INVENTORY_CASH_1))
		{
			nTempTab		= INVENTORY_TAB_CASH_2;
			nTempIdx		= invenIdx - (ITEM_COUNT_IN_INVENTORY_NORMAL + ITEM_COUNT_IN_INVENTORY_CASH_1);
		}
		else if (invenIdx >= ITEM_COUNT_IN_INVENTORY_NORMAL)
		{
			nTempTab		= INVENTORY_TAB_CASH_1;
			nTempIdx		= invenIdx - ITEM_COUNT_IN_INVENTORY_NORMAL;
		}
		else
		{
			nTempTab		= INVENTORY_TAB_NORMAL;
			nTempIdx		= invenIdx;
		}
	}
	else
	{
		nTempTab = 0;
		nTempIdx = invenIdx;
	}

	nTempUniIndex	= nUniIndex;
	llTempCount		= llCount;

	CUIIcon* pIcon = _getMyItemSlot(invenIdx);

	if (pIcon == NULL)
		return;

	CItems*	pItems	= pIcon->getItems();
	nTempIndex		= pItems->Item_Index;
	ULONG ulItemPlus= pItems->Item_Plus;

	CItemData*	pItemData = pItems->ItemData;
	const char* szItemName = pItemData->GetName();

	CUIManager* pUIManager = CUIManager::getSingleton();

	// Ask quantity
	if( ( bBuyShop && ( pItemData->GetFlag() & ITEM_FLAG_COUNT ) ) ||
		( !bBuyShop && ( pItemData->GetFlag() & ITEM_FLAG_COUNT ) && llTempCount > 1 ) )
	{
		CTString	strMessage;
		strMessage.PrintF( _S2( 150, szItemName, "몇 개의 %s<를> 옮기시겠습니까?" ), szItemName );

		CmdShopAddItem* pCmd = new CmdShopAddItem;
		pCmd->setData(this);

		int nMax = 0;

		if( !bBuyShop )
			nMax = pItems->Item_Sum;
		else
			nMax = pItemData->GetStack();

		UIMGR()->GetMsgBoxNumOnly()->SetInfo(pCmd, _S( 263, "상점" ), strMessage, 1, nMax);
	}
	else if( ulItemPlus > 0 && !(pItemData->GetType() == CItemData::ITEM_ETC && (pItemData->GetSubType() == CItemData::ITEM_ETC_MONSTER_MERCENARY_CARD || nTempIndex == 6941) ) )
	{
		CTString	strMessage;
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 263, "상점" ), UMBS_YESNO,
			UI_SHOP, MSGCMD_SHOP_ADD_PLUSITEM );

		strMessage.PrintF( _S2( 264, szItemName, "[%s +%d]<를> 옮기시겠습니까?" ), szItemName, ulItemPlus );
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );
	}
	else
	{
		ShopToTrade( llTempCount );
	}
}

// ----------------------------------------------------------------------------
// Name : DelShopItem()
// Desc : From trade to shop
// ----------------------------------------------------------------------------
void CShopUI::DelShopItem( int invenIdx, int nUniIndex, SQUAD llCount, int nTradeItemID )
{
	if (nTradeItemID < 0)
		return;

	nTempIdx		= invenIdx;
	nTempUniIndex	= nUniIndex;
	llTempCount		= llCount;
	nTempTradeItemID= nTradeItemID;

	CUIIcon* pIcon = _getTradeItemSlot(nTempTradeItemID);

	if (pIcon == NULL)
		return;

	nTempIndex		= pIcon->getIndex();
	CItemData* pItemData = _pNetwork->GetItemData( nTempIndex );

	// Ask quantity
	if( llTempCount > 1 )
	{
		CUIManager* pUIManager = CUIManager::getSingleton();

		CTString	strMessage;
		const char	*szItemName = _pNetwork->GetItemName( nTempIndex );
		strMessage.PrintF( _S2( 150, szItemName, "몇 개의 %s<를> 옮기시겠습니까?" ), szItemName );

		CmdShopDelItem* pCmd = new CmdShopDelItem;
		pCmd->setData(this);
		UIMGR()->GetMsgBoxNumOnly()->SetInfo(pCmd, _S( 263, "상점" ), strMessage, 1, llTempCount);
	}
	else
	{
		TradeToShop( llTempCount );
	}
}

void CShopUI::AddItemCallback()
{
	if (UIMGR()->GetMsgBoxNumOnly()->GetData() > 0)
		ShopToTrade( UIMGR()->GetMsgBoxNumOnly()->GetData() );
}

void CShopUI::DelItemCallback()
{
	if (UIMGR()->GetMsgBoxNumOnly()->GetData() > 0)
		TradeToShop( UIMGR()->GetMsgBoxNumOnly()->GetData() );
}

void CShopUI::ShopToTrade( SQUAD llCount )
{
	BOOL bBuyShop = IsBuyShop();
	int nSelectedShopID = _getSlotSelectIndex(eARRY_TRADE);

	if( bBuyShop )
	{
		CItemData	*pItemData = _pNetwork->GetItemData( nTempIndex );

		MyInfo* pInfo = MY_INFO();
		__int64 llitemPrice = CalculateItemPrice( m_nSelectShopID, *pItemData, llCount, bBuyShop );
		__int64 iAddPrice = pInfo->CalcPriceRate(llitemPrice, bBuyShop) + pInfo->CalcPriceAddition(bBuyShop);

		SQUAD	llPrice = m_nTotalPrice + llitemPrice + iAddPrice;

		// Not enough money
		if(_pNetwork->MyCharacterInfo.money < llPrice )
		{
			// Add system message
			CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( _S( 266, "소지금이 부족합니다." ), SYSMSG_ERROR );
			return;
		}

		const int iFame = pItemData->GetFame();

		// Not enough fame
		if( iFame > 0 && iFame > _pNetwork->MyCharacterInfo.fame )
		{
			// Add system message
			CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( _S( 1217, "명성치가 부족합니다." ), SYSMSG_ERROR );		
			return;
		}
	}

	// Find same item in trade slot
	int iItem;
	CUIIcon* pIcon = NULL;
	CUIIcon* pIconShop = NULL;

	if (bBuyShop)
	{
		for( iItem = 0; iItem < SHOP_TRADE_SLOT_TOTAL; iItem++ )
		{
			pIcon = _getTradeItemSlot(iItem);

			if (pIcon && pIcon->getIndex() == nTempIndex)
			{
				CItemData	*pItemData = _pNetwork->GetItemData( nTempIndex );

				if (pIcon->getItems() && pIcon->getItems()->Item_Sum + llCount <= pItemData->GetStack())
					break;
			}
		}
	}
	else
	{
		for( iItem = 0; iItem < SHOP_TRADE_SLOT_TOTAL; iItem++ )
		{
			pIcon = _getTradeItemSlot(iItem);

			if (pIcon && pIcon->getItems() != NULL &&  
				pIcon->getItems()->Item_UniIndex == nTempUniIndex)
			{
				CItemData	*pItemData = _pNetwork->GetItemData( nTempIndex );

				if (pIcon->getItems()->Item_Sum + llCount <= pItemData->GetStack())
					break;
			}
		}
	}

	// If there is same item
	if( iItem < SHOP_TRADE_SLOT_TOTAL )
	{
		// Check if item is countable
		CItemData*	pItemData = _pNetwork->GetItemData( nTempIndex );
		if( pItemData->GetFlag() & ITEM_FLAG_COUNT )
		{
			// Update count of trade item
			pIcon = _getTradeItemSlot(iItem);
			SQUAD	llNewCount = 0;

			if (pIcon && pIcon->getItems())
			{
				llNewCount = pIcon->getItems()->Item_Sum;
				llNewCount += llCount;
				pIcon->setCount(llNewCount);
			}			

			if( !bBuyShop )
			{
				if (nTempTab > INVENTORY_TAB_NORMAL)
					nTempIdx += ITEM_COUNT_IN_INVENTORY_NORMAL;

				if (nTempTab > INVENTORY_TAB_CASH_1)
					nTempIdx += ITEM_COUNT_IN_INVENTORY_CASH_1;

				pIconShop = _getMyItemSlot(nTempIdx);

				if (pIconShop && pIconShop->getItems())
				{
					// Update count of shop item
					llNewCount = pIconShop->getItems()->Item_Sum;
					llNewCount -= llCount;

					pIconShop->setCount(llNewCount);

					if( llNewCount <= 0 )
					{
						pIconShop->Hide(TRUE);
						if (m_nSelShopItemID == nTempIdx)
						{
							_setSlotSelectIndex(eARRY_SELECT, -1);
						}
					}
				}			
			}

			m_nTotalPrice = CalculateTotalPrice( m_nSelectShopID, m_nNumOfItem, bBuyShop );
			m_nSendTotalPrice = CalculateTotalPrice( m_nSelectShopID, m_nNumOfItem, bBuyShop, FALSE );
			_setNas(eTEXT_TRADE_NAS, m_nTotalPrice);
			return;
		}
	}

	// Find empty slot
	for( iItem = 0; iItem < SHOP_TRADE_SLOT_TOTAL; iItem++ )
	{
		pIcon = _getTradeItemSlot(iItem);

		if (pIcon && pIcon->IsEmpty())
			break;
	}

	// If there is not empty slot
	if( iItem == SHOP_TRADE_SLOT_TOTAL )
	{
		// Add system message
		if( bBuyShop )
			CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( _S( 267, "최대 10개의 아이템을 구매할 수 있습니다." ), SYSMSG_ERROR );
		else
			CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( _S( 268, "최대 10개의 아이템을 판매할 수 있습니다." ), SYSMSG_ERROR );

		return;
	}

	if (bBuyShop == FALSE)
	{
		if (nTempTab > INVENTORY_TAB_NORMAL)
			nTempIdx += ITEM_COUNT_IN_INVENTORY_NORMAL;

		if (nTempTab >= INVENTORY_TAB_CASH_2)
			nTempIdx += ITEM_COUNT_IN_INVENTORY_CASH_1;
	}

	CUIManager* pUIManager = CUIManager::getSingleton();	

	pIconShop = _getMyItemSlot(nTempIdx);
	pIcon = _getTradeItemSlot(iItem);

	CItems* pCopy = new CItems;
	memcpy(pCopy, pIconShop->getItems(), sizeof(CItems));
	m_vecTrade.push_back(pCopy);

	pIcon->setData(m_vecTrade[iItem]);	
	pIcon->setCount(llCount);
	pIcon->SetWhichUI(UI_SHOP);

	m_nTotalPrice = CalculateTotalPrice( m_nSelectShopID, m_nNumOfItem, bBuyShop );
	m_nSendTotalPrice = CalculateTotalPrice( m_nSelectShopID, m_nNumOfItem, bBuyShop, FALSE );
	_setNas(eTEXT_TRADE_NAS, m_nTotalPrice);

	if (bBuyShop == FALSE)
	{
		// Update count of shop item
		SQUAD	llNewCount = pIconShop->getItems()->Item_Sum;
		llNewCount -= llCount;

		pIconShop->setCount(llNewCount);

		if( llNewCount <= 0 )
			pIconShop->Hide(TRUE);
	}
}

void CShopUI::TradeToShop( SQUAD llCount )
{
	// Update count of trade item

	CUIIcon* pIcon = _getTradeItemSlot(nTempTradeItemID);
	CUIIcon* pIconTemp = NULL;
	SQUAD	llNewCount = 0;

	if (pIcon && pIcon->getItems())
	{
		llNewCount = pIcon->getItems()->Item_Sum;
		llNewCount -= llCount;
		pIcon->setCount(llNewCount);
	}
		
	if( llNewCount <= 0 )
	{
		// Rearrange buttons
		for( int iArrItem = nTempTradeItemID; iArrItem < SHOP_TRADE_SLOT_TOTAL; iArrItem++ )
		{
			pIcon = _getTradeItemSlot(iArrItem);

			if (pIcon != NULL)
			{
				if ((iArrItem + 1) == SHOP_TRADE_SLOT_TOTAL)
				{
					pIcon->clearIconData();
					break;
				}

				pIconTemp = _getTradeItemSlot(iArrItem + 1);

				if (pIconTemp && pIconTemp->IsEmpty())
				{
					pIcon->clearIconData();
					break;
				}

				pIcon->setData(m_vecTrade[iArrItem + 1]);
			}			
		}

		SAFE_DELETE(m_vecTrade[nTempTradeItemID]);
		m_vecTrade.erase(m_vecTrade.begin() + nTempTradeItemID);

		// Unselect item
		if( nTempTradeItemID == m_nSelTradeItemID )
			_setSlotSelectIndex(eARRY_TRADE, -1);
	}

	BOOL bBuyShop = IsBuyShop();

	if( bBuyShop == FALSE )
	{
		pIcon = _getMyItemSlot(nTempIdx);

		if (pIcon != NULL)
		{
			pIcon->Hide(FALSE);
			// Update count of shop item
			llNewCount = pIcon->getItems()->Item_Sum;
			llNewCount += llCount;
			pIcon->setCount(llNewCount);
		}
	}

	m_nTotalPrice = CalculateTotalPrice( m_nSelectShopID, m_nNumOfItem, bBuyShop );
	m_nSendTotalPrice = CalculateTotalPrice( m_nSelectShopID, m_nNumOfItem, bBuyShop, FALSE );
	_setNas(eTEXT_TRADE_NAS, m_nTotalPrice);
}

void CShopUI::openUI(int iMobIndex, int iMobVirIdx, FLOAT fX, FLOAT fZ, int nUIType)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if (pUIManager->GetInventory()->IsLocked() == TRUE ||
		pUIManager->GetInventory()->IsLockedArrange() == TRUE)
	{
		pUIManager->GetInventory()->ShowLockErrorMessage();
		return;
	}

	if (GetHide() == FALSE)
		return;

	if (nUIType <= eSHOP_NONE || nUIType > eSHOP_LEASE)
		return;

	pUIManager->GetInventory()->Lock( TRUE, FALSE, LOCK_SHOP );
	pUIManager->SetCSFlagOn( CSF_SHOP );

	Hide(FALSE);
	SetVisible(TRUE);

	m_nUIType = nUIType;
	m_fNpcX = fX;
	m_fNpcZ = fZ;
	m_iMobIdx = iMobIndex;
	m_iMobVirIdx = iMobVirIdx;

	CMobData* MD = CMobData::getData(m_iMobIdx);
	CShopData &SD = _pNetwork->GetShopData(m_iMobIdx);	
	m_nSelectShopID = SD.GetIndex();

	_setPosUI();
	updateUI();

	pUIManager->RearrangeOrder( UI_SHOP_UI, TRUE );
}

void CShopUI::closeUI()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	Hide(TRUE);
	SetVisible(FALSE);
	resetUI();

	pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_SHOP );
	pUIManager->CloseMessageBox( MSGCMD_SHOP_ADD_PLUSITEM );
	pUIManager->GetMsgBoxNumOnly()->CloseBox();
	pUIManager->RearrangeOrder( UI_SHOP_UI, FALSE );
}

void CShopUI::resetUI()
{
	m_fNpcX = 0.0f;
	m_fNpcZ = 0.0f;
	m_iMobIdx = -1;
	m_iMobVirIdx = -1;
	m_nSelectShopID = -1;
	m_nSelTradeItemID = -1;
	m_nSelShopItemID = -1;
	m_nUIType = eSHOP_NONE;
	m_nLeaseJob = -1;
	m_nTotalPrice = 0;
	m_nSendTotalPrice = 0;
	m_nNumOfItem = 0;

	int i;
	for (i = 0; i < ePARTS_MAX; ++i)
		m_nUIParts[i] = -1;

	_resetSlot(eARRY_SELECT);
	_resetSlot(eARRY_TRADE);
	_resetLeaseInfo();
	_setNas(eTEXT_TRADE_NAS, 0);
	clearTrade();

	UIMGR()->SetCSFlagOff( CSF_SHOP );
}

void CShopUI::updateUI()
{
	// Set Money
	_setNas(eTEXT_MY_NAS, _pNetwork->MyCharacterInfo.money);
	CTString strBtnString;

	switch(m_nUIType)
	{
	case eSHOP_BUY:
		_updateBuy();
		strBtnString.PrintF(_S(253, "구매"));
		break;

	case eSHOP_SELL:
		_updateSell();
		strBtnString.PrintF(_S(254, "판매"));
		break;

	case eSHOP_LEASE:
		_updateLease();
		strBtnString.PrintF(_S(3046, "대여"));
		break;

	default:
		return;
	}

	_setBtnString(eBTN_OK, strBtnString);
}

void CShopUI::initialize()
{
#ifndef		WORLD_EDITOR
	// Image
	std::string strImgID[eIMG_MAX] = { "img_title", "img_mySlot", "img_selectSlot", "img_lease", "img_bottom" };

	for(int i = 0; i < eIMG_MAX; ++i)
		m_pImg[i] = (CUIImage*)findUI(strImgID[i].c_str());
	
	if (m_pImg[eIMG_TITLE] != NULL)
	{
		int l, t;
		l = m_pImg[eIMG_TITLE]->GetPosX();
		t = m_pImg[eIMG_TITLE]->GetPosY();
		setTitleRect(l, t, l + m_pImg[eIMG_TITLE]->GetWidth(), t + m_pImg[eIMG_TITLE]->GetHeight());
	}

	std::string strBtnID[eBTN_MAX] = { "btn_close", "btn_ok", "btn_cancel"};

	for(int i = 0; i < eBTN_MAX; ++i)
		m_pBtn[i] = (CUIButton*)findUI(strBtnID[i].c_str());

	if (m_pBtn[eBTN_CLOSE] != NULL)
		m_pBtn[eBTN_CLOSE]->SetCommandFUp(boost::bind(&CShopUI::closeUI, this));

	if (m_pBtn[eBTN_CANCEL] != NULL)
		m_pBtn[eBTN_CANCEL]->SetCommandFUp(boost::bind(&CShopUI::closeUI, this));

	if (m_pBtn[eBTN_OK] != NULL)
		m_pBtn[eBTN_OK]->SetCommandFUp(boost::bind(&CShopUI::PressOK, this));

	std::string strTextID[eTEXT_MAX] = { "text_myNas", "text_tradeNas", "text_lease", "text_name",
	"text_nameVal", "text_level", "text_job", "text_att", "text_matt", "text_time" };

	for(int i = 0; i < eTEXT_MAX; ++i)
		m_pText[i] = (CUIText*)findUI(strTextID[i].c_str());

	std::string strArryID[eARRY_MAX] = { "arry_Trade", "arry_Select" };

	for(int i = 0; i < eARRY_MAX; ++i)
		m_pArry[i] = (CUIArray*)findUI(strArryID[i].c_str());

	if (m_pArry[eARRY_TRADE] != NULL)
	{
		int nSize = m_pArry[eARRY_TRADE]->GetArrayChildCount();
		
		CUIIcon* pIcon = NULL;

		for(int i = 0; i < nSize; ++i)
		{
			pIcon = _getTradeItemSlot(i);
			
			if (pIcon == NULL)
				continue;

			CmdDragIcon* pCmd = new CmdDragIcon;
			pCmd->setData(pIcon);
			pIcon->SetCommandDrag(pCmd);
			pIcon->SetWhichUI(UI_SHOP);
		}
	}

	m_pSelectScroll = (CUIScrollBar*)findUI("scroll_select");

#endif	//	WORLD_EDITOR
}

WMSG_RESULT CShopUI::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;


	CUIIcon* pIcon = NULL;
	int i, nIconCount;
	if (m_pArry[eARRY_TRADE] && m_pArry[eARRY_TRADE]->IsInside(x, y) && m_pArry[eARRY_TRADE]->GetHide() == FALSE)
	{
		m_nSelShopItemID = -1;
		m_nSelTradeItemID = -1;

		nIconCount = m_pArry[eARRY_TRADE]->getChildCount();
		for (i = 0; i < nIconCount; ++i)
		{
			pIcon = _getTradeItemSlot(i);

			if (pIcon != NULL)
			{
				if (pIcon->IsInside(x, y))
				{
					m_nSelTradeItemID = i;
					break;
				}
			}
		}
	}
	else if (m_pArry[eARRY_SELECT] && m_pArry[eARRY_SELECT]->IsInside(x, y) && m_pArry[eARRY_SELECT]->GetHide() == FALSE)
	{
		m_nSelShopItemID = -1;
		m_nSelTradeItemID = -1;

		nIconCount = m_pArry[eARRY_SELECT]->getChildCount();
		for (i = 0; i < nIconCount; ++i)
		{
			pIcon = _getMyItemSlot(i);

			if (pIcon != NULL)
			{
				if (pIcon->IsInside(x, y))
				{
					m_nSelShopItemID = i;
					break;
				}
			}
		}
	}

	_setSlotSelectIndex(eARRY_SELECT, m_nSelShopItemID);
	_setSlotSelectIndex(eARRY_TRADE, m_nSelTradeItemID);
	UIMGR()->RearrangeOrder( UI_SHOP_UI, TRUE );
	return CUIWindow::OnLButtonDown(x, y);
}

WMSG_RESULT CShopUI::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	CUIManager* pUIMgr = UIMGR();

	CUIIcon* pDragIcon = pUIMgr->GetDragIcon();

	if (pDragIcon != NULL && pDragIcon->IsEmpty() == false &&
		pDragIcon->getBtnType() == UBET_ITEM && pDragIcon->GetWhichUI() == UI_SHOP)
	{
		CItems* pItems = pDragIcon->getItems();
		CUIIcon* pIcon = NULL;

		if (pItems != NULL)
		{
			if (m_pArry[eARRY_TRADE] && m_pArry[eARRY_TRADE]->IsInside(x, y) && m_pArry[eARRY_TRADE]->GetHide() == FALSE)
			{
				pIcon = _getTradeItemSlot(m_nSelTradeItemID);

				if (m_nSelTradeItemID < 0 || (pIcon != NULL && pIcon->getItems() != NULL && pIcon->getItems()->Item_UniIndex != pItems->Item_UniIndex))
				{
					int		nTab = 0;

					if (pItems->Item_Tab > INVENTORY_TAB_NORMAL)
						nTab += ITEM_COUNT_IN_INVENTORY_NORMAL;
					if (pItems->Item_Tab > INVENTORY_TAB_CASH_1)
						nTab += ITEM_COUNT_IN_INVENTORY_CASH_1;

					AddShopItem(pItems->InvenIndex + nTab, pItems->Item_UniIndex, pItems->Item_Sum);

					// Reset holding button
					pUIMgr->ResetHoldBtn();
					return WMSG_SUCCESS;
				}
			}
			else if (m_pArry[eARRY_SELECT] && m_pArry[eARRY_SELECT]->IsInside(x, y) && m_pArry[eARRY_SELECT]->GetHide() == FALSE)
			{
				pIcon = _getMyItemSlot(m_nSelShopItemID);

				if (m_nSelShopItemID < 0 || (pIcon && pIcon->getItems() && pIcon->getItems()->Item_UniIndex != pItems->Item_UniIndex))
				{
					int nInvenIdx = pItems->InvenIndex;

					if (pItems->Item_Tab > INVENTORY_TAB_NORMAL)
					{
						nInvenIdx += ITEM_COUNT_IN_INVENTORY_NORMAL;
					}

					if (pItems->Item_Tab > INVENTORY_TAB_CASH_1)
					{
						nInvenIdx += ITEM_COUNT_IN_INVENTORY_CASH_1;
					}

					DelShopItem(nInvenIdx, pItems->Item_UniIndex, 
						pItems->Item_Sum, m_nSelTradeItemID );

					// Reset holding button
					pUIMgr->ResetHoldBtn();
					return WMSG_SUCCESS;
				}
			}
		}		
	}

	pUIMgr->ResetHoldBtn();
	return CUIWindow::OnLButtonUp(x, y);
}

WMSG_RESULT CShopUI::OnLButtonDBLClick( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	CUIManager* pUIMgr = UIMGR();
	CUIIcon* pIcon = NULL;

	if (m_pArry[eARRY_TRADE] && m_pArry[eARRY_TRADE]->IsInside(x, y) && m_pArry[eARRY_TRADE]->GetHide() == FALSE)
	{
		_setSlotSelectIndex(eARRY_TRADE, -1);

		for( int iItem = 0; iItem < SHOP_TRADE_SLOT_TOTAL; iItem++ )
		{
			pIcon = _getTradeItemSlot(iItem);

			if (pIcon != NULL && pIcon->IsInside(x, y) == TRUE)
			{
				if (pIcon->getItems() != NULL)					
				{
					// Close message box of shop
					pUIMgr->CloseMessageBox( MSGCMD_SHOP_ADD_PLUSITEM );
					pUIMgr->GetMsgBoxNumOnly()->CloseBox();

					CItems* pItems = pIcon->getItems();

					int nInvenIdx = pItems->InvenIndex;

					if (pItems->Item_Tab > INVENTORY_TAB_NORMAL)
						nInvenIdx += ITEM_COUNT_IN_INVENTORY_NORMAL;

					if (pItems->Item_Tab > INVENTORY_TAB_CASH_1)
						nInvenIdx += ITEM_COUNT_IN_INVENTORY_CASH_1;

					DelShopItem( nInvenIdx, pItems->Item_UniIndex,
						pItems->Item_Sum, iItem );					
				}
				return WMSG_SUCCESS;
			}
		}
	}
	else if (m_pArry[eARRY_SELECT] && m_pArry[eARRY_SELECT]->IsInside(x, y) && m_pArry[eARRY_SELECT]->GetHide() == FALSE)
	{
		_setSlotSelectIndex(eARRY_SELECT, -1);
		
		if (m_pSelectScroll == NULL)
			return WMSG_SUCCESS;

		int nCount = m_pSelectScroll->GetScrollPos();

		int iRowS = nCount * SHOP_SLOT_COL;
		int iRowE = (nCount + SHOP_SLOT_ROW) * SHOP_SLOT_COL;

		for( int i = iRowS; i < iRowE; ++i )
		{
			pIcon = _getMyItemSlot(i);

			if (pIcon != NULL && pIcon->IsInside(x, y) == TRUE)
			{
				if (pIcon->GetHide() == TRUE)
					return WMSG_SUCCESS;

				if(IsBuyShop() == TRUE)
				{
					// Close message box of shop
					pUIMgr->CloseMessageBox( MSGCMD_SHOP_ADD_PLUSITEM );
					pUIMgr->GetMsgBoxNumOnly()->CloseBox();

					if (pIcon->getItems() == NULL)
						return WMSG_FAIL;

					AddShopItem(i, pIcon->getItems()->Item_UniIndex,
						pIcon->getItems()->Item_Sum);
				}
				else
				{
					int		t, idx;

					if (i >= (ITEM_COUNT_IN_INVENTORY_NORMAL + ITEM_COUNT_IN_INVENTORY_CASH_1))
					{
						t = INVENTORY_TAB_CASH_2;
						idx = i - (ITEM_COUNT_IN_INVENTORY_NORMAL + ITEM_COUNT_IN_INVENTORY_CASH_1);
					}
					else if (i >= ITEM_COUNT_IN_INVENTORY_NORMAL)
					{
						t = INVENTORY_TAB_CASH_1;
						idx = i - ITEM_COUNT_IN_INVENTORY_NORMAL;
					}
					else
					{
						t = INVENTORY_TAB_NORMAL;
						idx = i;
					}

					// Not wearing, not refine stone, can trade
					CItems		*pItems = &_pNetwork->MySlotItem[t][idx];
					CItemData	*pItemData = pItems->ItemData;

					if (pItemData == NULL)
						continue;
#ifdef ADD_SUBJOB
					if( pItemData->IsFlag( ITEM_FLAG_SELLER ) ? 
						pUIMgr->CheckSellerItem(UI_SHOP, pItemData->GetFlag()) : 
					pItems->Item_Wearing == -1 && ( pItemData->GetType() != CItemData::ITEM_ETC ||
						pItemData->GetSubType() != CItemData::ITEM_ETC_REFINE ) &&
						pItemData->GetFlag() & ITEM_FLAG_TRADE && !pItems->IsFlag(FLAG_ITEM_LENT) )
#else
					if( pItems->Item_Wearing == -1 &&
						( pItemData->GetType() != CItemData::ITEM_ETC || pItemData->GetSubType() != CItemData::ITEM_ETC_REFINE ) &&
						pItemData->GetFlag() & ITEM_FLAG_TRADE && !pItems->IsFlag(FLAG_ITEM_LENT))
#endif
					{
						// Close message box of shop
						pUIMgr->CloseMessageBox( MSGCMD_SHOP_ADD_PLUSITEM );
						pUIMgr->GetMsgBoxNumOnly()->CloseBox();

						if (pIcon->getItems()->GetToggle() == true)
							return WMSG_FAIL;

						AddShopItem(i, pIcon->getItems()->Item_UniIndex,
							pIcon->getItems()->Item_Sum);
					}												
				}
				return WMSG_SUCCESS;
			}
		}
	}

	return WMSG_FAIL;
}

void CShopUI::OnUpdate( float fDeltaTime, ULONG ElapsedTime )
{
	if (GetHide() == TRUE)
		return;

	FLOAT	fDiffX = _pNetwork->MyCharacterInfo.x - m_fNpcX;
	FLOAT	fDiffZ = _pNetwork->MyCharacterInfo.z - m_fNpcZ;

	if( fDiffX * fDiffX + fDiffZ * fDiffZ > UI_VALID_SQRDIST)
		closeUI();

	CUIIcon* pIcon = NULL;
	int i, count;
	
	if (m_pArry[eARRY_TRADE] != NULL && m_pArry[eARRY_TRADE]->GetHide() == FALSE)
	{
		count = m_pArry[eARRY_TRADE]->GetArrayChildCount();

		for (i = 0; i < count; ++i)
		{
			pIcon = _getTradeItemSlot(i);

			if (pIcon == NULL || pIcon->IsEmpty() == TRUE)
				continue;

			pIcon->Update(fDeltaTime, ElapsedTime);
		}
	}

	if (m_pArry[eARRY_SELECT] != NULL && m_pArry[eARRY_SELECT]->GetHide() == FALSE)
	{
		count = m_pArry[eARRY_SELECT]->GetArrayChildCount();

		for (i = 0; i < count; ++i)
		{
			pIcon = _getMyItemSlot(i);

			if (pIcon == NULL || pIcon->IsEmpty() == TRUE)
				continue;

			pIcon->Update(fDeltaTime, ElapsedTime);
		}
	}
}

void CShopUI::_setPosUI()
{
	if (GetHide() == TRUE)
		return;

	switch(m_nUIType)
	{
	case eSHOP_BUY:
		{
			m_nUIParts[ePARTS_TOP] = eIMG_SELECT_SLOT;
			m_nUIParts[ePARTS_BOTTOM] = eIMG_TRADE_SLOT;
			m_nUIParts[ePARTS_HIDE] = eIMG_LEASE_INFO;
		}
		break;

	case eSHOP_SELL:
		{
			m_nUIParts[ePARTS_TOP] = eIMG_TRADE_SLOT;
			m_nUIParts[ePARTS_BOTTOM] = eIMG_SELECT_SLOT;
			m_nUIParts[ePARTS_HIDE] = eIMG_LEASE_INFO;
		}
		break;

	case eSHOP_LEASE:
		{
			m_nUIParts[ePARTS_TOP] = eIMG_SELECT_SLOT;
			m_nUIParts[ePARTS_BOTTOM] = eIMG_LEASE_INFO;
			m_nUIParts[ePARTS_HIDE] = eIMG_TRADE_SLOT;
		}
		break;

	default:
		return;
	}

	int i;
	for (i = 0; i < ePARTS_MAX; ++i)
	{
		if (m_pImg[m_nUIParts[i]] == NULL)
			return;
	}

	if (m_pImg[eIMG_TITLE] == NULL || m_pImg[eIMG_BOTTOM] == NULL)
		return;

	int nPosY;
	nPosY = m_pImg[eIMG_TITLE]->GetHeight();
	m_pImg[m_nUIParts[ePARTS_TOP]]->SetPosY(nPosY);
	m_pImg[m_nUIParts[ePARTS_TOP]]->Hide(FALSE);

	nPosY += m_pImg[m_nUIParts[ePARTS_TOP]]->GetHeight();
	m_pImg[m_nUIParts[ePARTS_BOTTOM]]->SetPosY(nPosY);
	m_pImg[m_nUIParts[ePARTS_BOTTOM]]->Hide(FALSE);

	nPosY += m_pImg[m_nUIParts[ePARTS_BOTTOM]]->GetHeight();
	m_pImg[eIMG_BOTTOM]->SetPosY(nPosY);

	m_pImg[m_nUIParts[ePARTS_HIDE]]->Hide(TRUE);

	nPosY += m_pImg[eIMG_BOTTOM]->GetHeight();
	SetHeight(nPosY);
}

void CShopUI::_setText( int nTextType, CTString strContents)
{
	if (m_pText[nTextType] == NULL)
		return;

	m_pText[nTextType]->SetText(strContents);
}

void CShopUI::_setNas( int nNasType, SQUAD llNas )
{
	if (nNasType < eTEXT_MY_NAS || nNasType > eTEXT_LEASE_NAS)
		return;

	if (m_pText[nNasType] != NULL)
	{
		CUIManager* pUIMgr = UIMGR();
		m_pText[nNasType]->SetText(pUIMgr->IntegerToCommaString(llNas));
		m_pText[nNasType]->setFontColor(pUIMgr->GetNasColor(llNas));
	}
}

void CShopUI::_updateBuy()
{
	if (m_pArry[eARRY_SELECT] == NULL)
		return;

	if (m_pArry[eARRY_TRADE] == NULL)
		return;

	m_pArry[eARRY_TRADE]->Hide(FALSE);

	_resetSlot(eARRY_SELECT);
	_resetSlot(eARRY_TRADE);

	_setNas(eTEXT_TRADE_NAS, 0);
		
	// Set Shop Item
	CMobData* MD = CMobData::getData(m_iMobIdx);
	CShopData &SD = _pNetwork->GetShopData(m_iMobIdx);	

	if(m_nSelectShopID == -1)	
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	CUIArrayItem* pTempItem = m_pArry[eARRY_SELECT]->GetArrayItemTemplate();

	if (pTempItem == NULL)
		return;

	CUIArrayItem* pItem = NULL;
	CUIIcon* pIcon = NULL;

	int t;
	for(t = 0; t < SD.m_iNumOfItem; ++t)
	{
		m_pArry[eARRY_SELECT]->AddArrayItem((CUIArrayItem*)pTempItem->Clone());
		pItem = m_pArry[eARRY_SELECT]->GetArrayItem(t);

		if (pItem == NULL)
			continue;

		pIcon = (CUIIcon*)pItem->findUI("icon_item");

		if (pIcon == NULL)
			continue;

		int iItemIndex = SD.m_vectorSellItems[t];
		CItemData* pID = _pNetwork->GetItemData(iItemIndex);

		if(pID->GetItemIndex() == -1)
			continue;

		CItems* pItems = new CItems;
		pItems->Item_Index = iItemIndex;
		pItems->InvenIndex = t;
		pItems->ItemData = pID;
		pIcon->setData(pItems, false);
		pIcon->SetWhichUI(UI_SHOP);
		pIcon->setCount(1);

		CmdDragIcon* pCmd = new CmdDragIcon;
		pCmd->setData(pIcon);
		pIcon->SetCommandDrag(pCmd);
	}

	m_pArry[eARRY_SELECT]->SetSelectIdx(-1);
	m_pArry[eARRY_SELECT]->UpdateItem();
}

void CShopUI::_updateSell()
{
	if (m_pArry[eARRY_SELECT] == NULL)
		return;

	if (m_pArry[eARRY_TRADE] == NULL)
		return;

	m_pArry[eARRY_TRADE]->Hide(FALSE);

	_resetSlot(eARRY_SELECT);
	_resetSlot(eARRY_TRADE);

	_setNas(eTEXT_TRADE_NAS, 0);

	// Set Player inven Item
	CUIArrayItem* pTempItem = m_pArry[eARRY_SELECT]->GetArrayItemTemplate();

	if (pTempItem == NULL)
		return;

	CUIArrayItem* pItem = NULL;
	CUIIcon* pIcon = NULL;

	int		t, i, idx;
	bool bDrag;
	for (i = 0; i < SHOP_SLOT_SELL_MAX; ++i)
	{

		if (i >= (ITEM_COUNT_IN_INVENTORY_NORMAL + ITEM_COUNT_IN_INVENTORY_CASH_1))
		{
			t = INVENTORY_TAB_CASH_2;
			idx = i - (ITEM_COUNT_IN_INVENTORY_NORMAL + ITEM_COUNT_IN_INVENTORY_CASH_1);
		}
		else if (i >= ITEM_COUNT_IN_INVENTORY_NORMAL)
		{
			t = INVENTORY_TAB_CASH_1;
			idx = i - ITEM_COUNT_IN_INVENTORY_NORMAL;
		}
		else
		{
			t = INVENTORY_TAB_NORMAL;
			idx = i;
		}

		m_pArry[eARRY_SELECT]->AddArrayItem((CUIArrayItem*)pTempItem->Clone());
		pItem = m_pArry[eARRY_SELECT]->GetArrayItem(i);

		if (pItem == NULL)
			continue;

		pIcon = (CUIIcon*)pItem->findUI("icon_item");

		if (pIcon == NULL)
			continue;

		CItems*	pItems = &_pNetwork->MySlotItem[t][idx];
		if (pItems->Item_Sum > 0)
		{
			// 인벤토리 접근 시에만 t, idx 변환하고, shop 에 사용된느 것은 순차 인덱스 사용
			CItems* pCopy = new CItems;
			memcpy(pCopy, pItems, sizeof(CItems));
			pIcon->setData(pCopy, false);
			pIcon->setCount(pItems->Item_Sum);
			pIcon->SetWhichUI(UI_SHOP);
			bDrag = false;

			CItemData*	pItemData = pItems->ItemData;

			if (pItemData == NULL)
				continue;
#ifdef ADD_SUBJOB
			if (pItemData->IsFlag(ITEM_FLAG_SELLER))
			{
				if( UIMGR()->CheckSellerItem(UI_SHOP, pItemData->GetFlag()) )
					bDrag = true;
			}
			else
			{
				if ((pItemData->GetType() != CItemData::ITEM_ETC || pItemData->GetSubType() != CItemData::ITEM_ETC_REFINE) &&
					(pItemData->GetFlag() & ITEM_FLAG_TRADE) && !pItems->IsFlag(FLAG_ITEM_LENT))
					bDrag = true;
			}
#else
			if ((pItemData->GetType() != CItemData::ITEM_ETC || pItemData->GetSubType() != CItemData::ITEM_ETC_REFINE) &&
				(pItemData->GetFlag() & ITEM_FLAG_TRADE) && !pItems->IsFlag(FLAG_ITEM_LENT))
				bDrag = true;
#endif
			if (bDrag == true)
			{
				CmdDragIcon* pCmd = new CmdDragIcon;
				pCmd->setData(pIcon);
				pIcon->SetCommandDrag(pCmd);
			}
			else
				pIcon->setImage(eIMG_TYPE_HOLD, true);
		}
		else
			pIcon->clearIconData();
	}

	m_pArry[eARRY_SELECT]->SetSelectIdx(-1);
	m_pArry[eARRY_SELECT]->UpdateItem();
}

bool ItemLevelOfComp( const int& x, const int& y )
{
	CItemData*	pItemData[2];

	pItemData[0] = _pNetwork->GetItemData(x);
	pItemData[1] = _pNetwork->GetItemData(y);

	return pItemData[0]->GetLevel() < pItemData[1]->GetLevel();
}

void CShopUI::_updateLease()
{
	if (m_nLeaseJob <= -1)
		return;

	if (m_pArry[eARRY_TRADE] != NULL)
		m_pArry[eARRY_TRADE]->Hide(TRUE);

	_resetLeaseInfo();
	_resetSlot(eARRY_SELECT);

	_setNas(eTEXT_LEASE_NAS, 0);

	CUIArrayItem* pTempItem = m_pArry[eARRY_SELECT]->GetArrayItemTemplate();

	if (pTempItem == NULL)
		return;

	CItemData*	pItemData[2];

	int			tv_itemIdx;
	int			tv_itemLevel;

	std::vector<int> vecbtnItems;

	int selNum;
	int max = CItemData::getsize();

	CItemData::_map::iterator	iter = CItemData::_mapdata.begin();
	CItemData::_map::iterator	eiter = CItemData::_mapdata.end();

	for (selNum = 0; iter != eiter; ++iter)
	{	
		CItemData* pData = (*iter).second;

		if (pData == NULL)
			continue;

		if(	pData->GetJob() & 0x01 << m_nLeaseJob &&
			pData->GetType() == CItemData::ITEM_WEAPON )
		{
			tv_itemLevel = pData->GetLevel();
			if(	tv_itemLevel >= LEASE_MIN_LEVEL &&
				tv_itemLevel <= LEASE_MAX_LEVEL )
			{
				if(pData->IsFlag(ITEM_FLAG_LENT) )
				{
					tv_itemIdx = pData->GetItemIndex();
					vecbtnItems.push_back(tv_itemIdx);

					selNum++;
				}
			}
		}
	}
	
	// Set Lease Item
	CUIArrayItem* pItem = NULL;
	CUIArrayItem* pPrevItem = NULL;
	CUIIcon* pIcon = NULL;
	CUIIcon* pPrevIcon = NULL;
	CUIManager* pUIManager = CUIManager::getSingleton();

	// 아이템 레벨별 정렬
	std::sort(vecbtnItems.begin(), vecbtnItems.end(), ItemLevelOfComp );

	std::vector<int>::iterator btnItr;
	for ( btnItr = vecbtnItems.begin(), selNum = 0; btnItr != vecbtnItems.end(); btnItr++ )
	{
		m_pArry[eARRY_SELECT]->AddArrayItem((CUIArrayItem*)pTempItem->Clone());
		pItem = m_pArry[eARRY_SELECT]->GetArrayItem(selNum);

		if (pItem == NULL)
			continue;

		pIcon = (CUIIcon*)pItem->findUI("icon_item");

		if (pIcon == NULL)
			continue;

		tv_itemIdx = (*btnItr);

		if (selNum != 0)
		{
			pPrevItem = m_pArry[eARRY_SELECT]->GetArrayItem(selNum - 1);

			if (pPrevItem == NULL)
				continue;

			pPrevIcon = (CUIIcon*)pPrevItem->findUI("icon_item");

			if (pPrevIcon == NULL)
				continue;

			pItemData[0] = _pNetwork->GetItemData((*btnItr));
			pItemData[1] = _pNetwork->GetItemData(pPrevIcon->getIndex());

			if(pItemData[1]->GetLevel() == pItemData[0]->GetLevel())
			{
				// 나중에 서브타입이 작다면 바꿔준다.
				if(pItemData[1]->GetSubType() > pItemData[0]->GetSubType())
				{
					CItems* pItems = new CItems;
					pItems->Item_Index = tv_itemIdx;
					pItems->ItemData = CItemData::getData(tv_itemIdx);
					pPrevIcon->setData(pItems, false);
					pPrevIcon->setCount(1);
					pPrevIcon->setImage(eIMG_TYPE_RENT, true);
					pPrevIcon->SetWhichUI(UI_SHOP);
					tv_itemIdx = pItemData[1]->GetItemIndex();
				}
			}
		}

		// 이부분이 Shop 인벤토리 안으로 들어갈것...
		CItems* pItems = new CItems;
		pItems->Item_Index = tv_itemIdx;
		pItems->ItemData = CItemData::getData(tv_itemIdx);
		pIcon->setData(pItems, false);
		pIcon->setCount(1);
		pIcon->setImage(eIMG_TYPE_RENT, true);
		pIcon->SetWhichUI(UI_SHOP);
		selNum++;
	}
	m_pArry[eARRY_SELECT]->SetSelectIdx(-1);
	m_pArry[eARRY_SELECT]->UpdateItem();
}

void CShopUI::_resetSlot( int nSlotType )
{
	switch(nSlotType)
	{
	case eARRY_SELECT:
		{
			if (m_pArry[eARRY_SELECT] != NULL)
				m_pArry[eARRY_SELECT]->ResetArray();
		}
		break;
	case eARRY_TRADE:
		{
			if (m_pArry[eARRY_TRADE] != NULL)
			{
				CUIIcon* pIcon = NULL;
				for (int i = 0; i < m_pArry[eARRY_TRADE]->GetArrayChildCount(); ++i)
				{
					pIcon = _getTradeItemSlot(i);

					if (pIcon != NULL)
						pIcon->clearIconData();
				}
			}
		}
		break;
	}	
}

void CShopUI::_resetLeaseInfo()
{
	if (m_pText[eTEXT_ITEM_NAME_TITLE] != NULL)
		m_pText[eTEXT_ITEM_NAME_TITLE]->Hide(FALSE);

	if (m_pText[eTEXT_ITEM_NAME] != NULL)
		m_pText[eTEXT_ITEM_NAME]->Hide(TRUE);

	for (int i = eTEXT_ITEM_NAME; i < eTEXT_MAX; ++i)
	{
		if (m_pText[i] != NULL) 
			_setText(i, "");
	}

	_setNas(eTEXT_LEASE_NAS, 0);
}

CUIIcon* CShopUI::_getTradeItemSlot( int nSlotPos )
{
	if (m_pArry[eARRY_TRADE] == NULL)
		return NULL;

	int nMaxSlot = m_pArry[eARRY_TRADE]->GetArrayChildCount();

	if (nMaxSlot <= nSlotPos)
		return NULL;

	CUIArrayItem* pItem = m_pArry[eARRY_TRADE]->GetArrayItem(nSlotPos);

	if (pItem == NULL)
		return NULL;

	CUIIcon* pIcon = (CUIIcon*)pItem->findUI("icon_item");

	if (pIcon == NULL)
		return NULL;

	return pIcon;
}

CUIIcon* CShopUI::_getMyItemSlot( int nInvenIdx )
{
	if (m_pArry[eARRY_SELECT] == NULL)
		return NULL;

	int nMaxSlot = m_pArry[eARRY_SELECT]->GetArrayChildCount();

	if (nMaxSlot <= nInvenIdx)
		return NULL;

	CUIArrayItem* pItem = m_pArry[eARRY_SELECT]->GetArrayItem(nInvenIdx);

	if (pItem == NULL)
		return NULL;

	CUIIcon* pIcon = (CUIIcon*)pItem->findUI("icon_item");

	if (pIcon == NULL)
		return NULL;

	return pIcon;
}

__int64 CShopUI::CalculateTotalPrice(int iShopID, int& iCount, BOOL bSell, BOOL bRate)
{
 	if(iShopID	<= 0)								
		return 0;
 
 	MyInfo* pInfo = MY_INFO();
 	__int64	iTotalPrice	= 0;
 	iCount				= 0;
 
	for( int i = 0; i < SHOP_TRADE_SLOT_TOTAL; i++ )
 	{
		CUIIcon* pIcon = _getTradeItemSlot(i);
		
		if( pIcon == NULL || pIcon->IsEmpty() )
 			continue;
 
 		const int iIndex		= pIcon->getIndex();
 		SQUAD iItemCount		= pIcon->getItems()->Item_Sum;
 		CItemData* pID			= _pNetwork->GetItemData(iIndex);
 
 		if( pID->GetItemIndex() == -1 )
 			continue;
 
 
 		__int64 llitemPrice = CalculateItemPrice( iShopID, *pID, iItemCount, bSell );
 		__int64 iAddPrice = pInfo->CalcPriceRate(llitemPrice, bSell) + pInfo->CalcPriceAddition(bSell);

		// 로얄럼블 입장권은 스킬가격 무시
		if (iIndex == 7340 ||
			iIndex == 7341 ||
			iIndex == 7342)
			iAddPrice = 0;
 
 		if (bRate == TRUE)
 			llitemPrice += iAddPrice;
 
 		iTotalPrice += llitemPrice;
 		iCount++;
 	}

	return iTotalPrice;
}

__int64	CShopUI::CalculateItemPrice(int iShopID, const CItemData &ID, int iNumOfItem, BOOL bSell)
{
	if(iShopID <= 0)
		return 0;

 	const CShopData &SD = _pNetwork->GetShopData(iShopID);	
 	const int iShopSellRate	= SD.GetSellRate();
 	int iShopBuyRate	= SD.GetBuyRate();
 	__int64	iPrice		= ID.GetPrice();

 	if( bSell )
 		iPrice = (iPrice * iShopBuyRate) / 100;
 	else
 	{
 		iPrice = (iPrice * iShopSellRate) / 100;
 
 		// [090617: selo] 미국의 경우 아이템 레벨이 97이상이면 아이템의 가격을 반값으로 한다.
 		if (g_iCountry == USA)
 		{
 			if( 97 <= ID.GetLevel() )
 				iPrice /= 2;
 		}
 	}
 
#ifdef NEW_CHAO_SYS
 	int fpkpenalty = abs(_pNetwork->MyCharacterInfo.pkpenalty);
 
 	if(_pNetwork->MyCharacterInfo.pkpenalty < 0 && bSell)
 	{
 		if (g_iCountry == RUSSIA)
 		{
 			iPrice = iPrice + (iPrice * fpkpenalty / 32000 * (20));
 		}
 	}
#endif
 
 	iPrice *= iNumOfItem;
	return iPrice;
}

int CShopUI::_getSlotSelectIndex(int nSlotType)
{
	if (nSlotType < eARRY_TRADE || nSlotType >= eARRY_MAX)
		return -1;

	if (m_pArry[nSlotType] == NULL)
		return -1;

	return m_pArry[nSlotType]->GetSelectIdx();
}

CItems* CShopUI::GetTradeItem( int idx )
{
	if (idx < 0 || idx >= m_vecTrade.size())
		return NULL;

	return m_vecTrade[idx];
}

void CShopUI::clearTrade()
{
	for (int i = 0; i < m_vecTrade.size(); ++i)
		SAFE_DELETE(m_vecTrade[i]);

	m_vecTrade.clear();	
}

void CShopUI::_setSlotSelectIndex( int nSlotType, int nIndex )
{
	if (nSlotType < eARRY_TRADE || nSlotType >= eARRY_MAX)
		return;

	if (m_pArry[nSlotType] == NULL)
		return;

	if (nSlotType == eARRY_TRADE)
		m_nSelTradeItemID = nIndex;
	else if (nSlotType == eARRY_SELECT)
	{
		m_nSelShopItemID = nIndex;
		
		if (m_nUIType == eSHOP_LEASE)
		{
			if (nIndex < 0 || nIndex >= m_pArry[nSlotType]->GetArrayChildCount())
				_resetLeaseInfo();
			else
				_setLeaseInfo(nIndex);
		}
	}

	m_pArry[nSlotType]->SetSelectIdx(nIndex);
}

BOOL CShopUI::IsBuyShop()
{
	BOOL bBuyShop = TRUE;

	if (m_nUIType == eSHOP_SELL)
		bBuyShop = FALSE;

	return bBuyShop;
}

bool CShopUI::_checkSendCondition()
{
	if (m_pArry[eARRY_TRADE] == NULL)
		return false;

	int nSize = m_pArry[eARRY_TRADE]->GetArrayChildCount();
	CUIIcon* pIcon = NULL;
	for (int i = 0; i < nSize; ++i)
	{
		pIcon = _getTradeItemSlot(i);

		if (pIcon == NULL)
			continue;

		// 구입 및 판매할 아이템이 하나라도 있으면 OK
		if (pIcon->IsEmpty() == FALSE)
			return true;
	}

	return false;
}

void CShopUI::_setBtnString( int nBtnType, CTString strString )
{
	if (nBtnType <= 0 || nBtnType >= eBTN_MAX)
		return;

	if (m_pBtn[nBtnType] != NULL)
		m_pBtn[nBtnType]->SetText(strString);
}

void CShopUI::_setLeaseInfo( int nSlotIndex )
{
	CUIIcon* pIcon = NULL;

	pIcon = _getMyItemSlot(nSlotIndex);

	if (pIcon == NULL)
		return;

	if (m_pText[eTEXT_ITEM_NAME_TITLE] != NULL)
		m_pText[eTEXT_ITEM_NAME_TITLE]->Hide(TRUE);

	if (m_pText[eTEXT_ITEM_NAME] != NULL)
		m_pText[eTEXT_ITEM_NAME]->Hide(FALSE);

	int tv_itemIdx = pIcon->getIndex();
	CItemData* pItemData = _pNetwork->GetItemData( tv_itemIdx );

	// Weapon Name
	const char* szItemName = _pNetwork->GetItemName( tv_itemIdx );
	CTString tv_str;
	int tv_int;

	if ( g_iCountry == KOREA || g_iCountry == USA || g_iCountry == RUSSIA || g_iCountry == THAILAND )
		tv_str = CTString("+12 ");	// 국내 무기대여아이템 레벨 +6 에서 +12로 변경
	else
		tv_str = CTString("+6 ");

	tv_str += szItemName;	
	_setText(eTEXT_ITEM_NAME, tv_str);
	
	// Weapon Level
	tv_int = pItemData->GetLevel();
	tv_str.PrintF("%d", tv_int);
	_setText(eTEXT_ITEM_LEVEL, tv_str);

	// Class 
	tv_int = pItemData->GetJob();	//로그Ex로그 144 메이지 아크메이지264
	switch(tv_int)
	{
	case 1:
		tv_str = _S(43,"타이탄");
		break;
	case 2:
		tv_str = _S(44,"기사");
		break;
	case 4:
		tv_str = _S(45,"힐러");
		break;
	case 8:
		tv_str = _S(46,"메이지");
		break;
	case 264:	//2013/01/22 jeil 무기 대여상에서 사용직업 정보 잘못나오던것 수정
		tv_str = _S(46,"메이지");
#ifdef CHAR_EX_MAGE
		tv_str += _S(5820,"아크메이지");	// 2013/01/08 jeil EX메이지 추가 스트링 나오면 추가수정 필요 
#endif
		break;
	case 16:
		tv_str = _S(47,"로그");
		break;
	case 144:	//2013/01/22 jeil 무기 대여상에서 사용직업 정보 잘못나오던것 수정
		tv_str = _S(47,"로그");
#ifdef CHAR_EX_ROGUE
		tv_str += _S(5732,"EX로그");	// [2012/08/27 : Sora] EX로그 추가
#endif
		break;
	case 32:
		tv_str = _S(48,"소서러");
		break;
	default : 
		tv_str = CTString("Alien");
		break;
	}		
	
	_setText(eTEXT_ITEM_JOB, tv_str);

	int tv_plus = 6;  // 로컬은 +6으로 고정

	if (g_iCountry == KOREA || g_iCountry == USA || g_iCountry == RUSSIA || g_iCountry == THAILAND)
		tv_plus = 12 ;  // +6으로 고정(+6에서 +12로 변경)

	// attack ability
	tv_int = CItems::CalculatePlusDamage( pItemData->GetPhysicalAttack(), tv_plus, pItemData->GetLevel() >= 146 ? TRUE : FALSE );
	if( tv_int > 0 )
		tv_str.PrintF( CTString("%d + %d" ), pItemData->GetPhysicalAttack(), tv_int );
	else
		tv_str.PrintF( CTString( "%d" ), pItemData->GetPhysicalAttack() );

	_setText(eTEXT_ITEM_ATT, tv_str);

	// magic attack ability
	tv_int = CItems::CalculatePlusDamage( pItemData->GetMagicAttack(), tv_plus, pItemData->GetLevel() >= 146 ? TRUE : FALSE );
	if( tv_int > 0 )
		tv_str.PrintF( CTString("%d+%d" ), pItemData->GetMagicAttack(), tv_int );
	else
		tv_str.PrintF( CTString( "%d" ), pItemData->GetMagicAttack() );

	_setText(eTEXT_ITEM_MAGIC, tv_str);

	tv_int = 1;
	tv_str.PrintF(_S(3079, "%d일"), tv_int);
	_setText(eTEXT_ITEM_TIME, tv_str);

	// Lease Price
	LONGLONG llPrice =(pItemData->GetPrice() * 1.5) + 50000;
	_setNas(eTEXT_LEASE_NAS, llPrice);
}


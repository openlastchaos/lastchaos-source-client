#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <vector>
#include "PersonalshopUI.h"
#include "PersonalshopCmd.h"
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UIPetInfo.h>
#include <Engine/Help/Util_Help.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UIRadar.h>
#include <Engine/Interface/UISocketSystem.h>
#include <Engine/Interface/UIWildPetInfo.h>

#include <Engine/Contents/Base/UIMsgBoxNumeric_only.h>
#include <Engine/Info/MyInfo.h>

#define PACKAGE_ITEM_INDEX		(506)

// ----------------------------------------------------------------------------
// Name : CUIPersonalShop()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIPersonalshopNew::CUIPersonalshopNew()
	: m_pArrTrade(NULL)
	, m_pArrPackage(NULL)
	, m_pArrShop(NULL)
	, m_pEditName(NULL)
	, m_pEditPackagePrice(NULL)
	, m_pBtnCancel(NULL)
	, m_pBtnBuy(NULL)
	, m_pBtnPackOk(NULL)
	, m_pBtnPackCancel(NULL)
	, m_pChkPremium(NULL)
	, m_pBtnStart(NULL)
	, m_pTxtNas(NULL)
	, m_pTxtTotal(NULL)
	, m_pTxtPackagePrice(NULL)
	, m_pTxtTitle(NULL)
	, m_bTitleClick(false)
	, m_pGroupSell(NULL)
	, m_pGroupBuy(NULL)
	, m_pImgUnmove(NULL)
	, m_pImgSelect(NULL)
{
	setInherit(false);

	m_bBuyShop			= FALSE;
	m_bShopStart		= FALSE;
	m_bPackageBuy		= FALSE;
	m_nSelectedChaID	= -1;	
	m_nNumOfItem		= 0;
	m_nTotalPrice		= 0;
	m_bPremium			= FALSE;
	m_llPackagePrice	= 0;
	m_bCashPersonShop	= FALSE;
	m_bCashPersonShop_open = FALSE;
}

// ----------------------------------------------------------------------------
// Name : ~CUIPersonalShop()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIPersonalshopNew::~CUIPersonalshopNew()
{
	clearContainer();
}

void CUIPersonalshopNew::initialize()
{
#ifndef		WORLD_EDITOR
	int		i;

	m_pEditName = (CUIEditBox*)findUI("ed_name");				// xml max length : 80

	m_pChkPremium = (CUICheckButton*)findUI("chk_premium");

	if (m_pChkPremium != NULL)
	{
		m_pChkPremium->SetCommandF(boost::bind(&CUIPersonalshopNew::check_premium, this));
	}

	m_pEditPackagePrice = (CUIEditBox*)findUI("ed_price");
	m_pBtnPackOk = (CUIButton*)findUI("btn_package_ok");
	m_pBtnPackCancel = (CUIButton*)findUI("btn_package_cancel");

	if (m_pBtnPackOk != NULL)
	{
		m_pBtnPackOk->SetCommandFUp(boost::bind(&CUIPersonalshopNew::package_ok, this));
	}

	if (m_pBtnPackCancel != NULL)
	{
		m_pBtnPackCancel->SetCommandFUp(boost::bind(&CUIPersonalshopNew::package_cancel, this));
	}

	m_pBtnStart = (CUIButton*)findUI("btn_start");

	m_pBtnCancel = (CUIButton*)findUI("btn_buy_cancel");
	m_pBtnBuy = (CUIButton*)findUI("btn_buy_ok");

	m_pTxtNas = (CUIText*)findUI("txt_nas");
	m_pTxtTotal = (CUIText*)findUI("txt_total");
	

	m_pTxtTitle = (CUIText*)findUI("txt_title");

	{
		// 닫기 버튼 명령 연결
		CUIButton* pButton = (CUIButton*)findUI("btn_close");

		if (pButton)
		{
			CmdPSClose* pCmd = new CmdPSClose;
			pCmd->setData(this);
			pButton->SetCommandUp(pCmd);
		}
	}

	{
		// 물품 등록
		m_pArrTrade = (CUIArray*)findUI("arr_item");

		if (m_pArrTrade != NULL)
		{
			CUIArrayItem* pItem;

			for (i = 0; i < PERSONAL_TRADE_SLOT_TOTAL; ++i)
			{
				pItem = m_pArrTrade->GetArrayItem(i);

				CmdPSProcCmd* pDrop = new CmdPSProcCmd;
				pDrop->setData(CMD_LBUTTONUP, this, SLOT_TRADE);
				m_pArrTrade->SetCommandUp(pDrop);

				if (pItem != NULL)
				{
					m_pIconsTradeItem[i] = (CUIIcon*)pItem->findUI("icon");

					if (m_pIconsTradeItem[i] != NULL)
					{
						m_pIconsTradeItem[i]->clearIconData();
						m_pIconsTradeItem[i]->SetWhichUI(UI_PERSONALSHOP);

						CmdPSProcCmd* pCmd = new CmdPSProcCmd;
						pCmd->setData(CMD_DBLCLICK, this, SLOT_TRADE);
						m_pIconsTradeItem[i]->SetCommandDBL(pCmd);

						CmdDragIcon* pCmdDrag = new CmdDragIcon;
						pCmdDrag->setData(m_pIconsTradeItem[i]);
						m_pIconsTradeItem[i]->SetCommandDrag(pCmdDrag);						
					}
				}
			}
		}
	}

	{
		// 패키지
		m_pArrPackage = (CUIArray*)findUI("arr_package");

		if (m_pArrPackage != NULL)
		{
			CmdPSProcCmd* pDrop = new CmdPSProcCmd;
			pDrop->setData(CMD_LBUTTONUP, this, SLOT_PACKAGE);
			m_pArrPackage->SetCommandUp(pDrop);

			CUIArrayItem* pItem;

			for (i = 0; i < PERSONAL_PACKAGE_SLOT_COL; ++i)
			{
				pItem = m_pArrPackage->GetArrayItem(i);

				if (pItem != NULL)
				{
					m_pIconsPackageItem[i] = (CUIIcon*)pItem->findUI("icon");

					if (m_pIconsPackageItem[i] != NULL)
					{
						m_pIconsPackageItem[i]->clearIconData();
						m_pIconsPackageItem[i]->SetWhichUI(UI_PERSONALSHOP);
					}
				}
			}
		}

		m_pTxtPackagePrice = (CUIText*)findUI("txt_package_price");
	}

	// 인벤연결
	{
		m_pArrShop = (CUIArray*)findUI("arr_inven");

		if (m_pArrShop != NULL)
		{
			CmdPSProcCmd* pDrop = new CmdPSProcCmd;
			pDrop->setData(CMD_LBUTTONUP, this, SLOT_SHOP);
			m_pArrShop->SetCommandUp(pDrop);

			m_pArrShop->ResetArray();

			int row = PERSONAL_SHOP_SLOT_MAX / PERSONAL_SHOP_SLOT_COL;
			m_pArrShop->SetRowCol(row, PERSONAL_SHOP_SLOT_COL);
			m_pArrShop->SetShowRowCount(PERSONAL_SHOP_SLOT_ROW);

			CUIArrayItem* pItem = m_pArrShop->GetArrayItemTemplate();

			for (i = 0; i < PERSONAL_SHOP_SLOT_MAX; ++i)
			{
				CUIArrayItem* pClone = (CUIArrayItem*)pItem->Clone();

				m_pArrShop->AddArrayItem(pClone, false);

				m_pIconsShopItem[i] = (CUIIcon*)pClone->findUI("icon");

				if (m_pIconsShopItem[i] != NULL)
				{
					m_pIconsShopItem[i]->clearIconData();
					m_pIconsShopItem[i]->SetWhichUI(UI_PERSONALSHOP);

					// 단편화가 될 수 있으므로, 처음 등록 시 모두 등록
					CmdPSProcCmd* pCmd = new CmdPSProcCmd;
					pCmd->setData(CMD_DBLCLICK, this, SLOT_SHOP);
					m_pIconsShopItem[i]->SetCommandDBL(pCmd);

					CmdDragIcon* pCmdDrag = new CmdDragIcon;
					pCmdDrag->setData(m_pIconsShopItem[i]);
					m_pIconsShopItem[i]->SetCommandDrag(pCmdDrag);
				}
			}

			m_pArrShop->UpdateItem();
		}
	}

	m_pGroupSell = (CUIBase*)findUI("group_sell");
	m_pGroupBuy = (CUIBase*)findUI("group_buy");

	m_pImgUnmove = (CUIImage*)findUI("img_lock");
	m_pImgSelect = (CUIImage*)findUI("img_select");

	{
		CUIButton* pBtnStart = (CUIButton*)findUI("btn_start");

		if (pBtnStart != NULL)
		{
			pBtnStart->SetCommandFUp(boost::bind(&CUIPersonalshopNew::start_shop, this));
		}
	}

	{
		CUIButton* pBtnBuy = (CUIButton*)findUI("btn_buy_ok");

		if (pBtnBuy != NULL)
		{
			pBtnBuy->SetCommandFUp(boost::bind(&CUIPersonalshopNew::buy, this));
		}
	}

	{
		CUIButton* pBtnCancel = (CUIButton*)findUI("btn_buy_cancel");

		if (pBtnCancel != NULL)
		{
			pBtnCancel->SetCommandFUp(boost::bind(&CUIPersonalshopNew::cancel, this));
		}
	}
#endif	// WORLD_EDITOR
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIPersonalshopNew::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI ) / 2 - GetWidth(), ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIPersonalshopNew::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

//------------------------------------------------------------------------------
// CUIPersonalShop::SetFocus
// Explain:  
// Date : 2005-03-10(오후 1:03:58) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIPersonalshopNew::SetFocus( bool bVisible )
{
	CUIWindow::SetFocus( bVisible );
	if (m_pEditName != NULL)
		m_pEditName->SetFocus( bVisible );

	if( !bVisible )
	{
		if (m_pEditPackagePrice != NULL)
			m_pEditPackagePrice->SetFocus( FALSE );
	}
}

// ----------------------------------------------------------------------------
// Name : OpenPersonalShop()
// Desc :
// ----------------------------------------------------------------------------
void CUIPersonalshopNew::OpenPersonalShop( bool bSellShoop )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->IsCSFlagOn( CSF_PERSONALSHOP ) || IsVisible() )
	{
		// FIXME : 코드가 중복되는 곳이 너무 많음.
		if(!m_bBuyShop && m_bShopStart)
		{
			//pUIManager->GetInventory()->Lock( m_bShopStart );

			//bool	bVisible = !IsVisible();
			//pUIManager->RearrangeOrder( UI_PERSONALSHOP, bVisible );
			pUIManager->GetChattingUI()->AddSysMessage( _S( 745, "상점 개설중에는 창을 닫을 수 없습니다." ), SYSMSG_ERROR );	
			return;
		}
		else
		{
			ResetShop();
			pUIManager->SetCSFlagOff( CSF_PERSONALSHOP );
		}
		return;
	}

	// If inventory is already locked
	if( pUIManager->GetInventory()->IsLocked() ||
		pUIManager->GetInventory()->IsLockedArrange())
	{
		pUIManager->GetInventory()->ShowLockErrorMessage();
		pUIManager->SetCSFlagOff( CSF_PERSONALSHOP );
		return;
	}

	m_pChkPremium->Hide(FALSE);
	m_pChkPremium->SetEnable(TRUE);
	//m_btnPackageOK.SetEnable( FALSE );
	//m_btnPackageCancel.SetEnable( FALSE );
	m_pEditPackagePrice->SetEnable( m_bPremium );

	// 판매 상점 창의 경우 초기화 될 필요 없음.
	//ResetShop();

	m_nSelectedChaID	= -1;

	pUIManager->CloseMessageBox( MSGCMD_DROPITEM );

	PrepareSellShop();
}

// ----------------------------------------------------------------------------
// Name : TradePersonalShop()
// Desc : 개인 상점에서 아이템 사기 & 팔기
// ----------------------------------------------------------------------------
void CUIPersonalshopNew::TradePersonalShop( INDEX iChaIndex, FLOAT fX, FLOAT fZ, bool bBuy )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// FIXME : 중복코드 제거
	if( pUIManager->IsCSFlagOn( CSF_PERSONALSHOP ) || IsVisible() )
	{
		return;
	}

	if( (m_nSelectedChaID == iChaIndex) && IsVisible() )
	{
		return;
	}

	if(((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsMoving())
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 746, "이동중에는 개임상점거래를 할 수 없습니다." ), SYSMSG_ERROR );	
		return;
	}

	if(((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsSkilling())
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 747, "스킬 사용중에는 개인상점거래를 할 수 없습니다." ), SYSMSG_ERROR );	
		return;
	}

	if(pUIManager->IsCSFlagOn( CSF_TELEPORT ))
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 748, "순간 이동중에는 개인상점거래를 할 수 없습니다." ), SYSMSG_ERROR );	
		return;
	}

	// If inventory is already locked
	if( pUIManager->GetInventory()->IsLocked() )
	{
		pUIManager->GetInventory()->ShowLockErrorMessage();
		return;
	}

	pUIManager->SetCSFlagOn( CSF_PERSONALSHOP );
	//m_bBuyShop = TRUE;

	ResetShop(TRUE);

	if(INFO()->GetTargetDBIdx(eTARGET) == 482)
	{
		m_bCashPersonShop_open = TRUE;
		SendCashPersonOpenShop( iChaIndex );
	}else
	{
		SendPersonalOpenShop( iChaIndex );
	}

}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIPersonalshopNew::PrepareBuyShop()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// Lock inventory
	pUIManager->GetInventory()->Lock( TRUE, FALSE, LOCK_PERSONAL_SHOP);

	m_bBuyShop			= TRUE;
	RefreshUserItem();

	if (m_pEditPackagePrice != NULL)
		m_pEditPackagePrice->Hide(TRUE);

	// Set money
	if( _pNetwork->MyCharacterInfo.money > 0 )
	{
		CTString strPlayerMoney;
		strPlayerMoney.PrintF( "%I64d", _pNetwork->MyCharacterInfo.money );
		pUIManager->InsertCommaToString( strPlayerMoney );
		if (m_pTxtNas != NULL)
		{
			COLOR col = pUIManager->GetNasColor(_pNetwork->MyCharacterInfo.money);
			m_pTxtNas->SetText(strPlayerMoney);
			m_pTxtNas->setFontColor(col);
		}
	}

	// Set money
	if( m_llPackagePrice > 0 )
	{
		CTString strPackagePrice;
		strPackagePrice.PrintF("%I64d", m_llPackagePrice);
		pUIManager->InsertCommaToString(strPackagePrice);

		if (m_pTxtPackagePrice != NULL)
		{
			m_pTxtPackagePrice->Hide(FALSE);

			m_pTxtPackagePrice->SetText(strPackagePrice);
			COLOR col = pUIManager->GetNasColor(m_llPackagePrice);
			m_pTxtPackagePrice->setFontColor(col);
		}
	}

	if (m_pGroupSell)
		m_pGroupSell->Hide(TRUE);
	if (m_pGroupBuy)
		m_pGroupBuy->Hide(FALSE);

	for (int i = 0; i < PERSONAL_PACKAGE_SLOT_COL; ++i)
	{
		if (m_pIconsPackageItem[i] != NULL)
		{
			m_pIconsPackageItem[i]->SetCommandDBL(NULL);
			m_pIconsPackageItem[i]->SetCommandDrag(NULL);
		}
	}

	Hide(FALSE);
	pUIManager->RearrangeOrder( UI_PERSONALSHOP, TRUE );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIPersonalshopNew::PrepareSellShop()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// Lock inventory
	pUIManager->GetInventory()->Lock( TRUE, FALSE, LOCK_PERSONAL_SHOP );

	m_bBuyShop			= FALSE;
	RefreshPlayerItem();

	if (m_pTxtPackagePrice != NULL)
		m_pTxtPackagePrice->Hide(TRUE);

	if (m_pGroupSell != NULL)
		m_pGroupSell->Hide(FALSE);
	if (m_pGroupBuy != NULL)
		m_pGroupBuy->Hide(TRUE);

	if (m_pBtnPackOk != NULL)
		m_pBtnPackOk->SetEnable(FALSE);

	if (m_pBtnPackCancel != NULL)
		m_pBtnPackCancel->SetEnable(FALSE);

	if (m_pEditPackagePrice != NULL)
		m_pEditPackagePrice->Hide(FALSE);

	for (int i = 0; i < PERSONAL_PACKAGE_SLOT_COL; ++i)
	{
		if (m_pIconsPackageItem[i] != NULL)
		{
			CmdPSProcCmd* pCmd = new CmdPSProcCmd;
			pCmd->setData(CMD_DBLCLICK, this, SLOT_PACKAGE);
			m_pIconsPackageItem[i]->SetCommandDBL(pCmd);

			CmdDragIcon* pCmdDrag = new CmdDragIcon;
			pCmdDrag->setData(m_pIconsPackageItem[i]);
			m_pIconsPackageItem[i]->SetCommandDrag(pCmdDrag);
		}
	}

	Hide(FALSE);
	pUIManager->RearrangeOrder( UI_PERSONALSHOP, TRUE );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIPersonalshopNew::RefreshPlayerItem()
{
	int		i;
	int		tab, idx;

	for (i = 0; i < PERSONAL_SHOP_SLOT_MAX; ++i)
	{
		if (i >= (ITEM_COUNT_IN_INVENTORY_NORMAL + ITEM_COUNT_IN_INVENTORY_CASH_1))
		{
			tab = INVENTORY_TAB_CASH_2;
			idx = i - (ITEM_COUNT_IN_INVENTORY_NORMAL + ITEM_COUNT_IN_INVENTORY_CASH_1);
		}
		else if (i >= ITEM_COUNT_IN_INVENTORY_NORMAL)
		{
			tab = INVENTORY_TAB_CASH_1;
			idx = i - ITEM_COUNT_IN_INVENTORY_NORMAL;
		}
		else
		{
			tab = INVENTORY_TAB_NORMAL;
			idx = i;
		}

		CItems*	pItems = &_pNetwork->MySlotItem[tab][idx];

		if( pItems->Item_Sum > 0 )
		{
			CItems* pCopy = new CItems;
			memcpy(pCopy, pItems, sizeof(CItems));
			m_pIconsShopItem[i]->setData(pCopy, false);		// count 변경을 하기 때문에 복제한다.

			// 판매 불가능 아이템 표시
			if (UTIL_HELP()->IsAvailable4Sale(pCopy, UI_PERSONALSHOP) == false)
			{
				CUIBase* pParent = m_pIconsShopItem[i]->getParent();

				if (pParent != NULL)
				{
					if (m_pImgUnmove != NULL)
					{
						CUIImage* pClone = (CUIImage*)m_pImgUnmove->Clone();
						pClone->Hide(FALSE);
						pParent->addChild(pClone);		// 제일 앞쪽에 넣어준다.
					}					
				}
			}
			else
			{
				CUIBase* pParent = m_pIconsShopItem[i]->getParent();

				if (pParent != NULL)
				{
					CUIImage* pImg = (CUIImage*)pParent->findUI("img_lock");

					// 이전에 열었던 데이터에 있던 것이므로 삭제
					if (pImg != NULL)
					{
						pParent->deleteChild(pImg);
					}
				}
			}			

			if(pItems->ItemData->GetType() == CItemData::ITEM_ACCESSORY &&
				pItems->ItemData->GetSubType() == CItemData::ACCESSORY_WILDPET)
			{
				_pNetwork->SendPetitemInfo(_pNetwork->MyCharacterInfo.index, pItems->Item_Plus);
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIPersonalshopNew::RefreshUserItem()
{
	int iPos = 0;
	vec_Items_iter end = m_vectorSellItemList.end();
	vec_Items_iter pos;

	for(pos = m_vectorSellItemList.begin(); pos != end; ++pos, ++iPos)
	{		
		if( (*pos)->Item_Sum > 0 )
		{
			const int iRow = iPos / PERSONAL_TRADE_SLOT_COL;
			const int iCol = iPos % PERSONAL_TRADE_SLOT_COL;

			m_pIconsTradeItem[iPos]->setData((*pos));
		}
	}

	{
		int iPos = 0;
		vec_Items_iter end = m_vectorSellPackageList.end();
		vec_Items_iter pos;

		for(pos = m_vectorSellPackageList.begin(); pos != end; ++pos, ++iPos)
		{		
			if( (*pos)->Item_Sum > 0 )
			{
				const int iRow = iPos / PERSONAL_PACKAGE_SLOT_COL;
				const int iCol = iPos % PERSONAL_PACKAGE_SLOT_COL;

				m_pIconsPackageItem[iPos]->setData((*pos));
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIPersonalshopNew::ResetShop(bool bOpen)
{
	m_nSelectedChaID	= -1;
	m_nTotalPrice		= 0;
	m_nNumOfItem		= 0;
	m_llPackagePrice	= 0;
	m_bBuyShop			= FALSE;
	m_bShopStart		= FALSE;
	m_bPremium			= FALSE;
	m_bPackageBuy		= FALSE;
	m_pEditPackagePrice->ResetString();
	m_pEditPackagePrice->SetFocus(FALSE);
	m_pEditName->ResetString();
	m_pEditName->SetFocus(FALSE);

	ChangeShopState( m_bShopStart );
	m_pChkPremium->SetCheck( m_bPremium );

	m_pBtnCancel->SetVisible(FALSE);
	m_pBtnCancel->SetEnable(FALSE);
	m_pBtnBuy->SetVisible(FALSE);
	m_pBtnBuy->SetEnable(FALSE);
	//m_pChkPremium->SetEnable( m_bPremium );
	m_pBtnPackOk->SetEnable( m_bPremium );
	m_pBtnPackCancel->SetEnable( m_bPremium );
	m_pEditPackagePrice->SetEnable( m_bPremium );

	ClearItems();

	if( bOpen == FALSE )
	{
		CUIManager* pUIManager = CUIManager::getSingleton();

		Hide(TRUE);
		pUIManager->RearrangeOrder( UI_PERSONALSHOP, FALSE );		

		// Unlock inventory
		pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_PERSONAL_SHOP );

		// Close message box of shop
		pUIManager->CloseMessageBox( MSGCMD_SHOP_ADD_PLUSITEM );
		pUIManager->CloseMessageBox( MSGCMD_SHOP_START );
		pUIManager->CloseMessageBox( MSGCMD_SHOP_PRICE );
		pUIManager->CloseMessageBox( MSGCMD_PERSONALSHOP_ERROR );
		pUIManager->GetMsgBoxNumOnly()->CloseBox();
	}

	m_bCashPersonShop = FALSE;
	m_bCashPersonShop_open = FALSE;

}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIPersonalshopNew::ClearItems()
{
	int		i;
	for( i = 0; i < PERSONAL_TRADE_SLOT_TOTAL; ++i)
	{
		m_pIconsTradeItem[i]->clearIconData();
		m_pIconsTradeItem[i]->Hide(FALSE);
	}

	for( i = 0; i < PERSONAL_SHOP_SLOT_MAX; ++i)
	{
		CUIBase* pParent = m_pIconsShopItem[i]->getParent();

		if (pParent != NULL)
		{
			CUIImage* pImg = (CUIImage*)pParent->findUI("img_lock");

			// 이전에 열었던 데이터에 있던 것이므로 삭제
			if (pImg != NULL)
			{
				pParent->deleteChild(pImg);
			}
		}

		m_pIconsShopItem[i]->clearIconData();
		m_pIconsShopItem[i]->Hide(FALSE);
	}

	// Package Slot(5x1)	
	for( i = 0; i < PERSONAL_PACKAGE_SLOT_COL; ++i)
	{
		m_pIconsPackageItem[i]->clearIconData();
		m_pIconsPackageItem[i]->Hide(FALSE);
	}

	clearContainer();
}

// ========================================================================= //
//                             Command functions                             //
// ========================================================================= //

static int		nTempIdx;
static int		nTempTradeItemID;
static int		nTempPackageItemID;
static SQUAD	llTempPrice;
static int		nTempSlot;

// ----------------------------------------------------------------------------
// Name : AddShopItem()
// Desc : From shop to trade
// ----------------------------------------------------------------------------
void CUIPersonalshopNew::AddShopItem( int nIdx, int nUniIndex, SQUAD llCount, int nWhichSlot )
{
	int	tab, idx;
	if (nIdx >= (ITEM_COUNT_IN_INVENTORY_NORMAL + ITEM_COUNT_IN_INVENTORY_CASH_1))
	{
		tab = INVENTORY_TAB_CASH_2;
		idx = nIdx - (ITEM_COUNT_IN_INVENTORY_NORMAL + ITEM_COUNT_IN_INVENTORY_CASH_1);
	}
	else if (nIdx >= ITEM_COUNT_IN_INVENTORY_NORMAL)
	{
		tab = INVENTORY_TAB_CASH_1;
		idx = nIdx - ITEM_COUNT_IN_INVENTORY_NORMAL;
	}
	else
	{
		tab = INVENTORY_TAB_NORMAL;
		idx = nIdx;
	}

	nTempIdx		= nIdx;
	nTempSlot		= nWhichSlot;

	CUIManager* pUIManager = CUIManager::getSingleton();

	CItems* pItems = m_pIconsShopItem[nIdx]->getItems();

	if (pItems == NULL)
		return;

	CItemData*	pItemData = pItems->ItemData;

	m_pTempItems = pItems;

	bool bMonsterMercenaryItem = pItemData->GetType() == CItemData::ITEM_ETC && pItemData->GetSubType() == CItemData::ITEM_ETC_MONSTER_MERCENARY_CARD; 

	if ((pItemData->GetType() == CItemData::ITEM_ETC && 
		pItemData->GetSubType() == CItemData::ITEM_ETC_MONEY))
		return;

	if (m_bBuyShop == FALSE)
	{
		// 대여 아이템 판매 불가
		if (UTIL_HELP()->IsAvailable4Sale(pItems, UI_PERSONALSHOP) == false)
		{
			// [2010/12/09 : Sora] 몬스터용병카드 조건 추가
			pUIManager->CloseMessageBox(MSGCMD_NULL);
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 757, "개인 상점" ), UMBS_OK,UI_NONE, MSGCMD_NULL);
			CTString strMessage(_S(3052,"판매할 수 없는 아이템 입니다."));
			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );
			return;
		}
	}

	if( !m_bBuyShop && nWhichSlot != SLOT_PACKAGE )
	{
		// Ask Price
		CTString	strMessage;
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 757, "개인 상점" ), UMBS_OKCANCEL | UMBS_INPUTBOX,		
			UI_PERSONALSHOP, MSGCMD_SHOP_PRICE );
		const char*	szItemName = _pNetwork->GetItemName(pItems->Item_Index);

		if (pItems->GetItemPlus() > 0 && !pUIManager->IsPet(pItemData) && !pUIManager->IsWildPet(pItemData) &&
			bMonsterMercenaryItem == FALSE)
		{
			// [2010/12/09 : Sora] 몬스터용병카드 조건 추가
			strMessage.PrintF( _S( 761, "%s +%d의 개당 가격을 입력해 주십시오." ), szItemName, pItems->Item_Plus );		
		}
		else 
		{
			strMessage.PrintF( _S( 762, "%s의 개당 가격을 입력해 주십시오." ), szItemName );		
		}
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );
	}	
	else
	{
		AskQuantity();
	}
}

// ----------------------------------------------------------------------------
// Name : AskQuantity()
// Desc : 
// ----------------------------------------------------------------------------
void CUIPersonalshopNew::AskQuantity()
{
	if (m_pTempItems == NULL)
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	CItemData*	pItemData = m_pTempItems->ItemData;
	const char*	szItemName = _pNetwork->GetItemName(m_pTempItems->Item_Index);

	// Ask quantity
	if ((m_bBuyShop && (pItemData->GetFlag() & ITEM_FLAG_COUNT)) ||
		(!m_bBuyShop&&  (pItemData->GetFlag() & ITEM_FLAG_COUNT) && m_pTempItems->Item_Sum > 1))
	{
		CTString	strMessage;
		strMessage.PrintF( _S2( 150, szItemName, "몇 개의 %s<를> 옮기시겠습니까?" ), szItemName );

		CmdPersnalShopAddItem* pCmd = new CmdPersnalShopAddItem;
		pCmd->setData(this);
		UIMGR()->GetMsgBoxNumOnly()->SetInfo(pCmd, _S( 757, "개인상점" ), strMessage, 1, m_pTempItems->Item_Sum);
	}
	else if( m_pTempItems->Item_Plus > 0 && !(pUIManager->IsPet(pItemData)||pUIManager->IsWildPet(pItemData) || 
		pItemData->GetType() == CItemData::ITEM_ETC && pItemData->GetSubType() == CItemData::ITEM_ETC_MONSTER_MERCENARY_CARD))
	{
		// [2010/12/09 : Sora] 몬스터용병카드 조건 추가
		CTString	strMessage;
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 757, "개인상점" ), UMBS_YESNO,
			UI_PERSONALSHOP, MSGCMD_SHOP_ADD_PLUSITEM );
		strMessage.PrintF(_S2(264, szItemName, "[%s +%d]<를> 옮기시겠습니까?"), szItemName, m_pTempItems->Item_Plus);
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );

	}
	else
	{
		ShopToTrade(m_pTempItems->Item_Sum, llTempPrice, nTempSlot);
	}
}

// ----------------------------------------------------------------------------
// Name : DelShopItem()
// Desc : From trade to shop
// ----------------------------------------------------------------------------
void CUIPersonalshopNew::DelShopItem( int nTab, int nIdx, int nUniIndex, SQUAD llCount, int nTradeItemID, int nPackageItemID, int nWhichSlot )
{
	nTempIdx			= nIdx;

	if ( nTab >= INVENTORY_TAB_CASH_1 )
	{
		nTempIdx += ITEM_COUNT_IN_INVENTORY_NORMAL;
	}

	if ( nTab >= INVENTORY_TAB_CASH_2 )
	{
		nTempIdx += ITEM_COUNT_IN_INVENTORY_CASH_1;
	}

	nTempTradeItemID	= nTradeItemID;
	nTempPackageItemID	= nPackageItemID;
	nTempSlot			= nWhichSlot;

	if(nTempTradeItemID >= 0)
	{
		CItems* pItems = m_pIconsTradeItem[nTempTradeItemID]->getItems();

		if (pItems == NULL)
			return;

		m_pTempItems = pItems;
		CItemData*	pItemData = pItems->ItemData;

		// Ask quantity
		if (pItems->Item_Sum > 1)
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			CTString	strMessage;
			const char*	szItemName = _pNetwork->GetItemName(pItems->Item_Index);
			strMessage.PrintF( _S2( 150, szItemName, "몇 개의 %s<를> 옮기시겠습니까?" ), szItemName );

			CmdPersnalShopDelItem* pCmd = new CmdPersnalShopDelItem;
			pCmd->setData(this);
			UIMGR()->GetMsgBoxNumOnly()->SetInfo(pCmd, _S( 263, "상점" ), strMessage, 1, pItems->Item_Sum);
		}
		else
		{
			TradeToShop( pItems->Item_Sum, nWhichSlot );
		}
	}
	else if(nPackageItemID >= 0)
	{
		CItems* pItems = m_pIconsPackageItem[nTempPackageItemID]->getItems();

		if (pItems == NULL)
			return;

		m_pTempItems = pItems;

		CItemData*	pItemData = pItems->ItemData;

		// Ask quantity
		if (pItems->Item_Sum > 1)
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			CTString	strMessage;
			const char*	szItemName = _pNetwork->GetItemName(pItems->Item_Index);
			strMessage.PrintF( _S2( 150, szItemName, "몇 개의 %s<를> 옮기시겠습니까?" ), szItemName );

			CmdPersnalShopDelItem* pCmd = new CmdPersnalShopDelItem;
			pCmd->setData(this);
			UIMGR()->GetMsgBoxNumOnly()->SetInfo(pCmd, _S( 263, "상점" ), strMessage, 1, pItems->Item_Sum);
		}
		else
		{
			TradeToShop(pItems->Item_Sum, nWhichSlot);
		}
	}
	else
	{
		//		TradeToShop(pItems->Item_Sum, nWhichSlot);
	}
}

// ----------------------------------------------------------------------------
// Name : BuyItems()
// Desc :
// ----------------------------------------------------------------------------
void CUIPersonalshopNew::BuyItems()
{
	// Buy nothing
	if( m_nTotalPrice < 0 )
		return;

	// Not enough money
	if(_pNetwork->MyCharacterInfo.money < m_nTotalPrice )
	{
		// Add system message
		CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( _S( 266, "소지금이 부족합니다." ), SYSMSG_ERROR );
		return;
	}

	SendPersonalShopBuy();
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : nCommandCode - 
//			bOK - 
//			&strInput - 
//-----------------------------------------------------------------------------
void CUIPersonalshopNew::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	if( !bOK )
		return;

	switch( nCommandCode )
	{
	case MSGCMD_SHOP_ADD_PLUSITEM:
		{
			if (m_pTempItems != NULL)
			{
				ShopToTrade(1, llTempPrice, nTempSlot);
			}
		}
		break;

	case MSGCMD_SHOP_PRICE:		// 가격 입력.
		{
			char	*pcInput = strInput.str_String;
			int		nLength = strInput.Length();
			int iChar;
			for( iChar = 0; iChar < nLength; ++iChar )
			{
				if( pcInput[iChar] < '0' || pcInput[iChar] > '9' )
					break;
			}

			if( iChar == nLength )
			{
				SQUAD	llCount = _atoi64( pcInput );
				llTempPrice		= llCount;
				AskQuantity();
			}
		}
		break;
	case MSGCMD_PERSONALSHOP_ERROR:
		{
			// Nothing
		}
		break;
	case MSGCMD_SHOP_START:		// 판매 개시
		{			
			SendPersonalShopStart();
		}
		break;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  :
//-----------------------------------------------------------------------------
void CUIPersonalshopNew::ShopToTrade( SQUAD llCount, SQUAD llPrice, int iSlot )
{
	if (m_pTempItems == NULL)
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	m_pArrShop->SetSelectIdx(-1);

	// My Inventory -> Package Slot
	if(iSlot == SLOT_PACKAGE)
	{
		// Find same item in trade slot
		int		iItem;
		for (iItem = 0; iItem < PERSONAL_PACKAGE_SLOT_COL; ++iItem)
		{
			if (m_pIconsPackageItem[iItem]->getItems() != NULL &&
				m_pIconsPackageItem[iItem]->getItems()->Item_UniIndex == m_pTempItems->Item_UniIndex)
				break;
		}

		// If there is same item
		if (iItem < PERSONAL_PACKAGE_SLOT_COL)
		{
			// Check if item is countable
			CItems* pItems = m_pIconsPackageItem[iItem]->getItems();

			if (pItems == NULL)
				return;

			CItemData*	pItemData = pItems->ItemData;

			if( pItemData->GetFlag() & ITEM_FLAG_COUNT )
			{
				if (m_bBuyShop == FALSE)
				{
					// Update count of trade item
					SQUAD	llNewCount = pItems->Item_Sum;
					llNewCount += llCount;
					m_pIconsPackageItem[iItem]->setCount(llNewCount);

					// Update count of shop item
					CItems* pTempItem = m_pIconsShopItem[nTempIdx]->getItems();

					if (pTempItem == NULL)
						return;

					llNewCount = pTempItem->Item_Sum;
					llNewCount -= llCount;
					m_pIconsShopItem[nTempIdx]->setCount(llNewCount);

					if (llNewCount <= 0)
					{
						m_pIconsShopItem[nTempIdx]->Hide(TRUE);
					}

					m_nTotalPrice = CalculateTotalPrice( m_nNumOfItem );
					CTString strTotalPrice;
					strTotalPrice.PrintF("%I64d", m_nTotalPrice);
					pUIManager->InsertCommaToString(strTotalPrice);

					if (m_pTxtTotal != NULL)
					{
						m_pTxtTotal->SetText(strTotalPrice);
						COLOR col = pUIManager->GetNasColor(m_nTotalPrice);
						m_pTxtTotal->setFontColor(col);
					}

					return;
				}
			}
		}

		// Find empty slot
		for( iItem = 0; iItem < PERSONAL_PACKAGE_SLOT_COL; ++iItem )
		{
			if (m_pIconsPackageItem[iItem]->IsEmpty() == true)
				break;
		}

		// If there is not empty slot
		if( iItem == PERSONAL_PACKAGE_SLOT_COL )
		{
			// Add system message
			if( m_bBuyShop )
				pUIManager->GetChattingUI()->AddSysMessage( _S( 267, "최대 10개의 아이템을 구매할 수 있습니다." ), SYSMSG_ERROR );
			else
				pUIManager->GetChattingUI()->AddSysMessage( _S( 268, "최대 10개의 아이템을 판매할 수 있습니다." ), SYSMSG_ERROR );

			return;
		}

		if (m_bBuyShop == FALSE)
		{
			CItems* pCopy = new CItems;
			memcpy(pCopy, m_pTempItems, sizeof(CItems));
			pCopy->Item_Sum = llCount;
			pCopy->Item_Price = llPrice;
			m_pIconsPackageItem[iItem]->setData(pCopy);
			m_vectorSellPackageList.push_back(pCopy);
		}
	}
	// My Inventory -> Trade Slot
	else if (m_pTempItems->Item_Index != PACKAGE_ITEM_INDEX && iSlot == SLOT_TRADE)
	{
		// Find same item in trade slot
		int iItem;
		for( iItem = 0; iItem < PERSONAL_TRADE_SLOT_TOTAL; ++iItem )
		{
			if (m_pIconsTradeItem[iItem]->getItems() != NULL &&
				m_pIconsTradeItem[iItem]->getItems()->Item_UniIndex == m_pTempItems->Item_UniIndex)
				break;
		}

		// If there is same item
		if (iItem < PERSONAL_TRADE_SLOT_TOTAL)
		{
			// Check if item is countable
			CItemData*	pItemData = m_pTempItems->ItemData;

			if (pItemData->GetFlag() & ITEM_FLAG_COUNT)
			{				
				if( !m_bBuyShop )		// 개인상점 구매중 중첩 아이템의 경우 [최대 10개...]란 메시지가 뜨는 버그로 삭제함
					// 이 부분을 막을 경우에. 개인 상점 구매중에 물품 목록의 갯수가 계속 늘어나게 됨.
				{
					CItems* pItems = m_pIconsTradeItem[iItem]->getItems();

					if (pItems == NULL)
						return;

					// 같은 UniIndex 인데 다른 가격이라면 등록하지 않는다.
					if (pItems->Item_Price != llPrice)
						return;

					// Update count of trade item
					SQUAD	llNewCount = pItems->Item_Sum;
					llNewCount += llCount;
					m_pIconsTradeItem[iItem]->setCount(llNewCount);


					// Update count of shop item
					CItems* pTempItem = m_pIconsShopItem[nTempIdx]->getItems();

					if (pTempItem == NULL)
						return;

					llNewCount = pTempItem->Item_Sum;
					llNewCount -= llCount;
					m_pIconsShopItem[nTempIdx]->setCount(llNewCount);

					if( llNewCount <= 0 )
					{
						m_pIconsShopItem[nTempIdx]->Hide(TRUE);
					}

					m_nTotalPrice = CalculateTotalPrice( m_nNumOfItem );
					CTString strTotalPrice;
					strTotalPrice.PrintF("%I64d", m_nTotalPrice);
					pUIManager->InsertCommaToString(strTotalPrice);

					if (m_pTxtTotal != NULL)
						m_pTxtTotal->SetText(strTotalPrice);
					return;
				}
			}
		}

		// Find empty slot
		for( iItem = 0; iItem < PERSONAL_TRADE_SLOT_TOTAL; ++iItem )
		{
			if (m_pIconsTradeItem[iItem]->IsEmpty() == true)
				break;
		}

		// If there is not empty slot
		if( iItem == PERSONAL_TRADE_SLOT_TOTAL )
		{
			// Add system message
			if( m_bBuyShop )
				pUIManager->GetChattingUI()->AddSysMessage( _S( 267, "최대 10개의 아이템을 구매할 수 있습니다." ), SYSMSG_ERROR );
			else
				pUIManager->GetChattingUI()->AddSysMessage( _S( 268, "최대 10개의 아이템을 판매할 수 있습니다." ), SYSMSG_ERROR );

			return;
		}

		if( !m_bBuyShop )
		{
			CItems* pCopy = new CItems;
			memcpy(pCopy, m_pTempItems, sizeof(CItems));
			pCopy->Item_Sum = llCount;
			pCopy->Item_Price = llPrice;
			m_pIconsTradeItem[iItem]->setData(pCopy);
			m_vectorSellItemList.push_back(pCopy);
		}
	}

	//if (m_bBuyShop == false)
	{
		// Update count of shop item
		CItems* pTempItem = m_pIconsShopItem[nTempIdx]->getItems();

		if (pTempItem == NULL)
			return;

		SQUAD	llNewCount = pTempItem->Item_Sum;
		if(llNewCount < llCount)
			return;
		llNewCount -= llCount;
		m_pIconsShopItem[nTempIdx]->setCount(llNewCount);

		if( llNewCount <= 0 )
		{
			// 패키지 아이템의 경우, 패키지 상태를 해제하는 것임?
			if(m_pTempItems->Item_Index == PACKAGE_ITEM_INDEX)
			{
				m_bPackageBuy	= FALSE;				
			}

			m_pIconsShopItem[nTempIdx]->Hide(TRUE);

			if (m_bBuyShop == TRUE)
				m_pIconsShopItem[nTempIdx]->clearIconData();
		}
	}

	m_nTotalPrice = CalculateTotalPrice( m_nNumOfItem );
	CTString strTotalPrice;
	strTotalPrice.PrintF("%I64d", m_nTotalPrice);
	pUIManager->InsertCommaToString(strTotalPrice);

	if (m_pTxtTotal != NULL)
		m_pTxtTotal->SetText(strTotalPrice);
}

//-----------------------------------------------------------------------------
// Purpose: Trade 슬롯에서 상점 슬롯으로.
// Input  :
//-----------------------------------------------------------------------------
void CUIPersonalshopNew::TradeToShop( SQUAD llCount, int iSlot )
{
	SQUAD	llNewCount;
	SQUAD	llPrice = 0;

	// 선택된 아이템이 Trade 창에 있던것.
	if (nTempTradeItemID >= 0)
	{
		m_pTempItems = m_pIconsTradeItem[nTempTradeItemID]->getItems();

		if (m_pTempItems == NULL)
			return;

		llNewCount = m_pTempItems->Item_Sum;

		if( !m_bBuyShop )
		{
			llNewCount -= llCount;
			m_pIconsTradeItem[nTempTradeItemID]->setCount(llNewCount);

			if( llNewCount <= 0 )
			{
				// Rearrange buttons
				for( int iArrItem = nTempTradeItemID; iArrItem < PERSONAL_TRADE_SLOT_TOTAL; ++iArrItem )
				{
					if( ( iArrItem + 1 ) == PERSONAL_TRADE_SLOT_TOTAL )
					{
						m_pIconsTradeItem[iArrItem]->clearIconData();
						break;
					}

					if (m_pIconsTradeItem[iArrItem + 1]->IsEmpty() == true)
					{
						m_pIconsTradeItem[iArrItem]->clearIconData();
						break;
					}

					m_pIconsTradeItem[iArrItem]->setData(m_vectorSellItemList[iArrItem + 1]);
				}

				for (int i = 0; i < m_vectorSellItemList.size(); ++i)
				{
					if (m_vectorSellItemList[i] != NULL &&
						m_vectorSellItemList[i] == m_pTempItems)
					{
						SAFE_DELETE(m_vectorSellItemList[i]);
						m_vectorSellItemList.erase(m_vectorSellItemList.begin() + i);
						break;
					}
				}
			}

			m_pArrTrade->SetSelectIdx(-1);
		}
	}
	// 패키지에서 선택한 경우. m_bBuyShop일 경우 필요 없음)
	else if( !m_bBuyShop && nTempPackageItemID >= 0 )
	{
		// Update count of Package item
		m_pTempItems = m_pIconsPackageItem[nTempPackageItemID]->getItems();

		if (m_pTempItems == NULL)
			return;

		llNewCount = m_pTempItems->Item_Sum;

		//if( !m_bBuyShop )
		//{
		llNewCount -= llCount;
		m_pIconsPackageItem[nTempPackageItemID]->setCount(llNewCount);

		if( llNewCount <= 0 )
		{
			// Rearrange buttons
			for( int iArrItem = nTempPackageItemID; iArrItem < PERSONAL_PACKAGE_SLOT_COL; ++iArrItem )
			{
				if( ( iArrItem + 1 ) == PERSONAL_PACKAGE_SLOT_COL )
				{
					m_pIconsPackageItem[iArrItem]->clearIconData();
					break;
				}

				if (m_pIconsPackageItem[iArrItem + 1]->IsEmpty() == true)
				{
					m_pIconsPackageItem[iArrItem]->clearIconData();
					break;
				}

				m_pIconsPackageItem[iArrItem]->setData(m_vectorSellPackageList[iArrItem + 1]);
			}

			SAFE_DELETE(m_vectorSellPackageList[nTempPackageItemID]);
			m_vectorSellPackageList.erase(m_vectorSellPackageList.begin() + nTempPackageItemID);
		}

		m_pArrPackage->SetSelectIdx(-1);
		//}
	}

	// 판매 모드 일때...
	if( !m_bBuyShop )
	{
		// Update count of shop item
		if (m_pIconsShopItem[nTempIdx]->getItems() != NULL)
		{
			llNewCount = m_pIconsShopItem[nTempIdx]->getItems()->Item_Sum;
			llNewCount += llCount;

			m_pIconsShopItem[nTempIdx]->Hide(FALSE);

			m_pIconsShopItem[nTempIdx]->setCount(llNewCount);
		}		
	}
	// 구매 모드일때...
	else
	{
		CUIManager* pUIManager = CUIManager::getSingleton();

		int nIdx = -1;

		if (m_pTempItems == NULL)
			return;

		FindShopSlot(nIdx, m_pTempItems->Item_UniIndex);

		if( nIdx >= 0 && nIdx < PERSONAL_SHOP_SLOT_MAX )
		{
			// Check if item is countable
			CItemData*	pItemData = m_pTempItems->ItemData;
			if (pItemData != NULL && pItemData->GetFlag() & ITEM_FLAG_COUNT)
			{
				// Update count of trade item
				CItems* pItems = m_pIconsShopItem[nIdx]->getItems();

				if (pItems == NULL)
					return;

				SQUAD	llNewCount = pItems->Item_Sum;
				llNewCount += llCount;

				// 원본의 합계보다 클 수 없다.
				if (pItems->Item_Sum < llNewCount)
					return;

				if (llNewCount <= pItemData->GetStack())
				{
					m_pIconsShopItem[nIdx]->setCount(llNewCount);

					if( !m_bBuyShop )
					{
						// Update count of shop item
						llNewCount = pItems->Item_Sum;
						llNewCount -= llCount;
						m_pIconsShopItem[nIdx]->setCount(llNewCount);

						if( llNewCount <= 0 )
						{
							m_pIconsShopItem[nIdx]->clearIconData();
						}
					}

					m_nTotalPrice = CalculateTotalPrice( m_nNumOfItem );
					CTString strTotalPrice;
					strTotalPrice.PrintF("%I64d", m_nTotalPrice);
					pUIManager->InsertCommaToString(strTotalPrice);

					if (m_pTxtTotal != NULL)
						m_pTxtTotal->SetText(strTotalPrice);
					return;
				}
			}

			return;
		}

		int iEmptyIdx = -1;

		//FindShopSlot( iEmptyRow, iEmptyCol, -1 );
		FindEmptySlot( iEmptyIdx );

		// If there is not empty slot
		if( iEmptyIdx == PERSONAL_SHOP_SLOT_MAX )
		{
			// Add system message
			if( m_bBuyShop )
				pUIManager->GetChattingUI()->AddSysMessage( _S( 267, "최대 10개의 아이템을 구매할 수 있습니다." ), SYSMSG_ERROR );
			else
				pUIManager->GetChattingUI()->AddSysMessage( _S( 268, "최대 10개의 아이템을 판매할 수 있습니다." ), SYSMSG_ERROR );

			return;
		}

		// Update count of shop item
		llNewCount = 0;
		if (m_pIconsShopItem[iEmptyIdx]->getItems() != NULL)
			llNewCount += m_pIconsShopItem[iEmptyIdx]->getItems()->Item_Sum;
		llNewCount += llCount;

		CItems* pCopy = new CItems;
		memcpy(pCopy, m_pTempItems, sizeof(CItems));
		pCopy->InvenIndex = iEmptyIdx;
		pCopy->Item_Sum = llNewCount;
		m_pIconsShopItem[iEmptyIdx]->setData(pCopy, false);
		m_pIconsShopItem[iEmptyIdx]->Hide(FALSE);

		m_nTotalPrice = CalculateTotalPrice( m_nNumOfItem );
		CTString strTotalPrice;
		strTotalPrice.PrintF("%I64d", m_nTotalPrice);
		pUIManager->InsertCommaToString(strTotalPrice);

		if (m_pTxtTotal != NULL)
			m_pTxtTotal->SetText(strTotalPrice);
		return;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  :
//-----------------------------------------------------------------------------
void CUIPersonalshopNew::PackageToShop( bool bAdd )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if(bAdd)
	{		
		for( int i = 0; i < PERSONAL_SHOP_SLOT_MAX; ++i )
		{
			if (m_pIconsShopItem[i]->IsEmpty() == true)
			{
				m_pIconsShopItem[i]->setData(UBET_ITEM, PACKAGE_ITEM_INDEX);
				m_pIconsShopItem[i]->setCount(1);
				m_pIconsShopItem[i]->Hide(FALSE);

				m_bPackageBuy	= bAdd;
				m_nTotalPrice = CalculateTotalPrice( m_nNumOfItem );
				CTString strTotalPrice;
				strTotalPrice.PrintF("%I64d", m_nTotalPrice);
				pUIManager->InsertCommaToString(strTotalPrice);

				if (m_pTxtTotal != NULL)
					m_pTxtTotal->SetText(strTotalPrice);
				return;
			}
		}
	}
	else
	{
		for( int i = 0; i < PERSONAL_SHOP_SLOT_MAX; ++i )
		{
			if (m_pIconsShopItem[i]->IsEmpty() == true)
				continue;
			else
			{
				const int iIndex = m_pIconsShopItem[i]->getIndex();
				if(iIndex == PACKAGE_ITEM_INDEX)
				{
					m_pIconsShopItem[i]->clearIconData();

					m_bPackageBuy	= bAdd;
					m_nTotalPrice = CalculateTotalPrice( m_nNumOfItem );
					CTString strTotalPrice;
					strTotalPrice.PrintF("%I64d", m_nTotalPrice);
					pUIManager->InsertCommaToString(strTotalPrice);

					if (m_pTxtTotal != NULL)
						m_pTxtTotal->SetText(strTotalPrice);
					return;
				}
			}
		}
	}

	m_nTotalPrice = CalculateTotalPrice( m_nNumOfItem );
	CTString strTotalPrice;
	strTotalPrice.PrintF("%I64d", m_nTotalPrice);
	pUIManager->InsertCommaToString(strTotalPrice);

	if (m_pTxtTotal != NULL)
		m_pTxtTotal->SetText(strTotalPrice);
}

//-----------------------------------------------------------------------------
// Purpose: 가격을 계산합니다.
// Input  : iShopID - 
//			bSell - 
// Output : __int64
//-----------------------------------------------------------------------------
__int64 CUIPersonalshopNew::CalculateTotalPrice(int& iCount)
{
	__int64	iTotalPrice	= 0;
	iCount				= 0;

	for( int i = 0; i < PERSONAL_SHOP_SLOT_MAX; ++i )
	{
		if( m_pIconsShopItem[i]->IsEmpty() == true)
			continue;

		CItems* pItems = m_pIconsShopItem[i]->getItems();

		if(pItems->Item_Index == -1 || pItems->Item_UniIndex == -1)
			continue;

		iTotalPrice					+= pItems->Item_Price * pItems->Item_Sum;
		iCount++;
	}
	if( m_bPackageBuy )
	{
		iTotalPrice		+= m_llPackagePrice;
	}

	return iTotalPrice;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : iShopID - 
//			&ID - 
//			iNumOfItem - 
//			bSell - 
//-----------------------------------------------------------------------------
__int64	CUIPersonalshopNew::CalculateItemPrice(int iShopID, const CItemData &ID, int iNumOfItem, bool bSell)
{
	if(iShopID <= 0)
		return 0;

	const CShopData &SD = _pNetwork->GetShopData(iShopID);	
	const int iShopSellRate	= SD.GetSellRate();
	const int iShopBuyRate	= SD.GetBuyRate();
	__int64	iPrice		= 0;

	if( bSell )
		iPrice = (ID.GetPrice() * iShopBuyRate) / 100;
	else
		iPrice = (ID.GetPrice() * iShopSellRate) / 100;

	iPrice *= iNumOfItem;
	return iPrice;
}

//-----------------------------------------------------------------------------
// Purpose: 개인 상점 판매 개시
// Input  : 
//-----------------------------------------------------------------------------
void CUIPersonalshopNew::SendPersonalShopStart()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// FIXME : 중복코드 제거
	if(((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsMoving())
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 746, "이동중에는 개인상점거래를 할 수 없습니다." ), SYSMSG_ERROR );	
		return;
	}

	if(((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsSkilling())
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 747, "스킬 사용중에는 개인상점거래를 할 수 없습니다." ), SYSMSG_ERROR );	
		return;
	}

	if(pUIManager->IsCSFlagOn( CSF_TELEPORT ) )
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 748, "순간 이동중에는 개인상점거래를 할 수 없습니다." ), SYSMSG_ERROR );	
		return;
	}

	if( ((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsSitting() )
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 763, "서 있는 상태에서만 개인상점거래를 개설 할 수 있습니다." ), SYSMSG_ERROR );	
		return;
	}
	// 와일드펫 탑승 상태일 경우 상점을 개설할 수 없음.
	if(_pNetwork->MyCharacterInfo.bWildPetRide == TRUE || pUIManager->IsCSFlagOn(CSF_PETRIDING))
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 5314, "개인 상점을 개설할 수 없는 상태입니다." ), SYSMSG_ERROR );	
		return;
	}
	// 로그의 데스모션은 상점을 개설할 수 없음
	if(_pNetwork->MyCharacterInfo.statusEffect.IsState(EST_ASSIST_FAKEDEATH))
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 5314, "개인 상점을 개설할 수 없는 상태입니다." ), SYSMSG_ERROR );	
		return;
	}

	// 서버로부터 응답 체크가 필요한 메세지

	// 판매시작					: charindex(n) shoptype(c) shopname(str) [normal_count(c) normal_item(v:normal_count) pack_price(ll) pack_count(c) pack_item(v:pack_count)]:client
	// normal_item				: row(c) col(c) item_idx(n) item_db_idx(n) count(ll) price(ll)
	// pack_item				: row(c) col(c) item_idx(n) item_db_idx(n) count(ll)
	SBYTE sbShopType = PST_NOSHOP;

	if (m_pEditName != NULL)
	{
		m_strShopName = m_pEditName->GetString();

		if(m_strShopName.Length() < 4)
		{
			pUIManager->CloseMessageBox(MSGCMD_PERSONALSHOP_ERROR);

			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 757, "개인상점" ), UMBS_OK, UI_PERSONALSHOP, MSGCMD_PERSONALSHOP_ERROR );
			CTString	strMessage = _S( 764, "상점 이름이 너무 짧습니다.\n(한글 2자이상, 영문 4자이상)" );		

			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );

			m_pEditName->ResetString();
			m_pEditName->SetFocus( TRUE );
			return;
		}

		int nIndexBuffer[32];
		char szBuffer[256];

		strcpy ( szBuffer,m_strShopName.str_String );

		// Date : 2005-01-15,   By Lee Ki-hwan
		if( _UIFiltering.Filtering ( szBuffer, nIndexBuffer ) == TRUE ||
			UTIL_HELP()->IsSpecialChar(szBuffer) == true)
		{
			pUIManager->CloseMessageBox(MSGCMD_PERSONALSHOP_ERROR);

			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 757, "개인상점" ), UMBS_OK, UI_PERSONALSHOP, MSGCMD_PERSONALSHOP_ERROR );

			CTString	strMessage = _S ( 751, "상점 이름에 잘못된 문자가 포함되어 있습니다." );

			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );

			m_pEditName->SetFocus( TRUE );

			return;
		}
	}

	int iNormalItemCount	= 0;
	int iPackageItemCount	= 0;
	int iItem, iCol;
	// Trade Slot(5x2)
	for( iItem = 0; iItem < PERSONAL_TRADE_SLOT_TOTAL; ++iItem )
	{
		if (m_pIconsTradeItem[iItem]->IsEmpty() == true)
			continue;
		iNormalItemCount++;
	}

	// Package Slot(5x1)
	for( iCol = 0; iCol < PERSONAL_PACKAGE_SLOT_COL; ++iCol)
	{
		if (m_pIconsPackageItem[iCol]->IsEmpty() == true)
			continue;
		iPackageItemCount++;
	}

	if (iPackageItemCount > 0 && m_bPremium && m_pEditPackagePrice != NULL)
	{
		CTString strPrice;
		strPrice = m_pEditPackagePrice->GetString();

		if(strPrice == "" || strPrice.Length() <= 0)
		{
			pUIManager->CloseMessageBox(MSGCMD_PERSONALSHOP_ERROR);
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 757, "개인상점" ), UMBS_OK,	
				UI_PERSONALSHOP, MSGCMD_PERSONALSHOP_ERROR );
			CTString	strMessage = _S( 765, "패키지 가격을 입력해주십시오." );		
			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );

			m_pEditPackagePrice->ResetString();
			return;
		}
		char	*pcInput	= strPrice.str_String;
		int		nLength		= strPrice.Length();
		int		iChar;
		for( iChar = 0; iChar < nLength; ++iChar )
		{
			if( pcInput[iChar] < '0' || pcInput[iChar] > '9' )
				break;
		}

		if( iChar == nLength )
		{
			SQUAD	llCount		= _atoi64( pcInput );
			m_llPackagePrice	= llCount;
		}
	}

	sbShopType |= PST_SELL;

	if(m_bPremium)
	{
		sbShopType |= PST_PREMIUM;	
	}

	if(iPackageItemCount > 0)
	{
		sbShopType |= PST_PACKAGE;	
	}

	//	UBYTE Maintype;

	//	Maintype = m_bCashPersonShop ? MSG_EXTEND:MSG_PERSONALSHOP;

	CNetworkMessage nmPersonalShop(m_bCashPersonShop ? MSG_EXTEND:MSG_PERSONALSHOP);

	// 판매대행 상인
	if(m_bCashPersonShop)
	{
		nmPersonalShop << (LONG)MSG_EX_ALTERNATE_MERCHANT;
		nmPersonalShop << (UBYTE)MSG_ALTERNATEMERCHANT_SELL_START;

	}else
	{
		//		Maintype = MSG_PERSONALSHOP;
		nmPersonalShop << (UBYTE)MSG_PERSONALSHOP_SELL_START;

	}	

	//	if(m_bCashPersonShop) nmPersonalShop << MSG_EX_ALTERNATE_MERCHANT;

	//	nmPersonalShop << Subtype;
	nmPersonalShop << _pNetwork->MyCharacterInfo.index;
	nmPersonalShop << (SBYTE)sbShopType;
	nmPersonalShop << m_strShopName;

	// Normal Item List
	nmPersonalShop << (SBYTE)iNormalItemCount;	

	//row(c) col(c) item_idx(n) item_db_idx(n) count(ll) price(ll)
	for( iItem = 0; iItem < PERSONAL_TRADE_SLOT_TOTAL; ++iItem )
	{
		if (m_pIconsTradeItem[iItem]->IsEmpty() == true)
			continue;

		nmPersonalShop << (SWORD)m_vectorSellItemList[iItem]->Item_Tab;
		nmPersonalShop << (SWORD)m_vectorSellItemList[iItem]->InvenIndex;
		nmPersonalShop << (LONG)m_vectorSellItemList[iItem]->Item_UniIndex;
		nmPersonalShop << (LONG)m_vectorSellItemList[iItem]->Item_Index;
		nmPersonalShop << m_vectorSellItemList[iItem]->Item_Sum;
		nmPersonalShop << m_vectorSellItemList[iItem]->Item_Price;
	}

	// Package Item List
	nmPersonalShop << m_llPackagePrice;
	nmPersonalShop << (SBYTE)iPackageItemCount;

	//row(c) col(c) item_idx(n) item_db_idx(n) count(ll)
	for( iCol = 0; iCol < PERSONAL_PACKAGE_SLOT_COL; ++iCol)
	{
		if (m_pIconsPackageItem[iCol]->IsEmpty() == true)
			continue;

		nmPersonalShop << (SWORD)m_vectorSellPackageList[iCol]->Item_Tab;
		nmPersonalShop << (SWORD)m_vectorSellPackageList[iCol]->InvenIndex;
		nmPersonalShop << (LONG)m_vectorSellPackageList[iCol]->Item_UniIndex;
		nmPersonalShop << (LONG)m_vectorSellPackageList[iCol]->Item_Index;
		nmPersonalShop << m_vectorSellPackageList[iCol]->Item_Sum;
	}

	_pNetwork->SendToServerNew(nmPersonalShop);	
	_pNetwork->m_iNetworkResponse[MSG_PERSONALSHOP]		= 1;
}

//-----------------------------------------------------------------------------
// Purpose: 개인 상점에서 중단.
// Input  : 
//-----------------------------------------------------------------------------
void CUIPersonalshopNew::SendPersonalShopStop()
{
	// FIXME : 중복코드 제거
	if(((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsActionSitting())
	{
		return;
	}

	CNetworkMessage nmPersonalShop(MSG_PERSONALSHOP);
	nmPersonalShop << (UBYTE)MSG_PERSONALSHOP_CHANGE;	
	nmPersonalShop << _pNetwork->MyCharacterInfo.index;
	nmPersonalShop << (SBYTE)PST_NOSHOP;
	_pNetwork->SendToServerNew(nmPersonalShop);	
}

//-----------------------------------------------------------------------------
// Purpose: 개인 상점에서 아이템 사기
// Input  : 
//-----------------------------------------------------------------------------
void CUIPersonalshopNew::SendPersonalShopBuy()
{
	SBYTE sbBuyPack = m_bPackageBuy;
	// 구매요청					: charindex(n) pack_buy(c) normal_count(c) ([normal_item_index(n) normal_item_count(ll)]:normal_count)
	CNetworkMessage nmPersonalShop(MSG_PERSONALSHOP);
	nmPersonalShop << (UBYTE)MSG_PERSONALSHOP_BUY;
	nmPersonalShop << (ULONG)m_nSelectedChaID;	
	nmPersonalShop << sbBuyPack;

	int iNormalItemCount	= 0;

	int		i;
	for( i = 0; i < PERSONAL_SHOP_SLOT_MAX; ++i )
	{
		if (m_pIconsShopItem[i]->IsEmpty() == true ||
			m_pIconsShopItem[i]->getIndex() == PACKAGE_ITEM_INDEX)
			continue;
		iNormalItemCount++;
	}
	nmPersonalShop << (SBYTE)iNormalItemCount;

	for( i = 0; i < PERSONAL_SHOP_SLOT_MAX; ++i )
	{
		if (m_pIconsShopItem[i]->IsEmpty() == true ||
			m_pIconsShopItem[i]->getIndex() == PACKAGE_ITEM_INDEX)
			continue;

		if (m_pIconsShopItem[i]->getItems() == NULL)
			continue;

		nmPersonalShop << (LONG)m_pIconsShopItem[i]->getItems()->Item_UniIndex;
		nmPersonalShop << m_pIconsShopItem[i]->getItems()->Item_Sum;
	}

	_pNetwork->SendToServerNew(nmPersonalShop);
}
//-----------------------------------------------------------------------------
// Purpose: 판매 대행 상점에서 아이템 사기
// Input  : 
//-----------------------------------------------------------------------------
void CUIPersonalshopNew::SendCashPersonShopBuy()
{
	SBYTE sbBuyPack = m_bPackageBuy;
	// 구매요청					: charindex(n) pack_buy(c) normal_count(c) ([normal_item_index(n) normal_item_count(ll)]:normal_count)
	CNetworkMessage nmPersonalShop(MSG_EXTEND);
	nmPersonalShop << (LONG)MSG_EX_ALTERNATE_MERCHANT;
	nmPersonalShop << (UBYTE)MSG_ALTERNATEMERCHANT_BUY;
	nmPersonalShop << (ULONG)m_nSelectedChaID;	
	nmPersonalShop << sbBuyPack;

	int iNormalItemCount	= 0;
	int	i;

	for( i = 0; i < PERSONAL_SHOP_SLOT_MAX; ++i )
	{
		if (m_pIconsShopItem[i]->IsEmpty() == true)
			continue;
		iNormalItemCount++;
	}
	nmPersonalShop << (SBYTE)iNormalItemCount;

	for( i = 0; i < PERSONAL_SHOP_SLOT_MAX; ++i )
	{
		if (m_pIconsShopItem[i]->IsEmpty() == true)
			continue;

		if (m_pIconsShopItem[i]->getItems() == NULL)
			continue;

		nmPersonalShop << (LONG)m_pIconsShopItem[i]->getItems()->Item_UniIndex;
		nmPersonalShop << m_pIconsShopItem[i]->getItems()->Item_Sum;
	}

	_pNetwork->SendToServerNew(nmPersonalShop);
}

//-----------------------------------------------------------------------------
// Purpose: 개인 상점 물품 목록 얻기.
// Input  : 
//-----------------------------------------------------------------------------
void CUIPersonalshopNew::ReceivePersonalShopItemList(int iChaIndex, CNetworkMessage *istr) 
{
	// 판매리스트 요청			: charindex(n) [ shoptype(c) normal_count(c) normal_item(v:normal_count) pack_price(ll) pack_count(c) pack_item(v:pack_count)]:server
	// normal_item				: item_idx(n) item_db_idx(n) plus(n) flag(n) option_count(c) ([option_type(c) option_level(c)]:option_count) count(ll) price(ll)
	// pack_item				: item_idx(n) item_db_idx(n) plus(n) flag(n) option_count(c) ([option_type(c) option_level(c)]:option_count) count(ll)

	CUIManager* pUIManager = CUIManager::getSingleton();

	ClearItems();

	SBYTE	sbShopType;
	SBYTE	sbNormalCount;
	SBYTE	sbPackageCount;

	//m_nSelectedChaID = iChaIndex;

	(*istr) >> sbShopType
		>> sbNormalCount;

	if(sbShopType & PST_PREMIUM)
	{
		m_bPremium = TRUE;
		m_pChkPremium->SetCheck(TRUE);
	}

	for(int i = 0; i < sbNormalCount; ++i)
	{
		LONG	lIndex;
		LONG	lUniIndex;
		ULONG	ulPlus,ulPlus2;
		ULONG	ulFlag;
		SLONG	slUsed;
		SLONG	slUsed2;
		SBYTE	sbOptionCount;
		SBYTE	sbOptionType;
		LONG	lOptionLevel;
		SQUAD	llCount;
		SQUAD	llPrice;
#ifdef	DURABILITY
		LONG	nDurabilityNow;
		LONG	nDurabilityMax;
#endif	// DURABILITY

		(*istr) >> lUniIndex;
		(*istr) >> lIndex;		
		(*istr) >> ulPlus;
		(*istr) >> ulFlag;
#ifdef	DURABILITY
		(*istr) >> nDurabilityNow;
		(*istr) >> nDurabilityMax;
#endif
		(*istr) >> sbOptionCount;

		CItems* TempItem = new CItems(lIndex);

		TempItem->InitOptionData();

		//레어 아이템이면...
		if( _pNetwork->GetItemData(lIndex)->GetFlag() & ITEM_FLAG_RARE )
		{
			//옵션 개수가 0이면 미감정 레어아이템
			if( sbOptionCount ==0)
				TempItem->SetRareIndex(0);
			//감정된 레어 아이템
			else
				pUIManager->SetRareOption(istr, *TempItem);
		}
		//레어 아이템이 아니면...
		else
		{
			LONG lOriginOptionVar = ORIGIN_VAR_DEFAULT;

			for( SBYTE sbOption = 0; sbOption < sbOptionCount; sbOption++ )
			{
				(*istr) >> sbOptionType;
				(*istr) >> lOptionLevel;

				if ( _pNetwork->GetItemData(lIndex)->GetFlag() & ITEM_FLAG_ORIGIN )
				{
					(*istr) >> lOriginOptionVar;
				}

				TempItem->SetOptionData(sbOption, sbOptionType, lOptionLevel, lOriginOptionVar);
			}
		}
		(*istr) >> llCount;
		(*istr) >> llPrice;
		(*istr) >> slUsed;
		(*istr) >> slUsed2;

		if (_pNetwork->GetItemData(lIndex)->GetFlag() & ITEM_FLAG_ORIGIN)
		{
			SBYTE sbBelong, sbSkillcont;
			LONG lSkillIndex;
			SBYTE sbSkillLevel;

			(*istr) >> sbBelong;
			(*istr) >> sbSkillcont;

			TempItem->SetItemBelong(sbBelong);

			for (SBYTE sbSkillpos = 0; sbSkillpos < sbSkillcont; sbSkillpos++)
			{
				(*istr) >> lSkillIndex;
				(*istr) >> sbSkillLevel;

				TempItem->SetItemSkill(sbSkillpos, lSkillIndex, sbSkillLevel);
			}
		}

		(*istr) >> ulPlus2;
		TempItem->SetItemPlus2(ulPlus2);
		TempItem->InitSocketInfo();

		if( _pNetwork->GetItemData(lIndex)->GetFlag() & ITEM_FLAG_SOCKET )
		{
			SBYTE	sbSocketCreateCount = 0;
			SBYTE	sbSocketCount = 0;
			LONG	lSocketJewelIndex = 0;

			LONG	lSocketInfo[JEWEL_MAX_COUNT] = {-1,};
			int	i;

			for (i = 0; i < JEWEL_MAX_COUNT; i++)
			{
				(*istr) >> lSocketInfo[i];
				if (lSocketInfo[i] >= 0)
					sbSocketCreateCount++;
			}

			TempItem->SetSocketCount( sbSocketCreateCount );	

			for (i = 0; i < JEWEL_MAX_COUNT; i++)
			{
				if (lSocketInfo[i] >= 0)
					TempItem->SetSocketOption( i, lSocketInfo[i] );
			}
		}

		TempItem->SetData(lIndex, lUniIndex, 0, i, ulPlus, ulFlag, -1, slUsed, slUsed2, -1, llCount);
		TempItem->SetPrice(llPrice);
#ifdef	DURABILITY
		TempItem->SetDurability(nDurabilityNow, nDurabilityMax);
#endif	// DURABILITY

		if(_pNetwork->GetItemData(lIndex)->GetType() == CItemData::ITEM_ACCESSORY &&
			_pNetwork->GetItemData(lIndex)->GetSubType() == CItemData::ACCESSORY_WILDPET)
		{
			_pNetwork->SendPetitemInfo(iChaIndex, ulPlus);
		}

		m_vectorSellItemList.push_back(TempItem);
	}

	if(sbShopType & PST_PACKAGE)
	{
		(*istr) >> m_llPackagePrice;
		(*istr) >> sbPackageCount;	
		for(int i = 0; i < sbPackageCount; ++i)
		{
			LONG	lIndex;
			LONG	lUniIndex;
			ULONG	ulPlus,ulPlus2 = 0;
			ULONG	ulFlag;
			SLONG	slUsed;
			SLONG	slUsed2;
			SBYTE	sbOptionCount;
			SBYTE	sbOptionType;
			LONG	lOptionLevel;
			SQUAD	llCount;
#ifdef	DURABILITY
			LONG	nDurabilityNow;
			LONG	nDurabilityMax;
#endif	// DURABILITY

			(*istr) >> lUniIndex;
			(*istr) >> lIndex;		
			(*istr) >> ulPlus;
			(*istr) >> ulFlag;
#ifdef	DURABILITY
			(*istr) >> nDurabilityNow;
			(*istr) >> nDurabilityMax;
#endif
			(*istr) >> sbOptionCount;

			CItems*	TempItem = new CItems(lIndex);
			TempItem->InitOptionData();

			if( _pNetwork->GetItemData(lIndex)->GetFlag() & ITEM_FLAG_RARE )
			{
				//옵션 개수가 0이면 미감정 레어아이템
				if( sbOptionCount ==0)
					TempItem->SetRareIndex(0);
				//감정된 레어 아이템
				else
					pUIManager->SetRareOption(istr, *TempItem);
			}
			else
			{
				LONG lOriginOptionVar = ORIGIN_VAR_DEFAULT;

				for( SBYTE sbOption = 0; sbOption < sbOptionCount; sbOption++ )
				{
					(*istr) >> sbOptionType;
					(*istr) >> lOptionLevel;

					if ( _pNetwork->GetItemData(lIndex)->GetFlag() & ITEM_FLAG_ORIGIN )
					{
						(*istr) >> lOriginOptionVar;
					}

					TempItem->SetOptionData( sbOption, sbOptionType, lOptionLevel, lOriginOptionVar );
				}
			}
			(*istr) >> llCount;
			(*istr) >> slUsed;
			(*istr) >> slUsed2;

			if (_pNetwork->GetItemData(lIndex)->GetFlag() & ITEM_FLAG_ORIGIN)
			{
				SBYTE sbBelong, sbSkillcont;
				LONG lSkillIndex;
				SBYTE sbSkillLevel;

				(*istr) >> sbBelong;
				(*istr) >> sbSkillcont;

				TempItem->SetItemBelong(sbBelong);

				for (SBYTE sbSkillpos = 0; sbSkillpos < sbSkillcont; sbSkillpos++)
				{
					(*istr) >> lSkillIndex;
					(*istr) >> sbSkillLevel;

					TempItem->SetItemSkill(sbSkillpos, lSkillIndex, sbSkillLevel);
				}
			}

			(*istr) >> ulPlus2;
			TempItem->SetItemPlus2(ulPlus2);
			TempItem->InitSocketInfo();

			if( _pNetwork->GetItemData(lIndex)->GetFlag() & ITEM_FLAG_SOCKET )
			{
				SBYTE	sbSocketCreateCount = 0;
				SBYTE	sbSocketCount = 0;
				LONG	lSocketJewelIndex = 0;
				LONG	lSocketOptionIndex = 0;
				SBYTE	sbSocketOptionLevel = 0;

				TempItem->InitSocketInfo();

				LONG	lSocketInfo[JEWEL_MAX_COUNT] = {-1,};
				int	i;

				for (i = 0; i < JEWEL_MAX_COUNT; i++)
				{
					(*istr) >> lSocketInfo[i];
					if (lSocketInfo[i] >= 0)
						sbSocketCreateCount++;
				}

				TempItem->SetSocketCount( sbSocketCreateCount );	

				for (i = 0; i < JEWEL_MAX_COUNT; i++)
				{
					if (lSocketInfo[i] >= 0)
						TempItem->SetSocketOption( i, lSocketInfo[i] );
				}
			}

			TempItem->SetData(lIndex, lUniIndex, -1, -1, ulPlus, ulFlag, -1, slUsed, slUsed2, -1, llCount);
#ifdef	DURABILITY
			TempItem->SetDurability(nDurabilityNow, nDurabilityMax);
#endif	// DURABILITY

			if(_pNetwork->GetItemData(lIndex)->GetType() == CItemData::ITEM_ACCESSORY &&
				_pNetwork->GetItemData(lIndex)->GetSubType() == CItemData::ACCESSORY_WILDPET)
			{
				_pNetwork->SendPetitemInfo(iChaIndex, ulPlus);
			}

			m_vectorSellPackageList.push_back(TempItem);
		}

		m_pBtnPackOk->SetEnable(TRUE);
	}

	m_pBtnCancel->SetVisible(TRUE);
	m_pBtnCancel->SetEnable(TRUE);
	m_pBtnBuy->SetVisible(TRUE);
	m_pBtnBuy->SetEnable(TRUE);
	
	m_pChkPremium->Hide(TRUE);
	m_pChkPremium->SetEnable(FALSE);
	
	m_pEditName->SetEnable( FALSE );
	m_pEditPackagePrice->SetEnable( FALSE );
	m_nSelectedChaID = iChaIndex;

	pUIManager->CloseMessageBox( MSGCMD_DROPITEM );

	PrepareBuyShop();
}

//-----------------------------------------------------------------------------
// Purpose: 개인 상점 물품 목록 갱신
// Input  : 
//-----------------------------------------------------------------------------
void CUIPersonalshopNew::ReceivePersonalShopItemUpdate(CNetworkMessage *istr)
{
	SBYTE	sbBuyPack		= 0;
	LONG	lChaIndex		= 0;
	SBYTE	sbNormalCount	= 0;

	// 구매요청					: charindex(n) pack_buy(c) normal_count(c) ([normal_item_index(n) normal_item_count(ll)]:normal_count)
	(*istr) >> lChaIndex;	
	(*istr) >> sbBuyPack;
	(*istr) >> sbNormalCount;

	// Package 상품을 팔았을때...
	if(sbBuyPack == 1)
	{
		for(int iCol = 0; iCol < PERSONAL_PACKAGE_SLOT_COL; ++iCol)
		{
			m_pIconsPackageItem[iCol]->clearIconData();
		}

		m_llPackagePrice	= 0;
		
		if (m_pTxtPackagePrice != NULL)
			m_pTxtPackagePrice->SetText(CTString("0"));		

		int nCnt = m_vectorSellPackageList.size();

		for (int i = 0; i < nCnt; ++i)
		{
			SAFE_DELETE(m_vectorSellPackageList[i]);
		}

		m_vectorSellPackageList.clear();
	}

	RefreshPlayerItem();

	LONG	lItemIndex		= -1;
	SQUAD	llItemCount		= 0;
	for( int i = 0; i < sbNormalCount; ++i)
	{
		(*istr) >> lItemIndex;
		(*istr) >> llItemCount;

		for( int iItem = 0; iItem < PERSONAL_TRADE_SLOT_TOTAL; ++iItem )
		{
			if (m_pIconsTradeItem[iItem]->IsEmpty() == true)
				continue;

			if (m_pIconsTradeItem[iItem]->getItems() == NULL)
				continue;

			if (m_pIconsTradeItem[iItem]->getItems()->Item_UniIndex == lItemIndex)
			{
				SQUAD llNewItemCount	= m_pIconsTradeItem[iItem]->getItems()->Item_Sum;
				llNewItemCount			-= llItemCount;

				if(llNewItemCount <= 0)
				{
					m_pIconsTradeItem[iItem]->clearIconData();

					vec_Items_iter iter = m_vectorSellItemList.begin();
					vec_Items_iter eiter = m_vectorSellItemList.end();

					for (; iter != eiter; ++iter)
					{
						if ((*iter)->Item_UniIndex == lItemIndex)
						{
							SAFE_DELETE(*iter);
							iter = m_vectorSellItemList.erase(iter);
							break;
						}
					}
				}
				else
				{
					m_pIconsTradeItem[iItem]->setCount(llNewItemCount);
				}
				break;
			}
		}
	}

	// 상점 윗부분과 아래 인벤토리에 해당하는 부분의 중복을 제거하기 위한 부분.
	// FIXME : 느릴거 같은 코드.  최적화 필요.
	for( int iItem = 0; iItem < PERSONAL_TRADE_SLOT_TOTAL; ++iItem )
	{
		if (m_pIconsTradeItem[iItem]->IsEmpty() == true)
			continue;

		if (m_pIconsTradeItem[iItem]->getItems() == NULL)
			continue;

		SQUAD llItemCount	= m_pIconsTradeItem[iItem]->getItems()->Item_Sum;
		int iItemUniIndex	= m_pIconsTradeItem[iItem]->getItems()->Item_UniIndex;

		// 빈 공간에 추가함.
		for( int i = 0; i < PERSONAL_SHOP_SLOT_MAX; ++i )
		{
			if (m_pIconsShopItem[i]->IsEmpty() == true)
				continue;

			if (m_pIconsShopItem[i]->getItems() == NULL)
				continue;

			if( m_pIconsShopItem[i]->getItems()->Item_UniIndex == iItemUniIndex )
			{
				SQUAD llNewItemCount	= m_pIconsShopItem[i]->getItems()->Item_Sum;
				llNewItemCount			-= llItemCount;
				m_pIconsShopItem[i]->setCount(llNewItemCount);

				if(llItemCount <= 0)
				{
					m_pIconsShopItem[i]->Hide(TRUE);
				}

				break;
			}
		}
	}

	//RefreshPlayerItem();

	m_nTotalPrice = CalculateTotalPrice( m_nNumOfItem );
	CTString strTotalPrice;
	strTotalPrice.PrintF("%I64d", m_nTotalPrice);
	UIMGR()->InsertCommaToString(strTotalPrice);

	if (m_pTxtTotal != NULL)
		m_pTxtTotal->SetText(strTotalPrice);
}

//-----------------------------------------------------------------------------
// Purpose: 개인 상점 물품 목록 얻기.
// Input  : 
//-----------------------------------------------------------------------------
void CUIPersonalshopNew::SendPersonalOpenShop(int iChaIndex)
{
	CNetworkMessage nmPersonalShop(MSG_PERSONALSHOP);
	nmPersonalShop << (UBYTE)MSG_PERSONALSHOP_SELL_LIST;
	nmPersonalShop << (ULONG)iChaIndex;
	_pNetwork->SendToServerNew(nmPersonalShop);
}

//-----------------------------------------------------------------------------
// Purpose: 판매 대행 상점 물품 목록 얻기.
// Input  : 
//-----------------------------------------------------------------------------
void CUIPersonalshopNew::SendCashPersonOpenShop(int iChaIndex)
{
	CNetworkMessage nmPersonalShop(MSG_EXTEND);
	nmPersonalShop << (LONG)MSG_EX_ALTERNATE_MERCHANT;
	nmPersonalShop << (UBYTE)MSG_ALTERNATEMERCHANT_SELL_LIST;
	nmPersonalShop << (ULONG)iChaIndex;
	_pNetwork->SendToServerNew(nmPersonalShop);
}

//-----------------------------------------------------------------------------
// Purpose: 패키지 슬롯을 클리어함.
// Input  : 
//-----------------------------------------------------------------------------
void CUIPersonalshopNew::ClearPackageSlot()
{
	// Package Slot(5x1)
	for(int iCol = 0; iCol < PERSONAL_PACKAGE_SLOT_COL; ++iCol)
	{
		if (m_pIconsPackageItem[iCol]->IsEmpty() == true)
			continue;

		if (m_pIconsPackageItem[iCol]->getItems() == NULL)
			continue;

		const SQUAD llOldCount	= m_pIconsPackageItem[iCol]->getItems()->Item_Sum;

		int nTab = m_pIconsPackageItem[iCol]->getItems()->Item_Tab;
		int nIdx = m_pIconsPackageItem[iCol]->getItems()->InvenIndex;

		if ( nTab >= INVENTORY_TAB_CASH_1 )
		{
			nIdx += ITEM_COUNT_IN_INVENTORY_NORMAL;
		}

		if ( nTab >= INVENTORY_TAB_CASH_2 )
		{
			nIdx += ITEM_COUNT_IN_INVENTORY_CASH_1;
		}

		if (nIdx < 0 || nIdx >= PERSONAL_SHOP_SLOT_MAX)
		{
			m_pIconsPackageItem[iCol]->clearIconData();
			continue;
		}

		SQUAD llNew				= m_pIconsShopItem[nIdx]->getItems()->Item_Sum + llOldCount;

		m_pIconsShopItem[nIdx]->Hide(FALSE);
		m_pIconsShopItem[nIdx]->setCount(llNew);

		m_pIconsPackageItem[iCol]->clearIconData();
	}

	for (int i = 0; i < m_vectorSellPackageList.size(); ++i)
		SAFE_DELETE(m_vectorSellPackageList[i]);

	m_vectorSellPackageList.clear();
	m_pEditPackagePrice->ResetString();
	m_llPackagePrice = 0;
}

//-----------------------------------------------------------------------------
// Purpose: 상점의 상태를 변경합니다.
// Input  : 
//-----------------------------------------------------------------------------
void CUIPersonalshopNew::ChangeShopState(bool bShopStart)
{
	m_bShopStart = bShopStart;

	if (m_bShopStart == false)
	{
		m_pBtnStart->SetText(_S(742, "판매개시"));
	}
	else
	{
		m_pBtnStart->SetText(_S(743, "판매중지"));
	}

	//m_btnPackageOK.SetEnable(m_bPremium & !m_bShopStart);
	//m_btnPackageCancel.SetEnable(m_bPremium & !m_bShopStart);
	//m_pChkPremium->SetEnable(!m_bShopStart);
	m_pEditPackagePrice->SetEnable(!m_bShopStart);
	m_pEditName->SetEnable(!m_bShopStart);
}

//-----------------------------------------------------------------------------
// Purpose: 인덱스에 해당하는 상점 슬롯을 찾음.
// Input  : 
//-----------------------------------------------------------------------------
void CUIPersonalshopNew::FindShopSlot( int& nIdx, int iVirtualIdx )
{
	// 빈 공간에 추가함.
	for( nIdx = 0; nIdx < PERSONAL_SHOP_SLOT_MAX; ++nIdx )
	{
		if (m_pIconsShopItem[nIdx]->IsEmpty() == false && m_pIconsShopItem[nIdx]->getItems() &&
			m_pIconsShopItem[nIdx]->getItems()->Item_UniIndex == iVirtualIdx)
			return;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 비어있는 상점 슬롯을 찾음.
// Input  : 
//-----------------------------------------------------------------------------
void CUIPersonalshopNew::FindEmptySlot( int& nIdx )
{
	// 빈 공간에 추가함.
	for( nIdx = 0; nIdx < PERSONAL_SHOP_SLOT_MAX; ++nIdx )
	{
		if (m_pIconsShopItem[nIdx]->IsEmpty() == true)
			return;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 구매중에 판매자가 개인상점 닫았다. 구매창을 닫는다.
// Input  : 
//-----------------------------------------------------------------------------
bool CUIPersonalshopNew::EndBuyShop(int nChaID)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if (pUIManager->IsCSFlagOn( CSF_PERSONALSHOP ) || IsVisible())
	{ //구매자 상태이고
		if (m_nSelectedChaID == nChaID)
		{ // 구매하려는 판매 상점이 닫히면 구매창을 닫자
			ResetShop();
			pUIManager->SetCSFlagOff( CSF_PERSONALSHOP );
			return TRUE;
		}
	}

	return FALSE;
}

void CUIPersonalshopNew::AddItemCallback()
{
	SQUAD	llCount = UIMGR()->GetMsgBoxNumOnly()->GetData();

	if( m_bBuyShop )
	{
		if( llCount > 0 )
		{
			ShopToTrade(llCount, llTempPrice, nTempSlot);
		}
	}
	else
	{
		if (llCount > 0 && llCount <= m_pTempItems->Item_Sum)
		{
			ShopToTrade(llCount, llTempPrice, nTempSlot);
		}
	}
}

void CUIPersonalshopNew::DelItemCallback()
{
	SQUAD	llCount = UIMGR()->GetMsgBoxNumOnly()->GetData();

	if (llCount > 0 && llCount <= m_pTempItems->Item_Sum)
		TradeToShop( llCount, nTempSlot );
}

void CUIPersonalshopNew::clearContainer()
{
	int		i;

	for (i = 0; i < m_vectorSellItemList.size(); ++i)
		SAFE_DELETE(m_vectorSellItemList[i]);

	m_vectorSellItemList.clear();

	for (i = 0; i < m_vectorSellPackageList.size(); ++i)
		SAFE_DELETE(m_vectorSellPackageList[i]);

	m_vectorSellPackageList.clear();
}

WMSG_RESULT CUIPersonalshopNew::LButtonUp( UINT16 x, UINT16 y )
{
	WMSG_RESULT ret = CUIBase::LButtonUp(x, y);

	UIMGR()->ResetHoldBtn();

	return ret;
}

WMSG_RESULT CUIPersonalshopNew::OnLButtonDown(UINT16 x, UINT16 y)
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if (m_pTxtTitle && m_pTxtTitle->IsInside(x, y))
	{
		m_bTitleClick = true;

		ox = x;
		oy = y;
	}

	if (m_pEditPackagePrice != NULL)
		m_pEditPackagePrice->SetFocus(NULL);

	if (m_pEditName != NULL)
		m_pEditName->SetFocus(NULL);

	return CUIBase::OnLButtonDown(x, y);
}

WMSG_RESULT CUIPersonalshopNew::OnLButtonUp(UINT16 x, UINT16 y)
{
	if (m_bHide)
		return WMSG_FAIL;

	m_bTitleClick = false;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	return CUIBase::OnLButtonUp(x, y);
}

WMSG_RESULT CUIPersonalshopNew::OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg)
{
	if (m_bHide)
		return WMSG_FAIL;

	if (m_bTitleClick && (pMsg->wParam & MK_LBUTTON))
	{
		int	ndX = x - ox;
		int	ndY = y - oy;

		ox = x;
		oy = y;

		Move(ndX, ndY);

		return WMSG_SUCCESS;
	}

	if (IsInside(x, y) == FALSE)
	{
		m_bTitleClick = false;
		return WMSG_FAIL;
	}

	CUIManager::getSingleton()->SetMouseCursorInsideUIs();

	return WMSG_FAIL;
}

void CUIPersonalshopNew::CloseMsg()
{
	CUIManager* pUIManager = UIMGR();

	pUIManager->CloseMessageBox( MSGCMD_SHOP_ADD_PLUSITEM );
	pUIManager->CloseMessageBox( MSGCMD_SHOP_START );
	pUIManager->CloseMessageBox( MSGCMD_SHOP_PRICE );
	pUIManager->CloseMessageBox( MSGCMD_PERSONALSHOP_ERROR );
	pUIManager->CloseMessageBox( MSGCMD_SHOP_START );
	pUIManager->GetMsgBoxNumOnly()->CloseBox();
}

BOOL CUIPersonalshopNew::IsEditBoxFocused()
{
	if (m_pEditPackagePrice != NULL && m_pEditPackagePrice->IsFocused())
		return TRUE;

	if (m_pEditName != NULL && m_pEditName->IsFocused())
		return TRUE;

	return FALSE;
}

void CUIPersonalshopNew::proc_command( eCMD_TYPE eCmd, eSlotType eSlot )
{
	switch (eCmd)
	{
	case CMD_DBLCLICK:
		proc_dblclick(eSlot);
		break;
	case CMD_LBUTTONUP:
		proc_lbutton_up(eSlot);
		break;
	}
}

void CUIPersonalshopNew::proc_dblclick( eSlotType type )
{
	switch (type)
	{
	case SLOT_TRADE:
		{
			if (m_pArrTrade != NULL)
			{
				CloseMsg();

				int idx = m_pArrTrade->GetSelectIdx();

				if (idx < 0)
					return;

				if (m_pIconsTradeItem[idx]->IsEmpty() == true)
					return;

				CItems* pItems = m_pIconsTradeItem[idx]->getItems();

				if (pItems == NULL)
					return;

				DelShopItem( pItems->Item_Tab, pItems->InvenIndex,
					pItems->Item_UniIndex, pItems->Item_Sum,
					idx, -1, SLOT_TRADE );
			}			
		}
		break;
	case SLOT_PACKAGE:
		{
			if (m_pArrPackage != NULL)
			{
				CloseMsg();

				int idx = m_pArrPackage->GetSelectIdx();

				if (idx < 0)
					return;

				if (m_pIconsPackageItem[idx]->IsEmpty() == true)
					return;

				CItems* pItems = m_pIconsPackageItem[idx]->getItems();

				if (pItems == NULL)
					return;

				DelShopItem(pItems->Item_Tab, pItems->InvenIndex,
					pItems->Item_UniIndex, pItems->Item_Sum,
					-1, idx, SLOT_PACKAGE);
			}
		}
		break;
	case SLOT_SHOP:
		{
			if (m_pArrShop != NULL)
			{
				CloseMsg();

				int idx = m_pArrShop->GetSelectIdx();

				if (idx < 0)
					return;

				if (m_pIconsShopItem[idx]->IsEmpty() == true)
					return;

				CItems* pItems = m_pIconsShopItem[idx]->getItems();

				if (pItems == NULL)
					return;

				if (m_bBuyShop == true)
				{
					if (m_pIconsShopItem[idx]->getIndex() == PACKAGE_ITEM_INDEX)
					{
						m_pIconsShopItem[idx]->clearIconData();
						m_bPackageBuy	= FALSE;

						m_pBtnPackOk->SetEnable(TRUE);
						m_pBtnPackCancel->SetEnable(FALSE);

						return;
					}

					AddShopItem(idx, pItems->Item_UniIndex, pItems->Item_Sum, SLOT_TRADE);
				}
				else
				{
					if (UTIL_HELP()->IsAvailable4Sale(pItems, UI_PERSONALSHOP) == false)
						return;

					AddShopItem(idx, pItems->Item_UniIndex, pItems->Item_Sum, SLOT_TRADE);
				}
			}
		}
		break;
	}
}

void CUIPersonalshopNew::proc_lbutton_up( eSlotType type )
{
	if ((m_bBuyShop || !m_bShopStart))
	{
		CUIManager* pUIManager = UIMGR();
		CUIIcon* pDrag = pUIManager->GetDragIcon();

		if (pDrag == NULL)
			return;

		// 패키지 아이템은 드랍되면 안된다.
		if (pDrag->getIndex() == PACKAGE_ITEM_INDEX)
			return;

		if (pDrag->getBtnType() == UBET_ITEM &&
			pDrag->GetWhichUI() == UI_PERSONALSHOP)
		{
			CItems* pItems = pDrag->getItems();

			if (pItems == NULL)
				return;

			switch (type)
			{
			case SLOT_TRADE:
				{
					int idx = -1;

					if (m_pArrShop != NULL)
						idx = m_pArrShop->GetSelectIdx();

					if (idx < 0)
						return;					

					AddShopItem(idx, pItems->Item_UniIndex, pItems->Item_Sum, SLOT_TRADE);
				}
				break;
			case SLOT_PACKAGE:
				{
					if (m_pChkPremium && m_pChkPremium->IsChecked() == TRUE)
					{
						int idx = -1;

						if (m_pArrShop != NULL)
							idx = m_pArrShop->GetSelectIdx();

						if (idx < 0)
							return;					

						AddShopItem(idx, pItems->Item_UniIndex, pItems->Item_Sum, SLOT_PACKAGE);
					}
					else
					{
						pUIManager->CloseMessageBox(MSGCMD_PERSONALSHOP_ERROR);
						CUIMsgBox_Info	MsgBoxInfo;
						MsgBoxInfo.SetMsgBoxInfo( _S( 757, "개인상점" ), UMBS_OK,	
							UI_PERSONALSHOP, MSGCMD_PERSONALSHOP_ERROR );
						CTString	strMessage = _S( 760, "패키지 판매는 프리미엄 서비스를 사용해야만 가능합니다." );		
						MsgBoxInfo.AddString( strMessage );
						pUIManager->CreateMessageBox( MsgBoxInfo );
					}
				}
				break;
			case SLOT_SHOP:
				{
					int TradeIdx = -1;
					int	PackageIdx = -1;

					if (m_pArrTrade != NULL)
						TradeIdx = m_pArrTrade->GetSelectIdx();

					if (m_pArrPackage != NULL)
						PackageIdx = m_pArrPackage->GetSelectIdx();

					DelShopItem(pItems->Item_Tab, pItems->InvenIndex,
						pItems->Item_UniIndex, pItems->Item_Sum,
						TradeIdx, PackageIdx, SLOT_SHOP);
				}
				break;
			}
		}		
	}
}

void CUIPersonalshopNew::start_shop()
{
	CUIManager* pUIManager = UIMGR();

	if (m_bShopStart == false)
	{
		pUIManager->CloseMessageBox( MSGCMD_SHOP_START );

		//m_bShopStart = TRUE;
		CTString	strMessage;
		CUIMsgBox_Info	MsgBoxInfo;

		// 펫 탑승중일 때, 개인상점을 열 수 없다.
		if(_pNetwork->MyCharacterInfo.bWildPetRide == TRUE)
		{
			pUIManager->CloseMessageBox(MSGCMD_PERSONALSHOP_ERROR);
			MsgBoxInfo.SetMsgBoxInfo( _S( 757, "개인상점" ), UMBS_OK, UI_PERSONALSHOP, MSGCMD_PERSONALSHOP_ERROR );
			MsgBoxInfo.AddString( _S( 5314, "개인 상점을 개설할 수 없는 상태입니다." ) );
			pUIManager->CreateMessageBox( MsgBoxInfo );
			return;
		}

		MsgBoxInfo.SetMsgBoxInfo( _S( 757, "개인상점" ), UMBS_OKCANCEL,		
			UI_PERSONALSHOP, MSGCMD_SHOP_START );						

		int iRatio = 2;
		if(m_bPremium)
		{
			iRatio = 2;
			strMessage.PrintF( _S( 758, "프리미엄 서비스의 사용료는 판매된 아이템 가격의 %d 퍼센트입니다.  계속 진행하시겠습니까?" ), iRatio );
		}
		else
		{
			strMessage.PrintF( _S( 759, "개인상점을 개설합니다.  계속 진행하시겠습니까?" ) );		
		}

		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );
	}
	else
	{
		SendPersonalShopStop();
	}	
}

void CUIPersonalshopNew::cancel()
{
	CUIManager* pUIManager = UIMGR();

	if(!m_bBuyShop && m_bShopStart)
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 745, "상점 개설중에는 창을 닫을 수 없습니다." ), SYSMSG_ERROR );	
	}
	else
	{
		ResetShop();
		pUIManager->SetCSFlagOff( CSF_PERSONALSHOP );
	}
}

void CUIPersonalshopNew::buy()
{
	// Send Network Message
	BuyItems();

	// Clear Member Value
	ResetShop();

	UIMGR()->SetCSFlagOff( CSF_PERSONALSHOP );
}

void CUIPersonalshopNew::check_premium()
{
	if (m_pChkPremium != NULL)
	{
		m_bPremium = m_pChkPremium->IsChecked();

		if (m_pEditPackagePrice != NULL)
		{
			m_pEditPackagePrice->SetEnable(m_bPremium);

			if (m_bPremium == FALSE)
			{
				ClearPackageSlot();
			}
		}
	}
}

void CUIPersonalshopNew::package_ok()
{
	PackageToShop(TRUE);

	m_pBtnPackOk->SetEnable(FALSE);
	m_pBtnPackCancel->SetEnable(TRUE);
}

void CUIPersonalshopNew::package_cancel()
{
	PackageToShop(FALSE);

	m_pBtnPackOk->SetEnable(TRUE);
	m_pBtnPackCancel->SetEnable(FALSE);
}

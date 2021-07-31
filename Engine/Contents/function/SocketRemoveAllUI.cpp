#include "stdh.h"
#include "SocketRemoveAllUI.h"

#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UISocketSystem.h>

#define	PRICE_CLEAR		10000.0f;

CSocketRemoveAllUI::CSocketRemoveAllUI()
	: m_pMoveArea(NULL)
	, m_pConsumeNas(NULL)
	, m_pSocketItem(NULL)
{
	int i;
	for(i = 0; i < eBTN_MAX; ++i)
		m_pBtn[i] = NULL;

	setInherit(false);
}


void CSocketRemoveAllUI::PressOK()
{
	if (m_pSocketItem == NULL)
		return;

	CItems* pItem = m_pSocketItem->getItems();

	if (pItem == NULL)
		return;

	CUISocketSystem* pSocketSys = UIMGR()->GetSocketSystem();

	if (pSocketSys == NULL)
		return;

	if( pItem->GetAmountOfJewelryInserted() <= pSocketSys->IsEmptyInvenSlotCount() )
	{
		SWORD item_Tab = pItem->Item_Tab;
		SWORD invenIdx = pItem->InvenIndex;
		LONG itemVirIdx = pItem->Item_UniIndex;

		pSocketSys->SendCleanAllJewelReq(item_Tab, invenIdx, itemVirIdx);
		OkBtnLockOn();
	}
	else
		pSocketSys->showErrorMessage(CUISocketSystem::SOCKET_ERROR_NOT_ENOUGH_INVEN_SLOT);
}

void CSocketRemoveAllUI::OkBtnLockOn()
{
	if (m_pBtn[eBTN_OK] == NULL)
		return;

	m_pBtn[eBTN_OK]->SetEnable(FALSE);
}

void CSocketRemoveAllUI::OkBtnLockOff()
{
	if (m_pBtn[eBTN_OK] == NULL)
		return;

	m_pBtn[eBTN_OK]->SetEnable(TRUE);
}

void CSocketRemoveAllUI::initialize()
{
#ifndef		WORLD_EDITOR
	m_pMoveArea = (CUIImage*)findUI("move_area");

	if (m_pMoveArea != NULL)
	{
		int l, t;
		l = m_pMoveArea->GetPosX();
		t = m_pMoveArea->GetPosY();
		setTitleRect(l, t, l + m_pMoveArea->GetWidth(), t + m_pMoveArea->GetHeight());
	}

	std::string strBtnID[eBTN_MAX] = { "btn_close", "btn_ok" };		
	for(int i = 0; i < eBTN_MAX; ++i)
		m_pBtn[i] = (CUIButton*)findUI(strBtnID[i].c_str());

	if (m_pBtn[eBTN_CLOSE] != NULL)
		m_pBtn[eBTN_CLOSE]->SetCommandFUp(boost::bind(&CSocketRemoveAllUI::closeUI, this));

	if (m_pBtn[eBTN_OK] != NULL)
		m_pBtn[eBTN_OK]->SetCommandFUp(boost::bind(&CSocketRemoveAllUI::PressOK, this));

	m_pConsumeNas = (CUIText*)findUI("text_nas");
	m_pSocketItem = (CUIIcon*)findUI("icon_item");
#endif	//	WORLD_EDITOR
}

void CSocketRemoveAllUI::openUI()
{
	if (GetHide() == FALSE)
		return;
	
	CUIManager* pUIMgr = UIMGR();
	CUIInventory* pInven = pUIMgr->GetInventory();

	if (pInven->IsLocked() == TRUE ||
		pInven->IsLockedArrange() == TRUE)
	{
		pInven->ShowLockErrorMessage();
		return;
	}

	Hide(FALSE);
	SetVisible(TRUE);

	resetUI();
	
	pUIMgr->RearrangeOrder( UI_SOCKET_REMOVE_ALL, TRUE );
	pInven->Lock( TRUE, TRUE, LOCK_SOCKETSYSTEM );
}

void CSocketRemoveAllUI::closeUI()
{
	Hide(TRUE);
	SetVisible(FALSE);

	CUIManager* pUIMgr = UIMGR();
	pUIMgr->RearrangeOrder( UI_SOCKET_REMOVE_ALL, FALSE );
	pUIMgr->GetInventory()->Lock( FALSE, FALSE, LOCK_SOCKETSYSTEM );
}

void CSocketRemoveAllUI::resetUI()
{
	OkBtnLockOn();
	setNeedMoney(0);
	if (m_pSocketItem != NULL)
		m_pSocketItem->clearIconData();
}

WMSG_RESULT CSocketRemoveAllUI::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->RearrangeOrder( UI_SOCKET_REMOVE_ALL, TRUE );

	return CUIWindow::OnLButtonDown(x, y);
}

WMSG_RESULT CSocketRemoveAllUI::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	CUIManager* pUIManager = CUIManager::getSingleton();

	if (m_pSocketItem && m_pSocketItem->IsInside(x, y))
	{
		CUIIcon* pDrag = pUIManager->GetDragIcon();

		if (pDrag && pDrag->IsEmpty() == false)
		{
			if (CheckSocketItem(pDrag) == true)
			{
				m_pSocketItem->copyItem(pDrag);
				SQUAD sqNeedMoney = calcConsumeNas(m_pSocketItem->getItems());
				OkBtnLockOff();
				setNeedMoney(sqNeedMoney);
			}
			else
				pUIManager->GetSocketSystem()->showErrorMessage();
		}
	}
	
	pUIManager->ResetHoldBtn();
	return CUIWindow::OnLButtonUp(x, y);
}

bool CSocketRemoveAllUI::CheckSocketItem(CUIIcon* pIcon)
{
	if (pIcon == NULL)
		return false;
	
	// 아이템이어야 한다.
	if (pIcon->getBtnType() != UBET_ITEM)
		return false;

	CItems* pItem = pIcon->getItems();

	if (pItem == NULL)
		return false;

	// 착용중이 아니어야 하고 소켓이 뚫린 아이템 이어야 한다.
	if (pItem->Item_Wearing > -1 || pItem->GetSocketCount() <= 0)
		return false;

	CItemData* pItemData = pItem->ItemData;

	if (pItemData == NULL)
		return false;

	if (!pItemData->IsFlag(ITEM_FLAG_SOCKET))
		return false;
	
	return true;
}

SQUAD CSocketRemoveAllUI::calcConsumeNas( CItems* pItem )
{
	if (pItem == NULL)
		return 0;

	SQUAD sqNeedMoney = 0.0f;
	SBYTE sbCount = 0;
	SBYTE sbjewelCount = 0;

	for( int k = 0; k < MAX_SOCKET_OPTION; ++k )
	{
		if (k == 0)
		{
			if( pItem->GetSocketJewelIndex( k ) > 0 )
				sqNeedMoney += 100000.0f;
		}

		if(pItem->GetSocketJewelIndex( k ) >= 0)
			++sbCount;
		if (pItem->GetSocketJewelIndex( k ) > 0)
			++sbjewelCount;
	}

	if( pItem->ItemData && pItem->ItemData->IsFlag( ITEM_FLAG_LORD ) )
		sqNeedMoney = 0;
	else
		sqNeedMoney += sbjewelCount * sbCount * PRICE_CLEAR;

	return sqNeedMoney;
}

void CSocketRemoveAllUI::setNeedMoney( SQUAD nas )
{
	if (m_pConsumeNas != NULL)
	{
		CUIManager* pUIManager = CUIManager::getSingleton();
		CTString strTemp;
		CTString sqMoney;
		sqMoney.PrintF( "%I64d", nas );
		pUIManager->InsertCommaToString( sqMoney );
		strTemp.PrintF( "%s:%s", _S( 4979, "제거 비용"), sqMoney );
		m_pConsumeNas->SetText(strTemp);
		m_pConsumeNas->setFontColor(pUIManager->GetNasColor(sqMoney));
	}
}

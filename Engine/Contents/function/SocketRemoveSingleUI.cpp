#include "stdh.h"
#include "SocketRemoveSingleUI.h"

#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UISocketSystem.h>

enum eGUIDE_STATE
{
	eSTATE_NONE = 0,
	eSTATE_REGIST_ITEM,
};

CSocketRemoveSingleUI::CSocketRemoveSingleUI()
	: m_pMoveArea(NULL)
	, m_pTextConsumeNas(NULL)
	, m_pTbGuide(NULL)
	, m_nSelectIndex(-1)
{
	int i;
	for(i = 0; i < eBTN_MAX; ++i)
		m_pBtn[i] = NULL;

	for(i = 0; i < eICON_MAX; ++i)
		m_pIcon[i] = NULL;

	for(i = 0; i < eIMG_SLOT_MAX; ++i)
	{
		m_pImgBlock[i] = NULL;
		m_pImgSelect[i] = NULL;
	}

	setInherit(false);
}

void CSocketRemoveSingleUI::PressOK()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CUISocketSystem* pSocket = pUIManager->GetSocketSystem();

	if (pSocket->IsEmptyInvenSlotCount() > 0)
	{
		if( pUIManager->DoesMessageBoxExist( MSGLCMD_SOCKET_SYSTEM_EMPTY_BOX ) ) return;

		CUIMsgBox_Info	MsgBoxInfo;
		CTString		strMessage[MAX_MSGSTRING];
		strMessage[0] = _S( 5897, "보석 개별 제거" );
		strMessage[1] = _S( 5899, "보석 제거 작업에 필요한 마법 스크롤을 올려주세요." );

		MsgBoxInfo.SetMsgBoxInfo( strMessage[0], UMBS_YESNO | UMBS_BUTTONEX, UI_NONE, MSGLCMD_SOCKET_SYSTEM_EMPTY_BOX );

		MsgBoxInfo.SetBtnType( UBET_ITEM, CItemData::ITEM_ONCEUSE, CItemData::ITEM_SUB_PROCESS_DOC );
		MsgBoxInfo.AddString( strMessage[1] );

		if (pUIManager->CreateMessageBox( MsgBoxInfo ))
		{

			pSocket->SetRemoveSingleItem(m_pIcon[eICON_ITEM]);
			pSocket->SetRemoveSingleJewelPos(m_nSelectIndex);
			closeUI();
		}
	}
	else
	{
		pSocket->showErrorMessage(CUISocketSystem::SOCKET_ERROR_NOT_ENOUGH_INVEN_SLOT);
	}	
}

void CSocketRemoveSingleUI::openUI()
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
	
	pUIMgr->RearrangeOrder( UI_SOCKET_REMOVE_SINGLE, TRUE );
	pInven->Lock( TRUE, TRUE, LOCK_SOCKETSYSTEM );
}

void CSocketRemoveSingleUI::closeUI()
{
	Hide(TRUE);
	SetVisible(FALSE);

	CUIManager* pUIMgr = UIMGR();
	pUIMgr->RearrangeOrder( UI_SOCKET_REMOVE_SINGLE, FALSE );
	pUIMgr->GetInventory()->Lock( FALSE, FALSE, LOCK_SOCKETSYSTEM );
}

void CSocketRemoveSingleUI::resetUI()
{
	_resetIcon(eICON_ITEM);
	_resetSocketSlot();
	GuideStringChange(eSTATE_NONE);
	SetNasString(0);
	BtnLockOn();
}

void CSocketRemoveSingleUI::initialize()
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
	int i;
	// Btn
	std::string strBtnID[eBTN_MAX] = { "btn_close", "btn_ok" };

	for(i = 0; i < eBTN_MAX; ++i)
		m_pBtn[i] = (CUIButton*)findUI(strBtnID[i].c_str());

	if (m_pBtn[eBTN_CLOSE] != NULL)
		m_pBtn[eBTN_CLOSE]->SetCommandFUp(boost::bind(&CSocketRemoveSingleUI::closeUI, this));

	if (m_pBtn[eBTN_OK] != NULL)
		m_pBtn[eBTN_OK]->SetCommandFUp(boost::bind(&CSocketRemoveSingleUI::PressOK, this));

	// IMG
	CTString strImgBlockID;
	CTString strImgCombineID;

	for(i = 0; i < eIMG_SLOT_MAX; ++i)
	{
		strImgBlockID.PrintF("img_block%d", i + 1);
		m_pImgBlock[i] = (CUIImage*)findUI(strImgBlockID.str_String);

		if (m_pImgBlock[i] != NULL)
			m_pImgBlock[i]->Hide(TRUE);

		strImgCombineID.PrintF("img_select%d", i + 1);
		m_pImgSelect[i] = (CUIImage*)findUI(strImgCombineID.str_String);

		if (m_pImgSelect[i] != NULL)
			m_pImgSelect[i]->Hide(TRUE);
	}

	// Icon
	CTString strIconID;
	for(i = 0; i < eICON_MAX; ++i)
	{
		strIconID.PrintF("item_num%d", i);
		m_pIcon[i] = (CUIIcon*)findUI(strIconID.str_String);

		if (m_pIcon[i] != NULL)
			m_pIcon[i]->clearIconData();
	}

	m_pTextConsumeNas = (CUIText*)findUI("text_nas");
	m_pTbGuide = (CUITextBox*)findUI("tb_guide");
#endif	//	WORLD_EDITOR
}

WMSG_RESULT CSocketRemoveSingleUI::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	for (int i = eICON_SLOT_SOCKET1; i < eICON_MAX; ++i)
	{
		if (m_pIcon[i] && m_pIcon[i]->IsInside(x, y))
		{
			if (m_pIcon[i]->IsEmpty() == false)
			{
				if (m_nSelectIndex >= 0)
					JuwelSlotStateChange(m_nSelectIndex, eSLOT_STATE_NONE);
				m_nSelectIndex = i - 1;
				JuwelSlotStateChange(m_nSelectIndex, eSLOT_STATE_SELECT);
				BtnLockOff();
				break;
			}
		}
	}	

	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->RearrangeOrder( UI_SOCKET_REMOVE_SINGLE, TRUE );

	return CUIWindow::OnLButtonDown(x, y);
}

WMSG_RESULT CSocketRemoveSingleUI::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	CUIManager* pUIManager = CUIManager::getSingleton();
	CUIIcon* pDrag = pUIManager->GetDragIcon();

	if (pDrag && pDrag->IsEmpty() == false)
	{
		CItems* pItemDrag = pDrag->getItems();

		if (pItemDrag == NULL)
			return WMSG_FAIL;

		if (pDrag->getBtnType() == UBET_ITEM)
		{
			if (m_pIcon[eICON_ITEM] && m_pIcon[eICON_ITEM]->IsInside(x, y))
			{
				if (CheckItem(pItemDrag) == true)
				{
					m_pIcon[eICON_ITEM]->copyItem(pDrag);
					GuideStringChange(eSTATE_REGIST_ITEM);
					_resetSocketSlot();
					_updateSlotState();
					BtnLockOn();
					SetNasString(0);
				}
				else
					pUIManager->GetSocketSystem()->showErrorMessage();
			}
		}
	}

	pUIManager->ResetHoldBtn();
	return CUIWindow::OnLButtonUp(x, y);
}

void CSocketRemoveSingleUI::GuideStringChange( int nState )
{
	if (m_pTbGuide == NULL)
		return;

	CTString strTemp;
	COLOR colorTemp = 0xFFFFFFFF;
	// 가이드 셋팅
	switch(nState)
	{
	case eSTATE_NONE:
		strTemp.PrintF( _S(5898, "장비를 올려놓은 후 제거를 원하는 소켓의 보석을 선택하여 주세요.") );
		break;
	default:
		strTemp.PrintF( CTString("") );
		break;
	}

	m_pTbGuide->SetText(strTemp, colorTemp);
}

void CSocketRemoveSingleUI::SetNasString( SQUAD sqNas )
{
	// 나스 스트링 설정
	if (m_pTextConsumeNas != NULL)
	{
		CUIManager* pUIManager = CUIManager::getSingleton();
		CTString sqMoney;
		sqMoney.PrintF( "%I64d", sqNas );
		pUIManager->InsertCommaToString( sqMoney );
		m_pTextConsumeNas->SetText(sqMoney);
		m_pTextConsumeNas->setFontColor(pUIManager->GetNasColor(sqMoney));
	}
}

bool CSocketRemoveSingleUI::CheckItem( CItems* pItem )
{
	if (pItem == NULL)
		return false;

	CItemData* pItemData = pItem->ItemData;

	if (pItemData == NULL)
		return false;

	if (!pItemData->IsFlag(ITEM_FLAG_SOCKET))
		return false;

	if (pItem->GetSocketCount() <= 0)
		return false;

	int jewelcount = 0;
	for (int i = 0; i < JEWEL_MAX_COUNT; ++i)
	{
		if (0 < pItem->GetSocketJewelIndex(i))
			jewelcount++;
	}

	if (jewelcount <= 0)
		return false;

	if (pItem->Item_Wearing >= 0)
		return false;

	if (pItem->Item_Flag & FLAG_ITEM_COMPOSITION)
		return false;
	return true;
}

void CSocketRemoveSingleUI::BtnLockOn()
{
	// Btn Lock
	if (m_pBtn[eBTN_OK] != NULL)
		m_pBtn[eBTN_OK]->SetEnable(FALSE);
}

void CSocketRemoveSingleUI::BtnLockOff()
{
	// Btn Lock 해제
	if (m_pBtn[eBTN_OK] != NULL)
		m_pBtn[eBTN_OK]->SetEnable(TRUE);
}

void CSocketRemoveSingleUI::JuwelSlotStateChange(int nSlotIdx, eJuwelslotState eState /*= eSLOT_STATE_NONE*/ )
{
	if (nSlotIdx < 0 || nSlotIdx >= eIMG_SLOT_MAX)
		return;

	if (m_pImgBlock[nSlotIdx] == NULL || m_pImgSelect[nSlotIdx] == NULL)
		return;

	BOOL bBlockHide = TRUE;
	BOOL bSelectHide = TRUE;

	switch(eState)
	{
	case eSLOT_STATE_BLOCK:
		bBlockHide = FALSE;
		break;
	case eSLOT_STATE_SELECT:
		bSelectHide = FALSE;
		break;
	}

	m_pImgBlock[nSlotIdx]->Hide(bBlockHide);
	m_pImgSelect[nSlotIdx]->Hide(bSelectHide);
}

void CSocketRemoveSingleUI::_resetIcon( int nSlotIdx )
{
	if (nSlotIdx < 0 || nSlotIdx >= eICON_MAX)
		return;

	if (m_pIcon[nSlotIdx] != NULL)
		m_pIcon[nSlotIdx]->clearIconData();
}

void CSocketRemoveSingleUI::_updateSlotState()
{
	if (m_pIcon[eICON_ITEM] == NULL || m_pIcon[eICON_ITEM]->IsEmpty() == true)
		return;

	CItems* pItems = m_pIcon[eICON_ITEM]->getItems();

	if (pItems == NULL)
		return;

	if (pItems->ItemData == NULL || !pItems->ItemData->IsFlag(ITEM_FLAG_SOCKET) )
		return;
	
	eJuwelslotState eState;

	for( int i = eICON_SLOT_SOCKET1; i < eICON_MAX; ++i )
	{
		if (m_pIcon[i] == NULL)
			continue;
			
		LONG jewelindex = pItems->GetSocketJewelIndex(i - 1);

		if (jewelindex > 0)
		{
			m_pIcon[i]->setData(UBET_ITEM, jewelindex);
			
			if (m_nSelectIndex == i - 1)
				eState = eSLOT_STATE_SELECT;
			else 
				eState = eSLOT_STATE_NONE;
		}
		else
		{
			m_pIcon[i]->clearIconData();
			eState = eSLOT_STATE_BLOCK;
		}
		
		JuwelSlotStateChange(i - 1, eState);
	}	
}

void CSocketRemoveSingleUI::_resetSocketSlot()
{
	int i;
	for (i = eICON_SLOT_SOCKET1; i < eICON_MAX; ++i)
	{
		if (m_pIcon[i] != NULL)
			m_pIcon[i]->clearIconData();
	}

	for (i = eIMG_SLOT1; i < eIMG_SLOT_MAX; ++i)
		JuwelSlotStateChange(i, eSLOT_STATE_BLOCK);

	m_nSelectIndex = -1;
}


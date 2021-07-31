#include "stdh.h"
#include "SocketCombineUI.h"

#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UISocketSystem.h>

#define JEWEL_DFEND_SCROLL_INDEX 9907

enum eGUIDE_STATE
{
	eSTATE_NONE = 0,
	eSTATE_REGIST_ITEM,
	eSTATE_ALL_READY,
	eSTATE_PROCESSING,
	eSTATE_CREATE_SUCCESS,
	eSTATE_FAIL,
	eSTATE_FAIL_USED_SCROLL
};

//--------------------------------------------------------------
// 프로그래스 완료
class CmdCombineSocket : public Command
{
public:
	CmdCombineSocket() : pUI_(NULL) {}
	void setData(CSocketCombineUI* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->ProgressComplete();
	}
private:
	CSocketCombineUI* pUI_;
};

CSocketCombineUI::CSocketCombineUI()
	: m_pMoveArea(NULL)
	, m_pProgressBar(NULL)
	, m_pTextConsumeNas(NULL)
	, m_pTbGuide(NULL)
	, m_bProgress(false)
	, m_bUseDefendScroll(false)
	, m_nRegistSlotIdx(-1)
{
	int i;
	for(i = 0; i < eBTN_MAX; ++i)
		m_pBtn[i] = NULL;

	for(i = 0; i < eICON_MAX; ++i)
		m_pIcon[i] = NULL;

	for(i = 0; i < eIMG_SLOT_MAX; ++i)
	{
		m_pImgBlock[i] = NULL;
		m_pImgCombine[i] = NULL;
	}

	setInherit(false);
}

void CSocketCombineUI::PressOK()
{
	if (m_bProgress == true)
		return;


	CUIManager* pUIMgr = UIMGR();

	if( pUIMgr->GetInventory()->IsInventoryFull() )
	{
		pUIMgr->GetSocketSystem()->_showErrorSystemMessage(CUISocketSystem::JEWEL_COMPOS_ERROR_NOT_INVENTORY_EMPTY);
		return;
	}	

	// 프로그래스바 스타트
	if (m_pProgressBar != NULL)
	{
		_resetProgress();
		m_pProgressBar->StartProgress();
		m_bProgress = true;

		// 버튼 락처리
		BtnLockOn();
		GuideStringChange(eSTATE_PROCESSING);
	}
}

void CSocketCombineUI::ProgressComplete()
{
	if (m_pIcon[eICON_ITEM] == NULL || m_pIcon[eICON_ITEM]->IsEmpty() == true)
		return;

	if (m_pIcon[m_nRegistSlotIdx] == NULL || m_pIcon[m_nRegistSlotIdx]->IsEmpty() == true)
		return;

	if (m_pIcon[eICON_SCROLL] == NULL)
		return;

	// Send Message
	CUISocketSystem::stPACKET_ITEM Item;
	Item.Item_Tab = m_pIcon[eICON_ITEM]->getItems()->Item_Tab;
	Item.InvenIdx = m_pIcon[eICON_ITEM]->getItems()->InvenIndex;
	Item.ItemVirIdx = m_pIcon[eICON_ITEM]->getItems()->Item_UniIndex;

	CUISocketSystem::stPACKET_ITEM Juwel;
	Juwel.Item_Tab = m_pIcon[m_nRegistSlotIdx]->getItems()->Item_Tab;
	Juwel.InvenIdx = m_pIcon[m_nRegistSlotIdx]->getItems()->InvenIndex;
	Juwel.ItemVirIdx = m_pIcon[m_nRegistSlotIdx]->getItems()->Item_UniIndex;
	Juwel.SlotIdx = m_nRegistSlotIdx - 1;

	CUISocketSystem::stPACKET_ITEM Scroll;
	if (m_pIcon[eICON_SCROLL]->IsEmpty() == false)
	{
		Scroll.Item_Tab = m_pIcon[eICON_SCROLL]->getItems()->Item_Tab;
		Scroll.InvenIdx = m_pIcon[eICON_SCROLL]->getItems()->InvenIndex;
		Scroll.ItemVirIdx = m_pIcon[eICON_SCROLL]->getItems()->Item_UniIndex;
	}	
	
 	UIMGR()->GetSocketSystem()->SendCombineReq(Item, Juwel, Scroll);
}

void CSocketCombineUI::ShowResultSuccess(int nItemVirIdx)
{
	m_bUseDefendScroll = false;
	m_bProgress = false;
	_resetIcon(eICON_ITEM);
	_resetIcon(eICON_SCROLL);
	_resetSocketSlot();
	SetNasString(0);
	BtnLockOn();

	CUIManager* pUIManager = CUIManager::getSingleton();
	m_pIcon[eICON_ITEM]->copyItem( pUIManager->GetInventory()->GetItemIcon( nItemVirIdx ) );
	_updateSlotState();
	GuideStringChange(eSTATE_CREATE_SUCCESS);
}

void CSocketCombineUI::ShowResultFail()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	int i;

	if (m_bUseDefendScroll == true)
		GuideStringChange(eSTATE_FAIL_USED_SCROLL);
	else
		GuideStringChange(eSTATE_FAIL);

	for( i = 0; i < eICON_MAX; ++i )
		_updateBtn(i);

	bool bReady = false;
	
	for( i = eICON_SLOT_SOCKET1; i < eICON_SLOT_SOCKET_MAX; ++i )
	{
		if (m_pIcon[i]->IsEmpty() == false)
		{
			m_nRegistSlotIdx = i;
			bReady = true;
			break;
		}
	}

	if (bReady == false)
	{
		SetNasString(0);
		m_nRegistSlotIdx = -1;
	}
	else
	{
		_updateSlotState();
		BtnLockOff();
	}

	m_bProgress = false;
}

void CSocketCombineUI::openUI()
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
	
	pUIMgr->RearrangeOrder( UI_SOCKET_COMBINE, TRUE );
	pInven->Lock( TRUE, TRUE, LOCK_SOCKETSYSTEM );
}

void CSocketCombineUI::closeUI()
{
	Hide(TRUE);
	SetVisible(FALSE);

	CUIManager* pUIMgr = UIMGR();
	pUIMgr->RearrangeOrder( UI_SOCKET_COMBINE, FALSE );
	pUIMgr->GetInventory()->Lock( FALSE, FALSE, LOCK_SOCKETSYSTEM );
}

void CSocketCombineUI::resetUI()
{
	m_bUseDefendScroll = false;
	_resetIcon(eICON_ITEM);
	_resetIcon(eICON_SCROLL);
	_resetSocketSlot();
	_resetProgress();
	GuideStringChange(eSTATE_NONE);
	SetNasString(0);
	BtnLockOn();
}

void CSocketCombineUI::initialize()
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

	// Btn
	std::string strBtnID[eBTN_MAX] = { "btn_close", "btn_combine" };

	for(int i = 0; i < eBTN_MAX; ++i)
		m_pBtn[i] = (CUIButton*)findUI(strBtnID[i].c_str());

	if (m_pBtn[eBTN_CLOSE] != NULL)
		m_pBtn[eBTN_CLOSE]->SetCommandFUp(boost::bind(&CSocketCombineUI::closeUI, this));

	if (m_pBtn[eBTN_OK] != NULL)
		m_pBtn[eBTN_OK]->SetCommandFUp(boost::bind(&CSocketCombineUI::PressOK, this));

	// IMG
	CTString strImgBlockID;
	CTString strImgCombineID;

	for(int i = 0; i < eIMG_SLOT_MAX; ++i)
	{
		strImgBlockID.PrintF("img_block%d", i + 1);
		m_pImgBlock[i] = (CUIImage*)findUI(strImgBlockID.str_String);

		if (m_pImgBlock[i] != NULL)
			m_pImgBlock[i]->Hide(TRUE);

		strImgCombineID.PrintF("img_combine%d", i + 1);
		m_pImgCombine[i] = (CUIImage*)findUI(strImgCombineID.str_String);

		if (m_pImgCombine[i] != NULL)
			m_pImgCombine[i]->Hide(TRUE);
	}

	// Icon
	CTString strIconID;
	for(int i = 0; i < eICON_MAX; ++i)
	{
		strIconID.PrintF("item_num%d", i);
		m_pIcon[i] = (CUIIcon*)findUI(strIconID.str_String);

		if (m_pIcon[i] != NULL)
			m_pIcon[i]->clearIconData();
	}

	m_pProgressBar = (CUIProgressBar*)findUI("pb_bar");

	if (m_pProgressBar != NULL)
	{
		CmdCombineSocket* pCmd = new CmdCombineSocket;
		pCmd->setData(this);
		m_pProgressBar->SetCmdComplete(pCmd);
	}

	m_pTextConsumeNas = (CUIText*)findUI("text_nas");
	m_pTbGuide = (CUITextBox*)findUI("tb_guide");
#endif	//	WORLD_EDITOR
}

WMSG_RESULT CSocketCombineUI::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if (m_bProgress == true)
		return WMSG_FAIL;

	if (m_pIcon[eICON_SCROLL] && m_pIcon[eICON_SCROLL]->IsInside(x, y))
	{
		if (m_pIcon[eICON_SCROLL]->IsEmpty() == false)
			_resetIcon(eICON_SCROLL);
	}
	else
	{
		for (int i = eICON_SLOT_SOCKET1; i < eICON_SLOT_SOCKET_MAX; ++i)
		{
			if (m_pIcon[i] && m_pIcon[i]->IsInside(x, y))
			{
				if (m_pIcon[i]->IsEmpty() == false && i == m_nRegistSlotIdx)
				{
					BtnLockOn();
					_resetSocketSlot();
					_updateSlotState();
					SetNasString(0);
					GuideStringChange(eSTATE_REGIST_ITEM);
					break;
				}
			}
		}
	}

	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->RearrangeOrder( UI_SOCKET_COMBINE, TRUE );

	return CUIWindow::OnLButtonDown(x, y);
}

WMSG_RESULT CSocketCombineUI::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	CUIManager* pUIManager = CUIManager::getSingleton();

	if (m_bProgress == true)
	{
		pUIManager->ResetHoldBtn();
		return WMSG_FAIL;
	}

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
				_resetProgress();

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
			else if (m_pIcon[eICON_SCROLL] && m_pIcon[eICON_SCROLL]->IsInside(x, y))
			{
				if (CheckScrollItem(pItemDrag) == true)
				{
					m_pIcon[eICON_SCROLL]->copyItem(pDrag);
					m_pIcon[eICON_SCROLL]->setCount(pDrag->getItems()->Item_Sum);
					m_bUseDefendScroll = true;
				}
				else
					pUIManager->GetSocketSystem()->showErrorMessage();
			}
			else
			{
				for (int i = eICON_SLOT_SOCKET1; i < eICON_SLOT_SOCKET_MAX; ++i)
				{
					if (m_pIcon[i] && m_pIcon[i]->IsInside(x, y))
					{
						if (CheckJewelItem(pItemDrag, i) == true)
						{
							m_pIcon[i]->copyItem(pDrag);
							m_pIcon[i]->setCount(pDrag->getItems()->Item_Sum);
							m_nRegistSlotIdx = i;
							_updateSlotState();
							GuideStringChange(eSTATE_ALL_READY);
							BtnLockOff();
						}
					}
				}
			}
		}
	}

	pUIManager->ResetHoldBtn();
	return CUIWindow::OnLButtonUp(x, y);
}

void CSocketCombineUI::GuideStringChange( int nState )
{
	if (m_pTbGuide == NULL)
		return;

	CTString strTemp;
	COLOR colorTemp = 0xFFFFFFFF;
	// 가이드 셋팅
	switch(nState)
	{
	case eSTATE_NONE:
		strTemp.PrintF( _S( 4962, "아이템을 올려주세요.") );
		break;
	case eSTATE_REGIST_ITEM:
		strTemp.PrintF( _S( 5917, "결합 할 보석을 올려주세요.") );
		break;
	case eSTATE_ALL_READY:
		strTemp.PrintF( _S( 5918, "결합 보조아이템이 없으면 결합 실패 시 보석의 등급이 하락됩니다. 결합 버튼을 눌러주세요." ) );
		break;
	case eSTATE_CREATE_SUCCESS:
		strTemp.PrintF( _S(5920, "축하합니다! 보석결합에 성공하였습니다!!") );
		colorTemp = 0x0000EEFF;
		break;
	case eSTATE_FAIL:
		strTemp.PrintF( _S(5921, "보석결합에 실패하여 보석의 등급이 하락하였습니다!") );
		colorTemp = 0xEE0000FF;
		break;
	case eSTATE_FAIL_USED_SCROLL:
		strTemp.PrintF( _S(6051, "보석결합에 실패하였습니다!") );
		colorTemp = 0xEE0000FF;
		break;
	default:
		strTemp.PrintF( CTString("") );
		break;
	}

	m_pTbGuide->SetText(strTemp, colorTemp);
}

void CSocketCombineUI::SetNasString( SQUAD sqNas )
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

bool CSocketCombineUI::CheckItem( CItems* pItem )
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

	if (pItem->Item_Wearing >= 0)
		return false;

	if (pItem->Item_Flag & FLAG_ITEM_COMPOSITION)
		return false;
	return true;
}

bool CSocketCombineUI::CheckScrollItem( CItems* pItem )
{
	if (pItem == NULL)
		return false;

	// 재료 스크롤 체크
	CItemData* pItemData = pItem->ItemData;

	if (pItemData == NULL)
		return false;
	
	if( pItemData->GetType() == CItemData::ITEM_ETC && 
		pItemData->GetSubType() == CItemData::ITEM_ETC_FUNCTIONS &&
		pItemData->GetItemIndex() == JEWEL_DFEND_SCROLL_INDEX )
	{
		return true;
	}

	return false;
}

bool CSocketCombineUI::CheckJewelItem( CItems* pItem, int nSlotIdx )
{
	if (pItem == NULL)
		return false;

	if (nSlotIdx < eICON_SLOT_SOCKET1 || nSlotIdx >= eICON_SLOT_SOCKET_MAX)
		return false;

	if (m_pIcon[nSlotIdx] == NULL)
		return false;

	int nLockIndex = nSlotIdx - 1;

	if (m_pImgBlock[nLockIndex] && m_pImgBlock[nLockIndex]->GetHide() == FALSE)
		return false;

	if (m_pImgCombine[nLockIndex] && m_pImgCombine[nLockIndex]->GetHide() == FALSE)
		return false;

	if (m_pIcon[eICON_ITEM] == NULL || m_pIcon[eICON_ITEM]->IsEmpty() == true)
		return false;

	CItemData* pItemHoldBtn = pItem->ItemData;

	if (pItemHoldBtn == NULL)
		return false;

	LONG HoldBtnOptionType = pItemHoldBtn->GetSocketOptionIndex();


	CUIManager* pUIMgr = UIMGR();
	CUISocketSystem* pUISockSys = pUIMgr->GetSocketSystem();

	for( SBYTE sbJw = 1; sbJw < JEWEL_MAX_COUNT; ++sbJw )
	{	// 보석 옵션의 중복 여부 체크 sbJw 가 1부터인 이유는 카오스 보석과 일반보석의 옵션이 중복되기에 1부터 시작
		const LONG jwIdx = m_pIcon[eICON_ITEM]->getItems()->GetSocketJewelIndex(sbJw);
		CItemData* pData = _pNetwork->GetItemData( jwIdx );
		LONG	equipOptionType = -1;

		if (jwIdx > 0)
			equipOptionType = pData->GetSocketOptionIndex();

		if (equipOptionType == HoldBtnOptionType)
		{
			pUISockSys->showErrorMessage(CUISocketSystem::SOCKET_ERROR_JEWEL_OVERLAP);
			return false;
		}
	}

	if( pItemHoldBtn->GetType() == CItemData::ITEM_ETC && (pItemHoldBtn->GetSubType() == CItemData::ITEM_ETC_JEWEL ||
		pItemHoldBtn->GetSubType() == CItemData::ITEM_ETC_CHAOSJEWEL))
	{
		if (nSlotIdx == eICON_SLOT_SOCKET1)
		{
			if (pItemHoldBtn->GetSubType() != CItemData::ITEM_ETC_CHAOSJEWEL)
			{
				pUISockSys->showErrorMessage(CUISocketSystem::SOCKET_ERROR_COMBINE_TERMS);
				return false;
			}

			SetNasString(1000000.0f);
		}
		else
		{
			if (pItemHoldBtn->GetSubType() == CItemData::ITEM_ETC_CHAOSJEWEL)
			{
				pUISockSys->showErrorMessage(CUISocketSystem::SOCKET_ERROR_COMBINE_TERMS);
				return false;
			}

			SetNasString(100000.0f);
		}
	}
	else
	{
		pUISockSys->showErrorMessage();
		return false;
	}

	return true;
}

void CSocketCombineUI::BtnLockOn()
{
	// Btn Lock
	if (m_pBtn[eBTN_OK] != NULL)
		m_pBtn[eBTN_OK]->SetEnable(FALSE);
}

void CSocketCombineUI::BtnLockOff()
{
	// Btn Lock 해제
	if (m_pBtn[eBTN_OK] != NULL)
		m_pBtn[eBTN_OK]->SetEnable(TRUE);
}

void CSocketCombineUI::JuwelSlotStateChange(int nSlotIdx, eJuwelslotState eState /*= eSLOT_STATE_NONE*/ )
{
	if (nSlotIdx < 0 || nSlotIdx >= eIMG_SLOT_MAX)
		return;

	if (m_pImgBlock[nSlotIdx] == NULL || m_pImgCombine[nSlotIdx] == NULL)
		return;

	BOOL bBlockHide = TRUE;
	BOOL bCombineHide = TRUE;

	switch(eState)
	{
	case eSLOT_STATE_BLOCK:
		bBlockHide = FALSE;
		break;
	case eSLOT_STATE_COMBINE:
		bCombineHide = FALSE;
		break;
	}

	m_pImgBlock[nSlotIdx]->Hide(bBlockHide);
	m_pImgCombine[nSlotIdx]->Hide(bCombineHide);
}

void CSocketCombineUI::_resetProgress()
{
	if (m_pProgressBar != NULL)
	{
		m_pProgressBar->IdleProgress();
		m_pProgressBar->SetProgressPos(0);
	}

	m_bProgress = false;
}

void CSocketCombineUI::_resetIcon( int nSlotIdx )
{
	if (nSlotIdx < 0 || nSlotIdx >= eICON_MAX)
		return;

	if (m_pIcon[nSlotIdx] != NULL)
	{
		m_pIcon[nSlotIdx]->clearIconData();

		if (nSlotIdx == eICON_SCROLL)
		{
			CTString textEmptyGuide = _S( 6052, "겹합 보조아이템을 올려주세요." );
			m_pIcon[nSlotIdx]->setTooltip(textEmptyGuide.str_String);
			m_bUseDefendScroll = false;
		}
	}
}

void CSocketCombineUI::_updateSlotState()
{
	if (m_pIcon[eICON_ITEM] == NULL || m_pIcon[eICON_ITEM]->IsEmpty() == true)
		return;

	CItems* pItems = m_pIcon[eICON_ITEM]->getItems();

	if (pItems == NULL)
		return;

	if (pItems->ItemData == NULL || !pItems->ItemData->IsFlag(ITEM_FLAG_SOCKET) )
		return;
	
	eJuwelslotState eState;

	for( int i = eICON_SLOT_SOCKET1; i < eICON_SLOT_SOCKET_MAX; ++i )
	{
		if (m_pIcon[i] == NULL)
			continue;
			
		LONG jewelindex = pItems->GetSocketJewelIndex(i - 1);
	
		if ( jewelindex < 0 )
		{
			m_pIcon[i]->clearIconData();
			eState = eSLOT_STATE_BLOCK;
		}
		else if ( jewelindex == 0)
		{
			if (m_nRegistSlotIdx > -1)
			{
				if (m_nRegistSlotIdx == i)
					eState = eSLOT_STATE_NONE;
				else
				{
					m_pIcon[i]->clearIconData();
					eState = eSLOT_STATE_BLOCK;
				}
			}
			else
				eState = eSLOT_STATE_NONE;
		}
		else
		{
			m_pIcon[i]->setData(UBET_ITEM, jewelindex);
			eState = eSLOT_STATE_COMBINE;
		}
		
		JuwelSlotStateChange(i - 1, eState);
	}	
}

void CSocketCombineUI::_resetSocketSlot()
{
	int i;
	for (i = eICON_SLOT_SOCKET1; i < eICON_SLOT_SOCKET_MAX; ++i)
	{
		if (m_pIcon[i] != NULL)
			m_pIcon[i]->clearIconData();
	}

	for (i = eIMG_SLOT1; i < eIMG_SLOT_MAX; ++i)
		JuwelSlotStateChange(i, eSLOT_STATE_BLOCK);

	m_nRegistSlotIdx = -1;
}

void CSocketCombineUI::_updateBtn( int nSlotIdx )
{
	if (m_pIcon[nSlotIdx]->IsEmpty())
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	int nIndex = m_pIcon[nSlotIdx]->getItems()->Item_UniIndex;

	if (pUIManager->GetInventory()->GetItemVisble(nIndex) == TRUE &&
		m_pIcon[nSlotIdx]->getItems()->IsEmptyItem() == FALSE)
		m_pIcon[nSlotIdx]->copyItem( pUIManager->GetInventory()->GetItemIcon(nIndex) );
	else
	{
		_resetIcon(nSlotIdx);
	}
}

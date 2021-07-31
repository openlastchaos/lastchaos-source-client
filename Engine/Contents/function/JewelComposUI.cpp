#include "stdh.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "JewelComposUI.h"
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UISocketSystem.h>
#include <Engine/Entities/InternalClasses.h>

#define DEF_JEWEL_MAX 3
#define JEWEL_COMPOS_SCROLL_ITEM_INDEX				9816
#define CHAOSJEWEL_COMPOS_SCROLL_ITEM_INDEX			9817
#define DEF_JEWEL_COMPOS_DIRECTION_TIME				2000
#define DEF_JEWEL_COMPOS_CHANGE_JEWEL_EFFECT_TIME	50
enum
{
	eGUIDE_STATE_NONE = 0,
	eGUIDE_STATE_AVAILABLE_COMPOS,
	eGUIDE_STATE_LACK_NAS,
	eGUIDE_STATE_COMPOSING
};

enum
{
	eJEWEL_SOUND_DULL,
	eJEWEL_SOUND_REFINEMENT,
	eJEWEL_SOUND_SHINY,
};

CJewelComposUI::CJewelComposUI()
	: m_pMoveArea(NULL)
	, m_pTbGuide(NULL)
	, m_nOpenUIType(-1)
	, m_nOneMoreComposScrollpay(0)
	, m_bComposing(false)
	, m_dwStartTime(0)
	, m_dwChanegeTime(0)
{
	int i;
	for(i = 0; i < eBTN_MAX; ++i)
		m_pBtn[i] = NULL;

	for(i = 0; i < eICON_MAX; ++i)
		m_pIcon[i] = NULL;

	for(i = 0; i < eIMG_MAX; ++i)
		m_pImg[i] = NULL;

	for(i = 0; i < eTEXT_MAX; ++i)
		m_pText[i] = NULL;

	setInherit(false);
}

void CJewelComposUI::PressOK()
{
	if (m_bComposing == true)
		return;

	_setResultImgHide(m_nOpenUIType, TRUE);
	_resetIcon(eICON_RESULT);

	if (CheckRunningConditions() == false)
	{
		_updateUI();
		return;
	}

	int SoundType = rand() % 10;
	if (SoundType > 0)
		_PlaySound(eJEWEL_SOUND_DULL);
	else
		_PlaySound(eJEWEL_SOUND_REFINEMENT);

	BtnLockOn();
	_JewelComposDirectionStart();
}

void CJewelComposUI::SetResultJewelUI( int nVirJewelIndex )
{
	m_bComposing = false;

	_PlaySound(eJEWEL_SOUND_SHINY);

	if (m_pIcon[eICON_RESULT] != NULL)
	{
		CUIIcon* pResultIcon = UIMGR()->GetInventory()->GetItemIcon(nVirJewelIndex);
		m_pIcon[eICON_RESULT]->copyItem(pResultIcon);
	}

	_SetIconDisable(false);
	_setResultImgHide(m_nOpenUIType, FALSE);
	_updateSlotIcon();
	_updateUI();	
}

void CJewelComposUI::openUI(int nOpenType)
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

	if (nOpenType < eJEWEL_COMPOS_OPEN_NORMAL || nOpenType > eJEWEL_COMPOS_OPEN_CHAOS)
		return;

	Hide(FALSE);
	SetVisible(TRUE);

	m_nOpenUIType = nOpenType;
	resetUI();
	
	pUIMgr->RearrangeOrder( UI_JEWEL_COMPOS, TRUE );
	pInven->Lock( TRUE, TRUE, LOCK_SOCKETSYSTEM );
}

void CJewelComposUI::closeUI()
{
	Hide(TRUE);
	SetVisible(FALSE);

	CUIManager* pUIMgr = UIMGR();
	pUIMgr->RearrangeOrder( UI_JEWEL_COMPOS, FALSE );
	pUIMgr->GetInventory()->Lock( FALSE, FALSE, LOCK_SOCKETSYSTEM );
}

void CJewelComposUI::resetUI()
{
	BOOL bBackImgHide;

	switch(m_nOpenUIType)
	{
	case eJEWEL_COMPOS_OPEN_NORMAL:
		bBackImgHide = TRUE;
		break;
	case eJEWEL_COMPOS_OPEN_CHAOS:
		bBackImgHide = FALSE;
		break;
	default:
		closeUI();
		return;
	}

	for (int i = 0; i < eICON_MAX; ++i)
		_resetIcon(i);

	_setResultImgHide(eJEWEL_COMPOS_OPEN_CHAOS, TRUE);
	_setResultImgHide(eJEWEL_COMPOS_OPEN_NORMAL, TRUE);
	_HideChaosBackImg(bBackImgHide);

	_setTitle();
	GuideStringChange(eGUIDE_STATE_NONE);
	SetNasString(0);
	BtnLockOn();
	m_bComposing = false;
}

void CJewelComposUI::initialize()
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
		m_pBtn[eBTN_CLOSE]->SetCommandFUp(boost::bind(&CJewelComposUI::closeUI, this));

	if (m_pBtn[eBTN_OK] != NULL)
		m_pBtn[eBTN_OK]->SetCommandFUp(boost::bind(&CJewelComposUI::PressOK, this));

	// IMG
	std::string strImgID[eIMG_MAX] = { "img_result_normal", "img_result_chaos", "img_chaos_back" };

	for(i = 0; i < eIMG_MAX; ++i)
	{
		m_pImg[i] = (CUIImage*)findUI(strImgID[i].c_str());

		if (m_pImg[i] != NULL)
			m_pImg[i]->Hide(TRUE);
	}

	// Icon
	std::string strIconID[eICON_MAX] = { "icon_jewel1", "icon_jewel2", "icon_jewel3",
		"icon_scroll", "icon_result" };

	for(i = 0; i < eICON_MAX; ++i)
	{
		m_pIcon[i] = (CUIIcon*)findUI(strIconID[i].c_str());

		if (m_pIcon[i] != NULL)
			m_pIcon[i]->clearIconData();
	}

	// Text
	std::string strTextID[eTEXT_MAX] = { "text_title", "text_nas" };

	for(i = 0; i < eTEXT_MAX; ++i)
	{
		m_pText[i] = (CUIText*)findUI(strTextID[i].c_str());
	}

	if (m_pText[eTEXT_NAS] != NULL)
	{
		CTString strGuide;
		strGuide.PrintF( _S(5939, "합성에 필요한 나스입니다."));
		m_pText[eTEXT_NAS]->setTooltip(strGuide.str_String);
	}

	m_pTbGuide = (CUITextBox*)findUI("tb_guide");
#endif	//	WORLD_EDITOR
}

WMSG_RESULT CJewelComposUI::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if (m_bComposing == true)
		return WMSG_FAIL;

	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->RearrangeOrder( UI_JEWEL_COMPOS, TRUE );

	return CUIWindow::OnLButtonDown(x, y);
}

WMSG_RESULT CJewelComposUI::OnRButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if (m_bComposing == true)
		return WMSG_FAIL;

	for (int i = eICON_JEWEL_START; i < eICON_RESULT; ++i)
	{
		if (m_pIcon[i] == NULL || m_pIcon[i]->IsEmpty() == true)
			continue;

		if (m_pIcon[i]->IsInside(x, y))
		{
			_resetIcon(i);
			_updateUI();
			return WMSG_SUCCESS;
		}
	}

	return WMSG_FAIL;
}

WMSG_RESULT CJewelComposUI::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if (m_bComposing == true)
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
			for (int i = eICON_JEWEL_START; i < eICON_JEWEL_MAX; ++i)
			{
				if (m_pIcon[i] && m_pIcon[i]->IsInside(x, y))
				{
					if (CheckJewelItem(pItemDrag) == true)
					{
						m_pIcon[i]->copyItem(pDrag);
						m_pIcon[i]->setCount(pItemDrag->Item_Sum);

						_updateUI();
					}
					else
					{
						int nError;
						if (m_nOpenUIType == eJEWEL_COMPOS_OPEN_NORMAL)
							nError = CUISocketSystem::JEWEL_COMPOS_ERROR_NOT_GENERAL_JEWEL;
						else
							nError = CUISocketSystem::JEWEL_COMPOS_ERROR_NOT_CHAOS_JEWEL;

						pUIManager->GetSocketSystem()->_showErrorSystemMessage((CUISocketSystem::_JEWEL_ERROR_TYPE)nError);
					}
					break;
				}
			}

			if (m_pIcon[eICON_SCROLL] && m_pIcon[eICON_SCROLL]->IsInside(x, y))
			{
				if (CheckScrollItem(pItemDrag) == true)
				{
					if(!pUIManager->DoesMessageBoxExist(MSGCMD_JEWEL_COMPOS_JEWEL_COMPOS_SCROLL))
					{
						CTString strMessage;
						CUIMsgBox_Info msgBoxInfo;
						CTString strItemCount;
						msgBoxInfo.SetMsgBoxInfo(_S( 5925,"보석 합성"), UMBS_OKCANCEL | UMBS_INPUTBOX, UI_JEWEL_COMPOS, MSGCMD_JEWEL_COMPOS_JEWEL_COMPOS_SCROLL);

						strItemCount.PrintF(_s("%I64d"), pItemDrag->Item_Sum);
						
						strMessage.PrintF( _S( 5930, "%s 몇 개를 등록 하시겠습니까? (최대 99개 등록, 1회 합성마다 소모)"), pItemDrag->ItemData->GetName());
						msgBoxInfo.AddString(strMessage);
						pUIManager->CreateMessageBox( msgBoxInfo );
						pUIManager->GetMessageBox(MSGCMD_JEWEL_COMPOS_JEWEL_COMPOS_SCROLL)->GetInputBox().SetMaxChar(2);
						
						m_pIcon[eICON_SCROLL]->cloneItem(pDrag);
						m_pIcon[eICON_SCROLL]->setCount(_atoi64(strItemCount));
						m_nOneMoreComposScrollpay = 1;
					}
				}
				else
					pUIManager->GetSocketSystem()->_showErrorSystemMessage(CUISocketSystem::JEWEL_COMPOS_ERROR_NOT_COMPOS_SCROLL);					
			}
		}
	}

	pUIManager->ResetHoldBtn();
	return CUIWindow::OnLButtonUp(x, y);
}

void CJewelComposUI::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	switch(nCommandCode)
	{
	case MSGCMD_JEWEL_COMPOS_JEWEL_COMPOS_SCROLL:
		{
			if (m_pIcon[eICON_SCROLL] != NULL)
			{
				if(bOK)
				{
					int nIndex = m_pIcon[eICON_SCROLL]->getIndex();
					int nMax = CUIManager::getSingleton()->GetInventory()->GetItemCount(nIndex);
					CItems* pItem = m_pIcon[eICON_SCROLL]->getItems();

					if (pItem == NULL)
					{
						_resetIcon(eICON_SCROLL);
						return;
					}

					int nItemCount = pItem->Item_Sum;

					if (strInput.IsInteger() && _atoi64(strInput) <= nMax && _atoi64(strInput) > 0)
					{
						if (_atoi64(strInput) <= nItemCount)
							m_nOneMoreComposScrollpay = _atoi64(strInput);
						else
							m_nOneMoreComposScrollpay = nItemCount;
					}
					else if (_atoi64(strInput) > nMax)
					{
						if ( nItemCount >= nMax )
							m_nOneMoreComposScrollpay = nMax;
						else
							m_nOneMoreComposScrollpay = nItemCount;
					}
					else
						m_nOneMoreComposScrollpay = 1;

					m_pIcon[eICON_SCROLL]->setCount(m_nOneMoreComposScrollpay);
				}
				else
					_resetIcon(eICON_SCROLL);
			}			
		}
		break;
	}

	if (pUIManager->DoesMessageBoxExist(nCommandCode))
		pUIManager->CloseMessageBox(nCommandCode);
}

void CJewelComposUI::GuideStringChange( int nState )
{
	if (m_pTbGuide == NULL)
		return;

	CTString strTemp;
	// 가이드 셋팅
	switch(nState)
	{
	case eGUIDE_STATE_NONE:
		{
			if (m_nOpenUIType == eJEWEL_COMPOS_OPEN_NORMAL)
				strTemp.PrintF(_S( 5931, "일반보석을 등록하세요." ));
			else
				strTemp.PrintF(_S( 5929, "카오스보석을 등록하세요." ));
		}		
		break;
	case eGUIDE_STATE_AVAILABLE_COMPOS:
		strTemp.PrintF(_S( 5932, "보석합성이 가능합니다." ));
		break;
	case eGUIDE_STATE_LACK_NAS:
		strTemp.PrintF(_S( 306, "나스가 부족합니다." ));
		break;
	default:
		strTemp.PrintF( CTString("") );
		break;
	}

	m_pTbGuide->SetText(strTemp);
}

void CJewelComposUI::SetNasString( SQUAD sqNas )
{
	// 나스 스트링 설정
	if (m_pText[eTEXT_NAS] != NULL)
	{
		CUIManager* pUIManager = CUIManager::getSingleton();
		CTString sqMoney;
		COLOR colNas = 0xFFFFFFFF;

		if (sqNas > _pNetwork->MyCharacterInfo.money)
			colNas = 0xFF0000FF;

		sqMoney.PrintF( "%I64d", sqNas );
		pUIManager->InsertCommaToString( sqMoney );

		m_pText[eTEXT_NAS]->SetText(sqMoney);
		m_pText[eTEXT_NAS]->setFontColor(colNas);
	}
}

bool CJewelComposUI::CheckJewelItem( CItems* pItem )
{
	if (pItem == NULL)
		return false;

	CItemData* pItemData = pItem->ItemData;

	if (pItemData == NULL)
		return false;

	int nSubItemType;

	if (m_nOpenUIType == eJEWEL_COMPOS_OPEN_NORMAL)
		nSubItemType = CItemData::ITEM_ETC_JEWEL;
	else
		nSubItemType = CItemData::ITEM_ETC_CHAOSJEWEL;

	if ( pItemData->GetType() == CItemData::ITEM_ETC &&
		pItemData->GetSubType() == nSubItemType )
		return true;

	return false;
}

bool CJewelComposUI::CheckScrollItem( CItems* pItem )
{
	if (pItem == NULL)
		return false;

	CItemData* pItemData = pItem->ItemData;

	if (pItemData == NULL)
		return false;

	int nScrollIndex;
	if (m_nOpenUIType == eJEWEL_COMPOS_OPEN_NORMAL)
		nScrollIndex = JEWEL_COMPOS_SCROLL_ITEM_INDEX;
	else
		nScrollIndex = CHAOSJEWEL_COMPOS_SCROLL_ITEM_INDEX;

	if (pItemData->GetType() == CItemData::ITEM_ETC &&
		pItemData->GetSubType() == CItemData::ITEM_ETC_FUNCTIONS &&
		pItemData->GetItemIndex() == nScrollIndex)
		return true;

	return false;
}

bool CJewelComposUI::CheckRunningConditions()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CUISocketSystem* pSockSystem = pUIManager->GetSocketSystem();

	if( pUIManager->GetInventory()->IsInventoryFull() )
	{
		pSockSystem->_showErrorSystemMessage(CUISocketSystem::JEWEL_COMPOS_ERROR_NOT_INVENTORY_EMPTY);
		return false;
	}	

	_updateSlotIcon();

	for (int i = eICON_JEWEL_START; i < eICON_JEWEL_MAX; ++i)
	{
		if (m_pIcon[i] == NULL || m_pIcon[i]->getItems() == NULL)
		{
			pSockSystem->_showErrorSystemMessage(CUISocketSystem::JEWEL_COMPOS_ERROR_JEWEL_LACK);
			return false;
		}
	}

	return true;
}

void CJewelComposUI::BtnLockOn()
{
	// Btn Lock
	if (m_pBtn[eBTN_OK] != NULL)
		m_pBtn[eBTN_OK]->SetEnable(FALSE);
}

void CJewelComposUI::BtnLockOff()
{
	// Btn Lock 해제
	if (m_pBtn[eBTN_OK] != NULL)
		m_pBtn[eBTN_OK]->SetEnable(TRUE);
}

void CJewelComposUI::_resetIcon( int nSlotIdx )
{
	if (nSlotIdx < 0 || nSlotIdx >= eICON_MAX)
		return;

	CTString textEmptyGuide;

	switch(nSlotIdx)
	{
	case eICON_JEWEL1:
	case eICON_JEWEL2:
	case eICON_JEWEL3:
		{
			if (m_nOpenUIType == eJEWEL_COMPOS_OPEN_NORMAL)
				textEmptyGuide.PrintF( _S(5931, "일반보석을 등록하세요"));
			else
				textEmptyGuide.PrintF( _S(5929, "카오스보석을 등록하세요"));
		}
		break;
	case eICON_SCROLL:
		textEmptyGuide.PrintF( _S(5937, "보석 합성스크롤을 등록하면 합성에 도움이 됩니다."));
		m_nOneMoreComposScrollpay = 0;
		break;
	case eICON_RESULT:
		textEmptyGuide.PrintF( _S(5938, "합성으로 생성되는 보석을 표시합니다."));
		break;
	default:
		textEmptyGuide.PrintF( CTString(""));
		break;
	}

	if (m_pIcon[nSlotIdx] != NULL)
	{
		m_pIcon[nSlotIdx]->clearIconData();
		m_pIcon[nSlotIdx]->setTooltip(textEmptyGuide.str_String);
	}
}

void CJewelComposUI::_HideChaosBackImg( BOOL bHide )
{
	if (m_pImg[eIMG_CHAOS_UI_BACK] != NULL)
		m_pImg[eIMG_CHAOS_UI_BACK]->Hide(bHide);
}

void CJewelComposUI::_setTitle()
{
	CTString strTitle;

	if (m_nOpenUIType == eJEWEL_COMPOS_OPEN_NORMAL)
		strTitle.PrintF(_S(5927,"일반보석 합성"));
	else
		strTitle.PrintF(_S(5928,"카오스보석 합성"));

	if (m_pText[eTEXT_TITLE] != NULL)
		m_pText[eTEXT_TITLE]->SetText(strTitle);
}

void CJewelComposUI::_setResultImgHide( int nOpenType, BOOL bHide )
{
	if (nOpenType < 0 || nOpenType > eJEWEL_COMPOS_OPEN_CHAOS)
		return;

	if (m_pImg[nOpenType] != NULL)
		m_pImg[nOpenType]->Hide(bHide);
}

void CJewelComposUI::_updateUI()
{
	// guide update
	_updateGuide();

	// nas setting
	SetNasString(calcConsumeNas());

	// Btn update
	if(_getRegistJewelCount() >= DEF_JEWEL_MAX && _pNetwork->MyCharacterInfo.money >= calcConsumeNas())
		BtnLockOff();
	else
		BtnLockOn();
}

void CJewelComposUI::_updateGuide()
{
	if (m_bComposing == true)
	{
		GuideStringChange(eGUIDE_STATE_COMPOSING);
		return;
	}

	if (_getRegistJewelCount() < DEF_JEWEL_MAX)
		GuideStringChange(eGUIDE_STATE_NONE);
	else
	{
		if (_pNetwork->MyCharacterInfo.money >= calcConsumeNas())
			GuideStringChange(eGUIDE_STATE_AVAILABLE_COMPOS);
		else
			GuideStringChange(eGUIDE_STATE_LACK_NAS);
	}
}

void CJewelComposUI::_updateSlotIcon()
{
	CUIInventory* pInvenUI = UIMGR()->GetInventory();

	int nJeweluniIdx[eICON_JEWEL_MAX];

	for(int i = eICON_JEWEL_START; i < eICON_JEWEL_MAX; i++)
	{
		if (m_pIcon[i] == NULL || m_pIcon[i]->getItems() == NULL)
			continue;

		nJeweluniIdx[i] = m_pIcon[i]->getItems()->Item_UniIndex;
		CItems* pItems = pInvenUI->GetItems(nJeweluniIdx[i]);

		if (pItems == NULL || pItems->IsEmptyItem() == TRUE)
		{
			nJeweluniIdx[i] = -1;
			_resetIcon(i);
			continue;
		}

		m_pIcon[i]->setData(pItems);
	}

	int nTempCount = 0; // 동일 보석의 소모 총 갯수
	int nSameIndex = 0; // 동일한 보석의 고유 인덱스
	int nInvenCount = 0;

	// 1번슬롯과 2번슬롯만 같을때
	if ( nJeweluniIdx[eICON_JEWEL1] == nJeweluniIdx[eICON_JEWEL2] )
	{
		if (nJeweluniIdx[eICON_JEWEL1] > 0)
		{
			nTempCount = 2;
			nSameIndex = nJeweluniIdx[eICON_JEWEL1];

			// 3개의 슬롯이 모두 같은 보석일때
			if ( nJeweluniIdx[eICON_JEWEL1] == nJeweluniIdx[eICON_JEWEL3] )
				nTempCount = 3;

			while (pInvenUI->GetItemCountByUniIdx(nSameIndex) < nTempCount)
			{
				_resetIcon(nTempCount - 1);
				nTempCount--;
			}
		}
	}
	// 2번슬롯과 3번슬롯만 같을때
	else if( nJeweluniIdx[eICON_JEWEL2] == nJeweluniIdx[eICON_JEWEL3] )
	{
		if (nJeweluniIdx[eICON_JEWEL2] > 0)
		{
			nTempCount = 2;
			nSameIndex = nJeweluniIdx[eICON_JEWEL2];
			nInvenCount = pInvenUI->GetItemCountByUniIdx(nSameIndex);

			if (nInvenCount < nTempCount)
			{
				if (nInvenCount <= 0)
					_resetIcon(eICON_JEWEL2);

				_resetIcon(eICON_JEWEL3);
			}
		}
	}
	// 1번슬롯과 3번슬롯만 같을때
	else if( nJeweluniIdx[eICON_JEWEL1] == nJeweluniIdx[eICON_JEWEL3] )
	{
		if (nJeweluniIdx[eICON_JEWEL1] > 0)
		{
			nTempCount = 2;
			nSameIndex = nJeweluniIdx[eICON_JEWEL1];
			nInvenCount = pInvenUI->GetItemCountByUniIdx(nSameIndex);

			if (nInvenCount < nTempCount)
			{
				if (nInvenCount <= 0)
					_resetIcon(eICON_JEWEL1);

				_resetIcon(eICON_JEWEL3);
			}
		}
	}

	if (m_pIcon[eICON_SCROLL] != NULL && m_pIcon[eICON_SCROLL]->IsEmpty() == false)
	{
		int	nIndex = m_pIcon[eICON_SCROLL]->getIndex();
		int ItemIdx = m_pIcon[eICON_SCROLL]->getItems()->Item_UniIndex;

		if( pInvenUI->GetItemVisble(ItemIdx) )
		{
			m_pIcon[eICON_SCROLL]->setCount(m_nOneMoreComposScrollpay);

			if (pInvenUI->GetItemCount(nIndex) < m_nOneMoreComposScrollpay)
				_resetIcon(eICON_SCROLL);
		}
		else
			_resetIcon(eICON_SCROLL);
	}
}

int CJewelComposUI::_getRegistJewelCount()
{
	int i, nRegistCnt = 0;

	for (i = eICON_JEWEL_START; i < eICON_JEWEL_MAX; i++)
	{
		if (m_pIcon[i] == NULL)
			continue;

		if (m_pIcon[i]->IsEmpty() == true)
			continue;

		nRegistCnt++;
	}

	return nRegistCnt;
}

SQUAD CJewelComposUI::calcConsumeNas()
{
	int i, tempNas = 0;
	int JewelGrade = 0;
	bool bChaos = false;

	for (i = eICON_JEWEL_START; i < eICON_JEWEL_MAX; i++)
	{
		if (m_pIcon[i] == NULL)
			continue;

		if (m_pIcon[i]->IsEmpty() == true)
		{
			tempNas = 0;
			JewelGrade = 0;
			break;
		}

		if (m_pIcon[i]->getItems() == NULL)
			continue;

		CItemData* pItemData = m_pIcon[i]->getItems()->ItemData;
		JewelGrade += pItemData->GetNum0();

		if (pItemData->GetSubType() == CItemData::ITEM_ETC_CHAOSJEWEL)
			bChaos = true;
		else
			bChaos = false;
	}

	if (JewelGrade > 0)
		tempNas = _pNetwork->GetJewelComposNas(JewelGrade / DEF_JEWEL_MAX, bChaos);

	return tempNas;
}

void CJewelComposUI::_PlaySound( int nSoundType )
{
	CEntity			*penPlEntity;
	CPlayerEntity	*penPlayerEntity;

	penPlEntity = CEntity::GetPlayerEntity( 0 );
	penPlayerEntity = (CPlayerEntity *)penPlEntity;

	switch(nSoundType)
	{
	case eJEWEL_SOUND_DULL:
		penPlayerEntity->PlayJewelDullSound();
		break;
	case eJEWEL_SOUND_REFINEMENT:
		penPlayerEntity->PlayJewelRefinementSound();
		break;
	case eJEWEL_SOUND_SHINY:
		penPlayerEntity->PlayJewelShinySound();
		break;
	}
}

void CJewelComposUI::_JewelComposDirectionEnd()
{
	m_bComposing = false;

	if (CheckRunningConditions() == false)
	{
		_updateUI();
		_SetIconDisable(false);
		return;
	}

	CUISocketSystem::stPACKET_ITEM stPackJewel[eICON_MAX];

	for (int i = eICON_JEWEL_START; i < eICON_JEWEL_MAX; ++i)
	{
		stPackJewel[i].Item_Tab = m_pIcon[i]->getItems()->Item_Tab;
		stPackJewel[i].InvenIdx = m_pIcon[i]->getItems()->InvenIndex;
		stPackJewel[i].ItemVirIdx = m_pIcon[i]->getItems()->Item_UniIndex;
	}

	if (m_pIcon[eICON_SCROLL] && m_pIcon[eICON_SCROLL]->IsEmpty() == false)
	{
		stPackJewel[eICON_SCROLL].Item_Tab = m_pIcon[eICON_SCROLL]->getItems()->Item_Tab;
		stPackJewel[eICON_SCROLL].InvenIdx = m_pIcon[eICON_SCROLL]->getItems()->InvenIndex;
		stPackJewel[eICON_SCROLL].ItemVirIdx = m_pIcon[eICON_SCROLL]->getItems()->Item_UniIndex;
	}	

	bool bChaosJewel = false;

	if (m_nOpenUIType == eJEWEL_COMPOS_OPEN_CHAOS)
		bChaosJewel = true;

	UIMGR()->GetSocketSystem()->SendUpgradeReq(bChaosJewel, stPackJewel[eICON_JEWEL1], stPackJewel[eICON_JEWEL2], stPackJewel[eICON_JEWEL3],
		stPackJewel[eICON_SCROLL], m_nOneMoreComposScrollpay);
}

BOOL CJewelComposUI::_CheckComposDirectionTime()
{
	DWORD dwCurrentTime = timeGetTime();

	if( dwCurrentTime - m_dwStartTime > DEF_JEWEL_COMPOS_DIRECTION_TIME )
		return TRUE;

	return FALSE;
}

void CJewelComposUI::_JewelComposDirectionStart()
{
	if (_JewelComposDirectionGradeInfo() == FALSE)
		return;

	// 시작 시간 저장
	m_dwStartTime = timeGetTime();
	m_dwChanegeTime = m_dwStartTime;

	// 합성 연출 시작
	m_bComposing = true;
	m_iterChanegeInfo = m_DirectionInfo.begin();

	_SetIconDisable(true);
}

void CJewelComposUI::_JewelComposDirection()
{
	BOOL bEnd = _CheckComposDirectionTime();

	if(bEnd)
	{
		_JewelComposDirectionEnd();
	}
	else
	{
		_ChangeDirectionEfect();
	}
}

BOOL CJewelComposUI::_JewelComposDirectionGradeInfo()
{
	m_DirectionInfo.clear();

	int i, Grade[DEF_JEWEL_MAX], nAverageGrade = 0, nSum = 0;

	for (i = eICON_JEWEL_START; i < eICON_JEWEL_MAX; i++)
	{
		if (m_pIcon[i] == NULL || m_pIcon[i]->IsEmpty() == true)
			return FALSE;

		CItemData* pItemData = m_pIcon[i]->getItems()->ItemData;
		Grade[i] = pItemData->GetNum0();
		nSum += Grade[i];
	}

	nAverageGrade = nSum / DEF_JEWEL_MAX;

	if (m_nOpenUIType == eJEWEL_COMPOS_OPEN_NORMAL)
		return _pNetwork->GetJewelGradeInfo(nAverageGrade, m_DirectionInfo);
	else 
		return _pNetwork->GetChaosJewelGradeInfo(nAverageGrade, m_DirectionInfo);
}

void CJewelComposUI::_ChangeDirectionEfect()
{
	DWORD dwCurrentTime = timeGetTime();

	if(dwCurrentTime > m_dwChanegeTime)
	{
		m_dwChanegeTime += DEF_JEWEL_COMPOS_CHANGE_JEWEL_EFFECT_TIME;

		int jewelindex = m_iterChanegeInfo->Jewel_idx;

		if (m_pIcon[eICON_RESULT] != NULL)
			m_pIcon[eICON_RESULT]->setData(UBET_ITEM, jewelindex);

		++m_iterChanegeInfo;

		if (m_iterChanegeInfo == m_DirectionInfo.end())
			m_iterChanegeInfo = m_DirectionInfo.begin();		
	}
}

void CJewelComposUI::OnUpdate( float fDeltaTime, ULONG ElapsedTime )
{
	if (m_bComposing == true)
		_JewelComposDirection();
}

void CJewelComposUI::_SetIconDisable(bool bDisable)
{
	for (int i = eICON_JEWEL_START; i < eICON_RESULT; i++)
	{
		if (m_pIcon[i] == NULL)
			continue;

		m_pIcon[i]->SetDisable(bDisable);
	}
}


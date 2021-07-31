#include "stdh.h"
#include "SocketCreateUI.h"

#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UISocketSystem.h>

#define PRICE_CREATE (0.25f)

enum eGUIDE_STATE
{
	eSTATE_NONE = 0,
	eSTATE_REGIST_ITEM,
	eSTATE_ALL_READY,
	eSTATE_PROCESSING,
	eSTATE_CREATE_SUCCESS,
	eSTATE_FAIL
};

// 소켓 생성 칼라
static COLOR _ColorStr[JEWEL_MAX_COUNT] = 
{
	0x1C54CCFF,
	0x20A51CFF,
	0xE5E230FF,
	0xB2FDB7FF,
	0x01FDEEFF,
	0xCC66FFFF,
	0xFFFFFFFF
};

//--------------------------------------------------------------
// 프로그래스 완료
class CmdCreateSocket : public Command
{
public:
	CmdCreateSocket() : pUI_(NULL) {}
	void setData(CSocketCreateUI* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->ProgressComplete();
	}
private:
	CSocketCreateUI* pUI_;
};

CSocketCreateUI::CSocketCreateUI()
	: m_pMoveArea(NULL)
	, m_pProgressBar(NULL)
	, m_bProgress(false)
{
	int i;
	for(i = 0; i < eBTN_MAX; ++i)
		m_pBtn[i] = NULL;

	for(i = 0; i < eTEXT_MAX; ++i)
		m_pText[i] = NULL;

	for(i = 0; i < eICON_MAX; ++i)
		m_pIcon[i] = NULL;

	setInherit(false);
}

void CSocketCreateUI::PressOK()
{
	if (m_bProgress == true)
		return;

	// 프로그래스바 스타트
	if (m_pProgressBar != NULL)
	{
		m_pProgressBar->StartProgress();
		m_bProgress = true;

		// 버튼 락처리
		BtnLockOn();
		changeGuide(eSTATE_PROCESSING);

		if (m_pIcon[eICON_RESULT] != NULL)
			m_pIcon[eICON_RESULT]->clearIconData();
	}
}

void CSocketCreateUI::ProgressComplete()
{
	int i;

	CItems* pItem[eICON_RESULT];

	for (i = 0; i < eICON_RESULT; ++i)
	{
		if (m_pIcon[i] == NULL)
			return;

		pItem[i] = m_pIcon[i]->getItems();

		if (pItem[i] == NULL)
			return;
	}

	// Send Message
	UIMGR()->GetSocketSystem()->SendCreateReq(pItem[eICON_ITEM]->Item_Tab, pItem[eICON_ITEM]->InvenIndex, pItem[eICON_ITEM]->Item_UniIndex,
		pItem[eICON_SCROLL]->Item_Tab, pItem[eICON_SCROLL]->InvenIndex, pItem[eICON_SCROLL]->Item_UniIndex);
}

void CSocketCreateUI::BtnLockOn()
{
	// Btn Lock
	if (m_pBtn[eBTN_OK] != NULL)
		m_pBtn[eBTN_OK]->SetEnable(FALSE);
}

void CSocketCreateUI::BtnLockOff()
{
	// Btn Lock 해제
	if (m_pBtn[eBTN_OK] != NULL)
		m_pBtn[eBTN_OK]->SetEnable(TRUE);
}

void CSocketCreateUI::ShowResultSuccess(int nItemVirIdx)
{
	// UI 리셋
	_setCompleteUI();
	
	// 결과 아이템 셋팅
	CUIInventory* pInven = UIMGR()->GetInventory();

	if (pInven == NULL)
		return;
	
	if (m_pIcon[eICON_RESULT] != NULL)
		m_pIcon[eICON_RESULT]->copyItem(pInven->GetItemIcon(nItemVirIdx));

	pInven->Lock( FALSE, FALSE, LOCK_SOCKETSYSTEM );
	pInven->Lock( TRUE, TRUE, LOCK_SOCKETSYSTEM );	
	changeGuide(eSTATE_CREATE_SUCCESS);
}

void CSocketCreateUI::ShowResultFail()
{
	if (m_pIcon[eICON_SCROLL] != NULL)
		m_pIcon[eICON_SCROLL]->clearIconData();

	m_bProgress = false;
	SetNasString(0);
	changeGuide(eSTATE_FAIL);
}

void CSocketCreateUI::openUI()
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
	
	pUIMgr->RearrangeOrder( UI_SOCKET_CREATE, TRUE );
	pInven->Lock( TRUE, TRUE, LOCK_SOCKETSYSTEM );
}

void CSocketCreateUI::closeUI()
{
	Hide(TRUE);
	SetVisible(FALSE);

	CUIManager* pUIMgr = UIMGR();
	pUIMgr->RearrangeOrder( UI_SOCKET_CREATE, FALSE );
	pUIMgr->GetInventory()->Lock( FALSE, FALSE, LOCK_SOCKETSYSTEM );
}

void CSocketCreateUI::resetUI()
{
	_setCompleteUI();
	BtnLockOn();

	if (m_pProgressBar != NULL)
	{
		m_pProgressBar->IdleProgress();
		m_pProgressBar->SetProgressPos(0);
	}

	changeGuide(eSTATE_NONE);
}

void CSocketCreateUI::initialize()
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
	std::string strBtnID[eBTN_MAX] = { "btn_close", "btn_ok" };

	for(int i = 0; i < eBTN_MAX; ++i)
		m_pBtn[i] = (CUIButton*)findUI(strBtnID[i].c_str());

	if (m_pBtn[eBTN_CLOSE] != NULL)
		m_pBtn[eBTN_CLOSE]->SetCommandFUp(boost::bind(&CSocketCreateUI::closeUI, this));

	if (m_pBtn[eBTN_OK] != NULL)
		m_pBtn[eBTN_OK]->SetCommandFUp(boost::bind(&CSocketCreateUI::PressOK, this));

	// Text
	std::string strTextID[eTEXT_MAX] = { "text_guide", "text_nas" };

	for(int i = 0; i < eTEXT_MAX; ++i)
		m_pText[i] = (CUIText*)findUI(strTextID[i].c_str());

	// Icon
	std::string strIconID[eICON_MAX] = { "icon_item", "icon_scroll", "icon_result" };

	for(int i = 0; i < eICON_MAX; ++i)
	{
		m_pIcon[i] = (CUIIcon*)findUI(strIconID[i].c_str());

		if (m_pIcon[i] != NULL)
			m_pIcon[i]->clearIconData();
	}

	m_pProgressBar = (CUIProgressBar*)findUI("pb_bar");

	if (m_pProgressBar != NULL)
	{
		CmdCreateSocket* pCmd = new CmdCreateSocket;
		pCmd->setData(this);
		m_pProgressBar->SetCmdComplete(pCmd);
	}
#endif	//	WORLD_EDITOR
}

WMSG_RESULT CSocketCreateUI::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if (m_bProgress == true)
		return WMSG_FAIL;

	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->RearrangeOrder( UI_SOCKET_CREATE, TRUE );

	return CUIWindow::OnLButtonDown(x, y);
}

WMSG_RESULT CSocketCreateUI::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if (m_bProgress == true)
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
				if (m_pProgressBar != NULL)
				{
					m_pProgressBar->IdleProgress();
					m_pProgressBar->SetProgressPos(0);
				}

				if (CheckItem(pItemDrag) == true)
				{
					m_pIcon[eICON_ITEM]->copyItem(pDrag);
					SQUAD sqNeedMoney = calcConsumeNas(m_pIcon[eICON_ITEM]->getItems());
					SetNasString(sqNeedMoney);
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
				}
				else
					pUIManager->GetSocketSystem()->showErrorMessage();
			}

			if (m_pIcon[eICON_ITEM] && m_pIcon[eICON_SCROLL])
			{
				if (m_pIcon[eICON_ITEM]->IsEmpty() == false &&
					m_pIcon[eICON_SCROLL]->IsEmpty() == false)
					BtnLockOff();
			}
		}
	}

	pUIManager->ResetHoldBtn();
	return CUIWindow::OnLButtonUp(x, y);
}

void CSocketCreateUI::changeGuide( int nState )
{
	if (m_pText[eTEXT_GUIDE] == NULL)
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
		strTemp.PrintF( _S( 4984, "스크롤을 올려주세요.") );
		break;
	case eSTATE_ALL_READY:
		strTemp.PrintF( _S( 5914, "생성 버튼을 눌러주세요.") );
		break;
	case eSTATE_CREATE_SUCCESS:
		{
			if (m_pIcon[eICON_RESULT] != NULL)
			{
				CItems* pResultItem = m_pIcon[eICON_RESULT]->getItems();
				if (pResultItem != NULL)
				{
					int nSocketCount = pResultItem->Item_SocketCount;
					strTemp.PrintF( _S( 4974, "%d SOCKET SUCCESS!!"),  nSocketCount );
					colorTemp = _ColorStr[nSocketCount - 2]; // 소캣 생성은 최소 2
				}
			}			
		}
		break;
	case eSTATE_FAIL:
		strTemp.PrintF( _S( 4975, "소켓 추가 실패") );
		colorTemp = 0xFF0000FF;
		break;

	default:
		strTemp.PrintF( CTString("") );
		break;
	}

	m_pText[eTEXT_GUIDE]->SetText(strTemp);
	m_pText[eTEXT_GUIDE]->setFontColor(colorTemp);	
}

void CSocketCreateUI::SetNasString( SQUAD sqNas )
{
	// 나스 스트링 설정
	if (m_pText[eTEXT_NAS] != NULL)
	{
		CUIManager* pUIManager = CUIManager::getSingleton();
		CTString sqMoney;
		sqMoney.PrintF( "%I64d", sqNas );
		pUIManager->InsertCommaToString( sqMoney );
		m_pText[eTEXT_NAS]->SetText(sqMoney);
		m_pText[eTEXT_NAS]->setFontColor(pUIManager->GetNasColor(sqMoney));
	}
}

bool CSocketCreateUI::CheckItem( CItems* pItem )
{
	// 소켓을 뚫을 장비아이템 체크
	if (pItem == NULL)
		return false;

	// 거래대행 불가								
	CItemData* pItemData = pItem->ItemData;

	if (pItemData == NULL)
		return false;

	if (!pItemData->IsFlag(ITEM_FLAG_SOCKET))
		return false;

	if (pItem->Item_Wearing >= 0)
		return false;

	if ( pItem->IsFlag(FLAG_ITEM_SOCKET) || pItem->Item_Flag & FLAG_ITEM_COMPOSITION)
		return false;

	int nGuideType = eSTATE_REGIST_ITEM;

	if (m_pIcon[eICON_SCROLL] != NULL)
	{
		if (m_pIcon[eICON_SCROLL]->IsEmpty() == false)
			nGuideType = eSTATE_ALL_READY;
	}

	changeGuide(nGuideType);
	return true;
}

bool CSocketCreateUI::CheckScrollItem( CItems* pItem )
{
	if (pItem == NULL)
		return false;

	// 재료 스크롤 체크
	CItemData* pItemData = pItem->ItemData;

	if (pItemData == NULL)
		return false;
	
	if( pItemData->GetType() == CItemData::ITEM_ETC &&
		pItemData->GetSubType() == CItemData::ITEM_ETC_PROC_SCROLL )
	{
		int nGuideType = eSTATE_ALL_READY;

		if (m_pIcon[eICON_ITEM] != NULL)
		{
			if (m_pIcon[eICON_ITEM]->IsEmpty() == true)
				nGuideType = eSTATE_NONE;
		}

		changeGuide(nGuideType);
		return true;
	}

	return false;
}

SQUAD CSocketCreateUI::calcConsumeNas( CItems* pItem )
{
	// 소비 나스 계산
	if (pItem == NULL)
		return 0;

	if (pItem->ItemData == NULL)
		return 0;

	SQUAD sqNeedMoney = 0.0f;

	if(  pItem->ItemData->IsFlag( ITEM_FLAG_LORD ) )
		sqNeedMoney = 0;
	else
		sqNeedMoney = pItem->ItemData->GetPrice() * PRICE_CREATE;

	return sqNeedMoney;
}

void CSocketCreateUI::_setCompleteUI()
{
	for (int i = 0; i < eICON_MAX; ++i)
	{
		if (m_pIcon[i] != NULL)
			m_pIcon[i]->clearIconData();
	}

	m_bProgress = false;
	SetNasString(0);
}

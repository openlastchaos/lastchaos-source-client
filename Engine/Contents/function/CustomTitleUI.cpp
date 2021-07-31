#include "stdh.h"
#include "CustomTitleUI.h"
#include "CustomTitleCmd.h"
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIImageSplit.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/function/TitleData.h>
#include <Engine/Interface/UIArray.h>
#include <Engine/Interface/UIArrayItem.h>
#include <Engine/Interface/UITextBox.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UIFiltering.h>
#include <Engine/Contents/Base/UIMsgBoxMgr.h>
#include <Engine/Contents/Base/ChattingUI.h>

const INDEX _PreviewX = 259, _PreviewY = 478, _RTWidth = 259, _RTHeight = 478;
const int _ShowSelectItem_Max = 5;

#define MODEL_TREASURE	("Data\\Item\\Common\\ITEM_treasure02.smc")

CCustomTitleUI::CCustomTitleUI()
	: m_ModelRenderTarget(NULL)
	, m_pMoveArea(NULL)
	, m_pImgPreviewBack(NULL)
	, m_pEditTitleName(NULL)
	, m_pTbGuide(NULL)
	, m_pIconCashItem(NULL)
	, m_bDrag(false)
	, m_nOriX(0), m_nOriY(0)
	, m_nMouseX(0)
	, m_nMouseY(0)
	, m_nMsgID(0)
{
	int i;

	for (i = 0; i < eBTN_MAX; ++i)
		m_pbtn[i] = NULL;

	for (i = 0; i < eMENU_MAX; ++i)
		m_pArray[i] = NULL;

	setInherit(false);
}

CCustomTitleUI::~CCustomTitleUI()
{
	SAFE_DELETE(m_ModelRenderTarget);
	Destroy();
}

void CCustomTitleUI::MakeTitle()
{
	// 아이템 체크
	int nError = _CheckMakeError();

	if (nError != eMAKE_ERROR_SUCCESS)
	{
		ShowErrorMessage(nError);
		return;
	}

	TitleNetwork* pTitle = GAMEDATAMGR()->GetTitleNetwork();

	if (pTitle == NULL)
		return;

	CItems* pItem = m_pIconCashItem->getItems();
	
	pTitle->setSelectFCIndex(_getSelectIndex(eMENU_FRONT_COLOR));
	pTitle->setSelectBCIndex(_getSelectIndex(eMENU_BACK_COLOR));
	pTitle->setSelectEffectIndex(_getSelectIndex(eMENU_EFFECT));
	pTitle->setCustomTitleName(_getTitleName());

	pTitle->SendTitleMake(pItem->Item_Tab, pItem->InvenIndex, pItem->Item_UniIndex);
	m_nMsgID = 0;
}

void CCustomTitleUI::CancelMakeTitle()
{
	_CheckMakeBtnEnable();
	_CheckMsgBox();
}

void CCustomTitleUI::ChangeMenu( int nMenuType, eChangeType eType )
{
	if (nMenuType < 0 || nMenuType >= eMENU_MAX)
		return;

	int nMaxIndex = 0;
	
	switch(nMenuType)
	{
	case eMENU_FRONT_COLOR:
		nMaxIndex = CustomTitleData::m_vecFrontColor.size();
		break;
	case eMENU_BACK_COLOR:
		nMaxIndex = CustomTitleData::m_vecBackColor.size();
		break;
	case eMENU_EFFECT:
		nMaxIndex = CustomTitleData::m_vecCustomTitleEffect.size();
		break;
	}

	switch(eType)
	{
	case eChange_Left:
		{
			if (m_nShowStartIdx[nMenuType] <= 0)
			{
				m_nShowStartIdx[nMenuType] = 0;
				return;
			}
			else
				m_nShowStartIdx[nMenuType] -= 1;
		}
		break;

	case eChange_Right:
		{
			if (m_nShowStartIdx[nMenuType] + _ShowSelectItem_Max >= nMaxIndex)
			{
				m_nShowStartIdx[nMenuType] = nMaxIndex - _ShowSelectItem_Max;
				return;
			}
			else
				m_nShowStartIdx[nMenuType] += 1;
		}
		break;
	}

	_updateSelectMenu(nMenuType);
	
	if (nMenuType == eMENU_EFFECT)
		SelectEffect();	
}

void CCustomTitleUI::SelectEffect()
{
	int nEffectKey = _getSelectIndex(eMENU_EFFECT);
	CTString strEffectName = CTString("");

	if (nEffectKey >= 0)
		strEffectName = CustomTitleData::m_vecCustomTitleEffect[nEffectKey].c_str();

	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->HUD_SetTitleEffect(strEffectName);
	
	TitleNetwork* pTitle = GAMEDATAMGR()->GetTitleNetwork();

	if (pTitle != NULL)
		pTitle->setSelectEffectIndex(nEffectKey);

	_CheckMakeBtnEnable();
}

void CCustomTitleUI::SelectFrontColor()
{
	int nColorIdx = _getSelectIndex(eMENU_FRONT_COLOR);

	TitleNetwork* pTitle = GAMEDATAMGR()->GetTitleNetwork();

	if (pTitle != NULL)
		pTitle->setSelectFCIndex(nColorIdx);

	_CheckMakeBtnEnable();
}

void CCustomTitleUI::SelectBackColor()
{
	int nColorIdx = _getSelectIndex(eMENU_BACK_COLOR);

	TitleNetwork* pTitle = GAMEDATAMGR()->GetTitleNetwork();

	if (pTitle != NULL)
		pTitle->setSelectBCIndex(nColorIdx);

	_CheckMakeBtnEnable();
}

void CCustomTitleUI::openUI(float fX, float fZ )
{
	CUIManager* pUIMgr = UIMGR();
	// If inventory is already locked
	if (pUIMgr->GetInventory()->IsLocked() == TRUE ||
		pUIMgr->GetInventory()->IsLockedArrange() == TRUE)
	{
		pUIMgr->GetInventory()->ShowLockErrorMessage();
		return;
	}

	if (IsVisible() == TRUE)
		return;

	if (((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsHudModel_Used() == TRUE)
		return;
	
	Hide(FALSE);
	SetVisible(TRUE);

	if(m_ModelRenderTarget == NULL)
	{
		m_ModelRenderTarget = new CRenderTexture();
		m_ModelRenderTarget->Init(_RTWidth, _RTHeight);
	}

	_HudModelSetting();

	_resetUI();
	updateUI();

	m_fNpcX = fX;
	m_fNpcZ = fZ;
	
	pUIMgr->GetInventory()->Lock(TRUE, TRUE, LOCK_MAKE_TITLE);
	pUIMgr->RearrangeOrder( UI_CUSTOM_TITLE, TRUE );
}

void CCustomTitleUI::_HudModelSetting()
{
	CPlayerEntity* pHud = ((CPlayerEntity*)CEntity::GetPlayerEntity(0));

	if (pHud != NULL)
	{
		pHud->HUD_SetModelData(HUD_CHARACTER_TYPE, -1, eRENDER_UI_TYPE_MAKETITLE);

		int nItemIndex[WEAR_TOTAL];

		for (int i = 0; i < WEAR_TOTAL; ++i)
		{
			nItemIndex[i] = 0;

			if (_pNetwork->MyWearItem[i].Item_Index > 0)
			{
				if ((CTString)_pNetwork->MyWearItem[i].ItemData->GetItemSmcFileName() != MODEL_TREASURE)
				{
					nItemIndex[i] = _pNetwork->MyWearItem[i].Item_Index;
				}				
			}

			if (i < WEAR_COSTUME_TOTAL && _pNetwork->MyWearCostItem[i].Item_Index > 0)
			{
				if (_pNetwork->MyWearCostItem[i].ItemData->IsFlag(ITEM_FLAG_INVISIBLE_COSTUME) == false)
				{
					nItemIndex[i] = _pNetwork->MyWearCostItem[i].Item_Index;
				}				
			}

			pHud->HUD_WearingModel(i, _pNetwork->MyCharacterInfo.job, nItemIndex[i], FALSE);
		}

		pHud->SetHudModel_Use(TRUE);
	}	
}

void CCustomTitleUI::closeUI()
{
	Hide(TRUE);
	SetVisible(FALSE);

	SAFE_DELETE(m_ModelRenderTarget);
	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetHudModel_Use(FALSE);

	CUIManager* pUIMgr = UIMGR();
	_CheckMsgBox();

	pUIMgr->GetInventory()->Lock(FALSE, FALSE, LOCK_MAKE_TITLE);
	pUIMgr->RearrangeOrder( UI_CUSTOM_TITLE, FALSE );
}

void CCustomTitleUI::updateUI()
{
	for (int i = 0; i < eMENU_MAX; ++i)
	{
		_updateSelectMenu(i);
	}	
	_updatePreview();
}

void CCustomTitleUI::_updateSelectMenu( int nMenuType )
{
	if (nMenuType < 0 || nMenuType >= eMENU_MAX)
		return;

	if (m_pArray[nMenuType] == NULL)
		return;

	for (int i = 0; i < _ShowSelectItem_Max; ++i)
	{
		CUIArrayItem* pItem = m_pArray[nMenuType]->GetArrayItem(i);
		
		int nGetKey = i + m_nShowStartIdx[nMenuType];

		CUIImage* pImg = (CUIImage*)pItem->findUI("img_slot_color");

		if (nMenuType == eMENU_EFFECT)
		{
			if (pImg != NULL)
			{
				UIRectUV uv = m_uvSelectEffect[nGetKey]; 
				pImg->SetUV(uv);
			}
		}
		else
		{
			if (pImg != NULL)
			{
				COLOR col = 0xFFFFFFFF;
				
				if (nMenuType == eMENU_FRONT_COLOR)
				{
					col = CustomTitleData::m_vecFrontColor[nGetKey];
					SelectFrontColor();
				}
				else
				{
					col = CustomTitleData::m_vecBackColor[nGetKey];
					SelectBackColor();
				}

				pImg->SetColor(col);
			}
		}
	}	
}

void CCustomTitleUI::_updatePreview()
{
	SelectEffect();
	SelectFrontColor();
	SelectBackColor();
}

void CCustomTitleUI::_resetUI()
{
	TitleNetwork* pTitle = GAMEDATAMGR()->GetTitleNetwork();

	if (pTitle != NULL)
	{
		pTitle->ResetTitleSelectInfo();
	}

	if (m_pIconCashItem != NULL)
		m_pIconCashItem->clearIconData();

	if (m_pEditTitleName != NULL)
	{
		m_pEditTitleName->SetString(pTitle->getCustomTitleName());
		m_pEditTitleName->SetFocus(FALSE);
	}

	for (int i = 0; i < eMENU_MAX; ++i)
	{
		if (m_pArray[i] != NULL)
			m_pArray[i]->SetSelectIdx(0);

		m_nShowStartIdx[i] = 0;
	}

	_CheckMakeBtnEnable();
}

void CCustomTitleUI::initialize()
{
	int i;
	std::string strBtnId[eBTN_MAX] = { "btn_make", "btn_cancel", "btn_close"
	, "btn_color_left", "btn_color_right", "btn_Backcolor_left", "btn_Backcolor_right", "btn_Effect_left", "btn_Effect_right"};

	for (i = 0; i < eBTN_MAX; ++i)
		m_pbtn[i] = (CUIButton*)findUI(strBtnId[i].c_str());

	// 제작 버튼 커맨드 연결
	if (m_pbtn[eBTN_MAKE] != NULL)
	{
		CmdCustomPressMakeBtn* pCmd = new CmdCustomPressMakeBtn;
		pCmd->setData(this);
		m_pbtn[eBTN_MAKE]->SetCommandUp(pCmd);
	}
	
	// 취소 버튼 커맨드 연결
	if (m_pbtn[eBTN_CANCEL] != NULL)
	{
		CmdCustomTitleClose* pCmd = new CmdCustomTitleClose;
		pCmd->setData(this);
		m_pbtn[eBTN_CANCEL]->SetCommandUp(pCmd);
	}

	// X 버튼 커맨드 연결
	if (m_pbtn[eBTN_CLOSE] != NULL)
	{
		CmdCustomTitleClose* pCmd = new CmdCustomTitleClose;
		pCmd->setData(this);
		m_pbtn[eBTN_CLOSE]->SetCommandUp(pCmd);
	}

	// 호칭글자 컬러 바꾸기 메뉴(좌)
	if (m_pbtn[eBTN_FRONT_LEFT] != NULL)
	{
		CmdCustomTitleChangeMenu* pCmd = new CmdCustomTitleChangeMenu;
		pCmd->setData(this, eMENU_FRONT_COLOR, eChange_Left);
		m_pbtn[eBTN_FRONT_LEFT]->SetCommandUp(pCmd);
	}

	// 호칭글자 컬러 바꾸기 메뉴(우)
	if (m_pbtn[eBTN_FRONT_RIGHT] != NULL)
	{
		CmdCustomTitleChangeMenu* pCmd = new CmdCustomTitleChangeMenu;
		pCmd->setData(this, eMENU_FRONT_COLOR, eChange_Right);
		m_pbtn[eBTN_FRONT_RIGHT]->SetCommandUp(pCmd);
	}

	// 호칭배경 컬러 바꾸기 메뉴(좌)
	if (m_pbtn[eBTN_BACK_LEFT] != NULL)
	{
		CmdCustomTitleChangeMenu* pCmd = new CmdCustomTitleChangeMenu;
		pCmd->setData(this, eMENU_BACK_COLOR, eChange_Left);
		m_pbtn[eBTN_BACK_LEFT]->SetCommandUp(pCmd);
	}

	// 호칭배경 컬러 바꾸기 메뉴(우)
	if (m_pbtn[eBTN_BACK_RIGHT] != NULL)
	{
		CmdCustomTitleChangeMenu* pCmd = new CmdCustomTitleChangeMenu;
		pCmd->setData(this, eMENU_BACK_COLOR, eChange_Right);
		m_pbtn[eBTN_BACK_RIGHT]->SetCommandUp(pCmd);
	}

	// 이펙트 바꾸기 메뉴(좌)
	if (m_pbtn[eBTN_EFFECT_LEFT] != NULL)
	{
		CmdCustomTitleChangeMenu* pCmd = new CmdCustomTitleChangeMenu;
		pCmd->setData(this, eMENU_EFFECT, eChange_Left);
		m_pbtn[eBTN_EFFECT_LEFT]->SetCommandUp(pCmd);
	}

	// 이펙트 바꾸기 메뉴(우)
	if (m_pbtn[eBTN_EFFECT_RIGHT] != NULL)
	{
		CmdCustomTitleChangeMenu* pCmd = new CmdCustomTitleChangeMenu;
		pCmd->setData(this, eMENU_EFFECT, eChange_Right);
		m_pbtn[eBTN_EFFECT_RIGHT]->SetCommandUp(pCmd);
	}

	std::string strArrayId[eMENU_MAX] = { "array_color", "array_Backcolor", "array_Effect"};

	for (i = 0; i < eMENU_MAX; ++i)
	{
		m_pArray[i] = (CUIArray*)findUI(strArrayId[i].c_str());
	}

	// 호칭글색 클릭시
	if (m_pArray[eMENU_FRONT_COLOR] != NULL)
	{		
		CmdCustomTitleSelectFrontColor* pCmd = new CmdCustomTitleSelectFrontColor;
		pCmd->setData(this);
		m_pArray[eMENU_FRONT_COLOR]->SetCommandUp(pCmd);
	}

	// 호칭배경색 클릭시
	if (m_pArray[eMENU_BACK_COLOR] != NULL)
	{		
		CmdCustomTitleSelectBackColor* pCmd = new CmdCustomTitleSelectBackColor;
		pCmd->setData(this);
		m_pArray[eMENU_BACK_COLOR]->SetCommandUp(pCmd);
	}

	// 이펙트 클릭시
	if (m_pArray[eMENU_EFFECT] != NULL)
	{		
		CmdCustomTitleSelectEffect* pCmd = new CmdCustomTitleSelectEffect;
		pCmd->setData(this);
		m_pArray[eMENU_EFFECT]->SetCommandUp(pCmd);
	}
	
	m_pImgPreviewBack = (CUIImage*)findUI("img_preView");

	// 프리뷰 셋팅
	if (m_pImgPreviewBack != NULL)
	{
		m_rtPreview.Left = m_pImgPreviewBack->GetPosX();
		m_rtPreview.Top = m_pImgPreviewBack->GetPosY();
		m_rtPreview.Right = m_rtPreview.Left + m_pImgPreviewBack->GetWidth();		
		m_rtPreview.Bottom = m_rtPreview.Top + m_pImgPreviewBack->GetHeight();
	}

	m_pMoveArea = (CUIImage*)findUI("move_area");
	m_pIconCashItem = (CUIIcon*)findUI("icon_cashitem");
	m_pTbGuide = (CUITextBox*)findUI("tb_guide");
	m_pEditTitleName = (CUIEditBox*)findUI("edit_titleName");

	if (m_pTbGuide != NULL)
	{
		CTString strDesc = UIMGR()->GetString(7081) + "\\n" + UIMGR()->GetString(7082) + "\\n"
			 + UIMGR()->GetString(7083) + "\\n" + UIMGR()->GetString(7084);

		m_pTbGuide->SetText(strDesc);
	}

	// 이펙트 이미지 셋팅
	for (i = 0; i < DEF_EFFECT_MAX; ++i)
	{
		FLOAT nWidth = 26.f, nHeight = 27.f;
		FLOAT xGap = 3.f;
		FLOAT baseU0 = 618.f + (xGap * i) + (nWidth * i);
		FLOAT baseV0 = 92.f;

		m_uvSelectEffect[i].U0 = baseU0;
		m_uvSelectEffect[i].V0 = baseV0;
		m_uvSelectEffect[i].U1 = baseU0 + nWidth;
		m_uvSelectEffect[i].V1 = baseV0 + nHeight;
	}
}

void CCustomTitleUI::OnRender( CDrawPort* pDraw )
{
	// Check distance
	float	fDiffX = _pNetwork->MyCharacterInfo.x - m_fNpcX;
	float	fDiffZ = _pNetwork->MyCharacterInfo.z - m_fNpcZ;

	if( fDiffX * fDiffX + fDiffZ * fDiffZ > UI_VALID_SQRDIST )
		closeUI();

	if (m_pImgPreviewBack != NULL)
	{
		m_pImgPreviewBack->Hide(FALSE);
		m_pImgPreviewBack->SetColor(0x993300FF);
		m_pImgPreviewBack->Render(pDraw);
		m_pImgPreviewBack->Hide(TRUE);
	}

	if (m_ModelRenderTarget != NULL)
	{
		pDraw->Unlock();
		m_ModelRenderTarget->Begin();
		m_ModelRenderTarget->Clear(0x00000000);
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->HUD_DrawModel(pDraw, 0, 0, _PreviewX, _PreviewY, HUD_CHARACTER_TYPE, -3.0f, 1.8f);
		m_ModelRenderTarget->End();
		pDraw->Lock();

		FLOAT fU = _PreviewX / (FLOAT)_RTWidth;

		pDraw->InitTextureData(&m_ModelRenderTarget->rt_tdTexture, FALSE, PBT_BLEND);
		pDraw->AddTexture(m_nPosX + m_rtPreview.Left, 
			m_nPosY + m_rtPreview.Top,
			m_nPosX + m_rtPreview.Right,
			m_nPosY + m_rtPreview.Bottom,
			0.0f, 0.0f, fU, 1.0f, 0xFFFFFFFF);
		pDraw->FlushRenderingQueue();
	}
}

WMSG_RESULT CCustomTitleUI::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_nMouseX = x;
	m_nMouseY = y;

	if( m_bDrag && ( pMsg->wParam & MK_LBUTTON ) )
	{
		int ndX = x - m_nOriX;
		int ndY = y - m_nOriY;

		m_nOriX = x;
		m_nOriY = y;

		Move( ndX, ndY );
		return WMSG_SUCCESS;
	}

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	CUIManager::getSingleton()->SetMouseCursorInsideUIs();

	return WMSG_FAIL;
}

WMSG_RESULT CCustomTitleUI::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if( m_pMoveArea && m_pMoveArea->IsInside(x, y))
	{
		m_bDrag = true;
		m_nOriX = x;
		m_nOriY = y;
	}

	CUIManager::getSingleton()->RearrangeOrder( UI_CUSTOM_TITLE, TRUE );

	return WMSG_FAIL;
}

WMSG_RESULT CCustomTitleUI::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_bDrag = false;

	CUIManager* pUIManager = CUIManager::getSingleton();

	if ( m_pIconCashItem != NULL && m_pIconCashItem->IsInside(x, y))
	{
		CUIIcon* pDrag = pUIManager->GetDragIcon();

		if (pDrag != NULL)
		{
			if (pDrag->IsEmpty() != TRUE)
			{
				CItems* pItem = pDrag->getItems();

				if(pItem->Item_Index == DEF_CUSTOMTITLE_MAKE_ITEM)
					m_pIconCashItem->copyItem(pDrag);

				_CheckMakeBtnEnable();
			}			
		}
	}

	pUIManager->ResetHoldBtn();
	return WMSG_FAIL;
}

bool CCustomTitleUI::IsFilteringPass()
{
	if (m_pEditTitleName == NULL)
		return false;

	CTString szbuf = m_pEditTitleName->GetString();

	char szBuffer[32];
	strcpy ( szBuffer, szbuf );

	if( strlen( szbuf ) <= 0 )
	{
		return false;
	}
	else if( _UIFilteringCharacter.Filtering ( szBuffer ) == TRUE )
	{
		_resetTitleName();
		return false;
	}

	return true;
}

bool CCustomTitleUI::IsSameUniqueTitleSring()
{
	if (m_pEditTitleName == NULL)
		return false;

	CTString szbuf = m_pEditTitleName->GetString();

	int nSize = TitleStaticData::getsize();

	for (int i = 0; i < nSize; ++i)
	{
		int nItemIndex = TitleStaticData::getDataSeq(i)->GetItemIndex();

		if (szbuf == CItemData::getData(nItemIndex)->GetName())
		{
			_resetTitleName();
			return true;
		}
	}
	return false;
}

MakeError CCustomTitleUI::_CheckMakeError()
{
	if (m_pEditTitleName == NULL || m_pEditTitleName->GetString() == NULL)
		return eMAKE_ERROR_NO_STRING;

	if (IsFilteringPass() == false)
		return eMAKE_ERROR_FILTERING_NOPASS;

	if (IsSameUniqueTitleSring() == true)
		return eMAKE_ERROR_SAME_UNIQUE_TITLE;

	if (m_pIconCashItem == NULL || m_pIconCashItem->IsEmpty() == TRUE)
		return eMAKE_ERROR_NO_HAVE_CASHITEM;

	if (m_pIconCashItem->getItems()->Item_Index != DEF_CUSTOMTITLE_MAKE_ITEM)
		return eMAKE_ERROR_NOT_TITLE_MAKE_ITEM;

	for (int i = 0; i < eMENU_MAX; ++i)
	{
		if (m_pArray[i] == NULL || m_pArray[i]->GetSelectIdx() < 0)
			return MakeError(eMAKE_ERROR_NOT_SELECT_FRONT_COLOR + i);
	}

	CUIInventory* pInven = UIMGR()->GetInventory();

	if (pInven != NULL)
	{
		if (pInven->IsInventoryFull() == TRUE)
			return eMAKE_ERROR_FULL_INVEN;
	}

	return eMAKE_ERROR_SUCCESS;
}

void CCustomTitleUI::ShowErrorMessage( int nErrorCode )
{
	CTString strSysMessage;

	switch(nErrorCode)
	{
	case eMAKE_ERROR_SAME_UNIQUE_TITLE:
		strSysMessage = _S(7089, "고유 호칭은 제작 할 수 없습니다. 다른 호칭으로 변경해 주세요.");
		break;
	case eMAKE_ERROR_FILTERING_NOPASS:
		strSysMessage = _S(7094, "부적절한 호칭은 제작할 수 없습니다.");
		break;
	case eMAKE_ERROR_FULL_INVEN:
		strSysMessage = _S(7090, "인벤토리 공간이 부족하여 호칭을 제작 할 수 없습니다.");
		break;
	default:
		return;
	}
	_pNetwork->ClientSystemMessage(strSysMessage, SYSMSG_ERROR);
}

int CCustomTitleUI::_getSelectIndex( int selType )
{
	if (selType < 0 || selType >= eMENU_MAX)
		return -1;

	if (m_pArray[selType] != NULL)
	{
		int SelIndex = m_pArray[selType]->GetSelectIdx();

		if (SelIndex != -1)
			return SelIndex + m_nShowStartIdx[selType];
	}
	return -1;
}

char* CCustomTitleUI::_getTitleName()
{
	if (m_pEditTitleName == NULL)
		return NULL;

	return m_pEditTitleName->GetString();
}

BOOL CCustomTitleUI::IsEditBoxFocused()
{
	if (m_pEditTitleName == NULL)
		return FALSE;

	return m_pEditTitleName->IsFocused();
}

void CCustomTitleUI::KillFocusEditBox()
{
	if (m_pEditTitleName == NULL)
		return;

	_InputTitleName();
	m_pEditTitleName->SetFocus( FALSE );
}

WMSG_RESULT CCustomTitleUI::OnKeyMessage( MSG* pMsg )
{
	if ( pMsg->wParam == VK_RETURN )
	{
		if (m_pEditTitleName != NULL)
		{
			if(_InputTitleName() == true)
			{
				m_pEditTitleName->SetFocus( FALSE );
				return WMSG_SUCCESS;
			}
		}
	}
	return WMSG_FAIL;
}

bool CCustomTitleUI::_InputTitleName()
{
	if (m_pEditTitleName == NULL)
		return false;

	if (m_pEditTitleName->IsFocused() == FALSE)
		return false;

	TitleNetwork* pTitle = GAMEDATAMGR()->GetTitleNetwork();

	if (pTitle == NULL)
		return false;
	
	pTitle->setCustomTitleName(_getTitleName());
	_CheckMakeBtnEnable();
	return true;
}

void CCustomTitleUI::_resetTitleName()
{
	if (m_pEditTitleName != NULL)
		m_pEditTitleName->ResetString();
}

void CCustomTitleUI::_CheckMakeBtnEnable()
{
	if (m_pbtn[eBTN_MAKE] != NULL)
	{
		if (IsReadyMakeTitle() == true)
			m_pbtn[eBTN_MAKE]->SetEnable(TRUE);
		else
			m_pbtn[eBTN_MAKE]->SetEnable(FALSE);
	}
}

bool CCustomTitleUI::IsReadyMakeTitle()
{
	if (m_pEditTitleName == NULL || m_pEditTitleName->GetString() == NULL)
		return false;

	for (int i = 0; i < eMENU_MAX; ++i)
	{
		if (_getSelectIndex(i) < 0)
			return false;
	}

	if (m_pIconCashItem == NULL || m_pIconCashItem->IsEmpty() == TRUE)
		return false;

	return true;
}

void CCustomTitleUI::PressMakeBtn()
{
	_CheckMsgBox();

	CmdCustomTitleMake* pCmdOk = new CmdCustomTitleMake;
	pCmdOk->setData(this);

	CmdCustomTitleMakeCancel* pCmdCancel = new CmdCustomTitleMakeCancel;
	pCmdCancel->setData(this);

	m_nMsgID = MSGBOXMGR()->CreateMsgBox(_S(7088, "호칭 제작"),
		_S(7091, "설정 하신 내용으로 호칭을 제작 합니다. 확인 클릭 시 인벤토리 호칭이 생성 됩니다."),
		eMSG_YESNO, true, pCmdOk, pCmdCancel);

	if (m_pbtn[eBTN_MAKE] != NULL)
		m_pbtn[eBTN_MAKE]->SetEnable(FALSE);
}

void CCustomTitleUI::_CheckMsgBox()
{
	if (m_nMsgID > 0)
		MSGBOXMGR()->Close(m_nMsgID);

	m_nMsgID = 0;
}

void CCustomTitleUI::OnUpdate( float fDeltaTime, ULONG ElapsedTime )
{
	if (m_bHide)
		return;
	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->InteractionMsgBoxReject();
}

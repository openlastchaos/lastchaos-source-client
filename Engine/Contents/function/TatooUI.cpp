#include "stdh.h"
#include "TatooUI.h"

#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Info/MyInfo.h>

#define DEF_MAX_COLOR	9
#define DEF_MAX_PATTERN 5

// 서버에 보내주는 선택에대한 extra 값
const int ct_iColorShapeIndex[DEF_MAX_COLOR][DEF_MAX_PATTERN] = { 
	  {26, 27, 28, 29, 30} 
	, {31, 32, 33, 34, 35}
	, {13, 22, 23, 24, 25} 
	, {36, 37, 38, 39, 40}
	, {41, 42, 43, 44, 45} 
	, {46, 47, 48, 49, 50}	
	, {51, 52, 53, 54, 55} 
	, {11, 14, 15, 16, 17}
	, {12, 18, 19, 20, 21} 
};

// 색상 코드
const COLOR SelectColorList[DEF_MAX_COLOR] = {
	  0xFF0000FF // 빨강
	, 0xFFA500FF // 주황
	, 0xFFFF00FF // 노랑
	, 0x00FF00FF // 초록
	, 0x0000FFFF // 파랑
	, 0x000080FF // 남색
	, 0x800080FF // 보라
	, 0x000000FF // 검정
	, 0xFFFFFFFF // 하양
};

CTatooUI::CTatooUI()
	: m_pMoveArea(NULL)
{
	int i;
	for (i = 0; i < eBTN_MAX; ++i)
		m_pBtn[i] = NULL;

	for (i = 0; i < eIMG_MAX; ++i)
		m_pImgTemp[i] = NULL;

	for (i = 0; i < eARRAY_MAX; ++i)
		m_pArray[i] = NULL;

	setInherit(false);
}

void CTatooUI::OpenTatoo()
{
	if (GetHide() == FALSE)
		return;

	Hide(FALSE);
	SetVisible(TRUE);

	CheckShowShape();
	CUIManager* pUIMgr = UIMGR();
	pUIMgr->RearrangeOrder( UI_TATOO, TRUE );
}

void CTatooUI::Close()
{
	Hide(TRUE);
	SetVisible(FALSE);

	CUIManager* pUIMgr = UIMGR();
	pUIMgr->RearrangeOrder( UI_TATOO, FALSE );
}

void CTatooUI::initialize()
{
#ifndef		WORLD_EDITOR

	int i;
	// drag
	m_pMoveArea = (CUIImage*)findUI("move_area");

	if (m_pMoveArea != NULL)
	{
		int l, t;
		l = m_pMoveArea->GetPosX();
		t = m_pMoveArea->GetPosY();
		setTitleRect(l, t, l + m_pMoveArea->GetWidth(), t + m_pMoveArea->GetHeight());
	}

	// Button
	std::string strBtnID[eBTN_MAX] = { "btn_close", "btn_ok", "btn_cancel" };

	for (i = 0; i < eBTN_MAX; ++i)
		m_pBtn[i] = (CUIButton*)findUI(strBtnID[i].c_str());

	if (m_pBtn[eBTN_CLOSE] != NULL)
		m_pBtn[eBTN_CLOSE]->SetCommandFUp(boost::bind(&CTatooUI::Close, this));

	if (m_pBtn[eBTN_CANCEL] != NULL)
		m_pBtn[eBTN_CANCEL]->SetCommandFUp(boost::bind(&CTatooUI::Close, this));

	if (m_pBtn[eBTN_OK] != NULL)
		m_pBtn[eBTN_OK]->SetCommandFUp(boost::bind(&CTatooUI::MakeMessageBox, this));

	// Array
	std::string strArrayID[eARRAY_MAX] = { "array_color", "array_pattern" };

	for (i = 0; i < eARRAY_MAX; ++i)
		m_pArray[i] = (CUIArray*)findUI(strArrayID[i].c_str());

	// Image
	std::string strImgID[eIMG_MAX] = { "img_heart", "img_clover", "img_dia",
		"img_spade", "img_skull", "img_colorTemp" };

	for (i = 0; i < eIMG_MAX; ++i)
		m_pImgTemp[i] = (CUIImage*)findUI(strImgID[i].c_str());

	_setUI();

#endif		// WORLD_EDITOR
}


WMSG_RESULT CTatooUI::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist(MSGCMD_PET_TATOOCHANGE_USE) )
		return WMSG_SUCCESS;

	pUIManager->RearrangeOrder( UI_TATOO, TRUE );

	return CUIWindow::OnLButtonDown(x, y);
}

WMSG_RESULT CTatooUI::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->ResetHoldBtn();
	if( pUIManager->DoesMessageBoxExist(MSGCMD_PET_TATOOCHANGE_USE) )
		return WMSG_SUCCESS;

	return CUIWindow::OnLButtonUp(x, y);
}

void CTatooUI::SendItemUse()
{
	int nColorAndPatternIndex = GetColorAndShapeIndex();

	if (nColorAndPatternIndex <= 0)
	{
		Close();
		return;
	}

	int tv_tab, tv_idx;
	CUIManager::getSingleton()->GetInventory()->GetUseItemSlotInfo(tv_tab,tv_idx);
	CItems& Items = _pNetwork->MySlotItem[tv_tab][tv_idx];
	int iItemIndex = Items.Item_UniIndex;
	
	_pNetwork->SendItemUse(tv_tab, tv_idx, iItemIndex, nColorAndPatternIndex);
}

void CTatooUI::CheckShowShape()
{
	BOOL bHide = TRUE;

	ObjInfo* pInfo = ObjInfo::getSingleton();
	CPetTargetInfom* pPetInfo = pInfo->GetMyPetInfo();

	int iAge = pPetInfo->iAge;

	// [090831: selo] 타는 것 아니면 문양 선택 못하게 변경
	if( iAge < 2 )		// 타는거 아닐 때
		bHide = TRUE;
	else if( iAge >= 2 ) // 타는거 일 때
		bHide = FALSE;

	if(m_pArray[eARRAY_PATTERN] != NULL)
	{
		m_pArray[eARRAY_PATTERN]->Hide(bHide);
		m_pArray[eARRAY_PATTERN]->SetSelectIdx(-1);
	}
}

int CTatooUI::GetColorAndShapeIndex()
{
	int nErrorIndex = 0;

	if (m_pArray[eARRAY_COLOR] == NULL)
		return nErrorIndex;

	int nSelColorIndex = _getSelectIndex(eARRAY_COLOR);
	int nOnlyColorIndex = nSelColorIndex + 1;

	if (nSelColorIndex < 0 || nSelColorIndex >= DEF_MAX_COLOR)
		return nErrorIndex;

	if (m_pArray[eARRAY_PATTERN] == NULL)
		return nOnlyColorIndex;

	int nSelPatternIdx = _getSelectIndex(eARRAY_PATTERN);

	if (nSelPatternIdx < 0 || nSelPatternIdx >= DEF_MAX_PATTERN)
		return nOnlyColorIndex;

	return ct_iColorShapeIndex[nSelColorIndex][nSelPatternIdx];
}

void CTatooUI::MakeMessageBox()
{
	CTString strColor ="";
	CTString strShape =_S(4448, "민");

	int nSelColorIndex = _getSelectIndex(eARRAY_COLOR);
	int nSelPatternIdx = _getSelectIndex(eARRAY_PATTERN);

	switch(nSelColorIndex)
	{
	case 0:
		strColor = _S(4434, "빨강");
		break;
	case 1:
		strColor = _S(4435, "주황");
		break;
	case 2:
		strColor = _S(4436, "노랑");
		break;
	case 3:
		strColor = _S(4437, "초록");
		break;
	case 4:
		strColor = _S(4438, "파랑");
		break;
	case 5:
		strColor = _S(4439, "남색");
		break;
	case 6:
		strColor = _S(4440, "보라");
		break;
	case 7:
		strColor = _S(4441, "검정");
		break;
	case 8:
		strColor = _S(4442, "하양");
		break;
	}
	switch(nSelPatternIdx)
	{
	case 0:
		strShape = _S(4443, "하트");		
		break;
	case 1:
		strShape = _S(4445, "클로버");				
		break;
	case 2:		
		strShape = _S(4444, "다이아몬드");		
		break;
	case 3:
		strShape = _S(4446, "스페이드");		
		break;
	case 4:		
		strShape = _S(4447, "해골");
		break;
	}
	
	CUIMsgBox_Info	MsgBoxInfo;
	CTString		strMessage;
	strMessage.PrintF(_S(4433, "%s색상 %s문양을 선택하셨습니다. 이대로 진행하시겠습니까?"), strColor, strShape);
	MsgBoxInfo.SetMsgBoxInfo(_S(4430, "문양 변경"), UMBS_YESNO, UI_NONE, MSGCMD_PET_TATOOCHANGE_USE);
	MsgBoxInfo.AddString(strMessage);
	CUIManager::getSingleton()->CreateMessageBox(MsgBoxInfo);
}

int CTatooUI::_getSelectIndex( int nArrayType )
{
	if (nArrayType < 0 || nArrayType >= eARRAY_MAX)
		return -1;

	if (m_pArray[nArrayType] == NULL)
		return -1;

	return m_pArray[nArrayType]->GetSelectIdx();
}

void CTatooUI::_setUI()
{
	CUIArrayItem* pItem = NULL;
	CUIImage* pImg = NULL;
	
	int nCount = 0;
	int i;

	if (m_pArray[eARRAY_COLOR] != NULL)
	{
		nCount = m_pArray[eARRAY_COLOR]->GetArrayChildCount();

		if (nCount >= DEF_MAX_COLOR)
		{
			for (i = 0; i < DEF_MAX_COLOR; ++i)
			{
				pItem = m_pArray[eARRAY_COLOR]->GetArrayItem(i);

				if (pItem == NULL)
					continue;

				pItem->deleteChildAll();

				if (m_pImgTemp[eIMG_COLORTEMP] == NULL)
					continue;

				pImg = (CUIImage*)m_pImgTemp[eIMG_COLORTEMP]->Clone();

				if (pImg != NULL)
				{
					pImg->Hide(FALSE);
					pImg->SetColor(SelectColorList[i]);
#ifndef WORLD_EDITOR
					pImg->SetCommandFUp(boost::bind(&CTatooUI::CheckShowShape, this));
#endif	// WORLD_EDITOR	
					pItem->addChild((CUIBase*)pImg);
				}
			}

			m_pArray[eARRAY_COLOR]->UpdateItem();
		}		
	}

	if (m_pArray[eARRAY_PATTERN] != NULL)
	{
		nCount = m_pArray[eARRAY_PATTERN]->GetArrayChildCount();
		
		if (nCount >= DEF_MAX_PATTERN)
		{
			for (i = 0; i < DEF_MAX_PATTERN; ++i)
			{
				pItem = m_pArray[eARRAY_PATTERN]->GetArrayItem(i);

				if (pItem == NULL)
					continue;

				pItem->deleteChildAll();

				if (m_pImgTemp[i] == NULL)
					continue;

				pImg = (CUIImage*)m_pImgTemp[i]->Clone();

				if (pImg != NULL)
				{
					pImg->Hide(FALSE);
					pItem->addChild((CUIBase*)pImg);
				}
			}

			m_pArray[eARRAY_PATTERN]->UpdateItem();
		}		
	}
}

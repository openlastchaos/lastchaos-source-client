#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include "PetTargetUI.h"
#include <Engine/Interface/UIPetInfo.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Contents/function/WildPetInfoUI.h>
#include <Engine/Contents/Base/UIQuestBookNew.h>
#include <Engine/Info/MyInfo.h>

#define	DEF_PET_TARGET_BAR_WIDTH			120

CPetTargetUI::CPetTargetUI()
	: m_pBtnPetInfo(NULL)
	, m_bDrag(false)
	, m_nOriX(0), m_nOriY(0)
	, m_nMouseX(0)
	, m_nMouseY(0)
{
	int i;
	for (i = 0; i < eBACK_MAX; ++ i)
		m_pImgSplit[i] = NULL;

	for (i = 0; i < eTEXT_MAX; ++ i)
		m_pText[i] = NULL;

	for (i = 0; i < eIMG_MAX; ++ i)
		m_pImg[i] = NULL;
}

void CPetTargetUI::TogglePetInfo()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CUIPetInfo* PetInfoUI = pUIManager->GetPetInfo();

	if (PetInfoUI == NULL)
		return;

	if( PetInfoUI->IsVisible() )
		PetInfoUI->ClosePetInfo();								
	else
		PetInfoUI->OpenPetInfo();
}

void CPetTargetUI::UpdateHPInfo()
{
	ObjInfo* pInfo = ObjInfo::getSingleton();
	CPetTargetInfom* pPetInfo = pInfo->GetMyPetInfo();

	if (pPetInfo == NULL)
		return;

	FLOAT	fHPRatio = Clamp( pPetInfo->fHealth / pPetInfo->fMaxHealth, 0.0f, 1.0f );

	if (m_pImg[eIMG_HP] != NULL)
		m_pImg[eIMG_HP]->SetWidth(DEF_PET_TARGET_BAR_WIDTH * fHPRatio);
}

void CPetTargetUI::UpdateHungryInfo()
{
	ObjInfo* pInfo = ObjInfo::getSingleton();

	CPetTargetInfom* pPetInfo = pInfo->GetMyPetInfo();

	if (pPetInfo == NULL)
		return;

	FLOAT	fHungryRatio = Clamp( pPetInfo->fHungry / pPetInfo->fMaxHungry, 0.0f, 1.0f );

	if (m_pImg[eIMG_HUNGRY] != NULL)
		m_pImg[eIMG_HUNGRY]->SetWidth(DEF_PET_TARGET_BAR_WIDTH * fHungryRatio);
}

void CPetTargetUI::initialize()
{
#ifndef		WORLD_EDITOR

	// pet Info
	m_pBtnPetInfo = (CUIButton*)findUI("btn_petinfo");

	if (m_pBtnPetInfo != NULL)
		m_pBtnPetInfo->SetCommandFUp(boost::bind(&CPetTargetUI::TogglePetInfo, this));

	int i;
	// Back [ title, back ]
	std::string strBackID[eBACK_MAX] = { "is_title", "is_back" };

	for (i = 0; i < eBACK_MAX; ++ i)
	{
		m_pImgSplit[i] = (CUIImageSplit*)findUI(strBackID[i].c_str());

		if (m_pImgSplit[i] != NULL)
			m_pImgSplit[i]->setInherit(false);
	}

	// Text [ level, name ]
	std::string strTextID[eTEXT_MAX] = { "str_level", "str_name" };

	for (i = 0; i < eTEXT_MAX; ++ i)
		m_pText[i] = (CUIText*)findUI(strTextID[i].c_str());

	// Img [ HP, Hungry ]
	std::string strImgID[eIMG_MAX] = { "img_hp", "img_hungry" };

	for (i = 0; i < eIMG_MAX; ++ i)
		m_pImg[i] = (CUIImage*)findUI(strImgID[i].c_str());

#endif		// WORLD_EDITOR
}

void CPetTargetUI::openUI()
{
	Hide(FALSE);
	SetVisible(TRUE);
	updateUI();

	CUIManager* pUIMgr = UIMGR();
	pUIMgr->RearrangeOrder( UI_PET_TARGETINFO, TRUE );
}

void CPetTargetUI::closeUI()
{
	Hide(TRUE);
	SetVisible(FALSE);

	CUIManager* pUIMgr = UIMGR();
	pUIMgr->RearrangeOrder( UI_PET_TARGETINFO, FALSE );
}

void CPetTargetUI::updateUI()
{
	ObjInfo* pInfo = ObjInfo::getSingleton();
	CPetTargetInfom* pPetInfo = pInfo->GetMyPetInfo();

	if (pPetInfo == NULL)
		return;

	if( pPetInfo->bIsActive == FALSE)
		return;

	CTString strTemp;
	// Pet name
	if( pPetInfo->strNameCard.Length() > 0 )
		strTemp = pPetInfo->strNameCard;
	else
		strTemp = PetInfo().GetName(pPetInfo->iType, pPetInfo->iAge);

	if (m_pText[eTEXT_NAME] != NULL)
		m_pText[eTEXT_NAME]->SetText(strTemp);

	// Pet Level
	strTemp.PrintF("%d", pPetInfo->iLevel);

	if (m_pText[eTEXT_LEVEL] != NULL)
		m_pText[eTEXT_LEVEL]->SetText(strTemp);

	// HP bar Update
	UpdateHPInfo();

	// Hungry bar Update
	UpdateHungryInfo();
}

void CPetTargetUI::OnUpdate( float fDeltaTime, ULONG ElapsedTime )
{
	_updatePetInfoBtntooltip();
}

WMSG_RESULT CPetTargetUI::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
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

WMSG_RESULT CPetTargetUI::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if( m_pImgSplit[eBACK_TITLE] && m_pImgSplit[eBACK_TITLE]->IsInside(x, y))
	{
		m_bDrag = true;
		m_nOriX = x;
		m_nOriY = y;
	}
	else if (m_pImgSplit[eBACK_TAIL] && m_pImgSplit[eBACK_TAIL]->IsInside(x, y))
	{
		ObjInfo* pInfo = ObjInfo::getSingleton();
		CPetTargetInfom* pPetInfo = pInfo->GetMyPetInfo();

		if( pPetInfo && pPetInfo->pen_pEntity )
		{
			CEntity			*penPlEntity;
			CPlayerEntity	*penPlayerEntity;
			penPlEntity = CEntity::GetPlayerEntity( 0 );
			penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);
			penPlayerEntity->SetTarget( pPetInfo->pen_pEntity );
		}
	}

	CUIManager::getSingleton()->RearrangeOrder( UI_PET_TARGETINFO, TRUE );

	return WMSG_FAIL;
}

WMSG_RESULT CPetTargetUI::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_bDrag = false;

	CUIManager::getSingleton()->ResetHoldBtn();
	return WMSG_FAIL;
}

void CPetTargetUI::_updatePetInfoBtntooltip()
{
	if (GetHide() == TRUE)
		return;

	if (m_pBtnPetInfo != NULL)
	{
		extern INDEX g_iEnterChat;

		CTString strTooltip = _S(2173,"펫 정보창") ;

		if( g_iEnterChat )
			strTooltip += CTString("(P,ALT+P)");
		else 
			strTooltip += CTString("(ALT+P)");

		m_pBtnPetInfo->setTooltip(strTooltip);
	}
}

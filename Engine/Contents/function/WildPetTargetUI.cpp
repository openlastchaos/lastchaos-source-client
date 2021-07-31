#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <Engine/Interface/UIManager.h>
#include "WildPetTargetUI.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UIPlayerInfo.h>
#include <Engine/Contents/function/WildPetInfoUI.h>
#include <Engine/Contents/Base/UIQuestBookNew.h>
#include <Engine/Info/MyInfo.h>

#define INFOMARK_POS_X 108
#define INFOMARK_POS_Y 3

#define INFOMARK_SIZE 16

// WILDPET_UI_EX 공격형 펫 UI 개선 [11/28/2011 ldy1978220]
#define PETTARGETINFO_WIDTH_EX			255
#define	PETTARGETINFO_HEIGHT_EX			108

#define	PETTARGETINFO_BAR_WIDTH			120
#define	PETTARGETINFO_BAR_WIDTH_EX		208

#define DEF_UI_WIDTH				163
#define DEF_UI_WIDTH_EX				256
#define DEF_UI_ADD_WIDTH			(DEF_UI_WIDTH_EX - DEF_UI_WIDTH)
#define DEF_UI_HEIGHT				77
#define DEF_UI_HEIGHT_EX			104
#define DEF_UI_MOVE_AREA_HEIGHT		29
#define DEF_UI_PBAR_WIDTH			113
#define DEF_UI_PBAR_WIDTH_EX		(DEF_UI_PBAR_WIDTH + DEF_UI_ADD_WIDTH)
#define DEF_UI_PBAR_TEXT_WIDTH		122
#define DEF_UI_PBAR_TEXT_WIDTH_EX	(DEF_UI_PBAR_TEXT_WIDTH + DEF_UI_ADD_WIDTH)
#define DEF_UI_PET_NAME_WIDTH		95
#define DEF_UI_PET_NAME_WIDTH_EX	(DEF_UI_PET_NAME_WIDTH + DEF_UI_ADD_WIDTH)
#define DEF_UI_ACC_EXP_BAR_GAP		6

enum eSTATE_IMAGE_INDEX
{
	eIMG_IDX_HIGH = 0, // 최상
	eIMG_IDX_NORMAL,	 // 보통
	eIMG_IDX_LOW		 // 최하
};

#define DEF_HIGH_CUTLINE	60
#define DEF_NORMAL_CUTLINE	20

CWildPetTargetUI::CWildPetTargetUI()
	: m_pIsExpPetBack(NULL)
	, m_pImgAccExp(NULL)
	, m_pTextExpPetUseExp(NULL)
	, m_pIsExtendBack(NULL)
	, m_pIaAIState(NULL)
	, m_pIsBaseBack(NULL)
	, m_pMoveArea(NULL)
	, m_bDrag(false)
	, m_nOriX(0), m_nOriY(0)
	, m_nMouseX(0)
	, m_nMouseY(0)
	, m_bExtendUI(false)
{	
	int i;
	for (i = 0; i < eIMG_ARR_MAX; ++i)
	{
		m_pImgArray[i] = NULL;
		m_pTextArr[i] = NULL;
	}

	for (i = 0; i < eBTN_MAX; ++i)
		m_pBtn[i] = NULL;

	for (i = 0; i < eTEXT_MAX; ++i)
		m_pTextBase[i] = NULL;

	for (i = 0; i < ePROGRESS_MAX; ++i)
		m_pbBarBase[i] = NULL;
}

CWildPetTargetUI::~CWildPetTargetUI()
{
	Destroy();
}

void CWildPetTargetUI::ExtendBtnPress()
{
	m_bExtendUI = !m_bExtendUI;

	_setUI();
	updateUI();
}

void CWildPetTargetUI::ToggleWildPetInfo()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CWildPetInfoUI* pWildInfo = pUIManager->GetWildPetInfoUI();

	if (pWildInfo == NULL)
		return;

	if( pWildInfo->IsVisible() )
		pWildInfo->closeUI();							
	else
		pWildInfo->openUI();
}

void CWildPetTargetUI::openUI()
{
	Hide(FALSE);
	SetVisible(TRUE);

	_setUI();
	updateUI();

	CUIManager* pUIMgr = UIMGR();
	pUIMgr->RearrangeOrder( UI_WILDPET_TARGETINFO, TRUE );
}

void CWildPetTargetUI::closeUI()
{
	Hide(TRUE);
	SetVisible(FALSE);

	CUIManager* pUIMgr = UIMGR();
	pUIMgr->RearrangeOrder( UI_WILDPET_TARGETINFO, FALSE );
}

void CWildPetTargetUI::updateUI()
{
	updateBase();
	updateExpPet();
	updateExtend();
}

void CWildPetTargetUI::updateBase()
{
	ObjInfo* pInfo = ObjInfo::getSingleton();
	CWildPetTarget* pMyApet = pInfo->GetMyApetInfo();

	if (pMyApet == NULL)
		return;

	CTString tempString;

	// pet 이름
	if (m_pTextBase[eTEXT_NAME] != NULL)
	{
		tempString = pMyApet->m_strName.c_str();

		if (m_bExtendUI)
			tempString = CUIQuestBook::MakeTitleString(tempString, 30);
		else
			tempString = CUIQuestBook::MakeTitleString(tempString, 14);

		m_pTextBase[eTEXT_NAME]->SetText(tempString);
	}

	// pet 레벨
	if (m_pTextBase[eTEXT_LEVEL] != NULL)
	{
		tempString.PrintF("%d", pMyApet->m_nLevel);
		m_pTextBase[eTEXT_LEVEL]->SetText(tempString);
	}

	// pet HP
	if (m_pTextBase[eTEXT_HP] != NULL)
	{
		tempString.PrintF("%d/%d", pMyApet->m_nHP, pMyApet->m_nMaxHP);
		m_pTextBase[eTEXT_HP]->SetText(tempString);
	}

	if (m_pbBarBase[ePROGRESS_HP] != NULL)
		m_pbBarBase[ePROGRESS_HP]->SetProgressPos(calcHPBarInfo());

	// pet MP
	if (m_pTextBase[eTEXT_MP] != NULL)
	{
		tempString.PrintF("%d/%d", pMyApet->m_nMP, pMyApet->m_nMaxMP);
		m_pTextBase[eTEXT_MP]->SetText(tempString);
	}

	if (m_pbBarBase[ePROGRESS_MP] != NULL)
		m_pbBarBase[ePROGRESS_MP]->SetProgressPos(calcMPBarInfo());

	// pet EXP
	if (m_pTextBase[eTEXT_EXP] != NULL)
	{
		tempString.PrintF("%.2f%%", ((FLOAT)pMyApet->m_exp / (FLOAT)pMyApet->m_next_exp) * 100.0f);
		m_pTextBase[eTEXT_EXP]->SetText(tempString);
	}

	if (m_pbBarBase[ePROGRESS_EXP] != NULL)
		m_pbBarBase[ePROGRESS_EXP]->SetProgressPos(calcEXPBarInfo());
}

void CWildPetTargetUI::updateExpPet()
{
	ObjInfo* pInfo = ObjInfo::getSingleton();
	CWildPetData	*pPetdata;
	pPetdata = CWildPetData::getData(pInfo->GetMyApetInfo()->m_nIndex);

	if (pPetdata == NULL)
		return;

	if (pPetdata->nFlag & WILDPET_FLAG_EXP)
	{
		CTString strTemp;
		FLOAT fTemp = calcAccuExpInfo();
		strTemp.PrintF("%.2f%%", fTemp * 100.0f);

		// acc exp text set
		if (m_pTextExpPetUseExp != NULL)
			m_pTextExpPetUseExp->SetText(strTemp);

		// acc exp bar width
		if (m_pImgAccExp != NULL)
		{
			int nResultWidth = calcAccuExpBarWidth();
			m_pImgAccExp->SetWidth(nResultWidth);
		}
	}			
}

void CWildPetTargetUI::updateExtend()
{
	if (m_bExtendUI == false)
		return;

	ObjInfo* pInfo = ObjInfo::getSingleton();
	CWildPetTarget* pMyWildPet = pInfo->GetMyApetInfo();

	if (pMyWildPet == NULL)
		return;

	CTString strTemp;
	// 충성도
	eSTATE_IMAGE_INDEX eIndex;

	FLOAT temResult = (FLOAT)pMyWildPet->m_nFaith / (FLOAT)pMyWildPet->m_nMaxFaith * 100.0f;

	if(temResult > DEF_HIGH_CUTLINE)
		eIndex = eIMG_IDX_HIGH;	
	else if(temResult > DEF_NORMAL_CUTLINE)
		eIndex = eIMG_IDX_NORMAL;
	else
		eIndex = eIMG_IDX_LOW;

	if (m_pImgArray[eIMG_ARR_FAITH] != NULL)
		m_pImgArray[eIMG_ARR_FAITH]->SetRenderIdx(eIndex);

	strTemp.PrintF("%d/%d", pMyWildPet->m_nFaith, pMyWildPet->m_nMaxFaith);

	if (m_pTextArr[eIMG_ARR_FAITH] != NULL)
		m_pTextArr[eIMG_ARR_FAITH]->SetText(strTemp);

	// 배고품
	temResult = (FLOAT)pMyWildPet->m_nStm / (FLOAT)pMyWildPet->m_nMaxStm * 100.0f;

	if(temResult > DEF_HIGH_CUTLINE)
		eIndex = eIMG_IDX_HIGH;	
	else if(temResult > DEF_NORMAL_CUTLINE)
		eIndex = eIMG_IDX_NORMAL;
	else
		eIndex = eIMG_IDX_LOW;

	if (m_pImgArray[eIMG_ARR_HUNGRY] != NULL)
		m_pImgArray[eIMG_ARR_HUNGRY]->SetRenderIdx(eIndex);

	strTemp.PrintF("%d/%d", pMyWildPet->m_nStm, pMyWildPet->m_nMaxStm);

	if (m_pTextArr[eIMG_ARR_HUNGRY] != NULL)
		m_pTextArr[eIMG_ARR_HUNGRY]->SetText(strTemp);

	// AI 상태
	if (m_pIaAIState != NULL)
	{
		if (CUIManager::getSingleton()->GetWildPetInfoUI()->GetAIActive())
			m_pIaAIState->SetRenderIdx(1); // 켜짐
		else
			m_pIaAIState->SetRenderIdx(0); // 꺼짐
	}
}

void CWildPetTargetUI::initialize()
{
#ifndef		WORLD_EDITOR

	// title 이동 영역 잡아주기
	m_pMoveArea = (CUIImage*)findUI("move_area");

	if (m_pMoveArea != NULL)
		m_pMoveArea->SetSize(DEF_UI_WIDTH, DEF_UI_MOVE_AREA_HEIGHT);

	int i;
	// exp Pet
	m_pIsExpPetBack = (CUIImageSplit*)findUI("ia_expPet");

	if (m_pIsExpPetBack != NULL)
		m_pIsExpPetBack->setInherit(false);

	m_pTextExpPetUseExp = (CUIText*)findUI("str_expPetExp");

	m_pImgAccExp = (CUIImage*)findUI("img_accExp");

	// extend UI
	m_pIsExtendBack = (CUIImageSplit*)findUI("is_extend_back");

	if (m_pIsExtendBack != NULL)
		m_pIsExtendBack->setInherit(false);

	std::string strImgArrID[eIMG_ARR_MAX] = { "ia_hungry", "ia_faith" };
	std::string strArrTextID[eIMG_ARR_MAX] = { "str_hungry", "str_faith" };

	for (i = 0; i < eIMG_ARR_MAX; ++i)
	{
		m_pImgArray[i] = (CUIImageArray*)findUI(strImgArrID[i].c_str());
		m_pTextArr[i] = (CUIText*)findUI(strArrTextID[i].c_str());
	}

	m_pIaAIState = (CUIImageArray*)findUI("ia_AIState");

	// base
	m_pIsBaseBack = (CUIImageSplit*)findUI("is_back");

	if (m_pIsBaseBack != NULL)
		m_pIsBaseBack->setInherit(false);

	std::string strBtnID[eBTN_MAX] = { "btn_extend_up", "btn_extend_down", "btn_openInfo" };

	for (i = 0; i < eBTN_MAX; ++i)
		m_pBtn[i] = (CUIButton*)findUI(strBtnID[i].c_str());

	if (m_pBtn[eBTN_EXTEND_DOWN] != NULL)
		m_pBtn[eBTN_EXTEND_DOWN]->SetCommandFUp(boost::bind(&CWildPetTargetUI::ExtendBtnPress, this));

	if (m_pBtn[eBTN_EXTEND_UP] != NULL)
		m_pBtn[eBTN_EXTEND_UP]->SetCommandFUp(boost::bind(&CWildPetTargetUI::ExtendBtnPress, this));

	if (m_pBtn[eBTN_OPENINFO] != NULL)
		m_pBtn[eBTN_OPENINFO]->SetCommandFUp(boost::bind(&CWildPetTargetUI::ToggleWildPetInfo, this));

	std::string strBaseTextID[eTEXT_MAX] = { "str_level", "str_name", "str_hp", "str_mp", "str_exp" };

	for (i = 0; i < eTEXT_MAX; ++i)
		m_pTextBase[i] = (CUIText*)findUI(strBaseTextID[i].c_str());

	std::string strProgressID[ePROGRESS_MAX] = { "pb_hp", "pb_mp", "pb_exp" };

	for (i = 0; i < ePROGRESS_MAX; ++i)
		m_pbBarBase[i] = (CUIProgressBar*)findUI(strProgressID[i].c_str());

#endif		// WORLD_EDITOR
}

void CWildPetTargetUI::_setUI()
{
	BOOL bShowBtnUP, bShowBtnDown;
	int nWidth = 0;
	int nBarWidth = 0;
	int nBarTextWidth = 0;
	int nPetNameWidth = 0;
	if (m_bExtendUI == false)
	{
		bShowBtnUP = TRUE;
		bShowBtnDown = FALSE;
		nWidth = DEF_UI_WIDTH;
		nBarWidth = DEF_UI_PBAR_WIDTH;
		nBarTextWidth = DEF_UI_PBAR_TEXT_WIDTH;
		nPetNameWidth = DEF_UI_PET_NAME_WIDTH;
	}
	else
	{
		bShowBtnUP = FALSE;
		bShowBtnDown = TRUE;
		nWidth = DEF_UI_WIDTH_EX;
		nBarWidth = DEF_UI_PBAR_WIDTH_EX;
		nBarTextWidth = DEF_UI_PBAR_TEXT_WIDTH_EX;
		nPetNameWidth = DEF_UI_PET_NAME_WIDTH_EX;
	}

	if (m_pIsBaseBack != NULL)
	{
		m_pIsBaseBack->SetWidth(nWidth);
		m_pIsBaseBack->UpdateSplit();
	}

	if (m_pBtn[eBTN_EXTEND_UP] != NULL)
	{
		m_pBtn[eBTN_EXTEND_UP]->Hide(bShowBtnUP);
		m_pBtn[eBTN_EXTEND_UP]->updatePosition();
	}
	if (m_pBtn[eBTN_EXTEND_DOWN] != NULL)
	{
		m_pBtn[eBTN_EXTEND_DOWN]->Hide(bShowBtnDown);
		m_pBtn[eBTN_EXTEND_DOWN]->updatePosition();
	}

	if (m_pBtn[eBTN_OPENINFO] != NULL)
		m_pBtn[eBTN_OPENINFO]->updatePosition();

	int i;
	for (i = 0; i < ePROGRESS_MAX; ++i)
	{
		if (m_pbBarBase[i] != NULL)
		{
			m_pbBarBase[i]->SetWidth(nBarWidth);
			m_pbBarBase[i]->updateWidth();
		}
	}

	for (i = eTEXT_HP; i < eTEXT_MAX; ++i)
	{
		if (m_pTextBase[i] != NULL)
			m_pTextBase[i]->SetWidth(nBarTextWidth);
	}

	if (m_pTextBase[eTEXT_NAME] != NULL)
	{
		m_pTextBase[eTEXT_NAME]->SetWidth(nPetNameWidth);
	}

	_setUIExtend();
	_setUIExpPet();
}

void CWildPetTargetUI::_setUIExpPet()
{
	CWildPetData* m_Petdata;
	m_Petdata = CWildPetData::getData(INFO()->GetMyApetInfo()->m_nIndex);

	if (m_Petdata == NULL)
		return;

	if (m_pIsExpPetBack != NULL && m_pTextExpPetUseExp != NULL)
	{
		// exp UI set
		if (m_Petdata->nFlag & WILDPET_FLAG_EXP)
		{
			m_pIsExpPetBack->Hide(FALSE);

			int nWidth;
			int nPosY;
			if (m_bExtendUI == true)
			{
				nPosY = DEF_UI_HEIGHT_EX;
				nWidth = DEF_UI_WIDTH_EX;
			}
			else
			{
				nPosY = DEF_UI_HEIGHT;
				nWidth = DEF_UI_WIDTH;
			}

			m_pIsExpPetBack->SetPos(0, nPosY);
			m_pIsExpPetBack->SetWidth(nWidth);
			m_pIsExpPetBack->UpdateSplit();
			m_pTextExpPetUseExp->SetWidth(nWidth);			
		}
		else
		{
			m_pIsExpPetBack->Hide(TRUE);
		}		
	}
}

void CWildPetTargetUI::_setUIExtend()
{
	if (m_pIsExtendBack != NULL)
	{
		if (m_bExtendUI == true)
		{
			m_pIsExtendBack->Hide(FALSE);
			if (m_pMoveArea != NULL)
				m_pMoveArea->SetSize(DEF_UI_WIDTH_EX, DEF_UI_MOVE_AREA_HEIGHT);
		}
		else
		{
			m_pIsExtendBack->Hide(TRUE);
			if (m_pMoveArea != NULL)
				m_pMoveArea->SetSize(DEF_UI_WIDTH, DEF_UI_MOVE_AREA_HEIGHT);
		}
	}
}

FLOAT CWildPetTargetUI::calcAccuExpInfo()
{
	CWildPetData	*pPetData;
	ObjInfo* pInfo = ObjInfo::getSingleton();
	pPetData = CWildPetData::getData(pInfo->GetMyApetInfo()->m_nIndex);

	if (pPetData == NULL)
		return 0.0f;

	FLOAT temresult = (float)pPetData->accexpData[0].nMaxAccParam1 + (float)(pInfo->GetMyApetInfo()->m_nLevel * pPetData->accexpData[0].nMaxAccParam2) / 100.0f;
	SQUAD lAccuresult = _pNetwork->MyCharacterInfo.needExp * (int)(temresult * 100) / 10000;

	sPetItem_Info temPetitem;
	CUIManager::getSingleton()->GetWildPetInfoUI()->GetWildPetInfo(pInfo->GetMyApetInfo()->m_nIdxServer, temPetitem);

	FLOAT fAccuExp = (FLOAT)temPetitem.pet_accexp / (FLOAT)lAccuresult;

	if (fAccuExp > 100.0f)
		fAccuExp = 0.0f;

	return fAccuExp;
}

FLOAT CWildPetTargetUI::calcHPBarInfo()
{
	ObjInfo* pInfo = ObjInfo::getSingleton();

	if ((FLOAT)pInfo->GetMyApetInfo()->m_nHP <= 0)
		return 0.0f;

	FLOAT	fHPRatio = (FLOAT)pInfo->GetMyApetInfo()->m_nHP / (FLOAT)pInfo->GetMyApetInfo()->m_nMaxHP;

	if (fHPRatio > 100.0f)
		fHPRatio = 0.0f;

	return fHPRatio * 100.0f;
}

FLOAT CWildPetTargetUI::calcMPBarInfo()
{
	ObjInfo* pInfo = ObjInfo::getSingleton();

	if ((FLOAT)pInfo->GetMyApetInfo()->m_nMP <= 0)
		return 0.0f;

	FLOAT	fHPRatio = (FLOAT)pInfo->GetMyApetInfo()->m_nMP / (FLOAT)pInfo->GetMyApetInfo()->m_nMaxMP;

	if (fHPRatio > 100.0f)
		fHPRatio = 0.0f;

	return fHPRatio * 100.0f;
}

FLOAT CWildPetTargetUI::calcEXPBarInfo()
{
	ObjInfo* pInfo = ObjInfo::getSingleton();

	if ((FLOAT)pInfo->GetMyApetInfo()->m_exp <= 0)
		return 0.0f;

	FLOAT	fHPRatio = (FLOAT)pInfo->GetMyApetInfo()->m_exp / (FLOAT)pInfo->GetMyApetInfo()->m_next_exp;

	if (fHPRatio > 100.0f)
		fHPRatio = 0.0f;

	return fHPRatio * 100.0f;
}

FLOAT CWildPetTargetUI::calcAccuExpBarWidth()
{
	FLOAT fTempAccu = calcAccuExpInfo();

	int nBarWidth;
	if (m_bExtendUI == true)
		nBarWidth = DEF_UI_WIDTH_EX;
	else
		nBarWidth = DEF_UI_WIDTH;

	return (nBarWidth - DEF_UI_ACC_EXP_BAR_GAP) * fTempAccu;
}

void CWildPetTargetUI::OnUpdate( float fDeltaTime, ULONG ElapsedTime )
{
	_updateAccExpPetTooltip();
	_updateOpenWildPetInfoTooltip();
}

void CWildPetTargetUI::_updateAccExpPetTooltip()
{
	if (GetHide() == TRUE)
		return;

	ObjInfo* pInfo = ObjInfo::getSingleton();
	CWildPetData	*pPetdata;
	pPetdata = CWildPetData::getData(pInfo->GetMyApetInfo()->m_nIndex);

	if (pPetdata == NULL)
		return;

	if (!(pPetdata->nFlag & WILDPET_FLAG_EXP))
		return;

	// tool tip
	sPetItem_Info temPetitem;
	CUIManager::getSingleton()->GetWildPetInfoUI()->GetWildPetInfo(pInfo->GetMyApetInfo()->m_nIdxServer, temPetitem);

	CTString strTemp;
	if (temPetitem.pet_cooltime > 0)
	{
		strTemp.PrintF(_S(5644, "쿨타임"));				
		INDEX temCooltime = temPetitem.pet_cooltime - (unsigned int)_pTimer->GetLerpedCurrentTick();

		int iSec = temCooltime % 60;
		temCooltime /= 60;

		int iMin = temCooltime % 60;
		int iHour = temCooltime /= 60;
		CTString temtime;

		strTemp += ": ";

		if (iHour > 0)
		{
			temtime.PrintF(_S(2512,"%d시간"),iHour);
			strTemp += temtime;
		}
		if (iMin > 0)
		{
			temtime.PrintF(_S(2513, "%d분"),iMin);
			strTemp += temtime;
		}
		if (iSec > 0)
		{
			temtime.PrintF(_S(2514,"%d초"),iSec);
			strTemp += temtime;
		}

	}
	else	
	{
		FLOAT temresult = (float)pPetdata->accexpData[0].nMaxAccParam1 + (float)(MY_APET_INFO()->m_nLevel * pPetdata->accexpData[0].nMaxAccParam2) / 100.0f;
		SQUAD lAccuresult = _pNetwork->MyCharacterInfo.needExp * (int)(temresult * 100) / 10000;

		CTString strAccExp, strAccResult;
		strAccExp.PrintF("%I64d", temPetitem.pet_accexp);
		strAccResult.PrintF("%I64d", lAccuresult);

		CUIManager* pUIMgr = UIMGR();
		pUIMgr->InsertCommaToString(strAccExp);
		pUIMgr->InsertCommaToString(strAccResult);

		strTemp.PrintF("%s / %s", strAccExp, strAccResult);
	}

	if (m_pTextExpPetUseExp != NULL)
		m_pTextExpPetUseExp->setTooltip(strTemp);
}

void CWildPetTargetUI::_updateOpenWildPetInfoTooltip()
{
	if (GetHide() == TRUE)
		return;

	if (m_pBtn[eBTN_OPENINFO] != NULL)
	{
		extern INDEX g_iEnterChat;
		CTString strTooltip = _S(2173,"펫 정보창") ;
		if( g_iEnterChat )
			strTooltip += CTString("(P,ALT+P)");
		else 
			strTooltip += CTString("(ALT+P)");

		m_pBtn[eBTN_OPENINFO]->setTooltip(strTooltip);
	}
}


WMSG_RESULT CWildPetTargetUI::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
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

WMSG_RESULT CWildPetTargetUI::OnLButtonDown( UINT16 x, UINT16 y )
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
	else if (m_pIsBaseBack != NULL && m_pIsBaseBack->IsInside(x, y))
	{
		ObjInfo* pInfo = ObjInfo::getSingleton();
		if( pInfo->GetMyApetInfo()->m_pEntity )
		{
			CEntity			*penPlEntity;
			CPlayerEntity	*penPlayerEntity;
			penPlEntity = CEntity::GetPlayerEntity( 0 );
			penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);
			penPlayerEntity->SetTarget( pInfo->GetMyApetInfo()->m_pEntity );
		}
	}

	CUIManager::getSingleton()->RearrangeOrder( UI_WILDPET_TARGETINFO, TRUE );

	return WMSG_FAIL;
}

WMSG_RESULT CWildPetTargetUI::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_bDrag = false;

	CUIManager::getSingleton()->ResetHoldBtn();
	return WMSG_FAIL;
}
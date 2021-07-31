#include "stdh.h"
#include <Engine/Interface/UIManager.h>
#include <Engine/Info/MyInfo.h>
#include "WildPetInfoUI.h"
#include "WildPetInfoCmd.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UIInventory.h>
#include <Common/Packet/ptype_old_do_item.h>
#include <Engine/GameState.h>

extern INDEX g_iCountry;

enum eSTATE_IMAGE_INDEX
{
	eIMG_IDX_HIGH = 0, // 최상
	eIMG_IDX_NORMAL,	 // 보통
	eIMG_IDX_LOW		 // 최하
};

#define DEF_HIGH_CUTLINE	60
#define DEF_NORMAL_CUTLINE	20

CWildPetInfoUI::CWildPetInfoUI()
	: m_pMoveArea(NULL)
	, m_bDrag(false)
	, m_nOriX(0), m_nOriY(0)
	, m_nMouseX(0)
	, m_nMouseY(0)
	, m_pListSkill(NULL)
	, m_pChkAISet(NULL)
	, m_pImgBack(NULL)
	, m_pImgAISetUIBack(NULL)
	, m_pListAI(NULL)
	, m_pBtnAISetOK(NULL)
	, m_nAISlotCount(3)
	, m_pImgExpBack(NULL)
	, m_pBtnExpUse(NULL)
	, m_pInvenBack(NULL)
	, m_pMainTab(NULL)
	, m_Petdata(NULL)
{
	int i;
	for (i = 0; i < eSTRING_MAX; ++i)
		m_pText[i] = NULL;

	for (i = 0; i < eBTN_MAX; ++i)
		m_pBtn[i] = NULL;

	for (i = 0; i < WILDPET_WEAR_TOTAL; ++i)
		m_pIcon[i] = NULL; 

	for (i = 0; i < eTAB_MAX; ++i)
	{
		m_pChkTabBtn[i] = NULL;
		m_pTextChk[i] = NULL;
	}

	for (i = 0; i < eSTAT_MAX; ++i)
	{
		m_pStatText[i] = NULL;
		m_pBtnStat[i] = NULL;
	}

	for (i = 0; i < eSTATE_MAX; ++i)
	{
		m_pImgArr[i] = NULL;
		m_pTextState[i] = NULL;
	}

	for (i = 0; i < eEXP_STRING_MAX; ++i)
	{
		m_pTextExp[i] = NULL;
	}

	for (i = 0; i < WILDPET_AI_MAXSLOT; i++)
	{
		m_sAISlot[i].m_bActive = FALSE;
		m_sAISlot[i].m_nSillType = -1;
		m_sAISlot[i].m_nActionSlot = -1;
		m_sAISlot[i].m_nTargetSlot = -1;

		m_pchkAISlotActive[i] = NULL;
		m_pIconAITarget[i] = NULL;
		m_pIconAIAction[i] = NULL;
	}

	clear_upgradeinfo();
}

CWildPetInfoUI::~CWildPetInfoUI()
{
	Destroy();
}

void CWildPetInfoUI::updateUI()
{
	updateBaseTab();
	updateSkillTab();
	updateAISetUI();
	PetAIReSet();
}

void CWildPetInfoUI::updateBaseTab()
{
	ObjInfo* pInfo = ObjInfo::getSingleton();

	CWildPetTarget* pMyWildPet = pInfo->GetMyApetInfo();

	if (pMyWildPet == NULL)
		return;

	CTString strTemp;
	// 이름
	if (m_pText[eSTRING_NAME] != NULL)
	{
		strTemp = pMyWildPet->m_strName.c_str();
		m_pText[eSTRING_NAME]->SetText(strTemp);
	}
	// 레벨
	if (m_pText[eSTRING_LEVEL] != NULL)
	{
		strTemp.PrintF("%d", pMyWildPet->m_nLevel);
		m_pText[eSTRING_LEVEL]->SetText(strTemp);
	}

	// 종족
	INDEX iPetType = pInfo->GetMyApetInfo()->m_nType;
	
	switch(iPetType)
	{
	case eWILDPET_HUMAN:
		strTemp.PrintF(_S(4207, "인간형"));
		break;
	case eWILDPET_ANIMAL:
		strTemp.PrintF(_S(4208, "동물형"));
		break;
	case eWILDPET_DEMON:
		strTemp.PrintF(_S(5269, "데몬형"));
		break;
	}

	if (m_pText[eSTRING_SPECIES] != NULL)
		m_pText[eSTRING_SPECIES]->SetText(strTemp);

	// 경험치
	CTString strExp, strNextExp;
	strExp.PrintF("%I64d", pMyWildPet->m_exp);
	strNextExp.PrintF("%I64d", pMyWildPet->m_next_exp);

	CUIManager* pUIMgr = UIMGR();
	pUIMgr->InsertCommaToString(strExp);
	pUIMgr->InsertCommaToString(strNextExp);

	strTemp.PrintF("%s / %s", strExp, strNextExp);

	if (m_pText[eSTRING_EXP] != NULL)
		m_pText[eSTRING_EXP]->SetText(strTemp);

	// 충성도
	eSTATE_IMAGE_INDEX eIndex;

	FLOAT temResult = (FLOAT)pMyWildPet->m_nFaith / (FLOAT)pMyWildPet->m_nMaxFaith * 100.0f;

	if(temResult > DEF_HIGH_CUTLINE)
		eIndex = eIMG_IDX_HIGH;	
	else if(temResult > DEF_NORMAL_CUTLINE)
		eIndex = eIMG_IDX_NORMAL;
	else
		eIndex = eIMG_IDX_LOW;
	
	if (m_pImgArr[eSTATE_FAITH] != NULL)
		m_pImgArr[eSTATE_FAITH]->SetRenderIdx(eIndex);

	strTemp.PrintF("%d/%d", pMyWildPet->m_nFaith, pMyWildPet->m_nMaxFaith);

	if (m_pTextState[eSTATE_FAITH] != NULL)
		m_pTextState[eSTATE_FAITH]->SetText(strTemp);

	// 배고품
	temResult = (FLOAT)pMyWildPet->m_nStm / (FLOAT)pMyWildPet->m_nMaxStm * 100.0f;

	if(temResult > DEF_HIGH_CUTLINE)
		eIndex = eIMG_IDX_HIGH;	
	else if(temResult > DEF_NORMAL_CUTLINE)
		eIndex = eIMG_IDX_NORMAL;
	else
		eIndex = eIMG_IDX_LOW;

	if (m_pImgArr[eSTATE_HUNGRY] != NULL)
		m_pImgArr[eSTATE_HUNGRY]->SetRenderIdx(eIndex);

	strTemp.PrintF("%d/%d", pMyWildPet->m_nStm, pMyWildPet->m_nMaxStm);

	if (m_pTextState[eSTATE_HUNGRY] != NULL)
		m_pTextState[eSTATE_HUNGRY]->SetText(strTemp);

	// 스텟 or 경험치 펫 인포
	if (m_Petdata != NULL)
	{
		if (m_Petdata->nFlag & WILDPET_FLAG_EXP)
			updateExpInfo();
		else
			updateStat();
	}

	// inven update
	PetWearItemReSet();
}

void CWildPetInfoUI::updateSkillTab()
{
	// 스킬 업데이트
	if (m_pListSkill == NULL)
		return;

	m_pListSkill->DeleteAllListItem();

	int nSkillCount = m_vecSkills.size();

	if (nSkillCount <= 0)
	{
		m_pListSkill->UpdateScroll(nSkillCount);
		return;
	}

	CUIListItem* pTempItem = NULL;
	CUIListItem* pItem = NULL;

	pTempItem = m_pListSkill->GetListItemTemplate();

	if (pTempItem == NULL)
		return;

	CUIIcon* pIcon = NULL;
	CUIText* pText = NULL;

	for (int i = 0; i < nSkillCount; ++i)
	{
		m_pListSkill->AddListItem(pTempItem->Clone());
		
		pItem = (CUIListItem*)m_pListSkill->GetListItem(i);

		if (pItem == NULL)
			continue;

		CSkill&	rSkill = _pNetwork->GetSkillData(m_vecSkills[i]);
		SBYTE	sbLevel = MY_INFO()->GetPetSkillLevel(0, m_vecSkills[i]);
		BOOL	bDelay = MY_INFO()->GetPetSkillDelay(0, m_vecSkills[i]);
	
		// skill Icon
		pIcon = (CUIIcon*)pItem->findUI("icon_skill");

		if (pIcon != NULL)
		{
			pIcon->initialize();
			pIcon->setData(UBET_SKILL, m_vecSkills[i]);
			pIcon->SetWhichUI(UI_WILDPET_INFO);

			if (bDelay == TRUE)
				pIcon->setCooltime(true);

			// 스킬 사용 커맨드 연결
			if (rSkill.GetType() != CSkill::ST_PET_SKILL_PASSIVE)
			{
				CmdWildPetInfoSkillUse* pCmd = new CmdWildPetInfoSkillUse;
				pCmd->setData(this, m_vecSkills[i]);
				pIcon->SetCommandUp(pCmd);

				CmdDragIcon* pCmdDrag = new CmdDragIcon;
				pCmdDrag->setData(pIcon);
				pIcon->SetCommandDrag(pCmdDrag);
			}			
		}

		CTString strTemp;
		COLOR color = 0xC5C5C5FF;
		// skill Name
		pText = (CUIText*)pItem->findUI("str_skillName");

		if (pText != NULL)
		{
			strTemp = rSkill.GetName();
			pText->SetText(strTemp);
			pText->setFontColor(color);
		}
		// skill Level
		strTemp.PrintF("%s %d", _S(4414, "LV"), sbLevel);

		pText = (CUIText*)pItem->findUI("str_skillLevel");

		if (pText != NULL)
		{
			pText->SetText(strTemp);
			pText->setFontColor(color);
		}
	}

	m_pListSkill->UpdateList();
	m_pListSkill->UpdateScroll(nSkillCount);
	m_pListSkill->setCurSel(-1);
}

void CWildPetInfoUI::updateAISetUI()
{
	BOOL bActive;
	for (int i = 0; i < WILDPET_AI_MAXSLOT; ++i)
	{
		if (m_nAISlotCount > i)
			bActive = TRUE;
		else
			bActive = FALSE;

		if (m_pIconAITarget[i] != NULL)
			m_pIconAITarget[i]->Hide(!bActive);

		if (m_pIconAIAction[i] != NULL)
			m_pIconAIAction[i]->Hide(!bActive);

		if (m_pchkAISlotActive[i] != NULL)
			m_pchkAISlotActive[i]->Hide(!bActive);
	}
}

void CWildPetInfoUI::updateStat()
{
	ObjInfo* pInfo = ObjInfo::getSingleton();

	CWildPetTarget* pMyWildPet = pInfo->GetMyApetInfo();

	if (pMyWildPet == NULL)
		return;

	CTString strTemp;
	// 힘
	strTemp.PrintF("%d(%d)",pMyWildPet->m_nStr,pMyWildPet->m_nStrPlus);

	if (m_pStatText[eSTAT_STR] != NULL)
		m_pStatText[eSTAT_STR]->SetText(strTemp);

	// 체질
	strTemp.PrintF("%d(%d)",pMyWildPet->m_nCon,pMyWildPet->m_nConPlus);

	if (m_pStatText[eSTAT_CON] != NULL)
		m_pStatText[eSTAT_CON]->SetText(strTemp);

	// 민첩
	strTemp.PrintF("%d(%d)",pMyWildPet->m_nDex,pMyWildPet->m_nDexPlus);

	if (m_pStatText[eSTAT_DEX] != NULL)
		m_pStatText[eSTAT_DEX]->SetText(strTemp);

	// 지혜
	strTemp.PrintF("%d(%d)",pMyWildPet->m_nInt,pMyWildPet->m_nIntPlus);

	if (m_pStatText[eSTAT_INT] != NULL)
		m_pStatText[eSTAT_INT]->SetText(strTemp);

	// 성장포인트
	strTemp.PrintF("%d",pMyWildPet->m_nLevelupPoint);

	if (m_pText[eSTRING_STAT_POINT] != NULL)
		m_pText[eSTRING_STAT_POINT]->SetText(strTemp);

	COLOR	temColor = 0xFFFFFFFF;

	FLOAT temResult = (FLOAT)pMyWildPet->m_nFaith / (FLOAT)pMyWildPet->m_nMaxFaith * 100.0f;

	// 충성도에 따라 패널티가 주어짐 (공격력, 방어력 마법포함)
	if(temResult <= DEF_NORMAL_CUTLINE)
		temColor = 0XFF0000FF;
	else
		temColor = 0xFFFFFFFF;

	// 공격력
	strTemp.PrintF("%d",pMyWildPet->m_nWildPetState[STATE_ATTACK]);

	if (m_pText[eSTRING_ATT] != NULL)
	{
		m_pText[eSTRING_ATT]->SetText(strTemp);
		m_pText[eSTRING_ATT]->setFontColor(temColor);
	}

	// 마법공력력
	strTemp.PrintF("%d",pMyWildPet->m_nWildPetState[STATE_MAGIC_ATTACK]);

	if (m_pText[eSTRING_MAGIC_ATT] != NULL)
	{
		m_pText[eSTRING_MAGIC_ATT]->SetText(strTemp);
		m_pText[eSTRING_MAGIC_ATT]->setFontColor(temColor);
	}

	// 방어력
	strTemp.PrintF("%d",pMyWildPet->m_nWildPetState[STATE_DEFENCE]);

	if (m_pText[eSTRING_DEF] != NULL)
	{
		m_pText[eSTRING_DEF]->SetText(strTemp);
		m_pText[eSTRING_DEF]->setFontColor(temColor);
	}

	// 마법방어력
	strTemp.PrintF("%d",pMyWildPet->m_nWildPetState[STATE_MAGIC_DEFENCE]);

	if (m_pText[eSTRING_MAGIC_DEF] != NULL)
	{
		m_pText[eSTRING_MAGIC_DEF]->SetText(strTemp);
		m_pText[eSTRING_MAGIC_DEF]->setFontColor(temColor);
	}

	temResult = (FLOAT)pMyWildPet->m_nStm / (FLOAT)pMyWildPet->m_nMaxStm * 100.0f;

	// 배고품에 따라 패널티가 주어짐 (명중, 회피)
	if(temResult <= DEF_NORMAL_CUTLINE)
		temColor = 0XFF0000FF;
	else
		temColor = 0xFFFFFFFF;

	// 명중도
	strTemp.PrintF("%d",pMyWildPet->m_nWildPetState[STATE_HITPOINT]);

	if (m_pText[eSTRING_ACCURACY] != NULL)
	{
		m_pText[eSTRING_ACCURACY]->SetText(strTemp);
		m_pText[eSTRING_ACCURACY]->setFontColor(temColor);
	}

	// 회피
	strTemp.PrintF("%d",pMyWildPet->m_nWildPetState[STATE_AVOID]);

	if (m_pText[eSTRING_DODGE] != NULL)
	{
		m_pText[eSTRING_DODGE]->SetText(strTemp);
		m_pText[eSTRING_DODGE]->setFontColor(temColor);
	}

	_checkStatUpBtnEnable();
}

void CWildPetInfoUI::updateExpInfo()
{
	ObjInfo* pInfo = ObjInfo::getSingleton();

	CWildPetTarget* pMyWildPet = pInfo->GetMyApetInfo();

	if (pMyWildPet == NULL)
		return;

	CTString strTemp;
	// 경험치 축적 률
	FLOAT ntempresult = m_Petdata->accexpData[0].nAccRateParam1 + (pMyWildPet->m_nLevel * m_Petdata->accexpData[0].nAccRateParam2)/100.0f ;
	strTemp.PrintF("%.2f%%", ntempresult);

	if (m_pTextExp[eEXP_STRING_PERCENT] != NULL)
		m_pTextExp[eEXP_STRING_PERCENT]->SetText(strTemp);

	CUIManager* pUIMgr = UIMGR();
	// 경험치 축적 가능량
	FLOAT temresult = (float)m_Petdata->accexpData[0].nMaxAccParam1 + (float)(pMyWildPet->m_nLevel * m_Petdata->accexpData[0].nMaxAccParam2) / 100.0f;
	// 부동소수점 연산으로 인한 계산오차가 발생하므로 연산식 수정 [1/13/2012 ldy1978220]
	SQUAD lAccuresult = _pNetwork->MyCharacterInfo.needExp * (int)(temresult * 100) / 10000;

	strTemp.PrintF("%I64d",lAccuresult);
	pUIMgr->InsertCommaToString(strTemp);

	if (m_pTextExp[eEXP_STRING_MAX_EXP] != NULL)
		m_pTextExp[eEXP_STRING_MAX_EXP]->SetText(strTemp);

	// 축척 된 경험치 량
	sPetItem_Info temPetitem;
	GetWildPetInfo(pMyWildPet->m_nIdxServer, temPetitem);

	strTemp.PrintF("%I64d", temPetitem.pet_accexp);
	pUIMgr->InsertCommaToString(strTemp);

	if (m_pTextExp[eEXP_STRING_CURRENT_EXP] != NULL)
		m_pTextExp[eEXP_STRING_CURRENT_EXP]->SetText(strTemp);
}

void CWildPetInfoUI::ToggleVisible()
{
	BOOL	bVisible = GetHide();

	if(bVisible == TRUE)
		openUI();
	else
		closeUI();
}

void CWildPetInfoUI::openUI()
{
	ObjInfo* pInfo = ObjInfo::getSingleton();

	if (pInfo->GetMyApetInfo() == NULL)
	{
		closeUI();
		return;
	}

	if (GetHide() == FALSE)
		return;

	Hide(FALSE);
	SetVisible(TRUE);
	
	m_Petdata = CWildPetData::getData(pInfo->GetMyApetInfo()->m_nIndex);

	_setUI();
	updateUI();
	
	CUIManager* pUIMgr = UIMGR();
	pUIMgr->RearrangeOrder( UI_WILDPET_INFO, TRUE );
}

void CWildPetInfoUI::closeUI()
{
	Hide(TRUE);
	SetVisible(FALSE);

	PetWearItemReSet();
	
	CUIManager* pUIMgr = UIMGR();
	pUIMgr->RearrangeOrder( UI_WILDPET_INFO, FALSE );
}

void CWildPetInfoUI::initialize()
{
	int i;
	// 닫기. AI셋팅 버튼
	std::string strBtnID[eBTN_MAX] = { "btn_close", "btn_AISetting" };
	
	for (i = 0; i < eBTN_MAX; ++i)
		m_pBtn[i] = (CUIButton*)findUI(strBtnID[i].c_str());

#ifndef		WORLD_EDITOR
	if (m_pBtn[eBTN_CLOSE] != NULL)
		m_pBtn[eBTN_CLOSE]->SetCommandFUp(boost::bind(&CWildPetInfoUI::closeUI, this));

	if (m_pBtn[eBTN_AI_SET] != NULL)
		m_pBtn[eBTN_AI_SET]->SetCommandFUp(boost::bind(&CWildPetInfoUI::OpenAISetUI, this));
#endif	// WORLD_EDITOR

	// Tab 버튼
	m_pMainTab = (CUITab*)findUI("main_tab");

	if (m_pMainTab != NULL)
	{
		CmdWildPetInfoMainTabChange* pCmd = new CmdWildPetInfoMainTabChange;
		pCmd->setData(this);
		m_pMainTab->SetCommandChange(pCmd);
	}
	
	std::string strChkBtnID[eTAB_MAX] = { "chk_info", "chk_skill" };
	std::string strTabStringID[eTAB_MAX] = { "str_chkInfo", "str_chkSkill" };
		
	for (i = 0; i < eTAB_MAX; ++i)
	{
		m_pChkTabBtn[i] = (CUICheckButton*)findUI(strChkBtnID[i].c_str());
		m_pTextChk[i] = (CUIText*)findUI(strTabStringID[i].c_str());

		if (m_pTextChk[i] != NULL)
			m_pTextChk[i]->setVerticality(TRUE);
	}

	// Drag 영역
	m_pMoveArea = (CUIImage*)findUI("move_area");

	// Back Ground
	m_pImgBack = (CUIImage*)findUI("img_back");

	if (m_pImgBack)
		m_pImgBack->setInherit(false);

	// Exp 펫 관련
	m_pImgExpBack = (CUIImage*)findUI("img_expPetBack");
	m_pBtnExpUse = (CUIButton*)findUI("btn_useExp");

#ifndef		WORLD_EDITOR
	if (m_pBtnExpUse != NULL)
		m_pBtnExpUse->SetCommandFUp(boost::bind(&CWildPetInfoUI::ExpUse, this));
#endif	// WORLD_EDITOR

	std::string strExpID[eEXP_STRING_MAX] = {"str_expPercent", "str_expMax", "str_expcur", "str_petcooltimeTitle", "str_petcooltime"}; 

	for (i = 0; i < eEXP_STRING_MAX; ++i)
	{
		m_pTextExp[i] = (CUIText*)findUI(strExpID[i].c_str());
	}

	// 기본정보 탭
	_initBaseInfoTab();

	// 스킬정보 탭
	_initSkillTab();

	// AI셋팅 UI
	_initAISetUI();
}

void CWildPetInfoUI::_initBaseInfoTab()
{
	int i;
	// 착용 장비
	std::string strIconID[WILDPET_WEAR_TOTAL] = {"icon_head", "icon_body", "icon_weapon", "icon_acce"};

	for (i = 0; i < WILDPET_WEAR_TOTAL; ++i)
	{
		m_pIcon[i] = (CUIIcon*)findUI(strIconID[i].c_str());
		if (m_pIcon[i] != NULL)
		{
			m_pIcon[i]->clearIconData();
			m_pIcon[i]->SetWhichUI(UI_WILDPET_INFO);

			CmdDragIcon* pCmdDrag = new CmdDragIcon;
			pCmdDrag->setData(m_pIcon[i]);
			m_pIcon[i]->SetCommandDrag(pCmdDrag);

			CmdWildPetInfoInvenItemDBL* pCmdDBL = new CmdWildPetInfoInvenItemDBL;
			pCmdDBL->setData(this, i);
			m_pIcon[i]->SetCommandDBL(pCmdDBL);
		}		
	}

	m_pInvenBack = (CUIImageArray*)findUI("ia_inven_back");

	// stat 정보
	std::string strStatTextID[eSTAT_MAX] = { "str_str", "str_con", "str_dex", "str_int" };
	std::string strStatBtnID[eSTAT_MAX] = { "btn_strUp", "btn_conUp", "btn_dexUp", "btn_intUp" };

	for (i = 0; i < eSTAT_MAX; ++i)
	{
		m_pStatText[i] = (CUIText*)findUI(strStatTextID[i].c_str());
		m_pBtnStat[i] = (CUIButton*)findUI(strStatBtnID[i].c_str());

		if (m_pBtnStat[i] != NULL)
		{
			CmdWildPetInfoStatUp* pCmd = new CmdWildPetInfoStatUp;
			pCmd->setData(this, i);
			m_pBtnStat[i]->SetCommandUp(pCmd);
		}
	}

	// 배고픔, 충성도 상태
	std::string strImgState[eSTATE_MAX] = { "ia_hungry", "ia_faith" };
	std::string strTextState[eSTATE_MAX] = { "str_hungry", "str_faith" };

	for (i = 0; i < eSTATE_MAX; ++i)
	{
		m_pImgArr[i] = (CUIImageArray*)findUI(strImgState[i].c_str());
		m_pTextState[i] = (CUIText*)findUI(strTextState[i].c_str());
	}

	// AI (켜기,끄기) 체크버튼
	m_pChkAISet = (CUICheckButton*)findUI("chk_AISwitch");

#ifndef		WORLD_EDITOR
	if (m_pChkAISet != NULL)
	{
		m_pChkAISet->SetCommandF(boost::bind(&CWildPetInfoUI::SendAIActive, this));
		m_bAIActive = m_pChkAISet->IsChecked();
	}
#endif	// WORLD_EDITOR

	// 이름, 레벨, 종족, 경험치, 성장포인트, 공격력, 마법공격력, 방어력, 마법방어력, 명중도, 회피도
	std::string strTextID[eSTRING_MAX] = { "text_name", "text_level", "text_type", "text_exp", "str_statpoint",
	"str_att", "str_Matt", "str_def", "str_Mdef", "str_accuracy", "str_dodge" };

	for (i = 0; i < eSTRING_MAX; ++i)
		m_pText[i] = (CUIText*)findUI(strTextID[i].c_str());

}

void CWildPetInfoUI::_initSkillTab()
{
	m_pListSkill = (CUIList*)findUI("list_skill");
}

void CWildPetInfoUI::_initAISetUI()
{
	m_pImgAISetUIBack = (CUIImage*)findUI("img_AISetBack");

	if (m_pImgAISetUIBack != NULL)
		m_pImgAISetUIBack->setInherit(false);

	// AI Set
	m_pListAI = (CUIList*)findUI("list_AI");

	if (m_pListAI != NULL)
	{
		int nChildCnt = m_pListAI->getListItemCount();
		CUIListItem* pItem = NULL;
		for (int i = 0; i < nChildCnt; ++i)
		{
			pItem = (CUIListItem*)m_pListAI->GetListItem(i);

			if (pItem == NULL)
				continue;

			m_pIconAITarget[i] = (CUIIcon*)pItem->findUI("icon_ai");

			if (m_pIconAITarget[i] != NULL)
				m_pIconAITarget[i]->clearIconData();

			m_pIconAIAction[i] = (CUIIcon*)pItem->findUI("icon_action");

			if (m_pIconAIAction[i] != NULL)
				m_pIconAIAction[i]->clearIconData();

			m_pchkAISlotActive[i] = (CUICheckButton*)pItem->findUI("chk_selectAI");
		}
	}

	// 편집완료
	m_pBtnAISetOK = (CUIButton*)findUI("btn_ok");

#ifndef		WORLD_EDITOR
	if (m_pBtnAISetOK != NULL)
		m_pBtnAISetOK->SetCommandFUp(boost::bind(&CWildPetInfoUI::SendAIList, this));
#endif	// WORLD_EDITOR
}

void CWildPetInfoUI::_setUI()
{
	ObjInfo* pInfo = ObjInfo::getSingleton();

	if (pInfo->GetMyApetInfo() == NULL)
		return;

	// 펫 타입에 맞춰 인벤 배경 설정
	INDEX iPetType = pInfo->GetMyApetInfo()->m_nType;
	int nBackImgIndex = iPetType - 1; // 펫타입이 1 부터 시작하기에 -1을 함

	if (m_pInvenBack != NULL)
		m_pInvenBack->SetRenderIdx(nBackImgIndex);

	// 경험치 펫일 경우 스텟 창 변경
	
	if(m_Petdata == NULL)
		return;

	BOOL bShowState = TRUE;
	BOOL bExpPet = FALSE;

	if(m_Petdata->nFlag & WILDPET_FLAG_EXP)
	{
		bShowState = FALSE;
		bExpPet = TRUE;
	}

	if (m_pImgExpBack != NULL)
		m_pImgExpBack->Hide(!bExpPet);

	if (m_pBtn[eBTN_AI_SET] != NULL)
		m_pBtn[eBTN_AI_SET]->Hide(!bShowState);

	if (m_pChkAISet != NULL)
		m_pChkAISet->Hide(!bShowState);

	changeMainTab();
}

void CWildPetInfoUI::_checkStatUpBtnEnable()
{
	CWildPetTarget* pMyWildPet = ObjInfo::getSingleton()->GetMyApetInfo();

	if (pMyWildPet == NULL)
		return;

	BOOL bEnable = TRUE;

	if (pMyWildPet->m_nLevelupPoint <= 0)
		bEnable = FALSE;

	for (int i = 0; i < eSTAT_MAX; ++i)
		m_pBtnStat[i]->SetEnable(bEnable);
}

BOOL CWildPetInfoUI::IsLearnSkill(int nSkillIndex )
{
	vec_skill::iterator	iter = m_vecSkills.begin();	
	vec_skill::iterator	iterEnd = m_vecSkills.end();

	for (; iter != iterEnd; ++iter)
	{
		if ( *iter == nSkillIndex )
			return TRUE;
	}
	return FALSE;
}

BOOL CWildPetInfoUI::IsUsableSkill(LONG nSkillIndex )
{
	if( IsLearnSkill(nSkillIndex) == FALSE )
		return FALSE;

	CSkill& rSkill = _pNetwork->GetSkillData( nSkillIndex );
	// 실제 스킬 레벨은 0부터 시작.
	if (MY_APET_INFO()->m_nMP < rSkill.GetNeedMP(MY_INFO()->GetPetSkillLevel(0, nSkillIndex) - 1))
	{
		return FALSE;
	}
	return TRUE;
}

void CWildPetInfoUI::AddSkill(int nSkillIndex, SBYTE sbSkillLevel )
{
	m_vecSkills.push_back(nSkillIndex);

	updateSkillTab();
}

void CWildPetInfoUI::SkillClear()
{
	m_vecSkills.clear();

	if(m_pListSkill != NULL)
		m_pListSkill->DeleteAllListItem();
}

void CWildPetInfoUI::UseSkill( int nIndex )
{	
	if (m_bAIActive)
	{
		if (m_pChkAISet != NULL)
			m_pChkAISet->SetCheck(FALSE);

		// AI를 꺼준다.
		SendAIActive();
	}

	if( IsLearnSkill(nIndex) == FALSE )
		return;

	CSkill& rSkill = _pNetwork->GetSkillData( nIndex );
	ObjInfo* pInfo = ObjInfo::getSingleton();

	if (pInfo->GetMyApetInfo() == NULL)
		return;

	if( rSkill.GetType() == CSkill::ST_PET_SKILL_PASSIVE )
		return;

	// 실제 스킬 레벨은 0부터 시작.
	if (pInfo->GetMyApetInfo()->m_nMP < rSkill.GetNeedMP(MY_INFO()->GetPetSkillLevel(0, nIndex) - 1))
	{
		CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( _S( 320, "MP가 부족합니다."), SYSMSG_ERROR );
		return;
	}

	if( pInfo->GetMyApetInfo()->m_pEntity ) // 예외 처리
	{
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->UseWildSkill( pInfo->GetMyApetInfo()->m_pEntity, nIndex );	
	}

}

void CWildPetInfoUI::PetWearItemReSet()
{
	ObjInfo* pInfo = ObjInfo::getSingleton();

	for(int i = 0; i < WILDPET_WEAR_TOTAL; i++)
	{
		if(pInfo->GetMyApetInfo() != NULL && pInfo->GetMyApetInfo()->m_nPetWearIndex[i] < 0)
		{
			if (m_pIcon[i] != NULL)
			{
				m_pIcon[i]->clearIconData();
			}			
		}
	}
}

void CWildPetInfoUI::WearItemDBL(int nWearPos)
{
	if (nWearPos < 0 || nWearPos >= WILDPET_WEAR_TOTAL)
		return;
	
	if (MY_APET_INFO() == NULL || 
		MY_APET_INFO()->m_nPetWearIndex[nWearPos] <= 0)
		return;

	if (UIMGR()->GetInventory()->getLocked() == LOCK_PET_ITEM_UPGRADE)
		return;

	_pNetwork->SendUseWildPetItem(nWearPos, -1, -1, MY_APET_INFO()->m_nPetWearIndex[nWearPos]);
}

void CWildPetInfoUI::AddWildPetInfo( sPetItem_Info sPetInfo )
{
	sPetItem_Info TempData;
	TempData = sPetInfo;
	int nPetIndex = TempData.pet_index;

	WildPetInfo_map::iterator iter = m_mapWildPetInfo.find( nPetIndex );

	if( iter != m_mapWildPetInfo.end() )	//이미 가지고 있는 펫 데이터이면 업데이트...
		iter->second =TempData;
	else										//가지고 있지 않는 펫 데이터이면 추가...
	{
		WildPetInfo_map::value_type tmpVal( nPetIndex, TempData );
		m_mapWildPetInfo.insert( tmpVal );
	}

	if (GetHide() == FALSE)
		updateBaseTab();
}

BOOL CWildPetInfoUI::GetWildPetInfo(int nPetIndex, sPetItem_Info& WildPetInfo )
{
	WildPetInfo_map::iterator PetInfoiter = m_mapWildPetInfo.find( nPetIndex );

	if( PetInfoiter != m_mapWildPetInfo.end() )
	{
		WildPetInfo = PetInfoiter->second;
		return TRUE;
	}else return FALSE;
}

void CWildPetInfoUI::PetAIReSet()
{
	for (int nSlot = 0; nSlot < WILDPET_AI_MAXSLOT; nSlot++)
	{
		if (m_pIconAITarget != NULL)
		{
			m_pIconAITarget[nSlot]->clearIconData();

			if (m_sAISlot[nSlot].m_nTargetSlot > 0)
			{
				m_pIconAITarget[nSlot]->setData(UBET_ITEM, m_sAISlot[nSlot].m_nTargetSlot);
			}
		}

		if (m_pIconAIAction != NULL)
		{
			m_pIconAIAction[nSlot]->clearIconData();

			if (m_sAISlot[nSlot].m_nActionSlot > 0)
			{
				if(m_sAISlot[nSlot].m_nSillType == UBET_SKILL)
				{
					if(IsLearnSkill(m_sAISlot[nSlot].m_nActionSlot))
					{
						m_pIconAIAction[nSlot]->setData(UBET_SKILL, m_sAISlot[nSlot].m_nActionSlot);
					}
				}else
				{
					m_pIconAIAction[nSlot]->setData(UBET_ACTION, m_sAISlot[nSlot].m_nActionSlot);
				}
			}
		}
				
		if (m_pchkAISlotActive[nSlot] != NULL)
		{
			m_pchkAISlotActive[nSlot]->SetCheck(m_sAISlot[nSlot].m_bActive);
		}		
	}
}

void CWildPetInfoUI::RemoveSlot(CUIIcon* pIcon)
{
	if (m_pListAI == NULL)
		return;

	CUIListItem* pItem = NULL;
	CUIIcon* pIconTemp = NULL;
	int nCount = m_pListAI->getListItemCount();

	if (nCount <= 0)
		return;

	for (int i = 0; i < nCount; ++i)
	{
		pItem = (CUIListItem*)m_pListAI->GetListItem(i);

		if (pItem == NULL)
			continue;

		switch(pIcon->getBtnType())
		{
		case UBET_ITEM:
			{
				pIconTemp = (CUIIcon*)pItem->findUI("icon_ai");
			}
			break;
		case UBET_SKILL:
		case UBET_ACTION:
			{
				pIconTemp = (CUIIcon*)pItem->findUI("icon_action");
			}
			break;
		}

		if (pIconTemp && (pIconTemp->getIndex() == pIcon->getIndex()))
		{
			pIconTemp->clearIconData();
			break;
		}
	}	
}

void CWildPetInfoUI::AddAIData(int nSlotnum, sWildPet_AI_Slot sAIData)
{
	m_sAISlot[nSlotnum] = sAIData;
}

void CWildPetInfoUI::AIClear()
{
	for (int nslot = 0; nslot < WILDPET_AI_MAXSLOT; nslot++)
	{
		m_sAISlot[nslot].m_bActive = FALSE;
		m_sAISlot[nslot].m_nSillType = -1;
		m_sAISlot[nslot].m_nActionSlot = -1;
		m_sAISlot[nslot].m_nTargetSlot = -1;
	}

	updateAISetUI();
	PetAIReSet();
}

void CWildPetInfoUI::OpenAISetUI()
{
	if (m_pImgAISetUIBack != NULL)
	{
		BOOL bHide = m_pImgAISetUIBack->GetHide();
		m_pImgAISetUIBack->Hide(!bHide);

		PetAIReSet();
	}
}

void CWildPetInfoUI::CloseAISetUI()
{
	if (m_pImgAISetUIBack != NULL)
		m_pImgAISetUIBack->Hide(TRUE);
}

void CWildPetInfoUI::ErrorMessage(UBYTE errorcode)
{
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;

	switch(errorcode)
	{
	case MSG_APET_ERROR_OK:
		{
			//	strMessage = _S(1970, "성공");
			PetAIReSet();
			CloseAISetUI();
			return;
		}break;
	case MSG_APET_ERROR:
		{
			strMessage = _S(1971, "실패");
		}break;
	case MSG_APET_ERR_NOT_HAVE_ITEM:
		{
			strMessage = _S(2907, "이용 가능한 아이템이 아닙니다.");

		}break;
	case MSG_APET_ERR_WRONG_AI:
		{
			strMessage = _S(318, "타겟이 잘못되었습니다.");

		}break;
	}

	PetAIReSet();

	MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인"), UMBS_OK, UI_WILDPET_INFO, UI_NONE );
	MsgBoxInfo.AddString( strMessage );
	CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
}

BOOL CWildPetInfoUI::CheckAIItem( CItemData* pAIItem, int nSlot )
{
	if (pAIItem == NULL || m_pIconAIAction[nSlot] == NULL || m_pIconAIAction[nSlot]->GetHide() == TRUE)
		return FALSE;

	if (m_pIconAIAction[nSlot]->IsEmpty() != false)
		return TRUE;

	int nTargetType = pAIItem->GetPetAISet3();
	int nActionType = GetActionAIType(m_pIconAIAction[nSlot]);

	return CheckAIType(nTargetType, nActionType);
}

BOOL CWildPetInfoUI::CheckAIAction( CUIIcon* pIcon, int nSlot )
{
	if (pIcon == NULL || m_pIconAITarget[nSlot] == NULL || m_pIconAITarget[nSlot]->GetHide() == TRUE)
		return FALSE;

	if (m_pIconAITarget[nSlot]->IsEmpty() != false)
		return TRUE;

	CItemData* pCurItem = _pNetwork->GetItemData(m_pIconAITarget[nSlot]->getIndex());

	int nTargetType = pCurItem->GetPetAISet3();
	int nActionType = GetActionAIType(pIcon);

	return CheckAIType(nTargetType, nActionType);
}

BOOL CWildPetInfoUI::CheckAIType( int nAIType, int nActionType )
{
	switch (nAIType)
	{
	case CItemData::PET_AI_OWNER_ATTACK_ENEMY:// 주인이 공격하는 적
	case CItemData::PET_AI_ENEMY_ATTACK_OWNER:// 주인을 공격하는 적
	case CItemData::PET_AI_ENEMY_ATTACK_PET:// 펫을 공격하는 적
	case CItemData::PET_AI_ENEMY:// 적
		{
			if (nActionType == AI_TARGET_ENEMY)
				return TRUE;
		}
		break;
	case CItemData::PET_AI_PET:// 펫
		{
			if (nActionType == AI_TARGET_PET)
				return TRUE;
		}
		break;
	case CItemData::PET_AI_OWNER:// 주인
		{
			if (nActionType == AI_TARGET_OWNER)
				return TRUE;
		}
		break;
	case CItemData::PET_AI_ITEM:// 아이템
		{
			if (nActionType == AI_TARGET_ITEM)
				return TRUE;
		}
		break;
	}

	return FALSE;
}

int CWildPetInfoUI::GetActionAIType( CUIIcon* pIcon )
{
	if (pIcon == NULL)
		return AI_NONE;

	if(pIcon->getBtnType() == UBET_SKILL)
	{
		CSkill SkillData = _pNetwork->GetSkillData(pIcon->getIndex());

		if(SkillData.IsNeedTarget() == TRUE)
		{
			BOOL bHelp = SkillData.GetFlag() & SF_FORHELP;

			if (bHelp != FALSE)
				return AI_TARGET_OWNER;
			else
				return AI_TARGET_ENEMY;
		}
		else
		{
			return AI_TARGET_PET;
		}
	}
	else if (pIcon->getBtnType() == UBET_ACTION)
	{
		CAction* curAction =	CAction::getData(pIcon->getIndex());

		if (curAction == NULL)
			return AI_NONE;

		switch(curAction->GetIndex())
		{
		case 44/*[P2]공격*/:
			return AI_TARGET_ENEMY;
		case 45/*[P2]줍기*/:
			return AI_TARGET_ITEM;
		}
	}

	return AI_NONE;
}

BOOL CWildPetInfoUI::IsWildPetCoolTime()
{
	sPetItem_Info temPetitem;
	GetWildPetInfo(MY_APET_INFO()->m_nIdxServer, temPetitem);

	if (temPetitem.pet_cooltime > 0)
	{
		return TRUE;
	}
	return FALSE;
}

WMSG_RESULT CWildPetInfoUI::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
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

WMSG_RESULT CWildPetInfoUI::OnLButtonDown( UINT16 x, UINT16 y )
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

	if (m_pImgAISetUIBack && m_pImgAISetUIBack->GetHide() != TRUE && m_pImgAISetUIBack->IsInside(x, y))
	{
		int nSlotnum = 0;
		for ( nSlotnum = 0; nSlotnum < m_nAISlotCount; nSlotnum++)
		{
			if (m_pIconAITarget[nSlotnum] && m_pIconAITarget[nSlotnum]->IsInside(x, y))
			{
				if (m_pIconAITarget[nSlotnum]->IsEmpty() == false)
				{
					m_pIconAITarget[nSlotnum]->clearIconData();
					return WMSG_SUCCESS;
				}				
			}
			else if (m_pIconAIAction[nSlotnum] && m_pIconAIAction[nSlotnum]->IsInside(x, y))
			{
				if (m_pIconAIAction[nSlotnum]->IsEmpty() == false)
				{
					m_pIconAIAction[nSlotnum]->clearIconData();
					return WMSG_SUCCESS;
				}
			}
		}
	}

	// 잠겨있을 경우 child 로 메세지 전달 하지 않음.
	if (UIMGR()->GetInventory()->getLocked() == LOCK_PET_ITEM_UPGRADE)
		return WMSG_SUCCESS;

	CUIManager::getSingleton()->RearrangeOrder( UI_WILDPET_INFO, TRUE );
	return WMSG_FAIL;
}

WMSG_RESULT CWildPetInfoUI::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_bDrag = false;

	CUIManager* pUIManager = CUIManager::getSingleton();

	CUIIcon* pDrag = pUIManager->GetDragIcon();

	if (pDrag != NULL)
	{
		// 검사 범위가 좁기 때문에 먼저 검사한다.
		// check upgrade
		if (m_pInvenBack != NULL && m_pInvenBack->IsInside(x, y))
		{
			for (int i = 0; i < WILDPET_WEAR_TOTAL; ++i)
			{
				if (m_pIcon[i] != NULL && m_pIcon[i]->IsInside(x, y) == TRUE)
				{
					bool bRet = check_upgrade(i);

					if (bRet == true)
					{
						pUIManager->ResetHoldBtn();
						return WMSG_SUCCESS;
					}

					break;
				}
			}
		}		
		
		// check main windows
		if (m_pImgBack && m_pImgBack->IsInside(x, y))
		{

			if (m_pChkTabBtn[eTAB_BASE] && m_pChkTabBtn[eTAB_BASE]->IsChecked() == TRUE)
			{
				if (pDrag->getBtnType() == UBET_ITEM &&
					pDrag->GetWhichUI() == UI_INVENTORY)
				{
					CItems* pItems = pDrag->getItems();

					if (pItems == NULL)
						return WMSG_FAIL;

					CItemData* ptemItem = pItems->ItemData;
					_pNetwork->SendUseWildPetItem(ptemItem->GetWearingPosition(), 
						pItems->Item_Tab, pItems->InvenIndex, pItems->Item_Index);
				}
			}
		}
		// check ai window
		else if (m_pImgAISetUIBack && m_pImgAISetUIBack->GetHide() != TRUE && m_pImgAISetUIBack->IsInside(x, y))
		{
			int nSlotnum = 0;

			for ( nSlotnum = 0; nSlotnum < m_nAISlotCount; nSlotnum++)
			{
				if (m_pIconAITarget[nSlotnum] && m_pIconAITarget[nSlotnum]->IsInside(x, y))
				{
					if (m_pIconAITarget[nSlotnum]->IsEmpty() == false && 
						m_pIconAITarget[nSlotnum]->getIndex() == pDrag->getIndex())
					{
						pUIManager->ResetHoldBtn();
						return WMSG_SUCCESS;
					}

					if (pDrag->getBtnType() == UBET_ITEM &&
						pDrag->GetWhichUI() == UI_INVENTORY )
					{
						CItems* pItems = pDrag->getItems();

						if (pItems == NULL)
							return WMSG_FAIL;

						CItemData* pCurItem = pItems->ItemData;

						if (pCurItem->GetSubType() == CItemData::ITEM_PET_AI)
						{
							if (CheckAIItem(pCurItem, nSlotnum) == TRUE)
								m_pIconAITarget[nSlotnum]->setData(UBET_ITEM, pDrag->getIndex());
							else
								ErrorMessage(MSG_APET_ERR_WRONG_AI);
							break;
						}								
					}
				}
				else if (m_pIconAIAction[nSlotnum] && m_pIconAIAction[nSlotnum]->IsInside(x, y))
				{
					if (m_pIconAIAction[nSlotnum]->IsEmpty() == false &&
						m_pIconAIAction[nSlotnum]->getIndex() == pDrag->getIndex())
					{
						pUIManager->ResetHoldBtn();
						return WMSG_SUCCESS;
					}

					if (pDrag->getBtnType() == UBET_SKILL &&
						pDrag->GetWhichUI() == UI_WILDPET_INFO )
					{
						if ( CheckAIAction(pDrag, nSlotnum) == TRUE)
							m_pIconAIAction[nSlotnum]->setData(UBET_SKILL, pDrag->getIndex());
						else
							ErrorMessage(MSG_APET_ERR_WRONG_AI);
						break;
					}
					else if (pDrag->getBtnType() == UBET_ACTION &&
						pDrag->GetWhichUI() == UI_CHARACTERINFO )
					{
						CAction* curAction = CAction::getData(pDrag->getIndex());

						if (curAction == NULL)
							break;

						if (curAction->GetType() == 5)
						{
							if ( CheckAIAction(pDrag, nSlotnum) == TRUE)
								m_pIconAIAction[nSlotnum]->setData(UBET_ACTION, pUIManager->GetDragIcon()->getIndex());
							else
								ErrorMessage(MSG_APET_ERR_WRONG_AI);
							break;
						}								
					}
				}
			}
		}
	}
	pUIManager->ResetHoldBtn();
	return WMSG_FAIL;
}

void CWildPetInfoUI::clear_upgradeinfo()
{
	m_nUpgradeSelidx = -1;
	m_nUpgradeTab = -1;
	m_nUpgradeInvenIdx = -1;
}

void CWildPetInfoUI::DeleteEquip( ULONG idx, INDEX item_idx )
{
	ObjInfo* pInfo = ObjInfo::getSingleton();

	if (pInfo->GetMyApetInfo() != NULL)
	{
		if (pInfo->GetMyApetInfo()->bIsActive == FALSE)
		{
			return;
		}
		
		if (pInfo->GetMyApetInfo()->GetEntity() != NULL && item_idx >= 0)
			_pGameState->TakeOffArmorTest(pInfo->GetMyApetInfo()->GetEntity()->GetModelInstance(), item_idx);

		pInfo->GetMyApetInfo()->m_nPetWearIndex[idx] = -1;

		// _WildPetInfo는 실제 와일드 펫정보가 아니다.(_pNetwork->ga_srvServer.srv_actWildPet의 펫정보를 직접 바꾸어야 한다.)
		// _WildPetInfo변수가 포인터 변수가 아니다. &_WildPetInfo != &_pNetwork->ga_srvServer.srv_actWildPet
		if (pInfo->GetMyApetInfo()->GetEntity() != NULL)
			pInfo->GetMyApetInfo()->GetEntity()->en_pWildPetTarget->m_nPetWearIndex[idx] = -1;
	}

	PetWearItemReSet();
}

void CWildPetInfoUI::OnUpdate( float fDeltaTime, ULONG ElapsedTime )
{
	_updateSkillIconCoolTime();
	_updateAISkillIconCoolTime();
	_updateExpPetCooltime();
}

void CWildPetInfoUI::_updateSkillIconCoolTime()
{
	if (m_pListSkill == NULL)
		return;

	int nCount = m_vecSkills.size();

	if (nCount <= 0)
		return;

	CUIListItem* pItem = NULL;
	CUIIcon* pIcon = NULL;

	for (int i = 0; i < nCount; ++i)
	{
		pItem = (CUIListItem*)m_pListSkill->GetListItem(i);

		if (pItem == NULL)
			continue;

		CSkill&	rSkill = _pNetwork->GetSkillData(m_vecSkills[i]);
		BOOL	bDelay = MY_INFO()->GetPetSkillDelay(0, m_vecSkills[i]);

		if (bDelay == FALSE)
			continue;

		pIcon = (CUIIcon*)pItem->findUI("icon_skill");

		if (pIcon != NULL && pIcon->getIndex() == m_vecSkills[i])
			pIcon->setCooltime(true);
	}
}

void CWildPetInfoUI::_updateAISkillIconCoolTime()
{
	int nIndex;
	BOOL bDelay;

	for (int i = 0; i < WILDPET_AI_MAXSLOT; ++i)
	{
		if (m_pIconAIAction[i] == NULL || m_pIconAIAction[i]->GetHide() == TRUE || m_pIconAIAction[i]->IsEmpty() == true)
			continue;

		nIndex = m_pIconAIAction[i]->getIndex();
		bDelay = MY_INFO()->GetPetSkillDelay(0, nIndex);

		if (bDelay)
			m_pIconAIAction[i]->setCooltime(true);
	}
}

void CWildPetInfoUI::_updateExpPetCooltime()
{
	ObjInfo* pInfo = ObjInfo::getSingleton();

	if (pInfo == NULL)
		return;

	CWildPetTarget* pMyWildPet = pInfo->GetMyApetInfo();

	if (pMyWildPet == NULL)
		return;

	CTString strTemp;
	sPetItem_Info temPetitem;
	GetWildPetInfo(pMyWildPet->m_nIdxServer, temPetitem);

	if(IsWildPetCoolTime())
	{
		if (m_pBtnExpUse != NULL)
			m_pBtnExpUse->SetEnable(FALSE);

		if (m_pTextExp[eEXP_STRING_PET_COOLTIME_TITLE] != NULL)
			m_pTextExp[eEXP_STRING_PET_COOLTIME_TITLE]->Hide(FALSE);
		
		INDEX temCooltime = temPetitem.pet_cooltime - (unsigned int)_pTimer->GetLerpedCurrentTick();
		int iSec = temCooltime % 60;
		temCooltime /= 60;
		int iMin = temCooltime % 60;
		int iHour = temCooltime /= 60;
		CTString temtime;
		strTemp.Clear();

		if (iHour > 0)
		{
			temtime.PrintF(_S(2512,"%d시간"), iHour);
			strTemp += temtime;
		}
		if (iMin > 0)
		{
			temtime.PrintF(_S(2513, "%d분"), iMin);
			strTemp += temtime;
		}
		if (iSec > 0)
		{
			temtime.PrintF(_S(2514,"%d초"), iSec);
			strTemp += temtime;
		}

		if (m_pTextExp[eEXP_STRING_PET_COOLTIME] != NULL)
		{
			m_pTextExp[eEXP_STRING_PET_COOLTIME]->Hide(FALSE);
			m_pTextExp[eEXP_STRING_PET_COOLTIME]->SetText(strTemp);
		}
	}
	else
	{
		if (m_pTextExp[eEXP_STRING_PET_COOLTIME_TITLE] != NULL)
			m_pTextExp[eEXP_STRING_PET_COOLTIME_TITLE]->Hide(TRUE);

		if (m_pTextExp[eEXP_STRING_PET_COOLTIME] != NULL)
			m_pTextExp[eEXP_STRING_PET_COOLTIME]->Hide(TRUE);
		
		if(m_pBtnExpUse != NULL)
		{
			if (temPetitem.pet_accexp > 0)
				m_pBtnExpUse->SetEnable(TRUE);
			else
				m_pBtnExpUse->SetEnable(FALSE);
		}		
	}
}

///////////////// network//////////////////////
void CWildPetInfoUI::SendWildPetPointUP(UBYTE ubPoint)
{
	CNetworkMessage nmMessage(MSG_EXTEND);

	nmMessage << (ULONG)MSG_EX_ATTACK_PET;
	nmMessage << (SBYTE)MSG_SUB_STATUP;
	nmMessage << (UBYTE)ubPoint;

	_pNetwork->SendToServerNew(nmMessage);
}

void CWildPetInfoUI::SendAIList()
{
	if (m_nAISlotCount <= 0)
	{
		return;
	}

	int temSlotcount = 0;
	INDEX nSlot;
	for( nSlot = 0; nSlot < m_nAISlotCount; nSlot++)
	{
		if (m_pIconAITarget[nSlot] == NULL || m_pIconAIAction[nSlot] == NULL)
			continue;

		if (m_pIconAITarget[nSlot]->IsEmpty() == false && 
			m_pIconAIAction[nSlot]->IsEmpty() == false)
		{
			temSlotcount++;
		}
	}

	CNetworkMessage nmMessage(MSG_EXTEND);

	nmMessage << (ULONG)MSG_EX_ATTACK_PET;
	nmMessage << (SBYTE)MSG_SUB_AI_UPDATE_REQ;
	nmMessage << (LONG)m_nAISlotCount;
	nmMessage << (INDEX)temSlotcount;

	for( nSlot = 0; nSlot < m_nAISlotCount; nSlot++)
	{
		if (m_pIconAITarget[nSlot] == NULL || m_pIconAIAction[nSlot] == NULL)
			continue;

		if (m_pIconAITarget[nSlot]->IsEmpty() == true && 
			m_pIconAIAction[nSlot]->IsEmpty() == true)
			continue;

		nmMessage << nSlot;
		nmMessage << (ULONG)m_pIconAITarget[nSlot]->getIndex();


		if (m_pIconAIAction[nSlot]->getBtnType() == UBET_SKILL ||
			m_pIconAIAction[nSlot]->getBtnType() == UBET_ACTION)
		{
			nmMessage << (ULONG)m_pIconAIAction[nSlot]->getBtnType();
			nmMessage << (ULONG)m_pIconAIAction[nSlot]->getIndex();
		}
		else
		{
			nmMessage << (ULONG) -1;
			nmMessage << (ULONG) -1;
		}

		UBYTE bCheckd = FALSE;

		if (m_pchkAISlotActive[nSlot] && m_pchkAISlotActive[nSlot]->IsChecked())
			bCheckd = TRUE;

		nmMessage << bCheckd;
	}


	_pNetwork->SendToServerNew(nmMessage);
}

void CWildPetInfoUI::SendAIActive()
{
	if (m_pChkAISet == NULL)
		return;

	CNetworkMessage nmMessage(MSG_EXTEND);

	m_bAIActive = m_pChkAISet->IsChecked();

	SBYTE bActive = m_bAIActive;
	nmMessage << (ULONG)MSG_EX_ATTACK_PET;
	nmMessage << (SBYTE)MSG_SUB_AI_ONOFF_REQ;
	nmMessage << bActive;

	_pNetwork->SendToServerNew(nmMessage);
}

void CWildPetInfoUI::SendUpgradeReq()
{
	if (m_nUpgradeSelidx < 0 || m_nUpgradeSelidx >= WILDPET_WEAR_TOTAL)
		return;

	if (m_pIcon[m_nUpgradeSelidx]->IsEmpty() == true)
		return;

	CItems* pItems = m_pIcon[m_nUpgradeSelidx]->getItems();

	if (pItems == NULL)
		return;

	CNetworkMessage nmMessage;
	RequestClient::doItemUpgradePet* packet = reinterpret_cast<RequestClient::doItemUpgradePet*>(nmMessage.nm_pubMessage);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_UPGRADE_PET;
	packet->wearPos = m_nUpgradeSelidx;
	packet->tab = m_nUpgradeTab;
	packet->inven_index = m_nUpgradeInvenIdx;

	nmMessage.setSize( sizeof(*packet) );

	_pNetwork->SendToServerNew(nmMessage);

	clear_upgradeinfo();
}

void CWildPetInfoUI::ReceiveWearItemData(CNetworkMessage *istr)
{
	SBYTE nWearpos;
	ULONG nItemIndex, nItemPlus, nItemPlus2 = 0;
	ULONG nFlag,nUsed,nUsed2;
	SBYTE	sbOptionCount;
	SBYTE	sbOptionType;
	LONG	lOptionLevel;

	(*istr) >> nWearpos;
	(*istr) >> nItemIndex;
	(*istr) >> nItemPlus;
	(*istr) >> nFlag;
	(*istr) >> nUsed;
	(*istr) >> nUsed2;
	(*istr) >> sbOptionCount;

	CItemData* pItemData = _pNetwork->GetItemData(nItemIndex);

	CItems* pItems = new CItems;
	pItems->Item_Index = nItemIndex;
	pItems->Item_Plus = nItemPlus;
	pItems->Item_Wearing = nWearpos;
	pItems->Item_Used = nUsed;
	pItems->Item_Used2 = nUsed2;
	pItems->Item_Flag = nFlag;

	pItems->ItemData = CItemData::getData(nItemIndex);

	//레어 아이템일때...
	if( pItemData->GetFlag() & ITEM_FLAG_RARE )
	{
		//옵션 개수가 0이면 미감정 레어아이템
		if( sbOptionCount ==0)
		{
			pItems->SetRareIndex(0);
		}		
		else
		{
			//감정된 레어아이템이면...
			CUIManager::getSingleton()->SetRareOption(istr, *pItems);
		}
	}
	//레어 아이템이 아니면.....
	else
	{
		LONG lOriginOptionVar = ORIGIN_VAR_DEFAULT;

		for( SBYTE sbOption = 0; sbOption < sbOptionCount; sbOption++ )
		{
			(*istr) >> sbOptionType;
			(*istr) >> lOptionLevel;

			if ( pItemData->GetFlag() & ITEM_FLAG_ORIGIN )
			{
				(*istr) >> lOriginOptionVar;
			}

			pItems->SetOptionData(sbOption, sbOptionType, lOptionLevel, lOriginOptionVar);
		}
	}

	if ( pItemData->GetFlag() & ITEM_FLAG_ORIGIN)
	{
		SBYTE sbBelong, sbSkillcont;
		LONG lSkillIndex;
		SBYTE sbSkillLevel;

		(*istr) >> sbBelong;
		(*istr) >> sbSkillcont;

		pItems->Item_Belong = sbBelong;

		for (SBYTE sbSkillpos = 0; sbSkillpos < sbSkillcont; sbSkillpos++)
		{
			(*istr) >> lSkillIndex;
			(*istr) >> sbSkillLevel;

			pItems->SetItemSkill(sbSkillpos, lSkillIndex, sbSkillLevel);
		}

	}

	(*istr) >> nItemPlus2;
	pItems->SetItemPlus2(nItemPlus2);

	if (m_pIcon[nWearpos] != NULL)
		m_pIcon[nWearpos]->setData(pItems, false);
}

void CWildPetInfoUI::ReceiveDeleteEquip( CNetworkMessage *istr )
{
	CTString strPetname,strmassage;
	INDEX nItem_Index;
	SBYTE wear_pos;
	
	CUIManager* pUIManager = UIMGR();	

	(*istr) >> strPetname;
	(*istr) >> nItem_Index;
	(*istr) >> wear_pos;

	strmassage.PrintF(_S(4213, "%s 의 %s 아이템이 기간 만료로 사라집니다." ),strPetname,_pNetwork->GetItemName(nItem_Index));
	pUIManager->GetChattingUI()->AddSysMessage( strmassage, SYSMSG_NORMAL );

	DeleteEquip(wear_pos, nItem_Index);
}

void CWildPetInfoUI::ExpUse()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if (pUIManager->DoesMessageBoxExist(MSGCMD_USE_PET_ACCUMULATE))
		return;

	if (pUIManager->GetInventory()->IsLocked() == TRUE ||
		pUIManager->GetInventory()->IsLockedArrange() == TRUE)
	{
		pUIManager->GetInventory()->ShowLockErrorMessage();
		return;
	}

	pUIManager->GetInventory()->Lock(TRUE, TRUE, LOCK_EXP_PET);

	CUIMsgBox_Info	MsgBoxInfo;
	CTString strMsg;

	MsgBoxInfo.SetMsgBoxInfo(_S(5632, "경험치 사용"),UMBS_SELECTBOX,UI_NONE,MSGCMD_USE_PET_ACCUMULATE);
	strMsg.PrintF(_S(5633, "축척된 경험치를 사용 하시겠습니까?"));
	MsgBoxInfo.AddString(strMsg);
	strMsg.PrintF(_S(5634, "(축적된 경험치 만큼 경험치를 획득하게 됩니다. 단, 재사용시 4시간의 대기시간이 필요합니다.)"));
	MsgBoxInfo.AddString(strMsg);

	pUIManager->CreateMessageBox(MsgBoxInfo);
}

void CWildPetInfoUI::changeMainTab()
{
	COLOR colText[2];
	colText[0] = 0xAAAAAAFF; // 비활성화 FONT COLOR
	colText[1] = 0xF2F2F2FF; // 활성화 FONT COLOR

	int nChked = 0;
	for (int i = 0; i < eTAB_MAX; ++i)
	{
		if (m_pChkTabBtn[i] != NULL)
			nChked = m_pChkTabBtn[i]->IsChecked();

		if (m_pTextChk[i] != NULL)
			m_pTextChk[i]->setFontColor(colText[nChked]);
	}
}

bool CWildPetInfoUI::check_upgrade( int idx )
{
	CUIManager* pUIMgr = UIMGR();

	if (idx < 0 || idx >= WILDPET_WEAR_TOTAL)
		return false;

	if (m_pIcon[idx] == NULL || m_pIcon[idx]->IsEmpty() == true)
		return false;

	CUIIcon* pDrag = pUIMgr->GetDragIcon();

	if (pDrag == NULL)
		return false;

	// Drap Item 이 제련석인지 검사
	CItems* pItems = pDrag->getItems();

	if (pItems == NULL)
		return false;

	CItemData* pData = pItems->ItemData;

	if (pData == NULL)
		return false;

	if (pData->GetType() != CItemData::ITEM_ETC || pData->GetSubType() != CItemData::ITEM_ETC_REFINE)
	{
		UICHAT()->AddSysMessage(_S(170, "업그레이드 가능 아이템이 아닙니다."));
		return false;
	}

	// 펫의 무기와 방어구에 인첸트는 [고급 제련석, 행운의 고급제련석, 초고급제련석, 카오스 제련석] 만 사용 가능합니다.
	if (pData->GetRefineType() != CItemData::REFINE_SPECIAL &&
		pData->GetRefineType() != CItemData::REFINE_SUPER_SPECIAL &&
		pData->GetRefineType() != CItemData::REFINE_LUCKY &&
		pData->GetRefineType() != CItemData::REFINE_CHAOS)
	{
		UICHAT()->AddSysMessage(_S(170, "업그레이드 가능 아이템이 아닙니다."));
		return false;
	}

	CItems* pPetItems = m_pIcon[idx]->getItems();
	CItemData* pPetItemData;

	if (pPetItems == NULL)
		return false;

	pPetItemData = pPetItems->ItemData;

	if (pPetItemData == NULL)
		return false;

	if (pUIMgr->DoesMessageBoxExist(MSGCMD_USE_PET_ITEM_UPGRADE))
		return false;

	if (pUIMgr->DoesMessageBoxExist(MSGCMD_USE_PET_ITEM_UPGRADE_INFO))
		pUIMgr->CloseMessageBox(MSGCMD_USE_PET_ITEM_UPGRADE_INFO);

	if (pUIMgr->GetInventory()->IsLocked() == TRUE ||
		pUIMgr->GetInventory()->IsLockedArrange() == TRUE)
	{
		pUIMgr->GetInventory()->ShowLockErrorMessage();
		return false;
	}

	m_nUpgradeSelidx = idx;
	m_nUpgradeTab = pItems->Item_Tab;
	m_nUpgradeInvenIdx = pItems->InvenIndex;	

	CUIMsgBox_Info	MsgBoxInfo;
	CTString strTitle, strMsg;
	DWORD style;
	bool bLock = false;
	int nCmdCode = MSGCMD_USE_PET_ITEM_UPGRADE;
	ULONG nPlus = pPetItems->GetItemPlus();
	const char* szItemName = _pNetwork->GetItemName(pPetItemData->GetItemIndex());

	switch (pData->GetRefineType())
	{
	case  CItemData::REFINE_SPECIAL:
	case  CItemData::REFINE_LUCKY:
		{
			if (nPlus >= DEF_PET_ITEM_UPGRADE_MAX)
			{
				strMsg.PrintF("%s", _S(2642, "더이상 업그레이드 할 수 없습니다."));
				pUIMgr->GetChattingUI()->AddSysMessage(strMsg, SYSMSG_ERROR);

				// 장비 장착으로 메세지 전달하지 않음. (true)
				return true;
			}

			strTitle = _S(175, "아이템 업그레이드");

			style = UMBS_YESNO;

			// +3 ~
			if( nPlus > 2 )
			{
				strMsg.PrintF( _S(172, "[%s +%d]<를> 업그레이드 하시겠습니까? 실패하면 아이템이 파괴될 수 있습니다."), szItemName, nPlus);
			}
			// 0 ~ +2
			else if( nPlus > 0 )
			{
				strMsg.PrintF( _S(173, "[%s +%d]<를> 업그레이드 하시겠습니까? 실패하면 능력치가 상승하지 않습니다."), szItemName, nPlus);
			}
			// 0
			else 
			{
				strMsg.PrintF( _S(174, "[%s]<를> 업그레이드 하시겠습니까? 실패하면 능력치가 상승하지 않습니다."), szItemName);
			}


			strMsg.PrintF(_S(6513, "[%s +%d]<를> 업그레이드 하시겠습니까? 실패하면 아이템이 파괴될 수 있습니다."), 
				_pNetwork->GetItemName(pPetItems->Item_Index), pPetItems->GetItemPlus());
			style = UMBS_YESNO;
			bLock = true;
		}
		break;
	case  CItemData::REFINE_SUPER_SPECIAL:
		{
			if(nPlus >= 14)
			{
				strTitle.PrintF(_S( 1670, "주의!!!" ));
				strMsg.PrintF(_S( 1671, "초 고급 제련석은 +14이상의 장비에는\n사용할 수 없습니다." ));
				style = UMBS_OK;
				nCmdCode = MSGCMD_USE_PET_ITEM_UPGRADE_INFO;
			}
			else if (pItems->IsFlag(FLAG_ITEM_SUPER_STONE_USED) == FALSE)
			{
				strTitle.PrintF(_S( 1670, "주의!!!" ));
				strMsg.PrintF(_S( 1672, "초 고급 제련석는 하나의 장비에\n한번만 사용할 수 있습니다.\n정말 사용하시겠습니까?" ));
				style = UMBS_YESNO;
				bLock = true;
			}
			else
			{
				strTitle.PrintF(_S( 1673, "제련 불가!!!" ));
				strMsg.PrintF(_S( 1674, "초 고급 제련석을 이미\n사용한 장비입니다.\n다른 아이템에 사용해 주십시요." ));
				style = UMBS_OK;
				nCmdCode = MSGCMD_USE_PET_ITEM_UPGRADE_INFO;
			}
		}
		break;	
	case  CItemData::REFINE_CHAOS:
		{
			if (nPlus > 11)
			{
				strTitle.PrintF(_S( 1670, "주의!!!" ));
				strMsg.PrintF(_S(4134, "카오스 제련석은 +12이상의 장비에는 사용할 수 없습니다." ) );
				style = UMBS_OK;
				nCmdCode = MSGCMD_USE_PET_ITEM_UPGRADE_INFO;
			}
			else
			{
				CTString strItemname;
				
				if (nPlus > 0)
					strItemname.PrintF("[%s +%d]", _pNetwork->GetItemName(pPetItems->Item_Index), nPlus);
				else
					strItemname.PrintF("[%s]", _pNetwork->GetItemName(pPetItems->Item_Index));

				strTitle.PrintF(_S(175, "아이템 업그레이드"));
				strMsg.PrintF( _S(4135, "%s<를> 카오스 제련석을 사용하여 업그레이드 하시겠습니까?" ), strItemname);
				style = UMBS_OKCANCEL;
				bLock = true;
			}
		}
		break;
	}


	MsgBoxInfo.SetMsgBoxInfo(strTitle, style, UI_NONE, nCmdCode);
	
	MsgBoxInfo.AddString(strMsg);

	pUIMgr->CreateMessageBox(MsgBoxInfo);

	if (bLock == true)
		pUIMgr->GetInventory()->Lock(TRUE, TRUE, LOCK_PET_ITEM_UPGRADE);

	return true;
}


#include "stdh.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "SSkillLearnUI.h"
#include <Engine/Contents/Base/UICharacterInfoNew.h>
#include <Engine/Contents/function/SummonUI.h>
#include <Engine/Info/MyInfo.h>
#define SSKILLLEARN_NEW_SLOT_ROW_TOTAL 50

// enum eSSkillSatisfiedList	//스킬을 배울때 각 상황의 불만족을 체크하는 플래그
// {
// 	ALL_SATISFIED =				0X00000000,
// 	NOT_SATISFIED_LEVEL =		0x00000001, 
// 	NOT_SATISFIED_SP =			0X00000002,
// 	NOT_SATISFIED_STR =			0X00000004,
// 	NOT_SATISFIED_DEX =			0X00000008,
// 	NOT_SATISFIED_INT =			0X00000010,
// 	NOT_SATISFIED_CON =			0X00000020,
// 	NOT_SATISFIED_SKILL_0 =		0X00000040,	// 필요한 스킬은 최대 3개로 제한 되어 있다.
// 	NOT_SATISFIED_SKILL_1 =		0X00000080,
// 	NOT_SATISFIED_SKILL_2 =		0X00000100,
// 	NOT_SATISFIED_ITEM_0 =		0X00000200, // 필요한 아이템은 최대 3개로 제한되어 있다.
// 	NOT_SATISFIED_ITEM_1 =		0X00000400,
// 	NOT_SATISFIED_ITEM_2 =		0X00000800,
// };

struct	sCollectSkill
{
	LONG	lSkillIndex;
	SBYTE	sbSkillLevel;
	WORD	sbNeedSkillLevel;

	sCollectSkill()
	{
		lSkillIndex		= -1;
		sbSkillLevel	= -1;
		sbNeedSkillLevel= -1;
	}

	bool operator<(const sCollectSkill &other) const
	{			
		if(sbNeedSkillLevel < other.sbNeedSkillLevel)		return true;
		else if(sbNeedSkillLevel > other.sbNeedSkillLevel)	return false;
		else												return false;
	}

	bool operator>(const sCollectSkill &other) const
	{			
		return other.operator < (*this);
	}

	void SetData(int nIndex, SBYTE sbLevel, SBYTE sbNeedLevel)
	{
		lSkillIndex			= nIndex;
		sbSkillLevel		= sbLevel;
		sbNeedSkillLevel	= sbNeedLevel;
	}
};

CSSkillLearnUI::CSSkillLearnUI()
	: m_pMoveArea(NULL)
	, m_pSkillList(NULL)
{
	int i;
	for(i = 0; i < eBTN_MAX; ++i)
		m_pBtn[i] = NULL;

	for(i = 0; i < eTEXT_MAX; ++i)
		m_pText[i] = NULL;

	setInherit(false);
}

CSSkillLearnUI::~CSSkillLearnUI()
{
}

void CSSkillLearnUI::BtnLock()
{
	if (m_pBtn[eBTN_OK] != NULL)
		m_pBtn[eBTN_OK]->SetEnable(FALSE);
}

void CSSkillLearnUI::BtnUnLock()
{
	if (m_pBtn[eBTN_OK] != NULL)
		m_pBtn[eBTN_OK]->SetEnable(TRUE);
}

void CSSkillLearnUI::SendLearnSSkill()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// Close message box of skill learn
	pUIManager->CloseMessageBox( MSGCMD_SKILLLEARN_NOTIFY );

	if( _pNetwork->MyCharacterInfo.job == SORCERER )
	{
		// 강신상태와 소환상태에서는 스킬을 배울수 없습니다.
		if( _pNetwork->MyCharacterInfo.nEvocationIndex > 0 )
		{
			// 강신상태이므로, 배울수 없음.
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 270, "스킬" ), UMBS_OK,
				UI_SKILLLEARN, MSGCMD_SKILLLEARN_NOTIFY );
			MsgBoxInfo.AddString( _S(2352,"강신 상태에서는 스킬을 배울수 없습니다.") );
			pUIManager->CreateMessageBox( MsgBoxInfo );
			return;
		}	

		// 소환상태에서는 스킬을 배울수 없음.
		for( int i = UI_SUMMON_START; i <= UI_SUMMON_END; ++i )
		{
			CSummonUI* pUISummon = (CSummonUI*)pUIManager->GetUI(i);
			if( pUISummon->GetSummonEntity() )
			{
				// Create message box of skill learn
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S( 270, "스킬" ), UMBS_OK,
					UI_SKILLLEARN, MSGCMD_SKILLLEARN_NOTIFY );
				MsgBoxInfo.AddString( _S(2353,"소환 상태에서는 스킬을 배울수 없습니다.") );
				pUIManager->CreateMessageBox( MsgBoxInfo );
				return;
			}
		}
	}

	if (m_pSkillList == NULL)
		return;

	int nSelIndex = m_pSkillList->getCurSel();

	if (nSelIndex < 0 || nSelIndex >= m_pSkillList->getListItemCount())
		return;

	CUIListItem* pItem = (CUIListItem*)m_pSkillList->GetListItem(nSelIndex);

	if (pItem == NULL)
		return;

	CUIIcon* pIcon = (CUIIcon*)pItem->findUI("icon_sskill");

	if (pIcon == NULL)
		return;

	SLONG	slIndex = pIcon->getIndex();
	_pNetwork->SendSSkillLearn( slIndex, m_iMobVirIdx );
}

void CSSkillLearnUI::openUI(int iMobIndex, int iMobVirIdx, FLOAT fX, FLOAT fZ)
{
	if (GetHide() == FALSE)
		return;

	Hide(FALSE);
	SetVisible(TRUE);

	m_fNpcX = fX;
	m_fNpcZ = fZ;
	m_iMobIdx = iMobIndex;
	m_iMobVirIdx = iMobVirIdx;

	_setSubTitle();
	updateList();

	UIMGR()->RearrangeOrder( UI_SSKILLLEARN, TRUE );
}

void CSSkillLearnUI::closeUI()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	Hide(TRUE);
	SetVisible(FALSE);
	resetUI();

	pUIManager->CloseMessageBox( MSGCMD_SKILLLEARN_NOTIFY );
	pUIManager->RearrangeOrder( UI_SSKILLLEARN, FALSE );
}

void CSSkillLearnUI::initialize()
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

	std::string strBtnID[eBTN_MAX] = { "btn_close", "btn_ok", "btn_cancel"};

	for(int i = 0; i < eBTN_MAX; ++i)
		m_pBtn[i] = (CUIButton*)findUI(strBtnID[i].c_str());

	if (m_pBtn[eBTN_CLOSE] != NULL)
		m_pBtn[eBTN_CLOSE]->SetCommandFUp(boost::bind(&CSSkillLearnUI::closeUI, this));

	if (m_pBtn[eBTN_CANCEL] != NULL)
		m_pBtn[eBTN_CANCEL]->SetCommandFUp(boost::bind(&CSSkillLearnUI::closeUI, this));

	if (m_pBtn[eBTN_OK] != NULL)
		m_pBtn[eBTN_OK]->SetCommandFUp(boost::bind(&CSSkillLearnUI::SendLearnSSkill, this));

	std::string strTextID[eTEXT_MAX] = { "text_subTitle", "text_skillpoint"};

	for(int i = 0; i < eTEXT_MAX; ++i)
		m_pText[i] = (CUIText*)findUI(strTextID[i].c_str());

	m_pSkillList = (CUIList*)findUI("list_skill");

	if (m_pSkillList != NULL)
		m_pSkillList->SetKeepSelect(true);
#endif	//	WORLD_EDITOR
}

WMSG_RESULT CSSkillLearnUI::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	UIMGR()->RearrangeOrder( UI_SSKILLLEARN, TRUE );
	return CUIWindow::OnLButtonDown(x, y);
}

WMSG_RESULT CSSkillLearnUI::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	CUIManager* pUIMgr = UIMGR();

	if (pUIMgr->GetDragIcon() == NULL || pUIMgr->GetDragIcon()->IsEmpty() == TRUE)
	{
		if (m_pSkillList != NULL && m_pSkillList->IsInside(x, y))
		{
			if (_CheckSatisfied() == true)
				BtnUnLock();
			else
				BtnLock();
		}
	}

	pUIMgr->ResetHoldBtn();
	return CUIWindow::OnLButtonUp(x, y);
}

void CSSkillLearnUI::updateList()
{
	if (m_pSkillList == NULL)
		return;

	CMobData* MD = CMobData::getData(m_iMobIdx);

	if (MD == NULL)
		return;

	int nSSKillType = MD->GetSpecialSkillMaster();

	INDEX	ctPosSpecial = 0;		// Possible
	INDEX	ctImposSpecial = 0;		// Impossible

	CUIManager* pUIManager = CUIManager::getSingleton();

	std::vector<sCollectSkill>		vectorPosSpecialSkill;
	std::vector<sCollectSkill>		vectorImposSpecialSkill;

	vectorPosSpecialSkill.resize(SSKILLLEARN_NEW_SLOT_ROW_TOTAL);		
	vectorImposSpecialSkill.resize(SSKILLLEARN_NEW_SLOT_ROW_TOTAL);

	CSpecialSkill::_map::iterator	iter = CSpecialSkill::_mapdata.begin();
	CSpecialSkill::_map::iterator	eiter = CSpecialSkill::_mapdata.end();

	for (;iter != eiter; ++iter)
	{
		CSpecialSkill* pSSkill = (*iter).second;

		if (pSSkill == NULL)
			continue;

		if( pSSkill->GetType() != nSSKillType)
			continue;

		// 순위 체크...
		const int iPreference = pSSkill->GetPreference();

		if(iPreference != -1)
		{
			int iIndex = pUIManager->GetCharacterInfo()->CheckSSkill( pSSkill->GetType() );
			if(iIndex != -1)
			{
				CSpecialSkill* pPrefer = CSpecialSkill::getData(iIndex);

				if (pPrefer == NULL)
					continue;

				if( iPreference < pPrefer->GetPreference() )
					continue;
			}
		}

		int		nSkillIndex		= pSSkill->GetIndex();
		SBYTE	sbSkillLevel	= MY_INFO()->GetSkillLevel(nSkillIndex, true);

		// If this skill is already max level
		if( sbSkillLevel >= pSSkill->GetMaxLevel() )
			continue;

		if( sbSkillLevel < 1 )
			sbSkillLevel = 0;

		ULONG	ulNeedCharLevel = pSSkill->GetLearnLevel( sbSkillLevel );
		sbSkillLevel++;

		// Special Skill
		if( ulNeedCharLevel <= _pNetwork->MyCharacterInfo.level )
			vectorPosSpecialSkill[ctPosSpecial++].SetData(nSkillIndex, sbSkillLevel, ulNeedCharLevel);
		else
			vectorImposSpecialSkill[ctImposSpecial++].SetData(nSkillIndex, sbSkillLevel, ulNeedCharLevel);	
	}

	std::sort(vectorPosSpecialSkill.begin(), vectorPosSpecialSkill.end());
	std::sort(vectorImposSpecialSkill.begin(), vectorImposSpecialSkill.end());

	int nOldSelIndex = m_pSkillList->getCurSel();
	m_pSkillList->DeleteAllListItem();
	m_vecSatisfied.clear();

	CUISkillLearn* pUISkilllearn = pUIManager->GetSkillLearn();

	if (pUISkilllearn == NULL)
		return;

	CUIListItem* pTemp = m_pSkillList->GetListItemTemplate();

	if (pTemp == NULL)
		return;

	CUIListItem* pItem = NULL;
	CTString strTemp;
	int iSkill = 0;
	int	iRow, nSatisfied; 

	CUIIcon* pIcon = NULL;
	CUIText* pText = NULL;
	COLOR color;
	int	nCharLevel	= _pNetwork->MyCharacterInfo.level;
	SQUAD nCharSP	= _pNetwork->MyCharacterInfo.sp;

	std::vector<sCollectSkill>* pVec[2] = { &vectorPosSpecialSkill, &vectorImposSpecialSkill };
	int nCount[2] = { ctPosSpecial, ctImposSpecial };

	for (int vecNum = 0; vecNum < 2; ++vecNum)
	{
		for( iRow = 0; iRow < nCount[vecNum]; iRow++ )
		{
			CSpecialSkill* pSSkill = CSpecialSkill::getData( (*pVec[vecNum])[iRow].lSkillIndex );

			if (pSSkill == NULL)
				continue;

			m_pSkillList->AddListItem(pTemp->Clone());
			pItem = (CUIListItem*)m_pSkillList->GetListItem(iSkill);

			if (pItem == NULL)
				break;

			int nIndex = pSSkill->GetIndex();
			SBYTE	sbLevel = MY_INFO()->GetSkillLevel(nIndex, true);
			int		nNeedLevel = pSSkill->GetLearnLevel( sbLevel );
			int		nlearnSp = pSSkill->GetLearnSP( sbLevel );
			sbLevel += 1;

			pIcon = (CUIIcon*)pItem->findUI("icon_sskill");

			if (pIcon != NULL)
			{
				pIcon->SetWhichUI(UI_SSKILLLEARN);
				pIcon->setSkill(nIndex, true);
			}

			pText = (CUIText*)pItem->findUI("text_sskillname");

			if (pText != NULL)
			{
				strTemp.PrintF( "%s", pSSkill->GetName() );
				color = nCharLevel >= nNeedLevel ? 0xFFC672FF : 0xBCBCBCFF;
				pText->SetText(strTemp);
				pText->setFontColor(color);
			}

			pText = (CUIText*)pItem->findUI("text_info");

			if (pText != NULL)
			{
				strTemp.PrintF( "%s %2d   %s %2d",_S( 4414, "LV" ), sbLevel, _S( 4415, "SP" ), nlearnSp );
				pText->SetText(strTemp);
			}		

			nSatisfied = pUISkilllearn->IsSatisfiedSkill( (*pVec[vecNum])[iRow].lSkillIndex, (*pVec[vecNum])[iRow].sbSkillLevel, TRUE  );
			m_vecSatisfied.push_back(nSatisfied);
			iSkill++;
		}
	}

	if (nOldSelIndex >= 0 && nOldSelIndex < m_pSkillList->getListItemCount())
		m_pSkillList->setCurSel(nOldSelIndex);

	if (_CheckSatisfied() == false)
	{
		m_pSkillList->setCurSel(-1);
		BtnLock();
	}
	else
		BtnUnLock();

	m_pSkillList->UpdateList();	
}

void CSSkillLearnUI::resetUI()
{
	m_fNpcX = 0.0f;
	m_fNpcZ = 0.0f;
	m_iMobIdx = -1;
	m_iMobVirIdx = -1;
	
	if (m_pSkillList != NULL)
		m_pSkillList->setCurSel(-1);
}

void CSSkillLearnUI::_setSubTitle()
{
	if (m_pText[eTEXT_SUB_TITLE] == NULL)
		return;

	CMobData* MD = CMobData::getData(m_iMobIdx);

	if (MD == NULL)
		return;

	int nSSKillType = MD->GetSpecialSkillMaster();
	CTString	strSubTitle;

	switch( nSSKillType )
	{
	case SSKILL_MINING:		// 채굴
		strSubTitle = _S( 630, "채굴 스킬" );		
		break;
	case SSKILL_GATHERING:	// 채집
		strSubTitle = _S( 633, "채집 스킬" );		
		break;
	case SSKILL_CHARGE:		// 차지
		strSubTitle = _S( 636, "차지 스킬" );		
		break;
	case SSKILL_STONE:		// 광석정련
		strSubTitle = _S( 639, "스톤정련 스킬" );	
		break;
	case SSKILL_PLANT:		// 식물가공
		strSubTitle = _S( 642, "식물가공 스킬" );	
		break;
	case SSKILL_ELEMENT:	// 원소정제
		strSubTitle = _S( 645, "원소정제 스킬" );	
		break;
	case SSKILL_MAKE_WEAPON:	// 무기제작
		strSubTitle = _S( 648, "무기제작 스킬" );	
		break;
	case SSKILL_MAKE_WEAR:	// 방어구제작
	case SSKILL_MAKE_G_B:
	case SSKILL_MAKE_ARMOR:
	case SSKILL_MAKE_H_S:
		strSubTitle = _S( 651, "방어구제작 스킬" );	
		break;
	case SSKILL_MAKE_POTINO:
		strSubTitle = _S( 767, "물약 제작 스킬" );	
		break;
	default:
		return;
	}

	m_pText[eTEXT_SUB_TITLE]->SetText(strSubTitle);
}

void CSSkillLearnUI::_updateSP()
{
	if (m_pText[eTEXT_SKILL_POINT] == NULL)
		return;

	CTString strString;
	strString.PrintF( "%d", _pNetwork->MyCharacterInfo.sp / 10000 );
	UIMGR()->InsertCommaToString(strString);
	m_pText[eTEXT_SKILL_POINT]->SetText(strString);
}

bool CSSkillLearnUI::_CheckSatisfied()
{
	if (m_pSkillList == NULL)
		return false;

	int nSelectIndex = m_pSkillList->getCurSel();

	if (nSelectIndex < 0 || nSelectIndex >= m_vecSatisfied.size())
		return false;

	if (m_vecSatisfied[nSelectIndex] != ALL_SATISFIED)
		return false;
	

	return true;
}

void CSSkillLearnUI::OnUpdate( float fDeltaTime, ULONG ElapsedTime )
{
	if (GetHide() == TRUE)
		return;

	FLOAT	fDiffX = _pNetwork->MyCharacterInfo.x - m_fNpcX;
	FLOAT	fDiffZ = _pNetwork->MyCharacterInfo.z - m_fNpcZ;

	if( fDiffX * fDiffX + fDiffZ * fDiffZ > UI_VALID_SQRDIST)
		closeUI();

	_updateSP();
}

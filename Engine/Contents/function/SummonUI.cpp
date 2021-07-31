#include "stdh.h"
#include "SummonUI.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/SlaveInfo.h>
#include <Engine/Interface/UIPlayerInfo.h>
#include <Engine/Info/MyInfo.h>

#define SUMMON_START_X_GAP			105 // ui플레이어 인포의 초상화 부분 부터 시작 해야 되므로 초상화의 넓이를 수동으로 가지고 있는다.
#define	SUMMON_BAR_WIDTH			107
#define COMMAND_SKILL_NUM			5
class CmdCommandChange : public Command
{
public:
	CmdCommandChange() : _pUI(NULL), _CmdType(0) {}
	void setData(CSummonUI* pUI, int cmd) { _pUI = pUI; _CmdType = cmd; }
	void execute() {
		if (_pUI)
			_pUI->SetCommand(_CmdType);
	}

private:
	CSummonUI*	_pUI;
	int _CmdType;
};

class CmdCommandGuideEnter : public Command
{
public:
	CmdCommandGuideEnter() : _pUI(NULL), _CmdType(0) {}
	void setData(CSummonUI* pUI, int cmd) { _pUI = pUI; _CmdType = cmd; }
	void execute() {
		if (_pUI)
			_pUI->ShowCommandGuide(TRUE, _CmdType);
	}

private:
	CSummonUI*	_pUI;
	int _CmdType;
};

class CmdCommandGuideLeave : public Command
{
public:
	CmdCommandGuideLeave() : _pUI(NULL) {}
	void setData(CSummonUI* pUI) { _pUI = pUI;}
	void execute() {
		if (_pUI)
			_pUI->ShowCommandGuide(FALSE);
	}

private:
	CSummonUI*	_pUI;
};

CSummonUI::CSummonUI(int nUIIndex)
	: m_pMoveArea(NULL)
	, m_pIaSummonType(NULL)
	, m_pIconSkill(NULL)
	, m_pSpTooltipBg(NULL)
	, m_pTextTooltip(NULL)
	, m_nUIIndex(nUIIndex)
{
	m_penEntity			= NULL;	
	m_nSummonType		= CSlaveInfo::SLAVE_FIRE;
	m_nCurrentCommand	= CSlaveInfo::COMMAND_PROTECTION;
	m_nOldCommand		= CSlaveInfo::COMMAND_NONE;
	m_nIndex			= -1;	
	m_nLeftTime			= 0;
	m_nMaxTime			= 0;
	m_bSetMaxTime		= FALSE;	

	for (int i = 0; i < eBTN_MAX; ++i)
		m_pBtn[i] = NULL;

	setInherit(false);
}

void CSummonUI::SetSummonType( int iType )
{
	m_nSummonType = iType;
	const INDEX iSkillIndex = SlaveInfo().GetSkillIndex( m_nSummonType );
	AddSkill( iSkillIndex );

	if (m_pIaSummonType != NULL)
		m_pIaSummonType->SetRenderIdx(iType);
}

void CSummonUI::ResetSummon()
{
	m_nIndex			= -1;
	m_nLeftTime			= 0;
	m_nMaxTime			= 0;
	m_bSetMaxTime		= FALSE;
	
	WarpOfResetSummon();
}

void CSummonUI::WarpOfResetSummon()
{
	m_penEntity			= NULL;	
	m_nSummonType		= CSlaveInfo::SLAVE_FIRE;
	m_nCurrentCommand	= CSlaveInfo::COMMAND_PROTECTION;
	m_nOldCommand		= CSlaveInfo::COMMAND_NONE;

	for( int i = 0; i < eBTN_MAX; ++i )
	{
		if (m_pBtn[i] != NULL)
			m_pBtn[i]->SetBtnState( UBS_IDLE );
	}	

	if (m_pIconSkill != NULL)
		m_pIconSkill->clearIconData();

	closeUI();
}

void CSummonUI::UseSkill()
{
	if (m_pIconSkill == NULL)
		return;

	const int iSkillIndex = m_pIconSkill->getIndex();
	if (iSkillIndex != -1 && MY_INFO()->GetSkillDelay(iSkillIndex) == false)
	{
		UseSkill( iSkillIndex );
	}
}

void CSummonUI::UseSkill( int nIndex )
{
	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->UseSlaveSkill( m_penEntity, nIndex );
}

BOOL CSummonUI::StartSkillDelay( int nIndex )
{
	BOOL bResult = FALSE;	
	if (m_pIconSkill != NULL && m_pIconSkill->getBtnType() == UBET_SKILL &&
		m_pIconSkill->getIndex() == nIndex)
	{
		m_pIconSkill->setCooltime(true);
		bResult = TRUE;
	}
	return bResult;
}

void CSummonUI::SetCommand( INDEX iCommand )
{
	if( !GetSummonEntity() )
		return;

	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetSummonCommand( GetSummonEntity(), iCommand );
}

INDEX& CSummonUI::GetCommand()
{
	return m_nCurrentCommand;
}

void CSummonUI::SetLeftTime( int nLeftTime )
{
	m_nLeftTime		= nLeftTime;
	m_tmLeftTime	= _pTimer->GetHighPrecisionTimer().GetMilliseconds ();
}

void CSummonUI::SetMaxTime( int nMaxTime )
{
	if( !m_bSetMaxTime )
	{
		m_nMaxTime = nMaxTime;
		m_bSetMaxTime = TRUE;
	}
}

void CSummonUI::openUI()
{
	if (GetHide() == FALSE)
		return;

	Hide(FALSE);
	SetVisible(TRUE);
	resetUI();
	
	UIMGR()->RearrangeOrder( m_nUIIndex, TRUE );
}

void CSummonUI::closeUI()
{
	Hide(TRUE);
	SetVisible(FALSE);

	UIMGR()->RearrangeOrder( m_nUIIndex, FALSE );
}

void CSummonUI::resetUI()
{
}

void CSummonUI::OnUpdate( float fDeltaTime, ULONG ElapsedTime )
{
	if( !GetSummonEntity() )
		return;

	UpdateHPInfo();
	UpdateTimeInfo();
	UpdateBtnState();
}

void CSummonUI::OnPostRender( CDrawPort* pDraw )
{
	if( !GetSummonEntity() )
		return;

	// Timer for highlight buttons
	static BOOL		bHighlight = FALSE;
	static DOUBLE	dElapsedTime = 0.0;
	static DOUBLE	dOldTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
	DOUBLE	dCurTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
	dElapsedTime += dCurTime - dOldTime;
	dOldTime = dCurTime;
	if( dElapsedTime > 0.5 )
	{
		bHighlight = !bHighlight;
		do
		{
			dElapsedTime -= 0.5;
		}
		while( dElapsedTime > 0.5 );
	}

	if( bHighlight )
	{	
		if( m_nCurrentCommand != CSlaveInfo::COMMAND_NONE )
		{
			if (m_pBtn[m_nCurrentCommand] != NULL)
				m_pBtn[m_nCurrentCommand]->RenderHighlight( 0xFFFFFFFF );		
		}
	}
}

void CSummonUI::initialize()
{
#ifndef		WORLD_EDITOR
	// Set Slave Name 
	CSlaveInfo::Instance().SetName(CSlaveInfo::SLAVE_FIRE,_S(2526,"불의 정령"));
	CSlaveInfo::Instance().SetName(CSlaveInfo::SLAVE_WIND,_S(2527,"바람의 정령"));
	CSlaveInfo::Instance().SetName(CSlaveInfo::SLAVE_WATER,_S(2528,"물의 정령"));
	CSlaveInfo::Instance().SetName(CSlaveInfo::SLAVE_LAND,_S(2529,"땅의 정령"));
	CSlaveInfo::Instance().SetName(CSlaveInfo::SLAVE_GUARD,_S(3981, "경비병"));

	m_pMoveArea = (CUIImage*)findUI("move_area");

	if (m_pMoveArea != NULL)
	{
		int l, t;
		l = m_pMoveArea->GetPosX();
		t = m_pMoveArea->GetPosY();
		setTitleRect(l, t, l + m_pMoveArea->GetWidth(), t + m_pMoveArea->GetHeight());
	}

	// AI Btn
	std::string strBtnID[eBTN_MAX] = { "btn_Attack", "btn_Pincers", "btn_Hold", "btn_Protection"};

	for(int i = 0; i < eBTN_MAX; ++i)
	{
		m_pBtn[i] = (CUIButton*)findUI(strBtnID[i].c_str());

		if (m_pBtn[i] != NULL)
		{
			CmdCommandChange* pCmd = new CmdCommandChange;
			pCmd->setData(this, i);
			m_pBtn[i]->SetCommandUp(pCmd);

			CmdCommandGuideEnter* pCmdEnter = new CmdCommandGuideEnter;
			pCmdEnter->setData(this, i);
			m_pBtn[i]->SetCommandEnter(pCmdEnter);

			CmdCommandGuideLeave* pCmdLeave = new CmdCommandGuideLeave;
			pCmdLeave->setData(this);
			m_pBtn[i]->SetCommandLeave(pCmdLeave);
		}
	}
	
	// Status Img bar
	std::string strImgID[eIMG_MAX] = { "img_hp", "img_time"};

	for(int i = 0; i < eIMG_MAX; ++i)
		m_pImg[i] = (CUIImage*)findUI(strImgID[i].c_str());

	// Summon Type
	m_pIaSummonType = (CUIImageArray*)findUI("ia_type");

	// Skill Icon
	m_pIconSkill  = (CUIIcon*)findUI("icon_skill");

	if (m_pIconSkill != NULL)
	{
		m_pIconSkill->clearIconData();

		CmdCommandGuideEnter* pCmdEnter = new CmdCommandGuideEnter;
		pCmdEnter->setData(this, COMMAND_SKILL_NUM);
		m_pIconSkill->SetCommandEnter(pCmdEnter);

		CmdCommandGuideLeave* pCmdLeave = new CmdCommandGuideLeave;
		pCmdLeave->setData(this);
		m_pIconSkill->SetCommandLeave(pCmdLeave);
	}

	// tooltip
	m_pSpTooltipBg  = (CUIImageSplit*)findUI("is_tooltipBg");
	m_pTextTooltip  = (CUIText*)findUI("text_tooltip");

	if (m_pSpTooltipBg != NULL)
		m_pSpTooltipBg->Hide(TRUE);

	if (m_pTextTooltip != NULL)
		m_pTextTooltip->Hide(TRUE);
#endif	//	WORLD_EDITOR
}

void CSummonUI::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	int iRow = m_nUIIndex - UI_SUMMON_START;

	SetPos( pixMinI + pUIManager->GetPlayerInfo()->GetPosX() + SUMMON_START_X_GAP,
		pUIManager->GetPlayerInfo()->GetHeight() + (GetHeight() * iRow));
}

void CSummonUI::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

WMSG_RESULT CSummonUI::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if (m_pIaSummonType && m_pIaSummonType->IsInside(x, y))
	{
		BOOL bIsButtonClick = FALSE;
		CEntity			*penPlEntity;
		CPlayerEntity	*penPlayerEntity;
		penPlEntity = CEntity::GetPlayerEntity( 0 );
		penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);
		penPlayerEntity->SetTarget( GetSummonEntity() );
	}

	UIMGR()->RearrangeOrder( m_nUIIndex, TRUE );
	return CUIWindow::OnLButtonDown(x, y);
}

WMSG_RESULT CSummonUI::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	CUIManager* pUIMgr = UIMGR();
	CUIIcon* pDragIcon = pUIMgr->GetDragIcon();

	if (pDragIcon != NULL && pDragIcon->IsEmpty() == false)
	{
		if ( (pDragIcon->getBtnType() == UBET_SKILL && pDragIcon->GetWhichUI() == UI_SKILL_NEW) || 
			(pDragIcon->getBtnType() == UBET_SKILL && pDragIcon->GetWhichUI() == UI_QUICKSLOT) )
		{
			int	nIndex = pDragIcon->getIndex();
			AddSkill( nIndex );
		}
	}
	else
	{
		if( !IsFocused() )
			return WMSG_FAIL;

		if( m_pIconSkill && m_pIconSkill->IsInside(x, y))
		{
			if (m_pIconSkill->IsEmpty() == false)
				UseSkill();
		}
	}

	pUIMgr->ResetHoldBtn();
	return CUIWindow::OnLButtonUp(x, y);
}

void CSummonUI::UpdateHPInfo()
{
	ObjInfo* pInfo = ObjInfo::getSingleton();
	CSlaveTargetInfom* pSlaveInfo = pInfo->GetMySlaveInfo(m_nUIIndex - UI_SUMMON_START);
	// HP	
	float fHP = pSlaveInfo->fHealth;
	float fMaxHP = pSlaveInfo->fMaxHealth;
	FLOAT fHPRatio = Clamp( fHP / fMaxHP, 0.0f, 1.0f );

	if (m_pImg[eIMG_HP] != NULL)
		m_pImg[eIMG_HP]->SetWidth(SUMMON_BAR_WIDTH * fHPRatio);
}

void CSummonUI::UpdateTimeInfo()
{
	__int64	llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds(); // 현재 시간 얻기
	__int64	llCurDelay = llCurTime - m_tmLeftTime;

	int lLeftTime = m_nLeftTime - (llCurDelay/1000);

	if ( lLeftTime < 0 ) lLeftTime = 0;

	float fTime = (float)lLeftTime/(float)m_nMaxTime;
	FLOAT	fTimeRatio = Clamp( fTime, 0.0f, 1.0f );

	if (m_pImg[eIMG_TIME] != NULL)
		m_pImg[eIMG_TIME]->SetWidth(SUMMON_BAR_WIDTH * fTimeRatio);
}

void CSummonUI::UpdateBtnState()
{
	if( m_nCurrentCommand != m_nOldCommand && m_nCurrentCommand != CSlaveInfo::COMMAND_NONE )
		m_nOldCommand = m_nCurrentCommand;
}

void CSummonUI::AddSkill( int iSkillIndex )
{
	if (m_pIconSkill == NULL)
		return;

	// Empty slot
	if( iSkillIndex == -1 )
	{
		m_pIconSkill->clearIconData();
	}
	else
	{
		CSkill	&rSkillData = _pNetwork->GetSkillData( iSkillIndex );

		if( rSkillData.GetUsingSorcererFlag() & (1 << (m_nSummonType + 1) ) )
		{
			m_pIconSkill->setSkill(iSkillIndex);
			SlaveInfo().SetSkillIndex( m_nSummonType, iSkillIndex );

			// 불의 정령 스킬의 경우 스킬 사용 시 UI가 닫히게 되면서 갱신이 안됨
			// 쿨타임 계산 할 수 있도록 강제로 세팅해줌
			// 나중에 MyInfo 에서 스킬 사용 시간 업데이트 할 수 있도록 할 것.
			if (iSkillIndex == 324)
			{
				m_pIconSkill->setCooltime(true);
			}
		}
	}
}

void CSummonUI::ShowCommandGuide( BOOL bShow, int nCommandID /*= -1 */ )
{
	if (m_pSpTooltipBg == NULL || m_pTextTooltip == NULL)
		return;

	if (bShow == FALSE)
	{
		m_pSpTooltipBg->Hide(TRUE);
		m_pTextTooltip->Hide(TRUE);
		return;
	}
	
	int	nInfoX, nInfoY, nWidth, nHeight;

	if (nCommandID == COMMAND_SKILL_NUM)
	{
		if (m_pIconSkill == NULL)
			return;

		m_pIconSkill->GetPos(nInfoX, nInfoY );
		nWidth = m_pIconSkill->GetWidth();
		nHeight = m_pIconSkill->GetHeight();
	}
	else
	{
		if (m_pBtn[nCommandID] == NULL)
			return;

		m_pBtn[nCommandID]->GetPos( nInfoX, nInfoY );
		nWidth = m_pBtn[nCommandID]->GetWidth();
		nHeight = m_pBtn[nCommandID]->GetHeight();
	}

	CTString strGuide = GetCmdGuideString(nCommandID);	

	int	nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + strGuide.Length() *
		( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
	int	nInfoHeight = 22;

	nInfoX += ( nWidth - nInfoWidth ) / 2;
	nInfoY -= nHeight ;

	CUIManager* pUIManager = CUIManager::getSingleton();

	if( nInfoX + nInfoWidth > pUIManager->GetMaxI() )
		nInfoX += pUIManager->GetMaxI() - ( nInfoX + nInfoWidth );

	m_pSpTooltipBg->SetPos(nInfoX, nInfoY);
	m_pSpTooltipBg->SetWidth(nInfoWidth);
	m_pSpTooltipBg->SetHeight(nInfoHeight);
	m_pSpTooltipBg->UpdateSplit();
	m_pTextTooltip->SetText(strGuide);

	m_pSpTooltipBg->Hide(FALSE);
	m_pTextTooltip->Hide(FALSE);
}

CTString CSummonUI::GetCmdGuideString( int nCommandID )
{
	CTString strGuide = CTString("");
	BOOL bFirstSlot	= (m_nUIIndex == UI_SUMMON_START);		// 첫번째 슬롯인 경우.

	switch( nCommandID )
	{
	case CSlaveInfo::COMMAND_ATTACK:
		if( bFirstSlot )
			strGuide.PrintF( "%s %s", _S(2354, "선공" ), CTString( "(Shift + 1)" ) );
		else
			strGuide.PrintF( "%s %s", _S(2354, "선공" ), CTString( "(Shift + Z)" ) );
		break;

	case CSlaveInfo::COMMAND_PINCER:
		if( bFirstSlot )
			strGuide.PrintF( "%s %s", _S(2355, "협공" ), CTString( "(Shift + 2)" ) );
		else
			strGuide.PrintF( "%s %s", _S(2355, "협공" ), CTString( "(Shift + X)" ) );
		break;

	case CSlaveInfo::COMMAND_HOLD:
		if( bFirstSlot )
			strGuide.PrintF( "%s %s", _S(2356, "홀드" ), CTString( "(Shift + 3)" ) );
		else
			strGuide.PrintF( "%s %s", _S(2356, "홀드" ), CTString( "(Shift + C)" ) );
		break;

	case CSlaveInfo::COMMAND_PROTECTION:
		if( bFirstSlot )
			strGuide.PrintF( "%s %s", _S(2357, "수호" ), CTString( "(Shift + 4)" ) );
		else
			strGuide.PrintF( "%s %s", _S(2357, "수호" ), CTString( "(Shift + V)" ) );
		break;

	case COMMAND_SKILL_NUM:
		{
			CTString strSkillName = CTString("");

			if (m_pIconSkill != NULL && m_pIconSkill->IsEmpty() == false)
			{
				int		nSkillIndex = m_pIconSkill->getIndex();
				CSkill	&rSkill = _pNetwork->GetSkillData( nSkillIndex );
				strSkillName.PrintF( "%s", rSkill.GetName());
			}			

			if( bFirstSlot )
				strGuide.PrintF( "%s %s", strSkillName, CTString( "(Shift + 5)" ) );
			else
				strGuide.PrintF( "%s %s", strSkillName, CTString( "(Shift + B)" ) );
		}
		break;
	}

	return strGuide;
}

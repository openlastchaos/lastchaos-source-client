#include "stdh.h"
#include <vector>
#include <Engine/Interface/UISkillLearn.h>
#include <Engine/Interface/UISummon.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UIQuickSlot.h>


#ifdef ADJUST_MEMORIZE_SKILL
#define	SLEARN_TAB_WIDTH				64
#define	SLEARN_ACTIVE_TAB_CX			45
#define	SLEARN_PASSIVE_TAB_CX			109
#define SLEARN_MEMORIZE_TAB_CX			173
#else
#define	SLEARN_TAB_WIDTH				96
#define	SLEARN_ACTIVE_TAB_CX			60
#define	SLEARN_PASSIVE_TAB_CX			156
#endif

#define	SLEARN_TAB_SY					34
#define	SLEARN_SPECIAL_TAB_CX			108
/*
typedef struct tagChangeJobCondition
{
	int		iLevel;					// 필요 레벨
	int		iNeedSkill;				// 필요 스킬
	int		iNeedSkillLevel;		// 필요 스킬 레벨

}sJobCondition;

static sJobCondition _JobCondition[TOTAL_JOB] =
{
	{	35, 0, 1,	},		// TITAN
	{	35, 0, 1,	},		// KNIGHT
	{	35, 0, 1,	},		// HEALER
	{	35, 0, 1,	},		// MAGE
	{	35, 0, 1,	},		// ROGUE
};
*/

enum eSelection
{
	SKILL_LEARN,
	SKILL_TALK,
	SKILL_EVENT,
	SKILL_JOB_1,			// 전직하기 1번
	SKILL_JOB_2,			// 전직하기 2번
#ifdef HELP_SYSTEM_1
	SKILL_NPC_HELP,
#endif
	SKILL_TITAN,
	SKILL_KNIGHT,
	SKILL_HEALER,
	SKILL_MAGE,
	SKILL_ROGUE,
	SKILL_SORCERER,

	SKILL_QUEST,
};

extern int _aSummonSkill[5];

#define EXTENSION_LEVEL		(31)
extern INDEX g_iCountry;

// [KH_07044] 3차 도움말 관련 추가
extern INDEX g_iShowHelp1Icon;


// Date : 2005-03-07,   By Lee Ki-hwan
static int	_iMaxMsgStringChar = 0;

//////////////////////////////////////////////////////////////////////////
// NEW_USER_INTERFACE
#define	SKILLLEARN_NEW_SLOT_SX				16
#define	SKILLLEARN_NEW_SLOT_SY				68
#define	SKILLLEARN_NEW_SLOT_OFFSETY				36
#define	SKILLLEARN_NEW_NAME_CX					132
#define	SKILLLEARN_NEW_NAME_SY					70
#define	SKILLLEARN_NEW_NEED_RX					174

#define	SKILLLEARN_NEW_ACTIVE_TAB_CX			64
#define	SKILLLEARN_NEW_PASSIVE_TAB_CX			192
#define	SKILLLEARN_NEW_TAB_SY					43

#define	SKILLLEARN_NEW_TAB_WIDTH				122

#define	SKILLLEARN_NEW_CURSP_SX					98
#define	SKILLLEARN_NEW_CURSP_RX					243
#define	SKILLLEARN_NEW_CURSP_SY					290

#define SKILL_NOT_EXIST					-1

#define SKILLINFO_LINE_MAX				20


extern CTextureData* m_ptdButtonTexture;

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// NEW_USER_INTERFACE

void CUISkillLearn::CreateNewSkillLearn(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight)
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	//타이틀 마우스 드래그 영역
	m_rcTitle.SetRect( 0, 0, SKILLLEARN_NEW_WIDTH, 36 );

	//아이콘 표시 영역
	m_rcIcons.SetRect( 11, 65, 231, 285 );

	//탭영역
	m_rcTab.SetRect( 6, 37 , 251, 57 );

	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\NewCharacterInfo.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	//배경
	m_rtBackground.SetUV(0, 649, 256, 990, fTexWidth, fTexHeight );

	m_ptdButtonTexture = CreateTexture( CTString( "Data\\Interface\\CommonBtn.tex" ) );
	fTexWidth = m_ptdButtonTexture->GetPixWidth();
	fTexHeight = m_ptdButtonTexture->GetPixHeight();


	//선택된테두리
	m_rtSelOutlineTopL.SetUV( 145, 138, 157, 150, fTexWidth, fTexHeight );;
	m_rtSelOutlineTopM.SetUV( 157, 138, 226, 150, fTexWidth, fTexHeight );;
	m_rtSelOutlineTopR.SetUV( 226, 138, 239, 150, fTexWidth, fTexHeight );;
	m_rtSelOutlineMiddleL.SetUV( 145, 150, 157, 158, fTexWidth, fTexHeight );;
	m_rtSelOutlineMiddleM.SetUV( 157, 150, 226, 158, fTexWidth, fTexHeight );;
	m_rtSelOutlineMiddleR.SetUV( 226, 150, 239, 158, fTexWidth, fTexHeight );;
	m_rtSelOutlineBottomL.SetUV( 145, 158, 157, 171, fTexWidth, fTexHeight );;
	m_rtSelOutlineBottomM.SetUV( 157, 158, 226, 171, fTexWidth, fTexHeight );;
	m_rtSelOutlineBottomR.SetUV( 226, 158, 239, 171, fTexWidth, fTexHeight );;

	m_rtSelectedTab.SetUV(0, 106, 100, 127, fTexWidth, fTexHeight );
	m_rtUnSelectedTab.SetUV(104, 106, 204, 127, fTexWidth, fTexHeight );


	m_btnClose.Create( this, CTString( "" ), 232, 4, 16, 16 );
	m_btnClose.SetUV( UBS_IDLE, 211, 33, 227, 49, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK,229, 33, 245, 49, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_DISABLE,155, 74, 169, 88, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );

	// Learn button
	m_btnLearn.Create( this, _S( 269, "습득" ), 11, 313, 89, 22 );
	m_btnLearn.SetUV( UBS_IDLE, 113, 0, 182, 21, fTexWidth, fTexHeight );
	m_btnLearn.SetUV( UBS_CLICK, 186, 0, 255, 21, fTexWidth, fTexHeight );
	m_btnLearn.SetUV( UBS_DISABLE, 145, 178, 214, 199, fTexWidth, fTexHeight );
	m_btnLearn.CopyUV( UBS_IDLE, UBS_ON );

	// Cancel button
	m_btnCancel.Create( this, _S( 139, "취소" ), 156, 313, 89, 22 );
	m_btnCancel.SetUV( UBS_IDLE, 113, 0, 182, 21, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 186, 0, 255, 21, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_DISABLE, 145, 178, 214, 199, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );

	m_sbScrollBar.Create( this, 232, 67, 10, 213 );
	m_sbScrollBar.CreateButtons( TRUE, 10, 10, 0, 0, 10 );
	m_sbScrollBar.SetScrollPos( 0 );
	m_sbScrollBar.SetScrollRange( SKILLLEARN_NEW_SLOT_ROW_TOTAL );
	m_sbScrollBar.SetCurItemCount( 0 );
	m_sbScrollBar.SetItemsPerPage( SKILLLEARN_NEW_SLOT_ROW );

	// Up button
	m_sbScrollBar.SetUpUV( UBS_IDLE, 156, 33, 166, 43, fTexWidth, fTexHeight );
	m_sbScrollBar.SetUpUV( UBS_CLICK, 168, 33, 178, 43, fTexWidth, fTexHeight );
	m_sbScrollBar.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbScrollBar.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbScrollBar.SetBarTopUV( 185, 32, 195, 42, fTexWidth, fTexHeight );
	m_sbScrollBar.SetBarMiddleUV( 185, 41, 195, 51, fTexWidth, fTexHeight );
	m_sbScrollBar.SetBarBottomUV( 185, 61, 195, 71, fTexWidth, fTexHeight );
	// Down button
	m_sbScrollBar.SetDownUV( UBS_IDLE, 156, 45, 166, 55, fTexWidth, fTexHeight );
	m_sbScrollBar.SetDownUV( UBS_CLICK, 168, 45, 178, 55, fTexWidth, fTexHeight );
	m_sbScrollBar.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbScrollBar.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Wheel region
	m_sbScrollBar.SetWheelRect( -180, 0, 179, 322 );

	m_rtInfoUL.SetUV( 0, 137, 20, 157, fTexWidth, fTexHeight );
	m_rtInfoUM.SetUV( 20, 137, 120, 157, fTexWidth, fTexHeight );
	m_rtInfoUR.SetUV( 120, 137, 140, 157, fTexWidth, fTexHeight );
	m_rtInfoML.SetUV( 0, 157, 20, 177, fTexWidth, fTexHeight );
	m_rtInfoMM.SetUV( 20, 157, 120, 177, fTexWidth, fTexHeight );
	m_rtInfoMR.SetUV( 120, 157, 140, 177, fTexWidth, fTexHeight );
	m_rtInfoLL.SetUV( 0, 206, 20, 226, fTexWidth, fTexHeight );
	m_rtInfoLM.SetUV( 20, 206, 120, 226, fTexWidth, fTexHeight );
	m_rtInfoLR.SetUV( 120, 206, 140, 226, fTexWidth, fTexHeight );


	m_rsSkillName.Create(NULL, 0, 0, SKILLINFO_MIN_WIDTH, 50);
	m_rsSkillDesc.Create(NULL, 0, 0, SKILLINFO_MIN_WIDTH, 50);

	m_rsCurrentSkillInfo.Create(NULL, 0, 0, SKILLINFO_MIN_WIDTH, 200);
	m_rsNextSkillInfo.Create(NULL, 0, 0, SKILLINFO_MIN_WIDTH, 200);

	m_rcSkillInfo.SetRect(0, 0, SKILLINFO_MIN_WIDTH, 50);


	// Active skill
	for( int iRow = 0; iRow < SKILLLEARN_NEW_SLOT_TOTAL ; iRow++ )
	{
		m_btnActiveSkills[iRow].Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_SKILLLEARN,
										UBET_SKILL, 0, iRow );

	}
	// Passive skill
	for( iRow = 0; iRow < SKILLLEARN_NEW_SLOT_TOTAL ; iRow++ )
	{
		m_btnPassiveSkills[iRow].Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_SKILLLEARN,
										UBET_SKILL, 0, iRow );

	}

	// Passive skill
	for( iRow = 0; iRow < SKILLLEARN_NEW_SLOT_TOTAL ; iRow++ )
	{
		m_btnSpecialSkills[iRow].Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_SKILLLEARN,
										UBET_SKILL, 0, iRow );

	}

	// 글부분을 클릭해도 선택되게 함
	m_rcButtonArea.SetRect(0, 0, 212, 34);

}

void CUISkillLearn::RenderSkillInfoDesc()
{
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdButtonTexture );

	_pUIMgr->GetDrawPort()->AddTexture( m_rcSkillInfo.Left, m_rcSkillInfo.Top,
										m_rcSkillInfo.Left + 20, m_rcSkillInfo.Top + 20,
										m_rtInfoUL.U0, m_rtInfoUL.V0, m_rtInfoUL.U1, m_rtInfoUL.V1,
										0xFFFFFFBB );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcSkillInfo.Left + 20, m_rcSkillInfo.Top,
										m_rcSkillInfo.Right - 20, m_rcSkillInfo.Top + 20,
										m_rtInfoUM.U0, m_rtInfoUM.V0, m_rtInfoUM.U1, m_rtInfoUM.V1,
										0xFFFFFFBB );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcSkillInfo.Right - 20, m_rcSkillInfo.Top,
										m_rcSkillInfo.Right, m_rcSkillInfo.Top + 20,
										m_rtInfoUR.U0, m_rtInfoUR.V0, m_rtInfoUR.U1, m_rtInfoUR.V1,
										0xFFFFFFBB );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcSkillInfo.Left, m_rcSkillInfo.Top + 20,
										m_rcSkillInfo.Left + 20, m_rcSkillInfo.Bottom - 20,
										m_rtInfoML.U0, m_rtInfoML.V0, m_rtInfoML.U1, m_rtInfoML.V1,
										0xFFFFFFBB );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcSkillInfo.Left + 20, m_rcSkillInfo.Top + 20,
										m_rcSkillInfo.Right - 20, m_rcSkillInfo.Bottom - 20,
										m_rtInfoMM.U0, m_rtInfoMM.V0, m_rtInfoMM.U1, m_rtInfoMM.V1,
										0xFFFFFFBB );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcSkillInfo.Right - 20, m_rcSkillInfo.Top + 20,
										m_rcSkillInfo.Right, m_rcSkillInfo.Bottom - 20,
										m_rtInfoMR.U0, m_rtInfoMR.V0, m_rtInfoMR.U1, m_rtInfoMR.V1,
										0xFFFFFFBB );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcSkillInfo.Left, m_rcSkillInfo.Bottom - 20,
										m_rcSkillInfo.Left + 20, m_rcSkillInfo.Bottom,
										m_rtInfoLL.U0, m_rtInfoLL.V0, m_rtInfoLL.U1, m_rtInfoLL.V1,
										0xFFFFFFBB );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcSkillInfo.Left + 20, m_rcSkillInfo.Bottom - 20,
										m_rcSkillInfo.Right - 20, m_rcSkillInfo.Bottom,
										m_rtInfoLM.U0, m_rtInfoLM.V0, m_rtInfoLM.U1, m_rtInfoLM.V1,
										0xFFFFFFBB );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcSkillInfo.Right - 20, m_rcSkillInfo.Bottom - 20,
										m_rcSkillInfo.Right, m_rcSkillInfo.Bottom,
										m_rtInfoLR.U0, m_rtInfoLR.V0, m_rtInfoLR.U1, m_rtInfoLR.V1,
										0xFFFFFFBB );


	m_rsSkillName.Render();
	m_rsSkillDesc.Render();

	m_rsCurrentSkillInfo.Render();
	m_rsNextSkillInfo.Render();

	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();
}

void CUISkillLearn::AddSkillInfoString(int nSkillInfoList, CTString strSkillInfo, COLOR strColor)
{
	if(nSkillInfoList == SKILLINFO_CURRENT)
	{
		m_rsCurrentSkillInfo.AddString(strSkillInfo, strColor);
	}
	else if(nSkillInfoList == SKILLINFO_NEXT)
	{
		m_rsNextSkillInfo.AddString(strSkillInfo, strColor);
	}

	int nHeight = 0;
	int nWidth =  0;

	if(m_rsNextSkillInfo.GetMaxStrWidth() == 0)
	{

		m_rsSkillName.SetWidth(SKILLINFO_MIN_WIDTH);
		nHeight = m_rsSkillName.GetMaxStrHeight();
		m_rsSkillName.SetHeight(nHeight);
		
		m_rsSkillDesc.SetWidth(SKILLINFO_MIN_WIDTH);
		nHeight = m_rsSkillDesc.GetMaxStrHeight();
		m_rsSkillDesc.SetHeight(nHeight);

		nWidth  = SKILLINFO_MIN_WIDTH;
	}
	else
	{

		m_rsSkillName.SetWidth(SKILLINFO_MAX_WIDTH);
		nHeight = m_rsSkillName.GetMaxStrHeight();
		m_rsSkillName.SetHeight(nHeight);
		
		m_rsSkillDesc.SetWidth(SKILLINFO_MAX_WIDTH);
		nHeight = m_rsSkillDesc.GetMaxStrHeight();
		m_rsSkillDesc.SetHeight(nHeight);

		nWidth  = SKILLINFO_MAX_WIDTH;
	}

	nHeight = m_rsSkillName.GetMaxStrHeight() + m_rsSkillDesc.GetMaxStrHeight();
	if( m_rsCurrentSkillInfo.GetMaxStrHeight() > m_rsNextSkillInfo.GetMaxStrHeight() )
	{
		nHeight += m_rsCurrentSkillInfo.GetMaxStrHeight();
	}
	else
	{
		nHeight += m_rsNextSkillInfo.GetMaxStrHeight();
	}


	m_rcSkillInfo.SetRect(m_rcSkillInfo.Left, m_rcSkillInfo.Top, m_rcSkillInfo.Left + nWidth + 20, m_rcSkillInfo.Top + nHeight + 20);
}

void CUISkillLearn::ResetSkillInfoString()
{
	m_rsSkillName.ClearString();
	m_rsSkillDesc.ClearString();

	m_rsCurrentSkillInfo.ClearString();
	m_rsNextSkillInfo.ClearString();
}

void CUISkillLearn::SetSkillInfoPos(int nPosX, int nPosY)
{
	int nNextPosy;

	m_rsSkillName.SetPos(nPosX + 10, nPosY + 10);
	nNextPosy = nPosY + m_rsSkillName.GetHeight() + 10;
	m_rsSkillDesc.SetPos(nPosX + 10, nNextPosy);

	nNextPosy += m_rsSkillDesc.GetHeight();
	m_rsCurrentSkillInfo.SetPos(nPosX + 10, nNextPosy);

	m_rsNextSkillInfo.SetPos(nPosX + m_rsCurrentSkillInfo.GetWidth() + 10, nNextPosy);

	m_rcSkillInfo.SetRect(nPosX, nPosY, nPosX+m_rcSkillInfo.GetWidth(), nPosY+m_rcSkillInfo.GetHeight());
}

int	CUISkillLearn::GetSkillInfoWidth()
{
	return m_rcSkillInfo.GetWidth();
}
int CUISkillLearn::GetSkillInfoHeight()
{
	return m_rcSkillInfo.GetHeight();
}

void CUISkillLearn::RenderNewSkillLearn()
{
	FLOAT	fDiffX = _pNetwork->MyCharacterInfo.x - m_fNpcX;
	FLOAT	fDiffZ = _pNetwork->MyCharacterInfo.z - m_fNpcZ;
	if( fDiffX * fDiffX + fDiffZ * fDiffZ > UI_VALID_SQRDIST && !m_bUseCard)
		CloseSkillLearn();

	int nPosX, nPosY;

	GetAbsPos(nPosX, nPosY);

	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	_pUIMgr->GetDrawPort()->AddTexture( nPosX, nPosY, nPosX + SKILLLEARN_NEW_WIDTH, nPosY + SKILLLEARN_NEW_HEIGHT,
										m_rtBackground.U0, m_rtBackground.V0, m_rtBackground.U1, m_rtBackground.V1,
										0xFFFFFFFF );
	
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();
 

	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdButtonTexture );
	nPosX = m_nPosX + m_rcTab.Left;
	nPosY = m_nPosY + m_rcTab.Top;
	

	// 스킬 탭 표시
	if(m_nCurrentSkillType == SKILL_ACTIVE)
	{
		_pUIMgr->GetDrawPort()->AddTexture( nPosX , nPosY, nPosX + (m_rcTab.GetWidth()/2)-1, nPosY + m_rcTab.GetHeight(),
						m_rtSelectedTab.U0, m_rtSelectedTab.V0, m_rtSelectedTab.U1, m_rtSelectedTab.V1,
						0xFFFFFFFF );

		_pUIMgr->GetDrawPort()->AddTexture( nPosX + (m_rcTab.GetWidth()/2)+1, nPosY, nPosX + m_rcTab.GetWidth(), nPosY + m_rcTab.GetHeight(),
					m_rtUnSelectedTab.U0, m_rtUnSelectedTab.V0, m_rtUnSelectedTab.U1, m_rtUnSelectedTab.V1,
					0xFFFFFFFF );
	}
	else if(m_nCurrentSkillType == SKILL_PASSIVE)
	{
		_pUIMgr->GetDrawPort()->AddTexture( nPosX , nPosY, nPosX + (m_rcTab.GetWidth()/2)-1, nPosY + m_rcTab.GetHeight(),
						m_rtUnSelectedTab.U0, m_rtUnSelectedTab.V0, m_rtUnSelectedTab.U1, m_rtUnSelectedTab.V1,
						0xFFFFFFFF );

		_pUIMgr->GetDrawPort()->AddTexture( nPosX + (m_rcTab.GetWidth()/2)+1, nPosY, nPosX + m_rcTab.GetWidth(), nPosY + m_rcTab.GetHeight(),
					m_rtSelectedTab.U0, m_rtSelectedTab.V0, m_rtSelectedTab.U1, m_rtSelectedTab.V1,
					0xFFFFFFFF );
	}
	else
	{
		_pUIMgr->GetDrawPort()->AddTexture( nPosX , nPosY, nPosX + (m_rcTab.GetWidth()/2)-1, nPosY + m_rcTab.GetHeight(),
						m_rtUnSelectedTab.U0, m_rtUnSelectedTab.V0, m_rtUnSelectedTab.U1, m_rtUnSelectedTab.V1,
						0xFFFFFFFF );

		_pUIMgr->GetDrawPort()->AddTexture( nPosX + (m_rcTab.GetWidth()/2)+1, nPosY, nPosX + m_rcTab.GetWidth(), nPosY + m_rcTab.GetHeight(),
					m_rtUnSelectedTab.U0, m_rtUnSelectedTab.V0, m_rtUnSelectedTab.U1, m_rtUnSelectedTab.V1,
					0xFFFFFFFF );
	}

	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

//	int iRow;
	int iRowS = m_sbScrollBar.GetScrollPos();
	int iRowE = iRowS + SKILLLEARN_NEW_SLOT_ROW;
	int	nX;
	int nY;

	m_btnClose.Render();

	m_btnLearn.Render();

	m_btnCancel.Render();

	m_sbScrollBar.Render();

	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	RenderNewSkillLearnBtns();



	// 선택 라인 표시
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdButtonTexture );
	iRowS = m_sbScrollBar.GetScrollPos();
	iRowE = iRowS + SKILLLEARN_NEW_SLOT_ROW;
	if( m_nSelectedSkillID >= 0 && iRowS <= m_nSelectedSkillID && m_nSelectedSkillID < iRowE )
	{

		m_btnSelectedSkill[m_nSelectedSkillID].GetAbsPos( nX, nY );


		_pUIMgr->GetDrawPort()->AddTexture( nX - 1, nY - 1, nX + 12, nY + 12,
											m_rtSelOutlineTopL.U0, m_rtSelOutlineTopL.V0, m_rtSelOutlineTopL.U1, m_rtSelOutlineTopL.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( nX + 12, nY - 1, nX + 198, nY + 12,
											m_rtSelOutlineTopM.U0, m_rtSelOutlineTopM.V0, m_rtSelOutlineTopM.U1, m_rtSelOutlineTopM.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( nX + 198, nY - 1, nX + 211, nY + 12,
											m_rtSelOutlineTopR.U0, m_rtSelOutlineTopR.V0, m_rtSelOutlineTopR.U1, m_rtSelOutlineTopR.V1,
											0xFFFFFFFF );

		_pUIMgr->GetDrawPort()->AddTexture( nX - 1, nY + 12, nX + 12, nY + 20,
											m_rtSelOutlineMiddleL.U0, m_rtSelOutlineMiddleL.V0, m_rtSelOutlineMiddleL.U1, m_rtSelOutlineMiddleL.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( nX + 12, nY + 12, nX + 198, nY + 20,
											m_rtSelOutlineMiddleM.U0, m_rtSelOutlineMiddleM.V0, m_rtSelOutlineMiddleM.U1, m_rtSelOutlineMiddleM.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( nX + 198, nY + 12, nX + 211, nY + 20,
											m_rtSelOutlineMiddleR.U0, m_rtSelOutlineMiddleR.V0, m_rtSelOutlineMiddleR.U1, m_rtSelOutlineMiddleR.V1,
											0xFFFFFFFF );

		_pUIMgr->GetDrawPort()->AddTexture( nX - 1, nY + 20, nX + 12, nY + 33,
											m_rtSelOutlineBottomL.U0, m_rtSelOutlineBottomL.V0, m_rtSelOutlineBottomL.U1, m_rtSelOutlineBottomL.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( nX + 12, nY + 20, nX + 198, nY + 33,
											m_rtSelOutlineBottomM.U0, m_rtSelOutlineBottomM.V0, m_rtSelOutlineBottomM.U1, m_rtSelOutlineBottomM.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( nX + 198, nY + 20, nX + 211, nY + 33,
											m_rtSelOutlineBottomR.U0, m_rtSelOutlineBottomR.V0, m_rtSelOutlineBottomR.U1, m_rtSelOutlineBottomR.V1,
											0xFFFFFFFF );

	}

	if(m_nSelectedSkillID != -1)
	{
		if(m_pSelectedSkillSatisfied[m_nSelectedSkillID] == ALL_SATISFIED)
		{
			if(!m_btnLearn.IsEnabled())
			{
				m_btnLearn.SetEnable(TRUE);
			}
		}
		else
		{
			if(m_btnLearn.IsEnabled())
			{
				m_btnLearn.SetEnable(FALSE);
			}
		}
	}
	else
	{
		if(m_btnLearn.IsEnabled())
		{
			m_btnLearn.SetEnable(FALSE);
		}
	}

	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	if(m_bSkillInfoVisible)
	{
		RenderSkillInfoDesc();
	}

	_pUIMgr->GetDrawPort()->PutTextExCX( _S(91, "스킬") , m_nPosX + (SKILLLEARN_NEW_WIDTH / 2),			
										m_nPosY + 15 );

	_pUIMgr->GetDrawPort()->EndTextEx();
}

void CUISkillLearn::RenderNewSkillLearnBtns()
{
	int	nX = SKILLLEARN_NEW_SLOT_SX, nY = SKILLLEARN_NEW_SLOT_SY;
	int	iRow, iRowS, iRowE;

	///버튼 렌더링
	iRowS = m_sbScrollBar.GetScrollPos();		
	iRowE = iRowS + SKILLLEARN_NEW_SLOT_ROW;
	for( iRow = iRowS; iRow < iRowE; iRow++, nY += SKILLLEARN_NEW_SLOT_OFFSETY )
	{      
		if( m_btnSelectedSkill[iRow].IsEmpty() )		
			continue;
		
		m_btnSelectedSkill[iRow].SetPos( nX, nY );

		if(m_pSelectedSkillSatisfied[iRow] == ALL_SATISFIED)
			m_btnSelectedSkill[iRow].Render();
		else
		{
			m_btnSelectedSkill[iRow].Render();
		}


	}

	_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_SKILL );
	

	//이름 출력
	nY = SKILLLEARN_NEW_NAME_SY;
	int	nCharLevel	= _pNetwork->MyCharacterInfo.level;
	int nCharSP		= _pNetwork->MyCharacterInfo.sp;
	iRowS = m_sbScrollBar.GetScrollPos();
	iRowE = iRowS + SKILLLEARN_NEW_SLOT_ROW;
	for( iRow = iRowS; iRow < iRowE; iRow++, nY += SKILLLEARN_NEW_SLOT_OFFSETY )
	{
		if( m_btnSelectedSkill[iRow].IsEmpty() )
			continue;

		if(m_nCurrentSkillType != SKILL_SPECIAL)
		{
			CSkill	&rSkill= _pNetwork->GetSkillData( m_btnSelectedSkill[iRow].GetSkillIndex() );

			SBYTE	sbLevel = m_btnSelectedSkill[iRow].GetSkillLevel();
			int		nNeedLevel = rSkill.GetLearnLevel( sbLevel - 1 );

			m_strDesc.PrintF( "%s", rSkill.GetName() );
			_pUIMgr->GetDrawPort()->PutTextExCX( m_strDesc, m_nPosX + SKILLLEARN_NEW_NAME_CX, m_nPosY + nY,
													m_pSelectedSkillSatisfied[iRow] == ALL_SATISFIED ? 0xFFC672FF : 0xBCBCBCFF );

			m_strDesc.PrintF( "Lv %2d   SP %2d", sbLevel, rSkill.GetLearnSP( sbLevel - 1 ) );
			_pUIMgr->GetDrawPort()->PutTextExRX( m_strDesc, m_nPosX + SKILLLEARN_NEW_NEED_RX,
													m_nPosY + nY + 17, 0xBDA99FFF );
		}
		else
		{
			CSpecialSkill &rSSkill = _pNetwork->GetSSkillData( m_btnSelectedSkill[iRow].GetSkillIndex() );

			SBYTE	sbLevel = m_btnSelectedSkill[iRow].GetSkillLevel();
			int		nNeedLevel = rSSkill.GetLearnLevel( sbLevel - 1 );

			m_strDesc.PrintF( "%s", rSSkill.GetName() );
			_pUIMgr->GetDrawPort()->PutTextExCX( m_strDesc, m_nPosX + SKILLLEARN_NEW_NAME_CX, m_nPosY + nY,
													nCharLevel >= nNeedLevel ? 0xFFC672FF : 0xBCBCBCFF );

			m_strDesc.PrintF( "Lv %2d   SP %2d", sbLevel, rSSkill.GetLearnSP( sbLevel - 1 ) );
			_pUIMgr->GetDrawPort()->PutTextExRX( m_strDesc, m_nPosX + SKILLLEARN_NEW_NEED_RX,
													m_nPosY + nY + 17, 0xBDA99FFF );
		}


	}

	if(m_nCurrentSkillType != SKILL_SPECIAL)
	{
		_pUIMgr->GetDrawPort()->PutTextExCX( _S( 275, "일반" ), m_nPosX + SKILLLEARN_NEW_ACTIVE_TAB_CX,
												m_nPosY + SKILLLEARN_NEW_TAB_SY,
												m_nCurrentSkillType == SKILL_ACTIVE ? 0xFFCB00FF : 0x6B6B6BFF );
		_pUIMgr->GetDrawPort()->PutTextExCX( _S( 276, "강화" ), m_nPosX + SKILLLEARN_NEW_PASSIVE_TAB_CX,
												m_nPosY + SKILLLEARN_NEW_TAB_SY,
												m_nCurrentSkillType == SKILL_PASSIVE ? 0xFFCB00FF : 0x6B6B6BFF );
	}
	else
	{
		CTString	strSubTitle;
		switch( m_nSSkillType )
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
		}
		_pUIMgr->GetDrawPort()->PutTextExCX( strSubTitle, m_nPosX + SKILLLEARN_NEW_ACTIVE_TAB_CX, m_nPosY + SKILLLEARN_NEW_TAB_SY, 0xFFCB00FF);
	}

	_pUIMgr->GetDrawPort()->PutTextEx(  _S( 90, "숙련도" ) , m_nPosX + SKILLLEARN_NEW_CURSP_SX,			
										m_nPosY + SKILLLEARN_NEW_CURSP_SY,  0xDED9A0FF);
	_pUIMgr->GetDrawPort()->PutTextExRX( _pUIMgr->GetCharacterInfo()->GetStringOfSP(),
											m_nPosX + SKILLLEARN_NEW_CURSP_RX, m_nPosY + SKILLLEARN_NEW_CURSP_SY-2, 0xBDA99FFF );

	_pUIMgr->GetDrawPort()->EndTextEx();
}


WMSG_RESULT CUISkillLearn::SKillLearnNewMouseMessage(MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	static BOOL bTitleBarClick = FALSE;

	static BOOL	bLButtonDownInBtn = FALSE;

	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );
	
	switch( pMsg->message )
	{
		case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				_pUIMgr->SetMouseCursorInsideUIs();


			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			// Move inventory
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				
				nOldX = nX;	nOldY = nY;
				
				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			// Close button
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnLearn.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;

			if( IsInsideRect( nX, nY, m_rcIcons ) )
			{
				int	iRow;
				int	iRowS = m_sbScrollBar.GetScrollPos();
				int	iRowE = iRowS + SKILLLEARN_NEW_SLOT_ROW;
				int	nWhichRow = -1;
			
				for( iRow = iRowS; iRow < iRowE; iRow++ )
				{
					if( m_btnSelectedSkill[iRow].MouseMessage( pMsg ) != WMSG_FAIL )
						nWhichRow = iRow;
				}

				// Show tool tip
				if( nWhichRow != -1 )
				{
					m_bSkillInfoVisible = TRUE;
					ShowSkillLearnInfo(nWhichRow);
				}
				else
					m_bSkillInfoVisible = FALSE;

				return WMSG_SUCCESS;
			}
			else
			{
				m_bSkillInfoVisible = FALSE;
			}

			if( bLButtonDownInBtn && m_nSelectedSkillID >= 0 && ( pMsg->wParam & MK_LBUTTON ) )
			{
				m_btnSelectedSkill[m_nSelectedSkillID].SetBtnState( UBES_IDLE );
				bLButtonDownInBtn = FALSE;
			}
			// Active icon scroll bar
			else if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
		}
		break;
		
		case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ))
			{
				nOldX = nX;		nOldY = nY;

				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
					
				}
				// Title bar
				if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
				}
				if( m_btnLearn.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Skill slot
				if( IsInsideRect( nX, nY, m_rcIcons ) )
				{
					int	nOldSelSkillID = m_nSelectedSkillID;
					m_nSelectedSkillID = -1;
    
					int	iRow;
					int	iRowS = m_sbScrollBar.GetScrollPos();
					int	iRowE = iRowS + SKILLLEARN_NEW_SLOT_ROW;
					int	nWhichRow = -1;

					int nPosX = 0;
					int nPosY = 0;
					int nWidth = m_rcButtonArea.GetWidth();
					int nHeight = m_rcButtonArea.GetHeight();

					iRowE = iRowS + SKILLLEARN_NEW_SLOT_ROW;

					for( iRow = iRowS; iRow < iRowE; iRow++ )
					{
						if( m_btnSelectedSkill[iRow].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Update selected skill
							m_nSelectedSkillID = iRow;

							bLButtonDownInBtn = TRUE;
							_pUIMgr->RearrangeOrder( UI_SKILLLEARN, TRUE );
							return WMSG_SUCCESS;
						}

						if(!m_btnSelectedSkill[iRow].IsEmpty())
						{
							nPosX =  m_btnSelectedSkill[iRow].GetPosX();
							nPosY =  m_btnSelectedSkill[iRow].GetPosY();
							m_rcButtonArea.SetRect(nPosX, nPosY, nPosX + nWidth, nPosY + nHeight);
							if(IsInsideRect(nX, nY, m_rcButtonArea))
							{
								if(m_nSelectedSkillID != iRow)
								{
									m_nSelectedSkillID = iRow;
								}
								_pUIMgr->RearrangeOrder( UI_SKILLLEARN, TRUE );
								return WMSG_SUCCESS;
							}
						}

					}
				}
				if( IsInsideRect( nX, nY, m_rcTab ) )
				{
					if( m_nCurrentSkillType != SKILL_SPECIAL )
					{
						int nState = ( nX - m_nPosX - m_rcTab.Left ) / SKILLLEARN_NEW_TAB_WIDTH;
						
						SetCurrentSkill(nState);
					}
				}

				_pUIMgr->RearrangeOrder( UI_SKILLLEARN, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

		case WM_LBUTTONUP:
		{
			bLButtonDownInBtn = FALSE;

			 if( _pUIMgr->GetHoldBtn().IsEmpty() )
			 {
				// Title bar
				bTitleBarClick = FALSE;

				// If character information isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						CloseSkillLearn();
					}

					return WMSG_SUCCESS;
				}
				// Learn button
				if( ( wmsgResult = m_btnLearn.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						SendLearnSkill();
					}

					return WMSG_SUCCESS;
				}
				// Cancel button
				if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						CloseSkillLearn();
					}

					return WMSG_SUCCESS;
				}
										
				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				else if( IsInsideRect( nX, nY, m_rcIcons ) )
				{
					int	iRow;
					int	iRowS = m_sbScrollBar.GetScrollPos();
					int	iRowE;
					iRowE = iRowS + SKILLLEARN_NEW_SLOT_ROW;

					for( iRow = iRowS; iRow < iRowE; iRow++ )
					{
						if( ( wmsgResult =  m_btnSelectedSkill[iRow].MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
						}
							return WMSG_SUCCESS;
					}
				}
			 }
			 else
			 {
				if( IsInside( nX, nY ) )
				{
					// Reset holding button
					_pUIMgr->ResetHoldBtn();

					return WMSG_SUCCESS;
				}
			 }
		}
		break;

		case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
			{
				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				return WMSG_SUCCESS;
			}
		}
		break;

		case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;
	}

	return WMSG_FAIL;
}


//////////////////////////////////////////////////////////////////////////
//
// 스킬 정보를 표시 합니다.(스킬 습득에서만 사용)
//
//////////////////////////////////////////////////////////////////////////
void CUISkillLearn::ShowSkillLearnInfo(int nRow /* = -1 */)
{
	CTString strTemp;
	COLOR	strColor;

	ResetSkillInfoString();

//	m_bSkillInfoVisible = FALSE;

	int nIndex = m_btnSelectedSkill[nRow].GetSkillIndex();

	if(m_nCurrentSkillType != SKILL_SPECIAL)
	{
		CSkill		&rSkill = _pNetwork->GetSkillData( nIndex );
		int nLevel = _pUIMgr->GetCharacterInfo()->GetSkillLevel(nIndex, FALSE);
		
		strColor =  0x0077FFFF;
		strTemp.PrintF("%s(%d/%d)",rSkill.GetName(), nLevel, rSkill.GetMaxLevel());
		m_rsSkillName.AddString(strTemp, strColor);

		strColor =  0x72D02EFF;
		strTemp = rSkill.GetDescription();
		m_rsSkillDesc.AddString(strTemp, strColor);

		if( rSkill.GetFlag() & SF_SINGLEMODE )
		{
			strTemp.PrintF(_S(499, "퍼스널던전 전용 스킬"));
			m_rsSkillName.AddString(strTemp, strColor);
		}
					
		
		int nLoopCnt = 0;
		if(nLevel == 0) // 배우지 않은 스킬(다음 정보만 표시)
		{
			nLoopCnt = 1;
		}
		else //배운 스킬 (현재, 다음 표시)
		{
			nLoopCnt = 2;
			nLevel--; 
		}

		for(int n = 0; n < nLoopCnt; n++, nLevel++)
		{
			int		nLearnSkillIndex[3];
			SBYTE	sbLearnSkillLevel[3];
			int		nLearnItemIndex[3];
			SQUAD	nLearnItemCount[3];
			BOOL	bLearnSkill = FALSE;
			BOOL	bLearnItem = FALSE;

			strColor = 0xF2F2F2FF;

			for( int i = 0; i < 3; i++ )
			{
				nLearnSkillIndex[i] = rSkill.GetLearnSkillIndex( nLevel, i );
				sbLearnSkillLevel[i] = rSkill.GetLearnSkillLevel( nLevel, i );
				nLearnItemIndex[i] = rSkill.GetLearnItemIndex( nLevel, i );
				nLearnItemCount[i] = rSkill.GetLearnItemCount( nLevel, i );
				
				if( nLearnSkillIndex[i] != -1 )
					bLearnSkill = TRUE;
				if( nLearnItemIndex[i] != -1 )
					bLearnItem = TRUE;
			}
			switch( rSkill.GetType() )
			{
			case CSkill::ST_MELEE:					// Active
			case CSkill::ST_RANGE:					// Active
			case CSkill::ST_MAGIC:					// Active
				{
					strTemp.PrintF( _S( 256, "필요 레벨 : %d" ), rSkill.GetLearnLevel( nLevel ) );
					if(n == 1 || nLoopCnt == 1)
					{
						strColor = m_pSelectedSkillSatisfied[nRow] & NOT_SATISFIED_LEVEL ? 0xFF0000FF : 0xFFFF00FF;
					}
					else
					{
						strColor = 0xF2F2F2FF;
					}
					
					AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, strColor);
					strTemp.PrintF( _S( 257, "필요 SP : %d" ), rSkill.GetLearnSP( nLevel ) ); // 숙련도
					if(n == 1 || nLoopCnt == 1)
					{
						strColor = m_pSelectedSkillSatisfied[nRow] & NOT_SATISFIED_SP ? 0xFF0000FF : 0xFFFF00FF;
					}
					else
					{
						strColor = 0xF2F2F2FF;
					}
					AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, strColor);				

					const int iLearnStr = rSkill.GetLearnStr( nLevel );
					const int iLearnDex = rSkill.GetLearnDex( nLevel );
					const int iLearnInt = rSkill.GetLearnInt( nLevel );
					const int iLearnCon = rSkill.GetLearnCon( nLevel );

					if( iLearnStr > 0 ) 
					{
						strTemp.PrintF( _S( 1391, "필요 힘 : %d" ), iLearnStr );		// 필요 힘 
						if(n == 1 || nLoopCnt == 1)
						{
							strColor = m_pSelectedSkillSatisfied[nRow] & NOT_SATISFIED_STR ? 0xFF0000FF : 0xFFFF00FF;
						}
						else
						{
							strColor = 0xF2F2F2FF;
						}
						AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, strColor);
					}

					if( iLearnDex > 0 ) 
					{
						strTemp.PrintF( _S( 1392, "필요 민첩 : %d" ), iLearnDex );	// 필요 민첩
						if(n == 1 || nLoopCnt == 1)
						{
							strColor = m_pSelectedSkillSatisfied[nRow] & NOT_SATISFIED_DEX ? 0xFF0000FF : 0xFFFF00FF;
						}
						else
						{
							strColor = 0xF2F2F2FF;
						}
						AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, strColor);
					}
					
					if( iLearnInt > 0 ) 
					{
						strTemp.PrintF( _S( 1393, "필요 지혜 : %d" ), iLearnInt );	// 필요 지혜
						if(n == 1 || nLoopCnt == 1)
						{
							strColor = m_pSelectedSkillSatisfied[nRow] & NOT_SATISFIED_INT ? 0xFF0000FF : 0xFFFF00FF;
						}
						else
						{
							strColor = 0xF2F2F2FF;
						}
						AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, strColor);
					}

					if( iLearnCon > 0 )
					{
						strTemp.PrintF( _S( 1394, "필요 체질 : %d" ), iLearnCon );	// 필요 체질
						if(n == 1 || nLoopCnt == 1)
						{
							strColor = m_pSelectedSkillSatisfied[nRow] & NOT_SATISFIED_CON ? 0xFF0000FF : 0xFFFF00FF;
						}
						else
						{
							strColor = 0xF2F2F2FF;
						}
						AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, strColor);
					}
					
					// Need skill
					if( bLearnSkill )
					{
						for( i = 0; i < 3; i++ )
						{
							if( nLearnSkillIndex[i] != -1 )
							{
								CSkill	&rNeedSkill = _pNetwork->GetSkillData( nLearnSkillIndex[i] );
								strTemp.PrintF( " : %s Lv.%d", rNeedSkill.GetName(), sbLearnSkillLevel[i] );
								strTemp = _S( 258, "필요 스킬" ) + strTemp;
								if(n == 1 || nLoopCnt == 1)
								{
									strColor = m_pSelectedSkillSatisfied[nRow] & NOT_SATISFIED_SKILL ? 0xFF0000FF : 0xFFFF00FF;
								}
								else
								{
									strColor = 0xF2F2F2FF;
								}
								AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, strColor);
							}
						}
					}
					
					// Need item
					if( bLearnItem )
					{
						for( i = 0; i < 3; i++ )
						{
							if( nLearnItemIndex[i] != -1 )
							{
								CItemData	&rNeedItem = _pNetwork->GetItemData( nLearnItemIndex[i] );
								strTemp.PrintF( _S( 260, "  %s %d개" ), _pNetwork->GetItemName( nLearnItemIndex[i] ), nLearnItemCount[i] );
								strTemp = _S( 259, "필요 아이템" ) + strTemp;
								if(n == 1 || nLoopCnt == 1)
								{
									strColor = m_pSelectedSkillSatisfied[nRow] & NOT_SATISFIED_ITEM ? 0xFF0000FF : 0xFFFF00FF;
								}
								else
								{
									strColor = 0xF2F2F2FF;
								}
								AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, strColor);
							}
						}
					}
					
					if(n == 1 || nLoopCnt == 1)
					{
						strColor = 0xFFFF00FF;
					}
					else
					{
						strColor = 0xF2F2F2FF;
					}

					int	nNeedMP = rSkill.GetNeedMP( nLevel );
					int	nNeedHP = rSkill.GetNeedHP( nLevel );
					if( nNeedHP == 0 )
					{
						if( nNeedMP != 0 )
						{
							strTemp.PrintF( _S( 64, "소모 MP : %d" ), nNeedMP );

								AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, strColor);
						}
					}
					else
					{
						if( nNeedMP == 0 )
						{
							strTemp.PrintF( _S( 500, "소모 HP : %d" ), nNeedHP );	
							AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, strColor);
						}
						else
						{
							strTemp.PrintF( _S( 64, "소모 MP : %d" ), nNeedMP );
							AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, strColor);
							strTemp.PrintF( _S( 500, "소모 HP : %d" ), nNeedHP );
							AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, strColor);
						}
					}

					if( rSkill.GetPower( nLevel ) > 0 )
					{
						strTemp.PrintF( _S( 65, "위력 : %d" ), rSkill.GetPower( nLevel ) );
						AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, strColor);
					}

					strTemp.PrintF( _S( 66, "유효 거리 : %.1f" ), rSkill.GetFireRange() );
					AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, strColor);
					if(rSkill.GetDurTime(nLevel) > 0)
					{
						strTemp.PrintF( _S( 4172, "지속시간 : %d초 " ), rSkill.GetDurTime(nLevel) / 10);
						AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, strColor);
					}
					
					if(rSkill.GetReUseTime() > 0)
					{
						strTemp.PrintF( _S( 4173, "재사용 시간 : %d초 " ), rSkill.GetReUseTime() );
						AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, strColor);
					}

				}
				break;

			case CSkill::ST_PASSIVE:				// Passive
			{
				strTemp.PrintF( _S( 256, "필요 레벨 : %d" ), rSkill.GetLearnLevel( nLevel ) );
				if(n == 1 || nLoopCnt == 1)
				{
					strColor = m_pSelectedSkillSatisfied[nRow] & NOT_SATISFIED_LEVEL ? 0xFF0000FF : 0xFFFF00FF;
				}
				else
				{
					strColor = 0xF2F2F2FF;
				}
				AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, strColor);
				strTemp.PrintF( _S( 257, "필요 SP : %d" ), rSkill.GetLearnSP( nLevel ) ); // 숙련도
				if(n == 1 || nLoopCnt == 1)
				{
					strColor = m_pSelectedSkillSatisfied[nRow] & NOT_SATISFIED_SP ? 0xFF0000FF : 0xFFFF00FF;
				}
				else
				{
					strColor = 0xF2F2F2FF;
				}
				AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, strColor);

				const int iLearnStr = rSkill.GetLearnStr( nLevel );
				const int iLearnDex = rSkill.GetLearnDex( nLevel );
				const int iLearnInt = rSkill.GetLearnInt( nLevel );
				const int iLearnCon = rSkill.GetLearnCon( nLevel );
				
					if( iLearnStr > 0 ) 
					{
						strTemp.PrintF( _S( 1391, "필요 힘 : %d" ), iLearnStr );		// 필요 힘 
						if(n == 1 || nLoopCnt == 1)
						{
							strColor = m_pSelectedSkillSatisfied[nRow] & NOT_SATISFIED_STR ? 0xFF0000FF : 0xFFFF00FF;
						}
						else
						{
							strColor = 0xF2F2F2FF;
						}
						AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, strColor);
					}

					if( iLearnDex > 0 ) 
					{
						strTemp.PrintF( _S( 1392, "필요 민첩 : %d" ), iLearnDex );	// 필요 민첩
						if(n == 1 || nLoopCnt == 1)
						{
							strColor = m_pSelectedSkillSatisfied[nRow] & NOT_SATISFIED_DEX ? 0xFF0000FF : 0xFFFF00FF;
						}
						else
						{
							strColor = 0xF2F2F2FF;
						}
						AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, strColor);
					}
					
					if( iLearnInt > 0 ) 
					{
						strTemp.PrintF( _S( 1393, "필요 지혜 : %d" ), iLearnInt );	// 필요 지혜
						if(n == 1 || nLoopCnt == 1)
						{
							strColor = m_pSelectedSkillSatisfied[nRow] & NOT_SATISFIED_INT ? 0xFF0000FF : 0xFFFF00FF;
						}
						else
						{
							strColor = 0xF2F2F2FF;
						}
						AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, strColor);
					}

					if( iLearnCon > 0 )
					{
						strTemp.PrintF( _S( 1394, "필요 체질 : %d" ), iLearnCon );	// 필요 체질
						if(n == 1 || nLoopCnt == 1)
						{
							strColor = m_pSelectedSkillSatisfied[nRow] & NOT_SATISFIED_CON ? 0xFF0000FF : 0xFFFF00FF;
						}
						else
						{
							strColor = 0xF2F2F2FF;
						}
						AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, strColor);
					}

				// Need skill
				if( bLearnSkill )
				{
					for( i = 0; i < 3; i++ )
					{
						if( nLearnSkillIndex[i] != -1 )
						{
							CSkill	&rNeedSkill = _pNetwork->GetSkillData( nLearnSkillIndex[i] );
							strTemp.PrintF( " : %s Lv.%d", rNeedSkill.GetName(), sbLearnSkillLevel[i] );
							strTemp = _S( 258, "필요 스킬" ) + strTemp;
							if(n == 1 || nLoopCnt == 1)
							{
								strColor = m_pSelectedSkillSatisfied[nRow] & NOT_SATISFIED_SKILL ? 0xFF0000FF : 0xFFFF00FF;
							}
							else
							{
								strColor = 0xF2F2F2FF;
							}
							AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, strColor);
						}
					}
				}

				// Need item
				if( bLearnItem )
				{
					for( i = 0; i < 3; i++ )
					{
						if( nLearnItemIndex[i] != -1 )
						{
							CItemData	&rNeedItem = _pNetwork->GetItemData( nLearnItemIndex[i] );
							strTemp.PrintF( _S( 260, "  %s %d개" ), _pNetwork->GetItemName( nLearnItemIndex[i] ), nLearnItemCount[i] );
							strTemp = _S( 259, "필요 아이템" ) + strTemp;
							if(n == 1 || nLoopCnt == 1)
							{
								strColor = m_pSelectedSkillSatisfied[nRow] & NOT_SATISFIED_ITEM ? 0xFF0000FF : 0xFFFF00FF;
							}
							else
							{
								strColor = 0xF2F2F2FF;
							}
							AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, strColor);
						}
					}
				}

				
				if(n == 1 || nLoopCnt == 1)
				{
					strColor = 0xFFFF00FF;
				}
				else
				{
					strColor = 0xF2F2F2FF;
				}

				if(rSkill.GetDurTime(nLevel) > 0)
				{
					strTemp.PrintF( _S( 4172, "지속시간 : %d초 " ), rSkill.GetDurTime(nLevel) / 10);
					AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, strColor);
				}
				
				if(rSkill.GetReUseTime() > 0)
				{
					strTemp.PrintF( _S( 4173, "재사용 시간 : %d초 " ), rSkill.GetReUseTime() );
					AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, strColor);
				}
			}
			break;
			}
		}

	}
	else // m_nCurrentSkillType == SKILL_SPECIAL
	{

		CSpecialSkill &rSkill = _pNetwork->GetSSkillData( nIndex );

		int nLevel = _pUIMgr->GetCharacterInfo()->GetSkillLevel(nIndex, TRUE);
		
		strTemp.PrintF("%s(%d/%d)",rSkill.GetName(), nLevel, rSkill.GetMaxLevel());
		m_rsSkillName.AddString(strTemp, 0xFFC672FF);

		strTemp = rSkill.GetDescription();
		m_rsSkillDesc.AddString(strTemp, 0xC5C5C5FF);
		
		int nLoopCnt = 0;
		if(nLevel == 0)
		{
			nLoopCnt = 1;
		}
		else
		{
			nLoopCnt = 2;
			nLevel--;
		}
		
		for(int n = 0; n < nLoopCnt; n++, nLevel++)
		{
			// Get learning condition
			int		nLearnSkillIndex;
			SBYTE	sbLearnSkillLevel;
			BOOL	bLearnSkill	= FALSE;
			BOOL	bLearnItem	= FALSE;		
			
			nLearnSkillIndex	= rSkill.GetLearnSkillIndex();
			sbLearnSkillLevel	= rSkill.GetLearnSkillLevel();
			
			if( nLearnSkillIndex != -1 )
				bLearnSkill = TRUE;
			
			strTemp.PrintF( _S( 256, "필요 레벨 : %d" ), rSkill.GetLearnLevel( nLevel ) );
			if(n == 1 || nLoopCnt == 1)
			{
				strColor = m_pSelectedSkillSatisfied[nRow] & NOT_SATISFIED_LEVEL ? 0xFF0000FF : 0xFFFF00FF;
			}
			else
			{
				strColor = 0xF2F2F2FF;
			}
			AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, strColor );
			strTemp.PrintF( _S( 257, "필요 SP : %d" ), rSkill.GetLearnSP( nLevel ) ); // 숙련도
			if(n == 1 || nLoopCnt == 1)
			{
				strColor = m_pSelectedSkillSatisfied[nRow] & NOT_SATISFIED_SP ? 0xFF0000FF : 0xFFFF00FF;
			}
			else
			{
				strColor = 0xF2F2F2FF;
			}
			AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, strColor );
			
			// Need skill
			if( bLearnSkill )
			{
				AddSkillInfoString(SKILLINFO_CURRENT, _S( 258, "필요 스킬" ), 0xBDA99FFF );
				if( nLearnSkillIndex != -1 )
				{
					CSpecialSkill	&rNeedSkill = _pNetwork->GetSSkillData( nLearnSkillIndex );
					strTemp.PrintF( " : %s Lv.%d", rNeedSkill.GetName(), sbLearnSkillLevel );
					if(n == 1 || nLoopCnt == 1)
					{
						if(m_pSelectedSkillSatisfied[nRow] & NOT_SATISFIED_SKILL)
						{
							strColor  = 0xFF0000FF;
						}
						else if(m_pSelectedSkillSatisfied[nRow] & NOT_SATISFIED_SKILL_LEVEL)
						{
							strColor  = 0xFF0000FF;
						}
						else
						{
							strColor =  0xFFFF00FF;
						}
						
					}
					else
					{
						strColor = 0xF2F2F2FF;
					}
					AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, strColor );
				}
			}
		}
	}

	int nNewPosX;
	int nNewPosY;

	m_btnSelectedSkill[nRow].GetAbsPos(nNewPosX, nNewPosY);

	if(nNewPosX + GetSkillInfoWidth() > _pUIMgr->GetDrawPort()->GetWidth())
	{
		nNewPosX -= GetSkillInfoWidth();
	}
	if(nNewPosY + GetSkillInfoHeight() + 34 > _pUIMgr->GetDrawPort()->GetHeight())
	{
		nNewPosY -= GetSkillInfoHeight();
	}
	else
	{
		nNewPosY += 34;
	}

	SetSkillInfoPos(nNewPosX, nNewPosY);

//	if(!m_skillInfoString.m_bVisible)
//	{
//		m_bSkillInfoVisible = FALSE;
//	}
}

//////////////////////////////////////////////////////////////////////////
//
// 현재 스킬이 조건을 만족하는지를 체크합니다.
//
//////////////////////////////////////////////////////////////////////////
int CUISkillLearn::IsSatisfiedSkill(int nSkillIndex, int nLevel, BOOL bSpecial )
{
	int nSatisfied = 0;

	if( nSkillIndex == SKILL_NOT_EXIST )
		return SKILL_NOT_EXIST;
	
	if( !bSpecial )
	{
		CSkill		&rSkill = _pNetwork->GetSkillData( nSkillIndex );

		--nLevel;

		int		nLearnSkillIndex[3];
		SBYTE	sbLearnSkillLevel[3];
		int		nLearnItemIndex[3];
		SQUAD	nLearnItemCount[3];
		BOOL	bLearnSkill = FALSE;
		BOOL	bLearnItem = FALSE;
		
		for( int i = 0; i < 3; i++ )
		{
			nLearnSkillIndex[i] = rSkill.GetLearnSkillIndex( nLevel, i );
			sbLearnSkillLevel[i] = rSkill.GetLearnSkillLevel( nLevel, i );
			nLearnItemIndex[i] = rSkill.GetLearnItemIndex( nLevel, i );
			nLearnItemCount[i] = rSkill.GetLearnItemCount( nLevel, i );
			
			if( nLearnSkillIndex[i] != -1 )
				bLearnSkill = TRUE;
			if( nLearnItemIndex[i] != -1 )
				bLearnItem = TRUE;
		}

		// 필요 레벨 체크
		if(rSkill.GetLearnLevel( nLevel ) > _pNetwork->MyCharacterInfo.level)
			nSatisfied += NOT_SATISFIED_LEVEL;
		// 필요 숙련도 체크
		if(rSkill.GetLearnSP( nLevel ) > (_pNetwork->MyCharacterInfo.sp / 10000))
			nSatisfied += NOT_SATISFIED_SP;
		// 필요 능력치 체크

		const int iLearnStr = rSkill.GetLearnStr( nLevel );
		const int iLearnDex = rSkill.GetLearnDex( nLevel );
		const int iLearnInt = rSkill.GetLearnInt( nLevel );
		const int iLearnCon = rSkill.GetLearnCon( nLevel );

		if( iLearnStr > 0 ) 
		{
			if( iLearnStr > _pNetwork->MyCharacterInfo.str )
				nSatisfied += NOT_SATISFIED_STR;
		}

		if( iLearnDex > 0 ) 
		{
			if( iLearnDex > _pNetwork->MyCharacterInfo.dex )
				nSatisfied += NOT_SATISFIED_DEX;
		}

		if( iLearnInt > 0 ) 
		{
			if( iLearnInt > _pNetwork->MyCharacterInfo.intel )
				nSatisfied += NOT_SATISFIED_INT;
		}

		if( iLearnCon > 0 ) 
		{
			if( iLearnCon > _pNetwork->MyCharacterInfo.con )
				nSatisfied += NOT_SATISFIED_CON;
		}

		// 필요 스킬 체크
		if( bLearnSkill )
		{
			for( i = 0; i < 3; i++ )
			{
				if( nLearnSkillIndex[i] != -1 )
				{
					//내 스킬 가져오는 방법을 찾자.
					// 캐릭터 인포쪽인가?
					int nCurrentSkillLevel = _pUIMgr->GetCharacterInfo()->GetSkillLevel(nLearnSkillIndex[i], FALSE);
					if(sbLearnSkillLevel[i] > nCurrentSkillLevel)
					{
						nSatisfied += NOT_SATISFIED_SKILL;
						break;
					}
					
				}
			}
		}

		// 필요 아이템 체크
		if( bLearnItem )
		{
			for( i = 0; i < 3; i++ )
			{
				if( nLearnItemIndex[i] != -1 )
				{
					SQUAD nCurrentItemCount = _pUIMgr->GetInventory()->GetItemCount(nLearnItemIndex[i]);
					if(nLearnItemCount[i] > nCurrentItemCount)
					{
						nSatisfied += NOT_SATISFIED_ITEM;
						break;
					}
					
				}
			}
		}

	}
	else
	{
		CSpecialSkill &rSkill = _pNetwork->GetSSkillData( nSkillIndex );

		--nLevel;

		// Get learning condition
		int		nLearnSkillIndex;
		SBYTE	sbLearnSkillLevel;
		BOOL	bLearnSkill	= FALSE;
		BOOL	bLearnItem	= FALSE;		
		
		nLearnSkillIndex	= rSkill.GetLearnSkillIndex();
		sbLearnSkillLevel	= rSkill.GetLearnSkillLevel();
		
		if( nLearnSkillIndex != -1 )
			bLearnSkill = TRUE;
		
		// 필요 레벨 체크
		if(rSkill.GetLearnLevel( nLevel ) > _pNetwork->MyCharacterInfo.level)
			nSatisfied += NOT_SATISFIED_LEVEL;
		// 필요 숙련도 체크
		if(rSkill.GetLearnSP( nLevel ) > (_pNetwork->MyCharacterInfo.sp / 10000))
			nSatisfied += NOT_SATISFIED_SP;

		// 필요 스킬 체크
		if( bLearnSkill )
		{
			if( nLearnSkillIndex != -1 )
			{
				int nCurrentSkillLevel = _pUIMgr->GetCharacterInfo()->GetSkillLevel(nLearnSkillIndex, TRUE);
				if(sbLearnSkillLevel > nCurrentSkillLevel)
				{
					if(nCurrentSkillLevel == 0)
					{
						nSatisfied += NOT_SATISFIED_SKILL;
					}
					else
					{
						nSatisfied += NOT_SATISFIED_SKILL_LEVEL;
					}
				}

			}
		}

	}



	return nSatisfied;

}


void CUISkillLearn::UpdateSkillLearn()
{
	m_strDesc.Clear();

	for( int iRow = 0; iRow < SKILLLEARN_NEW_SLOT_TOTAL; iRow++ )
	{

		m_nActiveSkillSatisfied[iRow] = 0;
		m_nPassiveSkillSatisfied[iRow] = 0;
		m_nSpecialSkillSatisfied[iRow] = 0;
	}

	for(iRow = 0; iRow < SKILLLEARN_NEW_SLOT_TOTAL; iRow++)
	{
		if(m_btnActiveSkills[iRow].IsEmpty())
			break;
		m_nActiveSkillSatisfied[iRow] = IsSatisfiedSkill( m_btnActiveSkills[iRow].GetSkillIndex(), m_btnActiveSkills[iRow].GetSkillLevel(), FALSE  );
	}

	for(iRow = 0; iRow < SKILLLEARN_NEW_SLOT_TOTAL; iRow++)
	{
		if(m_btnPassiveSkills[iRow].IsEmpty())
			break;
		m_nPassiveSkillSatisfied[iRow] = IsSatisfiedSkill( m_btnPassiveSkills[iRow].GetSkillIndex(), m_btnPassiveSkills[iRow].GetSkillLevel(), FALSE  );
	}

	for(iRow = 0; iRow < SKILLLEARN_NEW_SLOT_TOTAL; iRow++)
	{
		if(m_btnSpecialSkills[iRow].IsEmpty())
			break;
		m_nSpecialSkillSatisfied[iRow] = IsSatisfiedSkill( m_btnSpecialSkills[iRow].GetSkillIndex(), m_btnSpecialSkills[iRow].GetSkillLevel(), TRUE  );
	}
}

//
// 스킬 습득에서만 사용 현재 스킬 버튼을 지정
//
void CUISkillLearn::SetCurrentSkill(int skill)
{
	m_nCurrentSkillType = skill;

	m_nSelectedSkillID = -1;

	switch(skill)
	{
		case SKILL_ACTIVE:
			m_btnSelectedSkill = m_btnActiveSkills;
			m_pSelectedSkillSatisfied = m_nActiveSkillSatisfied;
			m_sbScrollBar.SetCurItemCount( m_nNumOfActiveSkill );
			break;
		case SKILL_PASSIVE:
			m_btnSelectedSkill = m_btnPassiveSkills;
			m_pSelectedSkillSatisfied = m_nPassiveSkillSatisfied;
			m_sbScrollBar.SetCurItemCount( m_nNumOfPassiveSkill );
			break;
		case SKILL_SPECIAL:
			m_btnSelectedSkill = m_btnSpecialSkills;
			m_pSelectedSkillSatisfied = m_nSpecialSkillSatisfied;
			m_sbScrollBar.SetCurItemCount( m_nNumOfSpecialSkill );
			break;
	}
}

#define EXTENSION_LEVEL		(31)
extern INDEX g_iCountry;

#ifdef HELP_SYSTEM_1
// [KH_07044] 3차 도움말 관련 추가
extern INDEX g_iShowHelp1Icon;
#endif

//////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------
// Name : CUISkillLearn()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUISkillLearn::CUISkillLearn()
{
	m_nSelActiveSkillID		= -1;
	m_nSelPassiveSkillID	= -1;
	m_nSelMemorizeSkillID	= -1;
	m_nSelSpecialSkillID	= -1;

	m_nCurrentTab			= SLEARN_TAB_ACTIVE;
	m_nSSkillType			= -1;
	m_iSelChangeJob			= -1;
	m_bUseCard				= FALSE;
#ifdef NEW_USER_INTERFACE
	m_nSelectedSkillID = -1;
	m_nNumOfActiveSkill = 0;
	m_nNumOfPassiveSkill = 0;
	m_nNumOfSpecialSkill = 0;
	m_nCurrentSkillType = -1;
	m_bUseCard				= FALSE;
	m_nSSkillType = -1;
	m_bSkillInfoVisible = FALSE;
	m_ptdButtonTexture = NULL;
#endif
}

// ----------------------------------------------------------------------------
// Name : ~CUISkillLearn()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUISkillLearn::~CUISkillLearn()
{
	if (m_ptdButtonTexture)
	{
		_pTextureStock->Release(m_ptdButtonTexture);
	}
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUISkillLearn::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CreateNewSkillLearn(pParentWnd, nX ,nY, nWidth, nHeight);
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUISkillLearn::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUISkillLearn::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

BOOL CheckSummonSkill( INDEX iSkillIndex )
{
	CSkill	&rSelSkill = _pNetwork->GetSkillData( iSkillIndex );
	if( rSelSkill.GetSorcererFlag() & ( SSF_USE_FIRE | SSF_USE_WIND | SSF_USE_EARTH | SSF_USE_WATER ) )
	{
		return TRUE;
	}
	else
	{
		const int iSummonSkillCount = sizeof( _aSummonSkill ) / sizeof(int);
		for( int i = 0; i < iSummonSkillCount; ++i )
		{
			if( _aSummonSkill[i] == iSkillIndex )
			{					
				return TRUE;
			}
		}
	}
	return FALSE;
}

// ----------------------------------------------------------------------------
// Name : InitSkillLearn()
// Desc :
// ----------------------------------------------------------------------------
void CUISkillLearn::InitSkillLearn( BOOL bSpecial )
{
	// Reset description
	m_lbSkillDesc.ResetAllStrings();

#ifdef NEW_USER_INTERFACE
	m_strDesc.Clear();
#endif
	
	// Reset buttons
	for( int iRow = 0; iRow < SKILLLEARN_NEW_SLOT_TOTAL; iRow++ )
	{
		m_btnActiveSkills[iRow].InitBtn();
		m_btnPassiveSkills[iRow].InitBtn();
#ifdef ADJUST_MEMORIZE_SKILL
		m_btnMemorizeSkills[iRow].InitBtn();
#endif
		m_btnSpecialSkills[iRow].InitBtn();
	}
	
	// Collect skills
	INDEX	ctPosActive = 0, ctPosPassive = 0, ctPosSpecial = 0, ctPosMemorize = 0;				// Possible
	INDEX	ctImposActive = 0, ctImposPassive = 0, ctImposSpecial = 0, ctImposMemorize = 0;		// Impossible
	
	if( !bSpecial )
	{
		std::vector<sCollectSkill>		vectorPosActiveSkill;
		std::vector<sCollectSkill>		vectorImposActiveSkill;
		std::vector<sCollectSkill>		vectorPosPassiveSkill;
		std::vector<sCollectSkill>		vectorImposPassiveSkill;
		std::vector<sCollectSkill>		vectorPosMemorizeSkill;
		std::vector<sCollectSkill>		vectorImposMemorizeSkill;
		
		vectorPosActiveSkill.resize(SKILLLEARN_NEW_SLOT_TOTAL);
		vectorImposActiveSkill.resize(SKILLLEARN_NEW_SLOT_TOTAL);
		vectorPosPassiveSkill.resize(SKILLLEARN_NEW_SLOT_TOTAL);
		vectorImposPassiveSkill.resize(SKILLLEARN_NEW_SLOT_TOTAL);
		vectorPosMemorizeSkill.resize(SKILLLEARN_NEW_SLOT_TOTAL);
		vectorImposMemorizeSkill.resize(SKILLLEARN_NEW_SLOT_TOTAL);
		
		for( int iSkill = 1; iSkill <= _pNetwork->wo_iNumOfSkill; iSkill++ )
		{
			CSkill	&rSkill = _pNetwork->GetSkillData( iSkill );
			
			// If job is different
			if( ( rSkill.GetJob() != _pNetwork->MyCharacterInfo.job ) )
				continue;
			
			if( rSkill.GetJob2() > 0 && ( rSkill.GetJob2() != _pNetwork->MyCharacterInfo.job2 ) )
				continue;

			// 소환스킬 인덱스 찾구,
			// 소환수 사용 스킬.
			if( _pNetwork->MyCharacterInfo.job == SORCERER && 
				_pNetwork->MyCharacterInfo.job2 == 2 && 
				rSkill.GetType() != CSkill::ST_PASSIVE )
			{
				if( CheckSummonSkill( iSkill ) )
					continue;				
			}
			
			int		nSkillIndex = rSkill.GetIndex();
			SBYTE	sbSkillLevel = _pUIMgr->GetCharacterInfo()->GetSkillLevel( nSkillIndex, FALSE );
			
			// If this skill is already max level
			if( sbSkillLevel >= rSkill.GetMaxLevel() )
				continue;
			
			if( sbSkillLevel < 1 )
				sbSkillLevel = 0;
			ULONG	ulNeedCharLevel = rSkill.GetLearnLevel( sbSkillLevel );
			sbSkillLevel++;
			
			switch( rSkill.GetType() )
			{
			case CSkill::ST_MELEE:				// Active skill
			case CSkill::ST_RANGE:
			case CSkill::ST_MAGIC:
				{
					if( ulNeedCharLevel <= _pNetwork->MyCharacterInfo.level )
					{
						vectorPosActiveSkill[ctPosActive++].SetData(nSkillIndex, sbSkillLevel, ulNeedCharLevel);				
					}
					else
					{
						vectorImposActiveSkill[ctImposActive++].SetData(nSkillIndex, sbSkillLevel, ulNeedCharLevel);					
					}
				}
				break;
				
			case CSkill::ST_PASSIVE:			// Passive skill
				{
					if( ulNeedCharLevel <= _pNetwork->MyCharacterInfo.level )
					{
						vectorPosPassiveSkill[ctPosPassive++].SetData(nSkillIndex, sbSkillLevel, ulNeedCharLevel);		
					}
					else
					{
						vectorImposPassiveSkill[ctImposPassive++].SetData(nSkillIndex, sbSkillLevel, ulNeedCharLevel);					
					}
				}
				break;
			}
		}
		
		std::sort(vectorPosActiveSkill.begin(), vectorPosActiveSkill.end());
		std::sort(vectorImposActiveSkill.begin(), vectorImposActiveSkill.end());
		std::sort(vectorPosPassiveSkill.begin(), vectorPosPassiveSkill.end());
		std::sort(vectorImposPassiveSkill.begin(), vectorImposPassiveSkill.end());
#ifdef ADJUST_MEMORIZE_SKILL
		std::sort(vectorPosMemorizeSkill.begin(), vectorPosMemorizeSkill.end());
		std::sort(vectorImposMemorizeSkill.begin(), vectorImposMemorizeSkill.end());
#endif

#ifdef NEW_USER_INTERFACE
		iSkill = 0;
		// Possible active skill
		for( iRow = 0; iRow < ctPosActive; iRow++ )
		{
			m_btnActiveSkills[iSkill].SetSkillInfo( vectorPosActiveSkill[iRow].lSkillIndex, vectorPosActiveSkill[iRow].sbSkillLevel );
			m_nActiveSkillSatisfied[iSkill] = IsSatisfiedSkill( vectorPosActiveSkill[iRow].lSkillIndex, vectorPosActiveSkill[iRow].sbSkillLevel, FALSE );
			iSkill++;
		}
		// Impossible active skill
		for( iRow = 0; iRow < ctImposActive; iRow++ )
		{
			m_btnActiveSkills[iSkill].SetSkillInfo( vectorImposActiveSkill[iRow].lSkillIndex, vectorImposActiveSkill[iRow].sbSkillLevel );
			m_nActiveSkillSatisfied[iSkill] = IsSatisfiedSkill( vectorImposActiveSkill[iRow].lSkillIndex, vectorImposActiveSkill[iRow].sbSkillLevel, FALSE  );
			iSkill++;
		}

		m_nNumOfActiveSkill = iSkill;

		// Add passive skill buttons
		iSkill = 0;
		// Possible passive skill
		for( iRow = 0; iRow < ctPosPassive; iRow++ )
		{
			m_btnPassiveSkills[iSkill].SetSkillInfo( vectorPosPassiveSkill[iRow].lSkillIndex, vectorPosPassiveSkill[iRow].sbSkillLevel );
			m_nPassiveSkillSatisfied[iSkill] = IsSatisfiedSkill( vectorPosPassiveSkill[iRow].lSkillIndex, vectorPosPassiveSkill[iRow].sbSkillLevel, FALSE  );
			iSkill++;
		}
		// Impossible passive skill
		for( iRow = 0; iRow < ctImposPassive; iRow++ )
		{
			m_btnPassiveSkills[iSkill].SetSkillInfo( vectorImposPassiveSkill[iRow].lSkillIndex, vectorImposPassiveSkill[iRow].sbSkillLevel );
			m_nPassiveSkillSatisfied[iSkill] = IsSatisfiedSkill( vectorImposPassiveSkill[iRow].lSkillIndex, vectorImposPassiveSkill[iRow].sbSkillLevel, FALSE  );
			iSkill++;
		}

		m_nNumOfPassiveSkill = iSkill;
		// Set scroll bar

		m_sbScrollBar.SetScrollPos( 0 );
		SetCurrentSkill(m_nCurrentSkillType);
#else
		// Add active skill buttons
		iSkill = 0;
		// Possible active skill
		for( iRow = 0; iRow < ctPosActive; iRow++ )
			m_btnActiveSkills[iSkill++].SetSkillInfo( vectorPosActiveSkill[iRow].lSkillIndex, vectorPosActiveSkill[iRow].sbSkillLevel );
		// Impossible active skill
		for( iRow = 0; iRow < ctImposActive; iRow++ )
			m_btnActiveSkills[iSkill++].SetSkillInfo( vectorImposActiveSkill[iRow].lSkillIndex, vectorImposActiveSkill[iRow].sbSkillLevel );
		// Set active scroll bar
		m_sbActiveSkillIcon.SetScrollPos( 0 );
		m_sbActiveSkillIcon.SetCurItemCount( iSkill );
		
		// Add passive skill buttons
		iSkill = 0;
		// Possible passive skill
		for( iRow = 0; iRow < ctPosPassive; iRow++ )
			m_btnPassiveSkills[iSkill++].SetSkillInfo( vectorPosPassiveSkill[iRow].lSkillIndex, vectorPosPassiveSkill[iRow].sbSkillLevel );
		// Impossible passive skill
		for( iRow = 0; iRow < ctImposPassive; iRow++ )
			m_btnPassiveSkills[iSkill++].SetSkillInfo( vectorImposPassiveSkill[iRow].lSkillIndex, vectorImposPassiveSkill[iRow].sbSkillLevel );
		// Set passive scroll bar
		m_sbPassiveSkillIcon.SetScrollPos( 0 );
		m_sbPassiveSkillIcon.SetCurItemCount( iSkill );

	#ifdef ADJUST_MEMORIZE_SKILL
		// Add memorize skill buttons
		iSkill = 0;
		// Possible memorize skill
		for( iRow = 0; iRow < ctPosMemorize; iRow++ )
			m_btnMemorizeSkills[iSkill++].SetSkillInfo( vectorPosMemorizeSkill[iRow].lSkillIndex, vectorPosMemorizeSkill[iRow].sbSkillLevel );
		// Impossible memorize skill
		for( iRow = 0; iRow < ctImposMemorize; iRow++ )
			m_btnMemorizeSkills[iSkill++].SetSkillInfo( vectorImposMemorizeSkill[iRow].lSkillIndex, vectorImposMemorizeSkill[iRow].sbSkillLevel );
		// Set memorize scroll bar
		m_sbMemorizeSkillIcon.SetScrollPos( 0 );
		m_sbMemorizeSkillIcon.SetCurItemCount( iSkill );
	#endif
#endif
	}
	// Special Skill
	else
	{
		std::vector<sCollectSkill>		vectorPosSpecialSkill;
		std::vector<sCollectSkill>		vectorImposSpecialSkill;
		
		vectorPosSpecialSkill.resize(SKILLLEARN_NEW_SLOT_TOTAL);		
		vectorImposSpecialSkill.resize(SKILLLEARN_NEW_SLOT_TOTAL);
		
		// Special Skill
		for( int iSkill = 1; iSkill <= _pNetwork->wo_iNumOfSSkill; iSkill++ )
		{
			CSpecialSkill	&rSkill = _pNetwork->GetSSkillData( iSkill );
			
			// 방어구제작
			if(m_nSSkillType == SSKILL_MAKE_WEAR 
				//||
				//m_nSSkillType == SSKILL_MAKE_G_B || 
				//m_nSSkillType == SSKILL_MAKE_ARMOR || 
				//m_nSSkillType == SSKILL_MAKE_H_S
				)
			{
				if( rSkill.GetType() != SSKILL_MAKE_WEAR 
					//&&
					//rSkill.GetType() != SSKILL_MAKE_G_B &&
					//rSkill.GetType() != SSKILL_MAKE_ARMOR &&
					//rSkill.GetType() != SSKILL_MAKE_H_S
					)
					continue;
			}
			// If skill is different
			else if( rSkill.GetType() != m_nSSkillType)
				continue;
			
			// 순위 체크...
			const int iPreference = rSkill.GetPreference();
			if(iPreference != -1)
			{
				int iIndex = _pUIMgr->GetCharacterInfo()->CheckSSkill( rSkill.GetType() );
				if(iIndex != -1)
				{
					if( iPreference < _pNetwork->GetSSkillData(iIndex).GetPreference() )
						continue;
				}
			}
			
			int		nSkillIndex		= rSkill.GetIndex();
			SBYTE	sbSkillLevel	= _pUIMgr->GetCharacterInfo()->GetSkillLevel( nSkillIndex, TRUE );
			
			// If this skill is already max level
			if( sbSkillLevel >= rSkill.GetMaxLevel() )
				continue;
			
			if( sbSkillLevel < 1 )
				sbSkillLevel = 0;
			ULONG	ulNeedCharLevel = rSkill.GetLearnLevel( sbSkillLevel );
			sbSkillLevel++;
			
			// Special Skill
			if( ulNeedCharLevel <= _pNetwork->MyCharacterInfo.level )
			{
				vectorPosSpecialSkill[ctPosSpecial++].SetData(nSkillIndex, sbSkillLevel, ulNeedCharLevel);
			}
			else
			{
				vectorImposSpecialSkill[ctImposSpecial++].SetData(nSkillIndex, sbSkillLevel, ulNeedCharLevel);	
			}
		}
		
		std::sort(vectorPosSpecialSkill.begin(), vectorPosSpecialSkill.end());
		std::sort(vectorImposSpecialSkill.begin(), vectorImposSpecialSkill.end());
#ifdef NEW_USER_INTERFACE
		iSkill = 0;
		for( iRow = 0; iRow < ctPosSpecial; iRow++ )
		{
			m_btnSpecialSkills[iSkill].SetSkillInfo( vectorPosSpecialSkill[iRow].lSkillIndex, vectorPosSpecialSkill[iRow].sbSkillLevel, TRUE );
			m_nSpecialSkillSatisfied[iSkill] = IsSatisfiedSkill( vectorPosSpecialSkill[iRow].lSkillIndex, vectorPosSpecialSkill[iRow].sbSkillLevel, TRUE  );
			iSkill++;
		}
		for( iRow = 0; iRow < ctImposSpecial; iRow++ )
		{
			m_btnSpecialSkills[iSkill].SetSkillInfo( vectorImposSpecialSkill[iRow].lSkillIndex, vectorImposSpecialSkill[iRow].sbSkillLevel, TRUE );
			m_nSpecialSkillSatisfied[iSkill] = IsSatisfiedSkill( vectorImposSpecialSkill[iRow].lSkillIndex, vectorImposSpecialSkill[iRow].sbSkillLevel, TRUE  );
			iSkill++;
		}

		m_nNumOfSpecialSkill = iSkill;
		// Set scroll bar

		m_sbScrollBar.SetScrollPos( 0 );
		SetCurrentSkill(SKILL_SPECIAL);
#else
		// Add special skill buttons
		iSkill = 0;
		// Possible special skill
		for( iRow = 0; iRow < ctPosSpecial; iRow++ )
			m_btnSpecialSkills[iSkill++].SetSkillInfo( vectorPosSpecialSkill[iRow].lSkillIndex, vectorPosSpecialSkill[iRow].sbSkillLevel, TRUE );
		// Impossible special skill
		for( iRow = 0; iRow < ctImposSpecial; iRow++ )
			m_btnSpecialSkills[iSkill++].SetSkillInfo( vectorImposSpecialSkill[iRow].lSkillIndex, vectorImposSpecialSkill[iRow].sbSkillLevel, TRUE );
		// Set special scroll bar
		m_sbSpecialSkillIcon.SetScrollPos( 0 );
		m_sbSpecialSkillIcon.SetCurItemCount( iSkill );
#endif
	}
}

// ----------------------------------------------------------------------------
// Name : OpenSkillLearn()
// Desc : nMasterType은 일반 스킬일때는 직업, 특수 스킬일때는 특수 스킬타입이 됩니다.
// ----------------------------------------------------------------------------
void CUISkillLearn::OpenSkillLearn( int iMobIndex, BOOL bHasQuest, FLOAT fX, FLOAT fZ )
{
	// If this is already exist
	if( _pUIMgr->DoesMessageBoxLExist( MSGLCMD_SKILLLEARN_REQ ) || IsVisible() )
		return;
	
	// Special Skill
	if( _pUIMgr->DoesMessageBoxLExist( MSGLCMD_SSKILLLEARN_REQ ) || IsVisible() )
		return;

#ifdef NEW_USER_INTERFACE
	m_rcIcons.SetRect( 7, 53, 203, SKILLLEARN_NEW_HEIGHT - 70 );
	m_sbScrollBar.SetPos(232, 67);
	m_sbScrollBar.SetHeight(213);
	m_btnClose.SetPos( 232, 4);
	SetWidth(SKILLLEARN_NEW_WIDTH);
	SetHeight(SKILLLEARN_NEW_HEIGHT);
	m_sbScrollBar.SetItemsPerPage( SKILLLEARN_NEW_SLOT_ROW );

	m_rcTitle.SetRect( 0, 0, SKILLLEARN_NEW_WIDTH, 36 );
	m_rcTab.SetRect( 6, 37 , 251, 57 );

	m_nSelectedSkillID = -1;
	m_nNumOfActiveSkill = 0;
	m_nNumOfPassiveSkill = 0;
	m_nCurrentSkillType = 0;
#endif

	// Set position of target npc
	m_fNpcX = fX;
	m_fNpcZ = fZ;

	CMobData& MD	= _pNetwork->GetMobData(iMobIndex);
	m_nSSkillType	= MD.GetSpecialSkillMaster();
	CTString	strNpcName = _pNetwork->GetMobName(iMobIndex);
	m_bSpecial		= MD.IsSSkillMaster() ? TRUE : FALSE;

	if( MD.IsSSkillMaster() )		// 특수 스킬 마스터일때...
	{
		// FIXME : 아래와 같이 메세지가 그때 그때 다르면 대략 낭패.
		switch( m_nSSkillType )
		{
		case SSKILL_MINING:			// 채굴
			{
				// Create skill learn message box
				_pUIMgr->CreateMessageBoxL( _S( 630, "채굴 스킬" ), UI_SKILLLEARN, MSGLCMD_SSKILLLEARN_REQ );		

				_pUIMgr->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, strNpcName, -1, 0xE18600FF );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 631, "광석을 채굴하는거는 그리 어렵지 않아." ), -1, 0xA3A1A3FF );		
				_pUIMgr->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 632, "하지만 얼마만큼 빠른 시간에 많은 양을 캐내느냐가 실력을 좌우 하지." ), -1, 0xA3A1A3FF );		
			}
			break;
		case SSKILL_GATHERING:		// 채집
			{
				// Create skill learn message box
				_pUIMgr->CreateMessageBoxL( _S( 633, "채집 스킬" ), UI_SKILLLEARN, MSGLCMD_SSKILLLEARN_REQ );		

				_pUIMgr->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, strNpcName, -1, 0xE18600FF );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 634, "크락을 채집할 때도 많은 기술이 필요하지." ), -1, 0xA3A1A3FF );		
				_pUIMgr->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 635, "기술이 좋으면 좋을수록 더 많은 양을 채집하고 잘만 하면 보기 힘들다는 크락의 꽃도 얻을 수 있다네." ), -1, 0xA3A1A3FF );		
			}
			break;
		case SSKILL_CHARGE:			// 차지
			{
				// Create skill learn message box
				_pUIMgr->CreateMessageBoxL( _S( 636, "차지 스킬" ), UI_SKILLLEARN, MSGLCMD_SSKILLLEARN_REQ );		

				_pUIMgr->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, strNpcName, -1, 0xE18600FF );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 637, "차지 기술에 대해 좀더 알고 싶습니까?" ), -1, 0xA3A1A3FF );		
				_pUIMgr->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 638, "좀 더 고급의 차지 기술을 배운다면 많은양의 원소를 얻을 수 있을 것입니다.  배워 보시겠습니까?" ), -1, 0xA3A1A3FF );	
			}
			break;
		case SSKILL_STONE:			// 광석정련술
			{
				// Create skill learn message box
				_pUIMgr->CreateMessageBoxL( _S( 639, "스톤정련 스킬" ), UI_SKILLLEARN, MSGLCMD_SSKILLLEARN_REQ );		

				_pUIMgr->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, strNpcName, -1, 0xE18600FF );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 640, "스톤을 정련 하는 것은 아무나 하는 것이 아냐! 자네가 배워 보겠다고?" ), -1, 0xA3A1A3FF );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 641, "요즘 젊은이들은 스톤 정련 하는 것을 너무 쉽게 보는 경향이 있어." ), -1, 0xA3A1A3FF );		
			}
			break;
		case SSKILL_PLANT:			// 식물가공술
			{
				// Create skill learn message box
				_pUIMgr->CreateMessageBoxL( _S( 642, "식물가공 스킬" ), UI_SKILLLEARN, MSGLCMD_SSKILLLEARN_REQ );		

				_pUIMgr->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, strNpcName, -1, 0xE18600FF );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 643, "크낙을 이용해 옷을 만들고 싶다고? 정신이 나갔군." ), -1, 0xA3A1A3FF );		
				_pUIMgr->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 644, "크낙으로 옷까지 만드려면 엄청나게 오랜 시간이 걸려! 과연 당신이 할 수 있을까?" ), -1, 0xA3A1A3FF );		
			}
			break;
		case SSKILL_ELEMENT:		// 원소정제술
			{
				// Create skill learn message box
				_pUIMgr->CreateMessageBoxL( _S( 645, "원소정제 스킬" ), UI_SKILLLEARN, MSGLCMD_SSKILLLEARN_REQ );		

				_pUIMgr->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, strNpcName, -1, 0xE18600FF );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 646, "원소를 정제 하려면 경건한 마음과 많은 인내가 필요 합니다." ), -1, 0xA3A1A3FF );		
				_pUIMgr->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 647, "하지만 정제술을 배우기만 한다면 그에 상응하는 보상을 받을 수 있을 것입니다. 자연은 거짓말을 하지 않으니까요." ), -1, 0xA3A1A3FF );		
			}
			break;

		case SSKILL_MAKE_WEAPON:	// 무기제작
			{
				// Create skill learn message box
				_pUIMgr->CreateMessageBoxL( _S( 648, "무기제작 스킬" ), UI_SKILLLEARN, MSGLCMD_SSKILLLEARN_REQ );		

				_pUIMgr->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, strNpcName, -1, 0xE18600FF );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 649, "이런! 또 무기 제작을 배우고 싶어서 온 친구군." ), -1, 0xA3A1A3FF );		
				_pUIMgr->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 650, "난 더 이상 무기를 만들지 않아 ! 더 이상 예전의 내가 아니라고! 말로 해도 소용 없군." ), -1, 0xA3A1A3FF );		
			}
			break;

		case SSKILL_MAKE_POTINO: // 물약 제작
			{
				// Create skill learn message box
				_pUIMgr->CreateMessageBoxL( _S( 767, "물약 제작 스킬" ), UI_SKILLLEARN, MSGLCMD_SSKILLLEARN_REQ );

				_pUIMgr->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, strNpcName, -1, 0xE18600FF );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 768, "물약 제조는 조심스럽고도 어려운 작업입니다." ), -1, 0xA3A1A3FF );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 769, "물약을 찾는 사람은 많은데 만들 사람이 없는 실정이기도 합니다. 힘들지만 물약 제작기술을 배워 보시겠나요?" ), -1, 0xA3A1A3FF );
			}
			break;
		case SSKILL_MAKE_WEAR:
		case SSKILL_MAKE_G_B:
		case SSKILL_MAKE_ARMOR:
		case SSKILL_MAKE_H_S:
			{
				// Create skill learn message box
				_pUIMgr->CreateMessageBoxL( _S( 651, "방어구제작 스킬" ), UI_SKILLLEARN, MSGLCMD_SSKILLLEARN_REQ );		

				_pUIMgr->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, strNpcName, -1, 0xE18600FF );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 652, "조금이나마 편이 살고 싶다면 방어구 제작 기술을 배워 보라구!" ), -1, 0xA3A1A3FF );		
				_pUIMgr->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 653, "요즘은 수요가 많아서 아주 할만 하다구!" ), -1, 0xA3A1A3FF );		
			}
			break;
		}
		
		CTString strMessage;		
		strMessage.PrintF( _S( 1218, "스킬을 습득한다." ) );		
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, FALSE, strMessage, SKILL_LEARN );

		if( bHasQuest )
		{
#ifdef	NEW_QUESTBOOK
			// 2009. 05. 27 김정래
			// 이야기한다 변경 처리
			CUIQuestBook::AddQuestListToMessageBoxL(MSGLCMD_SSKILLLEARN_REQ);				
#else
			strMessage.PrintF( _S( 1053, "이야기한다." ) );		
			_pUIMgr->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, FALSE, strMessage, SKILL_TALK );
#endif

		}

#ifdef HELP_SYSTEM_1			
		strMessage.PrintF(_S( 1748, "NPC 안내" ));
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, FALSE, strMessage, SKILL_NPC_HELP );
#endif

		strMessage.PrintF( _S( 1220, "취소한다." ) );		
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, FALSE, strMessage );
	}
	else						// 일반 스킬 마스터일때...
	{

		const int iJob = _pNetwork->MyCharacterInfo.job;
		// If job is different
		if( MD.GetSkillMaster() != iJob )
		{
			// WSS_BUG_FIX 070802 ------>><<
			if( _pUIMgr->DoesMessageBoxExist( MSGCMD_SKILLLEARN_NOTIFY ) ) { return;}

			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 270, "스킬" ), UMBS_OK,
				UI_SKILLLEARN, MSGCMD_SKILLLEARN_NOTIFY );
			MsgBoxInfo.AddString( _S(3589,"해당 직업의 스킬은 습득할 수 없습니다.") );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );
			return;
		}
		
		// Create skill learn message box
		_pUIMgr->CreateMessageBoxL( _S( 270, "스킬" ), UI_SKILLLEARN, MSGLCMD_SKILLLEARN_REQ );

		_pUIMgr->AddMessageBoxLString( MSGLCMD_SKILLLEARN_REQ, TRUE, strNpcName, -1, 0xE18600FF );
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SKILLLEARN_REQ, TRUE, _S( 271, "끊임없이 노력하는 자만이 진정한 강함을 손에 넣을 수 있는 법!\n\n그런 강인한 의지가 있는 자를 인도하는 것이 내가 하는 일이지.\n" ), -1, 0xA3A1A3FF );
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SKILLLEARN_REQ, TRUE, _S( 272, "현재의 자네 수준에 맞는 스킬을 선택하여 익히도록 하게나." ), -1, 0xA3A1A3FF );

		CTString strMessage;
		strMessage.PrintF( _S( 1218, "스킬을 습득한다." ) );		
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SKILLLEARN_REQ, FALSE, strMessage, SKILL_LEARN );

		if( bHasQuest )
		{
#ifdef	NEW_QUESTBOOK
			// 2009. 05. 27 김정래
			// 이야기한다 변경 처리
			CUIQuestBook::AddQuestListToMessageBoxL(MSGLCMD_SKILLLEARN_REQ);				
#else
			strMessage.PrintF( _S( 1053, "이야기한다." ) );	
			_pUIMgr->AddMessageBoxLString( MSGLCMD_SKILLLEARN_REQ, FALSE, strMessage, SKILL_TALK );
#endif
		}

		// 전직 가능 레벨이고 전직한 상태가 아닐때...
		if( _pNetwork->MyCharacterInfo.level >= EXTENSION_LEVEL && _pNetwork->MyCharacterInfo.job2 == 0 )
		{
			strMessage.PrintF( _S( 1223, "[%s]로 전직" ), JobInfo().GetExtensionName( iJob, 0 ) );	
			_pUIMgr->AddMessageBoxLString( MSGLCMD_SKILLLEARN_REQ, FALSE, strMessage, SKILL_JOB_1 );

			strMessage.PrintF( _S( 1223, "[%s]로 전직" ), JobInfo().GetExtensionName( iJob, 1 ) );	
			_pUIMgr->AddMessageBoxLString( MSGLCMD_SKILLLEARN_REQ, FALSE, strMessage, SKILL_JOB_2 );
		}

#ifdef HELP_SYSTEM_1
		strMessage.PrintF(_S( 1748, "NPC 안내" ));
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SKILLLEARN_REQ, FALSE, strMessage, SKILL_NPC_HELP );
#endif 

		strMessage.PrintF( _S( 1220, "취소한다." ) );		
		_pUIMgr->AddMessageBoxLString( MSGLCMD_SKILLLEARN_REQ, FALSE, strMessage );

		
	}
	
	m_nSelActiveSkillID		= -1;
	m_nSelPassiveSkillID	= -1;
	m_nSelMemorizeSkillID	= -1;
	m_nSelSpecialSkillID	= -1;
}

// ----------------------------------------------------------------------------
// Name : CloseSkillLearn()
// Desc :
// ----------------------------------------------------------------------------
void CUISkillLearn::CloseSkillLearn()
{
	// Close message box of skill learn

	_pUIMgr->CloseMessageBox( MSGCMD_SKILLLEARN_NOTIFY );

	_pUIMgr->RearrangeOrder( UI_SKILLLEARN, FALSE );
	
	m_bUseCard = FALSE;
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUISkillLearn::Render()
{
#ifdef NEW_USER_INTERFACE
	RenderNewSkillLearn();
	return;
#endif
	// Check distance
	FLOAT	fDiffX = _pNetwork->MyCharacterInfo.x - m_fNpcX;
	FLOAT	fDiffZ = _pNetwork->MyCharacterInfo.z - m_fNpcZ;
	if( fDiffX * fDiffX + fDiffZ * fDiffZ > UI_VALID_SQRDIST && !m_bUseCard)
		CloseSkillLearn();

	// Set skill learn texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	// Background
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
										m_rtBackground.U0, m_rtBackground.V0, m_rtBackground.U1, m_rtBackground.V1,
										0xFFFFFFFF );

#ifdef ADJUST_MEMORIZE_SKILL
	// Tab
	if( m_nCurrentTab != SLEARN_TAB_SPECIAL )
	{
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcTab.Left + SLEARN_TAB_WIDTH, m_nPosY + m_rcTab.Top,
											m_nPosX + m_rcTab.Left + SLEARN_TAB_WIDTH + 1, m_nPosY + m_rcTab.Bottom,
											m_rtTabLine.U0, m_rtTabLine.V0, m_rtTabLine.U1, m_rtTabLine.V1,
											0xFFFFFFFF );
	
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcTab.Left + (SLEARN_TAB_WIDTH * 2), m_nPosY + m_rcTab.Top,
											m_nPosX + m_rcTab.Left + (SLEARN_TAB_WIDTH * 2) + 1, m_nPosY + m_rcTab.Bottom,
											m_rtTabLine.U0, m_rtTabLine.V0, m_rtTabLine.U1, m_rtTabLine.V1,
											0xFFFFFFFF );
	}
#else
	// Tab
	if( m_nCurrentTab != SLEARN_TAB_SPECIAL )
	{
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcTab.Left + SLEARN_TAB_WIDTH, m_nPosY + m_rcTab.Top,
											m_nPosX + m_rcTab.Left + SLEARN_TAB_WIDTH + 1, m_nPosY + m_rcTab.Bottom,
											m_rtTabLine.U0, m_rtTabLine.V0, m_rtTabLine.U1, m_rtTabLine.V1,
											0xFFFFFFFF );
	}
#endif

	// Close button
	m_btnClose.Render();

	// Learn button
	m_btnLearn.Render();

	// Cancel button
	m_btnCancel.Render();

	// Scroll bar of skill icon
	if( m_nCurrentTab == SLEARN_TAB_ACTIVE )
		m_sbActiveSkillIcon.Render();
	else if( m_nCurrentTab == SLEARN_TAB_PASSIVE )
		m_sbPassiveSkillIcon.Render();
#ifdef ADJUST_MEMORIZE_SKILL
	else if( m_nCurrentTab == SLERAN_TAB_MEMORIZE )
		m_sbMemorizeSkillIcon.Render();
#endif
	else
		m_sbSpecialSkillIcon.Render();

	// List box of skill desc
	m_lbSkillDesc.Render();

	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	// Skill buttons
	RenderSkillBtns();

	// Text in skill learn
	_pUIMgr->GetDrawPort()->PutTextEx( _S( 270, "스킬" ), m_nPosX + SLEARN_TITLE_TEXT_OFFSETX,
										m_nPosY + SLEARN_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );

	if( m_nCurrentTab != SLEARN_TAB_SPECIAL )
	{
		_pUIMgr->GetDrawPort()->PutTextExCX( _S( 275, "일반" ), m_nPosX + SLEARN_ACTIVE_TAB_CX,
												m_nPosY + SLEARN_TAB_SY,
												m_nCurrentTab == SLEARN_TAB_ACTIVE ? 0xFFCB00FF : 0x6B6B6BFF );
		_pUIMgr->GetDrawPort()->PutTextExCX( _S( 276, "강화" ), m_nPosX + SLEARN_PASSIVE_TAB_CX,
												m_nPosY + SLEARN_TAB_SY,
												m_nCurrentTab == SLEARN_TAB_PASSIVE ? 0xFFCB00FF : 0x6B6B6BFF );
#ifdef ADJUST_MEMORIZE_SKILL
		_pUIMgr->GetDrawPort()->PutTextExCX( _S( 1084, "기억" ), m_nPosX + SLEARN_MEMORIZE_TAB_CX,			
												m_nPosY + SLEARN_TAB_SY,
												m_nCurrentTab == SLERAN_TAB_MEMORIZE ? 0xFFCB00FF : 0x6B6B6BFF );
#endif
	}
	else
	{
		CTString	strSubTitle;
		switch( m_nSSkillType )
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
		}

		_pUIMgr->GetDrawPort()->PutTextExCX( strSubTitle, m_nPosX + SLEARN_SPECIAL_TAB_CX, m_nPosY + SLEARN_TAB_SY,
												m_nCurrentTab == SLEARN_TAB_SPECIAL ? 0xFFCB00FF : 0x6B6B6BFF );
	}

	// 이기환 수정(05.01.03) : SP->숙련도 로 수정
	_pUIMgr->GetDrawPort()->PutTextEx(  _S( 90, "숙련도" ) , m_nPosX + SLEARN_CURSP_SX,			
										m_nPosY + SLEARN_CURSP_SY );
	_pUIMgr->GetDrawPort()->PutTextExRX( _pUIMgr->GetCharacterInfo()->GetStringOfSP(),
											m_nPosX + SLEARN_CURSP_RX, m_nPosY + SLEARN_CURSP_SY, 0xBDA99FFF );

	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : AddSkillDescString()
// Desc :
// ----------------------------------------------------------------------------
void CUISkillLearn::AddSkillDescString( CTString &strDesc, COLOR colDesc )
{
	// Get length of string
	INDEX	nLength = strDesc.Length();
	if( nLength == 0 )
		return;

	// wooss 051002
	if(g_iCountry == THAILAND){
		// Get length of string
		INDEX	nThaiLen = FindThaiLen(strDesc);
		INDEX	nChatMax= (_iMaxMsgStringChar-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
		if( nLength == 0 )
			return;
		// If length of string is less than max char
		if( nThaiLen <= nChatMax )
		{
			// Check line character
			for( int iPos = 0; iPos < nLength; iPos++ )
			{
				if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
					break;
			}

			// Not exist
			if( iPos == nLength )
			{
				m_lbSkillDesc.AddString( 0, strDesc, colDesc );
			}
			else
			{
				// Split string
				CTString	strTemp, strTemp2;
				strDesc.Split( iPos, strTemp2, strTemp );
				m_lbSkillDesc.AddString( 0, strTemp2, colDesc );

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddSkillDescString( strTemp, colDesc );
			}
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = _iMaxMsgStringChar;
			BOOL	b2ByteChar = FALSE;
			for( int iPos = 0; iPos < nLength; iPos++ )
			{
				if(nChatMax < FindThaiLen(strDesc,0,iPos))
					break;
			}
			nSplitPos = iPos;

			// Check line character
			for( iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
					break;
			}

			// Not exist
			if( iPos == nSplitPos )
			{
				// Split string
				CTString	strTemp, strTemp2;
				strDesc.Split( nSplitPos, strTemp2, strTemp );
				m_lbSkillDesc.AddString( 0, strTemp2, colDesc );

				// Trim space
				if( strTemp[0] == ' ' )
				{
					int	nTempLength = strTemp.Length();
					for( iPos = 1; iPos < nTempLength; iPos++ )
					{
						if( strTemp[iPos] != ' ' )
							break;
					}

					strTemp.TrimLeft( strTemp.Length() - iPos );
				}

				AddSkillDescString( strTemp, colDesc );
			}
			else
			{
				// Split string
				CTString	strTemp, strTemp2;
				strDesc.Split( iPos, strTemp2, strTemp );
				m_lbSkillDesc.AddString( 0, strTemp2, colDesc );

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddSkillDescString( strTemp, colDesc );
			}

		}
		
	} else {
		// If length of string is less than max char
		if( nLength <= _iMaxMsgStringChar )
		{
			// Check line character
			for( int iPos = 0; iPos < nLength; iPos++ )
			{
				if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
					break;
			}

			// Not exist
			if( iPos == nLength )
			{
				m_lbSkillDesc.AddString( 0, strDesc, colDesc );
			}
			else
			{
				// Split string
				CTString	strTemp, strTemp2;
				strDesc.Split( iPos, strTemp2, strTemp );
				m_lbSkillDesc.AddString( 0, strTemp2, colDesc );

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddSkillDescString( strTemp, colDesc );
			}
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = _iMaxMsgStringChar;
			BOOL	b2ByteChar = FALSE;
			for( int iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strDesc[iPos] & 0x80 )
					b2ByteChar = !b2ByteChar;
				else
					b2ByteChar = FALSE;
			}

			if( b2ByteChar )
				nSplitPos--;

			// Check line character
			for( iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
					break;
			}

			// Not exist
			if( iPos == nSplitPos )
			{
				// Split string
				CTString	strTemp, strTemp2;
				strDesc.Split( nSplitPos, strTemp2, strTemp );
				m_lbSkillDesc.AddString( 0, strTemp2, colDesc );

				// Trim space
				if( strTemp[0] == ' ' )
				{
					int	nTempLength = strTemp.Length();
					for( iPos = 1; iPos < nTempLength; iPos++ )
					{
						if( strTemp[iPos] != ' ' )
							break;
					}

					strTemp.TrimLeft( strTemp.Length() - iPos );
				}

				AddSkillDescString( strTemp, colDesc );
			}
			else
			{
				// Split string
				CTString	strTemp, strTemp2;
				strDesc.Split( iPos, strTemp2, strTemp );
				m_lbSkillDesc.AddString( 0, strTemp2, colDesc );

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddSkillDescString( strTemp, colDesc );
			}
		}
	}
}

// ----------------------------------------------------------------------------
// Name : GetSkillDesc()
// Desc :
// ----------------------------------------------------------------------------
void CUISkillLearn::GetSkillDesc( int nIndex, int nLevel, BOOL bSpecial )
{
	m_lbSkillDesc.ResetAllStrings();

	// If skill is not exist
	if( nIndex == -1 )
		return;

	// Make description of skill
	CTString	strTemp;
	
	if( !bSpecial )
	{
		CSkill		&rSkill = _pNetwork->GetSkillData( nIndex );
		
		strTemp.PrintF( "%s", rSkill.GetName() );
		AddSkillDescString( strTemp, 0xFFC672FF );

		if( rSkill.GetFlag() & SF_SINGLEMODE )
			AddSkillDescString( _S( 499, "퍼스널던전 전용 스킬" ), 0xCACACAFF );		

		AddSkillDescString( CTString( " " ) );

		const char	*pDesc = rSkill.GetDescription();
		if( pDesc != NULL )
		{
			strTemp.PrintF( "%s\n\n", pDesc );
			AddSkillDescString( strTemp, 0xC5C5C5FF );
		}
		
		--nLevel;
		
		// Get learning condition
		int		nLearnSkillIndex[3];
		SBYTE	sbLearnSkillLevel[3];
		int		nLearnItemIndex[3];
		SQUAD	nLearnItemCount[3];
		BOOL	bLearnSkill = FALSE;
		BOOL	bLearnItem = FALSE;
		
		for( int i = 0; i < 3; i++ )
		{
			nLearnSkillIndex[i] = rSkill.GetLearnSkillIndex( nLevel, i );
			sbLearnSkillLevel[i] = rSkill.GetLearnSkillLevel( nLevel, i );
			nLearnItemIndex[i] = rSkill.GetLearnItemIndex( nLevel, i );
			nLearnItemCount[i] = rSkill.GetLearnItemCount( nLevel, i );
			
			if( nLearnSkillIndex[i] != -1 )
				bLearnSkill = TRUE;
			if( nLearnItemIndex[i] != -1 )
				bLearnItem = TRUE;
		}
		
		switch( rSkill.GetType() )
		{
		case CSkill::ST_MELEE:					// Active
		case CSkill::ST_RANGE:					// Active
		case CSkill::ST_MAGIC:					// Active
			{
				strTemp.PrintF( _S( 256, "필요 레벨 : %d" ), rSkill.GetLearnLevel( nLevel ) );
				AddSkillDescString( strTemp, 0xBDA99FFF );
				strTemp.PrintF( _S( 257, "필요 SP : %d" ), rSkill.GetLearnSP( nLevel ) ); // 숙련도
				AddSkillDescString( strTemp, 0xBDA99FFF );				

				const int iLearnStr = rSkill.GetLearnStr( nLevel );
				const int iLearnDex = rSkill.GetLearnDex( nLevel );
				const int iLearnInt = rSkill.GetLearnInt( nLevel );
				const int iLearnCon = rSkill.GetLearnCon( nLevel );

				if( iLearnStr > 0 ) 
				{
					strTemp.PrintF( _S( 1391, "필요 힘 : %d" ), iLearnStr );		// 필요 힘 
					AddSkillDescString( strTemp, 0xBDA99FFF );
				}

				if( iLearnDex > 0 ) 
				{
					strTemp.PrintF( _S( 1392, "필요 민첩 : %d" ), iLearnDex );	// 필요 민첩
					AddSkillDescString( strTemp, 0xBDA99FFF );
				}
				
				if( iLearnInt > 0 ) 
				{
					strTemp.PrintF( _S( 1393, "필요 지혜 : %d" ), iLearnInt );	// 필요 지혜
					AddSkillDescString( strTemp, 0xBDA99FFF );
				}

				if( iLearnCon > 0 )
				{
					strTemp.PrintF( _S( 1394, "필요 체질 : %d" ), iLearnCon );	// 필요 체질
					AddSkillDescString( strTemp, 0xBDA99FFF );
				}
				
				// Need skill
				if( bLearnSkill )
				{
					AddSkillDescString( _S( 258, "필요 스킬" ), 0xBDA99FFF );
					for( i = 0; i < 3; i++ )
					{
						if( nLearnSkillIndex[i] != -1 )
						{
							CSkill	&rNeedSkill = _pNetwork->GetSkillData( nLearnSkillIndex[i] );
							strTemp.PrintF( "  %s Lv.%d", rNeedSkill.GetName(), sbLearnSkillLevel[i] );
							AddSkillDescString( strTemp, 0xBDA99FFF );
						}
					}
				}
				
				// Need item
				if( bLearnItem )
				{
					AddSkillDescString( _S( 259, "필요 아이템" ), 0xBDA99FFF );
					for( i = 0; i < 3; i++ )
					{
						if( nLearnItemIndex[i] != -1 )
						{
							CItemData	&rNeedItem = _pNetwork->GetItemData( nLearnItemIndex[i] );
							strTemp.PrintF( _S( 260, "  %s %d개" ), _pNetwork->GetItemName( nLearnItemIndex[i] ), nLearnItemCount[i] );
							AddSkillDescString( strTemp, 0xBDA99FFF );
						}
					}
				}

				int	nNeedMP = rSkill.GetNeedMP( nLevel );
				int	nNeedHP = rSkill.GetNeedHP( nLevel );
				if( nNeedHP == 0 )
				{
					if( nNeedMP != 0 )
					{
						strTemp.PrintF( _S( 64, "소모 MP : %d" ), nNeedMP );
						AddSkillDescString( strTemp, 0xBDA99FFF );
					}
				}
				else
				{
					if( nNeedMP == 0 )
					{
						strTemp.PrintF( _S( 500, "소모 HP : %d" ), nNeedHP );		
						AddSkillDescString( strTemp, 0xBDA99FFF );
					}
					else
					{
						strTemp.PrintF( _S( 64, "소모 MP : %d" ), nNeedMP );
						AddSkillDescString( strTemp, 0xBDA99FFF );
						strTemp.PrintF( _S( 500, "소모 HP : %d" ), nNeedHP );		
						AddSkillDescString( strTemp, 0xBDA99FFF );
					}
				}

				if( rSkill.GetPower( nLevel ) > 0 )
				{
					strTemp.PrintF( _S( 65, "위력 : %d" ), rSkill.GetPower( nLevel ) );
					AddSkillDescString( strTemp, 0xBDA99FFF );
				}

				strTemp.PrintF( _S( 66, "유효 거리 : %.1f" ), rSkill.GetFireRange() );
				AddSkillDescString( strTemp, 0xBDA99FFF );
				strTemp.PrintF( _S( 261, "최대스킬 레벨 : %d" ), rSkill.GetMaxLevel() );
				AddSkillDescString( strTemp, 0xBDA99FFF );
			}
			break;

		case CSkill::ST_PASSIVE:				// Passive
		{
			strTemp.PrintF( _S( 256, "필요 레벨 : %d" ), rSkill.GetLearnLevel( nLevel ) );
			AddSkillDescString( strTemp, 0xBDA99FFF );
			strTemp.PrintF( _S( 257, "필요 SP : %d" ), rSkill.GetLearnSP( nLevel ) ); // 숙련도
			AddSkillDescString( strTemp, 0xBDA99FFF );

			const int iLearnStr = rSkill.GetLearnStr( nLevel );
			const int iLearnDex = rSkill.GetLearnDex( nLevel );
			const int iLearnInt = rSkill.GetLearnInt( nLevel );
			const int iLearnCon = rSkill.GetLearnCon( nLevel );
			
			if( iLearnStr > 0 ) 
			{
				strTemp.PrintF( _S( 1391, "필요 힘 : %d" ), iLearnStr );		// 필요 힘
				AddSkillDescString( strTemp, 0xBDA99FFF );
			}
			
			if( iLearnDex > 0 ) 
			{
				strTemp.PrintF( _S( 1392, "필요 민첩 : %d" ), iLearnDex );	// 필요 민첩
				AddSkillDescString( strTemp, 0xBDA99FFF );
			}
			
			if( iLearnInt > 0 ) 
			{
				strTemp.PrintF( _S( 1393, "필요 지혜 : %d" ), iLearnInt );	// 필요 지혜
				AddSkillDescString( strTemp, 0xBDA99FFF );
			}
			
			if( iLearnCon > 0 )
			{
				strTemp.PrintF( _S( 1394, "필요 체질 : %d" ), iLearnCon );	// 필요 체질
				AddSkillDescString( strTemp, 0xBDA99FFF );
			}

			// Need skill
			if( bLearnSkill )
			{
				AddSkillDescString( _S( 258, "필요 스킬" ), 0xBDA99FFF );
				for( i = 0; i < 3; i++ )
				{
					if( nLearnSkillIndex[i] != -1 )
					{
						CSkill	&rNeedSkill = _pNetwork->GetSkillData( nLearnSkillIndex[i] );
						strTemp.PrintF( "  %s Lv.%d", rNeedSkill.GetName(), sbLearnSkillLevel[i] );
						AddSkillDescString( strTemp, 0xBDA99FFF );
					}
				}
			}

			// Need item
			if( bLearnItem )
			{
				AddSkillDescString( _S( 259, "필요 아이템" ), 0xBDA99FFF );
				for( i = 0; i < 3; i++ )
				{
					if( nLearnItemIndex[i] != -1 )
					{
						CItemData	&rNeedItem = _pNetwork->GetItemData( nLearnItemIndex[i] );
						strTemp.PrintF( _S( 260, "  %s %d개" ), _pNetwork->GetItemName( nLearnItemIndex[i] ), nLearnItemCount[i] );
						AddSkillDescString( strTemp, 0xBDA99FFF );
					}
				}
			}

			strTemp.PrintF( _S( 261, "최대스킬 레벨 : %d" ), rSkill.GetMaxLevel() );
			AddSkillDescString( strTemp, 0xBDA99FFF );
		}
		break;
		}
	}
	else
	{
		CSpecialSkill &rSkill = _pNetwork->GetSSkillData( nIndex );
		
		strTemp.PrintF( "%s\n\n", rSkill.GetName() );
		AddSkillDescString( strTemp, 0xFFC672FF );

		const char *pDesc = rSkill.GetDescription();
		if( pDesc != NULL )
		{
			strTemp.PrintF( "%s\n\n", pDesc );
			AddSkillDescString( strTemp, 0xC5C5C5FF );
		}
		
		--nLevel;
		
		// Get learning condition
		int		nLearnSkillIndex;
		SBYTE	sbLearnSkillLevel;
		BOOL	bLearnSkill	= FALSE;
		BOOL	bLearnItem	= FALSE;		
		
		nLearnSkillIndex	= rSkill.GetLearnSkillIndex();
		sbLearnSkillLevel	= rSkill.GetLearnSkillLevel();
		
		if( nLearnSkillIndex != -1 )
			bLearnSkill = TRUE;
		
		strTemp.PrintF( _S( 256, "필요 레벨 : %d" ), rSkill.GetLearnLevel( nLevel ) );
		AddSkillDescString( strTemp, 0xBDA99FFF );
		strTemp.PrintF( _S( 257, "필요 SP : %d" ), rSkill.GetLearnSP( nLevel ) ); // 숙련도
		AddSkillDescString( strTemp, 0xBDA99FFF );
		
		// Need skill
		if( bLearnSkill )
		{
			AddSkillDescString( _S( 258, "필요 스킬" ), 0xBDA99FFF );
			if( nLearnSkillIndex != -1 )
			{
				CSpecialSkill	&rNeedSkill = _pNetwork->GetSSkillData( nLearnSkillIndex );
				strTemp.PrintF( "  %s Lv.%d", rNeedSkill.GetName(), sbLearnSkillLevel );
				AddSkillDescString( strTemp, 0xBDA99FFF );
			}
		}
	}
}

// ----------------------------------------------------------------------------
// Name : RenderSkillBtns()
// Desc :
// ----------------------------------------------------------------------------
void CUISkillLearn::RenderSkillBtns()
{
	int	nX = SLEARN_SLOT_SX, nY = SLEARN_SLOT_SY;
	int	iRow, iRowS, iRowE;
	// Active skill tab
	if( m_nCurrentTab == SLEARN_TAB_ACTIVE )
	{
		// Active skill button
		iRowS = m_sbActiveSkillIcon.GetScrollPos();
		iRowE = iRowS + SLEARN_SLOT_ROW;
	    for( iRow = iRowS; iRow < iRowE; iRow++, nY += SLEARN_SLOT_OFFSETY )
		{      
			m_btnActiveSkills[iRow].SetPos( nX, nY );
			if( m_btnActiveSkills[iRow].IsEmpty() )		
				continue;
			
			m_btnActiveSkills[iRow].Render();
		}
	}
	// Passive skill tab
	else if( m_nCurrentTab == SLEARN_TAB_PASSIVE )
	{
		// Passive skill button
		iRowS = m_sbPassiveSkillIcon.GetScrollPos();		
		iRowE = iRowS + SLEARN_SLOT_ROW;
	    for( iRow = iRowS; iRow < iRowE; iRow++, nY += SLEARN_SLOT_OFFSETY )
		{
			m_btnPassiveSkills[iRow].SetPos( nX, nY );
			if( m_btnPassiveSkills[iRow].IsEmpty() )		
				continue;
			
			m_btnPassiveSkills[iRow].Render();
		}
	}
#ifdef ADJUST_MEMORIZE_SKILL
	// Memorize skill tab
	else if( m_nCurrentTab == SLERAN_TAB_MEMORIZE )
	{
		// Memorize skill button
		iRowS = m_sbMemorizeSkillIcon.GetScrollPos();		
		iRowE = iRowS + SLEARN_SLOT_ROW;
	    for( iRow = iRowS; iRow < iRowE; iRow++, nY += SLEARN_SLOT_OFFSETY )
		{
			m_btnMemorizeSkills[iRow].SetPos( nX, nY );
			if( m_btnMemorizeSkills[iRow].IsEmpty() )		
				continue;
			
			m_btnMemorizeSkills[iRow].Render();
		}
	}
#endif
	// Special skill tab
	else
	{
		// Special skill button
		iRowS = m_sbSpecialSkillIcon.GetScrollPos();		
		iRowE = iRowS + SLEARN_SLOT_ROW;
	    for( iRow = iRowS; iRow < iRowE; iRow++, nY += SLEARN_SLOT_OFFSETY )
		{
			m_btnSpecialSkills[iRow].SetPos( nX, nY );
			if( m_btnSpecialSkills[iRow].IsEmpty() )		
				continue;
			
			m_btnSpecialSkills[iRow].Render();
		}
	}

	// Render all button elements
	_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_SKILL );

	// Outline of selected button
	// Active skill tab
	if( m_nCurrentTab == SLEARN_TAB_ACTIVE )
	{
		iRowS = m_sbActiveSkillIcon.GetScrollPos();
		iRowE = iRowS + SLEARN_SLOT_ROW;
		if( m_nSelActiveSkillID >= 0 && iRowS <= m_nSelActiveSkillID && m_nSelActiveSkillID < iRowE )
		{
			// Set skill learn texture
			_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

			m_btnActiveSkills[m_nSelActiveSkillID].GetAbsPos( nX, nY );
			_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + BTN_SIZE, nY + BTN_SIZE,
												m_rtSelOutline.U0, m_rtSelOutline.V0,
												m_rtSelOutline.U1, m_rtSelOutline.V1,
												0xFFFFFFFF );

			// Render all elements
			_pUIMgr->GetDrawPort()->FlushRenderingQueue();
		}
	}
	// Passive skill tab
	else if( m_nCurrentTab == SLEARN_TAB_PASSIVE )
	{
		// Passive skill button
		iRowS = m_sbPassiveSkillIcon.GetScrollPos();		
		iRowE = iRowS + SLEARN_SLOT_ROW;
		if( m_nSelPassiveSkillID >= 0 && iRowS <= m_nSelPassiveSkillID && m_nSelPassiveSkillID < iRowE )
		{
			// Set skill learn texture
			_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

			m_btnPassiveSkills[m_nSelPassiveSkillID].GetAbsPos( nX, nY );
			_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + BTN_SIZE, nY + BTN_SIZE,
												m_rtSelOutline.U0, m_rtSelOutline.V0,
												m_rtSelOutline.U1, m_rtSelOutline.V1,
												0xFFFFFFFF );

			// Render all elements
			_pUIMgr->GetDrawPort()->FlushRenderingQueue();
		}
	}
#ifdef ADJUST_MEMORIZE_SKILL
	// Memorize skill tab
	else if( m_nCurrentTab == SLERAN_TAB_MEMORIZE )
	{
		// Memorize skill button
		iRowS = m_sbMemorizeSkillIcon.GetScrollPos();		
		iRowE = iRowS + SLEARN_SLOT_ROW;
		if( m_nSelMemorizeSkillID >= 0 && iRowS <= m_nSelMemorizeSkillID && m_nSelMemorizeSkillID < iRowE )
		{
			// Set skill learn texture
			_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

			m_btnPassiveSkills[m_nSelMemorizeSkillID].GetAbsPos( nX, nY );
			_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + BTN_SIZE, nY + BTN_SIZE,
												m_rtSelOutline.U0, m_rtSelOutline.V0,
												m_rtSelOutline.U1, m_rtSelOutline.V1,
												0xFFFFFFFF );

			// Render all elements
			_pUIMgr->GetDrawPort()->FlushRenderingQueue();
		}
	}
#endif
	// Special skill tab
	else
	{
		// Special skill button
		iRowS = m_sbSpecialSkillIcon.GetScrollPos();		
		iRowE = iRowS + SLEARN_SLOT_ROW;
		if( m_nSelSpecialSkillID >= 0 && iRowS <= m_nSelSpecialSkillID && m_nSelSpecialSkillID < iRowE )
		{
			// Set skill learn texture
			_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

			m_btnSpecialSkills[m_nSelSpecialSkillID].GetAbsPos( nX, nY );
			_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + BTN_SIZE, nY + BTN_SIZE,
												m_rtSelOutline.U0, m_rtSelOutline.V0,
												m_rtSelOutline.U1, m_rtSelOutline.V1,
												0xFFFFFFFF );

			// Render all elements
			_pUIMgr->GetDrawPort()->FlushRenderingQueue();
		}
	}

	nY = SLEARN_NAME_SY;
	// Active skill tab
	int	nCharLevel	= _pNetwork->MyCharacterInfo.level;
	int nCharSP		= _pNetwork->MyCharacterInfo.sp;
	if( m_nCurrentTab == SLEARN_TAB_ACTIVE )
	{
		iRowS = m_sbActiveSkillIcon.GetScrollPos();
		iRowE = iRowS + SLEARN_SLOT_ROW;
		for( iRow = iRowS; iRow < iRowE; iRow++, nY += SLEARN_SLOT_OFFSETY )
		{
			if( m_btnActiveSkills[iRow].IsEmpty() )
				continue;
  
			CSkill	&rSkill = _pNetwork->GetSkillData( m_btnActiveSkills[iRow].GetSkillIndex() );
			SBYTE	sbLevel = m_btnActiveSkills[iRow].GetSkillLevel();
			int		nNeedLevel = rSkill.GetLearnLevel( sbLevel - 1 );

			m_strShortDesc.PrintF( "%s", rSkill.GetName() );
			_pUIMgr->GetDrawPort()->PutTextExCX( m_strShortDesc, m_nPosX + SLEARN_NAME_CX, m_nPosY + nY,
													nCharLevel >= nNeedLevel ? 0xFFC672FF : 0xBCBCBCFF );

			m_strShortDesc.PrintF( "Lv %2d   SP %2d", sbLevel, rSkill.GetLearnSP( sbLevel - 1 ) );
			_pUIMgr->GetDrawPort()->PutTextExRX( m_strShortDesc, m_nPosX + SLEARN_NEED_RX,
													m_nPosY + nY + 17, 0xBDA99FFF );
		}
	}
	// Passive skill tab
	else if( m_nCurrentTab == SLEARN_TAB_PASSIVE )
	{
		iRowS = m_sbPassiveSkillIcon.GetScrollPos();
		iRowE = iRowS + SLEARN_SLOT_ROW;
		for( iRow = iRowS; iRow < iRowE; iRow++, nY += SLEARN_SLOT_OFFSETY )
		{
			if( m_btnPassiveSkills[iRow].IsEmpty() )
				continue;
  
			CSkill	&rSkill = _pNetwork->GetSkillData( m_btnPassiveSkills[iRow].GetSkillIndex() );
			SBYTE	sbLevel = m_btnPassiveSkills[iRow].GetSkillLevel();
			int		nNeedLevel = rSkill.GetLearnLevel( sbLevel - 1 );

			m_strShortDesc.PrintF( "%s", rSkill.GetName() );
			_pUIMgr->GetDrawPort()->PutTextExCX( m_strShortDesc, m_nPosX + SLEARN_NAME_CX, m_nPosY + nY,
													nCharLevel >= nNeedLevel ? 0xFFC672FF : 0xBCBCBCFF );

			m_strShortDesc.PrintF( "Lv %2d  SP %4d", sbLevel, rSkill.GetLearnSP( sbLevel - 1 ) );
			_pUIMgr->GetDrawPort()->PutTextExRX( m_strShortDesc, m_nPosX + SLEARN_NEED_RX,
													m_nPosY + nY + 17, 0xBDA99FFF );
		}
	}
#ifdef ADJUST_MEMORIZE_SKILL
	// Memorize skill tab
	else if( m_nCurrentTab == SLERAN_TAB_MEMORIZE )
	{
		iRowS = m_sbMemorizeSkillIcon.GetScrollPos();
		iRowE = iRowS + SLEARN_SLOT_ROW;
		for( iRow = iRowS; iRow < iRowE; iRow++, nY += SLEARN_SLOT_OFFSETY )
		{
			if( m_btnMemorizeSkills[iRow].IsEmpty() )
				continue;
  
			CSkill	&rSkill = _pNetwork->GetSkillData( m_btnMemorizeSkills[iRow].GetSkillIndex() );
			SBYTE	sbLevel = m_btnMemorizeSkills[iRow].GetSkillLevel();
			int		nNeedLevel = rSkill.GetLearnLevel( sbLevel - 1 );

			m_strShortDesc.PrintF( "%s", rSkill.GetName() );
			_pUIMgr->GetDrawPort()->PutTextExCX( m_strShortDesc, m_nPosX + SLEARN_NAME_CX, m_nPosY + nY,
													nCharLevel >= nNeedLevel ? 0xFFC672FF : 0xBCBCBCFF );

			m_strShortDesc.PrintF( "Lv %2d  SP %4d", sbLevel, rSkill.GetLearnSP( sbLevel - 1 ) );
			_pUIMgr->GetDrawPort()->PutTextExRX( m_strShortDesc, m_nPosX + SLEARN_NEED_RX,
													m_nPosY + nY + 17, 0xBDA99FFF );
		}
	}
#endif
	// Special skill tab
	else
	{
		iRowS = m_sbSpecialSkillIcon.GetScrollPos();
		iRowE = iRowS + SLEARN_SLOT_ROW;
		for( iRow = iRowS; iRow < iRowE; iRow++, nY += SLEARN_SLOT_OFFSETY )
		{
			if( m_btnSpecialSkills[iRow].IsEmpty() )
				continue;
  
			CSpecialSkill &rSkill = _pNetwork->GetSSkillData( m_btnSpecialSkills[iRow].GetSkillIndex() );
			SBYTE	sbLevel = m_btnSpecialSkills[iRow].GetSkillLevel();
			int		nNeedLevel = rSkill.GetLearnLevel( sbLevel - 1 );
			int		nNeedSP	= rSkill.GetLearnSP( sbLevel - 1 );

			m_strShortDesc.PrintF( "%s", rSkill.GetName() );

			if(nCharLevel > nNeedLevel && nCharSP > nNeedSP)
			{
				_pUIMgr->GetDrawPort()->PutTextExCX( m_strShortDesc, m_nPosX + SLEARN_NAME_CX, m_nPosY + nY,
														0xFFC672FF );
			}
			else
			{
				_pUIMgr->GetDrawPort()->PutTextExCX( m_strShortDesc, m_nPosX + SLEARN_NAME_CX, m_nPosY + nY,
														0xBCBCBCFF );
			}

			m_strShortDesc.PrintF( "Lv %2d  SP %4d", sbLevel, rSkill.GetLearnSP( sbLevel - 1 ) );
			_pUIMgr->GetDrawPort()->PutTextExRX( m_strShortDesc, m_nPosX + SLEARN_NEED_RX,
													m_nPosY + nY + 17, 0xBDA99FFF );
		}
	}
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUISkillLearn::MouseMessage( MSG *pMsg )
{
#ifdef NEW_USER_INTERFACE
	return SKillLearnNewMouseMessage(pMsg);
#endif

	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;

	// Extended button clicked
	static BOOL	bLButtonDownInBtn = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				_pUIMgr->SetMouseCursorInsideUIs();

			// Move skill learn
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			// Close button
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Learn button
			else if( m_btnLearn.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Cancel button
			else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// List box of skill desc
			else if( m_lbSkillDesc.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Active skill tab
			else if( m_nCurrentTab == SLEARN_TAB_ACTIVE )
			{
				// Reset state of selected button
				if( bLButtonDownInBtn && m_nSelActiveSkillID >= 0 && ( pMsg->wParam & MK_LBUTTON ) )
				{
					m_btnActiveSkills[m_nSelActiveSkillID].SetBtnState( UBES_IDLE );
					bLButtonDownInBtn = FALSE;
				}
				// Active icon scroll bar
				else if( m_sbActiveSkillIcon.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
			// Passive skill tab
			else if( m_nCurrentTab == SLEARN_TAB_PASSIVE )
			{
				// Reset state of selected button
				if( bLButtonDownInBtn && m_nSelPassiveSkillID >= 0 && ( pMsg->wParam & MK_LBUTTON ) )
				{
					m_btnPassiveSkills[m_nSelPassiveSkillID].SetBtnState( UBES_IDLE );
					bLButtonDownInBtn = FALSE;
				}
				// Passive icon scroll bar
				else if( m_sbPassiveSkillIcon.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
#ifdef ADJUST_MEMORIZE_SKILL
			// Memorize skill tab
			else if( m_nCurrentTab == SLERAN_TAB_MEMORIZE )
			{
				// Reset state of selected button
				if( bLButtonDownInBtn && m_nSelMemorizeSkillID >= 0 && ( pMsg->wParam & MK_LBUTTON ) )
				{
					m_btnMemorizeSkills[m_nSelMemorizeSkillID].SetBtnState( UBES_IDLE );
					bLButtonDownInBtn = FALSE;
				}
				// Memorize icon scroll bar
				else if( m_sbMemorizeSkillIcon.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
#endif
			// Special skill tab
			else
			{
				// Reset state of selected button
				if( bLButtonDownInBtn && m_nSelSpecialSkillID >= 0 && ( pMsg->wParam & MK_LBUTTON ) )
				{
					m_btnSpecialSkills[m_nSelSpecialSkillID].SetBtnState( UBES_IDLE );
					bLButtonDownInBtn = FALSE;
				}
				// Special icon scroll bar
				else if( m_sbSpecialSkillIcon.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				nOldX = nX;		nOldY = nY;

				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Title bar
				else if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
				}
				// Skill tab
				else if( m_nCurrentTab != SLEARN_TAB_SPECIAL && IsInsideRect( nX, nY, m_rcTab ) )
				{
					int	nOldTab = m_nCurrentTab;
					m_nCurrentTab = ( nX - m_nPosX - m_rcTab.Left ) / SLEARN_TAB_WIDTH;
#ifdef ADJUST_MEMORIZE_SKILL
					if( m_nCurrentTab < 0 || m_nCurrentTab > SLERAN_TAB_MEMORIZE )
#else
					if( m_nCurrentTab < 0 || m_nCurrentTab > SLEARN_TAB_PASSIVE )
#endif
						m_nCurrentTab = nOldTab;

					if( nOldTab != m_nCurrentTab )
					{
						if( m_nCurrentTab == SLEARN_TAB_ACTIVE )
						{
							if( m_nSelActiveSkillID >= 0)
							{
								GetSkillDesc( m_btnActiveSkills[m_nSelActiveSkillID].GetSkillIndex(),
												m_btnActiveSkills[m_nSelActiveSkillID].GetSkillLevel() );
							}
							else
								GetSkillDesc( -1 );
						}
						else if( m_nCurrentTab == SLEARN_TAB_PASSIVE )
						{
							if( m_nSelPassiveSkillID >= 0)
							{
								GetSkillDesc( m_btnPassiveSkills[m_nSelPassiveSkillID].GetSkillIndex(),
												m_btnPassiveSkills[m_nSelPassiveSkillID].GetSkillLevel() );
							}
							else
								GetSkillDesc( -1 );
						}
#ifdef ADJUST_MEMORIZE_SKILL
						else if( m_nCurrentTab == SLERAN_TAB_MEMORIZE )
						{
							if( m_nSelMemorizeSkillID >= 0)
							{
								GetSkillDesc( m_btnMemorizeSkills[m_nSelMemorizeSkillID].GetSkillIndex(),
												m_btnMemorizeSkills[m_nSelMemorizeSkillID].GetSkillLevel() );
							}
							else
								GetSkillDesc( -1 );
						}
#endif
					}
				}
				// Learn button
				else if( m_btnLearn.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Cancel button
				else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// List box of skill desc
				else if( m_lbSkillDesc.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Active skill tab
				else if( m_nCurrentTab == SLEARN_TAB_ACTIVE )
				{
					// Active icon scroll bar
					if( m_sbActiveSkillIcon.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Skill slot
					else if( IsInsideRect( nX, nY, m_rcIcons ) )
					{
						int	nOldSelSkillID = m_nSelActiveSkillID;
						m_nSelActiveSkillID = -1;
        
						int	iRowS = m_sbActiveSkillIcon.GetScrollPos();
						int	iRowE = iRowS + SLEARN_SLOT_ROW;
						for( int iRow = iRowS; iRow < iRowE; iRow++ )
						{
							if( m_btnActiveSkills[iRow].MouseMessage( pMsg ) != WMSG_FAIL )
							{
								// Update selected skill
								m_nSelActiveSkillID = iRow;
								if( nOldSelSkillID != m_nSelActiveSkillID )
								{
									GetSkillDesc( m_btnActiveSkills[iRow].GetSkillIndex(),
													m_btnActiveSkills[iRow].GetSkillLevel() );
								}

								bLButtonDownInBtn = TRUE;

								_pUIMgr->RearrangeOrder( UI_SKILLLEARN, TRUE );
								return WMSG_SUCCESS;
							}
						}

						GetSkillDesc( -1 );
					}
				}
				// Passive skill tab
				else if( m_nCurrentTab == SLEARN_TAB_PASSIVE )
				{
					// Passive icon scroll bar
					if( m_sbPassiveSkillIcon.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Skill slot
					else if( IsInsideRect( nX, nY, m_rcIcons ) )
					{
						int	nOldSelSkillID = m_nSelPassiveSkillID;
						m_nSelPassiveSkillID = -1;
        
						int	iRowS = m_sbPassiveSkillIcon.GetScrollPos();
						int	iRowE = iRowS + SLEARN_SLOT_ROW;
						for( int iRow = iRowS; iRow < iRowE; iRow++ )
						{
							if( m_btnPassiveSkills[iRow].MouseMessage( pMsg ) != WMSG_FAIL )
							{
								// Update selected skill
								m_nSelPassiveSkillID = iRow;
								if( nOldSelSkillID != m_nSelPassiveSkillID )
								{
									GetSkillDesc( m_btnPassiveSkills[iRow].GetSkillIndex(),
													m_btnPassiveSkills[iRow].GetSkillLevel() );
								}

								bLButtonDownInBtn = TRUE;

								_pUIMgr->RearrangeOrder( UI_SKILLLEARN, TRUE );
								return WMSG_SUCCESS;
							}
						}

						GetSkillDesc( -1 );
					}
				}
#ifdef ADJUST_MEMORIZE_SKILL
				// Memorize skill tab
				else if( m_nCurrentTab == SLERAN_TAB_MEMORIZE )
				{
					// Memorize icon scroll bar
					if( m_sbMemorizeSkillIcon.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Skill slot
					else if( IsInsideRect( nX, nY, m_rcIcons ) )
					{
						int	nOldSelSkillID = m_nSelMemorizeSkillID;
						m_nSelMemorizeSkillID = -1;
        
						int	iRowS = m_sbMemorizeSkillIcon.GetScrollPos();
						int	iRowE = iRowS + SLEARN_SLOT_ROW;
						for( int iRow = iRowS; iRow < iRowE; iRow++ )
						{
							if( m_btnMemorizeSkills[iRow].MouseMessage( pMsg ) != WMSG_FAIL )
							{
								// Update selected skill
								m_nSelMemorizeSkillID = iRow;
								if( nOldSelSkillID != m_nSelMemorizeSkillID )
								{
									GetSkillDesc( m_btnMemorizeSkills[iRow].GetSkillIndex(),
													m_btnMemorizeSkills[iRow].GetSkillLevel() );
								}

								bLButtonDownInBtn = TRUE;

								_pUIMgr->RearrangeOrder( UI_SKILLLEARN, TRUE );
								return WMSG_SUCCESS;
							}
						}

						GetSkillDesc( -1 );
					}
				}
#endif
				// Special skill tab
				else
				{
					// Special icon scroll bar
					if( m_sbSpecialSkillIcon.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Skill slot
					else if( IsInsideRect( nX, nY, m_rcIcons ) )
					{
						int	nOldSelSkillID = m_nSelSpecialSkillID;
						m_nSelSpecialSkillID = -1;
        
						int	iRowS = m_sbSpecialSkillIcon.GetScrollPos();
						int	iRowE = iRowS + SLEARN_SLOT_ROW;
						for( int iRow = iRowS; iRow < iRowE; iRow++ )
						{
							if( m_btnSpecialSkills[iRow].MouseMessage( pMsg ) != WMSG_FAIL )
							{
								// Update selected skill
								m_nSelSpecialSkillID = iRow;
								if( nOldSelSkillID != m_nSelSpecialSkillID )
								{
									GetSkillDesc( m_btnSpecialSkills[iRow].GetSkillIndex(),
													m_btnSpecialSkills[iRow].GetSkillLevel(), TRUE );
								}

								bLButtonDownInBtn = TRUE;

								_pUIMgr->RearrangeOrder( UI_SKILLLEARN, TRUE );
								return WMSG_SUCCESS;
							}
						}

						GetSkillDesc( -1 );
					}
				}

				_pUIMgr->RearrangeOrder( UI_SKILLLEARN, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			bLButtonDownInBtn = FALSE;

			// If holding button doesn't exist
			if( _pUIMgr->GetHoldBtn().IsEmpty() )
			{
				// Title bar
				bTitleBarClick = FALSE;

				// If skill learn isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseSkillLearn();

					return WMSG_SUCCESS;
				}
				// Learn button
				else if( ( wmsgResult = m_btnLearn.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						SendLearnSkill();

					return WMSG_SUCCESS;
				}
				// Cancel button
				else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseSkillLearn();

					return WMSG_SUCCESS;
				}
				// List box of skill desc
				else if( m_lbSkillDesc.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				// Active skill tab
				else if( m_nCurrentTab == SLEARN_TAB_ACTIVE )
				{
					// Active icon scroll bar
					if( m_sbActiveSkillIcon.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Skill slot
					else if( IsInsideRect( nX, nY, m_rcIcons ) )
					{
						int	iRowS = m_sbActiveSkillIcon.GetScrollPos();
						int	iRowE = iRowS + SLEARN_SLOT_ROW;
						for( int iRow = iRowS; iRow < iRowE; iRow++ )
						{
							if( m_btnActiveSkills[iRow].MouseMessage( pMsg ) != WMSG_FAIL )
								return WMSG_SUCCESS;
						}
					}
				}
				// Passive skill tab
				else if( m_nCurrentTab == SLEARN_TAB_PASSIVE )
				{
					// Passive icon scroll bar
					if( m_sbPassiveSkillIcon.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Skill slot
					else if( IsInsideRect( nX, nY, m_rcIcons ) )
					{
						int	iRowS = m_sbPassiveSkillIcon.GetScrollPos();
						int	iRowE = iRowS + SLEARN_SLOT_ROW;
						for( int iRow = iRowS; iRow < iRowE; iRow++ )
						{
							if( m_btnPassiveSkills[iRow].MouseMessage( pMsg ) != WMSG_FAIL )
								return WMSG_SUCCESS;
						}
					}
				}
#ifdef ADJUST_MEMORIZE_SKILL
				// Memorize skill tab
				else if( m_nCurrentTab == SLERAN_TAB_MEMORIZE )
				{
					// Memorize icon scroll bar
					if( m_sbMemorizeSkillIcon.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Skill slot
					else if( IsInsideRect( nX, nY, m_rcIcons ) )
					{
						int	iRowS = m_sbMemorizeSkillIcon.GetScrollPos();
						int	iRowE = iRowS + SLEARN_SLOT_ROW;
						for( int iRow = iRowS; iRow < iRowE; iRow++ )
						{
							if( m_btnMemorizeSkills[iRow].MouseMessage( pMsg ) != WMSG_FAIL )
								return WMSG_SUCCESS;
						}
					}
				}
#endif
				// Special skill tab
				else
				{
					// Special icon scroll bar
					if( m_sbSpecialSkillIcon.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Skill slot
					else if( IsInsideRect( nX, nY, m_rcIcons ) )
					{
						int	iRowS = m_sbSpecialSkillIcon.GetScrollPos();
						int	iRowE = iRowS + SLEARN_SLOT_ROW;
						for( int iRow = iRowS; iRow < iRowE; iRow++ )
						{
							if( m_btnSpecialSkills[iRow].MouseMessage( pMsg ) != WMSG_FAIL )
								return WMSG_SUCCESS;
						}
					}
				}
			}
			// If holding button exists
			else
			{
				if( IsInside( nX, nY ) )
				{
					// Reset holding button
					_pUIMgr->ResetHoldBtn();

					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
			{
				// List box of skill desc
				if( m_lbSkillDesc.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Active skill tab
				else if( m_nCurrentTab == SLEARN_TAB_ACTIVE )
				{
					// Active icon scroll bar
					if( m_sbActiveSkillIcon.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						// Nothing
					}
				}
				// Passive skill tab
				else if( m_nCurrentTab == SLEARN_TAB_PASSIVE )
				{
					// Passive icon scroll bar
					if( m_sbPassiveSkillIcon.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						// Nothing
					}
				}
#ifdef ADJUST_MEMORIZE_SKILL
				// Memorize skill tab
				else if( m_nCurrentTab == SLERAN_TAB_MEMORIZE )
				{
					// Memorize icon scroll bar
					if( m_sbMemorizeSkillIcon.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						// Nothing
					}
				}
#endif
				// Special skill tab
				else
				{
					// Special icon scroll bar
					if( m_sbSpecialSkillIcon.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						// Nothing
					}
				}

				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				// List box of skill desc
				if( m_lbSkillDesc.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				// Active skill tab
				else if( m_nCurrentTab == SLEARN_TAB_ACTIVE )
				{
					// Active icon scroll bar
					if( m_sbActiveSkillIcon.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
				// Passive skill tab
				else if( m_nCurrentTab == SLEARN_TAB_PASSIVE )
				{
					// Passive icon scroll bar
					if( m_sbPassiveSkillIcon.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
#ifdef ADJUST_MEMORIZE_SKILL
				// Memorize skill tab
				else if( m_nCurrentTab == SLERAN_TAB_MEMORIZE )
				{
					// Memorize icon scroll bar
					if( m_sbMemorizeSkillIcon.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
#endif
				// Special skill tab
				else
				{
					// Special icon scroll bar
					if( m_sbSpecialSkillIcon.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
			}
		}
		break;
	}

	return WMSG_FAIL;
}


// ========================================================================= //
//                             Command functions                             //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUISkillLearn::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
	case MSGCMD_SKILLLEARN_NOTIFY:
		{
			_pUIMgr->CloseMessageBox(MSGCMD_SKILLLEARN_NOTIFY);
		}
		break;

	case MSGCMD_CHANGEJOB:			// 전직 할꺼에요?
		if( bOK )
		{
			// 서버에 메세지 보내기.
			_pNetwork->ChangeJobReq( m_iSelChangeJob );
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : MsgBoxLCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUISkillLearn::MsgBoxLCommand( int nCommandCode, int nResult )
{
	CTString strMessage;
	switch( nCommandCode )
	{
	case MSGLCMD_SKILLLEARN_REQ:
		if( nResult == SKILL_LEARN )					// 스킬을 습득한다.
		{
			_pUIMgr->RearrangeOrder( UI_SKILLLEARN, TRUE );

#ifdef HELP_SYSTEM_1
// [KH_07044] 3차 도움말 관련 추가
			if(g_iShowHelp1Icon)
			{
				_pUIMgr->GetHelp3()->ClearHelpString();
				_pUIMgr->GetHelp3()->AddHelpString(_S(3308, "캐릭터가 배울 수 있는 스킬의 목록이 보여집니다."));
				_pUIMgr->GetHelp3()->AddHelpString(_S(3309, "목록의 스킬 아이콘을 클릭하면 해당 스킬의 설명과 필요조건이 표시되며 배우고 싶은 스킬을 결정했다면 스킬 아이콘을 클릭 한 뒤 습득 버튼을 누릅니다."));
				_pUIMgr->GetHelp3()->AddHelpString(_S(3310, "※ 스킬을 배우는데 필요한 조건에 만족된 경우에만 스킬을 습득할 수 있습니다."));
				_pUIMgr->GetHelp3()->AddHelpString(_S(3311, "※ 스킬을 배울 때는 스킬 종류 및 레벨에 따라 필요한 숙련도가 자동 삭감됩니다."));
				_pUIMgr->GetHelp3()->AddHelpString(_S(3312, "※ 습득한 스킬은 캐릭터의 스킬 창(ALT+S)에서 확인할 수 있습니다."));
				_pUIMgr->GetHelp3()->OpenHelp(this);
			}
#endif

			InitSkillLearn( FALSE );
			m_nCurrentTab = SLEARN_TAB_ACTIVE;
		}
		else if( nResult == SKILL_TALK )				// 이야기 한다.
		{
			//TODO : NewQuestSystem
			// 퀘스트 창 띄우기
			CUIQuestBook::TalkWithNPC();
		}
		else if( nResult == SKILL_JOB_1 )				// 전직 클래스 1
		{
			_pUIMgr->CreateMessageBoxL( _S( 1224, "전직 정보" ), UI_SKILLLEARN, MSGLCMD_CHANGEJOB_REQ );		
			const int iNeedLevel	= EXTENSION_LEVEL;
			const SQUAD	llNeedNas	= 50000;
			const int iJob			= _pNetwork->MyCharacterInfo.job;

			// FiXME : 아래 부분은 정리가 필요한 부분.

			switch( iJob )
			{
			case TITAN:
				{
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, JobInfo().GetName( iJob, JOB_2ND_HIGHLANDER ), -1, 0xFFC672FF );				
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1225, "전 클래스 가운데서 공격력이 으뜸인 클래스로서 적에게 맹공을 퍼붓는 공격수로서의 몫을 톡톡히 해 내는 클래스입니다.\n" ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1226, "사용 무기 : 도끼" ), -1, 0xA3A1A3FF );							

					strMessage.PrintF( _S( 1227, "선결 조건 : %dlv달성, 보디 크래셔 lv5 습득" ), iNeedLevel );		
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );					
					strMessage.PrintF( _S( 1228, "필요 나스 : %I64d 나스" ), llNeedNas );		
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1229, "힘 : 공격상승, 물리방어 상승, 사용무게 증가." ), -1, 0xA3A1A3FF );		
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1230, "민첩 : 명중, 회피상승" ), -1, 0xA3A1A3FF );								
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1231, "지혜 : 마법 방어 상승, 몬스터 선공율 감소" ), -1, 0xA3A1A3FF );			
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1232, "체질 : 생명,마나 상승, 포션 효율 증대" ), -1, 0xA3A1A3FF );				
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1233, "전직 보너스 스탯 : 힘" ), -1, 0xA3A1A3FF );							
					
					//_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "타고난 강인한 육체에서 터져 나오는 투혼은 타이탄 중에서도 으뜸으로 인정받는 자들이 바로 하이랜더 입니다." ), -1, 0xA3A1A3FF );							
					//_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "이들은 전장에서 한 걸음의 물러섬없이 전장을 자유롭게 종횡무진하며 거대한 도끼를 자신의 육체처럼 자유롭게 사용합니다.\n" ), -1, 0xA3A1A3FF );							
				}
				break;
			case KNIGHT:		//  로열 나이트
				{
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, JobInfo().GetName( iJob, JOB_2ND_ROYALKNIGHT ), -1, 0xFFC672FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1234, "기본적으로 단련된 단단한 방어력과 높아진 공격력을 비롯한 빠른 공격으로 적을 제압하며 개인간 전투에서 안정적인 전투를 보여주는 클래스입니다." ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1235, "사용 무기 : 이도류" ), -1, 0xA3A1A3FF );							
					strMessage.PrintF( _S( 1236, "선결 조건 : %dlv달성, 마나 브레이크 lv5 습득" ), iNeedLevel );		
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							
					strMessage.PrintF( _S( 1228, "필요 나스 : %I64d 나스" ), llNeedNas );		
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1238, "힘 : 공격상승, 물리방어 상승, 인벤무게 증가" ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1239, "민첩 : 명중, 회피상승" ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1240, "지혜 : 마법 방어 상승, 몬스터 선공율 감소" ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1241, "체질 : 생명,마나 상승, 포션 효율 증대" ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1242, "전직 보너스 스탯 : 지혜" ), -1, 0xA3A1A3FF );							
					
					//_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "정의를 실천하는 방법으로서 악으로부터 소중한 존재를 보호하는 것에서, 악을 응징하는 것으로 선회한 자들이 바로 로열 나이트입니다. " ), -1, 0xA3A1A3FF );							
					//_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "로열 나이트는 그들을 지켜주던 굳건한 방패는 뒤로 하고 대신 예리하게 날이 선 두 자루의 검을 택하여 그들이 가진 능력 이상으로 공격력을 끌어 올렸습니다.\n" ), -1, 0xA3A1A3FF );							
					
					
				}
				break;
			case HEALER:
				{
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, JobInfo().GetName( iJob, JOB_2ND_ARCHER ), -1, 0xFFC672FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1243, "하이엘프 아처들은 화살 한발만으로도 다수의 적을 공격하거나, 빠른 이동속도, 활을 이용한 장거리 공격으로 히트 앤 런 방식의 공격에 특화된 능력을 가지고 있습니다." ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1244, "사용 무기 : 활" ), -1, 0xA3A1A3FF );							
					strMessage.PrintF( _S( 1245, "선결 조건 : %dlv달성, 더블 스팅샷 lv5 습득" ), iNeedLevel );		
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							
					strMessage.PrintF( _S( 1228, "필요 나스 : %I64d 나스" ), llNeedNas );		
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1246, "힘 :  물리방어 상승, 사용무게 증가" ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1247, "민첩 : 공격 상승, 명중, 회피상승" ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1248, "지혜 : 마법 방어 상승, 몬스터 선공율 감소" ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1249, "체질 : 생명,마나 상승, 포션 효율 증대" ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1250, "전직 보너스 스탯 : 힘" ), -1, 0xA3A1A3FF );							
					
					//_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "많은 하이엘프들이 그들이 선천적으로 타고난 힘인 마법을 이용해 동료들의 생명을 구하며 치유와 재생에 힘 쓰지만 때론 마법적인 힘보다, 물리적인 힘에 관심을 갖고 이에 집중하는 자들이 하이엘프 아처입니다.\n" ), -1, 0xA3A1A3FF );							
				}
				break;
			case MAGE:
				{
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, JobInfo().GetName( iJob, JOB_2ND_WIZARD ), -1, 0xFFC672FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1251, "높은 공격력으로 파괴적이고 넓은 공간을 일순간에 불바다로 만들어 버리는 광역마법에 특화 된 자들로 마법에 활용에 있어 으뜸인 클래스입니다." ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1252, "사용 무기 : 스태프" ), -1, 0xA3A1A3FF );							
					strMessage.PrintF( _S( 1253, "선결 조건 : %dlv달성, 노바 브레이크 lv5 습득" ), iNeedLevel );		
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							

					strMessage.PrintF( _S( 1228, "필요 나스 : %I64d 나스" ), llNeedNas );		
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1254, "힘 :  물리방어 상승, 사용무게 증가" ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1255, "민첩 : 명중, 회피상승" ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1256, "지혜 : 마법 공격상승, 마법 방어 상승, 몬스터 선공율 감소" ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1257, "체질 : 생명,마나 상승, 포션 효율 증대" ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1258, "전직 보너스 스탯 : 지혜" ), -1, 0xA3A1A3FF );							
					
					//_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "위자드는 마법 그 자체를 광범위하게 연구하는 메이지들 중에 마법의 원초적인 정수만을 뽑아 거대한 힘을 구성하는 자들입니다." ), -1, 0xA3A1A3FF );							
					//_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "위자드가 허공을 향해 성호를 그을 때마다 강력한 군세 자체가 소멸 될 정도의 위력을 지니기에 그 한 명 한 명이 하나의 군단급으로 평가된다.\n" ), -1, 0xA3A1A3FF );							
				}
				break;
			case ROGUE:
				{
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, JobInfo().GetName( iJob, JOB_2ND_ASSASSIN ), -1, 0xFFC672FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1378, "안정적인 공격, 방어 능력과 빠른 스킬 속도를 겸비한 클래스로서 1:1의 전투에서 맹 활약을 펼치는 기습형 클래스입니다." ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1379, "사용 무기 : 단검" ), -1, 0xA3A1A3FF );							
					strMessage.PrintF( _S( 1380, "선결 조건 : %dlv달성, 인비저빌리티 lv5 습득" ), iNeedLevel );		
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							

					strMessage.PrintF( _S( 1228, "필요 나스 : %I64d 나스" ), llNeedNas );		
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1381, "힘 :  물리방어 상승, 사용무게 증가" ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1382, "민첩 : 공격 상승 명중, 회피상승" ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1383, "지혜 : 마법 방어 상승, 몬스터 선공율 감소" ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1384, "체질 : 생명,마나 상승, 포션 효율 증대" ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1385, "전직 보너스 스탯 : 민첩" ), -1, 0xA3A1A3FF );							
					
					//_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "어세신은 짧은 단검과 날렵한 몸놀림을 이용한 초 접근전을 펼치는 것이 주된 전투법이며, 상대가 미쳐 단련하지 못은 부분이나 갑옷으로 감싸지 못한 미세한 틈을 노려 공격하며 조금이라도 빈틈을 보이면 곧장 급소를 공격하여 순식간에 상대의 목숨을 끊는 것이 어세신의 특기입니다.\n" ), -1, 0xA3A1A3FF );							
				}
				break;
			case SORCERER:
				{
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, JobInfo().GetName( iJob, 1 ), -1, 0xFFC672FF );									
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 2342,"엘리멘탈 리스트는 정령 소환을 마스터하여 불, 물, 바람, 대지로 구분되는 4대 정령들을 강화 시키고 그들을 소환하여 함께 전투에 임하는 클래스입니다."  ), -1, 0xA3A1A3FF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 2343,"사용 무기 : 폴암"  ), -1, 0xA3A1A3FF );

					strMessage.PrintF( _S(2344, "선결 조건 : %dlv달성, 아이스 스파이크 lv5 습득" ), iNeedLevel );		
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							

					strMessage.PrintF( _S( 1228, "필요 나스 : %I64d 나스" ), llNeedNas );		
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S(2345, "힘 : 물리방어 상승, 사용무게 증가"  ), -1, 0xA3A1A3FF );	
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S(2346, "민첩 : 명중, 회피상승"  ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S(2347, "지혜 : 마법 공격 상승, 마법 방어 상승, 몬스터 선공율 감소"  ), -1, 0xA3A1A3FF );		
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S(2348, "체질 : 생명, 마나 상승, 포션 효율 증대"  ), -1, 0xA3A1A3FF );			
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S(2349, "전직 보너스 스탯 : 지혜"  ), -1, 0xA3A1A3FF );											
					
				}
				break;
			}

			CTString strMessage;
			strMessage.PrintF( _S( 1267, "[%s]로 전직한다." ), JobInfo().GetExtensionName( _pNetwork->MyCharacterInfo.job, 0 ) );		
			_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, FALSE, strMessage, 0 );
			_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, FALSE, _S( 1268, "취소하기") );			

			m_iSelChangeJob = 1;
		}
		else if( nResult == SKILL_JOB_2 )				// 전직 클래스 2
		{
			_pUIMgr->CreateMessageBoxL( _S( 1224, "전직 정보" ), UI_SKILLLEARN, MSGLCMD_CHANGEJOB_REQ );		
			const int	iNeedLevel	= EXTENSION_LEVEL;
			const SQUAD	llNeedNas	= 50000;
			const int	iJob		= _pNetwork->MyCharacterInfo.job;

			// FiXME : 아래 부분은 정리가 필요한 부분.
			switch( iJob )
			{
			case TITAN:
				{
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, JobInfo().GetName( iJob, JOB_2ND_WARMASTER ), -1, 0xFFC672FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1269,  "대규모 전투에서 단연 눈에 뛰는 활약을 보여주며 하이랜더를 상회하는 공격력과 전 클래스를 통틀어 가장 높은 체력을 가지고 있습니다."  ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1270,  "사용 무기 : 대검"  ), -1, 0xA3A1A3FF );							
					strMessage.PrintF( _S( 1271, "선결 조건 : %dlv달성, 보디 크래셔 lv5 습득" ), iNeedLevel );		
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							
					strMessage.PrintF( _S( 1228, "필요 나스 : %I64d 나스" ), llNeedNas );		
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1272,  "힘 : 공격상승, 물리방어 상승, 사용무게 증가"  ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1273,  "민첩 : 명중, 회피상승"  ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1274,  "지혜 : 마법 방어 상승, 몬스터 선공율 감소"  ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1275,  "체질 : 생명,마나 상승, 포션 효율 증대"  ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1276,  "전직 보너스 스탯 : 체질"  ), -1, 0xA3A1A3FF );							
					
					//_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "호전적인 성격으로 오랜 시간 전투가 몸에 베인 타이탄 중에 탁월한 감각으로 전장의 세를 읽고 순발력있게 대처하거나 아군의 사기를 높은 함성으로 붇돋아 주는 클래스가 이들 워 마스터입니다.\n" ), -1, 0xA3A1A3FF );							
					
					
				}
				break;
			case KNIGHT:		// 템플 나이트
				{
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, JobInfo().GetName( iJob, JOB_2ND_TEMPLEKNIGHT ), -1, 0xFFC672FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1277,  "파티원을 위험해서 구하거나, 몬스터를 조롱하여 자신에게 덤벼들게 하는 등의 파티에서 든든한 벽이 되어 주며, 대규모 전투에서도 선봉장의 역할을 수행하게 됩니다."  ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1278,  "사용 무기 : 검과 방패"  ), -1, 0xA3A1A3FF );							

					strMessage.PrintF( _S( 1279, "선결 조건 : %dlv달성, 마나 브레이크 lv5 습득" ), iNeedLevel );		
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							

					strMessage.PrintF( _S( 1228, "필요 나스 : %I64d 나스" ), llNeedNas );		
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1280,  "힘 : 공격상승, 물리방어 상승, 사용무게 증가"  ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1281,  "민첩 : 명중, 회피상승"  ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1282,  "지혜 : 마법 방어 상승, 몬스터 선공율 감소"  ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1283,  "체질 : 생명,마나 상승, 포션 효율 증대"  ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1284,  "전직 보너스 스탯 : 체질"  ), -1, 0xA3A1A3FF );							
					
					//_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "희생을 자신들이 가진 최고의 미덕으로 여기고 있는 템플나이트는 파티원을 위험에서 보호하며 그들을 지원하여, 팀 플레이를 원활하게 해 주는 윤활유 같은 역할을 해 주는 존재입니다. " ), -1, 0xA3A1A3FF );							
					//_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "때문에 이들의 공격력은 로열 나이트에 비해 비록 뒤쳐지지만, 높은 방어력과 튼튼한 체력은 이들이 가진 큰 장점입니다.\n" ), -1, 0xA3A1A3FF );							
					
					
				}
				break;
			case HEALER:
				{
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, JobInfo().GetName( iJob, JOB_2ND_CLERIC ), -1, 0xFFC672FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1285, "높은 회복력으로 적을 치유하며, 동료의 능력을 최대치로 끌어올려주는 보조마법 등을 사용하는 등 파티 플레이의 든든한 지원자 역할을 수행하는 클래스 입니다."  ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1286, "사용 무기 : 완드"  ), -1, 0xA3A1A3FF );							

					strMessage.PrintF( _S( 1287, "선결 조건 : %dlv달성, 더블 스팅샷 lv5 습득" ), iNeedLevel );		
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							

					strMessage.PrintF( _S( 1228, "필요 나스 : %I64d 나스" ), llNeedNas );		
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1288, "힘 :  물리방어 상승, 사용무게 증가"  ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1289, "민첩 : 명중, 회피상승"  ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1290, "지혜 : 마법 방어 상승, 몬스터 선공율 감소"  ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1291, "체질 : 생명,마나 상승, 포션 효율 증대"  ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1292, "전직 보너스 스탯 : 지혜"  ), -1, 0xA3A1A3FF );							
					
					//_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "하이엘프들 가운데 파괴를 증오하고, 창조와 재생에 관해 몰두하는 하이엘프 클레릭은 높은 마법력으로 동료들에게 생명력을 불어 넣어 주며, 독, 결빙, 상태이상의 마법에서 육체를 자유로울 수 있도록 도와줍니다.\n" ), -1, 0xA3A1A3FF );				
					
					
				}
				break;
			case MAGE:
				{
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, JobInfo().GetName( iJob, JOB_2ND_WITCH ), -1, 0xFFC672FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1293, "상태이상 마법 및 저주 마법을 사용하는 클래스로서 적이 가공할 힘을 가졌다 하더라도 그 힘을 원천적으로 봉쇄하는등의 지능적인 마법사 입니다."  ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1294, "사용 무기 : 완드"  ), -1, 0xA3A1A3FF );							

					strMessage.PrintF( _S( 1295, "선결 조건 : %dlv달성, 노바 브레이크 lv5 습득" ), iNeedLevel );		
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							

					strMessage.PrintF( _S( 1228, "필요 나스 : %I64d 나스" ), llNeedNas );		
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1296,  "힘 :  물리방어 상승, 사용무게 증가"  ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1297,  "민첩 : 명중, 회피상승"  ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1298,  "지혜 : 마법 공격상승, 마법 방어 상승, 몬스터 선공율 감소"  ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1299,  "체질 : 생명,마나 상승, 포션 효율 증대"  ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1300,  "전직 보너스 스탯 : 민첩"  ), -1, 0xA3A1A3FF );							
					
					//_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "위치는 위험스럽고, 치명적이며 은밀한 마법을 연구하는 자들입니다. 이들이 행하는 마법의 연구대상은 다름아닌 인간의 육체이며, 작은 마법의 흐름으로 적을 혼란에 빠뜨리거나, 잠재우고, 신체조직을 변이 시키는 등의 마법을 사용해 적을 무기력하게 만듭니다.\n" ), -1, 0xA3A1A3FF );							
					
					
				}
				break;
			case ROGUE:
				{
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, JobInfo().GetName( iJob, JOB_2ND_RANGER ), -1, 0xFFC672FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1301,  "레인저는 주변의 자연지물을 이용한 함정과 석궁을 이용한 장거리 공격을 하며, 속사의 명수입니다."  ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1302,  "사용 무기 : 석궁"  ), -1, 0xA3A1A3FF );							

					strMessage.PrintF( _S( 1303, "선결 조건 : %dlv달성, 인비저빌리티 lv5 습득" ), iNeedLevel );		
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							

					strMessage.PrintF( _S( 1228, "필요 나스 : %I64d 나스" ), llNeedNas );		
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1304,  "힘 :  물리방어 상승, 사용무게 증가"  ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1305,  "민첩 : 공격 상승 명중, 회피상승"  ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1306,  "지혜 : 마법 방어 상승, 몬스터 선공율 감소"  ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1307,  "체질 : 생명,마나 상승, 포션 효율 증대"  ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1308,  "전직 보너스 : 힘"  ), -1, 0xA3A1A3FF );							
					
					//_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "레인저의 두려움은 근거리가 아닌 중, 원거리에서의 전투와 다양한 함정과 도구의 활용에서 나타난다." ), -1, 0xA3A1A3FF );							
					//_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "짧은 시간 내에도 그 곳의 환경을 활용한 다양한 트랩을 설치하는 것은 물론 어떤 상황에서도 어김없이 급소를 향해 정확한 공격이 가능한 레인저는 자신의 몸놀림을 활용하여 사방팔방을 전장을 활보한다.\n" ), -1, 0xA3A1A3FF );							
					
					
				}
				break;
			case SORCERER:
				{
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, JobInfo().GetName( iJob, 2 ), -1, 0xFFC672FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S(2350,"스페셜리스트는 정령의 힘을 빌어 불, 바람의 정령 강신과 물, 땅의 정령 강신을 통해 자신의 신체를 강화시켜 직접 전투에 임하는 전투 스타일을 갖는 클래스 입니다. "), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S(2351, "사용 무기 : 사이드" ), -1, 0xA3A1A3FF );							

					strMessage.PrintF( _S(2344, "선결 조건 : %dlv달성, 아이스 스파이크 lv5 습득" ), iNeedLevel );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							

					strMessage.PrintF( _S( 1228, "필요 나스 : %I64d 나스" ), llNeedNas );		
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S(2345, "힘 : 물리방어 상승, 사용무게 증가" ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S(2346, "민첩 : 명중, 회피상승" ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S(2347, "지혜 : 마법 공격상승, 마법 방어 상승, 몬스터 선공율 감소" ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S(2348, "체질 : 생명,마나 상승, 포션 효율 증대" ), -1, 0xA3A1A3FF );							
					_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S(2349, "전직 보너스 스탯 : 지혜" ), -1, 0xA3A1A3FF );												
				}
				break;
			}
			
			
			CTString strMessage;
			strMessage.PrintF( _S( 1267, "[%s]로 전직한다." ), JobInfo().GetExtensionName( _pNetwork->MyCharacterInfo.job, 1 ) );		
			_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, FALSE, strMessage, 0 );
			_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, FALSE, _S( 1268, "취소하기" ) );			

			m_iSelChangeJob = 2;
		}
#ifdef HELP_SYSTEM_1
		else if(nResult == SKILL_NPC_HELP)											// 취소한다.
		{
			_pUIMgr->RearrangeOrder( UI_NPCHELP, TRUE );
		}
#endif
		// [090527: selo] 확장팩 퀘스트 수정
		else if( ciQuestClassifier < nResult )	
		{
			// 선택한 퀘스트에 대해 수락 또는 보상 창을 연다.
			CUIQuestBook::SelectQuestFromMessageBox( nResult );
		}

		else
		{
			CloseSkillLearn();
		}
		break;

	case MSGLCMD_CHANGEJOB_REQ:							// 전직 정보창.
		{
			if( nResult == 0 )
			{				
				_pUIMgr->CloseMessageBox( MSGCMD_CHANGEJOB );				
				
				CTString	strMessage;
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S( 1317, "전직" ), UMBS_OKCANCEL, UI_SKILLLEARN, MSGCMD_CHANGEJOB );		
				strMessage.PrintF( _S( 1318, "%s로 전직 하시겠습니까?" ), JobInfo().GetExtensionName( _pNetwork->MyCharacterInfo.job, m_iSelChangeJob - 1 ) );	
				MsgBoxInfo.AddString( strMessage );
				_pUIMgr->CreateMessageBox( MsgBoxInfo );
			}
			else
			{
			}
		}
		break;

		// Special Skill
	case MSGLCMD_SSKILLLEARN_REQ:
		if( nResult == SKILL_LEARN )					// 스킬을 습득한다.
		{
			_pUIMgr->RearrangeOrder( UI_SKILLLEARN, TRUE );

			InitSkillLearn( TRUE );
			m_nCurrentTab = SLEARN_TAB_SPECIAL;
		}
		else if( nResult == SKILL_TALK )				// 이야기 한다.
		{
			//TODO : NewQuestSystem
			// 퀘스트 창 띄우기
			CUIQuestBook::TalkWithNPC();
		}
#ifdef HELP_SYSTEM_1
		else if( nResult == SKILL_NPC_HELP)								
		{
			_pUIMgr->RearrangeOrder( UI_NPCHELP, TRUE );
		}
#endif
		// [090527: selo] 확장팩 퀘스트 수정
		else if( ciQuestClassifier < nResult )	
		{
			// 선택한 퀘스트에 대해 수락 또는 보상 창을 연다.
			CUIQuestBook::SelectQuestFromMessageBox( nResult );
		}

		else								// 취소한다.
		{
			CloseSkillLearn();
		}
		break;
	case MSGLCMD_RESERVE_REQ:
		{
			// [090527: selo] 확장팩 퀘스트 이야기 한다 처리 수정을 위한 루틴
			int iQuestIndex = -1;
			if( ciQuestClassifier < nResult )	
			{
				iQuestIndex = nResult;
				nResult = ciQuestClassifier;
			}

			switch(nResult)
			{
			_pUIMgr->SetCSFlagOff( CSF_SHOP );

			case SKILL_TITAN:
				{
					OpenSkillLearn(75,FALSE,m_fNpcX,m_fNpcZ);
				}
				break;
			case SKILL_KNIGHT:
				{
					OpenSkillLearn(76,FALSE,m_fNpcX,m_fNpcZ);
				}
				break;
			case SKILL_HEALER:
				{
					OpenSkillLearn(77,FALSE,m_fNpcX,m_fNpcZ);
				}
				break;
			case SKILL_MAGE:
				{
					OpenSkillLearn(123,FALSE,m_fNpcX,m_fNpcZ);
				}
				break;
			case SKILL_ROGUE:
				{
					OpenSkillLearn(176,FALSE,m_fNpcX,m_fNpcZ);
				}
				break;
			case SKILL_SORCERER:
				{
					OpenSkillLearn(255,FALSE,m_fNpcX,m_fNpcZ);
				}
				break;
			case SKILL_QUEST:
				{
					CUIQuestBook::TalkWithNPC();
				}
				break;
			// [090527: selo] 확장팩 퀘스트 수정
			case ciQuestClassifier:
				{
					// 선택한 퀘스트에 대해 수락 또는 보상 창을 연다.
					CUIQuestBook::SelectQuestFromMessageBox( iQuestIndex );					
				}
				break;
			default:
				{
					m_bUseCard = FALSE;
				}
			}
		}break;


	}
}

// ========================================================================= //
//                           Send message functions                          //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : LearnSkill()
// Desc :
// ----------------------------------------------------------------------------
void CUISkillLearn::SendLearnSkill()
{
	// Close message box of skill learn
	_pUIMgr->CloseMessageBox( MSGCMD_SKILLLEARN_NOTIFY );
	
	if( _pNetwork->MyCharacterInfo.job == SORCERER )
	{
		// 강신상태와 소환상태에서는 스킬을 배울수 없습니다.
		if( _pNetwork->MyCharacterInfo.sbEvocationType != -1 )
		{
			// 강신상태이므로, 배울수 없음.
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 270, "스킬" ), UMBS_OK,
				UI_SKILLLEARN, MSGCMD_SKILLLEARN_NOTIFY );
			MsgBoxInfo.AddString( _S(2352,"강신 상태에서는 스킬을 배울수 없습니다.") );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );
			return;
		}	
		
		// 소환상태에서는 스킬을 배울수 없음.
		for( int i = UI_SUMMON_START; i <= UI_SUMMON_END; ++i )
		{
			CUISummon* pUISummon = (CUISummon*)_pUIMgr->GetUI(i);
			if( pUISummon->GetSummonEntity() )
			{
				// Create message box of skill learn
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S( 270, "스킬" ), UMBS_OK,
					UI_SKILLLEARN, MSGCMD_SKILLLEARN_NOTIFY );
				MsgBoxInfo.AddString( _S(2353,"소환 상태에서는 스킬을 배울수 없습니다.") );
				_pUIMgr->CreateMessageBox( MsgBoxInfo );
				return;
			}
		}
	}

	SLONG	slIndex;
#ifdef NEW_USER_INTERFACE
	if( m_nSelectedSkillID < 0 )
		return;

	if( m_btnSelectedSkill[m_nSelectedSkillID].IsEmpty() )
		return;

	slIndex = m_btnSelectedSkill[m_nSelectedSkillID].GetSkillIndex();
	if(m_nCurrentSkillType != SKILL_SPECIAL)
	{	
		_pNetwork->SendSkillLearn( slIndex );
	}
	else
	{
		_pNetwork->SendSSkillLearn( slIndex );
	}
#else
	if( m_nCurrentTab == SLEARN_TAB_ACTIVE )
	{
		if( m_nSelActiveSkillID < 0 )
			return;

		if( m_btnActiveSkills[m_nSelActiveSkillID].IsEmpty() )
			return;

		slIndex = m_btnActiveSkills[m_nSelActiveSkillID].GetSkillIndex();
		_pNetwork->SendSkillLearn( slIndex );
	}
	else if( m_nCurrentTab == SLEARN_TAB_PASSIVE )
	{
		if( m_nSelPassiveSkillID < 0 )
			return;

		if( m_btnPassiveSkills[m_nSelPassiveSkillID].IsEmpty() )
			return;

		slIndex = m_btnPassiveSkills[m_nSelPassiveSkillID].GetSkillIndex();
		_pNetwork->SendSkillLearn( slIndex );
	}
	#ifdef ADJUST_MEMORIZE_SKILL
	else if( m_nCurrentTab == SLERAN_TAB_MEMORIZE )
	{
		if( m_nSelMemorizeSkillID < 0 )
			return;

		if( m_btnMemorizeSkills[m_nSelMemorizeSkillID].IsEmpty() )
			return;

		slIndex = m_btnMemorizeSkills[m_nSelMemorizeSkillID].GetSkillIndex();
		_pNetwork->SendSkillLearn( slIndex );
	}
	#endif
	else
	{
		if( m_nSelSpecialSkillID < 0 )
			return;

		if( m_btnSpecialSkills[m_nSelSpecialSkillID].IsEmpty() )
			return;

		slIndex = m_btnSpecialSkills[m_nSelSpecialSkillID].GetSkillIndex();
		_pNetwork->SendSSkillLearn( slIndex );
	}
#endif
}


// ========================================================================= //
//                         Receive message functions                         //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : LearnSkill()
// Desc :
// ----------------------------------------------------------------------------
void CUISkillLearn::LearnSkill( SLONG slIndex, SBYTE sbIsNew, SBYTE sbLevel, BOOL bSpecial )
{
	if( !bSpecial )
	{
		CSkill		&rSkill = _pNetwork->GetSkillData( slIndex );
		
		// Close message box of skill learn
		_pUIMgr->CloseMessageBox( MSGCMD_SKILLLEARN_NOTIFY );
		
		// Create message box of skill learn
		CTString	strMessage;
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 270, "스킬" ), UMBS_OK, UI_SKILLLEARN, MSGCMD_SKILLLEARN_NOTIFY );
		strMessage.PrintF( _S( 277, "%s 스킬을 습득하였습니다" ), rSkill.GetName() );
		MsgBoxInfo.AddString( strMessage );
		_pUIMgr->CreateMessageBox( MsgBoxInfo );
		
		if( sbIsNew == 0 )
		{
			_pUIMgr->GetCharacterInfo()->UpdateSkillLevel( slIndex, sbLevel, bSpecial );
			_pUIMgr->GetQuickSlot()->UpdateSkillLevel( slIndex, sbLevel );
		}
		else
			_pUIMgr->GetCharacterInfo()->AddSkill( slIndex, sbLevel, bSpecial );
		
		BOOL	bUpdate = FALSE;

#ifdef NEW_USER_INTERFACE
		for( int iRow = 0; iRow < SKILLLEARN_NEW_SLOT_TOTAL; iRow++ )
		{
			if( m_btnSelectedSkill[iRow].GetSkillIndex() == slIndex )
			{
				m_btnSelectedSkill[iRow].SetSkillLevel( sbLevel + 1 );
				bUpdate = TRUE;
				break;
			}
		}
		

		if( sbLevel >= rSkill.GetMaxLevel() )
		{

			m_nSelectedSkillID = -1;
			
			InitSkillLearn( bSpecial );
			
			return;
		}
#else
		// Not Special Skill
		for( int iRow = 0; iRow < SKILLLEARN_NEW_SLOT_TOTAL; iRow++ )
		{
			if( m_btnActiveSkills[iRow].GetSkillIndex() == slIndex )
			{
				m_btnActiveSkills[iRow].SetSkillLevel( sbLevel + 1 );
				bUpdate = TRUE;
				break;
			}
		}
		
		if( !bUpdate )
		{
			for( iRow = 0; iRow < SKILLLEARN_NEW_SLOT_TOTAL; iRow++ )
			{
				if( m_btnPassiveSkills[iRow].GetSkillIndex() == slIndex )
				{
					m_btnPassiveSkills[iRow].SetSkillLevel( sbLevel + 1 );
					break;
				}
			}
		}

#ifdef ADJUST_MEMORIZE_SKILL
		/*
		for( iRow = 0; iRow < SLEARN_SLOT_ROW_TOTAL; iRow++ )
		{
			if( m_btnMemorizeSkills[iRow].GetSkillIndex() == slIndex )
			{
				m_btnMemorizeSkills[iRow].SetSkillLevel( sbLevel + 1 );
				break;
			}
		}
		*/
#endif
		
		if( sbLevel >= rSkill.GetMaxLevel() )
		{
			if( m_nCurrentTab == SLEARN_TAB_ACTIVE )
				m_nSelActiveSkillID = -1;
			else if( m_nCurrentTab == SLEARN_TAB_PASSIVE )
				m_nSelPassiveSkillID = -1;
#ifdef ADJUST_MEMORIZE_SKILL
			else if( m_nCurrentTab == SLERAN_TAB_MEMORIZE )
				m_nSelMemorizeSkillID = -1;
#endif
			else
				m_nSelSpecialSkillID = -1;
			
			GetSkillDesc( -1 );
			
			InitSkillLearn( bSpecial );
			
			return;
		}
#endif
	}
	else
	{
		CSpecialSkill	&rSkill = _pNetwork->GetSSkillData( slIndex );
		
		// Close message box of skill learn
		_pUIMgr->CloseMessageBox( MSGCMD_SKILLLEARN_NOTIFY );
		
		// Create message box of skill learn
		CTString	strMessage;
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 270, "스킬" ), UMBS_OK, UI_SKILLLEARN, MSGCMD_SKILLLEARN_NOTIFY );
		strMessage.PrintF( _S( 277, "%s 스킬을 습득하였습니다" ), rSkill.GetName() );
		MsgBoxInfo.AddString( strMessage );
		_pUIMgr->CreateMessageBox( MsgBoxInfo );
		
		if( sbIsNew == 0 )
		{
			_pUIMgr->GetCharacterInfo()->UpdateSkillLevel( slIndex, sbLevel, bSpecial );
			_pUIMgr->GetQuickSlot()->UpdateSkillLevel( slIndex, sbLevel );
		}
		else
			_pUIMgr->GetCharacterInfo()->AddSkill( slIndex, sbLevel, bSpecial );
		
		BOOL	bUpdate = FALSE;		
		if( !bUpdate )
		{
			for( int iRow = 0; iRow < SKILLLEARN_NEW_SLOT_TOTAL; iRow++ )
			{
				if( m_btnSpecialSkills[iRow].GetSkillIndex() == slIndex )
				{
					m_btnSpecialSkills[iRow].SetSkillLevel( sbLevel + 1 );
					break;
				}
			}
		}
		
		if( sbLevel >= rSkill.GetMaxLevel() )
		{
#ifdef NEW_USER_INTERFACE
		if( sbLevel >= rSkill.GetMaxLevel() )
		{
			m_nSelectedSkillID = -1;
			
			InitSkillLearn( bSpecial );
			
			return;
		}
#else
			if( m_nCurrentTab == SLEARN_TAB_ACTIVE )
				m_nSelActiveSkillID = -1;
			else if( m_nCurrentTab == SLEARN_TAB_PASSIVE )
				m_nSelPassiveSkillID = -1;
#ifdef ADJUST_MEMORIZE_SKILL
			else if( m_nCurrentTab == SLERAN_TAB_MEMORIZE )
				m_nSelMemorizeSkillID = -1;
#endif
			else
				m_nSelSpecialSkillID = -1;
			
			GetSkillDesc( -1 );
			
			InitSkillLearn( bSpecial );
			
			return;
#endif
		}
	}
	
#ifndef NEW_USER_INTERFACE
	GetSkillDesc( slIndex, sbLevel + 1, bSpecial );
#endif
}

// ----------------------------------------------------------------------------
// Name : LearnSkillError()
// Desc :
// ----------------------------------------------------------------------------
void CUISkillLearn::LearnSkillError( UBYTE ubError )
{
	CTString	strMessage;

	switch( ubError )
	{
	case MSG_SKILL_LEARN_ERROR_LEVEL:
		strMessage = _S( 278, "레벨이 부족하여 스킬을 습득할 수 없습니다." );
		break;

	case MSG_SKILL_LEARN_ERROR_SP:
		strMessage = _S( 279, "숙련도가 부족하여 스킬을 습득할 수 없습니다." );
		break;

	case MSG_SKILL_LEARN_ERROR_ITEM:
		strMessage = _S( 280, "아이템이 존재하지 않아 스킬을 습득할 수 없습니다." );
		break;

	case MSG_SKILL_LEARN_ERROR_SKILL:
		strMessage = _S( 281, "스킬 조건이 맞지 않아 스킬을 습득할 수 없습니다." );
		break;

	case MSG_SKILL_LEARN_ERROR_SYSTEM:
		strMessage = _S( 282, "스킬을 습득할 수 없습니다." );
		break;

	case MSG_SKILL_LEARN_ERROR_STR:			// 힘 부족
		strMessage = _S( 1319, "힘이 부족하여 스킬을 습득할 수 없습니다." );		
		break;

	case MSG_SKILL_LEARN_ERROR_DEX:			// 덱스 부족
		strMessage = _S( 1320, "민첩이 부족하여 스킬을 습득할 수 없습니다." );		
		break;

	case MSG_SKILL_LEARN_ERROR_INT:			// 지혜 부족
		strMessage = _S( 1321, "지혜가 부족하여 스킬을 습득할 수 없습니다." );		
		break;

	case MSG_SKILL_LEARN_ERROR_CON:			// 체질 부족
		strMessage = _S( 1322, "체질이 부족하여 스킬을 습득할 수 없습니다." );		
		break;
	}

	// Close message box of skill learn
	_pUIMgr->CloseMessageBox( MSGCMD_SKILLLEARN_NOTIFY );

	// Create message box of skill learn
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 270, "스킬" ), UMBS_OK,
								UI_SKILLLEARN, MSGCMD_SKILLLEARN_NOTIFY );
	MsgBoxInfo.AddString( strMessage );
	_pUIMgr->CreateMessageBox( MsgBoxInfo );
}

// ----------------------------------------------------------------------------
// Name : LearnSSkillError()
// Desc :
// ----------------------------------------------------------------------------
void CUISkillLearn::LearnSSkillError( UBYTE ubError )
{
	CTString	strMessage;

	switch(ubError)
	{
	case MSG_SSKILL_LEARN_ERROR_LEVEL:
		strMessage = _S( 278, "레벨이 부족하여 스킬을 습득할 수 없습니다." );
		break;
	case MSG_SSKILL_LEARN_ERROR_SP:
		strMessage = _S( 279, "숙련도가 부족하여 스킬을 습득할 수 없습니다." );
		break;
	case MSG_SSKILL_LEARN_ERROR_SSKILL:
		strMessage = _S( 654, "해당 기술을 배우는데 필요한 기술을 배우지 않았습니다." );			
		break;
	case MSG_SSKILL_LEARN_ERROR_SSKILL_LEVEL:
		strMessage = _S( 655, "스킬 레벨이 부족합니다." );	
		break;
	case MSG_SSKILL_LEARN_ERROR_SYSTEM:
		strMessage = _S( 282, "스킬을 습득할 수 없습니다." );
		break;
	}
	
	// Close message box of skill learn
	_pUIMgr->CloseMessageBox( MSGCMD_SKILLLEARN_NOTIFY );

	// Create message box of skill learn
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 656, "생산스킬" ), UMBS_OK,			
								UI_SKILLLEARN, MSGCMD_SKILLLEARN_NOTIFY );
	MsgBoxInfo.AddString( strMessage );
	_pUIMgr->CreateMessageBox( MsgBoxInfo );
}
// ----------------------------------------------------------------------------
// Name : PriorityOpen()
// Desc : 어둠의 스킬 마스터의 UI 창
//			스킬 습득 UI 창보다 먼저 실행
// ----------------------------------------------------------------------------	
void CUISkillLearn::PriorityOpen(int iIndex, BOOL bUseCard)
{
	if (_pUIMgr->DoesMessageBoxLExist(MSGLCMD_RESERVE_REQ)) return;
	
	m_bUseCard = bUseCard; // 카드 사용 유무

	CTString	strNpcName = _pNetwork->GetMobName(iIndex);
	// Create skill learn message box
	_pUIMgr->CreateMessageBoxL( _S( 270, "스킬" ), UI_SKILLLEARN, MSGLCMD_RESERVE_REQ );

	_pUIMgr->AddMessageBoxLString( MSGLCMD_RESERVE_REQ, TRUE, strNpcName, -1, 0xE18600FF );
	_pUIMgr->AddMessageBoxLString( MSGLCMD_RESERVE_REQ, TRUE, _S( 271, "끊임없이 노력하는 자만이 진정한 강함을 손에 넣을 수 있는 법!\n\n그런 강인한 의지가 있는 자를 인도하는 것이 내가 하는 일이지.\n" ), -1, 0xA3A1A3FF );
	_pUIMgr->AddMessageBoxLString( MSGLCMD_RESERVE_REQ, TRUE, _S( 3541,"그대 또한 강함을 쫓는 자인가? 그런 강인한 의지가 있는 자를 인도 하는 것이 내 일이지! 각오는 되어 있겠지?"), -1, 0xA3A1A3FF);

	_pUIMgr->AddMessageBoxLString( MSGLCMD_RESERVE_REQ, FALSE, _S( 3555, "타이탄 스킬을 습득한다."),SKILL_TITAN);
	_pUIMgr->AddMessageBoxLString( MSGLCMD_RESERVE_REQ, FALSE, _S( 3556, "나이트 스킬을 습득한다."),SKILL_KNIGHT);
	_pUIMgr->AddMessageBoxLString( MSGLCMD_RESERVE_REQ, FALSE, _S( 3557, "힐러 스킬을 습득한다."),SKILL_HEALER);
	_pUIMgr->AddMessageBoxLString( MSGLCMD_RESERVE_REQ, FALSE, _S( 3558, "메이지 스킬을 습득한다."),SKILL_MAGE);
	_pUIMgr->AddMessageBoxLString( MSGLCMD_RESERVE_REQ, FALSE, _S( 3559, "로그 스킬을 습득한다."),SKILL_ROGUE);
	_pUIMgr->AddMessageBoxLString( MSGLCMD_RESERVE_REQ, FALSE, _S( 3560, "소서러 스킬을 습득한다."),SKILL_SORCERER);
	
#ifdef	NEW_QUESTBOOK
	// 2009. 05. 27 김정래
	// 이야기한다 변경 처리
	CUIQuestBook::AddQuestListToMessageBoxL(MSGLCMD_RESERVE_REQ);				
#else
	_pUIMgr->AddMessageBoxLString( MSGLCMD_RESERVE_REQ, FALSE, _S(1053, "이야기한다."), SKILL_QUEST);
#endif
			
	_pUIMgr->AddMessageBoxLString( MSGLCMD_RESERVE_REQ, FALSE, _S( 1220, "취소한다." ) );

}
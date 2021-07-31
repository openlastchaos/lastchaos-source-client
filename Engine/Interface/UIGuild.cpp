#include "stdh.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIGuild.h>
#include <algorithm>

// WSS_NEW_GUILD_SYSTEM 070704
#ifdef NEW_GUILD_SYSTEM
#define MAX_GUILD_LEVEL		(50)
#else
#define MAX_GUILD_LEVEL		(5)
#endif

extern INDEX g_iCountry;
// Date : 2005-03-07,   By Lee Ki-hwan
static int	_iMaxMsgStringChar = 0;
#define GUILD_NPC			89
#define pWEB					m_pWebBoard

// WSS_NEW_GUILD_SYSTEM 070705 ---------------------------->><<
// 길드 시스템 추가로 GP및 레벨 상향 조정

// 길드 제한 조건
struct sGuildConditionTable
{
	int		iLevel;				// 길드 레벨
	int		iCaptionNeedLevel;	// 단장의 최소 조건 레벨
	int		iNeedSP;			// 필요 SP
	SQUAD	llNeedMoney;		// 필요 나스
	int		iMaxMember;			// 길드 인원.
	int		iNeedGP;			// 필요 GP 
};

// 길드 제한 조건 테이블
sGuildConditionTable _GuildConditionTable[MAX_GUILD_LEVEL] = 
{
	{ 1, 10, 10, 10000, 10 ,0},				// 1 LV
	{ 2, 15, 30, 50000, 15 ,0},				// 2 LV
	{ 3, 20, 60, 250000, 20 ,0},			// 3 LV
	{ 4, 25, 120, 500000, 25 ,0},			// 4 LV
	{ 5, 30, 240, 1000000, 30 ,0},			// 5 LV
#ifdef NEW_GUILD_SYSTEM
	{ 6, 30 , 500 , 2000000, 30 ,0},		// 6 LV
	{ 7, 30 , 550 , 2500000, 30 ,18000},	// 7 LV // 기준 레벨 (Nas/GP/SP계산시)
#endif
};

enum eSelection
{
	SEL_GUILD_CREATE,
	SEL_GUILD_UPGRADE,
	SEL_GUILD_DESTROY,
	SEL_GUILD_ROOM,
	SEL_GUILD_TALK,
};

// FIXME : 아래 이름이 개판.
#define	GUILD_TAB_WIDTH					(100)
#define START_BUTTON_Y					(GUILD_HEIGHT - 123)	// 284 - 123 = 161
#define DESC_LIST_BOX_HEIGHT			(190)
#define MEMBER_LIST_BOX_WIDTH			(190)
#define MEMBER_LIST_BOX_HEIGHT			(88)
#define	EDIT_BOX_Y						(GUILD_HEIGHT - 57)		// 284 - 57 = 227
#define START_BOTTOM_BUTTON_Y			(GUILD_HEIGHT - 29)		// 284 - 29 = 255
#define SPAN_TEXT_Y						(17)

#define	GUILD_DESC_CHAR_WIDTH			168

//WSS_NEW_GUILD_SYSTEM 070716
#define MAX_NOTICE_TITLE			(64)
#define MAX_NOTICE_CONTENTS			(512)
#define MAX_POSITION_NAME			(12)
#define MAX_PERCENT_LENGTH			(3)
#define NEW_GUILD_SYSTEM_WIDTH		(602)
#define NEW_GUILD_SYSTEM_HEIGHT		(346)
#define GUILD_SKILL_SLOT_ROW		(5)
#define GUILD_SKILL_SLOT_OFFSETY	(40)
/*
#ifdef LIMIT_GUILD_LEVEL
#undef LIMIT_GUILD_LEVEL
#define LIMIT_GUILD_LEVEL 0
#endif*/
// ----------------------------------------------------------------------------
// Name : CUIGuild()
// Desc : 
// ----------------------------------------------------------------------------
CUIGuild::CUIGuild()
{
	m_eGuildState			= GUILD_REQ;
	m_nCurrentTab			= GUILD_TAB_MEMBER;
	m_strBossName			= "";
	m_strViceBoss[0]		= "";
	m_strViceBoss[1]		= "";
	m_strGuildName			= "";
	m_iGuildLevel			= -1;
	m_iGuildIndex			= -1;
	m_iNumOfMember			= 0;
	m_iUserRanking			= GUILD_MEMBER_MEMBER;
	m_bChackNpc				= FALSE;
// WSS_NEW_GUILD_SYSTEM 070716 -------------------------------------->>
	m_iSelTab				= 0;				// 디폴트 텝
	m_iGuildAverageLevel	= 0;				// 길드 평균 레벨
	m_iGuildOwnLand			= 0;				// 길드 소유 영지
	m_iGuildTotalPoint		= 0;				// 길드 총 포인트
	m_iGuildMyPoint			= 0;				// 나의 기여 포인트
	m_bApplySettingOn		= FALSE;
	m_iNumOfMaxMember		= 0;
	m_iGuildSkillPos		= 0;
	m_bIsSelList			= FALSE;
	m_btnGuildSkill			= NULL;
	m_pWebBoard				= NULL;
	m_bEnableCorrect		= FALSE;
// ------------------------------------------------------------------<<
	
}

// ----------------------------------------------------------------------------
// Name : ~CUIGuild()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIGuild::~CUIGuild()
{
	if(!m_vectorMemberList.empty())
	{
		m_vectorMemberList.clear();
	}
	if (m_btnGuildSkill) 
		delete[] m_btnGuildSkill;
	m_arGuildSkillList.Clear();

	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIGuild::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	_iMaxMsgStringChar = GUILD_DESC_CHAR_WIDTH / ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 216, 22 );
	m_rcTab.SetRect( 6, 26, 210, 42 );

	// Create skill learn texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MessageBox.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	m_rtBackTop.SetUV( 0, 0, 216, 26, fTexWidth, fTexHeight );
	m_rtBackMiddle1.SetUV( 0, 31, 216, 33, fTexWidth, fTexHeight );
	m_rtBackMiddle2.SetUV( 0, 35, 216, 37, fTexWidth, fTexHeight );
	m_rtBackManagerTop.SetUV( 36, 69, 252, 106, fTexWidth, fTexHeight );		// 37
	m_rtBackManagerMiddle.SetUV( 36, 106, 252, 112, fTexWidth, fTexHeight );	// 6
	m_rtBackManagerBottom.SetUV( 36, 112, 252, 116, fTexWidth, fTexHeight );	// 4
	m_rtBackBottom.SetUV( 0, 38, 216, 45, fTexWidth, fTexHeight );

	// Input box
	m_rtInputBoxL.SetUV( 131, 46, 135, 59, fTexWidth, fTexHeight );
	m_rtInputBoxM.SetUV( 137, 46, 139, 59, fTexWidth, fTexHeight );
	m_rtInputBoxR.SetUV( 141, 46, 145, 59, fTexWidth, fTexHeight );

	m_rtTab.SetUV( 43, 69, 44, 86, fTexWidth, fTexHeight );

	// Close button
	m_btnClose.Create( this, CTString( "" ), 184, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 219, 0, 233, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 234, 0, 248, 14, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Learn button
	m_btnOK.Create( this, _S( 191, "확인" ), 78, START_BOTTOM_BUTTON_Y, 63, 21 );
	m_btnOK.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnOK.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnOK.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOK.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Cancel button
	m_btnCancel.Create( this, _S( 139, "취소" ), 145, START_BOTTOM_BUTTON_Y, 63, 21 );
	m_btnCancel.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Exit button
	m_btnExit.Create( this, _S( 870, "닫기" ), 145, START_BOTTOM_BUTTON_Y, 63, 21 );			
	m_btnExit.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnExit.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnExit.CopyUV( UBS_IDLE, UBS_ON );
	m_btnExit.CopyUV( UBS_IDLE, UBS_DISABLE );

	int nButtonY = START_BUTTON_Y;

	// Change Boss button
	m_btnChangeBoss.Create( this, _S( 871, "길드장 이임" ), 92, START_BUTTON_Y, 115, 21 );
	m_btnChangeBoss.SetUV( UBS_IDLE, 134, 117, 228, 138, fTexWidth, fTexHeight );
	m_btnChangeBoss.SetUV( UBS_CLICK, 134, 139, 228, 160, fTexWidth, fTexHeight );
	m_btnChangeBoss.CopyUV( UBS_IDLE, UBS_ON );
	m_btnChangeBoss.CopyUV( UBS_IDLE, UBS_DISABLE );

	nButtonY += 23;
	// Accept button
	m_btnAccept.Create( this, _S( 872, "길드부장 임명" ), 92, nButtonY, 115, 21 );		
	m_btnAccept.SetUV( UBS_IDLE, 134, 117, 228, 138, fTexWidth, fTexHeight );
	m_btnAccept.SetUV( UBS_CLICK, 134, 139, 228, 160, fTexWidth, fTexHeight );
	m_btnAccept.CopyUV( UBS_IDLE, UBS_ON );
	m_btnAccept.CopyUV( UBS_IDLE, UBS_DISABLE );

	nButtonY += 23;
	// Reject button
	m_btnReject.Create( this, _S( 873, "길드부장 해임" ), 92, nButtonY, 115, 21 );		
	m_btnReject.SetUV( UBS_IDLE, 134, 117, 228, 138, fTexWidth, fTexHeight );
	m_btnReject.SetUV( UBS_CLICK, 134, 139, 228, 160, fTexWidth, fTexHeight );
	m_btnReject.CopyUV( UBS_IDLE, UBS_ON );
	m_btnReject.CopyUV( UBS_IDLE, UBS_DISABLE );

	nButtonY += 23;
	// MemberQuit button
	m_btnMemberFire.Create( this, _S( 874, "길드원 퇴출" ), 92, nButtonY, 115, 21 );	
	m_btnMemberFire.SetUV( UBS_IDLE, 134, 117, 228, 138, fTexWidth, fTexHeight );
	m_btnMemberFire.SetUV( UBS_CLICK, 134, 139, 228, 160, fTexWidth, fTexHeight );
	m_btnMemberFire.CopyUV( UBS_IDLE, UBS_ON );
	m_btnMemberFire.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Guild Name edit box
	m_ebGuildName.Create( this, 100, EDIT_BOX_Y - 2, 90, 16, 16 );
	m_ebGuildName.SetReadingWindowUV( 146, 46, 163, 62, fTexWidth, fTexHeight );
	m_ebGuildName.SetCandidateUV( 146, 46, 163, 62, fTexWidth, fTexHeight );

	// List box of guild description
	m_lbGuildDesc.Create( this, 8, 28, 190, 190, _pUIFontTexMgr->GetLineHeight(), 13, 3, 1, FALSE );
	m_lbGuildDesc.CreateScroll( TRUE, 0, 0, 9, DESC_LIST_BOX_HEIGHT, 9, 7, 0, 0, 10 );
	m_lbGuildDesc.SetSelBar( 190, _pUIFontTexMgr->GetLineHeight(), 187, 46, 204, 61, fTexWidth, fTexHeight );
	// Up button
	m_lbGuildDesc.SetScrollUpUV( UBS_IDLE, 230, 16, 239, 23, fTexWidth, fTexHeight );
	m_lbGuildDesc.SetScrollUpUV( UBS_CLICK, 240, 16, 249, 23, fTexWidth, fTexHeight );
	m_lbGuildDesc.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbGuildDesc.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_lbGuildDesc.SetScrollDownUV( UBS_IDLE, 230, 24, 239, 31, fTexWidth, fTexHeight );
	m_lbGuildDesc.SetScrollDownUV( UBS_CLICK, 240, 24, 249, 31, fTexWidth, fTexHeight );
	m_lbGuildDesc.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbGuildDesc.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbGuildDesc.SetScrollBarTopUV( 219, 16, 228, 26, fTexWidth, fTexHeight );
	m_lbGuildDesc.SetScrollBarMiddleUV( 219, 27, 228, 29, fTexWidth, fTexHeight );
	m_lbGuildDesc.SetScrollBarBottomUV( 219, 30, 228, 40, fTexWidth, fTexHeight );

	// List box of guild description
	m_lbMemberList.Create( this, 8, 63, MEMBER_LIST_BOX_WIDTH, MEMBER_LIST_BOX_HEIGHT, _pUIFontTexMgr->GetLineHeight(), 13, 3, 2, TRUE );
	m_lbMemberList.CreateScroll( TRUE, 0, 0, 9, MEMBER_LIST_BOX_HEIGHT, 9, 7, 0, 0, 10 );
	m_lbMemberList.SetSelBar( MEMBER_LIST_BOX_WIDTH, _pUIFontTexMgr->GetLineHeight(), 187, 46, 204, 61, fTexWidth, fTexHeight );
	m_lbMemberList.SetOverColor( 0xF8E1B5FF );
	m_lbMemberList.SetSelectColor( 0xF8E1B5FF );
	m_lbMemberList.SetColumnPosX( 1, 110 );
	// Up button
	m_lbMemberList.SetScrollUpUV( UBS_IDLE, 230, 16, 239, 23, fTexWidth, fTexHeight );
	m_lbMemberList.SetScrollUpUV( UBS_CLICK, 240, 16, 249, 23, fTexWidth, fTexHeight );
	m_lbMemberList.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbMemberList.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_lbMemberList.SetScrollDownUV( UBS_IDLE, 230, 24, 239, 31, fTexWidth, fTexHeight );
	m_lbMemberList.SetScrollDownUV( UBS_CLICK, 240, 24, 249, 31, fTexWidth, fTexHeight );
	m_lbMemberList.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbMemberList.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbMemberList.SetScrollBarTopUV( 219, 16, 228, 26, fTexWidth, fTexHeight );
	m_lbMemberList.SetScrollBarMiddleUV( 219, 27, 228, 29, fTexWidth, fTexHeight );
	m_lbMemberList.SetScrollBarBottomUV( 219, 30, 228, 40, fTexWidth, fTexHeight );	

	// List box of guild description
	m_lbApplicantList.Create( this, 8, 63, MEMBER_LIST_BOX_WIDTH, MEMBER_LIST_BOX_HEIGHT * 2, _pUIFontTexMgr->GetLineHeight()+5, 13, 3, 2, FALSE );
	m_lbApplicantList.CreateScroll( TRUE, 0, 0, 9, MEMBER_LIST_BOX_HEIGHT * 2, 9, 7, 0, 0, 10 );
	m_lbApplicantList.SetSelBar( MEMBER_LIST_BOX_WIDTH, _pUIFontTexMgr->GetLineHeight()+5, 187, 46, 204, 61, fTexWidth, fTexHeight );
	m_lbApplicantList.SetOverColor( 0xF8E1B5FF );
	m_lbApplicantList.SetSelectColor( 0xF8E1B5FF );
	m_lbApplicantList.SetColumnPosX( 1, 115 );
	// Up button
	m_lbApplicantList.SetScrollUpUV( UBS_IDLE, 230, 16, 239, 23, fTexWidth, fTexHeight );
	m_lbApplicantList.SetScrollUpUV( UBS_CLICK, 240, 16, 249, 23, fTexWidth, fTexHeight );
	m_lbApplicantList.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbApplicantList.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_lbApplicantList.SetScrollDownUV( UBS_IDLE, 230, 24, 239, 31, fTexWidth, fTexHeight );
	m_lbApplicantList.SetScrollDownUV( UBS_CLICK, 240, 24, 249, 31, fTexWidth, fTexHeight );
	m_lbApplicantList.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbApplicantList.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbApplicantList.SetScrollBarTopUV( 219, 16, 228, 26, fTexWidth, fTexHeight );
	m_lbApplicantList.SetScrollBarMiddleUV( 219, 27, 228, 29, fTexWidth, fTexHeight );
	m_lbApplicantList.SetScrollBarBottomUV( 219, 30, 228, 40, fTexWidth, fTexHeight );	

// WSS_NEW_GUILD_SYSTEM 070704	
#ifdef NEW_GUILD_SYSTEM
	for(int i= 7;i<MAX_GUILD_LEVEL;i++)
	{
		if(i+1>=5 && i+1<=10) 
			_GuildConditionTable[i].iCaptionNeedLevel	=	30;	
		else if(i+1>=11 && i+1<=19) 
			_GuildConditionTable[i].iCaptionNeedLevel	=	40;	
		else if(i+1>=20) 
			_GuildConditionTable[i].iCaptionNeedLevel	=	30 + 10*((i+1)/10);	
		
		_GuildConditionTable[i].iLevel				=	i+1;		
		// 필요나스/sp/gp = 7레벨업시 필요량 * (1.09^(길드LV - 7))
		_GuildConditionTable[i].iMaxMember			=	_GuildConditionTable[6].iMaxMember; 
		_GuildConditionTable[i].iNeedGP				=	_GuildConditionTable[6].iNeedGP	* pow(1.09,i-6);
		_GuildConditionTable[i].iNeedSP				=	_GuildConditionTable[6].iNeedSP	* pow(1.09,i-6);
		_GuildConditionTable[i].llNeedMoney			=	_GuildConditionTable[6].llNeedMoney	* pow(1.09,i-6);
	}
	CreateNew(pParentWnd,nX,nY);

#endif
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIGuild::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIGuild::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// WSS_NEW_GUILD_SYSTEM 070702
// 불필요해서 주석처리
//------------------------------------------------------------------------------
// CUIGuild::SetFocus
// Explain:  
// Date : 2005-03-10(오후 1:02:39) Lee Ki-hwan
//------------------------------------------------------------------------------
//void CUIGuild::SetFocus( BOOL bVisible )
//{
//	CUIWindow::SetFocus( bVisible );
//
//	m_ebGuildName.SetFocus( TRUE );
//	if( !bVisible )
//	{
//		m_ebGuildName.SetFocus( FALSE );
//	}
//}


// ----------------------------------------------------------------------------
// Name : OpenGuild()
// Desc : 
// ----------------------------------------------------------------------------
void CUIGuild::OpenGuild( int iMobIndex, BOOL bHasQuest, int iUserRanking, int iGuildLevel )
{
	if(_pUIMgr->DoesMessageBoxLExist( MSGLCMD_GUILD_REQ ) || IsVisible())
		return;	

	ResetGuild();

	m_iUserRanking = iUserRanking;
	m_iGuildLevel	= iGuildLevel;
	
	_pUIMgr->CloseMessageBox(MSGCMD_GUILD_ERROR);
	_pUIMgr->CloseMessageBox(MSGCMD_GUILD_DESTROY);
	_pUIMgr->CloseMessageBox(MSGCMD_GUILD_JOIN);
	_pUIMgr->CloseMessageBox(MSGCMD_GUILD_JOIN_REQ);
	_pUIMgr->CloseMessageBox(MSGCMD_GUILD_QUIT);
	_pUIMgr->CloseMessageBox(MSGCMD_GUILD_QUIT_CONFIRM);
	_pUIMgr->CloseMessageBox(MSGCMD_GUILD_APPLICANT_JOIN);	
	_pUIMgr->CloseMessageBox(MSGCMD_GUILD_ROOM);

	// Create guild message box
	
	// Reset Window Size // WSS_NEW_GUILD_SYSTEM 070725
	SetSize( GUILD_WIDTH, GUILD_HEIGHT );

	_pUIMgr->CreateMessageBoxL( _S( 865, "길드" ) , UI_GUILD, MSGLCMD_GUILD_REQ );					

	CTString	strNpcName = _pNetwork->GetMobName(iMobIndex);
	_pUIMgr->AddMessageBoxLString( MSGLCMD_GUILD_REQ, TRUE, strNpcName, -1, 0xE18600FF );

	_pUIMgr->AddMessageBoxLString( MSGLCMD_GUILD_REQ, TRUE, _S( 875, "사악한 무리를 감시하느라 한시도 시간을 낼 수 없네." ), -1, 0xA3A1A3FF );	
	_pUIMgr->AddMessageBoxLString( MSGLCMD_GUILD_REQ, TRUE, _S( 876, "용건이 없다면 그냥 돌아 가시게." ), -1, 0xA3A1A3FF );		

	if( m_iUserRanking == GUILD_MEMBER_MEMBER || m_iUserRanking == GUILD_MEMBER_VICE_BOSS || m_iUserRanking == GUILD_MEMBER_NOMEMBER )
	{
		_pUIMgr->AddMessageBoxLString( MSGLCMD_GUILD_REQ, FALSE,  _S( 877, "길드 결성." ), SEL_GUILD_CREATE );	
	}

	if( m_iUserRanking == GUILD_MEMBER_BOSS )
	{
		_pUIMgr->AddMessageBoxLString( MSGLCMD_GUILD_REQ, FALSE, _S( 878, "길드 승급." ), SEL_GUILD_UPGRADE );
		_pUIMgr->AddMessageBoxLString( MSGLCMD_GUILD_REQ, FALSE, _S( 879, "길드 해산." ), SEL_GUILD_DESTROY );
	}

	if( m_iUserRanking != GUILD_MEMBER_NOMEMBER )
	{
		_pUIMgr->AddMessageBoxLString( MSGLCMD_GUILD_REQ, FALSE, _S( 1011, "길드 대화방으로 이동." ), SEL_GUILD_ROOM );
	}

	if( bHasQuest )
	{		
#ifdef	NEW_QUESTBOOK
		// 2009. 05. 27 김정래
		// 이야기한다 변경 처리
		CUIQuestBook::AddQuestListToMessageBoxL(MSGLCMD_GUILD_REQ);				
#else
		_pUIMgr->AddMessageBoxLString( MSGLCMD_GUILD_REQ, FALSE, _S( 1053, "이야기한다." ), SEL_GUILD_TALK );		
#endif
	}

	_pUIMgr->AddMessageBoxLString( MSGLCMD_GUILD_REQ, FALSE, _S( 1220, "취소하기." ) );				
}

// ----------------------------------------------------------------------------
// Name : OpenGuildManager()
// Desc : 
// ----------------------------------------------------------------------------
void CUIGuild::OpenGuildManager( int iUserRanking )
{
	if( iUserRanking < 0 || iUserRanking == GUILD_MEMBER_NOMEMBER )
	{
		return;
	}

	if( IsVisible() )
	{
		ResetGuild();
		return;
	}

	// Reset Window Size // WSS_NEW_GUILD_SYSTEM 070725
	SetSize( GUILD_WIDTH, GUILD_HEIGHT );

	// FIXME : 길드 메니저에서는 호출하면 안됨.
	// FIXME : 애들 목록을 받은 것을 다 갖고 있어야 하는데, ResetGuild()를 호출하면 다 날라감.
	//ResetGuild();

	m_iUserRanking	= iUserRanking;
	m_eGuildState	= GUILD_MANAGER;

	m_btnChangeBoss.SetEnable( (m_iUserRanking == GUILD_MEMBER_BOSS) ? TRUE : FALSE );
	m_btnAccept.SetEnable( (m_iUserRanking == GUILD_MEMBER_BOSS) ? TRUE : FALSE );
	m_btnReject.SetEnable( (m_iUserRanking == GUILD_MEMBER_BOSS) ? TRUE : FALSE );
	m_btnMemberFire.SetEnable( (m_iUserRanking < GUILD_MEMBER_MEMBER) ? TRUE : FALSE );	

	if( m_iUserRanking == GUILD_MEMBER_BOSS || m_iUserRanking == GUILD_MEMBER_VICE_BOSS )
	{
		RefreshMemberList( TRUE );
	}
	else
	{
		RefreshApplicantList( TRUE );
	}

	_pUIMgr->RearrangeOrder( UI_GUILD, TRUE );

}

// ----------------------------------------------------------------------------
// Name : ResetGuild()
// Desc : 
// ----------------------------------------------------------------------------
void CUIGuild::ResetGuild()
{
	m_ebGuildName.ResetString();
	m_lbGuildDesc.ResetAllStrings();
	m_lbMemberList.ResetAllStrings();
	m_lbApplicantList.ResetAllStrings();

	//m_strBossName			= "";
	m_strViceBoss[0]		= "";
	m_strViceBoss[1]		= "";
	m_strGuildName			= "";

	m_iGuildLevel			= -1;
	m_iGuildIndex			= -1;
	m_iNumOfMember			= 0;

	m_eGuildState			= GUILD_REQ;
	m_nCurrentTab			= GUILD_TAB_MEMBER;
	m_iUserRanking			= GUILD_MEMBER_MEMBER;

	// NOTE : 길드를 탈퇴하거나 해체 하지 않는 이상, 클리어 하면 안될거 같음.
	//ClearMemberList();

	_pUIMgr->RearrangeOrder( UI_GUILD, FALSE );
}

// ----------------------------------------------------------------------------
// Name : ClearMemberList()
// Desc : 멤버 목록을 클리어합니다.
// ----------------------------------------------------------------------------
void CUIGuild::ClearMemberList()
{
	// NOTE : 길드를 탈퇴하거나 해체 하지 않는 이상, 클리어 하면 안될거 같음.
	if(!m_vectorMemberList.empty())
	{
		m_strBossName			= "";
		m_vectorMemberList.clear();
	}
}

// ----------------------------------------------------------------------------
// Name : PressOKBtn()
// Desc : 확인 버튼을 눌렀을 때의 처리.
// ----------------------------------------------------------------------------
void CUIGuild::PressOKBtn()
{
	switch( m_eGuildState )
	{
	case GUILD_CREATE:		// 길드 생성
		CreateGuild();
		break;
	case GUILD_UPGRADE:		// 길드 승급
		UpgradeGuild();
		break;
	case GUILD_DESTROY:		// 길드 해체
		{
			_pUIMgr->CloseMessageBox(MSGCMD_GUILD_ERROR);
			_pUIMgr->CloseMessageBox(MSGCMD_GUILD_DESTROY);
			_pUIMgr->CloseMessageBox(MSGCMD_GUILD_JOIN);
			_pUIMgr->CloseMessageBox(MSGCMD_GUILD_JOIN_REQ);
			_pUIMgr->CloseMessageBox(MSGCMD_GUILD_QUIT);
			_pUIMgr->CloseMessageBox(MSGCMD_GUILD_QUIT_CONFIRM);
			_pUIMgr->CloseMessageBox(MSGCMD_GUILD_APPLICANT_JOIN);	
			_pUIMgr->CloseMessageBox(MSGCMD_GUILD_ROOM);
			
			// Create message box of guild destroy
			CTString	strMessage;
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 865, "길드" ), UMBS_OKCANCEL, UI_GUILD, MSGCMD_GUILD_DESTROY );	
			strMessage.PrintF( _S( 881, "정말로 [%s] 길드를 해산 하시겠습니까?" ), _pNetwork->MyCharacterInfo.strGuildName );		
			MsgBoxInfo.AddString( strMessage );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : CreateGuild()
// Desc : 길드 생성
// ----------------------------------------------------------------------------
void CUIGuild::CreateGuild()
{
	// 길드 생성.
	if( m_ebGuildName.GetString() )
	{
		m_strGuildName = m_ebGuildName.GetString();

		if(!_pUIMgr->checkName(m_strGuildName,1))
		{
			_pUIMgr->CloseMessageBox(MSGCMD_GUILD_ERROR);
			_pUIMgr->CloseMessageBox(MSGCMD_GUILD_DESTROY);
			_pUIMgr->CloseMessageBox(MSGCMD_GUILD_JOIN);
			_pUIMgr->CloseMessageBox(MSGCMD_GUILD_JOIN_REQ);
			_pUIMgr->CloseMessageBox(MSGCMD_GUILD_QUIT);
			_pUIMgr->CloseMessageBox(MSGCMD_GUILD_QUIT_CONFIRM);
			_pUIMgr->CloseMessageBox(MSGCMD_GUILD_APPLICANT_JOIN);
			_pUIMgr->CloseMessageBox(MSGCMD_GUILD_ROOM);
			
			m_ebGuildName.ResetString();
			m_ebGuildName.SetFocus( TRUE );
			return;
		}
				
		// 길드 생성 메시지 보내기.
		_pNetwork->GuildCreate( m_strGuildName );
	}
}

// ----------------------------------------------------------------------------
// Name : GoGuildZone()
// Desc : 길드 전용 공간으로 이동...
// ----------------------------------------------------------------------------
void CUIGuild::GoGuildZone()
{
	const int iGuildZoneID = 8;
	if( _pNetwork->MyCharacterInfo.lGuildIndex != GUILD_MEMBER_NOMEMBER )
	{
		_pNetwork->GoZone( iGuildZoneID, 0 , GUILD_NPC );
	}
}

// ----------------------------------------------------------------------------
// Name : UpgradeGuild()
// Desc : 길드 승급
// ----------------------------------------------------------------------------
void CUIGuild::UpgradeGuild()
{
	_pNetwork->GuildUpgrade();
}

// ----------------------------------------------------------------------------
// Name : DestroyGuild()
// Desc : 길드 해체
// ----------------------------------------------------------------------------
void CUIGuild::DestroyGuild()
{
	_pNetwork->GuildDestroy();
}

static LONG g_lChaIndex		= -1;		// 가입 신청과 관련된 부분에만 쓰임.
static LONG g_lGuildIndex	= -1;
static LONG g_lMemberIndex	= -1;		// 멤버에 관련된 부분에만 쓰임.

// ----------------------------------------------------------------------------
// Name : JoinGuild()
// Desc : 길드 가입
// ----------------------------------------------------------------------------
void CUIGuild::JoinGuild( LONG lGuildIndex, LONG lChaIndex, const CTString& strName )
{
	_pUIMgr->CloseMessageBox(MSGCMD_GUILD_ERROR);
	_pUIMgr->CloseMessageBox(MSGCMD_GUILD_DESTROY);
	_pUIMgr->CloseMessageBox(MSGCMD_GUILD_JOIN);
	_pUIMgr->CloseMessageBox(MSGCMD_GUILD_JOIN_REQ);
	_pUIMgr->CloseMessageBox(MSGCMD_GUILD_QUIT);
	_pUIMgr->CloseMessageBox(MSGCMD_GUILD_QUIT_CONFIRM);
	_pUIMgr->CloseMessageBox(MSGCMD_GUILD_APPLICANT_JOIN);	
	_pUIMgr->CloseMessageBox(MSGCMD_GUILD_ROOM);
	
	g_lChaIndex		= lChaIndex;
	g_lGuildIndex	= lGuildIndex;
	
	// Create message box of guild destroy
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 865, "길드" ), UMBS_OKCANCEL, UI_GUILD, MSGCMD_GUILD_JOIN );	
	strMessage.PrintF( _S( 884, "[%s] 길드에 가입 신청을 하시겠습니까?" ), strName );		
	MsgBoxInfo.AddString( strMessage );
	_pUIMgr->CreateMessageBox( MsgBoxInfo );
}

// ----------------------------------------------------------------------------
// Name : QuitGuild()
// Desc : 길드 탈퇴
// ----------------------------------------------------------------------------
void CUIGuild::QuitGuild( )
{
	_pUIMgr->CloseMessageBox(MSGCMD_GUILD_ERROR);
	_pUIMgr->CloseMessageBox(MSGCMD_GUILD_DESTROY);
	_pUIMgr->CloseMessageBox(MSGCMD_GUILD_JOIN);
	_pUIMgr->CloseMessageBox(MSGCMD_GUILD_JOIN_REQ);
	_pUIMgr->CloseMessageBox(MSGCMD_GUILD_QUIT);
	_pUIMgr->CloseMessageBox(MSGCMD_GUILD_QUIT_CONFIRM);
	_pUIMgr->CloseMessageBox(MSGCMD_GUILD_APPLICANT_JOIN);	
	_pUIMgr->CloseMessageBox(MSGCMD_GUILD_ROOM);
	
	// Create message box of guild destroy
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 865, "길드" ), UMBS_OKCANCEL, UI_GUILD, MSGCMD_GUILD_QUIT );	
	strMessage.PrintF( _S( 885, "[%s] 길드를 탈퇴 하시겠습니까?" ), _pNetwork->MyCharacterInfo.strGuildName );		
	MsgBoxInfo.AddString( strMessage );
	_pUIMgr->CreateMessageBox( MsgBoxInfo );
}

// ----------------------------------------------------------------------------
// Name : ChangeBoss()
// Desc : 단장 이임
// ----------------------------------------------------------------------------
void CUIGuild::ChangeBoss()
{
	// WSS_NEW_GUILD_SYSTEM 070719
	int	iSelMember;
#ifdef NEW_GUILD_SYSTEM
	if(m_iGuildLevel < LIMIT_GUILD_LEVEL)
#endif
		iSelMember = m_lbMemberList.GetCurSel();
#ifdef NEW_GUILD_SYSTEM
	else 
		iSelMember = m_lbManageMemberList.GetCurSel();
#endif

	if( iSelMember != -1 )
	{
		sGuildMember &TempMember = m_vectorMemberList[iSelMember];
		if( _pNetwork->MyCharacterInfo.index == TempMember.lIndex )
			return;

		g_lMemberIndex = TempMember.lIndex;

		_pUIMgr->CloseMessageBox( MSGCMD_GUILD_KICK );
		_pUIMgr->CloseMessageBox( MSGCMD_GUILD_CHANGE_BOSS );
		_pUIMgr->CloseMessageBox( MSGCMD_GUILD_ADD_VICE_BOSS );
		_pUIMgr->CloseMessageBox( MSGCMD_GUILD_DEL_VICE_BOSS );

		// Create message box of guild destroy
		CTString	strMessage;
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 865, "길드" ), UMBS_OKCANCEL, UI_GUILD, MSGCMD_GUILD_CHANGE_BOSS );	
		strMessage.PrintF( _S( 886, "[%s] 에게 길드장을 이임 하시겠습니까?" ), TempMember.strMemberName );		
		MsgBoxInfo.AddString( strMessage );
		_pUIMgr->CreateMessageBox( MsgBoxInfo );
	}
}

// ----------------------------------------------------------------------------
// Name : AddViceBoss()
// Desc : 부단장 임명
// ----------------------------------------------------------------------------
void CUIGuild::AddViceBoss()
{
	// WSS_NEW_GUILD_SYSTEM 070719
	int	iSelMember;
#ifdef NEW_GUILD_SYSTEM
	if(m_iGuildLevel < LIMIT_GUILD_LEVEL)
#endif
		iSelMember = m_lbMemberList.GetCurSel();
#ifdef NEW_GUILD_SYSTEM
	else 
		iSelMember = m_lbManageMemberList.GetCurSel();
#endif

	if( iSelMember != -1 )
	{
		sGuildMember &TempMember = m_vectorMemberList[iSelMember];
		g_lMemberIndex = TempMember.lIndex;

		if( TempMember.eRanking == GUILD_MEMBER_VICE_BOSS )
		{
			return;
		}

		_pUIMgr->CloseMessageBox( MSGCMD_GUILD_KICK );
		_pUIMgr->CloseMessageBox( MSGCMD_GUILD_CHANGE_BOSS );
		_pUIMgr->CloseMessageBox( MSGCMD_GUILD_ADD_VICE_BOSS );
		_pUIMgr->CloseMessageBox( MSGCMD_GUILD_DEL_VICE_BOSS );

		// Create message box of guild destroy
		CTString	strMessage;
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 865, "길드" ), UMBS_OKCANCEL, UI_GUILD, MSGCMD_GUILD_ADD_VICE_BOSS );	
		strMessage.PrintF( _S( 887, "[%s]를 길드부장으로 임명 하시겠습니까?" ), TempMember.strMemberName );		
		MsgBoxInfo.AddString( strMessage );
		_pUIMgr->CreateMessageBox( MsgBoxInfo );		
	}
}

// ----------------------------------------------------------------------------
// Name : DelViceBoss()
// Desc : 부단장 해임
// ----------------------------------------------------------------------------
void CUIGuild::DelViceBoss()
{
	// WSS_NEW_GUILD_SYSTEM 070719
	int	iSelMember;
	#ifdef NEW_GUILD_SYSTEM
		if(m_iGuildLevel < LIMIT_GUILD_LEVEL)
	#endif
			iSelMember = m_lbMemberList.GetCurSel();
	#ifdef NEW_GUILD_SYSTEM
		else 
			iSelMember = m_lbManageMemberList.GetCurSel();
	#endif	
	 
	if( iSelMember != -1 )
	{
		sGuildMember &TempMember = m_vectorMemberList[iSelMember];

		// 부단장만 가능함.
		if(TempMember.eRanking != GUILD_MEMBER_VICE_BOSS)
			return;

		g_lMemberIndex = TempMember.lIndex;

		_pUIMgr->CloseMessageBox( MSGCMD_GUILD_KICK );
		_pUIMgr->CloseMessageBox( MSGCMD_GUILD_CHANGE_BOSS );
		_pUIMgr->CloseMessageBox( MSGCMD_GUILD_ADD_VICE_BOSS );
		_pUIMgr->CloseMessageBox( MSGCMD_GUILD_DEL_VICE_BOSS );

		// Create message box of guild destroy
		CTString	strMessage;
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 865, "길드" ), UMBS_OKCANCEL, UI_GUILD, MSGCMD_GUILD_DEL_VICE_BOSS );	
		strMessage.PrintF( _S( 888, "[%s]를 길드부장에서 해임 하시겠습니까?" ), TempMember.strMemberName );		
		MsgBoxInfo.AddString( strMessage );
		_pUIMgr->CreateMessageBox( MsgBoxInfo );
	}
}

// ----------------------------------------------------------------------------
// Name : MemberFire()
// Desc : 단원 퇴출
// ----------------------------------------------------------------------------
void CUIGuild::MemberFire()
{
	// WSS_NEW_GUILD_SYSTEM 070719
	int	iSelMember;
	#ifdef NEW_GUILD_SYSTEM
		if(m_iGuildLevel < LIMIT_GUILD_LEVEL)
	#endif
			iSelMember = m_lbMemberList.GetCurSel();
	#ifdef NEW_GUILD_SYSTEM
		else 
			iSelMember = m_lbManageMemberList.GetCurSel();
	#endif
		
	if( iSelMember != -1 )
	{
		sGuildMember &TempMember = m_vectorMemberList[iSelMember];

		// 멤버의 레벨이 자기 레벨보다 높으면 퇴출 못시킴.
		if( TempMember.eRanking < _pNetwork->MyCharacterInfo.lGuildPosition )
			return;

		g_lMemberIndex = TempMember.lIndex;

		_pUIMgr->CloseMessageBox( MSGCMD_GUILD_KICK );
		_pUIMgr->CloseMessageBox( MSGCMD_GUILD_CHANGE_BOSS );
		_pUIMgr->CloseMessageBox( MSGCMD_GUILD_ADD_VICE_BOSS );
		_pUIMgr->CloseMessageBox( MSGCMD_GUILD_DEL_VICE_BOSS );
		
		// Create message box of guild destroy
		CTString	strMessage;
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 865, "길드" ), UMBS_OKCANCEL, UI_GUILD, MSGCMD_GUILD_KICK );	
		strMessage.PrintF( _S( 889, "[%s]를 퇴출 하시겠습니까?" ), TempMember.strMemberName );		
		MsgBoxInfo.AddString( strMessage );
		_pUIMgr->CreateMessageBox( MsgBoxInfo );		
	}
}

// ----------------------------------------------------------------------------
// Name : ApplicantAccept()
// Desc : 가입 승인
// ----------------------------------------------------------------------------
void CUIGuild::ApplicantAccept( LONG lIndex )
{
	CTString strMemberName = "";

	g_lChaIndex = lIndex;

	// 다른 캐릭터들의 길드 레벨을 변경시킴.
	for( INDEX ipl = 0; ipl < _pNetwork->ga_srvServer.srv_actCha.Count(); ++ipl )
	{
		CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[ipl];
		if( ct.cha_Index == g_lChaIndex )
		{
			strMemberName = ct.cha_strName;
			break;
		}
	}

	// Create message box of guild destroy
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 865, "길드" ), UMBS_OKCANCEL, UI_GUILD, MSGCMD_GUILD_APPLICANT_JOIN );	
	strMessage.PrintF( _S( 890, "[%s] 님께서 가입 신청을 하셨습니다. 수락하시겠습니까?" ),  strMemberName );		
	MsgBoxInfo.AddString( strMessage );
	_pUIMgr->CreateMessageBox( MsgBoxInfo );
}

// ----------------------------------------------------------------------------
// Name : AddToMemberList()
// Desc : 멤버 목록에 추가
// ----------------------------------------------------------------------------
void CUIGuild::AddToMemberList( LONG lIndex, const CTString& strName, int eLevel, BOOL bOnline )
{
	// 단원이 목록을 보려고 하는 경우...
	/*
	if( m_iUserRanking != GUILD_MEMBER_MEMBER && 
		eLevel == GUILD_MEMBER_BOSS )		// 단장
	{
		m_strBossName = strName;
	}
	else
	*/
	{
		sGuildMember TempGuildMember;
		TempGuildMember.lIndex			= lIndex;
		TempGuildMember.strMemberName	= strName;
		TempGuildMember.eRanking		= eLevel;
		TempGuildMember.bOnline			= bOnline;
		m_vectorMemberList.push_back(TempGuildMember);
	}
}

// ----------------------------------------------------------------------------
// Name : DelFromMemberList()
// Desc : 멤버 목록에서 제거
// ----------------------------------------------------------------------------
void CUIGuild::DelFromMemberList( LONG lIndex )
{
	sGuildMember TempGuildMember;
	TempGuildMember.lIndex = lIndex;
	std::vector<sGuildMember>::iterator iter = 
		std::find_if(m_vectorMemberList.begin(), m_vectorMemberList.end(), FindMember( TempGuildMember ) );

	if( iter != m_vectorMemberList.end() )
	{
		m_vectorMemberList.erase( iter );
	}
}

// ----------------------------------------------------------------------------
// Name : ChangeMemberLevel()
// Desc : 멤버의 레벨을 변경함.
// ----------------------------------------------------------------------------
void CUIGuild::ChangeMemberLevel( LONG lIndex, int eLevel )
{
	sGuildMember TempGuildMember;
	TempGuildMember.lIndex = lIndex;
	std::vector<sGuildMember>::iterator iter = 
		std::find_if(m_vectorMemberList.begin(), m_vectorMemberList.end(), FindMember( TempGuildMember ) );

	if( iter != m_vectorMemberList.end() )
	{
		(*iter).eRanking = eLevel;
	}
}

// ----------------------------------------------------------------------------
// Name : ChangeMemberOnline()
// Desc : 멤버의 온라인 상태를 변경함.
// ----------------------------------------------------------------------------
void CUIGuild::ChangeMemberOnline( LONG lIndex, BOOL bOnline )
{
	sGuildMember TempGuildMember;
	TempGuildMember.lIndex = lIndex;
	std::vector<sGuildMember>::iterator iter = 
		std::find_if(m_vectorMemberList.begin(), m_vectorMemberList.end(), FindMember( TempGuildMember ) );

	if( iter != m_vectorMemberList.end() )
	{
		(*iter).bOnline = bOnline;
	}
}

// ----------------------------------------------------------------------------
// Name : RefreshMemberList()
// Desc : 목록을 갱신합니다.
// ----------------------------------------------------------------------------
void CUIGuild::RefreshMemberList( BOOL bInit )
{
	if( !bInit && !IsVisible() )
		return;

	m_lbMemberList.ResetAllStrings();
	std::sort(m_vectorMemberList.begin(), m_vectorMemberList.end());

	CTString	strLevel;
	COLOR		crLevel;

	int			iBossCount		= 0;
	int			iViceBossCount	= 0;
	m_strViceBoss[0]			= "";
	m_strViceBoss[1]			= "";
	std::vector<sGuildMember>::const_iterator member_end = m_vectorMemberList.end();
	std::vector<sGuildMember>::iterator it = m_vectorMemberList.begin();
	for(it = m_vectorMemberList.begin(); it != member_end; ++it)
	{
		switch( (*it).eRanking )
		{
		case GUILD_MEMBER_BOSS:			// 길드장
			{
				strLevel	= _S( 891, "길드장" );	
				crLevel		= 0xFF9533FF;
				ASSERT(iBossCount <= 1 && "Invalid Boss Count");
				m_strBossName = (*it).strMemberName;
				iBossCount++;

				// 단장은 이름이 표시되면 안되므로...
				//continue;
			}
			break;
		case GUILD_MEMBER_VICE_BOSS:	// 길드부장
			{
				strLevel	= _S( 892, "길드부장" );	
				crLevel		= 0xFFC672FF;
				ASSERT(iViceBossCount <= 2 && "Invalid Vice-Boss Count");
				m_strViceBoss[iViceBossCount] = (*it).strMemberName;
				iViceBossCount++;
			}
			break;
		case GUILD_MEMBER_MEMBER:		// 길드원
			{
				strLevel	= _S( 893, "길드원" );	
				crLevel		= 0xCCCCCCFF;
			}
			break;
			/*
		case GUILD_MEMBER_APPLICANT:	// 지원자
			{
				ASSERTALWAYS("호출되어서는 안되는 부분\n");
				strLevel	= _S( 894, "지원자" );	
				crLevel		= 0x9400D6FF;
			}
			break;
			*/
		}

		BOOL bOnline = (*it).bOnline;
		m_lbMemberList.AddString( 0, (*it).strMemberName, bOnline ? 0xFFFFFFFF : 0xAAAAAAFF );
		m_lbMemberList.AddString( 1, strLevel, crLevel);
	}
}

// ----------------------------------------------------------------------------
// Name : RefreshApplicantList()
// Desc : 목록을 갱신합니다.
// ----------------------------------------------------------------------------
void CUIGuild::RefreshApplicantList( BOOL bInit )
{
	if( !bInit && !IsVisible() )
		return;

	m_lbApplicantList.ResetAllStrings();
	std::sort(m_vectorMemberList.begin(), m_vectorMemberList.end());

	CTString	strLevel;
	COLOR		crLevel;

	m_lbApplicantList.AddString( 0, _S( 895, "온라인" ), 0xBCBCBCFF );		
	m_lbApplicantList.AddString( 1, CTString(""), 0xFFFFFFFF);

	int			iBossCount		= 0;
	int			iViceBossCount	= 0;

	// On-Line
	std::vector<sGuildMember>::const_iterator member_end = m_vectorMemberList.end();
	std::vector<sGuildMember>::iterator it = m_vectorMemberList.begin();
	for(it = m_vectorMemberList.begin(); it != member_end; ++it)
	{
		if( (*it).bOnline )
		{
			switch( (*it).eRanking )
			{
			case GUILD_MEMBER_BOSS:			// 길드장
				{
					strLevel	= _S( 891, "길드장" );	
					crLevel		= 0xFF9533FF;
					ASSERT(iBossCount <= 1 && "Invalid Boss Count");
					m_strBossName = (*it).strMemberName;
					iBossCount++;
				}
				break;
			case GUILD_MEMBER_VICE_BOSS:	// 길드부장
				{
					strLevel	= _S( 892, "길드부장" );	
					crLevel		= 0xFFC672FF;
					ASSERT(iViceBossCount <= 2 && "Invalid Vice-Boss Count");
					m_strViceBoss[iViceBossCount] = (*it).strMemberName;
					iViceBossCount++;
				}
				break;
			case GUILD_MEMBER_MEMBER:		// 길드원
				{
					strLevel	= _S( 893, "길드원" );	
					crLevel		= 0xCCCCCCFF;
				}
				break;
				/*
			case GUILD_MEMBER_APPLICANT:	// 지원자
				{
					ASSERTALWAYS("호출되어서는 안되는 부분\n");
					strLevel	= _S( 894, "지원자" );	
					crLevel		= 0x9400D6FF;
				}
				break;
				*/
			}
			m_lbApplicantList.AddString( 0, (*it).strMemberName, 0xFFFFFFFF );
			m_lbApplicantList.AddString( 1, strLevel, crLevel);
		}
	}

	m_lbApplicantList.AddString( 0, CTString("  "), 0xFFFFFFFF );		
	m_lbApplicantList.AddString( 1, CTString(""), 0xFFFFFFFF);

	m_lbApplicantList.AddString( 0, _S( 896, "오프라인" ), 0xCCCCCCFF );		
	m_lbApplicantList.AddString( 1, CTString(""), 0xFFFFFFFF);

	// Off-Line
	for(it = m_vectorMemberList.begin(); it != member_end; ++it)
	{
		if( !(*it).bOnline )
		{
			switch( (*it).eRanking )
			{
			case GUILD_MEMBER_BOSS:			// 길드장
				{
					strLevel	= _S( 891, "길드장" );	
					crLevel		= 0xFF9533FF;
					ASSERT(iBossCount <= 1 && "Invalid Boss Count");
					m_strBossName = (*it).strMemberName;
					iBossCount++;
				}
				break;
			case GUILD_MEMBER_VICE_BOSS:	// 길드부장
				{
					strLevel	= _S( 892, "길드부장" );	
					crLevel		= 0xFFC672FF;
					ASSERT(iViceBossCount <= 2 && "Invalid Vice-Boss Count");
					m_strViceBoss[iViceBossCount] = (*it).strMemberName;
					iViceBossCount++;
				}
				break;
			case GUILD_MEMBER_MEMBER:		// 길드원
				{
					strLevel	= _S( 893, "길드원" );	
					crLevel		= 0xCCCCCCFF;
				}
				break;
				/*
			case GUILD_MEMBER_APPLICANT:	// 지원자
				{
					ASSERTALWAYS("호출되어서는 안되는 부분\n");
					strLevel	= _S( 894, "지원자" );	
					crLevel		= 0x9400D6FF;
				}
				break;
				*/
			}
			m_lbApplicantList.AddString( 0, (*it).strMemberName, 0xAAAAAAFF );
			m_lbApplicantList.AddString( 1, strLevel, crLevel);
		}
	}
}

// ----------------------------------------------------------------------------
// Name : GetGuildDesc()
// Desc :
// ----------------------------------------------------------------------------
void CUIGuild::GetGuildDesc( BOOL bShow )
{
	m_lbGuildDesc.ResetAllStrings();

	if( bShow == FALSE )
		return;

	BOOL bSatisfied = TRUE;

	// Make description of guild
	CTString	strTemp	= "";

	switch( m_eGuildState )
	{
	case GUILD_CREATE:		// 길드 생성
		{
			AddGuildDescString( _S( 897, "길드를 결성 하려는가?" ) );	
			AddGuildDescString( _S( 898, "용기 있는 자로군." ) );			
			AddGuildDescString( _S( 899, "하지만, 그에 상응하는 비용과 일정 수준의 희생은 감수 해야 하네." ) );		
			AddGuildDescString( _S( 900, "그래도 좋다면 승낙하지." ) );		
			AddGuildDescString( _S( 901, "이 세상은 혼돈을 끝낼 인재가 필요하니까" ) );		
			AddGuildDescString( CTString( " " ) );

			ASSERT( m_iGuildLevel <= MAX_GUILD_LEVEL && "Invalid Guild Level" );

			const int	iMyLevel	= _pNetwork->MyCharacterInfo.level;
			const SQUAD llMyMoney	= _pNetwork->MyCharacterInfo.money;
			const SLONG	lMySP		= _pNetwork->MyCharacterInfo.sp / 10000;			
			const int	iNeedLevel	= _GuildConditionTable[m_iGuildLevel + 1].iCaptionNeedLevel;
			const SQUAD llNeedMoney = _GuildConditionTable[m_iGuildLevel + 1].llNeedMoney;
			const int	iNeedSP		= _GuildConditionTable[m_iGuildLevel + 1].iNeedSP;

			strTemp.PrintF( _S( 902, "최대 길드 인원 : %d 명" ), _GuildConditionTable[m_iGuildLevel + 1].iMaxMember);		
			AddGuildDescString( strTemp, 0xFFFFFFFF );

			if( iMyLevel < iNeedLevel )
			{
				bSatisfied = FALSE;
			}

			strTemp.PrintF( _S( 903, "필요 레벨 : %d Lv" ), _GuildConditionTable[m_iGuildLevel + 1].iCaptionNeedLevel);		
			AddGuildDescString( strTemp, iMyLevel >= iNeedLevel ? 0xFFC672FF : 0xBCBCBCFF );

			if( llMyMoney < llNeedMoney )
			{
				bSatisfied = FALSE;
			}

			strTemp.PrintF( _S( 904, "필요 나스 : %I64d 나스" ), _GuildConditionTable[m_iGuildLevel + 1].llNeedMoney);		
			AddGuildDescString( strTemp, llMyMoney >= llNeedMoney ? 0xFFC672FF : 0xBCBCBCFF );

			if( lMySP < iNeedSP )
			{
				bSatisfied = FALSE;
			}

			strTemp.PrintF( _S( 905, "필요 S P  : %d SP" ), _GuildConditionTable[m_iGuildLevel + 1].iNeedSP);		
			AddGuildDescString( strTemp, lMySP >= iNeedSP ? 0xFFC672FF : 0xBCBCBCFF );
		}
		break;
	case GUILD_UPGRADE:		// 길드 승급
		{
			AddGuildDescString( _S( 906, "일은 잘 되어 가나? 많은 이들이 자네를 따르나 보군." ) );	
			AddGuildDescString( _S( 907, "자격이 된다면야 더 많은 사람들을 모으는 건 자네의 능력이네만," ) );			
			AddGuildDescString( _S( 908, "만일 자네가 그 많은 사람을 감당할 수 없을 수도 있으니 잘 판단하게." ) );		
			AddGuildDescString( CTString( " " ) );

			ASSERT( m_iGuildLevel <= MAX_GUILD_LEVEL && "Invalid Guild Level" );

			const int	iMyLevel	= _pNetwork->MyCharacterInfo.level;
			const SQUAD llMyMoney	= _pNetwork->MyCharacterInfo.money;
			const int	lMySP		= _pNetwork->MyCharacterInfo.sp / 10000;
			const int	iNeedLevel	= _GuildConditionTable[m_iGuildLevel].iCaptionNeedLevel;
			const SQUAD llNeedMoney = _GuildConditionTable[m_iGuildLevel].llNeedMoney;
			const int	iNeedSP		= _GuildConditionTable[m_iGuildLevel].iNeedSP;			

			strTemp.PrintF( _S( 909, "현재 길드 레벨 : %d Lv" ), m_iGuildLevel);		
			AddGuildDescString( strTemp, 0xFFC672FF );

			strTemp.PrintF( _S( 902, "최대 길드 인원 : %d 명" ), _GuildConditionTable[m_iGuildLevel].iMaxMember);		
			AddGuildDescString( strTemp, 0xFFFFFFFF );

			if( iMyLevel < iNeedLevel )
			{
				bSatisfied = FALSE;
			}

			strTemp.PrintF( _S( 903, "필요 레벨 : %d Lv" ), _GuildConditionTable[m_iGuildLevel].iCaptionNeedLevel);		
			AddGuildDescString( strTemp, iMyLevel >= iNeedLevel ? 0xFFC672FF : 0xBCBCBCFF );

			if( llMyMoney < llNeedMoney )
			{
				bSatisfied = FALSE;
			}

			strTemp.PrintF( _S( 904, "필요 나스 : %I64d 나스" ), _GuildConditionTable[m_iGuildLevel].llNeedMoney);		
			AddGuildDescString( strTemp, llMyMoney >= llNeedMoney ? 0xFFC672FF : 0xBCBCBCFF );

			if( lMySP < iNeedSP )
			{
				bSatisfied = FALSE;
			}

			strTemp.PrintF( _S( 905, "필요 S P  : %d SP" ), _GuildConditionTable[m_iGuildLevel].iNeedSP);		
			AddGuildDescString( strTemp, lMySP >= iNeedSP ? 0xFFC672FF : 0xBCBCBCFF );

#ifdef NEW_GUILD_SYSTEM
			strTemp.PrintF( _S(3839, "필요 G P  : %d GP" ), _GuildConditionTable[m_iGuildLevel].iNeedGP);		
			AddGuildDescString( strTemp, 0x70BEFFFF);
#endif
		}
		break;
	case GUILD_DESTROY:		// 길드 해산
		{
			AddGuildDescString( _S( 910, "자네도 피곤한가 보군." ) );	
			AddGuildDescString( _S( 911, "하지만, 지금 그만 둔다면 지금까지 자네를 따랐던 이들의 실망도 크겠지만, " ) );			
			AddGuildDescString( _S( 912, "자네 개인적으로도 길드에 들였던 노력이 모두 허사가 되네." ) );		
			AddGuildDescString( _S( 913, "그래도 상관 없다면 자네의 뜻대로 하게." ) );		
			AddGuildDescString( _S( 914, "지금까지 도와준 것만으로도 자네에게 고마울 따름이니까." ) );		
		}
		break;
	}

	m_btnOK.SetEnable( bSatisfied );
}

// ----------------------------------------------------------------------------
// Name : AddGuildDescString()
// Desc :
// ----------------------------------------------------------------------------
void CUIGuild::AddGuildDescString( CTString &strDesc, COLOR colDesc )
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
				m_lbGuildDesc.AddString( 0, strDesc, colDesc );
			}
			else
			{
				// Split string
				CTString	strTemp, strTemp2;
				strDesc.Split( iPos, strTemp2, strTemp );
				m_lbGuildDesc.AddString( 0, strTemp2, colDesc );

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddGuildDescString( strTemp, colDesc );
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
				m_lbGuildDesc.AddString( 0, strTemp2, colDesc );

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

				AddGuildDescString( strTemp, colDesc );
			}
			else
			{
				// Split string
				CTString	strTemp, strTemp2;
				strDesc.Split( iPos, strTemp2, strTemp );
				m_lbGuildDesc.AddString( 0, strTemp2, colDesc );

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddGuildDescString( strTemp, colDesc );
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
				m_lbGuildDesc.AddString( 0, strDesc, colDesc );
			}
			else
			{
				// Split string
				CTString	strTemp, strTemp2;
				strDesc.Split( iPos, strTemp2, strTemp );
				m_lbGuildDesc.AddString( 0, strTemp2, colDesc );

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddGuildDescString( strTemp, colDesc );
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
				m_lbGuildDesc.AddString( 0, strTemp2, colDesc );

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

				AddGuildDescString( strTemp, colDesc );
			}
			else
			{
				// Split string
				CTString	strTemp, strTemp2;
				strDesc.Split( iPos, strTemp2, strTemp );
				m_lbGuildDesc.AddString( 0, strTemp2, colDesc );

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddGuildDescString( strTemp, colDesc );
			}
		}
	}
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIGuild::Render()
{

	// WSS_NEW_GUILD_SYSTEM 070611 ---------------------->>
	if( m_eGuildState == GUILD_MANAGER_NEW )
	{		
		RenderNew();
		return;
	}
	// --------------------------------------------------<<

#ifdef CHECK_DISTANCE
	// Check distance
	FLOAT	fDiffX = _pNetwork->MyCharacterInfo.x - m_fNpcX;
	FLOAT	fDiffZ = _pNetwork->MyCharacterInfo.z - m_fNpcZ;
	if( fDiffX * fDiffX + fDiffZ * fDiffZ > UI_VALID_SQRDIST && m_eGuildState != GUILD_MANAGER )
		ResetGuild();
#endif

	// Set skill learn texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	int	nX, nY, nX2, nY2;

	// Background
	// Top
	nX = m_nPosX + m_nWidth;
	nY = m_nPosY + 26;
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY, nX, nY,
										m_rtBackTop.U0, m_rtBackTop.V0,
										m_rtBackTop.U1, m_rtBackTop.V1,
										0xFFFFFFFF );

	if( m_eGuildState != GUILD_MANAGER )
	{	
		// Not Manager
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, nX, nY + DESC_LIST_BOX_HEIGHT + 2,
											m_rtBackMiddle1.U0, m_rtBackMiddle1.V0,
											m_rtBackMiddle1.U1, m_rtBackMiddle1.V1,
											0xFFFFFFFF );

		nY += DESC_LIST_BOX_HEIGHT + 2;
	}
	else
	{
		// Manager
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, nX, nY + 37,
											m_rtBackManagerTop.U0, m_rtBackManagerTop.V0,
											m_rtBackManagerTop.U1, m_rtBackManagerTop.V1,
											0xFFFFFFFF );

		nY += 37;

		if( m_iUserRanking < GUILD_MEMBER_MEMBER )
		{
			_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, nX, nY + MEMBER_LIST_BOX_HEIGHT + 2,
				m_rtBackManagerMiddle.U0, m_rtBackManagerMiddle.V0,
				m_rtBackManagerMiddle.U1, m_rtBackManagerMiddle.V1,
				0xFFFFFFFF );
			
			nY += MEMBER_LIST_BOX_HEIGHT + 2;
		}
		else
		{
			_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, nX, nY + MEMBER_LIST_BOX_HEIGHT * 2 + 2,
				m_rtBackManagerMiddle.U0, m_rtBackManagerMiddle.V0,
				m_rtBackManagerMiddle.U1, m_rtBackManagerMiddle.V1,
				0xFFFFFFFF );
			
			nY += MEMBER_LIST_BOX_HEIGHT * 2 + 2;
		}

		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, nX, nY + 4,
											m_rtBackManagerBottom.U0, m_rtBackManagerBottom.V0,
											m_rtBackManagerBottom.U1, m_rtBackManagerBottom.V1,
											0xFFFFFFFF );

		nY += 4;
	}

	// Middle 2
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, nX, m_nPosY + m_nHeight - 7,
										m_rtBackMiddle2.U0, m_rtBackMiddle2.V0,
										m_rtBackMiddle2.U1, m_rtBackMiddle2.V1,
										0xFFFFFFFF );

	// Bottom
	nY = m_nPosY + m_nHeight - 7;
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, nX, m_nPosY + m_nHeight,
										m_rtBackBottom.U0, m_rtBackBottom.V0,
										m_rtBackBottom.U1, m_rtBackBottom.V1,
										0xFFFFFFFF );
	// ----------------------------------------------------------------------------
	if( m_eGuildState != GUILD_MANAGER )
	{
	}
	// Guild Manager
	else
	{
		nX = m_nPosX + m_rcTab.Left + GUILD_TAB_WIDTH;
		nY = m_nPosY + m_rcTab.Top;
		nX2 = nX + 1;
		nY2 = nY + 16;
		_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX2, nY2,
												m_rtTab.U0, m_rtTab.V0, m_rtTab.U1, m_rtTab.V1,
												0xFFFFFFFF );	
	}
	
	// Close button
	m_btnClose.Render();

	// Create Guild
	if(m_eGuildState == GUILD_CREATE)
	{
		// Edit Box
		// Lower left
		nX	= m_nPosX + 95;
		nX2 = m_nPosX + m_nWidth - 10;
		nY	= m_nPosY + (EDIT_BOX_Y - 3);
		nY2 = nY + 16;
		_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + 4, nY2,
			m_rtInputBoxL.U0, m_rtInputBoxL.V0, m_rtInputBoxL.U1, m_rtInputBoxL.V1,
			0xFFFFFFFF );
		// Lower middle
		_pUIMgr->GetDrawPort()->AddTexture( nX + 4, nY, nX2 - 4, nY2,
			m_rtInputBoxM.U0, m_rtInputBoxM.V0, m_rtInputBoxM.U1, m_rtInputBoxM.V1,
			0xFFFFFFFF );
		// Lower right
		_pUIMgr->GetDrawPort()->AddTexture( nX2 - 4, nY, nX2, nY2,
			m_rtInputBoxR.U0, m_rtInputBoxR.V0, m_rtInputBoxR.U1, m_rtInputBoxR.V1,
			0xFFFFFFFF );

		m_ebGuildName.Render();
	}

	if(m_eGuildState != GUILD_MANAGER)
	{
		// OK button
		m_btnOK.Render();

		// Cancel button
		m_btnCancel.Render();

		m_lbGuildDesc.Render();
	}

	if(m_eGuildState == GUILD_MANAGER)
	{
		if(m_nCurrentTab == GUILD_TAB_MEMBER)
		{
			if( m_iUserRanking < GUILD_MEMBER_MEMBER )
			{
				m_lbMemberList.Render();
				m_btnChangeBoss.Render();		// 단장 이임
				m_btnAccept.Render();			// 부단장 임명
				m_btnReject.Render();			// 부단장 해임
				m_btnMemberFire.Render();		// 멤버 퇴출
			}
			else
			{
				m_lbApplicantList.Render();
			}
		}

		m_btnExit.Render();
	}

	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	CTString strTitle = "";
	switch( m_eGuildState )
	{
	case GUILD_CREATE :
		strTitle = _S( 915, "길드 결성" );		
		break;
	case GUILD_UPGRADE:
		strTitle = _S( 916, "길드 승급" );		
		break;
	case GUILD_DESTROY:
		strTitle = _S( 917, "길드 해체" );		
		break;
	case GUILD_MANAGER:
		//strTitle = _S( 918, "길드 관리" );		
		strTitle.PrintF( _S( 919, "%s 길드 Lv.%d" ), _pNetwork->MyCharacterInfo.strGuildName, _pNetwork->MyCharacterInfo.lGuildLevel ); 
		break;
	}

	if( m_eGuildState == GUILD_CREATE)
	{
		// Text in guild
		_pUIMgr->GetDrawPort()->PutTextEx( _S( 920, "길드 이름" ), m_nPosX + GUILD_TITLE_TEXT_OFFSETX - 4,		
										m_nPosY + (EDIT_BOX_Y - 2), 0xFFFFFFFF );
	}

	// Text in guild
	_pUIMgr->GetDrawPort()->PutTextEx( strTitle, m_nPosX + GUILD_TITLE_TEXT_OFFSETX,
										m_nPosY + GUILD_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );

	if( m_eGuildState == GUILD_MANAGER)
	{
		// Tab
		nY = m_nPosY + 29;
		nX = m_nPosX + m_rcTab.Left + GUILD_TAB_WIDTH / 2;
		_pUIMgr->GetDrawPort()->PutTextExCX( _S( 921, "길드원 관리" ), nX, nY,			
											m_nCurrentTab == GUILD_TAB_MEMBER ? 0xE1B300FF : 0x6B6B6BFF );
	
		nX += GUILD_TAB_WIDTH;
		_pUIMgr->GetDrawPort()->PutTextExCX( _S( 922, "군 단" ), nX, nY,				
											m_nCurrentTab == GUILD_TAB_TREE ? 0xE1B300FF : 0x6B6B6BFF );

		if( m_nCurrentTab == GUILD_TAB_MEMBER )
		{
			CTString strText;

			CTString strLevel;
			switch(m_iUserRanking)
			{
			case GUILD_MEMBER_BOSS:			// 길드장
				{
					strLevel	= _S( 891, "길드장" );	
				}
				break;
			case GUILD_MEMBER_VICE_BOSS:	// 길드부장
				{
					strLevel	= _S( 892, "길드부장" );	
				}
				break;
			case GUILD_MEMBER_MEMBER:		// 길드원
				{
					strLevel	= _S( 893, "길드원" );	
				}
				break;
			}

			strText.PrintF( "%s %s", _pNetwork->MyCharacterInfo.name, strLevel );
			_pUIMgr->GetDrawPort()->PutTextExCX( strText, m_nPosX + (GUILD_WIDTH / 2),
											m_nPosY + 46, 0xFFFFFFFF );
		}
	
		if( m_iUserRanking != GUILD_MEMBER_MEMBER )
		{
			int nTextY = START_BUTTON_Y + 5;
			
			// Text in guild
			_pUIMgr->GetDrawPort()->PutTextEx( _S( 891, "길드장" ), m_nPosX + 10,		
				m_nPosY + nTextY, 0xBCBCBCFF );
			
			nTextY += SPAN_TEXT_Y;
			_pUIMgr->GetDrawPort()->PutTextEx( m_strBossName, m_nPosX + 10,
				m_nPosY + nTextY, 0xFFFFFFFF );
			
			nTextY += SPAN_TEXT_Y + 4;
			_pUIMgr->GetDrawPort()->PutTextEx( _S( 892, "길드부장" ), m_nPosX + 10,		
				m_nPosY + nTextY, 0xBCBCBCFF );
			
			nTextY += SPAN_TEXT_Y;
			_pUIMgr->GetDrawPort()->PutTextEx( m_strViceBoss[0], m_nPosX + 10,
				m_nPosY + nTextY, 0xFFFFFFFF );
			
			nTextY += SPAN_TEXT_Y;
			_pUIMgr->GetDrawPort()->PutTextEx( m_strViceBoss[1], m_nPosX + 10,
				m_nPosY + nTextY, 0xFFFFFFFF );
		}
		
	}

	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();
	
	if(m_eGuildState == GUILD_CREATE)
	{
		// Reading window
		if( m_ebGuildName.DoesShowReadingWindow() )
		{
			// Set texture
			_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

			// Reading window
			m_ebGuildName.RenderReadingWindow();

			// Render all elements
			_pUIMgr->GetDrawPort()->FlushRenderingQueue();

			// Flush all render text queue
			_pUIMgr->GetDrawPort()->EndTextEx();
		}
	}

}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT	CUIGuild::MouseMessage( MSG *pMsg )
{

	// TODO : 길드 정보 선택
	// 길드레벨 6이상...
	if( m_eGuildState == GUILD_MANAGER_NEW )
			return MouseMessageNew (pMsg);
	
	
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL	bTitleBarClick = FALSE;

	// Item clicked
	static BOOL	bLButtonDownInItem = FALSE;

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

			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			// Move shop
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;
				
				Move( ndX, ndY );
				
				return WMSG_SUCCESS;
			}

			// Close Button
			if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;

			if( m_eGuildState != GUILD_MANAGER)
			{
				// OK Button
				if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				// Cancel Button
				else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				// List box
				else if( m_lbGuildDesc.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				// Guild Name
				else if( m_eGuildState == GUILD_CREATE && m_ebGuildName.MouseMessage( pMsg ) != WMSG_FAIL )
				{					
					return WMSG_SUCCESS;
				}
			}
			else
			{
				if( m_nCurrentTab == GUILD_TAB_MEMBER )
				{
					if( m_iUserRanking < GUILD_MEMBER_MEMBER)
					{
						// Change Boss
						if( m_btnChangeBoss.MouseMessage( pMsg ) != WMSG_FAIL )
							return WMSG_SUCCESS;
						// Accept
						else if( m_btnAccept.MouseMessage( pMsg ) != WMSG_FAIL )
							return WMSG_SUCCESS;
						// Reject
						else if( m_btnReject.MouseMessage( pMsg ) != WMSG_FAIL )
							return WMSG_SUCCESS;
						// Member fire
						else if( m_btnMemberFire.MouseMessage( pMsg ) != WMSG_FAIL )
							return WMSG_SUCCESS;
						// List box
						else if( m_lbMemberList.MouseMessage( pMsg ) != WMSG_FAIL )
							return WMSG_SUCCESS;
						// Exit
						else if( m_btnExit.MouseMessage( pMsg ) != WMSG_FAIL )
							return WMSG_SUCCESS;
					}
					else
					{
						// List box
						if( m_lbApplicantList.MouseMessage( pMsg ) != WMSG_FAIL )
							return WMSG_SUCCESS;
						// Exit
						else if( m_btnExit.MouseMessage( pMsg ) != WMSG_FAIL )
							return WMSG_SUCCESS;
					}
				}	
				else if( m_nCurrentTab == GUILD_TAB_TREE )
				{
					// Exit
					if( m_btnExit.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
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
				// tab
				else if( m_eGuildState == GUILD_MANAGER && IsInsideRect( nX, nY, m_rcTab ) )
				{
					int	nOldTab = m_nCurrentTab;
					int	nCurTab = ( nX - m_nPosX - m_rcTab.Left ) / GUILD_TAB_WIDTH;

					if( nCurTab >= 0 && nCurTab < GUILD_TAB_TOTAL )
					{
						m_nCurrentTab = nCurTab;
					}
				}
				else if( m_eGuildState != GUILD_MANAGER )
				{
					// OK Button
					if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Cancel Button
					else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;					
					// Guild Name
					else if( (m_eGuildState == GUILD_CREATE) && (m_ebGuildName.MouseMessage( pMsg ) != WMSG_FAIL ) )
					{
						// WSS_NEW_GUILD_SYSTEM 070702
						m_ebGuildName.SetFocus(TRUE);
						// Date : 2005-03-10,   By Lee Ki-hwan
						_pUIMgr->RearrangeOrder( UI_GUILD, TRUE );
						return WMSG_SUCCESS;
					}
					// List boxi
					else if( m_lbGuildDesc.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
				else
				{
					if( m_nCurrentTab == GUILD_TAB_MEMBER )
					{
						if( m_iUserRanking < GUILD_MEMBER_MEMBER)
						{
							// Change Boss
							if( m_btnChangeBoss.MouseMessage( pMsg ) != WMSG_FAIL )
								return WMSG_SUCCESS;
							// Accept
							else if( m_btnAccept.MouseMessage( pMsg ) != WMSG_FAIL )
								return WMSG_SUCCESS;
							// Reject
							else if( m_btnReject.MouseMessage( pMsg ) != WMSG_FAIL )
								return WMSG_SUCCESS;
							// Member fire
							else if( m_btnMemberFire.MouseMessage( pMsg ) != WMSG_FAIL )
								return WMSG_SUCCESS;
							// List box
							else if( m_lbMemberList.MouseMessage( pMsg ) != WMSG_FAIL )
								return WMSG_SUCCESS;
							// Exit
							else if( m_btnExit.MouseMessage( pMsg ) != WMSG_FAIL )
								return WMSG_SUCCESS;
						}
						else
						{
							// List box
							if( m_lbApplicantList.MouseMessage( pMsg ) != WMSG_FAIL )
								return WMSG_SUCCESS;
							// Exit
							else if( m_btnExit.MouseMessage( pMsg ) != WMSG_FAIL )
								return WMSG_SUCCESS;
						}
					}				
					else if( m_nCurrentTab == GUILD_TAB_TREE )
					{
						// Exit
						if( m_btnExit.MouseMessage( pMsg ) != WMSG_FAIL )
							return WMSG_SUCCESS;
					}
				}

				_pUIMgr->RearrangeOrder( UI_GUILD, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			bTitleBarClick = FALSE;

			// Close button
			if( (wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if(wmsgResult == WMSG_COMMAND)
				{
					ResetGuild();
				}
				return WMSG_SUCCESS;
			}
			else if( m_eGuildState != GUILD_MANAGER )
			{
				// OK Button
				if( (wmsgResult = m_btnOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if(wmsgResult == WMSG_COMMAND)
					{
						PressOKBtn();
					}
					return WMSG_SUCCESS;
				}
				// Cancel Button
				else if( (wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if(wmsgResult == WMSG_COMMAND)
					{
						ResetGuild();
					}
					return WMSG_SUCCESS;
				}
				// List box
				else if( m_lbGuildDesc.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS;
				}
			}
			else
			{
				if( m_nCurrentTab == GUILD_TAB_MEMBER )
				{	
					if( m_iUserRanking < GUILD_MEMBER_MEMBER)
					{
						// Change Boss(단장 이임)
						if( (wmsgResult = m_btnChangeBoss.MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							if(wmsgResult == WMSG_COMMAND)
							{
								ChangeBoss();
							}
							return WMSG_SUCCESS;
						}
						// Accept(부단장 임명)
						else if( (wmsgResult = m_btnAccept.MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							if(wmsgResult == WMSG_COMMAND)
							{
								AddViceBoss();
							}
							return WMSG_SUCCESS;
						}
						// Reject(부단장 해임)
						else if( (wmsgResult = m_btnReject.MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							if(wmsgResult == WMSG_COMMAND)
							{
								DelViceBoss();
							}
							return WMSG_SUCCESS;
						}
						// Member fire(단원 퇴출)
						else if( (wmsgResult = m_btnMemberFire.MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							if(wmsgResult == WMSG_COMMAND)
							{
								MemberFire();
							}
							return WMSG_SUCCESS;
						}
						// List box
						else if( m_lbMemberList.MouseMessage( pMsg ) != WMSG_FAIL )
						{
							return WMSG_SUCCESS;
						}
						// Exit
						else if( (wmsgResult = m_btnExit.MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							if(wmsgResult == WMSG_COMMAND)
							{
								ResetGuild();
							}
							return WMSG_SUCCESS;
						}
					}
					else
					{
						// List box
						if( m_lbApplicantList.MouseMessage( pMsg ) != WMSG_FAIL )
						{
							return WMSG_SUCCESS;
						}
						// Exit
						else if( (wmsgResult = m_btnExit.MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							if(wmsgResult == WMSG_COMMAND)
							{
								ResetGuild();
							}
							return WMSG_SUCCESS;
						}
					}
				}			
				else if( m_nCurrentTab == GUILD_TAB_TREE )
				{
					// Exit
					if( (wmsgResult = m_btnExit.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if(wmsgResult == WMSG_COMMAND)
						{
							ResetGuild();
						}
						return WMSG_SUCCESS;
					}
				}
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				if( m_eGuildState != GUILD_MANAGER )
				{
					// List box of guild desc
					if( m_lbGuildDesc.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
				else
				{
					if( m_nCurrentTab == GUILD_TAB_MEMBER )
					{
						if( m_iUserRanking < GUILD_MEMBER_MEMBER)
						{
							// List box of member list
							if( m_lbMemberList.MouseMessage( pMsg ) != WMSG_FAIL )
								return WMSG_SUCCESS;
						}
						else
						{
							// List box of member list
							if( m_lbApplicantList.MouseMessage( pMsg ) != WMSG_FAIL )
								return WMSG_SUCCESS;
						}
					}				
				}
			}
		}
		break;
	}
	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : KeyMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT	CUIGuild::KeyMessage( MSG *pMsg )
{	
	if( !IsFocused() )
		return WMSG_FAIL;

	if( m_iGuildLevel < LIMIT_GUILD_LEVEL )
	{
		if( m_ebGuildName.KeyMessage( pMsg ) != WMSG_FAIL )
		{
			return WMSG_SUCCESS; 
		}
	}
	else 
	{

		switch(m_iSelTab)
		{
			case NEW_GUILD_NOTICE:
			{
				if( m_ebNoticeTitle.KeyMessage( pMsg ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS; 
				}
				if( m_mebNoticeContent.KeyMessage( pMsg ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS; 
				}	
				if( pMsg->wParam == VK_TAB )
				{
					if( m_ebNoticeTitle.IsFocused() )
					{
						m_ebNoticeTitle.SetFocus( FALSE );
						m_mebNoticeContent.SetFocus( TRUE );					
					}
					else if( m_mebNoticeContent.IsFocused() )
					{
						m_ebNoticeTitle.SetFocus( TRUE );
						m_mebNoticeContent.SetFocus( FALSE );
					}
					return WMSG_SUCCESS;
				}
			}
			break;
			case NEW_GUILD_MANAGE:
			{
				if(m_bApplySettingOn)
				{
					if( m_ebChangePositionName.KeyMessage( pMsg ) != WMSG_FAIL )
					{
						return WMSG_SUCCESS; 
					}
					if( m_ebChangePayExp.KeyMessage( pMsg ) != WMSG_FAIL )
					{
						return WMSG_SUCCESS; 
					}
					if( m_ebChangePayFame.KeyMessage( pMsg ) != WMSG_FAIL )
					{
						return WMSG_SUCCESS; 
					}
					
					if( pMsg->wParam == VK_TAB )
					{
						if( m_ebChangePositionName.IsFocused() )
						{
							m_ebChangePositionName.SetFocus( FALSE );
							m_ebChangePayExp.SetFocus( TRUE );
							m_ebChangePayFame.SetFocus( FALSE );
						}
						else if( m_ebChangePayExp.IsFocused() )
						{
							m_ebChangePositionName.SetFocus( FALSE );
							m_ebChangePayExp.SetFocus( FALSE );
							m_ebChangePayFame.SetFocus( TRUE );
						}
						else if( m_ebChangePayFame.IsFocused() )
						{
							m_ebChangePositionName.SetFocus( TRUE );
							m_ebChangePayExp.SetFocus( FALSE );
							m_ebChangePayFame.SetFocus( FALSE );
						}

						return WMSG_SUCCESS;
					}
				}
			}			
			break;	
#ifndef LOCAL_NEW_GUILD // 해외 로컬 웹보드사용 안함
			case NEW_GUILD_BOARD:
			{
				// List
				if( pWEB->m_nCurBoardType == UWT_LIST )
				{
					if( pMsg->wParam == VK_RETURN )
					{
						if( m_ebSearch.IsFocused() )
						{
							pWEB->m_nWantPage = 1;
							pWEB->DelayCommandPrepare(new CCommandList);
							return WMSG_SUCCESS;
						}
					}
					// Search edit box
					if( m_ebSearch.KeyMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
				else if( pWEB->m_nCurBoardType >= UWT_WRITE && pWEB->m_nCurBoardType <= UWT_MODIFY )
				{
					// 이기환 수정 시작(05.01.01) : 제목과 내용 사이에 TAB키로 포커스 이동
					if( pMsg->wParam == VK_TAB )
					{
						if( m_ebWriteSubject.IsFocused() )
						{
							m_ebWriteSubject.SetFocus ( FALSE );
							m_mebContent.SetFocus ( TRUE );

						}
						else if( m_mebContent.IsFocused() )
						{
							m_ebWriteSubject.SetFocus ( TRUE );
							m_mebContent.SetFocus ( FALSE );
						}
						return WMSG_SUCCESS;
					}
					// 이기환 수정 끝 : 
						
					
					// 이기환 수정 시작(05.01.01) : 멀티 에디트 박스 루틴 추가 
					// Subject edit box
					if ( m_ebWriteSubject.KeyMessage ( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Content multi-edit box
					else if ( m_mebContent.KeyMessage ( pMsg ) != WMSG_FAIL )
					{
						return WMSG_SUCCESS;
					}
					// 이기환 수정 끝
				}
			}
			break;
#endif		
		}	
	}	
	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : IMEMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT	CUIGuild::IMEMessage( MSG *pMsg )
{
	if( m_iGuildLevel < LIMIT_GUILD_LEVEL )
	{
		if(m_ebGuildName.IsFocused())
		return m_ebGuildName.IMEMessage( pMsg );
	}
	else {
		switch(m_iSelTab)
		{
			case NEW_GUILD_NOTICE:
			{
				if(m_ebNoticeTitle.IsFocused())
					return m_ebNoticeTitle.IMEMessage( pMsg );
				if(m_mebNoticeContent.IsFocused())
					return m_mebNoticeContent.IMEMessage( pMsg );
			}
			break;
			case NEW_GUILD_MANAGE:
			{
				if(m_bApplySettingOn)
				{
					if(m_ebChangePositionName.IsFocused())
						return m_ebChangePositionName.IMEMessage( pMsg );
					if(m_ebChangePayExp.IsFocused())
						return m_ebChangePayExp.IMEMessage( pMsg );
					if(m_ebChangePayFame.IsFocused())
						return m_ebChangePayFame.IMEMessage( pMsg );
				}
			}			
			break;
#ifndef LOCAL_NEW_GUILD // 해외 로컬 웹보드사용 안함
			case NEW_GUILD_BOARD:
			{
				// List
				if( pWEB->m_nCurBoardType == UWT_LIST )
				{				
					// Search edit box
					return m_ebSearch.IMEMessage( pMsg );				
				}
				else if( pWEB->m_nCurBoardType >= UWT_WRITE && pWEB->m_nCurBoardType <= UWT_MODIFY )
				{					
					// Subject edit box
					WMSG_RESULT wmsgResult;
					if ( ( wmsgResult = m_ebWriteSubject.IMEMessage( pMsg ) ) != WMSG_FAIL ) 
					{
						return wmsgResult;
					}
					// Content multi-edit box
					return m_mebContent.IMEMessage ( pMsg );			
				}
			}
			break;
#endif
		}	
	}
	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : CharMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT	CUIGuild::CharMessage( MSG *pMsg )
{
	if( m_iGuildLevel < LIMIT_GUILD_LEVEL )
	{
		if( m_ebGuildName.CharMessage( pMsg ) != WMSG_FAIL )
		{
			return WMSG_SUCCESS;
		}

	}
	else 
	{
		switch(m_iSelTab)
		{
			case NEW_GUILD_NOTICE:
			{
				if( m_ebNoticeTitle.CharMessage( pMsg ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS; 
				}
				if( m_mebNoticeContent.CharMessage( pMsg ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS; 
				}
			}
			break;
			case NEW_GUILD_MANAGE:
			{
				if(m_bApplySettingOn)
				{
					if( m_ebChangePositionName.CharMessage( pMsg ) != WMSG_FAIL )
					{
						return WMSG_SUCCESS; 
					}
					if( m_ebChangePayExp.CharMessage( pMsg ) != WMSG_FAIL )
					{
						return WMSG_SUCCESS; 
					}
					if( m_ebChangePayFame.CharMessage( pMsg ) != WMSG_FAIL )
					{
						return WMSG_SUCCESS; 
					}

				}
			}			
			break;
#ifndef LOCAL_NEW_GUILD // 해외 로컬 웹보드사용 안함
			case NEW_GUILD_BOARD:
			{
				// List
				if( pWEB->m_nCurBoardType == UWT_LIST )
				{
					// Search edit box
					if( m_ebSearch.CharMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
				else if( pWEB->m_nCurBoardType >= UWT_WRITE && pWEB->m_nCurBoardType <= UWT_MODIFY )
				{
					// Subject edit box
					if( m_ebWriteSubject.CharMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// 이기환 수정 시작(05.01.01) : 멀티 에디트 박스 루틴 추가 
					// Content multi-edit box
					else if ( m_mebContent.CharMessage ( pMsg ) != WMSG_FAIL )
					{
						return WMSG_SUCCESS;
					}								
				}
			}
			break;
#endif
		
		}	

	}
	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUIGuild::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{	
	switch( nCommandCode )
	{
	case MSGCMD_GUILD_ERROR:	// 에러 발생
		{
			if( !bOK )
				return;
		}
		break;
	case MSGCMD_GUILD_JOIN:		// 길드 가입
		{
			if( !bOK )
				return;

			CTString	strMessage;
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo(  _S( 865, "길드" ) , UMBS_CANCEL, UI_GUILD, MSGCMD_GUILD_JOIN_REQ );	
			strMessage.PrintF(  _S( 923, "길드 가입 신청중입니다." )  );		
			MsgBoxInfo.AddString( strMessage );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );

			_pNetwork->GuildJoin( g_lGuildIndex, g_lChaIndex, _pNetwork->MyCharacterInfo.index );
		}
		break;
	case MSGCMD_GUILD_JOIN_REQ:
		{
			// 단장이 취소하면 1 아니면 0
			_pNetwork->GuildApplicantReject( FALSE );
		}
		break;
	case MSGCMD_GUILD_QUIT:		// 길드 탈퇴
		{
			if( !bOK )
				return;
			// Create message box of guild destroy
			CTString	strMessage;
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 865, "길드" ), UMBS_OKCANCEL, UI_GUILD, MSGCMD_GUILD_QUIT_CONFIRM );	
			strMessage.PrintF( _S( 924, "정말로 길드에서 탈퇴하시겠습니까? 탈퇴하시면 7일 동안 길드에 가입하실 수 없습니다." ) );		
			MsgBoxInfo.AddString( strMessage );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );
		}
		break;
	case MSGCMD_GUILD_QUIT_CONFIRM:		// 탈퇴 확인
		{
			if( !bOK )
				return;
			_pNetwork->GuildQuit();
		}
		break;
	case MSGCMD_GUILD_DESTROY:			// 길드 해산
		{
			if( !bOK )
				return;
			DestroyGuild();
		}
		break;
	case MSGCMD_GUILD_APPLICANT_JOIN:	// 신청자 참여.
		{
			if( !bOK )
				// 단장이 취소하면 1 아니면 0
				_pNetwork->GuildApplicantReject( TRUE );			// 가입 거부
			else
				_pNetwork->GuildApplicantAccept( g_lChaIndex );		// 가입 허용
		}
		break;

	case MSGCMD_GUILD_ADD_VICE_BOSS:	// 부관 임명
		{
			if( !bOK )
				return;
			_pNetwork->GuildAddViceBoss( g_lMemberIndex );
		}
		break;
	case MSGCMD_GUILD_DEL_VICE_BOSS:	// 부관 해임
		{
			if( !bOK )
				return;
			_pNetwork->GuildDelViceBoss( g_lMemberIndex );
		}
		break;
	case MSGCMD_GUILD_CHANGE_BOSS:		// 단장 이임
		{
			if( !bOK )
				return;
			_pNetwork->GuildChangeBoss( g_lMemberIndex );
		}
		break;
	case MSGCMD_GUILD_KICK:				// 멤버 짜르기
		{
			if( !bOK )
				return;
			_pNetwork->GuildMemberFire( g_lMemberIndex );
		}
		break;
	case MSGCMD_GUILD_ROOM:				// 길드 전용 공간으로...
		{
			if( !bOK )
				return;
			GoGuildZone();
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : MsgBoxLCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUIGuild::MsgBoxLCommand( int nCommandCode, int nResult )
{
	switch( nCommandCode )
	{
	case MSGLCMD_GUILD_REQ:
		{
			// [090527: selo] 확장팩 퀘스트 이야기 한다 처리 수정을 위한 루틴
			int iQuestIndex = -1;
			if( ciQuestClassifier < nResult )	
			{
				iQuestIndex = nResult;
				nResult = ciQuestClassifier;
			}

			switch ( nResult ) 
			{
			case SEL_GUILD_CREATE:
				{
					// 길드 멤버가 아닐때만 길드 생성함.
					if( m_iUserRanking == GUILD_MEMBER_NOMEMBER )
					{
						m_eGuildState = GUILD_CREATE; 
						GetGuildDesc( TRUE );
						_pUIMgr->RearrangeOrder( UI_GUILD, TRUE );
					}
					else
					{
						m_eGuildState = GUILD_REQ;
						_pUIMgr->RearrangeOrder( UI_GUILD, FALSE );
					}
				}
				break;

			case SEL_GUILD_DESTROY:
				{
					m_eGuildState = GUILD_DESTROY;
					GetGuildDesc( TRUE );
					_pUIMgr->RearrangeOrder( UI_GUILD, TRUE );
				}
				break;

			case SEL_GUILD_UPGRADE:			// 길드 승급
				{
					if( m_iGuildLevel < MAX_GUILD_LEVEL )
					{
						m_eGuildState = GUILD_UPGRADE;
						GetGuildDesc( TRUE );
						_pUIMgr->RearrangeOrder( UI_GUILD, TRUE );
					}
					else
					{
						m_eGuildState = GUILD_REQ;
						_pUIMgr->RearrangeOrder( UI_GUILD, FALSE );
					}
				}
				break;

			case SEL_GUILD_ROOM:
				{
					_pUIMgr->CloseMessageBox( MSGCMD_GUILD_ROOM );
					CTString	strMessage;
					CUIMsgBox_Info	MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo( _S( 865, "길드" ), UMBS_OKCANCEL, UI_GUILD, MSGCMD_GUILD_ROOM );	
					strMessage.PrintF( _S( 1012, "길드 대화방으로 이동하시겠습니까?" ) );
					MsgBoxInfo.AddString( strMessage );
					_pUIMgr->CreateMessageBox( MsgBoxInfo );				
				}
				break;

			case SEL_GUILD_TALK:
				{
					// 퀘스트 창 띄우기
					//_pUIMgr->GetQuest()->OpenQuest( _pUIMgr->GetCharacterInfo()->GetMobIndex(), m_fNpcX, m_fNpcZ );
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

			default:		// 취소
				{
					m_eGuildState = GUILD_REQ;
					_pUIMgr->RearrangeOrder( UI_GUILD, FALSE );
				}
				break;
			}
		}
	}
}


void CUIGuild::SetMemberName(int charIndex,CTString strName)
{
	std::vector<sGuildMember>::iterator iBegin,iEnd,iNext;
	iBegin = iNext = m_vectorMemberList.begin();
	iEnd = m_vectorMemberList.end();
	for( ; iNext != iEnd ; iNext++)
	{
		if(iNext->lIndex == charIndex){
			iNext->strMemberName = strName;
			return;
		}
	}


	
}

// 길드 시스템 추가 작업 ------------------------------------------------------------------------------------------->>
// WSS_NEW_GUILD_SYSTEM 070703

// ----------------------------------------------------------------------------
// Name : InitNewGuildSystem()
// Desc :
// ----------------------------------------------------------------------------
void CUIGuild::InitNewGuildSystem()
{
	m_iSelTab				= 0;				// 디폴트 텝
	m_iGuildAverageLevel	= 0;				// 길드 평균 레벨
	m_iGuildOwnLand			= 0;				// 길드 소유 영지
	m_iGuildTotalPoint		= 0;				// 길드 총 포인트
	m_iGuildMyPoint			= 0;				// 나의 기여 포인트
	m_bApplySettingOn		= FALSE;			// 설정 변경창 상태
	m_iOnlineMembers		= 0;				// 접속한 길드원	
	m_iGuildSkillPos		= 0;
	m_bIsSelList			= FALSE;	
	m_bEnableCorrect		= FALSE;			// 공지사항 수정여부
	m_lbMemberAllianceList.ResetAllStrings();
	m_lbMemberHostilityList.ResetAllStrings();
	m_lbGuildMemberList.ResetAllStrings();
	m_sbGuildSkillBar.SetScrollPos(0);
	m_ebNoticeTitle.ResetString();
	m_mebNoticeContent.ResetString();	
	m_lbManageMemberList.ResetAllStrings();
	m_lbGuildSkillDesc.ResetAllStrings();	
	m_ebSearch.ResetString();
	m_lbListContent.ResetAllStrings();
	m_lbReadContent.ResetAllStrings();
	m_ebWriteSubject.ResetString();	
	m_mebContent.ResetString();

	// Init Web Board
	pWEB->m_nCurSection = UWS_GUILD;
	pWEB->m_nCurBoardType = UWT_LIST;	
	pWEB->m_nWantModifyMode = UWM_WRITE;
	pWEB->m_nCurrentPage = 0;
	
}

// ----------------------------------------------------------------------------
// Name : CreateNew()
// Desc :
// ----------------------------------------------------------------------------
void CUIGuild::CreateNew( CUIWindow *pParentWnd, int nX, int nY)
{	
	// Region of each part
	m_rcTitleNew.SetRect( 0, 0, NEW_GUILD_SYSTEM_WIDTH-35, 22 );
	m_rcWindowNew.SetRect( 0, 0, NEW_GUILD_SYSTEM_WIDTH, NEW_GUILD_SYSTEM_HEIGHT );
	for(int i=0; i<MAX_GUILDINFO_TAB; i++)
	{
		m_rcTabNew[i].SetRect( 98*i+7, 27, 98*(i+1)+6, 47 );
	}

	// window position
//	SetPos( nX, nY );	
//	SetSize( NEW_GUILD_SYSTEM_WIDTH, NEW_GUILD_SYSTEM_HEIGHT );

	// Create texture
	ASSERT(m_ptdBaseTexture!=NULL);
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	
	// 길드 정보 --------------------------------------------->>
	// UV Coordinate of each part

	// Set Box
	m_bxBackGroundBox.SetBoxUV(m_ptdBaseTexture,30,22,WRect(0,0,216,22));
	m_bxBackGroundBox2.SetBoxUV(m_ptdBaseTexture,20,11,WRect(0,22,216,45));	
	m_bxBox1.SetBoxUV(m_ptdBaseTexture,3,WRect(0,68,34,102));
	m_bxBox1_1.SetBoxUV(m_ptdBaseTexture,3,WRect(0,68,32,100));
	m_bxBox1_2.SetBoxUV(m_ptdBaseTexture,3,WRect(0,72,36,102));
	m_bxBox2.SetBoxUV(m_ptdBaseTexture,3,WRect(42,86,245,105));

	// List box of Alliance List
	m_lbMemberAllianceList.Create( this, 287,151,135,133, _pUIFontTexMgr->GetLineHeight(), 13, 3, 2, TRUE );
	m_lbMemberAllianceList.CreateScroll( TRUE, 0, 0, 9, 133, 9, 7, 0, 0, 10 );
	m_lbMemberAllianceList.SetSelBar( 129 , _pUIFontTexMgr->GetLineHeight(), 187, 46, 204, 61, fTexWidth, fTexHeight );
	m_lbMemberAllianceList.SetOverColor( 0xF8E1B5FF );
	m_lbMemberAllianceList.SetSelectColor( 0xF8E1B5FF );
	m_lbMemberAllianceList.SetColumnPosX( 1, 110 );
	// Up button
	m_lbMemberAllianceList.SetScrollUpUV( UBS_IDLE, 230, 16, 239, 23, fTexWidth, fTexHeight );
	m_lbMemberAllianceList.SetScrollUpUV( UBS_CLICK, 240, 16, 249, 23, fTexWidth, fTexHeight );
	m_lbMemberAllianceList.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbMemberAllianceList.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_lbMemberAllianceList.SetScrollDownUV( UBS_IDLE, 230, 24, 239, 31, fTexWidth, fTexHeight );
	m_lbMemberAllianceList.SetScrollDownUV( UBS_CLICK, 240, 24, 249, 31, fTexWidth, fTexHeight );
	m_lbMemberAllianceList.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbMemberAllianceList.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbMemberAllianceList.SetScrollBarTopUV( 219, 16, 228, 26, fTexWidth, fTexHeight );
	m_lbMemberAllianceList.SetScrollBarMiddleUV( 219, 27, 228, 29, fTexWidth, fTexHeight );
	m_lbMemberAllianceList.SetScrollBarBottomUV( 219, 30, 228, 40, fTexWidth, fTexHeight );	

	// List box of Hostility List
	m_lbMemberHostilityList.Create( this, 437, 151, 135, 133, _pUIFontTexMgr->GetLineHeight(), 13, 3, 2, TRUE );
	m_lbMemberHostilityList.CreateScroll( TRUE, 0, 0, 9, 133, 9, 7, 0, 0, 10 );
	m_lbMemberHostilityList.SetSelBar( 129 , _pUIFontTexMgr->GetLineHeight(), 187, 46, 204, 61, fTexWidth, fTexHeight );
	m_lbMemberHostilityList.SetOverColor( 0xF8E1B5FF );
	m_lbMemberHostilityList.SetSelectColor( 0xF8E1B5FF );
	m_lbMemberHostilityList.SetColumnPosX( 1, 110 );
	// Up button
	m_lbMemberHostilityList.SetScrollUpUV( UBS_IDLE, 230, 16, 239, 23, fTexWidth, fTexHeight );
	m_lbMemberHostilityList.SetScrollUpUV( UBS_CLICK, 240, 16, 249, 23, fTexWidth, fTexHeight );
	m_lbMemberHostilityList.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbMemberHostilityList.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_lbMemberHostilityList.SetScrollDownUV( UBS_IDLE, 230, 24, 239, 31, fTexWidth, fTexHeight );
	m_lbMemberHostilityList.SetScrollDownUV( UBS_CLICK, 240, 24, 249, 31, fTexWidth, fTexHeight );
	m_lbMemberHostilityList.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbMemberHostilityList.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbMemberHostilityList.SetScrollBarTopUV( 219, 16, 228, 26, fTexWidth, fTexHeight );
	m_lbMemberHostilityList.SetScrollBarMiddleUV( 219, 27, 228, 29, fTexWidth, fTexHeight );
	m_lbMemberHostilityList.SetScrollBarBottomUV( 219, 30, 228, 40, fTexWidth, fTexHeight );	

	// Background
	m_uvTabNew.SetUV( 0, 0, 216, 26, fTexWidth, fTexHeight );

	// Lines
	m_uvLineH.SetUV(43,86,244,86,fTexWidth,fTexHeight);
	m_uvLineV.SetUV(43,86,43,103,fTexWidth,fTexHeight);

	// Buttons
	// Close button
	m_btnCloseNew.Create( this, CTString( "" ), 572, 5, 14, 14 );
	m_btnCloseNew.SetUV( UBS_IDLE, 219, 0, 233, 14, fTexWidth, fTexHeight );
	m_btnCloseNew.SetUV( UBS_CLICK, 234, 0, 248, 14, fTexWidth, fTexHeight );
	m_btnCloseNew.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCloseNew.CopyUV( UBS_IDLE, UBS_DISABLE );
	// Exit button
	m_btnExitNew.Create( this, _S( 870, "닫기" ), 523,308, 63, 21 );			
	m_btnExitNew.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnExitNew.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnExitNew.CopyUV( UBS_IDLE, UBS_ON );
	m_btnExitNew.CopyUV( UBS_IDLE, UBS_DISABLE );
	// Edit button
	m_btnEdit.Create( this, _S(3840, "편집" ), 215, 255, 63, 21 );			
	m_btnEdit.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnEdit.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnEdit.CopyUV( UBS_IDLE, UBS_ON );
	m_btnEdit.CopyUV( UBS_IDLE, UBS_DISABLE );
	// Guild Mark button
	m_btnGuildMarkBack.Create( this, CTString(""), 26, 244, 33, 33 );			
	m_btnGuildMarkBack.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnGuildMarkBack.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnGuildMarkBack.CopyUV( UBS_IDLE, UBS_ON );
	m_btnGuildMarkBack.CopyUV( UBS_IDLE, UBS_DISABLE );
	// ---------------------------------------------------------<<

	// 길드원 정보 --------------------------------------------->>
	// Member List
	m_lbGuildMemberList.Create( this, 20, 78, 553, 161, _pUIFontTexMgr->GetLineHeight(), 13, 3, 8, TRUE );
	m_lbGuildMemberList.CreateScroll( TRUE, 0, 0, 9, 161, 9, 7, 0, 0, 10 );
	m_lbGuildMemberList.SetSelBar( 553 , _pUIFontTexMgr->GetLineHeight(), 187, 46, 204, 61, fTexWidth, fTexHeight );
	m_lbGuildMemberList.SetOverColor( 0xF8E1B5FF );
	m_lbGuildMemberList.SetSelectColor( 0xF8E1B5FF );
	m_lbGuildMemberList.SetColumnPosX( 0, 0 );
	m_lbGuildMemberList.SetColumnPosX( 1, 63 ,TEXT_CENTER);
	m_lbGuildMemberList.SetColumnPosX( 2, 150 ,TEXT_CENTER);
	m_lbGuildMemberList.SetColumnPosX( 3, 208 ,TEXT_CENTER);
	m_lbGuildMemberList.SetColumnPosX( 4, 270 ,TEXT_CENTER);
	m_lbGuildMemberList.SetColumnPosX( 5, 356 ,TEXT_CENTER);
	m_lbGuildMemberList.SetColumnPosX( 6, 421 ,TEXT_CENTER);
	m_lbGuildMemberList.SetColumnPosX( 7, 534,TEXT_RIGHT);

	// Up button
	m_lbGuildMemberList.SetScrollUpUV( UBS_IDLE, 230, 16, 239, 23, fTexWidth, fTexHeight );
	m_lbGuildMemberList.SetScrollUpUV( UBS_CLICK, 240, 16, 249, 23, fTexWidth, fTexHeight );
	m_lbGuildMemberList.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbGuildMemberList.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_lbGuildMemberList.SetScrollDownUV( UBS_IDLE, 230, 24, 239, 31, fTexWidth, fTexHeight );
	m_lbGuildMemberList.SetScrollDownUV( UBS_CLICK, 240, 24, 249, 31, fTexWidth, fTexHeight );
	m_lbGuildMemberList.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbGuildMemberList.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbGuildMemberList.SetScrollBarTopUV( 219, 16, 228, 26, fTexWidth, fTexHeight );
	m_lbGuildMemberList.SetScrollBarMiddleUV( 219, 27, 228, 29, fTexWidth, fTexHeight );
	m_lbGuildMemberList.SetScrollBarBottomUV( 219, 30, 228, 40, fTexWidth, fTexHeight );	
	
	// -----------------------------------------------------------<<

	// 길드 스킬 ------------------------------------------------->>
	// Get Skill
	m_btnGetSkill.Create( this, _S(3841, "습득하기" ), 409, 246, 100, 21 );
	m_btnGetSkill.SetUV( UBS_IDLE, 134, 117, 228, 138, fTexWidth, fTexHeight );
	m_btnGetSkill.SetUV( UBS_CLICK, 134, 139, 228, 160, fTexWidth, fTexHeight );
	m_btnGetSkill.CopyUV( UBS_IDLE, UBS_ON );
	m_btnGetSkill.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Left Skill Info scroll bar
	m_sbGuildSkillBar.Create( this, 318, 59, 9, 219 );
	m_sbGuildSkillBar.CreateButtons( TRUE, 9, 7, 0, 0, 10 );
	m_sbGuildSkillBar.SetScrollPos( 0 );
	m_sbGuildSkillBar.SetCurItemCount( 0 );
	m_sbGuildSkillBar.SetItemsPerPage( 5 );
	// Up button
	m_sbGuildSkillBar.SetUpUV( UBS_IDLE, 228, 16, 237, 22, fTexWidth, fTexHeight );
	m_sbGuildSkillBar.SetUpUV( UBS_CLICK, 228, 33, 237, 39, fTexWidth, fTexHeight );
	m_sbGuildSkillBar.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbGuildSkillBar.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_sbGuildSkillBar.SetDownUV( UBS_IDLE, 228, 25, 237, 32, fTexWidth, fTexHeight );
	m_sbGuildSkillBar.SetDownUV( UBS_CLICK, 228, 42, 237, 49, fTexWidth, fTexHeight );
	m_sbGuildSkillBar.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbGuildSkillBar.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbGuildSkillBar.SetBarTopUV( 217, 16, 226, 25, fTexWidth, fTexHeight );
	m_sbGuildSkillBar.SetBarMiddleUV( 217, 26, 226, 30, fTexWidth, fTexHeight );
	m_sbGuildSkillBar.SetBarBottomUV( 217, 31, 226, 41, fTexWidth, fTexHeight );
	// Wheel region
	m_sbGuildSkillBar.SetWheelRect( 21, 59, 326, 278 );	
	
	// Member List	
	m_lbGuildSkillDesc.Create( this, 341, 78, 226, 75, _pUIFontTexMgr->GetLineHeight(), 5, 3, 1, FALSE );
	m_lbGuildSkillDesc.CreateScroll( TRUE, 0, 0, 9, 75, 9, 7, 0, 0, 10 );	
	
	// Up button
	m_lbGuildSkillDesc.SetScrollUpUV( UBS_IDLE, 230, 16, 239, 23, fTexWidth, fTexHeight );
	m_lbGuildSkillDesc.SetScrollUpUV( UBS_CLICK, 240, 16, 249, 23, fTexWidth, fTexHeight );
	m_lbGuildSkillDesc.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbGuildSkillDesc.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_lbGuildSkillDesc.SetScrollDownUV( UBS_IDLE, 230, 24, 239, 31, fTexWidth, fTexHeight );
	m_lbGuildSkillDesc.SetScrollDownUV( UBS_CLICK, 240, 24, 249, 31, fTexWidth, fTexHeight );
	m_lbGuildSkillDesc.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbGuildSkillDesc.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbGuildSkillDesc.SetScrollBarTopUV( 219, 16, 228, 26, fTexWidth, fTexHeight );
	m_lbGuildSkillDesc.SetScrollBarMiddleUV( 219, 27, 228, 29, fTexWidth, fTexHeight );
	m_lbGuildSkillDesc.SetScrollBarBottomUV( 219, 30, 228, 40, fTexWidth, fTexHeight );

	m_rcGuildSkillList.SetRect( 33, 69, 307, 269 );
	// -----------------------------------------------------------<<

	// 길드 게시판 ----------------------------------------------->>
	m_bxBox3.SetBoxUV(m_ptdBaseTexture,3,WRect(147,47,159,59));
	m_bxBox4.SetBoxUV(m_ptdBaseTexture,3,WRect(55,162,71,177));
	
	// Prev button
	m_btnPrev.Create( this, CTString( "" ), 0, 0, 10, 10 );
	m_btnPrev.SetUV( UBS_IDLE, 242, 162, 230, 172, fTexWidth, fTexHeight );
	m_btnPrev.SetUV( UBS_CLICK, 230, 147, 242, 159, fTexWidth, fTexHeight );
	m_btnPrev.CopyUV( UBS_IDLE, UBS_ON );
	m_btnPrev.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Next button
	m_btnNext.Create( this, CTString( "" ), 0, 0, 10, 10 );
	m_btnNext.SetUV( UBS_IDLE,  230, 172, 242, 162,fTexWidth, fTexHeight );
	m_btnNext.SetUV( UBS_CLICK, 242, 159, 230, 147, fTexWidth, fTexHeight );
	m_btnNext.CopyUV( UBS_IDLE, UBS_ON );
	m_btnNext.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Search button
	m_btnSearch.Create( this, _S( 386, "검색" ), 392, 270, 63, 21 );
	m_btnSearch.SetUV( UBS_IDLE, 134, 117, 228, 138, fTexWidth, fTexHeight );
	m_btnSearch.SetUV( UBS_CLICK, 134, 139, 228, 160, fTexWidth, fTexHeight );
	m_btnSearch.CopyUV( UBS_IDLE, UBS_ON );
	m_btnSearch.CopyUV( UBS_IDLE, UBS_DISABLE );
																// List button
	m_btnList.Create( this, _S( 313, "목록" ), 23, 270, 63, 21 );
	m_btnList.SetUV( UBS_IDLE, 134, 117, 228, 138, fTexWidth, fTexHeight );
	m_btnList.SetUV( UBS_CLICK, 134, 139, 228, 160, fTexWidth, fTexHeight );
	m_btnList.CopyUV( UBS_IDLE, UBS_ON );
	m_btnList.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Write button
	m_btnWrite.Create( this, _S( 314, "쓰기" ), 503, 270, 63, 21 );
	m_btnWrite.SetUV( UBS_IDLE, 134, 117, 228, 138, fTexWidth, fTexHeight );
	m_btnWrite.SetUV( UBS_CLICK, 134, 139, 228, 160, fTexWidth, fTexHeight );
	m_btnWrite.CopyUV( UBS_IDLE, UBS_ON );
	m_btnWrite.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Reply button
	m_btnReply.Create( this, _S( 321, "답글" ), 377, 270, 63, 21 );
	m_btnReply.SetUV( UBS_IDLE, 134, 117, 228, 138, fTexWidth, fTexHeight );
	m_btnReply.SetUV( UBS_CLICK, 134, 139, 228, 160, fTexWidth, fTexHeight );
	m_btnReply.CopyUV( UBS_IDLE, UBS_ON );
	m_btnReply.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Delete button
	m_btnDelete.Create( this, _S( 338, "삭제" ), 446, 270, 63, 21 );
	m_btnDelete.SetUV( UBS_IDLE, 134, 117, 228, 138, fTexWidth, fTexHeight );
	m_btnDelete.SetUV( UBS_CLICK, 134, 139, 228, 160, fTexWidth, fTexHeight );
	m_btnDelete.CopyUV( UBS_IDLE, UBS_ON );
	m_btnDelete.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Modify button
	m_btnModify.Create( this, _S( 339, "수정" ), 515, 270, 63, 21 );
	m_btnModify.SetUV( UBS_IDLE, 134, 117, 228, 138, fTexWidth, fTexHeight );
	m_btnModify.SetUV( UBS_CLICK, 134, 139, 228, 160, fTexWidth, fTexHeight );
	m_btnModify.CopyUV( UBS_IDLE, UBS_ON );
	m_btnModify.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Page buttons
	for( i = 0; i < 10; i++ )
	{
		m_btnPage[i].Create( this, CTString( "10" ), 0, 0, 0, 0 );
		m_btnPage[i].SetTextColor( UBS_IDLE, 0x9EB19AFF );
		m_btnPage[i].SetTextColor( UBS_ON, 0xFFD13CFF );
		m_btnPage[i].SetTextColor( UBS_CLICK, 0xFFD13CFF );
		m_btnPage[i].SetTextColor( UBS_DISABLE, 0xFFFFCAFF );
	}

	// List box of list type
	m_lbListContent.Create( this, 15, 75, 532, 174, 18, 0, 9, 5, TRUE );
	m_lbListContent.SetOverColor( 0xFFD13CFF );
	m_lbListContent.SetSelectColor( 0xFFD13CFF );
	m_lbListContent.SetColumnPosX( 0, 30, TEXT_CENTER );						// No
	m_lbListContent.SetColumnPosX( 1, 60 );									// Subject
	m_lbListContent.SetColumnPosX( 2, 345, TEXT_CENTER );						// Name
	m_lbListContent.SetColumnPosX( 3, 425, TEXT_CENTER );						// Date
	m_lbListContent.SetColumnPosX( 4, 504, TEXT_CENTER );						// Hit

	// List box of read type
	m_lbReadContent.Create( this, 24, 112, 537, 137, _pUIFontTexMgr->GetLineHeight()+5, 3, 3, 1, FALSE );
	m_lbReadContent.CreateScroll( TRUE, 0, 0, 9, 137, 9, 7, 0, 0, 10 );
	// Up button
	m_lbReadContent.SetScrollUpUV( UBS_IDLE, 230, 16, 239, 23, fTexWidth, fTexHeight );
	m_lbReadContent.SetScrollUpUV( UBS_CLICK, 240, 16, 249, 23, fTexWidth, fTexHeight );
	m_lbReadContent.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbReadContent.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_lbReadContent.SetScrollDownUV( UBS_IDLE, 230, 24, 239, 31, fTexWidth, fTexHeight );
	m_lbReadContent.SetScrollDownUV( UBS_CLICK, 240, 24, 249, 31, fTexWidth, fTexHeight );
	m_lbReadContent.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbReadContent.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbReadContent.SetScrollBarTopUV( 219, 16, 228, 26, fTexWidth, fTexHeight );
	m_lbReadContent.SetScrollBarMiddleUV( 219, 27, 228, 29, fTexWidth, fTexHeight );
	m_lbReadContent.SetScrollBarBottomUV( 219, 30, 228, 40, fTexWidth, fTexHeight );

	// Search combobox
	m_cmbSearch.Create( this, 150, 271, 77, 15, 63, 1, 13, 13,
						2, _pUIFontTexMgr->GetFontHeight() + 4, 4, 4 );
	m_cmbSearch.SetBackUV( 54, 162, 72, 177, fTexWidth, fTexHeight );
	m_cmbSearch.SetDownBtnUV( 230, 162, 242, 172, fTexWidth, fTexHeight );
	m_cmbSearch.SetUpBtnUV( 242, 172, 230, 162, fTexWidth, fTexHeight );	
	m_cmbSearch.SetDropListUV( 54, 162, 70, 176, fTexWidth, fTexHeight );
	m_cmbSearch.AddString( _S( 341, "제목" ) );
	m_cmbSearch.AddString( _S( 244, "작성자" ) );
	m_cmbSearch.SetCurSel( 0 );

	// Search edit box
	m_ebSearch.Create( this, 233, 271, 145, 16, 32 );
	m_ebSearch.SetReadingWindowUV( 55, 162, 71, 177, fTexWidth, fTexHeight );
	m_ebSearch.SetCandidateUV( 55,162,71,177, fTexWidth, fTexHeight );

	// Subject edit box in writing
	m_ebWriteSubject.Create( this, 112, 80, 375, 16, 80 );
	m_ebWriteSubject.SetReadingWindowUV( 153, 98, 170, 114, fTexWidth, fTexHeight );
	m_ebWriteSubject.SetCandidateUV( 153, 98, 170, 114, fTexWidth, fTexHeight );

	m_mebContent.Create ( this, 111, 117, 444, 128 , 9 );
	
	// -----------------------------------------------------------<<

	// 길드 공지 입력--------------------------------------------->>
	m_ebNoticeTitle.Create( this, 84, 75, 470, 16, MAX_NOTICE_TITLE );	// 제목 
	m_ebNoticeTitle.SetReadingWindowUV( 146, 46, 163, 62, fTexWidth, fTexHeight );
	m_ebNoticeTitle.SetCandidateUV( 146, 46, 163, 62, fTexWidth, fTexHeight );
	m_mebNoticeContent.Create ( this, 82, 115, 465, 135, 10 );	
	// 공지 버튼
	m_btnNotice.Create( this, _S(3842, "공지하기" ), 80, 261, 100, 21 );
	m_btnNotice.SetUV( UBS_IDLE, 134, 117, 228, 138, fTexWidth, fTexHeight );
	m_btnNotice.SetUV( UBS_CLICK, 134, 139, 228, 160, fTexWidth, fTexHeight );
	m_btnNotice.CopyUV( UBS_IDLE, UBS_ON );
	m_btnNotice.CopyUV( UBS_IDLE, UBS_DISABLE );
	// 수정 버튼
	m_btnNoticeCorrect.Create( this, _S(3843,  "수정하기" ) , 340, 261 , 100, 21 );	
	m_btnNoticeCorrect.SetUV( UBS_IDLE, 134, 117, 228, 138, fTexWidth, fTexHeight );
	m_btnNoticeCorrect.SetUV( UBS_CLICK, 134, 139, 228, 160, fTexWidth, fTexHeight );
	m_btnNoticeCorrect.CopyUV( UBS_IDLE, UBS_ON );
	m_btnNoticeCorrect.CopyUV( UBS_IDLE, UBS_DISABLE );
	// 작성완료 버튼
	m_btnUpdateNotice.Create( this, _S(3844, "작성완료" ) , 454, 261 , 100, 21 );	
	m_btnUpdateNotice.SetUV( UBS_IDLE, 134, 117, 228, 138, fTexWidth, fTexHeight );
	m_btnUpdateNotice.SetUV( UBS_CLICK, 134, 139, 228, 160, fTexWidth, fTexHeight );
	m_btnUpdateNotice.CopyUV( UBS_IDLE, UBS_ON );
	m_btnUpdateNotice.CopyUV( UBS_IDLE, UBS_DISABLE );
	
	// -----------------------------------------------------------<<

	// 길드 관리 ------------------------------------------------->>
	// Buttons
	// Change Boss button
	m_btnChangeBossNew.Create( this, _S( 871, "길드장 이임" ), 20, 265, 100, 21 );
	m_btnChangeBossNew.SetUV( UBS_IDLE, 134, 117, 228, 138, fTexWidth, fTexHeight );
	m_btnChangeBossNew.SetUV( UBS_CLICK, 134, 139, 228, 160, fTexWidth, fTexHeight );
	m_btnChangeBossNew.CopyUV( UBS_IDLE, UBS_ON );
	m_btnChangeBossNew.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Accept button
	m_btnAcceptNew.Create( this, _S( 872, "길드부장 임명" ), 138, 265, 100, 21 );		
	m_btnAcceptNew.SetUV( UBS_IDLE, 134, 117, 228, 138, fTexWidth, fTexHeight );
	m_btnAcceptNew.SetUV( UBS_CLICK, 134, 139, 228, 160, fTexWidth, fTexHeight );
	m_btnAcceptNew.CopyUV( UBS_IDLE, UBS_ON );
	m_btnAcceptNew.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Reject button
	m_btnRejectNew.Create( this, _S( 873, "길드부장 해임" ), 254, 265, 100, 21 );		
	m_btnRejectNew.SetUV( UBS_IDLE, 134, 117, 228, 138, fTexWidth, fTexHeight );
	m_btnRejectNew.SetUV( UBS_CLICK, 134, 139, 228, 160, fTexWidth, fTexHeight );
	m_btnRejectNew.CopyUV( UBS_IDLE, UBS_ON );
	m_btnRejectNew.CopyUV( UBS_IDLE, UBS_DISABLE );

	// MemberQuit button
	m_btnMemberFireNew.Create( this, _S( 874, "길드원 퇴출" ), 372, 265, 100, 21 );	
	m_btnMemberFireNew.SetUV( UBS_IDLE, 134, 117, 228, 138, fTexWidth, fTexHeight );
	m_btnMemberFireNew.SetUV( UBS_CLICK, 134, 139, 228, 160, fTexWidth, fTexHeight );
	m_btnMemberFireNew.CopyUV( UBS_IDLE, UBS_ON );
	m_btnMemberFireNew.CopyUV( UBS_IDLE, UBS_DISABLE );

	// MemberQuit button
	m_btnChangeSetting.Create( this, _S(3845, "설정 변경" ), 488, 265, 100, 21 );	
	m_btnChangeSetting.SetUV( UBS_IDLE, 134, 117, 228, 138, fTexWidth, fTexHeight );
	m_btnChangeSetting.SetUV( UBS_CLICK, 134, 139, 228, 160, fTexWidth, fTexHeight );
	m_btnChangeSetting.CopyUV( UBS_IDLE, UBS_ON );
	m_btnChangeSetting.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Member List
	m_lbManageMemberList.Create( this, 20, 78, 553, 161, _pUIFontTexMgr->GetLineHeight(), 13, 3, 6, TRUE );
	m_lbManageMemberList.CreateScroll( TRUE, 0, 0, 9, 161, 9, 7, 0, 0, 10 );
	m_lbManageMemberList.SetSelBar( 553 , _pUIFontTexMgr->GetLineHeight(), 187, 46, 204, 61, fTexWidth, fTexHeight );
	m_lbManageMemberList.SetOverColor( 0xF8E1B5FF );
	m_lbManageMemberList.SetSelectColor( 0xF8E1B5FF );
	m_lbManageMemberList.SetColumnPosX( 0, 27 ,TEXT_CENTER);
	m_lbManageMemberList.SetColumnPosX( 1, 112 ,TEXT_CENTER);
	m_lbManageMemberList.SetColumnPosX( 2, 177 ,TEXT_CENTER);
	m_lbManageMemberList.SetColumnPosX( 3, 256 ,TEXT_CENTER);
	m_lbManageMemberList.SetColumnPosX( 4, 402 ,TEXT_RIGHT);
	m_lbManageMemberList.SetColumnPosX( 5, 515 ,TEXT_RIGHT);
	// Up button
	m_lbManageMemberList.SetScrollUpUV( UBS_IDLE, 230, 16, 239, 23, fTexWidth, fTexHeight );
	m_lbManageMemberList.SetScrollUpUV( UBS_CLICK, 240, 16, 249, 23, fTexWidth, fTexHeight );
	m_lbManageMemberList.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbManageMemberList.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_lbManageMemberList.SetScrollDownUV( UBS_IDLE, 230, 24, 239, 31, fTexWidth, fTexHeight );
	m_lbManageMemberList.SetScrollDownUV( UBS_CLICK, 240, 24, 249, 31, fTexWidth, fTexHeight );
	m_lbManageMemberList.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbManageMemberList.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbManageMemberList.SetScrollBarTopUV( 219, 16, 228, 26, fTexWidth, fTexHeight );
	m_lbManageMemberList.SetScrollBarMiddleUV( 219, 27, 228, 29, fTexWidth, fTexHeight );
	m_lbManageMemberList.SetScrollBarBottomUV( 219, 30, 228, 40, fTexWidth, fTexHeight );	

	// Setting Popup Window	
	m_btnApplySetting.Create( this, _S(3846, "적용" ), 220, 218, 63, 21 );
	m_btnApplySetting.SetUV( UBS_IDLE, 134, 117, 228, 138, fTexWidth, fTexHeight );
	m_btnApplySetting.SetUV( UBS_CLICK, 134, 139, 228, 160, fTexWidth, fTexHeight );
	m_btnApplySetting.CopyUV( UBS_IDLE, UBS_ON );
	m_btnApplySetting.CopyUV( UBS_IDLE, UBS_DISABLE );
	
	m_btnApplySettingClose.Create( this, _S(870, "닫기" ), 295, 218, 63, 21 );
	m_btnApplySettingClose.SetUV( UBS_IDLE, 134, 117, 228, 138, fTexWidth, fTexHeight );
	m_btnApplySettingClose.SetUV( UBS_CLICK, 134, 139, 228, 160, fTexWidth, fTexHeight );
	m_btnApplySettingClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnApplySettingClose.CopyUV( UBS_IDLE, UBS_DISABLE );
	
	m_ebChangePositionName.Create( this, 220, 100, 140, 16, MAX_POSITION_NAME );
	m_ebChangePositionName.SetReadingWindowUV( 146, 46, 163, 62, fTexWidth, fTexHeight );
	m_ebChangePositionName.SetCandidateUV( 146, 46, 163, 62, fTexWidth, fTexHeight );
		
	m_ebChangePayExp.Create( this, 220, 142, 140, 16, MAX_PERCENT_LENGTH );
	m_ebChangePayExp.SetReadingWindowUV( 146, 46, 163, 62, fTexWidth, fTexHeight );
	m_ebChangePayExp.SetCandidateUV( 146, 46, 163, 62, fTexWidth, fTexHeight );
	
	m_ebChangePayFame.Create( this, 220, 184, 140, 16, MAX_PERCENT_LENGTH );
	m_ebChangePayFame.SetReadingWindowUV( 146, 46, 163, 62, fTexWidth, fTexHeight );
	m_ebChangePayFame.SetCandidateUV( 146, 46, 163, 62, fTexWidth, fTexHeight );

	// -----------------------------------------------------------<<
}

// ----------------------------------------------------------------------------
// WSS_NEW_GUILD_SYSTEM 070703
// Name : RenderNewGuildManagePopup()
// Desc : 길드 설정 수정
// ----------------------------------------------------------------------------
void CUIGuild::RenderNewGuildManagePopup()
{
	int nX = m_nPosX;
	int nY = m_nPosY;

	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();

	// Set skill learn texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	m_bxBox1.SetBoxPos(WRect(0,0,NEW_GUILD_SYSTEM_WIDTH,NEW_GUILD_SYSTEM_HEIGHT));
	m_bxBox1.Render(nX,nY);

	m_bxBackGroundBox.SetBoxPos(WRect(203,55,377,77));
	m_bxBackGroundBox.Render(nX,nY);
	m_bxBackGroundBox2.SetBoxPos(WRect(203,77,377,250));
	m_bxBackGroundBox2.Render(nX,nY);
	m_bxBox1.SetBoxPos(WRect(218,98,361,120));
	m_bxBox1.Render(nX,nY);
	m_bxBox1.SetBoxPos(WRect(218,140,361,162));
	m_bxBox1.Render(nX,nY);
	m_bxBox1.SetBoxPos(WRect(218,182,361,204));
	m_bxBox1.Render(nX,nY);

	m_btnApplySetting.Render();
	m_btnApplySettingClose.Render();

	m_ebChangePositionName.Render();
	m_ebChangePayExp.Render();
	m_ebChangePayFame.Render();

	// Text
	_pUIMgr->GetDrawPort()->PutTextEx( _S(3845, "설정 변경" ), nX + 220 , nY + 60);
	_pUIMgr->GetDrawPort()->PutTextEx( _S(3847, "직위명 설정" ), nX + 220 , nY + 81);
	_pUIMgr->GetDrawPort()->PutTextEx( _S(3848, "경험치 상납설정" ), nX + 220 , nY + 123);
	_pUIMgr->GetDrawPort()->PutTextEx( _S(3849, "명성치 상납설정" ), nX + 220 , nY + 165);
	_pUIMgr->GetDrawPort()->PutTextEx( CTString( "%" ), nX + 345 , nY + 143);
	_pUIMgr->GetDrawPort()->PutTextEx( CTString( "%" ), nX + 345 , nY + 185);

}

// ----------------------------------------------------------------------------
// WSS_NEW_GUILD_SYSTEM 070703
// Name : RenderNewGuildManage()
// Desc : 길드 시스템 개편 수정 인터페이스
// ----------------------------------------------------------------------------
void CUIGuild::RenderNewGuildManage(int nX,int nY)
{
	if(_pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_BOSS )
	{
		if(!m_btnChangeBossNew.IsEnabled())
		{
			m_btnChangeBossNew.SetEnable(TRUE);	// 단장 이임
			m_btnAcceptNew.SetEnable(TRUE);		// 부단장 임명
			m_btnRejectNew.SetEnable(TRUE);		// 부단장 해임
			m_btnMemberFireNew.SetEnable(TRUE);	// 멤버 퇴출
			m_btnChangeSetting.SetEnable(TRUE);	// 설정 변경		
		}
	}
	else if (_pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_VICE_BOSS)
	{
		if (m_btnChangeBossNew.IsEnabled())
		{
			m_btnChangeBossNew.SetEnable(FALSE);		// 단장 이임
			m_btnAcceptNew.SetEnable(FALSE);			// 부단장 임명
			m_btnRejectNew.SetEnable(FALSE);			// 부단장 해임
			m_btnMemberFireNew.SetEnable(TRUE);			// 멤버 퇴출
			m_btnChangeSetting.SetEnable(FALSE);		// 설정 변경
		}
	}
	else 
	{
		if(m_btnChangeBossNew.IsEnabled())
		{
			m_btnChangeBossNew.SetEnable(FALSE);		// 단장 이임
			m_btnAcceptNew.SetEnable(FALSE);			// 부단장 임명
			m_btnRejectNew.SetEnable(FALSE);			// 부단장 해임
			m_btnMemberFireNew.SetEnable(FALSE);		// 멤버 퇴출
			m_btnChangeSetting.SetEnable(FALSE);		// 설정 변경
		}
	}

	// Button
	m_btnChangeBossNew.Render();		// 단장 이임
	m_btnAcceptNew.Render();			// 부단장 임명
	m_btnRejectNew.Render();			// 부단장 해임
	m_btnMemberFireNew.Render();		// 멤버 퇴출
	m_btnChangeSetting.Render();		// 설정 변경	

	// Box
	m_bxBox2.SetBoxPos(WRect(nX+20,nY+56,nX+583,nY+239));
	m_bxBox2.Render();	

	// List
	m_lbManageMemberList.Render();		// Guild Member List

	// Line
	_pUIMgr->GetDrawPort()->AddTexture(nX+21,nY+74,nX+582,nY+75,
	m_uvLineH.U0,m_uvLineH.V0,m_uvLineH.U1,m_uvLineH.V1,0xFFFFFFFF);	

	_pUIMgr->GetDrawPort()->AddTexture(nX+98,nY+74,nX+99,nY+239,
	m_uvLineV.U0,m_uvLineV.V0,m_uvLineV.U1,m_uvLineV.V1,0xFFFFFFFF);
	_pUIMgr->GetDrawPort()->AddTexture(nX+186,nY+74,nX+187,nY+239,
	m_uvLineV.U0,m_uvLineV.V0,m_uvLineV.U1,m_uvLineV.V1,0xFFFFFFFF);	
	_pUIMgr->GetDrawPort()->AddTexture(nX+231,nY+74,nX+232,nY+239,
	m_uvLineV.U0,m_uvLineV.V0,m_uvLineV.U1,m_uvLineV.V1,0xFFFFFFFF);
	_pUIMgr->GetDrawPort()->AddTexture(nX+341,nY+74,nX+342,nY+239,
	m_uvLineV.U0,m_uvLineV.V0,m_uvLineV.U1,m_uvLineV.V1,0xFFFFFFFF);	
	_pUIMgr->GetDrawPort()->AddTexture(nX+457,nY+74,nX+458,nY+239,
	m_uvLineV.U0,m_uvLineV.V0,m_uvLineV.U1,m_uvLineV.V1,0xFFFFFFFF);		

	// Text
	_pUIMgr->GetDrawPort()->PutTextExCX( _S(3850, "서열" ), nX + 58 , nY +60);
	_pUIMgr->GetDrawPort()->PutTextExCX( _S(3847, "직위명 설정" ), nX +143, nY +60);
	_pUIMgr->GetDrawPort()->PutTextExCX( _S(3851, "Lv." ), nX +210, nY +60);
	_pUIMgr->GetDrawPort()->PutTextExCX( _S(3105, "캐릭터명" ), nX +286 , nY +60);
	_pUIMgr->GetDrawPort()->PutTextExCX( _S(3848, "경험치 상납설정" ), nX +400, nY +60);
	_pUIMgr->GetDrawPort()->PutTextExCX( _S(3849, "명성치 상납설정" ), nX +519, nY +60);		

	if(m_bApplySettingOn)
		RenderNewGuildManagePopup();
}

// ----------------------------------------------------------------------------
// WSS_NEW_GUILD_SYSTEM 070702
// Name : RenderNewGuildNoticeInput()
// Desc : 길드 시스템 개편 수정 인터페이스
// ----------------------------------------------------------------------------
void CUIGuild::RenderNewGuildNoticeInput()
{
	// Add render regions
	int	nX, nY;
	
	nX = m_nPosX;
	nY = m_nPosY;

	if(_pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_BOSS)
	{
		if(!m_btnNotice.IsEnabled())
		{
			m_btnNotice.SetEnable(TRUE);
			m_btnNoticeCorrect.SetEnable(TRUE);
			m_btnUpdateNotice.SetEnable(TRUE);		
		}		
		if (m_bEnableCorrect)
		{
			if( !m_ebNoticeTitle.IsEnabled() )
			{
				m_ebNoticeTitle.SetEnable(TRUE);
				m_mebNoticeContent.SetEnable(TRUE);
			}
		}
		else 
		{
			if( m_ebNoticeTitle.IsEnabled() )
			{
				m_ebNoticeTitle.SetEnable(FALSE);
				m_mebNoticeContent.SetEnable(FALSE);
			}
		}		
	}
	else 
	{
		if(m_btnNotice.IsEnabled())
		{
			m_btnNotice.SetEnable(FALSE);
			m_btnNoticeCorrect.SetEnable(FALSE);
			m_btnUpdateNotice.SetEnable(FALSE);
			m_ebNoticeTitle.SetEnable(FALSE);
			m_mebNoticeContent.SetEnable(FALSE);
		}
	}
	m_btnNotice.Render();						// 공지 버튼
	m_btnNoticeCorrect.Render();				// 수정 버튼
	m_btnUpdateNotice.Render();					// 작성완료 버튼	

	m_bxBox2.SetBoxPos(WRect(80,72,553,100));
	m_bxBox2.Render(nX,nY);
	m_bxBox2.SetBoxPos(WRect(80,111,553,249));
	m_bxBox2.Render(nX,nY);	

		// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();


	m_ebNoticeTitle.Render();					// 제목	
	m_mebNoticeContent.Render();				// 내용
	
	_pUIMgr->GetDrawPort()->PutTextExCX( _S(341, "제목" ), nX + 43 , nY + 78);
	_pUIMgr->GetDrawPort()->PutTextExCX( _S(3852, "내용" ), nX + 43 , nY + 171);	
}


// ----------------------------------------------------------------------------
// WSS_NEW_GUILD_SYSTEM 070608
// Name : RenderNewGuildSkill()
// Desc : 길드 시스템 개편 수정 인터페이스
// ----------------------------------------------------------------------------
void CUIGuild::RenderNewGuildSkill(int nX,int nY)
{
	// Box 2 (Black)
	// Box Left
	m_bxBox2.SetBoxPos(WRect(nX+21,nY+59,nX+317,nY+277));
	m_bxBox2.Render();	
	// Box Right
	m_bxBox2.SetBoxPos(WRect(nX+337,nY+59,nX+577,nY+223));
	m_bxBox2.Render();	
	// Box 1 (Grey)
	int tInc = 33;
	int tCab = 7;
	int nY2 = nY + 70;
	m_bxBox1.SetBoxPos(WRect(nX+31,nY2 ,nX+307,(nY2+tInc)));
	m_bxBox1.Render();
	m_bxBox1.SetBoxPos(WRect(nX+32,nY2 ,nX+66,(nY2+tInc)));
	m_bxBox1.Render();
	nY2+=tInc+tCab;
	m_bxBox1.SetBoxPos(WRect(nX+31,nY2,nX+307,(nY2+tInc)));
	m_bxBox1.Render();	
	m_bxBox1.SetBoxPos(WRect(nX+32,nY2,nX+66,(nY2+tInc)));
	m_bxBox1.Render();	
	nY2+=tInc+tCab;
	m_bxBox1.SetBoxPos(WRect(nX+31,nY2,nX+307,(nY2+tInc)));
	m_bxBox1.Render();	
	m_bxBox1.SetBoxPos(WRect(nX+32,nY2,nX+66,(nY2+tInc)));
	m_bxBox1.Render();	
	nY2+=tInc+tCab;
	m_bxBox1.SetBoxPos(WRect(nX+31,nY2,nX+307,(nY2+tInc)));
	m_bxBox1.Render();	
	m_bxBox1.SetBoxPos(WRect(nX+32,nY2,nX+66,(nY2+tInc)));
	m_bxBox1.Render();	
	nY2+=tInc+tCab;
	m_bxBox1.SetBoxPos(WRect(nX+31,nY2,nX+307,(nY2+tInc)));
	m_bxBox1.Render();	
	m_bxBox1.SetBoxPos(WRect(nX+32,nY2,nX+66,(nY2+tInc)));
	m_bxBox1.Render();	
	m_bxBox1.SetBoxPos(WRect(nX+338,nY+77,nX+577,nY+153));
	m_bxBox1.Render();	

	// Line
	_pUIMgr->GetDrawPort()->AddTexture(nX+339,nY+76,nX+577,nY+77,
	m_uvLineH.U0,m_uvLineH.V0,m_uvLineH.U1,m_uvLineH.V1,0xFFFFFFFF);		

	// Guild Skill List ----------------------------------------------------->>
	int iListCnt = m_arGuildSkillList.Count();
	// Left Guild Skill List		
	int iRowS = m_sbGuildSkillBar.GetScrollPos();
	int iRowE = iRowS + GUILD_SKILL_SLOT_ROW;
	int tLv,tList,iIdx,iNeed;
	BOOL isLevelMax;
	CTString tStr;
	iRowE = (iRowE>iListCnt)? iListCnt:iRowE;
	nY2 = 0;
	for(int  iRow = iRowS; iRow < iRowE; iRow++, nY2 += GUILD_SKILL_SLOT_OFFSETY )
	{
		// Render Buttons
		ASSERT( !m_btnGuildSkill[iRow].IsEmpty() );
		m_btnGuildSkill[iRow].SetPos( 33, nY2+70 );
		m_btnGuildSkill[iRow].Render();
		// Render Text		
		// Left Skill Desc
		int tLv = m_arGuildSkillList[iRow].GetCurLevel();
		tStr.PrintF("%s Lv%d/Lv%d",
			CTString(m_arGuildSkillList[iRow].GetName()),
			tLv,m_arGuildSkillList[iRow].GetMaxLevel()
			);
		_pUIMgr->GetDrawPort()->PutTextEx( tStr, nX + 75 , nY +nY2+80,C_lYELLOW|CT_AMASK);
		isLevelMax = (m_arGuildSkillList[iRow].GetCurLevel()<m_arGuildSkillList[iRow].GetMaxLevel())?FALSE:TRUE;
		if(!isLevelMax)
		{			
			tStr.PrintF("GP %d",m_arGuildSkillList[iRow].GetLearnGP(tLv));
			_pUIMgr->GetDrawPort()->PutTextExRX( tStr, nX + 303 , nY +nY2+80);
		}
		
	}
	_pUIMgr->GetDrawPort()->PutTextEx( _S(3853, "필요 레벨"), nX+358 , nY+155);
	_pUIMgr->GetDrawPort()->PutTextEx( _S(3854, "필요 길드포인트"), nX+358 , nY+170);
	_pUIMgr->GetDrawPort()->PutTextEx( _S(1181, "필요 나스"), nX+358 , nY+185);
	nY2 = 15;
	tList = m_sbGuildSkillBar.GetScrollPos()+m_iGuildSkillPos;
	isLevelMax = (m_arGuildSkillList[tList].GetCurLevel()<m_arGuildSkillList[tList].GetMaxLevel())?FALSE:TRUE;
	if(m_bIsSelList&&!isLevelMax)
	{	
		nY2 =0;
		// Right Next Level Info		
		tLv = m_arGuildSkillList[tList].GetCurLevel();
		tStr.PrintF("%s Lv %d",m_arGuildSkillList[tList].GetName(),tLv);
		_pUIMgr->GetDrawPort()->PutTextEx( tStr, nX + 348 , nY +61,C_lYELLOW|CT_AMASK);					
		// Render skill description
		m_lbGuildSkillDesc.Render();	
		tStr.PrintF("%d Lv",m_arGuildSkillList[tList].GetLearnLevel(tLv));
		_pUIMgr->GetDrawPort()->PutTextExRX( tStr, nX + 560 , nY + 155);
		tStr.PrintF("%d",m_arGuildSkillList[tList].GetLearnGP(tLv),tLv);
		_pUIMgr->GetDrawPort()->PutTextExRX( tStr, nX + 560 , nY + 170);
				
		for( iNeed = 0 ;iNeed<3 ;iNeed++)
		{
			iIdx = m_arGuildSkillList[tList].GetLearnItemIndex(tLv,iNeed);
			if(iIdx == 19) // 나스 아이템 인덱스
			{
				nY2+=15;
				tStr.PrintF("%d",m_arGuildSkillList[tList].GetLearnItemCount(tLv,iNeed));				
				_pUIMgr->GetDrawPort()->PutTextExRX( tStr , nX + 560 , nY +nY2+ 170);
				
			}			
		}
	}
	_pUIMgr->GetDrawPort()->PutTextEx( _S(3855, "필요 길드스킬"), nX + 358 , nY+ nY2+185);
	if(m_bIsSelList&&!isLevelMax)
	{
		for( iNeed = 0 ;iNeed<3 ;iNeed++)
		{
			iIdx = m_arGuildSkillList[tList].GetLearnSkillIndex(tLv,iNeed);
			if(iIdx>0)
			{	
				CSkill tSkill = _pNetwork->GetSkillData(iIdx);				
				tStr.PrintF("%s %d",tSkill.GetName(),m_arGuildSkillList[tList].GetLearnSkillLevel(tLv,iNeed));			
				_pUIMgr->GetDrawPort()->PutTextExRX( tStr , nX + 560 , nY +nY2+ 200);
				nY+=15;
			}
		}
	}
	// ------------------------------------------------------------------------<<
	
	// Button
	if( _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_BOSS)
	{
		if(!m_btnGetSkill.IsEnabled())
			m_btnGetSkill.SetEnable(TRUE);		
	}
	else 
	{
		if(m_btnGetSkill.IsEnabled())
			m_btnGetSkill.SetEnable(FALSE);
	}
	m_btnGetSkill.Render();

	// ScrollBar
	m_sbGuildSkillBar.Render();
	
	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();
	// Flush Skill Button Rendering Queue
	_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_SKILL );
	
}

// ----------------------------------------------------------------------------
// WSS_NEW_GUILD_SYSTEM 070719
// Name : RenderNewGuildBoardWrite()
// Desc : 길드 시스템 개편 수정 인터페이스
// ----------------------------------------------------------------------------
void CUIGuild::RenderNewGuildBoardWrite()
{
	// Rener Box
	m_bxBox3.SetBoxPos(WRect(25,54,98,71));
	m_bxBox3.Render(m_nPosX,m_nPosY);
	m_bxBox3.SetBoxPos(WRect(25,78,98,96));
	m_bxBox3.Render(m_nPosX,m_nPosY);
	m_bxBox3.SetBoxPos(WRect(25,115,98,245));
	m_bxBox3.Render(m_nPosX,m_nPosY);

	m_bxBox1.SetBoxPos(WRect(109,54,557,71));
	m_bxBox1.Render(m_nPosX,m_nPosY);
	m_bxBox1.SetBoxPos(WRect(109,78,557,96));
	m_bxBox1.Render(m_nPosX,m_nPosY);
	m_bxBox1.SetBoxPos(WRect(109,115,557,245));
	m_bxBox1.Render(m_nPosX,m_nPosY);

	// Text in write type
	_pUIMgr->GetDrawPort()->PutTextEx ( _pNetwork->MyCharacterInfo.name, m_nPosX + 112,
										m_nPosY + 55, 0xFFFFCAFF ); 
	
	_pUIMgr->GetDrawPort()->PutTextEx( _S( 244, "작성자" ), m_nPosX+43,
										m_nPosY + 57, 0xFFFFCAFF );
	_pUIMgr->GetDrawPort()->PutTextEx( _S( 195, "제  목" ), m_nPosX+43,
										m_nPosY + 81, 0xFFFFCAFF );
	_pUIMgr->GetDrawPort()->PutTextEx( _S( 418, "내  용" ), m_nPosX+43,
										m_nPosY + 120, 0xFFFFCAFF );

	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	m_mebContent.Render ();

	// Set web board texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	// List button
	m_btnList.Render();

	// Subject edit box
	m_ebWriteSubject.Render();

	if( pWEB->m_nCurBoardType == UWT_MODIFY )
	{
		// Modify button
		m_btnModify.Render();
	}
	else
	{
		// Write button
		m_btnWrite.Render();
	}
	
	if ( m_ebWriteSubject.DoesShowReadingWindow () )
	{
		// Set web board texture
		_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

		// Reading window
		m_ebWriteSubject.RenderReadingWindow();

		// Render all elements
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();

		// Flush all render text queue
		_pUIMgr->GetDrawPort()->EndTextEx();
	}
}

// ----------------------------------------------------------------------------
// WSS_NEW_GUILD_SYSTEM 070719
// Name : RenderNewGuildBoardRead()
// Desc : 길드 시스템 개편 수정 인터페이스
// ----------------------------------------------------------------------------
void CUIGuild::RenderNewGuildBoardRead()
{
	// Rener Box
	m_bxBox3.SetBoxPos(WRect(22,52,554,73));
	m_bxBox3.Render(m_nPosX,m_nPosY);

	m_bxBox1.SetBoxPos(WRect(21,90,557,247));
	m_bxBox1.Render(m_nPosX,m_nPosY);

	// Render Buttons
	m_btnList.Render();
	m_btnReply.Render();
	m_btnDelete.Render();
	m_btnModify.Render();
	
	// List box
	GetBoardReadContent();
	m_lbReadContent.Render();

	// Text in read type
	_pUIMgr->GetDrawPort()->PutTextEx( pWEB->m_strReadNo, m_nPosX + 35,
											m_nPosY + 55, 0xFFFFCAFF );
	_pUIMgr->GetDrawPort()->PutTextEx( pWEB->m_strReadSubject, m_nPosX + 102,
											m_nPosY + 55, 0xFFFFCAFF );
	_pUIMgr->GetDrawPort()->PutTextExRX( pWEB->m_strReadName, m_nPosX + 546,
											m_nPosY + 55, 0xFFFFCAFF );

	_pUIMgr->GetDrawPort()->PutTextEx( pWEB->m_strReadDate, m_nPosX + 401,
											m_nPosY + 93, 0xFFFFCAFF );
	_pUIMgr->GetDrawPort()->PutTextExRX( pWEB->m_strReadHit, m_nPosX + 548,
											m_nPosY + 93, 0xFFFFCAFF );

}


// ----------------------------------------------------------------------------
// WSS_NEW_GUILD_SYSTEM 070719
// Name : RenderNewGuildBoardList()
// Desc : 길드 시스템 개편 수정 인터페이스
// ----------------------------------------------------------------------------
void CUIGuild::RenderNewGuildBoardList()
{
	m_bxBox3.SetBoxPos(WRect(22,52,554,73));
	m_bxBox3.Render(m_nPosX,m_nPosY);

	// Page buttons
	for( int iPage = 0; iPage < pWEB->m_nValidPageBtnCount; iPage++ )
	{		
		m_btnPage[iPage].SetText(pWEB->m_btnPage[iPage].GetText());
		m_btnPage[iPage].SetPos(pWEB->m_btnPage[iPage].GetPosX(),251);
		m_btnPage[iPage].Render();	
	}

	// Prev button
	if( pWEB->m_nCurrentFirstPage > 1 )
	{
		m_btnPrev.SetPos(pWEB->m_btnPrev.GetPosX(),253);
		m_btnPrev.Render();
	}

	// Next button
	if( pWEB->m_nCurrentLastPage < pWEB->m_nTotalPage )
	{
		m_btnNext.SetPos(pWEB->m_btnNext.GetPosX(),253);
		m_btnNext.Render();
	}

	// Text in list type
	_pUIMgr->GetDrawPort()->PutTextEx( CTString( "No" ), m_nPosX + 38,m_nPosY + 57, 0xFFFFCAFF );
	_pUIMgr->GetDrawPort()->PutTextEx( _S( 341, "제목" ), m_nPosX + 184,m_nPosY + 57, 0xFFFFCAFF );
	_pUIMgr->GetDrawPort()->PutTextEx( _S( 244, "작성자" ), m_nPosX + 341,m_nPosY + 57, 0xFFFFCAFF );
	_pUIMgr->GetDrawPort()->PutTextEx( _S( 398, "작성일" ), m_nPosX + 420,m_nPosY + 57, 0xFFFFCAFF );
	_pUIMgr->GetDrawPort()->PutTextEx( _S( 399, "조회" ), m_nPosX + 504,m_nPosY + 57, 0xFFFFCAFF );

	// List box	
	GetBoardListContent();
	m_lbListContent.Render();

	// Search button
	m_btnSearch.Render();

	// Search combobox
	m_cmbSearch.Render();

	// Search edit box
	m_ebSearch.Render();	
	m_bxBox4.SetBoxPos(WRect(233,271,378,287));
	m_bxBox4.Render(m_nPosX,m_nPosY);

	// Write button
	m_btnWrite.Render();
}

// ----------------------------------------------------------------------------
// WSS_NEW_GUILD_SYSTEM 070719
// Name : RenderNewGuildBoard()
// Desc : 길드 시스템 개편 수정 인터페이스
// ----------------------------------------------------------------------------
void CUIGuild::RenderNewGuildBoard(int nx,int ny)
{
	if(!pWEB->DelayCommandExecute()) return;
	switch (pWEB->m_nCurBoardType)
	{
		
		case UWT_LIST:
			RenderNewGuildBoardList();
			break;
		case UWT_READ:
			RenderNewGuildBoardRead();
			break;
		case UWT_WRITE:
		case UWT_MODIFY:
		case UWT_REPLY:
			RenderNewGuildBoardWrite();
			break;

	}

}

// ----------------------------------------------------------------------------
// WSS_NEW_GUILD_SYSTEM 070608
// Name : RenderNewGuildMemberInfo()
// Desc : 길드 시스템 개편 수정 인터페이스
// ----------------------------------------------------------------------------
void CUIGuild::RenderNewGuildMemberInfo(int nX,int nY)
{
	// Box
	m_bxBox2.SetBoxPos(WRect(nX+20,nY+56,nX+583,nY+239));
	m_bxBox2.Render();	

	// List
	m_lbGuildMemberList.Render();		// Guild Member List

	// Line
	_pUIMgr->GetDrawPort()->AddTexture(nX+21,nY+74,nX+582,nY+75,
	m_uvLineH.U0,m_uvLineH.V0,m_uvLineH.U1,m_uvLineH.V1,0xFFFFFFFF);	

	_pUIMgr->GetDrawPort()->AddTexture(nX+47,nY+74,nX+48,nY+239,
	m_uvLineV.U0,m_uvLineV.V0,m_uvLineV.U1,m_uvLineV.V1,0xFFFFFFFF);
	_pUIMgr->GetDrawPort()->AddTexture(nX+146,nY+74,nX+147,nY+239,
	m_uvLineV.U0,m_uvLineV.V0,m_uvLineV.U1,m_uvLineV.V1,0xFFFFFFFF);	
	_pUIMgr->GetDrawPort()->AddTexture(nX+221,nY+74,nX+222,nY+239,
	m_uvLineV.U0,m_uvLineV.V0,m_uvLineV.U1,m_uvLineV.V1,0xFFFFFFFF);
	_pUIMgr->GetDrawPort()->AddTexture(nX+261,nY+74,nX+262,nY+239,
	m_uvLineV.U0,m_uvLineV.V0,m_uvLineV.U1,m_uvLineV.V1,0xFFFFFFFF);	
	_pUIMgr->GetDrawPort()->AddTexture(nX+343,nY+74,nX+344,nY+239,
	m_uvLineV.U0,m_uvLineV.V0,m_uvLineV.U1,m_uvLineV.V1,0xFFFFFFFF);	
	_pUIMgr->GetDrawPort()->AddTexture(nX+429,nY+74,nX+430,nY+239,
	m_uvLineV.U0,m_uvLineV.V0,m_uvLineV.U1,m_uvLineV.V1,0xFFFFFFFF);	
	_pUIMgr->GetDrawPort()->AddTexture(nX+476,nY+74,nX+477,nY+239,
	m_uvLineV.U0,m_uvLineV.V0,m_uvLineV.U1,m_uvLineV.V1,0xFFFFFFFF);		

	// Text
	_pUIMgr->GetDrawPort()->PutTextExCX( _S(3105, "캐릭터 명" ), nX + 92 , nY +60);
	_pUIMgr->GetDrawPort()->PutTextExCX( _S(3856, "직위 명" ), nX +184, nY +60);
	_pUIMgr->GetDrawPort()->PutTextExCX( _S(3851, "Lv" ), nX +240, nY +60);
	_pUIMgr->GetDrawPort()->PutTextExCX( _S(72, "클래스" ), nX +300 , nY +60);
	_pUIMgr->GetDrawPort()->PutTextExCX( _S(3857, "접속위치" ), nX +386, nY +60);
	_pUIMgr->GetDrawPort()->PutTextExCX( _S(3858, "기여도" ), nX +453, nY +60);
	_pUIMgr->GetDrawPort()->PutTextExCX( _S(3859, "누적 포인트" ), nX +529, nY+60);
 
//	_pUIMgr->GetDrawPort()->PutTextEx( _s( "길드원 관리" ), nX +21, nY+245);
	_pUIMgr->GetDrawPort()->PutTextExRX( _S(3860, "접속한 길드원" ), nX +581, nY+245);
	CTString tStr;
	tStr.PrintF("%d/%d",m_iOnlineMembers,m_iNumOfMember);
	_pUIMgr->GetDrawPort()->PutTextExRX( tStr.str_String,nX+581,nY+259);


		
}
// ----------------------------------------------------------------------------
// WSS_NEW_GUILD_SYSTEM 070608
// Name : RenderNew()
// Desc : 길드 시스템 개편 수정 인터페이스
// ----------------------------------------------------------------------------
void CUIGuild::RenderNewGuildInfo(int nX,int nY)
{
	// Left Top
	m_bxBox2.SetBoxPos(WRect(nX+21,nY+54,nX+282,nY+218));
	m_bxBox2.Render();

	// Left Bottom
	m_bxBox2.SetBoxPos(WRect(nX+21,nY+224,nX+282,nY+286));
	m_bxBox2.Render();

	// Right Top
	m_bxBox2.SetBoxPos(WRect(nX+287,nY+54,nX+582,nY+121));
	m_bxBox2.Render();

	// Right Bottom1
	m_bxBox2.SetBoxPos(WRect(nX+287,nY+128,nX+432,nY+285));
	m_bxBox2.Render();

	// Right Bottom2
	m_bxBox2.SetBoxPos(WRect(nX+437,nY+128,nX+582,nY+285));
	m_bxBox2.Render();

	// Horizon Line
	_pUIMgr->GetDrawPort()->AddTexture(nX+22,nY+244,nX+281,nY+245,
		m_uvLineH.U0,m_uvLineH.V0,m_uvLineH.U1,m_uvLineH.V1,0xFFFFFFFF);
	_pUIMgr->GetDrawPort()->AddTexture(nX+288,nY+75,nX+581,nY+76,
		m_uvLineH.U0,m_uvLineH.V0,m_uvLineH.U1,m_uvLineH.V1,0xFFFFFFFF);
	_pUIMgr->GetDrawPort()->AddTexture(nX+288,nY+149,nX+431,nY+150,
		m_uvLineH.U0,m_uvLineH.V0,m_uvLineH.U1,m_uvLineH.V1,0xFFFFFFFF);
	_pUIMgr->GetDrawPort()->AddTexture(nX+438,nY+149,nX+581,nY+150,
		m_uvLineH.U0,m_uvLineH.V0,m_uvLineH.U1,m_uvLineH.V1,0xFFFFFFFF);

	// Render List
	m_lbMemberAllianceList.Render();
	m_lbMemberHostilityList.Render();

	// Buttons
	m_btnEdit.Render();

	// Guild Mark
	m_bxBox1.SetBoxPos(WRect(nX+26,nY+249,nX+59,nY+282));
	m_bxBox1.Render();

//	m_btnGuildMarkBack.Render();
	
	// Text 
	int nX2 = m_nPosX + 30;
	int nY2 = m_nPosY + 50;
	int nSpace = _pUIFontTexMgr->GetFontHeight() + 10;
	int nInc   = nSpace;
	CTString tStr;
	
	_pUIMgr->GetDrawPort()->PutTextEx( _S(3451, "길드명" ), nX2 , nY2 + nSpace);
	_pUIMgr->GetDrawPort()->PutTextEx( m_strGuildName, nX2 + 135 , nY2 + nSpace);

	_pUIMgr->GetDrawPort()->PutTextEx( _S(3861, "길드 레벨" ), nX2 , nY2 + (nSpace+=nInc));
	tStr.PrintF("%d",m_iGuildLevel);
	_pUIMgr->GetDrawPort()->PutTextEx( tStr, nX2 + 135 , nY2 + nSpace);

	_pUIMgr->GetDrawPort()->PutTextEx( _S(891, "길드장" ), nX2 , nY2 + (nSpace+=nInc));
	_pUIMgr->GetDrawPort()->PutTextEx( m_strBossName, nX2 + 135 , nY2 + nSpace);

	_pUIMgr->GetDrawPort()->PutTextEx( _S(3862, "길드원 인원" ),nX2 , nY2 + (nSpace+=nInc));
	tStr.PrintF("%d/%d",m_iNumOfMember,m_iNumOfMaxMember);
	_pUIMgr->GetDrawPort()->PutTextEx( tStr, nX2 + 135 , nY2 + nSpace);

	_pUIMgr->GetDrawPort()->PutTextEx( _S(3863, "길드 평균레벨" ),nX2 , nY2 + (nSpace+=nInc));
	tStr.PrintF("%d",m_iGuildAverageLevel);
	_pUIMgr->GetDrawPort()->PutTextEx( tStr, nX2 + 135 , nY2 + nSpace);

	_pUIMgr->GetDrawPort()->PutTextEx( _S(3864, "소유 영지" ),nX2 , nY2 + (nSpace+=nInc));
	if(m_iGuildOwnLand>0)
		tStr.PrintF("%d",m_iGuildOwnLand);
	else 
		tStr=_S(3865, "없음");
	_pUIMgr->GetDrawPort()->PutTextEx( tStr, nX2 + 135 , nY2 + nSpace);

	_pUIMgr->GetDrawPort()->PutTextEx( _S(3866, "길드 마크" ),nX +28 , nY + 228);	
	_pUIMgr->GetDrawPort()->PutTextExCX( _S(3867,"[사용기간 : 1년]" ),nX + 137 , nY + 259);
	_pUIMgr->GetDrawPort()->PutTextEx( _S(3447, "길드 포인트(GP)" ),nX +294 , nY + 58);

	_pUIMgr->GetDrawPort()->PutTextEx( _S(3868, "총 포인트" ),nX +304 , nY + 81);
	tStr.PrintF("%d",m_iGuildTotalPoint);
	_pUIMgr->GetDrawPort()->PutTextEx( tStr, nX + 500 , nY + 81);
	_pUIMgr->GetDrawPort()->PutTextEx( _S(3869, "기여 포인트" ),nX +304 , nY + 100);	
	tStr.PrintF("%d",m_iGuildMyPoint);
	_pUIMgr->GetDrawPort()->PutTextEx( tStr, nX + 500 , nY + 100);
	
	_pUIMgr->GetDrawPort()->PutTextEx( _S(3870, "동맹 군단" ),nX +294 , nY + 132, COLOR(0x064729FF));	
	_pUIMgr->GetDrawPort()->PutTextEx( _S(3871, "적대 군단" ),nX +446 , nY + 132, COLOR(0x531D2AFF));		


}

// ----------------------------------------------------------------------------
// WSS_NEW_GUILD_SYSTEM 070608
// Name : RenderNew()
// Desc : 길드 시스템 개편 수정 인터페이스
// ----------------------------------------------------------------------------
void CUIGuild::RenderNew()
{
	// Set skill learn texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	int	nX, nY;
	
	nX = m_nPosX;
	nY = m_nPosY;

	// Background
	m_bxBackGroundBox.SetBoxPos(WRect(nX,nY,nX+NEW_GUILD_SYSTEM_WIDTH,nY+22));
	m_bxBackGroundBox.Render();
	m_bxBackGroundBox2.SetBoxPos(WRect(nX,nY+22,nX+NEW_GUILD_SYSTEM_WIDTH,nY+NEW_GUILD_SYSTEM_HEIGHT));
	m_bxBackGroundBox2.Render();
	// Tab
	
	for(int i=0;i<MAX_GUILDINFO_TAB;i++)
	{		
		if( m_iSelTab != i)
		{
			m_bxBox2.SetBoxPos(WRect(nX+m_rcTabNew[i].Left,nY+m_rcTabNew[i].Top,
							nX+m_rcTabNew[i].Right,nY+m_rcTabNew[i].Bottom));
			m_bxBox2.Render();
		}		
	}
	int tCabL=5 ,tCabR=5;
	if(m_iSelTab==0) tCabL = 0; else tCabL =5; 
	if(m_iSelTab==(MAX_GUILDINFO_TAB-1)) tCabR = 0; else tCabR =5;
	m_bxBox1_1.SetBoxPos(WRect(nX+m_rcTabNew[m_iSelTab].Left-tCabL,nY+m_rcTabNew[m_iSelTab].Top-3,
							nX+m_rcTabNew[m_iSelTab].Right+tCabR,nY+m_rcTabNew[m_iSelTab].Bottom));
	m_bxBox1_1.Render();

	m_bxBox1_2.SetBoxPos(WRect(nX+9,nY+47,nX+595,nY+295));
	m_bxBox1_2.Render();
	
	// Buttons
	m_btnExitNew.Render();
	m_btnCloseNew.Render();
		
	// Render Guild Info
	switch(m_iSelTab)
	{
		case NEW_GUILD_INFO:
			RenderNewGuildInfo(nX,nY);
			break;
		case NEW_GUILD_MEMBER_INFO:
			RenderNewGuildMemberInfo(nX,nY);
			break;
		case NEW_GUILD_SKILL:
			RenderNewGuildSkill(nX,nY);
			break;
#ifndef LOCAL_NEW_GUILD // 해외 로컬 웹보드사용 안함
		case NEW_GUILD_BOARD:
			RenderNewGuildBoard(nX,nY);
			break;
#endif
		case NEW_GUILD_NOTICE:
			RenderNewGuildNoticeInput();
			break;
		case NEW_GUILD_MANAGE:
			RenderNewGuildManage(nX,nY);
			break;
	}

	// Text	
	_pUIMgr->GetDrawPort()->PutTextEx( m_strGuildName, m_nPosX + 19, m_nPosY + 6);
	_pUIMgr->GetDrawPort()->PutTextExCX( _S( 3872, "길드 정보" ), 
		m_nPosX + m_rcTabNew[NEW_GUILD_INFO].Left + m_rcTabNew[0].GetWidth()/2, m_nPosY + m_rcTabNew[NEW_GUILD_INFO].Top+3);
	_pUIMgr->GetDrawPort()->PutTextExCX( _S( 3873, "길드원 정보" ), 
		m_nPosX + m_rcTabNew[NEW_GUILD_MEMBER_INFO].Left + m_rcTabNew[0].GetWidth()/2, m_nPosY + m_rcTabNew[NEW_GUILD_MEMBER_INFO].Top+3);
	_pUIMgr->GetDrawPort()->PutTextExCX( _S( 3874, "길드 스킬" ), 
		m_nPosX + m_rcTabNew[NEW_GUILD_SKILL].Left + m_rcTabNew[0].GetWidth()/2, m_nPosY + m_rcTabNew[NEW_GUILD_SKILL].Top+3);
#ifndef LOCAL_NEW_GUILD // 해외 로컬 웹보드사용 안함
	_pUIMgr->GetDrawPort()->PutTextExCX( _S( 3875, "길드 게시판" ), 
		m_nPosX + m_rcTabNew[NEW_GUILD_BOARD].Left + m_rcTabNew[0].GetWidth()/2, m_nPosY + m_rcTabNew[NEW_GUILD_BOARD].Top+3);
#endif
	_pUIMgr->GetDrawPort()->PutTextExCX( _S( 3876, "길드 공지사항" ), 
		m_nPosX + m_rcTabNew[NEW_GUILD_NOTICE].Left + m_rcTabNew[0].GetWidth()/2, m_nPosY + m_rcTabNew[NEW_GUILD_NOTICE].Top+3);
	_pUIMgr->GetDrawPort()->PutTextExCX( _S( 918, "길드 관리" ), 
		m_nPosX + m_rcTabNew[NEW_GUILD_MANAGE].Left + m_rcTabNew[0].GetWidth()/2, m_nPosY + m_rcTabNew[NEW_GUILD_MANAGE].Top+3);
	
	
	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();
}



// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT	CUIGuild::MouseMessageNewBoard( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;	
	
	// Mouse message
	switch( pMsg->message )
	{
		case WM_MOUSEMOVE:
			{
				// Guild Web Board
			
				// List
				if( pWEB->m_nCurBoardType == UWT_LIST )
				{
					// Search combobox
					if( m_cmbSearch.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Search button
					else if( m_btnSearch.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Write button
					else if( m_btnWrite.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// List box
					else if( m_lbListContent.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Prev button
					else if( m_btnPrev.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Next button
					else if( m_btnNext.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Page buttons
					else
					{
						for( int iPage = 0; iPage < pWEB->m_nValidPageBtnCount; iPage++ )
						{
							if( m_btnPage[iPage].MouseMessage( pMsg ) != WMSG_FAIL )
								return WMSG_SUCCESS;
						}
					}
				}
				// Read
				else if( pWEB->m_nCurBoardType == UWT_READ )
				{
					// List button
					if( m_btnList.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Reply button
					else if( m_btnReply.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Modify button
					else if( m_btnModify.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Delete button
					else if( m_btnDelete.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// List box of read type
					else if( m_lbReadContent.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
				// Write
				else if( pWEB->m_nCurBoardType == UWT_WRITE )
				{
					// List button
					if( m_btnList.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Write button
					else if( m_btnWrite.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					else if ( m_ebWriteSubject.MouseMessage ( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					else if ( m_mebContent.MouseMessage ( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
				// Reply
				else if( pWEB->m_nCurBoardType == UWT_REPLY )
				{
					// List button
					if( m_btnList.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Write button
					else if( m_btnWrite.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					else if ( m_ebWriteSubject.MouseMessage ( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					else if ( m_mebContent.MouseMessage ( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
				// Modify
				else if( pWEB->m_nCurBoardType == UWT_MODIFY )
				{
					// List button
					if( m_btnList.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Modify button
					else if( m_btnModify.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					else if ( m_ebWriteSubject.MouseMessage ( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					else if ( m_mebContent.MouseMessage ( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
		
			}
			break;
		case WM_LBUTTONDOWN:
			{
				// List
				if( pWEB->m_nCurBoardType == UWT_LIST )
				{
					// Search combobox
					if( m_cmbSearch.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						// Nothing
					}
					// Search edit box
					else if( m_ebSearch.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						// Nothing
					}
					// Search button
					else if( m_btnSearch.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						// Nothing
					}
					// Write button
					else if( m_btnWrite.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						// Nothing
					}
					// List box
					else if( ( wmsgResult = m_lbListContent.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							// 공지 내용 요청..
							pWEB->m_nWantWrite = m_lbListContent.GetCurSel();
							pWEB->DelayCommandPrepare(new CCommandView);							
						}
					}
					// Prev button
					else if( m_btnPrev.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						// Nothing
					}
					// Next button
					else if( m_btnNext.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						// Nothing
					}
					// Page buttons
					else
					{
						for( int iPage = 0; iPage < pWEB->m_nValidPageBtnCount; iPage++ )
						{
							if( m_btnPage[iPage].MouseMessage( pMsg ) != WMSG_FAIL )
								break;
						}
					}
				}
				// Read
				else if( pWEB->m_nCurBoardType == UWT_READ )
				{
					// List button
					if( m_btnList.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						// Nothing
					}
					// Reply button
					else if( m_btnReply.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						// Nohthing
					}
					// Modify button
					else if( m_btnModify.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						// Nohthing
					}
					// Delete button
					else if( m_btnDelete.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						// Nohthing
					}
					// List box of read type
					else if( m_lbReadContent.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						// Nothing
					}
				}
				// Write
				else if( pWEB->m_nCurBoardType == UWT_WRITE )
				{
					// List button
					if( m_btnList.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						// Nohthing
					}
					// Write button
					else if( m_btnWrite.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						
					}
					else if ( m_ebWriteSubject.MouseMessage ( pMsg ) != WMSG_FAIL )
					{
						m_mebContent.SetFocus ( FALSE );
					//	return WMSG_SUCCESS;
					}
					// 이기환 수정 시작 (05.01.01) : 멀티 에디트 박스 루틴 추가
					else if ( m_mebContent.MouseMessage ( pMsg ) != WMSG_FAIL )
					{
						m_ebWriteSubject.SetFocus( FALSE );
					//	return WMSG_SUCCESS;
					}
				}
				// Reply
				else if( pWEB->m_nCurBoardType == UWT_REPLY )
				{
					// List button
					if( m_btnList.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						// Nohthing
					}
					// Write button
					else if( m_btnWrite.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						// Nohthing
					}
					else if ( m_ebWriteSubject.MouseMessage ( pMsg ) != WMSG_FAIL )
					{
						m_mebContent.SetFocus ( FALSE );
					//	return WMSG_SUCCESS;
					}
					// 이기환 수정 시작 (05.01.01) : 멀티 에디트 박스 루틴 추가
					else if ( m_mebContent.MouseMessage ( pMsg ) != WMSG_FAIL )
					{
						m_ebWriteSubject.SetFocus( FALSE );
					//	return WMSG_SUCCESS;
					}
				}
				// Modify
				else if( pWEB->m_nCurBoardType == UWT_MODIFY )
				{
					// List button
					if( m_btnList.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						// Nohthing
					}
					// Modify button
					else if( m_btnModify.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						// Nohthing
					}
					else if ( m_ebWriteSubject.MouseMessage ( pMsg ) != WMSG_FAIL )
					{
						m_mebContent.SetFocus ( FALSE );
					//	return WMSG_SUCCESS;
					}
					// 이기환 수정 시작 (05.01.01) : 멀티 에디트 박스 루틴 추가
					else if ( m_mebContent.MouseMessage ( pMsg ) != WMSG_FAIL )
					{
						m_ebWriteSubject.SetFocus( FALSE );
					//	return WMSG_SUCCESS;
					}
				}
			}		
			break;
		case WM_LBUTTONUP:
			{
				// List
				if( pWEB->m_nCurBoardType == UWT_LIST )
				{
					// List box
					if( m_lbListContent.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Search button
					else if( ( wmsgResult = m_btnSearch.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							if( m_ebSearch.GetString() )
							{
								pWEB->m_cmbSearch.SetCurSel(m_cmbSearch.GetCurSel());
								pWEB->m_ebSearch.SetString(m_ebSearch.GetString());
								pWEB->m_nWantPage = 1;
								pWEB->DelayCommandPrepare(new CCommandList);								
							}
						}
						return WMSG_SUCCESS;
					}
					// Write button
					else if( ( wmsgResult = m_btnWrite.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							// user name input ...
							// 글쓰기 요청
							pWEB->m_nCurBoardType = UWT_WRITE;
							pWEB->m_ebWriteSubject.ResetString();
							//!!TODO:내용도 리셋.
							m_ebWriteSubject.SetFocus ( TRUE );
							m_mebContent.ResetString ();
							m_mebContent.SetFocus ( FALSE );

						}
						return WMSG_SUCCESS;
					}
					// Prev button
					else if( ( wmsgResult = m_btnPrev.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							// 페이지 재설정
							pWEB->m_nWantPage = pWEB->m_nCurrentPage - 10;
							if(pWEB->m_nWantPage < 1) pWEB->m_nWantPage = 1;
							pWEB->DelayCommandPrepare(new CCommandList);
						}
						return WMSG_SUCCESS;
					}
					// Next button
					else if( ( wmsgResult = m_btnNext.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							// 페이지 재설정
							pWEB->m_nWantPage = pWEB->m_nCurrentPage + 10;
							if(pWEB->m_nWantPage > pWEB->m_nTotalPage) pWEB->m_nWantPage = pWEB->m_nTotalPage;
							pWEB->DelayCommandPrepare(new CCommandList);
						}
						return WMSG_SUCCESS;
					}
					// Page buttons
					else
					{
						for( int iPage = 0; iPage < pWEB->m_nValidPageBtnCount; iPage++ )
						{
							if( ( wmsgResult = m_btnPage[iPage].MouseMessage( pMsg ) ) != WMSG_FAIL )
							{
								if( wmsgResult == WMSG_COMMAND )
								{
									// 페이지 변경 처리
									pWEB->m_nWantPage = pWEB->m_nCurrentFirstPage + iPage;
									pWEB->DelayCommandPrepare(new CCommandList);									
								}
								return WMSG_SUCCESS;
							}
						}
					}
				}
				// Read
				else if( pWEB->m_nCurBoardType == UWT_READ )
				{
					// List button
					if( ( wmsgResult = m_btnList.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							// 목록 요청							
							pWEB->DelayCommandPrepare(new CCommandList);							
							pWEB->m_nCurBoardType = UWT_LIST;
							pWEB->m_nWantPage = pWEB->m_nCurrentPage;							
						}
						return WMSG_SUCCESS;
					}
					// Reply button
					else if( ( wmsgResult = m_btnReply.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							// 답글 페이지로
							pWEB->m_nCurBoardType = UWT_REPLY;
							CTString strTemp;
							strTemp.PrintF("Re: %s", pWEB->m_strReadSubject);
							m_ebWriteSubject.ResetString();
							m_ebWriteSubject.InsertChars(0, strTemp.str_String);
							//멀티에디터에 내용 삽입. 내용이 아래와 같이 수정됨.
							//공백라인
							//----<피답글의 작성자>이 쓰신 글입니다 -----
							//모든 줄 앞에 '>' 삽입.
								
							CTString strContent;
							
							for ( int i = 0; i < m_lbReadContent.GetCurItemCount(0); i++ )
							{
								strContent += m_lbReadContent.GetString ( 0, i );
								strContent += "\r\n";
							}

							m_ebWriteSubject.SetFocus ( FALSE );
							m_mebContent.SetFocus ( TRUE );
							m_mebContent.SetString ( strContent.str_String, pWEB->m_strReadName.str_String, ">" );
						}
						return WMSG_SUCCESS;
					}
					// Modify button
					else if( ( wmsgResult = m_btnModify.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							// 수정 페이지로
							pWEB->m_nCurBoardType = UWT_MODIFY;
							m_ebWriteSubject.ResetString();
							m_ebWriteSubject.InsertChars(0, pWEB->m_strReadSubject.str_String);

							//멀티에디터에 내용 삽입.
							m_mebContent.ResetString();
							
							CTString strContent;
							for ( int i = 0; i < m_lbReadContent.GetCurItemCount(0); i++ )
							{
								strContent += m_lbReadContent.GetString ( 0, i );
								strContent += "\r\n";
							}
							m_ebWriteSubject.SetFocus ( FALSE );	
							m_mebContent.SetFocus ( TRUE );	
							m_mebContent.SetString ( strContent.str_String );

						}
						return WMSG_SUCCESS;
					}
					// Delete button
					else if( ( wmsgResult = m_btnDelete.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							// 삭제 요청
							//TODO : 예의상 한번 물어보자.
							pWEB->m_nWantModifyMode = UWM_DELETE;
							pWEB->DelayCommandPrepare(new CCommandModify);
						}
						return WMSG_SUCCESS;
					}
					// List box of read type
					else if( m_lbReadContent.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
				// Write
				else if( pWEB->m_nCurBoardType == UWT_WRITE )
				{
					SetFocus ( TRUE );

					// List button
					if( ( wmsgResult = m_btnList.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							// 목록 요청							
							pWEB->m_nWantPage = pWEB->m_nCurrentPage;
							pWEB->DelayCommandPrepare(new CCommandList);
						}
						return WMSG_SUCCESS;
					}
					// Write button
					else if( ( wmsgResult = m_btnWrite.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							// 쓰기 요청
							pWEB->m_nWantModifyMode = UWM_WRITE;
							pWEB->m_ebWriteSubject.SetString( m_ebWriteSubject.GetString());
							pWEB->m_mebContent.SetString(m_mebContent.GetString().str_String);
							pWEB->DelayCommandPrepare(new CCommandModify);
						}
						return WMSG_SUCCESS;
					}
				
				}
				// Reply
				else if( pWEB->m_nCurBoardType == UWT_REPLY )
				{
					// List button
					if( ( wmsgResult = m_btnList.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							// 목록 요청
							pWEB->m_nCurBoardType = UWT_LIST;
							pWEB->m_nWantPage = pWEB->m_nCurrentPage;
							pWEB->DelayCommandPrepare(new CCommandList);
						}
						return WMSG_SUCCESS;
					}
					// Write button
					else if( ( wmsgResult = m_btnWrite.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							// 쓰기 요청
							pWEB->m_nWantModifyMode = UWM_REPLY;
							pWEB->m_ebWriteSubject.SetString( m_ebWriteSubject.GetString());
							pWEB->m_mebContent.SetString(m_mebContent.GetString().str_String);
							pWEB->DelayCommandPrepare(new CCommandModify);
						}
						return WMSG_SUCCESS;
					}
					
				}
				// Modify
				else if( pWEB->m_nCurBoardType == UWT_MODIFY )
				{
					// List button
					if( ( wmsgResult = m_btnList.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							// 목록 요청
							pWEB->m_nCurBoardType = UWT_LIST;
							pWEB->m_nWantPage = pWEB->m_nCurrentPage;
							pWEB->DelayCommandPrepare(new CCommandList);
						}
						return WMSG_SUCCESS;
					}
					// Modify button
					else if( ( wmsgResult = m_btnModify.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							// 수정 요청
							pWEB->m_ebWriteSubject.SetString( m_ebWriteSubject.GetString() );	
							pWEB->m_mebContent.SetString( m_mebContent.GetString().str_String );	
							pWEB->m_nWantModifyMode = UWM_MODIFY;
							pWEB->DelayCommandPrepare(new CCommandModify);
						}
						return WMSG_SUCCESS;
					}
				
				}
			}		
			break;
		case WM_MOUSEWHEEL:
			{
				// Read
				if( pWEB->m_nCurBoardType == UWT_READ )
				{
					// List box of read type
					if( m_lbReadContent.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
				else if( pWEB->m_nCurBoardType == UWT_WRITE )
				{
					if( m_mebContent.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
				else if( pWEB->m_nCurBoardType == UWT_REPLY )
				{
					if( m_mebContent.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
				else if( pWEB->m_nCurBoardType == UWT_MODIFY )
				{
					if( m_mebContent.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}

			}
			break;

		case WM_LBUTTONDBLCLK:
		{			
			// Read
			if( pWEB->m_nCurBoardType == UWT_READ )
			{
				// List box of read type
				if( m_lbReadContent.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}					
		}
	break;
	}

		return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT	CUIGuild::MouseMessageNew( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL	bTitleBarClick = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );
	
	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			// Move Window
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;
				
				Move( ndX, ndY );
				
				return WMSG_SUCCESS;
			}

			if( IsInsideRect( nX, nY ,m_rcWindowNew) )
			{
				_pUIMgr->SetMouseCursorInsideUIs();
				
				switch(m_iSelTab)
				{
					case NEW_GUILD_INFO: 
						// List
						m_lbMemberAllianceList.MouseMessage( pMsg );
						m_lbMemberHostilityList.MouseMessage( pMsg );							
						break;
					case NEW_GUILD_MEMBER_INFO:
						m_lbGuildMemberList.MouseMessage( pMsg );
						break;
					case NEW_GUILD_SKILL:
						m_sbGuildSkillBar.MouseMessage( pMsg );
						m_lbGuildSkillDesc.MouseMessage( pMsg );
						break;
#ifndef LOCAL_NEW_GUILD // 해외 로컬 웹보드사용 안함
					case NEW_GUILD_BOARD:
						MouseMessageNewBoard(pMsg);
						break;
#endif
					case NEW_GUILD_NOTICE:	
						m_ebNoticeTitle.MouseMessage( pMsg );
						m_mebNoticeContent.MouseMessage( pMsg );
						break;
					case NEW_GUILD_MANAGE:
						m_lbManageMemberList.MouseMessage( pMsg );
						break; 						
				}	
				return WMSG_SUCCESS;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			if( IsInsideRect( nX, nY ,m_rcWindowNew) )
			{
				nOldX = nX;		nOldY = nY;
				
				if (_pUIMgr->GetGuild()->IsEnabled() && !_pUIMgr->GetGuild()->IsVisible())
				{
					_pUIMgr->RearrangeOrder( UI_GUILD, TRUE );
				}
			
				// Close button
				m_btnCloseNew.MouseMessage( pMsg );
				// 닫기 버튼
				m_btnExitNew.MouseMessage(pMsg);
				// Title bar
				if( IsInsideRect( nX, nY, m_rcTitleNew ) )
				{
					bTitleBarClick = TRUE;					
				}
				
				switch(m_iSelTab)
				{
					case NEW_GUILD_INFO: 
						// Edit button
						m_btnEdit.MouseMessage( pMsg );						
						// List
						m_lbMemberAllianceList.MouseMessage( pMsg );
						m_lbMemberHostilityList.MouseMessage( pMsg );							
						break;
					case NEW_GUILD_MEMBER_INFO:					
						m_lbGuildMemberList.MouseMessage( pMsg );
						break;
					case NEW_GUILD_SKILL:						
						if(m_btnGetSkill.MouseMessage( pMsg )) {}
						else if(m_sbGuildSkillBar.MouseMessage( pMsg )){}
						else if(m_lbGuildSkillDesc.MouseMessage( pMsg )){}
						break;
#ifndef LOCAL_NEW_GUILD // 해외 로컬 웹보드사용 안함
					case NEW_GUILD_BOARD:
						MouseMessageNewBoard(pMsg);
						break;
#endif
					case NEW_GUILD_NOTICE:
						if( m_bEnableCorrect )
						{						
							if( m_ebNoticeTitle.MouseMessage( pMsg ) != WMSG_FAIL )
							{					
								m_ebNoticeTitle.SetFocus(TRUE);
								m_mebNoticeContent.SetFocus(FALSE);
							}
							else if( m_mebNoticeContent.MouseMessage( pMsg ) != WMSG_FAIL )
							{					
								m_ebNoticeTitle.SetFocus(FALSE);
								m_mebNoticeContent.SetFocus(TRUE);
							}
							else 
							{
								m_ebNoticeTitle.SetFocus(FALSE);
								m_mebNoticeContent.SetFocus(FALSE);
							}
						}
						m_btnNotice.MouseMessage( pMsg );		
						m_btnNoticeCorrect.MouseMessage( pMsg );	
						m_btnUpdateNotice.MouseMessage( pMsg );	

						break;
					case NEW_GUILD_MANAGE:
						if(m_bApplySettingOn)
						{
							m_btnApplySetting.MouseMessage( pMsg );
							m_btnApplySettingClose.MouseMessage( pMsg );
							
							if( (wmsgResult = m_ebChangePositionName.MouseMessage( pMsg )) != WMSG_FAIL )
							{
								m_ebChangePositionName.SetFocus(TRUE);
								m_ebChangePayFame.SetFocus(FALSE);
								m_ebChangePayExp.SetFocus(FALSE);
							}
							else if( (wmsgResult = m_ebChangePayExp.MouseMessage( pMsg )) != WMSG_FAIL )
							{
								m_ebChangePositionName.SetFocus(FALSE);
								m_ebChangePayFame.SetFocus(FALSE);
								m_ebChangePayExp.SetFocus(TRUE);
							} 
							else if( (wmsgResult = m_ebChangePayFame.MouseMessage( pMsg )) != WMSG_FAIL )
							{
								m_ebChangePositionName.SetFocus(FALSE);
								m_ebChangePayExp.SetFocus(FALSE);
								m_ebChangePayFame.SetFocus(TRUE);
							} 
							else 
							{
								m_ebChangePositionName.SetFocus(FALSE);
								m_ebChangePayExp.SetFocus(FALSE);
								m_ebChangePayFame.SetFocus(FALSE);
								wmsgResult = WMSG_SUCCESS;				
							}

							_pUIMgr->RearrangeOrder( UI_GUILD, TRUE );
							return wmsgResult;
						}
						else 
						{
							m_btnChangeBossNew.MouseMessage( pMsg );							
							m_btnAcceptNew.MouseMessage( pMsg );							
							m_btnRejectNew.MouseMessage( pMsg );							
							m_btnMemberFireNew.MouseMessage( pMsg );							
							m_btnChangeSetting.MouseMessage( pMsg );
							m_lbManageMemberList.MouseMessage( pMsg );
						}						
						break;
				}
				
				
				_pUIMgr->RearrangeOrder( UI_GUILD, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			bTitleBarClick = FALSE;
			// tab
			if(!m_bApplySettingOn)
			{
				for(int i=0; i<MAX_GUILDINFO_TAB;i++)
				{
					if( IsInsideRect( nX, nY, m_rcTabNew[i] ) )
					{
						if( m_iSelTab != i)
						{
							int tOldTab = m_iSelTab;
							m_iSelTab = i;							
							
							switch(m_iSelTab)
							{
#ifndef LOCAL_NEW_GUILD // 해외 로컬 웹보드사용 안함
								case NEW_GUILD_BOARD:
								{
									// TODO :: 임시로 게시판 막음... 이후 웹팀 작업되면 추가
								//	m_iSelTab = tOldTab;
								//	_pUIMgr->GetChatting()->AddSysMessage( _s( "현재 버전은 지원하지 않습니다." ) );
								//	return WMSG_SUCCESS;

									pWEB->m_nWantPage = 1;
									pWEB->DelayCommandPrepare(new CCommandList);
									return WMSG_SUCCESS;
								}
								break;
#endif
								case NEW_GUILD_NOTICE:
								{
									m_iSelTab = tOldTab;
									m_bEnableCorrect		= FALSE;			// 공지사항 수정여부
									SendRequestGuildTab(NEW_GUILD_NOTICE);
									return WMSG_SUCCESS;
								}
								break;							
								case NEW_GUILD_MANAGE:
								{
									if( _pNetwork->MyCharacterInfo.lGuildPosition != GUILD_MEMBER_BOSS &&
										_pNetwork->MyCharacterInfo.lGuildPosition != GUILD_MEMBER_VICE_BOSS)
									{
										m_iSelTab = tOldTab;
										return WMSG_SUCCESS;
									}
								}
								break;
							}
							
							// 길드 정보 요청
							SendRequestGuildTab(m_iSelTab);
							break;
						}						
					}
				}		
			}

			// Close button
			if( (wmsgResult = m_btnCloseNew.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if(wmsgResult == WMSG_COMMAND)
				{
					ResetGuild();
					InitNewGuildSystem();
				}
				return WMSG_SUCCESS;
			}
			if( (wmsgResult = m_btnExitNew.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if(wmsgResult == WMSG_COMMAND)
				{
					ResetGuild();
					InitNewGuildSystem();
				}
				return WMSG_SUCCESS;
			}			

			switch(m_iSelTab)
			{
				case NEW_GUILD_INFO: 
					{
						// Edit button
						if( (wmsgResult = m_btnEdit.MouseMessage( pMsg )) != WMSG_FAIL)
						{						
							if(wmsgResult == WMSG_COMMAND)
							{
								// TODO : 길드마크 에디트 기획완료후 작업								
							}
							return WMSG_SUCCESS;
						}
						// List
						if( (wmsgResult = m_lbMemberAllianceList.MouseMessage( pMsg )) != WMSG_FAIL )
						{
							return WMSG_SUCCESS;
						}					
					}
					break;
				case NEW_GUILD_MEMBER_INFO:
										
					break;
				case NEW_GUILD_SKILL:
					{
						if(m_arGuildSkillList.Count() == 0) break;
						
						if(IsInsideRect(nX,nY,m_rcGuildSkillList) )
						{
							m_lbGuildSkillDesc.ResetAllStrings();
							
							m_iGuildSkillPos = (nY-(m_nPosY+m_rcGuildSkillList.Top))/40;

							if(m_arGuildSkillList.Count()>m_iGuildSkillPos)
							{
								m_bIsSelList = TRUE;								
							}
							else 
							{
								m_bIsSelList = FALSE;
								m_iGuildSkillPos = 0;
							}

							_pUIMgr->AddStringToList(
								&m_lbGuildSkillDesc,
								CTString(m_arGuildSkillList[m_sbGuildSkillBar.GetScrollPos()+m_iGuildSkillPos].GetDescription()),
								35);
							
						}
						else if ( (wmsgResult=m_btnGetSkill.MouseMessage( pMsg )) != WMSG_FAIL)
						{
							if(wmsgResult == WMSG_COMMAND)
							{
								// TODO : 기술 습득 요청																
								SendLearnGuildSkill(m_arGuildSkillList[m_sbGuildSkillBar.GetScrollPos()+m_iGuildSkillPos].GetIndex());
							}
							return WMSG_SUCCESS;
						}
						else if( (wmsgResult=m_sbGuildSkillBar.MouseMessage( pMsg )) != WMSG_FAIL)
						{
							return WMSG_SUCCESS;
						}
						else if( (wmsgResult=m_lbGuildSkillDesc.MouseMessage( pMsg )) != WMSG_FAIL)
						{
							return WMSG_SUCCESS;
						}					
					}
					break;
#ifndef LOCAL_NEW_GUILD // 해외 로컬 웹보드사용 안함
				case NEW_GUILD_BOARD:
					MouseMessageNewBoard(pMsg);
					break;
#endif
				case NEW_GUILD_NOTICE:
					{
						if ( m_btnNotice.MouseMessage( pMsg ) == WMSG_COMMAND)
						{
							SendRequestGuildNotice();
							return WMSG_SUCCESS;
						}
						else if ( m_btnNoticeCorrect.MouseMessage( pMsg ) == WMSG_COMMAND)
						{
							// TODO :: 수정창 활성화
							m_bEnableCorrect = TRUE;
							m_ebNoticeTitle.SetFocus(FALSE);
							m_mebNoticeContent.SetFocus(TRUE);
							return WMSG_SUCCESS;
						}
						else if ( m_btnUpdateNotice.MouseMessage( pMsg ) == WMSG_COMMAND)
						{
							SendUpdateGuildNotice(m_ebNoticeTitle.GetString(),m_mebNoticeContent.GetString());
							return WMSG_SUCCESS;
						}
					}
					break;
				case NEW_GUILD_MANAGE:
					{
						if(m_bApplySettingOn)
						{							
							if( (wmsgResult = m_btnApplySetting.MouseMessage( pMsg ) ) != WMSG_FAIL )
							{
								if(wmsgResult == WMSG_COMMAND)
								{									
									if( CheckDataValidation() )
									{
										// TODO :: send apply message
										SendAdjustGuildMemberInfo(
											m_vManageMemberIndex[m_lbManageMemberList.GetCurSel()],
											m_ebChangePositionName.GetString(),
											atoi(m_ebChangePayExp.GetString()),
											atoi(m_ebChangePayFame.GetString()));
									}	
									// 비활성화
									SetManagePopup(FALSE);
								}
								return WMSG_SUCCESS;
							}
							if( (wmsgResult = m_btnApplySettingClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
							{
								if(wmsgResult == WMSG_COMMAND)
								{
									// 비활성화
									SetManagePopup(FALSE);
								}
								return WMSG_SUCCESS;
							}						
							if( (wmsgResult = m_ebChangePositionName.MouseMessage( pMsg )) != WMSG_FAIL )
							{
								return WMSG_SUCCESS;							
							}
							if( (wmsgResult = m_ebChangePayExp.MouseMessage( pMsg )) != WMSG_FAIL )
							{
								return WMSG_SUCCESS;							
							}
							if( (wmsgResult = m_ebChangePayFame.MouseMessage( pMsg )) != WMSG_FAIL )
							{
								return WMSG_SUCCESS;							
							}

						}
						else
						{
							// Change Boss(단장 이임)
							if( (wmsgResult = m_btnChangeBossNew.MouseMessage( pMsg ) ) != WMSG_FAIL )
							{
								if(wmsgResult == WMSG_COMMAND)
								{
									ChangeBoss();
								}
								return WMSG_SUCCESS;
							}
							// Accept(부단장 임명)
							else if( (wmsgResult = m_btnAcceptNew.MouseMessage( pMsg ) ) != WMSG_FAIL )
							{
								if(wmsgResult == WMSG_COMMAND)
								{
									AddViceBoss();
								}
								return WMSG_SUCCESS;
							}
							// Reject(부단장 해임)
							else if( (wmsgResult = m_btnRejectNew.MouseMessage( pMsg ) ) != WMSG_FAIL )
							{
								if(wmsgResult == WMSG_COMMAND)
								{
									DelViceBoss();
								}
								return WMSG_SUCCESS;
							}
							// Member fire(단원 퇴출)
							else if( (wmsgResult = m_btnMemberFireNew.MouseMessage( pMsg ) ) != WMSG_FAIL )
							{
								if(wmsgResult == WMSG_COMMAND)
								{
									MemberFire();
								}
								return WMSG_SUCCESS;
							}
							// 설정 변경
							else if( (wmsgResult = m_btnChangeSetting.MouseMessage( pMsg ) ) != WMSG_FAIL )
							{
								// 활성화
								if(m_lbManageMemberList.GetCurSel()>=0)
								{
									SetManagePopup(TRUE);
									ResetManagePopupString();
								}
								return WMSG_SUCCESS;						
							}
						}					
					}
					break;
			}			
		
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInsideRect( nX, nY , m_rcWindowNew ) )
			{
				switch(m_iSelTab)
				{
					case NEW_GUILD_INFO: 
						// List
						m_lbMemberAllianceList.MouseMessage( pMsg );
						m_lbMemberHostilityList.MouseMessage( pMsg );							
						break;
					case NEW_GUILD_MEMBER_INFO:
						m_lbGuildMemberList.MouseMessage( pMsg );
						break;
					case NEW_GUILD_SKILL:
						m_sbGuildSkillBar.MouseMessage( pMsg );						
						break;
#ifndef LOCAL_NEW_GUILD // 해외 로컬 웹보드사용 안함
					case NEW_GUILD_BOARD:
						MouseMessageNewBoard(pMsg);
						break;
#endif
					case NEW_GUILD_NOTICE:
						m_ebNoticeTitle.MouseMessage( pMsg );
						m_mebNoticeContent.MouseMessage( pMsg );
						break;
					case NEW_GUILD_MANAGE:
						m_lbManageMemberList.MouseMessage( pMsg );
						break;
				}
				
				return WMSG_SUCCESS;
			}
		}
		break;
	case WM_LBUTTONDBLCLK :		
	case WM_RBUTTONDBLCLK :
		return WMSG_SUCCESS;
	}
	return WMSG_FAIL;
}

void CUIGuild::SetManagePopup(BOOL bEnable)
{
	m_bApplySettingOn = bEnable;
	m_btnApplySetting.SetEnable(bEnable);		
	m_btnApplySettingClose.SetEnable(bEnable);		
	m_ebChangePositionName.SetEnable(bEnable);
	m_ebChangePayExp.SetEnable(bEnable);		
	m_ebChangePayFame.SetEnable(bEnable);	
	m_btnExitNew.SetEnable(!bEnable);
	m_btnCloseNew.SetEnable(!bEnable);
}

void CUIGuild::ResetManagePopupString()
{
	m_ebChangePositionName.ResetString();
	m_ebChangePayExp.ResetString();
	m_ebChangePayFame.ResetString();
}

BOOL CUIGuild::CheckDataValidation()
{
	CUIMsgBox_Info	MsgBoxInfo;
	int tInt;
	CTString tStr;
	_pUIMgr->CloseMessageBox(MSGCMD_NULL);
	MsgBoxInfo.SetMsgBoxInfo( _S(3877, "입력 오류" ), UMBS_OK,
									UI_NONE,MSGCMD_NULL );

	// 직위명 설정 문자 검사
	tStr = m_ebChangePositionName.GetString();
	if( tStr.Length() == 0 )
	{
		MsgBoxInfo.AddString( _S(3878, "입력된 문자가 없습니다.") );
		_pUIMgr->CreateMessageBox( MsgBoxInfo );		
		return FALSE;
		
	}
	else if( _UIFiltering.Filtering ( tStr.str_String ) == TRUE )
	{		
		CTString	strMessage = _S( 435, "잘못된 문자[" );	
		strMessage += tStr.str_String;
		strMessage += _S( 436, "]가 포함되어 있습니다." );
		
		MsgBoxInfo.AddString( strMessage );
		_pUIMgr->CreateMessageBox( MsgBoxInfo );
		m_ebChangePositionName.ResetString();
		return FALSE;
	}

	// 상납 설정 검사
	tStr = m_ebChangePayExp.GetString();
	if( !tStr.IsInteger() )
	{
		MsgBoxInfo.AddString( _S(3879, "경험치 입력이 잘못 되었습니다.") );
		_pUIMgr->CreateMessageBox( MsgBoxInfo );
		
		return FALSE;
	}

	tInt = atoi(tStr.str_String);
	if( ((tInt > 100 || tInt < 0) && g_iCountry != USA) || 
		((tInt >50 || tInt < 0 ) && g_iCountry == USA))
	{
		if (g_iCountry == USA)
		{
			MsgBoxInfo.AddString( _S(2274, "[0~50]사이 숫자를 입력해 주세요.") );
		}
		else
		{
			MsgBoxInfo.AddString( _S(3880, "[0~100]사이 숫자를 입력해 주세요.") );
		}

		_pUIMgr->CreateMessageBox( MsgBoxInfo );
		
		return FALSE;
	}

	tStr = m_ebChangePayFame.GetString();
	if( !tStr.IsInteger() )
	{
		MsgBoxInfo.AddString( _S(3881, "명성치 입력이 잘못 되었습니다.") );
		_pUIMgr->CreateMessageBox( MsgBoxInfo );		
		return FALSE;
	}

	tInt = atoi(tStr.str_String);
	if(tInt > 100 || tInt < 0)
	{
		MsgBoxInfo.AddString( _S(3880, "[0~100]사이 숫자를 입력해 주세요.") );
		_pUIMgr->CreateMessageBox( MsgBoxInfo );
		
		return FALSE;
	}

	return TRUE;
}

// Message Send -------------------->><<
// 길드 정보 요청 
void CUIGuild::SendRequestGuildTab(int iTabNum)
{
	CNetworkMessage nmGuild(MSG_GUILD);

	switch(iTabNum)
	{
	case NEW_GUILD_INFO :		
		nmGuild << (UBYTE)MSG_NEW_GUILD_INFO;
		break;
	case NEW_GUILD_MEMBER_INFO :
		{
			m_lbGuildMemberList.ResetAllStrings();
			m_iOnlineMembers =0;

			nmGuild << (UBYTE)MSG_NEW_GUILD_MEMBERLIST;			
		}
		break;
	case NEW_GUILD_SKILL :
		nmGuild << (UBYTE)MSG_NEW_GUILD_SKILL;	
		break;
#ifndef LOCAL_NEW_GUILD // 해외 로컬 웹보드사용 안함
	case NEW_GUILD_BOARD :
		// TODO :: 길드 게시판
		break;
#endif
	case NEW_GUILD_NOTICE :
		nmGuild << (UBYTE)MSG_NEW_GUILD_NOTICE;	
		break;
	case NEW_GUILD_MANAGE :
		{
			// Member List Init
			m_lbManageMemberList.ResetAllStrings();
			m_vManageMemberIndex.clear();
			ClearMemberList();

			nmGuild << (UBYTE)MSG_NEW_GUILD_MANAGE;
		}
		break;
	}
	_pNetwork->SendToServerNew(nmGuild);

}

// 길드 성향 변경
void CUIGuild::SendChangeGuildInclination(UBYTE ubIncline)
{	
	CNetworkMessage nmGuild(MSG_GUILD);
	nmGuild << (UBYTE)MSG_NEW_GUILD_INCLINE_ESTABLISH << ubIncline;
	_pNetwork->SendToServerNew(nmGuild);
}

// 길드 정보 수정
void CUIGuild::SendAdjustGuildMemberInfo( int charIdx, CTString posName ,int expPer, int famePer)
{	
	CNetworkMessage nmGuild(MSG_GUILD);
	nmGuild << (UBYTE)MSG_NEW_GUILD_MEMBER_ADJUST
			<< (ULONG)charIdx
			<< posName
			<< (ULONG)expPer
			<< (ULONG)famePer;
	_pNetwork->SendToServerNew(nmGuild);	
}

// 길드 공지 업데이트(길드장)
void CUIGuild::SendUpdateGuildNotice( CTString strTitle, CTString strContents)
{
	CNetworkMessage nmGuild(MSG_GUILD);
	EncodeNoticeString(strContents);
	nmGuild << (UBYTE)MSG_NEW_GUILD_NOTICE_UPDATE
			<< strTitle
			<< strContents;	
	
	_pNetwork->SendToServerNew(nmGuild);
}

//  길드장 공지 요청
void CUIGuild::SendRequestGuildNotice()
{
	CNetworkMessage nmGuild(MSG_GUILD);
	nmGuild << (UBYTE)MSG_NEW_GUILD_NOTICE_REQUEST;
	_pNetwork->SendToServerNew(nmGuild);

}

// 길드 스킬 습득하기
void CUIGuild::SendLearnGuildSkill(int skillIdx) 
{	
	CNetworkMessage nmGuild(MSG_GUILD);
	nmGuild << (UBYTE)MSG_NEW_GUILD_SKILL_LEARN;
	nmGuild << (ULONG)skillIdx;
	_pNetwork->SendToServerNew(nmGuild);
}

// Message Receive----------------->><<
void CUIGuild::ReceiveNewGuildMessage(UBYTE ubType,CNetworkMessage* istr)
{
	switch(ubType)
	{		
		case MSG_NEW_GUILD_INCLINE_ESTABLISH:			
			// TODO : 길드 성향 처리
			break;		
		case MSG_NEW_GUILD_INFO:
			{
				ULONG GuildLevel;
				ULONG NumOfMember;
				ULONG NumOfMaxMember;
				ULONG GuildAverageLevel;
				ULONG GuildTotalPoint;
				ULONG GuildMyPoint;	

				(*istr) >> m_strGuildName
						>> GuildLevel
						>> m_strBossName
						>> NumOfMember
						>> NumOfMaxMember
						>> GuildAverageLevel
						>> GuildTotalPoint
						>> GuildMyPoint;	
				m_iGuildLevel			= (int)GuildLevel;
				m_iNumOfMember			= (int)NumOfMember;
				m_iNumOfMaxMember       = (int)NumOfMaxMember;
				m_iGuildAverageLevel    = (int)GuildAverageLevel;
				m_iGuildTotalPoint  	= (int)GuildTotalPoint;
				m_iGuildMyPoint			= (int)GuildMyPoint;	
			}
			break;
		case MSG_NEW_GUILD_MEMBERLIST:
			{
				clsGuildMemberNew tGuildMemberNew;
				CTString tPosName;
				ULONG ulListCnt;
				ULONG charLevel;
				LONG  zoneIndex;
				ULONG cumulPoint;
				UBYTE iJob;
				UBYTE iJob2;	
				UBYTE bPlaying;
				ULONG ulPosition;	// 캐릭터 직위(BOSS,VICEBOSS.MEMBER)

				//m_lbGuildMemberList.ResetAllStrings();
				//m_iOnlineMembers =0;
								
				(*istr) >> ulListCnt;
				for(int i=0; i<ulListCnt; i++)
				{
					(*istr)	>> tGuildMemberNew.strMemberName
							>> tPosName
							>> charLevel
							>> bPlaying
							>> zoneIndex
							>> cumulPoint
							>> iJob
							>> iJob2
							>> ulPosition;
					if(bPlaying)
						m_iOnlineMembers++;
					tGuildMemberNew.sPosName	= ResetPosName(tPosName,(INT)ulPosition);
					tGuildMemberNew.iLevel		= (INT)charLevel;
					tGuildMemberNew.bOnline		= (BOOL)bPlaying;
					tGuildMemberNew.iLocation	= (INT)zoneIndex;
					tGuildMemberNew.iCumulPoint	= (INT)cumulPoint;
					tGuildMemberNew.iJob		= (INT)iJob;
					tGuildMemberNew.iJob2		= (INT)iJob2;
			
					// TODO :: 제대로 받는 확인... iJob이 비트로 설정할때도 있어서...
					tGuildMemberNew.sJobName	= JobInfo().GetName(iJob,iJob2);					
					if((int)zoneIndex == -1 )
						tGuildMemberNew.sLocName = _S(3882, "미접속");
					else 
						tGuildMemberNew.sLocName = ZoneInfo().GetZoneName( zoneIndex );
					
					AddGuildMemberInfo(tGuildMemberNew);
				}				
			}
			break;
		case MSG_NEW_GUILD_SKILL:
			// TODO :: 이후 스킬툴에 추가 후 작업
			// skillcount(n) skillindex(n) skillLeanLevel(n) skillMaxLevel(n)
			{
				ULONG tLevel;
				ULONG tIndex;
				ULONG tCount;
				(*istr) >> tCount;
				
				if( tCount == 0 )
				{
					for(int i=0; i<m_arGuildSkillList.Count(); ++i)
					{
						m_arGuildSkillList[i].SetCurLevel(0);
					}
				}
				else
				{
					for(int i=0;i<tCount ;i++)
					{
						(*istr) >> tIndex
								>> tLevel;

						for(int j=0; j<m_arGuildSkillList.Count(); ++j)
						{
							if( m_arGuildSkillList[j].GetIndex() == tIndex )
							{
								m_arGuildSkillList[j].SetCurLevel(tLevel);
								break;
							}
						}
					}
				}
			}
			break;
		case MSG_NEW_GUILD_NOTICE:
			{	
				m_iSelTab = NEW_GUILD_NOTICE;
				CTString tStr;
				(*istr) >> tStr;
				m_ebNoticeTitle.SetString(tStr.str_String);
				(*istr) >> tStr;
				DecodeNoticeString(tStr);
				m_mebNoticeContent.SetString(tStr.str_String);
			}
			break;			
		case MSG_NEW_GUILD_NOTICE_TRANSMISSION:
			{
				CTString tStr,tStr2;
				(*istr) >> m_strGuildName;		
				(*istr) >> tStr;				
				(*istr) >> tStr2;			
				DecodeNoticeString(tStr2);
				_pUIMgr->GetGuildNotice()->SetGuildNotice(m_strGuildName,tStr,tStr2);
				_pUIMgr->RearrangeOrder( UI_GUILD_NOTICE, TRUE );
			}
			break;
		case MSG_NEW_GUILD_MANAGE:
			{
				clsGuildMemberNew tMemberInfo;
				BYTE bFirstListMember;
				INDEX tLoop;
				ULONG eRanking;
				ULONG iLevel;
				ULONG iExpServ;
				ULONG iFameServ;
				ULONG icharIdx;
				
				(*istr) >> bFirstListMember;
				(*istr) >> tLoop;
				
				if (bFirstListMember > 0)
				{
					// Member List Init
					m_lbManageMemberList.ResetAllStrings();
					m_vManageMemberIndex.clear();
					ClearMemberList();					
				}
				
				for(int i=0;i<tLoop;i++)
				{
					(*istr) >> eRanking
							>> tMemberInfo.strMemberName
							>> tMemberInfo.sPosName
							>> iLevel
							>> iExpServ
							>> iFameServ
							>> icharIdx;
					tMemberInfo.eRanking	= eRanking;
					tMemberInfo.iLevel		= iLevel;
					tMemberInfo.iExpServ	= iExpServ;
					tMemberInfo.iFameServ	= iFameServ;
					tMemberInfo.sRanking	= ResetPosName( CTString(""), eRanking );
					tMemberInfo.sPosName	= ResetPosName( tMemberInfo.sPosName, eRanking );

					m_vManageMemberIndex.push_back(icharIdx);
					AddGuildManageInfo(tMemberInfo);
					// 길드장 이임,임명,해임,퇴출등 기존 루틴을 사용하기 위해					
					AddToMemberList(icharIdx,tMemberInfo.strMemberName,eRanking,FALSE);
				}
			}			
			break;
		case MSG_NEW_GUILD_SKILL_LEARN:
			{
				ULONG skillIdx;
				UBYTE isNew;
				BYTE  skillLv;
				(*istr) >> skillIdx
						>> isNew
						>> skillLv;	

				CTString tStr;				
				INDEX tCnt = m_arGuildSkillList.Count();
				tStr.PrintF("%d",tCnt);
				for(int i=0;i<tCnt; i++)
				{
					if( m_arGuildSkillList[i].GetIndex() == skillIdx)
					{
						m_arGuildSkillList[i].SetCurLevel((int)skillLv);
						break;
					}
				}				
			}
			break;
		case MSG_NEW_GUILD_POINT_RANKING:
			{
				ULONG charindex;
				ULONG guildindex;
				ULONG rankingv;
				(*istr) >> charindex
						>> guildindex
						>> rankingv;
				if(g_iCountry == MALAYSIA) //[ttos_2009_5_14]: 말레이시아는 포인트 랭킹을 적용 안함
				{
					break;
				}


				if( charindex == _pNetwork->MyCharacterInfo.index)
				{				
					if( _pNetwork->MyCharacterInfo.lGuildIndex == guildindex )
						_pNetwork->MyCharacterInfo.sbGuildRank = rankingv;		
				}
				else
				{
					for( INDEX ipl = 0; ipl < _pNetwork->ga_srvServer.srv_actCha.Count(); ++ipl )
					{
						CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[ipl];
						if( ct.cha_Index == charindex && ct.cha_lGuildIndex == guildindex )
						{						
							ct.cha_sbGuildRank = rankingv;		
							break;
						}
					}
				}
			}
			break;
	}	
}

// 길드 멤버 정보 리스트 추가
void CUIGuild::AddGuildMemberInfo(clsGuildMemberNew tMemberInfo)
{
	CTString tPer;
	if(tMemberInfo.bOnline)
		m_lbGuildMemberList.AddString(0,CTString("!"));
	else
		m_lbGuildMemberList.AddString(0,CTString(" "));

	m_lbGuildMemberList.AddString(1,tMemberInfo.strMemberName);
	m_lbGuildMemberList.AddString(2,tMemberInfo.sPosName);

	//자기 정보면 직위명추가
	if(tMemberInfo.strMemberName.Matches(_pNetwork->MyCharacterInfo.name))
	{
		_pNetwork->MyCharacterInfo.guildPosName = tMemberInfo.sPosName;
	}

	tPer.PrintF("%d",tMemberInfo.iLevel);
	m_lbGuildMemberList.AddString(3,tPer);
	tPer.PrintF("%s",tMemberInfo.sJobName);
	m_lbGuildMemberList.AddString(4,tPer);
	tPer.PrintF("%s",tMemberInfo.sLocName);
	m_lbGuildMemberList.AddString(5,tPer);		
	if(m_iGuildTotalPoint>0)
		tPer.PrintF("%d%",(tMemberInfo.iCumulPoint*100)/m_iGuildTotalPoint);
	else 
		tPer.PrintF("0");
	m_lbGuildMemberList.AddString(6,tPer);				
	tPer.PrintF("%d",tMemberInfo.iCumulPoint);
	m_lbGuildMemberList.AddString(7,tPer);				
}

// 길드 멤버 정보 리스트 추가
void CUIGuild::AddGuildManageInfo(clsGuildMemberNew tMemberInfo)
{
	//자기 정보면 직위명추가
	if(tMemberInfo.strMemberName.Matches(_pNetwork->MyCharacterInfo.name))
	{
		_pNetwork->MyCharacterInfo.guildPosName = tMemberInfo.sPosName;
	}
	CTString tPer;
	tPer.PrintF("%s",tMemberInfo.sRanking);
	m_lbManageMemberList.AddString(0,tPer);	
	m_lbManageMemberList.AddString(1,SetPosName(tMemberInfo.sPosName));
	tPer.PrintF("%d",tMemberInfo.iLevel);
	m_lbManageMemberList.AddString(2,tPer);
	m_lbManageMemberList.AddString(3,tMemberInfo.strMemberName);
	tPer.PrintF("%d %",tMemberInfo.iExpServ);
	m_lbManageMemberList.AddString(4,tPer);
	tPer.PrintF("%d %",tMemberInfo.iFameServ);
	m_lbManageMemberList.AddString(5,tPer);			
}

// ----------------------------------------------------------------------------
// Name : OpenGuildManagerNew()
// Desc : 
// ----------------------------------------------------------------------------
void CUIGuild::OpenGuildManagerNew()
{
	if (IsVisible()) return;

	m_iGuildLevel = _pNetwork->MyCharacterInfo.lGuildLevel;

	// 레벨이 6이하면 출력하지 않는다...	
	if( m_iGuildLevel < LIMIT_GUILD_LEVEL )
	{
		return;
	}
	// 웹게시판 연결
	m_pWebBoard = _pUIMgr->GetWebBoard();

	// 초기화 
	InitNewGuildSystem();	

	// 길드 스킬 리스트.
	SetGuildSkillList();

	// 길드 정보 요청
	SendRequestGuildTab(0);

	// 사이즈및 위치 설정 
	SetSize( NEW_GUILD_SYSTEM_WIDTH, NEW_GUILD_SYSTEM_HEIGHT );
	extern ENGINE_API CDrawPort	*_pdpMain;
	ResetPosition(_pdpMain->dp_MinI,_pdpMain->dp_MinJ,_pdpMain->dp_MaxI,_pdpMain->dp_MaxJ);
	
	m_eGuildState	= GUILD_MANAGER_NEW;

	_pUIMgr->RearrangeOrder( UI_GUILD, TRUE );

}

void CUIGuild::EncodeNoticeString(CTString& tStr)
{
	while(tStr.ReplaceSubstr("\r\n","``"));
}

void CUIGuild::DecodeNoticeString(CTString& tStr)
{
	while(tStr.ReplaceSubstr("``","\r\n"));
}

void CUIGuild::AdjustGuildMemberInfo()
{	
	INDEX tRow = m_lbManageMemberList.GetCurSel();
	m_lbManageMemberList.SetString(1,tRow,CTString(m_ebChangePositionName.GetString()));
	m_lbManageMemberList.SetString(4,tRow,CTString(m_ebChangePayExp.GetString()));
	m_lbManageMemberList.SetString(5,tRow,CTString(m_ebChangePayFame.GetString()));
}

void CUIGuild::SetGuildSkillList()
{
	// Set Skill List
	if( m_arGuildSkillList.Count() > 0 )
		return;	

	INDEX tCnt = _pNetwork->ga_World.wo_aSkillData.Count();							
	INDEX tCnt2 = 0;
	INDEX tLoop = 0;

	// Check Guild Skill Count
	for( tLoop=0;tLoop<tCnt;tLoop++)
	{
		if( _pNetwork->GetSkillData(tLoop).GetType() == CSkill::ST_GUILD_SKILL_PASSIVE)
			{										
				tCnt2++;
			}
	}
	
	// Set Data Size
	m_btnGuildSkill = new(CUIButtonEx[tCnt2]);
	// Guild skill buttons
	for( tLoop = 0; tLoop < tCnt2 ; tLoop++ )
	{
		m_btnGuildSkill[tLoop].Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_GUILD,
											UBET_SKILL, 0, tLoop );		
	}	

	m_arGuildSkillList.New(tCnt2);
	m_sbGuildSkillBar.SetCurItemCount(tCnt2);

	// Set Guild Skill Info
	tCnt2 = 0;
	for( tLoop=0;tLoop<tCnt;tLoop++)
	{
		CSkill	tSkill = _pNetwork->GetSkillData(tLoop);
		if( tSkill.GetType() == CSkill::ST_GUILD_SKILL_PASSIVE)
		{								
			ASSERT(m_arGuildSkillList.Count()>=tCnt2);
			m_btnGuildSkill[tCnt2].SetSkillInfo(tLoop,0);
			m_arGuildSkillList[tCnt2] = tSkill;
			tCnt2++;
		}
	}	
}

void CUIGuild::GetBoardListContent()
{	
	if(m_lbListContent.GetCurItemCount(0)>0)
	{
		if( m_lbListContent.GetString(0,0) == pWEB->m_lbListContent.GetString(0,0) ) return;
	}

	int tColCnt = pWEB->m_lbListContent.GetColumnCount();
	int tItemCnt = pWEB->m_lbListContent.GetCurItemCount(0);
	m_lbListContent.ResetAllStrings();
	for(int j=0;j<tItemCnt;j++)
	{	
		for(int i=0;i<tColCnt;i++)
		{
			m_lbListContent.AddString(i,pWEB->m_lbListContent.GetString(i,j));
		}	
	}
}

void CUIGuild::GetBoardReadContent()
{	
	if(m_lbReadContent.GetCurItemCount(0)>0)
	{
		if( m_lbReadContent.GetString(0,0) == pWEB->m_lbReadContent.GetString(0,0) ) return;
	}
	m_lbReadContent.ResetAllStrings();	
	for(int i=0;i<pWEB->m_lbReadContent.GetCurItemCount(0);i++)
	{
		m_lbReadContent.AddString(0,pWEB->m_lbReadContent.GetString(0,i));
	}		
	
}

int CUIGuild::GetGuildSkillLevel(int skillIdx)
{
	int tLoop = m_arGuildSkillList.Count();
	for(int i=0;i<tLoop;i++)
	{
		if(m_arGuildSkillList[i].GetIndex() == skillIdx)	
		{
			return m_arGuildSkillList[i].GetCurLevel();
		}
	}
	return 0;
}

CTString CUIGuild::SetPosName(CTString tName)
{
	if( tName.Length() )
	{
		return tName;
	}
	else 
	{
		// TODO : 지정된 이름이 없으면 디폴트 직위명을 붙여 준다. 
		return tName;		
	}

}

void CUIGuild::ResetGuildMaxMember(int guildSkillLevel)
{
	// WSS_NEW_GUILD_SYSTEM 070704	
	for(int i= 7;i<MAX_GUILD_LEVEL;i++)
	{
		// 필요나스/sp/gp = 7레벨업시 필요량 * (1.09^(길드LV - 7))
		if (guildSkillLevel < 6)
		{// 길드 스킬 레벨이 6레벨 미만이면 +5씩 최대 멤버 증가
			_GuildConditionTable[i].iMaxMember = _GuildConditionTable[6].iMaxMember + 5*guildSkillLevel;
		}
		else
		{// 길드 스킬 레벨이 6레벨 이상이면 +9씩 최대 멤버 증가
			const int tmpGuildMember = 55;
			_GuildConditionTable[i].iMaxMember = tmpGuildMember + (9*(guildSkillLevel-5));
		}
	}		
}

CTString CUIGuild::ResetPosName(CTString tPosName,int ulPosition)
{
	if( tPosName.Length() == 0 )
	{
		if( ulPosition == GUILD_MEMBER_BOSS)
			return _S(891, "길드장");
		else if( ulPosition == GUILD_MEMBER_VICE_BOSS)
			return _S(892, "길드부장");
		else if( ulPosition == GUILD_MEMBER_MEMBER)
			return _S(893, "길드원");		
	}
	return tPosName;
}

BOOL CUIGuild::IsEditBoxFocused() 
{
	return (m_ebGuildName.IsFocused() ||
	m_ebSearch.IsFocused() ||
	m_cmbSearch.IsFocused() ||
	m_ebWriteSubject.IsFocused() ||
	m_mebContent.IsFocused() ||
	m_ebNoticeTitle.IsFocused() ||
	m_mebNoticeContent.IsFocused() ||
	m_ebChangePositionName.IsFocused() ||
	m_ebChangePayExp.IsFocused() ||
	m_ebChangePayFame.IsFocused());	
}

void CUIGuild::KillFocusEditBox() 
{ 
	m_ebGuildName.SetFocus( FALSE );
	m_ebSearch.SetFocus( FALSE );
	m_cmbSearch.SetFocus( FALSE );
	m_ebWriteSubject.SetFocus( FALSE );
	m_mebContent.SetFocus( FALSE );
	m_ebNoticeTitle.SetFocus( FALSE );
	m_mebNoticeContent.SetFocus( FALSE );
	m_ebChangePositionName.SetFocus( FALSE );
	m_ebChangePayExp.SetFocus( FALSE );
	m_ebChangePayFame.SetFocus( FALSE );
}

// -------------------------------------------------------------------------------------------------------->>

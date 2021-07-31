#include "stdh.h"
#include <vector>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIQuest.h>
#include <Engine/Entities/QuestSystem.h>
#include <Engine/Interface/UISelectWord.h>
#include <Engine/Interface/UISkillLearn.h>
#include <Engine/LocalDefine.h>
#include <Engine/Interface/UIShop.h>
#ifdef HELP_SYSTEM_1
	// [KH_07044] 3차 도움말 관련 추가
	extern INDEX g_iShowHelp1Icon;
#endif

extern INDEX g_iCountry;
extern INDEX g_iTempFlag;
///////////////////////////////////////////////////////////////////////////////////////
//
// g_iTempflag :	< 0x0000**** - ZONE FLAG >				
//	(ps.dta)				
//					0x00000001 - 아자카협곡				0x00000002 - 차원의 문 on/off
//					
//					< 0x****0000 - EVENT FLAG >
//
//					0x00010000 - WEAPONCHANGE_EVENT		0x00020000 - TREASURE_BOX_EVENT
//					0x00040000 - QUEST_HARVEST_MOON_DAY -> Replace Open Event(temporary) 
//					0x00080000 - Christmas Event	
//					0x00100000 - Newyear Event
//					0x00200000 - Coupon Event (JAPAN)
//					0x00400000 - Connect Event (JAPAN)	
//					0x80000000 - EVENT on/off
//
///////////////////////////////////////////////////////////////////////////////////////

//#define PLATINUM_EXCHANGE
//#define RAIN_EVENT
//#define WORLDCUP_ITEM
//#define WORLDCUP_GOLDENBALL
//#define LOVE_LOVE_EVENT				// 2007 발렌타인's 데이

// 월드컵이벤트
#define WORLDCUP_MAX_GROUP 4

// 불교 촛불 이벤트
//#define BUDDHISM_EVENT

// 곤충 채집 이벤트
//#define COLLECT_BUGS_EVENT
#define QUEST_HARVEST_MOON_DAY		// 추석 이벤트 
//#define OPENBETA_EVENTITEM_GIFT // 미국 오픈 베타 아이템 지급( & Brazil )

// 프로모 패키지 이벤트
#define PROMO_PACK

int	iSelCountry =-1;
int iSelGroup =-1;
int iGroup[WORLDCUP_MAX_GROUP+1] ={ 0, 5, 14, 23, 32 };

#define BILL_LOCATION_HOME	 0
#define BILL_LOCATION_PCBANG 1

enum eWeaponChangeSelection
{
	EVENT_CHANGEWEAPON,
};

enum eGateOfDimensionSelection
{
	GOD_ENTER,						// 차원의 문 입장
	GOD_RANKING,					// 차원의 문 순위 확인
	GOD_PRIZE,						// 보상
};
enum e24Event
{
	EXCH_FIVE,						// 맞고 머니 교환
	EXCH_FOUR,						// 포커 머니 교환 
	OPEN_TREA,						// 보물 상자 열기
};

enum eClothesExchangeSelection
{
	COAT_PANTS,
	GLOVE_FOOTWEAR,
	HELMET,
};

enum ePlatiumExchangeSelection
{
	PLATINUM_STONE_EXCHANGE1,
	PLATINUM_STONE_EXCHANGE2,
	PLATINUM_STONE_EXCHANGE3
};

enum eWorldCupExchangeSelection
{
	WORLDCUP_EVENT1_EXCHANGE,
	WORLDCUP_EVENT1_RECEIVE,
	WORLDCUP_EVENT1_EXCHANGE100,
	WORLDCUP_EVENT1_EXCHANGE75,
	WORLDCUP_EVENT1_EXCHANGE50,
	WORLDCUP_EVENT1_EXCHANGE25,
	WORLDCUP_EVENT1_REWARD1,
	WORLDCUP_EVENT1_REWARD2,
	WORLDCUP_EVENT1_REWARD3,
	WORLDCUP_EVENT1_REWARD4,
};

// Date : 2005-03-07,   By Lee Ki-hwan
static int	_iMaxMsgStringChar = 0;

// WSS_GUILDMASTER 070416 ------------------------------------->>
BOOL CheckGuildMasterEvent()
{
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;	
	_pUIMgr->DoesMessageBoxExist(MSGCMD_NULL);
	
/*	if( _pNetwork->MyCharacterInfo.lGuildPosition != GUILD_MEMBER_BOSS && _pNetwork->MyCharacterInfo.lGuildPosition !=GUILD_MEMBER_VICE_BOSS )
	{
		MsgBoxInfo.SetMsgBoxInfo( _S( 100, "이벤트" ), UMBS_OK, UI_NONE, MSGCMD_NULL);		
		strMessage.PrintF( _s( "길드 마스터와 부길드 마스터만이 길드 포인트를 등록할 수 있습니다." ));
		MsgBoxInfo.AddString( strMessage );
		_pUIMgr->CreateMessageBox( MsgBoxInfo );
		return FALSE;
	}
	else*/
	if( _pUIMgr->GetInventory()->GetItemCount(2331) <= 0 ) // 2331 빨간 카네이션
	{
		MsgBoxInfo.SetMsgBoxInfo( _S( 100, "이벤트" ), UMBS_OK, UI_NONE, MSGCMD_NULL);		
		strMessage.PrintF( _S(2236,"이벤트 아이템이 존재하지 않습니다.") );
		MsgBoxInfo.AddString( strMessage );
		_pUIMgr->CreateMessageBox( MsgBoxInfo );
		return FALSE;
	}
	
	return TRUE;
}
// ------------------------------------------------------------<<


// ----------------------------------------------------------------------------
// Name : CUIQuest()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIQuest::CUIQuest()
{
	m_nSelQuestID		= -1;	
	m_bShowQuestInfo	= FALSE;
	m_nCurQuestInfoLines = 0;
	m_iNpcIndex			= -1;
	m_bLockQuest		= FALSE;
	m_iCurQuestIndex	= -1;

	for(int i=0 ; i< WORLDCUP_MAX_COUNTRY ;i++)
		m_strCountry[i] = _S(i+2858,"2006 월드컵 본선 진출국");
/*
	"브라질","독일","이탈리아","영국","아르헨티나",
	"프랑스","네덜란드","스페인","포르투갈","체코",
	"스웨덴","멕시코","우크라이나","크로아티아","미국",
	"스위스","코트디부아르","세르비아-몬테네그로","폴란드","파라과이",
	"대한민국","호주","일본","가나","튀니지",
	"에콰도르","토고","앙골라","이란","사우디아라비아",
	"코스타리카","트리니다드 토바고",
	*/
	
}

// ----------------------------------------------------------------------------
// Name : ~CUIQuest()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIQuest::~CUIQuest()
{
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuest::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	int diff = SKILLLEARN_HEIGHT - QUEST_HEIGHT;

	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );
	
	_iMaxMsgStringChar = QUESTINFO_CHAR_WIDTH / ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 216, 22 );
	m_rcIcons.SetRect( 17, 55, 51, 200 );
	
	// Create remission texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\SkillLearn.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();
	
	// UV Coordinate of each part
	m_rtBackgroundTop.SetUV( 0, 0, 216, QUEST_TOP_HEIGHT, fTexWidth, fTexHeight );
	m_rtBackgroundBottom.SetUV( 0, QUEST_TOP_HEIGHT + diff, 216, SKILLLEARN_HEIGHT, fTexWidth, fTexHeight );
	m_rtSelOutline.SetUV( 218, 80, 250, 112, fTexWidth, fTexHeight );

	m_rtInfoUL.SetUV( 147, 471, 154, 478, fTexWidth, fTexHeight );
	m_rtInfoUM.SetUV( 157, 471, 159, 478, fTexWidth, fTexHeight );
	m_rtInfoUR.SetUV( 162, 471, 169, 478, fTexWidth, fTexHeight );
	m_rtInfoML.SetUV( 147, 481, 154, 483, fTexWidth, fTexHeight );
	m_rtInfoMM.SetUV( 157, 481, 159, 483, fTexWidth, fTexHeight );
	m_rtInfoMR.SetUV( 162, 481, 169, 483, fTexWidth, fTexHeight );
	m_rtInfoLL.SetUV( 147, 486, 154, 493, fTexWidth, fTexHeight );
	m_rtInfoLM.SetUV( 157, 486, 159, 493, fTexWidth, fTexHeight );
	m_rtInfoLR.SetUV( 162, 486, 169, 493, fTexWidth, fTexHeight );
	
	// Close button
	m_btnClose.Create( this, CTString( "" ), 184, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 219, 0, 233, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 219, 15, 233, 29, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );
	
	// Product button
	m_btnOK.Create( this, _S( 191, "확인" ), 70, 372-diff, 63, 21 );			
	m_btnOK.SetUV( UBS_IDLE, 0, 403, 63, 424, fTexWidth, fTexHeight );
	m_btnOK.SetUV( UBS_CLICK, 66, 403, 129, 424, fTexWidth, fTexHeight );
	m_btnOK.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOK.CopyUV( UBS_IDLE, UBS_DISABLE );
	m_btnOK.SetEnable( FALSE );

	// Cancel button
	m_btnCancel.Create( this, _S( 139, "취소" ), 141, 372-diff, 63, 21 );
	m_btnCancel.SetUV( UBS_IDLE, 0, 403, 63, 424, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 66, 403, 129, 424, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );
	
	// Scroll bar of special remission icons
	m_sbQuestIcon.Create( this, 193, 51, 9, 153 );
	m_sbQuestIcon.CreateButtons( TRUE, 9, 7, 0, 0, 10 );
	m_sbQuestIcon.SetScrollPos( 0 );
	m_sbQuestIcon.SetScrollRange( QUESTLIST_SLOT_ROW_TOTAL );
	m_sbQuestIcon.SetCurItemCount( 0 );
	m_sbQuestIcon.SetItemsPerPage( QUESTLIST_SLOT_ROW );
	// Up button
	m_sbQuestIcon.SetUpUV( UBS_IDLE, 219, 62, 228, 69, fTexWidth, fTexHeight );
	m_sbQuestIcon.SetUpUV( UBS_CLICK, 230, 62, 239, 69, fTexWidth, fTexHeight );
	m_sbQuestIcon.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbQuestIcon.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbQuestIcon.SetBarTopUV( 219, 32, 228, 42, fTexWidth, fTexHeight );
	m_sbQuestIcon.SetBarMiddleUV( 219, 43, 228, 45, fTexWidth, fTexHeight );
	m_sbQuestIcon.SetBarBottomUV( 219, 46, 228, 56, fTexWidth, fTexHeight );
	// Down button
	m_sbQuestIcon.SetDownUV( UBS_IDLE, 219, 71, 228, 78, fTexWidth, fTexHeight );
	m_sbQuestIcon.SetDownUV( UBS_CLICK, 230, 71, 239, 78, fTexWidth, fTexHeight );
	m_sbQuestIcon.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbQuestIcon.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Wheel region
	m_sbQuestIcon.SetWheelRect( -181, -1, 180, 154 );
	
	// List box of remission description
	m_lbQuestDesc.Create( this, 13, 207, 180, 124, _pUIFontTexMgr->GetLineHeight(), 7, 6, 1, FALSE );
	m_lbQuestDesc.CreateScroll( TRUE, 0, 0, 9, 124, 9, 7, 0, 0, 10 );
	// Up button
	m_lbQuestDesc.SetScrollUpUV( UBS_IDLE, 219, 62, 228, 69, fTexWidth, fTexHeight );
	m_lbQuestDesc.SetScrollUpUV( UBS_CLICK, 230, 62, 239, 69, fTexWidth, fTexHeight );
	m_lbQuestDesc.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbQuestDesc.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbQuestDesc.SetScrollBarTopUV( 219, 32, 228, 42, fTexWidth, fTexHeight );
	m_lbQuestDesc.SetScrollBarMiddleUV( 219, 43, 228, 45, fTexWidth, fTexHeight );
	m_lbQuestDesc.SetScrollBarBottomUV( 219, 46, 228, 56, fTexWidth, fTexHeight );
	// Down button
	m_lbQuestDesc.SetScrollDownUV( UBS_IDLE, 219, 71, 228, 78, fTexWidth, fTexHeight );
	m_lbQuestDesc.SetScrollDownUV( UBS_CLICK, 230, 71, 239, 78, fTexWidth, fTexHeight );
	m_lbQuestDesc.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbQuestDesc.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );

	m_strTeamA = CTString( " " );
	m_strTeamB = CTString( " " );
	m_bGoldenBallEntry = FALSE;
	m_bGoldenBallRequital = FALSE;
	m_bStartGoldenBall = FALSE;
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuest::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuest::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : InitQuest()
// Desc :
// ----------------------------------------------------------------------------
BOOL CUIQuest::InitQuest( )
{
	// Reset description
	m_lbQuestDesc.ResetAllStrings();
	m_vectorbtnQuests.clear();
	m_nSelQuestID	= -1;

	std::sort(m_vectorQuestList.begin(), m_vectorQuestList.end());

	CUIButtonEx TempUIButtonEx;
	
	std::vector<sQuestList>::const_iterator end = m_vectorQuestList.end();
	for(std::vector<sQuestList>::iterator pos = m_vectorQuestList.begin();
		pos != end;
		++pos)
	{
		const SBYTE sbFlag = (*pos).sbQuestFlag;
		if(sbFlag == QUEST_FLAG_ING)
			continue;

		// FIXME : 퀘스트 생성시 서버에서 받아오는 부분이 빠졌으니, Dynamic부분 빠져두 되는데...
		TempUIButtonEx.Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_QUEST, UBET_QUEST ); 
		CQuestDynamicData *pQuestDD = NULL;		
		if((*pos).sbQuestFlag != QUEST_FLAG_COMPLETE)
		{
			CQuestDynamicData QuestDD(CQuestSystem::Instance().GetStaticData((*pos).iQuestIndex));
			pQuestDD = &QuestDD;
			TempUIButtonEx.SetQuestInfo( (*pos).iQuestIndex, pQuestDD->GetQuestType1(), sbFlag );
		}
		else
		{
			pQuestDD = CQuestSystem::Instance().GetDynamicDataByQuestIndex((*pos).iQuestIndex);
			TempUIButtonEx.SetQuestInfo( (*pos).iQuestIndex, pQuestDD->GetQuestType1(), sbFlag );
		}
		m_vectorbtnQuests.push_back ( TempUIButtonEx );
	}
	
	// Set special scroll bar
	m_sbQuestIcon.SetScrollPos( 0 );
	m_sbQuestIcon.SetCurItemCount( m_vectorbtnQuests.size()  );

	if(m_vectorbtnQuests.empty())
		return FALSE;
	return TRUE;
}

static BOOL g_bHasQuest = FALSE;
static BOOL g_bHasEvent = FALSE;
// ----------------------------------------------------------------------------
// Name : OpenQuest()
// Desc : 
// ----------------------------------------------------------------------------
void CUIQuest::OpenQuest( int iMobIndex, BOOL bHasQuest, FLOAT fX, FLOAT fZ )
{
	// If this is already exist
	if( _pUIMgr->DoesMessageBoxLExist( MSGLCMD_QUEST_REQ ) || IsVisible() ) 		return;
	if( _pUIMgr->DoesMessageBoxLExist( MSGLCMD_RENUAL_EVENT ) || IsVisible() )		return;
	if( _pUIMgr->DoesMessageBoxLExist( MSGLCMD_HARVEST_MOON_DAY1 ) || IsVisible() )	return;
	if( _pUIMgr->DoesMessageBoxLExist( MSGLCMD_HARVEST_MOON_DAY2 ) || IsVisible() )	return;
	if( _pUIMgr->DoesMessageBoxLExist( MSGLCMD_EVENT ) || IsVisible() )				return;
	if( _pUIMgr->DoesMessageBoxLExist( MSGLCMD_GATE_OF_DIMENSION ) || IsVisible() )	return;
	if( _pUIMgr->DoesMessageBoxLExist( MSGLCMD_CHANGEWEAPON_EVENT_REQ ) || IsVisible() )	return;
	if( _pUIMgr->DoesMessageBoxLExist( MSGLCMD_TREASUREBOX_EVENT ) || IsVisible() )	return;
	
	if( _pUIMgr->GetSelectWord()->IsVisible() ) return;

	// Set position of target npc
	m_fNpcX = fX;
	m_fNpcZ = fZ;

	CMobData& MD = _pNetwork->GetMobData(iMobIndex);

	// NOTE : 던젼 마스터의 경우.
	if( iMobIndex == 71 )
	{
		// 퀘스트 보상 확인.
		if(_pUIMgr->GetQuestBookComplete()->GetCompleteQuestCount())
		{
			_pUIMgr->GetQuestBookComplete()->OpenQuestBook();
			_pUIMgr->GetQuestBookComplete()->SetTargetIndex( _pUIMgr->GetQuestBookList()->GetTargetIndex() );
			_pUIMgr->GetQuestBookList()->SetTargetIndex(-1);
			_pUIMgr->GetQuestBookNew()->SetTargetIndex(-1);

			// [090528: selo] 이전 버튼을 사용하기 위해 NPC 인덱스를 한번더 저장하는 기능
			_pUIMgr->GetQuestBookComplete()->SetPrevNPCIndex(71);
			_pUIMgr->GetQuestBookComplete()->SetPrevNPCUIType(UI_QUEST);
		}
		else
		{
#ifdef HELP_SYSTEM_1
			// [KH_07044] 3차 도움말 관련 추가
			if(g_iShowHelp1Icon)
			{
				_pUIMgr->GetHelp3()->ClearHelpString();
				_pUIMgr->GetHelp3()->AddHelpString(_S(3294, "퍼스널 던전은 혼자서만 들어가는 던전으로 각 던전마다 정해진 임무를 수행하는 것이 목적입니다. 수행하고자 하는 임무의 목록을 선택하면 임무의 내용과 보상 & 수행조건을 확인할 수 있으며 수락을 클릭하면 자동으로 해당 퍼스널 던전으로 이동됩니다."));
				_pUIMgr->GetHelp3()->AddHelpString(_S(3295, "※ 퍼스널 던전에 들어가기 위해서는 적정레벨과 입장권이 필요할 수 있습니다."));
				_pUIMgr->GetHelp3()->AddHelpString(_S(3296, "※ 퍼스널 던전에서는 모든 일반공격이 범위공격으로 적용되며 퍼스널 던전 전용 스킬을 사용할 수 있습니다."));
				_pUIMgr->GetHelp3()->AddHelpString(_S(3297, "※ 임무를 완수 하면 자동으로 마을로 귀환되며, 던전 마스터에게 보상을 받을 수 있습니다."));
				_pUIMgr->GetHelp3()->OpenHelp(this);
			}
#endif
			// Create quest message box
			_pUIMgr->CreateMessageBoxL( _S( 99,"퀘스트" ), UI_QUEST, MSGLCMD_QUEST_REQ );		
			
			CTString	strNpcName = _pNetwork->GetMobName(iMobIndex);
			_pUIMgr->AddMessageBoxLString( MSGLCMD_QUEST_REQ, TRUE, strNpcName, -1, 0xE18600FF );
			
			_pUIMgr->AddMessageBoxLString( MSGLCMD_QUEST_REQ, TRUE, _S( 581, "저에게 어떤 볼일이 있으신가요?" ), -1, 0xA3A1A3FF );			

			_pUIMgr->AddMessageBoxLString( MSGLCMD_QUEST_REQ, FALSE, _S( 1683, "발록 격파" ), QUEST_KILL_BOSS );			
			_pUIMgr->AddMessageBoxLString( MSGLCMD_QUEST_REQ, FALSE, _S( 1684, "공주 구출" ), QUEST_SAVE_PRINCESS );
			
			// wooss 051006 태국 클베 버전 임시 주석처리 
			// wooss 051102 오베용 주석 해제
			// 0x0001 아자카 on/off 
			// 0x0002 차원의 문 on/off
			if(g_iTempFlag&0x01) 
				_pUIMgr->AddMessageBoxLString( MSGLCMD_QUEST_REQ, FALSE, _S( 1685, "아자카 협곡" ), QUEST_AZAKA_RAVINE );	

#ifndef CLOSEBETA_PERSONDUNGON_CHECK_NA_20081127
			if(g_iTempFlag&0x02 || g_iCountry == MALAYSIA)
				_pUIMgr->AddMessageBoxLString( MSGLCMD_QUEST_REQ, FALSE, _S( 1686, "차원의 문" ), QUEST_GATE_OF_DIMENSION );	
#endif
		}
	}
 	else if(iMobIndex == 336 )  	// 눈사람 2006 X-Mas event Snowman [12/11/2006 Theodoric]	TEVENT_XMAS_2006
 	{
 		_pUIMgr->CreateMessageBoxL(_S(100,"이벤트"),UI_QUEST,MSGLCMD_QUEST_REQ);
		CTString strNpcName = _pNetwork->GetMobName(iMobIndex);
 		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE,  strNpcName,-1,0xE18600FF);
 		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE,  _S(3175,"메리 크리스마스!! 모든 대륙에 평화를!!"),-1,0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE,  _S(3176,"평화를 수호하는 형제여 무슨 일로 저를 찾아 오셨습니까?"),-1,0xa3a1a3ff);
 		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE,  _S(3177,"제가 도와 드릴 수 있는 일이라면, 크리스마스 정신으로 무엇이든 도와 드리겠습니다."),-1,0xa3a1a3ff);
 		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE,  CTString(" "));		
 		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, FALSE, _S(3169, "크리스트마스 이벤트" ), EVENT_XMAS_2006 );	
 		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, FALSE, _S(1874, "취소한다."  ) );	
 	}
	// WSS_MINIGAME 070420 -------------------------------------------------------------->>
	else if(iMobIndex == 340 )  	// 사로얀 2006 X-Mas event Snowman [12/11/2006 Theodoric]	TEVENT_XMAS_2006
 	{
		if (_pUIMgr->DoesMessageBoxLExist(MSGLCMD_EVENT_MINIGAME)) return;

 		_pUIMgr->CreateMessageBoxL(_S(100,"이벤트"),UI_QUEST,MSGLCMD_QUEST_REQ);
		CTString strNpcName = _pNetwork->GetMobName(iMobIndex);
 		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE,  strNpcName,-1,0xE18600FF);
 		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE,  _S(3420, "안녕하세요. 저는 사로얀! 대현자 '스타니슬라나'의 충실한 종입니다."),-1,0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE,  _S(3421, "내가 모시는 주인님은 현명하기로 따지면 아이리스 대륙에서 다섯 손가락에 들고 마음은 소녀처럼 맑지요."),-1,0xa3a1a3ff); 		
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE,  _S(3422, "실은 우리 주인님이 좋아하는 귀여운 어린이를 위해 웬디라는 곰돌이 인형을 만드시다가 그만 큰 문제가 생겨버렸습니다."),-1,0xa3a1a3ff);
 		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE,  _S(3423, "주인님께서는 '나의 숙적이자 사악한 마도사인 프라우론이 흑마법을 사용해 완성된 웬디를 조각 내어 숨겨 놓은 것 같다. "),-1,0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE,  _S(3424, "아이리스 대륙의 용사님들께 조각난 웬디를 찾아달라고 부탁하려무나' 라고 저에게 말씀하셨습니다. "),-1,0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE,  _S(3425, "그래서 저는 주인님의 곰인형 웬디를 찾아주실 용사님을 만나러 아이리스 대륙 모든 마을을 동분서주하고 있습니다."),-1,0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE,  _S(3426, "곰인형을 모두 찾으면 귀여운 어린이들에게 선물해 줄 수 있을텐데…도와주시겠습니까?"),-1,0xa3a1a3ff);
 		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, FALSE, _S(3427, "이벤트 참여하기" ), EVENT_MINIGAME );	
 		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, FALSE, _S(1874, "취소한다."  ) );	
 	}
	// -----------------------------------------------------------------------------------<<
	else if (iMobIndex == 341) // eons 꽃놀이 이벤트(정원사)
	{
		_pUIMgr->CreateMessageBoxL(_S(100, "이벤트"), UI_QUEST, MSGLCMD_QUEST_REQ);
		CTString strNpcName = _pNetwork->GetMobName(iMobIndex);

		/**********************************************************************************************
		// 2007년
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, strNpcName, -1, 0xE18600FF);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(3489, "저는 꽃나무를 관리하는 정원사입니다. "), -1, 0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(3490, "이맘때쯤이면 옆에 있는 이 나무에 활짝 꽃이 피어야 할 텐데, 이상하게도 아직 꽃을 피우지 못하고 있습니다."), -1, 0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(3491, "나무에 꽃을 피우기 위해서 [퇴비]와 [정화수]가 필요한데, 혹시 가지고 계시다면 저에게 가져다 주시지 않으실래요?"), -1, 0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(3510, "꽃나무의 개화를 도와주시면 [퇴비]나 [정화수] 1개당 기여도를 1씩 드리고, 기여도 100마다 귀한 보상품을 받을 수 있는 교환권을 1장씩 드리겠습니다."), -1, 0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, CTString(" "));
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(3513, "교환권 보상품 목록"), -1, 0xE18600FF);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(3514, "고급제련석 3개 "), -1, 0x6BD2FFFF);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(3515, "숙련도 복구 주문서 5장"), -1, 0x6BD2FFFF);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(3516, "순간이동 마법서 5장 "), -1, 0x6BD2FFFF);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(3517,"드롭율 증폭제 10개"), -1, 0x6BD2FFFF);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(3518,"데스나이트 소환서 1장"), -1, 0x6BD2FFFF);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(3519,"바알 소환서 1장"), -1, 0x6BD2FFFF);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(3520, "문스톤 7개"), -1, 0x6BD2FFFF);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(3521, "부스터 7개"), -1, 0x6BD2FFFF);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(3522, "분홍 벗 꽃 5개"), -1, 0x6BD2FFFF);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(3523, "흰 벗 꽃 5개"), -1, 0x6BD2FFFF);

		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, FALSE, _S(3492, "개화를 돕는다"), EVENT_FLOWERS_SUPPORT);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, FALSE, _S(3493, "기여도와 교환권"), EVENT_SUPPORT_EXCHANGE);
		// 2007년
		/*********************************************************************************************/
		
		/**********************************************************************************************/
		// 2008년
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, strNpcName, -1, 0xE18600FF);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(3489, "저는 꽃나무를 관리하는 정원사입니다. "), -1, 0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(4110, "꽃이 만개하여 예쁘게 피여 있죠? 이것이 다 작년에 저를 도와주신 덕분이죠. 그래서 말인데요."), -1, 0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(4111, "이 꽃나무를 관리하는데 퇴비가 많이 들어가는데 저에게 퇴비를 좀 가져다 주세요. 그럼 제가 작은 선물을 드릴께요."), -1, 0xa3a1a3ff);

		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, FALSE, _S(4112, "퇴비를 건네준다."), EVENT_SAKURA_2008);
		// 2008년
		/*********************************************************************************************/

		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, FALSE, _S(1874, "취소한다."));
	}
	else if (iMobIndex == 342)
	{
		// empty : 꽃나무를 계속 도는 문제로
	}
	else if (iMobIndex == 484) // 크리스마스 트리 (2007)
	{
		_pUIMgr->CreateMessageBoxL(_S(100, "이벤트"), UI_QUEST, MSGLCMD_QUEST_REQ);
		CTString strNpcName = _pNetwork->GetMobName(iMobIndex);

		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, strNpcName, -1, 0xE18600FF);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(3947, "겨울이 되면 다들 춥다고들 하지만 연말 연시 따듯한 소식이 들려와 춥지만은 않습니다."), -1, 0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(3948, "하얀 눈을 맞으며 소원을 빌면 이루어질 것 같지 않나요?"), -1, 0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(3949, "올해는 여러분 모두에게 행복하고 따뜻한 크리스마스 되길 바랍니다."), -1, 0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(3950, "Merry Christmas"), -1, 0xa3a1a3ff);

		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, FALSE, _S(3951, "크리스마스 트리 장식하기"), EVENT_XMAS2007_DECO);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, FALSE, _S(3952, "크리스마스 축복받기"), EVENT_XMAS2007_BLESSEDNESS);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, FALSE, _S(1268, "취소하기"));
	}
	else if (iMobIndex == 455)//허수아비 npc
	{
		_pUIMgr->CreateMessageBoxL(_S(100,"이벤트"), UI_QUEST, MSGLCMD_QUEST_REQ);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, (CTString)MD.GetName(), -1, 0xE18600FF);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE,
			_S(3775, "나는 죽은 자들의 광대!! 오늘은 우리들의 축제!! 재미있게 즐기지 않으면 지옥을 구경시켜 주겠다!!"),-1, 0xa3a1a3ff);

		if (IS_EVENT_ON(TEVENT_HOLLOWEEN2007))
		{
			_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, FALSE, _S(3776, "악마날개 머리띠 지급받기"), EVENT_HALLOWEEN2007_DEVILHAIR);
			_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, FALSE, _S(3777, "사탕바구니에 사탕받기"), EVENT_HALLOWEEN2007_CANDYBASKET);
			_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, FALSE, _S(3778, "호박머리 탈 교환받기"), EVENT_HALLOWEEN2007_PUMKINHEAD);
			_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, FALSE, _S(3779, "마녀모자 교환받기"), EVENT_HALLOWEEN2007_WITCHHAT);
			_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, FALSE, _S(3780, "호박교환 받기"), EVENT_HALLOWEEN2007_PUMKIN);
		}
		
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, FALSE, _S(1874, "취소한다."));
	}
	else  if(iMobIndex == 254 )
	{	// 상시 이벤트 NPC 랜디....wooss 051031		
		// Create Event Message Box
		_pUIMgr->CreateMessageBoxL(_S(100,"이벤트"),UI_QUEST,MSGLCMD_QUEST_REQ);
		CTString strNpcName = _pNetwork->GetMobName(iMobIndex);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ,TRUE, strNpcName,-1,0xE18600FF);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ,TRUE, _S(2204,"무슨 일로 저를 찾아 오셨나요?"),-1,0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ,TRUE, _S(2205,"제가 도와 드릴 수 있는 일이 있다면 도와 드리겠습니다."),-1,0xa3a1a3ff);

		if(g_iTempFlag&0x00080000  )
			_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ,FALSE,_S(2286,"2005 크리스마스 이벤트"),EVENT_NEWYEAR1);
		if(g_iTempFlag&0x00100000  )
			_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ,FALSE,_S(2287,"2006 신년 인내의 열매 이벤트"),EVENT_NEWYEAR2);
//		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ,FALSE,_S(2206,"2pan4pan 게임머니 교환"),QUEST_EXCHANGE_MONEY);
//		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ,FALSE,_S(2207,"2pan4pan 보물상자 열기"),QUEST_OPEN_TREASURE);
//		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ,FALSE,_S(2208,"2pan4pan 캐릭터카드 보상"),QUEST_CHARACTER_CARD);
//		_pUIMgr->AddMessageBoxLString( MSGLCMD_QUEST_REQ, FALSE, _S( 2433, "친구 찾기 이벤트" ), EVENT_FIND_FRIEND);
		
		//WSS_GUILD_MASTER 070411 ----------------------->>
		if( IS_EVENT_ON(TEVENT_PARENTS_2007)||IS_EVENT_ON(TEVENT_CHILDREN_2007)||IS_EVENT_ON(TEVENT_TEACHER_2007) )
		{
			_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, FALSE, _S(3395, "가정의 달 이벤트" ), EVENT_MAY );	 		
		}
		// ----------------------------------------------<<
		if( IS_EVENT_ON(TEVENT_OX_QUIZ) )
		{
			_pUIMgr->AddMessageBoxLString( MSGLCMD_QUEST_REQ, FALSE, _S(2616, "O.X 이벤트 존 입장" ), EVENT_OX_GOZONE );
		}
		// -------------- wooss 060521 platinum item exchage --------------------------->>
#ifdef PLATINUM_EXCHANGE
		_pUIMgr->AddMessageBoxLString( MSGLCMD_QUEST_REQ, FALSE, _S(2752,"플래티늄 제련석 교환" ), PLATINUM_EXCHANGE_STONE );
#endif
		// -----------------------------------------------------------------------------<<

		// ---------- WORLDCUP EVENT wooss 060530 ------------------------------------------------>>
#ifdef WORLDCUP_ITEM
		_pUIMgr->AddMessageBoxLString( MSGLCMD_QUEST_REQ, FALSE, _S( 2795, "우승국가 맞추기 이벤트" ), WIN_SELECT );
#endif
		// ---------- WORLDCUP EVENT wooss 060530 ------------------------------------------------<<

		if( IS_EVENT_ON(TEVENT_RAIN) )	//070621 ttos: 일본의 빗방울 이벤트
		{
			_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ,FALSE,_S(2753, "빗방울 이벤트" ),EVENT_RAINYDAY);			
		}
#ifdef WORLDCUP_GOLDENBALL
		_pUIMgr->AddMessageBoxLString( MSGLCMD_QUEST_REQ, FALSE, _S( 2796, "골든 볼 이벤트" ), EVENT_GOLDENBALL );
#endif		

		if( IS_EVENT_ON(TEVENT_BUDDHIST) )
		{
			_pUIMgr->AddMessageBoxLString( MSGLCMD_QUEST_REQ, FALSE, _S( 2918, "불교 촛불 축제 이벤트 참여" ), EVENT_BUDDHISM );
		}

#ifdef QUEST_HARVEST_MOON_DAY
		if (IS_EVENT_ON(TEVENT_CHUSEOK_2006))
		{
			_pUIMgr->AddMessageBoxLString( MSGLCMD_QUEST_REQ, FALSE, _S( 1860, "추석맞이 이벤트" ), QUEST_HARVEST_MOON_DAY_EVENT2 );
		}
#endif

		if( IS_EVENT_ON(TEVENT_VALENTINE_2007) )
		{
			_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, FALSE, _S(3203, "러브러브 이벤트"), EVENT_LOVE_LOVE);
		}

		_pUIMgr->AddMessageBoxLString( MSGLCMD_QUEST_REQ, FALSE, _S( 1220, "취소한다." ) );			
	}
	else if (iMobIndex == 408) // 이벤트 진행요원
	{
		_pUIMgr->CreateMessageBoxL(_S(100, "이벤트"), UI_QUEST, MSGLCMD_QUEST_REQ);
		CTString strNpcName = _pNetwork->GetMobName(iMobIndex);

		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, strNpcName, -1, 0xE18600FF);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(3613, "안녕하세요. 저는 루나시스 공식 유랑단에서 파견된 이벤트 진행요원입니다."), -1, 0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(3614, "루나시스는 마법과 놀이의 대륙으로 원래는 인간보다 뛰어난 두뇌와 육체를 지닌 천인들이 살고 있었다고도 전해지는 창조신에 의해 만들어진 전설의 부유대륙이에요."), -1, 0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(3615, "특성상 위치가 한정되어 있지 않지만 일정 주기로 아이리스의 남쪽이나 에게하의 서쪽에서 육안으로 확인할 수 있는 위치까지 접근하기도 합니다."), -1, 0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, CTString(" "));
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(3616, "그런데, 이번엔 무슨일로 저를 찾아오셨나요?"), -1, 0xa3a1a3ff);

		if (IS_EVENT_ON(TEVENT_BJMONO_2007))
		{
			_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, FALSE, _s("SK BJ MONO 응모권 받기"), EVENT_BJMONO_2007_REQ);
			_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, FALSE, _s("SK BJ MONO 응모권 교환"), EVENT_BJMONO_2007_CHANGE_REQ);
		}
		if (IS_EVENT_ON(TEVENT_TG2007_1000DAYS))
		{
			_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, FALSE, _S(3828, "라스트카오스 1000일 기념모자 받기"), EVENT_TG2007_1000DAYS_CAP);
			_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, FALSE, _S(3829, "기념모자 업그레이드"), EVENT_TG2007_1000DAYS_CAP_UPGRADE);
			_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, FALSE, _S(3830, "폭죽 받기 이벤트"), EVENT_TG2007_1000DAYS_FIRECRACKER);
		}

		if (IS_EVENT_ON(TEVENT_TG2007_SCREENSHOT)) // WSS_TG2007 2007/09/14
		{
			_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, FALSE, _s("송편받기 이벤트"), EVENT_TG2007_SCREENSHOT);			
		}

		if (IS_EVENT_ON(TEVENT_CHAOSBALL)) // WSS_TG2007 2007/09/14
		{
			_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, FALSE, _S(4118, "아이리스 상점 이벤트 [카오스 볼을 열어라!]"), EVENT_CHAOSBALL);			
		}

		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, FALSE, _S(1874, "취소한다."));
	}
	else if( iMobIndex == 834 ) // 피닉스 클럽 마스터
	{
		_pUIMgr->CreateMessageBoxL(_S(100, "이벤트"), UI_QUEST, MSGLCMD_QUEST_REQ);
		CTString strNpcName = _pNetwork->GetMobName(iMobIndex);

		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, strNpcName, -1, 0xE18600FF);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(4225, "어서오세요. 피닉스 클럽에 오신 것을 환영합니다"), -1, 0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _s(" "));
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(4226, "아이리스 대륙에서 모험을 시작하고 많은 경험을 쌓아 100레벨 이상이 되었다면 새로운 캐릭터를 생성하여 피닉스가 될 수 있습니다."), -1, 0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(4227, "피닉스가 되면 아래와 같은 혜택을 통해 보통의 클래스들보다 좀 더 쉽게 모험을 할 수 있게 됩니다."), -1, 0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _s(" "));
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(4228, "피닉스 가입조건"), -1, 0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(4229, "1. 100레벨 이상의 캐릭터를 보유한 서버에 새롭게 생성한 신규 캐릭터"), -1, 0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(4249, "2. 피닉스 클럽 가입권을 보유한 캐릭터"), -1, 0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _s(" "));
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(4230, "피닉스 혜택"), -1, 0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(4231, "1. 피닉스의 재능"), -1, 0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(4250, "캐릭터 레벨이 35로 상승(기본 성장포인트 포함)"), -1, 0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(4251, "숙련도, 피닉스 보너스 성장포인트 30개 추가 지급"), -1, 0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(4232, "2. 피닉스의 축복"), -1, 0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(4252, "물리&마법 공격력/물리 방어력5%상승(100레벨 이전까지 적용)"), -1, 0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(4253, "3. 피닉스의 부활"), -1, 0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(4254, "사망 시 제자리에서 부활이 가능한 아이템 제공"), -1, 0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(4233, "4. 피닉스의 날개"), -1, 0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(4255, "피닉스 전용 투구(기간 무제한, 코스튬 투구로 결합주문서 사용가능) 제공"), -1, 0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(4256, "피닉스 전용 장비 세트 제공"), -1, 0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(4257, "펫 시즌 1용 피닉스의 염색약 [피닉스의 결정] 제공"), -1, 0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _s(" "));
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, FALSE, _S(4234, "피닉스로 등록한다."), EVENT_PHOENIX_REQ);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, FALSE, _S(1268, "취소하기"));
	}
	else if( iMobIndex == 899 || iMobIndex == 900 ) //경매대행 npc
	{
		_pUIMgr->CreateMessageBoxL(_S(4287, "거래 대행 서비스"), UI_QUEST, MSGLCMD_QUEST_REQ);
		CTString strNpcName = _pNetwork->GetMobName(iMobIndex);

		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, strNpcName, -1, 0xE18600FF);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(4352, "상인의 후예 길드는 대륙에서 가장 안전하고 빠른 거래 대행 서비스를 소정의 수수료를 받고 제공하고 있다."), -1, 0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _s(" "));
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S(4353, "거래를 맡기고 싶은 물건이 있는 것인가?"), -1, 0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _s(" "));
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, FALSE, _S(4354, "거래 대행 서비스를 이용한다."), USE_AUCTION);
		_pUIMgr->AddMessageBoxLString( MSGLCMD_QUEST_REQ, FALSE, _S( 1748, "NPC 안내" ), NPC_HELP);
		_pUIMgr->AddMessageBoxLString( MSGLCMD_QUEST_REQ, FALSE, _S( 1220, "취소한다." ) );	
	}
// [070807: Su-won] EVENT_ADULT_OPEN
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// WSS_DRATAN_SIEGEWARFARE 070730 ----------------------------------------------->>
	else if( iMobIndex == 351)	// 드라탄 공성 마스터 타워
	{	
		// Create Event Message Box
		_pUIMgr->CreateMessageBoxL(_S( 3671,"마스터 타워 메인 메뉴"),UI_QUEST,MSGLCMD_QUEST_REQ);
		CTString strNpcName = _pNetwork->GetMobName(iMobIndex);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ,TRUE, strNpcName,-1,0xE18600FF);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ,TRUE, _S( 3672,"안녕하세요. 성주님"),-1,0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ,TRUE, _S( 3673,"이곳에서는 성을 수호하기 위한 각종 타워를 설치하고 강화, 수리하거나 성문을 강화할 수 있습니다."),-1,0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ,TRUE, _S( 3674,"필요한 기능을 선택하시기 바랍니다."),-1,0xa3a1a3ff);
		
		_pUIMgr->AddMessageBoxLString( MSGLCMD_QUEST_REQ, FALSE, _S( 3675, "타워 가동하기" ), SIEGEWARFARE_MASTERTOWER1 );
		_pUIMgr->AddMessageBoxLString( MSGLCMD_QUEST_REQ, FALSE, _S( 3676, "타워 강화하기" ), SIEGEWARFARE_MASTERTOWER2 );			
		_pUIMgr->AddMessageBoxLString( MSGLCMD_QUEST_REQ, FALSE, _S( 3677, "성문 강화하기" ), SIEGEWARFARE_MASTERTOWER3 );			
		_pUIMgr->AddMessageBoxLString( MSGLCMD_QUEST_REQ, FALSE, _S( 3678, "마스터 타워 수리하기" ), SIEGEWARFARE_MASTERTOWER4 );			
		_pUIMgr->AddMessageBoxLString( MSGLCMD_QUEST_REQ, FALSE, _S( 1220, "취소한다." ) );		
	}
	//----------------------------------------------
	//071129 ttos: 판매 대행 NPC 회장
	else if(iMobIndex == 470)
	{
		// Create Event Message Box
		_pUIMgr->CreateMessageBoxL(_s( "아이템 판매 대행협회 회장"),UI_QUEST,MSGLCMD_QUEST_REQ);
//		CTString strNpcName = _pNetwork->GetMobName(iMobIndex);
//		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ,TRUE, strNpcName,-1,0xE18600FF);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ,TRUE, _s( "안녕하세요. 저는 아이템 판매 대행협회 회장입니다."),-1,0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ,TRUE, _s( "저희 아이템 판매 대행협회에서는 고객님의 소중한 아이템을 안전하게 판매 대행을 하고 있습니다."),-1,0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ,TRUE, _s( "저희를 믿고 물품을 맡겨만 주시면 안전하게 판매를 해드리겠습니다."),-1,0xa3a1a3ff);
		
		_pUIMgr->AddMessageBoxLString( MSGLCMD_QUEST_REQ, FALSE, _s( "아이템 판매 대행 종료" ), CASH_PERSONSHOP_CLOSE );
		_pUIMgr->AddMessageBoxLString( MSGLCMD_QUEST_REQ, FALSE, _s( "보관된 물품 회수" ), CASH_PERSONSHOP_ITEM );			
		_pUIMgr->AddMessageBoxLString( MSGLCMD_QUEST_REQ, FALSE, _s( "보관된 나스 회수" ), CASH_PERSONSHOP_NAS );			
		_pUIMgr->AddMessageBoxLString( MSGLCMD_QUEST_REQ, FALSE, _S( 1220, "취소한다." ) );		
	}	
	// ------------------------------------------------------------------------------<<
	else if( iMobIndex == 893 ) //[ttos_2009_3_18]: 하나포스 및 SK브로드밴드 가입자 이벤트
	{
		_pUIMgr->CreateMessageBoxL(_S( 4449, "초보 지원사"), UI_QUEST, MSGLCMD_QUEST_REQ);
		CTString strNpcName = _pNetwork->GetMobName(iMobIndex);

		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, strNpcName, -1, 0xE18600FF);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S( 4450, "아이리스대륙에서도 가장 번화한 도시 란돌에 오신것을 환영합니다!"), -1, 0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S( 4451, "저는 란돌 기사단에서 파견된 초보지원사로 이곳에 처음 오신 여러분들에게"), -1, 0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S( 4452, "앞으로 겪게될 모험과 난관을 헤쳐나가는데 도움이 될 마법을 걸어드리고 있어요!"), -1, 0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S( 4453, "이 마법을 받으면 초보자 여러분들의 공격력이 향상되고 경험치와 숙련도를 더 많이 받을 수 있을겁니다."), -1, 0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, TRUE, _S( 4454, "단, 접속을 종료하면 마법은 사라지게 되며, 60레벨까지는 마법이 풀려도 다시 마법을 받을 수 있습니다. "), -1, 0xa3a1a3ff);
		_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, FALSE, _S( 4455, "초보 지원 마법 받기"), EVENT_HANAPOS_SK_EVENT);
		_pUIMgr->AddMessageBoxLString( MSGLCMD_QUEST_REQ, FALSE, _S( 1748, "NPC 안내" ), NPC_HELP);
		_pUIMgr->AddMessageBoxLString( MSGLCMD_QUEST_REQ, FALSE, _S( 1220, "취소한다." ) );	
	}
	else 
	{
		// Create quest message box
		_pUIMgr->CreateMessageBoxL( _S( 99,"퀘스트" ), UI_QUEST, MSGLCMD_QUEST_REQ );		
		
		CTString	strNpcName = _pNetwork->GetMobName(iMobIndex);
		_pUIMgr->AddMessageBoxLString( MSGLCMD_QUEST_REQ, TRUE, strNpcName, -1, 0xE18600FF );
		
		_pUIMgr->AddMessageBoxLString( MSGLCMD_QUEST_REQ, TRUE, _S( 581, "저에게 어떤 볼일이 있으신가요?" ), -1, 0xA3A1A3FF );
		
#ifndef NEW_QUESTBOOK
		_pUIMgr->AddMessageBoxLString( MSGLCMD_QUEST_REQ, FALSE, _S( 1053,"이야기한다." ), QUEST_TALK  );
#else		
		// 2009. 05. 27 김정래
		// 이야기한다 변경 처리
		CUIQuestBook::AddQuestListToMessageBoxL(MSGLCMD_QUEST_REQ);				
#endif
	
		// wooss 임시로 이벤트 막아둠 051006
		// 풀어줌 051101
		if(MD.IsEvent()&&(g_iTempFlag&0x80000000) )
		{
			g_bHasEvent = TRUE;
			_pUIMgr->AddMessageBoxLString( MSGLCMD_QUEST_REQ, FALSE, _S( 100, "이벤트" ), QUEST_EVENT );				
		}
//#ifdef WEAPONCHANGE_EVENT

		if( iMobIndex == 139 )		// 로레인 NPC 일경우...
		{
			if(g_iTempFlag&0x00010000 )
				_pUIMgr->AddMessageBoxLString( MSGLCMD_QUEST_REQ, FALSE, _S( 1204, "무기교체 이벤트" ), QUEST_CHANGEWEAPON );			
			
			if(IS_EVENT_ON(TEVENT_SAKURA))
				_pUIMgr->AddMessageBoxLString( MSGLCMD_QUEST_REQ, FALSE, _S( 2536, "전통의상 교환" ), EVENT_CLOTHES_EXCHANGE );

			if(IS_EVENT_ON(TEVENT_COLLECT_BUG))
				_pUIMgr->AddMessageBoxLString( MSGLCMD_QUEST_REQ, FALSE, _S( 2946, "여름 곤충채집 이벤트" ), EVENT_COLLECTBUGS );
		}

		_pUIMgr->AddMessageBoxLString( MSGLCMD_QUEST_REQ, FALSE, _S( 1220, "취소한다." ) );			
	}
	
	m_iNpcIndex		= iMobIndex;
	g_bHasQuest		= bHasQuest;
}

// ----------------------------------------------------------------------------
// Name : OpenQuest()
// Desc : 
// ----------------------------------------------------------------------------
void CUIQuest::OpenQuest( int iMobIndex, FLOAT fX, FLOAT fZ )
{
	if(IsVisible())
		return;	

	// Set position of target npc
	m_fNpcX = fX;
	m_fNpcZ = fZ;

	_pUIMgr->RearrangeOrder( UI_QUEST, TRUE );

	InitQuest();
	//m_nSelQuestID	= -1;	
	m_iNpcIndex		= iMobIndex;
	LockQuest(FALSE);

	// FIXME : 혹시 몰라서...
	//m_btnOK.SetEnable(TRUE);
	m_btnCancel.SetEnable(TRUE);
	m_btnClose.SetEnable(TRUE);
	//_pUIMgr->SetCSFlagOn( CSF_QUEST );
}

// ----------------------------------------------------------------------------
// Name : MsgBoxLCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuest::MsgBoxLCommand( int nCommandCode, int nResult )
{
	CTString tv_str; 
	
	switch( nCommandCode )
	{
	case MSGLCMD_RENUAL_EVENT:
		{
			if( nResult == QUEST_RENUAL_EVENT_OK )
			{
				_pUIMgr->GetSelectWord()->OpenSelectWord();						
				//!! 서버에 확인 메세지 보내기 
			}
			else
			{
				// 종료
			//	MsgBoxLCommand( MSGLCMD_QUEST_REQ, QUEST_RENUAL_EVENT ); 
			}	

		}
		break;
	case MSGLCMD_HARVEST_MOON_DAY1:
		{
			_pUIMgr->CloseMessageBox( MSGCMD_HARVEST_MOON_DAY_EVENT_UPGRADE1 );
			_pUIMgr->CloseMessageBox( MSGCMD_HARVEST_MOON_DAY_EVENT_UPGRADE2 );
			_pUIMgr->CloseMessageBox( MSGCND_MOON_DAY_EVENT_GIVE_ITEM );
				
			CTString strMessage;
			CUIMsgBox_Info	MsgBoxInfo;
				
			//strMessage = _S( 1859, "작물을 업그레이드 하시겠습니까?" );	
			if( nResult == QUEST_HARVEST_MOON_DAY_EVENT_UPGRADE1 )
			{
				strMessage = _S(3131, "송편을 만드시겠습니까?" );
				MsgBoxInfo.SetMsgBoxInfo( _S( 1860,  "추석맞이 이벤트" ) , UMBS_OKCANCEL,
				UI_QUEST, MSGCMD_HARVEST_MOON_DAY_EVENT_UPGRADE1 );
			}
			else if( nResult == QUEST_HARVEST_MOON_DAY_EVENT_UPGRADE2 )
			{
				strMessage = _S(3132, "오색 송편을 만드시겠습니까?" );
				MsgBoxInfo.SetMsgBoxInfo( _S( 1860,  "추석맞이 이벤트" ) , UMBS_OKCANCEL,
					UI_QUEST, MSGCMD_HARVEST_MOON_DAY_EVENT_UPGRADE2 );
			}
						
			MsgBoxInfo.AddString( strMessage );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );
		}
		break;
	case MSGLCMD_HARVEST_MOON_DAY2:
		{
			/******
			if( nResult == QUEST_HARVEST_MOON_DAY_EVENT_UPGRADE )
			{
				_pUIMgr->CloseMessageBox( MSGCMD_HARVEST_MOON_DAY_EVENT_UPGRADE );
				_pUIMgr->CloseMessageBox( HARVEST_MOON_DAY_EVENT_UPGRADE_GIVE_ITEM );
				
				CTString strMessage;
				CUIMsgBox_Info	MsgBoxInfo;
				
				strMessage = _S( 1859, "작물을 업그레이드 하시겠습니까?" );	
				MsgBoxInfo.SetMsgBoxInfo( _S( 1860,  "추석맞이 이벤트" ) , UMBS_OKCANCEL,
					UI_QUEST, MSGCMD_HARVEST_MOON_DAY_EVENT_UPGRADE );
							
				MsgBoxInfo.AddString( strMessage );
				_pUIMgr->CreateMessageBox( MsgBoxInfo );
					
			//	_pNetwork->SendChuseokUpgrade();
			}
			else 
			*******/
			if( nResult == MSGCND_MOON_DAY_EVENT_GIVE_ITEM )
			{
				_pUIMgr->CloseMessageBox( MSGCMD_HARVEST_MOON_DAY_EVENT_UPGRADE1 );
				_pUIMgr->CloseMessageBox( MSGCMD_HARVEST_MOON_DAY_EVENT_UPGRADE2 );
				_pUIMgr->CloseMessageBox( MSGCND_MOON_DAY_EVENT_GIVE_ITEM );
				CTString strMessage;
				CUIMsgBox_Info	MsgBoxInfo;
				
				strMessage = _S( 1861, "추석 이벤트 보상품을 지급 받으시겠습니까?" ); 
				MsgBoxInfo.SetMsgBoxInfo( _S( 1860,  "추석맞이 이벤트" ) , UMBS_OKCANCEL,
					UI_QUEST, MSGCND_MOON_DAY_EVENT_GIVE_ITEM );
				
				
				MsgBoxInfo.AddString( strMessage );
				_pUIMgr->CreateMessageBox( MsgBoxInfo );

			//	_pNetwork->SendChuseokExchange();
			}

		}
		break;
	case MSGLCMD_EVENT :
		{
			if( nResult == QUEST_RENUAL_EVENT )
			{
				CTString strMessage;
				_pUIMgr->CloseMessageBoxL( MSGLCMD_RENUAL_EVENT );

				strMessage = _S(2209, "낱말 맞추기 이벤트" );	 
				_pUIMgr->CreateMessageBoxL( strMessage, UI_QUEST, MSGLCMD_RENUAL_EVENT );				
				_pUIMgr->AddMessageBoxLString( MSGLCMD_RENUAL_EVENT, TRUE, strMessage, -1,0xE18600FF );				

				strMessage = _S(2210, "조합하신 낱말들을 인벤토리에 소지 하신채로 연관된 게임 타이틀을 선택하시면 완성된 낱말에 따라 보상이 지급됩니다." );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_RENUAL_EVENT, TRUE, strMessage, -1, 0xA3A1A3FF );				
				strMessage = " ";
				_pUIMgr->AddMessageBoxLString( MSGLCMD_RENUAL_EVENT, TRUE, strMessage, -1, 0xA3A1A3FF );				
				strMessage = _S(2211, "단 이미 한번 지급받은 상품에 대해서는 다시 지급이 안되므로 주의하시기 바랍니다." );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_RENUAL_EVENT, TRUE, strMessage, -1, 0xA3A1A3FF );				
				strMessage = " ";
				_pUIMgr->AddMessageBoxLString( MSGLCMD_RENUAL_EVENT, TRUE, strMessage, -1, 0xA3A1A3FF );				
				
				_pUIMgr->AddMessageBoxLString( MSGLCMD_RENUAL_EVENT, FALSE, _S(2212, "게임 타이틀 선택."  ), QUEST_RENUAL_EVENT_OK );		
				_pUIMgr->AddMessageBoxLString( MSGLCMD_RENUAL_EVENT, FALSE, _S( 1874, "취소한다."  ) );			

			}
			else if( nResult == QUEST_TREASURE_BOX )
			{
				// 서버로 메세지 보내기.
				_pNetwork->SendEventTreasureList();
			}
			else if (nResult == EVENT_OPENBETAITEMGIFT)
			{
				_pNetwork->SendUsaOpenBetaGift();
			}
			else if( nResult == QUEST_HARVEST_MOON_DAY_EVENT1 )
			{
				CTString strMessage;
				_pUIMgr->CloseMessageBoxL( MSGLCMD_HARVEST_MOON_DAY1 );

				strMessage = _S( 1860, "추석맞이 이벤트" );	
				/***** 
				_pUIMgr->CreateMessageBoxL( strMessage, UI_QUEST, MSGLCMD_HARVEST_MOON_DAY );				
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY, TRUE, strMessage, -1,0xE18600FF );				

				strMessage = _S( 1862, "단계별 작물을 다음 단계로 업그레이드를 하시려면 배양토가 필요합니다." );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY, TRUE, strMessage, -1, 0xA3A1A3FF );				
					
				strMessage = _S( 1863, "다음 단계로의 업그레이드는 일정확률로 업그레이드되며 실패할 수도 있습니다." );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY, TRUE, strMessage, -1, 0xA3A1A3FF );				

				strMessage = _S( 1864, "업그레이드된 작물은 보상품과 교환하거나 다음 단계로 업그레이드 할 수 있습니다." );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY, TRUE, strMessage, -1, 0xA3A1A3FF );				

				strMessage = _S( 1865, "보상품은 각 작물별로 다른 확률로 획득할 수있습니다." );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY, TRUE, strMessage, -1, 0xA3A1A3FF );				

				strMessage = " ";
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY, TRUE, strMessage, -1, 0xA3A1A3FF );	

				strMessage = _S( 1866, "보상품" );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY, TRUE, strMessage, -1,0xE18600FF );	

				strMessage = _S( 1867, "10,000 Nas" );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY, TRUE, strMessage, -1, 0xA3A1A3FF );	
				strMessage = _S( 1868, "50,000 Nas" );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY, TRUE, strMessage, -1, 0xA3A1A3FF );	
				strMessage = _S( 1869, "200,000 Nas" );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY, TRUE, strMessage, -1, 0xA3A1A3FF );	
				strMessage = _S( 1870, "500,000 Nas" );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY, TRUE, strMessage, -1, 0xA3A1A3FF );	
				strMessage = _S( 1871, "고급제련석 2개" );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY, TRUE, strMessage, -1, 0xA3A1A3FF );	

			
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY, FALSE, _S( 1872, "작물을 업그레이드한다."  ), QUEST_HARVEST_MOON_DAY_EVENT_UPGRADE );		
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY, FALSE, _S( 1873, "보상품을 지급받는다."  ), QUEST_HARVEST_MOON_DAY_EVENT_GIVE_ITEM );			
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY, FALSE, _S( 1874, "취소한다."  ) );			
				******/
				_pUIMgr->CreateMessageBoxL( strMessage, UI_QUEST, MSGLCMD_HARVEST_MOON_DAY1 );				
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY1, TRUE, strMessage, -1,0xE18600FF );				

				strMessage = _S(3122, "민족의 명절 추석을 맞아서 맛잇는 송편을 만들어 보시겠어요?" );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY1, TRUE, strMessage, -1, 0xA3A1A3FF );				
				strMessage = _S(3123, "다른 재료는 저한테 다 있으니 여러분은 쌀가루, 꿀, 솔잎을 가져와 주세요." );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY1, TRUE, strMessage, -1, 0xA3A1A3FF );				
				strMessage = _S(3124, "재료들은 이벤트 기간 동안 몬스터를 사냥하면 얻을 수 있으며 각각 1개씩 총 3개를 모아오시면" );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY1, TRUE, strMessage, -1, 0xA3A1A3FF );
				strMessage = _S(3125, "이동속도와 공격속도가 향상되는 특별한 송편을 1개 만들어 드리겠습니다." );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY1, TRUE, strMessage, -1, 0xA3A1A3FF );				
				strMessage = _S(3126, "그리고 송편 10개를 모아오면 오색송편으로 만들 수 있는데," );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY1, TRUE, strMessage, -1, 0xA3A1A3FF );
				strMessage = _S(3127, "오색송편은 랜디가 아주 좋아하는 송편으로 가져다 주면 보상품을 받을 수 있습니다." );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY1, TRUE, strMessage, -1, 0xA3A1A3FF );
				strMessage = " ";
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY1, TRUE, strMessage, -1, 0xA3A1A3FF );	
				strMessage = _S(3128, "단, 송편을 만들다가 실패하면 사용한 재료가 모두 사라지고, 오색송편을 만들다가 실패하면 송편 2개만 남게 됩니다." );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY1, TRUE, strMessage, -1, 0xA3A1A3FF );
				strMessage = " ";
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY1, TRUE, strMessage, -1, 0xA3A1A3FF );

				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY1, FALSE, _S(3129, "송편을 만든다."  ), QUEST_HARVEST_MOON_DAY_EVENT_UPGRADE1 );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY1, FALSE, _S(3130, "오색송편을 만든다."  ), QUEST_HARVEST_MOON_DAY_EVENT_UPGRADE2 );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY1, FALSE, _S( 1874, "취소한다."  ) );			
			}
			else if(nResult ==  EVENT_COUPON )
			{
				if(_pUIMgr->DoesMessageBoxExist( MSGCMD_EVENT_COUPON_SENDNUM )) break;
				if(_pUIMgr->DoesMessageBoxExist( MSGCMD_NULL)) break;
				
				int tv_all = (INVEN_SLOT_ROW_TOTAL*INVEN_SLOT_COL);
				if ( _pUIMgr->GetInventory()->GetItemAll() > tv_all-5) {
					CUIMsgBox_Info MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo(_S(169,"이벤트"),UMBS_OK,UI_NONE,MSGCMD_NULL);
					MsgBoxInfo.AddString(_S(116,"인벤토리 공간이 부족하여 보상을 받을 수 없습니다."));
					_pUIMgr->CreateMessageBox(MsgBoxInfo);

				}
				else{
					CUIMsgBox_Info MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo(_S(169,"이벤트"),UMBS_YESNO|UMBS_INPUTBOX,UI_QUEST,MSGCMD_EVENT_COUPON_SENDNUM);
					MsgBoxInfo.AddString(_S(2402,"해당 쿠폰 번호를 입력하면 새로운 의상을 얻을 수 있습니다."));
					MsgBoxInfo.AddString(_S(2403,"쿠폰 번호를 입력해 주십시오."));
					_pUIMgr->CreateMessageBox(MsgBoxInfo);
				}
			}
			else if( nResult == EVENT_CONNECT )
			{
				_pNetwork->SendConnectItemReq();								
			}
			else if(nResult ==  EVENT_PROMOPACK )
			{
				CTString strMessage;
				_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_PROMOPACK );

				strMessage = _S(3145, "프로모 패키지 상품 이벤트" );	
				_pUIMgr->CreateMessageBoxL( strMessage, UI_QUEST, MSGLCMD_EVENT_PROMOPACK );				
				_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_PROMOPACK, TRUE, strMessage, -1,0xE18600FF );				

				strMessage = _S(3146, "프로모 패키지 상품을 구입한 유저가 패키지에 적힌 인증 key를 입력하면 게임아이템을 보상품으로 받을 수 있는 이벤트입니다." );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_PROMOPACK, TRUE, strMessage, -1, 0xA3A1A3FF );				
				strMessage = " ";
				_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_PROMOPACK, TRUE, strMessage, -1, 0xA3A1A3FF );				
				
				strMessage = _S(3147, "프로모 패키지 이벤트 보상품" );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_PROMOPACK, TRUE, strMessage, -1, 0xA3A1A3FF );	

				strMessage = _S(3148, "1. 대형 HP, MP 확장포션 5개씩" );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_PROMOPACK, TRUE, strMessage, -1, 0xA3A1A3FF );	
				strMessage = _S(3149, "2. A~E 등급 능력치를 가진 악세사리 아이템 중 한개" );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_PROMOPACK, TRUE, strMessage, -1, 0xA3A1A3FF );	

				_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_PROMOPACK, FALSE, _S( 3150, "인증 Key 입력"  ), EVENT_PROMO_KEY_ENTER );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_PROMOPACK, FALSE, _S( 1874, "취소한다."  ) );
			}
			// wooss 070305 
			// kw : WSS_WHITEDAY_2007 ------------------------------------------------------------------------------------->>
			else if( nResult == EVENT_WHITEDAY_2007 )
			{
				CTString strMessage;
				_pUIMgr->CloseMessageBoxL( MSGLCMD_WHITEDAY_2007 );

				strMessage = _S( 3238, "화이트데이 이벤트" );	 
				_pUIMgr->CreateMessageBoxL( strMessage, UI_QUEST, MSGLCMD_WHITEDAY_2007 );				
				_pUIMgr->AddMessageBoxLString( MSGLCMD_WHITEDAY_2007, TRUE, strMessage, -1,0xE18600FF );				

				strMessage = _S( 3239, "사탕을 주고 받으며 사랑과 우정을 나누는 화이트데이를 맞아서 몬스터들이 사탕을 준다고 합니다." );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_WHITEDAY_2007, TRUE, strMessage, -1, 0xA3A1A3FF );				
				strMessage = _S( 3240, "시원한 하늘색 사탕과 달콤한 분홍색 사탕이 먹고 싶은데 가져와 주시겠어요?" );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_WHITEDAY_2007, TRUE, strMessage, -1, 0xA3A1A3FF );				
				strMessage = _S( 3241, "하늘색 사탕 5개와 분홍색 5개를 가져다 주시면 감사의 표시로 러브매직 주문 중 1가지를 무작위로 선택해서 걸어드리고," );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_WHITEDAY_2007, TRUE, strMessage, -1, 0xA3A1A3FF );
				strMessage = _S( 3242, "사탕 20개를 가져다 주시면 사탕 색상에 맞는 편지지를 드리겠습니다." );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_WHITEDAY_2007, TRUE, strMessage, -1, 0xA3A1A3FF );				
				strMessage = _S( 3243, "러브매직 주문은 일시적으로 캐릭터의 능력을 높여주는 주문이며, 편지지는 게임전체에 공개적으로 사랑의 메시지를 전할 수 있는 아이템입니다." );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_WHITEDAY_2007, TRUE, strMessage, -1, 0xA3A1A3FF );
				strMessage = _S( 3244, "※ 러브매직 주문 종류" );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_WHITEDAY_2007, TRUE, strMessage, -1, 0xA3A1A3FF );
				strMessage = _S( 3245, "바람의 러브매직 - 주문에 걸린 순간부터 10분간 이동속도가 향상된다." );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_WHITEDAY_2007, TRUE, strMessage, -1, 0xA3A1A3FF );
				strMessage = _S( 3246, "힘의 러브매직 - 주문에 걸린 순간부터 30분간 힘이 20 증가된다." );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_WHITEDAY_2007, TRUE, strMessage, -1, 0xA3A1A3FF );
				strMessage = _S( 3247, "민첩의 러브매직 - 주문에 걸린 순간부터 30분간 민첩이 20 증가된다." );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_WHITEDAY_2007, TRUE, strMessage, -1, 0xA3A1A3FF );
				strMessage = _S( 3248, "지혜의 러브매직 - 주문에 걸린 순간부터 30분간 지혜가 20 증가된다." );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_WHITEDAY_2007, TRUE, strMessage, -1, 0xA3A1A3FF );
				strMessage = _S( 3249, "체질의 러브매직 - 주문에 걸린 순간부터 30분간 체질이 50 증가된다." );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_WHITEDAY_2007, TRUE, strMessage, -1, 0xA3A1A3FF );
				strMessage = _S( 3250, "로레인의 러브매직 - 주문에 걸린 순간부터 30분간 몬스터 공격시 일정 확률로 본인 캐릭터 데미지의 10배에 해당하는 데미지를 입힌다." );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_WHITEDAY_2007, TRUE, strMessage, -1, 0xA3A1A3FF );
				strMessage = _S( 3251, "※ 이벤트 기간 : 2007년 3월 13일 ~ 3월 27일" );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_WHITEDAY_2007, TRUE, strMessage, -1, 0xA3A1A3FF );
				strMessage = _S( 3252, "※ 이벤트 기간 종료후 남아있는 사탕 및 편지지는 모두 사라집니다." );				
			
				_pUIMgr->AddMessageBoxLString( MSGLCMD_WHITEDAY_2007, FALSE, _S( 3253, "러브매직 주문을 받는다."  ), EVENT_REWARD_1 );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_WHITEDAY_2007, FALSE, _S( 3254, "하늘색 편지지로 교환한다."  ), EVENT_REWARD_2 );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_WHITEDAY_2007, FALSE, _S( 3255, "분홍색 편지지로 교환한다."  ), EVENT_REWARD_3 );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_WHITEDAY_2007, FALSE, _S( 1874, "취소한다."  ) );	
				
			}
			// --------------------------------------------------------------------------------------------------------------<<
			
			// [070705: Su-won] EVENT_SUMMER_2007
			else if( nResult == EVENT_SUMMER_2007 )
			{
				CTString strMessage;
				_pUIMgr->CloseMessageBoxL( MSGLCMD_SUMMER_2007 );

				strMessage = _S(3561, "종이접기 이벤트" );	 
				_pUIMgr->CreateMessageBoxL( strMessage, UI_QUEST, MSGLCMD_SUMMER_2007 );				
				_pUIMgr->AddMessageBoxLString( MSGLCMD_SUMMER_2007, TRUE, strMessage, -1,0xE18600FF );				

				strMessage = _S(3566, "여름방학동안 친구들과 종이접기를했던 경험을 떠올리며 어린 시절로 돌아가보는 것은 어떨까요?" );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_SUMMER_2007, TRUE, strMessage, -1, 0xA3A1A3FF );				
				strMessage = _S(3567, "색종이를 가지고 오시면 재미있는 종이 접기를 가르쳐 드리겠습니다." );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_SUMMER_2007, TRUE, strMessage, -1, 0xA3A1A3FF );
				strMessage.PrintF(" %s", _S(3565, "※ 이벤트 기간 : 2007년 7월 10일(화) ~ 8월 24일(화)") );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_SUMMER_2007, TRUE, strMessage, -1, 0xA3A1A3FF );
				strMessage = _S(3568, "※ 색종이는 종이 접기 후 소비되며, 이벤트기간 종류 후 한달 간 사용가능합니다." );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_SUMMER_2007, TRUE, strMessage, -1, 0xA3A1A3FF );
			
				_pUIMgr->AddMessageBoxLString( MSGLCMD_SUMMER_2007, FALSE, _S(3569, "종이접기"  ), EVENT_REWARD_1 );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_SUMMER_2007, FALSE, _S(3570, "색종이조합"  ), EVENT_REWARD_2 );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_SUMMER_2007, FALSE, _S( 1874, "취소한다."  ) );	
				
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// [070807: Su-won] EVENT_ADULT_OPEN
			else if( nResult == EVENT_ADULT_MAGICCARD)
			{
				if( _pUIMgr->DoesMessageBoxLExist( MSGLCMD_EVENT_ADULT_MAGICCARD ) ) break;
				
				CTString strMessage;

				_pUIMgr->CreateMessageBoxL(_S(100,"이벤트"), UI_QUEST, MSGLCMD_EVENT_ADULT_MAGICCARD);				

				strMessage = _S(3619, "제가 아끼는 중요한 놀이도구 중 하나인 매직카드에서 2장이 그만 바람에 날려서 어디론가 사라졌어요.");
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_ADULT_MAGICCARD, TRUE, strMessage, -1, 0xA3A1A3FF);
				strMessage = _S(3620, "그 카드에는 마법이 걸려있어서 잘못 건드리면 위험한 일이 벌어질지도 모르는데... 큰일이네요!");
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_ADULT_MAGICCARD, TRUE, strMessage, -1, 0xA3A1A3FF);
				strMessage = _S(3621, "혹시라도 스페이스 A카드와 하트 A카드를 발견하시면 꼭 저한테 가져다 주세요! 감사의 표시로 러브매직 마법주문을 걸어 드릴께요.");
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_ADULT_MAGICCARD, TRUE, strMessage, -1, 0xA3A1A3FF);
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_ADULT_MAGICCARD, TRUE,  CTString(" "));


				strMessage = _S(3244, "※ 러브매직 주문 종류");
				_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_ADULT_MAGICCARD, TRUE, strMessage, -1, 0xE18600FF );
				strMessage = _S(3245, "바람의 러브매직-주문에 걸린 순간부터 10분간 이동속도가 향상된다.");
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_ADULT_MAGICCARD, TRUE, strMessage, -1, 0xA3A1A3FF);
				strMessage = _S(3246, "힘의 러브매직-주문에 걸린 순간부터 30분간 힘이 20 증가된다.");
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_ADULT_MAGICCARD, TRUE, strMessage, -1, 0xA3A1A3FF);
				strMessage = _S(3247, "민첩의 러브매직-주문에 걸린 순간부터 30분간 민첩이 20증가된다.");
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_ADULT_MAGICCARD, TRUE, strMessage, -1, 0xA3A1A3FF);
				strMessage = _S(3248, "지혜의 러브매직-주문에 걸린 순간부터 30분간 지혜이 20증가된다.");
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_ADULT_MAGICCARD, TRUE, strMessage, -1, 0xA3A1A3FF);
				strMessage = _S(3249, "체질의 러브매직-주문에 걸린 순간부터 30분간 체질이 50증가된다.");
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_ADULT_MAGICCARD, TRUE, strMessage, -1, 0xA3A1A3FF);
				strMessage = _S(3250, "로레인의 러브매직-주문에 걸린 순간부터 30분간 몬스터 공격시 일정 확률로 본인 캐릭터 데미지의 10배에 해당하는 데미지를 입힌다.");
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_ADULT_MAGICCARD, TRUE, strMessage, -1, 0xA3A1A3FF);
				
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_ADULT_MAGICCARD, FALSE, _S(3622, "매직카드 보상 받기"), 0);
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_ADULT_MAGICCARD, FALSE, _S(1874,"취소한다."));

			}
			else if( nResult == EVENT_ADULT_CHANGEJOB)
			{
				if( _pUIMgr->DoesMessageBoxLExist( MSGLCMD_EVENT_ADULT_CHANGEJOB ) ) break;

				CTString strMessage;

				_pUIMgr->CreateMessageBoxL(_S(100,"이벤트"), UI_QUEST, MSGLCMD_EVENT_ADULT_CHANGEJOB);				

				strMessage = _S(3623, "전직이라는 것은 그동안 많은 경험을 쌓으셨다는 증거기도 하지만 앞으로의 모험에서 더 어려운 적을 만나게 된다는 뜻이기도 하지요.");
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_ADULT_CHANGEJOB, TRUE, strMessage, -1, 0xA3A1A3FF);
				strMessage = _S(3624, "그런 의미로 전직을 하신 분들께 제가 정말 힘들게 특별히 공수해온 루나시스 최고의 주술사가 만든 절망의 부적을 특별히 드리겠습니다.");
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_ADULT_CHANGEJOB, TRUE, strMessage, -1, 0xA3A1A3FF);
				strMessage = _S(3625, "적을 공격하기 전에 이걸 한번 써주면 절망에 빠지게 해서 다음 공격이 아주 강력하게 먹히니까 꽤 쓸모가 있을 거에요.");
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_ADULT_CHANGEJOB, TRUE, strMessage, -1, 0xA3A1A3FF);
				strMessage = _S(3626, "단! 상품의 개수는 먼저 오신 순서대로 많이 드리니 참고하세요.");
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_ADULT_CHANGEJOB, TRUE, strMessage, -1, 0xA3A1A3FF);

				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_ADULT_CHANGEJOB, FALSE, _S(3627, "전직 축하 상품 받기"), 0);
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_ADULT_CHANGEJOB, FALSE, _S(1874,"취소한다."));
			}
			else if( nResult == EVENT_ADULT_ALCHEMIST)
			{
				if( _pUIMgr->DoesMessageBoxLExist( MSGLCMD_EVENT_ADULT_ALCHEMIST ) ) break;

				CTString strMessage;

				_pUIMgr->CreateMessageBoxL(_S(100,"이벤트"), UI_QUEST, MSGLCMD_EVENT_ADULT_ALCHEMIST);				

				strMessage = _S(3628, "정말 탁월한 선택이시오. 사실 내가 그동안 심혈을 기울여서 연구한 결과로 대단한 연금술을 개발해 냈는데...");
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_ADULT_ALCHEMIST, TRUE, strMessage, -1, 0xA3A1A3FF);
				strMessage = _S(3629, "바로 헌 장비를 가지고 바로 윗단계의 새 장비로 조합해주는 특별한 기술이지요.");
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_ADULT_ALCHEMIST, TRUE, strMessage, -1, 0xA3A1A3FF);
				strMessage = _S(3630, "조합이 잘되면 제련된 새 장비를 얻을 수도 있으니 이거 정말 대단하지 않소?");
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_ADULT_ALCHEMIST, TRUE, strMessage, -1, 0xA3A1A3FF);
				strMessage = _S(3631, "너무 기분이 좋아서 기념으로 무료로 장비를 조합해주는 이벤트를 해볼까하는데, 참여해 보겠소?");
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_ADULT_ALCHEMIST, TRUE, strMessage, -1, 0xA3A1A3FF);
				strMessage = _S(3632, "이벤트 기간동안 쓰던 장비와 재료만 가지고 오면 되니 부담 가질 것 없다오, 하하.");
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_ADULT_ALCHEMIST, TRUE, strMessage, -1, 0xA3A1A3FF);
				strMessage = _S(3633, "아참, 조합을 시도하기 전에 이건 알아두시오. 아~주 운이 나쁘면 가끔 연금술이 실패가 되기도 하는데 실패하면 장비를 제외한 사용된 재료들이 모두 사라져 버릴 수도 있고,");
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_ADULT_ALCHEMIST, TRUE, strMessage, -1, 0xA3A1A3FF);
				strMessage = _S(3634, "40레벨 이상의 장비는 만들 수 없다는 것도 기억하면 좋겠소.");
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_ADULT_ALCHEMIST, TRUE, strMessage, -1, 0xA3A1A3FF);
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_ADULT_ALCHEMIST, TRUE,  CTString(" "));
				
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_ADULT_ALCHEMIST, FALSE, _S(3427, "이벤트 참여하기"), 0);
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_ADULT_ALCHEMIST, FALSE, _S(1874,"취소한다."));

			}			
			else if( nResult == EVENT_SHOOT )
			{
				_pUIMgr->CloseMessageBox( MSGCMD_EVENT_SHOOT );

				CTString strMessage;
				// Create message box of remission
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S(3658, "거침없이 쏜다!" ), UMBS_OKCANCEL, UI_QUEST, MSGCMD_EVENT_SHOOT );
				strMessage.PrintF( _S(3659, "거침없이 쏜다! 이벤트에 참여하시겠습니까?" ) );
				MsgBoxInfo.AddString( strMessage );
				_pUIMgr->CreateMessageBox( MsgBoxInfo );
			}
			// [070807: Su-won] EVENT_ADULT_OPEN
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			else if( nResult == EVENT_TG2007_FRUITFULL ) // WSS_TG2007 2007/09/14 풍년 이벤트
			{
				_pUIMgr->CreateMessageBoxL(_S(100,"이벤트"),UI_QUEST,MSGLCMD_TG2007_FRUITFULL);
				_pUIMgr->AddMessageBoxLString(MSGLCMD_TG2007_FRUITFULL, TRUE, _S( 3761,"풍년이벤트"),0x6BD2FFFF);
				_pUIMgr->AddMessageBoxLString(MSGLCMD_TG2007_FRUITFULL, TRUE, _S( 3766,"단계별로 라스트카오스 씨앗을 다음 단계로 업그레이드를 하시려면 관심아이템이 필요합니다.") );
				_pUIMgr->AddMessageBoxLString(MSGLCMD_TG2007_FRUITFULL, TRUE, _S( 3767,"다음단계로의 업그레이드는 일정 확류로 업그레이드 되며 실패할 수도 있습니다.") );
				_pUIMgr->AddMessageBoxLString(MSGLCMD_TG2007_FRUITFULL, TRUE, _S( 3768,"업그레이드된 라스트카오스 씨앗은 보상품과 교환하거나 다음 단계로 업그레이드 할 수 있습니다.") );
				_pUIMgr->AddMessageBoxLString(MSGLCMD_TG2007_FRUITFULL, TRUE, _S( 3769,"보상품은 각 단계별로 다른 확률로 획득할 수 있습니다.") );
				_pUIMgr->AddMessageBoxLString(MSGLCMD_TG2007_FRUITFULL, TRUE, _S( 1866,"보상품"),0xA3A1A3FF );
				_pUIMgr->AddMessageBoxLString(MSGLCMD_TG2007_FRUITFULL, TRUE, _S( 3770,"문스톤"),0xA3A1A3FF );
				_pUIMgr->AddMessageBoxLString(MSGLCMD_TG2007_FRUITFULL, TRUE, _S( 3771,"드랍 증폭제 3개"),0xA3A1A3FF );
				_pUIMgr->AddMessageBoxLString(MSGLCMD_TG2007_FRUITFULL, TRUE, _S( 3772,"개인창고 7일 확장카드"),0xA3A1A3FF );
				_pUIMgr->AddMessageBoxLString(MSGLCMD_TG2007_FRUITFULL, TRUE, _S( 3773,"지력의 물약"),0xA3A1A3FF );
				_pUIMgr->AddMessageBoxLString(MSGLCMD_TG2007_FRUITFULL, TRUE, _S( 3774,"건강의 물약"),0xA3A1A3FF );
 				_pUIMgr->AddMessageBoxLString(MSGLCMD_TG2007_FRUITFULL, FALSE, _S( 1872,"작물을 업그레이드 한다." ), EVENT_REWARD_1 );
				_pUIMgr->AddMessageBoxLString(MSGLCMD_TG2007_FRUITFULL, FALSE, _S( 1873,"보상품을 지급받는다." ), EVENT_REWARD_2 );
				_pUIMgr->AddMessageBoxLString(MSGLCMD_TG2007_FRUITFULL, FALSE, _S( 1874,"취소한다."));				
			}
			else if( nResult == EVENT_RED_TREASUREBOX )
			{
				int iTreasureBoxLevel =0;
				if( _pUIMgr->GetInventory()->GetItemCount(2660) )	//5Lv 붉은색 보물상자
				{
					iTreasureBoxLevel =5;

					if( _pNetwork->MyCharacterInfo.level < 5 )
					{
						_pNetwork->ClientSystemMessage( _S( 1757, "레벨이 부족하여 보물 상자를 열수 없습니다." ), SYSMSG_ERROR );
						return;
					}
				}
				else if( _pUIMgr->GetInventory()->GetItemCount(2661) )	//12Lv 붉은색 보물상자
				{
					iTreasureBoxLevel =12;

					if( _pNetwork->MyCharacterInfo.level < 12 )
					{
						_pNetwork->ClientSystemMessage( _S( 1757, "레벨이 부족하여 보물 상자를 열수 없습니다." ), SYSMSG_ERROR );
						return;
					}
				}
				else if( _pUIMgr->GetInventory()->GetItemCount(2662) )	//16Lv 붉은색 보물상자
				{
					iTreasureBoxLevel =16;

					if( _pNetwork->MyCharacterInfo.level < 16 )
					{
						_pNetwork->ClientSystemMessage( _S( 1757, "레벨이 부족하여 보물 상자를 열수 없습니다." ), SYSMSG_ERROR );
						return;
					}
				}
				else
				{
					_pNetwork->ClientSystemMessage( _S( 1758, "보물 상자가 없습니다." ), SYSMSG_ERROR );
					return;
				}

				CTString strMessage;

				_pUIMgr->CloseMessageBoxL( MSGLCMD_RED_TREASUREBOX_EVENT );
					
				_pUIMgr->CreateMessageBoxL( _S(100,"이벤트"), UI_QUEST, MSGLCMD_RED_TREASUREBOX_EVENT );			
				
				_pUIMgr->AddMessageBoxLString( MSGLCMD_RED_TREASUREBOX_EVENT, TRUE, _S(4010, "붉은색 보물상자 이벤트" ), -1, 0xE18600FF );	
				strMessage.PrintF( _S(4018, "%d Lv 붉은색 보물상자를 열면 아래 아이템이 지급됩니다.  %d Lv 붉은색 보물상자를 열어보시겠습니까?" ), iTreasureBoxLevel, iTreasureBoxLevel );	
				_pUIMgr->AddMessageBoxLString( MSGLCMD_RED_TREASUREBOX_EVENT, TRUE, strMessage, -1, 0xA3A1A3FF );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_RED_TREASUREBOX_EVENT, TRUE, CTString("  "), -1, 0xA3A1A3FF );
				strMessage.PrintF( _S(4019, "%d Lv 붉은색 보물상자 지급 품목"), iTreasureBoxLevel);	
				_pUIMgr->AddMessageBoxLString( MSGLCMD_RED_TREASUREBOX_EVENT, TRUE, strMessage, -1, 0xE18600FF );			


				strMessage.PrintF( _S( 61, "%s %d개" ), _pNetwork->GetItemName(2658), 5);
				_pUIMgr->AddMessageBoxLString( MSGLCMD_RED_TREASUREBOX_EVENT, TRUE, strMessage, -1, 0xA3A1A3FF );			
				strMessage.PrintF( _S( 61, "%s %d개" ), _pNetwork->GetItemName(2659), 5);
				_pUIMgr->AddMessageBoxLString( MSGLCMD_RED_TREASUREBOX_EVENT, TRUE, strMessage, -1, 0xA3A1A3FF );
				
				if( iTreasureBoxLevel == 12 )
				{
					//strMessage.PrintF( _S(4016, "교복세트(%d일)"), 7);
					//_pUIMgr->AddMessageBoxLString( MSGLCMD_RED_TREASUREBOX_EVENT, TRUE, strMessage, -1, 0xA3A1A3FF );			
					//strMessage.PrintF( _S( 61, "%s %d개" ), _pNetwork->GetItemName(2664), 4);
					strMessage.PrintF( _S( 61, "%s %d개" ), _pNetwork->GetItemName(2860), 10);
					_pUIMgr->AddMessageBoxLString( MSGLCMD_RED_TREASUREBOX_EVENT, TRUE, strMessage, -1, 0xA3A1A3FF );
				}
				
				_pUIMgr->AddMessageBoxLString( MSGLCMD_RED_TREASUREBOX_EVENT, FALSE, _S(4020, "붉은색 보물상자를 연다." ), 1 );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_RED_TREASUREBOX_EVENT, FALSE, _S( 1220, "취소한다." ) );	
			}
		}
		break;
	case MSGLCMD_QUEST_REQ:
		{
			
			if( nResult == QUEST_TALK )		// Quest
			{
				CUIQuestBook::TalkWithNPC();
/*
				if(g_bHasQuest)
				{
					CUIQuestBook::TalkWithNPC();
*/
/*
					_pUIMgr->RearrangeOrder( UI_QUEST, TRUE );
					
					InitQuest();
					//m_nSelQuestID	= -1;	
					//m_iNpcIndex		= iMobIndex;
					LockQuest(FALSE);
					
					// FIXME : 혹시 몰라서...
					//m_btnOK.SetEnable(TRUE);
					m_btnCancel.SetEnable(TRUE);
					m_btnClose.SetEnable(TRUE);
					//_pUIMgr->SetCSFlagOn( CSF_QUEST );
*/
/*
				}
				else
				{
					_pUIMgr->CloseMessageBox( MSGCMD_QUEST_NOTIFY );
					CUIQuestBook::UnlockQuest();
					CTString strMessage;
					// Create message box of remission
					CUIMsgBox_Info	MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo( _S( 99, "퀘스트" ), UMBS_OK,			
						UI_CHARACTERINFO, MSGCMD_QUEST_NOTIFY );
					strMessage.PrintF( _S( 584, "수행할 수 있는 퀘스트가 없습니다." ) );					
					MsgBoxInfo.AddString( strMessage );
					_pUIMgr->CreateMessageBox( MsgBoxInfo );
				}
*/
			}
			else if( g_bHasEvent && nResult == QUEST_EVENT )		// Event
			{
				CTString strMessage;
				_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT );
				
				strMessage = _S( 1875, "이벤트 선택창" );
				_pUIMgr->CreateMessageBoxL( strMessage, UI_QUEST, MSGLCMD_EVENT );				
				_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT, TRUE, strMessage, -1,0xE18600FF );				

				_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT, TRUE, _S( 1876, "진행할 이벤트를 선택하여 주십시오." ), -1, 0xA3A1A3FF );	

//				if(g_iTempFlag&0x00020000 )
				// wooss 070305 ---------------------------------<< 
				// kw : WSS_WHITEDAY_2007 , WSS_EVENT_LOD 
				if( IS_EVENT_ON(TEVENT_TREASUREBOX)  )
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT, FALSE, _S( 1877, "보물상자 이벤트." ), QUEST_TREASURE_BOX );	

				#ifdef OPENBETA_EVENTITEM_GIFT
				if ( g_iCountry==BRAZIL )
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT, FALSE, _S( 3068, "이벤트 무기 지급"), EVENT_OPENBETAITEMGIFT);
				#endif

				#ifdef QUEST_HARVEST_MOON_DAY
				//if(g_iTempFlag&0x00040000 )
					//_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT, FALSE, _S( 1878, "추석이벤트." ), QUEST_HARVEST_MOON_DAY_EVENT );	
				if (IS_EVENT_ON(TEVENT_CHUSEOK_2006))
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT, FALSE, _S( 1860, "추석맞이 이벤트." ), QUEST_HARVEST_MOON_DAY_EVENT1 );
				#endif

				#ifdef RENUAL_EVENT			// 2P4P Renual Event  10/6 Update
						_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT, FALSE, _S(2213, "이판사판 오픈 이벤트." ), QUEST_RENUAL_EVENT );	
				#endif
				if(g_iTempFlag&0x00200000)
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT, FALSE, _S(2404, "쿠폰 이벤트" ), EVENT_COUPON );
				if(g_iTempFlag&0x00400000)
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT, FALSE, _S(2509, "접속 이벤트 상품 받기" ), EVENT_CONNECT );

				#ifdef PROMO_PACK
				if (g_iCountry==MALAYSIA || g_iCountry==HONGKONG || g_iCountry==USA || g_iCountry==GERMANY || g_iCountry == POLAND)
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT, FALSE, _S(3145, "프로모 패키지상품 이벤트" ), EVENT_PROMOPACK );
				#endif
				// wooss 070305 
				// kw : WSS_WHITEDAY_2007 , WSS_EVENT_LOD --------------------------------------------------------------------->>
				if( IS_EVENT_ON(TEVENT_WHITEDAY2007)  )
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT, FALSE, _S(2520, "화이트데이 이벤트" ), EVENT_WHITEDAY_2007 );
				// ------------------------------------------------------------------------------------------------------------<<
		
				// [070705: Su-won] EVENT_SUMMER_2007
				if( IS_EVENT_ON(TEVENT_SUMMER_2007)  )
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT, FALSE, _S(3561, "종이 접기 이벤트" ), EVENT_SUMMER_2007 );

				if( IS_EVENT_ON(TEVENT_ADULT_OPEN) )
				{
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT, FALSE, _S(3658, "거침없이 쏜다!"), EVENT_SHOOT);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT, FALSE, _S(3617, "[매직카드를 찾아라!] 이벤트 참여"), EVENT_ADULT_MAGICCARD);
					//_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, FALSE, _S(3618, "[전직 달성! 절망의 부적을 찾아라!] 이벤트 참여"), EVENT_ADULT_CHANGEJOB);
					//_pUIMgr->AddMessageBoxLString(MSGLCMD_QUEST_REQ, FALSE, _S(3648, "성인들만의 특권"), EVENT_ADULT_CHANGEJOB);
				}
				// WSS_TG2007 2007/09/17 풍년 이벤트
				if( IS_EVENT_ON(TEVENT_TG2007_FRUITFULL)  )
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT, FALSE, _S( 3761,"풍년이벤트"), EVENT_TG2007_FRUITFULL );


				_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT, FALSE, _S( 1874, "취소한다." ) );		
/*
#ifdef QUIZ_EVENT
				_pUIMgr->GetQuiz()->OpenQuiz( );
#else
				_pUIMgr->CloseMessageBox( MSGCMD_EVENT_PRIZE );
				
				// Create message box of remission
				//CTString	strMessage;
				//CUIMsgBox_Info	MsgBoxInfo;	
				//MsgBoxInfo.SetMsgBoxInfo(  _S( 585, "이벤트" ), UMBS_OKCANCEL, UI_CHARACTERINFO, MSGCMD_EVENT_PRIZE);		
				//strMessage.PrintF( _S( 586, "이벤트 보상을 요청하시겠습니까?" ) );	
				//MsgBoxInfo.AddString( strMessage );
				//_pUIMgr->CreateMessageBox( MsgBoxInfo );			
				// 이벤트 기간이 아닙니다.
				CUIMsgBox_Info	MsgBoxInfo;	
				MsgBoxInfo.SetMsgBoxInfo( _S( 585, "이벤트" ), UMBS_OK, UI_NONE, MSGCMD_EVENT_PRIZE);		
				MsgBoxInfo.AddString( _S( 945, "지금은 이벤트 기간이 아닙니다." ) );	
				_pUIMgr->CreateMessageBox( MsgBoxInfo );
#endif
*/				
			}
			else if( nResult == QUEST_CHANGEWEAPON )
			{
				_pUIMgr->CloseMessageBoxL( MSGLCMD_CHANGEWEAPON_EVENT_REQ );
				_pUIMgr->CreateMessageBoxL( _S( 1204, "무기교체 이벤트" ), UI_QUEST, MSGLCMD_CHANGEWEAPON_EVENT_REQ );				
				_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_EVENT_REQ, TRUE, _S( 1205, "이벤트로 무기 교체를 할 경우 사용하던 무기의 제련값이나, 블러드 옵션은 그대로 교체 무기에 옮겨집니다." ), -1, 0xA3A1A3FF );				
				_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_EVENT_REQ, TRUE, CTString("  "), -1, 0xA3A1A3FF );				
				_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_EVENT_REQ, TRUE, _S( 1206, "교체되는 무기는 해당 클래스가 사용하던 무기의 상반된 무기만 가능합니다." ), -1, 0xA3A1A3FF );				
				_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_EVENT_REQ, TRUE, _S( 1207, "(Ex > 타이탄 : 대검 <-> 도끼로만 교체)" ), -1, 0xA3A1A3FF );				
				_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_EVENT_REQ, TRUE, CTString("  "), -1, 0xA3A1A3FF );				
				_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_EVENT_REQ, TRUE, _S( 1208, "교체 가능 회수는 이벤트 기간에 한해서 제한 없이 할 수 있습니다." ), -1, 0xA3A1A3FF );				
				_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_EVENT_REQ, TRUE, CTString("  "), -1, 0xA3A1A3FF );				
				_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_EVENT_REQ, TRUE, _S( 1209, "로그와 힐러의 경우 옵션이 교체되는 무기에 적합하게 변경되니 홈페이지를 참고하시기 바랍니다." ), -1, 0xA3A1A3FF );				
				_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_EVENT_REQ, FALSE, _S( 1210, "무기를 교체한다." ), EVENT_CHANGEWEAPON );		
				_pUIMgr->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_EVENT_REQ, FALSE, _S( 1220, "취소한다." ) );					
				
			}
			// FIXME : 싱글 던젼 입장 부분...
			// FIXME : 죄다 하드 코딩~~~!!!
			// FIXME : 코드 중복도 심함....
			else if( nResult == QUEST_KILL_BOSS )			// 발록 격파.
			{
				// FIXME : Hardcoding 인덱스
				const int iQuestIndex = 13;
				if( CQuestSystem::Instance().CanIDoQuest( iQuestIndex ) )
				{
					if( CQuestSystem::Instance().GetDynamicDataByQuestIndex( iQuestIndex ) == NULL )
					{										
						_pUIMgr->GetQuestBookNew()->OpenQuestBook( iQuestIndex );
					}
					else
					{
						CTString strSysMessage;
						strSysMessage.PrintF( _S( 1687, "이미 수행중인 퀘스트입니다."  ) );		
						_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR );
					}
				}
				else
				{
					CTString strSysMessage;
					strSysMessage.PrintF( _S( 1688, "조건이 맞지 않습니다."  ) );		
					_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR );
				}
			}	
			else if( nResult == QUEST_SAVE_PRINCESS )		// 공주 구출
			{
				// FIXME : Hardcoding 인덱스
				const int iQuestIndex = 14;
				if( CQuestSystem::Instance().CanIDoQuest( iQuestIndex ) )
				{
					if( CQuestSystem::Instance().GetDynamicDataByQuestIndex( iQuestIndex ) == NULL )
					{										
						_pUIMgr->GetQuestBookNew()->OpenQuestBook( iQuestIndex );
					}
					else
					{
						CTString strSysMessage;
						strSysMessage.PrintF( _S( 1687, "이미 수행중인 퀘스트입니다."  ) );		
						_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR );
					}
				}
				else
				{
					CTString strSysMessage;
					strSysMessage.PrintF( _S( 1688, "조건이 맞지 않습니다."  ) );		
					_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR );
				}
			}
			else if( nResult == QUEST_AZAKA_RAVINE )		// 아자카 협곡
			{
				// FIXME : Hardcoding 인덱스
				const int iQuestIndex = 104;
				if( CQuestSystem::Instance().CanIDoQuest( iQuestIndex ) )
				{
					if( CQuestSystem::Instance().GetDynamicDataByQuestIndex( iQuestIndex ) == NULL )
					{										
						_pUIMgr->GetQuestBookNew()->OpenQuestBook( iQuestIndex );
					}
					else
					{
						CTString strSysMessage;
						strSysMessage.PrintF( _S( 1687, "이미 수행중인 퀘스트입니다."  ) );		
						_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR );
					}
				}
				else
				{
					CTString strSysMessage;
					strSysMessage.PrintF( _S( 1688, "조건이 맞지 않습니다."  ) );		
					_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR );
				}
			}
			else if( nResult == QUEST_GATE_OF_DIMENSION )	// 차원의 문
			{
				_pUIMgr->CloseMessageBoxL( MSGLCMD_GATE_OF_DIMENSION );
				_pUIMgr->CreateMessageBoxL( _S( 1686, "차원의 문" ), UI_QUEST, MSGLCMD_GATE_OF_DIMENSION );				
				_pUIMgr->AddMessageBoxLString( MSGLCMD_GATE_OF_DIMENSION, TRUE, _S( 1689, "저에게 어떤 볼일이 있으신가요?" ), -1, 0xA3A1A3FF );		
				_pUIMgr->AddMessageBoxLString( MSGLCMD_GATE_OF_DIMENSION, TRUE, _S( 1690, "차원의 문은 많은 몬스터들이 차원의 문을 통해 아이리스 대륙으로 넘어오기 때문에 위험한 곳입니다." ), -1, 0xA3A1A3FF );	
				_pUIMgr->AddMessageBoxLString( MSGLCMD_GATE_OF_DIMENSION, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_GATE_OF_DIMENSION, TRUE, _S( 1691, "차원에 문에서 돌아 오시면 저를 다시한번 찾아 주세요 당신이 아이리스 대륙에 기여하는 정도를 확인하실 수 있을 겁니다." ), -1, 0xA3A1A3FF );	
				_pUIMgr->AddMessageBoxLString( MSGLCMD_GATE_OF_DIMENSION, TRUE, _S( 1692, "물론 그 기여도에 따라 란돌성주가 내리는 포상도 받으실 수 있을 겁니다." ), -1, 0xA3A1A3FF );		
				_pUIMgr->AddMessageBoxLString( MSGLCMD_GATE_OF_DIMENSION, FALSE, _S( 1693, "차원의 문 입장" ), GOD_ENTER );	
				_pUIMgr->AddMessageBoxLString( MSGLCMD_GATE_OF_DIMENSION, FALSE, _S( 1694, "차원의 문 순위확인" ), GOD_RANKING );		
				_pUIMgr->AddMessageBoxLString( MSGLCMD_GATE_OF_DIMENSION, FALSE, _S( 1695, "보상"), GOD_PRIZE );		
			}
			else if(nResult == QUEST_EXCHANGE_MONEY)
			{
				
				_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_2PAN4PAN_ONE );
				_pUIMgr->CreateMessageBoxL( _S( 1875, "이벤트 선택창" ), UI_QUEST, MSGLCMD_EVENT_2PAN4PAN_ONE );	
				_pUIMgr->AddMessageBoxLString( 
					MSGLCMD_EVENT_2PAN4PAN_ONE, 
					TRUE,
					_S(2214,"2pan4pan 맞고 카드와 2pan4pan 포커 카등는 게임포털 사이트인 2pan4pan의 맞고, 포커 게임에서 사용할 수 있는 게임 머니로 교환할 수 있습니다." ),
					-1, 0xA3A1A3FF );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_2PAN4PAN_ONE, TRUE, CTString("\n\n") , -1,0xA3A1A3FF );		
				_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_2PAN4PAN_ONE, TRUE, _S(2215, "2pan4pan 맞고카드 :          : 맞고머니" ), -1, 0xFFA41CFF );		
				_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_2PAN4PAN_ONE, TRUE, _S(2216, "2pan4pan 포커카드 :          : 포커머니" ), -1, 0xFFA41CFF );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_2PAN4PAN_ONE, FALSE, _S(2217,"2pan4pan 맞고카드 교환"),EXCH_FIVE);
				_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_2PAN4PAN_ONE, FALSE, _S(2218,"2pan4pan 포커카드 교환"),EXCH_FOUR);
				_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_2PAN4PAN_ONE, FALSE,_S( 1220, "취소한다." ));
				
			}
			else if(nResult == QUEST_OPEN_TREASURE)
			{
				_pNetwork->SendEvent24(0,MSG_EVENT_2PAN4PAN_BOX_CHECK);

			}
			else if(nResult == QUEST_CHARACTER_CARD)
			{
				_pNetwork->SendEvent24(0,MSG_EVENT_2PAN4PAN_GOODS_CHECK);
								
			}
			
			else if(nResult == EVENT_NEWYEAR1)
			{
				if(_pUIMgr->DoesMessageBoxLExist( MSGLCMD_EVENT_NEWYEAR1 )) break;
				_pUIMgr->CreateMessageBoxL(_S (1875,"이벤트 선택창"), UI_QUEST,MSGLCMD_EVENT_NEWYEAR1);
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NEWYEAR1, TRUE,	_S(2286,"2005 크리스마스 이벤트"),-1,0xA3A1A3FF);
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NEWYEAR1, TRUE, 
					_S(2288,"꿈과 희망 아이템을 조합하여 보상을 받으실 수 있습니다.\n보상 물품은 홈페이지를 참조하세요."));
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NEWYEAR1, TRUE,	_S(2305,"꿈과 희망 이벤트 보상 조건"),-1,0xA3A1A3FF);
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NEWYEAR1, TRUE,	
					_S(2289,"꿈(4개) + 희망(1개)\n꿈(8개) + 희망(2개)\n꿈(16개) + 희망(4개)\n꿈(24개) + 희망(6개)\
					\n꿈(40개) + 희망(10개)\n꿈(56개) + 희망(14개)\n꿈(80개) + 희망(20개)"));
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NEWYEAR1, FALSE, _S(2290,"꿈과 희망 보상품을 지급 받는다."),EVENT_NEWYEAR1_REWARD);
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NEWYEAR1, FALSE, _S(1220,"취소한다."));
								
			}
			else if(nResult == EVENT_NEWYEAR2)
			{
				if(_pUIMgr->DoesMessageBoxLExist( MSGLCMD_EVENT_NEWYEAR2 )) break;
				_pUIMgr->CreateMessageBoxL(_S (1875,"이벤트 선택창"), UI_QUEST,MSGLCMD_EVENT_NEWYEAR2);
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NEWYEAR2, TRUE,	_S(2287,"2006 신년 인내의 열매 이벤트"),-1,0xA3A1A3FF);
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NEWYEAR2, TRUE, 
					_S(2291,"이벤트 기간동안 유저들은 자신의 캐릭터 사냥 시간에 따른 보상품을 지급 받으실 수 있습니다.\n보상품은 이벤트 기간이 끝난 다음부터 지급됩니다."));
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NEWYEAR2, TRUE,	_S(2292,"인내의 열매 이벤트 보상품"),-1,0xA3A1A3FF);
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NEWYEAR2, TRUE,	
					_S(2293,"10시간          100,000 Nas\n20시간          200,000 Nas\n30시간      고급 제련석 1개"));
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NEWYEAR2, FALSE, _S(2294,"접속시간 확인"),EVENT_NEWYEAR2_TIMECHECK);
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NEWYEAR2, FALSE, _S(2295,"인내의 열매 보상품을 지급 받는다."),EVENT_NEWYEAR2_REWARD);
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NEWYEAR2, FALSE, _S(1220,"취소한다."));

			}
			else if(nResult == EVENT_FIND_FRIEND)
			{
				if(_pUIMgr->DoesMessageBoxLExist( MSGLCMD_EVENT_FIND_FRIEND )) break;
				_pUIMgr->CreateMessageBoxL(_S (1875,"이벤트 선택창"), UI_QUEST,MSGLCMD_EVENT_FIND_FRIEND);
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_FIND_FRIEND, TRUE,	_S( 2433, "친구 찾기 이벤트"),-1,0xA3A1A3FF);
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_FIND_FRIEND, TRUE, 	_S( 2480, "보고 싶은 친구를 등록하세요."));
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_FIND_FRIEND, TRUE, 
					_S( 2481, "친구가 다시 돌아와 게임을 하면 친구는 60시간동안 사냥중에 2배의 경험치를 얻게 되고 친구가 일정시간 전투를 하면 친구와 함께 보상을 받을 수 있는 이벤트 입니다."));
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_FIND_FRIEND, TRUE,	_S( 2482, "친구 찾기 이벤트 보상품"),-1,0xA3A1A3FF);
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_FIND_FRIEND, TRUE,	
					_S( 2483,"친구가 다시 돌아와 30시간 이상 사냥을 하게 되면 휴면 캐릭은 고급 제련석 2개가 친구를 등록한 캐릭터은 고급제련석 1개가 지급됩니다."));
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_FIND_FRIEND, FALSE, _S( 2484, "친구 등록"),EVENT_FF_REG);
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_FIND_FRIEND, FALSE, _S( 2485, "휴면캐릭 사냥시간 확인"),EVENT_FF_TIMECHECK);
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_FIND_FRIEND, FALSE, _S( 2486, "보상가능 확인 및 지급"),EVENT_FF_REWARD);
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_FIND_FRIEND, FALSE, _S(1220,"취소한다."));

			}
			else if( nResult == EVENT_CLOTHES_EXCHANGE ) // 060322 eons
			{
				if( _pUIMgr->DoesMessageBoxLExist( MSGLCMD_CLOTHES_EXCHANGE ) ) break;
				_pUIMgr->CreateMessageBoxL( _S( 2537, "전통의상 교환 이벤트" ), UI_QUEST, MSGLCMD_CLOTHES_EXCHANGE );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_CLOTHES_EXCHANGE, TRUE,
					_S( 2538, "전통의상으로 교환하기 위해서는 벚꽃 봉우리가 필요합니다." ) );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_CLOTHES_EXCHANGE, TRUE,
					_S( 2539, "상의, 하의 : 벚꽃 봉우리 18개" ), -1, 0xA3A1A3FF );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_CLOTHES_EXCHANGE, TRUE,
					_S( 2540, "장갑, 신발 : 벚꽃 봉우리 12개" ), -1, 0xA3A1A3FF );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_CLOTHES_EXCHANGE, TRUE,
					_S( 2541, "투구 : 벚꽃 봉우리 7개" ), -1, 0xA3A1A3FF );

				_pUIMgr->AddMessageBoxLString( MSGLCMD_CLOTHES_EXCHANGE, FALSE, _S( 2542, "상의, 하의" ), COAT_PANTS );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_CLOTHES_EXCHANGE, FALSE, _S( 2543, "장갑, 신발" ), GLOVE_FOOTWEAR );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_CLOTHES_EXCHANGE, FALSE, _S( 2544, "투구" ), HELMET );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_CLOTHES_EXCHANGE, FALSE, _S( 1220, "취소한다." ) );
			}
			else if( nResult == EVENT_OX_GOZONE )	// O.X 이벤트
			{
				CTString strSysMessage;
				CUIMsgBox_Info	MsgBoxInfo;

				MsgBoxInfo.SetMsgBoxInfo( _S( 169, "이벤트" ), UMBS_USER_12, UI_QUEST, MSGLCMD_EVENT_OXQUIZ, MSGBOX_WIDTH + 20 );
				MsgBoxInfo.SetUserBtnName( _S( 191, "확인" ), _S( 139, "취소" ) );

				strSysMessage.PrintF( _S(2617, "O.X 이벤트 존으로 입장하시겠습니까?" ) );
				MsgBoxInfo.AddString( strSysMessage, 0xF3F3F3FF );

				_pUIMgr->CreateMessageBox( MsgBoxInfo );
			}
			else if( nResult == WIN_SELECT )	// 우승국 알아 맞추기
			{
				if( _pUIMgr->DoesMessageBoxLExist( MSGCMD_WORLDCUP_EVENT1 ) ) break;
				_pUIMgr->CreateMessageBoxL(  _S( 169, "이벤트" ), UI_QUEST, MSGCMD_WORLDCUP_EVENT1 );
				_pUIMgr->AddMessageBoxLString( MSGCMD_WORLDCUP_EVENT1, TRUE,
					_S( 2797, "1. 우승국가 맞추기 이벤트" ) , -1 , 0xFFE591FF );
				_pUIMgr->AddMessageBoxLString( MSGCMD_WORLDCUP_EVENT1, TRUE,
					_S( 2798, "우승컵을 모아 축구대회 본선 참가 32개국 중 우승이 예상되는 국가의 국기와 교환하면 대회가 끝난 이후 결과에 따라 보상 받을 수 있습니다." ) );
				_pUIMgr->AddMessageBoxLString( MSGCMD_WORLDCUP_EVENT1, TRUE,
					_S( 2799, "※ 우승 확률에 따라 필요한 우승컵의 개수가 다릅니다." ), -1 , 0xFFE591FF );
				_pUIMgr->AddMessageBoxLString( MSGCMD_WORLDCUP_EVENT1, TRUE,
					_S( 2800, "※ 국기교환기간 : 6월9일 ~ 6월 30일" ), -1 , 0xFFE591FF );
				_pUIMgr->AddMessageBoxLString( MSGCMD_WORLDCUP_EVENT1, TRUE, CTString (""));
				_pUIMgr->AddMessageBoxLString( MSGCMD_WORLDCUP_EVENT1, TRUE,
					_S( 2801, "[보상품]" ), -1, 0xA3A1A3FF );
				_pUIMgr->AddMessageBoxLString( MSGCMD_WORLDCUP_EVENT1, TRUE,
					_S( 2802, "1위 : 고급제련석 3개" ), -1, 0xA3A1A3FF );
				_pUIMgr->AddMessageBoxLString( MSGCMD_WORLDCUP_EVENT1, TRUE,
					_S( 2803, "2위 : 고급제련석 1개" ), -1, 0xA3A1A3FF );
				_pUIMgr->AddMessageBoxLString( MSGCMD_WORLDCUP_EVENT1, TRUE,
					_S( 2804, "3위 : 300,000나스" ), -1, 0xA3A1A3FF );
				_pUIMgr->AddMessageBoxLString( MSGCMD_WORLDCUP_EVENT1, TRUE,
					_S( 2805, "그외 : 문스톤 5개" ), -1, 0xA3A1A3FF );
				_pUIMgr->AddMessageBoxLString( MSGCMD_WORLDCUP_EVENT1, FALSE, _S( 2806, "우승컵을 국기로 교환한다." ), WORLDCUP_EVENT1_EXCHANGE );
				_pUIMgr->AddMessageBoxLString( MSGCMD_WORLDCUP_EVENT1, FALSE, _S( 2807, "보상품을 지급 받는다" ), WORLDCUP_EVENT1_RECEIVE );
				_pUIMgr->AddMessageBoxLString( MSGCMD_WORLDCUP_EVENT1, FALSE, _S( 1220, "취소한다." ) );
			}
			else if( nResult == EVENT_GOLDENBALL )
			{
				if( _pUIMgr->DoesMessageBoxLExist( MSGCMD_WORLDCUP_GOLDENBALL ) ) break;
				
				if( !m_bStartGoldenBall && !m_bGoldenBallRequital )
				{
					_pNetwork->ClientSystemMessage( _S( 945, "지금은 이벤트 기간이 아닙니다." ), SYSMSG_ERROR );
					return;
				}

				CTString strMessageA, strMessageB, strMessageC;
				strMessageA.PrintF( _S( 2808, "현재 골든볼 이벤트는 %s VS %s 의 축구 경기 입니다." ), m_strTeamA, m_strTeamB );
				strMessageB.PrintF( _S( 2809, "%s VS %s 골든 볼 응모" ), m_strTeamA, m_strTeamB );
				strMessageC.PrintF( _S( 2810, "%s VS %s 골든 볼 보상" ), m_strTeamA, m_strTeamB );

				_pUIMgr->CreateMessageBoxL( _S( 169, "이벤트" ), UI_QUEST, MSGCMD_WORLDCUP_GOLDENBALL );
				_pUIMgr->AddMessageBoxLString( MSGCMD_WORLDCUP_GOLDENBALL, TRUE, strMessageA );
				_pUIMgr->AddMessageBoxLString( MSGCMD_WORLDCUP_GOLDENBALL, TRUE,
					_S( 2811, "자신이 예측한 결과와 경기 결과가 같을 경우 상품을 지급합니다." ) );
				_pUIMgr->AddMessageBoxLString( MSGCMD_WORLDCUP_GOLDENBALL, TRUE, CTString( " " ) );
				
				if( m_bGoldenBallRequital ) // 골든볼 보상중이라면
				{// 경기결과를 표시
					strMessageA.PrintF( _S( 2791, "%s VS %s 축구경기의 결과는 %d : %d 입니다." ),
						_pUIMgr->GetQuest()->GetStrTeamA(), _pUIMgr->GetQuest()->GetStrTeamB(),
						_pUIMgr->GetQuest()->GetScoreTeamA(), _pUIMgr->GetQuest()->GetScoreTeamB() );

					_pUIMgr->AddMessageBoxLString( MSGCMD_WORLDCUP_GOLDENBALL, TRUE, strMessageA, 0x6BD2FFFF );
				}

				_pUIMgr->AddMessageBoxLString( MSGCMD_WORLDCUP_GOLDENBALL, FALSE, strMessageB, MSGCMD_WORLDCUP_GOLDENBALL_ENTRY );
				_pUIMgr->AddMessageBoxLString( MSGCMD_WORLDCUP_GOLDENBALL, FALSE, strMessageC, MSGCMD_WORLDCUP_GOLDENBALL_REQUITAL );
				_pUIMgr->AddMessageBoxLString( MSGCMD_WORLDCUP_GOLDENBALL, FALSE, _S( 1220, "취소한다." ) );
			}
			else if( nResult == EVENT_RAINYDAY) 
			{
				if( _pUIMgr->DoesMessageBoxLExist( MSGLCMD_RAINYDAY ) ) break;
				_pUIMgr->CreateMessageBoxL( _S(2753, "빗방울 이벤트" ), UI_QUEST, MSGLCMD_RAINYDAY );

				_pUIMgr->AddMessageBoxLString( MSGLCMD_RAINYDAY, TRUE,
					_S(2754, "촉촉한 빗방울이 쏟아지는 장마기간 동안 빗방울 이벤트가 진행되고 있습니다." ) );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_RAINYDAY, TRUE,
					_S(2755, "여러분이 습득하신 빗방울이나 빗물병을 가져 오시면 보상아이템품목 중 1개 품목 일정확률로 드립니다." ) );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_RAINYDAY, TRUE,
					_S(2756, "빗방울 10개를 가져 오시면 이벤트 보상을 받으실 수 있습니다." ));
				_pUIMgr->AddMessageBoxLString( MSGLCMD_RAINYDAY, TRUE,
					_S(2757, "빗물병 1개는 빗방울 5개와 같으며, 교환시에는 빗물병이 우선적으로 교환되고 잔 여분에 대하여 빗방울이 교환됩니다." ));

				_pUIMgr->AddMessageBoxLString( MSGLCMD_RAINYDAY, TRUE,	_S(2758,  "보상 아이템" ), -1, 0xA3A1A3FF );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_RAINYDAY, TRUE,	_S( 2759,"사탕 5개" ), -1, 0xA3A1A3FF );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_RAINYDAY, TRUE,	_S(  2760,"푸른넨,케르 셋트" ), -1, 0xA3A1A3FF );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_RAINYDAY, TRUE,	_S(  2761,"붉은넨,케르 셋트" ), -1, 0xA3A1A3FF );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_RAINYDAY, TRUE,	_S(  2762,"대형 회복제 5개" ), -1, 0xA3A1A3FF );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_RAINYDAY, TRUE,	_S(  2763,"노력의 스크롤 1개" ), -1, 0xA3A1A3FF );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_RAINYDAY, TRUE,	_S(  2764,"행운의 스크롤 1개" ), -1, 0xA3A1A3FF );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_RAINYDAY, TRUE,	_S(  2765,"고급 제련석 1개" ), -1, 0xA3A1A3FF );
			
				_pUIMgr->AddMessageBoxLString( MSGLCMD_RAINYDAY, FALSE, _S( 2766,"보상물품으로 교환한다." ), 1 );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_RAINYDAY, FALSE, _S( 1220, "취소한다." ) );
			}
			else if( nResult == EVENT_BUDDHISM )
			{
				if( _pUIMgr->DoesMessageBoxLExist( MSGLCMD_BUDDHISM_EVENT ) ) break;
				_pUIMgr->CreateMessageBoxL( _S( 2913, "불교 촛불 축제 이벤트" ), UI_QUEST, MSGLCMD_BUDDHISM_EVENT );
				/*************************************
				// 2006년
				_pUIMgr->AddMessageBoxLString( MSGLCMD_BUDDHISM_EVENT, TRUE, _S( 2919, "그대여!!!" ) );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_BUDDHISM_EVENT, TRUE,
					_S( 2920, "축제에 필요한 양초와 성수병을 받으려면 각종 색깔의 모래가 든 봉투를 모아 와야만 하네." ) );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_BUDDHISM_EVENT, TRUE,
					_S( 2921, "4가지 색깔의 모래봉투를 각각 5개씩 20개를 모아오면 양초 3개, 각각 10개씩 40개를 모아오면 성수 2병과 교환해 주도록 하지." ) );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_BUDDHISM_EVENT, TRUE,
					_S( 2922, "양초는 10분간 공격력 20% 증가시켜주고, 성수병은 20분간 경험치 30%증가 시켜 준다네." ) );
				/**************************************/

				/********************************************/
				// 2007년
				_pUIMgr->AddMessageBoxLString( MSGLCMD_BUDDHISM_EVENT, TRUE,
					_S(3590, "각종 색깔의 모래가 든 봉투를 모아오면 아이템을 드립니다." ) );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_BUDDHISM_EVENT, TRUE,
					_S(3591, "4가지 색깔의 모래봉투를 각각 10개씩 모아오면 행운의 고급제련석을, 각각 7개씩 모아오면 고급제련석을, 각각 5개씩 모아오면 성수병을, 각각 3개씩 모아오면 드롭율 증폭제를, 각각 2개씩 모아오면 양초를 드리고," ) );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_BUDDHISM_EVENT, TRUE,
					_S(3592, "적색과 황색 각각 2개씩을 모아오면 구원의 눈물을, 녹색과 남색을 각각 2개씩 모아오면 용서의 눈물을 드립니다." ) );

				_pUIMgr->AddMessageBoxLString( MSGLCMD_BUDDHISM_EVENT, TRUE,
					_S( 2923, "보상으로 어떤걸 원하는가?" ) );
				/*********************************************/
				
				_pUIMgr->AddMessageBoxLString( MSGLCMD_BUDDHISM_EVENT, TRUE, CTString(" ") );
				/*************************************
				// 2006년
				//_pUIMgr->AddMessageBoxLString( MSGLCMD_BUDDHISM_EVENT, FALSE, _S( 2924, "양초 3개와 교환한다." ), 0);
				//_pUIMgr->AddMessageBoxLString( MSGLCMD_BUDDHISM_EVENT, FALSE, _S( 2925, "성수병2개와 교환한다." ), 1);
				/*************************************/

				/*************************************/
				// 2007년
				_pUIMgr->AddMessageBoxLString( MSGLCMD_BUDDHISM_EVENT, FALSE, CTString(_pNetwork->GetItemName(974)), 0);	//행운의 고급제련석
				_pUIMgr->AddMessageBoxLString( MSGLCMD_BUDDHISM_EVENT, FALSE, CTString(_pNetwork->GetItemName(85)), 1);		//고급제련석
				_pUIMgr->AddMessageBoxLString( MSGLCMD_BUDDHISM_EVENT, FALSE, CTString(_pNetwork->GetItemName(1576)), 2);	//성수병
				_pUIMgr->AddMessageBoxLString( MSGLCMD_BUDDHISM_EVENT, FALSE, CTString(_pNetwork->GetItemName(884)), 3);	//드롭율 증폭제
				_pUIMgr->AddMessageBoxLString( MSGLCMD_BUDDHISM_EVENT, FALSE, CTString(_pNetwork->GetItemName(1575)), 4);	//양초
				_pUIMgr->AddMessageBoxLString( MSGLCMD_BUDDHISM_EVENT, FALSE, CTString(_pNetwork->GetItemName(675)), 5);	//구원의 눈물
				_pUIMgr->AddMessageBoxLString( MSGLCMD_BUDDHISM_EVENT, FALSE, CTString(_pNetwork->GetItemName(676)), 6);	//용서의 눈물
				/*************************************/
				_pUIMgr->AddMessageBoxLString( MSGLCMD_BUDDHISM_EVENT, FALSE, _S( 1220, "취소한다." ) );
			}
			else if( nResult == EVENT_COLLECTBUGS )
			{
				if( _pUIMgr->DoesMessageBoxLExist( MSGLCMD_COLLECT_INSECT_EVENT ) ) break;
				
				_pUIMgr->CreateMessageBoxL( _S( 169, "이벤트" ), UI_QUEST, MSGLCMD_COLLECT_INSECT_EVENT );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_COLLECT_INSECT_EVENT, TRUE, 
					_S( 2947, "여름방학 숙제로 곤충채집을 했던 경험을 떠올리며 어린시절로 돌아가보는 것은 어떨까요?" ) );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_COLLECT_INSECT_EVENT, TRUE,
					_S( 2948, "곤충채집상자를 구입하시면 몬스터로부터 여름 곤충을 채집할 수 있습니다." ) );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_COLLECT_INSECT_EVENT, TRUE,
					_S( 2949, "여름곤충은 총 10종류로 각각 1~10 포인트가 정해져 있고, 채집상자에는 16마리의 곤충을 수집할 수 있으니 다 채워지면 저에게 가져오세요." ) );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_COLLECT_INSECT_EVENT, TRUE,
					_S( 2950, "채집상자의 총 포인트 합계에 따라서 보상품을 드립니다." ) );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_COLLECT_INSECT_EVENT, TRUE, CTString(" ") );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_COLLECT_INSECT_EVENT, TRUE,
					_S( 2951, "※ 이벤트 기간: 2006년 7월 20일 ~ 8월 31일" ), -1, 0x6BD2FFFF );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_COLLECT_INSECT_EVENT, TRUE,
					_S( 2952, "※ 채집상자는 보상 후 사라지나, 이벤트 기간 동안 재 구입이 가능하여 계속해서 이벤트에 참여하 수 있습니다." ), -1, 0x6BD2FFFF );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_COLLECT_INSECT_EVENT, TRUE, CTString(" ") );

				_pUIMgr->AddMessageBoxLString( MSGLCMD_COLLECT_INSECT_EVENT, FALSE, _S( 2953, "곤충 채집상자 구입(100 Nas)" ), 0 );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_COLLECT_INSECT_EVENT, FALSE,_S( 2954, "곤충 채집상자 보상" ), 1 );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_COLLECT_INSECT_EVENT, FALSE, _S( 1220, "취소한다." ) );
			}
			else if( nResult == QUEST_HARVEST_MOON_DAY_EVENT2 )
			{
				CTString strMessage;
				_pUIMgr->CloseMessageBoxL( MSGLCMD_HARVEST_MOON_DAY2 );

				strMessage = _S( 1860, "추석맞이 이벤트" );	 
				_pUIMgr->CreateMessageBoxL( strMessage, UI_QUEST, MSGLCMD_HARVEST_MOON_DAY2 );				
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY2, TRUE, strMessage, -1,0xE18600FF );				

				/**********
				strMessage = _S( 1862, "단계별 작물을 다음 단계로 업그레이드를 하시려면 배양토가 필요합니다." );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY, TRUE, strMessage, -1, 0xA3A1A3FF );				
					
				strMessage = _S( 1863, "다음 단계로의 업그레이드는 일정확률로 업그레이드되며 실패할 수도 있습니다." );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY, TRUE, strMessage, -1, 0xA3A1A3FF );				

				strMessage = _S( 1864, "업그레이드된 작물은 보상품과 교환하거나 다음 단계로 업그레이드 할 수 있습니다." );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY, TRUE, strMessage, -1, 0xA3A1A3FF );				

				strMessage = _S( 1865, "보상품은 각 작물별로 다른 확률로 획득할 수있습니다." );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY, TRUE, strMessage, -1, 0xA3A1A3FF );				
				**********/
				strMessage = _S(3133, "추석에는 역시 송편이 빠질 수 없죠. 그 중에서도 저는 특히 오색송편을 제일 좋아한답니다." );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY2, TRUE, strMessage, -1, 0xA3A1A3FF );				
				strMessage = _S(3134, "추석이벤트 기간 동안 로레인에게 쌀가루와 꿀, 솔잎을 모아서 가져가면 송편을 만들어주는데요" );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY2, TRUE, strMessage, -1, 0xA3A1A3FF );				
				strMessage = _S(3135, "송편 10개가 있으면 맛있는 오색송편도 만들어 준답니다." );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY2, TRUE, strMessage, -1, 0xA3A1A3FF );
				strMessage = _S(3136, "혹시 오색송편이 있으시다면 저한테 주시지 않으실래요? 아래의 보상품 중 1개를 드리겠습니다." );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY2, TRUE, strMessage, -1, 0xA3A1A3FF );				
				strMessage = " ";
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY2, TRUE, strMessage, -1, 0xA3A1A3FF );	

				strMessage = _S( 1866, "보상품" );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY2, TRUE, strMessage, -1,0xE18600FF );	

				strMessage = _S( 1867, "10,000 Nas" );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY2, TRUE, strMessage, -1, 0xA3A1A3FF );	
				strMessage = _S( 1868, "50,000 Nas" );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY2, TRUE, strMessage, -1, 0xA3A1A3FF );	
				strMessage = _S( 1869, "200,000 Nas" );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY2, TRUE, strMessage, -1, 0xA3A1A3FF );	
				strMessage = _S( 1870, "500,000 Nas" );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY2, TRUE, strMessage, -1, 0xA3A1A3FF );	
				strMessage = _S( 1871, "고급제련석 2개" );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY2, TRUE, strMessage, -1, 0xA3A1A3FF );	

			
				//_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY, FALSE, _S( 1872, "작물을 업그레이드한다."  ), QUEST_HARVEST_MOON_DAY_EVENT_UPGRADE );		
				//_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY, FALSE, _S( 1873, "보상품을 지급받는다."  ), QUEST_HARVEST_MOON_DAY_EVENT_GIVE_ITEM );			
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY2, FALSE, _S(3137, "오색송편을 보상품으로 교환한다."  ), MSGCND_MOON_DAY_EVENT_GIVE_ITEM );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_HARVEST_MOON_DAY2, FALSE, _S( 1874, "취소한다."  ) );			
			}
			else if( nResult == EVENT_XMAS_2006 )// 2006 X-Mas Event [12/12/2006 Theodoric]
			{
				_pUIMgr->CloseMessageBox(MSGLCMD_EVENT_XMAS_2006_CHANGE);
				CUIMsgBox_Info	MsgBoxInfo;
				CTString strTitle;
				CMobData& MD = _pNetwork->GetMobData(336); // 눈사람
				strTitle.PrintF("%s", MD.GetName());
				
				MsgBoxInfo.SetMsgBoxInfo( strTitle, UMBS_USER_12|UMBS_BUTTONEX , UI_NONE, MSGLCMD_EVENT_XMAS_2006_CHANGE );
				MsgBoxInfo.SetUserBtnName( _S(127, "교환" ), _S( 139, "취소" ) );
				MsgBoxInfo.SetBtnType( UBET_ITEM, CItemData::ITEM_POTION, CItemData::POTION_ETC );

				CTString strSysMessage = _S(3178, "오호! 맛있어 보이는 케이크군!!") ;
				MsgBoxInfo.AddString( strSysMessage );
				strSysMessage = _S(3179, "그 케이크를 내게 주지 않겠나?");
				MsgBoxInfo.AddString( strSysMessage );
				strSysMessage = _S(3180, "케이크 3개당 진귀한 아이템을 하나씩 주도록하지.");
				MsgBoxInfo.AddString( strSysMessage );				

				_pUIMgr->CreateMessageBox( MsgBoxInfo );
			}
			else if (nResult == EVENT_LOVE_LOVE)
			{
				_pUIMgr->CloseMessageBox(MSGLCMD_EVENT_LOVE_LOVE);
				
				CTString strMessage, strMessageA, strMessageB, strMessageC, strMessageD;

				if (IS_EVENT_ON(TEVENT_LUNARNEWYEAR_2008))
				{
					strMessage = _S(3996, "2008 설날 복주머니 이벤트");
					strMessageA = _S(4004, "2008년 새해 복 많이 받으시고 소망 이루시기 바랍니다.");
					strMessageB = _S(4005, "가지고 계신 소망상자에 들어있는 복주머니가 같은은색깔로 줄을 이루고 있다면 소망상자를 교환 할 수 있습니다.");
					strMessageC = _S(4006, "소망상자 교환");
					strMessageD = _S(4007, "복주머니의 색상에 따라 빙고가 되었을 때 좋은 상품이 지급되는 확률이 달라집니다.");
				}
				else
				{
					strMessage = _S(3203, "러브러브 이벤트");
					strMessageA = _S(3214, "초코상자에 들어 있는 초코렛이 같은 종류로 줄을 이루고 있다면 초코상자를 교환할 수 있습니다.");
					strMessageB = _S(3215, "카카오 30, 카카오 60, 카카오 90 3종류의 초코렛에 따라 빙고가 되었을 때 좋은 상품이 지급되는 확률이 달라집니다.");
					strMessageC = _S(3216, "초코상자 교환");
				}

				_pUIMgr->CreateMessageBoxL(strMessage, UI_QUEST, MSGLCMD_EVENT_LOVE_LOVE);				
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_LOVE_LOVE, TRUE, strMessage, -1,0xE18600FF);
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_LOVE_LOVE, TRUE, strMessageA, -1, 0xA3A1A3FF);
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_LOVE_LOVE, TRUE, strMessageB, -1, 0xA3A1A3FF);

				if (IS_EVENT_ON(TEVENT_LUNARNEWYEAR_2008))
				{
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_LOVE_LOVE, TRUE, strMessageD, -1, 0xA3A1A3FF);
				}

				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_LOVE_LOVE, FALSE, strMessageC, 0);
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_LOVE_LOVE, FALSE, _S(1874,"취소한다."));
			}
#ifdef HELP_SYSTEM_1
			else if( nResult == NPC_HELP)
			{
				_pUIMgr->RearrangeOrder( UI_NPCHELP, TRUE );
			}		
#endif
			// WSS_EVENT_MAY -------------------------------->>
			else if( nResult == EVENT_MAY)
			{
				_pUIMgr->CreateMessageBoxL(_S(100,"이벤트"),UI_QUEST,MSGLCMD_EVENT_MAY);
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_MAY, TRUE, _S(3396, "반갑습니다. 5월 가정의 달을 맞이하여 라스트 카오스에서 즐거운 가정의 달 되시라고 게임내 4가지 이벤트를 준비했습니다.") );
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_MAY, TRUE, _S(3397, "즐겁고 행복한 5월 가정의 달 보내세요.") );
 				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_MAY, TRUE,  CTString(" "));
				if(IS_EVENT_ON(TEVENT_CHILDREN_2007))
 				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_MAY, FALSE, _S(3370, "저 레벨 탈출작전" ), EVENT_MAY_CHILDREN );	
				if(IS_EVENT_ON(TEVENT_PARENTS_2007))
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_MAY, FALSE, _S(3362, "카네이션을 모아라~" ), EVENT_MAY_PARENTS );	
				if(IS_EVENT_ON(TEVENT_TEACHER_2007))
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_MAY, FALSE, _S(3379, "스승의 은혜" ), EVENT_MAY_MASTER );	
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_MAY, FALSE, _S(1874,"취소한다."));
			}					
			// ----------------------------------------------<<

			// WSS_MINIGAME 070420 -------------------------------->>
			else if( nResult == EVENT_MINIGAME)
			{
				if (_pUIMgr->DoesMessageBoxLExist(MSGLCMD_EVENT_MINIGAME) ) break;

				_pUIMgr->CreateMessageBoxL(_S(100,"이벤트"),UI_QUEST,MSGLCMD_EVENT_MINIGAME);
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_MINIGAME, TRUE, _S(3428, "곰돌이 인형 웬디의 조각 모으기") );
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_MINIGAME, TRUE, _S(3429, "몬스터를 사냥하면 일정 확률로 [곰돌이 웬디의 조각]을 얻을 수 있습니다.") );
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_MINIGAME, TRUE, _S(3430, "조각난 웬디는 1번부터 9번까지 아홉 개의 조각으로 되어있으며 조각을 모두 모아 저에게 가져오시면 저와 같이 가위, 바위, 보 게임을 즐기실 수 있어요.") );
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_MINIGAME, TRUE, _S(3431, "가위, 바위, 보 게임에서 저에게 승리하셨을 때 마다 다음 단계에 도전할지 그냥 현재의 상품을 받고 그만둘지 선택할 수 있는데, 연속해서 많이 이길수록 좋은 상품을 받을 수 있답니다.") );
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_MINIGAME, TRUE, CTString(" ") );
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_MINIGAME, TRUE, _S(3432, "보상품 소개") );
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_MINIGAME, TRUE, _S(3433, "※가위바위보 게임에 참여하신 후 첫 회에 패배하신 경우(0승) 단 한번만 '체험용 교복셋트'를 드리며, 첫 1승 시에는 '체험용 엽기무기' 1개를 드립니다.") );
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_MINIGAME, TRUE, _S(3434, "가위바위보 0승 상품: 중형 퀵 HP 회복 물약 1개") );
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_MINIGAME, TRUE,  _S(3435, "가위바위보 1승 상품: 중형 퀵 MP 회복 물약 5개"));
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_MINIGAME, TRUE,  _S(3436, "가위바위보 2승 상품: 중형 퀵 HP 회복 물약 5개"));
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_MINIGAME, TRUE,  _S(3437, "가위바위보 3승 상품: 대형 퀵 듀얼 회복 물약 5개"));
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_MINIGAME, TRUE,  _S(3438, "가위바위보 4승 상품: 결합주문서 1개"));
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_MINIGAME, TRUE,  _S(3439, "가위바위보 5승 상품: 결합주문서 2개"));
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_MINIGAME, TRUE,  _S(3440, "가위바위보 6승 상품: 고급제련석 1개"));
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_MINIGAME, TRUE,  _S(3441, "가위바위보 7승 상품: 고급제련석 2개"));
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_MINIGAME, TRUE,  _S(3442, "가위바위보 8승 상품: 고급제련석 3개"));
 				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_MINIGAME, TRUE,  CTString(" "));		
 				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_MINIGAME, FALSE, _S(3443, "가위바위보 게임하기" ), EVENT_REWARD_1 );	
		//		종료 주석....
		//		현황만 안 보이게 처리
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_MINIGAME, FALSE, _S(3444, "곰돌이 인형 웬디 찾기 현황보기" ), EVENT_REWARD_2 );	
				_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_MINIGAME, FALSE, _S(1874,"취소한다."));
			}		
			
			// ----------------------------------------------<<
			else if(nResult == EVENT_FLOWERS_SUPPORT)
			{
				_pUIMgr->GetFlowerTree()->OpenFlowerTree();				
			}
			else if(nResult == EVENT_SUPPORT_EXCHANGE)
			{
				if (!_pUIMgr->DoesMessageBoxExist(MSGCMD_FLOWERTREE_TICKET) )
					_pNetwork->SendFlowerTreeReq((UBYTE)MSG_EVENT_FLOWERTREE_2007_MY_POINT);
			}
			else if(nResult == SIEGEWARFARE_MASTERTOWER1)	// 타워 가동하기
			{
				_pUIMgr->GetSiegeWarfareNew()->SendRequestTowerSet();
				//_pUISWDoc->SetUIState(SWS_APPLY_TOWER);
				//_pUIMgr->GetSiegeWarfareNew()->OpenCheckTower();
			}
			else if(nResult == SIEGEWARFARE_MASTERTOWER2)	// 타워 강화하기
			{
				_pUIMgr->CreateMessageBoxL(_S(100,"이벤트"),UI_QUEST,MSGLCMD_SIEGE_WARFARE_UPGRADE);
				_pUIMgr->AddMessageBoxLString(MSGLCMD_SIEGE_WARFARE_UPGRADE, TRUE, _S( 3679,"가동시킨 수호타워를 업그레이드 할 수 있습니다.") );
				_pUIMgr->AddMessageBoxLString(MSGLCMD_SIEGE_WARFARE_UPGRADE, TRUE, _S( 3680,"비용은 조금 들겠지만 성을 수호하는 수호타워를 강화시키면 현재의 타워 기능에 비해 탁월한 효과를 보실 수 있을 겁니다.") );				
				_pUIMgr->AddMessageBoxLString(MSGLCMD_SIEGE_WARFARE_UPGRADE, TRUE, _S( 3681,"강화하고자 하는 수호 타워를 선택해 주세요.") );
 				_pUIMgr->AddMessageBoxLString(MSGLCMD_SIEGE_WARFARE_UPGRADE, FALSE, _S( 3682,"공격형 타워 강화" ), EVENT_REWARD_1 );	
				_pUIMgr->AddMessageBoxLString(MSGLCMD_SIEGE_WARFARE_UPGRADE, FALSE, _S( 3683,"가드형 타워 강화" ), EVENT_REWARD_2 );	
				_pUIMgr->AddMessageBoxLString(MSGLCMD_SIEGE_WARFARE_UPGRADE, FALSE, _S(1874,"취소한다."));
				
			}
			else if(nResult == SIEGEWARFARE_MASTERTOWER3)   // 성문 강화하기
			{
				_pUIMgr->GetSiegeWarfareNew()->SetUpgradeType( SWUT_GATE );						
				_pUIMgr->GetSiegeWarfareNew()->SendTowerStateRequest(SWUT_GATE);	
			}
			else if(nResult == SIEGEWARFARE_MASTERTOWER4)   // 마스터 타워 수리하기
			{				
				_pUIMgr->GetSiegeWarfareNew()->SetRepairTowerIndex(m_iNpcIndex);
				_pUIMgr->GetSiegeWarfareNew()->SendTowerRepairStateRequest(m_iNpcIndex);
			}
			else if (nResult == EVENT_BJMONO_2007_REQ)
			{
				_pNetwork->SendBJMono2007Req(MSG_EVENT_BJMONO_2007_TICKET_REQ); // 응모권 요청
			}
			else if (nResult == EVENT_BJMONO_2007_CHANGE_REQ)
			{
				_pUIMgr->CloseMessageBox(MSGCMD_BJMONO_2007_CHANGE_REQ);

				CMobData& MD = _pNetwork->GetMobData(408); // 이벤트 진행 요원
				CTString strMessage, strName;
				CUIMsgBox_Info MsgBoxInfo;
				strName = MD.GetName();

				MsgBoxInfo.SetMsgBoxInfo(strName, UMBS_OKCANCEL|UMBS_INPUTBOX, UI_QUEST, MSGCMD_BJMONO_2007_CHANGE_REQ);
				strMessage.PrintF(_s("몇 장의 응모권을 교환 하시겠습니까?"));
				MsgBoxInfo.AddString(strMessage);
				_pUIMgr->CreateMessageBox(MsgBoxInfo);
			}			
			else if ( nResult == EVENT_TG2007_SCREENSHOT) // WSS_TG2007 2007/09/14 스크린샷 이벤트
			{
				// TODO :: 송편 받기 요청
				_pNetwork->SendTG2007ScreenshotReq();
			}
			else if ( nResult == EVENT_TG2007_1000DAYS_CAP )
			{
				// TODO :: 라스트카오스 1000일 기념모자 받기
				_pNetwork->SendLC1000DayTakeHatReq();
			}
			else if ( nResult == EVENT_TG2007_1000DAYS_CAP_UPGRADE )
			{ // TODO :: 기념모자 업드레이드
				CUIMsgBox_Info MsgBoxInfo;
				CTString strMessage;

				MsgBoxInfo.SetMsgBoxInfo(_S(3829, "기념모자 업그레이드"), UMBS_YESNO | UMBS_BUTTONEX, UI_QUEST, MSGCMD_TG2007_1000DAYS_CAP_UPGRADE);
				MsgBoxInfo.SetBtnType(UBET_ITEM, CItemData::ITEM_SHIELD, CItemData::ITEM_SHIELD_HEAD);
				strMessage.PrintF(_S(3832, "업그레이드 할 기념모자를 올려놔 주십시요"));
				MsgBoxInfo.AddString(strMessage);
				_pUIMgr->CreateMessageBox(MsgBoxInfo);
			}
			else if ( nResult ==  EVENT_TG2007_1000DAYS_FIRECRACKER)
			{ //  TODO :: 폭죽 받기 이벤트
				_pNetwork->SendTakeFireCracker();
			}
			else if ( nResult == EVENT_HALLOWEEN2007_DEVILHAIR)
			{ // TODO :: 할로윈이벤트2007, 악마날개 머리띠 지급 요청
				_pNetwork->SendHalloween2007Req(MSG_EVENT_TAKE_DEVILHEAIR);
			}
			else if ( nResult == EVENT_HALLOWEEN2007_CANDYBASKET)
			{ // TODO :: 할로윈 이벤트2007, 사탕받기 요청
				_pNetwork->SendHalloween2007Req(MSG_EVENT_CHANGE_CANDYBASKET);
			}
			else if ( nResult == EVENT_HALLOWEEN2007_PUMKINHEAD)
			{ // TODO :: 할로윈 이벤트2007, 호박머리 탈 교환받기
				_pNetwork->SendHalloween2007Req(MSG_EVENT_CHANGE_PUMKINHEAD);
			}
			else if ( nResult == EVENT_HALLOWEEN2007_WITCHHAT)
			{ // TODO :: 할로윈 이벤트2007, 마녀모자 교환 요청
				_pNetwork->SendHalloween2007Req(MSG_EVENT_CHANGE_WITCHHAT);
			}
			else if ( nResult == EVENT_HALLOWEEN2007_PUMKIN)
			{ // TODO :: 할로윈 이벤트2007, 호박 교환 요청
				_pNetwork->SendHalloween2007Req(MSG_EVENT_CHANGE_PUMKIN);
			}
			else if ( nResult == EVENT_XMAS2007_DECO )
			{ // 크리스마스 장식 
				CUIMsgBox_Info MsgBoxInfo;
				CTString strMessage;

				MsgBoxInfo.SetMsgBoxInfo(_S(169, "이벤트"), UMBS_YESNO, UI_QUEST, MSGCMD_EVENT_XMAS_2007_DECO);
				MsgBoxInfo.AddString(_S(3953, "크리스마스 트리를 장식하면 보유한 크리스마스 트리 장식은 모두 사라집니다"));
				_pUIMgr->CreateMessageBox(MsgBoxInfo);
			}
			else if ( nResult == EVENT_XMAS2007_BLESSEDNESS )
			{ // 크리스마스 축복 요청
				_pNetwork->SendXMAS2007DecoReq(MSG_EVENT_XMASTREE_GET_BLESS);
			}
			//071129 ttos : 판매 대행협회 회장
			else if( nResult == CASH_PERSONSHOP_CLOSE)
			{ // TODO :: 판매 대행 종료
				_pNetwork->SendCashPersonShop(MSG_ALTERNATEMERCHANT_END);
			}
			else if (nResult == CASH_PERSONSHOP_ITEM)
			{ // TODO :: 보관된 아이템 회수
				_pNetwork->SendCashPersonShop(MSG_ALTERNATEMERCHANT_PRODUCT_RECOVERY);
			}
			else if(nResult == CASH_PERSONSHOP_NAS)
			{ // TODO :: 보관된 나스 회수
				_pNetwork->SendCashPersonShop( MSG_ALTERNATEMERCHANT_PRODUCT_NAS);
			}
			else if(nResult == EVENT_SAKURA_2008)	//2008년 벚꽃 놀이 이벤트 보상 교환 요청
			{
				if (_pUIMgr->DoesMessageBoxExist(MSGCMD_EVENT_SAKURA_2008))
					_pUIMgr->CloseMessageBox(MSGCMD_EVENT_SAKURA_2008);

				CUIMsgBox_Info MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo(_S(169, "이벤트"), UMBS_YESNO, UI_QUEST, MSGCMD_EVENT_SAKURA_2008);
				MsgBoxInfo.AddString(_S(4113, "가지고 있는 퇴비는 모두 사라집니다. 퇴비를 건네주겠습니까?"));
				_pUIMgr->CreateMessageBox(MsgBoxInfo);
			}
			// [080422: Su-won] EVENT_CHAOSBALL
			else if(nResult == EVENT_CHAOSBALL)
			{
				if( _pUIMgr->DoesMessageBoxLExist( MSGLCMD_CHAOSBALL_EVENT ) ) 
					break;

				_pUIMgr->CreateMessageBoxL( _S(100, "이벤트"), UI_QUEST, MSGLCMD_CHAOSBALL_EVENT );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_CHAOSBALL_EVENT, TRUE, 
					_S(4128, "카오스 볼은 고대의 마법문양이 새겨진 신비한 공입니다." ) );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_CHAOSBALL_EVENT, TRUE,	
					_S(4129, "아이리스 대륙에서는 흔히 볼 수 있는 물건은 아니지만, 저의 고향 루나시스에서는 진귀한 보물이나 기운을 담아놓는 상자로 이용하곤 한답니다." ) );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_CHAOSBALL_EVENT, TRUE,
					_S(4130, "주위를 일렁이는 기운과 단단한 표면때문에 정식으로 해제를 하기 전까지는 안에 뭐가 들어있는지, 어떤 힘이 들어있는지 결코 알 수 없으니 진귀한 보물을 숨겨두기에는 안성맞춤이지요." ));
				_pUIMgr->AddMessageBoxLString( MSGLCMD_CHAOSBALL_EVENT, TRUE,
					_S(4131, "최근에 아이리스 상점에서 판매하는 몇몇 물건에 들어있다고 하는데, 카오스 볼을 가지고 계신다면 저에게 가져다 주세요. 봉인을 해제해서 안에 들어있는 보물을 꺼내 드리겠습니다." ));

				_pUIMgr->AddMessageBoxLString( MSGLCMD_CHAOSBALL_EVENT, FALSE, _S(4132, "카오스 볼 봉인해제하기" ), 1 );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_CHAOSBALL_EVENT, FALSE, _S(1220, "취소한다." ) );
			}
			else if(nResult == EVENT_PHOENIX_REQ) //피닉스 이벤트
			{
				_pNetwork->SendPhoenixCharacterCondition();
			}
			else if(nResult == USE_AUCTION)
			{
				if(!_pUIMgr->IsUIVisible(UI_AUCTION))
				{
					_pUIMgr->GetAuction()->OpenAuction(m_iNpcIndex, m_fNpcX, m_fNpcZ);
				}
			}
			else if(nResult == EVENT_HANAPOS_SK_EVENT)//[ttos_2009_3_18]: 하나포스 및 SK브로드밴드 가입자 이벤트
			{
				_pNetwork->SendHanaposEvent();
			}
		}
		break;
	case MSGLCMD_GATE_OF_DIMENSION:
		{
			if( nResult == GOD_ENTER )				// 차원의 문 입장
			{
				// FIXME : Hardcoding 인덱스
				const int iQuestIndex = 105;
				if( CQuestSystem::Instance().CanIDoQuest( iQuestIndex ) )
				{
					if( CQuestSystem::Instance().GetDynamicDataByQuestIndex( iQuestIndex ) == NULL )
					{										
						_pUIMgr->GetQuestBookNew()->OpenQuestBook( iQuestIndex );
					}
					else
					{
						CTString strSysMessage;
						strSysMessage.PrintF( _S( 1687, "이미 수행중인 퀘스트입니다."  ) );		
						_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR );
					}
				}
				else
				{
					CTString strSysMessage;
					strSysMessage.PrintF( _S( 1688, "조건이 맞지 않습니다."  ) );		
					_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR );
				}
			}
			else if( nResult == GOD_RANKING )		// 차원의 문 순위확인
			{
				_pNetwork->Ranking_RequestList( _pNetwork->MyCharacterInfo.job );
				//_pUIMgr->GetRanking()->OpenRankingList();
			}
			else if( nResult == GOD_PRIZE )			// 보상.
			{
				_pNetwork->Ranking_RequestPrizeList();
				//_pUIMgr->GetRanking()->OpenRankingPrize();
			}
		}
		break;

	case MSGLCMD_CHANGEWEAPON_EVENT_REQ:
		{
			if( nResult == EVENT_CHANGEWEAPON )
			{
				_pUIMgr->CloseMessageBox( MSGCMD_WEAPONCHANGE_EVENT );
				CTString strMessage;
				// Create message box of remission
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S( 1204, "무기 교체 이벤트" ), UMBS_OKCANCEL,	
					UI_QUEST, MSGCMD_WEAPONCHANGE_EVENT );
				strMessage.PrintF( _S( 1213, "무기를 교체하시겠습니까?" ) );				
				MsgBoxInfo.AddString( strMessage );
				_pUIMgr->CreateMessageBox( MsgBoxInfo );
			}
			else
			{
			}
		}
		break;
		
	case MSGLCMD_TREASUREBOX_EVENT:
		{
			if( nResult == 1 )
			{
				_pNetwork->SendEventOpenTreasure();
			}
		}
		break;

	case MSGLCMD_EVENT_2PAN4PAN_ONE:
		{
			if(nResult == EXCH_FIVE){
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S( 100, "이벤트" ), UMBS_YESNO, UI_QUEST, MSGCMD_ASK_ONE_FIVE );
				MsgBoxInfo.AddString( _S(2219, "2pan4pan 맞고카드를 맞고머니로 교환하시겠습니까?"));
				_pUIMgr->CreateMessageBox( MsgBoxInfo );
			}
			else if(nResult == EXCH_FOUR){
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S( 100, "이벤트" ), UMBS_YESNO, UI_QUEST, MSGCMD_ASK_ONE_FOUR );			
				MsgBoxInfo.AddString( _S(2220, "2pan4pan 포커카드를 포커머니로 교환하시겠습니까?"));
				_pUIMgr->CreateMessageBox( MsgBoxInfo );
			}

		}
		break;
	case MSGLCMD_EVENT_2PAN4PAN_TWO:
		{
			if(nResult == OPEN_TREA ){
				_pNetwork->SendEvent24(0,MSG_EVENT_2PAN4PAN_BOX_REQ);

			}			
		}
		break;
		
	case MSGLCMD_EVENT_NEWYEAR1:
		{
			if(nResult == EVENT_NEWYEAR1_REWARD){
				CUIMsgBox_Info MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo(_S(169,"이벤트"),UMBS_YESNO,UI_QUEST,MSGCMD_ASK_NEWYEAR1);
				MsgBoxInfo.AddString(_S(2296,"보상품을 지급 받으시겠습니까?"));
				_pUIMgr->CreateMessageBox(MsgBoxInfo);
			}
		}
		break;
	case MSGLCMD_EVENT_NEWYEAR2:
		{
			if(nResult == EVENT_NEWYEAR2_TIMECHECK){
				_pNetwork->SendEventNewyear(EVENT_NEWYEAR2_TIMECHECK);				
				
			}
			else if(nResult == EVENT_NEWYEAR2_REWARD){
				CUIMsgBox_Info MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo(_S(169,"이벤트"),UMBS_YESNO,UI_QUEST,MSGCMD_ASK_NEWYEAR2);
				MsgBoxInfo.AddString(_S(2296,"보상품을 지급 받으시겠습니까?"));
				_pUIMgr->CreateMessageBox(MsgBoxInfo);

			}
		}
		break;

	case MSGLCMD_EVENT_FIND_FRIEND :
		{
			if( nResult == EVENT_FF_REG){
				CUIMsgBox_Info MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo(_S( 2433, "친구 찾기 이벤트"),UMBS_OKCANCEL|UMBS_INPUTBOX,UI_QUEST,MSGCMD_INPUT_FRIEND);
				MsgBoxInfo.AddString(_S( 2487, "찾기를 원하는 친구의 캐릭명을 입력 하세요."));
				_pUIMgr->CreateMessageBox(MsgBoxInfo);

			} else if (nResult == EVENT_FF_TIMECHECK){
				// TODO : get TIME 
				_pNetwork->SendFindFriend(MSG_EVENT_SEARCHFRIEND_TIME_CHECK);
				
			} else if (nResult == EVENT_FF_REWARD){
				// TODO : send message to make friend list
				_pNetwork->SendFindFriend(MSG_EVENT_SEARCHFRIEND_LIST_GOODS);
			}

		}
		break;
	case MSGLCMD_CLOTHES_EXCHANGE:	// 일본 전통 의상 교환 이벤트 eons
		{
			_pNetwork->SendClothesExchange( nResult );
		}
		break;

	case MSGCMD_WORLDCUP_EVENT1 :
		{
			switch(nResult)
			{
				case WORLDCUP_EVENT1_EXCHANGE:
				{
					if( _pUIMgr->DoesMessageBoxLExist( MSGCMD_WORLDCUP_EVENT1_EXCHANGE ) ) break;
					_pUIMgr->CreateMessageBoxL(  _S( 169, "이벤트" ), UI_QUEST, MSGCMD_WORLDCUP_EVENT1_EXCHANGE );
					_pUIMgr->AddMessageBoxLString( MSGCMD_WORLDCUP_EVENT1_EXCHANGE, TRUE,
						_S( 2813, "참가국 별 국기 교환에 필요한 우승컵 개수" ) );
					_pUIMgr->AddMessageBoxLString( MSGCMD_WORLDCUP_EVENT1_EXCHANGE, TRUE,
						_S( 2814, "※ 100개 - 브라질,독일,이탈리아,영국,아르헨티나" ) );
					_pUIMgr->AddMessageBoxLString( MSGCMD_WORLDCUP_EVENT1_EXCHANGE, TRUE,
						_S( 2815, "※ 75개 - 프랑스, 네덜란드, 스페인, 포르투갈, 체코, 스웨덴, 멕시코, 우크라이나, 크로아티아" ) );
					_pUIMgr->AddMessageBoxLString( MSGCMD_WORLDCUP_EVENT1_EXCHANGE, TRUE,
						_S( 2816, "※ 50개 - 미국, 스위스, 코트디부아르, 세르비아-몬테네그로, 폴란드, 파라과이,한국, 호주, 일본" ) );
					_pUIMgr->AddMessageBoxLString( MSGCMD_WORLDCUP_EVENT1_EXCHANGE, TRUE,
						_S( 2817, "※ 25개 - 가나, 튀니지, 에콰도르, 토고, 앙골라, 이란, 사우디아라비아, 코스타리카, 트리니다드 토바고" ) );
					_pUIMgr->AddMessageBoxLString( MSGCMD_WORLDCUP_EVENT1_EXCHANGE, FALSE, _S( 2818, "우승컵 100개 교환" ), WORLDCUP_EVENT1_EXCHANGE100 );
					_pUIMgr->AddMessageBoxLString( MSGCMD_WORLDCUP_EVENT1_EXCHANGE, FALSE, _S( 2819, "우승컵 75개 교환" ), WORLDCUP_EVENT1_EXCHANGE75 );
					_pUIMgr->AddMessageBoxLString( MSGCMD_WORLDCUP_EVENT1_EXCHANGE, FALSE, _S( 2820, "우승컵 50개 교환" ), WORLDCUP_EVENT1_EXCHANGE50 );
					_pUIMgr->AddMessageBoxLString( MSGCMD_WORLDCUP_EVENT1_EXCHANGE, FALSE, _S( 2821, "우승컵 30개 교환" ), WORLDCUP_EVENT1_EXCHANGE25 );
					_pUIMgr->AddMessageBoxLString( MSGCMD_WORLDCUP_EVENT1_EXCHANGE, FALSE, _S( 1220, "취소한다." ) );
				}
				break;
				
				case WORLDCUP_EVENT1_RECEIVE:
				{
					if( _pUIMgr->DoesMessageBoxLExist( MSGCMD_WORLDCUP_EVENT1_RECEIVE ) ) break;
					_pUIMgr->CreateMessageBoxL(  _S( 169, "이벤트" ), UI_QUEST, MSGCMD_WORLDCUP_EVENT1_RECEIVE );
					_pUIMgr->AddMessageBoxLString( MSGCMD_WORLDCUP_EVENT1_RECEIVE, TRUE,
						_S( 2822, "축구 대회 결과 " ) );
					_pUIMgr->AddMessageBoxLString( MSGCMD_WORLDCUP_EVENT1_RECEIVE, TRUE,
						_S( 2823, "1위 미정" ) );
					_pUIMgr->AddMessageBoxLString( MSGCMD_WORLDCUP_EVENT1_RECEIVE, TRUE,
						_S( 2824, "2위 미정" ) );
					_pUIMgr->AddMessageBoxLString( MSGCMD_WORLDCUP_EVENT1_RECEIVE, TRUE,
						_S( 2825, "3위 미정" ) );
					_pUIMgr->AddMessageBoxLString( MSGCMD_WORLDCUP_EVENT1_RECEIVE, FALSE, _S( 2826, "1위 보상" ), WORLDCUP_EVENT1_REWARD1 );
					_pUIMgr->AddMessageBoxLString( MSGCMD_WORLDCUP_EVENT1_RECEIVE, FALSE, _S( 2827, "2위 보상" ), WORLDCUP_EVENT1_REWARD2 );
					_pUIMgr->AddMessageBoxLString( MSGCMD_WORLDCUP_EVENT1_RECEIVE, FALSE, _S( 2828, "3위 보상" ), WORLDCUP_EVENT1_REWARD3 );
					_pUIMgr->AddMessageBoxLString( MSGCMD_WORLDCUP_EVENT1_RECEIVE, FALSE, _S( 2829, "그외 순위 보상" ), WORLDCUP_EVENT1_REWARD4 );
					_pUIMgr->AddMessageBoxLString( MSGCMD_WORLDCUP_EVENT1_RECEIVE, FALSE, _S( 1220, "취소한다." ) );
				}
				break;
			}
		}
		break;

	
	case MSGCMD_WORLDCUP_EVENT1_EXCHANGE :
		{
			nResult-=WORLDCUP_EVENT1_EXCHANGE100;

			if(nResult >= 0 && nResult < WORLDCUP_MAX_GROUP )
			{

				CUIMsgBox_Info MsgBoxInfo;
				CTString strMessage;
				strMessage.PrintF(_S( 2830, "우승컵을 교환할 참가국을 선택해 주세요."));
				
				MsgBoxInfo.SetMsgBoxInfo(_S(191,"확인"),UMBS_YESNO | UMBS_COMBOBOX ,UI_QUEST,MSGCMD_WORLDCUP_EVENT1_NUM);
				MsgBoxInfo.AddString(strMessage);
				_pUIMgr->CreateMessageBox(MsgBoxInfo);

			
				
					iSelGroup = nResult;
					_pUIMgr->GetMessageBox(MSGCMD_WORLDCUP_EVENT1_NUM)->GetComboBox().ResetStrings();
					for(int i=iGroup[iSelGroup] ; i<iGroup[iSelGroup+1]; i++  )
					{
						_pUIMgr->GetMessageBox(MSGCMD_WORLDCUP_EVENT1_NUM)->GetComboBox().AddString(m_strCountry[i]);

					}
			}
							
		}
		break;
	case MSGCMD_WORLDCUP_EVENT1_RECEIVE :
		{
			nResult-=WORLDCUP_EVENT1_REWARD1;

			if(nResult >= 0 && nResult < WORLDCUP_MAX_GROUP )
			{
				_pNetwork->SendWorldCupEvent(MSGCMD_WORLDCUP_EVENT1_RECEIVE,nResult+1);
			}
								
		}
		break;

	case MSGCMD_WORLDCUP_GOLDENBALL:
		{
			switch( nResult )
			{
			case MSGCMD_WORLDCUP_GOLDENBALL_ENTRY:
				{
					if( _pUIMgr->DoesMessageBoxExist( MSGCMD_GOLDENBALL_EVENT ) )
						return;

					if( !m_bGoldenBallEntry )
					{
						_pNetwork->ClientSystemMessage( _S( 2831, "현재 응모 가능 시간이 아닙니다." ), SYSMSG_ERROR );
						return;
					}
					
					CTString strMessageA, strMessageB;
					strMessageA.PrintF( _S( 2832, "%s VS %s 경기\n\n승리국가와 점수를 예상하여 입력해 주세요\n\n" ), m_strTeamA, m_strTeamB );
					strMessageB.PrintF( _S( 2833, "%-5s점수     %-5s점수" ), m_strTeamA, m_strTeamB );

					strMessageA += strMessageB;

					CUIMsgBox_Info MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo( _S( 2834, "골든 볼 이벤트" ), UMBS_OKCANCEL | UMBS_INPUTBOX | UMBS_SECOND_INPUTBOX,
																UI_NONE, MSGCMD_GOLDENBALL_EVENT );

					MsgBoxInfo.AddString( strMessageA, 0xF2F2F2FF, TEXT_CENTER );
					MsgBoxInfo.SetMaxRow( 8 );

					MsgBoxInfo.SetInputBox( 7, 6, 2, 30 );
					MsgBoxInfo.SetSEInputBox( 7, 12, 2, 30, 54 );

					_pUIMgr->CreateMessageBox( MsgBoxInfo );
				}
				break;
			case MSGCMD_WORLDCUP_GOLDENBALL_REQUITAL:
				{
					_pNetwork->SendWorldCupGoldenBallEvent( MSG_EVENT_GOLDENBALL_GIFT, 0, 0 );
				}
				break;
			}
		}		
		break;
	case MSGLCMD_RAINYDAY:	// 일본 빗방울 이벤트
		{
			if( nResult == 1)
			{
				_pNetwork->SendRainyDayExchange();
			}
		}
		break;
	case MSGLCMD_BUDDHISM_EVENT:
		{
			if( nResult >= 0 && nResult <= 6 )
			{ _pNetwork->SendBuddhismEvent( (BYTE)nResult ); }
		}
		break;
	case MSGLCMD_COLLECT_INSECT_EVENT:
		{
			switch( nResult )
			{
			case 0:
				{// 구입
					_pUIMgr->CloseMessageBox( MSGCMD_COLLECTBOX_BUY_REQ );

					CTString strMessage;
					CUIMsgBox_Info MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_YESNO, UI_QUEST, MSGCMD_COLLECTBOX_BUY_REQ );
					strMessage.PrintF( _S( 2955, "여름 이벤트용 곤충 채집상자의 가격은 100나스 입니다. 구입하시겠습니까?" ) );
					MsgBoxInfo.AddString( strMessage );
					_pUIMgr->CreateMessageBox( MsgBoxInfo );
				}
				break;
			case 1:
				{// 보상
					if( _pUIMgr->DoesMessageBoxLExist( MSGLCMD_COLLECT_BOX_GIFT ) ) break;
					_pUIMgr->CreateMessageBoxL( _S( 2954, "곤충 채집상자 보상" ), UI_QUEST, MSGLCMD_COLLECT_BOX_GIFT );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_COLLECT_BOX_GIFT, TRUE, _S( 2956, "곤충 채집상자를 모두 채워 오셨나요?" ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_COLLECT_BOX_GIFT, TRUE,
						_S( 2957, "채집상자 안에 있는 곤충의 포인트 합계에 따라 아래의 보상품과 교환해 드리겠습니다." ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_COLLECT_BOX_GIFT, TRUE,
						_S( 2958, "이벤트기간 동안은 보상 후 다시 곤충 채집상자를 구입하여 이벤트를 계속 할 수 있습니다." ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_COLLECT_BOX_GIFT, TRUE, CTString(" ") );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_COLLECT_BOX_GIFT, TRUE, _S( 1866, "보상품" ), -1, 0x6BD2FFFF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_COLLECT_BOX_GIFT, TRUE, _S( 2959, "1~25 포인트: 500나스" ), -1, 0xA3A1A3FF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_COLLECT_BOX_GIFT, TRUE, _S( 2960, "26~40 포인트: 메모리 스크롤 3개" ), -1, 0xA3A1A3FF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_COLLECT_BOX_GIFT, TRUE, _S( 2961, "41~55 포인트: 대형 마나 회복제 5개" ), -1, 0xA3A1A3FF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_COLLECT_BOX_GIFT, TRUE, _S( 2962, "56~70 포인트: 대형 체력 회복제 5개" ), -1, 0xA3A1A3FF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_COLLECT_BOX_GIFT, TRUE, _S( 2963, "71~85 포인트: 문스톤 2개" ), -1, 0xA3A1A3FF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_COLLECT_BOX_GIFT, TRUE, _S( 2964, "86~100 포인트: 문스톤 5개" ), -1, 0xA3A1A3FF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_COLLECT_BOX_GIFT, TRUE, _S( 2965, "101~115 포인트: 참외 2개" ), -1, 0xA3A1A3FF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_COLLECT_BOX_GIFT, TRUE, _S( 2966, "116~120 포인트: 자두 2개" ), -1, 0xA3A1A3FF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_COLLECT_BOX_GIFT, TRUE, _S( 2967, "121~125 포인트: 수박 2개" ), -1, 0xA3A1A3FF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_COLLECT_BOX_GIFT, TRUE, _S( 2968, "126~130 포인트: 고급 제련석 1개" ), -1, 0xA3A1A3FF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_COLLECT_BOX_GIFT, TRUE, _S( 2969, "131~159 포인트: 고급 제련석 2개" ), -1, 0xA3A1A3FF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_COLLECT_BOX_GIFT, TRUE, _S( 2970, "160 포인트 이상: 고급 제련석 3개" ), -1, 0xA3A1A3FF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_COLLECT_BOX_GIFT, FALSE, _S( 2971, "곤충 채집상자 보상 받기" ), 0 );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_COLLECT_BOX_GIFT, FALSE, _S( 1220, "취소한다." ) );
				}
				break;
			}
		}
		break;
	case MSGLCMD_COLLECT_BOX_GIFT:
		{
			if( nResult == 0 )
			{
				_pNetwork->SendGiftCollectBox();
			}
		}
	case MSGLCMD_EVENT_PROMOPACK:
		{
			if( nResult == EVENT_PROMO_KEY_ENTER )
			{
				CUIMsgBox_Info MsgBoxInfo;
				int tmpInputMaxChar = 12;

				if (g_iCountry == HONGKONG) tmpInputMaxChar = 17; // 홍콩은 key입력이 최대 15자

				MsgBoxInfo.m_nInputMaxChar = tmpInputMaxChar;
				MsgBoxInfo.m_nInputWidth = 78;
				MsgBoxInfo.m_nInputPosX = 70;
				MsgBoxInfo.m_nInputPosY = 48;

				if (g_iCountry == GERMANY || g_iCountry == POLAND) 
				{
					MsgBoxInfo.m_nInputMaxChar = 20;
					MsgBoxInfo.m_nInputWidth = 130;
					MsgBoxInfo.m_nInputPosX = 44;
				}

				MsgBoxInfo.SetMsgBoxInfo(_S(3150, "Serial Key Enter"),UMBS_OKCANCEL|UMBS_INPUTBOX,UI_QUEST,MSGCMD_PROMO_KEY_ENTER);
				_pUIMgr->CreateMessageBox(MsgBoxInfo);
			}
		}
		break;
	case MSGLCMD_EVENT_LOVE_LOVE:
		{
			if (nResult == 0 )
			{
				if (_pUIMgr->DoesMessageBoxExist(MSGCMD_OK_EXCHANGE_CHOCOBOX) ||
					_pUIMgr->DoesMessageBoxExist(MSGCMD_OK_EXCHANGE_LUCKYBOX))
					return;

				CUIMsgBox_Info MsgBoxInfo;
				CTString strMessage, strTitle;
				int nCommand;

				if (IS_EVENT_ON(TEVENT_LUNARNEWYEAR_2008))
				{
					strMessage = _S(4008, "소망상자를 올린 후 확인버튼을 클릭하세요");
					strTitle = _S(4006, "소망상자 교환");
					nCommand = MSGCMD_OK_EXCHANGE_LUCKYBOX;
				}
				else
				{
					strMessage = _S(3217, "초코상자를 올린 후 확인버튼을 클릭하세요");
					strTitle = _S(3216, "초코상자 교환");
					nCommand = MSGCMD_OK_EXCHANGE_CHOCOBOX;
				}

				MsgBoxInfo.SetMsgBoxInfo(strTitle, UMBS_YESNO | UMBS_BUTTONEX, UI_QUEST, nCommand);
				MsgBoxInfo.SetBtnType(UBET_ITEM, CItemData::ITEM_ONCEUSE, CItemData::ITEM_SUB_BOX);
				MsgBoxInfo.AddString(strMessage);
				_pUIMgr->CreateMessageBox(MsgBoxInfo);
			}
		}
		break;
	
		// wooss 070305	------------------------------>>
		// kw : WSS_WHITEDAY_2007
	case  MSGLCMD_WHITEDAY_2007:
		{
			CTString tstr;
			switch(nResult)
			{
				case EVENT_REWARD_1:
					// TODO : 러브매직 주문을 받는다.
				case EVENT_REWARD_2:
					// TODO : 하늘색 편지지로 교환
				case EVENT_REWARD_3:
					// TODO : 분홍색 편지지로 교환
					_pNetwork->SendWhiteday2007((UBYTE)(nResult - EVENT_REWARD_1));
					break;

			}

		} // MSGLCMD_WHITEDAY_2007
		break;	
		// ---------------------------------------------<<
	case MSGLCMD_EVENT_MAY:
		{
			switch(nResult)
			{
				case EVENT_MAY_CHILDREN:
					{
						// TODO : 어린이날 이벤트 
						
						_pUIMgr->GetShop()->EventOpenShop( 254, 0, m_fNpcX,m_fNpcZ);
					}
					break;					
				case EVENT_MAY_PARENTS:
					{
						// TODO : 어버이날 이벤트 
						// WSS_GUILDMASTER 070411 -------------------------------------------------------------->>						
						if( _pUIMgr->DoesMessageBoxLExist( MSGLCMD_GUILDMASTER ) ) break;
						_pUIMgr->CreateMessageBoxL(  _S( 169, "이벤트" ), UI_QUEST, MSGLCMD_GUILDMASTER );
						_pUIMgr->AddMessageBoxLString( MSGLCMD_GUILDMASTER, TRUE,CTString(""));
						_pUIMgr->AddMessageBoxLString( MSGLCMD_GUILDMASTER, TRUE,_S(3445, "어버이날 이벤트는 어버이날을 맞이하여 빨간 카네이션을 길드원들이 길드마스터에게 선물을 하는 이벤트입니다."));
						_pUIMgr->AddMessageBoxLString( MSGLCMD_GUILDMASTER, TRUE,_S(3446, "길드 마스터는 빨간 카네이션을 선물로 받고 이를 길드 포이트로 등록하여 가장 높은 점수를 내는 길드를 뽑는 이벤트 입니다."));
						_pUIMgr->AddMessageBoxLString( MSGLCMD_GUILDMASTER, TRUE,CTString(""));
						_pUIMgr->AddMessageBoxLString( MSGLCMD_GUILDMASTER, FALSE,_S(3447, "길드 포인트"), EVENT_REWARD_1 );
						_pUIMgr->AddMessageBoxLString( MSGLCMD_GUILDMASTER, FALSE,_S(3448, "교환권"), EVENT_REWARD_2 );
						_pUIMgr->AddMessageBoxLString( MSGLCMD_GUILDMASTER, FALSE, _S( 1220, "취소한다." ) );
						// ---------------------------------------------------------------------------------------<<
					
					}
					break;
				case EVENT_MAY_MASTER:
					{
						// TODO : 스승의날 이벤트
// [KH_070413] 스승의날 이벤트 관련 추가
						if(!_pNetwork->HasItem(MEDAL_OF_TEACHER) &&
							!_pNetwork->HasItem(MEDAL_OF_DISCIPLE))
						{
							CUIMsgBox_Info MsgBoxInfo;
							MsgBoxInfo.SetMsgBoxInfo(_S(3344, "스승의 은혜 이벤트"), UMBS_OK, UI_NONE,MSGCMD_NULL);
							MsgBoxInfo.AddString(_S(3345, "스승의 은혜 이벤트를 참가하여 보세요. 그럼 좋은 일이 있을 거에요."));
							_pUIMgr->CreateMessageBox(MsgBoxInfo);
						}
						else
							_pNetwork->SendEventMaster();
					}
					break;
			}
		}			
		break;
		// --------------------------------------------<<

	// WSS_GUILDMASTER 070411 -------------------------------------------------------------->>
	case MSGLCMD_GUILDMASTER:
		{
			switch(nResult)
			{
				case EVENT_REWARD_1:
					{
						_pUIMgr->CloseMessageBox(MSGCMD_GUILDMASTER_1);
						CUIMsgBox_Info	MsgBoxInfo;
						CTString strTitle =_S(169, "이벤트");
						MsgBoxInfo.SetMsgBoxInfo( strTitle, UMBS_LISTBOX|UMBS_USER_12 | UMBS_ALIGN_CENTER , UI_QUEST, MSGCMD_GUILDMASTER_1 );
						MsgBoxInfo.SetUserBtnName( _S(2489, "등록" ), _S(3449, "갱신") );
						MsgBoxInfo.SetListBoxPlacement(5,25,330,100,5,3);
						_pUIMgr->CreateMessageBox( MsgBoxInfo );
						CUIListBox* tBox = &_pUIMgr->GetMessageBox(MSGCMD_GUILDMASTER_1)->GetListBox();
						tBox->ResetAllStrings();
						tBox->AddString( 0, _S(1714, "순위"),0xF2F2F2FF, FALSE );
						tBox->AddString( 1, _S(3450, "포인트"),0xF2F2F2FF, FALSE );
						tBox->AddString( 2, _S(3451, "길드명"),0xF2F2F2FF, FALSE );						
						
						// Adjust column position of list box
						int tSpc = _pUIFontTexMgr->GetFontWidth();
						tBox->SetColumnPosX( 0, 5*tSpc, TEXT_CENTER );
						tBox->SetColumnPosX( 1, 25*tSpc, TEXT_RIGHT );
						tBox->SetColumnPosX( 2, 45*tSpc, TEXT_CENTER );						

						// 길드 리스트 요청...
						_pNetwork->SendRequestParentsday( MSG_EVENT_PARENTSDAY_2007_ACCUMULATEPOINT_RANKING_LIST);
					}
					break;					
				case EVENT_REWARD_2:
					{						
						if( _pUIMgr->DoesMessageBoxLExist( MSGLCMD_GUILDMASTER_2 ) ) break;
						_pUIMgr->CreateMessageBoxL(  _S( 169, "이벤트" ), UI_QUEST, MSGLCMD_GUILDMASTER_2 );
						_pUIMgr->AddMessageBoxLString( MSGLCMD_GUILDMASTER_2, TRUE,	CTString(" "));
						_pUIMgr->AddMessageBoxLString( MSGLCMD_GUILDMASTER_2, TRUE,	_S(3466, "길드 포인트 100포인트당 교환권 1장을 받을 수 있습니다. 지급 받은 교환권을 사용하여 상품을 받아 길드원들과 이벤트를 즐겨 보세요." ) );
						_pUIMgr->AddMessageBoxLString( MSGLCMD_GUILDMASTER_2, FALSE, _S(3467, "교환권 받기" ), EVENT_REWARD_1 );
						_pUIMgr->AddMessageBoxLString( MSGLCMD_GUILDMASTER_2, FALSE, _S(3468, "교환권 사용하기" ), EVENT_REWARD_2 );
						_pUIMgr->AddMessageBoxLString( MSGLCMD_GUILDMASTER_2, FALSE, _S(1220, "취소한다." ) );
						
					}
					break;			
			}
		}			
		break;
	case MSGLCMD_GUILDMASTER_2:
		{
			// TODO : 교환권
			switch(nResult)
			{
				case EVENT_REWARD_1:
					{
						// 교환권 받기
						_pUIMgr->CloseMessageBox(MSGCMD_GUILDMASTER_2_1);
						CUIMsgBox_Info	MsgBoxInfo;
						CTString strMessage =_S(169, "이벤트");
						MsgBoxInfo.SetMsgBoxInfo( strMessage, UMBS_OKCANCEL, UI_QUEST, MSGCMD_GUILDMASTER_2_1 );
						strMessage = _S(3452, "누적된 교환권을 받으시겠습니까?");
						MsgBoxInfo.AddString(strMessage);
						_pUIMgr->CreateMessageBox( MsgBoxInfo );
					}
					break;					
				case EVENT_REWARD_2:
					{	
						// 교환권 사용하기
						_pNetwork->SendRequestParentsday( MSG_EVENT_PARENTSDAY_2007_EXCHANGE_ITEM);
						
					}
					break;			
			}

		}
		break; 
	// --------------------------------------------------------------------------------------<<

	// WSS_EVENT_MINIGAME 070420 ------------------------------------------------------------>>
	case MSGLCMD_EVENT_MINIGAME:
		{
			switch(nResult)
			{
				case EVENT_REWARD_1:
					{
						// TODO : 가위바위보 게임하기
						// Send MSG_EVENT_GOMDORI_2007_CHECKITEM 	
						_pNetwork->SendMinigameDefaultMessage(MSG_EVENT_GOMDORI_2007_CHECKITEM);
					}
					break;					
				case EVENT_REWARD_2:
					{
						// TODO : 곰돌이 인형 웬디 찾기 현황보기
						_pNetwork->SendMinigameDefaultMessage(MSG_EVENT_GOMDORI_2007_VIEW_STATUS);					
					}
					break;				
			}
		}			
		break;

	case MSGLCMD_SIEGE_WARFARE_UPGRADE:
		{
			switch(nResult)
			{
				case EVENT_REWARD_1:  // 공격형 강화
					{
						_pUIMgr->GetSiegeWarfareNew()->SetUpgradeType( SWUT_ATTACK );						
						_pUIMgr->GetSiegeWarfareNew()->SendTowerStateRequest(SWUT_ATTACK);				
						
					}
					break;					
				case EVENT_REWARD_2:  // 가드형 강화
					{
						_pUIMgr->GetSiegeWarfareNew()->SetUpgradeType( SWUT_GUARD );						
						_pUIMgr->GetSiegeWarfareNew()->SendTowerStateRequest(SWUT_GUARD);
										
					}
					break;				
			}
		}			
		break;
	// -----------------------------------------------------------------------------------------<<
		
	// [070705: Su-won] EVENT_SUMMER_2007
	case MSGLCMD_SUMMER_2007:
		{
			switch(nResult)
			{
				case EVENT_REWARD_1:
					{
						//종이 접기
						_pUIMgr->CloseMessageBox(MSGCMD_SUMMER2007_PAPERFOLDING);
						CUIMsgBox_Info	MsgBoxInfo;
						CTString strMessage =_S(3569, "종이접기");
						MsgBoxInfo.SetMsgBoxInfo( strMessage, UMBS_OKCANCEL | UMBS_BUTTONEX, UI_QUEST, MSGCMD_SUMMER2007_PAPERFOLDING );
						MsgBoxInfo.SetBtnType( UBET_ITEM, CItemData::ITEM_ETC, CItemData::ITEM_ETC_MATERIAL, 3 );
						strMessage = _S(3571, "종이접기를 할 색종이를 올려 놓으세요.");
						MsgBoxInfo.AddString(strMessage);
						strMessage = _S(3572, "종이접기에는 100나스가 필요합니다.");
						MsgBoxInfo.AddString(strMessage);
						_pUIMgr->CreateMessageBox( MsgBoxInfo );
					}
					break;					
				case EVENT_REWARD_2:
					{
						//색종이 조합
						_pUIMgr->CloseMessageBox(MSGCMD_SUMMER2007_PAPERBLENDING);
						CUIMsgBox_Info	MsgBoxInfo;
						CTString strMessage =_S(3570, "색종이 조합");
						MsgBoxInfo.SetMsgBoxInfo( strMessage, UMBS_OKCANCEL | UMBS_BUTTONEX, UI_QUEST, MSGCMD_SUMMER2007_PAPERBLENDING );
						MsgBoxInfo.SetBtnType( UBET_ITEM, CItemData::ITEM_ETC, CItemData::ITEM_ETC_MATERIAL);
						strMessage = _S(3573, "조합할 색종이를 올려 놓으세요.");
						MsgBoxInfo.AddString(strMessage);
						strMessage = _S(3574, "동일한 색종이 5장을 조합하여 상위 색종이 1장을 생성할 수 있습니다.");
						MsgBoxInfo.AddString(strMessage);
						strMessage = _S(3575, "단! 실패할 수도 있으니 주의하세요.");
						MsgBoxInfo.AddString(strMessage);
						_pUIMgr->CreateMessageBox( MsgBoxInfo );
					}
					break;				
			}
		}			
		break;
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// [070807: Su-won] EVENT_ADULT_OPEN
	case MSGLCMD_EVENT_ADULT_MAGICCARD:
		{
			if( nResult ==0)
			{
				_pNetwork->SendAdultOpenEventReq(0);
			}
		}
		break;
	case MSGLCMD_EVENT_ADULT_CHANGEJOB:
		{
			if( nResult ==0)
			{
				_pNetwork->SendAdultOpenEventReq(1);
			}
		}
		break;
	case MSGLCMD_EVENT_ADULT_ALCHEMIST:
		{
			if( nResult ==0 )
			{
				_pUIMgr->CloseMessageBox(MSGCMD_ADULTEVENT_ALCHEMIST);

				CUIMsgBox_Info	MsgBoxInfo;
				CTString strMessage =_S(3635, "장비조합 연금술 이벤트");
				MsgBoxInfo.SetMsgBoxInfo( strMessage, UMBS_OKCANCEL | UMBS_BUTTONEX, UI_QUEST, MSGCMD_ADULTEVENT_ALCHEMIST );
				MsgBoxInfo.SetBtnType(UBET_ITEM, -1, -1);

				strMessage = _S(3636, "조합할 장비를 올려주세요.");
				MsgBoxInfo.AddString(strMessage);
				MsgBoxInfo.AddString(CTString(" "));

				strMessage = _S(2462, "필요 재료");
				MsgBoxInfo.AddString(strMessage, 0xE18600FF);
				MsgBoxInfo.AddString(CTString(" "));
				MsgBoxInfo.AddString(CTString(" "));

				_pUIMgr->CreateMessageBox( MsgBoxInfo );
			}
		}
		break;
		// [070807: Su-won] EVENT_ADULT_OPEN
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case MSGLCMD_TG2007_FRUITFULL: // WSS_TG2007 2007/09/14
		{
			switch(nResult)
			{
				case EVENT_REWARD_1:  // 다음 단계로 업그레이드 한다.
					{
						// TODO ::  업그레이드 요청
						_pNetwork->SendTG2007RichYearReq(MSG_EVENT_RICHYEAR_UPGRADE);
					}
					break;					
				case EVENT_REWARD_2:  // 보상품을 지급 받는다.
					{
						// TODO ::  보상품 지급 요청	
						_pNetwork->SendTG2007RichYearReq(MSG_EVENT_RICHYEAR_EXCHANGE);
					
					}
					break;				
			}
		}			
		break;

		// 붉은색 보물 상자 열기
	case MSGLCMD_RED_TREASUREBOX_EVENT:
		{
			if( nResult == 1 )
			{
				CNetworkMessage nmEvent( MSG_EVENT );
				nmEvent << (UBYTE)MSG_EVENT_TREASUREBOX_RED;
				nmEvent << (UBYTE)MSG_EVENT_TREASUREBOX_RED_OPEN_REQ;
				nmEvent << _pNetwork->MyCharacterInfo.index;
				_pNetwork->SendToServerNew( nmEvent );
			}
		}
		break;

	// [080422: Su-won] EVENT_CHAOSBALL
	case MSGLCMD_CHAOSBALL_EVENT:
		{
			if( nResult == 1)
			{
				if (_pUIMgr->DoesMessageBoxExist(MSGCMD_EVENT_CHAOSBALL))
					_pUIMgr->CloseMessageBox(MSGCMD_EVENT_CHAOSBALL);

				CUIMsgBox_Info	MsgBoxInfo;
				CTString strMessage =_S(4132, "카오스 볼 봉인해제하기");
				MsgBoxInfo.SetMsgBoxInfo( strMessage, UMBS_OKCANCEL | UMBS_BUTTONEX, UI_QUEST, MSGCMD_EVENT_CHAOSBALL );
				MsgBoxInfo.SetBtnType( UBET_ITEM, CItemData::ITEM_ETC, CItemData::ITEM_ETC_MATERIAL, 1 );
				strMessage = _S(4133, "카오스 볼을 올린 후 확인 버튼을 클릭하세요.");
				MsgBoxInfo.AddString(strMessage);
				_pUIMgr->CreateMessageBox( MsgBoxInfo );
			}
		}
	}


}

// ----------------------------------------------------------------------------
// Name : CloseQuest()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuest::CloseQuest()
{
	// Close message box of remission
	_pUIMgr->CloseMessageBox( MSGCMD_QUEST_NOTIFY );
	_pUIMgr->CloseMessageBox( MSGCMD_QUEST_NEWQUEST );
	_pUIMgr->CloseMessageBox( MSGCMD_QUEST_START );
	
	_pUIMgr->CloseMessageBoxL( MSGLCMD_QUEST_REQ); 
	_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_2PAN4PAN_ONE); 
	_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_2PAN4PAN_TWO); 

	_pUIMgr->CloseMessageBox( MSGCMD_ASK_ONE_FIVE );   
	_pUIMgr->CloseMessageBox( MSGCMD_ASK_ONE_FOUR );
	_pUIMgr->CloseMessageBox( MSGCMD_ASK_TWO_OPEN );   

	_pUIMgr->CloseMessageBox( MSGLCMD_EVENT_NEWYEAR1);
	_pUIMgr->CloseMessageBox( MSGLCMD_EVENT_NEWYEAR2);

	_pUIMgr->RearrangeOrder( UI_QUEST, FALSE );
	
	CUIQuestBook::UnlockQuest();
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuest::Render()
{
	// Check distance
	FLOAT	fDiffX = _pNetwork->MyCharacterInfo.x - m_fNpcX;
	FLOAT	fDiffZ = _pNetwork->MyCharacterInfo.z - m_fNpcZ;
	if( fDiffX * fDiffX + fDiffZ * fDiffZ > UI_VALID_SQRDIST )
		CloseQuest();

	// Set remission texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );
	
	// Add render regions
	// Background up
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + PRODUCT_TOP_HEIGHT,
										m_rtBackgroundTop.U0, m_rtBackgroundTop.V0, m_rtBackgroundTop.U1, m_rtBackgroundTop.V1,
										0xFFFFFFFF );

	// Background down 
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY+PRODUCT_TOP_HEIGHT, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
										m_rtBackgroundBottom.U0, m_rtBackgroundBottom.V0, m_rtBackgroundBottom.U1, m_rtBackgroundBottom.V1,
										0xFFFFFFFF );
	
	// Close button
	m_btnClose.Render();
	
	// OK button
	m_btnOK.Render();
	
	// Cancel button
	m_btnCancel.Render();
	
	m_sbQuestIcon.Render();
	
	// List box of remission desc
	m_lbQuestDesc.Render();
	
	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	
	// Quest buttons
	RenderQuestBtns();
	
	if(m_iNpcIndex != -1)
	{
		//CMobData& MD = _pNetwork->GetMobData(m_iNpcIndex);
		CTString strName;
		//strName.PrintF("%s", MD.GetMonsterName());
		strName.PrintF("%s", _pNetwork->GetMobName(m_iNpcIndex));

		// Text in remission
		_pUIMgr->GetDrawPort()->PutTextEx( strName, m_nPosX + QUEST_TITLE_TEXT_OFFSETX,
											m_nPosY + QUEST_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );
	}
	
	_pUIMgr->GetDrawPort()->PutTextExCX( _S( 587, "퀘스트 목록" ), m_nPosX + QUEST_TAB_CX, m_nPosY + QUEST_TAB_SY,		
		0x6B6B6BFF );
	
	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();

	// Quest information
	if( m_bShowQuestInfo )
	{
		// Set Quest texture
		_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

		_pUIMgr->GetDrawPort()->AddTexture( m_rcQuestInfo.Left, m_rcQuestInfo.Top,
											m_rcQuestInfo.Left + 7, m_rcQuestInfo.Top + 7,
											m_rtInfoUL.U0, m_rtInfoUL.V0, m_rtInfoUL.U1, m_rtInfoUL.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_rcQuestInfo.Left + 7, m_rcQuestInfo.Top,
											m_rcQuestInfo.Right - 7, m_rcQuestInfo.Top + 7,
											m_rtInfoUM.U0, m_rtInfoUM.V0, m_rtInfoUM.U1, m_rtInfoUM.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_rcQuestInfo.Right - 7, m_rcQuestInfo.Top,
											m_rcQuestInfo.Right, m_rcQuestInfo.Top + 7,
											m_rtInfoUR.U0, m_rtInfoUR.V0, m_rtInfoUR.U1, m_rtInfoUR.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_rcQuestInfo.Left, m_rcQuestInfo.Top + 7,
											m_rcQuestInfo.Left + 7, m_rcQuestInfo.Bottom - 7,
											m_rtInfoML.U0, m_rtInfoML.V0, m_rtInfoML.U1, m_rtInfoML.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_rcQuestInfo.Left + 7, m_rcQuestInfo.Top + 7,
											m_rcQuestInfo.Right - 7, m_rcQuestInfo.Bottom - 7,
											m_rtInfoMM.U0, m_rtInfoMM.V0, m_rtInfoMM.U1, m_rtInfoMM.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_rcQuestInfo.Right - 7, m_rcQuestInfo.Top + 7,
											m_rcQuestInfo.Right, m_rcQuestInfo.Bottom - 7,
											m_rtInfoMR.U0, m_rtInfoMR.V0, m_rtInfoMR.U1, m_rtInfoMR.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_rcQuestInfo.Left, m_rcQuestInfo.Bottom - 7,
											m_rcQuestInfo.Left + 7, m_rcQuestInfo.Bottom,
											m_rtInfoLL.U0, m_rtInfoLL.V0, m_rtInfoLL.U1, m_rtInfoLL.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_rcQuestInfo.Left + 7, m_rcQuestInfo.Bottom - 7,
											m_rcQuestInfo.Right - 7, m_rcQuestInfo.Bottom,
											m_rtInfoLM.U0, m_rtInfoLM.V0, m_rtInfoLM.U1, m_rtInfoLM.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_rcQuestInfo.Right - 7, m_rcQuestInfo.Bottom - 7,
											m_rcQuestInfo.Right, m_rcQuestInfo.Bottom,
											m_rtInfoLR.U0, m_rtInfoLR.V0, m_rtInfoLR.U1, m_rtInfoLR.V1,
											0xFFFFFFFF );

		// Render all elements
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();

		// Render Quest information
		int	nInfoX = m_rcQuestInfo.Left + 12;
		int	nInfoY = m_rcQuestInfo.Top + 8;
		for( int iInfo = 0; iInfo < m_nCurQuestInfoLines; ++iInfo )
		{
			_pUIMgr->GetDrawPort()->PutTextEx( m_strQuestInfo[iInfo], nInfoX, nInfoY, m_colQuestInfo[iInfo] );
			nInfoY += _pUIFontTexMgr->GetLineHeight();
		}
		//_pUIMgr->GetDrawPort()->PutTextEx( m_strShortDesc, nInfoX, nInfoY, 0xFFFFFFFF );

		// Flush all render text queue
		_pUIMgr->GetDrawPort()->EndTextEx();
	}
}

// ----------------------------------------------------------------------------
// Name : AddQuestInfoString()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuest::AddQuestInfoString( CTString &strQuestInfo, COLOR colQuestInfo )
{
	if( m_nCurQuestInfoLines >= _iMaxMsgStringChar )
		return;

	// Get length of string
	INDEX	nLength = strQuestInfo.Length();
	if( nLength <= 0 )
		return;

	// wooss 051002
	if(g_iCountry == THAILAND){
		// Get length of string
		INDEX	nThaiLen = FindThaiLen(strQuestInfo);
		INDEX	nChatMax= (_iMaxMsgStringChar-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
		if( nLength == 0 )
			return;
		// If length of string is less than max char
		if( nThaiLen <= nChatMax )
		{
		m_strQuestInfo[m_nCurQuestInfoLines] = strQuestInfo;
			m_colQuestInfo[m_nCurQuestInfoLines++] = colQuestInfo;
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = _iMaxMsgStringChar;
			BOOL	b2ByteChar = FALSE;
			for( int iPos = 0; iPos < nLength; iPos++ )
			{
				if(nChatMax < FindThaiLen(strQuestInfo,0,iPos))
					break;
			}
			nSplitPos = iPos;

				// Split string
			CTString	strTemp;
			strQuestInfo.Split( nSplitPos, m_strQuestInfo[m_nCurQuestInfoLines], strTemp );
			m_colQuestInfo[m_nCurQuestInfoLines++] = colQuestInfo;

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

			AddQuestInfoString( strTemp, colQuestInfo );

		}
		
	} else {
		// If length of string is less than max char
		if( nLength <= _iMaxMsgStringChar )
		{
			m_strQuestInfo[m_nCurQuestInfoLines] = strQuestInfo;
			m_colQuestInfo[m_nCurQuestInfoLines++] = colQuestInfo;
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = _iMaxMsgStringChar;
			BOOL	b2ByteChar = FALSE;
			for( int iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strQuestInfo[iPos] & 0x80 )
					b2ByteChar = !b2ByteChar;
				else
					b2ByteChar = FALSE;
			}

			if( b2ByteChar )
				nSplitPos--;

			// Split string
			CTString	strTemp;
			strQuestInfo.Split( nSplitPos, m_strQuestInfo[m_nCurQuestInfoLines], strTemp );
			m_colQuestInfo[m_nCurQuestInfoLines++] = colQuestInfo;

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

			AddQuestInfoString( strTemp, colQuestInfo );
		}
	}
}

// ----------------------------------------------------------------------------
// Name : GetQuestInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuest::GetQuestInfo( int nQuestIndex, SBYTE sbQuestFlag, int &nInfoWidth, int &nInfoHeight )
{
	CTString	strTemp;
	m_nCurQuestInfoLines = 0;
	CQuestDynamicData	*pQuestDD = NULL;

	if(sbQuestFlag == QUEST_FLAG_COMPLETE)
	{
		AddQuestInfoString( strTemp, 0x94BAC6FF );

		pQuestDD = CQuestSystem::Instance().GetDynamicDataByQuestIndex( nQuestIndex );		
		if( pQuestDD != NULL )
		{
			strTemp.PrintF("%s", pQuestDD->GetName());
			AddQuestInfoString( strTemp, 0x94BAC6FF );
			strTemp.PrintF(_S( 465, "레벨 : %d ~ %d" ), pQuestDD->GetNeedMinLevel(), pQuestDD->GetNeedMaxLevel());
			AddQuestInfoString( strTemp, 0x94BAC6FF );
		}
	}
	else
	{
		CQuestDynamicData QuestDD(CQuestSystem::Instance().GetStaticData(nQuestIndex));
		pQuestDD = &QuestDD;
		if( pQuestDD != NULL )
		{
			strTemp.PrintF("%s", pQuestDD->GetName());
			AddQuestInfoString( strTemp, 0x94BAC6FF );
			strTemp.PrintF(_S( 465, "레벨 : %d ~ %d" ), pQuestDD->GetNeedMinLevel(), pQuestDD->GetNeedMaxLevel());
			AddQuestInfoString( strTemp, 0x94BAC6FF );
		}
	}

	nInfoWidth = 27 - _pUIFontTexMgr->GetFontSpacing() + _iMaxMsgStringChar *
					( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
	nInfoHeight = 19 - _pUIFontTexMgr->GetLineSpacing() + m_nCurQuestInfoLines * _pUIFontTexMgr->GetLineHeight();
}

// ----------------------------------------------------------------------------
// Name : ShowQuestInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuest::ShowQuestInfo( BOOL bShowInfo, CUIButtonEx *pQuestBtn )
{
	static int	nOldQuestIndex = -1;

	m_bShowQuestInfo = FALSE;

	// Hide Quest information
	if( !bShowInfo )
	{
		nOldQuestIndex = -1;
		return;
	}

	int		nQuestIndex = pQuestBtn->GetQuestIndex();
	SBYTE	sbQuestFlag	= pQuestBtn->GetQuestFlag();
	BOOL	bUpdateInfo = FALSE;
	int		nInfoWidth, nInfoHeight;
	int		nInfoPosX, nInfoPosY;

	m_bShowQuestInfo = TRUE;

	// Update Quest information
	if( nOldQuestIndex != nQuestIndex )
	{
		bUpdateInfo = TRUE;
		nOldQuestIndex = nQuestIndex;
		pQuestBtn->GetAbsPos( nInfoPosX, nInfoPosY );
		
		nInfoWidth = 27 - _pUIFontTexMgr->GetFontSpacing() + _iMaxMsgStringChar *
			( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
		nInfoHeight = 19 - _pUIFontTexMgr->GetLineSpacing() + 0 * _pUIFontTexMgr->GetLineHeight();

		// Get Quest information
		GetQuestInfo( nQuestIndex, sbQuestFlag, nInfoWidth, nInfoHeight );
		//GetQuestDesc(FALSE, nQuestIndex, 0);
	}

	// Update Quest information box
	if( bUpdateInfo )
	{
		nInfoPosX += BTN_SIZE / 2 - nInfoWidth / 2;

		if( nInfoPosX < _pUIMgr->GetMinI() )
			nInfoPosX = _pUIMgr->GetMinI();
		else if( nInfoPosX + nInfoWidth > _pUIMgr->GetMaxI() )
			nInfoPosX = _pUIMgr->GetMaxI() - nInfoWidth;

		if( nInfoPosY - nInfoHeight < _pUIMgr->GetMinJ() )
		{
			nInfoPosY += BTN_SIZE;
			m_rcQuestInfo.SetRect( nInfoPosX, nInfoPosY, nInfoPosX + nInfoWidth, nInfoPosY + nInfoHeight );
		}
		else
		{
			m_rcQuestInfo.SetRect( nInfoPosX, nInfoPosY - nInfoHeight, nInfoPosX + nInfoWidth, nInfoPosY );
		}
	}

	if( !m_bShowQuestInfo )
	{
		nOldQuestIndex = -1;
		m_bShowQuestInfo = FALSE;
	}
}

// ----------------------------------------------------------------------------
// Name : AddQuestDescString()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuest::AddQuestDescString( CTString &strDesc, const COLOR colDesc )
{
	// Get length of string
	INDEX	nLength = strDesc.Length();
	if( nLength == 0 )
		return;


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
			m_lbQuestDesc.AddString( 0, strDesc, colDesc );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strDesc.Split( iPos, strTemp2, strTemp );
			m_lbQuestDesc.AddString( 0, strTemp2, colDesc );

			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );

			AddQuestDescString( strTemp, colDesc );
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
			m_lbQuestDesc.AddString( 0, strTemp2, colDesc );

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

			AddQuestDescString( strTemp, colDesc );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strDesc.Split( iPos, strTemp2, strTemp );
			m_lbQuestDesc.AddString( 0, strTemp2, colDesc );

			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );

			AddQuestDescString( strTemp, colDesc );
		}
	}
}

// ----------------------------------------------------------------------------
// Name : GetQuestDesc()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuest::GetQuestDesc( BOOL bFullDesc, int nQuestIndex, SBYTE sbFlag )
{
	// Full description
	if( bFullDesc )
	{
		m_lbQuestDesc.ResetAllStrings();
		
		// Quest is not exist
		if( nQuestIndex == -1 )
			return;
		
		CQuestDynamicData	*pQuestDD = NULL;
		//if(sbFlag != QUEST_FLAG_CAN || sbFlag != QUEST_FLAG_NOT)

		// FIXME : 퀘스트 생성시 서버에서 받아오는 부분이 빠졌으니, Dynamic부분 빠져두 되는데...
		if(sbFlag == QUEST_FLAG_COMPLETE)
		{
			pQuestDD = CQuestSystem::Instance().GetDynamicDataByQuestIndex( nQuestIndex );

			if( pQuestDD != NULL )
			{
				AddQuestDescString( pQuestDD->GetIntroDesc(), pQuestDD->GetColorIntroDesc() );
				
				for( INDEX i = 0; i < pQuestDD->GetCountTitleDesc(); ++i )
					AddQuestDescString( pQuestDD->GetTitleDesc( i ), pQuestDD->GetColorTitleDesc( i ) );
				
				//for( i = 0; i < pQuestDD->GetCountStatusDesc(); ++i )
				//	AddQuestDescString( pQuestDD->GetStatusDesc( i ), pQuestDD->GetColorStatusDesc( i ) );
				
				for( i = 0; i < pQuestDD->GetCountPrizeDesc(); ++i )
					AddQuestDescString(  pQuestDD->GetPrizeDesc( i ), pQuestDD->GetColorPrizeDesc( i ) );

				//for( i = 0; i < pQuestDD->GetCountNeedDesc(); ++i )
				//	AddQuestDescString(  pQuestDD->GetNeedDesc( i ), pQuestDD->GetColorNeedDesc( i ) );
			}
		}
		else
		{
			CQuestDynamicData QuestDD(CQuestSystem::Instance().GetStaticData(nQuestIndex));
			pQuestDD = &QuestDD;
			if( pQuestDD != NULL )
			{
				for( INDEX i = 0; i < pQuestDD->GetCountTitleDesc(); ++i )
					AddQuestDescString( pQuestDD->GetTitleDesc( i ), pQuestDD->GetColorTitleDesc( i ) );
				
				// FIXME : 맘에 안듬.
				if(sbFlag == QUEST_FLAG_NOT_LEVEL)
				{
					AddQuestDescString( _S( 592, "레벨 조건이 맞지 않아서 퀘스트를 시작할 수 없습니다." ), 0xFFFFFFFF );	
				}
				else if(sbFlag == QUEST_FLAG_NOT_JOB)
				{
					AddQuestDescString( _S( 593, "직업 조건이 맞지 않아서 퀘스트를 시작할 수 없습니다." ), 0xFFFFFFFF );		
				}
				else if(sbFlag == QUEST_FLAG_NOT_ITEM)
				{
					AddQuestDescString( _S( 594, "아이템 조건이 맞지 않아서 퀘스트를 시작할 수 없습니다." ), 0xFFFFFFFF );		
				}

				//AddQuestDescString( pQuestDD->GetIntroDesc(), pQuestDD->GetColorIntroDesc() );

				//for( INDEX i = 0; i < pQuestDD->GetCountTitleDesc(); ++i )
				//	AddQuestDescString( pQuestDD->GetTitleDesc( i ), pQuestDD->GetColorTitleDesc( i ) );
				
				//for( i = 0; i < pQuestDD->GetCountStatusDesc(); ++i )
				//	AddQuestDescString( pQuestDD->GetStatusDesc( i ), pQuestDD->GetColorStatusDesc( i ) );
				
				//for( i = 0; i < pQuestDD->GetCountPrizeDesc(); ++i )
				//	AddQuestDescString(  pQuestDD->GetPrizeDesc( i ), pQuestDD->GetColorPrizeDesc( i ) );

				for( i = 0; i < pQuestDD->GetCountNeedDesc(); ++i )
					AddQuestDescString(  pQuestDD->GetNeedDesc( i ), pQuestDD->GetColorNeedDesc( i ) );
			}
		}
	}
	// Name
	else
	{
		// FIXME : 퀘스트 생성시 서버에서 받아오는 부분이 빠졌으니, Dynamic부분 빠져두 되는데...
		CQuestDynamicData	*pQuestDD = NULL;
		if(sbFlag == QUEST_FLAG_COMPLETE)
		{
			pQuestDD = CQuestSystem::Instance().GetDynamicDataByQuestIndex( nQuestIndex );
			if( pQuestDD != NULL )
				m_strShortDesc = pQuestDD->GetName();
			else
				m_strShortDesc = CTString( "" );
		}
		else
		{
			CQuestDynamicData QuestDD(CQuestSystem::Instance().GetStaticData(nQuestIndex));
			pQuestDD = &QuestDD;
			if( pQuestDD != NULL )
				m_strShortDesc = pQuestDD->GetName();
			else
				m_strShortDesc = CTString( "" );
		}
	}
}

// ----------------------------------------------------------------------------
// Name : RenderQuestBtns()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuest::RenderQuestBtns()
{
	int	nX = QUEST_SLOT_SX, nY = QUEST_SLOT_SY;
	int	iRow, iRowS, iRowE;	
	
	// Quest remission button
	iRowS = m_sbQuestIcon.GetScrollPos();		
	iRowE = iRowS + QUESTLIST_SLOT_ROW;

	if ( m_vectorbtnQuests.size() < iRowE )
	{
		iRowE = m_vectorbtnQuests.size();
	}

	for( iRow = iRowS; iRow < iRowE; iRow++, nY += QUEST_SLOT_OFFSETY )
	{
		m_vectorbtnQuests[iRow].SetPos( nX, nY );
		if( m_vectorbtnQuests[iRow].IsEmpty() )		
			continue;
		
		m_vectorbtnQuests[iRow].Render();
	}
	
	// Render all button elements
	_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_QUEST );
	
	if( m_nSelQuestID >= 0 )
	{
		// Set remission learn texture
		_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );
		
		m_vectorbtnQuests[m_nSelQuestID].GetAbsPos( nX, nY );
		_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + BTN_SIZE, nY + BTN_SIZE,
												m_rtSelOutline.U0, m_rtSelOutline.V0,
												m_rtSelOutline.U1, m_rtSelOutline.V1,
												0xFFFFFFFF );
		
		// Render all elements
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	}
	
	nY = QUEST_NAME_SY;
	
	iRowS = m_sbQuestIcon.GetScrollPos();
	iRowE = iRowS + QUESTLIST_SLOT_ROW;
	if ( m_vectorbtnQuests.size() < iRowE )
	{
		iRowE = m_vectorbtnQuests.size();
	}
	for( iRow = iRowS; iRow < iRowE; iRow++, nY += QUEST_SLOT_OFFSETY )
	{
		if( m_vectorbtnQuests[iRow].IsEmpty() )
			continue;

		GetQuestDesc( FALSE, m_vectorbtnQuests[iRow].GetQuestIndex(), m_vectorbtnQuests[iRow].GetQuestFlag() );
		_pUIMgr->GetDrawPort()->PutTextExCX( m_strShortDesc, m_nPosX + QUEST_NAME_CX, m_nPosY + nY, 0xC5C5C5FF );
		
		const SBYTE sbFlag = m_vectorbtnQuests[iRow].GetQuestFlag();
		if( sbFlag == QUEST_FLAG_COMPLETE )
		{
			_pUIMgr->GetDrawPort()->PutTextExRX( _S( 429, "완료" ),
				m_nPosX + QUEST_NEED_RX, m_nPosY + nY + 17, 0xFF9170FF );
		}
		//else if( sbFlag == QUEST_FLAG_ING )
		//{
			//_pUIMgr->GetDrawPort()->PutTextExRX( _S( 430, "진행중" ),
				//m_nPosX + QUEST_NEED_RX, m_nPosY + nY + 17, 0xBDA99FFF );
		//}
		else if( sbFlag == QUEST_FLAG_CAN )
		{
			_pUIMgr->GetDrawPort()->PutTextExRX( _S( 588, "수행 가능" ),		
				m_nPosX + QUEST_NEED_RX, m_nPosY + nY + 17, 0xFFC672FF );
		}
		else if( sbFlag == QUEST_FLAG_NOT_LEVEL )
		{
			_pUIMgr->GetDrawPort()->PutTextExRX( _S( 589, "수행 불가능" ),		
				m_nPosX + QUEST_NEED_RX, m_nPosY + nY + 17, 0xBCBCBCFF );
		}
		else if( sbFlag == QUEST_FLAG_NOT_JOB )
		{
			_pUIMgr->GetDrawPort()->PutTextExRX( _S( 589, "수행 불가능" ),		
				m_nPosX + QUEST_NEED_RX, m_nPosY + nY + 17, 0xBCBCBCFF );
		}
		else if( sbFlag == QUEST_FLAG_NOT_ITEM )
		{
			_pUIMgr->GetDrawPort()->PutTextExRX( _S( 589, "수행 불가능" ),		
				m_nPosX + QUEST_NEED_RX, m_nPosY + nY + 17, 0xBCBCBCFF );
		}
		else
		{
			ASSERTALWAYS("호출되면 안되는 부분");
		}
	}
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIQuest::MouseMessage( MSG *pMsg )
{
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
			
			// Move remission
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
			// OK button
			else if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Cancel button
			else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// List box of remission desc
			else if( m_lbQuestDesc.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			
			// Quest remission tab
			else
			{
				// Reset state of selected button
				if( bLButtonDownInBtn && m_nSelQuestID >= 0 && ( pMsg->wParam & MK_LBUTTON ) )
				{
					if( !m_bLockQuest )
					{
						m_vectorbtnQuests[m_nSelQuestID].SetBtnState( UBES_IDLE );
					}
					bLButtonDownInBtn = FALSE;
				}
				// Quest icon scroll bar
				else if( m_sbQuestIcon.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				// Active skill slot
				else if( IsInsideRect( nX, nY, m_rcIcons ) )
				{					
					int	iRowS = m_sbQuestIcon.GetScrollPos();
					int	iRowE = iRowS + QUESTLIST_SLOT_ROW;
					
					if ( m_vectorbtnQuests.size() < iRowE )
					{
						iRowE = m_vectorbtnQuests.size();
					}
					
					int	nWhichRow = -1;
					for( int iRow = iRowS; iRow < iRowE; iRow++ )
					{
						if( m_vectorbtnQuests[iRow].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							nWhichRow = iRow;
						}
					}
					
					// FIXME : 수정해야 하는데, 시간이 부족해서...
					// Show tool tip
					//if( nWhichRow != -1 )
					//	ShowQuestInfo( TRUE, &(m_vectorbtnQuests[nWhichRow]) );
					//else
					//	ShowQuestInfo( FALSE );
					
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
				// OK button
				else if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Cancel button
				else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// List box of remission desc
				else if( m_lbQuestDesc.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Quest remission tab
				else
				{
					// Quest icon scroll bar
					if( m_sbQuestIcon.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Quest slot
					else if( !m_bLockQuest && IsInsideRect( nX, nY, m_rcIcons ) )
					{
						int	nOldSelQuestID = m_nSelQuestID;
						m_nSelQuestID = -1;
						
						int	iRowS = m_sbQuestIcon.GetScrollPos();
						int	iRowE = iRowS + QUESTLIST_SLOT_ROW;

						if ( m_vectorbtnQuests.size() < iRowE )
						{
							iRowE = m_vectorbtnQuests.size();
						}

						for( int iRow = iRowS; iRow < iRowE; iRow++ )
						{
							if( m_vectorbtnQuests[iRow].MouseMessage( pMsg ) != WMSG_FAIL )
							{
								// Update selected remission
								m_nSelQuestID = iRow;
								if( nOldSelQuestID != m_nSelQuestID )
								{									
									const SBYTE sbFlag = m_vectorbtnQuests[iRow].GetQuestFlag();
									GetQuestDesc( TRUE, m_vectorbtnQuests[iRow].GetQuestIndex(), sbFlag );
									if(sbFlag == QUEST_FLAG_CAN || 
										sbFlag == QUEST_FLAG_COMPLETE)
									{
										m_btnOK.SetEnable(TRUE);
									}
									else
									{
										m_btnOK.SetEnable(FALSE);
									}
								}
								
								bLButtonDownInBtn = TRUE;
								
								_pUIMgr->RearrangeOrder( UI_QUEST, TRUE );
								return WMSG_SUCCESS;
							}
						}						
						m_btnOK.SetEnable(FALSE);
					}
				}
				
				_pUIMgr->RearrangeOrder( UI_QUEST, TRUE );
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
				
				// If remission isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;
				
				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseQuest();
					
					return WMSG_SUCCESS;
				}
				// OK button
				else if( ( wmsgResult = m_btnOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						PressOK();
					
					return WMSG_SUCCESS;
				}
				// Cancel button
				else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseQuest();
					
					return WMSG_SUCCESS;
				}
				// List box of remission desc
				else if( m_lbQuestDesc.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				
				// Quest remission tab
				else
				{
					// Quest icon scroll bar
					if( m_sbQuestIcon.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Quest slot
					else if( IsInsideRect( nX, nY, m_rcIcons ) )
					{
						int	iRowS = m_sbQuestIcon.GetScrollPos();
						int	iRowE = iRowS + QUESTLIST_SLOT_ROW;
						if ( m_vectorbtnQuests.size() < iRowE )
						{
							iRowE = m_vectorbtnQuests.size();
						}
						for( int iRow = iRowS; iRow < iRowE; iRow++ )
						{
							if( m_vectorbtnQuests[iRow].MouseMessage( pMsg ) != WMSG_FAIL )
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
				// List box of remission desc
				if( m_lbQuestDesc.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Quest remission tab
				else
				{
					// Quest icon scroll bar
					if( m_sbQuestIcon.MouseMessage( pMsg ) != WMSG_FAIL )
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
				// List box of remission desc
				if( m_lbQuestDesc.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				// Quest remission tab
				else
				{
					// Quest icon scroll bar
					if( m_sbQuestIcon.MouseMessage( pMsg ) != WMSG_FAIL )
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
void CUIQuest::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	// WSS_GUILDMASTER 070416
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;	

	switch( nCommandCode )
	{
	case MSGCMD_HARVEST_MOON_DAY_EVENT_UPGRADE1:
		{
			//2006 추석 이벤트: 송편을 만듦
			if( bOK )
			{
				_pNetwork->Send2006ChuseokRicecakeMake();
			}
			else
			{
				// 이전창으로 돌아감
				MsgBoxLCommand( MSGLCMD_EVENT, QUEST_HARVEST_MOON_DAY_EVENT1 ); 
			}
		}
		break;
	case MSGCMD_HARVEST_MOON_DAY_EVENT_UPGRADE2:
		{
			//2006 추석 이벤트: 오색송편을 만듦
			if( bOK )
			{
				_pNetwork->Send2006ChuseokRainbowMake();
			}
			else
			{
				// 이전창으로 돌아감
				MsgBoxLCommand( MSGLCMD_EVENT, QUEST_HARVEST_MOON_DAY_EVENT1 ); 
			}
		}
		break;
	case MSGCND_MOON_DAY_EVENT_GIVE_ITEM:
		{
			//2006 추석 이벤트: 오색송편을 보상품과 교환함
			if( bOK )
			{
				_pNetwork->Send2006ChuseokExchange();
			}
			else
			{
				// 이전창으로 돌아감
				MsgBoxLCommand( MSGLCMD_EVENT, QUEST_HARVEST_MOON_DAY_EVENT2 );
			}

		}
		break;
	case MSGCMD_QUEST_FAIL:
		{
			CQuestDynamicData	*pQuestDD = CQuestSystem::Instance().GetDynamicDataByQuestIndex( m_iCurQuestIndex );
			if(pQuestDD && _pNetwork->m_bSingleMode)
			{
				INDEX startType = pQuestDD->GetStartType();
				INDEX startData = pQuestDD->GetStartData();
				INDEX prizeNPCIndex = pQuestDD->GetPrizeNPCIndex();
				if( pQuestDD->GetQuestType1() == QTYPE_KIND_SAVE || pQuestDD->GetQuestType1() == QTYPE_KIND_TUTORIAL || pQuestDD->GetQuestType1() == QTYPE_KIND_DEFEAT )			// 싱글던젼 퀘스트의 경우(구출 퀘스트의 경우)
				{
					INDEX questIndex = m_iCurQuestIndex;
					BOOL isComplete = pQuestDD->IsQuestComplete();
					if( CQuestSystem::Instance().Remove( questIndex ) )
					{
						BOOL bDeleted = _pUIMgr->GetQuestBookList()->DelFromQuestList(questIndex, isComplete);
						if(!bDeleted)//뻑나는거 방지용
						{
							bDeleted = _pUIMgr->GetQuestBookList()->DelFromQuestList(questIndex, !isComplete);
							ASSERTALWAYS("Somthing is wrong, client fail delete quest from list");
							if(!bDeleted)
							{
								ASSERTALWAYS("Somthing is wrong, unknown quest index");
							}
						}
					}
					_pUIMgr->GetQuestBookList()->RefreshQuestList();
					
					if(_pUIMgr->GetQuestBookContent()->GetCurrentQuest() == questIndex)
						_pUIMgr->GetQuestBookContent()->CloseQuestBook();
					if(_pUIMgr->GetQuestBookComplete()->GetCurrentQuest() == questIndex)
						_pUIMgr->GetQuestBookComplete()->CloseQuestBook();
					CQuestSystem::Instance().SetQuestAllow(questIndex, CQuestSystem::QAT_ALLOW);
					
					if(startType == QSTART_NPC)
						CQuestSystem::Instance().RefreshNPCQuestMark(startData);
					CQuestSystem::Instance().RefreshNPCQuestMark(prizeNPCIndex);

					// FIXME : 하드 코딩한 부분.
					// Hardcoding
					_pNetwork->GoZone(0, 0);
				}			
			}			
		}
		break;
	case MSGCMD_QUEST_NOTIFY:
		break;
	case MSGCMD_QUEST_NEWQUEST:
		{
			CQuestDynamicData	*pQuestDD = CQuestSystem::Instance().GetDynamicDataByQuestIndex( m_iCurQuestIndex );
			if(pQuestDD)
			{
				if( pQuestDD->GetQuestType1() == QTYPE_KIND_SAVE || pQuestDD->GetQuestType1() == QTYPE_KIND_TUTORIAL || pQuestDD->GetQuestType1() == QTYPE_KIND_DEFEAT )			// 싱글던젼 퀘스트의 경우(구출 퀘스트의 경우)
				{
					// 일단 던젼으로 이동 시키고나서...
					// 무조건 0으로 판단함.				
					const int iWorldNum = pQuestDD->GetConditionData(0, 0);						// 월드 번호.
					const int iExtraNum = pQuestDD->GetConditionData(0, 1);						// Extra 번호.
					//---- woos 060515 ------------------------------------->>
					FLOAT3D tv_pos = (m_fNpcX,0,m_fNpcZ);
					_pNetwork->GoZone(iWorldNum, iExtraNum,m_iNpcIndex);
					//------------------------------------------------------<<
					
				}				
			}
			m_iCurQuestIndex = -1;
		}
		break;
	case MSGCMD_QUEST_START:
		if(bOK)
		{
			SendQuest();
		}
		break;
	case MSGCMD_WEAPONCHANGE_EVENT:		// 무기 교체 이벤트.
		if(bOK)
		{
			_pNetwork->SendChangeWeaponEvent();
		}
		break;

	case MSGCMD_ASK_ONE_FIVE:
		if(bOK){
		_pUIMgr->CloseMessageBox(MSGCMD_ASK_ONE_FIVE);
		_pNetwork->SendEvent24(1,MSG_EVENT_2PAN4PAN_MONEY_REQ);
		}
		break;
	case MSGCMD_ASK_ONE_FOUR:
		if(bOK){
		_pUIMgr->CloseMessageBox(MSGCMD_ASK_ONE_FOUR);
		_pNetwork->SendEvent24(2,MSG_EVENT_2PAN4PAN_MONEY_REQ);
		}
		break;
	case MSGCMD_ASK_TWO_OPEN:
		if(bOK){
		_pUIMgr->CloseMessageBox(MSGCMD_ASK_TWO_OPEN);
		_pNetwork->SendEvent24(0,MSG_EVENT_2PAN4PAN_GOODS_REQ);
		}
		break;
	case MSGCMD_ASK_NEWYEAR1:
		if(bOK)
		{			
			_pNetwork->SendEventNewyear(EVENT_NEWYEAR1_REWARD);
		}
		break;
	case MSGCMD_ASK_NEWYEAR2:
		if(bOK)
		{
			_pNetwork->SendEventNewyear(EVENT_NEWYEAR2_REWARD);
		}
		break;
	case MSGCMD_INPUT_FRIEND :
		if(bOK)
		{
			_pNetwork->SendFindFriend(MSG_EVENT_SEARCHFRIEND_ADD_CHECK,&strInput);

		}
		break;
	case MSGCMD_EVENT_COUPON_SENDNUM :
		if(bOK){
			_pNetwork->SendCouponItemReq(strInput);			
		}
		break;
	case MSGLCMD_EVENT_OXQUIZ:
		if(bOK)
		{
			_pNetwork->GoZone( 14, 0 ,254);
		}
		break;
	
	case MSGCMD_WORLDCUP_EVENT1_NUM:
		if(bOK)
		{				
			iSelCountry = iGroup[iSelGroup] + _pUIMgr->GetMessageBox(MSGCMD_WORLDCUP_EVENT1_NUM)->GetComboBox().GetCurSel();
			_pNetwork->SendWorldCupEvent( MSGCMD_WORLDCUP_EVENT1_EXCHANGE, iSelCountry );
		}
		break;
	case MSGCMD_COLLECTBOX_BUY_REQ:
		if(bOK)
		{
			_pNetwork->SendBuyCollectBox();
		}
		break;
	case MSGCMD_PROMO_KEY_ENTER:
		if(bOK)
		{
			_pNetwork->SendCouponItemReq(strInput);			
		}
		break;
	case MSGCMD_OK_EXCHANGE_CHOCOBOX:
	case MSGCMD_OK_EXCHANGE_LUCKYBOX:
		if (bOK)
		{
			int nItemIndex = _pUIMgr->GetMessageBox(nCommandCode)->GetBtnEx().GetItemIndex();

			_pNetwork->SendReqGiftChocoBox(nItemIndex);
		}
		break;
	case MSGCMD_GUILDMASTER_1:
		if(bOK) // 등록
		{
			if(CheckGuildMasterEvent())
			{
				_pUIMgr->DoesMessageBoxExist(MSGCMD_GUILDMASTER_1_1);
				MsgBoxInfo.SetMsgBoxInfo( _S( 100, "이벤트" ), UMBS_OKCANCEL, UI_QUEST, MSGCMD_GUILDMASTER_1_1);
				strMessage.PrintF( _S(3469, "소유하신 빨간 카네이션이 사라지고 그 개수만큼 길드 포인트가 올라 갑니다. 진행하시겠습니까?" ));
				MsgBoxInfo.AddString( strMessage );
				_pUIMgr->CreateMessageBox( MsgBoxInfo );
			}
			
		}
		else	// 갱신	
		{
			_pNetwork->SendRequestParentsday( MSG_EVENT_PARENTSDAY_2007_ACCUMULATEPOINT_RANKING_LIST);
		}
		break;	
	case MSGCMD_GUILDMASTER_1_1:
		if(bOK)	// 등록 메시지
		{			
			_pNetwork->SendParentsdayAddPoint(_pNetwork->MyCharacterInfo.index);
		}		
		break;

	case MSGCMD_GUILDMASTER_2_1:
		if(bOK)
		{
			// 교환권 받기
			_pNetwork->SendRequestParentsday( MSG_EVENT_PARENTSDAY_2007_EXCHANGE_TICKET);
		}
		break;

		///////////////////////////////////////////////////////////////////////////////////
		// [070705 :Su-won] EVENT_SUMMER_2007
	case MSGCMD_SUMMER2007_PAPERFOLDING:
		if(bOK)
		{
			CUIMessageBox* pMsgBox =_pUIMgr->GetMessageBox(MSGCMD_SUMMER2007_PAPERFOLDING);
			int nItemIndex0 =pMsgBox->GetBtnEx(0).GetItemIndex();
			int nItemIndex1 =pMsgBox->GetBtnEx(1).GetItemIndex();
			int nItemIndex2 =pMsgBox->GetBtnEx(2).GetItemIndex();

			_pNetwork->SendPaperFoldingReq( nItemIndex0, nItemIndex1, nItemIndex2);
		}
		else
			MsgBoxLCommand( MSGLCMD_EVENT, EVENT_SUMMER_2007 ); 

		break;
	case MSGCMD_SUMMER2007_PAPERBLENDING:
		if(bOK)
		{
			int nItemIndex =_pUIMgr->GetMessageBox(MSGCMD_SUMMER2007_PAPERBLENDING)->GetBtnEx(0).GetItemIndex();
			
			_pNetwork->SendPaperBlendingReq( nItemIndex );
		}
		else
			MsgBoxLCommand( MSGLCMD_EVENT, EVENT_SUMMER_2007 ); 
		break;
		// [070705 :Su-won] EVENT_SUMMER_2007
		///////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
	// [070807: Su-won] EVENT_ADULT_OPEN
		//성인서버 오픈 이벤트: 연금술 이벤트
	case MSGCMD_ADULTEVENT_ALCHEMIST:
		{
			if(bOK)
			{
				int tv_tab,tv_row,tv_col;

				tv_tab =_pUIMgr->GetMessageBox(MSGCMD_ADULTEVENT_ALCHEMIST)->GetBtnEx(0).GetItemTab();
				tv_row =_pUIMgr->GetMessageBox(MSGCMD_ADULTEVENT_ALCHEMIST)->GetBtnEx(0).GetItemRow();
				tv_col =_pUIMgr->GetMessageBox(MSGCMD_ADULTEVENT_ALCHEMIST)->GetBtnEx(0).GetItemCol();

				_pNetwork->SendAdultOpenEventReq( 2, tv_tab, tv_row, tv_col);
				
			}
		}
		break;
		//'거침없이 쏜다!' 이벤트
	case MSGCMD_EVENT_SHOOT:
		{
			if(bOK)
			{
				CNetworkMessage nmMessage(MSG_EVENT);
				nmMessage << (UBYTE)MSG_EVENT_GIFT_2007;

				_pNetwork->SendToServerNew(nmMessage);
			}
		}
		break;
	// [070807: Su-won] EVENT_ADULT_OPEN
	///////////////////////////////////////////////////////
	case MSGCMD_BJMONO_2007_CHANGE_REQ:
		{
			if (bOK)
			{
				if (!(strInput.IsInteger()))
				{
					_pNetwork->ClientSystemMessage(_s("숫자를 입력해 주시기 바랍니다."));
					return;
				}

				const char* TempNum = strInput;
				int Num = atoi(TempNum);

				if (Num < 1 || Num > 6)
				{
					_pNetwork->ClientSystemMessage(_s("1~6장의 응모권만 교환 가능 합니다."));
					return;
				}

				_pNetwork->SendBJMono2007Req(MSG_EVENT_BJMONO_2007_CHANGE_TICKET_REQ, Num);
			}
		}
		break;
	case MSGCMD_TG2007_1000DAYS_CAP_UPGRADE:
		{
			if (bOK)
			{
				int iRow, iCol, iItemIndex;

				iRow = _pUIMgr->GetMessageBox(MSGCMD_TG2007_1000DAYS_CAP_UPGRADE)->GetBtnEx().GetItemRow();
				iCol = _pUIMgr->GetMessageBox(MSGCMD_TG2007_1000DAYS_CAP_UPGRADE)->GetBtnEx().GetItemCol();
				iItemIndex = _pUIMgr->GetMessageBox(MSGCMD_TG2007_1000DAYS_CAP_UPGRADE)->GetBtnEx().GetItemIndex();

				_pNetwork->SendLC1000DayHatUpgradeReq((SBYTE)iRow, (SBYTE)iCol, (LONG)iItemIndex);
			}
		}
		break;
	case MSGCMD_EVENT_XMAS_2007_DECO:
		{
			if (bOK)
			{   // 크리스 마스 장식
				_pNetwork->SendXMAS2007DecoReq(MSG_EVENT_XMASTREE_ADD_POINT);
			}
		}
		break;
	case MSGCMD_EVENT_SAKURA_2008:
		{
			if(bOK)
			{
				if( _pUIMgr->GetInventory()->GetItemCount(2345) <=0 )
				{
					CUIMsgBox_Info MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);

					MsgBoxInfo.AddString(_S(4114, "퇴비를 가지고 있지 않습니다."));
					_pUIMgr->CreateMessageBox(MsgBoxInfo);

					return;
				}

				CNetworkMessage nmMessage(MSG_EVENT);

				nmMessage << (UBYTE)MSG_EVENT_SAKURA_2008;
				nmMessage << (UBYTE)MSG_EVENT_SAKURAEVENT_2008;

				_pNetwork->SendToServerNew(nmMessage);
			}
		}
		break;
	// [080422: Su-won] EVENT_CHAOSBALL
	case MSGCMD_EVENT_CHAOSBALL:
		{
			if(bOK)
			{
				int tv_tab,tv_row,tv_col;

				tv_tab =_pUIMgr->GetMessageBox(MSGCMD_EVENT_CHAOSBALL)->GetBtnEx(0).GetItemTab();
				tv_row =_pUIMgr->GetMessageBox(MSGCMD_EVENT_CHAOSBALL)->GetBtnEx(0).GetItemRow();
				tv_col =_pUIMgr->GetMessageBox(MSGCMD_EVENT_CHAOSBALL)->GetBtnEx(0).GetItemCol();

				CNetworkMessage nmMessage(MSG_EXTEND);
				nmMessage << (ULONG)MSG_EX_CHAOSBALL;
				nmMessage << (LONG)tv_row;
				nmMessage << (LONG)tv_col;

				_pNetwork->SendToServerNew(nmMessage);
			}
		}
		break;

	}
}

// ========================================================================= //
//                           Send message functions                          //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : SendQuest()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuest::SendQuest()
{
	ASSERT(0 && "Not use");
	//Close message box of remission learn
	_pUIMgr->CloseMessageBox( MSGCMD_QUEST_NOTIFY );
	_pUIMgr->CloseMessageBox( MSGCMD_QUEST_NEWQUEST );
	_pUIMgr->CloseMessageBox( MSGCMD_QUEST_START );
	
	if( m_nSelQuestID < 0 )
		return;
	
	if( m_vectorbtnQuests[m_nSelQuestID].IsEmpty() )
		return;
	
	const SBYTE sbQuestFlag = m_vectorbtnQuests[m_nSelQuestID].GetQuestFlag();
	//ASSERT(sbQuestFlag != QUEST_FLAG_NOT_LEVEL || sbQuestFlag != QUEST_FLAG_ING);
	const int iQuestIndex = m_vectorbtnQuests[m_nSelQuestID].GetQuestIndex();

	// FIXME : SendQuestMessage()에 퀘스트 플래그에 따라서 처리하도록 하면 될듯...
	if( sbQuestFlag == QUEST_FLAG_COMPLETE )
	{
		//_pNetwork->SendQuestPrizeMessage( MSG_QUEST_PRIZE, iQuestIndex, m_iNpcIndex);
	}
	else if( sbQuestFlag == QUEST_FLAG_CAN)
	{
		_pNetwork->SendQuestMessage( MSG_QUEST_START, iQuestIndex );
	}
	//else if( sbQuestFlag == QUEST_FLAG_ING )
	//{
	//	_pNetwork->SendQuestMessage( MSG_QUEST_GIVEUP, iQuestIndex );
	//}
}

// ========================================================================= //
//                         Receive message functions                         //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : PressOK()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuest::PressOK()
{
	ASSERT(0 && "Not use");
	// Close message box of remission
	_pUIMgr->CloseMessageBox( MSGCMD_QUEST_START );	
	_pUIMgr->CloseMessageBox( MSGCMD_QUEST_NOTIFY );
	_pUIMgr->CloseMessageBox( MSGCMD_QUEST_NEWQUEST );

	const int iQuestIndex = m_vectorbtnQuests[m_nSelQuestID].GetQuestIndex();
	const SBYTE sbQuestFlag = m_vectorbtnQuests[m_nSelQuestID].GetQuestFlag();

	switch(sbQuestFlag)
	{
	case QUEST_FLAG_COMPLETE:
		{
			SendQuest();
		}
		break;		
	//case QUEST_FLAG_ING:
		//{
			//CTString	strMessage;
			//CUIMsgBox_Info	MsgBoxInfo;	
			//MsgBoxInfo.SetMsgBoxInfo( _S( 99, "퀘스트" ), UMBS_OKCANCEL, UI_QUEST, MSGCMD_QUEST_START);		
			//strMessage.PrintF( _T( 504, "정말 이 퀘스트를 취소하시겠습니까?" ));	
			//MsgBoxInfo.AddString( strMessage );
			//_pUIMgr->CreateMessageBox( MsgBoxInfo );			
		//}
		//break;
	case QUEST_FLAG_CAN:
		{
			//CQuestDynamicData	*pQuestDD = CQuestSystem::Instance().Create(iQuestIndex);			
			_pUIMgr->GetQuestBookNew()->OpenQuestBook();
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : QuestError()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuest::QuestError( UBYTE ubError )
{
	CTString	strMessage;
	
	switch(ubError)
	{
	case MSG_QUEST_ERR_START_ONCE:		// 퀘스트 시작시 에러 : 이미 수행한 1회용 퀘스트
		strMessage = _S( 590, "해당 퀘스트는 이미 수행한 퀘스트입니다." );		
		break;
	case MSG_QUEST_ERR_START_ALREADY:	// 퀘스트 시작시 에러 : 이미 수행중임
		strMessage = _S( 591, "이미 수행중인 퀘스트입니다." );		
		break;
	case MSG_QUEST_ERR_START_LEVEL:		// 퀘스트 시작시 에러 : 레벨 조건 안맞음
		strMessage = _S( 592, "레벨 조건이 맞지 않아서 퀘스트를 시작할 수 없습니다." );		
		break;
	case MSG_QUEST_ERR_START_JOB:		// 퀘스트 시작시 에러 : 직업 조건 안맞음
		strMessage = _S( 593, "직업 조건이 맞지 않아서 퀘스트를 시작할 수 없습니다." );		
		break;
	case MSG_QUEST_ERR_START_ITEM:		// 퀘스트 시작시 에러 : 아이템 조건 안맞음
		strMessage = _S( 594, "아이템 조건이 맞지 않아서 퀘스트를 시작할 수 없습니다." );		
		break;
	case MSG_QUEST_ERR_START_MAX:		// 퀘스트 시작시 에러 : 최대 수행 가능 수 초과
		strMessage = _S( 595, "수행 가능한 퀘스트 횟수를 초과했습니다." );		
		break;
	case MSG_QUEST_ERR_START_FULL:		// 퀘스트 시작시 에러 : 싱글던전의 경우 싱글던전 꽉참
		strMessage = _S( 596, "싱글던젼이 혼잡하여 퀘스트를 시작할 수 없습니다" );		
		break;
	case MSG_QUEST_ERR_DELIVERY_FULL:	// 전달 퀘스트 시 : 인벤토리가 꽉차서 아이템 퀘스트 에러
		strMessage = _S( 597, "인벤토리가 꽉 차서 퀘스트를 시작할 수 없습니다." );		
		break;
	case MSG_QUEST_ERR_PRIZE_FULL:		// 보상 받을때 인벤 꽉 차서 보상 실패
		strMessage = _S( 598, "인벤토리가 꽉 차서 보상을 받을 수 없습니다." );		
		break;
	case MSG_QUEST_ERR_PRZIE_ITEM:		// 보상 받을때 아이템 때문에 오류
		strMessage = _S( 599, "아이템 조건이 맞지 않아 보상을 받을 수 없습니다." );		
		break;
	case MSG_QUEST_ERR_PET_NOT_HAVE_ONEMORE:
		{
			strMessage = _S(2221,"펫을 더 이상 소유할 수 없습니다.");
			_pUIMgr->GetQuestBookComplete()->CloseQuestBook();
		}
		break;
	

	}

	// Close message box of remission
	_pUIMgr->CloseMessageBox( MSGCMD_QUEST_NOTIFY );
	_pUIMgr->CloseMessageBox( MSGCMD_QUEST_NEWQUEST );
	_pUIMgr->CloseMessageBox( MSGCMD_QUEST_START );
	
	// Create message box of remission
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 99, "퀘스트" ), UMBS_OK,			
								UI_QUEST, MSGCMD_QUEST_NOTIFY );
	MsgBoxInfo.AddString( strMessage );
	_pUIMgr->CreateMessageBox( MsgBoxInfo );
}

// ----------------------------------------------------------------------------
// Name : AddToQuestList()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuest::AddToQuestList(INDEX iQuestIndex, SBYTE sbQuestFlag)
{
	sQuestList	QuestList;
	QuestList.iQuestIndex	= iQuestIndex;
	QuestList.sbQuestFlag	= sbQuestFlag;
	m_vectorQuestList.push_back(QuestList);
}

// ----------------------------------------------------------------------------
// Name : ClearQuestList()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuest::ClearQuestList()
{
	if(!m_vectorQuestList.empty())
	{
		m_vectorQuestList.clear();
	}
}

// ----------------------------------------------------------------------------
// Name : LockQuest()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuest::LockQuest(BOOL bLock)
{ 
	if(!IsVisible())	return;
	m_bLockQuest = bLock; 
	if(bLock)
	{
		m_btnOK.SetEnable(FALSE);
		m_btnCancel.SetEnable(FALSE);
		m_btnClose.SetEnable(FALSE);
	}
	else
	{
		m_btnCancel.SetEnable(TRUE);
		m_btnClose.SetEnable(TRUE);
		if(m_nSelQuestID != -1)
		{
			const SBYTE sbFlag = m_vectorbtnQuests[m_nSelQuestID].GetQuestFlag();
			if(sbFlag == QUEST_FLAG_CAN || 
				sbFlag == QUEST_FLAG_COMPLETE)
			{
				m_btnOK.SetEnable(TRUE);
			}
		}
	}
}


#include "stdh.h"
#include <Engine/Interface/UIAutoHelp.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Interface/UIInternalClasses.h>

CUIAutoHelp*	_UIAutoHelp = NULL;
extern INDEX g_iCountry;
bool _SAutoHelpInfo::CheckInfo()
{
	// 한번만 하는 거여 .. ~
	if ( m_bJustOnce ) 
	{
		if ( m_bActive )
		{
			return false;
		}
	}

	if ( m_iEndLevel == -1 && m_iStartLevel == -1 )
	{
	
	}
	else
	{
		// FIXME : 헤더 내에서 네트워크를 사용하고 있음.
		SWORD Level = _pNetwork->MyCharacterInfo.level;
					
		// 레벨 체크 
		if ( Level < m_iStartLevel || Level > m_iEndLevel )
		{
			return false;
		}
	}
	// 종족 체크 ( 생략 즐 )

	
	m_bActive = true; 
	return true;
}

// ----------------------------------------------------------------------------
// Name : CUIAutoHelp()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIAutoHelp::CUIAutoHelp()
{
	m_ptdBaseTexture = NULL;
	Clear ();
}

// ----------------------------------------------------------------------------
// Name : ~CUIAutoHelp()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIAutoHelp::~CUIAutoHelp()
{
	if( m_ptdBaseTexture )
	{
		_pTextureStock->Release( m_ptdBaseTexture );
		m_ptdBaseTexture = NULL;
	}

	Clear ();
}

// ----------------------------------------------------------------------------
// Name : Clear()
// Desc :
// ----------------------------------------------------------------------------
void CUIAutoHelp::Clear()
{

	m_bStop				= FALSE;		// 정지 상태인감?
	m_fShowTime			= AUTOHELO_SHOW_TIME;	// 보여 지고 있는 시간
	m_tmStartTime		= 0;			// 보이기 시작 한 시간
	m_bVisible			= FALSE;		// 화면에 보여 주고 있는가?
	m_nActiveIndex		= -1;			// 현제 보여 주고 있는 도움말 Index
	m_tmEndTime			= 0;
	m_nKillMobIndex		= -1;
	m_RndHelp			= FALSE;

	for ( int i = 0; i < AUTOHELP_COUNT; i++ )
	{
		m_AutoHelpInfo[i].Clear();
	}
		
	memset ( m_bCondition, FALSE, sizeof ( BOOL ) * AUTOHELP_COUNT );
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIAutoHelp::Create()
{
	Clear ();
	
	// initialize valiables ---------------------wooss 060328-->>
	m_tmCheckTime	= _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	m_bChecked		= FALSE;
	m_iNoticeTime	= 3;
	// --------------------------------------------------------<<

	// Create refine texture
	m_ptdBaseTexture = _pTextureStock->Obtain_t ( CTString( "Data\\Interface\\Notice.tex" ) );

	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();
	
	m_rcGMNotice.SetRect( 0, 75, 0, 94 );
	m_rcNotice.SetRect( 0, 100, 0, 119 );
	
	
	// Notice
	m_rtNoticeL.SetUV( 365, 131, 397, 150, fTexWidth, fTexHeight );
	m_rtNoticeC.SetUV( 404, 131, 411, 150, fTexWidth, fTexHeight );
	m_rtNoticeR.SetUV( 419, 131, 451, 150, fTexWidth, fTexHeight );

	
	m_AutoHelpInfo[0].SetInfo ( _S( 467, "앉아 있으면 체력이 더 빨리 회복 됩니다." ), 
								1, 2 );

	m_AutoHelpInfo[1].SetInfo ( _S( 468,  "바닥에 떨어진 아이템을 마우스로 클릭하면 집을 수 있습니다." ), 
								1, 2, -1, TRUE );

	m_AutoHelpInfo[2].SetInfo (  _S( 469, "줍기 버튼을 이용하면 더 편리하게 아이템을 집을 수 있습니다." ),
								1, 2, -1, TRUE );

	m_AutoHelpInfo[3].SetInfo (  _S( 470, "스킬마스터를 찾아가면, 현재의 스킬레벨을 올리거나 새로운 스킬을 배울 수 있습니다." ) ,
								1, 3 );

	m_AutoHelpInfo[4].SetInfo (  _S( 471, "드라탄에는 생산지역이 있습니다. 지도를 열어 보세요." ),
								1, 4, -1, TRUE );

	m_AutoHelpInfo[5].SetInfo ( _S( 472, "5레벨이 되시면, 퍼스널 던전에 갈 수 있습니다." ), 
								2, 4 );
	// Quest ...			
	m_AutoHelpInfo[6].SetInfo ( _S( 473, "현재 레벨에서 [약제사 에노린]에게 가면 퀘스트를 수행할 수 있습니다." ), 
								1, 1, -1, TRUE );
	m_AutoHelpInfo[7].SetInfo ( _S( 474, "현재 레벨에서 [경비대장 레온]에게 가면 생산 스킬 퀘스트를 수행할 수 있습니다." ), 
								7, 7,-1, TRUE );
	m_AutoHelpInfo[8].SetInfo ( _S( 475, "현재 레벨에서 트라탄의 [마을 관리인 렌]에게 가면 생산 스킬 퀘스트를 수행할 수 있습니다." ), 
								8, 8, -1,TRUE );
	m_AutoHelpInfo[9].SetInfo ( _S( 476, "현재 레벨에서 [잡화상인 게레스]에게 가면 퀘스트를 수행할 수 있습니다." ), 
								9, 9, -1,TRUE );
	m_AutoHelpInfo[10].SetInfo ( _S( 477, "현재 레벨에서 [경비대장 레온]에게 가면 퀘스트를 수행할 수 있습니다." ), 
								15, 15, -1, TRUE );
	m_AutoHelpInfo[11].SetInfo ( _S( 478, "현재 레벨에서 [방어구상인 로엘]에게 가면 퀘스트를 수행할 수 있습니다." ), 
								23, 23, -1,TRUE );
	
	m_AutoHelpInfo[12].SetInfo ( _S( 479, "힘이 상승하면 인벤토리에 소지할 수 있는 물품의 무게가 증가합니다." ), 
								1, 9, -1 );

	// Random Help ...
	m_AutoHelpInfo[13].SetInfo ( _S( 480, "사용하지 않는 무기와 방어구를 알케미스트에게 가져가시면 제련석으로 교환할 수 있습니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[14].SetInfo ( _S( 481, "ALT+B, B키를 누르면 공지사항 및 게시판을 확인할 수 있습니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[15].SetInfo ( _S( 482, "ALT+W, W키를 누르면 지도를 통해서 게임 내 NPC 위치를 확인할 수 있습니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[16].SetInfo ( _S( 483, "ALT+T, T키를 누르면 공격력 방어력 등의 캐릭터 정보를 확인할 수 있습니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[17].SetInfo ( _S( 484, "ALT+A, A키를 누르면 공격/앉기/줍기/교환 아이콘을 확인할 수 있습니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );
	
	m_AutoHelpInfo[18].SetInfo ( _S( 485, "ALT+S, S키를 누르면 일반, 강화, 특수스킬을 확인할 수 있습니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[19].SetInfo ( _S( 486, "ALT+Q, Q키를 누르면 진행중인 퀘스트를 확인할 수 있습니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );
	
	m_AutoHelpInfo[20].SetInfo ( _S( 487, "ALT+C, C키를 누르면 소셜창을 확인할 수 있습니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[21].SetInfo ( _S( 488, "ALT+Z, Z키를 누르면 옵션설정의 시스템창을 확인할 수 있습니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[22].SetInfo ( _S( 489, "TAB, ALT+E, E키를 이용하여 캐릭터 인벤토리창을 열 수 있습니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[23].SetInfo ( _S( 490, "귓속말은 !캐릭터명 하고싶은 말을 통해서 이용할 수 있습니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[24].SetInfo ( _S( 491, "사냥중 앉기를 하면 서있을 때보다 빠르게 HP와 MP가 회복됩니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[25].SetInfo ( _S( 492, "인벤토리창을 연후 아이템을 더블 클릭하면 장착 및 탈착을 할 수 있습니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );
	
	m_AutoHelpInfo[26].SetInfo ( _S( 493, "아이템 업그레이드는 제련석을 이용하여 업그레이드할 수 있습니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[27].SetInfo ( _S( 494, "제련석은 사냥을 통해서 획득하거나 란돌성의 알케미스트를 통해서 교환할 수 있습니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[28].SetInfo ( _S( 495, "방향키(화살표를 이용하면 화면이동 및 시점변환을 할 수 있습니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[29].SetInfo ( _S( 496, "다른 존으로 이동하기 위해서는 공간술사 NPC를 통해서 이동할 수 있습니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[30].SetInfo ( _S( 497, "지도의 가장자리를 마우스로 움직이면, 지도의 크기를 확대 축소할 수 있습니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[31].SetInfo ( _S( 498, "지도를 연 상태에서 마을을 클릭하면 마을 상세지도가 나옵니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );
	
	// 070820_ttos: 브라질의 경고문구, 레벨 제한 없이 출력 
	if( g_iCountry == BRAZIL)
	{
		m_AutoHelpInfo[AU_NO_TOXICOSIS_S].SetInfo ( _S( 2547,"장시간의 게임은 몸과 마음을 병들게 합니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_MAX );
		m_AutoHelpInfo[AU_NO_TOXICOSIS_S+1].SetInfo ( _S( 2548,"장시간의 게임은 몸과 마음을 병들게 합니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_MAX );
		m_AutoHelpInfo[AU_NO_TOXICOSIS_S+2].SetInfo ( _S( 2549,"장시간의 게임은 몸과 마음을 병들게 합니다." ), 
									RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_MAX );
		m_AutoHelpInfo[AU_NO_TOXICOSIS_S+3].SetInfo ( _S( 2550,"장시간의 게임은 몸과 마음을 병들게 합니다." ), 
									RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_MAX );
		
		m_AutoHelpInfo[AU_NO_TOXICOSIS_VACATION_S].SetInfo ( _S( 2551,"방학...의외로 짧답니다...^^;" ), 
									RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_MAX );
		m_AutoHelpInfo[AU_NO_TOXICOSIS_VACATION_S+1].SetInfo ( _S( 2552,"방학...의외로 짧답니다...^^;" ), 
									RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_MAX );
		m_AutoHelpInfo[AU_NO_TOXICOSIS_VACATION_S+2].SetInfo ( _S( 2553,"방학...의외로 짧답니다...^^;" ), 
									RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_MAX );
		m_AutoHelpInfo[AU_NO_TOXICOSIS_VACATION_S+3].SetInfo ( _S( 2554,"방학...의외로 짧답니다...^^;" ), 
									RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_MAX );
	

	}else
	{
		m_AutoHelpInfo[AU_NO_TOXICOSIS_S].SetInfo ( _S( 2547,"장시간의 게임은 몸과 마음을 병들게 합니다." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );
		m_AutoHelpInfo[AU_NO_TOXICOSIS_S+1].SetInfo ( _S( 2548,"장시간의 게임은 몸과 마음을 병들게 합니다." ), 
									RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );
		m_AutoHelpInfo[AU_NO_TOXICOSIS_S+2].SetInfo ( _S( 2549,"장시간의 게임은 몸과 마음을 병들게 합니다." ), 
									RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );
		m_AutoHelpInfo[AU_NO_TOXICOSIS_S+3].SetInfo ( _S( 2550,"장시간의 게임은 몸과 마음을 병들게 합니다." ), 
									RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );
		
		m_AutoHelpInfo[AU_NO_TOXICOSIS_VACATION_S].SetInfo ( _S( 2551,"방학...의외로 짧답니다...^^;" ), 
									RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );
		m_AutoHelpInfo[AU_NO_TOXICOSIS_VACATION_S+1].SetInfo ( _S( 2552,"방학...의외로 짧답니다...^^;" ), 
									RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );
		m_AutoHelpInfo[AU_NO_TOXICOSIS_VACATION_S+2].SetInfo ( _S( 2553,"방학...의외로 짧답니다...^^;" ), 
									RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );
		m_AutoHelpInfo[AU_NO_TOXICOSIS_VACATION_S+3].SetInfo ( _S( 2554,"방학...의외로 짧답니다...^^;" ), 
									RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );
		

	}
	
	
	m_tmEndTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	m_RndHelp			= FALSE;
	
}

// ----------------------------------------------------------------------------
// Name : SetInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIAutoHelp::SetInfo ( DWORD dwAutoHelpInfo )
{
	if ( dwAutoHelpInfo < 0 || dwAutoHelpInfo >= AUTOHELP_COUNT ) 
	{
		return;
	}

	TIME NowTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

	if ( NowTime - m_tmEndTime < INTERVAL_TIME ) return;

	if( AU_NO_TOXICOSIS_S <= dwAutoHelpInfo && AU_NO_TOXICOSIS_VACATION_E >= dwAutoHelpInfo)
	{
		ShowAutoHelp ( dwAutoHelpInfo );
		return;
	}
	
	switch ( dwAutoHelpInfo )
	{

	case AU_MOB_KILL :
		{
			if ( m_bVisible )
			{
				if ( m_nActiveIndex != AU_GET_SKILL_POINT )
					return;

				m_bVisible			= FALSE;
				m_nActiveIndex		= -1;
			}
	
			float CharHp	= _pNetwork->MyCharacterInfo.hp;
			float CharMaxHp = _pNetwork->MyCharacterInfo.maxHP;

			float hp = ( CharHp / CharMaxHp ) * 100.0f;

			if ( hp <= 40 ) // 체력이 40% 이하이면...
			{
				if ( m_AutoHelpInfo[dwAutoHelpInfo].CheckInfo() )
				{
					ShowAutoHelp ( dwAutoHelpInfo );
				}
				break;
			}
		}
		break;

	case AU_ITEM_DROP :
		{
			if ( m_bVisible )
			{
				if ( m_nActiveIndex != AU_GET_SKILL_POINT )
					return;
	
				// 초기화
				m_bVisible			= FALSE;
				m_nActiveIndex		= -1;
		
			}

			if ( m_AutoHelpInfo[dwAutoHelpInfo].CheckInfo() )
			{
				ShowAutoHelp ( dwAutoHelpInfo );
			}

			m_bCondition[dwAutoHelpInfo] = TRUE;


		}
	case AU_GET_SKILL_POINT :
		{
			if ( m_bVisible ) return;
			if ( !m_AutoHelpInfo[dwAutoHelpInfo].m_bActive )
			{
				m_AutoHelpInfo[dwAutoHelpInfo].m_bActive = TRUE;
				return ;
			}
		}


	
	// 그 밖에 상황 조건이 안 겹치는 것들 ...
	default :
		{
			if ( m_bVisible && !m_RndHelp )
			{
				return;	
			}
			if ( m_AutoHelpInfo[dwAutoHelpInfo].CheckInfo() )
			{
				ShowAutoHelp ( dwAutoHelpInfo );
			}

			//m_bCondition[dwAutoHelpInfo] = TRUE;
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : ShowAutoHelp()
// Desc :
// ----------------------------------------------------------------------------
void CUIAutoHelp::ShowAutoHelp ( int nIndex )
{
	if ( nIndex < 0 || nIndex >= AUTOHELP_COUNT ) 
	{
		return;
	}

	// 데이터 초기화
	m_bVisible		= TRUE;
	m_nActiveIndex	= nIndex;
	m_fShowTime		= AUTOHELO_SHOW_TIME;

	int			nWidth = ( m_AutoHelpInfo[m_nActiveIndex].m_strMessage.Length() + 6 ) *
							( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() ) - 1;
	
	CDrawPort	*pdp = _pUIMgr->GetDrawPort();
	int			nCX = pdp->dp_MinI + ( pdp->dp_MaxI - pdp->dp_MinI ) / 2;

	// wooss 051019
	// for Thai
	int tv_num = 0;
	if(g_iCountry == THAILAND){
		nWidth=FindThaiLen(m_AutoHelpInfo[m_nActiveIndex].m_strMessage);
	}
	m_rcNotice.Left = nCX - nWidth / 2;
	m_rcNotice.Right = m_rcNotice.Left + nWidth;

	

	m_tmStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds ();

}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIAutoHelp::Render ()
{
	__int64	llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

	if( _pUIMgr->GetUIGameState() != UGS_GAMEON ) return;


	if ( !m_bVisible )
	{
		static int interval = INTERVAL_TIME + RND_HELP_TIME;
		static bool bFirst = false;
		static bool bTemp = false;

		TIME NowTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

/*		
		if(g_iCountry == THAILAND ){
			int tv_rnd;
			__int64 tv_time = llCurTime - m_tmCheckTime;
			INDEX tv_tmp ;
			time(&tv_tmp);
			tm* tv_tm = localtime(&tv_tmp);
			if(tv_tm->tm_hour == 3 && tv_tm->tm_min == 55 && tv_tm->tm_sec == 0) {
				tv_rnd = ( rand()% (AU_NO_TOXICOSIS_VACATION_E - AU_NO_TOXICOSIS_VACATION_S + 1) ) + AU_NO_TOXICOSIS_VACATION_S;
				SetInfo ( tv_rnd );
			} else {
				if(tv_time/(60*60*1000) != 0){
					if((tv_time/(60*60*1000))%(m_iNoticeTime) == 0 ) {
					m_iNoticeTime +=3;
					tv_rnd = ( rand()% (AU_NO_TOXICOSIS_E - AU_NO_TOXICOSIS_S + 1) ) + AU_NO_TOXICOSIS_S;
					SetInfo ( tv_rnd );
					}
				}
			}
		} 
		*/
		if(g_iCountry == BRAZIL)
		{
			int tv_rnd;
			tv_rnd = (rand()%(AU_NO_TOXICOSIS_VACATION_E - AU_NO_TOXICOSIS_S + 1) ) + AU_NO_TOXICOSIS_S;
			SetInfo ( tv_rnd );
		}

		if ( NowTime - m_tmEndTime > interval )
		{
			if ( bFirst == false )
			{
				m_tmEndTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
				bFirst = true;
				return;
			}


			m_RndHelp = TRUE;
		
			int rnd = ( rand()% (AU_RANDOM_HELP_END - AU_RANDOM_HELP_START ) ) + AU_RANDOM_HELP_START;

			SetInfo ( rnd ); // 1개 이상 일때 수정해주세요..^^
			
			m_tmEndTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
			m_RndHelp = FALSE;
		}
		return;
	}
		

	__int64	llCurDelay;

	llCurDelay = llCurTime - m_tmStartTime;

	if( llCurDelay < AUTOHELO_SHOW_TIME )
	{
		COLOR	colBackground = 0xFFFFFFFF;
		COLOR	colText = 0x5F71E2FF;

		if( llCurDelay > CHATMSG_NOTICE_FADEOUT )
		{
			FLOAT	fFadeRatio = (FLOAT)( CHATMSG_NOTICE_DELAY - llCurDelay ) / (FLOAT)CHATMSG_NOTICE_FADETIME;
			COLOR	colBlend = 0xFF * fFadeRatio;

			colBackground &= 0xFFFFFF00;
			colBackground |= colBlend;
			colText &= 0xFFFFFF00;
			colText |= colBlend;
		}
	
	
		// Set texture
		_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

		// Add render regions
		// Background
		_pUIMgr->GetDrawPort()->AddTexture( m_rcNotice.Left - 32, m_rcNotice.Top,
											m_rcNotice.Left, m_rcNotice.Bottom,
											m_rtNoticeL.U0, m_rtNoticeL.V0, m_rtNoticeL.U1, m_rtNoticeL.V1,
											colBackground );

		_pUIMgr->GetDrawPort()->AddTexture( m_rcNotice.Left, m_rcNotice.Top,
											m_rcNotice.Right, m_rcNotice.Bottom,
											m_rtNoticeC.U0, m_rtNoticeC.V0, m_rtNoticeC.U1, m_rtNoticeC.V1,
											colBackground );

		_pUIMgr->GetDrawPort()->AddTexture( m_rcNotice.Right, m_rcNotice.Top,
											m_rcNotice.Right + 32, m_rcNotice.Bottom,
											m_rtNoticeR.U0, m_rtNoticeR.V0, m_rtNoticeR.U1, m_rtNoticeR.V1,
											colBackground );

		// Render all elements
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();

		// 도움말 앞뒤로 표시하는 장식을 국가 별로 다르게 설정 합시다~
		extern INDEX	g_iCountry;
		CTString	strDecoration;

		switch( g_iCountry )
		{
		case KOREA:
			strDecoration = "♣";
			break;
		
		case TAIWAN:
		case TAIWAN2: // wooss 050929
			strDecoration = "";
			break;
		
		case CHINA: 
			strDecoration = "";
			break;
			
		case THAILAND:
			strDecoration ="";
			break;
			
		case JAPAN:
			strDecoration ="";
			break;
		case MALAYSIA:
			strDecoration ="";
			break;

		case USA:
			strDecoration ="";
			break;

		case BRAZIL:
			strDecoration ="";
			break;
		case HONGKONG:
			strDecoration ="";
			break;
		case GERMANY:
			strDecoration ="";
			break;
		case SPAIN://FRANCE_SPAIN_CLOSEBETA_NA_20081124
			strDecoration = "";
			break;
		case FRANCE:
			strDecoration = "";
			break;
		case POLAND:
			strDecoration = "";
			break;
		case TURKEY:
			strDecoration = "";
			break;
		}
		
		CTString strTemp = strDecoration + " ";
		strTemp += m_AutoHelpInfo[m_nActiveIndex].m_strMessage + " ";
		strTemp += strDecoration;
		
		// Text in notice region
		_pUIMgr->GetDrawPort()->PutTextEx( strTemp, m_rcNotice.Left, m_rcNotice.Top + 4, colText );

		// Flush all render text queue
		_pUIMgr->GetDrawPort()->EndTextEx();
	}
	else
	{
		m_bVisible			= FALSE;
		m_nActiveIndex		= -1;

		m_tmEndTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	
	}
}

// ----------------------------------------------------------------------------
// Name : SetGMNotice()
// Desc :
// ----------------------------------------------------------------------------
void CUIAutoHelp::SetGMNotice ( CTString strGMNotice, COLOR colText )
{
	m_strGMNotice = strGMNotice;
	m_colGMTextColor = colText;

	// 데이터 초기화
	m_bShowGMNotice	= TRUE;
	
	int			nWidth;
	
	if(g_iCountry == THAILAND ){
		nWidth = FindThaiLen(m_strGMNotice);

	} else {

		nWidth = m_strGMNotice.Length() *
							( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() ) - 1;
	}

	CDrawPort	*pdp = _pUIMgr->GetDrawPort();
	int			nCX = pdp->dp_MinI + ( pdp->dp_MaxI - pdp->dp_MinI ) / 2;

	m_rcGMNotice.Left = nCX - nWidth / 2;
	m_rcGMNotice.Right = m_rcGMNotice.Left + nWidth;

	m_tmGMNoticeTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds ();

}

// ----------------------------------------------------------------------------
// Name : RenderGMNotice()
// Desc : 운영자 공지
// ----------------------------------------------------------------------------
void CUIAutoHelp::RenderGMNotice ()
{
	if( m_bShowGMNotice )
	{
		__int64	llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
		__int64	llCurDelay = llCurTime - m_tmGMNoticeTime;
		if( llCurDelay < CHATMSG_NOTICE_DELAY )
		{
			COLOR	colBackground = 0xFFFFFFFF;
			COLOR	colText = m_colGMTextColor;
			if( llCurDelay > CHATMSG_NOTICE_FADEOUT )
			{
				FLOAT	fFadeRatio = (FLOAT)( CHATMSG_NOTICE_DELAY - llCurDelay ) / (FLOAT)CHATMSG_NOTICE_FADETIME;
				COLOR	colBlend = 0xFF * fFadeRatio;

				colBackground &= 0xFFFFFF00;
				colBackground |= colBlend;
				colText &= 0xFFFFFF00;
				colText |= colBlend;
			}

			// Set texture
			_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

			// Add render regions
			// Background
			_pUIMgr->GetDrawPort()->AddTexture( m_rcGMNotice.Left - 32, m_rcGMNotice.Top,
												m_rcGMNotice.Left, m_rcGMNotice.Bottom,
												m_rtNoticeL.U0, m_rtNoticeL.V0, m_rtNoticeL.U1, m_rtNoticeL.V1,
												colBackground );

			_pUIMgr->GetDrawPort()->AddTexture( m_rcGMNotice.Left, m_rcGMNotice.Top,
												m_rcGMNotice.Right, m_rcGMNotice.Bottom,
												m_rtNoticeC.U0, m_rtNoticeC.V0, m_rtNoticeC.U1, m_rtNoticeC.V1,
												colBackground );

			_pUIMgr->GetDrawPort()->AddTexture( m_rcGMNotice.Right, m_rcGMNotice.Top,
												m_rcGMNotice.Right + 32, m_rcGMNotice.Bottom,
												m_rtNoticeR.U0, m_rtNoticeR.V0, m_rtNoticeR.U1, m_rtNoticeR.V1,
												colBackground );

			// Render all elements
			_pUIMgr->GetDrawPort()->FlushRenderingQueue();

			// Text in notice region
			_pUIMgr->GetDrawPort()->PutTextEx( m_strGMNotice, m_rcGMNotice.Left, m_rcGMNotice.Top + 4, colText );

			// Flush all render text queue
			_pUIMgr->GetDrawPort()->EndTextEx();
		}
		else
			m_bShowGMNotice = FALSE;
	}
}
#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UIShop.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UIMonsterCombo.h>
#include <Engine/Interface/UIMonsterMercenary.h>
#include <Engine/Contents/Base/UIQuestNew.h>
#include <Engine/Contents/Base/UIQuestBookNew.h>
#include <Engine/Contents/function/GuildWarMixUI.h>
#include <Engine/Contents/function/ShopUI.h>
#include <Engine/info/MyInfo.h>

#define ENABLE_GUILD_CUBE

enum eSelection
{
	SHOP_BUY,		// 상점 구매
	SHOP_SELL,		// 상점 판매
	SHOP_TALK,		// 이야기 하기
	SHOP_EVENT,		// 이벤트.
	SHOP_GW_MIX,	// 공성 조합 
	SHOP_SUPERGOJE,	// 초고급 제련석 받기, 특정 NPC 하드코딩

	SHOP_NPC_HELP,		//NPC 안내 시스템

	SHOP_NPC_AFFINITY,			// [6/1/2009 rumist] npc affinity system. 친화도. 

	LEASE_WEAPON_TITAN,			// 무기 대여상
	LEASE_WEAPON_KNIGHT,
	LEASE_WEAPON_HEALER,
	LEASE_WEAPON_MAGE,
	LEASE_WEAPON_ROGUE,
	LEASE_WEAPON_SOCERER,
	LEASE_WEAPON_EXROGUE = 15,
	LEASE_WEAPON_EXMAGE,		//2013/01/08 jeil EX메이지 추가 

	//ttos: 몬스터 콤보
	COMBO_MAKE,
	COMBO_MOVE_STAGE,
	COMBO_MOVE_GO_LOBBY,
	COMBO_RECALL_LOBBY,
	COMBO_RECALL_STAGE,
	COMBO_GIVEUP,
	COMBO_COMPLETE,
	COMBO_START,

	//Cube
	CUBE_PARTY_ENTRANCE,
	CUBE_PARTY_GROUP_ENTRANCE,
	CUBE_PARTY_PERSON_ENTRANCE,
	CUBE_GUILD_GROUP_ENTRANCE,
	CUBE_GUILD_PERSON_ENTRANCE,
	CUBE_GUILD_ENTRANCE,
	CUBE_PLAYER_STATE,
	CUBE_GUILD_STATE,
	CUBE_PLAYER_REWARD,

	// ($E_WC2010) [100511: selo] 2010 월드컵 이벤트
	EVENT_WORLDCUP_2010_INFO,				// 월드컵 2010 이벤트 안내
	EVENT_WORLDCUP_2010_BALL,				// 월드컵 2010 축구공 교환
	EVENT_WORLDCUP_2010_BALL_GIFT,			// 월드컵 2010 축구공 보상
	EVENT_WORLDCUP_2010_GOLDENBALL,			// 월드컵 2010 황금축구공 교환
	EVENT_WORLDCUP_2010_GOLDENBALL_GIFT,	// 월드컵 2010 황금축구공 보상
};

CUIShop::CUIShop()
{
	m_nNpcVirIdx		= -1;
	m_nNpcIndex			= -1;
}

// ----------------------------------------------------------------------------
// Name : ~CUIShop()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIShop::~CUIShop()
{
}
// ----------------------------------------------------------------------------
// Name : OpenShop()
// Desc :
// ----------------------------------------------------------------------------
void CUIShop::OpenShop( int iMobIndex, int iMobVirIdx, BOOL bHasQuest, FLOAT fX, FLOAT fZ )
{
	OBFUSCATE();

	CUIManager* pUIManager = CUIManager::getSingleton();

	if(pUIManager->DoesMessageBoxLExist( MSGLCMD_SHOP_REQ ))
		return;

	// If inventory is already locked
	if( pUIManager->GetInventory()->IsLocked() )
	{
		pUIManager->GetInventory()->ShowLockErrorMessage();
		return;
	}

	// Set position of target npc
	m_fNpcX = fX;
	m_fNpcZ = fZ;

	m_nNpcVirIdx = iMobVirIdx;
	m_nNpcIndex = iMobIndex;

	CMobData* MD = CMobData::getData(m_nNpcIndex);
	
	switch( _pNetwork->MyCharacterInfo.sbJoinFlagMerac )
	{
		// 수성길드
	case WCJF_OWNER:
	case WCJF_DEFENSE_GUILD:
	case WCJF_DEFENSE_CHAR:
		{
			if( m_nNpcIndex == 234 || m_nNpcIndex == 235 ) // 공성측 상인이면 종료
				return;
		}
		break;
		
		// 공성측 길드
	case WCJF_ATTACK_GUILD:
	case WCJF_ATTACK_CHAR:
		{
			if( m_nNpcIndex == 232 || m_nNpcIndex == 233 ) // 수성측 상인이면 종료 
				return;
		}
		break;
	}

	pUIManager->CloseMessageBox( MSGCMD_DROPITEM );

	// Create refine message box
	pUIManager->CreateMessageBoxL( _S( 263, "상점" ) , UI_SHOP, MSGLCMD_SHOP_REQ );

	CTString	strNpcName = CMobData::getData(m_nNpcIndex)->GetName();
	pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, strNpcName, -1, 0xE18600FF );

	if( m_nNpcIndex == 272 ) // 무기 대여상
	{
		CTString tv_str;
		tv_str = _S(3054,"험난하고 위험한 아이리스를 여행하려면 이정도 무기 하나쯤은 가지고 계셔야 할 겁니다.");
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, _S(3055, "무기가 필요하시다고요? 파는건 아니지만 좋은 무기를 며칠간 빌려드릴 순 있는데 어떠세요?" ) , -1, 0xA3A1A3FF );
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, tv_str,-1, 0xA3A1A3FF );
		tv_str = _S(3056,"대여해 드리는 무기는 +12으로 제련된 것으로 더 이상은 제련을 못하며 1일간 사용할 수 있습니다."); // (+6에서 +12로 변경)
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, tv_str,-1, 0xA3A1A3FF );
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, _S(3057, "무기 종류를 골라보세요." ) , -1, 0xA3A1A3FF );
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(3058,"타이탄 무기류" ), LEASE_WEAPON_TITAN );
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(3059,"나이트 무기류" ), LEASE_WEAPON_KNIGHT  );
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(3060,"힐러 무기류" ), LEASE_WEAPON_HEALER  );
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(3061,"메이지 무기류" ), LEASE_WEAPON_MAGE  );
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(3062,"로그 무기류" ), LEASE_WEAPON_ROGUE  );
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(3063,"소서러 무기류" ), LEASE_WEAPON_SOCERER  );
#ifdef CHAR_EX_ROGUE
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(5738,"EX로그 무기류" ), LEASE_WEAPON_EXROGUE  );	// [2012/08/27 : Sora] EX로그 추가
#endif
#ifdef CHAR_EX_MAGE
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(5826,"아크메이지 무기류" ), LEASE_WEAPON_EXMAGE  );	// 2013/01/08 jeil EX메이지 추가 스트링 받으면 추가 수정 필요 
#endif
	}
	//------ 몬스터 콤보 NPC
	else if(m_nNpcIndex == 488)
	{

		pUIManager->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4036, "안녕하세요 자신의 능력을 실험하기 위하여 몬스터 콤보만큼 확실한 것도 없죠"),-1,0xa3a1a3ff);
		pUIManager->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4037, "자신의 능력을 알지 못하고 자만하다간 몬스터 콤보를 클리어 하기는 힘들겠죠."),-1,0xa3a1a3ff);
		pUIManager->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4038, "자 이제 자신의 능력을 실험해 보시죠"),-1,0xa3a1a3ff);
		
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4039, "몬스터 콤보 만들기" ), COMBO_MAKE );
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4040, "상점 이용하기" ), SHOP_BUY );			
		
	}
	else if(m_nNpcIndex == 489)
	{

		pUIManager->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4036, "안녕하세요 자신의 능력을 실험하기 위하여 몬스터 콤보만큼 확실한 것도 없죠"),-1,0xa3a1a3ff);
		pUIManager->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4037, "자신의 능력을 알지 못하고 자만하다간 몬스터 콤보를 클리어 하기는 힘들겠죠."),-1,0xa3a1a3ff);
		pUIManager->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4038, "자 이제 자신의 능력을 실험해 보시죠"),-1,0xa3a1a3ff);
		
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4041, "몬스터 콤보 스테이지 이동" ), COMBO_MOVE_STAGE );
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4042, "파티원 소환하기" ), COMBO_RECALL_LOBBY );
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4043, "몬스터 콤보 나가기" ), COMBO_GIVEUP );
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4040, "상점 이용하기" ), SHOP_BUY );
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S( 1220, "취소한다." ) );
		return;
		
	}
	else if(m_nNpcIndex == 490)
	{

		pUIManager->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4036, "안녕하세요 자신의 능력을 실험하기 위하여 몬스터 콤보만큼 확실한 것도 없죠"),-1,0xa3a1a3ff);
		pUIManager->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4037, "자신의 능력을 알지 못하고 자만하다간 몬스터 콤보를 클리어 하기는 힘들겠죠."),-1,0xa3a1a3ff);
		pUIManager->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4038, "자 이제 자신의 능력을 실험해 보시죠"),-1,0xa3a1a3ff);
		
		if(pUIManager->GetCombo()->GetComboClear() == 1)
		{	
			pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4044, "다음 스테이지로 이동" ), COMBO_MOVE_STAGE );
		}

		if(pUIManager->GetCombo()->GetComboClear() == 2)
		{
			pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4045, "콤보 완료 필드로 돌아가기" ), COMBO_COMPLETE );
		}else{
			pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4046, "몬스터 콤보 대기방 이동" ), COMBO_MOVE_GO_LOBBY );
		}
		
		return;
				
	}
	else if(m_nNpcIndex >= 521 && m_nNpcIndex <= 525)			//Cube NPCs
	{
		if( pUIManager->DoesMessageBoxLExist(MSGLCMD_CUBE_PARTY_ENTRANCE) )
			pUIManager->CloseMessageBoxL(MSGLCMD_CUBE_PARTY_ENTRANCE);

		if( pUIManager->DoesMessageBoxLExist(MSGLCMD_CUBE_PARTY_ENTRANCE) )
			pUIManager->CloseMessageBoxL(MSGLCMD_CUBE_GUILD_ENTRANCE);

		if( pUIManager->DoesMessageBoxExist(MSGCMD_CUBE_STATE) )
			pUIManager->CloseMessageBox(MSGCMD_CUBE_STATE);
			
		pUIManager->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4321, "이 큐브에 들어가기 위해서는 입장권이 필요합니다."),-1,0xa3a1a3ff);
		pUIManager->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4322, "꼭 파티를 해서 들어가야 합니다. 파티에 속해 있지 않으면 들어갈 수 없습니다."),-1,0xa3a1a3ff);
		pUIManager->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4323, "단체 입장을 하게 되면 파티원이 강제로 이동하게 됩니다."),-1,0xa3a1a3ff);
		pUIManager->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4324, "그럼 행운을 빕니다."),-1,0xa3a1a3ff);
		
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4325, "입장큐브 구입" ), SHOP_BUY );
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4326, "파티큐브입장 신청" ), CUBE_PARTY_ENTRANCE);
#ifdef ENABLE_GUILD_CUBE
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4394, "길드큐브입장 신청" ), CUBE_GUILD_ENTRANCE);
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(5441, "개인 큐브 보상 신청" ), CUBE_PLAYER_REWARD);
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4390, "개인 큐브 포인트 현황" ), CUBE_PLAYER_STATE);
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4388, "길드 큐브 포인트 현황" ), CUBE_GUILD_STATE);
#endif
	}
	else
	{
		if( m_nNpcIndex == 233 || m_nNpcIndex == 235 ) // 공성 조합상인
		{
			pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, _S( 1975, "전장에서 장사를 한다는 건 목숨을 내걸고 하는 일이지." ) , -1, 0xA3A1A3FF );	
			pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, _S( 1976, "그만큼 힘든 일 이기도 하고 그렇다고 바가지를 씌우거나 하지는 않네..." ) , -1, 0xA3A1A3FF );	
			pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, CTString( " " ) , -1, 0xA3A1A3FF );	
			pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, _S( 1977, "진귀한 물건들이 많이 있으니 골라보게.." ) , -1, 0xA3A1A3FF );	
			pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, CTString( " " ) , -1, 0xA3A1A3FF );	
			pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, _S( 1978, "난 조합 아이템만 취급하니 염두해 두게. 물론 아이템 조합은 서비스로 해주고 있지. " ) , -1, 0xA3A1A3FF );	
		}
		else
		{
			pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, _S( 621, "어서 오시게." ) , -1, 0xA3A1A3FF );	
			pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, _S( 622, "이 근방에서는 그래도 여기가 제일 가격도 싸고 종류도 다양 할 꺼야." ) , -1, 0xA3A1A3FF );	
			pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, _S( 623, "머, 영 고를 물건이 없다면 할 수 없지만 다른 곳을 가도 마찬가지야." ) , -1, 0xA3A1A3FF );	
		}


		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S( 624, "구매한다." ), SHOP_BUY  );		
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S( 625, "판매한다." ), SHOP_SELL  );		
	}
	
	if( m_nNpcIndex == 233 || m_nNpcIndex == 235 ) // 공성 조합상인
	{	
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S( 1979, "아이템을 조합한다." ), SHOP_GW_MIX );	
	}

	if(bHasQuest)
	{
		// 2009. 05. 27 김정래
		// 이야기한다 변경 처리
		CUIQuestBook::AddQuestListToMessageBoxL(MSGLCMD_SHOP_REQ);				
	}

	// FIXME : 퀘스트가 없을 경우에 문제가 됨.
	// FIXME : 고로, 이벤트 NPC는 되도록 퀘스트를 갖고 있는 형태로???
	if(MD->IsEvent())
	{
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S( 100, "이벤트" ), SHOP_EVENT  );		
	}

	// ($E_WC2010) [100511: selo] 2010 남아공 월드컵 Event1.
	// 해당 이벤트가 진행 중이고 방어구 상인일 때 메뉴를 보인다
	if(IS_EVENT_ON(TEVENT_WORLDCUP_2010) &&
		(m_nNpcIndex == 90 || m_nNpcIndex == 129 || m_nNpcIndex == 169 || m_nNpcIndex == 279 || m_nNpcIndex == 508 || m_nNpcIndex == 1029) )
	{
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4889, "월드 축제 이벤트 안내"), EVENT_WORLDCUP_2010_INFO );
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4890, "황금 축구공으로 교환"), EVENT_WORLDCUP_2010_BALL );
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4881, "월드 축제 상자로 교환"), EVENT_WORLDCUP_2010_GOLDENBALL );
	}

	if(m_nNpcIndex == 226)
	{
		pUIManager->CloseMessageBox( MSGCMD_SUPERGOJE_REQUEST );
		pUIManager->CloseMessageBox( MSGCMD_SUPERGOJE_NOTIFY );
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S( 1782, "초 고급 제련석 받기" ), SHOP_SUPERGOJE  );
	}

	pUIManager->AddMessageBoxLString(MSGLCMD_SHOP_REQ, FALSE, _S( 1748, "NPC 안내" ), SHOP_NPC_HELP); //ttos : 안내시스템 추가시
	pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S( 1220, "취소한다." ) );	
}

// ----------------------------------------------------------------------------
// Name : MsgBoxLCommand()
// Desc :
//		cpp2angel edit (04.12.18)
// ----------------------------------------------------------------------------
void CUIShop::MsgBoxLCommand( int nCommandCode, int nResult )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	switch( nCommandCode )
	{
	case MSGLCMD_SHOP_REQ:
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
			case SHOP_BUY : //  Buy
				{
					pUIManager->GetShopUI()->openUI(m_nNpcIndex, m_nNpcVirIdx, m_fNpcX, m_fNpcZ, eSHOP_BUY);					
				}
				break;

			case SHOP_SELL : // Sell
				{	
					pUIManager->GetShopUI()->openUI(m_nNpcIndex, m_nNpcVirIdx, m_fNpcX, m_fNpcZ, eSHOP_SELL);	
				}
				break;
			case SHOP_GW_MIX: // 공성 조합
				{
					pUIManager->GetGWMix()->OpenGWMix();
				}
				break;
			case SHOP_TALK :// Quest ...
				{
					// Character state flags
					pUIManager->SetCSFlagOff( CSF_SHOP );
					CUIQuestBook::TalkWithNPC();
				}
				break;
				// [090527: selo] 확장팩 퀘스트 수정
			case ciQuestClassifier:
				{
					// Character state flags
					pUIManager->SetCSFlagOff( CSF_SHOP );

					// 선택한 퀘스트에 대해 수락 또는 보상 창을 연다.
					CUIQuestBook::SelectQuestFromMessageBox( iQuestIndex );					
				}
				break;
			case SHOP_SUPERGOJE:
				{
					// Character state flags
					pUIManager->SetCSFlagOff( CSF_SHOP );

					CUIMsgBox_Info	MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo(  CTString(""), UMBS_YESNO, UI_SHOP, MSGCMD_SUPERGOJE_REQUEST);
					MsgBoxInfo.AddString( _S( 1739, "초고급 제련석을 지급 받겠습니까?" ) );
					pUIManager->CreateMessageBox( MsgBoxInfo );
				} break;

			case LEASE_WEAPON_TITAN:
			case LEASE_WEAPON_KNIGHT:
			case LEASE_WEAPON_HEALER:
			case LEASE_WEAPON_MAGE:
			case LEASE_WEAPON_ROGUE:
			case LEASE_WEAPON_SOCERER:
			case LEASE_WEAPON_EXROGUE:	
			case LEASE_WEAPON_EXMAGE:	//2013/01/21 jeil EX메이지 추가 
				{
 					int tv_job = nResult - LEASE_WEAPON_TITAN;
					pUIManager->GetShopUI()->SetLeaseJob(tv_job);
					pUIManager->GetShopUI()->openUI(m_nNpcIndex, m_nNpcVirIdx, m_fNpcX, m_fNpcZ, eSHOP_LEASE);	
				}
				break;
			case SHOP_NPC_HELP:
				{
					pUIManager->RearrangeOrder( UI_NPCHELP, TRUE );
				}
				break;
			case COMBO_MAKE:
				{
					//콤보 편집으로 이동시 CSF_SHOP 플래그를 제거한다
					pUIManager->SetCSFlagOff( CSF_SHOP );					
					pUIManager->GetCombo()->OpenMonsterCombo(TRUE, m_fNpcX, m_fNpcZ, m_nNpcVirIdx);
				}
				break;
			
			case COMBO_MOVE_STAGE:
				{
					pUIManager->SetCSFlagOff( CSF_SHOP );
					pUIManager->GetCombo()->SendComboMessage( MSG_EX_MONSTERCOMBO_GOTO_STAGE);
				}
				break;
			case COMBO_MOVE_GO_LOBBY:
				{
					pUIManager->SetCSFlagOff( CSF_SHOP );
					pUIManager->GetCombo()->SendComboMessage(MSG_EX_MONSTERCOMBO_GOTO_WAITROOM);

				}
				break;
			case COMBO_RECALL_LOBBY:
				{
					pUIManager->SetCSFlagOff( CSF_SHOP );
					pUIManager->GetCombo()->SendComboMessage(MSG_EX_MONSTERCOMBO_RECALL_TO_COMBO);
				}
				break;
			case COMBO_RECALL_STAGE:
				{
					pUIManager->SetCSFlagOff( CSF_SHOP );
					pUIManager->GetCombo()->SendComboMessage(MSG_EX_MONSTERCOMBO_RECALL_TO_STAGE);
				}
				break;
			case COMBO_GIVEUP:
				{
					// Character state flags
					pUIManager->SetCSFlagOff( CSF_SHOP );
					CUIMsgBox_Info	MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo(  _S(4047,"몬스터 콤보"), UMBS_YESNO, UI_SHOP, MSGCMD_COMBO_GIVEUP);
					MsgBoxInfo.AddString( _S(4048, "드라탄으로 이동되며 다시 입장하기 위해서는 입장료를 지불해야 합니다. \n 그래도 몬스터 콤보를 포기하시겠습니까?" ) );
					pUIManager->CreateMessageBox( MsgBoxInfo );
				}
				break;
			case COMBO_START:
				{
					pUIManager->SetCSFlagOff( CSF_SHOP );
					pUIManager->GetCombo()->SendComboMessage(MSG_EX_MONSTERCOMBO_START);
				}
				break;
			case COMBO_COMPLETE:
				{
					pUIManager->SetCSFlagOff( CSF_SHOP );
					pUIManager->GetCombo()->SendComboMessage(MSG_EX_MONSTERCOMBO_GIVEUP, FALSE, m_nNpcVirIdx);
				}
				break;
			///////////////////////////////////////////////////////////////////////////////////
			// Cube
			case CUBE_PARTY_ENTRANCE:
				{
					pUIManager->CreateMessageBoxL( _S(4327, "큐브입장선택" ) , UI_SHOP, MSGLCMD_CUBE_PARTY_ENTRANCE);

					pUIManager->AddMessageBoxLString(MSGLCMD_CUBE_PARTY_ENTRANCE,TRUE, _S(4328, "큐브입장방법을 선택하여 주십시오."),-1,0xa3a1a3ff);
					pUIManager->AddMessageBoxLString(MSGLCMD_CUBE_PARTY_ENTRANCE,TRUE, CTString(" "),-1,0xa3a1a3ff);
					pUIManager->AddMessageBoxLString(MSGLCMD_CUBE_PARTY_ENTRANCE,TRUE, _S(4329, "주의사항"),-1,0xa3a1a3ff);
					pUIManager->AddMessageBoxLString(MSGLCMD_CUBE_PARTY_ENTRANCE,TRUE, _S(4330, "단체입장시 파티원 모두가 강제이동됩니다."),-1,0xa3a1a3ff);
					pUIManager->AddMessageBoxLString(MSGLCMD_CUBE_PARTY_ENTRANCE,TRUE, _S(4331, "개인입장시 조건이 맞아야 입장이 가능합니다."),-1,0xa3a1a3ff);
					pUIManager->AddMessageBoxLString(MSGLCMD_CUBE_PARTY_ENTRANCE,TRUE, _S(4332, "단체입장을 추천드립니다."),-1,0xa3a1a3ff);
					
					pUIManager->AddMessageBoxLString( MSGLCMD_CUBE_PARTY_ENTRANCE, FALSE, _S(4333, "단체입장" ), CUBE_PARTY_GROUP_ENTRANCE);
					pUIManager->AddMessageBoxLString( MSGLCMD_CUBE_PARTY_ENTRANCE, FALSE, _S(4334, "개인입장" ), CUBE_PARTY_PERSON_ENTRANCE);
					pUIManager->AddMessageBoxLString( MSGLCMD_CUBE_PARTY_ENTRANCE, FALSE, _S(139, "취소" ) );
				}
				break;
			case CUBE_GUILD_ENTRANCE:
				{
					pUIManager->CreateMessageBoxL( _S(4327, "큐브입장선택" ) , UI_SHOP, MSGLCMD_CUBE_GUILD_ENTRANCE);

					pUIManager->AddMessageBoxLString(MSGLCMD_CUBE_GUILD_ENTRANCE,TRUE, _S(4328, "큐브입장방법을 선택하여 주십시오."),-1,0xa3a1a3ff);
					pUIManager->AddMessageBoxLString(MSGLCMD_CUBE_GUILD_ENTRANCE,TRUE, CTString(" "),-1,0xa3a1a3ff);
					pUIManager->AddMessageBoxLString(MSGLCMD_CUBE_GUILD_ENTRANCE,TRUE, _S(4329, "주의사항"),-1,0xa3a1a3ff);
					pUIManager->AddMessageBoxLString(MSGLCMD_CUBE_GUILD_ENTRANCE,TRUE, _S(4335, "길드입장시 길드원 모두가 강제이동됩니다."),-1,0xa3a1a3ff);
					pUIManager->AddMessageBoxLString(MSGLCMD_CUBE_GUILD_ENTRANCE,TRUE, _S(4331, "개인입장시 조건이 맞아야 입장이 가능합니다."),-1,0xa3a1a3ff);
					pUIManager->AddMessageBoxLString(MSGLCMD_CUBE_GUILD_ENTRANCE,TRUE, _S(4332, "단체입장을 추천드립니다."),-1,0xa3a1a3ff);
					
					pUIManager->AddMessageBoxLString( MSGLCMD_CUBE_GUILD_ENTRANCE, FALSE, _S(4336, "길드입장" ), CUBE_GUILD_GROUP_ENTRANCE);
					pUIManager->AddMessageBoxLString( MSGLCMD_CUBE_GUILD_ENTRANCE, FALSE, _S(4334, "개인입장" ), CUBE_GUILD_PERSON_ENTRANCE);
					pUIManager->AddMessageBoxLString( MSGLCMD_CUBE_GUILD_ENTRANCE, FALSE, _S(139, "취소" ) );
				}
				break;
			case CUBE_PLAYER_STATE:
			case CUBE_GUILD_STATE:
				{
					CNetworkMessage nmMessage(MSG_EXTEND);
			
					nmMessage << (LONG)MSG_EX_EXTREME_CUBE;

					if (nResult == CUBE_GUILD_STATE)
						nmMessage << (UBYTE)MSG_EX_EXTREME_CUBE_STATE_REQ; // 길드 큐브 현황
					else
						nmMessage << (UBYTE)MSG_EX_EXTREME_CUBE_STATE_PERSONAL_REQ; // 개인 큐브 현황

					_pNetwork->SendToServerNew(nmMessage);
				}
				break;
			case CUBE_PLAYER_REWARD:
				{
					CNetworkMessage nmMessage(MSG_EXTEND);
					
					nmMessage << (LONG)MSG_EX_EXTREME_CUBE;
					nmMessage << (UBYTE)MSG_EX_EXTREME_CUBE_REWARD_PERSONAL_REQ;

					_pNetwork->SendToServerNew(nmMessage);
				}break;
			// Cube
			///////////////////////////////////////////////////////////////////////////////////	
				
			//////////////////////////////////////////////////////////////////////////
			// ($E_WC2010) [100511: selo] 2010 월드컵 이벤트
			case EVENT_WORLDCUP_2010_INFO:
				{
					if( pUIManager->DoesMessageBoxLExist(MSGLCMD_EVENT_WORLDCUP_EVENT1) )
						pUIManager->CloseMessageBoxL(MSGLCMD_EVENT_WORLDCUP_EVENT1);
					
					pUIManager->SetCSFlagOff( CSF_SHOP );
					pUIManager->CreateMessageBoxL( _S(4889, "월드 축제 이벤트 안내" ) , UI_SHOP, MSGLCMD_EVENT_WORLDCUP_EVENT1);

					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_WORLDCUP_EVENT1, TRUE, _S(4892, "고민하지 마시오!! 내게 축구공만 가져온다면 어디에서도 볼 수 없는 황금 축구공을 드리리다."),-1,0xa3a1a3ff);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_WORLDCUP_EVENT1, TRUE, _S(4893, "축구공은 마을 밖에 있는 '트라이앵글 볼'들이 가지고 있소."),-1,0xa3a1a3ff);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_WORLDCUP_EVENT1, TRUE, _S(4894, "요리조리 잘 피해 다니긴 해도 당신 정도면 아주 가볍게 처리할 수 있을게요."),-1,0xa3a1a3ff);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_WORLDCUP_EVENT1, FALSE, _S( 191, "확인" ) );
				}
				break;
			case EVENT_WORLDCUP_2010_BALL:
				{
					if( pUIManager->DoesMessageBoxLExist(MSGLCMD_EVENT_WORLDCUP_EVENT1) )
						pUIManager->CloseMessageBoxL(MSGLCMD_EVENT_WORLDCUP_EVENT1);
					
					pUIManager->SetCSFlagOff( CSF_SHOP );
					pUIManager->CreateMessageBoxL( _S(4890, "황금 축구공으로 교환"), UI_SHOP, MSGLCMD_EVENT_WORLDCUP_EVENT1);
					
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_WORLDCUP_EVENT1, TRUE, _S(4895, "축구공 10개만 가지고 오면 황금 축구공 1개로 바꿔주겠소."),-1,0xa3a1a3ff);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_WORLDCUP_EVENT1, FALSE, _S( 149, "교환" ), EVENT_WORLDCUP_2010_BALL_GIFT );

				}
				break;
			case EVENT_WORLDCUP_2010_GOLDENBALL:
				{
					if( pUIManager->DoesMessageBoxLExist(MSGLCMD_EVENT_WORLDCUP_EVENT1) )
						pUIManager->CloseMessageBoxL(MSGLCMD_EVENT_WORLDCUP_EVENT1);
					
					pUIManager->SetCSFlagOff( CSF_SHOP );
					pUIManager->CreateMessageBoxL( _S(4881, "월드 축제 상자로 교환"), UI_SHOP, MSGLCMD_EVENT_WORLDCUP_EVENT1);
					
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_WORLDCUP_EVENT1, TRUE, _S(4896, "황금 축구공 50개 정도만 가지고 온다면, 당신이 원하는 월드 축제 상자를 드리겠소."),-1,0xa3a1a3ff);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_WORLDCUP_EVENT1, TRUE, _S(4897, "쉽지 않을 것이니 분발하시오. 하하... 고생하시오."),-1,0xa3a1a3ff);

					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_WORLDCUP_EVENT1, FALSE, _S( 149, "교환" ), EVENT_WORLDCUP_2010_GOLDENBALL_GIFT );
				}
				break;
			// 2010 월드컵 이벤트
			//////////////////////////////////////////////////////////////////////////
				
			default:
				{
					// Character state flags
					pUIManager->SetCSFlagOff( CSF_SHOP );
				}
				break;
		
			}
		}  // case MSGLCMD_SHOP_REQ
		break;

	///////////////////////////////////////////////////////////////////////////////////	
	// Cube
	case MSGLCMD_CUBE_PARTY_ENTRANCE:
		{
			switch ( nResult )
			{
			case CUBE_PARTY_GROUP_ENTRANCE:
				{
					CUIMsgBox_Info	MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OKCANCEL, UI_SHOP, MSGCMD_CUBE_PARTY_GROUP_ENTRANCE);
					MsgBoxInfo.AddString( _S(4339, "단체입장을 통해서 큐브에 입장하시겠습니까? 파티큐브가 없으면 입장할 수 없습니다.") );
					pUIManager->CreateMessageBox( MsgBoxInfo );
				}
				break;
			case CUBE_PARTY_PERSON_ENTRANCE:
				{
					CUIMsgBox_Info	MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OKCANCEL, UI_SHOP, MSGCMD_CUBE_PARTY_PERSON_ENTRANCE);
					MsgBoxInfo.AddString( _S(4338, "개인입장을 통해서 큐브에 입장하시겠습니까?") );
					pUIManager->CreateMessageBox( MsgBoxInfo );
				}
				break;
			}
		}
		break;
	case MSGLCMD_CUBE_GUILD_ENTRANCE:
		{
		switch( nResult )
			{
			case CUBE_GUILD_GROUP_ENTRANCE:
				{
					CUIMsgBox_Info	MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OKCANCEL, UI_SHOP, MSGCMD_CUBE_GUILD_GROUP_ENTRANCE);
					MsgBoxInfo.AddString( _S(4337, "길드입장을 통해서 큐브에 입장하시겠습니까? 길드큐브가 없으면 입장할 수 없습니다.") );
					pUIManager->CreateMessageBox( MsgBoxInfo );
				}
				break;
			case CUBE_GUILD_PERSON_ENTRANCE:
				{
					CUIMsgBox_Info	MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OKCANCEL, UI_SHOP, MSGCMD_CUBE_GUILD_PERSON_ENTRANCE);
					MsgBoxInfo.AddString( _S(4338, "개인입장을 통해서 큐브에 입장하시겠습니까?") );
					pUIManager->CreateMessageBox( MsgBoxInfo );
				}
				break;
			}
		}
		break;
	// Cube
	///////////////////////////////////////////////////////////////////////////////////	

	//////////////////////////////////////////////////////////////////////////
	// ($E_WC2010) [100511: selo] 2010 월드컵 이벤트
	case MSGLCMD_EVENT_WORLDCUP_EVENT1:
		{
			switch( nResult )
			{
			case EVENT_WORLDCUP_2010_BALL_GIFT:
				{
					_pNetwork->SendWorldCup2010_Event(MSG_EVENT_WORLDCUP2010_TRADE_REQ, 1);
				}
				break;
			case EVENT_WORLDCUP_2010_GOLDENBALL_GIFT:
				{
					_pNetwork->SendWorldCup2010_Event(MSG_EVENT_WORLDCUP2010_TRADE_REQ, 2);
				}
				break;
			}
		}
		break;
	// 2010 월드컵 이벤트
	//////////////////////////////////////////////////////////////////////////
	}// switch( nCommandCode )
}


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : nCommandCode - 
//			bOK - 
//			&strInput - 
//-----------------------------------------------------------------------------
void CUIShop::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	if( !bOK )
		return;

	CUIManager* pUIMgr = UIMGR();

	switch( nCommandCode )
	{
	case MSGCMD_SHOP_ADD_PLUSITEM:
		{
			pUIMgr->GetShopUI()->ShopToTrade( 1 );
		}
		break;

	case MSGCMD_SUPERGOJE_REQUEST:
		{
			_pNetwork->SendSuperStoneReq();			
		} break;

	case MSGCMD_SHOP_LEASE_ITEM :
		{
			// Send Network Message
			pUIMgr->GetShopUI()->BuyItems();
			pUIMgr->GetShopUI()->closeUI();
		}
		break;
	case MSGCMD_COMBO_GIVEUP:
		{
			CUIManager::getSingleton()->GetCombo()->SendComboMessage(MSG_EX_MONSTERCOMBO_GIVEUP, FALSE, m_nNpcVirIdx);
		}
		break;

	///////////////////////////////////////////////////////////////////////////////////	
	// Cube
	case MSGCMD_CUBE_PARTY_GROUP_ENTRANCE:
		{
			UIMGR()->SetCSFlagOn(CSF_TELEPORT);

			CNetworkMessage nmMessage(MSG_EXTEND);

			nmMessage << (LONG)MSG_EX_EXTREME_CUBE;
			nmMessage << (UBYTE)MSG_EX_EXTREME_CUBE_PARTY;

			_pNetwork->SendToServerNew(nmMessage);
		}
		break;
	case MSGCMD_CUBE_PARTY_PERSON_ENTRANCE:
		{
			UIMGR()->SetCSFlagOn(CSF_TELEPORT);

			CNetworkMessage nmMessage(MSG_EXTEND);

			nmMessage << (LONG)MSG_EX_EXTREME_CUBE;
			nmMessage << (UBYTE)MSG_EX_EXTREME_CUBE_PARTY_PERSON;

			_pNetwork->SendToServerNew(nmMessage);
		}
		break;
	case MSGCMD_CUBE_GUILD_GROUP_ENTRANCE:
		{
			UIMGR()->SetCSFlagOn(CSF_TELEPORT);

			CNetworkMessage nmMessage(MSG_EXTEND);

			nmMessage << (LONG)MSG_EX_EXTREME_CUBE;
			nmMessage << (UBYTE)MSG_EX_EXTREME_CUBE_GUILD;

			_pNetwork->SendToServerNew(nmMessage);
		}
		break;
	case MSGCMD_CUBE_GUILD_PERSON_ENTRANCE:
		{
			UIMGR()->SetCSFlagOn(CSF_TELEPORT);

			CNetworkMessage nmMessage(MSG_EXTEND);
			
			nmMessage << (LONG)MSG_EX_EXTREME_CUBE;
			nmMessage << (UBYTE)MSG_EX_EXTREME_CUBE_GUILD_PERSON;

			_pNetwork->SendToServerNew(nmMessage);
		}
		break;
	case MSGCMD_CUBE_STATE:
		{
			CUIManager::getSingleton()->GetMessageBox(MSGCMD_CUBE_STATE)->InitMessageBox();
		}
		break;
	}
}

void CUIShop::CreateCubeStateMsgBox(CNetworkMessage *istr, BOOL bGuild)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist(MSGCMD_CUBE_STATE) )
		pUIManager->CloseMessageBox(MSGCMD_CUBE_STATE);

	LONG lMyPoint = 0, lInfoCubePoint = 0; // 자신이 소속된 길드 및 개인 포인트 , 각 리스트의 큐브 포인트
	BYTE InfoCount = 0, InfoRank = 0; // 리스트 갯수, 랭크 정보

	CTString strGuildName, strMyName; // 길드 이름, 길드 마스터 및 캐릭터 이름

	(*istr) >> lMyPoint;
	(*istr) >> InfoCount;

	CUIMsgBox_Info MsgBoxInfo;
	CTString strMsg, strMsgEx;
	CTString strPoint = pUIManager->IntegerToCommaString(lMyPoint);

	if (bGuild)
	{
		strMsg = _S(4388, "길드 큐브 포인트 현황");
		strMsgEx = _S(4389, "자신이 소속된 길드 큐브 포인트 : %s");
		strGuildName = _S(3451,"길드명");
		strMyName = _S(4344, "길드 마스터");
	}
	else
	{
		strMsg = _S(4390, "개인 큐브 포인트 현황");
		strMsgEx = _S(4391, "자신의 개인 큐브 포인트 : %s");
		strGuildName = _S(4392, "소속 길드");
		strMyName = _S(4393, "캐릭터 이름");
	}

	MsgBoxInfo.SetMsgBoxInfo(strMsg, UMBS_OK | UMBS_LISTBOX, UI_SHOP, MSGCMD_CUBE_STATE);

	strMsg.PrintF(strMsgEx, strPoint);
	MsgBoxInfo.AddStringEx(strMsg, 0, 1);
	MsgBoxInfo.AddStringEx(_S(4343, "큐브포인트 현황"), 2, 1);
	MsgBoxInfo.AddStringEx(_S(1714, "순위"), 4, 1);
	MsgBoxInfo.AddStringEx( strGuildName, 4, 9 );
	MsgBoxInfo.AddStringEx( strMyName, 4, 28 );
	MsgBoxInfo.AddStringEx( _S(4345, "큐브포인트"), 4, 47 );
	MsgBoxInfo.SetListBoxPlacement(20, 113, 350, 84, 5, 4); // 173 - 118 = 55
	MsgBoxInfo.m_nColorBoxCount = 9;
	pUIManager->CreateMessageBox( MsgBoxInfo );
	pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->SetWidth(400);
	pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->GetListBox().ResetAllStrings();
	pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->GetListBox().SetColumnPosX(0, 6);
	pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->GetListBox().SetColumnPosX(1, 42);
	pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->GetListBox().SetColumnPosX(2, 156);
	pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->GetListBox().SetColumnPosX(3, 335, TEXT_RIGHT);

	int i;

	for (i=0; i<InfoCount; ++i)
	{
		(*istr) >> InfoRank >> strGuildName >> strMyName >> lInfoCubePoint;

		strMsg = pUIManager->IntegerToCommaString(InfoRank);
		pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->GetListBox().AddString(0, strMsg, 0xF2F2F2FF, FALSE);
		pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->GetListBox().AddString(1, strGuildName, 0xF2F2F2FF, FALSE);
		pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->GetListBox().AddString(2, strMyName, 0xF2F2F2FF, FALSE);
		strMsg = pUIManager->IntegerToCommaString(lInfoCubePoint);
		pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->GetListBox().AddString(3, strMsg, 0xF2F2F2FF, FALSE );
	}

	WRect rcHorizonalLine, rcVerticalLine0, rcVerticalLine1;
	// 전체적으로 높이 -48 , 추가 -12
	rcHorizonalLine.SetRect( 15, 58, 386, 58);
	rcVerticalLine0.SetRect( 15, 58, 15, 204);
	rcVerticalLine1.SetRect( 51, 82, 51, 204);

	pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->SetColorBox(0, 0, rcHorizonalLine);
	rcHorizonalLine.Offset(0, 24);
	pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->SetColorBox(1, 0, rcHorizonalLine);
	rcHorizonalLine.Offset(0, 25);
	pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->SetColorBox(2, 0, rcHorizonalLine);
	rcHorizonalLine.Offset(0, 95);
	pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->SetColorBox(3, 0, rcHorizonalLine);

	pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->SetColorBox(4, 0, rcVerticalLine0);
	rcVerticalLine0.Offset(370, 0);
	pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->SetColorBox(5, 0, rcVerticalLine0);

	pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->SetColorBox(6, 0, rcVerticalLine1);
	rcVerticalLine1.Offset(114, 0);
	pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->SetColorBox(7, 0, rcVerticalLine1);
	rcVerticalLine1.Offset(114, 0);
	pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->SetColorBox(8, 0, rcVerticalLine1);
}

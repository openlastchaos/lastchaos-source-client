// ----------------------------------------------------------------------------
//  File : UIManager.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UIMANAGER_H_
#define	UIMANAGER_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


#include <Engine/Graphics/DrawPort.h>
#include <GameMP/Game.h>
#include <Engine/Interface/UIPlayerInfo.h>
#include <Engine/Interface/UITargetInfo.h>
#include <Engine/Interface/UIRadar.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UICharacterInfo.h>
#include <Engine/Interface/UIExchange.h>
#include <Engine/Interface/UIPortal.h>
#include <Engine/Interface/UIPersonalShop.h>
#include <Engine/Interface/UIGuild.h>
#include <Engine/Interface/UIGuildBattle.h> // Date : 2005-03-17,   By Lee Ki-hwan
#include <Engine/Interface/UIWareHouse.h>
#include <Engine/Interface/UISystemMenu.h>
#include <Engine/Interface/UIParty.h>
#include <Engine/Interface/UIMap.h>
#include <Engine/Interface/UIRefine.h>
#include <Engine/Interface/UIProcess.h>		
#include <Engine/Interface/UIProduct.h>		// 이기환 ( 12.7)
#include <Engine/Interface/UIMix.h>			// 이기환 ( 12.7)
#include <Engine/Interface/UICompound.h>	// Date : 2005-01-12,   By Lee Ki-hwan
#include <Engine/Interface/UIBilling.h>	// Date : 2005-04-30(오후 3:58:10), By Lee Ki-hwan
#include <Engine/Interface/UITalk.h> // Date : 2005-05-24(오후 1:30:50), By Lee Ki-hwana

//#include <Engine/Interface/UISkillLearn.h>
#include <Engine/Interface/UIRemission.h>
#include <Engine/Interface/UIMessenger.h>
#include <Engine/Interface/UIWebBoard.h>
#include <Engine/Interface/UIQuest.h>
#include <Engine/Interface/UIChatting.h>
#include <Engine/Interface/UISecurity.h>

#include <Engine/Interface/UILogin.h>
#include <Engine/Interface/UIOption.h>
#include <Engine/Interface/UISelectServer.h>
#include <Engine/Interface/UIChangeWeapon.h>
#include <Engine/Interface/UIInitJob.h>
#include <Engine/Interface/UIHelper.h>
#include <Engine/Interface/UIQuiz.h>
#include <Engine/Interface/UIHelp.h>		// 도움말 추가
#include <Engine/Interface/UIHelpOld.h>		// 예전 도움말
#include <Engine/Interface/UIGamble.h>
#include <Engine/Interface/UIRanking.h>
#include <Engine/Interface/UISiegeWarfare.h>
#include <Engine/Interface/UISiegeWarfareNew.h>	// WSS_DRATAN_SIEGEWAREFARE 070727
#include <Engine/Interface/UIProcessNPC.h>
#include <Engine/Interface/UISelectResource.h>
#include <Engine/Interface/UIBillItem.h>
#include <Engine/Interface/UIGuildWarPortal.h>
#include <Engine/Interface/UIGWMix.h>
#include <Engine/Interface/UIGuildStash.h>
#include <Engine/Interface/UIMouseCursor.h>

#include <Engine/Interface/UISelectList.h>
#include <Engine/Interface/UIPetItemMix.h>
#include <Engine/Interface/UIPetFree.h>
#include <Engine/Interface/UIOXQuizEvent.h>

// wooss 050819
// UI CASH SHOP
#include <Engine/Interface/UICashShop.h>
#include <Engine/Interface/UIPartyAuto.h>// party auto matching system
#include <Engine/Interface/UICollectBox.h>
#include <Engine/Interface/UIMixNew.h>
#include <Engine/interface/UIBingoBox.h>
#include <Engine/Interface/UINpcHelp.h>
// wooss 070330 ----------------------->><<
// kw : WSS_HELP_SYSTEM_1
#include <Engine/interface/UIHelpIcon.h>

// [KH_070419] 심플 팝업 관련 추가
#include <Engine/Interface/UISimplePop.h>

// WSS_MINIGAME ----------------------->><<
#include <Engine/interface/UIMinigame.h>

#include <Engine/interface/UIFlowerTree.h>

// WSS_GUILDMASTER ---------------------->><<
#include <Engine/Interface/UIDrawFigure.h>
// WSS_NEW_GUILD_SYSTEM 070715 ---------->><<
#include <Engine/Interface/UIGuildNotice.h>
// ttos_080116 : 몬스터 콤보
#include <Engine/Interface/UIMonsterCombo.h>
// 공격 펫 시스템
#include <Engine/Interface/UIWildPetInfo.h>

//거래대행 시스템
#include <Engine/Interface/UIAuction.h>

// [090709: selo] 펫 문양 시스템
#include <Engine/Interface/UITatoo.h>

//[ttos_2009_7_17]: CHARATER_CHAT_FILTER 캐릭터 채팅 필터
//******************************************************
#include <Engine/Interface/UIFiltering.h>

class CWorld;

class CUITeleport;
class CUITeleportPrimium;
class CUIQuestBook;
class CUISelectWord;
class CUINotice;
class CUIPetTraining;
class CUIPetTargetInfo;
class CUIWildPetTargetInfo;
class CUIPetInfo;
class CUISummon;
class CUIMessageBox;
class CUIMsgBox_Info;
class CUISingleBattle;
class CUISkillLearn;
class CUIQuickSlot;
class CUISelChar;
class CUICreateChar;
class CUIShop;
class CUIChatting;
class CUINpcHelp;

// Link library
#pragma comment( lib, "imm32.lib" )

// Game state
enum UIGameState
{
	UGS_NONE			= -1,		// None
	UGS_LOGIN			= 0,		// ID & PW page
	UGS_SELSERVER		= 1,		// Server selection page
	UGS_SELCHAR			= 2,		// Character selection page
	UGS_CREATECHAR		= 3,		// Character creation page
	UGS_GAMEON			= 4,		// Game running
	UGS_GAMELOADING		= 5,		// Game loading
	UGS_UI_TEST			= 6,		// Game UI TEST quick loanding ui wooss 050822
};


// User interface types
enum UI_TYPE
{
	UI_NONE					= -1,
	UI_TYPE_START			= 0,

	UI_ALWAYS_SHOW_START	= UI_TYPE_START,				// 항상 보이는 인터페이스 |<--
	UI_PLAYERINFO			= UI_ALWAYS_SHOW_START,
	UI_CHATTING,
	UI_PETTARGETINFO,
	UI_WILDPETTARGETINFO,
	UI_QUICKSLOT,
	UI_SUMMON_START,
	UI_SUMMON_FIRST	= UI_SUMMON_START,
	UI_SUMMON_SECOND,
	UI_SUMMON_END	= UI_SUMMON_SECOND,	
	UI_RADAR,
	UI_NOTICE,
	UI_TARGETINFO,	
	UI_HELP_ICON,	
	UI_ALWAYS_SHOW_END,										// 항상 보이는 인터페이스     -->|

	UI_SELECTIVE_SHOW_START	= UI_ALWAYS_SHOW_END,			// 선택적으로 보이는 인터페이스 |<--
	UI_INVENTORY			= UI_SELECTIVE_SHOW_START,
	UI_CHARACTERINFO,
	UI_EXCHANGE,
	UI_PORTAL,
	UI_SHOP,
	UI_PERSONALSHOP,
	UI_WAREHOUSE,
	UI_SECURITY,
	UI_SYSTEMMENU,
	UI_PARTY,
	UI_GUILD,
	UI_HELP,
	UI_HELP3,
	UI_SINGLE_BATTLE,
	UI_GUILD_BATTLE, // Date : 2005-03-17,   By Lee Ki-hwan
	UI_SIEGE_WARFARE,	// 공성전 // Date : 2005-06-27(오전 10:46:11), By Lee Ki-hwan
	UI_SIEGE_WARFARE_NEW,	// 신공성전 // WSS_DRATAN_SIEGEWARFARE 070727
	UI_GUILDWARPORTAL,
	UI_BILLING,			// 대만 유료화(정액) Date : 2005-04-30(오후 3:55:19), By Lee Ki-hwan
	UI_BILL_ITEM,		// 대만 유료화 아이템 지급 시스템
	UI_MAP,
	UI_OPTION,
	UI_REFINE,
	UI_PROCESS,
	UI_PRODUCT,		// 이기환(12.7)	
	UI_MIX,
	UI_MIXNEW,		//  wooss 060523
	UI_COMPOUND,	// Date : 2005-01-12,   By Lee Ki-hwan
	UI_REMISSION,
	UI_QUEST,
	UI_QUESTBOOK_LIST,
	UI_QUESTBOOK_CONTENT,
	UI_QUESTBOOK_COMPLETE,
	UI_QUESTBOOK_NEW,
	UI_SKILLLEARN,
	UI_CHANGEWEAPON,
	UI_HELPER,
	UI_INITJOB,
	UI_TELEPORT,
	UI_TELEPORT_PRIMIUM,	// [KH_070315] 프리미엄 메모리스크롤 관련 추가
	UI_MESSENGER,
	UI_WEBBOARD,
	UI_QUIZ,
	UI_PETTRAINING,
	UI_PETINFO,
	UI_RANKING,	
	UI_GAMBLE,
	UI_PROCESSNPC,
	UI_SELECTRESOURCE,
	UI_CASH_SHOP,			//wooss 050820 
	UI_GW_MIX, // 공성 아이템 조합 
	UI_GUILDSTASH,	// 길드 나스 보관함 
	UI_SELECTWORD,
	UI_SELECTLIST,	// 친구찾기이벤트 060126
	UI_PETITEMMIX,	// 펫 아이템 조합( 유니크 아이템 제작 )
	UI_PETFREE,			// 펫 봉인 해제
	UI_QUIZEVENT,		// O.X QUIZ EVENT
	UI_PARTYAUTO,
	UI_COLLECTBOX,		// 곤충 채집 박스 
	UI_BINGOBOX,		// 러브러브 이벤트 초코렛 채집 박스
	UI_NPCHELP,			// NPC 안내 시스템
	UI_SIMPLE_POP,		// [KH_070419] 심플 팝업 관련 추가
	UI_MINIGAME,		// WSS_MINIGAME 070418
	UI_FLOWERTREE,		// 꽃놀이 인터페이스	

	UI_GUILD_NOTICE,	// WSS_NEW_GUILD_SYSTEM 070715 
	UI_MONSTER_COMBO,	// ttos_080116 : 몬스터 콤보
	UI_WILDPET_INFO,	// 공격 펫 Info
	UI_AUCTION,			// 거래대행
	UI_TATOO,			// [090709: selo] 펫 문양 시스템
	UI_CHAT_FILTER,		//[ttos_2009_7_17]: CHARATER_CHAT_FILTER 캐릭터 채팅 필터
	UI_SELECTIVE_SHOW_END,									// 선택적으로 보이는 인터페이스     -->|

	UI_LOGIN_SHOW_START		= UI_SELECTIVE_SHOW_END,		// 로그인 인터페이스 |<--
	UI_LOGIN				= UI_LOGIN_SHOW_START,
	UI_CREATE_CHAR,
	UI_SEL_CHAR,
	UI_SEL_SERVER,
	UI_LOGIN_SHOW_END,										// 로그인 인터페이스     -->|

	UI_MESSENGER_TALK_START = UI_LOGIN_SHOW_END,			// 메신저 대화 상자     -->|
	UI_MESSENGER_TALK1		= UI_MESSENGER_TALK_START,
	UI_MESSENGER_TALK2,
	UI_MESSENGER_TALK3,
	UI_MESSENGER_TALK4,
	UI_MESSENGER_TALK5,
	UI_MESSENGER_TALK6,
	UI_MESSENGER_TALK7,
	UI_MESSENGER_TALK8,
	UI_MESSENGER_TALK9,
	UI_MESSENGER_TALK10,
	UI_MESSENGER_TALK_END,									// 메신저 대화 상자 |<--

	UI_MESSAGEBOX_START		= UI_MESSENGER_TALK_END,		// 메세지 박스 인터페이스 |<--
	UI_MESSAGEBOX1			= UI_MESSAGEBOX_START,
	UI_MESSAGEBOX2,
	UI_MESSAGEBOX3,
	UI_MESSAGEBOX4,
	UI_MESSAGEBOX5,
	UI_MESSAGEBOX6,
	UI_MESSAGEBOX7,
	UI_MESSAGEBOX8,
	UI_MESSAGEBOX9,
	UI_MESSAGEBOX10,
	UI_MESSAGEBOX11,
	UI_MESSAGEBOX12,
	UI_MESSAGEBOX13,
	UI_MESSAGEBOX14,
	UI_MESSAGEBOX15,
	UI_MESSAGEBOX_END,										// 메세지 박스 안터페이스     -->|

	UI_MESSAGEBOXL_START	= UI_MESSAGEBOX_END,			// 대화 상자 인터페이스 |<--
	UI_MESSAGEBOXL1			= UI_MESSAGEBOXL_START,
	UI_MESSAGEBOXL2,
	UI_MESSAGEBOXL3,
	UI_MESSAGEBOXL4,
	UI_MESSAGEBOXL5,
	UI_MESSAGEBOXL_END,										// 대화 상자 안터페이스     -->|

	UI_TYPE_END				= UI_MESSAGEBOXL_END,
};


// Message box command code
enum UI_MSGCMD
{
	MSGCMD_DROPITEM	= 0,					// 아이템 드롭
	MSGCMD_EXCH_REQ_SRC,					// 교환 요청
	MSGCMD_EXCH_REQ_DEST,					// 교환 요청
	MSGCMD_EXCH_ADD_ITEM,					// 교환창에 아이템 추가
	MSGCMD_EXCH_DEL_ITEM,					// 교환창에서 아이템 삭제
	MSGCMD_WAREHOUSE_ADD_ITEM,				// 창고에 추가
	MSGCMD_WAREHOUSE_ADD_PLUSITEM,			// 창고에 추가
	MSGCMD_WAREHOUSE_ADD_MONEY,				// 창고에 돈 넣기.
	MSGCMD_WAREHOUSE_ADD_REQ,				// 보관 확인
	MSGCMD_WAREHOUSE_ERROR,					// 보관 오류.
	MSGCMD_WAREHOUSE_DEL_ITEM,				// 창고에서 인벤에 추가	
	MSGCMD_GUILD_JOIN,						// 길드 가입
	MSGCMD_GUILD_JOIN_REQ,					// 가입 신청중
	MSGCMD_GUILD_QUIT,						// 길드 탈퇴
	MSGCMD_GUILD_QUIT_CONFIRM,				// 길드 탈퇴 확인
	MSGCMD_GUILD_DESTROY,					// 길드 해체
	MSGCMD_GUILD_ERROR,						// 길드에서 에러 발생.
	MSGCMD_GUILD_ROOM,						// 길드 전용 공간으로 이동.
	MSGCMD_GUILD_APPLICANT_JOIN,			// 멤버 신청을 함.
	MSGCMD_GUILD_KICK,						// 멤버 퇴출
	MSGCMD_GUILD_ADD_VICE_BOSS,				// 부단장 임명
	MSGCMD_GUILD_DEL_VICE_BOSS,				// 부단장 퇴출
	MSGCMD_GUILD_CHANGE_BOSS,				// 단장 이임
	MSGCMD_GUILD_LORD_NOTICE,				// 성주 공지 
	 
	// MSGCMD_GUILD_BATTLE_ // Date : 2005-03-19,   By Lee Ki-hwan
	MSGCMD_GUILD_BATTLE_ERROR,
	MSGCMD_GUILD_BATTLE_REQ,
	MSGCMD_GUILD_BATTLE_ACCEPT,
	MSGCMD_GUILD_BATTLE_STOP_REQ,
	MSGCMD_GUILD_BATTLE_STOP_ACCEPT,
	MSGCMD_GUILD_BATTLE_STOP_REQ_REP,
	MSGCMD_GUILD_BATTLE_MESSAGE,

	//	MSGCMD_BILLING // Date : 2005-05-06(오후 1:53:01), By Lee Ki-hwan
	MSGCMD_BILLING_NOT_ALLOW,				// 사용권한 없음(화면7)
	MSGCMD_BILLING_CONFIRM,
	MSGCMD_BILLING_CHARGE_CONFIRM,
	MSGCMD_BILLING_ERROR,
	MSGCMD_BILL_ITEM_REP,

	MSGCMD_SHOP_ADD_ITEM,					// 상점창에 아이템 추가
	MSGCMD_SHOP_DEL_ITEM,					// 상점창에서 아이쳄 삭제
	MSGCMD_SHOP_ADD_PLUSITEM,				// 상점창에서 업그레이드 아이템 추가
	MSGCMD_SHOP_PRICE,						// 상점창에 가격 입력.
	MSGCMD_SHOP_FAIL_ITEM,					// 상점창에서 판매할 수 없는 아이템의 경우
	MSGCMD_SHOP_START,						// 상점 시작.	
	MSGCMD_PERSONALSHOP_ERROR,				// 개인 상점 이름에 오류.
	MSGCMD_SYSMENU_EXIT,					// 게임 종료
	MSGCMD_SYSMENU_RESTART,					// 게임 재시작
	MSGCMD_PARTY_INVITE,					// 파티 초대
	MSGCMD_PARTY_ALLOW,						// 파티 수락
	MSGCMD_PARTY_MANDATE_REQ,				// 파티 위임 요청 
	MSGCMD_PARTY_MANDATE,					// 파티 위임
	MSGCMD_INVEN_UPGRADE_REQ,				// 아이템 업그레이드 요청
	MSGCMD_INVEN_UPGRADE_REP,				// 아이쳄 업그레이드 결과
	MSGCMD_ITEM_LEVELDOWN_REQ,				// 아이템 레벨 다운 요청
	MSGCMD_ITEM_LEVELDOWN_REP,				// 아이템 레벨 다운 결과
	MSGCMD_INVEN_OPTION_ADD_REQ,			// 아이템 옵션 추가 요청
	MSGCMD_INVEN_OPTION_ADD_REP,			// 아이템 옵션 추가 결과
	MSGCMD_INVEN_OPTION_DEL_REQ,			// 아이템 옵션 제거 요청
	MSGCMD_INVEN_OPTION_DEL_REP,			// 아이템 옵션 제거 결과
	MSGCMD_REFINE_REP,						// 제련 결과
	MSGCMD_QUEST_GIVEUP,					// 퀘스트 포기
//	MSGCMD_QUEST_PRIZE,						// 퀘스트 보상
	MSGCMD_QUEST_NOTIFY,					// 퀘스트 에러, 확인
	MSGCMD_QUEST_NEWQUEST,					// 새로운 퀘스트 확인.
	MSGCMD_QUEST_FAIL,						// 퀘스트 실패(공주가 죽음.)
	MSGCMD_QUEST_COMPLETE,					// 퀘스트 완료 확인.
	MSGCMD_SKILLLEARN_NOTIFY,				// 스킬 습득 에러
	MSGCMD_REMISSION_NOTIFY,				// 면죄부 습득 에러
	MSGCMD_REMISSION_BUY,					// 면죄부 구입
	MSGCMD_QUEST_START,						// 퀘스트 시작
	MSGCMD_TELEPORT_MEMO,					// 메모리 스크롤 저장
	MSGCMD_LOGIN_ERROR,						// 로그인 에러
	MSGCMD_SERVER_ERROR,					// 서버 선택 부분에서 에러
	MSGCMD_SELCHAR_ERROR,					// 캐릭터 선택 부분에서 에러
	MSGCMD_CREATE_ERROR,					// 캐릭터 생성 부분에서 에러
	MSGCMD_DELETE_CHARACTER,				// 캐릭터 삭제
	MSGCMD_DELETE_CHARACTER_SECURITY,				// 캐릭터 삭제 보안 	
	MSGCMD_DELETE_CHARACTER_ERROR,			// 캐릭터 삭제 오류 
	MSGCMD_DISCONNECT,						// 접속끊김
	MSGCMD_CONNECT_ERROR,					// 접속 실패
	MSGCMD_WEBBOARD_ERROR,					// 웹에서 문제 생김
	MSGCMD_PROCESS_REP,						// 가공 결과
	MSGCMD_PROCESS_NOTIFY,					// 가공 에러, 확인
	MSGCMD_PRODUCT_REP,						// 제조 결과
	MSGCMD_PRODUCT_NOTIFY,					// 제조 에러, 확인
	MSGCMD_EVENT_PRIZE,						// 이벤트 보상 요청.
	MSGCMD_EVENT_RESULT,					// 이벤트 보상 결과.
	MSGCMD_WEAPONCHANGE_EVENT,				// 무기 교체 확인.
	MSGCMD_MIX_REP,							// 조합 결과
	MSGCMD_MIX_NOTIFY,						// 조합 에러, 확인
	MSGCMD_COMPOUND_REP,					// 합성 결과
	MSGCMD_CHANGEJOB,						// 전직 할꺼야???
	MSGCMD_INITJOB_NOTIFY,					// 임시
	MSGCMD_INITJOB_GIVEUP,					// 직업 포기
	MSGCMD_INITJOB_STAT,					// 스탯 환원
	MSGCMD_HELPER_NOTIFY,					//
	MSGCMD_HELPER_BEMYTEACHER,				// 내 선생이 되어달라고 요청.
	MSGCMD_HELPER_BEMYTEACHER_REQ,			// 내 선생이 되어달라고 요청 기다림.
	MSGCMD_HELPER_FIRE_MYTEACHER,			// 학생이 선생을 짜름.
	MSGCMD_HELPER_REGISTER,					// 선생으로 목록에 올림.
	MSGCMD_HELPER_CANCEL_REGISTER,			// 선생 목록에서 내림.
	MSGCMD_HELPER_STUDENT_ACCEPT,			// 후견인으로 등록 확인
	MSGCMD_SIEGE_WARFARE_ERROR,
	MSGCMD_SIEGE_WARFARE_INFO,				// 공성전- 공성정보 확인창
	MSGCMD_CHANGEWEAPON_REP,				// 무기 교체 확인
	MSGCMD_CHANGEWEAPON_NOTIFY,				// 무기 교체 메세지
	MSGCMD_PETTRAINING_NOTIFY,
	MSGCMD_PET_CHANGE,						// 탈 것으로 변환?	
	MSGCMD_PET_CHANGE_NOTIFY,				// 탈 것으로 변환?	
	MSGCMD_PET_DESTRUCTION,					// 펫 재료아이템 변환시 소멸 메세지		eons
	MSGCMD_PARTYAUTO_REGIST,				// 파티 오토 시스템 ( 파티 등록 )
	MSGCMD_PARTYAUTO_REGIST_ERROR,			// 파티 오토 시스템 ( 파티 등록 에러 )
	MSGCMD_PARTYAUTO_JOIN_ALLOW,			// 파티 오토 시스템의 조인 허락 및 거절
	
	// Date : 2005-05-23(오후 6:05:37), By Lee Ki-hwan
	MSGCMD_MESSENGER_START =  MSGCMD_CHANGEWEAPON_NOTIFY + 1, 
	MSGCMD_MESSENGER_ADD_REQ = MSGCMD_MESSENGER_START,	// 친구 등록 신청
	MSGCMD_MESSENGER_ADD_REP,				// 친구 등록 수락
	MSGCMD_MESSENGER_ADD_WAIT,				// 친구 등록 대기
	MSGCMD_MESSENGER_BLOCK_REQ,				// 친구 차단 요청
	MSGCMD_MESSENGER_BLOCK,					// 친구 차단
	MSGCMD_MESSENGER_UNBLOCK,					// 친구 해제
	MSGCMD_MESSENGER_DEL_REQ,				// 친구 삭제 요청
	MSGCMD_MESSENGER_DEL,					// 친구 삭제

	MSGCMD_MESSENGER_GROUP_ADD,				// 그룹 추가
	MSGCMD_MESSENGER_GROUP_DEL,				// 그룹 삭제
	MSGCMD_MESSENGER_GROUP_RENAME,			// 그룹 이름 변경
	MSGCMD_MESSENGER_GROUP_MOVE,			// 그룹 변경

	MSGCMD_MESSENGER_ERROR,					// 에러 
	MSGCMD_MESSENGER_END = MSGCMD_MESSENGER_ERROR,  
	MSGCMD_PC_DEATH,						// 캐릭터 사망시 마을로 이동	
	MSGCMD_SUPERGOJE_REQUEST,				// 초고급 제련석 받고 싶어?
	MSGCMD_SUPERGOJE_NOTIFY,				// 초고급 제련석 관련 알림
	MSGCMD_OLD_PASSWORD,					// 이전 패스워드
	MSGCMD_NEW_PASSWORD,					// 새로운 패스워드
	MSGCMD_CONFIRM_PASSWORD,				// 암호 확인
	MSGCMD_SECURITY_NOTIFY,
	MSGCMD_SECURITY_PASSWORD,				// 암호 묻기
	MSGCMD_PASSWORD_UNLOCK_A,				// 암호 풀기 번호 묻기 (대만)
	MSGCMD_PASSWORD_UNLOCK_R,				// 암호 입력 	

	MSGCMD_BOOSTER_UPGRADE_REQ,
	MSGCMD_BOOSTER_UPGRADE_REP,
	MSGCMD_SIEGE_WARFARE_MOVEING_CONFIRM,	// 공성 지역 이동 확인 	
	MSGCMD_BOOST_RANDON_PRODUCT_ERROR,		// 부스터 생산 오류
	
	// Guild Nast Stash // Date : 2005-09-06(오전 10:03:33), By Lee Ki-hwan
	MSGCMD_GUILDSTASH_TAKE_VIEW,	
	MSGCMD_GUILDSTASH_TAKE_STASH,	
	MSGCMD_GUILDSTASH_INFO,	
	MSGCMD_GUILDSTASH_ERROR,
	// Date : 2005-09-06(오전 10:03:41), By Lee Ki-hwan

	MSGCND_MOON_DAY_EVENT_GIVE_ITEM,		//2006 추석 이벤트: 오색송편을 보상품으로 교환
	MSGCMD_HARVEST_MOON_DAY_EVENT_UPGRADE1,	//2006 추석 이벤트: 송편을 만듦.
	MSGCMD_HARVEST_MOON_DAY_EVENT_UPGRADE2,	//2006 추석 이벤트: 오색송편을 만듦.
	
	// wooss 050802 add MSGCMD 
	MSGCMD_USE_CONFIRM_HEXP,				//사용후 죽었을 경우 주문서 사용여부를 확인한다 
	MSGCMD_USE_CONFIRM_AEXP,
	MSGCMD_USE_CONFIRM_HEXP_LUCKY,
	MSGCMD_USE_CONFIRM_AEXP_LUCKY,
	MSGCMD_USE_CONFIRM_REBIRTH,
	MSGCMD_USE_RECOVER_ITEM_HEXP,			// 경험치 복구 주문서를 사용한다
	MSGCMD_USE_RECOVER_ITEM_AEXP,			// 숙련치 복구 주문서
	MSGCMD_USE_RECOVER_ITEM_HEXP_LUCKY,
	MSGCMD_USE_RECOVER_ITEM_AEXP_LUCKY,
	MSGCMD_USE_REBIRTH_ITEM,				// 부활 주문서
	
	// wooss 050803 
	MSGCMD_CHECK_ITEM_HEXP,					// inventory창에서 더블 클릭시  
	MSGCMD_CHECK_ITEM_AEXP,					// 다른 아이템 사용여부 체크
	MSGCMD_CHECK_ITEM_REBIRTH,				// 경험치복구 ,숙련도복구, 부활 주문서

	// wooss 050807
	MSGCMD_USE_WARP_ITEM,					// 순간이동 마법서를 사용한다
	MSGCMD_WARP_TO_CANCEL,					// 순간이동을 취소한다
	MSGCMD_WARP_TAKE_CANCEL,				// 소환을 취소한다 
	MSGCMD_WARP_TO_REQ_CALL,				// 순간이동 요청을 받았을 경우 sessionState에서 처리 
	MSGCMD_WARP_TAKE_REQ_CALL,				// 소환 요청을 받았을 경우 sessionState에서 처리

	// wooss 050817
	MSGCMD_USE_PC_SUMMON_ITEM,				// 소환 마법서를 사용한다
	// wooss 050818
	MSGCMD_USE_BOSS_SUMMON_ITEM,			// 보스 몬스터 소환 마법서
	// wooss 050812
	MSGCMD_CONFIRM_CHANGE_MY_NAME,			// 캐릭터 개명카드 사용여부 확인
	MSGCMD_CONFIRM_CHANGE_GUILD_NAME,		// 길드 개명카드 사용여부 확인
	MSGCMD_USE_CHANGE_MY_NAME_ITEM,			// 캐릭터 개명카드 사용
	MSGCMD_USE_CHANGE_GUILD_NAME_ITEM,		// 길드 개명카드 사용
	MSGCMD_BAN_NAME,						// 형식이 맞지않는 이름

	MSGCMD_CONFIRM_MEMSCROLL_EX,			// 메모리 스크롤 확장 사용확인
	MSGCMD_CONFIRM_WAREHOUSE_EX,			// 창고 확장 사용확인
	MSGCMD_PROLONG_MEMSCROLL_EX,			// 메모리 스크롤을 기한 연장
	MSGCMD_PROLONG_WAREHOUSE_EX,			// 확장 창고 기한 연장
	// wooss 050821
	MSGCMD_CONFIRM_SLOT_ITEM,				// 캐릭터 확장 슬롯 사용 확인 
	MSGCMD_USE_SLOT_ITEM,					// 캐릭터 확장 슬롯 사용

	MSGCMD_CONFIRM_PARTY_RECALL_ITEM,		// 파티리콜 아이템 사용 확인 060303 wooss
	
	MSGCMD_USE_ITEM,						// 단순 확인창을 거칠경우 UseSlotItem() 다시 보낼때	
	MSGCMD_SELECT_WORD_NOTIFY,				// 단어 선택 화면 

	MSGCMD_ASK_ONE_FIVE,					// 2pan4pan 이벤트 메시지 051031
	MSGCMD_ASK_ONE_FOUR,	
	MSGCMD_ASK_TWO_OPEN,

	MSGCMD_ASK_NEWYEAR1,					// 신년 이벤트1 꿈희망 이벤트 
	MSGCMD_ASK_NEWYEAR2,					// 신년 이벤트2 인내의 열매 이벤트

	MSGCMD_EVENT_COUPON_SENDNUM,			// 060118 wooss japan coupon event

	MSGLCMD_EVENT_CLOTHES_EXCHANGE,			// 일본 전통 의상 교환 이벤트
	
	MSGCMD_INPUT_FRIEND,					// 친구 찾기 이벤트 
	MSGCMD_TIME_FRIEND,	
	MSGCMD_REWARD_FRIEND,	
	
	MSGCMD_EX_PARTY_RECALL_PROMPT,			// 파티 리콜 ( 유료 아이템) // wooss 060306

	MSGCMD_CONFIRM_UPGRADESTONE_ITEM,		// wooss 060516 platinum item

	MSGCMD_CONFIRM_MIXNEWREQ_ITEM,				// wooss 060523 composite item 			
	MSGCMD_CONFIRM_MIXNEW_ITEM,				// wooss 060523 composite item 			
	MSGCMD_CONFIRM_UNMIX_ITEM,
	MSGCMD_PET_MOUNT_CANCEL_CARD,			// 펫 마운트 훈련 취소 카드 사용 확인 ( 유료 아이템 )
	MSGCMD_USE_PET_MOUNT_CANCEL_CARD,		// 펫 마운트 훈련 취소 카드 사용( 유료 아이템 )
 	
	MSGCMD_COMFIRM_USE_ITEM,				// (By eons) 같은 스킬의 아이템 사용시 확인 메세지 
	MSGCMD_WORLDCUP_EVENT1_NUM,				// 월드컵 이벤트
	MSGCMD_GOLDENBALL_EVENT,				// (By eons) 골든볼 이벤트

	MSGCMD_GAMBLE_READY,					// Cash MoonStone
	MSGCMD_GAMBLE_READYSEL,

	MSGCMD_SHOP_LEASE_ITEM,					// Lease Shop CMD
	
	MSGCMD_COLLECTBOX_BUY_REQ,					// 곤충 채집 상자 구입
	MSGCMD_INSECTITEM_DROP,						// 곤충 아이템 드롭

	MSGCMD_PROMO_KEY_ENTER,						//프로모 패키지 인증키 입력(말레샤)

	//Su-won
	MSGCMD_GIFT_SEND,
	MSGCMD_GIFT_RECV,
	MSGCMD_GIFT_ERROR,
	MSGCMD_RARE_IDENTIFY,			// 장비 감정 아이템 사용
	MSGCMD_USE_RARE_IDENTIFY,		// 장비 감정 아이템 사용 확인

	MSGCMD_USE_PACKING_PAPER,		// 포장지 아이템 사용
	MSGCMD_SAVE_BINGOITEM,			// 빙고 아이템 저장 확인
	MSGCMD_OK_EXCHANGE_CHOCOBOX,	// 초코상자 교환 확인

	MSGCMD_GOTO_EGEHA,

	// wooss 070305 ----------------------------------------------------->>
	// kw : WSS_WHITEDAY_2007
	MSGCMD_WHITEDAY_BLUELETTER,			// 화이트데이 편지 아이템 사용
	MSGCMD_WHITEDAY_PINKLETTER,
	// ------------------------------------------------------------------<<

// [KH_070326] 달걀 이벤트 관련
	MSGCMD_CONFIRM_EASTER_EGGS,
	
	MSGCMD_SONGKRAN_REWARD,				//태국 송크란 이벤트 금,은그릇 사용
	//WSS_GUILD_MASTER 070411 ----------------------->>
	MSGCMD_GUILDMASTER_1,				// 길드 포인트 윈도우
	MSGCMD_GUILDMASTER_1_1,				// 길드 포인트 등록
	MSGCMD_GUILDMASTER_1_2,				// 길드 포인트 갱신
	MSGCMD_GUILDMASTER_2_1,				// 교환권 받기 
	// ----------------------------------------------<<
	MSGCMD_MINIGAME_SELECT_ITEM,		// WSS_MINIGAME 070420 엽기 무기 선택

	MSGCMD_FLOWERTREE_SUPPORT,			// 꽃놀이 이벤트 개화돕기
	MSGCMD_FLOWERTREE_TICKET,			// 꽃놀이 이벤트 교환권 받기
	MSGCMD_FLOWERTREE_TICKETEX,			// 꽃놀이 이벤트 교환권 받기 확인

	MSGCMD_PET_NAMECARD_USE,			// 펫의 명찰: 아이템 사용
	MSGCMD_PET_NAMECARD_INPUT,			// 펫의 명찰: 이름 입력

	// [070702 :Su-won] SUMMER_2007
	MSGCMD_SUMMER2007_PAPERFOLDING,		//종이접기
	MSGCMD_SUMMER2007_PAPERBLENDING,	//색종이 조합

	// [070807: Su-won] EVENT_ADULT_OPEN
	MSGCMD_ADULTEVENT_ALCHEMIST,		// 성인서버 오픈이벤트(장비조합 연금술 이벤트): 장비 조합
	MSGCMD_EVENT_SHOOT,  // 성인서버 오픈이벤트(성인들만의 특권): 참여 확인창
	
	MSGCMD_SIEGE_WARFARE_BUY_QUARTER,	// WSS_DRATAN_SEIGEWARFARE 2007/08/13
	MSGCMD_SIEGE_WARFARE_WAIT_QUARTER,	// WSS_DRATAN_SEIGEWARFARE 2007/08/13
	MSGCMD_SIEGEWARFARE_REBIRTH,		// WSS_DRATAN_SEIGEWARFARE 2007/08/22

	MSGCMD_BJMONO_2007_CHANGE_REQ,		// SK BJ MONO 응모권 교환 요청 메세지 박스
	MSGCMD_TG2007_1000DAYS_CAP_UPGRADE,	// 기념모자 업그레이드 메세지 박스

	// [070824: Su-won] REFINE_PROTECT
	MSGCMD_INVEN_UPGRADE_PROTECT_REQ,	// 제련 보호 아이템 사용

	// [070824: Su-won] PET_COLOR_CHANGE
	MSGCMD_PET_COLORCHANGE_USE,			// 펫 색상 변경 아이템 사용
	// [090713: selo] PET_TATOO_CHANGE
	MSGCMD_PET_TATOOCHANGE_USE,			// 펫 색상 및및 문양 변경 아이템 사용

	// [071122: Su-won] DRATAN_SIEGE_DUNGEON
	MSGCMD_SIEGE_DUNGEON_STATE,						//던전 상태창
	MSGCMD_SIEGE_DUNGEON_ENVIRONMENT_CONTROL,		//던전 내부 환경 제어창
	MSGCMD_SIEGE_DUNGEON_MONSTER_CONTROL,			//던전 내부 몬스터 제어창
	MSGCMD_SIEGE_DUNGEON_ADMISSION_FEE_CONTROL,		//던전 입장료 설정창
	MSGCMD_SIEGE_DUNGEON_HUNTING_FEE_CONTROL,		//던전 수렵세율 설정창

	MSGCMD_SIEGE_DUNGEON_ENVIRONMENT_CHANGE,		//던전 내부 환경 변경 확인창
	MSGCMD_SIEGE_DUNGEON_MONSTER_CHANGE,			//던전 내부 몬스터 변경 확인창
	MSGCMD_SIEGE_DUNGEON_ADMISSION_FEE_CHANGE,		//던전 입장료 변경 확인창
	MSGCMD_SIEGE_DUNGEON_HUNTING_FEE_CHANGE,		//던전 수렵세율 변경 확인창
	MSGCMD_SIEGE_DUNGEON_ENTER,						//던전 이동 확인창

	// [071122: Su-won] NEW_MOONSTONE
	MSGCMD_MOONSTONE_MIX,
	
	MSGCMD_EVENT_XMAS_2007_DECO,					// 크리스마스 트리 확인창
	MSGCMD_PETTRANSFORM_RU,							// 펫 루돌프 변신 주문서 확인창

	MSGCMD_EVENT_NEWYEAR_2008,						// 2008 신년이벤트, 버프 수락 확인창

	MSGCMD_USE_SMALL_CORD,							// 복주머니 노끈 사용
	MSGCMD_OK_EXCHANGE_LUCKYBOX,					// 소망상자 교환 확인

	MSGCMD_EVENT_SAKURA_2008,						// 2008년 벚꽃 이벤트: 보상교환 확인창

	MSGCMD_EVENT_CHAOSBALL,							//카오스볼 이벤트: 카오스볼 봉인해제 창
	
	MSGCMD_PRESSCORPS,

	MSGCMD_ITEMDELETE,								// 인벤토리 휴지통 메시지 처리

	MSGCMD_PARTY_OPTION,							//파티 옵션(종류 변경) 창

	MSGCMD_EVENT_PHOENIX,							//피닉스 이벤트
	///////////////////////////////////////////////////////////////////////////////////	
	// Cube
	MSGCMD_CUBE_PARTY_GROUP_ENTRANCE,						//Cube: 파티 입장
	MSGCMD_CUBE_PARTY_PERSON_ENTRANCE,							//Cube: 파티 개인 입장
	MSGCMD_CUBE_GUILD_GROUP_ENTRANCE,						//Cube: 길드 입장
	MSGCMD_CUBE_GUILD_PERSON_ENTRANCE,							//Cube: 길드 개인 입장
	MSGCMD_CUBE_STATE,														//Cube: 큐브 현황
	MSGCMD_CUBE_CHANGE_RATE,											//Cube: 세율 변경
	// Cube
	///////////////////////////////////////////////////////////////////////////////////	
	MSGCMD_INITJOB_SSKILL,							// 스페셜 스킬 초기화 확인창
	
	MSGCMD_AUCTION_BUYITEM,
	MSGCMD_AUCTION_ITEMREGISTER,
	MSGCMD_AUCTION_CANCELREGISTER,
	MSGCMD_AUCTION_INFO,

	//[ttos_2009_7_17]: CHARATER_CHAT_FILTER 캐릭터 채팅 필터
	MSGCMD_CHAT_BLOCK_REQ,
	MSGCMD_CHAT_BLOCK,
	MSGCMD_CHAT_ERROR,

	// wooss 050806
	MSGCMD_NULL,						// 단순 확인 메시지박스 처리 
	MSGCMD_TOTAL
};

enum UI_MSGLCMD
{
	MSGLCMD_REFINE_REQ = 0,					// 제련 요청창
	MSGLCMD_PROCESS_REQ,					// 가공 요청창
	MSGLCMD_PRODUCT_REQ,					// 제조 요청창
	MSGLCMD_MIX_REQ,						// 조합(재활의 상자) 요청창
	MSGLCMD_COMPOUND_REQ,					// 합성(비밀의 상자) 요청창
	MSGLCMD_QUEST_INTRO,					// 퀘스트 소개창
	MSGLCMD_QUEST_PRIZE,					// 퀘스트 보상창.
// FIXME : 튜토리얼만 특이해서... 맘에 안듬.
	MSGLCMD_QUEST_TUTORIAL,					// 튜토리얼 퀘스트
	MSGLCMD_SHOP_REQ,						// SHOP REQUEST		Insert By cpp2angle (04.12.18)
	MSGLCMD_GUILD_REQ,						// 길드
	MSGLCMD_GUILD_BATTLE_REQ,				// 길드 전투 // Date : 2005-03-17,   By Lee Ki-hwan
	MSGLCMD_WAREHOUSE_REQ,					// 창고...
	MSGLCMD_SECURITY_REQ,					// 보안...
	MSGLCMD_SKILLLEARN_REQ,					// 스킬 습득창
	MSGLCMD_SSKILLLEARN_REQ,				// 특수 스킬 습득창
	MSGLCMD_CHANGEJOB_REQ,					// 전직창...
	MSGLCMD_REMISSION_REQ,					// 면죄부
	MSGLCMD_QUEST_REQ,						// 퀘스트.
	MSGLCMD_EVENT,							// 이벤트 선택창.
	MSGLCMD_HARVEST_MOON_DAY1,				// 추석맞이 이벤트 
	MSGLCMD_HARVEST_MOON_DAY2 = 20,				// 추석맞이 이벤트 
	MSGLCMD_HELPER_REQ,						// 후견인 시스템.
	MSGLCMD_INITJOB_REQ,					// 임시
	MSGLCMD_CHANGEWEAPON_REQ,				// 무기 교체
	MSGLCMD_STORY_INTRO,					// 싱글던젼 내용 소개
	MSGLCMD_CHANGEWEAPON_EVENT_REQ,			// 무기 교체 이벤트	
	MSGLCMD_GATE_OF_DIMENSION,				// 퍼스널 던젼 4 - 차원의 문
	MSGLCMD_EVENT_NOTICE,					// 이벤트 알림
	MSGLCMD_TREASUREBOX_EVENT,				// 보물 상자 이벤트.
	MSGLCMD_SIEGE_WARFARE,					// 공성전 - 기본
	MSGLCMD_MERCEAR_REQ = 30,	
	MSGLCMD_PROCESSNPC_REQ,					// 가공 NPC
	MSGLCMD_PROCESSNPC_MINERAL_REQ,			// 광석 정련
	MSGLCMD_PROCESSNPC_ENERGY_REQ,			// 원소 정제
	MSGLCMD_PROCESSNPC_CROP_REQ,			// 식물 가공.
	MSGLCMD_PETTRAINING_REQ,				// 애완동물 훈련.
	MSGLCMD_PETCHANGE_REQ,					// 애완동물 변형
	MSGLCMD_PETSKILLINIT_REQ,				// 애완동물 스킬 초기화
	MSGLCMD_PETSTUFFCHANGE_REQ,				// 애완동물을 재료 아이템으로 변환  // eons
	MSGLCMD_PETUNIQUEPRODUCT_REQ,			// 유니크 아이템 제작		// eons
	MSGLCMD_GW_MIX_REQ = 40,						// 공성 아이템 조합 
	MSGLCMD_PARTYAUTOMATCH_REQ,				// 파티 오토 매칭 시스템	// eons
	MSGCMD_GW_MIX_REP,
	MSGLCMD_GUILDSTASH_REQ,
	MSGLCMD_RENUAL_EVENT,
	MSGLCMD_EVENT_2PAN4PAN_ONE,				// 이판사판 이벤트 머니 교환 wooss 051031
	MSGLCMD_EVENT_2PAN4PAN_TWO,				// 이판사판 이벤트 보물상자 열기
	MSGCMD_EVENT_2PAN4PAN_THREE,			// 이판사판 이벤트 캐릭터보상 카드
	
	MSGLCMD_EVENT_NEWYEAR1,					// 신년이벤트 꿈과 희망
	MSGLCMD_EVENT_NEWYEAR2,					// 신년이벤트 인내의 열매
	MSGLCMD_EVENT_FIND_FRIEND = 50,				// 친구 찾기 이벤트
	MSGLCMD_CLOTHES_EXCHANGE,				// 일본 전통의상 이벤트
	MSGLCMD_EVENT_OXQUIZ,					// O.X이벤트
	MSGLCMD_PLATINUM_STONE_EXCHANGE,		// 플래티늄 제련석 교환
	MSGCMD_ASK_PLATINUM_EXCHANGE1,			// 플래티늄 제련석 교환 확인
	MSGCMD_ASK_PLATINUM_EXCHANGE2,
	MSGCMD_ASK_PLATINUM_EXCHANGE3,
	MSGLCMD_RAINYDAY,						// 일본 빗방울 이벤트
	MSGLCMD_MOTHERDAY,						// 태국 어머니날 이벤트
	MSGCMD_WORLDCUP_EVENT1,			// 월드컵 이벤트 1 (우승국)
	MSGCMD_WORLDCUP_EVENT1_EXCHANGE,
	MSGCMD_WORLDCUP_EVENT1_RECEIVE,

	MSGCMD_WORLDCUP_GOLDENBALL,				// 골든볼 이벤트
	MSGCMD_WORLDCUP_GOLDENBALL_ENTRY,		// 골든볼 이벤트 응모
	MSGCMD_WORLDCUP_GOLDENBALL_REQUITAL,	// 골든 볼 이벤트 보상
	
	MSGLCMD_BUDDHISM_EVENT,					// 불교 촛불 축제 이벤트( 태국 )
	MSGLCMD_COLLECT_INSECT_EVENT,				// 여름 곤충 채집 이벤트
	MSGLCMD_COLLECT_BOX_GIFT,				// 곤충 채집상자 보상
	MSGLCMD_EVENT_PROMOPACK,				// 프로모 패키지 이벤트 (말레샤)
	
	MSGLCMD_EVENT_XMAS_2006,				// 2006 XMas Event [12/11/2006 Theodoric]
	MSGLCMD_EVENT_XMAS_2006_COUNT,			// 2006 XMas Event [12/11/2006 Theodoric]
	MSGLCMD_EVENT_XMAS_2006_CHANGE,			// 2006 XMas Event [12/11/2006 Theodoric]

	MSGLCMD_EVENT_LOVE_LOVE,				// 러브러브 이벤트

											// wooss 070305 kw: WSS_WHITEDAY_2007
	MSGLCMD_WHITEDAY_2007,					// 2007 WhiteDay Event
	
	MSGLCMD_EVENT_MAY,						// 5월 가정의달 이벤트
	
	MSGLCMD_GUILDMASTER,					// WSS_GUILD_MASTER 070411
	MSGLCMD_GUILDMASTER_2,
	
	MSGLCMD_EVENT_MINIGAME,					// WSS_MINIGAME 070420	

	MSGLCMD_SUMMER_2007,					// [070702 :Su-won] SUMMER_2007
	MSGLCMD_RESERVE_REQ,					// 예비 (악덕 상인, 어둠의 스킬 마스터 사용)	
	MSGLCMD_SIEGE_WARFARE_UPGRADE,			// WSS_DRATAN_SIEGEWARFARE 070730
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	// [070807: Su-won] EVENT_ADULT_OPEN
	MSGLCMD_EVENT_ADULT_MAGICCARD,			// 성인서버 오픈 이벤트(매직카드를 찾아라!)
	MSGLCMD_EVENT_ADULT_CHANGEJOB,			// 성인서버 오픈 이벤트(전직달성!절망의 부적을 받아라!)
	MSGLCMD_EVENT_ADULT_ALCHEMIST,			// 성인서버 오픈 이벤트(장비조합 연금술 이벤트)
	// [070807: Su-won] EVENT_ADULT_OPEN
	////////////////////////////////////////////////////////////////////////////////////////////////
	
	MSGLCMD_TG2007_FRUITFULL,				// WSS_TG2007 2007/09/14
	
	// [071122: Su-won] DRATAN_SIEGE_DUNGEON
	MSGLCMD_DRATAN_SIEGE_DUNGEON_REQ,
	MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL,

	MSGLCMD_RED_TREASUREBOX_EVENT,
	MSGCMD_EX_MONSTERCOMBO_GOTO_COMBO_PROMPT, // 몬스터 콤보 대기방 이동
	MSGCMD_EX_MONSTERCOMBO_RECALL_TO_COMBO_PROMPT, // 대기방에서 파티 소환
	MSGCMD_COMBO_GIVEUP,						// 몬스터 콤보 포기

	MSGLCMD_CHAOSBALL_EVENT,				//카오스볼 이벤트	

	//Cube
	MSGLCMD_CUBE_PARTY_ENTRANCE,		//파티입장 선택창
	MSGLCMD_CUBE_GUILD_ENTRANCE,		//길드입장 선택창
	
	MSGLCMD_TOTAL
};

// wooss 050802 define cash item 
#define		RECOVER_HEXP_ITEM		844		// 경험치 복구 주문서	
#define		RECOVER_HEXP_ITEM_LUCKY	2035	// 럭키 경험치 복구 주문서
#define		RECOVER_AEXP_ITEM		845		// 숙련도 복구 주문서
#define		RECOVER_AEXP_ITEM_LUCKY	2036	// 럭키 숙련도 복구 주문서
#define		REBIRTH_ITEM			846		// 부활 주문서
#define		WARP_ITEM				857		// 순간이동 마법서
#define		PC_SUMMON_ITEM			858		// 캐릭터(PC) 소환 마법서	
#define		BAAL_SUMMON_ITEM		859		// 바알 소환 마법서
#define		DEATHNIGHT_SUMMON_ITEM	860		// 데스나이트 소환 마법서
#define		CHANGE_MY_NAME_ITEM		842		// 캐릭터 개명카드
#define		CHANGE_GUILD_NAME_ITEM	843		// 길드 개명카드	
#define		MEMSCROLL_EX_ITEM		840		// 메모리 스크롤 확장 카드
#define		WAREHOUSE_EX_ITEM		841		// 개인창고 확장 카드
#define		INIT_STATUS_ITEM		847		// 스테이터스 초기화 카드
#define		INIT_COMBAT_SKILL_ITEM	848		// 전투스킬 초기화 카드
#define		INIT_SPECIAL_SKILL_ITEM	849		// 특수스킬 초기화 카드
#define		INIT_ALL_ITEM			851		// 능력치 초기화 카드
#define		EX_SLOT_ITEM			839		// 캐릭터 슬록 확장 카드
#define		EXCHANGE_WEAPON_ITEM	975		// 무기 교환 카드	
#define		EXCHANGE_ARMOR_ITEM		2355	// 방어구 교환 카드
#define		CHANGE_MY_NAME_ITEM_UE	1120	// 캐릭터 개명 카드(통합이벤트 기존 개명카드에서 속서을 다르게 처리)
#define		PARTY_RECALL_ITEM1		1298	// 무기 교환 아이템	
#define		PARTY_RECALL_ITEM2		1299	// 무기 교환 아이템	
#define		PARTY_RECALL_ITEM3		1300	// 무기 교환 아이템	
#define		PARTY_RECALL_ITEM4		1413	// 무기 교환 아이템	
#define		PARTY_RECALL_ITEM5		1414	// 무기 교환 아이템	
#define		PARTY_RECALL_ITEM6		1415	// 무기 교환 아이템	
#define		PLATINUM_IRIS_ITEM				1416	// 플래티늄 아이리스 
#define		PLATINUM_BOOSTER_ITEM			1417	// 플래티늄 부스터 
#define		PLATINUM_UPGRADESTONE_ITEM1		1418	// 플래티늄 제련석(감정 전)	
#define		PLATINUM_UPGRADESTONE_ITEM2		1419	// 플래티늄 제련석(감정 후)
#define		COMPOSITE_ITEM1					1420	// 결합 주문서
#define		COMPOSITE_ITEM2					1421	// 결합 주문서
#define		COMPOSITE_ITEM_EVENT1			2664	// 결합 주문서
#define		COMPOSITE_ITEM_EVENT2			2665	// 결합 주문서
#define		PET_MOUNT_CANCEL_CARD			1519	// 펫 마운트 훈련 취소 카드
#define		UNCOMPOSITE_ITEM				1540	// 결합 분리 주문서
#define		PACKING_PAPER_ITEM				2046	// 포장지 아이템
#define		SMALL_CORD_ITEM					2632	// 복주머니 노끈
// wooss 070305
// kw : WSS_WHITEDAY_2007
#define		BLUE_LETTER_ITEM				2135	// 하늘색 편지지
#define		PINK_LETTER_ITEM				2136	// 핑크색 편지지

// [KH_070315] 프리미엄 메모리스크롤 관련 추가
#define		PRIMIUM_TELEPORT				2052	// [KH_070315] 프리미엄 메모리스크

// [KH_070326] 달걀 이벤트 관련
#define		EASTER_EGGS						2148
// [KH_070413] 스승의날 이벤트 관련 추가
#define		MEDAL_OF_DISCIPLE				2329
#define		MEDAL_OF_TEACHER				2330

// WSS_GUILDMASTER 070511 어버이날 이벤트 상품으로 추가
#define		MEMSCROLL_EX_ITEM_7DAYS		2348		// 메모리 스크롤 확장 카드
#define		WAREHOUSE_EX_ITEM_7DAYS		2347		// 개인창고 확장 카드

#define		PET_NAMECARD_ITEM			2360		// 펫의 명찰

// [070824: Su-won] PET_COLOR_CHANGE
#define		PET_COLOR_RED				2396
#define		PET_COLOR_ORNAGE			2397
#define		PET_COLOR_YELLOW			2398
#define		PET_COLOR_GREEN				2399
#define		PET_COLOR_BLUE				2400
#define		PET_COLOR_INDIGO			2401
#define		PET_COLOR_VIOLET			2402
#define		PET_COLOR_WHITE				2568
#define		PET_COLOR_BLACK				2569
#define		PET_COLOR_PHOENIX			3262
#define		PET_COLOR_BLACK_HEART		3548
#define		PET_COLOR_WHITE_HEART		3549
#define		PET_COLOR_YELLOW_HEART		3550

// [090710: selo] PET_TATOO
#define		PET_TATOO_HORSE				740
#define		PET_TATOO_DRAGON			741


// [071122: Su-won] NEW_MOONSTONE
#define		MOONSTONE_POOR_MIX_BOX				2552
#define		MOONSTONE_USEFUL_MIX_BOX			2553
#define		MOONSTONE_SOLID_MIX_BOX				2554
#define		MOONSTONE_STRONG_MIX_BOX			2555
#define		MOONSTONE_TRUSTY_MIX_BOX			2556

// eons 루돌프 변신 주문서
#define		PET_TRANSFORM_RU			2604
#define		PET_TRANSFORM_RETURN		2605

#define		REBIRTH_ITEM_NEWBIE			2667		// 부활 주문서(초보자용)
#define		REBIRTH_ITEM_PHOENIX		3218		// 피닉스의 부활 아이템

// Type:일회용 - SubType:기타일회용 아이템
enum ITEM_ONCEUSE_ETC
{
	RARE_IDENTITY_ITEM1	=1838,			// 돋보기. 레어아이템 감정. 일정 확률로 감정
	RARE_IDENTITY_ITEM2	=1839,			// 황금 돋보기. 레어아이템 감정. 100% 감정
	GOLD_DISH =2146,					// 태국 송크란 이벤트 금그릇 
	SILVER_DISH =2147,					// 태국 송크란 이벤트 은그릇
	RARE_IDENTITY_ITEM_NEWBIE =2666,	// 초보자용 돋보기.
};

// Character state flags
#define	CSF_EMPTYALL			0x00000000
#define	CSF_SKILL				0x00000001		// 스킬 사용중
#define	CSF_TELEPORT			0x00000002		// 순간 이동중
#define	CSF_PARTY				0x00000004		// 파티 플레이중
#define	CSF_PARTY_REQ			0x00000008		// 파티 요청중
#define	CSF_EXCHANGE			0x00000010		// 교환중
#define	CSF_SHOP				0x00000020		// 상점 거래중
#define CSF_REMISSION			0x00000040		// 면죄부.
#define	CSF_PERSONALSHOP		0x00000080		// 상점 거래중
#define CSF_WAREHOUSE			0x00000100		// 보관함 이용중.
#define CSF_CHANGEWEAPON		0x00000200		// 무기 교체중...
#define CSF_QUIZ				0x00000400		// 퀴즈 맞추는 중.
#define CSF_MOONSTONE			0x00000800		// 문스톤 이벤트 중.
#define CSF_CASTLE_WAR_READY	0x00001000		// 공성 준비중에는 못 움직임...
#define CSF_MOUNT_HUNGRY		0x00002000		// 마운트 상태에서 배가 고픔.
// 공성, 길드 등등 추가 예정

// 이동할 수 없는 상태
#define CSF_CANNOT_MOVE_MASK		( CSF_SKILL | CSF_TELEPORT | CSF_PERSONALSHOP | CSF_MOONSTONE | CSF_CASTLE_WAR_READY | CSF_MOUNT_HUNGRY )
// 순간 이동 할 수 없는 상태
#define	CSF_CANNOT_TELEPORT_MASK	( CSF_SKILL | CSF_TELEPORT | CSF_PARTY_REQ |		\
									  CSF_EXCHANGE | CSF_PERSONALSHOP )					\
// 앉을 수 없는 상태
#define CSF_CANNOT_SITDOWN_MASK		( CSF_SKILL | CSF_TELEPORT | CSF_EXCHANGE )
#define CSF_CANNOT_ATTACK_MASK		( CSF_CASTLE_WAR_READY )


// Valid distance of UI
#define	UI_VALID_SQRDIST			300.0f


// edit by cpp2angel (04.12.20) : 생산 인터페이스에 사용되는 칼라 값
#define COLOR_TITLE		0xe1b300ff	// 타이틀 
#define COLOR_SUB_TITLE	0xff9170ff	// 서브 타이틀 
#define COLOR_TEXT		0xffffffff	// 기본 텍스트에 사용 
#define COLOR_NONE		0x7f7f7fff	// 없는 조건들

// Time 관련 메크로
// Date : 2005-07-07(오후 3:21:10), By Lee Ki-hwan
#define MINUTE_TO_SEC(x)	((x)*60)
#define HOUR_TO_SEC(x)		((x)*60*60)
#define DAY_TO_SEC(x)		((x)*60*60*24)

// Week 관련 메크로
// Date : 2005-07-07(오후 3:21:20), By Lee Ki-hwan
enum EWEEK { SUNDAY, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY, NOTDAY };

// Date : 2005-11-16(오후 7:42:49), By Lee Ki-hwan
// 타격 Type 
#define		HITTYPE_MISS			(1 << 0)		// 회피
#define		HITTYPE_WEAK			(1 << 1)		// 약하게
#define		HITTYPE_NORMAL			(1 << 2)		// 보통
#define		HITTYPE_STRONG			(1 << 3)		// 강하게
#define		HITTYPE_HARD			(1 << 4)		// 매우 강하게
#define		HITTYPE_CRITICAL		(1 << 5)		// 크리티컬
#define		HITTYPE_DEADLY			(1 << 6)		// 죽도록 강하게

// Date : 2005-11-16(오후 1:17:00), By Lee Ki-hwan
typedef struct _tagDAMAGE
{
	SBYTE	sbTargetFlag;	// 타격 종류 
	ULONG	ulDamage;		// 데미지 크기
	ULONG	ulIndex;		// 타격을 준 Index
	FLOAT	fX, fY;			// 출력 
	DOUBLE	dStartTime;
	BOOL	bVisible;
	FLOAT	fSize;
	FLOAT	fAlpha;
	BOOL	bDamaged;
	
	_tagDAMAGE()
	{
		sbTargetFlag	= 0;
		ulDamage		= 0;
		fX = fY			= 0.0f;
		dStartTime		= 0.0f;
		bVisible		= FALSE;
		fSize			= 1.0f;
		fAlpha			= 1.0f;
		bDamaged		= FALSE;
	}			

} DAMAGE;

// wooss 070324 --------------------------------------------------->>
// kw : WSS_HELP_SYSTEM_1
typedef struct _stHELP1
{
	int			m_index;
	int			m_subNum;
	int			m_subEnable;
	int			m_subLevel;
	CTString	m_strName;
	CTString	m_strDesc;
	CTString	m_strFile;
	int			m_uv_x;
	int			m_uv_y;
	int			m_width;
	int			m_height;

} HELP1;

typedef HELP1* pHELP1;
// ----------------------------------------------------------------<<


const int RareItem_Name_Color[5] = { 0x1C54CCFF, 0x20A51CFF, 0xE5E230FF, 0xFFFFFFFF, 0xFFFFFFFF};

// FIXME : 잦은 삽입과 삭제가 있는 부분이므로 list가 저절할듯...
typedef std::vector<DAMAGE>		DAMAGE_deque;

// ----------------------------------------------------------------------------
// Name : CUIManager
// Desc :
// ----------------------------------------------------------------------------
class ENGINE_API CUIManager
{
protected:
	UIGameState		m_ugsGameState;						// Game state for UIs
	CDrawPort		*m_pUIDrawPort;						// Pointer of draw port for rendering UIs
	CUIWindow		*m_apUIs[UI_TYPE_END];				// Pointers of UIs
	int				m_aUIOrder[UI_TYPE_END];			// Order of UIs
	CUIButtonEx		m_btnHoldBtn;						// Holding button
	CUIMouseCursor	*m_pumcMouseCursor;					// Pointer of mouse cursor
	BOOL			m_bShowUIs;							// If UIs are shown or not
	BOOL			m_bRButtonDown;						// Hide mouse cursor if lbutton is down
	BOOL			m_bMouseInsideUIs;					// Show normal mouse cursor if mouse point is inside UIs
	PIX				m_pixMinI, m_pixMinJ;				// Size of drawport
	PIX				m_pixMaxI, m_pixMaxJ;				// Size of drawport
	UIMCType		m_umctTypeInUI;						// Wanted mouse type in UIs

	// Name, chatting & shop popup
	CTextureData	*m_ptdPopupTexture;					// Texture of popup
	CTextureData	*m_ptBlackTexture;					// Texture of popup

	UIRect			m_rcName;							// Region of name
	UIRect			m_rcGuildName;						// Region of name
	UIRect			m_rcGuild;							// Region of guild
	UIRect			m_rcChat;							// Region of chatting
	UIRect			m_rcShop;							// Region of shop

	UIRectUV		m_rtBlack;							// Region of name
	UIRectUV		m_rtNameL;							// UV of name
	UIRectUV		m_rtNameC;
	UIRectUV		m_rtNameR;
	UIRectUV		m_rtNameRPK;
	UIRectUV		m_rtNameRDefPK1;
	UIRectUV		m_rtNameRDefPK2;
	UIRectUV		m_rtChatLU;							// UV of chatting
	UIRectUV		m_rtChatUp;
	UIRectUV		m_rtChatRU;
	UIRectUV		m_rtChatL;
	UIRectUV		m_rtChatC;
	UIRectUV		m_rtChatR;
	UIRectUV		m_rtChatLL;
	UIRectUV		m_rtChatLo;
	UIRectUV		m_rtChatRL;
	UIRectUV		m_rtShopLU;							// UV of personal shop
	UIRectUV		m_rtShopUp;
	UIRectUV		m_rtShopRU;
	UIRectUV		m_rtShopL;
	UIRectUV		m_rtShopC;
	UIRectUV		m_rtShopR;
	UIRectUV		m_rtShopLL;
	UIRectUV		m_rtShopLo;
	UIRectUV		m_rtShopRL;
	UIRectUV		m_rtShopPremLU;						// UV of premium personal shop
	UIRectUV		m_rtShopPremUp;
	UIRectUV		m_rtShopPremRU;
	UIRectUV		m_rtShopPremL;
	UIRectUV		m_rtShopPremC;
	UIRectUV		m_rtShopPremR;
	UIRectUV		m_rtShopPremLL;
	UIRectUV		m_rtShopPremLo;
	UIRectUV		m_rtShopPremRL;

	CStaticArray<CTString>	m_aStringData;				// Array of string data

	DWORD			m_dwCSF;							// Character state flags

	CGame			*m_pGame;
	int				m_LocalVersion;

public :
	// wooss 050822 
	// TEST UI를 위해 추가
	// TEST 환경이 아닐 경우 이부분을 0로 처리 
	// testUI_TYPE
	BOOL			m_testUI_MODE;
	UI_TYPE			m_testUI_TYPE;		//wooss  UI test variable 
	int				m_mPosX,m_mPosY;
	int				m_nHelpNpc_Index;	// NPC 안내시스템 
	// wooss 070324 --------------------------------------------------->><<
	// kw : WSS_HELP_SYSTEM_1
	CStaticArray<HELP1>	m_aHelp1Data;				// Array of string data

	CUIDrawBox		m_bxGuildRankBox;				// WSS_GUILDMASTER 070517	

protected:
	// Internal functions
	void	RenderObjectNamePopup( CProjection3D* pprProjection );
	void	RenderObjectIndexPopup( CProjection3D* pprProjection );
	void	LoadStringData( const CTFileName &fnString );
	// wooss 070324 --------------------------------------------------->>
	// kw : WSS_HELP_SYSTEM_1
	void	LoadHelp1Data( const CTFileName &fnString );
	// ----------------------------------------------------------------<<

	// Command functions
	void	DropItem( int nTab, int nRow, int nCol );

	// Network message functions ( send )
	void	SendDropItem( int nTab, int nRow, int nCol, SQUAD llCount = 1 );

	// WSS_GUILDMASTER 070517 ---------------------------------------->><<
	void	DrawGuildRankBox(UIRect,SBYTE,FLOAT);

public:
	int				m_nShowMessageBox;

	CUIManager();
	~CUIManager();

	// Create & destroy
	void	Create();
	void	Destroy();

	// Render
	void	Render( CDrawPort *pdp, CProjection3D* pprProjection );

	// Set game
	void	SetBackgroundWorld( CWorld *pWorld );
	void	SetGameHandle( CGame *pGame );

	// Get drawport
	CDrawPort	*GetDrawPort() const { return m_pUIDrawPort; }
	PIX		GetMinI() const { return m_pixMinI; }
	PIX		GetMinJ() const { return m_pixMinJ; }
	PIX		GetMaxI() const { return m_pixMaxI; }
	PIX		GetMaxJ() const { return m_pixMaxJ; }

	// String data
	CTString	&GetString( INDEX iIndex )
	{
		ASSERT( iIndex < m_aStringData.Count() );
		return m_aStringData[iIndex];
	}
	CTString	&GetString( INDEX iIndex, INDEX iSrc );
	CTString	&GetString( INDEX iIndex, const char *szSrc );
	CTString	&GetString( INDEX iIndex, const CTString &strSrc );
	CTString	&GetString( INDEX iIndex, const char *szSrc1, const char *szSrc2 );
	CTString	&GetString( INDEX iIndex, const CTString &strSrc1, const CTString &strSrc2 );

	// Game state
	void	SetUIGameState( UIGameState ugsGameState );
	UIGameState	GetUIGameState() const { return m_ugsGameState; }

	// Toggle visible UIs
	void	ToggleVisibleUIs() { m_bShowUIs = !m_bShowUIs; }
	BOOL	IsVisibleUIs()		{ return m_bShowUIs; }

	// Rearrange UIs
	void	RearrangeOrder( int nCurrentUI, BOOL bFrontOrder = TRUE );
	void	ChangeOrder( int nCurrentUI, int nOrder );

	// Adjust position of UIs
	void	ResetUIPos( CDrawPort *pdp );		// Initialize position of UIs
	void	AdjustUIPos( CDrawPort *pdp );		// Adjust position of UIs when main window changes

	// Set & get character state flags
	void	SetCSFlagOn( DWORD dwCSF ) { m_dwCSF |= dwCSF; }
	void	SetCSFlagOff( DWORD dwCSF ) { m_dwCSF &= ~dwCSF; }
	void	ResetCSFlag() { m_dwCSF = 0; }
	DWORD	IsCSFlagOn( DWORD dwCSF ) { return m_dwCSF & dwCSF; }

	// Set & get mouse position state
	void	SetMouseCursorInsideUIs( UIMCType umctType = UMCT_NORMAL )
	{
		m_umctTypeInUI = umctType;
		m_bMouseInsideUIs = TRUE;
	}
	BOOL	IsMouseInsideUIs() const { return m_bMouseInsideUIs; }
	BOOL	IsInsideUpperUIs( INDEX iCurUIIndex, int nX, int nY );

	// Get mouse cursor
	CUIMouseCursor		*GetMouseCursor() { return m_pumcMouseCursor; }

	// Get UI index by order
	int		GetUIIndexByOrder( int nOrder ) { return m_aUIOrder[nOrder]; }

	// Get UIs
	CUIWindow			*GetUI( int nUIIndex ) const { return m_apUIs[nUIIndex]; }
	CUIChatting			*GetChatting() { return (CUIChatting *)m_apUIs[UI_CHATTING]; }
	CUIPlayerInfo		*GetPlayerInfo() { return (CUIPlayerInfo *)m_apUIs[UI_PLAYERINFO]; }
	CUITargetInfo		*GetTargetInfo() { return (CUITargetInfo *)m_apUIs[UI_TARGETINFO]; }
	CUIHelpIcon			*GetHelpIcon() { return (CUIHelpIcon *)m_apUIs[UI_HELP_ICON]; }
	CUIQuickSlot		*GetQuickSlot() { return (CUIQuickSlot *)m_apUIs[UI_QUICKSLOT]; }
	CUIRadar			*GetRadar() { return (CUIRadar *)m_apUIs[UI_RADAR]; }
	CUIInventory		*GetInventory() { return (CUIInventory *)m_apUIs[UI_INVENTORY]; }
	CUICharacterInfo	*GetCharacterInfo() { return (CUICharacterInfo *)m_apUIs[UI_CHARACTERINFO]; }
	CUIExchange			*GetExchange() { return (CUIExchange *)m_apUIs[UI_EXCHANGE]; }
	CUIPortal			*GetPortal() { return (CUIPortal *)m_apUIs[UI_PORTAL]; }
	CUIShop				*GetShop() { return (CUIShop *)m_apUIs[UI_SHOP]; }
	CUIQuiz				*GetQuiz() { return (CUIQuiz *)m_apUIs[UI_QUIZ]; }
	CUIPetTraining		*GetPetTraining() { return (CUIPetTraining *)m_apUIs[UI_PETTRAINING]; }
	CUIPetInfo			*GetPetInfo() { return (CUIPetInfo *)m_apUIs[UI_PETINFO]; }
	CUIGamble			*GetGamble() { return (CUIGamble *)m_apUIs[UI_GAMBLE]; }
	CUIPersonalShop		*GetPersonalShop() { return (CUIPersonalShop *)m_apUIs[UI_PERSONALSHOP]; }
	CUIGuild			*GetGuild() { return (CUIGuild *)m_apUIs[UI_GUILD]; }
	CUIRanking			*GetRanking() { return (CUIRanking *)m_apUIs[UI_RANKING]; }
	CUIPetTargetInfo	*GetPetTargetInfo() { return (CUIPetTargetInfo *)m_apUIs[UI_PETTARGETINFO]; }
	CUIWildPetTargetInfo	*GetWildPetTargetInfo() { return (CUIWildPetTargetInfo *)m_apUIs[UI_WILDPETTARGETINFO]; }
	CUIGuildBattle		*GetGuildBattle() { return (CUIGuildBattle *)m_apUIs[UI_GUILD_BATTLE]; }
	CUIBilling			*GetBilling() { return (CUIBilling *)m_apUIs[UI_BILLING]; }
	CUIBillItem			*GetBillItem() { return (CUIBillItem *)m_apUIs[UI_BILL_ITEM]; }
	CUIWareHouse		*GetWareHouse() { return (CUIWareHouse *)m_apUIs[UI_WAREHOUSE]; }
	CUISecurity			*GetSecurity() { return (CUISecurity *)m_apUIs[UI_SECURITY]; }
	CUISystemMenu		*GetSystemMenu() { return (CUISystemMenu *)m_apUIs[UI_SYSTEMMENU]; }
	CUIParty			*GetParty() { return (CUIParty *)m_apUIs[UI_PARTY]; }
	CUIMap				*GetMap() { return (CUIMap *)m_apUIs[UI_MAP]; }
	CUIOption			*GetOption() { return (CUIOption *)m_apUIs[UI_OPTION]; }
	CUIRefine			*GetRefine() { return (CUIRefine *)m_apUIs[UI_REFINE]; }
	CUIProcess			*GetProcess() { return (CUIProcess *)m_apUIs[UI_PROCESS]; }
	CUIProduct			*GetProduct() { return (CUIProduct *)m_apUIs[UI_PRODUCT]; }
	CUIProcessNPC		*GetProcessNPC() { return (CUIProcessNPC *)m_apUIs[UI_PROCESSNPC]; }
	CUISelectResource	*GetSelectResource() { return (CUISelectResource *)m_apUIs[UI_SELECTRESOURCE]; }
	CUIGuildWarPortal	*GetGuildWarPortal() { return (CUIGuildWarPortal *)m_apUIs[UI_GUILDWARPORTAL]; }
	CUIMix				*GetMix() { return (CUIMix *)m_apUIs[UI_MIX]; }
	CUIMixNew			*GetMixNew() { return (CUIMixNew *)m_apUIs[UI_MIXNEW]; }
	CUICompound			*GetCompound() { return (CUICompound *)m_apUIs[UI_COMPOUND]; }
	CUIGuildStash		*GetGuildStash() { return (CUIGuildStash *)m_apUIs[UI_GUILDSTASH]; }

	CUIRemission		*GetRemission() { return (CUIRemission *)m_apUIs[UI_REMISSION]; }
	CUIQuest			*GetQuest() { return (CUIQuest *)m_apUIs[UI_QUEST]; }
	CUIQuestBook		*GetQuestBookList() { return (CUIQuestBook *)m_apUIs[UI_QUESTBOOK_LIST]; }
	CUIQuestBook		*GetQuestBookContent() { return (CUIQuestBook *)m_apUIs[UI_QUESTBOOK_CONTENT]; }
	CUIQuestBook		*GetQuestBookComplete() { return (CUIQuestBook *)m_apUIs[UI_QUESTBOOK_COMPLETE]; }
	CUIQuestBook		*GetQuestBookNew() { return (CUIQuestBook *)m_apUIs[UI_QUESTBOOK_NEW]; }

	CUISummon			*GetSummonFirst() { return (CUISummon *)m_apUIs[UI_SUMMON_FIRST]; }
	CUISummon			*GetSummonSecond() { return (CUISummon *)m_apUIs[UI_SUMMON_SECOND]; }

	CUISkillLearn		*GetSkillLearn() { return (CUISkillLearn *)m_apUIs[UI_SKILLLEARN]; }
	CUIChangeWeapon		*GetChangeWeapon() { return (CUIChangeWeapon *)m_apUIs[UI_CHANGEWEAPON]; }
	CUIHelper			*GetHelper() { return (CUIHelper *)m_apUIs[UI_HELPER]; }
	CUISingleBattle		*GetSingleBattle() { return (CUISingleBattle *)m_apUIs[UI_SINGLE_BATTLE]; }
	CUIHelp				*GetHelp() { return (CUIHelp *)m_apUIs[UI_HELP]; }
	CUIHelpOld			*GetHelpOld() { return (CUIHelpOld *)m_apUIs[UI_HELP]; }
	CUIInitJob			*GetInitJob() { return (CUIInitJob *)m_apUIs[UI_INITJOB]; }
	CUITeleport			*GetTeleport() { return (CUITeleport *)m_apUIs[UI_TELEPORT]; }
	CUITeleportPrimium	*GetTeleportPrimium() { return (CUITeleportPrimium *)m_apUIs[UI_TELEPORT_PRIMIUM]; }	// [KH_070315] 프리미엄 메모리스크롤 관련 추가
	CUIHelp3			*GetHelp3() { return (CUIHelp3 *)m_apUIs[UI_HELP3]; }
	CUIMessenger		*GetMessenger() { return (CUIMessenger *)m_apUIs[UI_MESSENGER]; }
	CUITalk				*GetTalk( int iUIIndex ) { return (CUITalk *)m_apUIs[iUIIndex]; }
	CUIWebBoard			*GetWebBoard() { return (CUIWebBoard *)m_apUIs[UI_WEBBOARD]; }
	CUINotice			*GetNotice() { return (CUINotice *)m_apUIs[UI_NOTICE]; }
	CUISiegeWarfare		*GetSiegeWarfare() { return (CUISiegeWarfare *)m_apUIs[UI_SIEGE_WARFARE]; }
	CUISiegeWarfareNew	*GetSiegeWarfareNew() { return (CUISiegeWarfareNew *)m_apUIs[UI_SIEGE_WARFARE_NEW]; } // WSS_DRATAN_SIEGEWARFARE 070727
	CUICashShop			*GetCashShop() { return (CUICashShop*)m_apUIs[UI_CASH_SHOP];}				//wooss
	CUISimplePop		*GetSimplePop() { return (CUISimplePop*)m_apUIs[UI_SIMPLE_POP]; }	// [KH_070419] 심플 팝업 관련 추가
	CUIGWMix			*GetGWMix() { return (CUIGWMix *)m_apUIs[UI_GW_MIX]; }
	CUISelectWord		*GetSelectWord() { return (CUISelectWord*)m_apUIs[UI_SELECTWORD]; }
	CUISelectList		*GetSelectList() { return (CUISelectList*)m_apUIs[UI_SELECTLIST]; } // 친구찾기 이벤트 060126
	CUIPetItemMix		*GetPetItemMix() { return (CUIPetItemMix*)m_apUIs[UI_PETITEMMIX]; } // 펫아이템 조합( 유니크 )
	CUIPetFree			*GetPetFree()	{ return (CUIPetFree*)m_apUIs[UI_PETFREE]; } // 펫 봉인 해제
	CUIOXQuizEvent		*GetOXQuizEvent() { return (CUIOXQuizEvent*)m_apUIs[UI_QUIZEVENT]; } // O.X 퀴즈 이벤트
	CUIPartyAuto		*GetPartyAuto() { return (CUIPartyAuto*)m_apUIs[UI_PARTYAUTO]; } // party auto matching
	CUICollectBox		*GetCollectBox() { return (CUICollectBox*)m_apUIs[UI_COLLECTBOX]; } // 곤충 채집상자
	CUIBingoBox			*GetBingoBox() { return (CUIBingoBox*)m_apUIs[UI_BINGOBOX]; } // 초코렛 수집 상자
	CUINpcHelp			*GetNpcHelp() {return (CUINpcHelp*)m_apUIs[UI_NPCHELP];} //Npc 안내시스템
	CUIMinigame			*GetMinigame() {return (CUIMinigame*)m_apUIs[UI_MINIGAME];} //WSS_MINIGAME 070418
	CUIFlowerTree		*GetFlowerTree() {return (CUIFlowerTree*)m_apUIs[UI_FLOWERTREE];} // 꽃놀이 인터페이스
	CUIGuildNotice		*GetGuildNotice() {return (CUIGuildNotice*)m_apUIs[UI_GUILD_NOTICE];}// WSS_NEW_GUILD_SYSTEM 070715 

	CUILogin			*GetLogin() { return (CUILogin *)m_apUIs[UI_LOGIN]; }
	CUISelChar			*GetSelChar() { return (CUISelChar *)m_apUIs[UI_SEL_CHAR]; }
	CUICreateChar		*GetCreateChar() { return (CUICreateChar *)m_apUIs[UI_CREATE_CHAR]; }
	CUISelectServer		*GetSelServer() { return (CUISelectServer *)m_apUIs[UI_SEL_SERVER]; }
	CUIMonsterCombo		*GetCombo() {return(CUIMonsterCombo *)m_apUIs[UI_MONSTER_COMBO]; }
	CUIWildPetInfo		*GetWildPetInfo() {return (CUIWildPetInfo *)m_apUIs[UI_WILDPET_INFO]; }
	CUIAuction			*GetAuction() { return (CUIAuction *)m_apUIs[UI_AUCTION]; }

	CUITatoo			*GetTatoo() { return (CUITatoo *)m_apUIs[UI_TATOO]; }	// [090709: selo] 펫 문양 시스템

	//[ttos_2009_7_17]: CHARATER_CHAT_FILTER 캐릭터 채팅 필터
	CUIChatFilter		*GetChatFilter()	{ return (CUIChatFilter *)m_apUIs[UI_CHAT_FILTER]; }
	CGame				*GetGame() { return m_pGame; }

	// Holding button
	void	SetHoldBtn( CUIButtonEx &btnHolding ) { m_btnHoldBtn.Copy( btnHolding ); }
	void	ResetHoldBtn() { m_btnHoldBtn.InitBtn(); }
	CUIButtonEx	&GetHoldBtn() { return m_btnHoldBtn; }

	// MessageBox
	void	CreateMessageBox( CUIMsgBox_Info &rMsgBoxInfo, int nPosX = -1, int nPosY = -1 );
	void	CloseMessageBox( int nCommandCode );
	BOOL	DoesMessageBoxExist( int nCommandCode );

	void	CreateMessageBoxL( CTString &strTitle, int nWhichUI, int nCommandCode,
								int nPosX = -1, int nPosY = -1 );
	void	AddMessageBoxLString( int nCommandCode, BOOL bDesc, CTString &strMessage, int iValue = -1, const COLOR colDesc = 0xF2F2F2FF, CTString strPrefix = "o"  );
	void	CloseMessageBoxL( int nCommandCode );
	BOOL	DoesMessageBoxLExist( int nCommandCode );
	CUIMessageBox *GetMessageBox( int nCommandCode );

	// WSS_SELECTIVE_UI_FUCNTION 2007/08/15 
	// SELECTIVE UI
	void	CloseSelectiveUI( int nUIidx=-1 );
	BOOL	DoesUIExist( int nUIidx );
	BOOL	IsUIVisible(int nUIidx);


	// Message
	void	MsgProc( MSG *pMsg, BOOL *pbIMEProc );

	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput ,CTString &strConfirm );
	
	//wooss 050805 add message command for select button
	void	MsgBoxBtnCommand( int nCommandCode, int btnNum, CTString &strInput );
	
	void	MsgBoxLCommand( int nCommandCode, int nResult ) {}

	// Misc functions
	void	InsertCommaToString( CTString &strCount );
	COLOR	GetNasColor( SQUAD llNas );
	COLOR	GetNasColor( CTString strNas );
	CTString NasNumToWord( SQUAD llNas);

	void	CancelSkill( BOOL bLostTarget = TRUE, BOOL bSkillError=FALSE);
	void	Reset();
	void	Lock( BOOL bLock );

	void	LostTarget();

	void	SetVersion(int ver){m_LocalVersion = ver;};
	int		GetVersion(){return m_LocalVersion;};
	
	// wooss  060404 
	BOOL	IsPet(CItemData& rItemData);
	BOOL	IsWildPet(CItemData& rItemData);
	// wooss 060421
	struct tm * LClocaltime ( time_t *timp );
	
	BOOL checkName(CTString strMsg, int chkType);

	CTString GetWeekToString( int nWeek )
	{
		if( nWeek < 0 || nWeek > 6 ) return CTString("None ");
		CTString strWeek[7] = { "일" , "월" , "화" , "수" , "목" , "금" , "토" };	// 번역
		return strWeek[nWeek];
	}
	
	void SetTitleName( INDEX bFullScreen, int iScreenSizeI, int iScreenSizeJ );
	static void GetClassOfItem( CItemData & rItemData, CTString &strClass );
	DAMAGE_deque	m_qDamage;	
	UIRectUV		m_rtNumber[10];
	UIRect			m_rcNumber;
	
	UIRectUV		m_rtShowText[3];
	UIRect			m_rcShowText[3];
	
	BOOL			m_bDamageState; 
	
	int				m_nCoolTimeReductionRate;		//스킬쿨타임감소옵션이 붙은 장비를 입고 있을 때 쿨타임 감소비율
	int				m_nNeedMPReductionRate;				//MP소모량 감소 옵션이 붙은 장비를 입고 있을 때 감소비율

	//[070703: Su-won] TARGET_MARK
	std::map<SLONG, CEffectGroup *> m_mapEG;
	
	void AddDamageData( ULONG ulDamage, SBYTE sbTargetFlag, ULONG ulClientIndex, BOOL bDamaged );
	void ShowDamage( ULONG ulClientIndex );
	
	void SetDamageState();	
	void ShowDamageList( FLOAT3D vPopupPos, FLOAT fPopupZ, ULONG ulClientIndex );
	
	void ClearDamageData();	
	
	void DrawNumber( int nX, int nY, int nNumber, COLOR colColor = 0xFFFFFFFF, FLOAT fRatio = 1.0f );
	void DrawText( int nX, int nY, SBYTE sbTargetFlag, COLOR colColor = 0xFFFFFFFF, FLOAT fRatio = 1.0f );

	BOOL IsRareItem(CTString strName);
	void SetRareOption( CNetworkMessage *istr, CItems& rItem );
	
	int  GetCoolTimeReductionRate() { return m_nCoolTimeReductionRate; }
	int  GetNeedMPReductionRate() { return m_nNeedMPReductionRate; }
	void SetCoolTimeReductionRate(int CoolTimeReductionRate) { m_nCoolTimeReductionRate =CoolTimeReductionRate; }
	void SetNeedMPReductionRate(int nNeedMPReductionRate) { m_nNeedMPReductionRate =nNeedMPReductionRate; }
	void AddCoolTimeReductionRate(int Add) { m_nCoolTimeReductionRate += Add; }
	void AddNeedMPReductionRate(int Add) { m_nNeedMPReductionRate += Add; }

	// WSS_MINIGAME_070422 -------------------------------------->>
	// For multilined string
	void AddStringToList(CUIListBox* pListBox,CTString& tStr, INDEX iMaxChar, COLOR tColor =0xFFFFFFFF,INDEX ilistCol=0);

	// ----------------------------------------------------------<<

	//[070703: Su-won] TARGET_MARK		|----->		
	BOOL IsEnemy( void* pTarget, TARGET_TYPE eType, BOOL bSearch =FALSE );
	void StartTargetEffect( SLONG slIndex, CEntity* penEntity, BOOL bEnemy );
	void StopTargetEffect( SLONG slIndex );
	//[070703: Su-won] TARGET_MARK		<-----|
	
	// 원정대, RAID 관련
	BOOL IsPlayInZone();	// 인스턴트존 플레이중인지 체크

protected:
	void RenderDamage( DAMAGE_deque::iterator iter, DOUBLE dDealyTime, int nX, int nY );
	void RenderNotice();
};

ENGINE_API extern CUIManager	*_pUIMgr;



inline ENGINE_API CTString _SFunc( int x, char* );

//#define KEY_SHOW_STRING_INDEX

#ifndef KEY_SHOW_STRING_INDEX
#define _S(x,comment) _pUIMgr->GetString(x)
#else
#define _S(x,comment) _SFunc(x,comment)
#endif
#define _S2(x,str1,comment) _pUIMgr->GetString(x,str1)
#define _S3(x,str1,str2,comment) _pUIMgr->GetString(x,str1,str2)
#define _s(str) CTString(str)

#endif	// UIMANAGER_H_


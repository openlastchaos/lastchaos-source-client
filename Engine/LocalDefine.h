//---------------------------------------------------------------------->>
// Name : LocalDefine.h
// Desc : for localization
// Date : [6/27/2006] , Wooss
//----------------------------------------------------------------------<<

#ifndef	LOCALDEFINE_H_
#define	LOCALDEFINE_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

/***********************************************
//		Malasia Define List

#define NEW_DELETE_CHAR
#define CHARGE_ITEM
#define SUPPORTER_EVENT
#define CASH_GIFT
#define RARE_ITEM
#define PET_TRANSFORM				// [071206: eons] 펫 변신

#define PET_NAMECARD
#define REFINE_PROTECT
#define PET_COLOR_CHANGE

#define NEW_GUILD_SYSTEM
#define HELP_SYSTEM_1

#define DRATAN_SIEGEWARFARE
#define ATTRIBUTEMAP_BLOCK
 
#define MONSTER_COMBO

//		Malasia Define List
***********************************************/


/***********************************************
//		German Define List

#define MONSTER_COMBO
#define DISPLAY_GUILD_RANK
#define DRATAN_SIEGEWARFARE//드라탄 공성전
#define NEW_MOONSTONE				// [071122: Su-won] NEW_MOONSTONE

#define		GER_MODIFY_PARTYCAHT_ABRIDGE_KEY_NA_20081224 
#define		ADD_CHAT_SPECIALCHAR_DISABLE_NA_20090210
#define		ADD_CHAT_CEILWRITING_CUT_NA_20081029
#define		MODIFY_MESSENGER_CHAT_BLANK_NA_20081028 
#define		RESTART_GAME									// 15초후 게임 재시작 요청
#define		PERSONALSHOP_1658_TEXT_SHOW_NA_20081020 
#define		FILTERING_WORD_VISIBLE_NA_20081013
#define		SORCERER_SUMMON_VILLAGE_VISIBLE_NA_20081008

#define RECOMMEND_SERVER_EVENT								// 추천서버 이벤트 
#define DISPLAY_SERVER_INFO
#define CHARGE_ITEM //유료화 여부( 확장 창고 기간 표시, ) - 말레샤		
#define LINE_CHANGE_BY_WORD		
#define PKPENALTY_QUEST			
#define NEW_GUILD_SYSTEM		
#define NEW_DELETE_CHAR		
#define CASH_GIFT			
#define ATTRIBUTEMAP_BLOCK	
#define PET_NAMECARD			
#define REFINE_PROTECT		
#define PET_COLOR_CHANGE		
#define SUPPORTER_EVENT
#define SIMPLE_POPUP			
#define HELP_SYSTEM_1

#define RARE_ITEM
#define Pet_IMPROVEMENT_1ST
//		German Define List
***********************************************/

/***********************************************

// 공동으로 활성화 되어있는 부분 [프랑스, 스페인, 폴란드 공통]

#define		ADD_CHAT_CEILWRITING_CUT_NA_20081029
#define		MODIFY_MESSENGER_CHAT_BLANK_NA_20081028 
#define		RESTART_GAME									// 15초후 게임 재시작 요청
#define		PERSONALSHOP_1658_TEXT_SHOW_NA_20081020 
#define		FILTERING_WORD_VISIBLE_NA_20081013
#define		SORCERER_SUMMON_VILLAGE_VISIBLE_NA_20081008

#define RECOMMEND_SERVER_EVENT								// 추천서버 이벤트 
#define DISPLAY_SERVER_INFO
#define CHARGE_ITEM //유료화 여부( 확장 창고 기간 표시, ) - 말레샤		
#define LINE_CHANGE_BY_WORD		
#define PKPENALTY_QUEST			
#define NEW_GUILD_SYSTEM		
#define NEW_DELETE_CHAR		
#define CASH_GIFT			
#define ATTRIBUTEMAP_BLOCK	
#define PET_NAMECARD			
#define REFINE_PROTECT		
#define PET_COLOR_CHANGE		
#define SUPPORTER_EVENT
#define SIMPLE_POPUP			
#define HELP_SYSTEM_1

#define RARE_ITEM
#define Pet_IMPROVEMENT_1ST
//#define ADD_CHAT_SPECIALCHAR_DISABLE_NA_20090210
// 공동으로 활성화 되어있는 부분 [프랑스, 스페인, 폴란드 공통]
/***********************************************/

/*******************************************************************************/
// USA Define List
//#ifndef FINALVERSION
//#define FINALVERSION
//#endif
#define CHATTING_BAN				//[ttos_2009_1_23]: 채팅 금지
#define	ADD_CHAT_CEILWRITING_CUT_NA_20081029 // [GER] 1분 - 도배글 차단
#define	IME_CHAR_COLOR_CHANGE_NA_20080910	// 채팅창 문자 색상 변경
#define DRATAN_SIEGEWARFARE	// 드라탄 공성
#define NEW_GUILD_SYSTEM	// 뉴길드 시스템
#define CHECK_CHARATER_MOVE	// 캐릭터 이동 수정
//#define DISPLAY_GUILD_RANK	// 길드 랭킹 마크 표시
#define GM_INVISIBLE_MODE	// 운영자 모드 인비지블 모드
#define NEW_WEAPONDROP	// 무기 아이템 드랍형태를 보유한 무기 모델 그대도 드랍되도록 설정
//#define NETCAFE_CAMPAIGN
#define RECOMMEND_SERVER_EVENT	// 추천 서버 이벤트
#define NEW_DELETE_CHAR	// 새로운 캐릭터 삭제 적용
//#define CHARGE_ITEM //유료화 여부( 확장 창고 기간 표시, ) - 말레샤
#define SUPPORTER_EVENT	// 서포터 이펙트 적용
#define CASH_GIFT		//캐쉬아이템 선물하기
#define RARE_ITEM		// 레어 아이템
#define Pet_IMPROVEMENT_1ST		// [070604: Su-won] 1차 펫 기능 개선
#define ATTRIBUTEMAP_BLOCK		// 속성 블럭
//#define SECURITY_CARD			// 홍콩 보안 카드
#define PET_NAMECARD			// 펫 이름 변경 카드
#define REFINE_PROTECT			// 보호 정련
#define PET_COLOR_CHANGE		// 펫 컬러 변경
#define TARGET_MARK				// 타겟 마크 표시
#define  SIMPLE_POPUP			// 심플 팝업
#define HELP_SYSTEM_1			// 도움말 시스템
#define NEW_MOONSTONE				// [071122: Su-won] NEW_MOONSTONE
#define PET_TRANSFORM				// [071206: eons] 펫 변신
#define CASH_PERSON_SHOP			// ttos : 판매대행 상인
//#define PKPENALTY_QUEST
//#define LINE_CHANGE_BY_WORD		//[080114: Su-won] 메시지 박스에서 단어 단위로 라인을 바꿈.
//#define DISPLAY_SERVER_INFO		//[080724: Su-won] 미니맵 아래에 현재 접속중인 서버, 채널 표시
#define WILD_PET_ADD				// ttos_080722 : 공격형 펫 LOD 파일 읽기추가
#define NEW_USER_INTERFACE			// 새로운 UI
#define NEW_USER_INTERFACE_MESSAGE	// 새로운 UI 네트워크 메세지
#define MONSTER_COMBO				// 몬스터 콤보

#define AUCTION_SYSTEM				//거래 대행 시스템
#define CHATTING_BAN				//[ttos_2009_1_23]: 채팅 금지
#define TEXTURE_MSGBOX_CHANGE_NA_20080902	// MSG BOX TEXTURE change
#define LOGIN_SAVEID					// [090715: selo] 아이디 저장하기
#define OPTION_REJECT_PARTY_AND_EXCHANGE	// [090716: selo] 교환/초대 거부
#define CHARATER_CHAT_FILTER		//[ttos_2009_7_17]: 캐릭터 채팅 필터
//#define NEW_QUESTBOOK				// [090526: selo] 확장팩 퀘스트 작업
//#define RAID_SYSTEM				// [sora] 레이드 시스템
//*******************************************************************************/

/***********************************************
//		Brasil Define List
//***********************************************
// 브라질일때
#define LAKA_Presscorp	// 라카 기자단 혜택
#define	IME_CHAR_COLOR_CHANGE_NA_20080910	// 채팅창 문자 색상 변경
#define DRATAN_SIEGEWARFARE	// 드라탄 공성
#define NEW_GUILD_SYSTEM	// 뉴길드 시스템
#define CHECK_CHARATER_MOVE	// 캐릭터 이동 수정
//#define DISPLAY_GUILD_RANK	// 길드 랭킹 마크 표시
#define GM_INVISIBLE_MODE	// 운영자 모드 인비지블 모드
#define NEW_WEAPONDROP	// 무기 아이템 드랍형태를 보유한 무기 모델 그대도 드랍되도록 설정
//#define NETCAFE_CAMPAIGN
#define RECOMMEND_SERVER_EVENT	// 추천 서버 이벤트
#define NEW_DELETE_CHAR	// 새로운 캐릭터 삭제 적용
//#define CHARGE_ITEM //유료화 여부( 확장 창고 기간 표시, ) - 말레샤
#define SUPPORTER_EVENT	// 서포터 이펙트 적용
#define CASH_GIFT		//캐쉬아이템 선물하기
#define RARE_ITEM		// 레어 아이템
#define Pet_IMPROVEMENT_1ST		// [070604: Su-won] 1차 펫 기능 개선
#define ATTRIBUTEMAP_BLOCK		// 속성 블럭
//#define SECURITY_CARD			// 홍콩 보안 카드
#define PET_NAMECARD			// 펫 이름 변경 카드
#define REFINE_PROTECT			// 보호 정련
#define PET_COLOR_CHANGE		// 펫 컬러 변경
#define TARGET_MARK			// 타겟 마크 표시
#define SIMPLE_POPUP			// 심플 팝업
#define HELP_SYSTEM_1			// 도움말 시스템
#define NEW_MOONSTONE				// [071122: Su-won] NEW_MOONSTONE
#define PET_TRANSFORM				// [071206: eons] 펫 변신
#define CASH_PERSON_SHOP			// ttos : 판매대행 상인
//#define PKPENALTY_QUEST
//#define LINE_CHANGE_BY_WORD		//[080114: Su-won] 메시지 박스에서 단어 단위로 라인을 바꿈.
//#define DISPLAY_SERVER_INFO		//[080724: Su-won] 미니맵 아래에 현재 접속중인 서버, 채널 표시
#define WILD_PET_ADD				// ttos_080722 : 공격형 펫 LOD 파일 읽기추가
#define NEW_USER_INTERFACE			// 새로운 UI
#define NEW_USER_INTERFACE_MESSAGE	// 새로운 UI 네트워크 메세지
#define MONSTER_COMBO				// 몬스터 콤보
//		Brasil Define List
//***********************************************/

/*********************************************************************************

// ----na_add---------------------------------<<

// [GER] 1분 - 도배글 차단
//#define		ADD_CHAT_CEILWRITING_CUT_NA_20081029

// [GER] 소서러: 소환수들 마을에서는 안보이게
#define	SORCERER_SUMMON_VILLAGE_VISIBLE_NA_20081008

// IME send name 색상 다르게
#define	IME_CHAR_COLOR_CHANGE_NA_20080910
// -------------------------------------------<<

// WSS_DRATAN_SIEGEWARFARE 070720
#define DRATAN_SIEGEWARFARE

// WSS_NEW_GUILD_SYSTEM 070608
#define NEW_GUILD_SYSTEM

// WSS_WALLMOVE_BUGFIX 070531
// 싱글던전은 체크 제외 시킴(클라이언트 자체 충돌 체크 하므로)
#define CHECK_CHARATER_MOVE

// WSS_GUILDMASTER 070517
// 어버이날 보상용 길드 표시
#define DISPLAY_GUILD_RANK

// WSS_VISIBLE 070511
// 운영자 비저블 모드 세팅
#define GM_INVISIBLE_MODE

// WSS_NEWWEAPONDROP 070508
// 무기 아이템 드랍형태를 보유한 무기 모델 그대도 드랍되도록 설정
#define NEW_WEAPONDROP

// Date : 2006-05-02(오전 10:16:01), By eons
//#define NETCAFE_CAMPAIGN

// Recommend Server Event
#define RECOMMEND_SERVER_EVENT

// New delete character (24hour)
#define NEW_DELETE_CHAR

//#define CHARGE_ITEM //유료화 여부( 확장 창고 기간 표시, ) - 말레샤
// 서포터 이벤트 
#define SUPPORTER_EVENT

#define CASH_GIFT		//캐쉬아이템 선물하기

#define RARE_ITEM

#define Pet_IMPROVEMENT_1ST		// [070604: Su-won] 1차 펫 기능 개선

// [070705: Su-won] Attribute맵 충돌체크
#define ATTRIBUTEMAP_BLOCK

// 070821_ttos : 홍콩 보안카드
//#define SECURITY_CARD

// [070820: Su-won] 펫명찰 아이템
#define PET_NAMECARD

// [070824: Su-won] REFINE_PROTECT
#define REFINE_PROTECT

// [070824: Su-won] PET_COLOR_CHANGE
#define PET_COLOR_CHANGE

// [070704: Su-won] 타겟 선택 표시
//#define TARGET_MARK

// 070907_ttos : 심플 메뉴
#define  SIMPLE_POPUP

// wooss 070401 ------------------------------>>
// WSS_HELP_SYSTEM_1
#define HELP_SYSTEM_1
// -------------------------------------------<<

#define NEW_MOONSTONE				// [071122: Su-won] NEW_MOONSTONE

#define PET_TRANSFORM				// [071206: eons] 펫 변신
#define CASH_PERSON_SHOP			// ttos : 판매대행 상인

//#define PKPENALTY_QUEST

//#define LINE_CHANGE_BY_WORD		//[080114: Su-won] 메시지 박스에서 단어 단위로 라인을 바꿈.

//#define DISPLAY_SERVER_INFO		//[080724: Su-won] 미니맵 아래에 현재 접속중인 서버, 채널 표시

#define WILD_PET_ADD				// ttos_080722 : 공격형 펫 LOD 파일 읽기추가
#define NEW_USER_INTERFACE
#define NEW_USER_INTERFACE_MESSAGE
#define ACCESSORY_EFFECT			//[ttos_2009_2_18]: 악세사리 이펙트 적용
//*******************************************************************************************/
// wooss 070228 ----------------------------------------------------->>
// 이벤트 툴의 event.lod데이터 값을 이용하여 이벤트를 적용한다.
// 추가되는 이벤트는 마지막 리스트에 서버 이벤트 인덱스에 맞게 적용 시킨다.
enum eEVENTALL
{
	TEVENT_START					=	0,	
	TEVENT_TREASUREBOX				=	1,	// 보물 상자 이벤트 
	TEVENT_TEACH					=	2,	// 후견인 이벤트 
	TEVENT_FRUIT					=	3,	// 과일 이벤트 
	TEVENT_CHUSEOK					=	4,	// 추석 이벤트 	
	TEVENT_SEPTEMBER				=	5,	// 9월 이벤트 
	TEVENT_2PAN4PAN1				=	6,	// 2판4판 이벤트 
	TEVENT_2PAN4PAN2				=	7,	// 2판4판 이벤트 
	TEVENT_PEPERO					=	8,	// 빼빼로 이벤트
	TEVENT_XMAS						=	9,	// 크리스마스 이벤트
	TEVENT_NEWYEAR					=	10, // 신년 이벤트
	TEVENT_CHANGE_ARMOR				=	11,	// 무기/방어구 교환 이벤트
	TEVENT_SEARCHFRIEND				=	12,	// 친구 찾기 이벤트
	TEVENT_VALENTINE_2006			=	13, // 발렌타인 이벤트
	TEVENT_WHITEDAY					=	14, // 화이트 데이 이벤트
	TEVENT_OLD_SERVER				=	15,	// 업데이트 기념 이벤트
	TEVENT_NEW_SERVER				=	16,	// 업데이트 기념 이벤트
	TEVENT_SAKURA					=	17, // 사쿠라 이벤트
	TEVENT_OX_QUIZ					=	18, // OX 퀴즈 이벤트
	TEVENT_WORLDCUP					=	19, // 월드컵 이벤트
	TEVENT_RAIN						=	20, // 빗방울 이벤트
	TEVENT_BUDDHIST					=	21,	// 태국 불교 이벤트
	TEVENT_COLLECT_BUG				=	22,	// 곤충 채집 이벤트
	TEVENT_NEWSERVER_BASTARD		=	23,	// 신섭 바스타드 오픈 이벤트: 신섭
	TEVENT_NEWSERVER_BASTARD_OLD	=	24,	// 신섭 바스타드 오픈 이벤트: 구섭
	TEVENT_CHUSEOK_2006				=	25, // 추석 이벤트
	TEVENT_HALLOWEEN				=	26, // 할로윈 이벤트
	TEVENT_XMAS_2006				=   27,	// 2006 크리스마스 이벤트
	TEVENT_CHILDREN_DAY				=	28,	// 태국 Children Day 이벤트 후견인 이벤트
	TEVENT_VALENTINE_2007			=	29, // 발렌타인데이 이벤트
	TEVENT_JPN_2007_NEWSERVER		=	30, // 일본 2007년 3월 신규 서버 오픈 접속자 이벤트
	TEVENT_WHITEDAY2007				=	31, // 화이트 데이 2007
	TEVENT_SONGKRAN					=	32, // 태국 송크란 이벤트
	TEVENT_PARENTS_2007				=	34, // 어버이날 이벤트 	
	TEVENT_GOMDOLI				    =	35, // 곰돌이 이벤트
	TEVENT_CHILDREN_2007		    =	36, // 어린이날 이벤트
	TEVENT_FLOWERTREE			    =	37, // 꽃놀이 이벤트 
	TEVENT_TEACHER_2007				=   38, // 스승의날 이벤트
	TEVENT_UCC_2007					=	39, // UCC 컨테스트 이벤트
	TEVENT_INDEPENDENCEDAY			=	40, // 미국 독립기념일 이벤트
	TEVENT_SUMMER_2007				=	41, // 2007 여름 종이접기 이벤트
	TEVENT_MOTHER					=	42, // 2007 태국 어머니날 이벤트
	
	TEVENT_ADULT_OPEN				=	44, // 성인 서버 오픈 이벤트('매직카드를 찾아라', '전직달성! 절망의부적을 받아라!')
	TEVENT_BJMONO_2007				=	45, // SK BJ Mono 이벤트
	TEVENT_TG2007_1000DAYS			=	46, // 라스트 카오스 천일 이벤트
	TEVENT_TG2007_FRUITFULL			=	47, // 풍년 이벤트
	TEVENT_TG2007_SCREENSHOT		=	48, // 스크린샷 이벤트
	TEVENT_HOLLOWEEN2007			=	49,	// 할로윈 이벤트(2007)
	TEVENT_XMAS_2007				=	50, // 크리스마스 이벤트(2007)
	TEVENT_NEWYEAR_2008				=	51, // 2008 신년 이벤트
	TEVENT_LUNARNEWYEAR_2008		=	52,	// 2008 설날 이벤트	
	TEVENT_RED_TREASUREBOX			=	53, // 붉은색 보물상자 이벤트
	TEVENT_SAKURA_2008				=	54, // 2008 벚꽃 이벤트
	TEVENT_CHAOSBALL				=	55, // 카오스볼 이벤트

	TEVENT_END								
	
};

extern ENGINE_API std::map<int,int> g_mapEvent;

// 이벤트가 설정되어 있다면 event value값을 리턴하고 아니면 FALSE(0)리턴...
#define IS_EVENT_ON(x) ((g_mapEvent.find(x)==g_mapEvent.end())?0:g_mapEvent[x])
// ------------------------------------------------------------------<<

// WSS_NPROTECT 070402 ------------------------------->>
#define NO_GAMEGUARD
// ---------------------------------------------------<<

#endif
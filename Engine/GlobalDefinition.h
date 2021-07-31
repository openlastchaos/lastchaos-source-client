// ----------------------------------------------------------------------------
//  File : GlobalDefinition.h
//  Desc : Created by Dongmin
// ----------------------------------------------------------------------------

#ifndef	GLOBALDEFINITION_H_
#define	GLOBALDEFINITION_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Base/FileName.h>

//0707
#define COMPRESSION_MESSAGE
#define SECRET_KEY

// EDIT : BS : 070413 : 신규 패킷 암호화
// EDIT : BS : 패킷 암호화
#define CRYPT_NET_MSG
//#define CRYPT_NET_MSG_MANUAL	// 패킷 암호화 키 변경을 수동으로 함에 따라 이전 키로 복호를 시도할 지 여부, DES3 이후는 무조건 자동이므로 주석처리 해야함
// 암호화 lib 링크
#ifdef NDEBUG
#pragma comment(lib, "LCCrypt.lib")
#else
#pragma comment(lib, "LCCrypt_mtd.lib")
#endif


#define START_WORLD		("Data\\world\\StartZone\\startzone.wld")  
#define LOGIN_WORLD		("Data\\World\\LoginWorld\\LoginWorld.wld")

//---------------------------------------------------------------------
#define CLASS_ENEMY		("Classes\\enemy.ecl")
#define CLASS_ITEM		("Classes\\Items.ecl")
#define CLASS_PET		("Classes\\Pet.ecl")
#define CLASS_SUMMON	("Classes\\Slave.ecl")
#define CLASS_WILDPET	("Classes\\WildPet.ecl")

#define LORD_SYMBOL_INDEX			225
#define DRATAN_LORD_SYMBOL_INDEX	401			// WSS_DRATAN_SEIGEWARFARE 2007/07/30 교감 크리스탈
#define DRATAN_MASTER_TOWER_INDEX	351			// WSS_DRATAN_SEIGEWARFARE 2007/07/30 마스터 타워
#define MOB_FLOWERTREE_INDEX		342
#define MOB_XMAS_TREE_DECO			486			// 크리스마스 장식용
#define MOB_XMAS_TREE_CENTER		484			// 크리스마스 광장용


//---------------------------------------------------------------------
// 서버에서 오는 메세지 처리에서만 사용
#define		PLAYER_STATE_SUPPORTER		(1 << 9)	// 게임 서포터 플래그 (특정 아이템 착용시 이펙트)
#define		PLAYER_STATE_OTHERZONE		(1 << 10)	// 다른 속성의 마을에 있을때
#define		PLAYER_STATE_CRISTAL_RESPOND	(1 << 11)	// 드라탄 공성 크리스탈 교감중 // WSS_DRATAN_SEIGEWARFARE 2007/07/31
#define		HAIR_RED_CAP				10 // 빨간 산타 모자
#define		HAIR_GREEN_CAP				20 // 녹색 산타 모자 
//--------------------enum zone name begin()--------------------
// zone no  = // 마을 이름
enum eZoneName
{
	F_JUNO,		// 0 쥬노마을
	D_BELFIST,	// 1 벨피스트 신전
	PD_STRAIA,	// 2 스트레이아 신전( 퍼스널 )
	P_PROCYON,	// 3 프로키온 신전
	F_DRATAN,	// 4 드라탄 필드
	PD_MINOC,	// 5 미노크 탄광( 퍼스널 )
	T_STRAIA,	// 6 스트레이아 신전 ( 튜토리얼 )
	F_MERAC,	// 7 메라크 필드
	GUILD_ROOM,	// 8 길드 전용방
	P_MARGADUM,	// 9 마르가듐 지하 감옥
	PD_AJAKA,	// 10 아자카 협곡( 퍼스널 )
	PD_GATEofDIMENSION,	// 11 차원의 문 ( 퍼스널 )
	P_TEMPLEofFORGETFULNESS, // 12 망각의 신전
	PKZONE_MARGADUM,	// 13 마르가듐 PK존( 태국 )
	OXEVENT_ROOM,		// 14 OX 이벤트 홀
	F_EGEHA				// 15 에게하 필드
};
//--------------------enum zone name end()----------------------

// Date : 2005-05-26(오전 10:40:39), By Lee Ki-hwan
// 접속 상태 (메신저에서 사용)
enum eCondition
{
	OFFLINE,		// 접속안함		
	ONLINE,			// 접속중
	LEFT_POSITION,	// 자리비움
	TOTAL_CONDITION,
};

//---------------------------------------------------------------------
// 직업 목록.
enum eJob
{
	TITAN		= 0,	// 타이탄
	KNIGHT		= 1,	// 기사		
	HEALER		= 2,	// 힐러
	MAGE		= 3,	// 메이지
	ROGUE		= 4,	// 로그
	SORCERER	= 5,	// 소서러
	TOTAL_JOB	= 6,	// 총 직업의 갯수.
};

// Date : 2005-09-23(오후 5:24:56), By Lee Ki-hwan
// 애완동물은 직업은 아닌데 스킬 직업중에서 골라야한다...
#define PET_JOB				6
#define WILDPET_JOB			7

// FIXME : 아직 필요 없음...
// 전직 후 직업
enum eJob2
{
	JOB_2ND_HIGHLANDER		= 1,		// 하이랜더
	JOB_2ND_WARMASTER		= 2,		// 워마스터
	JOB_2ND_ROYALKNIGHT		= 1,		// 로열나이트
	JOB_2ND_TEMPLEKNIGHT	= 2,		// 템플나이트
	JOB_2ND_ARCHER			= 1,		// 아처
	JOB_2ND_CLERIC			= 2,		// 클레릭
	JOB_2ND_WIZARD			= 1,		// 위자드
	JOB_2ND_WITCH			= 2,		// 위치
	JOB_2ND_ASSASSIN		= 1,		// 머쎄씬
	JOB_2ND_RANGER			= 2,		// 레인저

	JOB_2ND_WILDPET_HUMAN	= 1,		// 공격 펫 휴먼
	JOB_2ND_WILDPET_BEAST	= 2,		// 공격 펫 야수
	/*
	JOB_2ND_HORSE			= 1,
	JOB_2ND_DRAGON			= 2,
	JOB_2ND_HORSE_M			= 3,
	JOB_2ND_DRAGON_M		= 4,
	*/
};

#define TOTAL_HEAD	(3)
#define TOTAL_HAIR	(3)

// Job Bit Mask
#define MASK_TITAN		(1 << TITAN)
#define MASK_MAGE		(1 << MAGE)
#define	MASK_HEALER		(1 << HEALER)
#define MASK_KNIGHT		(1 << KNIGHT)
#define MASK_ROGUE		(1 << ROGUE)
#define MASK_SOCERER	(1 << SORCERER)

//---------------------------------------------------------------------
// Equipment
enum eEquipment
{
	WEAR_HELMET					= 0,
	WEAR_JACKET					= 1,
	WEAR_WEAPON					= 2,
	WEAR_PANTS					= 3,
	WEAR_SHIELD					= 4,
	WEAR_GLOVES					= 5,
	WEAR_BOOTS					= 6,
	WEAR_ACCESSORY1				= 7,
	WEAR_ACCESSORY2				= 8,
	WEAR_ACCESSORY3				= 9,
	WEAR_ACCESSORY4				= 10,
	WEAR_PET					= WEAR_ACCESSORY4,
	WEAR_ACCESSORY_ANYWHERE		= 11,	// 악세서리 관련 기획이 변경되어 추가됨. 악세서리의 경우 WearType에 이 값을
	WEAR_TOTAL					= 11,	// 보내면 서버에서는 빈 악세서리 슬롯에 악세서리를 착용한다.
};

enum eWildPetWear
{
	WILDPET_WEAR_HEAD	= 0,
	WILDPET_WEAR_BODY	= 1,
	WILDPET_WEAR_WEAPON = 2,
	WILDPET_WEAR_ACCE	= 3,
	WILDPET_WEAR_TOTAL	= 4,
};

#define	INVEN_SLOT_TAB				3
#define	INVEN_SLOT_COL				5
#define	INVEN_SLOT_ROW				5
#define	INVEN_SLOT_ROW_TOTAL		20

#define PET_TYPE_HORSE		(0x10)		// 말
#define PET_TYPE_DRAGON		(0x20)		// 용

#define PET_GRADE_CHILD		(0X01)		// 유년기
#define PET_GRADE_ADULT		(0X02)		// 성숙기
#define PET_GRADE_MOUNT		(0X03)		// 탈것

//---------------------------------------------------------------------
// Zone Bit Mask
#define	ZONE0		(1 << 0)
#define	ZONE1		(1 << 1)
#define	ZONE2		(1 << 2)
#define	ZONE3		(1 << 3)
#define	ZONE4		(1 << 4)
#define	ZONE5		(1 << 5)
#define	ZONE6		(1 << 6)
#define	ZONE7		(1 << 7)
#define	ZONE8		(1 << 8)
#define	ZONE9		(1 << 9)
#define ZONEALL		(0x3FF)
 
#define	PC_STATE_STANDING		0		// PC 상태 : 서있음
#define	PC_STATE_MOVING			1		// PC 상태 : 이동중
#define	PC_STATE_SITDOWN		2		// PC 상태 : 앉음
#define	PC_STATE_WARP			3		// PC 상태 : 귀환중

// Define country
#define	KOREA					0		// 한국
#define	TAIWAN					1		// 대만
#define	CHINA					2		// 중국
#define THAILAND				3		// 태국
#define TAIWAN2					4		// 대만 천하대란 wooss 050929
#define JAPAN					5		// 일본 wooss 051123
#define MALAYSIA    			6
#define USA						7		// 미국
#define BRAZIL					8		// 브라질
#define HONGKONG				9		// 홍콩
#define GERMANY					10		// 독일
#define SPAIN					11		// 스페인
#define FRANCE					12		// 프랑스		FRANCE_SPAIN_CLOSEBETA_NA_20081124
#define POLAND					13		// 폴란드
#define RUSSIA					14		// 러시아
#define TURKEY					15		// 터키

// 전체 화면 모드 구분
#define OLD_WINDOW_MODE		0
#define FULLSCREEN_MODE		1
#define TAIWAN_WINDOW_MODE	2

#define IsFullScreen(X)	( (X==FULLSCREEN_MODE)?TRUE:FALSE )

// Add LC time 060421 wooss
#define _LC_DAY_IN_MONTH		(30L)
#define _LC_DAY_SEC				(24L * 60L * 60L)    /* secs in a day */
#define _LC_YEAR_SEC			(360L * _LC_DAY_SEC)    /* secs in a year */
#define _LC_MONTH_SEC			(_LC_DAY_IN_MONTH * _LC_DAY_SEC)    /* secs in a year */

#define _LC_FOUR_YEAR_SEC		(1440L * _LC_DAY_SEC)   /* secs in a 4 year interval */
#define _LC_BASE_DOW			4                    /* 01-01-70 was a Thursday */

// weapon lease def
#define LEASE_MIN_LEVEL 37
#define LEASE_MAX_LEVEL 65

#define HEAD_CHANGE	//일본 헬멧 착용 시스템

// WSS_WALLMOVE_BUGFIX 070531---->><<
#define MAX_MOVE_SLOPE_GRADE	45.0f

#endif
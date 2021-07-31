#ifndef INCL_ITEMDATA_H
#define INCL_ITEMDATA_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <vector>
#include <Engine/Base/CTString.h>

#define		MAX_MAKE_ITEM_MATERIAL		10	// 아이템 제작시 최대 재료 수

// Item Proto flag
#define		ITEM_FLAG_COUNT		(1 << 0)	// 셀 수 있는 아이템인가
#define		ITEM_FLAG_DROP		(1 << 1)	// 드롭  가능 아이템인가가
#define		ITEM_FLAG_UPGRADE	(1 << 2)	// 업그레이드 가능한 아이템인가
#define		ITEM_FLAG_EXCHANGE	(1 << 3)	// 교환 가능 아이템인가가 (유저간)
#define		ITEM_FLAG_TRADE		(1 << 4)	// 매매 가능 아이템인가가
#define		ITEM_FLAG_BORKEN	(1 << 5)	// 파괴 가능 아이템인가가
#define		ITEM_FLAG_MADE		(1 << 6)	// 생산 가능 아이템인가가
#define		ITEM_FLAG_MIX		(1 << 7)	// 조합 아이템 : DB에 저장안됨
#define		ITEM_FLAG_CASH		(1 << 8)	// 유료 아이템
#define		ITEM_FLAG_LORD		(1 << 9)	// 성주 전용 아이템
#define		ITEM_FLAG_NO_STASH	(1 << 10)	// 창고 보관 불가능 아이템
#define		ITEM_FLAG_CHANGE    (1 << 11)	// 다른 직업용 아이템으로 교환 가능 여부 //wooss 051217
#define		ITEM_FLAG_COMPOSITE	(1 << 12)	// 아이템 합성 가능 여부
#define		ITEM_FLAG_CASHMOON	(1 << 13)	// 캐쉬 문스톤 룰렛 가능 여부 
#define		ITEM_FLAG_LENT		(1 << 14)	// 대여용 무기
#define		ITEM_FLAG_RARE		(1 << 15)	// 레어 아이템
#define		ITEM_FLAG_ABS		(1 << 16)	// 아이템의 남은 시간 존재 여부
#define		ITEM_FLAG_GENERNAL	(1 << 17)	// 아이템 성향
#define		ITEM_FLAG_CAO		(1 << 18)	// 카오 헌터 아이템
#define     ITEM_FLAG_ORIGIN	(1 << 19)	// 오리지널 옵션(SET_ITEM_ADD [ttos_2009_5_22]: 세트 아이템 적용)
#define		ITEM_FLAG_TRIGGER	(1 << 20)	// 트리거 [090707: selo]
#define		ITEM_FLAG_RAIDSPE	(1 << 21)	// 레이드 스페셜 아이템 [090707: selo]
#define		ITEM_FLAG_QUEST		(1 << 22)	// 퀘스트 아이템 [090616: selo]

	
/*
#define		BLOOD_ITEM			(1 << 7)	// 아이템 플래그.
#define		SEALED_ITEM			(1 << 8)	// 봉인된 아이템.
#define		ITEM_FLAG_SUPER_STONE_USED		(1 << 9)	// 슈퍼고제로 업그레이드 했음..//0627
#define		ITEM_FLAG_BOOSTER	(1 << 10)	// 부스터가 발라진 아이템인가?
*/

// Item Flag : 최상위 1비트는 사용 불가!!! 
// 6비트까지 케르, 넨, 일반 제련석의 레벨과, 플레티늄 제련석을 바른 아이템의 레벨로 사용 
#define		PLATINUM_MAX_PLUS 127 
#define		FLAG_ITEM_PLATINUM_GET(a, b) (b = a & PLATINUM_MAX_PLUS) 
#define		FLAG_ITEM_PLATINUM_SET(a, b) (a = ( (a &~ PLATINUM_MAX_PLUS) | b )) 
#define		FLAG_ITEM_OPTION_ENABLE			(1 << 7)	// 아이템 플래그에 옵션을 붙일 수 있는 상태를 구분, Bit 연산
#define		FLAG_ITEM_SEALED				(1 << 8)	// 아이템 봉인
#define		FLAG_ITEM_SUPER_STONE_USED		(1 << 9)	// 슈퍼고제로 업그레이드 했음..//0627
#define		FLAG_ITEM_BOOSTER_ADDED			(1 << 10)	// 부스터 장착 여부
#define		FLAG_ITEM_SILVERBOOSTER_ADDED	(1 << 11)	// 실버 부스터 장착 여부
#define		FLAG_ITEM_GOLDBOOSTER_ADDED		(1 << 12)	// 골드 부스터 장착 여부
#define		FLAG_ITEM_PLATINUMBOOSTER_ADDED	(1 << 13)	// 플래티늄 부스터 장착 여부
#define		FLAG_ITEM_COMPOSITION			(1 << 14)	// 합성 아이템 적용 여부
#define		FLAG_ITEM_LENT					(1 << 15)	// 대여된 아이템
#define		FLAG_ITEM_LEVELDOWN				(1 << 16)	// 레벨 다운된 아이템

struct ItemSmcParseInfo 
{
	INDEX	iItemIndex;
	INDEX	iCount;
	INDEX	iTexCnt[3];
	CTString BmFileName[3];
	CTString ItemTEXFileName[3];
	CTString ItemTEXNormalFileName[3];

	ItemSmcParseInfo() : iItemIndex(0),iCount(0) 
	{
		iTexCnt[0] = 0; iTexCnt[1] = 0; iTexCnt[2];
	}
};

struct sPetItem_Info 
{
	//공격 펫 아이템
	CTString	pet_name;
	INDEX		pet_index;
	INDEX		pet_level;
	INDEX		pet_str;
	INDEX		pet_con;
	INDEX		pet_dex;
	INDEX		pet_int;
};

class ENGINE_API CItemData
{
private:
	// ITEM 툴에서 사용하고 있는 구조체
	// CItemData의 내부에서만 사용됩니다.
	struct _ItemStat
	{
		int		index;			// 아이템 고유 INDEX
		char	name[50];		// 아이템 이름.
		int		job;			// 직업.	
		int		weight;			// 무게.
		int		fame;			// 명성치.
		int		level;			// 레벨
		int		flag;			// 플래그~
		int		wearing;		// 착용 위치.
		int		type;			// 타입
		int		subType;		// 서브 타입
		char	descr[255];		// 설명.

		int		needItemIndex[MAX_MAKE_ITEM_MATERIAL];
		int		needItemCount[MAX_MAKE_ITEM_MATERIAL];

		int		needSSkillIndex;
		int		needSSkillCount;
		int		needSSkillIndex2;
		int		needSSkillCount2;

		union
		{
			int	num0;
			int pAttack;		// 물리 공격력
			int	pDefense;		// 물리 방어력
			int sec;			// 초(s)(마다) 즉효(-1)
			int skillType;		// 보조.
			int	warpType;		// 워프 타입
			int questnum;		// 퀘스트 번호.
			int processtype;	// 가공 타입
			int optiontype;		// 옵션 타입
			int	producttype;	// 제조 타입
			int boxtype;		// Box Type // Date : 2005-01-12,   By Lee Ki-hwan
			int	refinetype;		// Refine Type // Date : 2005-01-12,   By Lee Ki-hwan
			int pettype;		// 애완동물 종류.
		};

		union
		{
			int num1;
			int	mAttack;		// 마법 공격력
			int	mDffense;		// 마법 방어력
			int recoverHP;		// HP 회복...
			int skillLevel;		// 보조
			int zonenum;		// 월드 번호.
			int processsubtype;	// 가공 서브 타입.
			int	productsubtype;	// 제조 타입
		};

		union
		{
			int num2;
			int attackSpeed;	// 공격 속도.
			int recoverMP;		// MP 회복
			int extranum;		// Extra 번호.
			int sskill;			// 특수 스킬
			int grade;			// 등급
		};

		union
		{
			int num3;
			int count;			// 포션 제작 갯수 
		};

		int		price;
		char	fileSMC[255];
		char	ArmorEffectName[255];

		int		iIconTexID;			// 아이콘 텍스쳐 ID
		int		iIconTexRow;		// 아이콘의 ROW상의 위치(최대 8)
		int		iIconTexCol;		// 아이콘의 COL상의 위치(최대 16)

		// [090608: selo] 퀘스트 사용제한 아이템 관련
		union
		{
			int		PetAI_Set0;
			int		restrictionZoneNo;	// 사용제한 존 번호
		};
		
		union
		{
			int		PetAI_Set1;
			int		restrictionPosX;	// 사용제한 좌표 X
		};
		
		union
		{
			int		PetAI_Set2;
			int		restrictionPosZ;	// 사용제한 좌표 Z			
		};
		
		union
		{
			int		PetAI_Set3;
			int		restrictionPosY;	// 사용제한 좌표 Y			
		};
		
		union
		{
			int		SetItem_Index;		//SET_ITEM_ADD				//[ttos_2009_5_22]: 세트 아이템 적용
			int		restrictionRadius;	// 사용제한 반경
		};

	};	

	_ItemStat	Item_Data;


	//0601 kwon 추가.
	char	fileBm[255];
	char	fileTex[255];
	char	fileTexNormal[255];
	char	fileBm2[255];
	char	fileTex2[255];
	char	fileTex2Normal[255];
	char	fileBm3[255];
	char	fileTex3[255];
	char	fileTex3Normal[255];
	int		Item_MeshCnt;
	int		Item_TexCnt;
	int		Item_Tex2Cnt;
	int		Item_Tex3Cnt;
	BOOL	bCreateSet;
	
public:
	DOUBLE	StartTime;

	enum ITEMTYPE
	{
		ITEM_WEAPON			= 0,	// 무기
		ITEM_SHIELD			= 1,	// 방어구
		ITEM_ONCEUSE		= 2,	// 일회용
		ITEM_BULLET			= 3,	// 탄환
		ITEM_ETC			= 4,	// 기타
		ITEM_ACCESSORY		= 5,	// 악세서리
		ITEM_POTION			= 6,	// 포션			// Date : 2005-01-07,   By Lee Ki-hwan
	};

	enum ITEMWEAPONTYPE
	{
		ITEM_WEAPON_KNIFE	= 0,	// 기사도(나이트)
		ITEM_WEAPON_CROSSBOW= 1,	// 석궁(로그)
		ITEM_WEAPON_STAFF	= 2,	// 스태프(메이지)
		ITEM_WEAPON_BIGSWORD= 3,	// 대검(타이탄)
		ITEM_WEAPON_AXE		= 4,	// 도끼(타이탄)
		ITEM_WEAPON_SSTAFF	= 5,	// 숏스태프(메이지)
		ITEM_WEAPON_BOW		= 6,	// 활(힐러)
		ITEM_WEAPON_DAGGER	= 7,	// 단검(로그)
		ITEM_WEAPON_MINING	= 8,	// 채굴도구
		ITEM_WEAPON_GATHERING= 9,	// 채집도구
		ITEM_WEAPON_CHARGE	= 10,	// 차지도구
		ITEM_WEAPON_TWOSWORD= 11,	// 이도류(나이트)
		ITEM_WEAPON_WAND	= 12,	// 완드(힐러)
		ITEM_WEAPON_SCYTHE	= 13,	// 사이드
		ITEM_WEAPON_POLEARM	= 14,	// 폴암.		
	};

	// 탄환 종류
	enum ITEMBULLETTYPE
	{
		ITEM_BULLET_ATTACK	= 0,	// Attack Bullet
		ITEM_BULLET_MANA	= 1,	// Mana Bullet
		ITEM_BULLET_ARROW	= 2,	// Arrow
	};

	// 기타 타입
	enum ITEMETCTYPE
	{
		ITEM_ETC_QUEST		= 0,	// 퀘스트
		ITEM_ETC_EVENT		= 1,	// 이벤트
		ITEM_ETC_SKILL		= 2,	// 스킬 습득
		ITEM_ETC_REFINE		= 3,	// 제련
		ITEM_ETC_MATERIAL	= 4,	// 재료.
		ITEM_ETC_MONEY		= 5,	// 돈
		ITEM_ETC_PRODUCT	= 6,	// 생산품
		ITEM_ETC_PROCESS	= 7,	// 가공품
		ITEM_ETC_OPTION		= 8,	// 옵션 아이템
		ITEM_ETC_SAMPLE		= 9,	// 시료
		// 10번은 Texture보이기 위해서 사용.
		IETC_MIX_TYPE1		= 11, // 공성조합1
		IETC_MIX_TYPE2		= 12, // 공성조합2
		IETC_MIX_TYPE3		= 13, // 공성조합3
		ITEM_PET_AI			= 14,	// 펫 AI
	};

	// 옵션 아이템 종류.
	enum ITEMOPTIONTYPE
	{
		ITEM_OPTION_BLOOD	= 0,	// Blood Item
		ITEM_OPTION_CLEAR	= 1,	// Clear Item
	};

	// 방어구 종류.
	enum ITEMSHIELDTYPE
	{
		ITEM_SHIELD_HEAD	= 0,	// 머리
		ITEM_SHIELD_COAT	= 1,	// 상의
		ITEM_SHIELD_PANTS	= 2,	// 하의
		ITEM_SHIELD_GLOVE	= 3,	// 장갑
		ITEM_SHIELD_SHOES	= 4,	// 신발
		ITEM_SHIELD_SHIELD	= 5,	// 방패		
	};

	// 착용 위치.
	enum ITEMWEARINGPOS
	{
		ITEM_WEAR_NONE		= -1,	// 미착용
		ITEM_WEAR_HEAD		= 0,	// 투구
		ITEM_WEAR_COAT		= 1,	// 상의		
		ITEM_WEAR_WEAPON	= 2,	// 무기
		ITEM_WEAR_PANTS		= 3,	// 하의
		ITEM_WEAR_SHIELD	= 4,	// 방패
		ITEM_WEAR_GLOVE		= 5,	// 장갑
		ITEM_WEAR_SHOES		= 6,	// 신발
		ITEM_WEAR_ACCESSORY1= 7,	// 액서서리1
		ITEM_WEAR_ACCESSORY2= 8,	// 액서서리2
		ITEM_WEAR_ACCESSORY3= 9,	// 액서서리3
		ITEM_WEAR_PET		= 10,	// 펫 착용
	};

	// 일회용
	enum ITEMSUBTYPE		// Date : 2005-01-07,   By Lee Ki-hwan : 포션 수정
	{
		ITEM_SUB_WARP			= 0,// 워프
	
		// 생산 시스템
		ITEM_SUB_PROCESS_DOC		= 1,	// 가공 문서
		ITEM_SUB_MAKE_TYPE_DOC		= 2,	// 제작 종류, 레벨 문서
		ITEM_SUB_BOX				= 3,	// 상자
		ITEM_SUB_MAKE_POTION_DOC	= 4,	// 포션 제작 문서 // Date : 2005-01-07,   By Lee Ki-hwan
		ITEM_SUB_CHANGE_DOC			= 5,
		ITEM_SUB_QUEST_SCROLL		= 6,
		ITEM_SUB_CASH_ITEM			= 7,	// 일회용 캐쉬아이템
		ITEM_SUB_ETC				= 9,
		ITEM_SUB_TARGET				= 10,	// 타겟 아이템
	};

	enum ACCESSORYTYPE
	{
		ACCESSORY_CHARM			= 0,	// 부적
		ACCESSORY_MAGICSTONE	= 1,	// 마련석
		ACCESSORY_LIGHTSTONE	= 2,	// 반짝이는돌
		ACCESSORY_EARING		= 3,	// 귀걸이.
		ACCESSORY_RING			= 4,	// 반지
		ACCESSORY_NECKLACE		= 5,	// 목걸이
		ACCESSORY_PET			= 6,	// 펫
		ACCESSORY_WILDPET		= 7,	// 공격형 펫
	};

	// SubType : 포션  // Date : 2005-01-07,   By Lee Ki-hwan
	enum POTIONTYPE
	{
		POTION_STATE	= 0,	// 상태이상치료
		POTION_HP		= 1,	// 생명회복
		POTION_MP		= 2,	// 마나회복
		POTION_DUAL		= 3,	// HP+MP 회복
		POTION_STAT		= 4,	// Stat 상승
		POTION_ETC		= 5,	// 기타 (이동향상)	
		POTION_UP		= 6,	// 공방향상
		POTION_TEARS	= 7,	// 눈물
		POTION_CRYSTAL	= 8,	// 결정
	};

	enum POTIONSUBTYPE_UP
	{
		POTION_UP_PHYSICAL	= 0,	// 물리
		POTION_UP_MAGIC		= 1,	// 마법
		POTION_UP_ATTACK	= 0,	// 공격
		POTION_UP_DEFENSE	= 1,	// 방어
	};

	// IONCE_PROCESS_DOC 종류 1
	enum PROCESSDOCTYPE
	{
		PROCESS_DOC_STONE		= 0,   // 광석 정련 문서
		PROCESS_DOC_PLANT		= 1,   // 식물 가공 문서
		PROCESS_DOC_ELEMENT		= 2,   // 원소 정제 문서
	};

	// 스톤 정련 문서 종류
	enum PROCESSDOCSUBTYPE_STONE 
	{
		PROCESS_DOC_SUB_STONE0	= 0,		// 스톤 온 정련 문서
		PROCESS_DOC_SUB_STONE1	= 1,		// 스톤 리스 정련 문서
		PROCESS_DOC_SUB_STONE2	= 2,		// 스톤 웨버 정련 문서
		PROCESS_DOC_SUB_STONE3	= 3,		// 스톤 비스트 정련 문서
		PROCESS_DOC_SUB_STONE4	= 4,		// 스톤 웰스던 정련 문서
	};
	
	// 식물 정련 문서 종류
	enum PROCESSDOCSUBTYPE_PLANT
	{
		PROCESS_DOC_SUB_PLANT0	= 0,		// 크락의 노란잎 가공 문서
		PROCESS_DOC_SUB_PLANT1	= 1,		// 크락의 줄기 가공 문서
		PROCESS_DOC_SUB_PLANT2	= 2,		// 크락의 파란잎 가공 문서
		PROCESS_DOC_SUB_PLANT3	= 3,		// 크락의 가시 가공 문서
		PROCESS_DOC_SUB_PLANT4	= 4,		// 크락의 꽃 가공 문서
	};

	// 원소 정련 문서 종류
	enum PROCESSDOCSUBTYPE_ELEMENT
	{
		PROCESS_DOC_SUB_ELEMENT0	= 0,	// E등급 원소 정제 문서
		PROCESS_DOC_SUB_ELEMENT1	= 1,	// D등급 원소 정제 문서
		PROCESS_DOC_SUB_ELEMENT2	= 2,	// C등급 원소 정제 문서
		PROCESS_DOC_SUB_ELEMENT3	= 3,	// B등급 원소 정제 문서
		PROCESS_DOC_SUB_ELEMENT4	= 4,	// A등급 원소 정제 문서
	};

	// ONCE_BOX  종류 // Date : 2005-01-12,   By Lee Ki-hwan
	enum BOXTYPE
	{
		BOX_REMAKE		= 0,		// 재활의상자
		BOX_ARCANE		= 1,		// 비밀의상자
	};

	enum REFINE_TYPE // Date : 2005-01-12,   By Lee Ki-hwan
	{
		REFINE_GENERAL	= 0,		// 일반제련석
		REFINE_SPECIAL	= 1,		// 고급제련석
	};

	enum LORD_ITEM_TYPE
	{
		LORD_ITEM_BIGSWORD	= 0,	// 대검(타이탄)
		LORD_ITEM_AXE		= 1,	// 도끼(타이탄)
		LORD_ITEM_KNIFE		= 2,	// 기사도(나이트)
		LORD_ITEM_TWOSWORD	= 3,	// 이도류(나이트)
		LORD_ITEM_BOW		= 4,	// 활(힐러)
		LORD_ITEM_WAND		= 5,	// 완드(힐러)
		LORD_ITEM_SSTAFF	= 6,	// 숏스태프(메이지)
		LORD_ITEM_STAFF		= 7,	// 스태프(메이지)
		LORD_ITEM_DAGGER	= 8,	// 단검(로그)
		LORD_ITEM_CROSSBOW	= 9,	// 석궁(로그)
		LORD_ITEM_SCYTHE    = 10,   // 사이드(소서러)
		LORD_ITEM_POLARM    = 11,	// 폴암(소서러)
		LORD_ITEM_TOTAL		= 12
	};

	// AI 적용 범위
	enum PET_AI_SET0 
	{
		PET_AI_RANGE_3M		= 1,		
		PET_AI_RANGE_5M		= 2,		
		PET_AI_RANGE_10M	= 3,
		PET_AI_RANGE_20M	= 4,
		PET_AI_RANGE_30M	= 5,

	};
	// AI 세부선택
	enum PET_AI_SET1
	{
		PET_AI_HP		= 1,
		PET_AI_MP		= 2,
		PET_AI_STM		= 3,
		PET_AI_FAITH	= 4,

	};

	// AI 세부선택 조건( %이하 )
	enum PET_AI_SET2
	{
		PET_AI_PERCENT_99	= 1,
		PET_AI_PERCENT_90	= 2,
		PET_AI_PERCENT_80	= 3,
		PET_AI_PERCENT_70	= 4,
		PET_AI_PERCENT_60	= 5,
		PET_AI_PERCENT_50	= 6,
	};

	// AI 적용 타겟
	enum PET_AI_SET3
	{
		PET_AI_OWNER_ATTACK_ENEMY	= 1,	// 주인이 공격하는 적
		PET_AI_ENEMY_ATTACK_OWNER	= 2,	// 주인을 공격하는 적
		PET_AI_ENEMY_ATTACK_PET		= 3,	// 펫을 공격하는 적
		PET_AI_ENEMY				= 4,	// 적
		PET_AI_PET					= 5,	// 펫
		PET_AI_OWNER				= 6,	// 주인
		PET_AI_ITEM					= 7,	// 아이템
	};

	/* Default constructor. */
	CItemData(void);
	/* Destructor. */
	~CItemData(void);

	//----------------------------------------------------
	// Properties
	// - ItemData는 ItemTool에서 설정된 데이터를 받아서 처리하는데,
	// - 게임 내부에서 데이터가 변경될 일이 거의 없을 것이다.
	// - 고로, 추후에는 프로퍼티 함수를 갖고 있는 것보다는 구조체를 
	// - public으로 노출하는게 좋을듯.	
	// - 아니면, ItemData 구조체를 반환하는 것도 좋을거 같은데...
	// - 또한, 게임 내에서 데이터가 변경될 일이 없으므로, Set 메소드들은 필요가 없을듯...
	//----------------------------------------------------	

	// 싱글 모드 전용.
	inline BOOL IsExclusiveSingleMode()
	{	return FALSE; };

	inline BOOL	CanUse(int iPlayerID)	
	{	return ( Item_Data.job & (1 << iPlayerID) );	};
	
	inline BOOL IsProductionItem()
	{
		return ( Item_Data.subType == ITEM_WEAPON_MINING ||
			Item_Data.subType == ITEM_WEAPON_GATHERING ||
			Item_Data.subType == ITEM_WEAPON_CHARGE );
	}

	inline void SetItemSmcFileName(const char* FileName)
	{	strcpy(Item_Data.fileSMC, FileName);}

	inline void SetArmorEffectName(const char* FileName)
	{	strcpy(Item_Data.ArmorEffectName, FileName);	}

	inline void SetItemBMFileName(const char* FileName)
	{	strcpy(fileBm, FileName);}
	
	inline void SetItemTEXFileName(const char* FileName)
	{	strcpy(fileTex, FileName);}
	
	inline void SetItemTEXNormalFileName(const char* FileName)
	{	strcpy(fileTexNormal, FileName);}
	
	inline void SetItemBM2FileName(const char* FileName)
	{	strcpy(fileBm2, FileName);}
	
	inline void SetItemTEX2FileName(const char* FileName)
	{	strcpy(fileTex2, FileName);	}
	
	inline void SetItemTEX2NormalFileName(const char* FileName)
	{	strcpy(fileTex2Normal, FileName);}

	inline void SetItemBM3FileName(const char* FileName)
	{	strcpy(fileBm3, FileName);}
	
	inline void SetItemTEX3FileName(const char* FileName)
	{	strcpy(fileTex3, FileName);	}
	
	inline void SetItemTEX3NormalFileName(const char* FileName)
	{	strcpy(fileTex3Normal, FileName);}
	
	inline void SetMeshCnt(int Index)
	{	Item_MeshCnt = Index;}
	
	inline void SetTexCnt(int Index) //첫번째 메쉬의 텍스쳐 카운트.
	{	Item_TexCnt = Index;}
	
	inline void SetTex2Cnt(int Index) //두번째 메쉬의 텍스쳐 카운트.
	{	Item_Tex2Cnt = Index;}
	
	inline void SetTex3Cnt(int Index) //세번째 메쉬의 텍스쳐 카운트.
	{	Item_Tex3Cnt = Index;}
	
	inline void SetCreateSet(BOOL bSet)
	{ bCreateSet = bSet; }

	//--------------------------------------------------------------
	inline const char* GetItemSmcFileName()
	{	return Item_Data.fileSMC;}

	inline const char* GetArmorEffectName()
	{	return Item_Data.ArmorEffectName;	}
	
	inline const char* GetItemBMFileName()
	{	return fileBm;}
	
	inline const char* GetItemTEXFileName()
	{	return fileTex;}
	
	inline const char* GetItemTEXNormalFileName()
	{	return fileTexNormal;}
	
	inline const char* GetItemBM2FileName()
	{	return fileBm2;}
	
	inline const char* GetItemTEX2FileName()
	{	return fileTex2;}
	
	inline const char* GetItemTEX2NormalFileName()
	{	return fileTex2Normal;}

	inline const char* GetItemBM3FileName()
	{	return fileBm3;}
	
	inline const char* GetItemTEX3FileName()
	{	return fileTex3;}
	
	inline const char* GetItemTEX3NormalFileName()
	{	return fileTex3Normal;}
	
	inline int GetFlag()		const
	{	return Item_Data.flag;}

	/* Item_Data.flag 은 lod에서 로딩한 Property Flag임 
	inline bool CanUseSuperGOJE() const
	{
		return (Item_Data.flag & FLAG_ITEM_SUPER_STONE_USED) == 0;
	}
	*/
	
	inline bool IsFlag( int nFlag ) const 
	{
		if( Item_Data.flag & nFlag ) return true;
		return false;
	}

	inline BOOL GetCreateSet( void ) const
	{ return bCreateSet; }
	
	inline int GetMeshCnt()		const
	{	return Item_MeshCnt;}
	
	inline int GetTexCnt()		const
	{	return Item_TexCnt;}
	
	inline int GetTex2Cnt()		const
	{	return Item_Tex2Cnt;}

	inline int GetTex3Cnt()		const
	{	return Item_Tex3Cnt;}
	
	inline int	GetNum0() const		
	{	return Item_Data.num0;	}

	inline int	GetNum1() const		
	{	return Item_Data.num1;	}

	inline int	GetNum2() const		
	{	return Item_Data.num2;	}

	inline int	GetNum3() const		
	{	return Item_Data.num3;	}

	inline int GetItemIndex()	const
	{	return Item_Data.index;}
	
	inline int GetGrade()		const
	{	return Item_Data.grade;}

	inline int GetJob()			const
	{	return Item_Data.job; }

	inline int GetLevel()		const
	{	return Item_Data.level;	}

	inline int GetWeight()		const
	{	return Item_Data.weight;	}

	inline int GetFame()		const
	{	return Item_Data.fame;	}
	
	// 착용 위치를 반환합니다.
	inline int GetWearingPosition()	const
	{	return Item_Data.wearing;}
	
	inline int	GetType()		const
	{	return Item_Data.type;}
	
	inline int	GetSubType()	const
	{	return Item_Data.subType;}
	
	inline int	GetZoneNum()	const
	{	return Item_Data.zonenum;	}

	inline int	GetExtraNum()	const
	{	return Item_Data.extranum;	}

	inline int	GetQuestNum()	const
	{	return	Item_Data.questnum;	}

	inline int	GetPetType()	const
	{	return Item_Data.pettype;	}
	
	inline int	GetPrice()		const
	{	return Item_Data.price;}

	inline int	GetProcessType()	const		// 가공 타입
	{	return Item_Data.processtype;	}

	inline int	GetProcessSubType()	const		// 서브 타입
	{	return Item_Data.processsubtype;	}

	inline int	GetProductType()	const		// 제조 타입
	{	return Item_Data.producttype;	}

	inline int	GetProductSubType()	const		// 제조 서브 타입
	{	return Item_Data.productsubtype;	}

	inline int	GetProcessSkill()	const		// 가공 스킬
	{	return Item_Data.sskill;	}

	inline int	GetBoxType()	const		// Box type
	{	return Item_Data.boxtype;	}

	inline int	GetRefineType()	const		// Refinetype type
	{	return Item_Data.refinetype;	}

	inline int GetMakePotionCount () const 
	{	return Item_Data.count;	}

	inline int	GetNeedItemIndex( int nIndex )	const	
	{	return Item_Data.needItemIndex[nIndex];	}

	inline int	GetNeedItemCount( int nIndex )	const
	{	return Item_Data.needItemCount[nIndex];	}

	inline int	GetNeedSSkillIndex()	const
	{	return Item_Data.needSSkillIndex;	}

	inline int	GetNeedSSkillCount()	const
	{	return Item_Data.needSSkillCount;	}

	inline int	GetNeedSSkillIndex2()	const
	{	return Item_Data.needSSkillIndex2;	}

	inline int	GetNeedSSkillCount2()	const
	{	return Item_Data.needSSkillCount2;	}
	
	inline int	GetPhysicalDefence()	const
	{	return Item_Data.pDefense;	}
	
	int	GetPhysicalAttack();		// 공성 보상 작업 추가로 일반 함수로 전환 
	int	GetMagicAttack();
		
	inline int	GetMagicDefence()		const
	{	return Item_Data.mDffense;	}
	
	inline int	GetAttackSpeed()		const
	{	return Item_Data.attackSpeed;}
	
	inline int	GetRecoverSec()			const
	{	return Item_Data.sec;		}

	inline int	GetRecoverHP()			const
	{	return Item_Data.recoverHP;	}
	
	inline int	GetRecoverMP()			const
	{	return Item_Data.recoverMP;	}
	
	inline int	GetSkillType()			const
	{	return Item_Data.skillType;	}
	
	inline int	GetSkillLevel()			const
	{	return Item_Data.skillLevel;}

	inline int	GetOptionType()			const
	{	return Item_Data.optiontype;}
	
	inline const char* GetName()
	{	return Item_Data.name;	}

	inline const char* GetDesc()
	{	return Item_Data.descr;	}

	inline int	GetWarpType() const 
	{ return Item_Data.warpType; }
	
	inline int GetIconTexID()			const
	{	return Item_Data.iIconTexID;}
	
	inline int GetIconTexRow()			const
	{	return Item_Data.iIconTexRow;}
	
	inline int GetIconTexCol()			const
	{	return Item_Data.iIconTexCol;	}

	inline int GetPetAISet0() const
	{ return Item_Data.PetAI_Set0; }

	inline int GetPetAISet1() const
	{ return Item_Data.PetAI_Set1; }

	inline int GetPetAISet2() const
	{ return Item_Data.PetAI_Set2; }

	inline int GetPetAISet3() const
	{ return Item_Data.PetAI_Set3; }

	// [090608: selo] 퀘스트 아이템 사용 제한
	inline int GetRestrictionZoneNo() const		// 사용제한 존 번호
	{ return Item_Data.restrictionZoneNo; }

	inline int GetRestrictionPosX() const		// 사용제한 X좌표
	{ return Item_Data.restrictionPosX; }

	inline int GetRestrictionPosY() const		// 사용제한 Y좌표
	{ return Item_Data.restrictionPosY; }

	inline int GetRestrictionPosZ() const		// 사용제한 Z좌표
	{ return Item_Data.restrictionPosZ; }

	inline int GetRestrictionRadius() const		// 사용제한 반경
	{ return Item_Data.restrictionRadius; }

	static bool IsLordItem( int nIndex ) ; // 성주 무기 인지 아닌지 판다. 
	static bool IsUniqueItem( int nIndex ) ; // 유니크 무기 인지 아닌지 판다. 
	
	//-----------------------------------------------------------------------------
	// Purpose: 아이템 데이터를 화일로부터 읽어들입니다.
	// Input  : &apItemData - 저장될 배열.
	//			FileName - 파일명
	// Output : 	static int
	//-----------------------------------------------------------------------------
	static int LoadItemDataFromFile(CStaticArray<CItemData> &apItemData, const char* FileName);
	static void LoadItemSmcDataFromFile(CStaticArray<ItemSmcParseInfo> &apItemSmcData, const char* FileName);
};

class CItemName
{
private:
	// ITEM 툴에서 사용하고 있는 구조체
	// CItemData의 내부에서만 사용됩니다.
	struct _ItemStat
	{
		int		index;			// 아이템 고유 INDEX
		char	name[50];		// 아이템 이름.		
		char	descr[255];
	};
	_ItemStat	Item_Data;

public:
	CItemName();
	~CItemName();

	inline const char* GetName()
	{	return Item_Data.name;	}

	inline const char* GetDesc()
	{	return Item_Data.descr;	}

	static int LoadItemNameFromFile(CStaticArray<CItemName> &apItemName, const char* FileName);
};

class CItemRareOption
{
private:
	struct RareOption
	{
		int index;
		int level;
	};

	int			m_iIndex;				// 레어 옵션 인덱스
	int			m_iGrade;				// 레어 옵션 등급 (0=A, 1=B, 2=C, 3=D, 4=E)
	int			m_iType;				// 레어 옵션 종류 (0=무기, 1=방어구, 2=액서사리)
	int			m_iLen_prefix;			// 접두어 길이
	char		m_strPrefix[50];			// 접두어
	int			m_iPhysical_attack;	// 물리 공격력
	int			m_iPhysical_defence;	// 물리 방어력
	int			m_iMagic_attack;		// 마법 공격력
	int			m_iMagic_defence;		// 마법 방어력
public:
	RareOption	m_Option[10];			// 레어 옵션
	
public:
	CItemRareOption();
	~CItemRareOption();

	inline const int GetIndex()				{ return m_iIndex; }
	inline const int GetGrade()				{ return m_iGrade; }	
	inline const int GetType()				{ return m_iType;}
	inline const char* GetPrefix()			{ return m_strPrefix; }
	inline const int GetPhysicalAttack()	{ return m_iPhysical_attack; }
	inline const int GetPhysicalDefence()	{ return m_iPhysical_defence; }
	inline const int GetMagicAttack()		{ return m_iMagic_attack; }
	inline const int GetMagicDefence()		{ return m_iMagic_defence;	}	

	static int LoadItemRareOptionFromFile(std::vector<CItemRareOption> &apItemRareOption, const char* FileName);
};
#endif
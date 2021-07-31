#ifndef INCL_MOBDATA_H
#define INCL_MOBDATA_H
#ifdef PRAGMA_ONCE
#pragma once
#endif


#include <Engine/Entities/Entity.h>
#include <Engine/Network/ChatMsgBuffer.h>

#define		NPC_SHOPPER				(1 << 0)	// 상점주인여부?
#define		NPC_FIRSTATTACK			(1 << 1)	// 선공여부?
#define		NPC_ATTACK				(1 << 2)	// 반격가능?
#define		NPC_MOVING				(1 << 3)	// 이동가능?
#define		NPC_PEACEFUL			(1 << 4)	// 평화적인 몹(원클릭으로 공격 안 시킬때)
#define		NPC_ZONEMOVER			(1 << 5)	// 존 이동시켜주는 NPC
#define		NPC_CASTLE_GUARD		(1 << 6)	// 수호병
#define		NPC_REFINER				(1 << 7)	// 연금술사 NPC
#define		NPC_QUEST				(1 << 8)	// Quest NPC
#define		NPC_CASTLE_TOWER		(1 << 9)	// 공격혁 수호상
#define		NPC_MINERAL				(1 << 10)	// 미네랄
#define		NPC_CROPS				(1 << 11)	// 농작물
#define		NPC_ENERGY				(1 << 12)	// 에너지
#define		NPC_ETERNAL				(1 << 13)	// 지속
#define		NPC_LORD_SYMBOL			(1 << 14)	// 성주의 증명
#define		NPC_REMISSION			(1 << 15)	// 면죄부
#define		NPC_EVENT				(1 << 16)	// 이벤트
#define		NPC_GUARD				(1 << 17)	// 경비병
#define		NPC_KEEPER				(1 << 18)	// 창고
#define		NPC_GUILD				(1 << 19)	// 길드
#define		NPC_MBOSS				(1 << 20)	// 준보스
#define		NPC_BOSS				(1 << 21)	// 보스
#define		NPC_RESETSTAT			(1 << 22)	// 스탯 초기화
#define		NPC_CHANGEWEAPON		(1 << 23)	// 무기 교체
#define		NPC_WARCASTLE			(1 << 24)	// 공성 NPC
#define		NPC_DISPLAY_MAP			(1 << 25)	// 지도에 표시되는지 여부
#define		NPC_QUEST_COLLECT		(1 << 26)	// 수집 퀘스트용
#define		NPC_PARTY_MOB			(1 << 27)	// 파티몹 표시
#define		NPC_RAID				(1 << 28)	// 레이드 몹
#define		NPC_SUBCITY				(1 << 29)	// 서브 마을 NPC			// WSS_ADD_FLAG 2007/08/09 카오헌터 마을 NPC
#define		NPC_CHAOCITY			(1 << 30)	// 카오마을 전용 리젠 NPC
#define		NPC_HUNTERCITY			(1 << 31)	// 헌터마을 전용 리젠 NPC
// FLAG Extension	// 2009년 2월
#define		NPC_AUCTION				(1 << 0)	// 거래 대행 NPC
#define		NPC_COLLSION_ENABLE		(1 << 1)	// 캐릭터의 모델에 충돌 flag적용 유무

#define		NPC_MASK				( NPC_ZONEMOVER | NPC_SHOPPER | NPC_REFINER | NPC_QUEST | NPC_REMISSION | NPC_EVENT | NPC_KEEPER | NPC_GUILD | NPC_RESETSTAT | NPC_CHANGEWEAPON | NPC_WARCASTLE | NPC_GUARD)
#define		NPC_RESOURCE_MASK		( NPC_MINERAL | NPC_CROPS | NPC_ETERNAL | NPC_ENERGY | NPC_QUEST_COLLECT)

enum eNpcAnimation
{
	NPC_ANIM_START = 0, 
	NPC_ANIM_WALK_1,
	NPC_ANIM_ATTACK_1,
	NPC_ANIM_ATTACK_2,
	NPC_ANIM_ATTACK_3,
	NPC_ANIM_ATTACK_4,
	NPC_ANIM_IDLE_1,	
	NPC_ANIM_IDLE_2,	
	NPC_ANIM_DIE,
	NPC_ANIM_DAMAGE,	
	NPC_ANIM_RUN_1,		
	NPC_ANIM_END = NPC_ANIM_RUN_1,
	NPC_ANIM_TOTAL,
};
// WSS_DRATAN_SEIGEWARFARE 2007/08/13 --------------->>
enum eSiegewarfareQuarters
{
	DRATAN_SIEGEWARFARE_QUARTER1 = 390,
	DRATAN_SIEGEWARFARE_QUARTER2,
	DRATAN_SIEGEWARFARE_QUARTER3,
	DRATAN_SIEGEWARFARE_QUARTER4,
	DRATAN_SIEGEWARFARE_QUARTER5,
	DRATAN_SIEGEWARFARE_QUARTER6,
	DRATAN_SIEGEWARFARE_QUARTER7,
	DRATAN_SIEGEWARFARE_QUARTER8,
	DRATAN_SIEGEWARFARE_QUARTER9,
	DRATAN_SIEGEWARFARE_QUARTER10,	
};

enum eSiegewarfareQuartersSetup
{
	QUARTER_NONE,
	QUARTER_MINE,
	QUARTER_INSTALL,	
	QUARTER_ATTACK,
};
// ---------------------------------------------------<<
class ENGINE_API CMobData 
{	
private:
	// NPC 툴에서 사용하고 있는 구조체
	// CMobData의 내부에서만 사용됩니다.
	struct _npcStat
	{
		int		index, level, hp, mp;
		int		attackSpeed;						// 공격 속도.
		float	walkspeed;
		float	runspeed;
		float	scale;
		float	AttackDistance;
		float	size;
		char	skill;								// Skill Master 필드.
		char	sskillmaster;
		int		effect[5];
		int		type;								// 어떤 형태의 NPC인가? 상인? 등등...
		int		typeSE;								// NPC플래그를 전부 사용하여 추가된 NPC플래그
		char	name[50];
		char	fileSMC[255];
		char	motion_Idle[50];
		char	motion_Walk[50];
		char	motion_Wound[50];
		char	motion_Attack[50];
		char	motion_Die[50];
		char	motion_Run[50];
		char	motion_Idle2[50];
		char	motion_Attack2[50];		

		char	attackType;
		char	fireDelayCount;
		float	client_fireobjDelay[4];
		char	fireEffect0[256];
		char	fireEffect1[256];
		char	fireEffect2[256];
		char	fireObject;
		float	fireSpeed;

		// 050221 : bs : 스킬 수정
		int		skill0_index;
		char	skill0_level;
		int		skill1_index;
		char	skill1_level;
	};

private:
	union
	{
		ULONG		mob_ulZoneFlag;
		//int			mob_iAttackType;				// 공격하는 방식(예 : 원거리, 근거리)
	};
	union
	{
		ULONG		mob_ulExtraFlag;
	};

	char		motion_Attack3[50];
	char		motion_Attack4[50];
	BOOL		mob_bBoss;
	_npcStat	mob_Data;
	
	//---------------------------------------------
	// 판매 대행 NPC
	INDEX		Owner_Index;
//	CTString	Owner_name;
//	CTString	Owner_Shop_name;
	ChatMsgBuffer	ShopMsg;		

public:
	/* Default constructor. */
	CMobData(void);
	/* Destructor. */
	~CMobData(void);

	inline BOOL IsBoss()	const
	{	return mob_bBoss;		}

	BOOL IsNPC() const ;	// WSS_DRATAN_SEIGEWARFARE 2007/08/08 수정함...

	int  IsMyQuarter() const ;	// WSS_DRATAN_SEIGEWARFARE 2007/08/13

	inline BOOL IsResource()	const
	{	return (mob_Data.type & NPC_RESOURCE_MASK);		}

	inline BOOL	IsShopper()		const
	{ 
		return (mob_Data.type & NPC_SHOPPER);	
	}	

	inline BOOL IsMovable()		const
	{
		return (mob_Data.type & NPC_MOVING);
	}

	inline BOOL IsZoneMoving()	const
	{ 
		return (mob_Data.type & NPC_ZONEMOVER);
	}

	inline BOOL IsCastleGuard() const
	{
		return (mob_Data.type & NPC_CASTLE_GUARD);
	}

	inline BOOL IsLordSymbol() const
	{
		return (mob_Data.type & NPC_LORD_SYMBOL);
	}

	inline BOOL IsCastleTower() const
	{
		return (mob_Data.type & NPC_CASTLE_TOWER);
	}
		
	inline BOOL IsRefiner()		const
	{ 
		return (mob_Data.type & NPC_REFINER); 
	}

	inline BOOL IsPeaceful()	const
	{
		return (mob_Data.type & NPC_PEACEFUL); 
	}	

	inline BOOL IsQuest()		const			// 퀘스트 NPC
	{
		return (mob_Data.type & NPC_QUEST);
	}
	
	inline BOOL IsSkillMaster() const			// 스킬 마스터
	{
		return (mob_Data.skill != -1);
	}

	inline BOOL IsMineral()		const			// 광물
	{
		return (mob_Data.type & NPC_MINERAL);
	}
	
	inline BOOL IsCrops()		const			// 작물
	{
		return (mob_Data.type & NPC_CROPS);
	}

	inline BOOL IsEnergy()		const			// 에너지
	{
		return (mob_Data.type & NPC_ENERGY);
	}

	inline BOOL IsCollect()		const			// 수집물
	{
		return (mob_Data.type & NPC_QUEST_COLLECT);
	}

	inline BOOL IsEternal()		const			// 지속 자원
	{
		return (mob_Data.type & NPC_ETERNAL);
	}

	inline BOOL IsSSkillMaster()	const		//특수 스킬 마스터.
	{
		return (mob_Data.sskillmaster != -1);
	}

	inline BOOL IsRemission()		const		// 면죄부
	{
		return (mob_Data.type & NPC_REMISSION);
	}
	
	inline char GetSkillMaster()	const
	{
		return mob_Data.skill;
	}

	inline char GetSpecialSkillMaster() const
	{
		return mob_Data.sskillmaster;
	}

	inline BOOL IsEvent()			const		// 이벤트
	{
		return (mob_Data.type & NPC_EVENT);
	}

	inline BOOL IsGuard() const
	{
		return ( mob_Data.type & NPC_GUARD );
	}

	inline BOOL IsWareHouse() const				// 창고
	{
		return ( mob_Data.type & NPC_KEEPER );
	}

	inline BOOL IsGuild() const					// 길드
	{
		return ( mob_Data.type & NPC_GUILD );
	}

	inline BOOL IsChangeWeapon() const			// 무기 교환
	{
		return ( mob_Data.type & NPC_CHANGEWEAPON );
	}

	inline BOOL IsCollectQuest() const			// 수집 퀘스트 NPC
	{
		return ( mob_Data.type & NPC_QUEST_COLLECT );
	}

	inline BOOL IsPartyMob() const			// 파티몹 
	{
		return ( mob_Data.type & NPC_PARTY_MOB );
	}

	inline BOOL IsWarCastle() const
	{
		return ( mob_Data.type & NPC_WARCASTLE );
	}

	inline BOOL IsResetStat() const				// 스탯 초기화
	{
		return ( mob_Data.type & NPC_RESETSTAT );
	}

	inline BOOL IsChaoVill() const				// 카오 마을  // WSS_REMOVE_NAMEPOPUP 2007/08/14
	{
		return ( mob_Data.type & NPC_CHAOCITY );
	}

	inline BOOL IsHunterVill() const				// 헌터 마을 // WSS_REMOVE_NAMEPOPUP 2007/08/14
	{
		return ( mob_Data.type & NPC_HUNTERCITY );
	}

	inline BOOL IsAuctionChar() const				// 거래 대행NPC
	{
		return ( mob_Data.typeSE & NPC_AUCTION );
	}

	inline BOOL IsCollsionModel()					// 모델에 충돌 적용될 NPC
	{
		return ( mob_Data.typeSE & NPC_COLLSION_ENABLE);
	}
	
	CTString	GetAnimAttackNameByID(int iAttackID);	
	inline void SetZoneMovingFlag(ULONG ulZoneFlag)
	{	mob_ulZoneFlag = ulZoneFlag; };
	
	inline void SetExtraFlag(ULONG ulExtraFlag)
	{	mob_ulExtraFlag = ulExtraFlag;	};

	inline int GetMobIndex()			const
	{	return mob_Data.index;}
	
	inline const char* GetMobSmcFileName()
	{	return mob_Data.fileSMC;}

	inline void SetMobSmcFileName(CTString MobName)
	{
		ZeroMemory(mob_Data.fileSMC, strlen(mob_Data.fileSMC));
		strcpy(mob_Data.fileSMC, MobName.str_String);
	}
	
	inline int GetLevel()				const
	{	return mob_Data.level;}
	
	inline float GetWalkSpeed()			const
	{	return mob_Data.walkspeed;}

	inline float GetRunSpeed()			const
	{	return mob_Data.runspeed;}

	inline float GetAttackDistance()	const
	{	return mob_Data.AttackDistance;	}
	
	inline float GetHealth()			const
	{	return mob_Data.hp;}
	
	inline int	GetMP()					const
	{	return mob_Data.mp;}

	inline int	GetAttackSpeed()		const
	{	return mob_Data.attackSpeed;	}

	inline float GetScale()				const
	{	return mob_Data.scale;}

	inline float GetSize()				const
	{	return mob_Data.size;}

	inline float GetScaledSize()		const
	{	return mob_Data.size * mob_Data.scale;}

	inline int	GetType()				const
	{	return mob_Data.type;	}
	inline ULONG GetZoneMovingFlag()	const	
	{	return mob_ulZoneFlag; }

	inline ULONG GetExtraFlag()			const
	{	return mob_ulExtraFlag;	};
	
	// NOTE : 월드 에디터에서만 쓰여야 합니다.
	inline const char*	GetName()
	{	return mob_Data.name;}
	
	inline const char*	GetAnimWalkName()
	{	return mob_Data.motion_Walk;}
	
	inline const char* GetAnimRunName()
	{	return mob_Data.motion_Run;	}
	
	inline const char*	GetAnimAttackName()
	{	return mob_Data.motion_Attack;}
	
	inline const char*	GetAnimDefaultName()
	{	return mob_Data.motion_Idle;}
	
	inline const char*	GetAnimDeathName()
	{	return mob_Data.motion_Die;}
	
	inline const char*	GetAnimWoundName()
	{	return mob_Data.motion_Wound;}
	
	inline const char* GetNormalBoxName()
	{	return ("Default");}	

	inline const char* GetAnimAttack2Name()
	{	return mob_Data.motion_Attack2;}

	inline const char*	GetAnimDefault2Name()
	{	return mob_Data.motion_Idle2;}

	inline const char* GetAnimAttack3Name()
	{	return motion_Attack3;}

	inline const char* GetAnimAttack4Name()
	{	return motion_Attack4;}

	inline char GetAttackType() const
	{	return mob_Data.attackType;	}	

	inline char GetDelayCount()		const
	{	return mob_Data.fireDelayCount;	}

	inline float GetDelay(int i)		const
	{	return mob_Data.client_fireobjDelay[i];	}

	inline const char* GetFireEffect0()
	{	return mob_Data.fireEffect0;	}

	inline const char* GetFireEffect1()
	{	return mob_Data.fireEffect1;	}

	inline const char* GetFireEffect2()
	{	return mob_Data.fireEffect2;	}

	inline int GetMissileType()	const
	{	return (int)mob_Data.fireObject;	}	

	inline float GetMissileSpeed()	const
	{	return mob_Data.fireSpeed;	}

	inline int GetSkill0Index()	const
	{	return mob_Data.skill0_index;	}

	inline char	GetSkill0Level()	const
	{	return mob_Data.skill0_level;	}

	inline int GetSkill1Index()	const
	{	return mob_Data.skill1_index;	}

	inline char	GetSkill1Level()	const
	{	return mob_Data.skill1_level;	}
	
	// WSS_DRATAN_SEIGEWARFARE 2007/08/23 -------------->>
	inline void	SetMobName(CTString tName)
	{	strcpy( mob_Data.name , tName.str_String); }
	// -------------------------------------------------<<

	inline void	SetOwnerIndex(INDEX Ownerindex)
	{
		Owner_Index = Ownerindex;
	}

	inline INDEX GetOwnerIndex()
	{
		return Owner_Index;
	}
	
	inline void SetShopmsg(CTString &strMsg)
	{
		ShopMsg.SetChatMsg(strMsg);
	}

public:
	//-----------------------------------------------------------------------------
	// Purpose: NPC 데이터를 읽어들여서 MobData 배열을 생성합니다.
	// Input  : &apMobData - 데이터가 저장될 배열.
	//			FileName - 읽어들일 화일명 MOB월드번호.lod
	// Output : int - 화일로부터 읽어들인 NPC 데이터의 갯수를 반환합니다.
	//-----------------------------------------------------------------------------
	static int LoadNPCDataFromFile(CStaticArray<CMobData> &apMobData, const char* FileName, const char* ZNFileName = NULL);
	static int SetMobDataToNPC(CEntity* penEntity, CMobData& MD, const char* szMobName);
};

class ENGINE_API CMobName
{
private:
	// NPC 툴에서 사용하고 있는 구조체
	// CMobNameData의 내부에서만 사용됩니다.
	struct _npcStat
	{
		int index;
		char name[50];
	};

private:	
	_npcStat	mob_Name;
	
public:
	/* Default constructor. */
	CMobName(void);
	/* Destructor. */
	~CMobName(void);

	inline int GetMobIndex()			const
	{	return mob_Name.index;}

	inline const char*	GetName()
	{	return mob_Name.name;}

public:
	//-----------------------------------------------------------------------------
	// Purpose: NPC 데이터를 읽어들여서 CMobNameData 배열을 생성합니다.
	// Input  : &apMobData - 데이터가 저장될 배열.
	//			FileName - 읽어들일 화일명 MOB월드번호.lod
	// Output : int - 화일로부터 읽어들인 NPC 데이터의 갯수를 반환합니다.
	//-----------------------------------------------------------------------------
	static int LoadMobNameFromFile(CStaticArray<CMobName> &apMobData, const char* FileName);
};
#endif
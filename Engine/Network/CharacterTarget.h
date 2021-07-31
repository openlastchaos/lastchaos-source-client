#ifndef __CHARACTERTARGET_H__
#define __CHARACTERTARGET_H__


//#include <Engine/Network/NetworkMessage.h>
#include <Engine/Base/CTString.h>
#include <Engine/Interface/UIBuff.h>
#include <Engine/Entities/StatusEffect.h>
#include <Engine/Entities/ItemEffect.h>
#include <Engine/Network/ChatMsgBuffer.h>


// FIXME : 많은 정리 필요...ㅡ.ㅡ
class ENGINE_API CCharacterTarget
{
public:
	INDEX			cha_Index;
	INDEX			cha_iType;   	
	INDEX			cha_iJob2;
	INDEX			cha_iClientIndex;
	CEntity			*cha_pEntity;
	CTString		cha_strName;
	SLONG			cha_pkstate;
	UWORD			cha_state;//0:평화모드 1:PK모드 2: 해제딜레이.//1210
	SBYTE			cha_sbShopType;	
	SBYTE			cha_yLayer;
	SBYTE			cha_BuffCount;
	LONG			cha_lGuildIndex;		// 임시	길드의 인덱스
	LONG			cha_lGuildLevel;		// 임시 길드의 레벨
	LONG			cha_lGuildPosition;		// 임시 길드에서의 캐릭터의 레벨
	CTString		cha_strGuildName;		// 길드 명.
	UBYTE			cha_ubGuildNameColor;	// 길드 명 색깔		[071123: Su-won] DRATAN_SIEGE_DUNGEON
	SBYTE			cha_sbGuildRank;		// 길드 랭크 WSS_GUILDMASTER 070517
											//  - 어버이날 이벤트로 추가됨(이후 다른 용도도 이용 가능) 
	BOOL			cha_bExtension;			// 전직 후 무기 추가된거 쓰고 있냐???
	BuffInfo		cha_Buff[BUFF_MAX_COUNT];
	ChatMsgBuffer	ChatMsg;		// yjpark
	ChatMsgBuffer	ShopMsg;		// yjpark
	BOOL			cha_bLegit; //정당방위
	CStatusEffect	cha_statusEffect;
	SLONG			cha_nChangeMobId;
	SLONG			cha_nTransformID;		// 변신체 ID?
	CItemEffect		cha_itemEffect;
	SBYTE			cha_sbItemEffectOption;
	SLONG			cha_nFame;
	SBYTE			cha_sbJoinFlagMerac;
	SBYTE			cha_sbJoinFlagDratan;	// WSS_DRATAN_SIEGEWARFARE 070720	
	UBYTE			cha_sbAttributePos;
	INDEX			cha_iPetType;			// 애완동물 종류
	INDEX			cha_iPetAge;			// 애완동물 나이?
	BOOL			cha_bPetRide;			// 애완동물을 타고 있는가?
//	CEntity			*cha_pPet;
	//CEntity			*cha_pFirstSummon;
	//CEntity			*cha_pSecondSummon;

	SBYTE			cha_sbPresscorps;
public:
	
	/* Default constructor. */
	CCharacterTarget(void);
	/* Destructor. */
	~CCharacterTarget(void);

	CCharacterTarget(const CCharacterTarget &other);
	CCharacterTarget &operator=(const CCharacterTarget &other);
	
	void	Init(void);
	void	SetData( INDEX index, INDEX type, CTString &name, CEntity *pEntity, SBYTE sbyLayer,UWORD state,  SLONG pkstate);
	void	SetShopType( SBYTE sbType);
	void	SetClientChaId( INDEX index );
	void	SetyLayer( SBYTE sbyLayer ) { cha_yLayer = sbyLayer; }
	void	AddBuff( BuffInfo &rBuffInfo )
	{
		cha_Buff[cha_BuffCount++] = rBuffInfo;
	}
	void	RemoveBuff( SLONG slItemIndex, SLONG slSkillIndex );

	int		GetPkState();
	void	SetChaState(SLONG state);
	void	SetChaPkState(SLONG pkstate);
	BOOL	IsLegitimate(){ return cha_bLegit;}
	void    SetLegitimate(BOOL legit){ cha_bLegit = legit;}

	void	ChangeStatus(CTagManager* pTM, SLONG status, CStatusEffect::eRelation rel)
	{
		if(cha_statusEffect.IsStatusChanged(status))
			cha_statusEffect.ChangeStatus(pTM, status, rel);
	}
	void	ResetStatus()
	{
		cha_statusEffect.Reset();
	}
	void	SetItemEffectOption( SBYTE sbOption ) { cha_sbItemEffectOption = sbOption; }

	// wooss 070310 --------------------------------------------------------------->><<
	// kw : WSS_WHITEDAY_2007
	BOOL	IsBuff( SLONG itemIndex );
	CEffectGroup*	IsBuffBySkill( SLONG skillIndex );
};


#define CHA_PVP_STATE_PEACE		0
#define CHA_PVP_STATE_PK		1
#define CHA_PVP_STATE_RELEASE	2

#endif //__CHARACTERTARGET_H__
#include "stdh.h"

#include <Engine/Base/Console.h>
#include <Engine/Network/CharacterTarget.h>
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Network/CNetwork.h>


#define		PLAYER_STATE_PKMODE			(1 << 3)	// PK 모드
#define		PLAYER_STATE_PKMODEDELAY	(1 << 4)	// PK 모드 해제 딜레이
/*
*  Constructor.
*/
CCharacterTarget::CCharacterTarget(void) 
{
	cha_Index = -1;
	cha_iType = -1;                
	cha_iJob2 = 0;
	cha_iClientIndex = -1;
	cha_yLayer = 0;
	cha_BuffCount = 0;
	cha_pEntity = NULL;
//	cha_pPet	= NULL;

//	cha_pFirstSummon	= NULL;
//	cha_pSecondSummon	= NULL;
	cha_state = 0;
	cha_pkstate = 0;
	cha_strName	= CTString("");
	cha_sbGuildRank = 0; 
	ChatMsg.Reset();
	ShopMsg.Reset();
	cha_bLegit = FALSE;	
	
	cha_lGuildIndex	= -1;
	cha_lGuildLevel	= -1;
	cha_lGuildPosition	= -1;
	cha_strGuildName	= CTString("");
	cha_ubGuildNameColor =0;				//[071123: Su-won] DRATAN_SIEGE_DUNGEON
	cha_nChangeMobId    = -1;
	cha_nTransformID	= -1;
	cha_sbItemEffectOption = 0;
	cha_bExtension		= FALSE;
	cha_statusEffect.SetType(CStatusEffect::T_CHARACTER);
	cha_sbJoinFlagMerac		= WCJF_NONE;
	cha_sbJoinFlagDratan	= WCJF_NONE;		// WSS_DRATAN_SIEGEWARFARE 070720	
	cha_sbAttributePos = ATTC_UNWALKABLE;
	cha_iPetType	= -1;
	cha_iPetAge		= -1;
	cha_bPetRide	= FALSE;
}

/*
*  Destructor.
*/
CCharacterTarget::~CCharacterTarget(void) 
{
}

CCharacterTarget::CCharacterTarget(const CCharacterTarget &other)
{
	cha_Index = other.cha_Index;
	cha_iType = other.cha_iType;
	cha_iJob2 = other.cha_iJob2;
	cha_iClientIndex = other.cha_iClientIndex;
	cha_pEntity = other.cha_pEntity;
//	cha_pPet	= other.cha_pPet;
//	cha_pFirstSummon	= other.cha_pFirstSummon;
//	cha_pSecondSummon	= other.cha_pSecondSummon;
	cha_strName = other.cha_strName;
	cha_pkstate = other.cha_pkstate;
	cha_state = other.cha_state;
	cha_sbShopType = other.cha_sbShopType;
	//cha_sbNameLength = other.cha_sbNameLength;
	cha_yLayer = other.cha_yLayer;
	cha_BuffCount = other.cha_BuffCount;
	cha_lGuildIndex = other.cha_lGuildIndex;
	cha_lGuildLevel = other.cha_lGuildLevel;
	cha_lGuildPosition = other.cha_lGuildPosition;
	cha_strGuildName = other.cha_strGuildName;
	cha_ubGuildNameColor =other.cha_ubGuildNameColor;	//[071123: Su-won] DRATAN_SIEGE_DUNGEON
	cha_bExtension = other.cha_bExtension;
	memcpy(cha_Buff, other.cha_Buff, sizeof(cha_Buff[0])*BUFF_MAX_COUNT);
	ChatMsg = other.ChatMsg;
	ShopMsg = other.ShopMsg;
	cha_bLegit = other.cha_bLegit;	
	cha_nChangeMobId	= other.cha_nChangeMobId;
	cha_nTransformID	= other.cha_nTransformID;
	cha_sbItemEffectOption = other.cha_sbItemEffectOption;
	cha_sbJoinFlagMerac	= other.cha_sbJoinFlagMerac;
	cha_sbJoinFlagDratan= other.cha_sbJoinFlagDratan;	// WSS_DRATAN_SIEGEWARFARE 070720	
	cha_sbAttributePos = other.cha_sbAttributePos;
	cha_iPetType	= other.cha_iPetType;
	cha_iPetAge		= other.cha_iPetAge;
	cha_bPetRide	= other.cha_bPetRide;
	//Hardcoding, status나 item effect에 vtable이 추가되거나 상속이 되는 경우 문제 생김.
	cha_statusEffect.Reset();
	cha_itemEffect.Clear();
	memcpy(&cha_statusEffect, &other.cha_statusEffect, sizeof(cha_statusEffect));
	memset((void*)&other.cha_statusEffect, 0, sizeof(other.cha_statusEffect));
	memcpy(&cha_itemEffect, &other.cha_itemEffect, sizeof(cha_itemEffect));
	memset((void*)&other.cha_itemEffect, 0, sizeof(other.cha_itemEffect));
	cha_statusEffect.SetType(CStatusEffect::T_CHARACTER);
}

CCharacterTarget &CCharacterTarget::operator=(const CCharacterTarget &other)
{
	cha_Index = other.cha_Index;
	cha_iType = other.cha_iType;
	cha_iJob2 = other.cha_iJob2;
	cha_iClientIndex = other.cha_iClientIndex;
	cha_pEntity = other.cha_pEntity;
//	cha_pPet	= other.cha_pPet;
	//cha_pFirstSummon	= other.cha_pFirstSummon;
	//cha_pSecondSummon	= other.cha_pSecondSummon;
	cha_strName = other.cha_strName;
	cha_pkstate = other.cha_pkstate;
	cha_state = other.cha_state;
	cha_sbShopType = other.cha_sbShopType;
	//cha_sbNameLength = other.cha_sbNameLength;
	cha_yLayer = other.cha_yLayer;
	cha_BuffCount = other.cha_BuffCount;
	cha_lGuildIndex = other.cha_lGuildIndex;
	cha_lGuildLevel = other.cha_lGuildLevel;
	cha_lGuildPosition = other.cha_lGuildPosition;
	cha_strGuildName = other.cha_strGuildName;
	cha_ubGuildNameColor =other.cha_ubGuildNameColor;	//[071123: Su-won] DRATAN_SIEGE_DUNGEON
	cha_bExtension = other.cha_bExtension;
	memcpy(cha_Buff, other.cha_Buff, sizeof(cha_Buff[0])*BUFF_MAX_COUNT);
	ChatMsg = other.ChatMsg;
	ShopMsg = other.ShopMsg;
	cha_bLegit = other.cha_bLegit;	
	cha_nChangeMobId = other.cha_nChangeMobId;
	cha_nTransformID	= other.cha_nTransformID;
	cha_sbItemEffectOption = other.cha_sbItemEffectOption;
	cha_sbJoinFlagMerac = other.cha_sbJoinFlagMerac;
	cha_sbJoinFlagDratan = other.cha_sbJoinFlagDratan;	// WSS_DRATAN_SIEGEWARFARE 070720	
	cha_sbAttributePos = other.cha_sbAttributePos;
	cha_iPetType	= other.cha_iPetType;
	cha_iPetAge		= other.cha_iPetAge;
	cha_bPetRide	= other.cha_bPetRide;
	//Hardcoding, status나 item effect에 vtable이 추가되거나 상속이 되는 경우 문제 생김.
	cha_statusEffect.Reset();
	cha_itemEffect.Clear();
	memcpy(&cha_statusEffect, &other.cha_statusEffect, sizeof(cha_statusEffect));
	memset((void*)&other.cha_statusEffect, 0, sizeof(other.cha_statusEffect));
	memcpy(&cha_itemEffect, &other.cha_itemEffect, sizeof(cha_itemEffect));
	memset((void*)&other.cha_itemEffect, 0, sizeof(other.cha_itemEffect));
	cha_statusEffect.SetType(CStatusEffect::T_CHARACTER);
	return *this;
}

void CCharacterTarget::Init()
{
	CEntity	*penEntity;

	if( _pNetwork->ga_World.EntityExists( cha_iClientIndex, penEntity ) ) 
	{
		// 애완동물을 타고 있는 경우라면 애완동물도 없애야 한다.
		_pNetwork->LeavePet( penEntity );

		penEntity->en_pCharacterTarget = NULL;
		if( penEntity == _pNetwork->_TargetInfoReal.pen_pEntity )
			_pNetwork->_TargetInfoReal.Init();

		if( penEntity == _pNetwork->_TargetInfo.pen_pEntity )
			_pNetwork->_TargetInfo.Init();

		penEntity->Destroy( FALSE );
	}

	cha_Index = -1;
	cha_iType = -1;                
	cha_iJob2 = 0;
	cha_iClientIndex = -1;
	cha_yLayer = 0;
	cha_BuffCount = 0;
	cha_pEntity = NULL;
//	cha_pPet	= NULL;
//	cha_pFirstSummon	= NULL;
//	cha_pSecondSummon	= NULL;
	cha_state = 0;
	cha_pkstate = 0;
	cha_strName	= CTString("");
	ChatMsg.Reset();
	ShopMsg.Reset();
	cha_bLegit = FALSE;	
	cha_statusEffect.Reset();
	cha_itemEffect.Clear();
	cha_sbItemEffectOption = 0;
	cha_sbJoinFlagMerac = WCJF_NONE;
	cha_sbJoinFlagDratan = WCJF_NONE;		// WSS_DRATAN_SIEGEWARFARE 070720	
	cha_sbAttributePos = ATTC_UNWALKABLE;
	cha_iPetType	= -1;
	cha_iPetAge		= -1;
	cha_bPetRide	= FALSE;
	
	cha_lGuildIndex		= -1;
	cha_lGuildLevel		= -1;
	cha_lGuildPosition	= -1;
	cha_strGuildName	= CTString("");
	cha_ubGuildNameColor =0;	//[071123: Su-won] DRATAN_SIEGE_DUNGEON
	cha_sbGuildRank		= 0;	// WSS_GUILDMASTER 070517
	cha_nChangeMobId    = -1;
	cha_nTransformID	= -1;
	cha_bExtension		= FALSE;
	cha_nFame			= 0;
	cha_statusEffect.SetType(CStatusEffect::T_CHARACTER);

	// wooss 070312 ---------------------------->>
	// kw : WSS_WHITEDAY_2007 
	for(int i=0;i<BUFF_MAX_COUNT;i++)
	{
		cha_Buff[i].Init();
	}	
	// -----------------------------------------<<
}

void CCharacterTarget::SetData( INDEX index, INDEX type, CTString &name, CEntity *pEntity, SBYTE sbyLayer,  UWORD state, SLONG pkstate)
{	
	cha_Index = index;
	cha_iType = type;         
	cha_strName = name;
	//cha_sbNameLength = cha_strName.Length();
	cha_yLayer = sbyLayer;
	cha_pEntity = pEntity;
	cha_state	= state; 
	cha_pkstate = pkstate;
	cha_statusEffect.SetType(CStatusEffect::T_CHARACTER);
}

void CCharacterTarget::SetClientChaId(INDEX index)
{	
	cha_iClientIndex = index;
}

void CCharacterTarget::RemoveBuff( SLONG slItemIndex, SLONG slSkillIndex )
{
	for( int iBuff = 0; iBuff < cha_BuffCount; iBuff++ )
	{
		if( cha_Buff[iBuff].m_slItemIndex == slItemIndex &&
			cha_Buff[iBuff].m_slSkillIndex == slSkillIndex )
			break;
	}

	ASSERT( iBuff < cha_BuffCount );
	if( iBuff == cha_BuffCount )
		return;

	for( iBuff++; iBuff < cha_BuffCount; iBuff++ )
	{
		cha_Buff[iBuff - 1] = cha_Buff[iBuff];
	}

	cha_BuffCount--;
	cha_Buff[cha_BuffCount].Init();
}

int CCharacterTarget::GetPkState()
{
	if((cha_state & PLAYER_STATE_PKMODE) && (cha_state & PLAYER_STATE_PKMODEDELAY))
	{
		return	CHA_PVP_STATE_RELEASE;	
	}
	else if(cha_state & PLAYER_STATE_PKMODE)
	{
		return CHA_PVP_STATE_PK;
	}
	else
	{
		return CHA_PVP_STATE_PEACE;
	}
}

void CCharacterTarget::SetShopType( SBYTE sbType)
{
	cha_sbShopType = sbType;
}


void CCharacterTarget::SetChaState(SLONG state)
{
	cha_state = state;
}

void CCharacterTarget::SetChaPkState(SLONG pkstate)
{
	cha_pkstate = pkstate;
}

// wooss 070310 --------------------------------------------------------------->>
// kw : WSS_WHITEDAY_2007
// ----------------------------------------------------------------------------
// Name : IsBuff() wooss 050803
// Desc : find some buff in buff array
// ----------------------------------------------------------------------------
BOOL CCharacterTarget::IsBuff( SLONG itemIndex )
{
	for(int i=0; i<BUFF_MAX_COUNT;i++)
		if ((cha_Buff[i].m_slItemIndex)==itemIndex) return TRUE;
	return FALSE;	
}

CEffectGroup* CCharacterTarget::IsBuffBySkill( SLONG skillIndex )
{
	for(int i=0; i<BUFF_MAX_COUNT;i++)
	{
		if ((cha_Buff[i].m_slSkillIndex)==skillIndex) 
		{
			return cha_Buff[i].m_pEG;
		}
	}
	return NULL;	
}
// ----------------------------------------------------------------------------<<

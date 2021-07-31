#include "stdh.h"

#include <Engine/Base/Console.h>
#include <Engine/Network/MobTarget.h>
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Network/CNetwork.h>
#include <Engine/Effect/CEffectGroupManager.h>


/*
 *  Constructor.
 */
CMobTarget::CMobTarget(void) 
{
	mob_Index = -1;
	mob_iType = -1;                
	mob_iClientIndex = -1;
	mob_bNPC = FALSE;
	mob_yLayer = 0;
	mob_BuffCount = 0;
	mob_pEntity = NULL;
	mob_pNormalEffect = NULL;
	mob_sbAttributePos	= ATTC_UNWALKABLE;
	mob_statusEffect.SetType(CStatusEffect::T_NPC);
}

/*
 *  Destructor.
 */
CMobTarget::~CMobTarget(void) 
{
	if(mob_pNormalEffect)
	{
		DestroyEffectGroupIfValid(mob_pNormalEffect);
		mob_pNormalEffect = NULL;
	}
}

CMobTarget::CMobTarget(const CMobTarget &other)
{
	mob_Index = other.mob_Index;
	mob_iType = other.mob_iType;
	mob_iClientIndex = other.mob_iClientIndex;
	mob_iLevel = other.mob_iLevel;
	mob_bNPC = other.mob_bNPC;
	mob_pEntity = other.mob_pEntity;
	mob_Name	= other.mob_Name;		
	mob_sbNameLength = other.mob_sbNameLength;
	mob_yLayer = other.mob_yLayer;
	mob_BuffCount = other.mob_BuffCount;
	memcpy(mob_Buff, other.mob_Buff, sizeof(mob_Buff[0])*20);
	//Hardcoding, status effect에 vtable이 추가되거나 상속이 되는 경우 문제 생김.
	memcpy(&mob_statusEffect, &other.mob_statusEffect, sizeof(mob_statusEffect));
	memset((void*)&other.mob_statusEffect, 0, sizeof(other.mob_statusEffect));
	mob_pNormalEffect = other.mob_pNormalEffect;
	mob_sbAttributePos	= other.mob_sbAttributePos;
	mob_statusEffect.SetType(CStatusEffect::T_NPC);
}

CMobTarget &CMobTarget::operator=(const CMobTarget &other)
{
	mob_Index = other.mob_Index;
	mob_iType = other.mob_iType;
	mob_iClientIndex = other.mob_iClientIndex;
	mob_iLevel = other.mob_iLevel;
	mob_bNPC = other.mob_bNPC;
	mob_pEntity = other.mob_pEntity;	
	mob_Name	= other.mob_Name;	
	mob_sbNameLength = other.mob_sbNameLength;
	mob_yLayer = other.mob_yLayer;
	mob_BuffCount = other.mob_BuffCount;
	//Hardcoding, status effect에 vtable이 추가되가나 상속이 되는 경우 문제 생김.
	memcpy(mob_Buff, other.mob_Buff, sizeof(mob_Buff[0])*20);
	memcpy(&mob_statusEffect, &other.mob_statusEffect, sizeof(mob_statusEffect));
	memset((void*)&other.mob_statusEffect, 0, sizeof(other.mob_statusEffect));
	mob_pNormalEffect = other.mob_pNormalEffect;
	mob_sbAttributePos	= other.mob_sbAttributePos;
	mob_statusEffect.SetType(CStatusEffect::T_NPC);
	return *this;
}

//강동민 수정 시작 싱글 던젼 작업	07.27
void CMobTarget::Init()
{
	CEntity	*penEntity;
	if( _pNetwork->ga_World.EntityExists( mob_iClientIndex, penEntity ) ) 
	{
		penEntity->en_pMobTarget = NULL;
		if( penEntity == _pNetwork->_TargetInfoReal.pen_pEntity )
		{
			_pNetwork->_TargetInfoReal.Init();
		}

		if( penEntity == _pNetwork->_TargetInfo.pen_pEntity )
		{
			_pNetwork->_TargetInfo.Init();
		}

		penEntity->Destroy( FALSE );
	}

	mob_Index = -1;
	mob_iType = -1;                
	mob_iClientIndex = -1;
	mob_bNPC = FALSE;
	mob_yLayer = 0;
	mob_BuffCount = 0;
	mob_pEntity = NULL;
	mob_statusEffect.Reset();
	mob_sbAttributePos = ATTC_UNWALKABLE;
	mob_Name	= CTString("");
	if(mob_pNormalEffect)
	{
		DestroyEffectGroupIfValid(mob_pNormalEffect);
		mob_pNormalEffect = NULL;
	}
	mob_statusEffect.SetType(CStatusEffect::T_NPC);
}
//강동민 수정 끝 싱글 던젼 작업		07.27
#include <Engine/Network/Server.h>	// TEST
void CMobTarget::SetData( INDEX index, INDEX type, CTString& strName, INDEX level, CEntity *pEntity,
							BOOL bNPC, SBYTE sbyLayer )
{
	mob_Index = index;
	mob_iType = type;    
	mob_Name = strName;	
	mob_sbNameLength = mob_Name.Length();
	mob_iLevel = level;
	mob_pEntity = pEntity;
	mob_bNPC = bNPC;
	mob_yLayer = sbyLayer;
}

void CMobTarget::SetClientMobId(INDEX index)
{	
	mob_iClientIndex = index;	
}

void CMobTarget::RemoveBuff( SLONG slItemIndex, SLONG slSkillIndex )
{
	for( int iBuff = 0; iBuff < mob_BuffCount; iBuff++ )
	{
		if( mob_Buff[iBuff].m_slItemIndex == slItemIndex &&
			mob_Buff[iBuff].m_slSkillIndex == slSkillIndex )
			break;
	}

	ASSERT( iBuff < mob_BuffCount );
	if( iBuff == mob_BuffCount )
		return;

	for( iBuff++; iBuff < mob_BuffCount; iBuff++ )
	{
		mob_Buff[iBuff - 1] = mob_Buff[iBuff];
	}

	mob_BuffCount--;
	mob_Buff[mob_BuffCount].Init();
}
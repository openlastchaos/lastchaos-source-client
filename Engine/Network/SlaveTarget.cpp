#include "stdh.h"

#include <Engine/Base/Console.h>
#include <Engine/Network/SlaveTarget.h>
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Network/CNetwork.h>
#include <Engine/Effect/CEffectGroupManager.h>

CSlaveTargetInfom::CSlaveTargetInfom(void)
{	
	Init();
}

CSlaveTargetInfom::~CSlaveTargetInfom(void)
{

}

void CSlaveTargetInfom::Init(void)
{
	lIndex			= -1;
	fMaxHealth		= 0.0f;
	fHealth			= 0.0f;
	bIsActive		= FALSE;
	pen_pEntity		= NULL;
	iType			= -1;		
}

/*
 *  Constructor.
 */
CSlaveTarget::CSlaveTarget(void) 
{
	slave_Index			= -1;
	slave_iType			= -1;                	
	slave_iClientIndex	= -1;
	slave_yLayer			= 0;
	slave_pEntity			= NULL;
	slave_pNormalEffect		= NULL;
	slave_sbAttributePos	= ATTC_UNWALKABLE;
	slave_OwnerIndex		= -1;
	slave_BuffCount			= 0;
	slave_OwnerName		= CTString("");
	slave_Name			= CTString("");
	slave_statusEffect.SetType(CStatusEffect::T_NPC);
}

/*
 *  Destructor.
 */
CSlaveTarget::~CSlaveTarget(void) 
{
	if(slave_pNormalEffect)
	{
		DestroyEffectGroupIfValid(slave_pNormalEffect);
		slave_pNormalEffect = NULL;
	}
}

CSlaveTarget::CSlaveTarget(const CSlaveTarget &other)
{
	slave_Index	= other.slave_Index;
	slave_iType	= other.slave_iType;	
	slave_iClientIndex = other.slave_iClientIndex;	
	slave_pEntity = other.slave_pEntity;
	slave_Name	= other.slave_Name;
	slave_OwnerName	= other.slave_OwnerName;
	slave_yLayer = other.slave_yLayer;
	memcpy(slave_Buff, other.slave_Buff, sizeof(slave_Buff[0])*20);
	//Hardcoding, status effect에 vtable이 추가되거나 상속이 되는 경우 문제 생김.
	memcpy(&slave_statusEffect, &other.slave_statusEffect, sizeof(slave_statusEffect));
	memset((void*)&other.slave_statusEffect, 0, sizeof(other.slave_statusEffect));
	slave_pNormalEffect = other.slave_pNormalEffect;
	slave_sbAttributePos	= other.slave_sbAttributePos;
	slave_OwnerIndex	= other.slave_OwnerIndex;
	slave_BuffCount		= other.slave_BuffCount;
	slave_statusEffect.Reset();
	memcpy(&slave_statusEffect, &other.slave_statusEffect, sizeof(slave_statusEffect));
	memset((void*)&other.slave_statusEffect, 0, sizeof(other.slave_statusEffect));
	slave_statusEffect.SetType(CStatusEffect::T_NPC);
}

CSlaveTarget &CSlaveTarget::operator=(const CSlaveTarget &other)
{
	slave_Index = other.slave_Index;
	slave_iType = other.slave_iType;	
	slave_iClientIndex = other.slave_iClientIndex;	
	slave_pEntity = other.slave_pEntity;
	slave_Name = other.slave_Name;
	slave_OwnerName = other.slave_OwnerName;
	slave_yLayer = other.slave_yLayer;
	//Hardcoding, status effect에 vtable이 추가되가나 상속이 되는 경우 문제 생김.
	memcpy(&slave_statusEffect, &other.slave_statusEffect, sizeof(slave_statusEffect));
	memset((void*)&other.slave_statusEffect, 0, sizeof(other.slave_statusEffect));
	slave_pNormalEffect = other.slave_pNormalEffect;
	slave_sbAttributePos	= other.slave_sbAttributePos;
	slave_OwnerIndex	= other.slave_OwnerIndex;
	slave_BuffCount		= other.slave_BuffCount;
	memcpy(slave_Buff, other.slave_Buff, sizeof(slave_Buff[0])*20);
	slave_statusEffect.Reset();
	memcpy(&slave_statusEffect, &other.slave_statusEffect, sizeof(slave_statusEffect));
	memset((void*)&other.slave_statusEffect, 0, sizeof(other.slave_statusEffect));
	slave_statusEffect.SetType(CStatusEffect::T_NPC);
	return *this;
}

void CSlaveTarget::RemoveBuff( SLONG slItemIndex, SLONG slSkillIndex )
{
	for( int iBuff = 0; iBuff < slave_BuffCount; iBuff++ )
	{
		if( slave_Buff[iBuff].m_slItemIndex == slItemIndex &&
			slave_Buff[iBuff].m_slSkillIndex == slSkillIndex )
			break;
	}

	ASSERT( iBuff < slave_BuffCount );
	if( iBuff == slave_BuffCount )
		return;

	for( iBuff++; iBuff < slave_BuffCount; iBuff++ )
	{
		slave_Buff[iBuff - 1] = slave_Buff[iBuff];
	}

	slave_BuffCount--;
	slave_Buff[slave_BuffCount].Init();
}


void CSlaveTarget::Init()
{
	CEntity	*penEntity;
	if( _pNetwork->ga_World.EntityExists( slave_iClientIndex, penEntity ) ) 
	{
		penEntity->en_pSlaveTarget = NULL;
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

	slave_Index			= -1;
	slave_iType			= -1;                	
	slave_iClientIndex	= -1;
	slave_yLayer			= 0;
	slave_pEntity			= NULL;
	slave_statusEffect.Reset();
	slave_sbAttributePos	= ATTC_UNWALKABLE;
	slave_OwnerIndex		= -1;
	slave_BuffCount			= 0;
	slave_OwnerName		= CTString("");
	slave_Name			= CTString("");
	if(slave_pNormalEffect)
	{
		DestroyEffectGroupIfValid(slave_pNormalEffect);
		slave_pNormalEffect = NULL;
	}
	slave_statusEffect.SetType(CStatusEffect::T_NPC);
}
//강동민 수정 끝 싱글 던젼 작업		07.27
#include <Engine/Network/Server.h>	// TEST
void CSlaveTarget::SetData( INDEX index, CTString& strName, CTString& strOwnerName, INDEX iOwnerIndex, INDEX iSlaveType, CEntity *pEntity, SBYTE sbyLayer)
{
	slave_Index		= index;
	slave_iType		= iSlaveType;    	
	slave_Name		= strName;	
	slave_OwnerName	= strOwnerName;	
	slave_pEntity		= pEntity;
	slave_yLayer		= sbyLayer;
	slave_OwnerIndex	= iOwnerIndex;
	slave_statusEffect.SetType(CStatusEffect::T_NPC);
}

void CSlaveTarget::SetClientSlaveId(INDEX index)
{	
	slave_iClientIndex = index;	
}
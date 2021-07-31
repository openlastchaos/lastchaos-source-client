#include "stdh.h"

#include <Engine/Base/Console.h>
#include <Engine/Network/ItemTarget.h>
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Network/CNetwork.h>
#include <Engine/Ska/ModelInstance.h>
#include <Engine/Effect/CEffectGroupManager.h>

/*
*  Constructor.
*/
CItemTarget::CItemTarget(void) 
{
	item_Index = -1;
	item_iType = -1;                
	item_iClientIndex = -1;
	item_yLayer = 0;
	item_pEntity = NULL;
	item_llCount = 0;
	item_place = FLOAT3D(0.0f,0.0f,0.0f);
	item_pDropItemEffect = NULL;
}

/*
*  Destructor.
*/
CItemTarget::~CItemTarget(void) 
{
	if(item_pDropItemEffect)
	{
		DestroyEffectGroupIfValid(item_pDropItemEffect);
		item_pDropItemEffect = NULL;
	}
}

void CItemTarget::Init(void)
{
	CEntity	*penEntity;
	if( _pNetwork->ga_World.EntityExists( item_iClientIndex, penEntity ) )
	{
		if( penEntity == _pNetwork->_TargetInfoReal.pen_pEntity )
		{
			_pNetwork->_TargetInfoReal.Init();
		}

		if( penEntity == _pNetwork->_TargetInfo.pen_pEntity )
		{
			_pNetwork->_TargetInfo.Init();
		}

		if(penEntity->GetRenderType() == CEntity::RT_SKAMODEL)
			penEntity->GetModelInstance()->m_tmSkaTagManager.Unregister("__ROOT");
		penEntity->Destroy( FALSE );
	}

	item_Index = -1;
	item_iType = -1;                
	item_iClientIndex = -1;
	item_yLayer = 0;
	item_pEntity = NULL;
	item_llCount = 0;
	item_place = FLOAT3D(0.0f,0.0f,0.0f);
	if(item_pDropItemEffect)
	{
		DestroyEffectGroupIfValid(item_pDropItemEffect);
		item_pDropItemEffect = NULL;
	}
}

void CItemTarget::SetData( INDEX index, INDEX type, CEntity* pEntity, SBYTE sbyLayer )
{	
	item_Index = index;
	item_iType = type;
	item_yLayer = sbyLayer;
	item_pEntity = pEntity;
}

void CItemTarget::SetData( INDEX index, const char *name, CEntity* pEntity, SBYTE sbyLayer, SQUAD count,FLOAT3D place )
{	
	item_Index = index;	
	strcpy(item_Name, name);
	item_yLayer = sbyLayer;
	item_pEntity = pEntity;
	item_llCount = count;
	item_place = place;
}

void CItemTarget::SetClientItemId(INDEX index)
{	
	item_iClientIndex = index;	
}
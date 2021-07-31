#ifndef __ITEMTARGET_H__
#define __ITEMTARGET_H__
//#include <Engine/Network/NetworkMessage.h>
#include <Engine/Effect/CEffectGroup.h>

class ENGINE_API CItemTarget {
public:
	
	INDEX	item_Index;
	INDEX	item_iType;   	
	INDEX	item_iClientIndex;  
	CEntity	*item_pEntity;
	char	item_Name[100];
	SBYTE	item_yLayer;
	SQUAD   item_llCount;
	FLOAT3D item_place;
	CEffectGroup *item_pDropItemEffect;

public:
	
	/* Default constructor. */
	CItemTarget(void);
	/* Destructor. */
	~CItemTarget(void);
	
	void	Init(void);
	void	SetData( INDEX index, INDEX type, CEntity* pEntity, SBYTE sbyLayer );
	void	SetData( INDEX index, const char *name, CEntity* pEntity, SBYTE sbyLayer, SQUAD count,FLOAT3D place);
	void	SetClientItemId( INDEX index );
	void	SetyLayer( SBYTE sbyLayer ) { item_yLayer = sbyLayer; }
};

#endif //__ITEMTARGET_H__
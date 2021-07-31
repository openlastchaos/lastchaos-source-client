// ----------------------------------------------------------------------------
//  File : WILDPETDATA.h
//  Desc : Created by Dongmin
// ----------------------------------------------------------------------------
#ifndef	WILDPETDATA_H_
#define	WILDPETDATA_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Entities/Entity.h>
#include <vector>


enum eWildPetAni
{
	WILD_PET_ANIM_START = 0, 
	WILD_PET_ANIM_IDLE1 = WILD_PET_ANIM_START,
	WILD_PET_ANIM_IDLE2,
	WILD_PET_ANIM_ATTACK1,
	WILD_PET_ANIM_ATTACK2,
	WILD_PET_ANIM_DAMAGE,
	WILD_PET_ANIM_DIE,
	WILD_PET_ANIM_WALK,
	WILD_PET_ANIM_RUN,
	WILD_PET_ANIM_HUNGRY, //WILD_PET_ANIM_LEVELUP ==> WILD_PET_ANIM_HUNGRY 레벨업에서 배고픔으로
	WILD_PET_ANIM_END = WILD_PET_ANIM_HUNGRY,
	WILD_PET_ANIM_TOTAL,
};

class ENGINE_API CWildPetData
{
private:
	typedef struct _tagWildPetData 
	{
		int	nIndex;
		char strName[256];
		INDEX	nType;
		char strFileName[256];
		char PetAni[WILD_PET_ANIM_TOTAL][50];

	}TWildPetData;

public:
	CWildPetData(void);
	~CWildPetData(void);
	TWildPetData m_WildPetData;

	static int	LoadWildPetDataFromFile(CStaticArray<CWildPetData> &apWildPetData, const char* FileName);
};

#endif
#ifndef __CITEMEFFECT_H__
#define __CITEMEFFECT_H__

#include <Engine/Base/Types.h>
#include <Engine/Base/Memory.h>
#include <Engine/GlobalDefinition.h>
#include <Engine/Entities/ItemData.h>

class CEffectGroup;
class CTagManager;
class ENGINE_API CItemEffect
{
public:

	typedef struct  _ITEMEFFECTGROUP
	{
		char szEffectName[64];
		CEffectGroup* m_pEffectGroup;

		_ITEMEFFECTGROUP() : m_pEffectGroup(NULL)
		{
			memset(szEffectName, 0, sizeof(szEffectName));
		}

		void operator = (const _ITEMEFFECTGROUP& Temp)
		{ // Only EffectName copy
			strcpy(szEffectName, Temp.szEffectName);
			m_pEffectGroup = NULL;
		}

	}ItemEffectGroup;

	CItemEffect();
	~CItemEffect();

	void Refresh(CTagManager *pTM, SBYTE sbOption);
	void RefreshAccessoryEffect(CTagManager *pTM);
	void DeleteEffect(const int wearPos);
	void Clear();
	void Change(const int job, CItemData *ItemData, const int wearPosNew, const int itemPlusNew, CTagManager *pTM, SBYTE sbOption, int nItemSubType );
	BOOL AddAccessoryEffect(const char* strEffectName, CTagManager *pTM);
	BOOL AddWearEffect(const char* strEffectName, const int wearPos, CTagManager *pTM);
	BOOL DelWearEffect(const int wearPos);
	BOOL DelAccessoryEffect(void);
	int GetSubTypeWeapon ( int nSubType );

	CItemEffect(const CItemEffect &);
	CItemEffect &operator=(const CItemEffect &);
protected:
	ItemEffectGroup m_PlusEffectGourp;
	ItemEffectGroup	m_ItemEffectGroup[WEAR_TOTAL];

	char m_szAccessoryEffectName[64];
	CEffectGroup *m_pAccessroyEffect;
	
	int m_aItemPlus[WEAR_TOTAL];
};

#endif //__CITEMEFFECT_H__
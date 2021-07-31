#ifndef __MOBTARGET_H__
#define __MOBTARGET_H__


//#include <Engine/Network/NetworkMessage.h>
#include <Engine/Base/CTString.h>
#include <Engine/Interface/UIBuff.h>
#include <Engine/Entities/StatusEffect.h>
#include <Engine/Effect/CEffectGroup.h>

// FIXME : 많은 정리 필요...ㅡ.ㅡ
class ENGINE_API CMobTarget 
{
public:
	INDEX			mob_Index;
	INDEX			mob_iType;
	INDEX			mob_iClientIndex;
	INDEX			mob_iLevel;
	BOOL			mob_bNPC;
	CEntity			*mob_pEntity;	
	CTString		mob_Name;	
	SBYTE			mob_sbNameLength;
	SBYTE			mob_yLayer;
	SBYTE			mob_BuffCount;
	BuffInfo		mob_Buff[20];
	CStatusEffect	mob_statusEffect;
	CEffectGroup	*mob_pNormalEffect;
	UBYTE			mob_sbAttributePos;

public:
	/* Default constructor. */
	CMobTarget(void);
	/* Destructor. */
	~CMobTarget(void);
	
	CMobTarget(const CMobTarget &other);
	CMobTarget &operator=(const CMobTarget &other);
	
	//강동민 수정 시작 싱글 던젼 작업	07.27
	void	Init(void);
	//강동민 수정 끝 싱글 던젼 작업		07.27
	void	SetData( INDEX index, INDEX type, CTString& strName, INDEX level, CEntity *pEntity,
						BOOL bNPC, SBYTE sbyLayer );
	void	SetClientMobId( INDEX index );
	BOOL	IsNPC() const { return mob_bNPC; }
	void	SetyLayer( SBYTE sbyLayer ) { mob_yLayer = sbyLayer; }
	void	AddBuff( BuffInfo &rBuffInfo )
	{
		mob_Buff[mob_BuffCount++] = rBuffInfo;
	}
	void	RemoveBuff( SLONG slItemIndex, SLONG slSkillIndex );
	void	ChangeStatus(CTagManager* pTM, SLONG status)
	{
		if(mob_statusEffect.IsStatusChanged(status))
			mob_statusEffect.ChangeStatus(pTM, status, CStatusEffect::R_ENEMY);
	}
	void	ResetStatus()
	{
		mob_statusEffect.Reset();
	}
};

#endif //__MOBTARGET_H__
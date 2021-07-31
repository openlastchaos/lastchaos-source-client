
19579
%{
#include "StdH.h"

#include <Engine/Effect/CWorldTag.h>
#include <Engine/Effect/CEffectGroup.h>
#include <Engine/Effect/CEffectGroupManager.h>
#include <Engine/Effect/CSkaEffect.h>
#include <Engine/Graphics/Color.h>
#include <Engine/Entities/ItemData.h>
#include <Engine/Network/ItemTarget.h>
#include <deque>
#include <Engine/Entities/Skill.h>
#include <Engine/Network/MessageDefine.h>
#include "EntitiesMP/Common/CharacterDefinition.h"

#define MOB_ATTACK_MOTION_NUM		(1)

struct sSkillEffectInfo
{
	void InitForNormalAttack(CMobData &mob, INDEX aniID)
	{
		m_bSkillAttack = FALSE;
		szEffectNameCast = mob.GetFireEffect0();
		szEffectNameMissile = mob.GetFireEffect1();
		szEffectNameHit = mob.GetFireEffect2();
		iFireDelayCount = mob.GetDelayCount();
		fFireDelay[0] = mob.GetDelay(0);
		fFireDelay[1] = mob.GetDelay(1);
		fFireDelay[2] = mob.GetDelay(2);
		fFireDelay[3] = mob.GetDelay(3);
		iMissileType = mob.GetMissileType();
		fMissileSpeed = mob.GetMissileSpeed();
		iAnimatioID = aniID;
		dwValidValue = 0x00000000;
	}
	void InitForSkillAttack(CSkill &skill)
	{
		m_bSkillAttack = TRUE;
		szEffectNameCast = skill.GetFireEffect1(0);
		szEffectNameMissile = skill.GetFireEffect2(0);
		szEffectNameHit = skill.GetFireEffect3(0);
		iFireDelayCount = skill.GetDelayCount(0);
		fFireDelay[0] = skill.GetDelay(0,0);
		fFireDelay[1] = skill.GetDelay(1,0);
		fFireDelay[2] = skill.GetDelay(2,0);
		fFireDelay[3] = skill.GetDelay(3,0);
		iMissileType = skill.GetMissileType(0);
		fMissileSpeed = skill.GetMissileSpeed(0);
		iAnimatioID = skill.idPlayer_Anim_Skill[0][2];
		dwValidValue = 0x00000000;
	}
	
	BOOL		m_bSkillAttack;
	const char	*szEffectNameCast;
	const char	*szEffectNameMissile;
	const char	*szEffectNameHit;
	int			iFireDelayCount;
	FLOAT		fFireDelay[4];
	int			iMissileType;
	FLOAT		fMissileSpeed;
	INDEX		iAnimatioID;
	DWORD		dwValidValue;
};
#define SkillEffectInfo() ((sSkillEffectInfo*)m_pSkillEffectInfo)

//-----------------------------------------------------------------------------
// Purpose: 컨테이너 내의 모든 엔티티에 한번에 데미지를 줍니다.
// Input  : pEntity - 
//			sTI - 
//			vCenter - 
// Output : static void
//-----------------------------------------------------------------------------
void DamagedTargetsInRange(CEntity* pEntity, CSelectedEntities& dcEntities, enum DamageType dmtType, FLOAT fDamageAmmount, FLOAT3D vCenter, BOOL bSkill);

void ShotFallDown(FLOAT3D vStartPos, FLOAT3D vToTargetDir, FLOAT fMoveSpeed
				 , const char *szHitEffectName, const char *szFallObjEffectName
				 , bool bCritical);
void GetTargetDirection2(CEntity *penMe, CEntity *penTarget, FLOAT3D &vTargetPos, FLOAT3D &vDirection)
{
	if(penTarget == NULL) {return;}

	if(penTarget == penMe)
	{
		vTargetPos = penMe->en_plPlacement.pl_PositionVector;
		vDirection = FLOAT3D(0,1,1);
		return;
	}
	FLOAT3D vCurrentCenter(((EntityInfo*)(penMe->GetEntityInfo()))->vTargetCenter[0],
	((EntityInfo*)(penMe->GetEntityInfo()))->vTargetCenter[1],
	((EntityInfo*)(penMe->GetEntityInfo()))->vTargetCenter[2] );
	FLOAT3D vCurrentPos = penMe->en_plPlacement.pl_PositionVector + vCurrentCenter;
	FLOAT3D vTargetCenter(0, 0, 0);
	vTargetPos = FLOAT3D(0, 0, 0);
	vDirection = FLOAT3D(0, 0, 0);
	FLOAT size = 0;
	if(penTarget != NULL && penTarget->GetFlags()&ENF_ALIVE)
	{
		if(penTarget->en_RenderType == CEntity::RT_SKAMODEL)
		{
			// Bounding Box를 이용하여 Effect를 찍을 위치를 찾는다.
			FLOATaabbox3D bbox;
			penTarget->GetModelInstance()->GetCurrentColisionBox(bbox);
			FLOAT3D vRadius = (bbox.maxvect - bbox.minvect) * 0.5f * 0.4f/*단순히 원으로 하면 너무 큼. 사이즈 조절 상수*/;
			size = vRadius.Length();
		}
		vTargetCenter = FLOAT3D(((EntityInfo*)(penTarget->GetEntityInfo()))->vTargetCenter[0],
		((EntityInfo*)(penTarget->GetEntityInfo()))->vTargetCenter[1],
		((EntityInfo*)(penTarget->GetEntityInfo()))->vTargetCenter[2] );
		vTargetPos = penTarget->en_plPlacement.pl_PositionVector + vTargetCenter;
		vDirection = vTargetPos - vCurrentPos;
		vDirection.Normalize();
		vTargetPos -= vDirection * size;
	}
	vDirection.Normalize(); 				
};

ENGINE_API void SetDropItemModel(CEntity *penEntity, CItemData &ItemData, CItemTarget &it);
%}

uses "EntitiesMP/PetBase";
uses "EntitiesMP/Global";
uses "EntitiesMP/BasicEffects";
uses "EntitiesMP/EnemyBase";
uses "EntitiesMP/Enemy";

event EPetAttackDamage {//0629
};

%{
// info structure
static EntityInfo eiPetBoss = {
	EIBT_FLESH, 200.0f,
	//0.0f, 1.75f*m_fStretch, 0.0f,     // source (eyes)
	//0.0f, 1.30f*m_fStretch, 0.0f,     // target (body)
	0.0f, 1.75f*1.0f, 0.0f,     // source (eyes)
	0.0f, 1.30f*1.0f, 0.0f,     // target (body)
};

//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
static INDEX g_iNPCMinBrightness = 32;	//명도 0~240
void CPet_OnInitClass(void)
{
	_pShell->DeclareSymbol("persistent user INDEX g_iNPCMinBrightness;", &g_iNPCMinBrightness);
}
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)

void ShotMissile(CEntity *penShoter, const char *szShotPosTagName
				 , CEntity *penTarget, FLOAT fMoveSpeed
				 , const char *szHitEffectName, const char *szArrowEffectName
				 , bool bCritical
				 , FLOAT fHorizonalOffset = 0.0f, FLOAT fVerticalOffset = 0.0f	//-1.0f ~ 1.0f
				 , INDEX iArrowType = 0, const char *szMissileModel = NULL);
void ShotMagicContinued(CEntity *penShoter, FLOAT3D vStartPos, FLOATquat3D qStartRot
				 , CEntity *penTarget, FLOAT fMoveSpeed
				 , const char *szHitEffectName, const char *szMagicEffectName
				 , bool bCritical, INDEX iOrbitType, BOOL bDirectTag = FALSE);
%}

class export CPet: CPetBase {
name      "Pet";
thumbnail "Mods\\test.tbn";
features  "ImplementsOnInitClass", "HasName", "IsTargetable";

properties:	
	3 FLOAT m_fBodyAnimTime		= -1.0f,	
	5 FLOAT m_fStretch			= 1.0f,		
	100 INDEX iAnimPet_Walk		= -1,	
	101 INDEX iAnimPet_Idle1		= -1,
	102 INDEX iAnimPet_Idle2		= -1,
	103 INDEX iAnimPet_Run			= -1,
	104 INDEX iAnimPet_Death		= -1,	
	105 INDEX iAnimPet_Wound		= -1,
	106 INDEX iAnimPet_Attack		= -1,		
	107 INDEX iAnimPet_NormalBox	= -1,	
	108 INDEX iAnimPet_LevelUp		= -1,	
	// 소셜 애니메이션.
	109 INDEX iAnimPet_Skill01		= -1,
	110 INDEX iAnimPet_Skill02		= -1,
	111 INDEX iAnimPet_Skill03		= -1,
	112 INDEX iAnimPet_Skill04		= -1,
	113 INDEX iAnimPet_Skill05		= -1,
	114 INDEX iAnimPet_Skill01_01	= -1,
	115 INDEX iAnimPet_Skill01_02	= -1,
	116 INDEX iAnimPet_Pick			= -1,
	117 INDEX iAnimPetType			= -1,
	118 INDEX iAnimPetAge			= -1,

	200	CTString	m_strName	" Name" = "Enemy",			
	247 enum	EventType	m_EventType			"Event Type"		= EVENT_NONE,
	248 CEntityPointer m_penEventTarget		"Event Target",					// 트리거를 통해서 Blind를 해제하도록 함.	
	251 BOOL	m_bIsRun					= FALSE,
	252 BOOL	m_bDie	= FALSE,
	//253 FLOAT m_fIdleTime = 0.0f,	
	225	FLOAT	m_fActionAnimTime = 0.0f,
	255	BOOL	m_bTransform = FALSE,
	{
		CEffectGroup *m_pSkillReadyEffect;
		CStaticArray<FLOAT3D> m_aDeathItemPosDrop;
		CStaticArray<CItemTarget> m_aDeathItemTargetDrop;
		CStaticArray<void *> m_aDeathItemDataDrop;
		INDEX	m_nMagicAttackSequence;
		void	*m_pSkillEffectInfo;
		INDEX	m_nEffectStep;
		CPetAnimation	m_PetAnimationIndices;
		FLOAT	m_fprevStretch;
		//INDEX	m_iSkillEffectStep;
		//CStaticArray<FLOAT> m_afHardCodeValue;
		//BOOL	m_bHardcodingSkillLoop;		
	}

components:	
	// 사운드 처리가 가장 문제되는 부분.
functions:

//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
	BOOL AdjustShadingParameters(FLOAT3D &vLightDirection, COLOR &colLight, COLOR &colAmbient)
	{
		//이 주석을 풀면 몹사망시 Fade-out처리가 된다. 단 약간 튀는 듯한 문제 있음.
		//CPetBase::AdjustShadingParameters(vLightDirection, colLight, colAmbient);
		g_iNPCMinBrightness = Clamp(g_iNPCMinBrightness, (INDEX)0, (INDEX)240);
		BOOL bRecalcAmbient = FALSE;
		UBYTE ubAH, ubAS, ubAV;
		ColorToHSV(colAmbient, ubAH, ubAS, ubAV);
		if(ubAV < g_iNPCMinBrightness)
		{
			UBYTE ubAR, ubAG, ubAB;
			ColorToRGB(colAmbient, ubAR, ubAG, ubAB);
			ubAR = g_iNPCMinBrightness;
			ubAG = g_iNPCMinBrightness;
			ubAB = g_iNPCMinBrightness;
			ColorToHSV(RGBToColor(ubAR, ubAG, ubAB), ubAH, ubAS, ubAV);
			bRecalcAmbient = TRUE;
		}
		if(bRecalcAmbient) {colAmbient = HSVToColor(ubAH, ubAS, ubAV);}
		return TRUE;
		//return CPetBase::AdjustShadingParameters(vLightDirection, colLight, colAmbient);
	}
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)

	virtual FLOAT GetLockRotationSpeed(void) 
	{ 
		return AngleDeg(1800.0f); 
	};

	// 펫이 움직일 수 있는 조건인지 판단합니다.
	virtual BOOL IsMovable()
	{
		// 플레이어의 펫이고, 혼자서 이동 가능한 펫일경우...
		if(m_bUseAI)
		{
			// 배고픔이 0이라면 못움직임.
			if(	( _pNetwork->_PetTargetInfo.bIsActive && ( _pNetwork->_PetTargetInfo.fHungry <= 0 || _pNetwork->_PetTargetInfo.fHealth <= 0 ) ) 
				|| m_bStuned || m_bHold )
			{
				return FALSE;
			}		
		}
		return TRUE;
	};
	
	void OnInitialize(const CEntityEvent &eeInput)
	{
		CPetBase::OnInitialize(eeInput);
	};

	void OnEnd(void) 
	{
		CPetBase::OnEnd();		
	};	

	// describe how this enemy killed player
	virtual CTString GetPlayerKillDescription(const CTString &strPlayerName, const EDeath &eDeath)
	{
		CTString str;
		str.PrintF(TRANS("A %s sent %s into the halls of Valhalla") ,m_strName, strPlayerName);
		return str;
	}

	/* Entity info */
	void *GetEntityInfo(void)
	{
			return &eiPetBoss;
	};

	void Precache(void) 
	{
		CPetBase::Precache();

//안태훈 수정 시작	//(Fix Snd Effect Bug)(0.1)
		if(en_pmiModelInstance) { en_pmiModelInstance->m_tmSkaTagManager.SetOwner(this); }
//안태훈 수정 끝	//(Fix Snd Effect Bug)(0.1)
	};

	virtual BOOL IsPet(void) const
	{
		return TRUE;
	}

	// 펫이 데미지를 입힐경우...
	void InflictDirectDamage(CEntity *penToDamage, CEntity *penInflictor, enum DamageType dmtType,
	FLOAT fDamageAmmount, const FLOAT3D &vHitPoint, const FLOAT3D &vDirection)
	{
		if(penToDamage )
		{
			if( !(penToDamage->GetFlags()&ENF_ALIVE ) )	{ return; }
			INDEX preHealth, curHealth;
			preHealth = ((CUnit*)penToDamage)->m_nPreHealth;
			curHealth = ((CUnit*)penToDamage)->m_nCurrentHealth;

			if(preHealth!= -1)// && curHealth > preHealth)
			{
				fDamageAmmount = 1;
				((CUnit*)penToDamage)->m_nCurrentHealth = ((CUnit*)penToDamage)->m_nPreHealth;			
			}
			else fDamageAmmount = 0;
	
			((CUnit*)penToDamage)->m_nPreHealth = -1; //1103
		}

		CEntity::InflictDirectDamage(penToDamage, penInflictor, dmtType,
			fDamageAmmount,vHitPoint, vDirection);	

		if(penToDamage && penToDamage->IsEnemy())
		{
			const INDEX iMobIndex = ((CEnemy*)penToDamage)->m_nMobDBIndex;
			// 성주의 권좌일때....
			if( iMobIndex == LORD_SYMBOL_INDEX)
			{
				CMobData& MD = _pNetwork->GetMobData(iMobIndex);
				INDEX iCurHealth = ((CUnit*)((CEntity*)penToDamage))->m_nCurrentHealth;
				static INDEX iType = 0;
				float fMaxHealth = MD.GetHealth();
				if( iCurHealth <= fMaxHealth * 0.25f)
				{
					if(iType != 1 )
					{
						iType = 1;
						penToDamage->SetSkaModel("data\\npc\\Gguard\\sword04.smc");
						penToDamage->GetModelInstance()->RefreshTagManager();
					}
				}
				else if( iCurHealth <= fMaxHealth * 0.50f)
				{
					if( iType != 2 )
					{
						iType = 2;
						penToDamage->SetSkaModel("data\\npc\\Gguard\\sword03.smc");
						penToDamage->GetModelInstance()->RefreshTagManager();
					}
				}
				else if( iCurHealth <= fMaxHealth * 0.75f)
				{
					if( iType != 3 )
					{
						iType = 3;
						penToDamage->SetSkaModel("data\\npc\\Gguard\\sword02.smc");
						penToDamage->GetModelInstance()->RefreshTagManager();
					}
				}
				else
				{
					if( iType != 0 )
					{
						iType = 0;
						penToDamage->SetSkaModel(MD.GetMobSmcFileName());
						penToDamage->GetModelInstance()->RefreshTagManager();
					}
				}
			}
		}	

		if(penToDamage )
		{
			if(((CUnit*)penToDamage)->m_nCurrentHealth <= 0 && penToDamage != this)
			{
				((CUnit*)((CEntity*)penToDamage))->DeathNow();
			}
		}
	};

	//--------------------------------------------------------------
	// 데미지를 입었을 경우의 처리...
	//--------------------------------------------------------------
	/* Receive damage */
	void ReceiveDamage(CEntity *penInflictor, enum DamageType dmtType,
		FLOAT fDamageAmmount, const FLOAT3D &vHitPoint, const FLOAT3D &vDirection) 
	{
		if(dmtType != DMT_NONE && fDamageAmmount > 0)
		{
			FLOAT3D vAxisY(0,1,0);
			FLOAT angle = acos(vDirection % vAxisY);
			FLOAT3D axis = vAxisY * vDirection;
			axis.Normalize();
			FLOATquat3D quat;
			quat.FromAxisAngle(axis, angle);
			StartEffectGroup("Normal Hit"		//Hardcording
				, _pTimer->GetLerpedCurrentTick(), vHitPoint, quat);
		}

		CPetBase::ReceiveDamage(penInflictor, dmtType, fDamageAmmount, vHitPoint, vDirection);
	};

	BOOL IsCommandAnimationPlaying()
	{
		// Command 애니메이션을 플레이하고 있는지를 체크합니다.
		if( IsAnimationPlaying(iAnimPet_Skill01) || 
			IsAnimationPlaying(iAnimPet_Skill02) || 
			IsAnimationPlaying(iAnimPet_Skill03) || 
			IsAnimationPlaying(iAnimPet_Skill04) || 
			IsAnimationPlaying(iAnimPet_Skill05) )
		{
			return TRUE;
		}
		return FALSE;
	};
	
	BOOL TransformPetModel(INDEX iMobIndex, FLOAT fStretch)
	{
		SetPhysicsFlags(GetPhysicsFlags() & ~(EPF_TRANSLATEDBYGRAVITY|EPF_ORIENTEDBYGRAVITY));
		CMobData &MD = _pNetwork->GetMobData( iMobIndex );
	
		SetSkaModel( MD.GetMobSmcFileName() );
		
		m_fprevStretch = m_fStretch;
		m_fStretch = fStretch;

		StretchModel(FLOAT3D(m_fStretch, m_fStretch, m_fStretch));

		// pet ani index backup
		m_PetAnimationIndices.m_iAnimation[PET_ANIM_WALK_1] = iAnimPet_Walk;
		m_PetAnimationIndices.m_iAnimation[PET_ANIM_IDLE_1] = iAnimPet_Idle1;
		m_PetAnimationIndices.m_iAnimation[PET_ANIM_IDLE_2] = iAnimPet_Idle2;
		m_PetAnimationIndices.m_iAnimation[PET_ANIM_RUN_1] = iAnimPet_Run;
		m_PetAnimationIndices.m_iAnimation[PET_ANIM_PICK] = iAnimPet_Pick;
		m_PetAnimationIndices.m_iAnimation[PET_ANIM_DAMAGE] = iAnimPet_Wound;
		m_PetAnimationIndices.m_iAnimation[PET_ANIM_DIE] = iAnimPet_Death;
		m_PetAnimationIndices.m_iAnimation[PET_ANIM_LEVELUP] = iAnimPet_LevelUp;
		m_PetAnimationIndices.m_iAnimation[PET_ANIM_ATTACK] = iAnimPet_Attack;
		// new ani index
		iAnimPet_Walk = ska_GetIDFromStringTable(MD.GetAnimWalkName());
		iAnimPet_Idle1 = ska_GetIDFromStringTable(MD.GetAnimDefaultName());
		iAnimPet_Idle2 = ska_GetIDFromStringTable(MD.GetAnimDefault2Name());
		iAnimPet_Run = ska_GetIDFromStringTable(MD.GetAnimRunName());
		iAnimPet_Pick = ska_GetIDFromStringTable(MD.GetAnimAttackName());
		iAnimPet_Attack = ska_GetIDFromStringTable(MD.GetAnimAttackName());
		iAnimPet_Death = ska_GetIDFromStringTable(MD.GetAnimDeathName());
		iAnimPet_LevelUp = ska_GetIDFromStringTable(MD.GetAnimWoundName());
		iAnimPet_Wound = ska_GetIDFromStringTable(MD.GetAnimWoundName());

		AddAnimation(iAnimPet_Idle1, AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
		//GetModelInstance()->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_IDLE],AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);
		SetPhysicsFlags(GetPhysicsFlags() | EPF_TRANSLATEDBYGRAVITY|EPF_ORIENTEDBYGRAVITY);
		m_bTransform = TRUE;

		return TRUE;
	}

	void ReturnPetModel(void)
	{
		if(!m_bTransform)
		{
			return;
		}

		CTString strNewLook;

		m_fStretch = m_fprevStretch;

		m_bTransform = FALSE;
		SetPhysicsFlags(GetPhysicsFlags() & ~(EPF_TRANSLATEDBYGRAVITY|EPF_ORIENTEDBYGRAVITY));
		SetSkaModel(PetInfo().GetFileName(m_iPetType, m_iPetAge));

		iAnimPet_Walk = m_PetAnimationIndices.m_iAnimation[PET_ANIM_WALK_1];
		iAnimPet_Idle1 = m_PetAnimationIndices.m_iAnimation[PET_ANIM_IDLE_1];
		iAnimPet_Idle2 = m_PetAnimationIndices.m_iAnimation[PET_ANIM_IDLE_2];
		iAnimPet_Run = m_PetAnimationIndices.m_iAnimation[PET_ANIM_RUN_1];
		iAnimPet_Pick = m_PetAnimationIndices.m_iAnimation[PET_ANIM_PICK];
		iAnimPet_Wound = m_PetAnimationIndices.m_iAnimation[PET_ANIM_DAMAGE];
		iAnimPet_Death = m_PetAnimationIndices.m_iAnimation[PET_ANIM_DIE];
		iAnimPet_LevelUp = m_PetAnimationIndices.m_iAnimation[PET_ANIM_LEVELUP];
		iAnimPet_Attack = m_PetAnimationIndices.m_iAnimation[PET_ANIM_ATTACK];
		
		SetPhysicsFlags(GetPhysicsFlags() | EPF_TRANSLATEDBYGRAVITY|EPF_ORIENTEDBYGRAVITY);
		// [070824: Su-won] PET_COLOR_CHANGE
		// 펫 색깔 변경 아이템을 사용한 상태라면 텍스쳐 파일을 바꿔줌...
		if( m_iPetColor >0 )
		{
			CTString strTexFile =PetInfo().GetColoredTexFileName(m_iPetType, m_iPetAge, m_iPetColor);
			GetModelInstance()->mi_aMeshInst[0].mi_tiTextures[0].ti_toTexture.SetData_t( strTexFile );
		}
	}

	//--------------------------------------------------------------
	// 몬스터의 애니메이션을 설정하는 함수들...
	//--------------------------------------------------------------
	// damage anim
	INDEX AnimForDamage(FLOAT fDamage) 
	{
		ASSERT(iAnimPet_Wound		!= -1	&& "Invalid Animation");
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");

		//0820 가만히 서있을때만 데미지 모션이 나와야 된다. 꼭!
		if(!IsAnimationPlaying(iAnimPet_Idle1) || !IsAnimationPlaying(iAnimPet_Idle2) )
		{
			INDEX animSet, animIdx;
			static INDEX animId = ska_StringToID("mldam");//HardCoding
			if(this->en_pmiModelInstance
			&& this->en_pmiModelInstance->FindAnimationByID(animId, &animSet, &animIdx))
			{
				//타격 모션
				this->en_pmiModelInstance->AddAnimationChild(animId, AN_REMOVE_ON_END, 1.0f, 0);
			}
			return animId;
		}

		NewClearState(CLEAR_STATE_LENGTH);    
		GetModelInstance()->AddAnimationChild(iAnimPet_Wound,AN_CLEAR,1,0);	
		
		return iAnimPet_Wound;
	};

	// death
	INDEX AnimForDeath(void)
	{	
		ASSERT(iAnimPet_Death		!= -1	&& "Invalid Animation");
	
		//DropDeathItem();//1019
				
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");
		NewClearState(CLEAR_STATE_LENGTH);    
		GetModelInstance()->AddAnimationChild(iAnimPet_Death,AN_NORESTART,1,0);

		return iAnimPet_Death;
	};

	// 죽을때 뭔가가...
	FLOAT WaitForDust(FLOAT3D &vStretch)
	{
		//0317 kwon
		vStretch = FLOAT3D(1,1,1);		
		if(GetModelInstance()->IsAnimationPlaying(iAnimPet_Death)) 
		{
			return 0.5f;
		} 
		return -1.0f;
	};

	void DeathNotify(void) 
	{
		// 몸의 비중...
		en_fDensity = 500.0f;

		if(m_EventType == EVENT_MOBDESTROY)
		{
			SendToTarget(m_penEventTarget, EET_TRIGGER, this);
		}		
	};

	// 클릭했을때의 이벤트.
	void ClickedEvent(void)
	{
		if(m_EventType == EVENT_MOBCLICK)
		{
			SendToTarget(m_penEventTarget, EET_TRIGGER, this);
		}
	};

		void StandingAnim(void) 
	{		
		m_bIsRun = FALSE;
		ASSERT(iAnimPet_Idle1	!= -1	&& "Invalid Animation");		
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");
		GetModelInstance()->AddAnimation(iAnimPet_Idle1,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);		
		GetModelInstance()->AddAnimationChild(iAnimPet_Idle1,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);		
	};

	void RunningAnim(void) 
	{
		m_bIsRun = TRUE;
		ASSERT(iAnimPet_Run		!= -1	&& "Invalid Animation");
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");
		GetModelInstance()->AddAnimation(iAnimPet_Run,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);
		GetModelInstance()->AddAnimationChild(iAnimPet_Run,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);
	};

	void WalkingAnim(void) 
	{
		m_bIsRun = FALSE;
		ASSERT(iAnimPet_Walk		!= -1	&& "Invalid Animation");
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");
		GetModelInstance()->AddAnimation(iAnimPet_Walk,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);		
		GetModelInstance()->AddAnimationChild(iAnimPet_Walk,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);		
	};

	void LevelUpAnim(void)
	{
		ASSERT(iAnimPet_LevelUp		!= -1	&& "Invalid Animation");
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");
		GetModelInstance()->AddAnimationChild(iAnimPet_LevelUp,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);				
	};

	void SitAnim(void)		// 계속 앉아 있기.
	{
		ASSERT(iAnimPet_Skill01		!= -1	&& "Invalid Animation");
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");
		GetModelInstance()->AddAnimationChild(iAnimPet_Skill01,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);				
		//m_fBodyAnimTime = GetModelInstance()->GetAnimLength(iAnimPet_Skill01); // anim length
		//((CPlayer*)(CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetBodyAnimTime(GetModelInstance()->GetAnimLength(iAnimPet_Skill01)); 
	};

	void StandUpAnim(void)	// 일어 서기.
	{
		ASSERT(iAnimPet_Skill01_02		!= -1	&& "Invalid Animation");
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");
		GetModelInstance()->AddAnimationChild(iAnimPet_Skill01_02,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);
		//m_fBodyAnimTime = GetModelInstance()->GetAnimLength(iAnimPet_Skill01_02); // anim length
		//((CPlayer*)(CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetBodyAnimTime(GetModelInstance()->GetAnimLength(iAnimPet_Skill01_02)); 
	};

	void SittingAnim(void)	// 앉기...
	{
		ASSERT(iAnimPet_Skill01_01		!= -1	&& "Invalid Animation");
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");
		GetModelInstance()->AddAnimationChild(iAnimPet_Skill01_01,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);
		//((CPlayer*)(CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetBodyAnimTime(GetModelInstance()->GetAnimLength(iAnimPet_Skill01_01)); 

		//m_fBodyAnimTime = GetModelInstance()->GetAnimLength(iAnimPet_Skill01_01); // anim length
		//m_fBodyAnimTime = GetPlayer()->GetModelInstance()->GetAnimLength(iAnimation); // anim length
	};

	void PickAnim(void)
	{
		ASSERT(iAnimPet_Pick		!= -1	&& "Invalid Animation");
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");
		GetModelInstance()->AddAnimationChild(iAnimPet_Pick,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);
	};

		void SkillAnim(INDEX iAnimIndex, DWORD dwFlag, float fSpeed)
	{
		ASSERT(iAnimIndex		!= -1	&& "Invalid Animation");
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");
		//NewClearState(CLEAR_STATE_LENGTH);		
		GetModelInstance()->AddAnimationChild(iAnimIndex,dwFlag,1,0, fSpeed);
		//((CPlayer*)(CPlayerEntity*)CEntity::GetPlayerEntity(0))->AddAnimationChild(iAnimIndex, dwFlag, 1.0f, 3,
		//	ESKA_MASTER_MODEL_INSTANCE, fSpeed);
	};

	void RotatingAnim(void) 
	{
		if(m_bIsRun)
		{
			RunningAnim();
		}
		else
		{
			WalkingAnim();
		}
	};

	// adjust sound and watcher parameters here if needed
	void EnemyPostInit(void) 
	{
		// set sound default parameters
		m_soSound.Set3DParameters(30.0f, 5.0f, 1.0f, 1.0f);
	};

	
//안태훈 수정 시작	//(5th Closed beta)(0.2)
	void AddDeathItem(FLOAT3D pos, class CItemTarget *pItemTarget, class CItemData *pItemData)
	{
		INDEX cnt = m_aDeathItemTargetDrop.Count();
		m_aDeathItemPosDrop.Expand(cnt + 1);
		m_aDeathItemPosDrop[cnt] = pos;
		m_aDeathItemTargetDrop.Expand(cnt + 1);
		m_aDeathItemTargetDrop[cnt] = *pItemTarget;
		m_aDeathItemDataDrop.Expand(cnt + 1);
		m_aDeathItemDataDrop[cnt] = pItemData;
	}

	virtual void DropDeathItem()
	{
		for(INDEX i = 0; i < m_aDeathItemTargetDrop.Count(); ++i )
		{
			CItemTarget *pItem = NULL;
			for(int iItem=0; iItem<_pNetwork->ga_srvServer.srv_aitItem.Count(); ++iItem)
			{
				if(m_aDeathItemTargetDrop[i].item_Index == _pNetwork->ga_srvServer.srv_aitItem[iItem].item_Index)
				{
					pItem = &(_pNetwork->ga_srvServer.srv_aitItem[iItem]);
				}
			}
			if(pItem == NULL) {continue;}	//이미 떨어지기 전에 누가 주워버렸음

			CItemTarget &it = *pItem;
			it = m_aDeathItemTargetDrop[i];
			CItemData &ItemData = *((CItemData*)m_aDeathItemDataDrop[i]);
			const char* szItemData = _pNetwork->GetItemName( ItemData.GetItemIndex() );
			CEntity* penEntity = NULL;
			CPlacement3D plPlacement;
			plPlacement.pl_PositionVector = m_aDeathItemPosDrop[i];
			plPlacement.pl_OrientationAngle = ANGLE3D(0.0f,0.0f,0.0f);
			ASSERT(it.item_pEntity == NULL);
			penEntity = _pNetwork->ga_World.CreateEntity_t(plPlacement, CLASS_ITEM,-1,TRUE);
			if(penEntity == NULL) {continue;}

			it.item_pEntity = penEntity;
			it.item_iClientIndex = penEntity->en_ulID;						
			penEntity->en_strItemName = szItemData;
			SetDropItemModel(penEntity, ItemData, it);

			//드랍 사운드 플레이
			if( ItemData.GetType() == CItemData::ITEM_ETC
			&& ItemData.GetType() == CItemData::ITEM_ETC_MONEY )
			{
				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->PlayItemSound(FALSE, TRUE);
			}
			else
			{
				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->PlayItemSound(FALSE, FALSE);
			}
		}
		m_aDeathItemPosDrop.Clear();
		m_aDeathItemTargetDrop.Clear();
		m_aDeathItemDataDrop.Clear();
	}

	void SkillAndAttackFire(int num)
	{
		if( !(m_penEnemy.ep_pen != NULL
			&& m_penEnemy->en_pmiModelInstance != NULL
			&& m_penEnemy->GetFlags() & ENF_ALIVE) )
		{
			return;
		}
		switch(SkillEffectInfo()->iMissileType)
		{
		case 0/*MT_NONE*/:
			{
				FLOAT3D vHitPoint;
				FLOAT3D vHitDir;
				GetTargetDirection2(this, m_penEnemy, vHitPoint, vHitDir);

				if( m_dcEnemies.Count() > 0 )
				{
					DamagedTargetsInRange(this, m_dcEnemies, DMT_EXPLOSION, 1, vHitPoint, TRUE);
					m_dcEnemies.Clear();
				}
				else
				{
					//damage effect 처리
					this->InflictDirectDamage(m_penEnemy, this, DMT_NONE, 1, vHitPoint, vHitDir);
				}

				FLOAT3D vAxisY(0,1,0);
				FLOAT angle = acos(vHitDir % vAxisY);
				FLOAT3D axis = vAxisY * vHitDir;
				axis.Normalize();
				FLOATquat3D quat;
				quat.FromAxisAngle(axis, angle);
				StartEffectGroup(SkillEffectInfo()->szEffectNameHit
								, _pTimer->GetLerpedCurrentTick()
								, vHitPoint, quat);
			} break;
		case 1/*MT_ARROW*/:
			{
				if( m_dcEnemies.Count() > 0 )
				{
					for( ENTITIES_ITERATOR it = m_dcEnemies.vectorSelectedEntities.begin();
						it != m_dcEnemies.vectorSelectedEntities.end(); ++it )
					{																		
						CEntity *pEn = (*it);
						if(pEn != NULL && pEn->IsFlagOff(ENF_DELETED))
						{
							ShotMissile( this, "RHAND", pEn, SkillEffectInfo()->fMissileSpeed, "Normal Hit", "Normal Arrow Trace", false );
						}
					}
				}
				else
				{
					ShotMissile( this, "RHAND", m_penEnemy, SkillEffectInfo()->fMissileSpeed, "Normal Hit", "Normal Arrow Trace", false );
				}
			} break;
		case 2/*MT_DIRECT*/:
			{
				StartEffectGroup(SkillEffectInfo()->szEffectNameHit
					, &m_penEnemy->en_pmiModelInstance->m_tmSkaTagManager
					, _pTimer->GetLerpedCurrentTick());
			} break;
		case 3/*MT_CONTINUE*/:
			{
				FLOAT3D lastEffectInfo = m_penEnemy->en_plPlacement.pl_PositionVector;
				lastEffectInfo(2) += 1;
				if(m_pSkillReadyEffect != NULL && CEffectGroupManager::Instance().IsValidCreated(m_pSkillReadyEffect))
				{
					for(INDEX i=0; i<m_pSkillReadyEffect->GetEffectCount(); ++i)
					{
						CEffect *pEffect = m_pSkillReadyEffect->GetEffectKeyVector()[i].m_pCreatedEffect;
						if(pEffect != NULL && pEffect->GetType() == ET_SKA)
						{
							CSkaEffect *pSkaEffect = (CSkaEffect*)pEffect;
							lastEffectInfo = pSkaEffect->GetInstancePosition();
							break;
						}
					}
					if(num == SkillEffectInfo()->iFireDelayCount)
					{
						m_pSkillReadyEffect->Stop(0.04f);
					}
				}
				if( m_dcEnemies.Count() > 0 )
				{
					for( ENTITIES_ITERATOR it = m_dcEnemies.vectorSelectedEntities.begin();
						it != m_dcEnemies.vectorSelectedEntities.end(); ++it )
					{
						CEntity *pEn = (*it);
						if(pEn != NULL && pEn->IsFlagOff(ENF_DELETED))
						{
							ShotMagicContinued(this, lastEffectInfo, FLOATquat3D(1,0,0,0)
										, pEn, SkillEffectInfo()->fMissileSpeed
										, SkillEffectInfo()->szEffectNameHit, SkillEffectInfo()->szEffectNameMissile
										, false, 3);
						}
					}
				}
				else
				{
					ShotMagicContinued(this, lastEffectInfo, FLOATquat3D(1,0,0,0)
										, m_penEnemy, SkillEffectInfo()->fMissileSpeed
										, SkillEffectInfo()->szEffectNameHit, SkillEffectInfo()->szEffectNameMissile
										, false, 3);
				}
			} break;
		case 4/*MT_INVISIBLE*/:		{} break;//안쓰임. 캐릭터, 플레이어쪽도 미구현.
		case 5/*MT_MAGIC*/:			{} break;//아직 안쓰임
		case 6/*MT_INVERT*/:		{} break;//아직 안쓰임
		case 7/*MT_MAGECUTTER*/:	{} break;//안쓰임, 캐릭 전용
		case 8/*MT_DIRECTDAMAGE*/:
			{
				StartEffectGroup(SkillEffectInfo()->szEffectNameHit
					, &m_penEnemy->en_pmiModelInstance->m_tmSkaTagManager
					, _pTimer->GetLerpedCurrentTick());
				if( m_dcEnemies.Count() > 0 )
				{
					DamagedTargetsInRange(this, m_dcEnemies, DMT_EXPLOSION, 1, FLOAT3D(0,0,0), TRUE);
					m_dcEnemies.Clear();
				}
				else
				{
					this->InflictDirectDamage( m_penEnemy, this, DMT_NONE, 1, FLOAT3D(0,0,0), FLOAT3D(0,0,0) );
				}
			} break;
		case 9/*MT_NOTHING*/:		{} break;
		//-----boss mob hardcoding area begin-----//
		case 21://baal skill 01, like meteo strike
			{
				CSkill &skill = _pNetwork->GetSkillData(m_nCurrentSkillNum);
				static INDEX s_iFireBallCount = 10;
				static FLOAT s_fFallHeight = 3;
				static FLOAT s_fFallHeightVariation = 3;
				static FLOAT s_fSpeed = 5;
				static FLOAT s_fSpeedVariation = 5;
				for(INDEX i=0; i<s_iFireBallCount; ++i)
				{
					FLOAT3D pos = CRandomTable::Instance().GetRndCylinderPos() * skill.GetAppRange();
					pos(2) = CRandomTable::Instance().GetRndFactor() * s_fFallHeightVariation + s_fFallHeight;
					pos += m_penEnemy->GetPlacement().pl_PositionVector;
					ShotFallDown(pos, FLOAT3D(0,1,0), s_fSpeed + s_fSpeedVariation * CRandomTable::Instance().GetRndFactor()
								, SkillEffectInfo()->szEffectNameHit, SkillEffectInfo()->szEffectNameMissile, FALSE);
				}
			} break;
		//-----boss mob hardcoding area end-----//
		default:
			{
				ASSERTALWAYS("Something wrong... Plz check fire obj type of this skill.");
			} break;
		}
	}

procedures:
/************************************************************
 *                A T T A C K   E N E M Y                   *
 ************************************************************/
	Fire(EVoid) : CPetBase::Fire
	{
		jump CPetBase::Fire();
	}

	Hit(EVoid) : CPetBase::Hit 
	{	
		autocall NormalAttack() EEnd;		
		return EReturn();
	};
		
	//0628 kwon
	AttackTarget(EVoid) : CPetBase::AttackTarget // 이 프로시저를 call하기전에 SetTargetEntity()가 선행되어야 한다.
	{
		m_vDesiredPosition = PlayerDestinationPos();
		
		INDEX attackAnimID = -1;
		const int iAttackMotion = m_nAttackCnt % MOB_ATTACK_MOTION_NUM;
		if( iAttackMotion == 0 )
		{
			attackAnimID = iAnimPet_Attack;
		}		
		m_pSkillEffectInfo = new sSkillEffectInfo;
		SkillEffectInfo()->InitForNormalAttack(_pNetwork->GetMobData(0), attackAnimID);

		if(strlen(SkillEffectInfo()->szEffectNameHit) == 0) {SkillEffectInfo()->szEffectNameHit = "Normal Hit";}
		if(SkillEffectInfo()->iFireDelayCount == 0)
		{
			SkillEffectInfo()->iMissileType = CSkill::MT_NONE;
			SkillEffectInfo()->iFireDelayCount = 1;
			SkillEffectInfo()->fFireDelay[0] = GetAnimLength(iAnimPet_Attack)/3;
		}
		autocall SkillAndMagicAnimation() EReturn;

		return EReturn();
	};

	SkillingTarget(EVoid) : CPetBase::SkillingTarget // 이 프로시저를 call하기전에 SetTargetEntity()가 선행되어야 한다.
	{
		m_vDesiredPosition = PlayerDestinationPos();
	
/*
		if(HardCodingSkill())
		{
			autocall HardCodingSkillAnimation() EReturn;
			return EReturn();
		}
*/
		
		CSkill &skill = _pNetwork->GetSkillData(m_nCurrentSkillNum);
		m_pSkillEffectInfo = new sSkillEffectInfo;
		SkillEffectInfo()->InitForSkillAttack(skill);
		autocall SkillAndMagicAnimation() EReturn;
	
		return EReturn();
	};

	// NOTE : 애완동물의 SOCIAL 애니메이션???
	ActionAnimation(EVoid): CPetBase::ActionAnimation
	{
		INDEX iActionSkill = -1;
		// FIXME : 하드 코딩으로 처리된 부분...
		if( m_nPlayActionNum == 271 )	// 고개 흔들기.
		{
			iActionSkill = iAnimPet_Skill01;		
		}
		else if( m_nPlayActionNum == 272 ) // 뒷발질 
		{
			iActionSkill = iAnimPet_Skill02;
		}
		else if( m_nPlayActionNum == 273 ) // 말울음
		{
			iActionSkill = iAnimPet_Skill03;
		}
		else if( m_nPlayActionNum == 284 ) //	로어
		{
			iActionSkill = iAnimPet_Skill01;
		}
		else if( m_nPlayActionNum == 285 ) //	애교
		{
			iActionSkill = iAnimPet_Skill03;
		}
		else if( m_nPlayActionNum == 286 ) //	날개짓
		{
			iActionSkill = iAnimPet_Skill02;
		}
		else if( m_nPlayActionNum == 287 ) //	불뿜기
		{
			iActionSkill = iAnimPet_Skill05;
		}
		else if( m_nPlayActionNum == 288 ) //	구애
		{
			iActionSkill = iAnimPet_Skill04;
		}
		else if( m_nPlayActionNum == ACTION_NUM_PICKITEM )		// 아이템 줍기
		{
			iActionSkill = iAnimPet_Pick;
		}

		//if( iActionSkill != -1 && m_iPetAge != 2)
		//{
			GetModelInstance()->AddAnimation(iActionSkill,AN_CLEAR,1.0f,0);	
			GetModelInstance()->AddAnimationChild(iActionSkill,AN_CLEAR,1.0f,0);	
			m_fActionAnimTime = GetModelInstance()->GetAnimLength(iActionSkill);

			autowait(m_fActionAnimTime-0.1f);		

			// 내 펫인데, 알아서 움직일때만 상태를 Clear해줌.
			// 타 캐릭터 펫인데 Clear하면 애니메이션이 중단되는 현상 있음.
			//if( m_bUseAI && IsFirstExtraFlagOn(ENF_EX1_CURRENT_PET))
			//{
				//NewClearState(0.1f);  
			//}
		//}		
		
		StandingAnim();
			
		return EReturn();
	};

	AttackAnimation(EVoid) //0628
	{
		const int iAttackMotion = m_nAttackCnt % MOB_ATTACK_MOTION_NUM;
		if( iAttackMotion == 0 )
		{
			GetModelInstance()->AddAnimationChild(iAnimPet_Attack, AN_CLEAR, 1.0f, 0);	
		}		

		m_fAttackFrequency = 0.25f;
		m_fAttackStartTime = _pTimer->CurrentTick();
		m_fImpactTime = GetAnimLength(iAnimPet_Attack)/3;//0630 우선 공격 애니메이션의 절반에서 임팩트.

		while(_pTimer->CurrentTick() - m_fAttackStartTime < GetAnimLength(iAnimPet_Attack)*0.8f){

			wait(m_fAttackFrequency) 
			{
				on (EBegin) : 
				{					
					if(m_bMoving)
					{						
					//	m_bStop = FALSE;		
					//	m_bAttack = FALSE;
					//	SetNoTargetEntity();	
						return EReturn();
					}

					m_fMoveSpeed = 0.0f; 
					ULONG ulFlags = SetDesiredMovement(); 				

					if(_pTimer->CurrentTick() - m_fAttackStartTime >m_fImpactTime) 
					{					
						SendEvent(EPetAttackDamage());
						m_fImpactTime = 1000.0f;//충분히 긴시간.
					}

					resume; 
				}
				on (EPetBaseDamage eEBDamage) : 
				{
					// if confused
					m_fDamageConfused -= eEBDamage.fAmount;
					if (m_fDamageConfused < 0.001f) 
					{
						m_fDamageConfused = m_fDamageWounded;
						// play wounding animation
						INDEX animSet, animIdx;
						static INDEX animId = ska_StringToID("mldam");//HardCoding
						if(this->en_pmiModelInstance
						&& this->en_pmiModelInstance->FindAnimationByID(animId, &animSet, &animIdx))
						{
							//타격 모션
							this->en_pmiModelInstance->AddAnimationChild(animId, AN_REMOVE_ON_END, 1.0f, 0);
						}
					}
					resume;
				}
				on (EPetAttackDamage eAttackDamage) : 
				{
//						CPrintF("Attack Damage!!! \n");
//0702 kwon
						CEntity *pen = m_penEnemy;
						FLOAT3D vCurrentCenter(((EntityInfo*)(this->GetEntityInfo()))->vTargetCenter[0],
							((EntityInfo*)(this->GetEntityInfo()))->vTargetCenter[1],
							((EntityInfo*)(this->GetEntityInfo()))->vTargetCenter[2] );
						FLOAT3D vCurrentPos = this->en_plPlacement.pl_PositionVector + vCurrentCenter;
						FLOAT3D vTargetCenter(0, 0, 0);
						FLOAT3D vTargetPos(0, 0, 0);
						FLOAT3D vDirection(0, 0, 0);
						FLOAT size = 0;
						if(pen != NULL && pen->GetFlags()&ENF_ALIVE)
						{
							if(pen->en_RenderType == RT_SKAMODEL)
							{
								// Bounding Box를 이용하여 Effect를 찍을 위치를 찾는다.
								FLOATaabbox3D bbox;
								pen->GetModelInstance()->GetCurrentColisionBox(bbox);
								FLOAT3D vRadius = (bbox.maxvect - bbox.minvect) * 0.5f * 0.4f/*단순히 원으로 하면 너무 큼. 사이즈 조절 상수*/;
								size = vRadius.Length();
							}
							vTargetCenter = FLOAT3D(((EntityInfo*)(pen->GetEntityInfo()))->vTargetCenter[0],
								((EntityInfo*)(pen->GetEntityInfo()))->vTargetCenter[1],
								((EntityInfo*)(pen->GetEntityInfo()))->vTargetCenter[2] );
							vTargetPos = pen->en_plPlacement.pl_PositionVector + vTargetCenter;
							vDirection = vTargetPos - vCurrentPos;
							vDirection.Normalize();
							vTargetPos -= vDirection * size;
						}
						vDirection.Normalize();

//강동민 수정 시작 싱글 던젼 작업	07.25
					// Enemy에 ReceiveDamage를 전송합니다.
					// ReceiveDamage에서 Health가 0보다 작으면 엔티티를 Destroy하는 부분이 있음.
					this->InflictDirectDamage(m_penEnemy, this , DMT_CLOSERANGE, (int)1, vTargetPos, vDirection);
					// FIXME:
					((CLiveEntity*)((CEntity*)m_penEnemy))->SetHealth(1000.0f);
//강동민 수정 끝 싱글 던젼 작업		07.25
						m_fImpactTime = 1000.0f;//충분히 긴시간.

						stop;
				}
				on (ETimer) : { stop; }			
			}
		}

		if(m_penKillEnemy!=NULL)
		{
			if(((CUnit*)((CEntity*)m_penKillEnemy))->m_nCurrentHealth <= 0)
			{
				((CUnit*)((CEntity*)m_penKillEnemy))->DeathNow();
			}
			m_penKillEnemy = NULL;
		}
			

		m_bAttack = FALSE;
		m_bMoving = FALSE;
		m_bStop = FALSE;		
		StandingAnim();
		SetNoTargetEntity();	

		return EReturn();	
	};

	SkillAndMagicAnimation(EVoid)
	{
		if(SkillEffectInfo()->dwValidValue != 0)
		{
			SkillEffectInfo()->dwValidValue = 0xBAD0BEEF;
			delete m_pSkillEffectInfo;
			m_pSkillEffectInfo = NULL;
			return EReturn();
		}

		m_fAttackFrequency = 0.05f;
		m_fAttackStartTime = _pTimer->GetLerpedCurrentTick();
		m_fImpactTime = GetAnimLength(iAnimPet_Attack)/3;

		GetModelInstance()->AddAnimationChild(SkillEffectInfo()->iAnimatioID,AN_CLEAR,1.0f,0);	

		m_nEffectStep = 1;
		m_pSkillReadyEffect = NULL;
		m_pSkillReadyEffect = StartEffectGroup(SkillEffectInfo()->szEffectNameCast
			, &en_pmiModelInstance->m_tmSkaTagManager
			, _pTimer->GetLerpedCurrentTick());

		while(_pTimer->GetLerpedCurrentTick() - m_fAttackStartTime < GetAnimLength(SkillEffectInfo()->iAnimatioID)*0.8f)
		{
			wait(m_fAttackFrequency) 
			{
				on (EBegin) : 
				{					
					if(m_bMoving || m_bStop)
					{
						DestroyEffectGroupIfValid(m_pSkillReadyEffect);
						GetModelInstance()->AddAnimationChild(iAnimPet_Idle1,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);							
						SkillEffectInfo()->dwValidValue = 0xBAD0BEEF;
						delete m_pSkillEffectInfo;
						m_pSkillEffectInfo = NULL;
						return EReturn();
					}
											
					m_fMoveSpeed = 0.0f;
					if(m_penEnemy.ep_pen != this)
					{
						SetDesiredMovement();
					}

					FLOAT time = _pTimer->GetLerpedCurrentTick() - m_fAttackStartTime;
					if(time >= SkillEffectInfo()->fFireDelay[0] && m_nEffectStep == 1 && SkillEffectInfo()->iFireDelayCount > 0)
					{
						++m_nEffectStep;
						SkillAndAttackFire(1);
					}
					if(time >= SkillEffectInfo()->fFireDelay[1] && m_nEffectStep == 2 && SkillEffectInfo()->iFireDelayCount > 1)
					{
						++m_nEffectStep;
						SkillAndAttackFire(2);
					}
					if(time >= SkillEffectInfo()->fFireDelay[2] && m_nEffectStep == 3 && SkillEffectInfo()->iFireDelayCount > 2)
					{
						++m_nEffectStep;
						SkillAndAttackFire(3);
					}
					if(time >= SkillEffectInfo()->fFireDelay[3] && m_nEffectStep == 4 && SkillEffectInfo()->iFireDelayCount > 3)
					{
						++m_nEffectStep;
						SkillAndAttackFire(4);
					}
					resume;
				}
				on (EEnemyBaseDamage eEBDamage) : 
				{
					// if confused
					m_fDamageConfused -= eEBDamage.fAmount;
					if (m_fDamageConfused < 0.001f) 
					{
						m_fDamageConfused = m_fDamageWounded;
						// play wounding animation
						INDEX animSet, animIdx;
						static INDEX animId = ska_StringToID("mldam");//HardCoding
						if(this->en_pmiModelInstance
						&& this->en_pmiModelInstance->FindAnimationByID(animId, &animSet, &animIdx))
						{
							//타격 모션
							this->en_pmiModelInstance->AddAnimationChild(animId, AN_REMOVE_ON_END, 1.0f, 0);
						}
					}
					resume;
				}
				on (ETimer) : { stop; }
			}
		}

		m_nEffectStep = 0;
		SkillEffectInfo()->dwValidValue = 0xBAD0BEEF;
		delete m_pSkillEffectInfo;
		m_pSkillEffectInfo = NULL;
		return EReturn();
	};

	// Normal attack
	NormalAttack(EVoid)
	{
		//StandingAnimFight();
		StandingAnim();
		autowait(0.8f + FRnd()*0.25f);

		INDEX attackAnimID = -1;
		const int iAttackMotion = rand() % MOB_ATTACK_MOTION_NUM;
		if( iAttackMotion == 0 )
		{
			attackAnimID = iAnimPet_Attack;
		}
		
		m_nAttackCnt = attackAnimID;

		autocall AttackTarget() EEnd;

		//autowait(0.60f);

		return EEnd();
	};

/************************************************************
 *                       M  A  I  N                         *
 ************************************************************/
	Main(EVoid) 
	{
		if(GetModelInstance() == NULL)
		{ 
			InitAsSkaEditorModel();
			SetSkaModel("Models\\Editor\\Ska\\Axis.smc");			
			return EEnd();
		}
//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
		if(GetModelInstance() != NULL)
		{
			CSkaTag tag;
			tag.SetName("__ROOT");
			tag.SetOffsetRot(GetEulerAngleFromQuaternion(en_pmiModelInstance->mi_qvOffset.qRot));
			GetModelInstance()->m_tmSkaTagManager.Register(&tag);
			FLOATaabbox3D aabb;
			GetModelInstance()->GetAllFramesBBox(aabb);
			tag.SetName("CENTER");
			tag.SetOffsetRot(GetEulerAngleFromQuaternion(en_pmiModelInstance->mi_qvOffset.qRot));
			tag.SetOffsetPos(0, aabb.Size()(2) * 0.5f * GetModelInstance()->mi_vStretch(2), 0);
			GetModelInstance()->m_tmSkaTagManager.Register(&tag);
			tag.SetName("__TOP");
			tag.SetOffsetRot(GetEulerAngleFromQuaternion(en_pmiModelInstance->mi_qvOffset.qRot));
			tag.SetOffsetPos(0, aabb.Size()(2) * GetModelInstance()->mi_vStretch(2), 0);
			GetModelInstance()->m_tmSkaTagManager.Register(&tag);
		}
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)		
		
		SetFlags(GetFlags()|ENF_ALIVE);		
		
		en_sbNetworkType = MSG_CHAR_PET;

		// 탈것이 아닐때...
		if(m_iPetAge != 2)
		{
			SetPhysicsFlags(EPF_ONBLOCK_CLIMBORSLIDE
				| EPF_TRANSLATEDBYGRAVITY
				//| EPF_PUSHABLE
				| EPF_MOVABLE				
				| EPF_ABSOLUTETRANSLATE );
			//SetPhysicsFlags(GetPhysicsFlags() &~EPF_PUSHABLE);
			SetCollisionFlags(ECF_MODEL_NO_COLLISION);
		}
		else
		{
			// 탈것일때...
			// declare yourself as a model		
			SetPhysicsFlags(EPF_MODEL_WALKING|EPF_HASLUNGS);		
			SetPhysicsFlags(EPF_MOVABLE);
			SetPhysicsFlags(GetPhysicsFlags() & ~EPF_TRANSLATEDBYGRAVITY);
			SetPhysicsFlags(EPF_MODEL_WALKING);
		}
		
		// set initial vars
		en_tmMaxHoldBreath = 60.0f;
		en_fDensity = 1000.0f;    // same density as water - to be able to dive freely		

		SetFlagOn(ENF_RENDERREFLECTION);

		// setup moving speed
		m_aWalkRotateSpeed		= AngleDeg(1800.0f);
		m_aAttackRotateSpeed	= AngleDeg(1800.0f);				
		m_aCloseRotateSpeed		= AngleDeg(1800.0f);

		// setup attack distances
		m_fAttackFireTime		= 2.0f;
		m_fCloseFireTime		= 2.0f;
		m_fBlowUpAmount			= 80.0f;
		m_fBodyParts			= 4;
		m_fDamageWounded		= 0.0f;
		
		// set stretch factors for height and width
		GetModelInstance()->StretchModel(FLOAT3D(m_fStretch, m_fStretch, m_fStretch));
		GetModelInstance()->mi_bRenderShadow = TRUE;

		SetHealth(10000.0f);//0807 몹의 체력.
		ModelChangeNotify();
		//StandingAnim();
		
//안태훈 수정 시작	//(Open beta)(2004-12-14)
		if(GetModelInstance())
		{
			CSkaTag tag;
			tag.SetName("__ROOT");
			tag.SetOffsetRot(GetEulerAngleFromQuaternion(en_pmiModelInstance->mi_qvOffset.qRot));
			GetModelInstance()->m_tmSkaTagManager.Register(&tag);
			tag.SetName("__TOP");
			tag.SetOffsetRot(GetEulerAngleFromQuaternion(en_pmiModelInstance->mi_qvOffset.qRot));
			FLOATaabbox3D aabb;
			GetModelInstance()->GetAllFramesBBox(aabb);
			tag.SetOffsetPos(0, aabb.Size()(2), 0);
			GetModelInstance()->m_tmSkaTagManager.Register(&tag);
		}
//안태훈 수정 끝	//(Open beta)(2004-12-14)
		
		if(m_bUseAI)
		{
			SetTargetEntity(CEntity::GetPlayerEntity(0));
		}

		// continue behavior in base class
		jump CPetBase::MainLoop();
	};
};
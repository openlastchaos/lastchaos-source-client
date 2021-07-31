
19600
%{
#include "StdH.h"

#include <Engine/Effect/CEffectGroup.h>
#include <Engine/Effect/CEffectGroupManager.h>
#include <Engine/Effect/CSkaEffect.h>
#include <Engine/Graphics/Color.h>
#include <Engine/Entities/ItemData.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/ItemTarget.h>
#include <deque>
#include <Engine/Entities/Skill.h>
#include <Engine/Network/MessageDefine.h>
#include "EntitiesMP/Common/CharacterDefinition.h"
#include "EntitiesMP/PlayerWeapons.h"
// #include <Engine/SlaveInfo.h>
#include <Engine/Interface/UIManager.h>
	
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

// struct sPetWear
// {
// 	INDEX m_nPetWear[WILDPET_WEAR_TOTAL];
// 	INDEX m_nPetWearPlus[WILDPET_WEAR_TOTAL];
//};

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
void GetTargetDirect(CEntity *penMe, CEntity *penTarget, FLOAT3D &vTargetPos, FLOAT3D &vDirection)
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

%}

uses "EntitiesMP/WildPetBase";
uses "EntitiesMP/Global";
uses "EntitiesMP/BasicEffects";
uses "EntitiesMP/EnemyBase";
uses "EntitiesMP/Enemy";

event EWildPetAttackDamage {};

%{
// info structure
static EntityInfo eiWildPetBoss = {
	EIBT_FLESH, 200.0f,
	//0.0f, 1.75f*m_fStretch, 0.0f,     // source (eyes)
	//0.0f, 1.30f*m_fStretch, 0.0f,     // target (body)
	0.0f, 1.75f*1.0f, 0.0f,     // source (eyes)
	0.0f, 1.30f*1.0f, 0.0f,     // target (body)
};

//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
static INDEX g_iNPCMinBrightness = 32;	//명도 0~240
void CWildPet_OnInitClass(void)
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

class CWildPet: CWildPetBase {
name      "WildPet";
thumbnail "";
features  "ImplementsOnInitClass", "HasName", "IsTargetable";

properties:	
	3 FLOAT m_fBodyAnimTime		= -1.0f,	
	5 FLOAT m_fStretch			= 1.0f,		
	100 INDEX iAnimWildPet_Walk		= -1,	
	101 INDEX iAnimWildPet_Idle1		= -1,
	102 INDEX iAnimWildPet_Idle2		= -1,
	103 INDEX iAnimWildPet_Run			= -1,
	104 INDEX iAnimWildPet_Death		= -1,	
	105 INDEX iAnimWildPet_Damage		= -1,
	106 INDEX iAnimWildPet_Attack1		= -1,		
	107	INDEX iAnimWildPet_Attack2		= -1,		
	108 INDEX iAnimWildPet_NormalBox	= -1,	
//	109 INDEX iAnimWildPet_LevelUp		= -1, // 레벨업 동작 대신 배고픔 동작
	109 INDEX iAnimWildPet_Hungry		= -1, // 배고픔 동작

	110 INDEX	iNetwarkID				= -1,
//	111	CTString	strPetName			= "",
	112	INDEX	iOwnerIndex				= -1,
//	112 CTString	strOwnerName		= "",
	113 CPlacement3D plPetPlacement		= CPlacement3D(FLOAT3D(0.0f, 0.0f, 0.0f), ANGLE3D(0.0f, 0.0f, 0.0f)),
	114	INDEX	iYLayer					= -1,
	115 INDEX	sbAttributePos			= -1,
	116	INDEX	iAttackAnim				= -1,

	200	CTString	m_strName	"Name" = "WildPet",			
	225	FLOAT	m_fActionAnimTime = 0.0f,
	247 enum	EventType	m_EventType			"Event Type"		= EVENT_NONE,
	248 CEntityPointer m_penEventTarget		"Event Target",					// 트리거를 통해서 Blind를 해제하도록 함.	
	251 BOOL	m_bIsRun		= FALSE,
	252 BOOL	m_bDie	= FALSE,
	253 INDEX	m_nSkillChack = 0,			// 스킬 예약
	{
		CEffectGroup *m_pSkillReadyEffect;
		CStaticArray<FLOAT3D> m_aDeathItemPosDrop;
		CStaticArray<CItemTarget> m_aDeathItemTargetDrop;
		CStaticArray<void *> m_aDeathItemDataDrop;
		CStaticArray<TagInfo>	m_saLastEffectInfo;
		INDEX	m_nMagicAttackSequence;
		void	*m_pSkillEffectInfo;
		INDEX	m_nEffectStep;
		CPetAnimation	m_PetAnimationIndices;
		FLOAT	m_fprevStretch;	
	//	sPetWear	PetWearInfo;
		BOOL					m_bPlayAttackAnim;

	}

components:	
	// 사운드 처리가 가장 문제되는 부분.
functions:

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
			if(	( _pNetwork->_WildPetInfo.bIsActive && ( _pNetwork->_WildPetInfo.m_nFaith <= 0 || _pNetwork->_WildPetInfo.m_nStm <= 0 ) ) 
				|| m_bStuned || m_bHold )
			{
				return FALSE;
			}		
		}
		return TRUE;
	};

		//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
	void GetTargetDirection(FLOAT3D &vTargetPos, FLOAT3D &vDirection)
	{
		if(m_penEnemy.ep_pen != NULL)
		{
			CEntity *pen = m_penEnemy;
			FLOAT3D vCurrentCenter(((EntityInfo*)(this->GetEntityInfo()))->vTargetCenter[0],
			((EntityInfo*)(this->GetEntityInfo()))->vTargetCenter[1],
			((EntityInfo*)(this->GetEntityInfo()))->vTargetCenter[2] );
			FLOAT3D vCurrentPos = this->en_plPlacement.pl_PositionVector + vCurrentCenter;
			FLOAT3D vTargetCenter(0, 0, 0);
			vTargetPos = FLOAT3D(0, 0, 0);
			vDirection = FLOAT3D(0, 0, 0);
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
		}
	}

	void OnInitialize(const CEntityEvent &eeInput)
	{
		CWildPetBase::OnInitialize(eeInput);
	};

	void OnEnd(void) 
	{
		CWildPetBase::OnEnd();		
	};

		/* Entity info */
	void *GetEntityInfo(void)
	{
		return &eiWildPetBoss;
	};

	void Precache(void) 
	{
		CWildPetBase::Precache();

		if(en_pmiModelInstance) { 
			en_pmiModelInstance->m_tmSkaTagManager.SetOwner(this); 
		}

	};

	virtual BOOL IsWildPet(void) const
	{
		return TRUE;
	}

	// 펫이 데미지를 입힐경우...
	void InflictDirectDamage(CEntity *penToDamage, CEntity *penInflictor, enum DamageType dmtType,
	FLOAT fDamageAmmount, const FLOAT3D &vHitPoint, const FLOAT3D &vDirection)
	{
		if(penToDamage)
		{
			_pUIMgr->ShowDamage( penToDamage->en_ulID );
			// Date : 2005-11-16(오후 2:05:09), By Lee Ki-hwan
			//_pUIMgr->SetDamageState();

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
		
		if(penToDamage)
		{
			CEntity::InflictDirectDamage(penToDamage, penInflictor, dmtType,
				fDamageAmmount,vHitPoint, vDirection);	
		}

		//((CLiveEntity*)((CEntity*)penToDamage))->SetHealth(1000.0f);

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
		_pUIMgr->ShowDamage( en_ulID );
		//CWildPetBase::ReceiveDamage(penInflictor, dmtType, fDamageAmmount, vHitPoint, vDirection);
	};

	//--------------------------------------------------------------
	// 애니메이션을 설정하는 함수들...
	//--------------------------------------------------------------
	// damage anim
	INDEX AnimForDamage(FLOAT fDamage) 
	{
		ASSERT(iAnimWildPet_Damage		!= -1	&& "Invalid Animation");
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");

		//0820 가만히 서있을때만 데미지 모션이 나와야 된다. 꼭! 임시 주석 처리
		if(IsAnimationPlaying(iAnimWildPet_Idle1) || IsAnimationPlaying(iAnimWildPet_Idle2) )
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
		GetModelInstance()->AddAnimation(iAnimWildPet_Damage,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);
		//GetModelInstance()->AddAnimationChild(iAnimWildPet_Damage,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);	
		
		return iAnimWildPet_Damage;
	};

	// death
	INDEX AnimForDeath(void)
	{	
		ASSERT(iAnimWildPet_Death		!= -1	&& "Invalid Animation");
	
		//DropDeathItem();//1019
				
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");
		NewClearState(CLEAR_STATE_LENGTH);    
		GetModelInstance()->AddAnimation(iAnimWildPet_Death,AN_NORESTART,1,0);

		return iAnimWildPet_Death;
	};

	// 죽을때 뭔가가...
	FLOAT WaitForDust(FLOAT3D &vStretch)
	{
		//0317 kwon
		vStretch = FLOAT3D(1,1,1);		
		if(GetModelInstance()->IsAnimationPlaying(iAnimWildPet_Death)) 
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
		float frandom = FRnd();
		if( frandom < 0.5f )
		{
			iAttackAnim = iAnimWildPet_Idle1;
		}
		else
		{
			iAttackAnim = iAnimWildPet_Idle2;
		}
		m_bIsRun = FALSE;
		ASSERT(iAttackAnim	!= -1	&& "Invalid Animation");		
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");
		GetModelInstance()->AddAnimation(iAttackAnim,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);		
		//GetModelInstance()->AddAnimationChild(iAttackAnim,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);		
	};

	void RunningAnim(void) 
	{
		m_bIsRun = TRUE;
		ASSERT(iAnimWildPet_Run		!= -1	&& "Invalid Animation");
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");
		GetModelInstance()->AddAnimation(iAnimWildPet_Run,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);
		//GetModelInstance()->AddAnimationChild(iAnimWildPet_Run,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);
	};

	void WalkingAnim(void) 
	{
		m_bIsRun = FALSE;
		ASSERT(iAnimWildPet_Walk		!= -1	&& "Invalid Animation");
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");
		GetModelInstance()->AddAnimation(iAnimWildPet_Walk,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);		
		//GetModelInstance()->AddAnimationChild(iAnimWildPet_Walk,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);		
	};

/*	void LevelUpAnim(void) // 펫에서 레벨업 동작은 사용하지 않는다. 대신에 배고픔 동작으로
	{
		ASSERT(iAnimWildPet_LevelUp		!= -1	&& "Invalid Animation");
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");
		GetModelInstance()->AddAnimation(iAnimWildPet_LevelUp,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);				
	};*/
	
	void HungryAnim(void)
	{
		ASSERT(iAnimWildPet_Hungry		!= -1	&& "Invalid Animation");
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");
		GetModelInstance()->AddAnimation(iAnimWildPet_Hungry,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);				
	};

// FIXME : 중복되는 부분이 많은 코드임.  정리 좀 하삼~~~!!!
	virtual BOOL UseSkill( int iSkillIndex )	
	{
		// 배고픔이 0이라면 못움직임.
		if(	( _pNetwork->_WildPetInfo.bIsActive && ( _pNetwork->_WildPetInfo.m_nFaith <= 0 || _pNetwork->_WildPetInfo.m_nStm <= 0 ) ) 
			|| m_bStuned || m_bHold )
		{
			return FALSE;
		}

		// 공격 애니메이션 중에는 스킬을 사용할 수 없음.
		if( m_bPlayAttackAnim )	
		{ 
			m_nSkillChack = iSkillIndex;   //ttos : 소환수 스킬 예약함
			return FALSE; 
		}
		
		// 스킬 딜레이 적용하기.
		CSkill &SkillData = _pNetwork->GetSkillData(iSkillIndex);
		m_dcEnemies.Clear();

		// 버프가 걸린 상태에서 스킬 사용 불가.
		if( m_bCannotUseSkill ) { return FALSE; }		

		BOOL bHelp = SkillData.GetFlag() & SF_FORHELP;		

		// 타겟이 필요한 스킬
		if( SkillData.IsNeedTarget() )
		{
			if( !m_penEnemy )		{ return FALSE; }
			if( m_penEnemy && m_penEnemy->GetFlags() & ENF_ALIVE)
			{
				// HELP 스킬이 아닌데 플레이어를 공격 할수는 없음.
				if( !bHelp && m_penEnemy->IsPlayer() )
				{
					return FALSE;
				}

				FLOAT3D vDelta = GetPlacement().pl_PositionVector - m_penEnemy->GetPlacement().pl_PositionVector;
				if( vDelta.Length() > SkillData.GetFireRange() )
				{
					m_nSkillChack = iSkillIndex;   //ttos : 소환수 스킬 예약함
					return FALSE;
				}

				SkillData.SetStartTime();
				// 여기서 범위 공격 처리해줄것...
				if( SkillData.GetTargetNum() > 1 )
				{
					if( _pNetwork->m_bSingleMode )
					{
						CPlacement3D Myplacement = GetPlacement();					
						_pNetwork->SendMoveMessage(this, Myplacement, m_fMoveSpeed);
					}

					// 주 타겟 에너미.
					m_dcEnemies.Clear();
					m_dcEnemies.Add(m_penEnemy);

					_pNetwork->SendSlaveSkillMessageInContainer(iSkillIndex, this, m_dcEnemies, FALSE);

					const char chTargetType = SkillData.GetTargetType();
					FLOAT fAngle = 360.0f;
					// 자기 중심으로 120도
					if( chTargetType == CSkill::STT_TARGET_D120 )
					{
						fAngle = 60.0f;
					}
					
					m_dcEnemies.Add(m_penEnemy);

					_pNetwork->FindTargetsInRange(this, m_penEnemy, m_dcEnemies, 
													SkillData.GetAppRange(), SkillData.GetTargetNum() - 1, 360.0f, iSkillIndex, chTargetType );
					
					_pNetwork->SendSlaveSkillMessageInContainer(iSkillIndex, this, m_dcEnemies, TRUE);
				}
				else
				{
					if( _pNetwork->m_bSingleMode )
					{
						CPlacement3D Myplacement = GetPlacement();					
						_pNetwork->SendMoveMessage(this, Myplacement, m_fMoveSpeed);
					}

					_pNetwork->SendSlaveSkillMessage( iSkillIndex, this, m_penEnemy, FALSE);
					_pNetwork->SendSlaveSkillMessage( iSkillIndex, this, m_penEnemy, TRUE);
				}
			}			
		}
		// 셀프 스킬
		else
		{
			SkillData.SetStartTime();
			if( SkillData.GetTargetNum() > 1 )
			{
				if( _pNetwork->m_bSingleMode )
				{
					CPlacement3D Myplacement = GetPlacement();					
					_pNetwork->SendMoveMessage(this, Myplacement, m_fMoveSpeed);
				}

				// 주 타겟 에너미.
				m_dcEnemies.Clear();
				m_dcEnemies.Add(this);

				_pNetwork->SendSlaveSkillMessageInContainer(iSkillIndex, this, m_dcEnemies, FALSE);

				const char chTargetType = SkillData.GetTargetType();

				// 자기 중심으로 멀티 공격
				if( chTargetType == CSkill::STT_SELF_RANGE )
				{
					_pNetwork->FindTargetsInRange(this, this, m_dcEnemies, 
						SkillData.GetAppRange(), SkillData.GetTargetNum() - 1, 360.0f, iSkillIndex );

					_pNetwork->SendSlaveSkillMessageInContainer(iSkillIndex, this, m_dcEnemies, TRUE);						
				}
			}
			else
			{
				if( _pNetwork->m_bSingleMode )
				{
					CPlacement3D Myplacement = GetPlacement();					
					_pNetwork->SendMoveMessage(this, Myplacement, m_fMoveSpeed);
				}

				_pNetwork->SendSlaveSkillMessage( iSkillIndex, this, this, FALSE);
				_pNetwork->SendSlaveSkillMessage( iSkillIndex, this, this, TRUE);
			}
		}

		return TRUE;
	};

	void SkillAnim(INDEX iAnimIndex, DWORD dwFlag, float fSpeed)
	{
		ASSERT(iAnimIndex		!= -1	&& "Invalid Animation");
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");
		//NewClearState(CLEAR_STATE_LENGTH);		
		GetModelInstance()->AddAnimation(iAnimIndex,dwFlag,1,0, fSpeed);
		//((CPlayer*)(CPlayerEntity*)CEntity::GetPlayerEntity(0))->AddAnimationChild(iAnimIndex, dwFlag, 1.0f, 3,
		//	ESKA_MASTER_MODEL_INSTANCE, fSpeed);
	};

	void AttackAnim(void) 
	{		
		float frandom = FRnd();
		if( frandom < 0.5f )
		{
			iAttackAnim = iAnimWildPet_Attack1;
		}
		else
		{
			iAttackAnim = iAnimWildPet_Attack2;
		}

		ASSERT(iAttackAnim		!= -1		&& "Invalid Animation");
		ASSERT(GetModelInstance() != NULL	&& "Invalid ModelInstance");

		static FLOAT	start_attack_time=0;
		static float  m_fBodyAnimTime=0;
		
		if(start_attack_time==0)
		{
			start_attack_time = _pTimer->CurrentTick();
			GetModelInstance()->AddAnimation(iAttackAnim,AN_NORESTART|AN_CLEAR,1.0f,0);
			m_fBodyAnimTime = GetModelInstance()->GetAnimLength(iAttackAnim);
		}
		if(start_attack_time !=0 && (_pTimer->CurrentTick() - start_attack_time >= m_fBodyAnimTime))
		{
			m_bAttack = FALSE;	
			start_attack_time = 0;		
		}
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
		m_WildPetSound.Set3DParameters(30.0f, 5.0f, 1.0f, 1.0f);
	};
	
	void AISerch(void)
	{
		if (_pUIMgr->GetWildPetInfo()->GetAIActive())
		{
			sWildPet_AI_Slot* sPetAI = _pUIMgr->GetWildPetInfo()->GetPetAI();
			INDEX nSlotnum = _pUIMgr->GetWildPetInfo()->GetPetAISlotCount();

			for (int nSlot = 0; nSlot < nSlotnum; nSlot++)
			{
				if (sPetAI[nSlot].m_bActive)
				{
					CItemData &curItem = _pNetwork->GetItemData(sPetAI[nSlot].m_nTargetSlot);
					FLOAT fRange = GetItemRange(curItem.GetPetAISet0());
					int nAISet1 = curItem.GetPetAISet1();					//0:HP 1:MP 2:배고품 3:충성도
					FLOAT fPercent = GetItemPercent(curItem.GetPetAISet2());
					int nTargettype = curItem.GetPetAISet3();

					if(AIResult(fRange, nAISet1, fPercent, nTargettype) )
					{
						if (sPetAI[nSlot].m_nSillType == UBET_SKILL)
						{
							if( !_pUIMgr->GetWildPetInfo()->GetSkillDelay(nSlot))
							{
								if(UseSkill(sPetAI[nSlot].m_nActionSlot))
								{
									m_wPetState = WPET_AI_USESKILL;
									return;
								}
								else if(m_nSkillChack)
								{
									m_wPetState = WPET_AI_NONE;
									return;
								}
							}

						}else if (sPetAI[nSlot].m_nSillType == UBET_ACTION)
						{
							if (sPetAI[nSlot].m_nActionSlot == 44 && nTargettype != 7 /*PET_AI_ITEM*/)
							{
								m_wPetState = WPET_AI_NORMALATTACK;
								return;

							}else if(sPetAI[nSlot].m_nActionSlot == 45 && nTargettype == 7 /*PET_AI_ITEM*/)
							{
								m_wPetState = WPET_AI_ACTION;
								return;
							}
						}
						// 여기까지 그냥 내려왔다면 AI[nSlot]번째 슬롯조건에 맞지 않음 
					}

				}
			}

		}
		m_wPetState = WPET_AI_FOLLOW;
		return;
	}
	// 아이템 데이터의 Set0 Result
	FLOAT GetItemRange(int nset0)
	{
		switch(nset0)
		{
		case 1/*PET_AI_RANGE_3M*/:
			{
				return 3.0f;
			}
		case 2/*PET_AI_RANGE_5M*/:
			{
				return 5.0f;
			}
		case 3/*PET_AI_RANGE_10M*/:
			{
				return 10.0f;
			}
		case 4/*PET_AI_RANGE_20M*/:
			{
				return 20.0f;
			}
		case 5/*PET_AI_RANGE_30M*/:
			{
				return 30.0f;
			}
		}
		return 0.0f;
	}
	// 아이템 데이터의 Set2 Result
	FLOAT GetItemPercent(int nset2)
	{
		switch(nset2)
		{
		case 1/*PET_AI_PERCENT_99*/:
			{
				return 99.0f;
			}
		case 2/*PET_AI_PERCENT_90*/:
			{
				return 90.0f;
			}
		case 3 /*PET_AI_PERCENT_80*/:
			{
				return 80.0f;
			}
		case 4 /*PET_AI_PERCENT_70*/:
			{
				return 70.0f;
			}
		case 5 /*PET_AI_PERCENT_60*/:
			{
				return 60.0f;
			}
		case 6 /*PET_AI_PERCENT_50*/:
			{
				return 50.0f;
			}
		}
		return 0.0f;
	}

	BOOL AIResult(FLOAT fRange, int nSet1, FLOAT fPercent, int nTargettype)
	{
		CEntity* pTmpEnt;

		switch(nTargettype)
		{
		case 1 /*PET_AI_OWNER_ATTACK_ENEMY*/:
			{
				pTmpEnt = ((CPlayer*)CEntity::GetPlayerEntity(0))->GetPlayerWeapons()->m_penRayHitTmp;

				if (pTmpEnt == NULL || !pTmpEnt->IsEnemy() || !pTmpEnt->IsFlagOn(ENF_ALIVE)
					|| pTmpEnt->GetFirstExFlags() & ( ENF_EX1_PEACEFUL | ENF_EX1_PRODUCTION | ENF_EX1_NPC | ENF_EX1_CURRENT_PET | ENF_EX1_CURRENT_WILDPET) )
				{
					return FALSE;
				}

				if (AISubChack(pTmpEnt,nSet1,fPercent))
				{
					
					if (fRange != 0.0f)
					{
						FLOAT fEnemyDistance = CalcDist(pTmpEnt);
						if(fRange > fEnemyDistance)
						{
							m_penEnemy = pTmpEnt;
							return TRUE;
						}

					}else
					{
						m_penEnemy = pTmpEnt;
						return TRUE;
					}
				}
			}break;
/*		case 2 ://*PET_AI_OWNER_CHOICE_ENEMY
			{
				pTmpEnt = ((CPlayer*)CEntity::GetPlayerEntity(0))->GetPlayerWeapons()->m_penRayHitClick;

				if (pTmpEnt == NULL || !pTmpEnt->IsEnemy())
				{
					return FALSE;
				}

				if (AISubChack(pTmpEnt,nSet1,fPercent))
				{
					
					if (fRange != 0.0f)
					{
						FLOAT fEnemyDistance = CalcDist(pTmpEnt);
						if(fRange > fEnemyDistance)
						{
							m_penEnemy = pTmpEnt;
							return TRUE;
						}

					}else
					{
						m_penEnemy = pTmpEnt;
						return TRUE;
					}
				}
			}break;*/
		case 2 /*PET_AI_ENEMY_ATTACK_OWNER*/:
			{// 못찾겠다...

			}break;
		case 3 /*PET_AI_ENEMY_ATTACK_PET*/:
			{

			}break;
		case 4 /*PET_AI_ENEMY*/:
			{// 항상 범위가 정해져야 한다.
				
				_pNetwork->FindTargetsInRangeEx(m_penOwner, m_dcTargets, fRange, 10);

				if (m_dcTargets.IsMember(m_penOwner))
				{
					m_dcTargets.SelectedEntitiesSort();
					m_penEnemy = (*m_dcTargets.vectorSelectedEntities.begin());
					return TRUE;
				}
			}break;
		case 5 /*PET_AI_PET*/:
			{
				pTmpEnt = this;
				if(AISubChack(pTmpEnt, nSet1,fPercent))
				{
					return TRUE;
				}
			}break;
		case 6 /*PET_AI_OWNER*/:
			{
				pTmpEnt = CEntity::GetPlayerEntity(0);
				if(AISubChack(pTmpEnt, nSet1,fPercent))
				{
					return TRUE;
				}
			}break;
		case 7 /*PET_AI_ITEM*/:
			{
				if (fRange != 0.0f)
				{
					SearchNearItem(fRange);
					if(m_dcTargets.Count() > 0) 
					{
						m_penEnemy = (*m_dcTargets.vectorSelectedEntities.begin());
						return TRUE;
					}
				}
				

			}break;
		}
		return FALSE;
	}

	// 가까이에 있는 아이템 목록을 얻습니다.
	void SearchNearItem(FLOAT fRange)
	{		
		m_dcTargets.Clear();

		for(INDEX ipl=0; ipl<_pNetwork->ga_srvServer.srv_aitItem.Count(); ipl++) 
		{
			CItemTarget &it = _pNetwork->ga_srvServer.srv_aitItem[ipl];
			if (it.item_pEntity && ( it.item_yLayer == _pNetwork->MyCharacterInfo.yLayer ) )
			{
				int myLayer		= _pNetwork->MyCharacterInfo.yLayer;
				FLOAT3D vDelta	= GetPlacement().pl_PositionVector - it.item_place;
				vDelta(2) = 0.0f; //0131 높이값 무시.				

				if( vDelta.Length() <= fRange )
				{
					m_dcTargets.Add( it.item_pEntity );
				}
			}
		}
	}

	BOOL AISubChack(CEntity* pcurEnt, int nset1, FLOAT fset2)
	{
		if (nset1 == 0 || fset2 == 0.0f)
		{
			return TRUE;
		}

		switch(nset1)
		{
		case 1 /*PET_AI_HP*/:
			{
				if(pcurEnt->IsCharacter())
				{
					if(_pNetwork->MyCharacterInfo.hp < fset2)
					{
						return TRUE;
					}
				}else if (pcurEnt == this)
				{
					if (_pNetwork->_WildPetInfo.m_nHP < fset2)
					{
						return TRUE;
					}
				} 
				else if(pcurEnt->IsEnemy())
				{

				}
				
			}break;
		case 2 /*PET_AI_MP*/:
			{
				if(pcurEnt->IsCharacter())
				{
					if(_pNetwork->MyCharacterInfo.mp < fset2)
					{
						return TRUE;
					}
				}else if(pcurEnt == this)
				{
					if(_pNetwork->_WildPetInfo.m_nMP < fset2)
					{
						return TRUE;

					}
				}

			}break;
		case 3 /*PET_AI_STM*/:
			{
				if (_pNetwork->_WildPetInfo.m_nStm < fset2)
				{
					return TRUE;
				}
			}break;
		case 4 /*PET_AI_FAITH*/:
			{
				if (_pNetwork->_WildPetInfo.m_nFaith < fset2)
				{
					return TRUE;
				}

			}break;

		}

		return FALSE;
	}

	void SkillAndAttackFire(int num)
	{
		if( !(m_penEnemy.ep_pen != NULL
			&& m_penEnemy->en_pmiModelInstance != NULL
			&& m_penEnemy->GetFlags() & ENF_ALIVE) )
		{
			return;
		}

		// 배고픔이 0이라면 못움직임.
		if(	( _pNetwork->_WildPetInfo.bIsActive && ( _pNetwork->_WildPetInfo.m_nFaith <= 0 || _pNetwork->_WildPetInfo.m_nStm <= 0 ) ) 
			|| m_bStuned || m_bHold )
		{
			return;
		}

		this->InflictDirectDamage(m_penEnemy, this, DMT_NONE, 1, FLOAT3D(0,0,0), FLOAT3D(0,0,0));

		switch(SkillEffectInfo()->iMissileType)
		{
		case 0/*MT_NONE*/:
			{
				FLOAT3D vHitPoint;
				FLOAT3D vHitDir;
				GetTargetDirect(this, m_penEnemy, vHitPoint, vHitDir);

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
	Fire(EVoid) : CWildPetBase::Fire
	{
		jump CWildPetBase::Fire();
	}

	Hit(EVoid) : CWildPetBase::Hit 
	{	
		autocall NormalAttack() EReturn;		
		return EReturn();
	};

//0628 kwon
	AttackTarget(EVoid) : CWildPetBase::AttackTarget // 이 프로시저를 call하기전에 SetTargetEntity()가 선행되어야 한다.
	{
		 		
		m_vDesiredPosition = EnemyDestinationPos();
		if(m_nSkillChack)
		{
			UseSkill(m_nSkillChack);
			return EReturn();
		}
		m_bPlayAttackAnim = TRUE;
		
		INDEX attackAnimID = -1;		
		attackAnimID = iAnimWildPet_Attack1;
		//--
		m_pSkillEffectInfo = new sSkillEffectInfo;
		SkillEffectInfo()->InitForNormalAttack(_pNetwork->GetMobData(0), attackAnimID);

		if(strlen(SkillEffectInfo()->szEffectNameHit) == 0) {SkillEffectInfo()->szEffectNameHit = "Normal Hit";}
		if(SkillEffectInfo()->iFireDelayCount == 0)
		{
			SkillEffectInfo()->iMissileType = CSkill::MT_NONE;
			SkillEffectInfo()->iFireDelayCount = 1;
			SkillEffectInfo()->fFireDelay[0] = GetAnimLength(iAnimWildPet_Attack1)/3;
		}
		//--
		autocall AttackAnimation() EReturn;
		m_bPlayAttackAnim = FALSE;
		return EReturn();                                                                     
		/************************************************************************
		m_vDesiredPosition = PlayerDestinationPos();

		INDEX attackAnimID = -1;
		const int iAttackMotion = m_nAttackCnt % MOB_ATTACK_MOTION_NUM;
		if( iAttackMotion == 0 )
		{
			attackAnimID = iAnimWildPet_Attack1;
		}		
		m_pSkillEffectInfo = new sSkillEffectInfo;
		SkillEffectInfo()->InitForNormalAttack(_pNetwork->GetMobData(0), attackAnimID);

		if(strlen(SkillEffectInfo()->szEffectNameHit) == 0) {SkillEffectInfo()->szEffectNameHit = "Normal Hit";}
		if(SkillEffectInfo()->iFireDelayCount == 0)
		{
			SkillEffectInfo()->iMissileType = CSkill::MT_NONE;
			SkillEffectInfo()->iFireDelayCount = 1;
			SkillEffectInfo()->fFireDelay[0] = GetAnimLength(iAnimWildPet_Attack1)/3;
		}
		autocall SkillAndMagicAnimation() EReturn;

		return EReturn();
		/*************************************************************************/
		
	};

/*	SkillingTarget(EVoid) : CPetBase::SkillingTarget // 이 프로시저를 call하기전에 SetTargetEntity()가 선행되어야 한다.// Pet에서 쓴거
	{
		m_vDesiredPosition = PlayerDestinationPos();
	
/*
		if(HardCodingSkill())
		{
			autocall HardCodingSkillAnimation() EReturn;
			return EReturn();
		}

		
		CSkill &skill = _pNetwork->GetSkillData(m_nCurrentSkillNum);
		m_pSkillEffectInfo = new sSkillEffectInfo;
		SkillEffectInfo()->InitForSkillAttack(skill);
		autocall SkillAndMagicAnimation() EReturn;
	
		return EReturn();
	};*/

	SkillingTarget(EVoid) : CWildPetBase::SkillingTarget // 이 프로시저를 call하기전에 SetTargetEntity()가 선행되어야 한다.
	{
		m_vDesiredPosition = EnemyDestinationPos();
		
		CSkill &skill = _pNetwork->GetSkillData(m_nCurrentSkillNum);
		m_pSkillEffectInfo = new sSkillEffectInfo;
		SkillEffectInfo()->InitForSkillAttack(skill);

		autocall SkillAnimation() EReturn;
	
		return EReturn();
	};
	
	SpellSkill(EVoid) : CWildPetBase::SpellSkill
	{		
		m_vDesiredPosition = EnemyDestinationPos();
	
		autocall PreSkillAnimation() EReturn;
	
		return EReturn();
	};

	PreSkillAnimation(EVoid)
	{
		m_vDesiredPosition = EnemyDestinationPos();
		
		StopMoving();//0831

		m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetReadyAnim(FALSE));
		m_fSkillAnimTime = GetModelInstance()->GetAnimLength(m_idCurrentSkillAnim) * m_fSkillSpeed;		
		GetModelInstance()->AddAnimation(m_idCurrentSkillAnim,AN_NORESTART,1.0f,0, m_fSkillSpeed);		
		m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();

//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
		m_nEffectStep = 0;
		m_saLastEffectInfo.Clear();
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)

		wait(m_fSkillAnimTime) {
			on (EBegin) : {		
//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
					CSkill &skill = _pNetwork->GetSkillData(m_nCurrentSkillNum);
					if(m_nEffectStep == 0)
					{
						++m_nEffectStep;
						m_pSkillReadyEffect = StartEffectGroup(skill.GetReadyEffect(FALSE)
										, &en_pmiModelInstance->m_tmSkaTagManager
										, _pTimer->GetLerpedCurrentTick());
						if(m_pSkillReadyEffect) {m_pSkillReadyEffect->SetSpeedMul(m_fSkillSpeed);}
					}
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)
					resume; 
				};
			on (ETimer) : { stop; };
		}

		m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireAnim(FALSE));
	
		m_fSkillAnimTime = GetModelInstance()->GetAnimLength(m_idCurrentSkillAnim) * m_fSkillSpeed;		
		
	
		GetModelInstance()->AddAnimation(m_idCurrentSkillAnim,AN_LOOPING|AN_NORESTART|AN_CLEAR,1.0f,0, m_fSkillSpeed);	
		m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
		autowait(0.1f);//0114 임시코드 still애니메이션 0.1초

		return EReturn();
	}

	AttackAnimation(EVoid) //0628
	{
		float frandom = FRnd();
		if( frandom < 0.5f )
		{
			iAttackAnim = iAnimWildPet_Attack1;
		}
		else
		{
			iAttackAnim = iAnimWildPet_Attack2;
		}

		GetModelInstance()->AddAnimation(iAttackAnim, AN_CLEAR, 1.0f, 0);			

		AddAnimation(iAttackAnim, AN_CLEAR, 1.0f, 0, -1, m_fAttackSpeedMul);

		m_fAttackFrequency = 0.25f;
		m_fAttackStartTime = _pTimer->CurrentTick();
		m_fImpactTime = GetAnimLength(iAttackAnim)/3;//0630 우선 공격 애니메이션의 절반에서 임팩트.

		while(_pTimer->CurrentTick() - m_fAttackStartTime < GetAnimLength(iAttackAnim)*0.8f)
		{

			wait(m_fAttackFrequency) 
			{
				on (EBegin) : 
				{					
					m_fMoveSpeed = 0.0f; 
					ULONG ulFlags = SetDesiredMovement(); 				

					if(_pTimer->CurrentTick() - m_fAttackStartTime >m_fImpactTime) 
					{					
						if( m_penEnemy )
						{
							_pUIMgr->ShowDamage( m_penEnemy->en_ulID );
						}
						SendEvent(EWildPetAttackDamage());
					}
					m_fImpactTime = 1000.0f;//충분히 긴시간.

					resume; 
				}
				on (EWildPetBaseDamage eEBDamage) : 
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
							this->en_pmiModelInstance->AddAnimation(animId, AN_REMOVE_ON_END, 1.0f, 0);
						}
					}
					resume;
				}
				on (EWildPetAttackDamage eAttackDamage) : 
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
						InflictDirectDamage(m_penEnemy, this , DMT_CLOSERANGE, (int)1, vTargetPos, vDirection);
					// FIXME:
					//((CLiveEntity*)((CEntity*)m_penEnemy))->SetHealth(1000.0f);
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
			
		if(!m_bUseAI)
		{
			m_bAttack = FALSE;
			m_bMoving = FALSE;
			m_bStop = FALSE;		
			StandingAnim();
			SetNoTargetEntity();	
		}
		return EReturn();	
	};

	SkillAnimation(EVoid) //0628
	{	
		m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireAnim(FALSE));

		if(m_idCurrentSkillAnim == -1)
		{
			return EReturn();
		}		

		StopRotating();
		if( m_penEnemy == NULL )
		{
			if( m_pSkillReadyEffect != NULL && CEffectGroupManager::Instance().IsValidCreated( m_pSkillReadyEffect ) )
			{
				m_pSkillReadyEffect->Stop( 0.1f );
				m_pSkillReadyEffect = NULL;
			}
		}

//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
		m_nEffectStep = 0;
//		m_iSkillEffectStep = 0;
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)

		NewClearState(CLEAR_STATE_LENGTH);    
		
		m_fSkillAnimTime = GetModelInstance()->GetAnimLength(m_idCurrentSkillAnim) * m_fSkillSpeed;
		m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();			

		GetModelInstance()->AddAnimation(m_idCurrentSkillAnim,AN_NORESTART,1.0f,m_fSkillSpeed);	
		
		m_fAttackFrequency = 0.25f;
		m_fImpactTime = GetAnimLength(m_idCurrentSkillAnim) * m_fSkillSpeed / 2;//0630 우선 스킬 애니메이션의 절반에서 임팩트.		

		while(_pTimer->CurrentTick() - m_tmSkillStartTime < m_fSkillAnimTime) //GetAnimLength(m_idCurrentSkillAnim))
		{
			wait(m_fAttackFrequency) 
			{
				on (EBegin) : 
				{
					if(m_bMoving)
					{										
						return EReturn();
					}

//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
					CSkill &skill = _pNetwork->GetSkillData(m_nCurrentSkillNum);
					
					if(m_nEffectStep == 0)
					{
						++m_nEffectStep;

						CEffectGroup *pFireEffect = StartEffectGroup(skill.GetFireEffect1(FALSE), &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
						if(pFireEffect) {pFireEffect->SetSpeedMul(m_fSkillSpeed);}
						if(m_pSkillReadyEffect != NULL && CEffectGroupManager::Instance().IsValidCreated(m_pSkillReadyEffect))
						{
							for(INDEX i=0; i<m_pSkillReadyEffect->GetEffectCount(); ++i)
							{
								CEffect *pEffect = m_pSkillReadyEffect->GetEffectKeyVector()[i].m_pCreatedEffect;
								if(pEffect != NULL && pEffect->GetType() == ET_SKA)
								{
									CSkaEffect *pSkaEffect = (CSkaEffect*)pEffect;
									TagInfo ti;
									ti.m_vPos = pSkaEffect->GetInstancePosition();
									ti.m_qRot = FLOATquat3D(1,0,0,0);
									INDEX cnt = m_saLastEffectInfo.Count();
									m_saLastEffectInfo.Expand(cnt + 1);
									m_saLastEffectInfo[cnt] = ti;
								}
							}
							m_pSkillReadyEffect->Stop(0.04f);
							m_pSkillReadyEffect = NULL;
						}
						else if(pFireEffect != NULL)
						{
							pFireEffect->Process(_pTimer->GetLerpedCurrentTick());
							for(INDEX i=0; i<pFireEffect->GetEffectCount(); ++i)
							{
								CEffect *pEffect = pFireEffect->GetEffectKeyVector()[i].m_pCreatedEffect;
								if(pEffect != NULL && pEffect->GetType() == ET_SKA)
								{
									CSkaEffect *pSkaEffect = (CSkaEffect*)pEffect;
									TagInfo ti;
									ti.m_vPos = pSkaEffect->GetInstancePosition();
									ti.m_qRot = FLOATquat3D(1,0,0,0);
									INDEX cnt = m_saLastEffectInfo.Count();
									m_saLastEffectInfo.Expand(cnt + 1);
									m_saLastEffectInfo[cnt] = ti;
									pSkaEffect->SetLifeTime(0);
								}
							}
						}
					}
					ASSERT(skill.GetDelay(m_nEffectStep-1, FALSE) < GetAnimLength(m_idCurrentSkillAnim) && "SkillAnimTime은 반드시 MissileSpeedFireTime보다 커야합니다.");

					// 이 부분에서 iSkillEffectStep의 값은 1~skill.GetDelayCount()이다.
					if( m_nEffectStep <= skill.GetDelayCount(FALSE)
					 && _pTimer->GetLerpedCurrentTick() - m_tmSkillStartTime > skill.GetDelay(m_nEffectStep-1, FALSE) * m_fSkillSpeed)
					{
						++m_nEffectStep;
						if(skill.GetMissileType(FALSE) == CSkill::MT_NONE && m_penEnemy.ep_pen != NULL)	// 근접 공격 - 마법 없음
						{
							FLOAT3D vHitPoint;
							FLOAT3D vHitDir;
							GetTargetDirection(vHitPoint, vHitDir);

							if( m_dcEnemies.Count() > 0 )
							{
								DamagedTargetsInRange(this, m_dcEnemies, DMT_EXPLOSION, 1, vHitPoint, TRUE);
								m_dcEnemies.Clear();
							}
							else
							{
								//damage effect 처리
								if(m_penEnemy != NULL)//0817 버그 수정.
								{
									this->InflictDirectDamage(m_penEnemy, this, DMT_NONE, 1, vHitPoint, vHitDir);
								}
							}
							FLOAT3D vAxisY(0,1,0);
							FLOAT angle = acos(vHitDir % vAxisY);
							FLOAT3D axis = vAxisY * vHitDir;
							axis.Normalize();
							FLOATquat3D quat;
							quat.FromAxisAngle(axis, angle);
							StartEffectGroup(skill.GetFireEffect3(FALSE), _pTimer->GetLerpedCurrentTick(), vHitPoint, quat);
						}
						else if(skill.GetMissileType(FALSE) == CSkill::MT_ARROW && m_penEnemy.ep_pen != NULL)	// 화살
						{
							ASSERT(en_pmiModelInstance != NULL);

							FLOAT fHoriOffset = 0.0f;
							FLOAT fVertiOffset = 0.0f;
							if(skill.GetDelayCount(FALSE) > 1)
							{
								FLOAT fDist = (m_penEnemy->en_plPlacement.pl_PositionVector - en_plPlacement.pl_PositionVector).Length();
								FLOAT factor = 1.3f * fDist / skill.GetFireRange();
								factor = Clamp(factor, 0.0f, 1.0f);
								if(m_nEffectStep == 2) {fHoriOffset = +1.0f * factor;}
								if(m_nEffectStep == 3) {fHoriOffset = -1.0f * factor;}
								//if(m_nEffectStep == 4) {fVertiOffset = +0.0f * factor;}
								if(m_nEffectStep == 5) {fVertiOffset = -1.0f * factor;}
							}
							if( FALSE )
							{
								if( m_dcEnemies.Count() > 0 )
								{
									for( ENTITIES_ITERATOR it = m_dcEnemies.vectorSelectedEntities.begin();
										it != m_dcEnemies.vectorSelectedEntities.end(); ++it )
									{																		
										CEntity *pEn = (*it);
										if(pEn != NULL && pEn->IsFlagOff(ENF_DELETED))
										{
											ShotMissile(this, "STAFF", pEn, skill.GetMissileSpeed(FALSE)
												, skill.GetFireEffect3(FALSE), skill.GetFireEffect2(FALSE)
												, false, fHoriOffset, fVertiOffset, 0);
										}
									}
									m_dcEnemies.Clear();
								}
								else
								{
									ShotMissile(this, "STAFF", m_penEnemy.ep_pen, skill.GetMissileSpeed(FALSE)
												, skill.GetFireEffect3(FALSE), skill.GetFireEffect2(FALSE)
												, false, fHoriOffset, fVertiOffset, 1);
								}
							}
							else
							{
								if( m_dcEnemies.Count() > 0 )
								{
									for( ENTITIES_ITERATOR it = m_dcEnemies.vectorSelectedEntities.begin();
										it != m_dcEnemies.vectorSelectedEntities.end(); ++it )
									{																		
										CEntity *pEn = (*it);
										if(pEn != NULL && pEn->IsFlagOff(ENF_DELETED))
										{
											ShotMissile(this, "RHAND", pEn, skill.GetMissileSpeed(FALSE)
												, skill.GetFireEffect3(FALSE), skill.GetFireEffect2(FALSE)
												, false, fHoriOffset, fVertiOffset, 0);
										}
									}
									m_dcEnemies.Clear();
								}
								else
								{
									ShotMissile(this, "RHAND", m_penEnemy.ep_pen, skill.GetMissileSpeed(FALSE)
												, skill.GetFireEffect3(FALSE), skill.GetFireEffect2(FALSE)
												, false, fHoriOffset, fVertiOffset, 0);
								}
							}
						}
						else if(skill.GetMissileType(FALSE) == CSkill::MT_DIRECT && m_penEnemy.ep_pen != NULL)	// 발사체 없음, 데미지 없음, 버프 효과 등
						{
							if( m_penEnemy->GetFlags() & ENF_ALIVE && m_penEnemy->en_pmiModelInstance != NULL)
							{
								StartEffectGroup(skill.GetFireEffect3(FALSE)
												, &m_penEnemy->en_pmiModelInstance->m_tmSkaTagManager
												, _pTimer->GetLerpedCurrentTick());
							}
						}
						else if(skill.GetMissileType(FALSE) == CSkill::MT_DIRECTDAMAGE && m_penEnemy.ep_pen != NULL)	// 발사체 없음, 데미지 있음
						{
							if( m_dcEnemies.Count() > 0 )
							{						
								for( ENTITIES_ITERATOR it = m_dcEnemies.vectorSelectedEntities.begin();
									it != m_dcEnemies.vectorSelectedEntities.end(); ++it )
								{									
									CEntity &en = *(*it);
									if(en.IsFlagOn(ENF_ALIVE) && en.en_pmiModelInstance != NULL)
									{
										StartEffectGroup(skill.GetFireEffect3(FALSE)
														, &(en.en_pmiModelInstance->m_tmSkaTagManager)
														, _pTimer->GetLerpedCurrentTick());								
									}
								}

								// 메이지의 싱글던젼용 스킬을 쓰면 몹이 안죽길래...
								if(m_dcEnemies.Count() > 0)
								{
									FLOAT3D vHitPoint;
									FLOAT3D vHitDir;
									GetTargetDirection(vHitPoint, vHitDir);
									DamagedTargetsInRange(this, m_dcEnemies, DMT_EXPLOSION, 1, vHitPoint, TRUE);
									m_dcEnemies.Clear();
								}
							}
							else
							{
								this->InflictDirectDamage( m_penEnemy, this, DMT_NONE, 1, FLOAT3D(0,0,0), FLOAT3D(0,0,0) );

								if(m_penEnemy->en_pmiModelInstance != NULL)
								{
									StartEffectGroup( skill.GetFireEffect3(FALSE)
													, &m_penEnemy->en_pmiModelInstance->m_tmSkaTagManager
													, _pTimer->GetLerpedCurrentTick() );
								}
							}
						}
						else if( (skill.GetMissileType(FALSE) == CSkill::MT_CONTINUE || 
								skill.GetMissileType(FALSE) == CSkill::MT_MAGECUTTER )
							&& m_penEnemy != NULL && m_saLastEffectInfo.Count() != 0)	// 시전중인 이펙트가 발사체가 되어 날아감
						{
							if( m_dcEnemies.Count() > 0 )
							{
								for( ENTITIES_ITERATOR it = m_dcEnemies.vectorSelectedEntities.begin();
									it != m_dcEnemies.vectorSelectedEntities.end(); ++it )
								{															
									CEntity &en = *(*it);
									if(en.IsFlagOff(ENF_DELETED))
									{
										ShotMagicContinued(this, m_saLastEffectInfo[0].m_vPos, m_saLastEffectInfo[0].m_qRot
													, &en, skill.GetMissileSpeed(FALSE)
													, skill.GetFireEffect3(FALSE), skill.GetFireEffect2(FALSE)
													, false, skill.GetMissileType(FALSE));
									}
								}
								m_dcEnemies.Clear();
							}
							else
							{
								ShotMagicContinued(this, m_saLastEffectInfo[0].m_vPos, m_saLastEffectInfo[0].m_qRot
													, m_penEnemy, skill.GetMissileSpeed(FALSE)
													, skill.GetFireEffect3(FALSE), skill.GetFireEffect2(FALSE)
													, false, skill.GetMissileType(FALSE));
							}
							INDEX cnt = m_saLastEffectInfo.Count();
							CStaticArray<TagInfo> temp;
							temp.New(cnt-1);
							for(INDEX i=1; i<m_saLastEffectInfo.Count(); ++i)
							{
								temp[i-1] = m_saLastEffectInfo[i];
							}
							m_saLastEffectInfo = temp;
						}						
					}
					m_fMoveSpeed = 0.0f;
					if(m_penEnemy != NULL
						&& skill.GetMissileType(FALSE) != CSkill::MT_NOTHING)
					{
						ULONG ulFlags = SetDesiredMovement();
					}

					resume; 
				}
				on (EWildPetAttackDamage eAttackDamage) : 
				{
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
					if( m_dcEnemies.Count() > 0 )
					{
						DamagedTargetsInRange(this, m_dcEnemies, DMT_CLOSERANGE, 1, vTargetPos, TRUE);
						m_dcEnemies.Clear();
					}
					else
					{
						if(m_penEnemy!=NULL)//0817 버그 수정.
						{
							this->InflictDirectDamage(m_penEnemy, this , DMT_CLOSERANGE, (int)1, vTargetPos, vDirection);
						}
					}
					m_fImpactTime = 1000.0f;//충분히 긴시간.

					stop;
				}
				
				on (ETimer) : { stop; }			
			}
		}

/*
		m_bAttack = FALSE;		
		
		
		*/
		m_bMoving = FALSE;		
		m_bStop = FALSE;		
		StandingAnim();				
		m_bSkilling = FALSE;
		m_nSkillChack = 0;			// ttos : 소환수 스킬 해제
		m_wPetState = WPET_AI_FOLLOW;
		//SetNoTargetEntity();
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
		m_fImpactTime = GetAnimLength(iAnimWildPet_Attack1)/3;

		GetModelInstance()->AddAnimation(SkillEffectInfo()->iAnimatioID,AN_CLEAR,1.0f,0);	

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
					if(m_bStop)
					{
						DestroyEffectGroupIfValid(m_pSkillReadyEffect);
						//GetModelInstance()->AddAnimation(iAnimWildPet_Idle1,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);
						StandingAnim();
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
							this->en_pmiModelInstance->AddAnimation(animId, AN_REMOVE_ON_END, 1.0f, 0);
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
		// 배고픔이 0이라면 못움직임.
		if(	( _pNetwork->_WildPetInfo.bIsActive && ( _pNetwork->_WildPetInfo.m_nFaith <= 0 || _pNetwork->_WildPetInfo.m_nStm <= 0 ) ) 
			|| m_bStuned || m_bHold )
		{
			return EReturn();
		}

		if( m_penEnemy && m_penEnemy->GetFlags()&ENF_ALIVE)
		{
			if( m_bUseAI )
			{
				// 플레이어가 죽었다면....
				if( !(CEntity::GetPlayerEntity(0)->GetFlags()&ENF_ALIVE) )
				{
					return EReturn();
				}
				//070525_ttos : 소환수 스킬 예약 
				if(m_bSkilling)
				{
					return EReturn();
				}else if(m_nSkillChack)
				{
					UseSkill(m_nSkillChack);	//ttos : 소환수 스킬 예약된거 시작
					m_nSkillChack = 0;			// 소환수 스킬 해제
					return EReturn();
				} 
				// 싱글 모드의 경우.
				if(_pNetwork->m_bSingleMode)
				{	
					// 공격하는 놈과 맞는 놈의 위치를 보내줌.				
					CPlacement3D Myplacement = GetPlacement();					
					_pNetwork->SendMoveMessage(this, Myplacement, m_fMoveSpeed);

					_pNetwork->AddMoveList(*m_penEnemy);

					_pNetwork->SendMoveList();
				}				
				_pNetwork->SendAttackMessage(this, m_penEnemy, FALSE);
			}
		}
		return EReturn();
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
		
		en_sbNetworkType = MSG_CHAR_WILDPET;

		SetPhysicsFlags(EPF_ONBLOCK_CLIMBORSLIDE
				| EPF_TRANSLATEDBYGRAVITY
				| EPF_MOVABLE				
				| EPF_ABSOLUTETRANSLATE );
		SetCollisionFlags(ECF_MODEL_NO_COLLISION);
	
		
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
		//m_iCurrentCommand	= CSlaveInfo::COMMAND_PINCER;	// 소환수용
		m_bPlayAttackAnim	= FALSE;
		
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

		// continue behavior in base class
		jump CWildPetBase::MainLoop();	
	};
};
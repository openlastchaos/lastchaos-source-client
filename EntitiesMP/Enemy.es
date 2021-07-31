
1864
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
#include <Engine/Interface/UIManager.h>

#include "EntitiesMP/RestrictedField.h"
#include "EntitiesMP/Common/RestrictedFieldContainer.h"

#define MOB_DOOMSLAYER	158
#define MOB_BAAL		152

#define MOB_ATTACK_MOTION_NUM		(4)

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
void GetTargetDirection(CEntity *penMe, CEntity *penTarget, FLOAT3D &vTargetPos, FLOAT3D &vDirection)
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

uses "EntitiesMP/EnemyBase";
uses "EntitiesMP/Global";
uses "EntitiesMP/BasicEffects";
uses "EntitiesMP/CharacterBase";
uses "EntitiesMP/Character";

event EAttackDamage {//0629
};

// 공격 타입.
enum AttackType
{
	0 ATK_NORMAL	"Attack Normal",
	1 ATK_MAGIC		"Attack Magic",
};

enum MobType
{
	0 MOB_ENEMY		"Boss Mob",		// 일반 몹.
	1 MOB_FOLLOWPC	"Follow PC",	// PC를 따라가는 NPC
};

%{
// info structure
static EntityInfo eiMobBoss = {
	EIBT_FLESH, 200.0f,
	//0.0f, 1.75f*m_fStretch, 0.0f,     // source (eyes)
	//0.0f, 1.30f*m_fStretch, 0.0f,     // target (body)
	0.0f, 1.75f*1.0f, 0.0f,     // source (eyes)
	0.0f, 1.30f*1.0f, 0.0f,     // target (body)
};

//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
static INDEX g_iNPCMinBrightness = 32;	//명도 0~240
void CEnemy_OnInitClass(void)
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

class CEnemy: CEnemyBase {
name      "Enemy";
thumbnail "Mods\\test.tbn";
features  "ImplementsOnInitClass", "HasName", "IsTargetable";

properties:
	1 enum	MobType m_MobType "Type" 'Y'		= MOB_ENEMY,	
	3 FLOAT m_fBodyAnimTime		= -1.0f,
	91 INDEX m_nMobDBIndex "Enemy List" 'E'		= 0,
	21	INDEX		m_iRegenTime "Regen Time" 'R' = 300,
	5 FLOAT m_fStretch			= 1.0f,
	80 enum AttackType m_AttackType = ATK_NORMAL,	
	99  INDEX idMob_Start		= -1,
	100 INDEX idMob_Walk		= -1,
	101 INDEX idMob_Attack		= -1,
	102 INDEX idMob_Default		= -1,
	103 INDEX idMob_Death		= -1,
	104 INDEX idMob_Wound		= -1,
	105 INDEX idMob_NormalBox	= -1,
	106 INDEX idMob_Attack2		= -1,
	107 INDEX idMob_Run			= -1,
	108 INDEX idMob_Default2	= -1,
	109 INDEX idMob_Attack3		= -1,
	110 INDEX idMob_Attack4		= -1,
	150 INDEX m_iTotalNum	"Total Num"	= -1,

// FIXME : 싱글던젼을 위해서만 필요함...ㅡ.ㅡ
// FIXME : 보스몹은 안 밀리게 해달라구 해서....
	160 BOOL	m_bBoss			= FALSE,

	200	CTString	m_strName	" Name" = "Enemy",		
//	201 INDEX	m_nMaxiHealth = 0,
//	202 INDEX	m_nCurrentHealth = 0,	
	203 INDEX	m_nMobClientIndex	= -1,
	//204 CSoundObject m_soSound2,
	//205 INDEX	m_nMobLevel = 0,
	//206 INDEX	m_nPreHealth = -1,	
	//235	INDEX m_iZoneMovingFlag "Zone Moving"		= 513,	// Zone Moving
	237 flags ZoneBits	m_zfZoneFlagBits	= 0,	// Zone Moving Flag
	238 flags ExtraBits	m_efExtraBits		= 0,	// Extra Flag
	240 BOOL	m_bPreCreate				"Precreated NPC"	= FALSE,	// 미리 생성되어야 하는 NPC의 경우.
	245 BOOL	m_bHostageNpc				"Is hostage NPC?"	= FALSE,	// 구출해야하는 NPC인가?
	247 enum EventType	m_EventType			"Event Type"		= EVENT_NONE,
	248 CEntityPointer m_penEventTarget		"Event Target",					// 트리거를 통해서 Blind를 해제하도록 함.
	250 BOOL	m_bInit						= FALSE,
	251 BOOL	m_bIsRun					= FALSE,
	252 BOOL	m_bDie	= FALSE,
	//253 FLOAT m_fIdleTime = 0.0f,
	{
		CEffectGroup *m_pSkillReadyEffect;
		CStaticArray<FLOAT3D> m_aDeathItemPosDrop;
		CStaticArray<CItemTarget> m_aDeathItemTargetDrop;
		CStaticArray<void *> m_aDeathItemDataDrop;
		INDEX	m_nMagicAttackSequence;
		void	*m_pSkillEffectInfo;
		INDEX	m_nEffectStep;
		//INDEX	m_iSkillEffectStep;
		//CStaticArray<FLOAT> m_afHardCodeValue;
		//BOOL	m_bHardcodingSkillLoop;		
		CEnemyAnimation		m_AnimationIndices;
	}

components:	
	// 사운드 처리가 가장 문제되는 부분.
functions:

//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
	BOOL AdjustShadingParameters(FLOAT3D &vLightDirection, COLOR &colLight, COLOR &colAmbient)
	{
		//이 주석을 풀면 몹사망시 Fade-out처리가 된다. 단 약간 튀는 듯한 문제 있음.
		CEnemyBase::AdjustShadingParameters(vLightDirection, colLight, colAmbient);
		/*g_iNPCMinBrightness = Clamp(g_iNPCMinBrightness, (INDEX)0, (INDEX)240);
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
		if(bRecalcAmbient) {colAmbient = HSVToColor(ubAH, ubAS, ubAV);}*/
		return TRUE;
		//return CEnemyBase::AdjustShadingParameters(vLightDirection, colLight, colAmbient);
	}
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)

	virtual FLOAT GetLockRotationSpeed(void) 
	{ 
		return AngleDeg(1800.0f); 
	};
	
	void OnInitialize(const CEntityEvent &eeInput)
	{
		CEnemyBase::OnInitialize(eeInput);
	};

	void OnEnd(void) 
	{
		CEnemyBase::OnEnd();		
	};	

	// 에너미가 움직일수 있는지 판단함...
	virtual BOOL IsMovable()
	{
		// 내 소환수이며...
		if( m_bUseAI )
		{			
			if( m_bStuned || m_bHold )
			{
				return FALSE;
			}
		}
		return TRUE;
	};

	// 에너미가 공격할 수 있는 상태인가?
	virtual BOOL IsAttackable()
	{
		if( IsFirstExtraFlagOn(ENF_EX1_NPC) )
		{
			return FALSE;		
		}
		else if( m_bUseAI )
		{
			if( m_bStuned || m_bHold )
			{
				return FALSE;
			}
		}

		return TRUE;
	};
	
	// describe how this enemy killed player
	virtual CTString GetPlayerKillDescription(const CTString &strPlayerName, const EDeath &eDeath)
	{
		CTString str;
		str.PrintF(TRANS("A %s sent %s into the halls of Valhalla") ,m_strName, strPlayerName);
		return str;
	}

	virtual BOOL IsBossMob()
	{
		return m_bBoss;
	};

	virtual BOOL IsSkillAttack( )
	{
		float frandom = FRnd();

		// 카미라의 스킬 공격 확률.
		if( frandom > 0.2f )
		{
			// 일반 공격
			return FALSE;
		}
			
		if( m_bUseAI )
		{
			// 버프가 걸린 상태에서 스킬 사용 불가.
			if( m_bCannotUseSkill ) { return FALSE; }

			CMobData& MD = _pNetwork->GetMobData(m_nMobDBIndex);

			if( MD.GetSkill0Index() <= 0 && MD.GetSkill1Index() <= 0 )
			{
				return FALSE;
			}
			
			float frandom = FRnd();
			if( frandom > 0.5f && MD.GetSkill1Index() > 0 )
			{
				m_nCurrentSkillNum	= MD.GetSkill1Index();
			}
			else
			{
				m_nCurrentSkillNum	= MD.GetSkill0Index();
			}

			CSkill& rSkill = _pNetwork->GetSkillData( m_nCurrentSkillNum );
			if( ( CalcDist(m_penEnemy) < rSkill.GetFireRange() && CanHitEnemy(m_penEnemy, Cos(AngleDeg(45.0f))) ) ) 
			{
				INDEX iMobIndex = GetNetworkID();
				CNetworkMessage nmMobMove(MSG_MOVE);
				UBYTE ubCharType	= (UBYTE)MSG_CHAR_NPC;
				UBYTE ubMoveType	= (UBYTE)MSG_MOVE_STOP;
				FLOAT fSpeed		= 0.0f;
				
				CPlacement3D pl		= GetLerpedPlacement();
				
				nmMobMove << ubCharType;					// Char Type
				nmMobMove << ubMoveType;					// Move Type
				nmMobMove << (ULONG)iMobIndex;				// INDEX
				nmMobMove << fSpeed;						// Speed
				nmMobMove << pl.pl_PositionVector(1);		// Pos X
				nmMobMove << pl.pl_PositionVector(3);		// Pos Z
				nmMobMove << pl.pl_PositionVector(2) + 0.5f;		// Pos H
				nmMobMove << pl.pl_OrientationAngle(1);		// Angle
				nmMobMove << _pNetwork->MyCharacterInfo.yLayer;		// Y Layer
				_pNetwork->SendToServerNew(nmMobMove);

				{
					CTString strSysMessage;
					CNetworkMessage nmPlayerSkill(MSG_SKILL);
					nmPlayerSkill << (UBYTE)MSG_SKILL_READY;
					ULONG nSkillIndex = m_nCurrentSkillNum;
					UBYTE ubCharType = (UBYTE)MSG_CHAR_NPC;		
					nmPlayerSkill << ubCharType;
					nmPlayerSkill << iMobIndex;
					nmPlayerSkill << (ULONG)nSkillIndex;
					LONG nTargetType;
					nTargetType	= MSG_CHAR_PC;
					nmPlayerSkill << (unsigned char)nTargetType; //우선은 타겟=NPC만.

					nmPlayerSkill << (ULONG)_pNetwork->MyCharacterInfo.index; 
					nmPlayerSkill << (SBYTE)0;
					_pNetwork->SendToServerNew(nmPlayerSkill);
				}
				{
					CTString strSysMessage;
					CNetworkMessage nmPlayerSkill(MSG_SKILL);
					nmPlayerSkill << (UBYTE)MSG_SKILL_FIRE;
					ULONG nSkillIndex = m_nCurrentSkillNum;
					UBYTE ubCharType = (UBYTE)MSG_CHAR_NPC;		
					nmPlayerSkill << ubCharType;
					nmPlayerSkill << iMobIndex;
					nmPlayerSkill << (ULONG)nSkillIndex;
					LONG nTargetType;
					nTargetType	= MSG_CHAR_PC;
					nmPlayerSkill << (unsigned char)nTargetType; //우선은 타겟=NPC만.

					nmPlayerSkill << (ULONG)_pNetwork->MyCharacterInfo.index; 
					nmPlayerSkill << (SBYTE)0;
					_pNetwork->SendToServerNew(nmPlayerSkill);
				}				
				return TRUE;
			}
		}
		m_nCurrentSkillNum	= -1;
		return FALSE;
	};

	/* Entity info */
	void *GetEntityInfo(void)
	{
			return &eiMobBoss;
	};

	void Precache(void) 
	{
		CEnemyBase::Precache();		

		//-------------------------------------------------------------------------------
		if(m_bPreCreate)
		{
			if(_pNetwork && _pNetwork->m_bSingleMode && _pNetwork->ga_World.wo_iNumOfNPC)
			{
				en_pwoWorld->m_vectorPreCreateNPC.push_back(this);
			}
		}
		//-------------------------------------------------------------------------------

//안태훈 수정 시작	//(Fix Snd Effect Bug)(0.1)
		if(en_pmiModelInstance) { en_pmiModelInstance->m_tmSkaTagManager.SetOwner(this); }
//안태훈 수정 끝	//(Fix Snd Effect Bug)(0.1)
	};

	virtual BOOL IsEnemy(void) const
	{
		return TRUE;
	}

	void InflictDirectDamage(CEntity *penToDamage, CEntity *penInflictor, enum DamageType dmtType,
	FLOAT fDamageAmmount, const FLOAT3D &vHitPoint, const FLOAT3D &vDirection)
	{
		if(penToDamage)
		{
			if( !(penToDamage->GetFlags()&ENF_ALIVE ) )	{ return; }

			_pUIMgr->ShowDamage( penToDamage->en_ulID );
			// Date : 2005-11-16(오후 2:05:09), By Lee Ki-hwan
			//_pUIMgr->SetDamageState();

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
		_pUIMgr->ShowDamage( en_ulID );

		// 평화 NPC(예를 들면 공주)가 아닐 경우에는 타겟이 때린 놈으로 변경되도록 함.
		if( m_bUseAI && !( GetFirstExFlags() & ENF_EX1_PEACEFUL ) )
		{
			// 무조건 때린 놈으로 바뀌면 안될거 같아서, 랜덤하게 바뀌도록 설정함.
			FLOAT fRand = FRnd();		
			if( fRand > 0.5f )
			{
				// 때린 놈 중에서 플레이어나 소환수의 경우에만...
				if( penInflictor->IsPlayer() || penInflictor->IsSlave() || penInflictor->IsWildPet())
				{
					// 때린 놈으로 타겟을 변경합니다.
					SetTargetEntity(penInflictor);				
				}
			}			
		}
		
		//CEnemyBase::ReceiveDamage(penInflictor, dmtType, fDamageAmmount, vHitPoint, vDirection);
	};

	//--------------------------------------------------------------
	// 몬스터의 애니메이션을 설정하는 함수들...
	//--------------------------------------------------------------
	// damage anim
	INDEX AnimForDamage(FLOAT fDamage) 
	{
		ASSERT(idMob_Wound		!= -1	&& "Invalid Animation");
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");

		// NOTE : 공성탑의 경우에는 데미지 애니메이션을 보여주지 않는다...
		CMobData& MD = _pNetwork->GetMobData(m_nMobDBIndex);
		if(MD.IsCastleTower())
		{
			return -1;
		}

		//0820 가만히 서있을때만 데미지 모션이 나와야 된다. 꼭!
		if( !IsAnimationPlaying(idMob_Default) && 
			!IsAnimationPlaying(idMob_Default2) )
		{
			INDEX animSet, animIdx;
			static INDEX animId = ska_StringToID("mldam");//HardCoding
			if(this->en_pmiModelInstance
			&& this->en_pmiModelInstance->FindAnimationByID(animId, &animSet, &animIdx))
			{
				//타격 모션
				this->en_pmiModelInstance->AddAnimation(animId, AN_REMOVE_ON_END, 1.0f, 0);
			}
			return animId;
		}

		NewClearState(CLEAR_STATE_LENGTH);    
		GetModelInstance()->AddAnimation(idMob_Wound,AN_CLEAR,1,0);	
		
		return idMob_Wound;
	};

	// death
	INDEX AnimForDeath(void)
	{	
		ASSERT(idMob_Death		!= -1	&& "Invalid Animation");
	
		//DropDeathItem();//1019
				
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");
		NewClearState(CLEAR_STATE_LENGTH);    
		GetModelInstance()->AddAnimation(idMob_Death,AN_NORESTART,1,0);

		return idMob_Death;
	};

	// 죽을때 뭔가가...
	FLOAT WaitForDust(FLOAT3D &vStretch)
	{
		//0317 kwon
		vStretch=FLOAT3D(1,1,1);		
		if(GetModelInstance()->IsAnimationPlaying(idMob_Death)) 
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

		if(_pNetwork->m_bSingleMode)
		{
			--_pNetwork->wo_dwEnemyCount;

			// NOTE : 밴더스내치의 경우에는 카운트에서 제외.
			if( m_nMobDBIndex != 220 )
			{
				_pNetwork->wo_dwKilledEnemyCount++;
			}
			
			// 특정 몹의 수를 확인합니다.
			CRestrictedField *pField = FindPlayerInRestrictedField((CPlayer*)CEntity::GetPlayerEntity(0));
			if(pField)
			{
				if( _pNetwork->wo_dwEnemyCount <= pField->iMobCount )
				{
					pField->m_bActive	= FALSE;
					SendToTarget(pField->m_penTarget, EET_TRIGGER, CEntity::GetPlayerEntity(0));
				}
			}

			if(_pNetwork->m_ubGMLevel > 1)
			{
				CTString strMessage;
				strMessage.PrintF("=====current enemies : %d=====\n", _pNetwork->wo_dwEnemyCount);
				_pNetwork->ClientSystemMessage(strMessage);
			}
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

	void StartAnim(void)
	{
		ASSERT(idMob_Start	!= -1	&& "Invalid Animation");
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");
		GetModelInstance()->AddAnimation(idMob_Start,AN_NORESTART|AN_CLEAR,1,0);
	};

	// virtual anim functions
	void StandingAnim(void) 
	{		
		m_bIsRun = FALSE;
		ASSERT(idMob_Default	!= -1	&& "Invalid Animation");
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");

		GetModelInstance()->AddAnimation(idMob_Default,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);
	};
	void RunningAnim(void) 
	{
		m_bIsRun = TRUE;
		ASSERT(idMob_Run		!= -1	&& "Invalid Animation");
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");
		GetModelInstance()->AddAnimation(idMob_Run,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);
	};
	void WalkingAnim(void) 
	{
		m_bIsRun = FALSE;
		ASSERT(idMob_Walk		!= -1	&& "Invalid Animation");
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");
		GetModelInstance()->AddAnimation(idMob_Walk,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);		
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
	void AttackAnim(void) 
	{
		ASSERT(idMob_Attack		!= -1		&& "Invalid Animation");
		ASSERT(GetModelInstance() != NULL	&& "Invalid ModelInstance");

		static FLOAT	start_attack_time=0;
		static float  m_fBodyAnimTime=0;
		
		if(start_attack_time==0)
		{
			start_attack_time = _pTimer->CurrentTick();
			GetModelInstance()->AddAnimation(idMob_Attack,AN_NORESTART|AN_CLEAR,1.0f,0);
			m_fBodyAnimTime = GetModelInstance()->GetAnimLength(idMob_Attack);
		}
		if(start_attack_time !=0 && (_pTimer->CurrentTick() - start_attack_time >= m_fBodyAnimTime))
		{
			m_bAttack = FALSE;	
			start_attack_time = 0;		
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
		//ULONG MobIndex = penEntity->en_ulID;		

		if( !(m_penEnemy.ep_pen != NULL
			&& m_penEnemy->en_pmiModelInstance != NULL
			&& m_penEnemy->GetFlags() & ENF_ALIVE) )
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
				GetTargetDirection(this, m_penEnemy, vHitPoint, vHitDir);

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
	Fire(EVoid) : CEnemyBase::Fire
	{
		jump CEnemyBase::Fire();
	}

	Hit(EVoid) : CEnemyBase::Hit 
	{
		if( !IsAttackable() )
		{
			return EReturn();
		}

		if (m_MobType == MOB_ENEMY) 
		{
			autocall NormalAttack() EEnd;
		// should never get here
		} 
		else
		{
			ASSERT(FALSE);
		}
		return EReturn();
	};
		
	//0628 kwon
	AttackTarget(EVoid) : CEnemyBase::AttackTarget // 이 프로시저를 call하기전에 SetTargetEntity()가 선행되어야 한다.
	{
		m_vDesiredPosition = PlayerDestinationPos();
		
		INDEX attackAnimID = -1;
		const int iAttackMotion = m_nAttackCnt % MOB_ATTACK_MOTION_NUM;
		if( iAttackMotion == 0 )
		{
			attackAnimID = idMob_Attack;
		}
		else if( iAttackMotion == 1 )
		{
			attackAnimID = idMob_Attack2;
		}
		else if( iAttackMotion == 2 )
		{
			attackAnimID = idMob_Attack3;
		}
		else if( iAttackMotion == 3 )
		{
			attackAnimID = idMob_Attack4;
		}
		m_pSkillEffectInfo = new sSkillEffectInfo;
		SkillEffectInfo()->InitForNormalAttack(_pNetwork->GetMobData(m_nMobDBIndex), attackAnimID);

		if(strlen(SkillEffectInfo()->szEffectNameHit) == 0) {SkillEffectInfo()->szEffectNameHit = "Normal Hit";}
		if(SkillEffectInfo()->iFireDelayCount == 0)
		{
			SkillEffectInfo()->iMissileType = CSkill::MT_NONE;
			SkillEffectInfo()->iFireDelayCount = 1;
			SkillEffectInfo()->fFireDelay[0] = GetAnimLength(idMob_Attack)/3;
		}
		autocall SkillAndMagicAnimation() EReturn;

		return EReturn();
	};

	SkillingTarget(EVoid) : CEnemyBase::SkillingTarget // 이 프로시저를 call하기전에 SetTargetEntity()가 선행되어야 한다.
	{
		m_vDesiredPosition = PlayerDestinationPos();
		
		CSkill &skill = _pNetwork->GetSkillData(m_nCurrentSkillNum);
		m_pSkillEffectInfo = new sSkillEffectInfo;
		SkillEffectInfo()->InitForSkillAttack(skill);
		autocall SkillAndMagicAnimation() EReturn;
	
		return EReturn();
	};

	IdleAnimation(EVoid) : CEnemyBase::IdleAnimation
	{
		autowait(15.0f + FRnd()*5.0f);
		GetModelInstance()->AddAnimation(idMob_Default2,AN_CLEAR,1.0f,0);	
		//m_fIdleTime = _pTimer->CurrentTick();

		FLOAT animtime = GetAnimLength(idMob_Default2);
		autowait(animtime);
		StandingAnim();
		return EReturn();
		
	}

	AttackAnimation(EVoid) //0628
	{
		const int iAttackMotion = m_nAttackCnt % MOB_ATTACK_MOTION_NUM;
		if( iAttackMotion == 0 )
		{
			GetModelInstance()->AddAnimation(idMob_Attack, AN_CLEAR, 1.0f, 0);	
		}
		else if( iAttackMotion == 1 )
		{
			GetModelInstance()->AddAnimation(idMob_Attack2, AN_CLEAR, 1.0f, 0);	
		}
		else if( iAttackMotion == 2 )
		{
			GetModelInstance()->AddAnimation(idMob_Attack3, AN_CLEAR, 1.0f, 0);	
		}
		else if( iAttackMotion == 3 )
		{
			GetModelInstance()->AddAnimation(idMob_Attack4, AN_CLEAR, 1.0f, 0);	
		}

		m_fAttackFrequency = 0.25f;
		m_fAttackStartTime = _pTimer->CurrentTick();
		m_fImpactTime = GetAnimLength(idMob_Attack)/3;//0630 우선 공격 애니메이션의 절반에서 임팩트.

		while(_pTimer->CurrentTick() - m_fAttackStartTime < GetAnimLength(idMob_Attack)*0.8f){

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
						SendEvent(EAttackDamage());
						m_fImpactTime = 1000.0f;//충분히 긴시간.
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
				on (EAttackDamage eAttackDamage) : 
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

//강동민 수정 시작 싱글 던젼 작업	07.25
					// Enemy에 ReceiveDamage를 전송합니다.
					// ReceiveDamage에서 Health가 0보다 작으면 엔티티를 Destroy하는 부분이 있음.
					this->InflictDirectDamage(m_penEnemy, this , DMT_CLOSERANGE, (int)1, vTargetPos, vDirection);
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
		m_fImpactTime = GetAnimLength(idMob_Attack)/3;

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
					if(m_bMoving || m_bStop)
					{
						DestroyEffectGroupIfValid(m_pSkillReadyEffect);
						GetModelInstance()->AddAnimation(idMob_Default,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);							
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

		if( !m_bUseAI )
		{
			m_bAttack = FALSE;
			m_bMoving = FALSE;
			m_bStop = FALSE;		
			StandingAnim();
			SetNoTargetEntity();	
		}

		return EReturn();
	};

	// Normal attack
	NormalAttack(EVoid)
	{
		StandingAnim();
		autowait(0.8f + FRnd()*0.25f);

		INDEX attackAnimID = -1;
		const int iAttackMotion = rand() % MOB_ATTACK_MOTION_NUM;
		if( iAttackMotion == 0 )
		{
			attackAnimID = idMob_Attack;
		}
		else if( iAttackMotion == 1 )
		{
			attackAnimID = idMob_Attack2;
		}
		else if( iAttackMotion == 2 )
		{
			attackAnimID = idMob_Attack3;
		}
		else if( iAttackMotion == 3 )
		{
			attackAnimID = idMob_Attack4;
		}

		m_nAttackCnt = attackAnimID;

		if(m_bUseAI)
		{
// EDIT : BS : BEGIN
//			CPlacement3D pl		= GetLerpedPlacement();
//			_pNetwork->AddMoveList( 
//				GetNetworkID(),
//				pl.pl_PositionVector(1), 
//				pl.pl_PositionVector(3), 
//				pl.pl_PositionVector(2),
//				pl.pl_OrientationAngle(1) );
			_pNetwork->AddMoveList(*this);
			_pNetwork->SendMoveList();
// EDIT : BS : END
			INDEX iAttackerIndex	= GetNetworkID();
			_pNetwork->AddAttackList( 0, iAttackerIndex, m_penEnemy->GetNetworkType(), m_penEnemy->GetNetworkID() );			
		}

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

			if(!m_bInit)
			{
				if(m_nMobDBIndex > 0)
				{					
					// Mob Respawn Message
					if(_cmiComm.IsNetworkOn())
					{
						CPlacement3D pl = GetLerpedPlacement();
						CNetworkMessage nmMobSpawn(MSG_NPC_REGEN);
						INDEX iIndex	= -1;
						INDEX iYLayer	= 0;
						INDEX iMobType	= m_nMobDBIndex;
						INDEX iEntityID = en_ulID;					
						_pNetwork->AddRegenList(iIndex, iMobType, 
						pl.pl_PositionVector(1), pl.pl_PositionVector(2), pl.pl_PositionVector(3), 
						pl.pl_OrientationAngle(1), iYLayer, iEntityID);

						en_RenderType = RT_SKAMODEL;
					}
				}
			}
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
		
		// declare yourself as a model
		SetPhysicsFlags(EPF_ONBLOCK_CLIMBORSLIDE
			| EPF_TRANSLATEDBYGRAVITY
			| EPF_PUSHABLE
			| EPF_MOVABLE			
			| EPF_ABSOLUTETRANSLATE /*| EPF_RT_SYNCHRONIZED*/);//|EPF_STICKYFEET|EPF_RT_SYNCHRONIZED);
		SetPhysicsFlags(GetPhysicsFlags() &~EPF_PUSHABLE);
// NOTE : 공성관련되서...
		// 이벤트 NPC만 충돌 체크
		CMobData& MD = _pNetwork->GetMobData(m_nMobDBIndex);

		// [070607: Su-won] 월드에디터에서는 그냥 통과.
		// 에디터상에서는 CNetworkLibrary::ga_World.wo_aMobData 에 NPC 데이터가 저장되지 않아서
		// CNetworkLibrary::GetMobData() 함수를 통해서 제대로된 NPC 데이터를 가져오지 못하기때문에
		// 월드에 배치된 NPC의 모델을 나타낼려고 할 때 에러발생함.
		extern BOOL _bWorldEditorApp;
		if( !_bWorldEditorApp )
		{
			if(MD.IsEvent() || MD.IsLordSymbol() || MD.IsZoneMoving() || MD.IsCollsionModel())
			{
				SetCollisionFlags(ECF_MODEL);		
			}
			else 
			{
				SetCollisionFlags(ECF_MODEL_NO_COLLISION);
			}
		}

		en_sbNetworkType = MSG_CHAR_NPC;

//강동민 수정 시작 테스트 클라이언트 작업	06.22
		if(!_pNetwork->m_bSingleMode || m_bPreCreate)
		{		
			SetFlagOn(ENF_ALIVE);
		}
		else
		{
			// 싱글 모드의 경우.
			m_bUseAI				= TRUE;
			m_fIgnoreRange			= 1000.0f;
			SetFlagOff(ENF_ALIVE);
		}
//강동민 수정 끝 테스트 클라이언트 작업		06.22
		en_tmMaxHoldBreath = 5.0f;
		en_fDensity = 2000.0f;
		if(	idMob_Walk			== -1 ||
			idMob_Run			== -1 ||
			idMob_Attack		== -1 ||
			idMob_Default		== -1 ||
			idMob_Death			== -1 ||
			idMob_Wound			== -1 ||
			idMob_NormalBox		== -1)
			{ 
				return EEnd();
			}

		SetFlagOn(ENF_RENDERREFLECTION);

				// setup moving speed
				//m_fWalkSpeed			= FRnd() + 0.5f;
				//m_fWalkSpeed			= 4.0f;
				m_aWalkRotateSpeed		= AngleDeg(1800.0f);
				//m_fAttackRunSpeed		= FRnd() + 1.0f;
				//m_fAttackRunSpeed		= 4.0f;
				m_aAttackRotateSpeed	= AngleDeg(1800.0f);
				//m_fCloseRunSpeed		= FRnd() + 1.0f;
				//m_fCloseRunSpeed		= 4.0f;
				m_aCloseRotateSpeed		= AngleDeg(1800.0f);

				// setup attack distances
				//m_fAttackDistance		= 5.0f;
				//m_fCloseDistance		= 20.0f;		// 근거리 모드????
				//m_fStopDistance			= 1.5f;			// (Stop Distance보다 가깝다면 적을 쫓아가지 않음.)
				m_fAttackFireTime		= 2.0f;
				m_fCloseFireTime		= 2.0f;
				m_fBlowUpAmount			= 80.0f;
				m_fBodyParts			= 4;
				m_fDamageWounded		= 0.0f;
//				m_iScore				= 0;

				m_nMobClientIndex		= en_ulID;	
				// set stretch factors for height and width
				GetModelInstance()->StretchModel(FLOAT3D(m_fStretch, m_fStretch, m_fStretch));

				extern BOOL	_bWorldEditorApp;
				if( !_bWorldEditorApp )
				{
					// 자이언트 석상의 경우 그림자 안나오도록...
					CMobData& MD = _pNetwork->GetMobData(m_nMobDBIndex);
					if(!MD.IsCastleTower())
					{
						GetModelInstance()->mi_bRenderShadow = TRUE;
					}
				}

		SetHealth(10000.0f);//0807 몹의 체력.
//		m_soSound2.Set3DParameters(25.0f, 5.0f, 1.0f, 1.0f);

//강동민 수정 시작 싱글 던젼 작업	08.05
		if(idMob_Start != -1)
		{
			FLOAT fAnimLength = GetAnimLength(idMob_Start);
			//GetModelInstance()->AddAnimation(idMob_Start,AN_NORESTART,1,0);
			StartAnim();
//			PlaySound(m_soSound, SOUND_ZOMBI_APPEAR, SOF_3D|SOF_VOLUMETRIC);
			autowait(fAnimLength);
		}
//강동민 수정 끝 싱글 던젼 작업		08.05
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

		// continue behavior in base class
		jump CEnemyBase::MainLoop();
	};
};
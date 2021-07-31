1511
%{
#include "StdH.h"
#include <Engine/Effect/CWorldTag.h>
#include <Engine/Effect/CEffectGroup.h>
#include <Engine/Effect/CEffectGroupManager.h>
#include <Engine/Effect/CSkaEffect.h>
//강동민 수정 시작 1차 버그 수정 작업	08.20
#include <Engine/GlobalDefinition.h>
#include <Engine/GameState.h>
#include "EntitiesMP/Common/CharacterDefinition.h"
#include <Engine/JobInfo.h>
#include <Engine/TransformInfo.h>
#include <Engine/Network/MessageDefine.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/PetInfo.h>
//강동민 수정 끝 1차 버그 수정 작업		08.20

// Check Attack Flag
#define ENF_EX2_PVP				(1L<<0)
#define ENF_EX2_LEGIT			(1L<<1)
#define ENF_EX2_MYPARTY			(1L<<2)
#define ENF_EX2_MYGUILD			(1L<<3)
#define ENF_EX2_ENEMYGUILD		(1L<<4)
#define ENF_EX2_WAR_OURFORCE	(1L<<5)
#define ENF_EX2_WAR_ENEMY		(1L<<6)
#define MODEL_TREASURE	("Data\\Item\\Common\\ITEM_treasure02.smc")

extern ENGINE_API BOOL g_bHead_change; // 일본 헬멧 추가 관련 

%}

//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
uses "EntitiesMP/Missile";
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)
uses "EntitiesMP/EnemyBase";
uses "EntitiesMP/Global";
uses "EntitiesMP/BasicEffects";

uses "EntitiesMP/CharacterBase";
uses "EntitiesMP/BasicEffects";
uses "EntitiesMP/EnemyBase";
uses "EntitiesMP/Player";
uses "EntitiesMP/PetBase";

event EChaAttackDamage {//0629
};
event EChaSkillDamage {
};

%{
	
// info structure
static EntityInfo eiChaBoss = {
	EIBT_FLESH, 200.0f,
	//0.0f, 1.75f*m_fStretch, 0.0f,     // source (eyes)
	//0.0f, 1.30f*m_fStretch, 0.0f,     // target (body)
	0.0f, 1.75f*1.0f, 0.0f,     // source (eyes)
	0.0f, 1.30f*1.0f, 0.0f,     // target (body)
};

//-----------------------------------------------------------------------------
// Purpose: 컨테이너 내의 모든 엔티티에 한번에 데미지를 줍니다.
// Input  : pEntity - 
//			sTI - 
//			vCenter - 
// Output : static void
//-----------------------------------------------------------------------------
void DamagedTargetsInRange(CEntity* pEntity, CSelectedEntities& dcEntities, enum DamageType dmtType, FLOAT fDamageAmmount, FLOAT3D vCenter, BOOL bSkill);

//안태훈 수정 시작	//(Open beta)(2004-12-08)
void ShotMissile(CEntity *penShoter, const char *szShotPosTagName
				 , CEntity *penTarget, FLOAT fMoveSpeed
				 , const char *szHitEffectName, const char *szArrowEffectName
				 , bool bCritical
				 , FLOAT fHorizonalOffset = 0.0f, FLOAT fVerticalOffset = 0.0f	//-1.0f ~ 1.0f
				 , INDEX ilArrowType = 0, const char *szMissileModel = NULL);
void ShotMagicContinued(CEntity *penShoter, FLOAT3D vStartPos, FLOATquat3D qStartRot
				 , CEntity *penTarget, FLOAT fMoveSpeed
				 , const char *szHitEffectName, const char *szMagicEffectName
				 , bool bCritical, INDEX iOrbitType, BOOL bDirectTag = FALSE);
void ShotGoWaitBack(CEntity *penShoter, FLOAT3D vStartPos, FLOATquat3D qStartRot
				 , CEntity *penTarget, FLOAT fMoveSpeed, FLOAT fWaitTime
				 , const char *szHitEffectName, const char *szMoveEffectName
				 , bool bCritical
				 , FLOAT fHorizonalOffset = 0.0f, FLOAT fVerticalOffset = 0.0f	//-1.0f ~ 1.0f
				 );
void ShotConnect(CEntity *penShoter, CEntity *penTarget, FLOAT fWaitTime
				 , const char *szHitEffectName, const char *szConnectEffectName);
//안태훈 수정 끝	//(Open beta)(2004-12-08)

%}

class CCharacter: CCharacterBase {
name      "Character";
thumbnail "Mods\\test.tbn";

properties:
	//1 enum	ChaType m_ChaType "Type" 'Y' = CHA_BOSS,
	3 FLOAT m_fBodyAnimTime		= -1.0f,
	5 FLOAT m_fStretch			= 1.0f,
	//80 enum AttkType m_AttackType "Attack Type" 'A' = ATTK_NORMAL,
	91 INDEX m_ChaList			= TITAN,	
	93 FLOAT m_tmDireStrikeStart = 0.0f,

//	116 INDEX m_bKillEnemy = FALSE,

	150 CSoundObject m_soMessage,  // message sounds
	151 FLOAT	m_fScale = 1.0f,
	212 BOOL	m_bRide		= FALSE,		// 펫을 타고 있는가?
	216 INDEX	m_iRideType	= -1,			// 타고 있는 펫의 타입(0일 경우 말, 1일 경우 용)

	223 BOOL	m_bPlayAction = FALSE,	
	225	FLOAT	m_fActionAnimTime = 0.0f,
	226	INDEX	m_nSitDown = 0,//2:앉기, 0:서기.

	227 INDEX idBodyAnim = 0,//1105
	228 INDEX m_nWearArmorUp = -1,
	229 INDEX m_nWearWeapon = -1,
	230 INDEX m_nWearArmorDown = -1,
	231 INDEX m_nWearShield = -1,
	232 INDEX m_nWearGlove = -1,
	233 INDEX m_nWearBoots	= -1,
	234 INDEX m_nWearHead	= -1,	//070223_ttos : 헬멧 추가
	239 BOOL	m_bExtension	= FALSE,
	240 INDEX	m_nHairStyle = -1,		// 헤어 스타일
	241 INDEX	m_nFaceStyle = -1,		// 얼굴 스타일

	242 INDEX	m_nPkMode = 0,		//피케이 모드.
	243 INDEX	m_nPkTitle = 0,	

	244 BOOL m_bPlayProduce = FALSE,//1221
	245 INDEX	m_nPlayProduceNum = -1,
	246 FLOAT 	m_tmDamageTime = 0.0f,
	
	249 INDEX   m_nPkState = 0,
	250 INDEX   m_nLegit = 0,
	251 BOOL	m_bDie	= FALSE,
	252 BOOL	m_bDisappearWeapon = FALSE,
	253 BOOL    m_bChanging = FALSE,
	//254 BOOL    m_bMobChange = FALSE,
	255 INDEX   m_nChangeMonsterId = -1,
	201 BOOL	m_bTransforming = FALSE,

	 25 INDEX	m_iTransformType = 0,
//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
	{
		INDEX					m_nEffectStep;	
		CEffectGroup			*m_pSkillReadyEffect;
		CStaticArray<TagInfo>	m_saLastEffectInfo;

		INDEX	m_iSkillEffectStep;
		CStaticArray<FLOAT> m_afHardCodeValue;
		BOOL	m_bHardcodingSkillLoop;		
		CCharacterAnimation		m_AnimationIndices;

		CModelInstance*		m_pRiderModelInstance;

		// 임시.
		QVect mi_qvOffset;      // current offset from parent model instance
		INDEX mi_iParentBoneID; // ID of parent bone in parent model instance
	}
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)

components:	
	// 사운드 처리가 가장 문제되는 부분.
	1 sound SOUND_POLYMOPH				"Data\\sounds\\character\\public\\C_transformation.wav",
	
	3 sound SOUND_DAIR_STRIKE_BLOW1		"Data\\Sounds\\Character\\rogue\\skill\\CRs_skill_06_blow01.wav",
	4 sound SOUND_DAIR_STRIKE_BLOW2		"Data\\Sounds\\Character\\rogue\\skill\\CRs_skill_06_blow02.wav",
	5 sound SOUND_DAIR_STRIKE_BLOW3		"Data\\Sounds\\Character\\rogue\\skill\\CRs_skill_06_blow03.wav",	
	7 sound SOUND_A_DAIR_STRIKE_BLOW1	"Data\\Sounds\\Character\\rogue\\skill\\CRs_a_skill_06_blow01.wav",
	8 sound SOUND_A_DAIR_STRIKE_BLOW2	"Data\\Sounds\\Character\\rogue\\skill\\CRs_a_skill_06_blow02.wav",
	9 sound SOUND_A_DAIR_STRIKE_BLOW3	"Data\\Sounds\\Character\\rogue\\skill\\CRs_a_skill_06_blow03.wav",

functions:

//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
	BOOL AdjustShadingParameters(FLOAT3D &vLightDirection, COLOR &colLight, COLOR &colAmbient)
	{
		INDEX g_iNPCMinBrightness = _pShell->GetINDEX("g_iNPCMinBrightness");
		g_iNPCMinBrightness = Clamp(g_iNPCMinBrightness, (INDEX)0, (INDEX)240);
		INDEX g_iChaLightModifyValue = _pShell->GetINDEX("g_iChaLightModifyValue");

		BOOL bRecalcAmbient = FALSE;
		BOOL bRecalcDiffuse = FALSE;

		UBYTE ubAH, ubAS, ubAV;
		ColorToHSV(colAmbient, ubAH, ubAS, ubAV);
		UBYTE ubDH, ubDS, ubDV;
		ColorToHSV(colLight, ubDH, ubDS, ubDV);

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
		if(ubAV < ubDV && g_iChaLightModifyValue < ubDV - ubAV)
		{
			INDEX value = (ubDV - ubAV - g_iChaLightModifyValue) >> 1;
			ubDV = Clamp(INDEX(ubDV) - value, INDEX(0), INDEX(255));
			ubAV = Clamp(INDEX(ubAV) + value, INDEX(0), INDEX(255));
			bRecalcAmbient = bRecalcDiffuse = TRUE;
		}

		if(bRecalcDiffuse) {colLight = HSVToColor(ubDH, ubDS, ubDV);}
		if(bRecalcAmbient) {colAmbient = HSVToColor(ubAH, ubAS, ubAV);}
		return TRUE;
	}
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)
//안태훈 수정 시작	//(Option)(0.1)
	void AdjustMipFactor(FLOAT &fMipFactor)
	{
		extern ENGINE_API FLOAT g_fChaLODMul;
		fMipFactor *= g_fChaLODMul;
	}
//안태훈 수정 끝	//(Option)(0.1)
	virtual FLOAT GetLockRotationSpeed(void) 
	{ 
		return AngleDeg(1800.0f); 
	};

	// NOTE : 애완동물을 타고 있을때 문제가 되는 부분은 이 함수로 모델인스턴스를 얻어올것.
	CModelInstance* GetCurrentPlayerModelInstance()
	{
		CModelInstance *pMI = NULL;
		if( m_bRide )
		{
			CModelInstance *pMITemp = GetModelInstance();

			INDEX ctmi = pMITemp->mi_cmiChildren.Count();
			if( ctmi > 0 )
			{
				pMI = &pMITemp->mi_cmiChildren[0];
			}
			else
			{
				pMI			= GetModelInstance();
			}
		}
		else
		{
			pMI			= GetModelInstance();
		}
		return pMI;
	};

	virtual void RidingPet(CEntity *pCharacter, INDEX iPetType )
	{
		SBYTE sbPetType	= iPetType & 0xFF;
		SBYTE sbPetColor =iPetType >>8;

		if( pCharacter->IsCharacter() && !m_bRide )
		{	
			mi_qvOffset			= pCharacter->GetModelInstance()->mi_qvOffset;
			mi_iParentBoneID	= pCharacter->GetModelInstance()->mi_iParentBoneID;			
			//DeleteModelInstance(m_pRiderModelInstance);

			FLOAT3D vPos;
			ANGLE3D vAngle;		
			m_pRiderModelInstance = ObtainModelInstance_t( PetInfo().GetFileName(sbPetType, 2) );

			// [070824: Su-won] PET_COLOR_CHANGE
			if( sbPetColor > 0 )
			{
				CTString strTexFile =PetInfo().GetColoredTexFileName(sbPetType, 2, sbPetColor);
				m_pRiderModelInstance ->mi_aMeshInst[0].mi_tiTextures[0].ti_toTexture.SetData_t( strTexFile );
			}

			INDEX ctmi = m_pRiderModelInstance->mi_cmiChildren.Count();
			if( ctmi > 0 )
			{
				CModelInstance &cmi = m_pRiderModelInstance->mi_cmiChildren[0];
				pCharacter->GetModelInstance()->mi_qvOffset			= cmi.mi_qvOffset;
				pCharacter->GetModelInstance()->mi_iParentBoneID	= cmi.mi_iParentBoneID;
				cmi.Copy(*pCharacter->GetModelInstance());						
			}
			m_bRide		= TRUE;
			m_iRideType = sbPetType;

			pCharacter->GetModelInstance()->Copy(*m_pRiderModelInstance);		
			pCharacter->GetModelInstance()->NewClearState(0.0f);
			ctmi = pCharacter->GetModelInstance()->mi_cmiChildren.Count();		
			if( ctmi > 0 )
			{
				CModelInstance &cmi = pCharacter->GetModelInstance()->mi_cmiChildren[0];			
				//_pNetwork->MyCharacterInfo.itemEffect.Refresh(&cmi.m_tmSkaTagManager, 1);
				if( en_pCharacterTarget )
				{
					en_pCharacterTarget->cha_itemEffect.Refresh(&cmi.m_tmSkaTagManager, 1);
				}
			}
			IdleAnim();
		}	
	}

	virtual void LeavingPet(CEntity *pCharacter )
	{
		if( pCharacter->IsCharacter() && m_bRide )
		{
			m_pRiderModelInstance->Clear();
			INDEX ctmi = pCharacter->GetModelInstance()->mi_cmiChildren.Count();
			if( ctmi > 0 )
			{
				CModelInstance &cmi = pCharacter->GetModelInstance()->mi_cmiChildren[0];						
				m_pRiderModelInstance->Copy(cmi);			
				pCharacter->GetModelInstance()->Copy(*m_pRiderModelInstance);
				DeleteModelInstance(m_pRiderModelInstance);
				pCharacter->GetModelInstance()->mi_qvOffset			= mi_qvOffset;
				pCharacter->GetModelInstance()->mi_iParentBoneID	= mi_iParentBoneID;
				pCharacter->GetModelInstance()->NewClearState(0.0f);
				//_pNetwork->MyCharacterInfo.itemEffect.Refresh(&pCharacter->GetModelInstance()->m_tmSkaTagManager, 1);
				if( en_pCharacterTarget )
				{
					en_pCharacterTarget->cha_itemEffect.Refresh(&pCharacter->GetModelInstance()->m_tmSkaTagManager, 1);
					en_pCharacterTarget->cha_statusEffect.Refresh(&pCharacter->GetModelInstance()->m_tmSkaTagManager, CStatusEffect::R_NONE);
				}
			}
			m_bRide		= FALSE;
			m_iRideType = -1;
			IdleAnim();
		}
	}

	void OnInitialize(const CEntityEvent &eeInput)
	{
		CCharacterBase::OnInitialize(eeInput);
	};

	void OnEnd(void) 
	{
		CCharacterBase::OnEnd();		
	};	
	

	void CCharacter_OnInitClass(void)
	{
		CDLLEntityClass *pdec = &CCharacter_DLLClass;

		pdec->PrecacheSound(  SOUND_POLYMOPH );
	
		pdec->PrecacheSound( SOUND_DAIR_STRIKE_BLOW1);
		pdec->PrecacheSound( SOUND_DAIR_STRIKE_BLOW2);
		pdec->PrecacheSound( SOUND_DAIR_STRIKE_BLOW3);
		
		pdec->PrecacheSound( SOUND_A_DAIR_STRIKE_BLOW1);
		pdec->PrecacheSound( SOUND_A_DAIR_STRIKE_BLOW2);
		pdec->PrecacheSound( SOUND_A_DAIR_STRIKE_BLOW3);

	}

	//--------------------------------------------------------------
	// 캐릭터의 애니메이션 이름 설정...
	//--------------------------------------------------------------

	void SetAnimation(INDEX iType)
	{
		for( int i = ANIM_START; i <= ANIM_END; ++i )
		{
			m_AnimationIndices.m_iAnimation[i] = ska_GetIDFromStringTable(JobInfo().GetAnimationName( iType, i ));
		}		
	}

	// describe how this enemy killed player
	virtual CTString GetPlayerKillDescription(const CTString &strPlayerName, const EDeath &eDeath)
	{
		CTString str;
		str.PrintF(TRANS("A Orcman sent %s into the halls of Valhalla"), strPlayerName);
		return str;
	}

	/* Entity info */
	void *GetEntityInfo(void)
	{
		return &eiChaBoss;	
	};

	void Precache(void) 
	{
		CCharacterBase::Precache();
//안태훈 수정 시작	//(Fix Snd Effect Bug)(0.1)
		if(en_pmiModelInstance) { en_pmiModelInstance->m_tmSkaTagManager.SetOwner(this); }
//안태훈 수정 끝	//(Fix Snd Effect Bug)(0.1)
	};

//강동민 수정 시작 다중 공격 작업	08.27
	virtual BOOL IsCharacter() const {return TRUE;};
//강동민 수정 끝 다중 공격 작업		08.27

	void InflictDirectDamage(CEntity *penToDamage, CEntity *penInflictor, enum DamageType dmtType,
	FLOAT fDamageAmmount, const FLOAT3D &vHitPoint, const FLOAT3D &vDirection)
	{		
		if(penToDamage) 
		{
			if( !(penToDamage->GetFlags()&ENF_ALIVE ) )	{ return; }
			_pUIMgr->ShowDamage( penToDamage->en_ulID );
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
			StartEffectGroup("Normal Hit2", _pTimer->CurrentTick(), vHitPoint, quat);		//Hardcording
		}
		_pUIMgr->ShowDamage( en_ulID );

		CCharacterBase::ReceiveDamage(penInflictor, dmtType, fDamageAmmount, vHitPoint, vDirection);

	};

	//--------------------------------------------------------------
	// 캐릭터의 애니메이션을 설정하는 함수들...
	//--------------------------------------------------------------
	// damage anim
	INDEX AnimForDamage(FLOAT fDamage) 
	{
		ASSERT(m_AnimationIndices.m_iAnimation[ANIM_DAMAGE]		!= -1	&& "Invalid Animation");
		ASSERT(m_AnimationIndices.m_iAnimation[ANIM_EXT_DAMAGE]	!= -1	&& "Invalid Animation");
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");

		INDEX iDamageAnimation = -1;

		if( !m_bExtension || m_bMobChange )
		{
			if( m_bRide )
			{
				// 레어 펫 추가로 짝수 타입은 말...060822 wooss
				if( m_iRideType%2 == CPetInfo::PET_HORSE )
				{
					iDamageAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_DAMAGE];
				}
				else
				{
					iDamageAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_DAMAGE];
				}
			}
			else
			{
				iDamageAnimation = m_AnimationIndices.m_iAnimation[ANIM_DAMAGE];			
			}
		}
		else
		{
			if( m_bRide )
			{
				// 레어 펫 추가로 짝수 타입은 말...060822 wooss
				if( m_iRideType%2 == CPetInfo::PET_HORSE )
				{
					iDamageAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_DAMAGE];
				}
				else
				{
					iDamageAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_DAMAGE];
				}
			}
			else
			{
				iDamageAnimation = m_AnimationIndices.m_iAnimation[ANIM_EXT_DAMAGE];			
			}
		}

		//GetModelInstance()->AddAnimation(iDamageAnimation,AN_CLEAR,1,0);
		AddAnimation(iDamageAnimation, AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
		if(m_bDisappearWeapon)//무기가 지워져 있다면, 나타나게 해줘야 한다.
		{
			AppearWearingWeapon();
			m_bDisappearWeapon = FALSE;
		}
		m_tmDamageTime = _pTimer->CurrentTick();		

		return iDamageAnimation;
	};

	// death
	INDEX AnimForDeath(void)
	{	
		ASSERT(m_AnimationIndices.m_iAnimation[ANIM_DIE]		!= -1	&& "Invalid Animation");
		ASSERT(m_AnimationIndices.m_iAnimation[ANIM_EXT_DIE]	!= -1	&& "Invalid Animation");		
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");

		if(m_bDisappearWeapon)//무기가 지워져 있다면, 나타나게 해줘야 한다.
		{
			AppearWearingWeapon();
			m_bDisappearWeapon = FALSE;
		}

		if(m_bMobChange)
		{
			ReturnChange(TRUE);
		}
		else if( m_bIsTransform )
		{
			ReturnSorcerer(TRUE);
		}

		GetModelInstance()->StopAllAnimEffect(0);
		GetModelInstance()->mi_aqAnims.aq_Lists.Clear();

		INDEX iDieAnimation = -1;

		if( !m_bExtension || m_bMobChange )
		{
			if( m_bRide )
			{
				// 레어 펫 추가로 짝수 타입은 말...060822 wooss
				if( m_iRideType%2 == CPetInfo::PET_HORSE )
				{
					iDieAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_DIE];
				}
				else
				{
					iDieAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_DIE];
				}				
			}
			else
			{
				iDieAnimation = m_AnimationIndices.m_iAnimation[ANIM_DIE];
			}
		}
		else
		{
			if( m_bRide )
			{
				// 레어 펫 추가로 짝수 타입은 말...060822 wooss
				if( m_iRideType%2 == CPetInfo::PET_HORSE )
				{
					iDieAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_DIE];
				}
				else
				{
					iDieAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_DIE];
				}				
			}
			else
			{
				iDieAnimation = m_AnimationIndices.m_iAnimation[ANIM_EXT_DIE];
			}
		}

		AddAnimation(iDieAnimation, AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
		//GetModelInstance()->AddAnimation(iDieAnimation, 0, 1, 0);
		return iDieAnimation;
	};

	// 죽을때 뭔가가...
	FLOAT WaitForDust(FLOAT3D &vStretch)
	{
		//0317 kwon
		vStretch=FLOAT3D(1,1,1);		
		if( GetModelInstance()->IsAnimationPlaying(m_AnimationIndices.m_iAnimation[ANIM_DIE]) || 
			GetModelInstance()->IsAnimationPlaying(m_AnimationIndices.m_iAnimation[ANIM_EXT_DIE]) ) 
		{
			return 0.5f;
		} 
		return -1.0f;
	};

	void DeathNotify(void) 
	{
		// 몸의 비중...
		en_fDensity = 500.0f;
	};

	// Idle Animation
	void IdleAnim(void)
	{
		CModelInstance *pMI = GetModelInstance();
		INDEX iIdleAnimation = -1;
		if(m_nWearWeapon != -1) //무기를 들고 있다면,
		{
			if(m_bExtension)
			{
				if( m_bRide )
				{
					// 레어 펫 추가로 짝수 타입은 말...060822 wooss
					if( m_iRideType%2 == CPetInfo::PET_HORSE )
					{
						iIdleAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_IDLE_2];
					}
					else
					{
						iIdleAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_IDLE_2];
					}					
				}
				else
				{
					iIdleAnimation = m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_IDLE];
				}
			}
			else
			{
				if( m_bRide )
				{
					// 레어 펫 추가로 짝수 타입은 말...060822 wooss
					if( m_iRideType%2 == CPetInfo::PET_HORSE )
					{
						iIdleAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_IDLE_1];
					}
					else
					{
						iIdleAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_IDLE_1];
					}					
				}
				else
				{
					iIdleAnimation = m_AnimationIndices.m_iAnimation[ANIM_ATTACK_IDLE];
				}
			}
		}
		else
		{
			if( m_bRide )
			{
				// 레어 펫 추가로 짝수 타입은 말...060822 wooss
				if( m_iRideType%2 == CPetInfo::PET_HORSE )
				{
					iIdleAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_IDLE_1];
				}
				else
				{
					iIdleAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_IDLE_1];
				}				
			}
			else
			{
				iIdleAnimation = m_AnimationIndices.m_iAnimation[ANIM_IDLE];
			}
		}
		AddAnimation(iIdleAnimation, AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
		//pMI->AddAnimation(iIdleAnimation,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);
	};

	// virtual anim functions
	void StandingAnim(void) 
	{
		//ASSERT(m_AnimationIndices.m_iAnimation[ANIM_IDLE]	!= -1	&& "Invalid Animation");
		//ASSERT(m_AnimationIndices.m_iAnimation[ANIM_ATTACK_IDLE]	!= -1	&& "Invalid Animation");
		//ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");

		CModelInstance *pMI = GetModelInstance();
/*
		if(m_fScale != 1.0f)
		{
			pMI->StretchModel(FLOAT3D(1.0f,1.0f,1.0f));
			m_fScale = 1.0f;
		}
*/
		if(m_bDisappearWeapon)//무기가 지워져 있다면, 나타나게 해줘야 한다.
		{
			AppearWearingWeapon();
			m_bDisappearWeapon = FALSE;
		}

		if(m_nSitDown == 2)
		{
			INDEX iAnim = -1;
			if( m_bRide )
			{				
				// 레어 펫 추가로 짝수 타입은 말...060822 wooss
				if( m_iRideType%2 == CPetInfo::PET_HORSE )
				{
					iAnim = m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_SIT_CONTINUE];
				}
				else
				{
					iAnim = m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_SIT_CONTINUE];
				}

			}
			else
			{
				iAnim = m_AnimationIndices.m_iAnimation[ANIM_SIT_CONTINUE];				
			}	
			AddAnimation(iAnim, AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);			
			return;
		}
		IdleAnim();				
		
	};
	void RunningAnim(void) 
	{
		//ASSERT(m_AnimationIndices.m_iAnimation[ANIM_RUN_1]		!= -1	&& "Invalid Animation");
		//ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");

		FLOAT fRunSpeedMul =1.0f;

		CModelInstance *pMI = GetModelInstance();
/*	
		if(m_fScale != 1.0f)
		{
			pMI->StretchModel(FLOAT3D(1.0f,1.0f,1.0f));
			m_fScale = 1.0f;
		}
*/
		if(m_bSpeedUp)
		{
			// FIXME : 어떻게 계산된 값인지 모르겠음.
			if(m_ChaList == TITAN)
			{
				fRunSpeedMul = 8.0f/6.2f;
			}
			else if(m_ChaList == HEALER)
			{
				fRunSpeedMul = 8.0f/7.0f;
			}
			else if(m_ChaList == KNIGHT)
			{
				fRunSpeedMul = 8.0f/6.0f;
			}
			else if(m_ChaList == MAGE)
			{
				fRunSpeedMul = 8.0f/6.0f;
			}
			else if(m_ChaList == ROGUE)
			{
				fRunSpeedMul = 8.0f/6.0f;
			}
			else if(m_ChaList == SORCERER)
			{
				fRunSpeedMul = 8.0f/6.0f;
			}
		}

		if(IsFlagOn(ENF_HIDDEN) && m_bHardcodingSkillLoop && (_pTimer->GetLerpedCurrentTick() - m_tmDireStrikeStart > 1.7f))
		{
			// 강동민 수정 시작
			// NOTE : 움직일때 Invisible이 풀리는 부분 수정.			
			for(INDEX i=0; _pNetwork->ga_srvServer.srv_actCha.Count(); ++i)
			{
				CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[i];
				if(ct.cha_iClientIndex == en_ulID)
				{
					if(ct.cha_statusEffect.IsState(EST_ASSIST_INVISIBLE) && _pNetwork->m_ubGMLevel < 2)
					{
						SetFlagOn(ENF_HIDDEN);
					}
					else
					{                                                                                                                                                                                                           
						SetFlagOff(ENF_HIDDEN);
					}
					break;				
				}
			}
		}

		if(m_bDisappearWeapon)//무기가 지워져 있다면, 나타나게 해줘야 한다.
		{
			AppearWearingWeapon();
			m_bDisappearWeapon = FALSE;
		}
		if(m_nWearWeapon != -1 && !m_bMobChange) //무기를 들고 있다면,//1008
		{
			if( !m_bExtension )
			{
				if( m_bRide )
				{
					// 레어 펫 추가로 짝수 타입은 말...060822 wooss
					if( m_iRideType%2 == CPetInfo::PET_HORSE )
					{
						//pMI->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_RUN_1],AN_LOOPING|AN_NORESTART|AN_CLEAR,1,ESKA_MASTER_MODEL_INSTANCE,fRunSpeedMul);
						AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_RUN_1], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE, fRunSpeedMul);
					}
					else
					{
						//pMI->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_RUN_1],AN_LOOPING|AN_NORESTART|AN_CLEAR,1,ESKA_MASTER_MODEL_INSTANCE,fRunSpeedMul);
						AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_RUN_1], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE, fRunSpeedMul);
					}
				}
				else
				{
					//pMI->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RUN_2],AN_LOOPING|AN_NORESTART|AN_CLEAR,1,ESKA_MASTER_MODEL_INSTANCE,fRunSpeedMul);
					AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RUN_2], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE, fRunSpeedMul);
				}
			}
			else
			{
				if( m_bRide )
				{
					// 레어 펫 추가로 짝수 타입은 말...060822 wooss
					if( m_iRideType%2 == CPetInfo::PET_HORSE )
					{
						//pMI->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_RUN_1],AN_LOOPING|AN_NORESTART|AN_CLEAR,1,ESKA_MASTER_MODEL_INSTANCE,fRunSpeedMul);
						AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_RUN_1], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE, fRunSpeedMul);
					}
					else
					{
						//pMI->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_RUN_1],AN_LOOPING|AN_NORESTART|AN_CLEAR,1,ESKA_MASTER_MODEL_INSTANCE,fRunSpeedMul);
						AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_RUN_1], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE, fRunSpeedMul);
					}
				}
				else
				{
					//pMI->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_EXT_RUN_2],AN_LOOPING|AN_NORESTART|AN_CLEAR,1,ESKA_MASTER_MODEL_INSTANCE,fRunSpeedMul);
					AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_EXT_RUN_2], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE, fRunSpeedMul);
				}
			}
		}
		else
		{
			if( m_bRide )
			{
				// 레어 펫 추가로 짝수 타입은 말...060822 wooss
				if( m_iRideType%2 == CPetInfo::PET_HORSE )
				{
					//pMI->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_RUN_1],AN_LOOPING|AN_NORESTART|AN_CLEAR,1, ESKA_MASTER_MODEL_INSTANCE,fRunSpeedMul);
					AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_RUN_1], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE, fRunSpeedMul);
				}
				else
				{
					//pMI->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_RUN_1],AN_LOOPING|AN_NORESTART|AN_CLEAR,1, ESKA_MASTER_MODEL_INSTANCE,fRunSpeedMul);
					AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_RUN_1], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE, fRunSpeedMul);
				}
			}
			else
			{
				//pMI->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RUN_1],AN_LOOPING|AN_NORESTART|AN_CLEAR,1, ESKA_MASTER_MODEL_INSTANCE,fRunSpeedMul);
				AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RUN_1], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE, fRunSpeedMul);
			}
		}
	};
	void WalkingAnim(void) 
	{
		//ASSERT(m_AnimationIndices.m_iAnimation[ANIM_WALK_1]		!= -1	&& "Invalid Animation");
		//ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");
		if(m_bDisappearWeapon)//무기가 지워져 있다면, 나타나게 해줘야 한다.
		{
			AppearWearingWeapon();
			m_bDisappearWeapon = FALSE;
		}
		if( m_bRide )
		{
			// 레어 펫 추가로 짝수 타입은 말...060822 wooss
			if( m_iRideType%2 == CPetInfo::PET_HORSE )
			{
				AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_WALK_1], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
				//GetModelInstance()->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_WALK_1],AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);
			}
			else
			{
				AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_WALK_1], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
				//GetModelInstance()->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_WALK_1],AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);
			}			
		}
		else
		{
			AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_WALK_1], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
			//GetModelInstance()->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_WALK_1],AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);
		}
	};
	void RotatingAnim(void)  
	{
		WalkingAnim(); 
	};

	// adjust sound and watcher parameters here if needed
	void EnemyPostInit(void) 
	{
		// set sound default parameters
		m_soSound.Set3DParameters(160.0f, 50.0f, 1.0f, 1.0f);
	};

	void InitCurrentWearing()
	{
		m_nWearHead			= -1;
		m_nWearArmorUp		= -1;
	    m_nWearWeapon		= -1;
	    m_nWearArmorDown	= -1;
	    m_nWearShield		= -1;
	    m_nWearGlove		= -1;
	    m_nWearBoots		= -1;		
	}

	// FIXME : Player.es와 같은 내용의 함수임.
	// FIXME : (옷 입고, 벗는 부분) 중복두 많고, 엄청 꼬여 있음.
	void WearingDefaultArmor(int type)//디폴트 몸 입히기.(갑옷을 벗은후에 호출) 
	{
		CModelInstance *pMI = GetCurrentPlayerModelInstance();
		MeshInstance *mi;
		CTFileName fnFileName;

		// FIXME : type 값과 입는 위치값이 다르기 때문에 문제 발생...
		int iWearPos = -1;
		switch( type )
		{

		case WEAR_HELMET:
			iWearPos = HEAD;			
			break;

		case WEAR_JACKET:
			iWearPos = BODYUP;
			break;
		case WEAR_PANTS:
			iWearPos = BODYDOWN;
			break;
		case WEAR_GLOVES:
			iWearPos = HAND;
			break;
		case WEAR_BOOTS:
			iWearPos = FOOT;
			break;
		}

		if( iWearPos == -1 )
		{
			return;
		}

		// 바지를 갈아 입을때는 치마도 같이...
		if( type == WEAR_PANTS )
		{
			// Mesh
			fnFileName = JobInfo().GetMeshName( m_ChaList, SKIRT );
			if(strlen( fnFileName ) > 0)
			{			
				mi = pMI->AddArmor( fnFileName );

				// Texture
				fnFileName = JobInfo().GetTextureName( m_ChaList, SKIRT );
				pMI->AddTexture_t( fnFileName, fnFileName.FileName(), mi );	

				// NormalMap
				fnFileName = JobInfo().GetTexNormalName( m_ChaList, SKIRT );
				if(strcmp(fnFileName, ""))
				{					
					pMI->AddTexture_t(fnFileName, fnFileName.FileName(), mi);
				}
			}
		}

		// Mesh
		fnFileName = JobInfo().GetMeshName( m_ChaList, iWearPos );
		mi = pMI->AddArmor( fnFileName );

		// Texture
		fnFileName = JobInfo().GetTextureName( m_ChaList, iWearPos );
		pMI->AddTexture_t( fnFileName, fnFileName.FileName(), mi );

		// NormalMap
		fnFileName = JobInfo().GetTexNormalName( m_ChaList, iWearPos );
		if(strcmp(fnFileName, ""))
		{			
			pMI->AddTexture_t(fnFileName, fnFileName.FileName(), mi);
		}
		for(INDEX i=0; i<_pNetwork->ga_srvServer.srv_actCha.Count(); ++i)
		{
			CCharacterTarget &ct = _pNetwork->ga_srvServer.srv_actCha[i];
			if(ct.cha_iClientIndex == en_ulID)
			{
				ct.cha_itemEffect.Refresh(&GetCurrentPlayerModelInstance()->m_tmSkaTagManager, ct.cha_sbItemEffectOption);
				break;
			}
		}
	}

	void DisplayCurrentWear()
	{
		INDEX delcnt = 0,cnt;
		CTFileName strBmName;
		CTFileName fnFileName;
		cnt = GetCurrentPlayerModelInstance()->mi_aMeshInst.Count(); 
		CPrintF("%s \n",m_strName);				
		for(int i=0;i<cnt;++i)
		{
			strBmName = GetCurrentPlayerModelInstance()->mi_aMeshInst[i].mi_pMesh->GetName();
			CPrintF("%s \n",strBmName);
		}
	}

	virtual	void DeleteDefaultArmor(int type)
	{
		CModelInstance *pMI = GetCurrentPlayerModelInstance();
		_pGameState->DeleteDefaultArmor( pMI, type, m_ChaList );
		for(INDEX i=0; i<_pNetwork->ga_srvServer.srv_actCha.Count(); ++i)
		{
			CCharacterTarget &ct = _pNetwork->ga_srvServer.srv_actCha[i];
			if(ct.cha_iClientIndex == en_ulID)
			{
				ct.cha_itemEffect.Refresh(&GetCurrentPlayerModelInstance()->m_tmSkaTagManager, ct.cha_sbItemEffectOption);
				break;
			}
		}
	}

	// FIXME : Player.es와 같은 내용의 함수임.	
	// FIXME : 뭔가 꼬여 있음...
	void DeleteCurrentArmor(int index) // 순수하게 갑옷을 벗기기.
	{	
		CItemData &ItemData = _pNetwork->GetItemData(index);
		CModelInstance *pMI = GetCurrentPlayerModelInstance();
		_pGameState->TakeOffArmor( pMI, ItemData );				
		for(INDEX i=0; i<_pNetwork->ga_srvServer.srv_actCha.Count(); ++i)
		{
			CCharacterTarget &ct = _pNetwork->ga_srvServer.srv_actCha[i];
			if(ct.cha_iClientIndex == en_ulID)
			{
				ct.cha_itemEffect.Refresh(&GetCurrentPlayerModelInstance()->m_tmSkaTagManager, ct.cha_sbItemEffectOption);
				break;
			}
		}
	}

//0127
//사교동작시에 무기를 지웠다가 나타나게 하기.
	void DeleteWearingWeapon(BOOL bException)
	{
//		if((bException && m_bDisappearWeapon) || (!bException && !m_bDisappearWeapon))
		if(!m_bDisappearWeapon)
		{
			int iCurWear = IsThereWearing(WEAR_WEAPON);
			if(iCurWear == -1) 
			{
				return;
			}
			CItemData &ItemData	= _pNetwork->GetItemData(iCurWear);
			CModelInstance *pMI = GetCurrentPlayerModelInstance();

			_pGameState->TakeOffArmor( pMI, ItemData );
			m_bExtension = FALSE;
			
			m_bDisappearWeapon = TRUE;

			//물 뿌리기 액션일 때
			if( m_nPlayActionNum ==ACTION_NUM_WATER_SPREAD )
			{
				CModelInstance *pMI = GetCurrentPlayerModelInstance();

				//바가지 모델 추가
				UBYTE ubJob = m_ChaList;
				MeshInstance *mi = pMI->AddArmor( _afnDishMeshName[ubJob][0] );
				pMI->AddTexture_t( _afnDishTextureName, _afnDishTextureName.FileName(),mi);
				pMI->AddArmor( _afnDishMeshName[ubJob][1] );

				//물뿌리는 이펙트 추가
				if( ubJob==0 || ubJob==1 || ubJob==5)	//남자일때...
				{
					StartEffectGroup("water_sm"		//Hardcording
							, &GetCurrentPlayerModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
				}
				else	//여자일때...
				{
					StartEffectGroup("water_sw"
							, &GetCurrentPlayerModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
				}
			}
		}
		for(INDEX i=0; i<_pNetwork->ga_srvServer.srv_actCha.Count(); ++i)
		{
			CCharacterTarget &ct = _pNetwork->ga_srvServer.srv_actCha[i];
			if(ct.cha_iClientIndex == en_ulID)
			{
				ct.cha_itemEffect.Refresh(&GetCurrentPlayerModelInstance()->m_tmSkaTagManager, ct.cha_sbItemEffectOption);
				break;
			}
		}
	}

	void AppearWearingWeapon()
	{
		if(m_bDisappearWeapon)
		{
			int iCurWear = IsThereWearing(WEAR_WEAPON);
			if(iCurWear == -1) 
			{
				return;
			}
			CModelInstance *pMI = GetCurrentPlayerModelInstance();
			CItemData &ItemData	= _pNetwork->GetItemData(iCurWear);

			_pGameState->WearingArmor( pMI, ItemData );
			
			m_bDisappearWeapon = FALSE;

			//물 뿌리기 액션일 때 - 바가지 모델 삭제
			if( m_nPlayActionNum ==ACTION_NUM_WATER_SPREAD )
			{
				CModelInstance *pMI = GetCurrentPlayerModelInstance();
				pMI->DeleteMesh(_afnDishMeshName[m_ChaList][0]);
				pMI->DeleteMesh(_afnDishMeshName[m_ChaList][1]);
			}
		}	
		for(INDEX i=0; i<_pNetwork->ga_srvServer.srv_actCha.Count(); ++i)
		{
			CCharacterTarget &ct = _pNetwork->ga_srvServer.srv_actCha[i];
			if(ct.cha_iClientIndex == en_ulID)
			{
				ct.cha_itemEffect.Refresh(&GetCurrentPlayerModelInstance()->m_tmSkaTagManager, ct.cha_sbItemEffectOption);
				break;
			}
		}
	}

	// FIXME : Player.es와 같은 내용의 함수임.	
	// FIXME : (옷 입고, 벗는 부분) 중복두 많고, 엄청 꼬여 있음.
	void TakeOffWearing(int wear_pos ,int ct_Index) //갑옷 벗기고 맨몸 붙이기. // 일본에 적용된 소스 
	{
		int iCurWear = IsThereWearing(wear_pos);
		if(iCurWear == -1 || ct_Index == -1) 
		{
			return;
		}

		CCharacterTarget &ct = _pNetwork->ga_srvServer.srv_actCha[ct_Index];
		CModelInstance *pMI = GetCurrentPlayerModelInstance();
		CItemData &ItemData	= _pNetwork->GetItemData(iCurWear);

		if (ItemData.GetSubType() == CItemData::ITEM_SHIELD_HEAD && (CTString)ItemData.GetItemSmcFileName() == MODEL_TREASURE)
		{
			return;
		}

		if(ItemData.GetType() == CItemData::ITEM_SHIELD || ItemData.GetType() == CItemData::ITEM_WEAPON)
		{
			if((CTString)ItemData.GetItemSmcFileName() != MODEL_TREASURE)
			{
				_pGameState->TakeOffArmor( pMI, ItemData );
				
				if(ItemData.GetSubType() == CItemData::ITEM_SHIELD_HEAD)
				{
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ChangeHairMesh(pMI,ct.cha_iType, m_nHairStyle-1);
				}
				else
				{
					WearingDefaultArmor(ItemData.GetWearingPosition());
				}
			}
		}
						
		SetWearing(wear_pos, -1); //맨몸이다...
		StandingAnim();
		ct.cha_itemEffect.Refresh(&GetCurrentPlayerModelInstance()->m_tmSkaTagManager, ct.cha_sbItemEffectOption);
	}
	//원본 소스
	void TakeOffWearing(int wear_pos) //갑옷 벗기고 맨몸 붙이기.
	{
//강동민 수정 시작 싱글 던젼 작업	08.09
		int iCurWear = IsThereWearing(wear_pos);
		if(iCurWear == -1) 
		{
			return;
		}


		CModelInstance *pMI = GetCurrentPlayerModelInstance();
		CItemData &ItemData	= _pNetwork->GetItemData(iCurWear);
//강동민 수정 끝 싱글 던젼 작업		08.09
		
		//0609 kwon 추가.
		// FIXME : 타입만 체크하면 될거 같은데...
		if((ItemData.GetType() == CItemData::ITEM_SHIELD
			&& 	(ItemData.GetSubType() == CItemData::ITEM_SHIELD_COAT 
			|| 	ItemData.GetSubType() == CItemData::ITEM_SHIELD_PANTS 
			|| 	ItemData.GetSubType() == CItemData::ITEM_SHIELD_GLOVE 
			|| 	ItemData.GetSubType() == CItemData::ITEM_SHIELD_SHOES
			|| 	ItemData.GetSubType() == CItemData::ITEM_SHIELD_SHIELD
			 )) ||  ItemData.GetType() == CItemData::ITEM_WEAPON //0808 무기도 이제 매쉬를 쓴다.
		)
		//if(ItemData.GetWearingPosition() != CItemData::ITEM_WEAR_NONE)
		{
			_pGameState->TakeOffArmor( pMI, ItemData );
			WearingDefaultArmor(ItemData.GetWearingPosition());
		}
						
		SetWearing(wear_pos, -1); //맨몸이다...

		StandingAnim();
		for(INDEX i=0; i<_pNetwork->ga_srvServer.srv_actCha.Count(); ++i)
		{
			CCharacterTarget &ct = _pNetwork->ga_srvServer.srv_actCha[i];
			if(ct.cha_iClientIndex == en_ulID)
			{
				ct.cha_itemEffect.Refresh(&GetCurrentPlayerModelInstance()->m_tmSkaTagManager, ct.cha_sbItemEffectOption);
				break;
			}
		}
	}

	// FIXME : Player.es와 같은 내용의 함수임.
	// FIXME : (옷 입고, 벗는 부분) 중복두 많고, 엄청 꼬여 있음.
	// FIXME : 왜 Player.es와 Character.es에서 처리하는 루틴이 조금씩 다른걸까???
	void WearingArmor(int index) //갑옷을 입을때 호출.
	{		
		CItemData &ItemData	= _pNetwork->GetItemData(index);
		INDEX i;

		if(g_bHead_change) // 일본 헬멧 추가 관련 타로컬이 들어가면 안됨
		{
			CCharacterTarget* ct;
			CItemData CurWearItemData;
			int iCurWear = -1;
			iCurWear = IsThereWearing(ItemData.GetWearingPosition());
			
			if (iCurWear != -1)
			{
				CurWearItemData	= _pNetwork->GetItemData(iCurWear);
			}

			for (i=0; _pNetwork->ga_srvServer.srv_actCha.Count(); ++i)
			{
				ct = &_pNetwork->ga_srvServer.srv_actCha[i];

				if (ct->cha_iClientIndex == en_ulID)	{ break; }
			}

			if(ItemData.GetType() == CItemData::ITEM_SHIELD || ItemData.GetType() == CItemData::ITEM_WEAPON )
			{
				// FIXME : 이 위치에 들어가야 할거 같지는 않은데....		
				
				const BOOL bExtension = _pNetwork->IsExtensionState( m_ChaList, ItemData );

				if( ItemData.GetType() == CItemData::ITEM_WEAPON )
				{
					ct->cha_bExtension	= bExtension;
					m_bExtension		= bExtension;
				}
				CModelInstance *pMI = GetCurrentPlayerModelInstance();

				if(IsThereWearing(ItemData.GetWearingPosition()) == CItemData::ITEM_WEAR_NONE) //현재 아무것도 안입고 있었다면,
				{
						_pGameState->DeleteDefaultArmor( pMI, ItemData.GetWearingPosition(), m_ChaList );
				}			
				else
				{
					// 갑옷 갈아입기.
					DeleteCurrentArmor(IsThereWearing(ItemData.GetWearingPosition()));//입고 있는 갑옷 지우기.					
				}

				if((CTString)ItemData.GetItemSmcFileName() == MODEL_TREASURE &&
					ItemData.GetSubType() == CItemData::ITEM_SHIELD_HEAD)
				{
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ChangeHairMesh(pMI,ct->cha_iType, m_nHairStyle-1);
				}
				else
				{
					_pGameState->WearingArmor( pMI, ItemData );
				}

			}

			SetWearing(ItemData.GetWearingPosition(), index );
			StandingAnim();

			ct->cha_itemEffect.Refresh(&GetCurrentPlayerModelInstance()->m_tmSkaTagManager, ct->cha_sbItemEffectOption);

		}		
		else
		{
			//0609 kwon 추가.
			// FIXME : 방어구 서브 타입을 체크할 필요가 있나?
			if((ItemData.GetType() == CItemData::ITEM_SHIELD
				&& 	(ItemData.GetSubType() == CItemData::ITEM_SHIELD_COAT 
				|| 	ItemData.GetSubType() == CItemData::ITEM_SHIELD_PANTS 
				|| 	ItemData.GetSubType() == CItemData::ITEM_SHIELD_GLOVE 
				|| 	ItemData.GetSubType() == CItemData::ITEM_SHIELD_SHOES
				|| 	ItemData.GetSubType() == CItemData::ITEM_SHIELD_SHIELD
				)) ||  ItemData.GetType() == CItemData::ITEM_WEAPON //0808 무기도 이제 매쉬를 쓴다.
			)
			{
				// FIXME : 이 위치에 들어가야 할거 같지는 않은데....
				if( ItemData.GetType() == CItemData::ITEM_WEAPON )
				{
					const BOOL bExtension = _pNetwork->IsExtensionState( m_ChaList, ItemData );
					for(i=0; _pNetwork->ga_srvServer.srv_actCha.Count(); ++i)
					{
						CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[i];
						if(ct.cha_iClientIndex == en_ulID)
						{
							ct.cha_bExtension	= bExtension;
							m_bExtension		= bExtension;
							break;				
						}
					}
				}

				CModelInstance *pMI = GetCurrentPlayerModelInstance();
				//0605 kwon 추가.
				if(IsThereWearing(ItemData.GetWearingPosition()) == CItemData::ITEM_WEAR_NONE) //현재 아무것도 안입고 있었다면,
				{
					_pGameState->DeleteDefaultArmor( pMI, ItemData.GetWearingPosition(), m_ChaList );
				}
				else
				{
					// 갑옷 갈아입기.
					DeleteCurrentArmor(IsThereWearing(ItemData.GetWearingPosition()));//입고 있는 갑옷 지우기.					
				}
				
				_pGameState->WearingArmor( pMI, ItemData );
			}

			SetWearing(ItemData.GetWearingPosition(), index );
			//CurrentWearing[ItemData.GetWearingPosition()] = index; //새로운 갑옷 인덱스 셋팅.

			StandingAnim();
	//		CPrintF(TRANS("Wear!!!  CurrentWearing[%d] = %d \n"),ItemData.GetWearingPosition(), index);	
			for(i=0; i<_pNetwork->ga_srvServer.srv_actCha.Count(); ++i)
			{
				CCharacterTarget &ct = _pNetwork->ga_srvServer.srv_actCha[i];
				if(ct.cha_iClientIndex == en_ulID)
				{
					ct.cha_itemEffect.Refresh(&GetCurrentPlayerModelInstance()->m_tmSkaTagManager, ct.cha_sbItemEffectOption);
					break;
				}
			}

		}
	}

INDEX IsThereWearing(int pos)
{
/*
	ARMOR				= 1,
	WEAPON				= 2,
	PANTS				= 3,
	SHIELD				= 4,
	GLOVE				= 5,
	BOOTS				= 6,

	228 INDEX m_nWearArmorUp = -1,
	229 INDEX m_nWearWeapon = -1,
	230 INDEX m_nWearArmorDown = -1,
	231 INDEX m_nWearShield = -1,
	232 INDEX m_nWearGlove = -1,
	233 INDEX m_nWearBoots = -1,
*/
	int wearId = -1;
	switch(pos)
	{

		case WEAR_HELMET:
			{
				if(g_bHead_change) // 일본 헬멧 추가 관련 타로컬이 들어가면 안됨
				{
					if (m_nWearHead != -1 )
					{
						CItemData &tmpItemData = _pNetwork->GetItemData(m_nWearHead);
						if ((CTString)tmpItemData.GetItemSmcFileName() != MODEL_TREASURE)
						{
							wearId = m_nWearHead;
						}
					}
				}
			}
			break;

		case WEAR_JACKET:
			wearId = m_nWearArmorUp;
			break;
		case WEAR_WEAPON:
			wearId = m_nWearWeapon;
			break;
		case WEAR_PANTS:
			wearId = m_nWearArmorDown;
			break;
		case WEAR_SHIELD:
			wearId = m_nWearShield;
			break;
		case WEAR_GLOVES:
			wearId = m_nWearGlove;
			break;
		case WEAR_BOOTS:
			wearId = m_nWearBoots;
			break;
	}
	
	return wearId;
}

void SetWearing(int pos,int itemId)
{
	switch(pos)
	{

		case WEAR_HELMET:
			if(g_bHead_change) // 일본 헬멧 추가 관련 타로컬이 들어가면 안됨
			{
				m_nWearHead = itemId;
			}			
			break;

		case WEAR_JACKET:
			m_nWearArmorUp = itemId;
			break;
		case WEAR_WEAPON:
			m_nWearWeapon = itemId;
			break;
		case WEAR_PANTS:
			m_nWearArmorDown = itemId;
			break;
		case WEAR_SHIELD:
			m_nWearShield = itemId;
			break;
		case WEAR_GLOVES:
			m_nWearGlove = itemId;
			break;
		case WEAR_BOOTS:
			m_nWearBoots = itemId;
			break;
	}	
	return; 
}

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
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)

	void SetAttackSpeed(SBYTE attackspeed)
	{
		const int Speed = attackspeed;
		const int iJob	= m_ChaList;

		CModelInstance *pMI = GetModelInstance();
		float fLength		= pMI->GetAnimLength(m_AnimationIndices.m_iAnimation[ANIM_ATTACK_1]) * 10;
		if( m_nWearWeapon != -1 && m_bExtension )
		{
			fLength		= pMI->GetAnimLength(m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_1]) * 10;
		}		
		m_fAttackSpeedMul	= (float)fLength / Speed; //스피드가 0.9로 왔다면, 1.1 이 된다.
		m_fAttackLengthMul	= (float)Speed / fLength;
	}
			
///////////////////////

	// FIXME : Player.es와 중복되는 부분이 엄청 많음...ㅡ.ㅡ
	void TransformSorcerer(int iType, BOOL bAppear)
	{
		CTString strNewLook;			
		
		SetPhysicsFlags(GetPhysicsFlags() & ~(EPF_TRANSLATEDBYGRAVITY|EPF_ORIENTEDBYGRAVITY));			

		SetSkaModel( TransformInfo().GetFileName(iType) );
		for( INDEX t = ANIM_BASIC_BEGIN; t < ANIM_BASIC_END; ++t )
		{
			m_AnimationIndices.m_iAnimation[t]		= 		ska_GetIDFromStringTable(TransformInfo().GetAnimationName( iType, t ));
		}		
		
		AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_IDLE], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
		//GetModelInstance()->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_IDLE],AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);
		SetPhysicsFlags(GetPhysicsFlags() | EPF_TRANSLATEDBYGRAVITY|EPF_ORIENTEDBYGRAVITY);
		m_bIsTransform = TRUE;
		
		for(INDEX i=0; _pNetwork->ga_srvServer.srv_actCha.Count(); ++i)
		{
			CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[i];
			if(ct.cha_iClientIndex == en_ulID)
			{
				ct.cha_itemEffect.Refresh(NULL, ct.cha_sbItemEffectOption);
				break;
			}
		}
	}

	// FIXME : Player.es와 중복되는 부분이 엄청 많음.
	void ReturnSorcerer(BOOL bDie)
	{
		if(!m_bIsTransform)
		{
			return;
		}

		CTString strNewLook;	
		m_bIsTransform = FALSE;
		SetPhysicsFlags(GetPhysicsFlags() & ~(EPF_TRANSLATEDBYGRAVITY|EPF_ORIENTEDBYGRAVITY));
		
		SetSkaModel(JobInfo().GetFileName(m_ChaList));

		CModelInstance* pMI = GetModelInstance();
		if(pMI)
		{
			pMI->mi_iFaceMesh = -1;
			pMI->mi_iHairMesh = -1;
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetCharacterAppearance(pMI, m_ChaList, m_nHairStyle, m_nFaceStyle);
		}

		for( INDEX t = ANIM_BASIC_BEGIN; t < ANIM_BASIC_END; ++t )
		{
			m_AnimationIndices.m_iAnimation[t] = ska_GetIDFromStringTable(JobInfo().GetAnimationName( m_ChaList, t ));
		}
		
		// FIXME : 어디는 6이고, 어디는 7???
		for(int i=0;i<WEAR_TOTAL;i++)
		{
			int bWear = IsThereWearing(i);	
			if(bWear != -1)
			{
				_pGameState->DeleteDefaultArmor( pMI, i, m_ChaList );
				WearingArmor(bWear);
			}
		}

		if( pMI )
		{
			CSkaTag tag;
			tag.SetName("__ROOT");
			tag.SetOffsetRot(GetEulerAngleFromQuaternion(en_pmiModelInstance->mi_qvOffset.qRot));
			pMI->m_tmSkaTagManager.Register(&tag);
			tag.SetName("__TOP");
			tag.SetOffsetRot(GetEulerAngleFromQuaternion(en_pmiModelInstance->mi_qvOffset.qRot));
			FLOATaabbox3D aabb;
			pMI->GetAllFramesBBox(aabb);
			tag.SetOffsetPos(0, aabb.Size()(2) * pMI->mi_vStretch(2), 0);
			pMI->m_tmSkaTagManager.Register(&tag);
		}

		IdleAnim();		
		SetPhysicsFlags(GetPhysicsFlags() | EPF_TRANSLATEDBYGRAVITY|EPF_ORIENTEDBYGRAVITY);
	}

	// FIXME : Player.es와 중복되는 부분이 엄청 많음...ㅡ.ㅡ
	void MonsterChange(int iType, BOOL bAppear)
	{
		CTString strNewLook;			
		
		SetPhysicsFlags(GetPhysicsFlags() & ~(EPF_TRANSLATEDBYGRAVITY|EPF_ORIENTEDBYGRAVITY));			
		CMobData &MD = _pNetwork->GetMobData( iType );
	
		SetSkaModel( MD.GetMobSmcFileName() );
		m_AnimationIndices.m_iAnimation[ANIM_WALK_1]		= ska_GetIDFromStringTable(MD.GetAnimWalkName());
		m_AnimationIndices.m_iAnimation[ANIM_WALK_2]		= ska_GetIDFromStringTable(MD.GetAnimWalkName());
		m_AnimationIndices.m_iAnimation[ANIM_IDLE]			= ska_GetIDFromStringTable(MD.GetAnimDefaultName());
		m_AnimationIndices.m_iAnimation[ANIM_ATTACK_IDLE]	= ska_GetIDFromStringTable(MD.GetAnimDefaultName());
		m_AnimationIndices.m_iAnimation[ANIM_RUN_1]			= ska_GetIDFromStringTable(MD.GetAnimRunName());
		m_AnimationIndices.m_iAnimation[ANIM_RUN_2]			= ska_GetIDFromStringTable(MD.GetAnimRunName());
		m_AnimationIndices.m_iAnimation[ANIM_DIE]			= ska_GetIDFromStringTable(MD.GetAnimDeathName());
		m_AnimationIndices.m_iAnimation[ANIM_DAMAGE]		= ska_GetIDFromStringTable(MD.GetAnimWoundName());
		
		AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_IDLE], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
		//GetModelInstance()->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_IDLE],AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);
		SetPhysicsFlags(GetPhysicsFlags() | EPF_TRANSLATEDBYGRAVITY|EPF_ORIENTEDBYGRAVITY);
		m_bMobChange = TRUE;
		
		for(INDEX i=0; _pNetwork->ga_srvServer.srv_actCha.Count(); ++i)
		{
			CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[i];
			if(ct.cha_iClientIndex == en_ulID)
			{
				ct.cha_itemEffect.Refresh(NULL, ct.cha_sbItemEffectOption);
				break;
			}
		}

		// GM_GHANGE일 경우 크기 수정
		if(iType == GM_CHANGE)
		{
			GetModelInstance()->StretchModel(FLOAT3D( GM_CHANGE_MUL*10,GM_CHANGE_MUL*10,GM_CHANGE_MUL*10 ));
		}
	}

	// FIXME : Player.es와 중복되는 부분이 엄청 많음.
	void ReturnChange(BOOL bDie)
	{
		if(!m_bMobChange)
		{
			return;
		}

		ReturnSorcerer(bDie);

		CTString strNewLook;	

		m_bMobChange = FALSE;
		SetPhysicsFlags(GetPhysicsFlags() & ~(EPF_TRANSLATEDBYGRAVITY|EPF_ORIENTEDBYGRAVITY));
		
		SetSkaModel(JobInfo().GetFileName(m_ChaList));

		CModelInstance* pMI = GetModelInstance();
		if(pMI)
		{
			pMI->mi_iFaceMesh = -1;
			pMI->mi_iHairMesh = -1;
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetCharacterAppearance(pMI, m_ChaList, m_nHairStyle, m_nFaceStyle);
		}

		m_AnimationIndices.m_iAnimation[ANIM_WALK_1]		= ska_GetIDFromStringTable(JobInfo().GetAnimationName( m_ChaList, ANIM_WALK_1 ));
		m_AnimationIndices.m_iAnimation[ANIM_WALK_2]		= ska_GetIDFromStringTable(JobInfo().GetAnimationName( m_ChaList, ANIM_WALK_2 ));
		m_AnimationIndices.m_iAnimation[ANIM_IDLE]			= ska_GetIDFromStringTable(JobInfo().GetAnimationName( m_ChaList, ANIM_IDLE ));
		m_AnimationIndices.m_iAnimation[ANIM_ATTACK_IDLE]	= ska_GetIDFromStringTable(JobInfo().GetAnimationName( m_ChaList, ANIM_ATTACK_IDLE ));
		m_AnimationIndices.m_iAnimation[ANIM_RUN_1]			= ska_GetIDFromStringTable(JobInfo().GetAnimationName( m_ChaList, ANIM_RUN_1 ));
		m_AnimationIndices.m_iAnimation[ANIM_RUN_2]			= ska_GetIDFromStringTable(JobInfo().GetAnimationName( m_ChaList, ANIM_RUN_1 ));
		m_AnimationIndices.m_iAnimation[ANIM_DIE]			= ska_GetIDFromStringTable(JobInfo().GetAnimationName( m_ChaList, ANIM_DIE ));
		m_AnimationIndices.m_iAnimation[ANIM_DAMAGE]		= ska_GetIDFromStringTable(JobInfo().GetAnimationName( m_ChaList, ANIM_DAMAGE ));				
		
		//SetNormalBox("Default");

		//m_bExtension	= FALSE;

		// FIXME : 어디는 6이고, 어디는 7???
		for(int i=0;i<6;i++)
		{
			int bWear = IsThereWearing(i);	
			if(bWear != -1)
			{
				_pGameState->DeleteDefaultArmor( pMI, i, m_ChaList );
				WearingArmor(bWear);
			}
		}

		if( pMI )
		{
			CSkaTag tag;
			tag.SetName("__ROOT");
			tag.SetOffsetRot(GetEulerAngleFromQuaternion(en_pmiModelInstance->mi_qvOffset.qRot));
			pMI->m_tmSkaTagManager.Register(&tag);
			tag.SetName("__TOP");
			tag.SetOffsetRot(GetEulerAngleFromQuaternion(en_pmiModelInstance->mi_qvOffset.qRot));
			FLOATaabbox3D aabb;
			pMI->GetAllFramesBBox(aabb);
			tag.SetOffsetPos(0, aabb.Size()(2) * pMI->mi_vStretch(2), 0);
			pMI->m_tmSkaTagManager.Register(&tag);
		}

		IdleAnim();		
		SetPhysicsFlags(GetPhysicsFlags() | EPF_TRANSLATEDBYGRAVITY|EPF_ORIENTEDBYGRAVITY);
	}

	void MinMaximize()
	{
		static bMinimize = TRUE;

	//	if(m_bChanging)
		{
			if(bMinimize)//작아진다. 
			{
				static int cnt=0;//누적 카운트

				static unsigned int tmStartTime = unsigned int(_pTimer->GetLerpedCurrentTick()*1000);

				if(cnt==0)
				{  	
					tmStartTime = unsigned int(_pTimer->GetLerpedCurrentTick()*1000);		
					cnt++;
				}

				int time = ((unsigned int(_pTimer->GetLerpedCurrentTick()*1000) - tmStartTime )%1000)/100;
				if(time < 1)
				{
					return;
				}

				cnt = cnt+ time;

				if(cnt > 9)
				{
					cnt = 9;
				}		
				m_fScale = 0.1f*(10-cnt);
				GetModelInstance()->StretchModel(FLOAT3D( m_fScale,m_fScale,m_fScale ));

				if(cnt==9)
				{				
					if(!m_bMobChange)
					{
						MonsterChange(m_nChangeMonsterId,FALSE);//다 작아졌으니 변신한다.
					}
					else
					{
						m_nChangeMonsterId = -1;
						ReturnChange(FALSE);	//다 작아졌으니 본모습으로 돌아온다.
					}

					bMinimize = FALSE;	
					cnt = 0;			
				}			
			}
			else//이제 커져야한다.
			{

				static int cnt=10;//누적 카운트

				static unsigned int tmStartTime = unsigned int(_pTimer->GetLerpedCurrentTick()*1000);	
				
				if(cnt==10)
				{
					tmStartTime = unsigned int(_pTimer->GetLerpedCurrentTick()*1000);		
					cnt--;

					if(m_nChangeMonsterId == GM_CHANGE)
					{
						static INDEX animID = ska_StringToID("cusedrich_skill");
						static FLOAT animTime = GetModelInstance()->GetAnimLength(animID);
						AddAnimation(animID, AN_NORESTART, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
						StartEffectGroup("cusedrich_disguise", &GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
					}
					else
					{
						StartEffectGroup("Polymorph", &GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
						PlaySound(m_soMessage, SOUND_POLYMOPH, SOF_3D | SOF_VOLUMETRIC);	

					}
				}
		

				int time = ((unsigned int(_pTimer->GetLerpedCurrentTick()*1000) - tmStartTime )%1000)/100;
				if(time < 1)
				{
					return;
				}

				cnt = cnt - time;

				if(cnt < 1)
				{
					cnt = 1;
				}	
				
				if(m_nChangeMonsterId == GM_CHANGE)
				{
					GetModelInstance()->StretchModel(FLOAT3D( GM_CHANGE_MUL*(11-cnt),GM_CHANGE_MUL*(11-cnt),GM_CHANGE_MUL*(11-cnt) ));
				}
				else 
				{
					GetModelInstance()->StretchModel(FLOAT3D( 0.1f*(11-cnt),0.1f*(11-cnt),0.1f*(11-cnt) ));
				}

				if(cnt==1)
				{
				
					SetSkaColisionInfo(); //마지막에 충돌박스를 다시 셋팅해줘야 현재의 크기로 셋팅된다.
					m_bChanging = FALSE;	//변신이 완전 끝났다.	
					bMinimize = TRUE;	
					cnt = 10;						

					//개인상점중에 변신이 끝났다면 다시 앉아야 한다.
					if(m_nPlayActionNum == ACTION_NUM_SELL)
					{
						m_nSitDown = 2;//2가 앉기.
						m_nPlayActionNum = 3;	
						ActionNow();		
					}
					for(int i=0; _pNetwork->ga_srvServer.srv_actCha.Count(); ++i)
					{
						CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[i];
						if(ct.cha_iClientIndex == en_ulID)
						{
							ct.cha_itemEffect.Refresh(&GetCurrentPlayerModelInstance()->m_tmSkaTagManager, ct.cha_sbItemEffectOption);
							break;
						}
					}
				}
			}		
		}
	}

	// 강신 관련 처리...
	void Transforming()
	{
		if(!m_bIsTransform)
		{
			TransformSorcerer(m_iTransformType, FALSE);
		}
		else
		{
			m_iTransformType = 0;
			ReturnSorcerer(FALSE);
		}

		m_bTransforming = FALSE;	
	}

	void SetPolymophMobIndex(int mobId)
	{
		m_nChangeMonsterId = mobId;
	}


	BOOL HardCodingSkill(BOOL bOnlyTest)
	{
		if( m_nCurrentSkillNum != 128	//로그,다이어 스트라이크
		&& m_nCurrentSkillNum != 155	//타이탄, 더블 무브먼트.
		&& m_nCurrentSkillNum != 191	//로그, 데스 모션
		)
		{
			return FALSE;
		}
		if(bOnlyTest) {return TRUE;}

		CSkill &skill = _pNetwork->GetSkillData(m_nCurrentSkillNum);
		CEntityPointer penTarget = m_penEnemy;
		//if(penTarget == NULL) {return TRUE;}
		if(m_afHardCodeValue.Count() == 0) {m_afHardCodeValue.New(32);}


        m_bHardcodingSkillLoop = TRUE;

		switch(m_nCurrentSkillNum)
		{
		case 128://로그, 다이어 스트라이크, 현재 단검만 처리됨
			{
				FLOAT fTotalSkillTime	= 2.5f * m_fSkillSpeed;
				FLOAT fFirstAttackTime	= 1.7f * m_fSkillSpeed;
				FLOAT fAttackTime		= 0.7f * m_fSkillSpeed;
				static INDEX iFireAnimID		= ska_StringToID(skill.GetFireAnim(m_bExtension));
				static FLOAT fAttackTimeOnce	= GetModelInstance()->GetAnimLength(iFireAnimID);
				static FLOAT fDistFromCB		= 1.0f;
				static FLOAT fDamageTime		= 999.0f;
				if(penTarget == NULL) {break;}
				
				if(m_iSkillEffectStep == 0)
				{					
					m_soMessage.Set3DParameters(25.0f, 5.0f, 1.0f, 1.0f);
					++m_iSkillEffectStep;
					m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
					m_tmDireStrikeStart = _pTimer->GetLerpedCurrentTick();
					//원래 위치와 방향 저장
					CPlacement3D pl = GetPlacement();
					m_afHardCodeValue[11] = pl.pl_OrientationAngle(1);
					m_afHardCodeValue[12] = pl.pl_OrientationAngle(2);
					m_afHardCodeValue[13] = pl.pl_OrientationAngle(3);
					m_afHardCodeValue[14] = pl.pl_PositionVector(1);
					m_afHardCodeValue[15] = pl.pl_PositionVector(2);
					m_afHardCodeValue[16] = pl.pl_PositionVector(3);
					for(INDEX i=0; i<_pNetwork->ga_srvServer.srv_actCha.Count(); ++i)
					{
						if(en_ulID == _pNetwork->ga_srvServer.srv_actCha[i].cha_iClientIndex) {break;}
					}
					CCharacterTarget &ct = _pNetwork->ga_srvServer.srv_actCha[i];
					ct.cha_itemEffect.Refresh(NULL, 1);
					ct.cha_statusEffect.Refresh(NULL, CStatusEffect::R_NONE);
					SetFlagOn(ENF_HIDDEN);
					FLOAT3D vDir = penTarget->GetPlacement().pl_PositionVector - GetPlacement().pl_PositionVector;
					m_afHardCodeValue[9] = vDir.Length() / fFirstAttackTime;
					m_afHardCodeValue[1] = _pTimer->GetLerpedCurrentTick();
					return TRUE;
				}
				//first
				if(m_iSkillEffectStep == 1)
				{
					if((_pTimer->GetLerpedCurrentTick() - m_tmSkillStartTime) < fFirstAttackTime)
					{
						FLOAT delta = _pTimer->GetLerpedCurrentTick() - m_afHardCodeValue[1];
						m_afHardCodeValue[1] = _pTimer->GetLerpedCurrentTick();
						FLOAT3D vDir = penTarget->GetPlacement().pl_PositionVector
										- GetPlacement().pl_PositionVector;
						vDir.SafeNormalize();
						CPlacement3D plNew = GetPlacement();
						plNew.pl_PositionVector += vDir * m_afHardCodeValue[9] * delta;
						SetPlacement(plNew);
						return TRUE;
					}
					++m_iSkillEffectStep;
					//적의 xz평면상의 크기 구하기
					FLOATaabbox3D aabb;
					penTarget->GetModelInstance()->GetCurrentColisionBox(aabb);
					FLOAT3D size = aabb.Size();
					m_afHardCodeValue[0] = sqrtf(size(1) * size(1) + size(3) * size(3)) * 0.5f;//적의크기반지름
					//캐릭터 투명화 끄기
					SetFlagOff(ENF_HIDDEN);
					for(INDEX i=0; i<_pNetwork->ga_srvServer.srv_actCha.Count(); ++i)
					{
						if(en_ulID == _pNetwork->ga_srvServer.srv_actCha[i].cha_iClientIndex) {break;}
					}
					CCharacterTarget &ct = _pNetwork->ga_srvServer.srv_actCha[i];
					CStatusEffect::eRelation rel = CStatusEffect::R_NONE;
					
					// 051203 아랫줄 주석 처리.  플래그로 처리.
					//if(_pUIMgr->GetParty()->IsPartyMember(ct.cha_Index)) {rel = CStatusEffect::R_PARTY;}
					if( ct.cha_pEntity->IsSecondExtraFlagOn( ENF_EX2_MYPARTY ) ) {rel = CStatusEffect::R_PARTY;}
					ct.cha_itemEffect.Refresh(&GetModelInstance()->m_tmSkaTagManager, 1);
					ct.cha_statusEffect.Refresh(&GetModelInstance()->m_tmSkaTagManager, rel);
					//플레이어의 위치 옮기기
					CPlacement3D plNew;
					FLOAT fRadius = m_afHardCodeValue[0] + fDistFromCB;
					FLOAT3D deltaPos(fRadius*(0.5f*1.7320f), 0, fRadius*0.5f);
					plNew.pl_PositionVector = penTarget->GetPlacement().pl_PositionVector + deltaPos;
					plNew.pl_OrientationAngle = GetEulerAngleFromDir((-deltaPos).SafeNormalize());
					SetPlacement(plNew);
					//애니메이션 실시
					NewClearState(0);
					AddAnimation(iFireAnimID, AN_NORESTART, 1, 0);

					if(!m_bExtension)
					{
						PlaySound(m_soMessage, SOUND_DAIR_STRIKE_BLOW1, SOF_3D | SOF_VOLUMETRIC);
					}
					else
					{					
						PlaySound(m_soMessage, SOUND_A_DAIR_STRIKE_BLOW1, SOF_3D | SOF_VOLUMETRIC);
					}			

					StartEffectGroup(skill.GetFireEffect1(m_bExtension), &GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());

					m_afHardCodeValue[1] = _pTimer->GetLerpedCurrentTick();//시간
					m_afHardCodeValue[2] = 2 * (m_afHardCodeValue[0] + fDistFromCB) / fAttackTimeOnce;//속도
					fDamageTime = skill.GetDelay(0, m_bExtension);
					return TRUE;
				}
				if(m_iSkillEffectStep == 2)
				{
					//플레이어의 위치 옮기기
					FLOAT delta = _pTimer->GetLerpedCurrentTick() - m_afHardCodeValue[1];
					m_afHardCodeValue[1] = _pTimer->GetLerpedCurrentTick();
					FLOAT deltaDist = m_afHardCodeValue[2] * delta;
					CPlacement3D plNew;
					FLOAT3D deltaDir = (penTarget->GetPlacement().pl_PositionVector - GetPlacement().pl_PositionVector).SafeNormalize();
					plNew.pl_PositionVector = GetPlacement().pl_PositionVector + deltaDir * deltaDist;
					plNew.pl_OrientationAngle = GetEulerAngleFromDir(deltaDir);
					SetPlacement(plNew);

					if(_pTimer->GetLerpedCurrentTick() - m_tmSkillStartTime > fFirstAttackTime + fDamageTime)
					{
						FLOAT3D vHitPoint = penTarget->GetPlacement().pl_PositionVector + deltaDir * (-m_afHardCodeValue[0]);
						//InflictDirectDamage(penTarget, this, DMT_CLOSERANGE, 1, vHitPoint, deltaDir);
						StartEffectGroup("Normal Hit", _pTimer->GetLerpedCurrentTick(), vHitPoint, deltaDir);
						fDamageTime = 999.0f;
					}
					if(_pTimer->GetLerpedCurrentTick() - m_tmSkillStartTime > fFirstAttackTime + fAttackTimeOnce)
					{
						fDamageTime = skill.GetDelay(0, m_bExtension);
						++m_iSkillEffectStep;
					}
					return TRUE;
				}
				//second
				if(m_iSkillEffectStep == 3)
				{
					++m_iSkillEffectStep;
					//플레이어의 위치 옮기기
					CPlacement3D plNew;
					FLOAT fRadius = m_afHardCodeValue[0] + fDistFromCB;
					FLOAT3D deltaPos(-fRadius*(0.5f*1.7320f), 0, fRadius*0.5f);
					plNew.pl_PositionVector = penTarget->GetPlacement().pl_PositionVector + deltaPos;
					plNew.pl_OrientationAngle = GetEulerAngleFromDir((-deltaPos).SafeNormalize());
					SetPlacement(plNew);
					//애니메이션 실시
					NewClearState(0);
					AddAnimation(iFireAnimID, AN_NORESTART, 1, 0);

					if(!m_bExtension)
					{
						PlaySound(m_soMessage, SOUND_DAIR_STRIKE_BLOW2, SOF_3D | SOF_VOLUMETRIC);
					}
					else
					{					
						PlaySound(m_soMessage, SOUND_A_DAIR_STRIKE_BLOW2, SOF_3D | SOF_VOLUMETRIC);
					}		

					StartEffectGroup(skill.GetFireEffect1(m_bExtension), &GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());

					m_afHardCodeValue[1] = _pTimer->GetLerpedCurrentTick();//시간
					return TRUE;
				}
				if(m_iSkillEffectStep == 4)
				{
					//플레이어의 위치 옮기기
					FLOAT delta = _pTimer->GetLerpedCurrentTick() - m_afHardCodeValue[1];
					m_afHardCodeValue[1] = _pTimer->GetLerpedCurrentTick();
					FLOAT deltaDist = m_afHardCodeValue[2] * delta;
					CPlacement3D plNew;
					FLOAT3D deltaDir = (penTarget->GetPlacement().pl_PositionVector - GetPlacement().pl_PositionVector).SafeNormalize();
					plNew.pl_PositionVector = GetPlacement().pl_PositionVector + deltaDir * deltaDist;
					plNew.pl_OrientationAngle = GetEulerAngleFromDir(deltaDir);
					SetPlacement(plNew);

					if(_pTimer->GetLerpedCurrentTick() - m_tmSkillStartTime > fFirstAttackTime + fDamageTime)
					{
						FLOAT3D vHitPoint = penTarget->GetPlacement().pl_PositionVector + deltaDir * (-m_afHardCodeValue[0]);
						//InflictDirectDamage(penTarget, this, DMT_CLOSERANGE, 1, vHitPoint, deltaDir);
						StartEffectGroup("Normal Hit", _pTimer->GetLerpedCurrentTick(), vHitPoint, deltaDir);
						fDamageTime = 999.0f;
					}
					if(_pTimer->GetLerpedCurrentTick() - m_tmSkillStartTime > fFirstAttackTime + 2*fAttackTimeOnce)
					{
						fDamageTime = skill.GetDelay(0, m_bExtension);
						++m_iSkillEffectStep;
					}
					return TRUE;
				}
				//third
				if(m_iSkillEffectStep == 5)
				{
					++m_iSkillEffectStep;
					//플레이어의 위치 옮기기
					CPlacement3D plNew;
					FLOAT fRadius = m_afHardCodeValue[0] + fDistFromCB;
					FLOAT3D deltaPos(0, 0, -fRadius*0.5f);
					plNew.pl_PositionVector = penTarget->GetPlacement().pl_PositionVector + deltaPos;
					plNew.pl_OrientationAngle = GetEulerAngleFromDir((-deltaPos).SafeNormalize());
					SetPlacement(plNew);
					//애니메이션 실시
					NewClearState(0);
					AddAnimation(iFireAnimID, AN_NORESTART, 1, 0);

					if(!m_bExtension)
					{
						PlaySound(m_soMessage, SOUND_DAIR_STRIKE_BLOW3, SOF_3D | SOF_VOLUMETRIC);
					}
					else
					{					
						PlaySound(m_soMessage, SOUND_A_DAIR_STRIKE_BLOW3, SOF_3D | SOF_VOLUMETRIC);
					}		

					StartEffectGroup(skill.GetFireEffect1(m_bExtension), &GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());

					m_afHardCodeValue[1] = _pTimer->GetLerpedCurrentTick();//시간
					return TRUE;
				}
				if(m_iSkillEffectStep == 6)
				{
					//플레이어의 위치 옮기기
					FLOAT delta = _pTimer->GetLerpedCurrentTick() - m_afHardCodeValue[1];
					m_afHardCodeValue[1] = _pTimer->GetLerpedCurrentTick();
					FLOAT deltaDist = m_afHardCodeValue[2] * delta;
					CPlacement3D plNew;
					FLOAT3D deltaDir = (penTarget->GetPlacement().pl_PositionVector - GetPlacement().pl_PositionVector).SafeNormalize();
					plNew.pl_PositionVector = GetPlacement().pl_PositionVector + deltaDir * deltaDist;
					plNew.pl_OrientationAngle = GetEulerAngleFromDir(deltaDir);
					SetPlacement(plNew);

					if(_pTimer->GetLerpedCurrentTick() - m_tmSkillStartTime > fFirstAttackTime + fDamageTime)
					{
						FLOAT3D vHitPoint = penTarget->GetPlacement().pl_PositionVector + deltaDir * (-m_afHardCodeValue[0]);
						//InflictDirectDamage(penTarget, this, DMT_CLOSERANGE, 1, vHitPoint, deltaDir);
						StartEffectGroup("Normal Hit", _pTimer->GetLerpedCurrentTick(), vHitPoint, deltaDir);
						fDamageTime = 999.0f;					
					}
					if(_pTimer->GetLerpedCurrentTick() - m_tmSkillStartTime > fFirstAttackTime + 3*fAttackTimeOnce)
					{
						fDamageTime = skill.GetDelay(0, m_bExtension);
						++m_iSkillEffectStep;
					}
					return TRUE;
				}
				//finalize
				if(m_iSkillEffectStep == 7)
				{
					InflictDirectDamage(penTarget, this, DMT_NONE, 1, FLOAT3D(0,0,0), FLOAT3D(0,0,0));
					CPlacement3D plNew;
					plNew.pl_OrientationAngle(1) = m_afHardCodeValue[11];
					plNew.pl_OrientationAngle(2) = m_afHardCodeValue[12];
					plNew.pl_OrientationAngle(3) = m_afHardCodeValue[13];
					plNew.pl_PositionVector(1) = m_afHardCodeValue[14];
					plNew.pl_PositionVector(2) = m_afHardCodeValue[15];
					plNew.pl_PositionVector(3) = m_afHardCodeValue[16];
					SetPlacement(plNew);
					StartEffectGroup(skill.GetFireEffect3(m_bExtension)
						, &penTarget->GetModelInstance()->m_tmSkaTagManager
						, _pTimer->GetLerpedCurrentTick());
					fDamageTime = 999.0f;
				}
			} break;
		case 155://타이탄 더블 무브먼트.
			{
				StartEffectGroup(skill.GetFireEffect1(m_bExtension),
								&GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
			} break;
		case 191://로그, 데스 모션
			{
				static INDEX animID = ska_StringToID(skill.GetFireAnim(m_bExtension));
				static FLOAT animTime = GetModelInstance()->GetAnimLength(animID);
				if(m_iSkillEffectStep == 0)
				{
					++m_iSkillEffectStep;
					m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
					AddAnimation(animID, AN_NORESTART|AN_CLEAR, 1.0f, 0);
					m_bIdleAnim = FALSE;
					return TRUE;
				}
				if(m_iSkillEffectStep == 1)
				{
					if(_pTimer->GetLerpedCurrentTick() - m_tmSkillStartTime <= animTime)
					{
						return TRUE;
					}
					
					m_bIdleAnim = FALSE;
					++m_iSkillEffectStep;
				}
			} break;
		default: {return FALSE;}
		}

		m_iSkillEffectStep = 0;

		m_idCurrentSkillAnim = -1;//스킬 종료.
		m_tmSkillStartTime = 0.0f;
		m_bSkilling = FALSE;
		m_nCurrentSkillNum = -1;
		m_fSkillAnimTime = 0.0f;

		m_bHardcodingSkillLoop = FALSE;
		if(m_bIdleAnim)
		{
			NewClearState(0.2f); 
			IdleAnim();			
		}
		
		SetNoTargetEntity();

		SetFlagOff(ENF_HIDDEN);
		StopRotating();
		
		return TRUE;
	}

/////////////////////////

procedures:
/************************************************************
 *                A T T A C K   E N E M Y                   *
 ************************************************************/
	Fire(EVoid) : CCharacterBase::Fire
	{
		jump CCharacterBase::Fire();
	}

	Hit(EVoid) : CCharacterBase::Hit 
	{		
		autocall NormalAttack() EEnd;
		return EReturn();
	};
		//0628 kwon
	AttackTarget(EVoid) : CCharacterBase::AttackTarget // 이 프로시저를 call하기전에 SetTargetEntity()가 선행되어야 한다.
	{	
		m_vDesiredPosition = PlayerDestinationPos();
		
		autocall AttackAnimation() EReturn;

		return EReturn();
	};

	AttackAnimation(EVoid) //0628
	{
		//INDEX idBodyAnim;
		float frandom = FRnd();

		if( m_nWearWeapon != -1 )
		{
		}		

		// NOTE : 무기에 따라서 애니메이션도 달라야 한다.
		// FIXME : 확율로 하는 듯한데... 캐릭터에 따라서 애니메이션 갯수가 다르면 어떻게 하지???
		if( !m_bExtension )
		{
			if( frandom < 0.25f )
			{
				idBodyAnim = m_AnimationIndices.m_iAnimation[ANIM_ATTACK_1];
			}
			else if(frandom < 0.5f )
			{
				idBodyAnim = m_AnimationIndices.m_iAnimation[ANIM_ATTACK_2];
			}
			else if(frandom < 0.75f )
			{
				idBodyAnim = m_AnimationIndices.m_iAnimation[ANIM_ATTACK_3];
			}
			else
			{
				idBodyAnim = m_AnimationIndices.m_iAnimation[ANIM_ATTACK_4];
			}
		}
		// 전직 및 로그 관련....
		else
		{
			if( frandom < 0.25f )
			{
				idBodyAnim = m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_1];
			}
			else if(frandom < 0.5f )
			{
				idBodyAnim = m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_2];				
			}
			else if(frandom < 0.75f )
			{
				idBodyAnim = m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_3];
			}
			else
			{
				idBodyAnim = m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_4];
			}
		}

		if(m_bDisappearWeapon)//무기가 지워져 있다면, 나타나게 해줘야 한다.
		{
			AppearWearingWeapon();
			m_bDisappearWeapon = FALSE;
		}
//		GetModelInstance()->AddAnimation(idBodyAnim,AN_CLEAR,1.0f,0,1.2f);//1009
		AddAnimation(idBodyAnim, AN_CLEAR, 1.0f, 0x03,
						ESKA_MASTER_MODEL_INSTANCE, m_fAttackSpeedMul);

		m_fAttackFrequency = 0.25f;
		m_fAttackStartTime = _pTimer->CurrentTick();
		
		// FIXME : 왜 여기서는 기본 애니 1번의 애니메이션 시간을 갖다가 쓰지? 다른 애니메이션은???
		m_fImpactTime = GetAnimLength(m_AnimationIndices.m_iAnimation[ANIM_ATTACK_1])*0.8f/2 * m_fAttackLengthMul;//0630 우선 공격 애니메이션의 절반에서 임팩트.

//안태훈 수정 시작	//(Closed Beta Effect)(0.1)
		m_nEffectStep = 0;
		m_saLastEffectInfo.Clear();
//안태훈 수정 끝	//(Closed Beta Effect)(0.1)

		while(_pTimer->CurrentTick() - m_fAttackStartTime < GetAnimLength(m_AnimationIndices.m_iAnimation[ANIM_ATTACK_1])*0.8f*m_fAttackLengthMul)
		{//1009
			wait(m_fAttackFrequency) 
			{
				on (EBegin) : 
				{ 
					
					if(m_bMoving)
					{
						return EReturn();
					}
					
					m_fMoveSpeed = 0.0f; 
					ULONG ulFlags = SetDesiredMovement(); 	
					
//안태훈 수정 시작	//(Closed Beta Effect)(0.1)
					if(m_nEffectStep == 0)
					{
						++m_nEffectStep;

						const int iJob = m_ChaList;
						if(iJob == KNIGHT)
						{							
							if( !m_bExtension )
							{
								if(	idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_1] || 
									idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_3] )
								{								
									StartEffectGroup("Knight Attack 1"		//Hardcording
										, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
								}
								else if( idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_2] || 
										idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_4])
								{
	
									StartEffectGroup("Knight Attack 2"		//Hardcording
										, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
								}
							}
							// 로그 및 전직 관련...
							else
							{
								if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_1] || idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_3])
								{								
									StartEffectGroup("Knight Attack 1_a"		//Hardcording
										, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
								}
								else if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_2] || idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_4])
								{
	
									StartEffectGroup("Knight Attack 2_a"		//Hardcording
										, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
								}
							}
						}
						else if(iJob == TITAN)
						{
							if( !m_bExtension )
							{
								if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_1] || idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_3])
								{
									StartEffectGroup("Titan Attack 1"		//Hardcording
										, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
								}
								else if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_2] || idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_4])
								{
									StartEffectGroup("Titan Attack 2"		//Hardcording
										, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
								}
							}
							else
							{
								if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_1] || idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_3])
								{
									StartEffectGroup("Titan Attack 1_a"		//Hardcording
										, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
								}
								else if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_2] || idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_4])
								{
									StartEffectGroup("Titan Attack 2_a"		//Hardcording
										, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
								}
							}
						}
						else if( iJob == MAGE )
						{
							if( !m_bExtension )
							{
								if( idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_1] )
								{
									StartEffectGroup( "mEnerBall0", &en_pmiModelInstance->m_tmSkaTagManager,
														_pTimer->GetLerpedCurrentTick() );
									//m_fImpactTime = 0.38f * m_fAttackLengthMul;
								}
								else if( idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_2] )
								{
									StartEffectGroup( "mEnerBall1", &en_pmiModelInstance->m_tmSkaTagManager,
														_pTimer->GetLerpedCurrentTick() );
									//m_fImpactTime = 0.46f * m_fAttackLengthMul;
								}
							}
							else
							{
								if( idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_1] )
								{
									StartEffectGroup( "mEnerBall0_a", &en_pmiModelInstance->m_tmSkaTagManager,
														_pTimer->GetLerpedCurrentTick() );
									//m_fImpactTime = 0.38f * m_fAttackLengthMul;
								}
								else if( idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_2] )
								{
									StartEffectGroup( "mEnerBall1_a", &en_pmiModelInstance->m_tmSkaTagManager,
														_pTimer->GetLerpedCurrentTick() );
									//m_fImpactTime = 0.46f * m_fAttackLengthMul;
								}
							}
						}
						else if( iJob == ROGUE )
						{
							if( !m_bExtension )
							{
								if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_1])
								{
									StartEffectGroup("Rogue Attack 1"		//Hardcording
										, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
								}
								else if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_2])
								{
									StartEffectGroup("Rogue Attack 2"		//Hardcording
										, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
								}
							}
						}
						else if(iJob == SORCERER)
						{
							if( !m_bExtension )
							{
								if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_1] || idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_3])
								{
									StartEffectGroup("Titan Attack 1"		//Hardcording
										, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
								}
								else if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_2] || idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_4])
								{
									StartEffectGroup("Titan Attack 2"		//Hardcording
										, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
								}
							}
							else
							{
								if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_1] || idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_3])
								{
									StartEffectGroup("Titan Attack 1_a"		//Hardcording
										, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
								}
								else if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_2] || idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_4])
								{
									StartEffectGroup("Titan Attack 2_a"		//Hardcording
										, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
								}
							}
						}

						// FIXME : 공격관련 ID를 배열로 만들순 없나???
						if( iJob >= TITAN && iJob <= TOTAL_JOB )
						{
							// 강신체 일때...
							if( m_bIsTransform  )
							{
								if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_1] || 
									idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_1])
								{
									m_fImpactTime = TransformInfo().GetImpactTime( m_iTransformType, ATTACK1 ) * m_fAttackLengthMul;
								}
								else if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_2] ||
									idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_2])
								{
									m_fImpactTime = TransformInfo().GetImpactTime( m_iTransformType, ATTACK2 ) * m_fAttackLengthMul;
								}
								else if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_3] ||
									idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_3])
								{
									m_fImpactTime = TransformInfo().GetImpactTime( m_iTransformType, ATTACK3 ) * m_fAttackLengthMul;
								}
								else if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_4] ||
									idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_4])
								{
									m_fImpactTime = TransformInfo().GetImpactTime( m_iTransformType, ATTACK4 ) * m_fAttackLengthMul;
								}
							}							
							else
							{
								if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_1])
								{
									m_fImpactTime = JobInfo().GetImpactTime( iJob, ATTACK1 ) * m_fAttackLengthMul;
								}
								else if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_2])
								{
									m_fImpactTime = JobInfo().GetImpactTime( iJob, ATTACK2 ) * m_fAttackLengthMul;
								}
								else if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_3])
								{
									m_fImpactTime = JobInfo().GetImpactTime( iJob, ATTACK3 ) * m_fAttackLengthMul;
								}
								else if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_4])
								{
									m_fImpactTime = JobInfo().GetImpactTime( iJob, ATTACK4 ) * m_fAttackLengthMul;
								}
								//---------- 로그 및 전직 ----------
								else if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_1])
								{
									m_fImpactTime = JobInfo().GetImpactTime( iJob, EXT_ATTACK1 ) * m_fAttackLengthMul;
								}
								else if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_2])
								{
									m_fImpactTime = JobInfo().GetImpactTime( iJob, EXT_ATTACK2 ) * m_fAttackLengthMul;
								}
								else if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_3])
								{
									m_fImpactTime = JobInfo().GetImpactTime( iJob, EXT_ATTACK3 ) * m_fAttackLengthMul;
								}
								else if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_4])
								{
									m_fImpactTime = JobInfo().GetImpactTime( iJob, EXT_ATTACK4 ) * m_fAttackLengthMul;
								}
							}
						}
					}
//안태훈 수정 끝	//(Closed Beta Effect)(0.1)

					// 원거리로 공격하는 경우에 대한 처리.
					if(_pTimer->CurrentTick() - m_fAttackStartTime > m_fImpactTime-0.1f) 
					{
						if( m_penEnemy.ep_pen != NULL && ( m_penEnemy->GetFlags() & ENF_ALIVE ) )
						{
							if(m_ChaList == HEALER)
						{
							if( m_bExtension )
							{
								ShotMissile(this, "STAFF", m_penEnemy.ep_pen, 40.0f
											, "Normal Hit", "Normal Arrow Trace", false
											, 0.0f, 0.0f, 1);	//Hardcoding
							}
							else
							{
								ShotMissile(this, "RHAND", m_penEnemy.ep_pen, 40.0f
											, "Normal Hit", "Normal Arrow Trace", false);	//Hardcoding
							}
						}
							else if( m_ChaList == MAGE)
						{
							FLOAT3D		vStartPos;
							FLOATquat3D	qStartRot;
							CTag		*pTag = NULL;

							if( idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_1] )
							{
								pTag = this->en_pmiModelInstance->m_tmSkaTagManager.Find( "WAND" );
								if( pTag == NULL )
								{
									pTag = this->en_pmiModelInstance->m_tmSkaTagManager.Find( "RHAND" );
								}
								if( pTag != NULL )
								{
									vStartPos = pTag->CurrentTagInfo().m_vPos;
									qStartRot = pTag->CurrentTagInfo().m_qRot;
								}
							}
							else if( idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_2] )
							{
								pTag = this->en_pmiModelInstance->m_tmSkaTagManager.Find( "LHAND" );
								if( pTag != NULL )
								{
									vStartPos = pTag->CurrentTagInfo().m_vPos;
									qStartRot = pTag->CurrentTagInfo().m_qRot;
								}
							}
							else if( idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_1] )
							{
								pTag = this->en_pmiModelInstance->m_tmSkaTagManager.Find( "RHAND" );
								if( pTag != NULL )
								{
									vStartPos = pTag->CurrentTagInfo().m_vPos;
									qStartRot = pTag->CurrentTagInfo().m_qRot;
								}
							}
							else if( idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_2] )
							{
								pTag = this->en_pmiModelInstance->m_tmSkaTagManager.Find( "RHAND" );
								if( pTag != NULL )
								{
									vStartPos = pTag->CurrentTagInfo().m_vPos;
									qStartRot = pTag->CurrentTagInfo().m_qRot;
								}
							}

							if( pTag != NULL )
							{
								ShotMagicContinued( this, vStartPos, qStartRot, m_penEnemy.ep_pen, 35.0F,
													"mHitEnerBall", "mMissileEnerBall",
													false, 0 );
							}
						}
							else if( m_ChaList == SORCERER)
						{
							if( m_bIsTransform && m_iTransformType == CTransformInfo::TYPE_1 )
							{
								FLOAT3D		vStartPos;
								FLOATquat3D	qStartRot;
								CTag		*pTag = NULL;

								pTag = this->en_pmiModelInstance->m_tmSkaTagManager.Find( "LHAND" );
								if( pTag != NULL )
								{
									vStartPos = pTag->CurrentTagInfo().m_vPos;
									qStartRot = pTag->CurrentTagInfo().m_qRot;
								}

								if( pTag != NULL )
								{
									ShotMagicContinued( this, vStartPos, qStartRot, m_penEnemy.ep_pen, 35.0F,
														"mHitEnerBall", "mMissileEnerBall",
														false, 0 );
								}
							}
							else
							{
								SendEvent(EChaAttackDamage());
							}
						}
						// 근거리로 공격하는 경우에 대한 처리.
						else
						{
							SendEvent(EChaAttackDamage());
						}
						m_fImpactTime = 1000.0f;//충분히 긴시간.
					}
					}

					resume; 
				}
				on (EChaAttackDamage eAttackDamage) : 
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
					if(m_penEnemy != NULL)//0817 버그 수정.
					{
						this->InflictDirectDamage(m_penEnemy, this , DMT_CLOSERANGE, (int)1, vTargetPos, vDirection);
					}					
					m_fImpactTime = 1000.0f;//충분히 긴시간.

					// NOTE : 이 부분이 왜 있는 건지 모르겠음..-_-;;;
					if(m_bKillEnemy)
					{
						if(m_penKillEnemy!=NULL)
						{
							if(m_penKillEnemy->IsEnemy())
							{
								if(((CUnit*)((CEntity*)m_penKillEnemy))->m_nCurrentHealth <= 0)
								{
									CMobData& MD = _pNetwork->GetMobData(((CEnemy*)(CEntity*)m_penKillEnemy)->m_nMobDBIndex);
									if(!MD.IsCastleTower())
									{
										((CUnit*)((CEntity*)m_penKillEnemy))->DeathNow();
									}
									else
									{
										m_penKillEnemy->SetFlagOff(ENF_ALIVE);
									}
								}
							}
							else
							{
								if(((CUnit*)((CEntity*)m_penKillEnemy))->m_nCurrentHealth <= 0)
								{
									((CUnit*)((CEntity*)m_penKillEnemy))->DeathNow();
								}
							}
						}
						m_penKillEnemy = NULL;
						m_bKillEnemy = FALSE;
					}

					stop;
				}
				on (ETimer) : { stop; }			
			}
		}

		// NOTE : 이 부분이 왜 있는 건지 모르겠음..-_-;;;
		if(m_bKillEnemy)
		{
			if(m_penKillEnemy!=NULL)
			{
				if(m_penKillEnemy->IsEnemy())
				{
					if(((CUnit*)((CEntity*)m_penKillEnemy))->m_nCurrentHealth <= 0)
					{
						CMobData& MD = _pNetwork->GetMobData(((CEnemy*)(CEntity*)m_penKillEnemy)->m_nMobDBIndex);
						if(!MD.IsCastleTower())
						{
							((CUnit*)((CEntity*)m_penKillEnemy))->DeathNow();
						}
						else
						{
							m_penKillEnemy->SetFlagOff(ENF_ALIVE);
						}
					}
				}
				else
				{
					if(((CUnit*)((CEntity*)m_penKillEnemy))->m_nCurrentHealth <= 0)
					{
						((CUnit*)((CEntity*)m_penKillEnemy))->DeathNow();
					}
				}
			}
			m_penKillEnemy = NULL;
			m_bKillEnemy = FALSE;
		}

		m_bAttack = FALSE;
		m_bMoving = FALSE;
		m_bStop = FALSE;		
		StandingAnim();
		SetNoTargetEntity();	

		return EReturn();
	};

	SkillingTarget(EVoid) : CCharacterBase::SkillingTarget // 이 프로시저를 call하기전에 SetTargetEntity()가 선행되어야 한다.
	{
		m_vDesiredPosition = PlayerDestinationPos();
	
		autocall SkillAnimation() EReturn;
	
		return EReturn();
	};
	
	SpellSkill(EVoid) : CCharacterBase::SpellSkill
	{
		m_vDesiredPosition = PlayerDestinationPos();
	
		autocall PreSkillAnimation() EReturn;
	
		return EReturn();
	};

	ProduceAnimation(EVoid): CCharacterBase::ProduceAnimation //1221
	{
		CModelInstance *pMI = GetModelInstance();
		if(m_bDisappearWeapon)//무기가 지워져 있다면, 나타나게 해줘야 한다.
		{
			AppearWearingWeapon();
			m_bDisappearWeapon = FALSE;
		}
		if(m_nPlayProduceNum == 1)//채광
		{
			AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_MINE], AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
			//pMI->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_MINE],AN_CLEAR,1.0f,0);
			m_fActionAnimTime = pMI->GetAnimLength(m_AnimationIndices.m_iAnimation[ANIM_MINE]);
		}
		else if(m_nPlayProduceNum == 2)//채집
		{
			//pMI->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_COLLECT],AN_CLEAR,1.0f,0);	
			AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_COLLECT], AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
			m_fActionAnimTime = pMI->GetAnimLength(m_AnimationIndices.m_iAnimation[ANIM_COLLECT]);
		}
		else if(m_nPlayProduceNum == 3)//차지.
		{
			//pMI->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_GATHER],AN_CLEAR,1.0f,0);	
			AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_GATHER], AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
			m_fActionAnimTime = pMI->GetAnimLength(m_AnimationIndices.m_iAnimation[ANIM_GATHER]);
		}
		else
		{
			IdleAnim();			
		}
		if((pMI->IsAnimationPlaying(m_AnimationIndices.m_iAnimation[ANIM_MINE])|| 
			pMI->IsAnimationPlaying(m_AnimationIndices.m_iAnimation[ANIM_COLLECT]) || pMI->IsAnimationPlaying(m_AnimationIndices.m_iAnimation[ANIM_GATHER])))
		{
			autowait(m_fActionAnimTime-0.1f);
			//autowait(m_fActionAnimTime*0.9f);

			NewClearState(0.1f);  
			StandingAnim();				
		}		
		return EReturn();
	};
	
	// WSS_DRATAN_SEIGEWARFARE 2007/08/01
	ConsensusAnimation(EVoid): CCharacterBase::ConsensusAnimation //1221
	{
		CModelInstance *pMI = GetModelInstance();
		//pMI->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_GATHER],AN_CLEAR,1.0f,0);	
		AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_GATHER], AN_CLEAR|AN_LOOPING, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
		m_fActionAnimTime = pMI->GetAnimLength(m_AnimationIndices.m_iAnimation[ANIM_GATHER]);
		return EReturn();
	};

//0824 액션 추가.
	// FiXME : 아래 루틴 정리가 필요함...ㅡ.ㅡ
	ActionAnimation(EVoid): CCharacterBase::ActionAnimation
	{
		if( (m_nPlayActionNum != ACTION_NUM_SITDOWN) && 
			(m_nPlayActionNum != ACTION_NUM_SELL) && 
			(m_nPlayActionNum != ACTION_NUM_PICKITEM) )
		{
			DeleteWearingWeapon(FALSE);
			for(INDEX i=0; _pNetwork->ga_srvServer.srv_actCha.Count(); ++i)
			{
				if(_pNetwork->ga_srvServer.srv_actCha[i].cha_iClientIndex == en_ulID)
				{
					_pNetwork->ga_srvServer.srv_actCha[i].cha_itemEffect.DeleteEffect(WEAR_WEAPON);
					break;
				}
			}
		}
		else
		{
			if(m_bDisappearWeapon)//무기가 지워져 있다면, 나타나게 해줘야 한다.
			{
				AppearWearingWeapon();
				m_bDisappearWeapon = FALSE;
				for(INDEX i=0; _pNetwork->ga_srvServer.srv_actCha.Count(); ++i)
				{
					CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[i];
					if(ct.cha_iClientIndex == en_ulID
						&& en_pmiModelInstance != NULL)
					{
						ct.cha_itemEffect.Refresh(&GetCurrentPlayerModelInstance()->m_tmSkaTagManager, ct.cha_sbItemEffectOption);
						break;
					}
				}
			}
		}		

		if(m_nPlayActionNum == ACTION_NUM_SITDOWN) //앉기,서기.
		{
			if(m_nSitDown == 0) //서기.
			{
				INDEX iAnim = -1;
				if( m_bRide )
				{
					// 레어 펫 추가로 짝수 타입은 말...060822 wooss
					if( m_iRideType%2 == CPetInfo::PET_HORSE )
					{
						iAnim = m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_STANDUP];
					}
					else
					{
						iAnim = m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_STANDUP];
					}
				}
				else
				{
					iAnim = m_AnimationIndices.m_iAnimation[ANIM_STANDUP];					
				}
				m_fActionAnimTime = GetModelInstance()->GetAnimLength(iAnim);
				AddAnimation(iAnim, AN_NORESTART, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);				
			}
			else//앉기.
			{
				INDEX iAnim = -1;
				if( m_bRide )
				{
					// 레어 펫 추가로 짝수 타입은 말...060822 wooss
					if( m_iRideType%2 == CPetInfo::PET_HORSE )
					{
						iAnim = m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_SIT];
					}
					else
					{
						iAnim = m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_SIT];
					}					
				}
				else
				{
					iAnim = m_AnimationIndices.m_iAnimation[ANIM_SIT];
				}
				m_fActionAnimTime = GetModelInstance()->GetAnimLength(iAnim);		
				AddAnimation(iAnim, AN_NORESTART, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
				//GetModelInstance()->AddAnimation(iAnim,AN_NORESTART,1.0f,0);					
			}			
			
		}
		else if(m_nPlayActionNum == ACTION_NUM_SELL)
		{
			if(m_nSitDown == 0) //서기.
			{
				INDEX iAnim = -1;
				if( m_bRide )
				{
					// 레어 펫 추가로 짝수 타입은 말...060822 wooss
					if( m_iRideType%2 == CPetInfo::PET_HORSE )
					{
						iAnim = m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_STANDUP];
					}
					else 
					{
						iAnim = m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_STANDUP];
					}
				}
				else
				{
					iAnim = m_AnimationIndices.m_iAnimation[ANIM_STANDUP];					
				}
				m_fActionAnimTime = GetModelInstance()->GetAnimLength(iAnim);
				AddAnimation(iAnim, AN_NORESTART, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
				//GetModelInstance()->AddAnimation(iAnim,AN_NORESTART,1.0f,0);				
			}
			else//앉기.
			{
				INDEX iAnim = -1;
				if( m_bRide )
				{
					// 레어 펫 추가로 짝수 타입은 말...060822 wooss
					if( m_iRideType%2 == CPetInfo::PET_HORSE )
					{
						iAnim = m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_SIT];
					}
					else
					{
						iAnim = m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_SIT];
					}					
				}
				else
				{
					iAnim = m_AnimationIndices.m_iAnimation[ANIM_SIT];										
				}
				m_fActionAnimTime = GetModelInstance()->GetAnimLength(iAnim);				
				AddAnimation(iAnim, AN_NORESTART, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);				
			}			
		}
		else if(m_nPlayActionNum == ACTION_NUM_PICKITEM)
		{
			INDEX iAnimationPick = -1;
			if( !m_bExtension || m_bMobChange )
			{
				if( m_bRide )
				{
					// 레어 펫 추가로 짝수 타입은 말...060822 wooss
					if( m_iRideType%2 == CPetInfo::PET_HORSE )
					{
						iAnimationPick = m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_PICK];
					}
					else
					{
						iAnimationPick = m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_PICK];
					}					
				}
				else
				{
					iAnimationPick = m_AnimationIndices.m_iAnimation[ANIM_PICK];
				}
			}
			else
			{
				if( m_bRide )
				{
					// 레어 펫 추가로 짝수 타입은 말...060822 wooss
					if( m_iRideType%2 == CPetInfo::PET_HORSE )
					{
						iAnimationPick = m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_PICK];
					}
					else
					{
						iAnimationPick = m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_PICK];
					}					
				}
				else
				{
					iAnimationPick = m_AnimationIndices.m_iAnimation[ANIM_EXT_PICK];
				}
			}
			//GetModelInstance()->AddAnimation(iAnimationPick,AN_NORESTART,1.0f,0);	
			AddAnimation(iAnimationPick, AN_NORESTART, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
			m_fActionAnimTime = GetModelInstance()->GetAnimLength(iAnimationPick);
		}
		else if(m_nPlayActionNum ==  ACTION_NUM_GREET)
		{
			//GetModelInstance()->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_0],AN_NORESTART,1.0f,0);	
			AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_0], AN_NORESTART, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
			m_fActionAnimTime = GetModelInstance()->GetAnimLength(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_0]);
		}
		else if(m_nPlayActionNum ==  ACTION_NUM_SMILE)
		{
			//GetModelInstance()->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_1],AN_NORESTART,1.0f,0);	
			AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_1], AN_NORESTART, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
			m_fActionAnimTime = GetModelInstance()->GetAnimLength(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_1]);
		}
		else if(m_nPlayActionNum ==  ACTION_NUM_CRY)
		{
			//GetModelInstance()->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_2],AN_NORESTART,1.0f,0);	
			AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_2], AN_NORESTART, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
			m_fActionAnimTime = GetModelInstance()->GetAnimLength(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_2]);
		}
		else if(m_nPlayActionNum ==  ACTION_NUM_NUMBER_ONE)//최고
		{
			//GetModelInstance()->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_3],AN_NORESTART,1.0f,0);	
			AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_3], AN_NORESTART, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
			m_fActionAnimTime = GetModelInstance()->GetAnimLength(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_3]);
		}
		else if(m_nPlayActionNum ==  ACTION_NUM_HANDCLAP)//박수
		{
			//GetModelInstance()->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_4],AN_NORESTART,1.0f,0);	
			AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_4], AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
			m_fActionAnimTime = GetModelInstance()->GetAnimLength(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_4]);
		}
		else if(m_nPlayActionNum ==  ACTION_NUM_REFUSE)//거절
		{
			//GetModelInstance()->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_5],AN_NORESTART,1.0f,0);	
			AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_5], AN_NORESTART, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
			m_fActionAnimTime = GetModelInstance()->GetAnimLength(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_5]);
		}
		else if(m_nPlayActionNum ==  ACTION_NUM_GOOD_LOOKS || m_nPlayActionNum ==  ACTION_NUM_GOOD_LOOKS2)//육체미
		{
			//GetModelInstance()->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_6],AN_NORESTART,1.0f,0);	
			AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_6], AN_NORESTART, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
			m_fActionAnimTime = GetModelInstance()->GetAnimLength(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_6]);
		}
		/*
		else if(m_nPlayActionNum ==  ACTION_NUM_BANTER)//야유
		{
			GetModelInstance()->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_7],AN_NORESTART,1.0f,0);	
			m_fActionAnimTime = GetModelInstance()->GetAnimLength(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_7]);
		}
		else if(m_nPlayActionNum ==  ACTION_NUM_CHEER)//환호
		{
			GetModelInstance()->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_8],AN_NORESTART,1.0f,0);	
			m_fActionAnimTime = GetModelInstance()->GetAnimLength(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_8]);
		}
		else if(m_nPlayActionNum ==  ACTION_NUM_COMBAT)//도전
		{
			GetModelInstance()->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_9],AN_NORESTART,1.0f,0);	
			m_fActionAnimTime = GetModelInstance()->GetAnimLength(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_9]);
		}
		*/
		else if(m_nPlayActionNum ==  ACTION_NUM_SUBMISSION)//복종
		{
			//GetModelInstance()->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_10],AN_NORESTART,1.0f,0);	
			AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_10], AN_NORESTART, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
			m_fActionAnimTime = GetModelInstance()->GetAnimLength(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_10]);
		}
		else if(m_nPlayActionNum == ACTION_NUM_WATER_SPREAD)
		{
			//GetModelInstance()->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_11],AN_NORESTART,1.0f,0);	
			AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_11], AN_NORESTART, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
			m_fActionAnimTime = GetModelInstance()->GetAnimLength(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_11]);
		}

		autowait(m_fActionAnimTime-0.1f);

		if( (m_nPlayActionNum == 3 || m_nPlayActionNum == ACTION_NUM_SELL) && m_nSitDown==2)
		{
			INDEX iAnim = -1;
			if( m_bRide )
			{				
				// 레어 펫 추가로 짝수 타입은 말...060822 wooss
				if( m_iRideType%2 == CPetInfo::PET_HORSE )
				{
					iAnim = m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_SIT_CONTINUE];
				}
				else
				{
					iAnim = m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_SIT_CONTINUE];
				}

			}
			else
			{
				iAnim = m_AnimationIndices.m_iAnimation[ANIM_SIT_CONTINUE];				
			}	
			AddAnimation(iAnim, AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
			//GetModelInstance()->AddAnimation(iAnim,AN_LOOPING|AN_NORESTART|AN_CLEAR,1.0f,0);			
		}
		else
		{
			AppearWearingWeapon();//0127
			for(INDEX i=0; _pNetwork->ga_srvServer.srv_actCha.Count(); ++i)
			{
				CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[i];
				if(ct.cha_iClientIndex == en_ulID
					&& en_pmiModelInstance != NULL)
				{
					ct.cha_itemEffect.Refresh(&GetCurrentPlayerModelInstance()->m_tmSkaTagManager, ct.cha_sbItemEffectOption);
					break;
				}
			}
			NewClearState(0.1f);  
			StandingAnim();				
		}
			
		return EReturn();
	};

	PreSkillAnimation(EVoid)
	{		
		m_vDesiredPosition = PlayerDestinationPos();
		
		StopMoving();//0831

		m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetReadyAnim(m_bExtension));

		m_fSkillAnimTime = GetModelInstance()->GetAnimLength(m_idCurrentSkillAnim) * m_fSkillSpeed;				

		AddAnimation(m_idCurrentSkillAnim, AN_NORESTART, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE, m_fSkillSpeed);
		m_tmSkillStartTime = _pTimer->CurrentTick();

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
						m_pSkillReadyEffect = StartEffectGroup(skill.GetReadyEffect(m_bExtension)
										, &en_pmiModelInstance->m_tmSkaTagManager
										, _pTimer->GetLerpedCurrentTick());
						if(m_pSkillReadyEffect) {m_pSkillReadyEffect->SetSpeedMul(m_fSkillSpeed);}
					}
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)
					resume; 
				};
			on (ETimer) : { stop; };
		}

		m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireAnim(m_bExtension));
	
		m_fSkillAnimTime = GetModelInstance()->GetAnimLength(m_idCurrentSkillAnim) * m_fSkillSpeed;

		AddAnimation(m_idCurrentSkillAnim, AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE, m_fSkillSpeed);
		
		m_tmSkillStartTime = _pTimer->CurrentTick();
		autowait(0.1f);//0114 임시코드 still애니메이션 0.1초

		return EReturn();
	}
		
	SkillAnimation(EVoid) //0628
	{
		m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireAnim(m_bExtension));
		if(m_idCurrentSkillAnim == -1)
		{
			return EReturn();
		}
		m_bIdleAnim = TRUE;

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
		m_iSkillEffectStep = 0;
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)

		NewClearState(0.2f);    

		if(HardCodingSkill(TRUE))//하드코딩...다이어 스트라이크
		{
			m_fSkillAnimTime = 999.0f;
			m_tmSkillStartTime = _pTimer->CurrentTick();

			m_fAttackFrequency = 0.050f;
			m_fImpactTime = 2.3f;
		}
		else
		{
			m_fSkillAnimTime = GetModelInstance()->GetAnimLength(m_idCurrentSkillAnim) * m_fSkillSpeed;
			m_tmSkillStartTime = _pTimer->CurrentTick();			
			
			AddAnimation(m_idCurrentSkillAnim, AN_NORESTART, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE, m_fSkillSpeed);
			
			m_fAttackFrequency = 0.25f;
			m_fImpactTime = GetAnimLength(m_idCurrentSkillAnim) * m_fSkillSpeed / 2;//0630 우선 스킬 애니메이션의 절반에서 임팩트.
		}

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

					if(HardCodingSkill(TRUE))//하드코딩...다이어 스트라이크
					{
						HardCodingSkill(FALSE);
					}
					else
					{
						if(m_nEffectStep == 0)
						{
							++m_nEffectStep;

							CEffectGroup *pFireEffect = StartEffectGroup(skill.GetFireEffect1(m_bExtension), &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
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
						//ASSERT(skill.GetDelay(m_nEffectStep-1) < GetAnimLength(m_idCurrentSkillAnim) && "SkillAnimTime은 반드시 MissileSpeedFireTime보다 커야합니다.");

						// 이 부분에서 iSkillEffectStep의 값은 1~skill.GetDelayCount()이다.
						if( m_nEffectStep <= skill.GetDelayCount(m_bExtension)
						 && _pTimer->CurrentTick() - m_tmSkillStartTime > skill.GetDelay(m_nEffectStep-1, m_bExtension) * m_fSkillSpeed)
						{
							++m_nEffectStep;
							if(skill.GetMissileType(m_bExtension) == CSkill::MT_NONE && m_penEnemy.ep_pen != NULL)	// 근접 공격 - 마법 없음
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
								StartEffectGroup(skill.GetFireEffect3(m_bExtension), _pTimer->GetLerpedCurrentTick(), vHitPoint, quat);
							}
							else if(skill.GetMissileType(m_bExtension) == CSkill::MT_ARROW && m_penEnemy.ep_pen != NULL)	// 화살
							{
								ASSERT(en_pmiModelInstance != NULL);

								FLOAT fHoriOffset = 0.0f;
								FLOAT fVertiOffset = 0.0f;
								if(skill.GetDelayCount(m_bExtension) > 1)
								{
									FLOAT fDist = (m_penEnemy->en_plPlacement.pl_PositionVector - en_plPlacement.pl_PositionVector).Length();
									FLOAT factor = 1.3f * fDist / skill.GetFireRange();
									factor = Clamp(factor, 0.0f, 1.0f);
									if(m_nEffectStep == 2) {fHoriOffset = +1.0f * factor;}
									if(m_nEffectStep == 3) {fHoriOffset = -1.0f * factor;}
									//if(m_nEffectStep == 4) {fVertiOffset = +0.0f * factor;}
									if(m_nEffectStep == 5) {fVertiOffset = -1.0f * factor;}
								}
								if( m_bExtension )
								{
									if( m_dcEnemies.Count() > 0 )
									{
										for( ENTITIES_ITERATOR it = m_dcEnemies.vectorSelectedEntities.begin();
											it != m_dcEnemies.vectorSelectedEntities.end(); ++it )
										{																		
											CEntity *pEn = (*it);
											if(pEn != NULL && pEn->IsFlagOff(ENF_DELETED))
											{
												ShotMissile(this, "STAFF", pEn, skill.GetMissileSpeed(m_bExtension)
													, skill.GetFireEffect3(m_bExtension), skill.GetFireEffect2(m_bExtension)
													, false, fHoriOffset, fVertiOffset, 0);
											}
										}
										m_dcEnemies.Clear();
									}
									else
									{
										ShotMissile(this, "STAFF", m_penEnemy.ep_pen, skill.GetMissileSpeed(m_bExtension)
													, skill.GetFireEffect3(m_bExtension), skill.GetFireEffect2(m_bExtension)
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
												ShotMissile(this, "RHAND", pEn, skill.GetMissileSpeed(m_bExtension)
													, skill.GetFireEffect3(m_bExtension), skill.GetFireEffect2(m_bExtension)
													, false, fHoriOffset, fVertiOffset, 0);
											}
										}
										m_dcEnemies.Clear();
									}
									else
									{
										ShotMissile(this, "RHAND", m_penEnemy.ep_pen, skill.GetMissileSpeed(m_bExtension)
													, skill.GetFireEffect3(m_bExtension), skill.GetFireEffect2(m_bExtension)
													, false, fHoriOffset, fVertiOffset, 0);
									}
								}
							}
							else if(skill.GetMissileType(m_bExtension) == CSkill::MT_DIRECT && m_penEnemy.ep_pen != NULL)	// 발사체 없음, 데미지 없음, 버프 효과 등
							{
								if( m_penEnemy->GetFlags() & ENF_ALIVE && m_penEnemy->en_pmiModelInstance != NULL)
								{
									StartEffectGroup(skill.GetFireEffect3(m_bExtension)
													, &m_penEnemy->en_pmiModelInstance->m_tmSkaTagManager
													, _pTimer->GetLerpedCurrentTick());
								}
							}
							else if(skill.GetMissileType(m_bExtension) == CSkill::MT_DIRECTDAMAGE && m_penEnemy.ep_pen != NULL)	// 발사체 없음, 데미지 있음
							{
								if( m_dcEnemies.Count() > 0 )
								{						
									for( ENTITIES_ITERATOR it = m_dcEnemies.vectorSelectedEntities.begin();
										it != m_dcEnemies.vectorSelectedEntities.end(); ++it )
									{									
										CEntity &en = *(*it);
										if(en.IsFlagOn(ENF_ALIVE) && en.en_pmiModelInstance != NULL)
										{
											StartEffectGroup(skill.GetFireEffect3(m_bExtension)
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
										StartEffectGroup( skill.GetFireEffect3(m_bExtension)
														, &m_penEnemy->en_pmiModelInstance->m_tmSkaTagManager
														, _pTimer->GetLerpedCurrentTick() );
									}
								}
							}
							else if( (skill.GetMissileType(m_bExtension) == CSkill::MT_CONTINUE || 
									skill.GetMissileType(m_bExtension) == CSkill::MT_MAGECUTTER )
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
														, &en, skill.GetMissileSpeed(m_bExtension)
														, skill.GetFireEffect3(m_bExtension), skill.GetFireEffect2(m_bExtension)
														, false, skill.GetMissileType(m_bExtension));
										}
									}
									m_dcEnemies.Clear();
								}
								else
								{
									ShotMagicContinued(this, m_saLastEffectInfo[0].m_vPos, m_saLastEffectInfo[0].m_qRot
														, m_penEnemy, skill.GetMissileSpeed(m_bExtension)
														, skill.GetFireEffect3(m_bExtension), skill.GetFireEffect2(m_bExtension)
														, false, skill.GetMissileType(m_bExtension));
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
							else if(skill.GetMissileType(_pNetwork->MyCharacterInfo.bExtension) == CSkill::MT_GOWAITBACK)
							{
								if( m_penEnemy != NULL )
								{
									ShotGoWaitBack(this, m_saLastEffectInfo[0].m_vPos, m_saLastEffectInfo[0].m_qRot
										, m_penEnemy, skill.GetMissileSpeed(_pNetwork->MyCharacterInfo.bExtension)
										, skill.GetDestDelay(_pNetwork->MyCharacterInfo.bExtension)
										, skill.GetFireEffect3(_pNetwork->MyCharacterInfo.bExtension)
										, skill.GetFireEffect2(_pNetwork->MyCharacterInfo.bExtension)
										, false);
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
							else if(skill.GetMissileType(_pNetwork->MyCharacterInfo.bExtension) == CSkill::MT_CONNECT)
							{
								if( m_penEnemy != NULL )
								{
									ShotConnect(this, m_penEnemy, skill.GetDestDelay(m_bExtension)
										, skill.GetFireEffect3(m_bExtension)
										, skill.GetFireEffect2(m_bExtension));
								}
							}
						}
						
					}

					m_fMoveSpeed = 0.0f;
					if(m_penEnemy != NULL
						&& skill.GetMissileType(m_bExtension) != CSkill::MT_NOTHING)
					{
						ULONG ulFlags = SetDesiredMovement();
					}

					resume; 
				}
				on (EChaSkillDamage eSkillDamage) : 
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

					if(m_penKillEnemy)
					{
						if(m_penKillEnemy->IsEnemy())
						{
							if(((CUnit*)((CEntity*)m_penKillEnemy))->m_nCurrentHealth <= 0)
							{
								CMobData& MD = _pNetwork->GetMobData(((CEnemy*)(CEntity*)m_penKillEnemy)->m_nMobDBIndex);
								if(!MD.IsCastleTower())
								{
									((CUnit*)((CEntity*)m_penKillEnemy))->DeathNow();
								}
								else
								{
									m_penKillEnemy->SetFlagOff(ENF_ALIVE);
								}
							}
						}
						else
						{
							if(((CUnit*)((CEntity*)m_penKillEnemy))->m_nCurrentHealth <= 0)
							{
								((CUnit*)((CEntity*)m_penKillEnemy))->DeathNow();
							}
						}
						m_penKillEnemy = NULL;
						m_bKillEnemy = FALSE;
//							CPrintF("DEATH NOW !!!!!!!!!!!!!!!!!!!!!!\n");
					}

					stop;
				}
				on (EReconsiderBehavior) :
				{
					resume;
				}				
				on (ETimer) : { stop; }			
			}
		}

		// NOTE : 이 부분이 왜 있는 건지 모르겠음..-_-;;;
		if(m_bKillEnemy)
		{
			if(m_penKillEnemy!=NULL)
			{
				if(m_penKillEnemy->IsEnemy())
				{
					if(((CUnit*)((CEntity*)m_penKillEnemy))->m_nCurrentHealth <= 0)
					{
						CMobData& MD = _pNetwork->GetMobData(((CEnemy*)(CEntity*)m_penKillEnemy)->m_nMobDBIndex);
						if(!MD.IsCastleTower())
						{
							((CUnit*)((CEntity*)m_penKillEnemy))->DeathNow();
						}
						else
						{
							m_penKillEnemy->SetFlagOff(ENF_ALIVE);
						}
					}
				}
				else
				{
					if(((CUnit*)((CEntity*)m_penKillEnemy))->m_nCurrentHealth <= 0)
					{
						((CUnit*)((CEntity*)m_penKillEnemy))->DeathNow();
					}
				}
			}
			m_penKillEnemy = NULL;
			m_bKillEnemy = FALSE;
		}

		m_bAttack = FALSE;
		m_bMoving = FALSE;
		m_bStop = FALSE;
		m_bSkilling = FALSE;

		if(m_bIdleAnim)
		{
			StandingAnim();
		}
		SetNoTargetEntity();

		return EReturn();
	};
		
		
	// Normal attack
	NormalAttack(EVoid)
	{
		StandingAnim();
		autowait(0.8f + FRnd()*0.25f);

		AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_ATTACK_1], AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
		//GetModelInstance()->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_ATTACK_1],AN_CLEAR,1.0f,0);
		//PlaySound(m_soSound, SOUND_ATTACK, SOF_3D);

		autowait(0.90f);
		if(m_penEnemy)
		{
			FLOAT3D vEnemyCenter( ((EntityInfo*)m_penEnemy->GetEntityInfo())->vSourceCenter[0], ((EntityInfo*)m_penEnemy->GetEntityInfo())->vSourceCenter[1], ((EntityInfo*)m_penEnemy->GetEntityInfo())->vSourceCenter[2] );
			FLOAT3D vMyCenter( ((EntityInfo*)this->GetEntityInfo())->vSourceCenter[0], ((EntityInfo*)this->GetEntityInfo())->vSourceCenter[1], ((EntityInfo*)this->GetEntityInfo())->vSourceCenter[2] );
			FLOAT size = 0;

			if(m_penEnemy->en_RenderType == RT_SKAMODEL)
			{
				// Bounding Box를 이용하여 Effect를 찍을 위치를 찾는다.
				FLOATaabbox3D bbox;
				m_penEnemy->GetModelInstance()->GetCurrentColisionBox(bbox);
				FLOAT3D vRadius = (bbox.maxvect - bbox.minvect) * 0.5f * 0.4f/*단순히 원으로 하면 너무 큼. 사이즈 조절 상수*/;
				size = vRadius.Length();
			}
			FLOAT3D vTargetPos = m_penEnemy->en_plPlacement.pl_PositionVector + vEnemyCenter;
			FLOAT3D vCurrentPos = this->en_plPlacement.pl_PositionVector + vMyCenter;

			FLOAT3D vDirection = vTargetPos - vCurrentPos;
			vDirection.Normalize();
			vTargetPos -= vDirection * size;
			if(m_penEnemy!=NULL)//0817 버그 수정.
			{
				this->InflictDirectDamage(m_penEnemy, this, DMT_CLOSERANGE, 1, vTargetPos, vDirection);
			}
		}
		autowait(0.60f);

		return EEnd();
	};

	Polymoph(EVoid): CCharacterBase::Polymoph //변신.
	{
		m_fActionAnimTime = _pTimer->CurrentTick();
		m_bChanging = TRUE;
		
		m_vDesiredPosition = GetPlacement().pl_PositionVector;
		StopMoving(); 

		while(_pTimer->CurrentTick() - m_fActionAnimTime < 1.8f)//변신 시간은 실제 1.8초
		{

			wait(0.1f) {
				on (EBegin) : {
					if(!m_bChanging)
					{
						return EReturn();
					}
					MinMaximize();
					resume; 
				}

				on (ETimer) : { stop; }	
			}
		}

		return EReturn();
	}

	Evocate(EVoid) : CCharacterBase::Evocate
	{
		m_fActionAnimTime = _pTimer->CurrentTick();
		m_bTransforming = TRUE;
		
		m_vDesiredPosition = GetPlacement().pl_PositionVector;
		StopMoving(); 

		while(_pTimer->CurrentTick() - m_fActionAnimTime < 1.8f)//변신 시간은 실제 1.8초
		{
			wait(0.1f) 
			{
				on (EBegin) : 
				{
					if(!m_bTransforming)
					{
						return EReturn();
					}
					Transforming();
					resume; 
				}

				on (ETimer) : { stop; }	
			}
		}

		return EReturn();
	};

/************************************************************
 *                       M  A  I  N                         *
 ************************************************************/
	Main(EVoid) 
	{
		if(IsFlagOn(ENF_ZONEMOVING) && en_pmiModelInstance != NULL)
		{
			StartEffectGroup("(EG)Portal", &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->CurrentTick());
		}
		
		//	StartEffectGroup( "GuildBattle", &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->CurrentTick());

		CModelInstance *pMI = GetModelInstance();

		if(pMI == NULL)
		{ 
			return EEnd();
		}
		// declare yourself as a model
		SetPhysicsFlags(EPF_MODEL_WALKING|EPF_HASLUNGS
		| EPF_ABSOLUTETRANSLATE);//1125
		
//		|EPF_NOACCELERATION);
/*	SetPhysicsFlags(EPF_ONBLOCK_CLIMBORSLIDE
			| EPF_TRANSLATEDBYGRAVITY
			| EPF_PUSHABLE
			| EPF_MOVABLE
			| EPF_HASLUNGS
			| EPF_ABSOLUTETRANSLATE);
*/
// NOTE : 공성관련되서...
		SetCollisionFlags(ECF_MODEL);
		//SetCollisionFlags(ECF_MODEL_NO_COLLISION);
//		SetCollisionFlags(GetCollisionFlags() & ~ 
		
		en_sbNetworkType = MSG_CHAR_PC;

		SetFlags(GetFlags()|ENF_ALIVE);		
		en_tmMaxHoldBreath = 5.0f;
		en_fDensity = 2000.0f;

		// setup moving speed
		m_fWalkSpeed			= 3.0f;
		m_aWalkRotateSpeed		= AngleDeg(1800.0f);
		m_fAttackRunSpeed		= 5.0f;
		m_aAttackRotateSpeed	= AngleDeg(1800.0f);
		m_fCloseRunSpeed		= 5.0f;
		m_aCloseRotateSpeed		= AngleDeg(1800.0f);

		// setup attack distances
		m_fAttackDistance		= 5.0f;
		m_fCloseDistance		= 2.0f;			// 근거리 모드????
		m_fStopDistance			= 0.1f;			// (Stop Distance보다 가깝다면 적을 쫓아가지 않음.)
		m_fAttackFireTime		= 0.3f;
		m_fCloseFireTime		= 1.5f;
		m_fBlowUpAmount			= 80.0f;
		m_fBodyParts			= 4;
		m_fDamageWounded		= 0.0f;
		
		// set stretch factors for height and width
		pMI->StretchModel(FLOAT3D(m_fStretch, m_fStretch, m_fStretch));
		pMI->mi_bRenderShadow = TRUE;

		//0605 kwon 추가.				
		InitCurrentWearing();


		SetHealth(10000.0f);//0703 캐릭터의 체력.

		ModelChangeNotify();

		if(m_nCurrentHealth ==0)
		{
			AnimForDeath();
		}
		else
		{
			StandingAnim();
		}
		
//안태훈 수정 시작	//(Open beta)(2004-12-14)
		if(pMI)
		{
			CSkaTag tag;
			tag.SetName("__ROOT");
			tag.SetOffsetRot(GetEulerAngleFromQuaternion(en_pmiModelInstance->mi_qvOffset.qRot));
			pMI->m_tmSkaTagManager.Register(&tag);
			tag.SetName("__TOP");
			tag.SetOffsetRot(GetEulerAngleFromQuaternion(en_pmiModelInstance->mi_qvOffset.qRot));
			FLOATaabbox3D aabb;
			pMI->GetAllFramesBBox(aabb);
			tag.SetOffsetPos(0, aabb.Size()(2) * pMI->mi_vStretch(2), 0);
			pMI->m_tmSkaTagManager.Register(&tag);
		}
//안태훈 수정 끝	//(Open beta)(2004-12-14)

		// continue behavior in base class
		jump CCharacterBase::MainLoop();
	};

};
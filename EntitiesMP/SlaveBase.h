/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#ifndef _SlaveBase_INCLUDED
#define _SlaveBase_INCLUDED 1
#include <EntitiesMP/Player.h>
#include <EntitiesMP/Watcher.h>
#include <EntitiesMP/BasicEffects.h>
#include <EntitiesMP/Projectile.h>
#include <EntitiesMP/Debris.h>
#include <EntitiesMP/EnemyMarker.h>
#include <EntitiesMP/MusicHolder.h>
#include <EntitiesMP/BloodSpray.h>
#include <EntitiesMP/Unit.h>
#define EVENTCODE_ESlaveBaseReconsiderBehavior 0x211f0000
class DECL_DLL ESlaveBaseReconsiderBehavior : public CEntityEvent {
public:
	ESlaveBaseReconsiderBehavior();
	CEntityEvent *MakeCopy(void);
	BOOL CheckIDs(void);
	SLONG GetSizeOf(void);
};
DECL_DLL inline void ClearToDefault(ESlaveBaseReconsiderBehavior &e) { e = ESlaveBaseReconsiderBehavior(); } ;
#define EVENTCODE_ESlaveFoundNewEnemy 0x211f0001
class DECL_DLL ESlaveFoundNewEnemy : public CEntityEvent {
public:
	ESlaveFoundNewEnemy();
	CEntityEvent *MakeCopy(void);
	BOOL CheckIDs(void);
	SLONG GetSizeOf(void);
};
DECL_DLL inline void ClearToDefault(ESlaveFoundNewEnemy &e) { e = ESlaveFoundNewEnemy(); } ;
#define EVENTCODE_ESlaveBaseForceWound 0x211f0002
class DECL_DLL ESlaveBaseForceWound : public CEntityEvent {
public:
	ESlaveBaseForceWound();
	CEntityEvent *MakeCopy(void);
	BOOL CheckIDs(void);
	SLONG GetSizeOf(void);
};
DECL_DLL inline void ClearToDefault(ESlaveBaseForceWound &e) { e = ESlaveBaseForceWound(); } ;
#define EVENTCODE_ESlaveBaseDeath 0x211f0003
class DECL_DLL ESlaveBaseDeath : public CEntityEvent {
public:
	ESlaveBaseDeath();
	CEntityEvent *MakeCopy(void);
	BOOL CheckIDs(void);
	SLONG GetSizeOf(void);
	CEntityID eidInflictor;
};
DECL_DLL inline void ClearToDefault(ESlaveBaseDeath &e) { e = ESlaveBaseDeath(); } ;
#define EVENTCODE_ESlaveBaseDamage 0x211f0004
class DECL_DLL ESlaveBaseDamage : public CEntityEvent {
public:
	ESlaveBaseDamage();
	CEntityEvent *MakeCopy(void);
	BOOL CheckIDs(void);
	SLONG GetSizeOf(void);
	CEntityID eidInflictor;
	FLOAT3D vDirection;
	FLOAT3D vHitPoint;
	FLOAT fAmount;
	enum DamageType dmtType;
	FLOAT fHealth;
};
DECL_DLL inline void ClearToDefault(ESlaveBaseDamage &e) { e = ESlaveBaseDamage(); } ;
#define EVENTCODE_ESlaveBaseBlowUp 0x211f0005
class DECL_DLL ESlaveBaseBlowUp : public CEntityEvent {
public:
	ESlaveBaseBlowUp();
	CEntityEvent *MakeCopy(void);
	BOOL CheckIDs(void);
	SLONG GetSizeOf(void);
	FLOAT3D vDamage;
	FLOAT3D vTranslation;
};
DECL_DLL inline void ClearToDefault(ESlaveBaseBlowUp &e) { e = ESlaveBaseBlowUp(); } ;
extern "C" DECL_DLL CDLLEntityClass CSlaveBase_DLLClass;
class DECL_DLL  CSlaveBase : public CUnit {
public:
virtual const CTString &GetName(void) const { return m_strName; };
virtual BOOL IsTargetable(void) const { return TRUE; };
	 virtual void SetDefaultProperties(void);
	CEntityPointer m_penWatcher;
	FLOAT3D m_vStartPosition;
	CEntityPointer m_penEnemy;
	CTString m_strDescription;
	CTString m_strName;
	CSoundObject m_soSound;
	CEntityPointer m_penOwner;
	FLOAT3D m_vDamage;
	FLOAT m_tmLastDamage;
	BOOL m_bRobotBlowup;
	FLOAT m_fBlowUpSize;
	CEntityPointer m_penPathMarker;
	FLOAT3D m_vPlayerSpotted;
	FLOAT m_fRangeLast;
	BOOL m_bFadeOut;
	FLOAT m_fFadeStartTime;
	FLOAT m_fFadeTime;
	CEntityPointer m_penDeathTarget;
	enum EventEType m_eetDeathType;
	COLOR m_colColor;
	FLOAT m_tmGiveUp;
	FLOAT m_tmReflexMin;
	FLOAT m_tmReflexMax;
	FLOAT m_fActivityRange;
	BOOL m_bMove;
	CEntityPointer m_penMarker;
	CEntityPointer m_penMainMusicHolder;
	FLOAT m_tmLastFussTime;
	FLOAT m_fSpiritStartTime;
	FLOAT m_tmSpraySpawned;
	FLOAT m_fSprayDamage;
	CEntityPointer m_penSpray;
	FLOAT m_fMaxDamageAmmount;
	FLOAT3D m_vLastStain;
	enum SprayParticlesType m_sptType;
	FLOAT3D m_vTacticsStartPosition;
	FLOAT m_fTacticVar3;
	FLOAT m_fTacticVar4;
	FLOAT m_fTacticVar5;
	COLOR m_colBurning;
	INDEX m_iSlaveType;
	INDEX m_iCurrentCommand;
	BOOL m_bMoving;
	FLOAT m_fAttackStartTime;
	FLOAT m_fAttackFrequency;
	BOOL m_bStop;
	FLOAT m_fImpactTime;
	INDEX m_nAttackCnt;
	CEntityPointer m_penKillEnemy;
	BOOL m_bPreSkilling;
CSelectedEntities m_dcTargets;
    CSlaveBase(void);
     ~ CSlaveBase(void);
  
#line 222 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
virtual CTString GetPlayerKillDescription(const CTString & strPlayerName,const EDeath & eDeath);
  
#line 229 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
virtual FLOAT GetCrushHealth(void);
  
#line 234 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
virtual void OnEnd(void);
  
#line 241 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
virtual FLOAT & GetProp(FLOAT & m_fBase);
  
#line 248 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
virtual void MovementAnimation(ULONG ulFlags);
   
#line 296 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
void MaybeSwitchToAnotherPlayer(void);
   
#line 323 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
BOOL SetTargetEntity(CEntity * penPlayer);
   
#line 329 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
void SetNoTargetEntity(void);
   
#line 335 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
void SetTargetNone(void);
   
#line 342 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
BOOL SetKillTargetEntity(CEntity * penKill);
   
#line 350 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
BOOL SetTargetSoft(CEntity * penPlayer);
   
#line 367 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
BOOL SetTargetHard(CEntity * penPlayer);
   
#line 390 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
BOOL SetTargetHardForce(CEntity * penPlayer);
   
#line 411 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
void SetTargetByCommand(INDEX iCommand);
   
#line 511 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
void AssignWatcher(CEntity * penWatcher);
   
#line 517 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
class CWatcher * GetWatcher(void);
   void Copy(CEntity & enOther,ULONG ulFlags);
   
#line 529 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
void Precache(void);
  
#line 555 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
virtual FLOAT3D PlayerDestinationPos(void);
   
#line 560 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
BOOL IfTargetCrushed(CEntity * penOther,const FLOAT3D & vDirection);
   
#line 582 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
BOOL MovesByTargetedRoute(CTString & strTargetProperty)const;
   
#line 587 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
BOOL DropsMarker(CTFileName & fnmMarkerClass,CTString & strTargetProperty)const;
   
#line 593 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
const CTString & GetDescription(void)const;
  
#line 602 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
virtual const CTFileName & GetComputerMessageName(void)const;
   
#line 609 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
void Read_t(CTStream * istr,BOOL bNetwork);
   
#line 621 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
BOOL FillEntityStatistics(EntityStats * pes);
   
#line 632 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
void ReceiveDamage(CEntity * penInflictor,enum DamageType dmtType,
#line 633 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
FLOAT fDamageAmmount,const FLOAT3D & vHitPoint,const FLOAT3D & vDirection);
   
#line 848 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
BOOL AdjustShadingParameters(FLOAT3D & vLightDirection,COLOR & colLight,COLOR & colAmbient);
   
#line 925 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
void AddToFuss(void);
   
#line 955 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
void RemoveFromFuss(void);
   
#line 970 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
BOOL ShouldCeaseAttack(void);
  
#line 989 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
virtual FLOAT GetAttackMoveFrequency(FLOAT fEnemyDistance);
  
#line 995 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
virtual void SetSpeedsToDesiredPosition(
#line 996 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
const FLOAT3D & vPosDelta,
#line 997 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
FLOAT fPosDist,
#line 998 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
BOOL bGoingToPlayer);
   
#line 1071 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
BOOL CanAttackEnemy(CEntity * penTarget,FLOAT fCosAngle);
  
#line 1094 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
virtual BOOL CanHitEnemy(CEntity * penTarget,FLOAT fCosAngle);
   
#line 1100 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
BOOL SeeEntity(CEntity * pen,FLOAT fCosAngle);
   
#line 1108 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
BOOL SeeEntityInPlane(CEntity * pen,FLOAT fCosAngle);
   
#line 1116 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
BOOL WouldNotLeaveAttackRadius(void);
  
#line 1132 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
virtual BOOL MayMoveToAttack(void);
  
#line 1148 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
virtual BOOL ShouldBlowUp(void);
   
#line 1161 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
void BlowUpBase(void);
  
#line 1171 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
virtual void BlowUp(void);
  
#line 1253 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
virtual void LeaveStain(BOOL bGrow);
  
#line 1294 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
virtual void AdjustDifficulty(void);
   
#line 1314 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
void WoundedNotify(const ESlaveBaseDamage & eEBDamage);
   
#line 1335 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
void SeeNotify();
  
#line 1355 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
virtual void ChargeAnim(void);
  
#line 1356 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
virtual INDEX AnimForDamage(FLOAT fDamage);
  
#line 1357 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
virtual void BlowUpNotify(void);
  
#line 1358 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
virtual INDEX AnimForDeath(void);
  
#line 1359 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
virtual FLOAT WaitForDust(FLOAT3D & vStretch);
  
#line 1360 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
virtual void DeathNotify(void);
  
#line 1361 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
virtual void IdleSound(void);
  
#line 1363 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
virtual void WoundSound(void);
  
#line 1364 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
virtual void DeathSound(void);
  
#line 1365 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
virtual FLOAT GetLockRotationSpeed(void);
  
#line 1366 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
virtual void AttackAnim(void);
  
#line 1367 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
virtual void IdleLoop(void);
  
#line 1368 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
virtual void StandUpAnim(void);
   
#line 1371 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
void RenderParticles(void);
  
#line 1389 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
virtual void EnemyPostInit(void);
   
#line 1393 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
BOOL HandleEvent(const CEntityEvent & ee);
   
#line 1439 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
FLOAT GetAnimLength(int iAnim);
   
#line 1449 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
FLOAT GetCurrentAnimLength();
   
#line 1459 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
BOOL IsAnimFinished();
   
#line 1468 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
FLOAT3D & GetModelStretch();
   
#line 1481 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
void StretchModel(FLOAT3D vStretch);
   
#line 1491 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
void StretchSingleModel(FLOAT3D vStretch);
   
#line 1502 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
SLONG GetUsedMemory(void);
   
#line 1513 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
void SpawnBlood_client(ESlaveBaseDamage eEBDamage);
   
#line 1580 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
void DeathNow();
   
#line 1591 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
void MoveNow();
   
#line 1598 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
void AttackNow();
   
#line 1611 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
void StopNow();
   
#line 1622 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
void StopandTeleport();
   
#line 1644 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
void SkillNow();
   
#line 1655 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
void PreSkillNow();
   
#line 1666 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
void ActionNow();
  
#line 1678 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
virtual void DropDeathItem();
#define  STATE_CSlaveBase_Main 1
	BOOL 
#line 1683 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
Main(const CEntityEvent &__eeInput);
#define  STATE_CSlaveBase_NewEnemySpotted 0x211f0006
	BOOL 
#line 1693 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
NewEnemySpotted(const CEntityEvent &__eeInput);
#define  STATE_CSlaveBase_StopAttack 0x211f0007
	BOOL 
#line 1731 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
StopAttack(const CEntityEvent &__eeInput);
#define  STATE_CSlaveBase_InitializeAttack 0x211f0008
	BOOL 
#line 1750 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
InitializeAttack(const CEntityEvent &__eeInput);
#define  STATE_CSlaveBase_AttackEnemy 0x211f0009
	BOOL 
#line 1781 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
AttackEnemy(const CEntityEvent &__eeInput);
	BOOL H0x211f000a_AttackEnemy_01(const CEntityEvent &__eeInput);
	BOOL H0x211f000b_AttackEnemy_02(const CEntityEvent &__eeInput);
	BOOL H0x211f000c_AttackEnemy_03(const CEntityEvent &__eeInput);
	BOOL H0x211f000d_AttackEnemy_04(const CEntityEvent &__eeInput);
	BOOL H0x211f000e_AttackEnemy_05(const CEntityEvent &__eeInput);
	BOOL H0x211f000f_AttackEnemy_06(const CEntityEvent &__eeInput);
	BOOL H0x211f0010_AttackEnemy_07(const CEntityEvent &__eeInput);
	BOOL H0x211f0011_AttackEnemy_08(const CEntityEvent &__eeInput);
#define  STATE_CSlaveBase_PerformAttack 0x211f0012
	BOOL 
#line 1804 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
PerformAttack(const CEntityEvent &__eeInput);
	BOOL H0x211f0013_PerformAttack_01(const CEntityEvent &__eeInput);
	BOOL H0x211f0014_PerformAttack_02(const CEntityEvent &__eeInput);
	BOOL H0x211f0015_PerformAttack_03(const CEntityEvent &__eeInput);
	BOOL H0x211f0016_PerformAttack_04(const CEntityEvent &__eeInput);
#define  STATE_CSlaveBase_FireOrHit 0x211f0017
	BOOL 
#line 1992 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
FireOrHit(const CEntityEvent &__eeInput);
	BOOL H0x211f0018_FireOrHit_01(const CEntityEvent &__eeInput);
	BOOL H0x211f0019_FireOrHit_02(const CEntityEvent &__eeInput);
	BOOL H0x211f001a_FireOrHit_03(const CEntityEvent &__eeInput);
	BOOL H0x211f001b_FireOrHit_04(const CEntityEvent &__eeInput);
#define  STATE_CSlaveBase_Hit 0x211f001c
	BOOL 
#line 2026 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
Hit(const CEntityEvent &__eeInput);
#define  STATE_CSlaveBase_Fire 0x211f001d
	BOOL 
#line 2032 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
Fire(const CEntityEvent &__eeInput);
#define  STATE_CSlaveBase_BeWounded 0x211f001e
	BOOL 
#line 2046 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
BeWounded(const CEntityEvent &__eeInput);
#define  STATE_CSlaveBase_Die 0x211f001f
	BOOL 
#line 2061 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
Die(const CEntityEvent &__eeInput);
	BOOL H0x211f0020_Die_01(const CEntityEvent &__eeInput);
	BOOL H0x211f0021_Die_02(const CEntityEvent &__eeInput);
#define  STATE_CSlaveBase_Death 0x211f0022
	BOOL 
#line 2196 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
Death(const CEntityEvent &__eeInput);
	BOOL H0x211f0023_Death_01(const CEntityEvent &__eeInput);
	BOOL H0x211f0024_Death_02(const CEntityEvent &__eeInput);
	BOOL H0x211f0025_Death_03(const CEntityEvent &__eeInput);
	BOOL H0x211f0026_Death_04(const CEntityEvent &__eeInput);
	BOOL H0x211f0027_Death_05(const CEntityEvent &__eeInput);
	BOOL H0x211f0028_Death_06(const CEntityEvent &__eeInput);
	BOOL H0x211f0029_Death_07(const CEntityEvent &__eeInput);
	BOOL H0x211f002a_Death_08(const CEntityEvent &__eeInput);
	BOOL H0x211f002b_Death_09(const CEntityEvent &__eeInput);
#define  STATE_CSlaveBase_DeathSequence 0x211f002c
	BOOL 
#line 2262 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
DeathSequence(const CEntityEvent &__eeInput);
	BOOL H0x211f002d_DeathSequence_01(const CEntityEvent &__eeInput);
	BOOL H0x211f002e_DeathSequence_02(const CEntityEvent &__eeInput);
	BOOL H0x211f002f_DeathSequence_03(const CEntityEvent &__eeInput);
	BOOL H0x211f0030_DeathSequence_04(const CEntityEvent &__eeInput);
	BOOL H0x211f0031_DeathSequence_05(const CEntityEvent &__eeInput);
	BOOL H0x211f0032_DeathSequence_06(const CEntityEvent &__eeInput);
	BOOL H0x211f0033_DeathSequence_07(const CEntityEvent &__eeInput);
	BOOL H0x211f0034_DeathSequence_08(const CEntityEvent &__eeInput);
#define  STATE_CSlaveBase_Active 0x211f0035
	BOOL 
#line 2312 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
Active(const CEntityEvent &__eeInput);
	BOOL H0x211f0036_Active_01(const CEntityEvent &__eeInput);
	BOOL H0x211f0037_Active_02(const CEntityEvent &__eeInput);
#define  STATE_CSlaveBase_SingleActive 0x211f0038
	BOOL 
#line 2445 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
SingleActive(const CEntityEvent &__eeInput);
	BOOL H0x211f0039_SingleActive_01(const CEntityEvent &__eeInput);
	BOOL H0x211f003a_SingleActive_02(const CEntityEvent &__eeInput);
#define  STATE_CSlaveBase_Inactive 0x211f003b
	BOOL 
#line 2592 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
Inactive(const CEntityEvent &__eeInput);
	BOOL H0x211f003c_Inactive_01(const CEntityEvent &__eeInput);
	BOOL H0x211f003d_Inactive_02(const CEntityEvent &__eeInput);
#define  STATE_CSlaveBase_SingleInActive 0x211f003e
	BOOL 
#line 2666 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
SingleInActive(const CEntityEvent &__eeInput);
	BOOL H0x211f003f_SingleInActive_01(const CEntityEvent &__eeInput);
	BOOL H0x211f0040_SingleInActive_02(const CEntityEvent &__eeInput);
#define  STATE_CSlaveBase_PreMainLoop 0x211f0041
	BOOL 
#line 2745 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
PreMainLoop(const CEntityEvent &__eeInput);
#define  STATE_CSlaveBase_MainLoop_internal 0x211f0042
	BOOL 
#line 2750 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
MainLoop_internal(const CEntityEvent &__eeInput);
	BOOL H0x211f0043_MainLoop_internal_01(const CEntityEvent &__eeInput);
	BOOL H0x211f0044_MainLoop_internal_02(const CEntityEvent &__eeInput);
	BOOL H0x211f0045_MainLoop_internal_03(const CEntityEvent &__eeInput);
	BOOL H0x211f0046_MainLoop_internal_04(const CEntityEvent &__eeInput);
#define  STATE_CSlaveBase_MainLoop 0x211f0047
	BOOL 
#line 2830 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
MainLoop(const CEntityEvent &__eeInput);
#define  STATE_CSlaveBase_StandardBehavior 0x211f0048
	BOOL 
#line 2840 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
StandardBehavior(const CEntityEvent &__eeInput);
	BOOL H0x211f0049_StandardBehavior_01(const CEntityEvent &__eeInput);
	BOOL H0x211f004a_StandardBehavior_02(const CEntityEvent &__eeInput);
#define  STATE_CSlaveBase_AttackTarget 0x211f004b
	BOOL 
#line 2947 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
AttackTarget(const CEntityEvent &__eeInput);
#define  STATE_CSlaveBase_SkillingTarget 0x211f004c
	BOOL 
#line 2952 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
SkillingTarget(const CEntityEvent &__eeInput);
#define  STATE_CSlaveBase_SpellSkill 0x211f004d
	BOOL 
#line 2957 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
SpellSkill(const CEntityEvent &__eeInput);
#define  STATE_CSlaveBase_StopMovement 0x211f004e
	BOOL 
#line 2962 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
StopMovement(const CEntityEvent &__eeInput);
#define  STATE_CSlaveBase_MovetoPoint 0x211f004f
	BOOL 
#line 2976 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SlaveBase.es"
MovetoPoint(const CEntityEvent &__eeInput);
	BOOL H0x211f0050_MovetoPoint_01(const CEntityEvent &__eeInput);
	BOOL H0x211f0051_MovetoPoint_02(const CEntityEvent &__eeInput);
	BOOL H0x211f0052_MovetoPoint_03(const CEntityEvent &__eeInput);
	BOOL H0x211f0053_MovetoPoint_04(const CEntityEvent &__eeInput);
};
#endif // _SlaveBase_INCLUDED
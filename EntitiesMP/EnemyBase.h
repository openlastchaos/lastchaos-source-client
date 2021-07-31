/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#ifndef _EnemyBase_INCLUDED
#define _EnemyBase_INCLUDED 1
#include <EntitiesMP/Player.h>
#include <EntitiesMP/Watcher.h>
#include <EntitiesMP/BasicEffects.h>
#include <EntitiesMP/Projectile.h>
#include <EntitiesMP/Debris.h>
#include <EntitiesMP/EnemyMarker.h>
#include <EntitiesMP/MusicHolder.h>
#include <EntitiesMP/BloodSpray.h>
#include <EntitiesMP/Unit.h>
#define EVENTCODE_EReconsiderBehavior 0x01360000
class DECL_DLL EReconsiderBehavior : public CEntityEvent {
public:
	EReconsiderBehavior();
	CEntityEvent *MakeCopy(void);
	BOOL CheckIDs(void);
	SLONG GetSizeOf(void);
};
DECL_DLL inline void ClearToDefault(EReconsiderBehavior &e) { e = EReconsiderBehavior(); } ;
#define EVENTCODE_EForceWound 0x01360001
class DECL_DLL EForceWound : public CEntityEvent {
public:
	EForceWound();
	CEntityEvent *MakeCopy(void);
	BOOL CheckIDs(void);
	SLONG GetSizeOf(void);
};
DECL_DLL inline void ClearToDefault(EForceWound &e) { e = EForceWound(); } ;
#define EVENTCODE_EEnemyBaseDeath 0x01360002
class DECL_DLL EEnemyBaseDeath : public CEntityEvent {
public:
	EEnemyBaseDeath();
	CEntityEvent *MakeCopy(void);
	BOOL CheckIDs(void);
	SLONG GetSizeOf(void);
	CEntityID eidInflictor;
};
DECL_DLL inline void ClearToDefault(EEnemyBaseDeath &e) { e = EEnemyBaseDeath(); } ;
#define EVENTCODE_EEnemyBaseDamage 0x01360003
class DECL_DLL EEnemyBaseDamage : public CEntityEvent {
public:
	EEnemyBaseDamage();
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
DECL_DLL inline void ClearToDefault(EEnemyBaseDamage &e) { e = EEnemyBaseDamage(); } ;
#define EVENTCODE_EBlowUp 0x01360004
class DECL_DLL EBlowUp : public CEntityEvent {
public:
	EBlowUp();
	CEntityEvent *MakeCopy(void);
	BOOL CheckIDs(void);
	SLONG GetSizeOf(void);
	FLOAT3D vDamage;
	FLOAT3D vTranslation;
};
DECL_DLL inline void ClearToDefault(EBlowUp &e) { e = EBlowUp(); } ;
extern "C" DECL_DLL CDLLEntityClass CEnemyBase_DLLClass;
class DECL_DLL  CEnemyBase : public CUnit {
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
	FLOAT3D m_vDamage;
	FLOAT m_tmLastDamage;
	BOOL m_bRobotBlowup;
	FLOAT m_fBlowUpSize;
	CEntityPointer m_penPathMarker;
	FLOAT3D m_vPlayerSpotted;
	FLOAT m_fLockStartTime;
	FLOAT m_fRangeLast;
	BOOL m_bFadeOut;
	FLOAT m_fFadeStartTime;
	FLOAT m_fFadeTime;
	CEntityPointer m_penSpawnerTarget;
	CEntityPointer m_penDeathTarget;
	enum EventEType m_eetDeathType;
	BOOL m_bTemplate;
	COLOR m_colColor;
	BOOL m_bDeaf;
	BOOL m_bBlind;
	FLOAT m_tmGiveUp;
	FLOAT m_tmReflexMin;
	FLOAT m_tmReflexMax;
	BOOL m_bApplyRandomStretch;
	FLOAT m_fRandomStretchFactor;
	FLOAT m_fStretchMultiplier;
	FLOAT m_fRandomStretchMultiplier;
	CEntityPointer m_penMarker;
	CEntityPointer m_penMainMusicHolder;
	FLOAT m_tmLastFussTime;
	BOOL m_bBoss;
	FLOAT m_fSpiritStartTime;
	FLOAT m_tmSpraySpawned;
	FLOAT m_fSprayDamage;
	CEntityPointer m_penSpray;
	FLOAT m_fMaxDamageAmmount;
	FLOAT3D m_vLastStain;
	enum SprayParticlesType m_sptType;
	CEntityPointer m_penTacticsHolder;
	BOOL m_bTacticActive;
	FLOAT m_tmTacticsActivation;
	FLOAT3D m_vTacticsStartPosition;
	FLOAT m_fTacticVar3;
	FLOAT m_fTacticVar4;
	FLOAT m_fTacticVar5;
	BOOL m_bTacticsStartOnSense;
	COLOR m_colBurning;
	BOOL m_bResizeAttachments;
	FLOAT m_tmStopJumpAnim;
	BOOL m_bMoving;
	BOOL m_bPolymoph;
	FLOAT m_fAttackStartTime;
	FLOAT m_fAttackFrequency;
	BOOL m_bStop;
	FLOAT m_fImpactTime;
	BOOL m_bNpc;
	INDEX m_nAttackCnt;
	CEntityPointer m_penKillEnemy;
	BOOL m_bAction;
    CEnemyBase(void);
     ~ CEnemyBase(void);
  
#line 222 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
virtual CTString GetPlayerKillDescription(const CTString & strPlayerName,const EDeath & eDeath);
  
#line 229 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
virtual FLOAT GetCrushHealth(void);
  
#line 235 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
virtual FLOAT & GetProp(FLOAT & m_fBase);
   
#line 242 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
void MaybeSwitchToAnotherPlayer(void);
   
#line 269 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
void AssignWatcher(CEntity * penWatcher);
   
#line 275 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
class CWatcher * GetWatcher(void);
   void Copy(CEntity & enOther,ULONG ulFlags);
   
#line 287 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
void Precache(void);
  
#line 313 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
virtual FLOAT3D PlayerDestinationPos(void);
   
#line 318 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
BOOL IfTargetCrushed(CEntity * penOther,const FLOAT3D & vDirection);
  
#line 334 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
virtual BOOL IsSkillAttack();
  
#line 339 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
virtual BOOL IsBossMob();
  
#line 346 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
virtual void MovementAnimation(ULONG ulFlags);
   
#line 394 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
BOOL MovesByTargetedRoute(CTString & strTargetProperty)const;
   
#line 399 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
BOOL DropsMarker(CTFileName & fnmMarkerClass,CTString & strTargetProperty)const;
   
#line 405 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
const CTString & GetDescription(void)const;
  
#line 414 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
virtual const CTFileName & GetComputerMessageName(void)const;
   
#line 421 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
void Read_t(CTStream * istr,BOOL bNetwork);
   
#line 433 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
BOOL FillEntityStatistics(EntityStats * pes);
   
#line 451 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
void ReceiveDamage(CEntity * penInflictor,enum DamageType dmtType,
#line 452 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
FLOAT fDamageAmmount,const FLOAT3D & vHitPoint,const FLOAT3D & vDirection);
   
#line 626 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
BOOL AdjustShadingParameters(FLOAT3D & vLightDirection,COLOR & colLight,COLOR & colAmbient);
   
#line 706 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
void AddToFuss(void);
   
#line 740 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
void RemoveFromFuss(void);
   
#line 755 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
BOOL ShouldCeaseAttack(void);
  
#line 770 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
virtual void SizeModel(void);
   
#line 801 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
void SetTargetNone(void);
   
#line 809 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
BOOL SetTargetSoft(CEntity * penPlayer);
   
#line 832 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
BOOL SetTargetHard(CEntity * penPlayer);
   
#line 855 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
BOOL SetTargetHardForce(CEntity * penPlayer);
   
#line 875 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
BOOL SetTargetEntity(CEntity * penPlayer);
   
#line 882 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
BOOL SetKillTargetEntity(CEntity * penKill);
   
#line 889 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
void SetNoTargetEntity(void);
  
#line 900 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
virtual FLOAT GetAttackMoveFrequency(FLOAT fEnemyDistance);
  
#line 913 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
virtual void SetSpeedsToDesiredPosition(
#line 914 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
const FLOAT3D & vPosDelta,
#line 915 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
FLOAT fPosDist,
#line 916 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
BOOL bGoingToPlayer);
   
#line 980 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
BOOL CanAttackEnemy(CEntity * penTarget,FLOAT fCosAngle);
  
#line 996 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
virtual BOOL CanHitEnemy(CEntity * penTarget,FLOAT fCosAngle);
   
#line 1012 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
BOOL SeeEntity(CEntity * pen,FLOAT fCosAngle);
   
#line 1020 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
BOOL SeeEntityInPlane(CEntity * pen,FLOAT fCosAngle);
   
#line 1028 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
BOOL WouldNotLeaveAttackRadius(void);
  
#line 1044 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
virtual BOOL MayMoveToAttack(void);
  
#line 1060 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
virtual BOOL ShouldBlowUp(void);
   
#line 1073 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
void BlowUpBase(void);
  
#line 1083 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
virtual void BlowUp(void);
  
#line 1165 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
virtual void LeaveStain(BOOL bGrow);
  
#line 1206 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
virtual void AdjustDifficulty(void);
   
#line 1226 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
void WoundedNotify(const EEnemyBaseDamage & eEBDamage);
   
#line 1247 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
void SeeNotify();
  
#line 1265 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
virtual void ChargeAnim(void);
  
#line 1266 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
virtual INDEX AnimForDamage(FLOAT fDamage);
  
#line 1267 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
virtual void BlowUpNotify(void);
  
#line 1268 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
virtual INDEX AnimForDeath(void);
  
#line 1269 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
virtual FLOAT WaitForDust(FLOAT3D & vStretch);
  
#line 1270 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
virtual void DeathNotify(void);
  
#line 1271 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
virtual void IdleSound(void);
  
#line 1272 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
virtual void SightSound(void);
  
#line 1273 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
virtual void WoundSound(void);
  
#line 1274 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
virtual void DeathSound(void);
  
#line 1275 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
virtual FLOAT GetLockRotationSpeed(void);
  
#line 1276 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
virtual void IdleLoop(void);
   
#line 1279 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
void RenderParticles(void);
  
#line 1297 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
virtual void EnemyPostInit(void);
   
#line 1301 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
BOOL HandleEvent(const CEntityEvent & ee);
   
#line 1347 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
FLOAT GetAnimLength(int iAnim);
   
#line 1357 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
FLOAT GetCurrentAnimLength();
   
#line 1367 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
BOOL IsAnimFinished();
   
#line 1376 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
FLOAT3D & GetModelStretch();
   
#line 1389 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
void StretchModel(FLOAT3D vStretch);
   
#line 1399 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
void StretchSingleModel(FLOAT3D vStretch);
   
#line 1410 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
SLONG GetUsedMemory(void);
   
#line 1421 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
void SpawnBlood_client(EEnemyBaseDamage eEBDamage);
   
#line 1488 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
void DeathNow();
   
#line 1501 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
void MoveNow();
   
#line 1510 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
void AttackNow();
   
#line 1530 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
void StopNow();
   
#line 1541 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
void SkillNow();
   
#line 1552 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
void PolymophNow();
   
#line 1564 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
void StopandTeleport();
  
#line 1587 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
virtual void DropDeathItem();
#define  STATE_CEnemyBase_Main 1
	BOOL 
#line 1593 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
Main(const CEntityEvent &__eeInput);
#define  STATE_CEnemyBase_NewEnemySpotted 0x01360005
	BOOL 
#line 1602 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
NewEnemySpotted(const CEntityEvent &__eeInput);
	BOOL H0x01360006_NewEnemySpotted_01(const CEntityEvent &__eeInput);
	BOOL H0x01360007_NewEnemySpotted_02(const CEntityEvent &__eeInput);
	BOOL H0x01360008_NewEnemySpotted_03(const CEntityEvent &__eeInput);
#define  STATE_CEnemyBase_StopAttack 0x01360009
	BOOL 
#line 1640 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
StopAttack(const CEntityEvent &__eeInput);
#define  STATE_CEnemyBase_InitializeAttack 0x0136000a
	BOOL 
#line 1657 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
InitializeAttack(const CEntityEvent &__eeInput);
#define  STATE_CEnemyBase_AttackEnemy 0x0136000b
	BOOL 
#line 1689 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
AttackEnemy(const CEntityEvent &__eeInput);
	BOOL H0x0136000c_AttackEnemy_01(const CEntityEvent &__eeInput);
	BOOL H0x0136000d_AttackEnemy_02(const CEntityEvent &__eeInput);
	BOOL H0x0136000e_AttackEnemy_03(const CEntityEvent &__eeInput);
	BOOL H0x0136000f_AttackEnemy_04(const CEntityEvent &__eeInput);
	BOOL H0x01360010_AttackEnemy_05(const CEntityEvent &__eeInput);
	BOOL H0x01360011_AttackEnemy_06(const CEntityEvent &__eeInput);
	BOOL H0x01360012_AttackEnemy_07(const CEntityEvent &__eeInput);
	BOOL H0x01360013_AttackEnemy_08(const CEntityEvent &__eeInput);
#define  STATE_CEnemyBase_PerformAttack 0x01360014
	BOOL 
#line 1712 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
PerformAttack(const CEntityEvent &__eeInput);
	BOOL H0x01360015_PerformAttack_01(const CEntityEvent &__eeInput);
	BOOL H0x01360016_PerformAttack_02(const CEntityEvent &__eeInput);
	BOOL H0x01360017_PerformAttack_03(const CEntityEvent &__eeInput);
	BOOL H0x01360018_PerformAttack_04(const CEntityEvent &__eeInput);
#define  STATE_CEnemyBase_FireOrHit 0x01360019
	BOOL 
#line 1896 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
FireOrHit(const CEntityEvent &__eeInput);
	BOOL H0x0136001a_FireOrHit_01(const CEntityEvent &__eeInput);
	BOOL H0x0136001b_FireOrHit_02(const CEntityEvent &__eeInput);
	BOOL H0x0136001c_FireOrHit_03(const CEntityEvent &__eeInput);
	BOOL H0x0136001d_FireOrHit_04(const CEntityEvent &__eeInput);
	BOOL H0x0136001e_FireOrHit_05(const CEntityEvent &__eeInput);
	BOOL H0x0136001f_FireOrHit_06(const CEntityEvent &__eeInput);
	BOOL H0x01360020_FireOrHit_07(const CEntityEvent &__eeInput);
	BOOL H0x01360021_FireOrHit_08(const CEntityEvent &__eeInput);
#define  STATE_CEnemyBase_Hit 0x01360022
	BOOL 
#line 1944 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
Hit(const CEntityEvent &__eeInput);
#define  STATE_CEnemyBase_Fire 0x01360023
	BOOL 
#line 1950 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
Fire(const CEntityEvent &__eeInput);
#define  STATE_CEnemyBase_Polymoph 0x01360024
	BOOL 
#line 1955 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
Polymoph(const CEntityEvent &__eeInput);
#define  STATE_CEnemyBase_Action 0x01360025
	BOOL 
#line 1961 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
Action(const CEntityEvent &__eeInput);
#define  STATE_CEnemyBase_BeWounded 0x01360026
	BOOL 
#line 1975 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
BeWounded(const CEntityEvent &__eeInput);
	BOOL H0x01360027_BeWounded_01(const CEntityEvent &__eeInput);
	BOOL H0x01360028_BeWounded_02(const CEntityEvent &__eeInput);
	BOOL H0x01360029_BeWounded_03(const CEntityEvent &__eeInput);
#define  STATE_CEnemyBase_Die 0x0136002a
	BOOL 
#line 1988 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
Die(const CEntityEvent &__eeInput);
	BOOL H0x0136002b_Die_01(const CEntityEvent &__eeInput);
	BOOL H0x0136002c_Die_02(const CEntityEvent &__eeInput);
#define  STATE_CEnemyBase_Death 0x0136002d
	BOOL 
#line 2135 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
Death(const CEntityEvent &__eeInput);
	BOOL H0x0136002e_Death_01(const CEntityEvent &__eeInput);
	BOOL H0x0136002f_Death_02(const CEntityEvent &__eeInput);
	BOOL H0x01360030_Death_03(const CEntityEvent &__eeInput);
	BOOL H0x01360031_Death_04(const CEntityEvent &__eeInput);
	BOOL H0x01360032_Death_05(const CEntityEvent &__eeInput);
	BOOL H0x01360033_Death_06(const CEntityEvent &__eeInput);
	BOOL H0x01360034_Death_07(const CEntityEvent &__eeInput);
	BOOL H0x01360035_Death_08(const CEntityEvent &__eeInput);
	BOOL H0x01360036_Death_09(const CEntityEvent &__eeInput);
#define  STATE_CEnemyBase_DeathSequence 0x01360037
	BOOL 
#line 2201 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
DeathSequence(const CEntityEvent &__eeInput);
	BOOL H0x01360038_DeathSequence_01(const CEntityEvent &__eeInput);
	BOOL H0x01360039_DeathSequence_02(const CEntityEvent &__eeInput);
	BOOL H0x0136003a_DeathSequence_03(const CEntityEvent &__eeInput);
	BOOL H0x0136003b_DeathSequence_04(const CEntityEvent &__eeInput);
	BOOL H0x0136003c_DeathSequence_05(const CEntityEvent &__eeInput);
	BOOL H0x0136003d_DeathSequence_06(const CEntityEvent &__eeInput);
	BOOL H0x0136003e_DeathSequence_07(const CEntityEvent &__eeInput);
	BOOL H0x0136003f_DeathSequence_08(const CEntityEvent &__eeInput);
#define  STATE_CEnemyBase_Active 0x01360040
	BOOL 
#line 2266 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
Active(const CEntityEvent &__eeInput);
	BOOL H0x01360041_Active_01(const CEntityEvent &__eeInput);
	BOOL H0x01360042_Active_02(const CEntityEvent &__eeInput);
#define  STATE_CEnemyBase_SingleActive 0x01360043
	BOOL 
#line 2411 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
SingleActive(const CEntityEvent &__eeInput);
	BOOL H0x01360044_SingleActive_01(const CEntityEvent &__eeInput);
	BOOL H0x01360045_SingleActive_02(const CEntityEvent &__eeInput);
#define  STATE_CEnemyBase_Inactive 0x01360046
	BOOL 
#line 2532 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
Inactive(const CEntityEvent &__eeInput);
	BOOL H0x01360047_Inactive_01(const CEntityEvent &__eeInput);
	BOOL H0x01360048_Inactive_02(const CEntityEvent &__eeInput);
#define  STATE_CEnemyBase_SingleInActive 0x01360049
	BOOL 
#line 2618 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
SingleInActive(const CEntityEvent &__eeInput);
	BOOL H0x0136004a_SingleInActive_01(const CEntityEvent &__eeInput);
	BOOL H0x0136004b_SingleInActive_02(const CEntityEvent &__eeInput);
#define  STATE_CEnemyBase_PreMainLoop 0x0136004c
	BOOL 
#line 2695 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
PreMainLoop(const CEntityEvent &__eeInput);
#define  STATE_CEnemyBase_MainLoop_internal 0x0136004d
	BOOL 
#line 2700 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
MainLoop_internal(const CEntityEvent &__eeInput);
	BOOL H0x0136004e_MainLoop_internal_01(const CEntityEvent &__eeInput);
	BOOL H0x0136004f_MainLoop_internal_02(const CEntityEvent &__eeInput);
#define  STATE_CEnemyBase_MainLoop 0x01360050
	BOOL 
#line 2789 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
MainLoop(const CEntityEvent &__eeInput);
#define  STATE_CEnemyBase_StandardBehavior 0x01360051
	BOOL 
#line 2799 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
StandardBehavior(const CEntityEvent &__eeInput);
	BOOL H0x01360052_StandardBehavior_01(const CEntityEvent &__eeInput);
	BOOL H0x01360053_StandardBehavior_02(const CEntityEvent &__eeInput);
#define  STATE_CEnemyBase_AttackTarget 0x01360054
	BOOL 
#line 2930 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
AttackTarget(const CEntityEvent &__eeInput);
#define  STATE_CEnemyBase_SkillingTarget 0x01360055
	BOOL 
#line 2935 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
SkillingTarget(const CEntityEvent &__eeInput);
#define  STATE_CEnemyBase_StopMovement 0x01360056
	BOOL 
#line 2941 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
StopMovement(const CEntityEvent &__eeInput);
#define  STATE_CEnemyBase_MovetoPoint 0x01360057
	BOOL 
#line 2955 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
MovetoPoint(const CEntityEvent &__eeInput);
	BOOL H0x01360058_MovetoPoint_01(const CEntityEvent &__eeInput);
	BOOL H0x01360059_MovetoPoint_02(const CEntityEvent &__eeInput);
	BOOL H0x0136005a_MovetoPoint_03(const CEntityEvent &__eeInput);
	BOOL H0x0136005b_MovetoPoint_04(const CEntityEvent &__eeInput);
#define  STATE_CEnemyBase_IdleAnimation 0x0136005c
	BOOL 
#line 3002 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
IdleAnimation(const CEntityEvent &__eeInput);
#define  STATE_CEnemyBase_IdleLoop 0x0136005d
	BOOL 
#line 3007 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemyBase.es"
IdleLoop(const CEntityEvent &__eeInput);
	BOOL H0x0136005e_IdleLoop_01(const CEntityEvent &__eeInput);
	BOOL H0x0136005f_IdleLoop_02(const CEntityEvent &__eeInput);
	BOOL H0x01360060_IdleLoop_03(const CEntityEvent &__eeInput);
	BOOL H0x01360061_IdleLoop_04(const CEntityEvent &__eeInput);
};
#endif // _EnemyBase_INCLUDED
/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#ifndef _EnemySpawner_INCLUDED
#define _EnemySpawner_INCLUDED 1
#include <EntitiesMP/Enemy.h>
#include <EntitiesMP/EnemyBase.h>
#include <EntitiesMP/BasicEffects.h>
extern DECL_DLL CEntityPropertyEnumType EnemySpawnerType_enum;
enum EnemySpawnerType {
	EST_SIMPLE = 0,
	EST_RESPAWNER = 1,
	EST_DESTROYABLE = 2,
	EST_TRIGGERED = 3,
	EST_TELEPORTER = 4,
	EST_RESPAWNERBYONE = 5,
	EST_MAINTAINGROUP = 6,
	EST_RESPAWNGROUP = 7,
	EST_NPCAPPEARACTION = 8,
};
DECL_DLL inline void ClearToDefault(EnemySpawnerType &e) { e = (EnemySpawnerType)0; } ;
#define EVENTCODE_ENetSpawnEntity 0x01300000
class DECL_DLL ENetSpawnEntity : public CEntityEvent {
public:
	ENetSpawnEntity();
	CEntityEvent *MakeCopy(void);
	BOOL CheckIDs(void);
	SLONG GetSizeOf(void);
	ULONG ulEntityID;
	BOOL bCopy;
	FLOAT3D vPosition;
	ANGLE3D vOrientation;
};
DECL_DLL inline void ClearToDefault(ENetSpawnEntity &e) { e = ENetSpawnEntity(); } ;
extern "C" DECL_DLL CDLLEntityClass CEnemySpawner_DLLClass;
class CEnemySpawner : public CRationalEntity {
public:
virtual const CTString &GetName(void) const { return m_strName; };
virtual CEntity *GetTarget(void) const { return m_penTarget; };
virtual BOOL IsTargetable(void) const { return TRUE; };
virtual BOOL GetRaidObject(void) const { return m_bRaidObject; };
virtual INDEX GetRaidEvent(void) const { return m_RaidEventType; };
	DECL_DLL virtual void SetDefaultProperties(void);
	CTString m_strName;
	BOOL m_bRaidObject;
	INDEX m_RaidEventType;
	CEntityPointer m_penTarget;
	CTString m_strDescription;
	RANGE m_fInnerCircle;
	RANGE m_fOuterCircle;
	FLOAT m_tmDelay;
	FLOAT m_tmSingleWait;
	FLOAT m_tmGroupWait;
	INDEX m_ctGroupSize;
	INDEX m_ctTotal;
	CEntityPointer m_penPatrol;
	enum EnemySpawnerType m_estType;
	BOOL m_bTelefrag;
	BOOL m_bSpawnEffect;
	BOOL m_bDoubleInSerious;
	CEntityPointer m_penSeriousTarget;
	BOOL m_bFirstPass;
	CSoundObject m_soSpawn;
	INDEX m_iInGroup;
	INDEX m_iEnemiesTriggered;
	CEntityPointer m_penTacticsHolder;
	BOOL m_bTacticsAutostart;
	INDEX m_iMobType;
	BOOL m_bAngleFix;
	BOOL m_bShowSpawnEffect;
	CTString m_strAnimation;
CEntityPointer m_penAction;
     ~ CEnemySpawner(void);
   
#line 94 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemySpawner.es"
void Precache(void);
   
#line 100 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemySpawner.es"
const CTString & GetDescription(void)const;
   
#line 121 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemySpawner.es"
BOOL CheckTemplateValid(CEntity * pen);
   
#line 134 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemySpawner.es"
BOOL IsTargetValid(SLONG slPropertyOffset,CEntity * penTarget);
   
#line 158 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemySpawner.es"
BOOL FillEntityStatistics(EntityStats * pes);
  
#line 172 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemySpawner.es"
virtual BOOL IsEnemySpawner(void)const;
   
#line 177 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemySpawner.es"
BOOL EntityIsAnimation(void);
   
#line 193 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemySpawner.es"
void EntityAppearStart(void);
   
#line 211 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemySpawner.es"
void EntityAppearEnd(void);
   
#line 223 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemySpawner.es"
void SpawnEntity(BOOL bCopy);
   
#line 452 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemySpawner.es"
void SpawnEntity_net(ENetSpawnEntity eNetSpawn);
   
#line 517 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemySpawner.es"
BOOL HandleEvent(const CEntityEvent & ee);
   
#line 543 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemySpawner.es"
SLONG GetUsedMemory(void);
#define  STATE_CEnemySpawner_Main 1
	BOOL 
#line 555 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemySpawner.es"
Main(const CEntityEvent &__eeInput);
	BOOL H0x01300001_Main_01(const CEntityEvent &__eeInput);
	BOOL H0x01300002_Main_02(const CEntityEvent &__eeInput);
	BOOL H0x01300003_Main_03(const CEntityEvent &__eeInput);
	BOOL H0x01300004_Main_04(const CEntityEvent &__eeInput);
#define  STATE_CEnemySpawner_SpawnGroup 0x01300005
	BOOL 
#line 669 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemySpawner.es"
SpawnGroup(const CEntityEvent &__eeInput);
	BOOL H0x01300006_SpawnGroup_01(const CEntityEvent &__eeInput);
	BOOL H0x01300007_SpawnGroup_02(const CEntityEvent &__eeInput);
	BOOL H0x01300008_SpawnGroup_03(const CEntityEvent &__eeInput);
	BOOL H0x01300009_SpawnGroup_04(const CEntityEvent &__eeInput);
#define  STATE_CEnemySpawner_Simple 0x0130000a
	BOOL 
#line 726 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemySpawner.es"
Simple(const CEntityEvent &__eeInput);
	BOOL H0x0130000b_Simple_01(const CEntityEvent &__eeInput);
	BOOL H0x0130000c_Simple_02(const CEntityEvent &__eeInput);
	BOOL H0x0130000d_Simple_03(const CEntityEvent &__eeInput);
	BOOL H0x0130000e_Simple_04(const CEntityEvent &__eeInput);
	BOOL H0x0130000f_Simple_05(const CEntityEvent &__eeInput);
	BOOL H0x01300010_Simple_06(const CEntityEvent &__eeInput);
	BOOL H0x01300011_Simple_07(const CEntityEvent &__eeInput);
	BOOL H0x01300012_Simple_08(const CEntityEvent &__eeInput);
	BOOL H0x01300013_Simple_09(const CEntityEvent &__eeInput);
	BOOL H0x01300014_Simple_10(const CEntityEvent &__eeInput);
	BOOL H0x01300015_Simple_11(const CEntityEvent &__eeInput);
#define  STATE_CEnemySpawner_Teleporter 0x01300016
	BOOL 
#line 752 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemySpawner.es"
Teleporter(const CEntityEvent &__eeInput);
	BOOL H0x01300017_Teleporter_01(const CEntityEvent &__eeInput);
	BOOL H0x01300018_Teleporter_02(const CEntityEvent &__eeInput);
	BOOL H0x01300019_Teleporter_03(const CEntityEvent &__eeInput);
	BOOL H0x0130001a_Teleporter_04(const CEntityEvent &__eeInput);
	BOOL H0x0130001b_Teleporter_05(const CEntityEvent &__eeInput);
#define  STATE_CEnemySpawner_Respawner 0x0130001c
	BOOL 
#line 776 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemySpawner.es"
Respawner(const CEntityEvent &__eeInput);
	BOOL H0x0130001d_Respawner_01(const CEntityEvent &__eeInput);
	BOOL H0x0130001e_Respawner_02(const CEntityEvent &__eeInput);
	BOOL H0x0130001f_Respawner_03(const CEntityEvent &__eeInput);
	BOOL H0x01300020_Respawner_04(const CEntityEvent &__eeInput);
	BOOL H0x01300021_Respawner_05(const CEntityEvent &__eeInput);
	BOOL H0x01300022_Respawner_06(const CEntityEvent &__eeInput);
	BOOL H0x01300023_Respawner_07(const CEntityEvent &__eeInput);
	BOOL H0x01300024_Respawner_08(const CEntityEvent &__eeInput);
	BOOL H0x01300025_Respawner_09(const CEntityEvent &__eeInput);
	BOOL H0x01300026_Respawner_10(const CEntityEvent &__eeInput);
	BOOL H0x01300027_Respawner_11(const CEntityEvent &__eeInput);
	BOOL H0x01300028_Respawner_12(const CEntityEvent &__eeInput);
	BOOL H0x01300029_Respawner_13(const CEntityEvent &__eeInput);
	BOOL H0x0130002a_Respawner_14(const CEntityEvent &__eeInput);
	BOOL H0x0130002b_Respawner_15(const CEntityEvent &__eeInput);
	BOOL H0x0130002c_Respawner_16(const CEntityEvent &__eeInput);
	BOOL H0x0130002d_Respawner_17(const CEntityEvent &__eeInput);
	BOOL H0x0130002e_Respawner_18(const CEntityEvent &__eeInput);
	BOOL H0x0130002f_Respawner_19(const CEntityEvent &__eeInput);
	BOOL H0x01300030_Respawner_20(const CEntityEvent &__eeInput);
	BOOL H0x01300031_Respawner_21(const CEntityEvent &__eeInput);
	BOOL H0x01300032_Respawner_22(const CEntityEvent &__eeInput);
	BOOL H0x01300033_Respawner_23(const CEntityEvent &__eeInput);
	BOOL H0x01300034_Respawner_24(const CEntityEvent &__eeInput);
	BOOL H0x01300035_Respawner_25(const CEntityEvent &__eeInput);
	BOOL H0x01300036_Respawner_26(const CEntityEvent &__eeInput);
	BOOL H0x01300037_Respawner_27(const CEntityEvent &__eeInput);
#define  STATE_CEnemySpawner_DestroyableInactive 0x01300038
	BOOL 
#line 837 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemySpawner.es"
DestroyableInactive(const CEntityEvent &__eeInput);
	BOOL H0x01300039_DestroyableInactive_01(const CEntityEvent &__eeInput);
	BOOL H0x0130003a_DestroyableInactive_02(const CEntityEvent &__eeInput);
#define  STATE_CEnemySpawner_DestroyableActiveSpawning 0x0130003b
	BOOL 
#line 843 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemySpawner.es"
DestroyableActiveSpawning(const CEntityEvent &__eeInput);
	BOOL H0x0130003c_DestroyableActiveSpawning_01(const CEntityEvent &__eeInput);
	BOOL H0x0130003d_DestroyableActiveSpawning_02(const CEntityEvent &__eeInput);
	BOOL H0x0130003e_DestroyableActiveSpawning_03(const CEntityEvent &__eeInput);
	BOOL H0x0130003f_DestroyableActiveSpawning_04(const CEntityEvent &__eeInput);
	BOOL H0x01300040_DestroyableActiveSpawning_05(const CEntityEvent &__eeInput);
	BOOL H0x01300041_DestroyableActiveSpawning_06(const CEntityEvent &__eeInput);
#define  STATE_CEnemySpawner_DestroyableActive 0x01300042
	BOOL 
#line 854 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemySpawner.es"
DestroyableActive(const CEntityEvent &__eeInput);
	BOOL H0x01300043_DestroyableActive_01(const CEntityEvent &__eeInput);
	BOOL H0x01300044_DestroyableActive_02(const CEntityEvent &__eeInput);
#define  STATE_CEnemySpawner_Destroyable 0x01300045
	BOOL 
#line 861 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemySpawner.es"
Destroyable(const CEntityEvent &__eeInput);
	BOOL H0x01300046_Destroyable_01(const CEntityEvent &__eeInput);
	BOOL H0x01300047_Destroyable_02(const CEntityEvent &__eeInput);
#define  STATE_CEnemySpawner_SpawnerAppearAction 0x01300048
	BOOL 
#line 869 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnemySpawner.es"
SpawnerAppearAction(const CEntityEvent &__eeInput);
	BOOL H0x01300049_SpawnerAppearAction_01(const CEntityEvent &__eeInput);
	BOOL H0x0130004a_SpawnerAppearAction_02(const CEntityEvent &__eeInput);
};
#endif // _EnemySpawner_INCLUDED
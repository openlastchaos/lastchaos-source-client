/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#ifndef _EntitiesMP_Effector_INCLUDED
#define _EntitiesMP_Effector_INCLUDED 1
extern DECL_DLL CEntityPropertyEnumType EffectorEffectType_enum;
enum EffectorEffectType {
  ET_NONE = 0,
  ET_DESTROY_OBELISK = 1,
  ET_DESTROY_PYLON = 2,
  ET_HIT_GROUND = 3,
  ET_LIGHTNING = 4,
  ET_SIZING_BIG_BLUE_FLARE = 5,
  ET_SIZING_RING_FLARE = 6,
  ET_MOVING_RING = 7,
  ET_PORTAL_LIGHTNING = 8,
  ET_MORPH_MODELS = 9,
  ET_DISAPPEAR_MODEL = 10,
  ET_APPEAR_MODEL = 11,
  ET_DISAPPEAR_MODEL_NOW = 12,
  ET_APPEAR_MODEL_NOW = 13,
};
DECL_DLL inline void ClearToDefault(EffectorEffectType &e) { e = (EffectorEffectType)0; } ;
#define EVENTCODE_ESpawnEffector 0x02600000
class DECL_DLL ESpawnEffector : public CEntityEvent {
public:
ESpawnEffector();
CEntityEvent *MakeCopy(void);
BOOL CheckIDs(void);
SLONG GetSizeOf(void);
enum EffectorEffectType eetType;
FLOAT3D vDamageDir;
FLOAT3D vDestination;
FLOAT tmLifeTime;
FLOAT fSize;
INDEX ctCount;
CEntityID eidModel;
CEntityID eidModel2;
};
DECL_DLL inline void ClearToDefault(ESpawnEffector &e) { e = ESpawnEffector(); } ;
#define EVENTCODE_ETriggerEffector 0x02600001
class DECL_DLL ETriggerEffector : public CEntityEvent {
public:
ETriggerEffector();
CEntityEvent *MakeCopy(void);
BOOL CheckIDs(void);
SLONG GetSizeOf(void);
};
DECL_DLL inline void ClearToDefault(ETriggerEffector &e) { e = ETriggerEffector(); } ;
extern "C" DECL_DLL CDLLEntityClass CEffector_DLLClass;
class CEffector : public CMovableModelEntity {
public:
  DECL_DLL virtual void SetDefaultProperties(void);
  enum EffectorEffectType m_eetType;
  FLOAT m_tmStarted;
  FLOAT3D m_vDamageDir;
  FLOAT3D m_vFXDestination;
  FLOAT m_tmLifeTime;
  FLOAT m_fSize;
  INDEX m_ctCount;
  BOOL m_bLightSource;
  CAnimObject m_aoLightAnimation;
  INDEX m_iLightAnimation;
  BOOL m_bAlive;
  CEntityPointer m_penModel;
  CEntityPointer m_penModel2;
  BOOL m_bWaitTrigger;
  INDEX m_iModel1ID;
  INDEX m_iModel2ID;
CLightSource m_lsLightSource;
   
#line 108 "E:/USA/EntitiesMP/Effector.es"
void InitializeFromNet();
   
#line 131 "E:/USA/EntitiesMP/Effector.es"
FLOAT CalculateLifeRatio(FLOAT fFadeInRatio,FLOAT fFadeOutRatio);
   
#line 138 "E:/USA/EntitiesMP/Effector.es"
void AdjustMipFactor(FLOAT & fMipFactor);
   
#line 219 "E:/USA/EntitiesMP/Effector.es"
BOOL AdjustShadingParameters(FLOAT3D & vLightDirection,COLOR & colLight,COLOR & colAmbient);
   
#line 234 "E:/USA/EntitiesMP/Effector.es"
void RenderMovingLightnings(void);
   
#line 295 "E:/USA/EntitiesMP/Effector.es"
void RenderParticles(void);
   
#line 331 "E:/USA/EntitiesMP/Effector.es"
void Read_t(CTStream * istr,BOOL bNetwork);
   
#line 341 "E:/USA/EntitiesMP/Effector.es"
CLightSource * GetLightSource(void);
   
#line 351 "E:/USA/EntitiesMP/Effector.es"
void SetupLightSource(void);
#define  STATE_CEffector_Main 1
  BOOL 
#line 406 "E:/USA/EntitiesMP/Effector.es"
Main(const CEntityEvent &__eeInput);
  BOOL H0x02600002_Main_01(const CEntityEvent &__eeInput);
  BOOL H0x02600003_Main_02(const CEntityEvent &__eeInput);
  BOOL H0x02600004_Main_03(const CEntityEvent &__eeInput);
  BOOL H0x02600005_Main_04(const CEntityEvent &__eeInput);
  BOOL H0x02600006_Main_05(const CEntityEvent &__eeInput);
  BOOL H0x02600007_Main_06(const CEntityEvent &__eeInput);
};
#endif // _EntitiesMP_Effector_INCLUDED

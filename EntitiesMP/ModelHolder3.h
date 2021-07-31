/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#ifndef _EntitiesMP_ModelHolder3_INCLUDED
#define _EntitiesMP_ModelHolder3_INCLUDED 1
#include <EntitiesMP/ModelDestruction.h>
#include <EntitiesMP/AnimationChanger.h>
#include <EntitiesMP/BloodSpray.h>
#define EVENTCODE_EModel3Activate 0x00f20000
class DECL_DLL EModel3Activate : public CEntityEvent {
public:
EModel3Activate();
CEntityEvent *MakeCopy(void);
BOOL CheckIDs(void);
SLONG GetSizeOf(void);
};
DECL_DLL inline void ClearToDefault(EModel3Activate &e) { e = EModel3Activate(); } ;
#define EVENTCODE_EModel3Deactivate 0x00f20001
class DECL_DLL EModel3Deactivate : public CEntityEvent {
public:
EModel3Deactivate();
CEntityEvent *MakeCopy(void);
BOOL CheckIDs(void);
SLONG GetSizeOf(void);
};
DECL_DLL inline void ClearToDefault(EModel3Deactivate &e) { e = EModel3Deactivate(); } ;
extern "C" DECL_DLL CDLLEntityClass CModelHolder3_DLLClass;
class CModelHolder3 : public CRationalEntity {
public:
virtual const CTString &GetName(void) const { return m_strName; };
virtual const CTString &GetDescription(void) const { return m_strDescription; };
  DECL_DLL virtual void SetDefaultProperties(void);
  CTFileName m_fnModel;
  CTFileName m_fnTexture;
  FLOAT m_fStretchAll;
  ANGLE3D m_vStretchXYZ;
  CTString m_strName;
  CTString m_strDescription;
  BOOL m_bColliding;
  CTString m_strModelAnimation;
  enum ShadowType m_stClusterShadows;
  BOOL m_bBackground;
  BOOL m_bTargetable;
  enum CustomShadingType m_cstCustomShading;
  ANGLE3D m_aShadingDirection;
  COLOR m_colLight;
  COLOR m_colAmbient;
  FLOAT3D m_vShadingDirection;
  BOOL m_bActive;
  COLOR m_colModelColor;
  RANGE m_rMipFadeDistMetric;
  FLOAT m_fMipFadeLenMetric;
  BOOL m_bRandomStretch;
  ANGLE3D m_fStretchRndXYZ;
  FLOAT m_fStretchRndAll;
  FLOAT3D m_fStretchRandom;
  CEntityPointer m_penDestruction;
  FLOAT3D m_vDamage;
  FLOAT m_tmLastDamage;
  CEntityPointer m_penDestroyTarget;
  CEntityPointer m_penLastDamager;
  FLOAT m_tmSpraySpawned;
  FLOAT m_fSprayDamage;
  CEntityPointer m_penSpray;
  FLOAT m_fMaxDamageAmmount;
  FLOAT m_fClassificationStretch;
  COLOR m_colBurning;
  enum DamageType m_dmtLastDamageType;
  FLOAT m_fChainSawCutDamage;
  FLOAT m_fMaxTessellationLevel;
  BOOL m_bDoNotLoopAnimation;
  BOOL m_bRenderReflection;
  BOOL m_bTerrainShadow;
  CTString m_strEffectGroupNameEvent;
  BOOL m_bDecorationObject;
  BOOL m_bHasRootTag;
  CTString m_strEffectGroupName;
CEffectGroup * m_pModelEffectGroupEvent;
CEffectGroup * m_pModelEffectGroup;
   
#line 120 "E:/USA/EntitiesMP/ModelHolder3.es"
void Precache(void);
   
#line 211 "E:/USA/EntitiesMP/ModelHolder3.es"
BOOL FillEntityStatistics(EntityStats * pes);
   
#line 230 "E:/USA/EntitiesMP/ModelHolder3.es"
FLOAT3D GetClassificationBoxStretch(void);
   
#line 237 "E:/USA/EntitiesMP/ModelHolder3.es"
FLOAT GetMaxTessellationLevel(void);
   
#line 244 "E:/USA/EntitiesMP/ModelHolder3.es"
void ReceiveDamage(CEntity * penInflictor,enum DamageType dmtType,
#line 245 "E:/USA/EntitiesMP/ModelHolder3.es"
FLOAT fDamageAmmount,const FLOAT3D & vHitPoint,const FLOAT3D & vDirection);
   
#line 391 "E:/USA/EntitiesMP/ModelHolder3.es"
void * GetEntityInfo(void);
   
#line 400 "E:/USA/EntitiesMP/ModelHolder3.es"
class CModelDestruction * GetDestruction(void);
   
#line 405 "E:/USA/EntitiesMP/ModelHolder3.es"
BOOL IsTargetable(void)const;
   
#line 411 "E:/USA/EntitiesMP/ModelHolder3.es"
void AdjustMipFactor(FLOAT & fMipFactor);
   
#line 454 "E:/USA/EntitiesMP/ModelHolder3.es"
BOOL AdjustShadingParameters(FLOAT3D & vLightDirection,COLOR & colLight,COLOR & colAmbient);
   
#line 506 "E:/USA/EntitiesMP/ModelHolder3.es"
void MirrorAndStretch(FLOAT fStretch,BOOL bMirrorX);
   
#line 516 "E:/USA/EntitiesMP/ModelHolder3.es"
void StretchModel(void);
   
#line 558 "E:/USA/EntitiesMP/ModelHolder3.es"
void InitModelHolder(void);
   
#line 658 "E:/USA/EntitiesMP/ModelHolder3.es"
void SetModelColor(const COLOR color);
#define  STATE_CModelHolder3_Die 0x00f20002
  BOOL 
#line 665 "E:/USA/EntitiesMP/ModelHolder3.es"
Die(const CEntityEvent &__eeInput);
#define  STATE_CModelHolder3_Main 1
  BOOL 
#line 763 "E:/USA/EntitiesMP/ModelHolder3.es"
Main(const CEntityEvent &__eeInput);
  BOOL H0x00f20003_Main_01(const CEntityEvent &__eeInput);
  BOOL H0x00f20004_Main_02(const CEntityEvent &__eeInput);
};
#endif // _EntitiesMP_ModelHolder3_INCLUDED

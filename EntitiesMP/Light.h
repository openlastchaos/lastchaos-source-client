/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#ifndef _EntitiesMP_Light_INCLUDED
#define _EntitiesMP_Light_INCLUDED 1
#include <EntitiesMP/ModelDestruction.h>
#include <EntitiesMP/AnimationChanger.h>
extern DECL_DLL CEntityPropertyEnumType LightType_enum;
enum LightType {
  LT_POINT = 0,
  LT_AMBIENT = 1,
  LT_STRONG_AMBIENT = 2,
  LT_DIRECTIONAL = 3,
  LT_STRONG_POINT = 4,
};
DECL_DLL inline void ClearToDefault(LightType &e) { e = (LightType)0; } ;
extern DECL_DLL CEntityPropertyEnumType LensFlareType_enum;
enum LensFlareType {
  LFT_NONE = 0,
  LFT_STANDARD = 1,
  LFT_STANDARD_REFLECTIONS = 2,
  LFT_YELLOW_STAR_RED_RING = 3,
  LFT_WHITE_GLOW_STAR_RED_RING = 4,
  LFT_WHITE_GLOW_STAR = 5,
  LFT_WHITE_STAR_RED_RING_STREAKS = 6,
  LFT_WHITE_STAR_RED_REFLECTIONS = 7,
  LFT_BLUE_STAR_BLUE_REFLECTIONS = 8,
  LFT_PV_SPACE_SHIP_WINDOW_FLARE = 9,
  LFT_WHITE_GLOW_STAR_RED_RING_FAR = 10,
  LFT_WHITE_GLOW_FAR = 11,
  LFT_WHITE_GLOW_STAR_NG = 12,
};
DECL_DLL inline void ClearToDefault(LensFlareType &e) { e = (LensFlareType)0; } ;
extern "C" DECL_DLL CDLLEntityClass CLight_DLLClass;
class CLight : public CEntity {
public:
virtual const CTString &GetName(void) const { return m_strName; };
virtual const CTString &GetDescription(void) const { return m_strDescription; };
  DECL_DLL virtual void SetDefaultProperties(void);
  COLOR m_colColor;
  COLOR m_colAmbient;
  FLOAT m_fAmbientAttenuation;
  RANGE m_rFallOffRange;
  RANGE m_rHotSpotRange;
  ILLUMINATIONTYPE m_itIllumination;
  enum LightType m_ltType;
  CTString m_strDescription;
  CTString m_strName;
  BOOL m_bDarkLight;
  FLOAT m_fNearClip;
  FLOAT m_fFarClip;
  BOOL m_bSubstractSectorAmbient;
  BOOL m_bRenderAsSmallLight;
  enum LensFlareType m_lftLensFlare;
  BOOL m_bBackground;
  BOOL m_bLensFlareOnly;
  CTFileName m_fnmLightAnimation;
  ANIMATION m_iLightAnimation;
  FLOAT m_tmOffsetPhase;
  FLOAT m_tmAniStartTime;
  FLOAT m_fTimeMultiply;
  CAnimObject m_aoLightAnimation;
  BOOL m_bTargetable;
  BOOL m_bDynamic;
  BOOL m_bDiffusion;
  CTFileName m_fnmAmbientLightAnimation;
  ANIMATION m_iAmbientLightAnimation;
  CAnimObject m_aoAmbientLightAnimation;
CLightSource m_lsLightSource;
CBoolDefaultFalse m_bdfInitialized;
   
#line 99 "E:/USA/EntitiesMP/Light.es"
void Read_t(CTStream * istr,BOOL bNetwork);
   
#line 113 "E:/USA/EntitiesMP/Light.es"
CAnimData * GetAnimData(SLONG slPropertyOffset);
   
#line 129 "E:/USA/EntitiesMP/Light.es"
BOOL IsTargetable(void)const;
   
#line 134 "E:/USA/EntitiesMP/Light.es"
BOOL IsImportant(void)const;
   
#line 140 "E:/USA/EntitiesMP/Light.es"
BOOL HandleEvent(const CEntityEvent & ee);
   
#line 189 "E:/USA/EntitiesMP/Light.es"
void MirrorAndStretch(FLOAT fStretch,BOOL bMirrorX);
   
#line 200 "E:/USA/EntitiesMP/Light.es"
CLightSource * GetLightSource(void);
   
#line 214 "E:/USA/EntitiesMP/Light.es"
void SetupLightSource(CLightSource & lsNew);
   
#line 330 "E:/USA/EntitiesMP/Light.es"
SLONG GetUsedMemory(void);
#define  STATE_CLight_Main 1
  BOOL 
#line 345 "E:/USA/EntitiesMP/Light.es"
Main(const CEntityEvent &__eeInput);
};
#endif // _EntitiesMP_Light_INCLUDED

/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#ifndef _EntitiesMP_ColorContoller_INCLUDED
#define _EntitiesMP_ColorContoller_INCLUDED 1
#include <EntitiesMP/ModelHolder2.h>
#include <EntitiesMP/ModelHolder3.h>
#include <EntitiesMP/Light.h>
extern DECL_DLL CEntityPropertyEnumType ColorControllerType_enum;
enum ColorControllerType {
  CCT_ONLYCOLOR = 0,
  CCT_ONLYALPHA = 1,
  CCT_COLORANDALPHA = 2,
  CCT_DAYNIGHTCOLOR = 3,
};
DECL_DLL inline void ClearToDefault(ColorControllerType &e) { e = (ColorControllerType)0; } ;
extern "C" DECL_DLL CDLLEntityClass CColorController_DLLClass;
class CColorController : public CRationalEntity {
public:
virtual const CTString &GetName(void) const { return m_strName; };
virtual BOOL IsTargetable(void) const { return TRUE; };
  DECL_DLL virtual void SetDefaultProperties(void);
  enum ColorControllerType m_cctType;
  CTString m_strName;
  COLOR m_colGoal;
  FLOAT m_fFadeInTime;
  FLOAT m_fFadeOutTime;
  CEntityPointer m_penTarget01;
  CEntityPointer m_penTarget02;
  CEntityPointer m_penTarget03;
  CEntityPointer m_penTarget04;
  CEntityPointer m_penTarget05;
  CEntityPointer m_penTarget06;
  CEntityPointer m_penTarget07;
  CEntityPointer m_penTarget08;
  CEntityPointer m_penTarget09;
  CEntityPointer m_penTarget10;
  CEntityPointer m_penTarget11;
  CEntityPointer m_penTarget12;
  CEntityPointer m_penTarget13;
  CEntityPointer m_penTarget14;
  CEntityPointer m_penTarget15;
  CEntityPointer m_penTarget16;
  CEntityPointer m_penTarget17;
  CEntityPointer m_penTarget18;
  CEntityPointer m_penTarget19;
  CEntityPointer m_penTarget20;
BOOL m_bIsFadeIn;
FLOAT m_fPrevGWTime;
FLOAT m_fAllFadeTime;
FLOAT m_fLeftFadeTime;
COLOR m_colTarget01;
COLOR m_colTarget02;
COLOR m_colTarget03;
COLOR m_colTarget04;
COLOR m_colTarget05;
COLOR m_colTarget06;
COLOR m_colTarget07;
COLOR m_colTarget08;
COLOR m_colTarget09;
COLOR m_colTarget10;
COLOR m_colTarget11;
COLOR m_colTarget12;
COLOR m_colTarget13;
COLOR m_colTarget14;
COLOR m_colTarget15;
COLOR m_colTarget16;
COLOR m_colTarget17;
COLOR m_colTarget18;
COLOR m_colTarget19;
COLOR m_colTarget20;
   
#line 88 "E:/USA/EntitiesMP/ColorContoller.es"
BOOL IsTargetValid(SLONG slPropertyOffset,CEntity * penTarget);
   
#line 106 "E:/USA/EntitiesMP/ColorContoller.es"
COLOR GetColor(CEntityPointer pen);
   
#line 124 "E:/USA/EntitiesMP/ColorContoller.es"
void SetColor(CEntityPointer pen,COLOR color);
   
#line 142 "E:/USA/EntitiesMP/ColorContoller.es"
void FillTargetOriginalColor();
   
#line 165 "E:/USA/EntitiesMP/ColorContoller.es"
FLOAT GetRatio();
   
#line 170 "E:/USA/EntitiesMP/ColorContoller.es"
void ColorProcess(CEntityPointer pen,COLOR colOriginal,FLOAT fRatio);
   
#line 203 "E:/USA/EntitiesMP/ColorContoller.es"
void DayNightProcess();
#define  STATE_CColorController_Fade 0x02640000
  BOOL 
#line 248 "E:/USA/EntitiesMP/ColorContoller.es"
Fade(const CEntityEvent &__eeInput);
  BOOL H0x02640001_Fade_01(const CEntityEvent &__eeInput);
  BOOL H0x02640002_Fade_02(const CEntityEvent &__eeInput);
  BOOL H0x02640003_Fade_03(const CEntityEvent &__eeInput);
  BOOL H0x02640004_Fade_04(const CEntityEvent &__eeInput);
#define  STATE_CColorController_MainLoop 0x02640005
  BOOL 
#line 283 "E:/USA/EntitiesMP/ColorContoller.es"
MainLoop(const CEntityEvent &__eeInput);
  BOOL H0x02640006_MainLoop_01(const CEntityEvent &__eeInput);
  BOOL H0x02640007_MainLoop_02(const CEntityEvent &__eeInput);
  BOOL H0x02640008_MainLoop_03(const CEntityEvent &__eeInput);
  BOOL H0x02640009_MainLoop_04(const CEntityEvent &__eeInput);
#define  STATE_CColorController_DyaNightLoop 0x0264000a
  BOOL 
#line 310 "E:/USA/EntitiesMP/ColorContoller.es"
DyaNightLoop(const CEntityEvent &__eeInput);
  BOOL H0x0264000b_DyaNightLoop_01(const CEntityEvent &__eeInput);
  BOOL H0x0264000c_DyaNightLoop_02(const CEntityEvent &__eeInput);
  BOOL H0x0264000d_DyaNightLoop_03(const CEntityEvent &__eeInput);
  BOOL H0x0264000e_DyaNightLoop_04(const CEntityEvent &__eeInput);
  BOOL H0x0264000f_DyaNightLoop_05(const CEntityEvent &__eeInput);
  BOOL H0x02640010_DyaNightLoop_06(const CEntityEvent &__eeInput);
#define  STATE_CColorController_Main 1
  BOOL 
#line 321 "E:/USA/EntitiesMP/ColorContoller.es"
Main(const CEntityEvent &__eeInput);
};
#endif // _EntitiesMP_ColorContoller_INCLUDED

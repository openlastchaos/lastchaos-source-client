/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#line 2 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"

#include "StdH.h"
#include <EntitiesMP/AnimationChanger.h>

#include <EntitiesMP/AnimationHub.h>
#include <EntitiesMP/AnimationHub_tables.h>
void CAnimationHub::SetDefaultProperties(void) {
  m_strName = "Animation hub";
  m_strDescription = "";
  m_tmDelayEach = 0.0f;
  m_penTarget0 = NULL;
  m_penTarget1 = NULL;
  m_penTarget2 = NULL;
  m_penTarget3 = NULL;
  m_penTarget4 = NULL;
  m_penTarget5 = NULL;
  m_penTarget6 = NULL;
  m_penTarget7 = NULL;
  m_penTarget8 = NULL;
  m_penTarget9 = NULL;
  m_tmDelay0 = 0.0f;
  m_tmDelay1 = 0.0f;
  m_tmDelay2 = 0.0f;
  m_tmDelay3 = 0.0f;
  m_tmDelay4 = 0.0f;
  m_tmDelay5 = 0.0f;
  m_tmDelay6 = 0.0f;
  m_tmDelay7 = 0.0f;
  m_tmDelay8 = 0.0f;
  m_tmDelay9 = 0.0f;
  m_iModelAnim = 0;
  m_bModelLoop = 0;
  m_iTextureAnim = 0;
  m_bTextureLoop = 0;
  m_iLightAnim = 0;
  m_bLightLoop = 0;
  m_colAmbient = 0;
  m_colDiffuse = 0;
  m_iCounter = 0;
  CRationalEntity::SetDefaultProperties();
}
  
#line 57 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
const CTString & CAnimationHub::GetDescription(void)const {
#line 58 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
((CTString &) m_strDescription ) . PrintF  ("-><none>");
#line 59 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
if(m_penTarget0  != NULL ){
#line 60 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
((CTString &) m_strDescription ) . PrintF  ("->%s..." , m_penTarget0  -> GetName  ());
#line 61 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
}
#line 62 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
return m_strDescription ;
#line 63 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
}
BOOL CAnimationHub::
#line 66 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
RelayEvents(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CAnimationHub_RelayEvents
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CAnimationHub::RelayEvents expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 69 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
m_iCounter  = 0;
#line 70 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
Jump(STATE_CURRENT,0x00e40004, FALSE, EInternal());return TRUE;}BOOL CAnimationHub::H0x00e40004_RelayEvents_04(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e40004
if(!(m_iCounter  < 10)){ Jump(STATE_CURRENT,0x00e40005, FALSE, EInternal());return TRUE;}
#line 72 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
FLOAT fDelay  = m_tmDelayEach  + (& m_tmDelay0 ) [ m_iCounter  ];
#line 74 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
if(!(fDelay  > 0)){ Jump(STATE_CURRENT,0x00e40003, FALSE, EInternal());return TRUE;}
#line 76 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
SetTimerAfter(fDelay );
Jump(STATE_CURRENT, 0x00e40001, FALSE, EBegin());return TRUE;}BOOL CAnimationHub::H0x00e40001_RelayEvents_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e40001
switch(__eeInput.ee_slEvent) {case EVENTCODE_EBegin: return TRUE;case EVENTCODE_ETimer: Jump(STATE_CURRENT,0x00e40002, FALSE, EInternal()); return TRUE;default: return FALSE; }}BOOL CAnimationHub::H0x00e40002_RelayEvents_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e40002
;Jump(STATE_CURRENT,0x00e40003, FALSE, EInternal());return TRUE;}BOOL CAnimationHub::H0x00e40003_RelayEvents_03(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e40003

#line 80 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
CEntity  * penTarget  = (& m_penTarget0 ) [ m_iCounter  ];
#line 82 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
if(penTarget  == NULL ){
#line 84 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
Jump(STATE_CURRENT, STATE_CAnimationHub_WaitChange, TRUE, EVoid());return TRUE;
#line 85 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
}
#line 87 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
EChangeAnim  eca ;
#line 88 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
eca  . iModelAnim  = m_iModelAnim ;
#line 89 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
eca  . bModelLoop  = m_bModelLoop ;
#line 90 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
eca  . iTextureAnim  = m_iTextureAnim ;
#line 91 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
eca  . bTextureLoop  = m_bTextureLoop ;
#line 92 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
eca  . iLightAnim  = m_iLightAnim ;
#line 93 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
eca  . bLightLoop  = m_bLightLoop ;
#line 94 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
eca  . colAmbient  = m_colAmbient ;
#line 95 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
eca  . colDiffuse  = m_colDiffuse ;
#line 96 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
penTarget  -> SendEvent  (eca );
#line 98 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
m_iCounter  ++;Jump(STATE_CURRENT,0x00e40004, FALSE, EInternal());return TRUE;
#line 99 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
}BOOL CAnimationHub::H0x00e40005_RelayEvents_05(const CEntityEvent &__eeInput) {
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e40005

#line 101 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
Jump(STATE_CURRENT, STATE_CAnimationHub_WaitChange, TRUE, EVoid());return TRUE; ASSERT(FALSE); return TRUE;};BOOL CAnimationHub::
#line 104 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
WaitChange(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CAnimationHub_WaitChange
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CAnimationHub::WaitChange expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 107 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
Jump(STATE_CURRENT,0x00e40009, FALSE, EInternal());return TRUE;}BOOL CAnimationHub::H0x00e40009_WaitChange_03(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e40009
if(!(TRUE )){ Jump(STATE_CURRENT,0x00e4000a, FALSE, EInternal());return TRUE;}
#line 108 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
SetTimerAt(THINKTIME_NEVER);
Jump(STATE_CURRENT, 0x00e40007, FALSE, EBegin());return TRUE;}BOOL CAnimationHub::H0x00e40007_WaitChange_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e40007
switch(__eeInput.ee_slEvent){case(EVENTCODE_EChangeAnim):{const EChangeAnim&eca= (EChangeAnim&)__eeInput;

#line 110 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
m_iModelAnim  = eca  . iModelAnim ;
#line 111 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
m_bModelLoop  = eca  . bModelLoop ;
#line 112 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
m_iTextureAnim  = eca  . iTextureAnim ;
#line 113 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
m_bTextureLoop  = eca  . bTextureLoop ;
#line 114 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
m_iLightAnim  = eca  . iLightAnim ;
#line 115 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
m_bLightLoop  = eca  . bLightLoop ;
#line 116 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
m_colAmbient  = eca  . colAmbient ;
#line 117 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
m_colDiffuse  = eca  . colDiffuse ;
#line 118 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
Jump(STATE_CURRENT, STATE_CAnimationHub_RelayEvents, TRUE, EVoid());return TRUE;
#line 119 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
}ASSERT(FALSE);break;default: return FALSE; break;
#line 120 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
}return TRUE;}BOOL CAnimationHub::H0x00e40008_WaitChange_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e40008
Jump(STATE_CURRENT,0x00e40009, FALSE, EInternal());return TRUE;
#line 121 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
}BOOL CAnimationHub::H0x00e4000a_WaitChange_04(const CEntityEvent &__eeInput) {
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e4000a
 ASSERT(FALSE); return TRUE;};BOOL CAnimationHub::
#line 124 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
Main(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CAnimationHub_Main
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CAnimationHub::Main expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 126 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
InitAsEditorModel  ();
#line 127 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
SetPhysicsFlags  (EPF_MODEL_IMMATERIAL );
#line 128 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
SetCollisionFlags  (ECF_IMMATERIAL );
#line 131 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
SetModel  (MODEL_HUB );
#line 132 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
SetModelMainTexture  (TEXTURE_HUB );
#line 135 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
for(INDEX i  = 0;i  < 10;i  ++){
#line 136 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
CEntityPointer & penTarget  = (& m_penTarget0 ) [ i  ];
#line 137 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
if(penTarget  != NULL  && 
#line 138 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
! IsOfClass  (penTarget  , & CModelHolder2_DLLClass ) && 
#line 139 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
! IsOfClass  (penTarget  , & CLight_DLLClass )){
#line 140 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
WarningMessage  ("All targets must be ModelHolder2 or Light!");
#line 141 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
penTarget  = NULL ;
#line 142 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
}
#line 143 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
}
#line 144 "E:/질산/Korea/Client_USA/EntitiesMP/AnimationHub.es"
Jump(STATE_CURRENT, STATE_CAnimationHub_WaitChange, TRUE, EVoid());return TRUE; ASSERT(FALSE); return TRUE;};
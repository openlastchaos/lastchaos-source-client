/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#line 2 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"

#include "StdH.h"

#include <EntitiesMP/Counter.h>
#include <EntitiesMP/Counter_tables.h>
void CCounter::SetDefaultProperties(void) {
  m_fCountdownSpeed = 12.0f;
  m_penTarget = NULL;
  m_fNumber = 0.0f;
  m_tmStart = -1.0f;
  m_strName = "";
  m_soSound.SetOwner(this);
m_soSound.Stop(FALSE);
  m_iCountFrom = 1023;
  m_pen0 = NULL;
  m_pen1 = NULL;
  m_pen2 = NULL;
  m_pen3 = NULL;
  m_pen4 = NULL;
  m_pen5 = NULL;
  m_pen6 = NULL;
  m_pen7 = NULL;
  m_pen8 = NULL;
  m_pen9 = NULL;
  CRationalEntity::SetDefaultProperties();
}
  
#line 38 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
void CCounter::Precache(void) 
#line 39 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
{
#line 40 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
PrecacheSound  (SOUND_TICK );
#line 41 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
CRationalEntity  :: Precache  ();
#line 42 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
}
  
#line 44 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
void CCounter::DisplayNumber(void) 
#line 45 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
{
#line 46 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
for(INDEX iDigit  = 0;iDigit  < 10;iDigit  ++)
#line 47 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
{
#line 48 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
CModelHolder2  * pmh  = (CModelHolder2  *) & * (& m_pen0 ) [ iDigit  ];
#line 49 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
if(pmh  != NULL  && pmh  -> GetModelObject  () != NULL  && 
#line 50 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
pmh  -> GetModelObject  () -> mo_toTexture  . GetData  () != NULL )
#line 51 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
{
#line 53 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
INDEX iOldAnim  = pmh  -> GetModelObject  () -> mo_toTexture  . GetAnim  ();
#line 54 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
INDEX iAnim  = (INDEX (m_fNumber ) & (1 << iDigit )) >> iDigit ;
#line 55 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
pmh  -> GetModelObject  () -> mo_toTexture  . PlayAnim  (iAnim  , 0);
#line 58 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
m_soSound  . Set3DParameters  (200.0f , 100.0f , 1.0f , 
#line 59 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
Clamp  (1.0f + (m_iCountFrom  - m_fNumber ) / m_iCountFrom  * 2.0f , 1.0f , 3.0f));
#line 60 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
if(iDigit  == 0 && iOldAnim  != iAnim  && ! m_soSound  . IsPlaying  ())
#line 61 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
{
#line 62 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
PlaySound  (m_soSound  , SOUND_TICK  , SOF_3D  | SOF_VOLUMETRIC );
#line 63 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
}
#line 64 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
}
#line 65 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
}
#line 66 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
}
BOOL CCounter::
#line 69 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
CountDown(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CCounter_CountDown
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CCounter::CountDown expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 71 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
Jump(STATE_CURRENT,0x00e80003, FALSE, EInternal());return TRUE;}BOOL CCounter::H0x00e80003_CountDown_03(const CEntityEvent &__eeInput)
#line 72 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
{
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e80003
if(!(TRUE )){ Jump(STATE_CURRENT,0x00e80004, FALSE, EInternal());return TRUE;}
#line 73 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
SetTimerAfter(_pTimer  -> TickQuantum );
Jump(STATE_CURRENT, 0x00e80001, FALSE, EBegin());return TRUE;}BOOL CCounter::H0x00e80001_CountDown_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e80001
switch(__eeInput.ee_slEvent) {case EVENTCODE_EBegin: return TRUE;case EVENTCODE_ETimer: Jump(STATE_CURRENT,0x00e80002, FALSE, EInternal()); return TRUE;default: return FALSE; }}BOOL CCounter::H0x00e80002_CountDown_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e80002
;
#line 74 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
FLOAT tmNow  = _pTimer  -> CurrentTick  ();
#line 75 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
FLOAT tmDelta  = tmNow  - m_tmStart ;
#line 76 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
FLOAT fSub  = Clamp  (tmDelta  / m_fCountdownSpeed  , 0.01f , 1.0f);
#line 77 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
m_fNumber  = Clamp  (m_fNumber  - fSub  , 0.0f , FLOAT (m_iCountFrom ));
#line 78 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
DisplayNumber  ();
#line 79 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
if(m_fNumber  == 0)
#line 80 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
{
#line 81 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
Return(STATE_CURRENT,EReturn  ());
#line 81 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
return TRUE;
#line 82 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
}Jump(STATE_CURRENT,0x00e80003, FALSE, EInternal());return TRUE;
#line 83 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
}BOOL CCounter::H0x00e80004_CountDown_04(const CEntityEvent &__eeInput) {
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e80004
 ASSERT(FALSE); return TRUE;};BOOL CCounter::
#line 89 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
Main(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CCounter_Main
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CCounter::Main expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 91 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
InitAsEditorModel  ();
#line 92 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
SetPhysicsFlags  (EPF_MODEL_IMMATERIAL );
#line 93 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
SetCollisionFlags  (ECF_IMMATERIAL );
#line 96 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
SetModel  (MODEL_MARKER );
#line 97 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
SetModelMainTexture  (TEXTURE_MARKER );
#line 99 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
SetTimerAfter(0.1f);
Jump(STATE_CURRENT, 0x00e80005, FALSE, EBegin());return TRUE;}BOOL CCounter::H0x00e80005_Main_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e80005
switch(__eeInput.ee_slEvent) {case EVENTCODE_EBegin: return TRUE;case EVENTCODE_ETimer: Jump(STATE_CURRENT,0x00e80006, FALSE, EInternal()); return TRUE;default: return FALSE; }}BOOL CCounter::H0x00e80006_Main_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e80006
;
#line 100 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
m_fNumber  = m_iCountFrom ;
#line 101 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
DisplayNumber  ();
#line 103 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
SetTimerAt(THINKTIME_NEVER);
Jump(STATE_CURRENT, 0x00e80007, FALSE, EBegin());return TRUE;}BOOL CCounter::H0x00e80007_Main_03(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e80007
switch(__eeInput.ee_slEvent){case(EVENTCODE_EBegin):{const EBegin&e= (EBegin&)__eeInput;

#line 105 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
return TRUE;
#line 106 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
}ASSERT(FALSE);break;case(EVENTCODE_ETrigger):{const ETrigger&eTrigger= (ETrigger&)__eeInput;

#line 108 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
m_fNumber  = m_iCountFrom ;
#line 109 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
DisplayNumber  ();
#line 110 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
m_tmStart  = _pTimer  -> CurrentTick  ();
#line 111 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
Call(STATE_CURRENT, STATE_CCounter_CountDown, TRUE, EVoid());return TRUE;
#line 112 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
}ASSERT(FALSE);break;case(EVENTCODE_EReturn):{const EReturn&e= (EReturn&)__eeInput;

#line 114 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
if(m_penTarget  != NULL )
#line 115 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
{
#line 116 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
SendToTarget  (m_penTarget  , EET_TRIGGER );
#line 117 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
}
#line 118 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
UnsetTimer();Jump(STATE_CURRENT,0x00e80008, FALSE, EInternal());return TRUE;
#line 119 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
}ASSERT(FALSE);break;default: return FALSE; break;
#line 120 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
}return TRUE;}BOOL CCounter::H0x00e80008_Main_04(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e80008

#line 122 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
Return(STATE_CURRENT,EVoid());
#line 122 "E:/질산/Korea/Client_USA/EntitiesMP/Counter.es"
return TRUE; ASSERT(FALSE); return TRUE;};
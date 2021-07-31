/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#line 2 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"

#include "StdH.h"
extern INDEX ent_bReportBrokenChains;

#include <EntitiesMP/Trigger.h>
#include <EntitiesMP/Trigger_tables.h>
void CTrigger::SetDefaultProperties(void) {
  m_strName = "Trigger";
  m_penTarget1 = NULL;
  m_penTarget2 = NULL;
  m_penTarget3 = NULL;
  m_penTarget4 = NULL;
  m_penTarget5 = NULL;
  m_penTarget6 = NULL;
  m_penTarget7 = NULL;
  m_penTarget8 = NULL;
  m_penTarget9 = NULL;
  m_penTarget10 = NULL;
  m_eetEvent1 = EET_TRIGGER ;
  m_eetEvent2 = EET_TRIGGER ;
  m_eetEvent3 = EET_TRIGGER ;
  m_eetEvent4 = EET_TRIGGER ;
  m_eetEvent5 = EET_TRIGGER ;
  m_eetEvent6 = EET_TRIGGER ;
  m_eetEvent7 = EET_TRIGGER ;
  m_eetEvent8 = EET_TRIGGER ;
  m_eetEvent9 = EET_TRIGGER ;
  m_eetEvent10 = EET_TRIGGER ;
  m_strMessage = "";
  m_fMessageTime = 3.0f;
  m_mssMessageSound = MSS_NONE ;
  m_fWaitTime = 0.0f;
  m_bAutoStart = FALSE ;
  m_iCount = 1;
  m_bUseCount = FALSE ;
  m_bReuseCount = FALSE ;
  m_bTellCount = FALSE ;
  m_bActive = TRUE ;
  m_fSendRange = 1.0f;
  m_eetRange = EET_IGNORE ;
  m_iCountTmp = 0;
  m_penCaused = NULL;
  m_ctMaxTrigs = -1;
  CRationalEntity::SetDefaultProperties();
}
  
#line 65 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
CEntity * CTrigger::GetTarget(void)const 
#line 66 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
{
#line 67 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
return m_penTarget1 ;
#line 68 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
}
  
#line 72 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
SLONG CTrigger::GetUsedMemory(void) 
#line 73 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
{
#line 75 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
SLONG  slUsedMemory  = sizeof  (CTrigger ) - sizeof  (CRationalEntity ) + CRationalEntity  :: GetUsedMemory  ();
#line 77 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
slUsedMemory  += m_strMessage  . Length  ();
#line 78 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
slUsedMemory  += m_strName  . Length  ();
#line 79 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
slUsedMemory  += 1 * sizeof  (CSoundObject);
#line 80 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
return slUsedMemory ;
#line 81 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
}
BOOL CTrigger::
#line 87 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
SendEventToTargets(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CTrigger_SendEventToTargets
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CTrigger::SendEventToTargets expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 89 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
if(!(m_fWaitTime  > 0.0f)){ Jump(STATE_CURRENT,0x00cd0003, FALSE, EInternal());return TRUE;}
#line 90 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
SetTimerAfter(m_fWaitTime );
Jump(STATE_CURRENT, 0x00cd0001, FALSE, EBegin());return TRUE;}BOOL CTrigger::H0x00cd0001_SendEventToTargets_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00cd0001
switch(__eeInput.ee_slEvent){case(EVENTCODE_EBegin):{const EBegin&e= (EBegin&)__eeInput;
return TRUE;}ASSERT(FALSE);break;case(EVENTCODE_ETimer):{const ETimer&e= (ETimer&)__eeInput;
UnsetTimer();Jump(STATE_CURRENT,0x00cd0002, FALSE, EInternal());return TRUE;}ASSERT(FALSE);break;case(EVENTCODE_EDeactivate):{const EDeactivate&e= (EDeactivate&)__eeInput;
return FALSE;}ASSERT(FALSE);break;default:{return TRUE;}ASSERT(FALSE);break;
#line 95 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
}return TRUE;}BOOL CTrigger::H0x00cd0002_SendEventToTargets_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00cd0002
Jump(STATE_CURRENT,0x00cd0003, FALSE, EInternal());return TRUE;}BOOL CTrigger::H0x00cd0003_SendEventToTargets_03(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00cd0003

#line 99 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
SendToTarget  (m_penTarget1  , m_eetEvent1  , m_penCaused );
#line 100 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
SendToTarget  (m_penTarget2  , m_eetEvent2  , m_penCaused );
#line 101 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
SendToTarget  (m_penTarget3  , m_eetEvent3  , m_penCaused );
#line 102 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
SendToTarget  (m_penTarget4  , m_eetEvent4  , m_penCaused );
#line 103 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
SendToTarget  (m_penTarget5  , m_eetEvent5  , m_penCaused );
#line 104 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
SendToTarget  (m_penTarget6  , m_eetEvent6  , m_penCaused );
#line 105 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
SendToTarget  (m_penTarget7  , m_eetEvent7  , m_penCaused );
#line 106 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
SendToTarget  (m_penTarget8  , m_eetEvent8  , m_penCaused );
#line 107 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
SendToTarget  (m_penTarget9  , m_eetEvent9  , m_penCaused );
#line 108 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
SendToTarget  (m_penTarget10  , m_eetEvent10  , m_penCaused );
#line 111 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
if(m_eetRange  != EET_IGNORE ){
#line 113 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
SendInRange  (this  , m_eetRange  , FLOATaabbox3D (GetPlacement  () . pl_PositionVector  , m_fSendRange ));
#line 114 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
}
#line 143 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
if(m_ctMaxTrigs  > 0)
#line 144 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
{
#line 145 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
SetFlagOff  (ENF_PROPSCHANGED );
#line 147 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
m_ctMaxTrigs  -= 1;
#line 149 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
if(m_ctMaxTrigs  <= 0)
#line 150 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
{
#line 152 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
Destroy  ();
#line 153 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
}
#line 154 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
}
#line 155 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
Return(STATE_CURRENT,EVoid());
#line 155 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
return TRUE; ASSERT(FALSE); return TRUE;};BOOL CTrigger::
#line 158 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
Active(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CTrigger_Active
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CTrigger::Active expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 159 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
ASSERT  (m_bActive );
#line 161 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
m_iCountTmp  = m_iCount ;
#line 164 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
SetTimerAt(THINKTIME_NEVER);
Jump(STATE_CURRENT, 0x00cd0005, FALSE, EBegin());return TRUE;}BOOL CTrigger::H0x00cd0005_Active_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00cd0005
switch(__eeInput.ee_slEvent){case(EVENTCODE_EBegin):{const EBegin&e= (EBegin&)__eeInput;

#line 167 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
if(m_bAutoStart ){
#line 168 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
Call(STATE_CURRENT, STATE_CTrigger_SendEventToTargets, TRUE, EVoid());return TRUE;
#line 169 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
}
#line 170 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
return TRUE;
#line 171 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
}ASSERT(FALSE);break;case(EVENTCODE_EStart):{const EStart&eStart= (EStart&)__eeInput;

#line 174 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
SendToTarget  (this  , EET_TRIGGER  , eStart  . penCaused );
#line 175 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
return TRUE;
#line 176 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
}ASSERT(FALSE);break;case(EVENTCODE_ETrigger):{const ETrigger&eTrigger= (ETrigger&)__eeInput;

#line 179 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
SetFlagOff  (ENF_PROPSCHANGED );
#line 180 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
m_penCaused  = eTrigger  . penCaused ;
#line 182 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
if(m_bUseCount ){
#line 184 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
if(m_iCountTmp  > 0){
#line 186 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
m_iCountTmp  --;
#line 188 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
if(m_iCountTmp  < 1){
#line 189 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
if(m_bReuseCount ){
#line 190 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
m_iCountTmp  = m_iCount ;
#line 191 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
}else {
#line 192 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
m_iCountTmp  = 0;
#line 193 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
}
#line 194 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
Call(STATE_CURRENT, STATE_CTrigger_SendEventToTargets, TRUE, EVoid());return TRUE;
#line 195 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
}else if(m_bTellCount ){
#line 199 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
}
#line 200 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
}
#line 202 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
}else {
#line 203 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
Call(STATE_CURRENT, STATE_CTrigger_SendEventToTargets, TRUE, EVoid());return TRUE;
#line 204 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
}
#line 205 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
return TRUE;
#line 206 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
}ASSERT(FALSE);break;case(EVENTCODE_EDeactivate):{const EDeactivate&e= (EDeactivate&)__eeInput;

#line 210 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
m_bActive  = FALSE ;
#line 211 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
Jump(STATE_CURRENT, STATE_CTrigger_Inactive, TRUE, EVoid());return TRUE;
#line 212 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
}ASSERT(FALSE);break;default: return FALSE; break;
#line 213 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
}return TRUE;}BOOL CTrigger::H0x00cd0006_Active_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00cd0006
 ASSERT(FALSE); return TRUE;};BOOL CTrigger::
#line 215 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
Inactive(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CTrigger_Inactive
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CTrigger::Inactive expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 216 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
ASSERT  (! m_bActive );
#line 217 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
Jump(STATE_CURRENT,0x00cd000c, FALSE, EInternal());return TRUE;}BOOL CTrigger::H0x00cd000c_Inactive_05(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00cd000c
if(!(TRUE )){ Jump(STATE_CURRENT,0x00cd000d, FALSE, EInternal());return TRUE;}
#line 219 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
SetTimerAt(THINKTIME_NEVER);
Jump(STATE_CURRENT, 0x00cd0008, FALSE, EBegin());return TRUE;}BOOL CTrigger::H0x00cd0008_Inactive_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00cd0008
switch(__eeInput.ee_slEvent){case(EVENTCODE_EActivate):{const EActivate&e= (EActivate&)__eeInput;

#line 223 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
m_bActive  = TRUE ;
#line 224 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
Jump(STATE_CURRENT, STATE_CTrigger_Active, TRUE, EVoid());return TRUE;
#line 225 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
}ASSERT(FALSE);break;default:{
#line 227 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
return TRUE;
#line 228 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
}ASSERT(FALSE);break;
#line 229 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
}return TRUE;}BOOL CTrigger::H0x00cd0009_Inactive_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00cd0009
;
#line 232 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
SetTimerAfter(0.1f);
Jump(STATE_CURRENT, 0x00cd000a, FALSE, EBegin());return TRUE;}BOOL CTrigger::H0x00cd000a_Inactive_03(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00cd000a
switch(__eeInput.ee_slEvent) {case EVENTCODE_EBegin: return TRUE;case EVENTCODE_ETimer: Jump(STATE_CURRENT,0x00cd000b, FALSE, EInternal()); return TRUE;default: return FALSE; }}BOOL CTrigger::H0x00cd000b_Inactive_04(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00cd000b
;Jump(STATE_CURRENT,0x00cd000c, FALSE, EInternal());return TRUE;
#line 233 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
}BOOL CTrigger::H0x00cd000d_Inactive_06(const CEntityEvent &__eeInput) {
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00cd000d
 ASSERT(FALSE); return TRUE;};BOOL CTrigger::
#line 236 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
Main(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CTrigger_Main
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CTrigger::Main expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 237 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
InitAsEditorModel  ();
#line 238 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
SetPhysicsFlags  (EPF_MODEL_IMMATERIAL );
#line 239 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
SetCollisionFlags  (ECF_IMMATERIAL );
#line 241 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
SetFlagOff  (ENF_PROPSCHANGED );
#line 242 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
SetFlagOn  (ENF_NONETCONNECT );
#line 243 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
SetFlagOn  (ENF_MARKDESTROY );
#line 246 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
SetModel  (MODEL_MARKER );
#line 247 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
SetModelMainTexture  (TEXTURE_MARKER );
#line 249 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
m_fSendRange  = ClampDn  (m_fSendRange  , 0.01f);
#line 252 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
SetTimerAfter(0.1f);
Jump(STATE_CURRENT, 0x00cd000e, FALSE, EBegin());return TRUE;}BOOL CTrigger::H0x00cd000e_Main_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00cd000e
switch(__eeInput.ee_slEvent) {case EVENTCODE_EBegin: return TRUE;case EVENTCODE_ETimer: Jump(STATE_CURRENT,0x00cd000f, FALSE, EInternal()); return TRUE;default: return FALSE; }}BOOL CTrigger::H0x00cd000f_Main_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00cd000f
;
#line 255 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
if(m_bActive ){
#line 256 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
Jump(STATE_CURRENT, STATE_CTrigger_Active, TRUE, EVoid());return TRUE;
#line 257 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
}else {
#line 258 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
Jump(STATE_CURRENT, STATE_CTrigger_Inactive, TRUE, EVoid());return TRUE;
#line 259 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
}
#line 262 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
Destroy  ();
#line 264 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
Return(STATE_CURRENT,EVoid());
#line 264 "E:/질산/Korea/Client_USA/EntitiesMP/Trigger.es"
return TRUE; ASSERT(FALSE); return TRUE;};
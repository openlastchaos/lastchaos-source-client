/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#line 2 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"

#include "StdH.h"

#include <EntitiesMP/Switch.h>
#include <EntitiesMP/Switch_tables.h>
CEntityEvent *EFlipTheSwitch::MakeCopy(void) { CEntityEvent *peeCopy = new EFlipTheSwitch(*this); return peeCopy;}
SLONG EFlipTheSwitch::GetSizeOf(void) { return sizeof(*this);}
EFlipTheSwitch::EFlipTheSwitch() : CEntityEvent(EVENTCODE_EFlipTheSwitch) {;
 ClearToDefault(iState);
 ClearToDefault(iUseable);
};
BOOL EFlipTheSwitch::CheckIDs(void) { return TRUE ; }
void CSwitch::SetDefaultProperties(void) {
  m_iModelONAnimation = 0;
  m_iTextureONAnimation = 0;
  m_iModelOFFAnimation = 0;
  m_iTextureOFFAnimation = 0;
  m_penTarget = NULL;
  m_eetEvent = EET_START ;
  m_eetOffEvent = EET_IGNORE ;
  m_penOffTarget = NULL;
  m_swtType = SWT_ONOFF ;
  m_strMessage = "";
  m_bSwitchON = FALSE ;
  m_penCaused = NULL;
  m_bUseable = FALSE ;
  m_bInvisible = FALSE ;
  CModelHolder2::SetDefaultProperties();
}
  
#line 51 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
BOOL CSwitch::HandleEvent(const CEntityEvent & ee) 
#line 52 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
{
#line 53 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
if(ee  . ee_slEvent  == EVENTCODE_EFlipTheSwitch ){
#line 54 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
if(! _pNetwork  -> IsServer  ()){
#line 55 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
EFlipTheSwitch  & efts  = (EFlipTheSwitch  &) ee ;
#line 56 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
m_bUseable  = efts  . iUseable ;
#line 57 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
if(efts  . iState  != m_bSwitchON ){
#line 58 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
if(efts  . iState ){
#line 60 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
GetModelObject  () -> PlayAnim  (m_iModelONAnimation  , 0);
#line 61 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
GetModelObject  () -> mo_toTexture  . PlayAnim  (m_iTextureONAnimation  , 0);
#line 62 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
m_bSwitchON  = TRUE ;
#line 63 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}else {
#line 65 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
GetModelObject  () -> PlayAnim  (m_iModelOFFAnimation  , 0);
#line 66 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
GetModelObject  () -> mo_toTexture  . PlayAnim  (m_iTextureOFFAnimation  , 0);
#line 67 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
m_bSwitchON  = FALSE ;
#line 68 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}
#line 69 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}
#line 70 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}
#line 71 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
return TRUE ;
#line 72 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}
#line 74 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
return CModelHolder2  :: HandleEvent  (ee );
#line 75 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}
  
#line 78 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
CAnimData * CSwitch::GetAnimData(SLONG slPropertyOffset) 
#line 79 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
{
#line 80 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
if(slPropertyOffset  == offsetof  (CSwitch  , m_iModelONAnimation ) || 
#line 81 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
slPropertyOffset  == offsetof  (CSwitch  , m_iModelOFFAnimation )){
#line 82 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
return GetModelObject  () -> GetData  ();
#line 83 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}else if(slPropertyOffset  == offsetof  (CSwitch  , m_iTextureONAnimation ) || 
#line 84 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
slPropertyOffset  == offsetof  (CSwitch  , m_iTextureOFFAnimation )){
#line 85 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
return GetModelObject  () -> mo_toTexture  . GetData  ();
#line 86 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}else {
#line 87 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
return CModelHolder2  :: GetAnimData  (slPropertyOffset );
#line 88 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}
#line 89 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}
  
#line 92 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
BOOL CSwitch::CanReactOnEntity(CEntity * pen) 
#line 93 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
{
#line 94 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
if(pen  == NULL ){
#line 95 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
return FALSE ;
#line 96 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}
#line 98 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
if(! (pen  -> GetFlags  () & ENF_ALIVE )){
#line 99 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
return FALSE ;
#line 100 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}
#line 102 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
return TRUE ;
#line 103 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}
  
#line 107 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
SLONG CSwitch::GetUsedMemory(void) 
#line 108 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
{
#line 110 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
SLONG  slUsedMemory  = sizeof  (CSwitch ) - sizeof  (CModelHolder2 ) + CModelHolder2  :: GetUsedMemory  ();
#line 112 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
slUsedMemory  += m_strMessage  . Length  ();
#line 113 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
return slUsedMemory ;
#line 114 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}
BOOL CSwitch::
#line 122 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
SwitchON(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CSwitch_SwitchON
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CSwitch::SwitchON expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 124 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
if(m_bSwitchON ){
#line 126 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
Return(STATE_CURRENT,EVoid());
#line 126 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
return TRUE;
#line 127 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}
#line 128 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
if(_pNetwork  -> IsServer  ()){
#line 129 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
EFlipTheSwitch  efts ;
#line 130 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
efts  . iState  = TRUE ;
#line 131 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
efts  . iUseable  = m_bUseable ;
#line 132 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
SendEvent  (efts  , TRUE );
#line 133 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}
#line 135 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
GetModelObject  () -> PlayAnim  (m_iModelONAnimation  , 0);
#line 136 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
GetModelObject  () -> mo_toTexture  . PlayAnim  (m_iTextureONAnimation  , 0);
#line 137 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
m_bSwitchON  = TRUE ;
#line 139 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
SendToTarget  (m_penTarget  , m_eetEvent  , m_penCaused );
#line 141 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
SetTimerAfter(GetModelObject  () -> GetAnimLength  (m_iModelONAnimation ));
Jump(STATE_CURRENT, 0x00d10002, FALSE, EBegin());return TRUE;}BOOL CSwitch::H0x00d10002_SwitchON_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00d10002
switch(__eeInput.ee_slEvent){case(EVENTCODE_EBegin):{const EBegin&e= (EBegin&)__eeInput;
return TRUE;}ASSERT(FALSE);break;case(EVENTCODE_ETimer):{const ETimer&e= (ETimer&)__eeInput;
UnsetTimer();Jump(STATE_CURRENT,0x00d10003, FALSE, EInternal());return TRUE;}ASSERT(FALSE);break;case(EVENTCODE_EDeath):{const EDeath&e= (EDeath&)__eeInput;
return FALSE;}ASSERT(FALSE);break;default:{return TRUE;}ASSERT(FALSE);break;
#line 143 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}return TRUE;}BOOL CSwitch::H0x00d10003_SwitchON_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00d10003

#line 145 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
Return(STATE_CURRENT,EReturn  ());
#line 145 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
return TRUE; ASSERT(FALSE); return TRUE;};BOOL CSwitch::
#line 149 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
SwitchOFF(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CSwitch_SwitchOFF
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CSwitch::SwitchOFF expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 151 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
if(! m_bSwitchON ){
#line 153 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
Return(STATE_CURRENT,EVoid());
#line 153 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
return TRUE;
#line 154 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}
#line 155 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
if(_pNetwork  -> IsServer  ()){
#line 156 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
EFlipTheSwitch  efts ;
#line 157 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
efts  . iState  = FALSE ;
#line 158 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
efts  . iUseable  = m_bUseable ;
#line 159 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
SendEvent  (efts  , TRUE );
#line 160 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}
#line 162 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
GetModelObject  () -> PlayAnim  (m_iModelOFFAnimation  , 0);
#line 163 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
GetModelObject  () -> mo_toTexture  . PlayAnim  (m_iTextureOFFAnimation  , 0);
#line 164 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
m_bSwitchON  = FALSE ;
#line 166 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
if(m_penOffTarget  != NULL )
#line 167 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
{
#line 168 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
SendToTarget  (m_penOffTarget  , m_eetOffEvent  , m_penCaused );
#line 169 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}
#line 170 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
else 
#line 171 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
{
#line 173 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
SendToTarget  (m_penTarget  , m_eetOffEvent  , m_penCaused );
#line 174 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}
#line 176 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
SetTimerAfter(GetModelObject  () -> GetAnimLength  (m_iModelOFFAnimation ));
Jump(STATE_CURRENT, 0x00d10005, FALSE, EBegin());return TRUE;}BOOL CSwitch::H0x00d10005_SwitchOFF_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00d10005
switch(__eeInput.ee_slEvent){case(EVENTCODE_EBegin):{const EBegin&e= (EBegin&)__eeInput;
return TRUE;}ASSERT(FALSE);break;case(EVENTCODE_ETimer):{const ETimer&e= (ETimer&)__eeInput;
UnsetTimer();Jump(STATE_CURRENT,0x00d10006, FALSE, EInternal());return TRUE;}ASSERT(FALSE);break;case(EVENTCODE_EDeath):{const EDeath&e= (EDeath&)__eeInput;
return FALSE;}ASSERT(FALSE);break;default:{return TRUE;}ASSERT(FALSE);break;
#line 178 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}return TRUE;}BOOL CSwitch::H0x00d10006_SwitchOFF_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00d10006

#line 180 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
Return(STATE_CURRENT,EReturn  ());
#line 180 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
return TRUE; ASSERT(FALSE); return TRUE;};BOOL CSwitch::
#line 183 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
MainLoop_Once(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CSwitch_MainLoop_Once
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CSwitch::MainLoop_Once expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 184 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
m_bUseable  = TRUE ;
#line 187 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
SetTimerAt(THINKTIME_NEVER);
Jump(STATE_CURRENT, 0x00d10008, FALSE, EBegin());return TRUE;}BOOL CSwitch::H0x00d10008_MainLoop_Once_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00d10008
switch(__eeInput.ee_slEvent){case(EVENTCODE_ETrigger):{const ETrigger&eTrigger= (ETrigger&)__eeInput;

#line 190 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
if(CanReactOnEntity  (eTrigger  . penCaused ) && m_bUseable ){
#line 191 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
m_bUseable  = FALSE ;
#line 192 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
m_penCaused  = eTrigger  . penCaused ;
#line 193 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
Call(STATE_CURRENT, STATE_CSwitch_SwitchON, TRUE, EVoid());return TRUE;
#line 194 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}
#line 195 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}ASSERT(FALSE);break;case(EVENTCODE_EStart):{const EStart&e= (EStart&)__eeInput;

#line 198 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
m_bUseable  = FALSE ;
#line 199 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
Call(STATE_CURRENT, STATE_CSwitch_SwitchON, TRUE, EVoid());return TRUE;
#line 200 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}ASSERT(FALSE);break;case(EVENTCODE_EStop):{const EStop&e= (EStop&)__eeInput;

#line 203 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
m_bUseable  = FALSE ;
#line 204 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
Call(STATE_CURRENT, STATE_CSwitch_SwitchOFF, TRUE, EVoid());return TRUE;
#line 205 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}ASSERT(FALSE);break;case(EVENTCODE_EReturn):{const EReturn&e= (EReturn&)__eeInput;

#line 207 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
m_bUseable  = ! m_bSwitchON ;
#line 208 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
if(_pNetwork  -> IsServer  ()){
#line 209 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
EFlipTheSwitch  efts ;
#line 210 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
efts  . iState  = m_bSwitchON ;
#line 211 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
efts  . iUseable  = m_bUseable ;
#line 212 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
SendEvent  (efts  , TRUE );
#line 213 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}
#line 214 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
return TRUE;
#line 215 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}ASSERT(FALSE);break;default: return FALSE; break;
#line 216 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}return TRUE;}BOOL CSwitch::H0x00d10009_MainLoop_Once_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00d10009
 ASSERT(FALSE); return TRUE;};BOOL CSwitch::
#line 219 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
MainLoop_OnOff(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CSwitch_MainLoop_OnOff
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CSwitch::MainLoop_OnOff expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 220 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
m_bUseable  = TRUE ;
#line 223 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
SetTimerAt(THINKTIME_NEVER);
Jump(STATE_CURRENT, 0x00d1000b, FALSE, EBegin());return TRUE;}BOOL CSwitch::H0x00d1000b_MainLoop_OnOff_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00d1000b
switch(__eeInput.ee_slEvent){case(EVENTCODE_ETrigger):{const ETrigger&eTrigger= (ETrigger&)__eeInput;

#line 226 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
if(CanReactOnEntity  (eTrigger  . penCaused ) && m_bUseable ){
#line 227 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
m_bUseable  = FALSE ;
#line 228 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
m_penCaused  = eTrigger  . penCaused ;
#line 230 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
if(m_bSwitchON ){
#line 231 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
Call(STATE_CURRENT, STATE_CSwitch_SwitchOFF, TRUE, EVoid());return TRUE;
#line 233 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}else {
#line 234 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
Call(STATE_CURRENT, STATE_CSwitch_SwitchON, TRUE, EVoid());return TRUE;
#line 235 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}
#line 236 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}
#line 237 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
return TRUE;
#line 238 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}ASSERT(FALSE);break;case(EVENTCODE_EStart):{const EStart&e= (EStart&)__eeInput;

#line 241 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
m_bUseable  = FALSE ;
#line 242 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
Call(STATE_CURRENT, STATE_CSwitch_SwitchON, TRUE, EVoid());return TRUE;
#line 243 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}ASSERT(FALSE);break;case(EVENTCODE_EStop):{const EStop&e= (EStop&)__eeInput;

#line 246 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
m_bUseable  = FALSE ;
#line 247 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
Call(STATE_CURRENT, STATE_CSwitch_SwitchOFF, TRUE, EVoid());return TRUE;
#line 248 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}ASSERT(FALSE);break;case(EVENTCODE_EDeath):{const EDeath&e= (EDeath&)__eeInput;

#line 251 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
if(m_penDestruction  != NULL ){
#line 252 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
Jump(STATE_CURRENT, STATE_CModelHolder2_Die, FALSE, EVoid());return TRUE;
#line 253 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}
#line 254 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
return TRUE;
#line 255 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}ASSERT(FALSE);break;case(EVENTCODE_EReturn):{const EReturn&e= (EReturn&)__eeInput;

#line 257 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
m_bUseable  = TRUE ;
#line 258 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
if(_pNetwork  -> IsServer  ()){
#line 259 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
EFlipTheSwitch  efts ;
#line 260 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
efts  . iState  = m_bSwitchON ;
#line 261 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
efts  . iUseable  = m_bUseable ;
#line 262 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
SendEvent  (efts  , TRUE );
#line 263 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}
#line 264 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
return TRUE;
#line 265 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}ASSERT(FALSE);break;default: return FALSE; break;
#line 266 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}return TRUE;}BOOL CSwitch::H0x00d1000c_MainLoop_OnOff_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00d1000c
 ASSERT(FALSE); return TRUE;};BOOL CSwitch::
#line 269 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
Main(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CSwitch_Main
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CSwitch::Main expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 271 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
CModelHolder2  :: InitModelHolder  ();
#line 273 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
if(m_bInvisible ){
#line 274 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
SwitchToEditorModel  ();
#line 275 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}
#line 277 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
if(m_swtType  == SWT_ONCE ){
#line 278 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
Jump(STATE_CURRENT, STATE_CSwitch_MainLoop_Once, TRUE, EVoid());return TRUE;
#line 279 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}else {
#line 280 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
Jump(STATE_CURRENT, STATE_CSwitch_MainLoop_OnOff, TRUE, EVoid());return TRUE;
#line 281 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
}
#line 283 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
Return(STATE_CURRENT,EVoid());
#line 283 "E:/질산/Korea/Client_USA/EntitiesMP/Switch.es"
return TRUE; ASSERT(FALSE); return TRUE;};
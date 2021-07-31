/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#line 2 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"

#include "StdH.h"
#include "EntitiesMP/Player.h"
void HUD_SetEntityForStackDisplay(CRationalEntity *pren);
//void HUD_SetSkaDebug(BOOL bSkaDebug);

#include <EntitiesMP/DebugEntityStatesDisplay.h>
#include <EntitiesMP/DebugEntityStatesDisplay_tables.h>
void CEntityStateDisplay::SetDefaultProperties(void) {
  m_strName = "EntityStateDisplay";
  m_penManualTarget = NULL;
  m_eTargetType = DTT_MANUAL ;
  m_bSkaStateDebug = FALSE ;
  CRationalEntity::SetDefaultProperties();
}
   CEntityStateDisplay:: ~ CEntityStateDisplay() 
#line 34 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
{
#line 35 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
HUD_SetEntityForStackDisplay  (NULL );
#line 36 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
}
  
#line 38 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
CPlayer * CEntityStateDisplay::GetPlayer() 
#line 39 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
{
#line 41 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
for(INDEX iPlayer  = 0;iPlayer  < GetMaxPlayers  ();iPlayer  ++){
#line 42 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
CEntity  * pen  = GetPlayerEntity  (iPlayer );
#line 43 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
if(pen  != NULL ){
#line 44 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
if(_pNetwork  -> IsPlayerLocal  (pen )){
#line 45 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
return (CPlayer  *) pen ;
#line 46 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
}
#line 47 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
}
#line 48 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
}
#line 49 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
return NULL ;
#line 50 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
}
  
#line 52 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
void CEntityStateDisplay::ObtainTarget() 
#line 53 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
{
#line 54 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
CEntity  * penTarget  = NULL ;
#line 55 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
switch(m_eTargetType ){
#line 56 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
case DTT_MANUAL : 
#line 57 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
penTarget  = m_penManualTarget ;
#line 58 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
break ;
#line 59 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
case DTT_PLAYER : 
#line 60 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
penTarget  = (CEntity  *) & * GetPlayer  ();
#line 61 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
break ;
#line 62 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
case DTT_PLAYERWEAPONS : 
#line 63 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
if(GetPlayer  () == NULL ){return ;}
#line 64 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
penTarget  = (CEntity  *) & * GetPlayer  () -> GetPlayerWeapons  ();
#line 65 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
break ;
#line 66 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
case DTT_PLAYERANIMATOR : 
#line 67 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
if(GetPlayer  () == NULL ){return ;}
#line 68 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
penTarget  = (CEntity  *) & * GetPlayer  () -> GetPlayerAnimator  ();
#line 69 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
break ;
#line 70 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
}
#line 72 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
if(penTarget  != NULL ){
#line 73 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
HUD_SetEntityForStackDisplay  ((CRationalEntity  *) penTarget );
#line 75 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
}else {
#line 76 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
HUD_SetEntityForStackDisplay  (NULL );
#line 78 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
}
#line 79 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
}
BOOL CEntityStateDisplay::
#line 82 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
Main(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CEntityStateDisplay_Main
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CEntityStateDisplay::Main expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 85 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
InitAsEditorModel  ();
#line 86 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
SetPhysicsFlags  (EPF_MODEL_IMMATERIAL );
#line 87 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
SetCollisionFlags  (ECF_IMMATERIAL );
#line 89 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
SetModel  (MODEL_MARKER );
#line 90 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
SetModelMainTexture  (TEXTURE_MARKER );
#line 92 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
GetModelObject  () -> StretchModel  (FLOAT3D (0.4f , 0.4f , 0.4f));
#line 94 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
if(m_eTargetType  != DTT_MANUAL ){
#line 95 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
m_penManualTarget  = NULL ;
#line 96 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
}
#line 99 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
SetTimerAfter(0.5f);
Jump(STATE_CURRENT, 0x00e90000, FALSE, EBegin());return TRUE;}BOOL CEntityStateDisplay::H0x00e90000_Main_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e90000
switch(__eeInput.ee_slEvent) {case EVENTCODE_EBegin: return TRUE;case EVENTCODE_ETimer: Jump(STATE_CURRENT,0x00e90001, FALSE, EInternal()); return TRUE;default: return FALSE; }}BOOL CEntityStateDisplay::H0x00e90001_Main_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e90001
;
#line 103 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
Jump(STATE_CURRENT,0x00e90004, FALSE, EInternal());return TRUE;}BOOL CEntityStateDisplay::H0x00e90004_Main_05(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e90004
if(!(TRUE )){ Jump(STATE_CURRENT,0x00e90005, FALSE, EInternal());return TRUE;}
#line 104 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
SetTimerAfter(0.2f);
Jump(STATE_CURRENT, 0x00e90002, FALSE, EBegin());return TRUE;}BOOL CEntityStateDisplay::H0x00e90002_Main_03(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e90002
switch(__eeInput.ee_slEvent)
#line 105 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
{case(EVENTCODE_EBegin):{const EBegin&e= (EBegin&)__eeInput;

#line 107 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
ObtainTarget  ();
#line 112 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
return TRUE;
#line 113 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
}ASSERT(FALSE);break;case(EVENTCODE_ETimer):{const ETimer&e= (ETimer&)__eeInput;

#line 115 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
UnsetTimer();Jump(STATE_CURRENT,0x00e90003, FALSE, EInternal());return TRUE;
#line 116 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
}ASSERT(FALSE);break;default: return FALSE; break;
#line 117 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
}return TRUE;}BOOL CEntityStateDisplay::H0x00e90003_Main_04(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e90003
Jump(STATE_CURRENT,0x00e90004, FALSE, EInternal());return TRUE;
#line 118 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
}BOOL CEntityStateDisplay::H0x00e90005_Main_06(const CEntityEvent &__eeInput) {
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e90005

#line 119 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
Return(STATE_CURRENT,EVoid());
#line 119 "E:/질산/Korea/Client_USA/EntitiesMP/DebugEntityStatesDisplay.es"
return TRUE; ASSERT(FALSE); return TRUE;};
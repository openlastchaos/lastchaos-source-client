/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#line 2 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"

#include "StdH.h"
#include "EntitiesMP/MusicHolder.h"
#include "EntitiesMP/Player.h"

#include <PlayerMarker.h>
#include <PlayerMarker_tables.h>
CEntityEvent *ETriggerPlayerMarker::MakeCopy(void) { 	CEntityEvent *peeCopy = new ETriggerPlayerMarker(*this); 	return peeCopy;}
SLONG ETriggerPlayerMarker::GetSizeOf(void) { 	return sizeof(*this);}
ETriggerPlayerMarker::ETriggerPlayerMarker() : CEntityEvent(EVENTCODE_ETriggerPlayerMarker) {
	ClearToDefault(eidCaused);
}
BOOL ETriggerPlayerMarker::CheckIDs(void) {	return TRUE && eidCaused.IsValid(); }

#line 14 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"

  extern void CPlayerWeapons_Precache(ULONG ulAvailable);

void CPlayerMarker::SetDefaultProperties(void) {
	m_fHealth = 100.0f;
	m_fShield = 0.0f;
	m_iGiveWeapons = 0x1;
	m_iTakeWeapons = 0x0;
	m_strGroup = "";
	m_bQuickStart = FALSE ;
	m_bStartInComputer = FALSE ;
	m_bStartInComputerOnce = TRUE ;
	m_penMessage = NULL;
	m_fMaxAmmoRatio = 0.0f;
	m_tmLastSpawned = -1000.0f;
	m_iTakeAmmo = 0x0;
	m_bNoRespawnInPlace = FALSE ;
	CMarker::SetDefaultProperties();
}

#line 43 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
void CPlayerMarker::Precache(void) {
#line 44 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
if(m_iGiveWeapons  > 1){
#line 45 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
CPlayerWeapons_Precache  (m_iGiveWeapons );
#line 46 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
}
#line 47 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
}

#line 48 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
BOOL CPlayerMarker::HandleEvent(const CEntityEvent & ee) {
#line 49 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
if(ee  . ee_slEvent  == EVENTCODE_ETrigger ){
#line 50 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
CEntity  * penMainMusicHolder  = _pNetwork  -> GetEntityWithName  ("MusicHolder" , 0);
#line 51 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
if(penMainMusicHolder  == NULL  || ! IsOfClass  (penMainMusicHolder  , & CMusicHolder_DLLClass )){
#line 52 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
return TRUE ;
#line 53 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
}
#line 54 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
if(_pNetwork  -> IsServer  ()){
#line 55 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
ETriggerPlayerMarker  etpm ;
#line 56 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
etpm  . eidCaused  = ((ETrigger  &) ee ) . penCaused ;
#line 57 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
SendEvent  (ETriggerPlayerMarker  () , TRUE );
#line 58 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
}
#line 59 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
return TRUE ;
#line 60 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
}else if(ee  . ee_slEvent  == EVENTCODE_ETriggerPlayerMarker ){
#line 61 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
CEntity  * penMainMusicHolder  = _pNetwork  -> GetEntityWithName  ("MusicHolder" , 0);
#line 62 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
if(penMainMusicHolder  == NULL  || ! IsOfClass  (penMainMusicHolder  , & CMusicHolder_DLLClass )){
#line 63 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
return TRUE ;
#line 64 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
}
#line 65 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
CMusicHolder  * pmh  = (CMusicHolder  *) penMainMusicHolder ;
#line 67 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
BOOL bNew  = (pmh  -> m_iRespawnMarker  != en_ulID );
#line 68 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
pmh  -> m_iRespawnMarker  = en_ulID ;
#line 71 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
CEntity  * penCaused  = (CEntity  *) (((ETriggerPlayerMarker  &) ee ) . eidCaused );
#line 72 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
if(bNew  && (GetSP  () -> sp_bSinglePlayer  && GetSP  () -> sp_gmGameMode  != CSessionProperties  :: GM_FLYOVER ) 
#line 73 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
&& penCaused  != NULL  && IsOfClass  (penCaused  , & CPlayer_DLLClass )){
#line 75 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
CPlayerSettings  * pps  = (CPlayerSettings  *) (((CPlayerEntity  *) penCaused ) -> en_pcCharacter  . pc_aubAppearance );
#line 82 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
}
#line 83 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
return TRUE ;
#line 84 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
}
#line 86 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
return FALSE ;
#line 87 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
}
BOOL CPlayerMarker::
#line 90 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
Main(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CPlayerMarker_Main
	ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CPlayerMarker::Main expects 'EVoid' as input!");	const EVoid &e = (const EVoid &)__eeInput;
#line 92 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
InitAsEditorModel  ();
#line 93 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
SetPhysicsFlags  (EPF_MODEL_IMMATERIAL );
#line 94 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
SetCollisionFlags  (ECF_IMMATERIAL );
#line 96 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
SetFlagOn  (ENF_MARKDESTROY );
#line 97 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
SetFlagOn  (ENF_NONETCONNECT );
#line 98 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
SetFlagOff  (ENF_PROPSCHANGED );
#line 101 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
SetModel  (MODEL_MARKER );
#line 102 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
SetModelMainTexture  (TEXTURE_MARKER );
#line 105 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
if(m_bQuickStart ){
#line 106 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
m_strName  . PrintF  ("Player Quick Start");
#line 107 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
}else {
#line 108 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
m_strName  . PrintF  ("Player Start - %s" , m_strGroup );
#line 109 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
}
#line 111 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
Return(STATE_CURRENT,EVoid());
#line 111 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerMarker.es"
return TRUE; ASSERT(FALSE); return TRUE;};
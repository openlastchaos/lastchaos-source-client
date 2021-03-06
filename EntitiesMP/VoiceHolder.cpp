/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#line 2 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"

#include "StdH.h"
#include "EntitiesMP\Player.h"
extern INDEX ent_bReportBrokenChains;

// NOTE: GENDER_MALE has to be the same as the one in Player.es!!!!!!!!!!!!
#define GENDER_MALE 0

#include <VoiceHolder.h>
#include <VoiceHolder_tables.h>
void CVoiceHolder::SetDefaultProperties(void) {
	m_strName = "VoiceHolder";
	m_strDescription = "";
	m_fnmMessage = CTString("");
	m_bActive = TRUE ;
	m_ctMaxTrigs = 1;
	CRationalEntity::SetDefaultProperties();
}

#line 31 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
void CVoiceHolder::Precache(void) 
#line 32 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
{
#line 33 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
m_aps  . Precache  (m_fnmMessage );
#line 34 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
}

#line 35 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
const CTString & CVoiceHolder::GetDescription(void)const {
#line 36 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
((CTString &) m_strDescription ) . PrintF  ("%s" , m_fnmMessage  . FileName  ());
#line 37 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
return m_strDescription ;
#line 38 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
}
BOOL CVoiceHolder::
#line 41 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
Main(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CVoiceHolder_Main
	ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CVoiceHolder::Main expects 'EVoid' as input!");	const EVoid &e = (const EVoid &)__eeInput;
#line 43 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
InitAsEditorModel  ();
#line 44 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
SetPhysicsFlags  (EPF_MODEL_IMMATERIAL );
#line 45 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
SetCollisionFlags  (ECF_IMMATERIAL );
#line 48 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
SetModel  (MODEL_MARKER );
#line 49 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
SetModelMainTexture  (TEXTURE_MARKER );
#line 50 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
SetTimerAt(THINKTIME_NEVER);
Jump(STATE_CURRENT, 0x00e70000, FALSE, EBegin());return TRUE;}BOOL CVoiceHolder::H0x00e70000_Main_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e70000
switch(__eeInput.ee_slEvent){case(EVENTCODE_ETrigger):{const ETrigger&eTrigger= (ETrigger&)__eeInput;

#line 52 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
if(! m_bActive ){
#line 53 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
return TRUE;
#line 54 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
}
#line 55 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
CEntity  * penCaused  = FixupCausedToPlayer  (this  , eTrigger  . penCaused );
#line 56 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
ASSERT  (IsOfClass  (penCaused  , & CPlayer_DLLClass ));
#line 57 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
if(IsOfClass  (penCaused  , & CPlayer_DLLClass )){
#line 58 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
CPlayer  * penPlayer  = (CPlayer  *) penCaused ;
#line 60 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
if(penPlayer  -> m_iGender  != GENDER_MALE ){
#line 61 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
return TRUE;
#line 62 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
}
#line 63 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
}
#line 64 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
EVoiceMessage  eMsg ;
#line 65 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
eMsg  . fnmMessage  = m_fnmMessage ;
#line 66 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
penCaused  -> SendEvent  (eMsg );
#line 68 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
if(m_ctMaxTrigs  > 0){
#line 70 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
m_ctMaxTrigs  -= 1;
#line 72 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
if(m_ctMaxTrigs  <= 0){
#line 74 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
Destroy  ();
#line 75 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
UnsetTimer();Jump(STATE_CURRENT,0x00e70001, FALSE, EInternal());return TRUE;
#line 76 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
}
#line 77 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
}
#line 78 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
return TRUE;
#line 79 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
}ASSERT(FALSE);break;case(EVENTCODE_EActivate):{const EActivate&e= (EActivate&)__eeInput;

#line 81 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
m_bActive  = TRUE ;
#line 82 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
return TRUE;
#line 83 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
}ASSERT(FALSE);break;case(EVENTCODE_EDeactivate):{const EDeactivate&e= (EDeactivate&)__eeInput;

#line 85 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
m_bActive  = FALSE ;
#line 86 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
return TRUE;
#line 87 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
}ASSERT(FALSE);break;default: return FALSE; break;
#line 88 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
}return TRUE;}BOOL CVoiceHolder::H0x00e70001_Main_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e70001

#line 89 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
Return(STATE_CURRENT,EVoid());
#line 89 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/VoiceHolder.es"
return TRUE; ASSERT(FALSE); return TRUE;};
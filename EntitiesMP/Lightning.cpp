/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#line 2 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"

#include "StdH.h"
#include "EntitiesMP/BackgroundViewer.h"
#include "EntitiesMP/WorldSettingsController.h"
#include "EntitiesMP/Light.h"

#include <Lightning.h>
#include <Lightning_tables.h>
CEntityEvent *ETriggerLightning::MakeCopy(void) { 	CEntityEvent *peeCopy = new ETriggerLightning(*this); 	return peeCopy;}
SLONG ETriggerLightning::GetSizeOf(void) { 	return sizeof(*this);}
ETriggerLightning::ETriggerLightning() : CEntityEvent(EVENTCODE_ETriggerLightning) {
}
BOOL ETriggerLightning::CheckIDs(void) {	return TRUE ; }

#line 12 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"

  struct ThunderInfo
  {
    INDEX ti_iSound;
    FLOAT ti_fThunderStrikeDelay;
  };

  struct ThunderInfo _atiThunderSounds[3] =
  {
    { SOUND_THUNDER1, 0.6f},
    { SOUND_THUNDER2, 0.0f},
    { SOUND_THUNDER3, 0.0f},
  };

void CLightning::SetDefaultProperties(void) {
	m_penTarget = NULL;
	m_penwsc = NULL;
	m_strName = "Lightning";
	m_tmLightningStart = -1.0f;
	m_soThunder.SetOwner(this);
	m_soThunder.Stop(FALSE);
	m_bBackground = FALSE ;
	m_penLight = NULL;
	m_iLightAnim = 0;
	m_iSoundPlaying = 0;
	m_fLightningPower = 1.0f;
	m_fSoundDelay = 0.0f;
	CMovableModelEntity::SetDefaultProperties();
}

#line 53 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
void CLightning::Precache(void) 
#line 54 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
{
#line 55 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
CMovableModelEntity  :: Precache  ();
#line 56 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
PrecacheSound  (SOUND_THUNDER1 );
#line 57 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
PrecacheSound  (SOUND_THUNDER2 );
#line 58 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
PrecacheSound  (SOUND_THUNDER3 );
#line 59 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
}

#line 62 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
CAnimData * CLightning::GetAnimData(SLONG slPropertyOffset) 
#line 63 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
{
#line 64 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
if(m_penLight  == NULL ){
#line 65 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
return NULL ;
#line 66 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
}
#line 69 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
if(IsOfClass  (m_penLight  , & CLight_DLLClass )){
#line 70 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
CLight  * penLight  = (CLight  *) & * m_penLight ;
#line 71 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
if(slPropertyOffset  == offsetof  (CLightning  , m_iLightAnim )){
#line 72 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
return penLight  -> m_aoLightAnimation  . GetData  ();
#line 73 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
}
#line 74 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
}else {
#line 75 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
WarningMessage  ("Target '%s' is not of light class!" , m_penLight  -> GetName  ());
#line 76 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
}
#line 77 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
return NULL ;
#line 78 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
}

#line 80 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
void CLightning::RenderParticles(void) 
#line 81 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
{
#line 82 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
if(m_penTarget  == NULL ){return ;};
#line 84 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
TIME  tmNow  = _pTimer  -> GetLerpedCurrentTick  ();
#line 86 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
if(m_tmLightningStart  > 0.0f && 
#line 87 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
((tmNow  - m_tmLightningStart ) > 0.0f) && 
#line 88 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
((tmNow  - m_tmLightningStart ) < 1.5f))
#line 89 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
{
#line 91 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
FLOAT3D vSrc  = GetPlacement  () . pl_PositionVector ;
#line 92 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
FLOAT3D vDst  = m_penTarget  -> GetPlacement  () . pl_PositionVector ;
#line 93 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
Particles_Lightning  (vSrc  , vDst  , m_tmLightningStart );
#line 94 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
}
#line 95 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
}
BOOL CLightning::
#line 98 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
Main(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CLightning_Main
	ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CLightning::Main expects 'EVoid' as input!");	const EVoid &e = (const EVoid &)__eeInput;
#line 101 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
InitAsEditorModel  ();
#line 102 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
SetPhysicsFlags  (EPF_MODEL_IMMATERIAL );
#line 103 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
SetCollisionFlags  (ECF_IMMATERIAL );
#line 104 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
SetFlagOn  (ENF_CLIENTHANDLING );
#line 107 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
SetModel  (MODEL_TELEPORT );
#line 108 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
SetModelMainTexture  (TEXTURE_TELEPORT );
#line 111 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
if(m_bBackground )
#line 112 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
{
#line 113 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
SetFlags  (GetFlags  () | ENF_BACKGROUND );
#line 114 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
}
#line 115 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
else 
#line 116 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
{
#line 117 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
SetFlags  (GetFlags  () & ~ ENF_BACKGROUND );
#line 118 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
}
#line 121 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
CBackgroundViewer  * penBcgViewer  = (CBackgroundViewer  *) GetWorld  () -> GetBackgroundViewer  ();
#line 122 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
if(penBcgViewer  == NULL )
#line 123 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
{
#line 125 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
Return(STATE_CURRENT,EVoid());
#line 125 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
return TRUE;
#line 126 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
}
#line 129 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
m_penwsc  = penBcgViewer  -> m_penWorldSettingsController ;
#line 130 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
if(m_penwsc  == NULL )
#line 131 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
{
#line 133 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
Return(STATE_CURRENT,EVoid());
#line 133 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
return TRUE;
#line 134 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
}
#line 137 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
if(! IsOfClass  (m_penwsc  , & CWorldSettingsController_DLLClass ))
#line 138 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
{
#line 140 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
Return(STATE_CURRENT,EVoid());
#line 140 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
return TRUE;
#line 141 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
}
#line 144 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
if((m_penTarget  == NULL ) || (! IsOfClass  (m_penTarget  , "Marker")))
#line 145 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
{
#line 146 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
if(m_penTarget  != NULL )
#line 147 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
{
#line 148 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
WarningMessage  ("Target '%s' is not of Marker class!" , m_penTarget  -> GetName  ());
#line 149 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
}
#line 151 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
Return(STATE_CURRENT,EVoid());
#line 151 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
return TRUE;
#line 152 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
}
#line 155 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
FLOAT3D vDirection  = 
#line 156 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
(m_penTarget  -> GetPlacement  () . pl_PositionVector  - 
#line 157 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
GetPlacement  () . pl_PositionVector );
#line 159 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
FLOAT3D vStretch  = vDirection ;
#line 160 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
vStretch  (1) = 1.0f;
#line 161 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
vStretch  (2) = 1.0f;
#line 162 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
vStretch  (3) = - vDirection  . Length  ();
#line 165 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
CPlacement3D pl  = GetPlacement  ();
#line 166 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
DirectionVectorToAngles  (vDirection  . Normalize  () , pl  . pl_OrientationAngle );
#line 167 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
SetPlacement  (pl );
#line 169 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
GetModelObject  () -> StretchModel  (vStretch );
#line 170 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
ModelChangeNotify  ();
#line 173 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
m_fLightningPower  = Clamp  (m_fLightningPower  , 0.0f , 1.0f);
#line 176 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
SetTimerAfter(0.1f);
Jump(STATE_CURRENT, 0x025f0001, FALSE, EBegin());return TRUE;}BOOL CLightning::H0x025f0001_Main_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x025f0001
switch(__eeInput.ee_slEvent) {case EVENTCODE_EBegin: return TRUE;case EVENTCODE_ETimer: Jump(STATE_CURRENT,0x025f0002, FALSE, EInternal()); return TRUE;default: return FALSE; }}BOOL CLightning::H0x025f0002_Main_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x025f0002
;
#line 178 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
Jump(STATE_CURRENT,0x025f0005, FALSE, EInternal());return TRUE;}BOOL CLightning::H0x025f0005_Main_05(const CEntityEvent &__eeInput)
#line 179 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
{
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x025f0005
if(!(TRUE )){ Jump(STATE_CURRENT,0x025f0006, FALSE, EInternal());return TRUE;}
#line 180 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
SetTimerAt(THINKTIME_NEVER);
Jump(STATE_CURRENT, 0x025f0003, FALSE, EBegin());return TRUE;}BOOL CLightning::H0x025f0003_Main_03(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x025f0003
switch(__eeInput.ee_slEvent)
#line 181 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
{case(EVENTCODE_EBegin):{const EBegin&e= (EBegin&)__eeInput;
return TRUE;}ASSERT(FALSE);break;case(EVENTCODE_ETrigger):{const ETrigger&eTrigger= (ETrigger&)__eeInput;

#line 184 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
if(_pNetwork  -> IsServer  ()){
#line 185 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
SendEvent  (ETriggerLightning  () , TRUE );
#line 186 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
}
#line 187 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
return TRUE;
#line 188 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
}ASSERT(FALSE);break;case(EVENTCODE_ETriggerLightning):{const ETriggerLightning&e= (ETriggerLightning&)__eeInput;

#line 190 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
Call(STATE_CURRENT, STATE_CLightning_LightningStike, TRUE, EVoid());return TRUE;
#line 191 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
return TRUE;
#line 192 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
}ASSERT(FALSE);break;default:
#line 194 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
{
#line 195 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
return TRUE;
#line 196 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
}ASSERT(FALSE);break;
#line 197 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
}return TRUE;}BOOL CLightning::H0x025f0004_Main_04(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x025f0004
;Jump(STATE_CURRENT,0x025f0005, FALSE, EInternal());return TRUE;
#line 198 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
}BOOL CLightning::H0x025f0006_Main_06(const CEntityEvent &__eeInput) {
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x025f0006
 ASSERT(FALSE); return TRUE;};BOOL CLightning::
#line 201 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
LightningStike(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CLightning_LightningStike
	ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CLightning::LightningStike expects 'EVoid' as input!");	const EVoid &e = (const EVoid &)__eeInput;
#line 204 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
m_iSoundPlaying  = 1 + IRnd  () % (ARRAYCOUNT  (_atiThunderSounds ) - 1);
#line 205 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
if(m_fSoundDelay  != 0)
#line 206 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
{
#line 207 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
m_iSoundPlaying  = 0;
#line 208 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
}
#line 209 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
m_soThunder  . SetVolume  (m_fLightningPower );
#line 210 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
m_soThunder  . SetPitch  (Lerp  (0.9f , 1.2f , FRnd  ()));
#line 212 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
if(m_fSoundDelay  == 0.0f)
#line 213 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
{
#line 215 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
PlaySound  (m_soThunder  , _atiThunderSounds  [ m_iSoundPlaying  ] . ti_iSound  , 0);
#line 216 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
}
#line 219 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
if(!(_atiThunderSounds  [ m_iSoundPlaying  ] . ti_fThunderStrikeDelay  > 0.0f)){ Jump(STATE_CURRENT,0x025f000a, FALSE, EInternal());return TRUE;}
#line 220 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
SetTimerAfter(_atiThunderSounds  [ m_iSoundPlaying  ] . ti_fThunderStrikeDelay );
Jump(STATE_CURRENT, 0x025f0008, FALSE, EBegin());return TRUE;}BOOL CLightning::H0x025f0008_LightningStike_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x025f0008
switch(__eeInput.ee_slEvent) {case EVENTCODE_EBegin: return TRUE;case EVENTCODE_ETimer: Jump(STATE_CURRENT,0x025f0009, FALSE, EInternal()); return TRUE;default: return FALSE; }}BOOL CLightning::H0x025f0009_LightningStike_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x025f0009
;Jump(STATE_CURRENT,0x025f000a, FALSE, EInternal());return TRUE;}BOOL CLightning::H0x025f000a_LightningStike_03(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x025f000a

#line 224 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
TIME  tmNow  = _pTimer  -> CurrentTick  ();
#line 225 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
m_tmLightningStart  = tmNow ;
#line 227 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
((CWorldSettingsController  *) & * m_penwsc ) -> m_tmLightningStart  = tmNow ;
#line 229 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
((CWorldSettingsController  *) & * m_penwsc ) -> m_fLightningPower  = m_fLightningPower ;
#line 232 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
if(m_penLight  != NULL )
#line 233 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
{
#line 234 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
EChangeAnim  eChange ;
#line 235 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
eChange  . iLightAnim  = m_iLightAnim ;
#line 236 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
eChange  . bLightLoop  = FALSE ;
#line 237 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
m_penLight  -> SendEvent  (eChange );
#line 238 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
}
#line 240 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
if(!(m_fSoundDelay  != 0.0f)){ Jump(STATE_CURRENT,0x025f000d, FALSE, EInternal());return TRUE;}
#line 243 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
SetTimerAfter(m_fSoundDelay );
Jump(STATE_CURRENT, 0x025f000b, FALSE, EBegin());return TRUE;}BOOL CLightning::H0x025f000b_LightningStike_04(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x025f000b
switch(__eeInput.ee_slEvent) {case EVENTCODE_EBegin: return TRUE;case EVENTCODE_ETimer: Jump(STATE_CURRENT,0x025f000c, FALSE, EInternal()); return TRUE;default: return FALSE; }}BOOL CLightning::H0x025f000c_LightningStike_05(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x025f000c
;
#line 245 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
PlaySound  (m_soThunder  , _atiThunderSounds  [ m_iSoundPlaying  ] . ti_iSound  , 0);Jump(STATE_CURRENT,0x025f000d, FALSE, EInternal());return TRUE;}BOOL CLightning::H0x025f000d_LightningStike_06(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x025f000d

#line 248 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
TIME  tmDelay  = GetSoundLength  (_atiThunderSounds  [ m_iSoundPlaying  ] . ti_iSound ) - 
#line 249 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
_atiThunderSounds  [ m_iSoundPlaying  ] . ti_fThunderStrikeDelay ;
#line 251 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
if(!(tmDelay  > 0.0f)){ Jump(STATE_CURRENT,0x025f0010, FALSE, EInternal());return TRUE;}
#line 253 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
SetTimerAfter(GetSoundLength  (_atiThunderSounds  [ m_iSoundPlaying  ] . ti_iSound ) - 
#line 254 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
_atiThunderSounds  [ m_iSoundPlaying  ] . ti_fThunderStrikeDelay );
Jump(STATE_CURRENT, 0x025f000e, FALSE, EBegin());return TRUE;}BOOL CLightning::H0x025f000e_LightningStike_07(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x025f000e
switch(__eeInput.ee_slEvent)
#line 255 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
{case(EVENTCODE_ETimer):
#line 257 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
{const ETimer&e= (ETimer&)__eeInput;

#line 258 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
UnsetTimer();Jump(STATE_CURRENT,0x025f000f, FALSE, EInternal());return TRUE;
#line 259 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
}ASSERT(FALSE);break;default:
#line 261 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
{
#line 262 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
return TRUE;
#line 263 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
}ASSERT(FALSE);break;
#line 264 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
}return TRUE;}BOOL CLightning::H0x025f000f_LightningStike_08(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x025f000f
Jump(STATE_CURRENT,0x025f0010, FALSE, EInternal());return TRUE;}BOOL CLightning::H0x025f0010_LightningStike_09(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x025f0010

#line 267 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
Return(STATE_CURRENT,EBegin  ());
#line 267 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Lightning.es"
return TRUE; ASSERT(FALSE); return TRUE;};
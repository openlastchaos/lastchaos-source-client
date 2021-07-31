/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#line 2 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"

#include "StdH.h"

#include <EntitiesMP/DebrisSka.h>
#include <EntitiesMP/DebrisSka_tables.h>
CEntityEvent *ESpawnDebrisSka::MakeCopy(void) { CEntityEvent *peeCopy = new ESpawnDebrisSka(*this); return peeCopy;}
SLONG ESpawnDebrisSka::GetSizeOf(void) { return sizeof(*this);}
ESpawnDebrisSka::ESpawnDebrisSka() : CEntityEvent(EVENTCODE_ESpawnDebrisSka) {;
 ClearToDefault(penSpawner);
 ClearToDefault(iSkaModelComponentID);
 ClearToDefault(Eeibt);
 ClearToDefault(fSize);
 ClearToDefault(dptParticles);
 ClearToDefault(betStain);
 ClearToDefault(colDebris);
 ClearToDefault(bCustomShading);
 ClearToDefault(aShadingDirection);
 ClearToDefault(colCustomAmbient);
 ClearToDefault(colCustomDiffuse);
 ClearToDefault(bImmaterialASAP);
 ClearToDefault(fDustStretch);
 ClearToDefault(vStretch);
 ClearToDefault(penFallFXPapa);
 ClearToDefault(bFromModelHolder3);
 ClearToDefault(penModelHolder3);
};
BOOL ESpawnDebrisSka::CheckIDs(void) { return TRUE ; }
#line 30 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"


void CDebrisSka::SetDefaultProperties(void) {
  m_dptParticles = DPT_NONE ;
  m_iBodyType = 0;
  m_bFade = FALSE ;
  m_fFadeStartTime = 0.0f;
  m_fFadeTime = 0.0f;
  m_fLastStainHitPoint = FLOAT3D(0 , 0 , 0);
  m_betStain = BET_NONE ;
  m_ctLeftStains = 0;
  m_tmStarted = 0.0f;
  m_fStretch = 1.0f;
  m_aShadingDirection = ANGLE3D(0 , 0 , 0);
  m_bCustomShading = FALSE ;
  m_colCustomAmbient = COLOR(C_WHITE  | CT_OPAQUE );
  m_colCustomDiffuse = COLOR(C_WHITE  | CT_OPAQUE );
  m_bImmaterialASAP = FALSE ;
  m_fDustStretch = 0.0f;
  m_bTouchedGround = FALSE ;
  m_penFallFXPapa = NULL;
  CMovableModelEntity::SetDefaultProperties();
}
  
#line 68 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
void * CDebrisSka::GetEntityInfo(void) {
#line 69 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
return GetStdEntityInfo  ((EntityInfoBodyType ) m_iBodyType );
#line 70 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}
  
#line 73 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
void CDebrisSka::ReceiveDamage(CEntity * penInflictor,enum DamageType dmtType,
#line 74 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
FLOAT fDamageAmmount,const FLOAT3D & vHitPoint,const FLOAT3D & vDirection) 
#line 75 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
{
#line 77 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
if((_pTimer  -> CurrentTick  () - m_tmStarted  < 1.0f) 
#line 78 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
&& (_pTimer  -> CurrentTick  () - m_tmStarted  < 5.0f)){
#line 79 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
return ;
#line 80 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}
#line 81 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
CMovableModelEntity  :: ReceiveDamage  (penInflictor  , dmtType  , fDamageAmmount  , vHitPoint  , vDirection );
#line 82 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}
  
#line 88 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
BOOL CDebrisSka::AdjustShadingParameters(FLOAT3D & vLightDirection,COLOR & colLight,COLOR & colAmbient) 
#line 89 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
{
#line 90 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
if(m_bCustomShading )
#line 91 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
{
#line 92 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
colLight  = m_colCustomDiffuse ;
#line 93 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
colAmbient  = m_colCustomAmbient ;
#line 94 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
AnglesToDirectionVector  (m_aShadingDirection  , vLightDirection );
#line 95 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
vLightDirection  = - vLightDirection ;
#line 96 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}
#line 98 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
if(m_bFade ){
#line 99 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
FLOAT fTimeRemain  = m_fFadeStartTime  + m_fFadeTime  - _pTimer  -> CurrentTick  ();
#line 100 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
if(fTimeRemain  < 0.0f){fTimeRemain  = 0.0f;}
#line 102 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
COLOR colAlpha  = GetModelInstance  () -> mi_colModelColor ;
#line 103 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
colAlpha  = (colAlpha  & 0xffffff00) + (COLOR (fTimeRemain  / m_fFadeTime  * 0xff) & 0xff);
#line 105 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
GetModelInstance  () -> mi_colModelColor  = colAlpha ;
#line 106 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}
#line 108 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
return FALSE ;
#line 109 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}
  
#line 118 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
void CDebrisSka::LeaveStain(void) 
#line 119 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
{
#line 121 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
if(m_betStain  == BET_NONE ){
#line 123 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
return ;
#line 124 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}
#line 127 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
if(m_ctLeftStains  > 5){
#line 128 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
return ;
#line 129 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}
#line 130 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
ESpawnEffect  ese ;
#line 131 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
FLOAT3D vPoint ;
#line 132 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
FLOATplane3D plPlaneNormal ;
#line 133 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
FLOAT fDistanceToEdge ;
#line 136 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
if(GetNearestPolygon  (vPoint  , plPlaneNormal  , fDistanceToEdge )){
#line 138 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
if((m_fLastStainHitPoint  - vPoint ) . Length  () > 3.0f && 
#line 139 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
(vPoint  - GetPlacement  () . pl_PositionVector ) . Length  () < 3.5f){
#line 140 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
m_fLastStainHitPoint  = vPoint ;
#line 142 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
ese  . colMuliplier  = C_WHITE  | CT_OPAQUE ;
#line 143 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
ese  . betType  = m_betStain ;
#line 144 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
ese  . vNormal  = FLOAT3D (plPlaneNormal );
#line 145 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
GetNormalComponent  (en_vCurrentTranslationAbsolute  , plPlaneNormal  , ese  . vDirection );
#line 146 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
FLOAT fLength  = ese  . vDirection  . Length  () / 7.5f;
#line 147 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
fLength  = Clamp  (fLength  , 1.0f , 15.0f);
#line 148 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
ese  . vStretch  = FLOAT3D (1.0f , fLength  * 1.0f , 1.0f);
#line 149 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
SpawnEffect  (CPlacement3D (vPoint  + ese  . vNormal  / 50.0f * (FRnd  () + 0.5f) , ANGLE3D (0 , 0 , 0)) , ese );
#line 150 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
m_ctLeftStains  ++;
#line 151 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}
#line 152 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}
#line 153 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}
  
#line 156 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
void CDebrisSka::SpawnEffect(const CPlacement3D & plEffect,const class ESpawnEffect & eSpawnEffect) 
#line 157 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
{
#line 158 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
CEntityPointer penEffect  = CreateEntity  (plEffect  , CLASS_BASIC_EFFECT  , WLD_AUTO_ENTITY_ID  , FALSE );
#line 159 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
penEffect  -> Initialize  (eSpawnEffect  , FALSE );
#line 160 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}
  
#line 163 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
void CDebrisSka::RenderParticles(void) 
#line 164 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
{
#line 166 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
if(en_vCurrentTranslationAbsolute  . Length  () < 0.1f){
#line 168 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
return ;
#line 169 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}
#line 170 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
switch(m_dptParticles ){
#line 171 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
case DPT_BLOODTRAIL : {
#line 172 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
Particles_BloodTrail  (this );
#line 173 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}break ;
#line 174 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
case DPR_SMOKETRAIL : {
#line 175 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
Particles_GrenadeTrail  (this );
#line 176 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}break ;
#line 177 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
case DPR_SPARKS : {
#line 178 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
Particles_ColoredStarsTrail  (this );
#line 179 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}break ;
#line 180 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
case DPR_FLYINGTRAIL : {
#line 182 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
Particles_BombTrail  (this );
#line 183 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}break ;
#line 184 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
case DPT_AFTERBURNER : {
#line 185 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
Particles_AfterBurner  (this  , m_tmStarted  , 0.5f);
#line 186 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}break ;
#line 187 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
default  : ASSERT  (FALSE );
#line 188 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
case DPT_NONE : 
#line 189 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
return ;
#line 190 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}
#line 191 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}
  
#line 194 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
void CDebrisSka::Explode(void) 
#line 195 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
{
#line 197 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
CPlacement3D plExplosion  = GetPlacement  ();
#line 198 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
CEntityPointer penExplosion  = CreateEntity  (plExplosion  , CLASS_BASIC_EFFECT );
#line 199 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
ESpawnEffect  eSpawnEffect ;
#line 200 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
eSpawnEffect  . colMuliplier  = C_WHITE  | CT_OPAQUE ;
#line 201 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
eSpawnEffect  . betType  = BET_BOMB ;
#line 202 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
eSpawnEffect  . vStretch  = FLOAT3D (0.3f , 0.3f , 0.3f);
#line 203 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
penExplosion  -> Initialize  (eSpawnEffect );
#line 204 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}
BOOL CDebrisSka::
#line 212 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
Main(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CDebrisSka_Main
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_ESpawnDebrisSka, "CDebrisSka::Main expects 'ESpawnDebrisSka' as input!");  const ESpawnDebrisSka &eSpawn = (const ESpawnDebrisSka &)__eeInput;
#line 214 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
InitAsSkaModel  ();
#line 215 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
SetPhysicsFlags  (EPF_MODEL_BOUNCING  | EPF_CANFADESPINNING );
#line 216 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
SetCollisionFlags  (ECF_DEBRIS );
#line 217 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
SetFlags  (GetFlags  () | ENF_SEETHROUGH );
#line 218 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
SetFlagOn  (ENF_CLIENTHANDLING );
#line 219 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
SetFlagOff  (ENF_PROPSCHANGED );
#line 220 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
SetHealth  (25.0f);
#line 221 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
en_fBounceDampNormal  = 0.15f;
#line 222 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
en_fBounceDampParallel  = 0.5f;
#line 223 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
en_fJumpControlMultiplier  = 0.0f;
#line 225 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
if(! _pNetwork  -> IsServer  ()){
#line 226 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
CPlacement3D plSpeed ;
#line 227 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
plSpeed  . pl_PositionVector  = en_vCurrentTranslationAbsolute ;
#line 228 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
plSpeed  . pl_OrientationAngle  = en_aCurrentRotationAbsolute ;
#line 229 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
AdjustDeadReckoning  (en_plPlacement  , plSpeed  , _pTimer  -> CurrentTick  ());
#line 230 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}
#line 233 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
if(eSpawn  . Eeibt  == EIBT_ICE ){
#line 234 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
en_fDensity  = 500.0f;
#line 235 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}else if(eSpawn  . Eeibt  == EIBT_WOOD ){
#line 236 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
en_fDensity  = 500.0f;
#line 237 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}else if(eSpawn  . Eeibt  == EIBT_FLESH ){
#line 238 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
en_fDensity  = 5000.0f;
#line 239 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
en_fBounceDampNormal  = 0.25f;
#line 240 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
en_fBounceDampParallel  = 0.75f;
#line 241 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}else if(TRUE ){
#line 242 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
en_fDensity  = 5000.0f;
#line 243 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}
#line 246 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
m_dptParticles  = eSpawn  . dptParticles ;
#line 247 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
if(m_dptParticles  == DPT_AFTERBURNER )
#line 248 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
{
#line 249 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
Particles_AfterBurner_Prepare  (this );
#line 250 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}
#line 251 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
m_betStain  = eSpawn  . betStain ;
#line 252 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
m_iBodyType  = (INDEX) eSpawn  . Eeibt ;
#line 254 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
CModelInstance * pmiComponent  = NULL ;
#line 255 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
if(! eSpawn  . bFromModelHolder3 ){
#line 256 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
pmiComponent  = eSpawn  . penSpawner  -> GetModelInstanceForComponent  (eSpawn  . iSkaModelComponentID );
#line 257 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}else {
#line 258 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
if(eSpawn  . penModelHolder3  != NULL  && IsOfClass  (eSpawn  . penModelHolder3  , "ModelHolder")){
#line 259 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
pmiComponent  = (eSpawn  . penModelHolder3 ) -> GetModelInstance  ();
#line 260 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}else {
#line 261 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
CPrintF  ("Error spawning SKA debris from MH3 template:\n");
#line 262 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
CPrintF  ("  =>could not acquire ModelInstance (Is Template a valid ModelHolder3?)\n");
#line 263 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}
#line 264 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}
#line 266 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
if(pmiComponent  == NULL ){
#line 267 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
Destroy  ();
#line 268 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
Return(STATE_CURRENT,EVoid());
#line 268 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
return TRUE;
#line 269 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}
#line 271 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
ASSERT  (pmiComponent  != NULL );
#line 272 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
if(en_pmiModelInstance  == NULL ){
#line 273 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
en_pmiModelInstance  = CreateModelInstance  ("");
#line 274 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}
#line 275 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
en_pmiModelInstance  -> Copy  (* pmiComponent );
#line 278 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
GetModelInstance  () -> mi_vStretch  = FLOAT3D (eSpawn  . fSize  , eSpawn  . fSize  , eSpawn  . fSize );
#line 279 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
GetModelInstance  () -> mi_vStretch  (1) *= eSpawn  . vStretch  (1);
#line 280 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
GetModelInstance  () -> mi_vStretch  (2) *= eSpawn  . vStretch  (2);
#line 281 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
GetModelInstance  () -> mi_vStretch  (3) *= eSpawn  . vStretch  (3);
#line 284 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
GetModelInstance  () -> mi_colModelColor  = eSpawn  . colDebris  | CT_OPAQUE ;
#line 285 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
m_bCustomShading  = eSpawn  . bCustomShading ;
#line 286 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
if(m_bCustomShading )
#line 287 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
{
#line 288 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
en_ulFlags  |= ENF_NOSHADINGINFO ;
#line 289 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}
#line 290 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
m_aShadingDirection  = eSpawn  . aShadingDirection ;
#line 291 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
m_colCustomAmbient  = eSpawn  . colCustomAmbient ;
#line 292 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
m_colCustomDiffuse  = eSpawn  . colCustomDiffuse ;
#line 293 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
m_bImmaterialASAP  = eSpawn  . bImmaterialASAP ;
#line 294 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
m_fDustStretch  = eSpawn  . fDustStretch ;
#line 295 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
m_penFallFXPapa  = eSpawn  . penFallFXPapa ;
#line 297 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
ModelChangeNotify  ();
#line 298 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
FLOATaabbox3D box ;
#line 299 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
GetBoundingBox  (box );
#line 300 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
FLOAT fEntitySize  = box  . Size  () . MaxNorm  ();
#line 301 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
if(fEntitySize  > 0.5f){
#line 302 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
SetCollisionFlags  (ECF_MODEL );
#line 303 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}
#line 304 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
en_fCollisionSpeedLimit  += ClampDn  (0.0f , fEntitySize  * 10.0f);
#line 305 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
m_bFade  = FALSE ;
#line 306 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
m_fLastStainHitPoint  = FLOAT3D (32000.0f , 32000.0f , 32000.0f);
#line 307 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
m_ctLeftStains  = 0;
#line 308 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
m_tmStarted  = _pTimer  -> CurrentTick  ();
#line 309 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
m_bTouchedGround  = FALSE ;
#line 312 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
FLOAT fWaitBeforeFade  = FRnd  () * 2.0f + 3.0f;
#line 313 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
SetTimerAfter(fWaitBeforeFade );
Jump(STATE_CURRENT, 0x02690001, FALSE, EBegin());return TRUE;}BOOL CDebrisSka::H0x02690001_Main_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x02690001
switch(__eeInput.ee_slEvent)
#line 314 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
{case(EVENTCODE_EBegin):{const EBegin&e= (EBegin&)__eeInput;
return TRUE;}ASSERT(FALSE);break;case(EVENTCODE_ETouch):{const ETouch&etouch= (ETouch&)__eeInput;

#line 319 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
if(etouch  . penOther  -> GetRenderType  () == RT_BRUSH )
#line 320 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
{
#line 322 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
if(m_penFallFXPapa  != NULL  && ! m_bTouchedGround )
#line 323 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
{
#line 325 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
FOREACHINLIST  (CEntity  , en_lnInParent  , m_penFallFXPapa  -> en_lhChildren  , iten )
#line 326 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
{
#line 328 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
CEntity  * penNew  = GetWorld  () -> CopyEntityInWorld  (* iten  , GetPlacement  ());
#line 329 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
penNew  -> SetParent  (NULL );
#line 330 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
if(IsOfClass  (& * penNew  , "SoundHolder")){
#line 331 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
penNew  -> SendEvent  (EStart  ());
#line 332 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}else {
#line 333 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
penNew  -> SendEvent  (ETrigger  ());
#line 334 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}
#line 335 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}
#line 336 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}
#line 338 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
if(m_fDustStretch  > 0 && ! m_bTouchedGround )
#line 339 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
{
#line 341 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
CPlacement3D plDust  = GetPlacement  ();
#line 342 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
plDust  . pl_PositionVector  = plDust  . pl_PositionVector  + FLOAT3D (0 , m_fDustStretch  * 0.25f , 0);
#line 344 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
ESpawnEffect  ese ;
#line 345 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
ese  . colMuliplier  = C_WHITE  | CT_OPAQUE ;
#line 346 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
ese  . vStretch  = FLOAT3D (m_fDustStretch  , m_fDustStretch  , m_fDustStretch );
#line 347 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
ese  . vNormal  = FLOAT3D (0 , 1 , 0);
#line 348 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
ese  . betType  = BET_DUST_FALL ;
#line 349 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
CEntityPointer penFX  = CreateEntity  (plDust  , CLASS_BASIC_EFFECT );
#line 350 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
penFX  -> Initialize  (ese );
#line 351 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}
#line 352 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
m_bTouchedGround  = TRUE ;
#line 355 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
LeaveStain  ();
#line 357 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
if(m_iBodyType  == EIBT_ROBOT )
#line 358 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
{
#line 360 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
Explode  ();
#line 361 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
SendEvent  (EDeath  ());
#line 362 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
return TRUE;
#line 363 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}
#line 364 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}
#line 365 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
if(m_bImmaterialASAP )
#line 366 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
{
#line 367 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
SetCollisionFlags  (ECF_DEBRIS );
#line 368 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}
#line 369 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
return TRUE;
#line 370 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}ASSERT(FALSE);break;case(EVENTCODE_EDeath):{const EDeath&e= (EDeath&)__eeInput;
Destroy  (FALSE );Return(STATE_CURRENT,EVoid());return TRUE;}ASSERT(FALSE);break;case(EVENTCODE_ETimer):{const ETimer&e= (ETimer&)__eeInput;
UnsetTimer();Jump(STATE_CURRENT,0x02690002, FALSE, EInternal());return TRUE;}ASSERT(FALSE);break;default: return FALSE; break;
#line 373 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
}return TRUE;}BOOL CDebrisSka::H0x02690002_Main_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x02690002

#line 376 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
SetCollisionFlags  (ECF_DEBRIS );
#line 377 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
m_fFadeStartTime  = _pTimer  -> CurrentTick  ();
#line 378 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
m_fFadeTime  = 5.0f;
#line 379 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
m_bFade  = TRUE ;
#line 380 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
SetTimerAfter(m_fFadeTime );
Jump(STATE_CURRENT, 0x02690003, FALSE, EBegin());return TRUE;}BOOL CDebrisSka::H0x02690003_Main_03(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x02690003
switch(__eeInput.ee_slEvent) {case EVENTCODE_EBegin: return TRUE;case EVENTCODE_ETimer: Jump(STATE_CURRENT,0x02690004, FALSE, EInternal()); return TRUE;default: return FALSE; }}BOOL CDebrisSka::H0x02690004_Main_04(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x02690004
;
#line 383 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
Destroy  (FALSE );
#line 385 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
Return(STATE_CURRENT,EVoid());
#line 385 "E:/질산/Korea/Client_USA/EntitiesMP/DebrisSka.es"
return TRUE; ASSERT(FALSE); return TRUE;};
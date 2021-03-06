/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#line 2 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"

#include "StdH.h"
#define TM_APPLY_DAMAGE_QUANTUM 0.25f
#define TM_APPLY_WHOLE_DAMAGE 7.5f
#define DAMAGE_AMMOUNT 30.0f
#define MIN_DAMAGE_QUANTUM (DAMAGE_AMMOUNT/TM_APPLY_WHOLE_DAMAGE*TM_APPLY_DAMAGE_QUANTUM)
#define MAX_DAMAGE_QUANTUM (MIN_DAMAGE_QUANTUM*10.0f)
#define DEATH_BURN_TIME 4.0f

#include "EntitiesMP/MovingBrush.h"
//#include "EntitiesMP/Santa.h"
#include "EntitiesMP/Player.h"

#include <Flame.h>
#include <Flame_tables.h>
CEntityEvent *EFlame::MakeCopy(void) { 	CEntityEvent *peeCopy = new EFlame(*this); 	return peeCopy;}
SLONG EFlame::GetSizeOf(void) { 	return sizeof(*this);}
EFlame::EFlame() : CEntityEvent(EVENTCODE_EFlame) {
	ClearToDefault(eidOwner);
	ClearToDefault(eidAttach);
}
BOOL EFlame::CheckIDs(void) {	return TRUE && eidOwner.IsValid()&& eidAttach.IsValid(); }

CEntityEvent *EStopFlaming::MakeCopy(void) { 	CEntityEvent *peeCopy = new EStopFlaming(*this); 	return peeCopy;}
SLONG EStopFlaming::GetSizeOf(void) { 	return sizeof(*this);}
EStopFlaming::EStopFlaming() : CEntityEvent(EVENTCODE_EStopFlaming) {
	ClearToDefault(m_bNow);
}
BOOL EStopFlaming::CheckIDs(void) {	return TRUE ; }

#line 29 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"

void CFlame_OnPrecache(CDLLEntityClass *pdec, INDEX iUser) 
{
  pdec->PrecacheModel(MODEL_FLAME);
  pdec->PrecacheTexture(TEXTURE_FLAME);
  pdec->PrecacheSound(SOUND_FLAME);
}

extern ULONG _ulPlayerRenderingMask;

void CFlame::SetDefaultProperties(void) {
	m_penOwner = NULL;
	m_penAttach = NULL;
	m_bLoop = FALSE ;
	m_vHitPoint = FLOAT3D(0.0f , 0.0f , 0.0f);
	m_soEffect.SetOwner(this);
	m_soEffect.Stop(FALSE);
	m_tmStart = 0.0f;
	m_fDamageToApply = 0.0f;
	m_fDamageStep = 0.0f;
	m_fAppliedDamage = 0.0f;
	m_tmFirstStart = 0.0f;
	m_ctFlames = 0;
	m_vPos01 = FLOAT3D(0 , 0 , 0);
	m_vPos02 = FLOAT3D(0 , 0 , 0);
	m_vPos03 = FLOAT3D(0 , 0 , 0);
	m_vPos04 = FLOAT3D(0 , 0 , 0);
	m_vPos05 = FLOAT3D(0 , 0 , 0);
	m_vPos06 = FLOAT3D(0 , 0 , 0);
	m_vPos07 = FLOAT3D(0 , 0 , 0);
	m_vPos08 = FLOAT3D(0 , 0 , 0);
	m_vPos09 = FLOAT3D(0 , 0 , 0);
	m_vPos10 = FLOAT3D(0 , 0 , 0);
	m_vPlaneNormal = FLOAT3D(0 , 0 , 0);
	m_bBurningBrush = FALSE ;
	m_tmDeathParticlesStart = 1e6;
	m_penPrediction = NULL;
	CMovableModelEntity::SetDefaultProperties();
}

#line 90 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
void CFlame::PostMoving(void) {
#line 91 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
CMovableModelEntity  :: PostMoving  ();
#line 94 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
CContentType  & ctDn  = GetWorld  () -> wo_actContentTypes  [ en_iDnContent  ];
#line 96 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
if(! (ctDn  . ct_ulFlags  & CTF_BREATHABLE_LUNGS )){
#line 97 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
EStopFlaming  esf ;
#line 98 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
esf  . m_bNow  = TRUE ;
#line 99 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
SendEvent  (esf );
#line 100 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
}
#line 103 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
en_ulFlags  &= ~ ENF_INRENDERING ;
#line 105 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
en_plLastPlacement  = en_plPlacement ;
#line 106 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
}

#line 109 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
void CFlame::Read_t(CTStream * istr,BOOL bNetwork) 
#line 110 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
{
#line 111 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
CMovableModelEntity  :: Read_t  (istr  , bNetwork );
#line 112 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
SetupLightSource  ();
#line 113 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
}

#line 115 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
BOOL CFlame::IsPointInsidePolygon(const FLOAT3D & vPos,CBrushPolygon * pbpo) 
#line 116 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
{
#line 117 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
FLOATplane3D & plPlane  = pbpo  -> bpo_pbplPlane  -> bpl_plAbsolute ;
#line 119 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
INDEX iMajorAxis1  , iMajorAxis2 ;
#line 120 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
GetMajorAxesForPlane  (plPlane  , iMajorAxis1  , iMajorAxis2 );
#line 123 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
CIntersector  isIntersector  (vPos  (iMajorAxis1 ) , vPos  (iMajorAxis2 ));
#line 125 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
FOREACHINSTATICARRAY  (pbpo  -> bpo_abpePolygonEdges  , CBrushPolygonEdge  , itbpePolygonEdge ){
#line 127 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
const FLOAT3D & vVertex0  = itbpePolygonEdge  -> bpe_pbedEdge  -> bed_pbvxVertex0  -> bvx_vAbsolute ;
#line 128 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
const FLOAT3D & vVertex1  = itbpePolygonEdge  -> bpe_pbedEdge  -> bed_pbvxVertex1  -> bvx_vAbsolute ;
#line 130 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
isIntersector  . AddEdge  (
#line 131 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
vVertex0  (iMajorAxis1 ) , vVertex0  (iMajorAxis2 ) , 
#line 132 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
vVertex1  (iMajorAxis1 ) , vVertex1  (iMajorAxis2 ));
#line 133 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
}
#line 135 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
return isIntersector  . IsIntersecting  ();
#line 136 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
}

#line 139 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
CLightSource * CFlame::GetLightSource(void) 
#line 140 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
{
#line 141 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
return & m_lsLightSource ;
#line 142 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
}

#line 145 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
void CFlame::RenderParticles(void) 
#line 146 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
{
#line 147 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
CEntity  * penParent  = GetParent  ();
#line 149 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
if(IsOfClass  (penParent  , & CPlayer_DLLClass )){
#line 150 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
INDEX iPlayer  = ((CPlayerEntity  *) penParent ) -> GetMyPlayerIndex  ();
#line 151 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
if(_ulPlayerRenderingMask  & (1 << iPlayer )){
#line 152 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
return ;
#line 153 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
}
#line 154 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
}
#line 156 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
FLOAT fTimeFactor  = CalculateRatio  (_pTimer  -> CurrentTick  () , m_tmFirstStart  , m_tmStart  + TM_APPLY_WHOLE_DAMAGE  , 0.05f , 0.2f);
#line 157 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
FLOAT fDeathFactor  = 1.0f;
#line 158 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
if(_pTimer  -> CurrentTick  () > m_tmDeathParticlesStart )
#line 159 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
{
#line 160 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
fDeathFactor  = 1.0f - Clamp  ((_pTimer  -> CurrentTick  () - m_tmDeathParticlesStart ) / DEATH_BURN_TIME  , 0.0f , 1.0f);
#line 161 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
}
#line 163 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
FLOAT fPower  = ClampUp  (m_fDamageStep  - MIN_DAMAGE_QUANTUM  , MAX_DAMAGE_QUANTUM ) / MAX_DAMAGE_QUANTUM ;
#line 164 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
if(penParent  != NULL ){
#line 166 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
if(penParent  -> en_RenderType  == CEntity  :: RT_EDITORMODEL  || penParent  -> en_RenderType  == CEntity  :: RT_SKAEDITORMODEL ){
#line 167 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
return ;
#line 168 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
}else {
#line 170 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
if(penParent  -> en_RenderType  == CEntity  :: RT_MODEL  || penParent  -> en_RenderType  == CEntity  :: RT_SKAMODEL ){
#line 172 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
Particles_Burning  (penParent  , fPower  , fTimeFactor  * fDeathFactor );
#line 174 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
}else {
#line 176 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
Particles_BrushBurning  (this  , & m_vPos01  , m_ctFlames  , m_vPlaneNormal  , fPower  , fTimeFactor  * fDeathFactor );
#line 177 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
}
#line 178 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
}
#line 179 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
}
#line 180 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
}

#line 183 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
void CFlame::SetupLightSource(void) 
#line 184 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
{
#line 186 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
CLightSource  lsNew ;
#line 187 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
lsNew  . ls_ulFlags  = LSF_NONPERSISTENT  | LSF_DYNAMIC ;
#line 188 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
if(m_bBurningBrush )
#line 189 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
{
#line 190 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
UBYTE  ubRndH  = UBYTE  (25 + (FLOAT (rand  ()) / RAND_MAX  - 0.5f) * 28);
#line 191 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
UBYTE  ubRndS  = 166;
#line 192 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
UBYTE  ubRndV  = 48;
#line 193 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
lsNew  . ls_colColor  = HSVToColor  (ubRndH  , ubRndS  , ubRndV );
#line 195 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
lsNew  . ls_rFallOff  = 4.0f;
#line 196 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
lsNew  . ls_rHotSpot  = 0.2f;
#line 197 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
}
#line 198 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
else 
#line 199 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
{
#line 200 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
lsNew  . ls_colColor  = 0x8F8F5000;
#line 201 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
lsNew  . ls_rFallOff  = 6.0f;
#line 202 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
lsNew  . ls_rHotSpot  = 0.50f;
#line 203 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
}
#line 204 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
lsNew  . ls_plftLensFlare  = NULL ;
#line 205 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
lsNew  . ls_ubPolygonalMask  = 0;
#line 206 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
lsNew  . ls_paoLightAnimation  = NULL ;
#line 208 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
m_lsLightSource  . ls_penEntity  = this ;
#line 209 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
m_lsLightSource  . SetLightSource  (lsNew );
#line 210 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
}
BOOL CFlame::
#line 217 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
Main(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CFlame_Main
	ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EFlame, "CFlame::Main expects 'EFlame' as input!");	const EFlame &ef = (const EFlame &)__eeInput;
#line 219 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
InitAsVoid  ();
#line 220 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
Return(STATE_CURRENT,EVoid());
#line 220 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
return TRUE;
#line 224 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
ASSERT  (((CEntity  *) ef  . eidOwner ) != NULL );
#line 225 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
ASSERT  (((CEntity  *) ef  . eidAttach ) != NULL );
#line 226 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
m_penOwner  = ef  . eidOwner ;
#line 227 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
m_penAttach  = ef  . eidAttach ;
#line 237 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
SetFlagOn  (ENF_CLIENTHANDLING );
#line 239 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
m_tmStart  = _pTimer  -> CurrentTick  ();
#line 240 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
m_tmFirstStart  = m_tmStart ;
#line 241 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
SetParent  (ef  . eidAttach );
#line 243 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
InitAsModel  ();
#line 244 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
SetPhysicsFlags  (EPF_MODEL_FLYING );
#line 245 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
SetCollisionFlags  (ECF_FLAME );
#line 246 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
SetFlags  (GetFlags  () | ENF_SEETHROUGH );
#line 248 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
SetModel  (MODEL_FLAME );
#line 249 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
SetModelMainTexture  (TEXTURE_FLAME );
#line 250 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
ModelChangeNotify  ();
#line 253 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
m_soEffect  . Set3DParameters  (20.0f , 2.0f , 0.7f , 1.0f);
#line 254 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
PlaySound  (m_soEffect  , SOUND_FLAME  , SOF_3D  | SOF_LOOP );
#line 257 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
AddToMovers  ();
#line 259 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
m_bBurningBrush  = FALSE ;
#line 260 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
BOOL bAllowFlame  = TRUE ;
#line 261 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
if(! (((CEntity  *) ef  . eidAttach ) -> en_RenderType  == CEntity  :: RT_MODEL  || ((CEntity  *) ef  . eidAttach ) -> en_RenderType  == CEntity  :: RT_EDITORMODEL  || 
#line 262 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
((CEntity  *) ef  . eidAttach ) -> en_RenderType  == CEntity  :: RT_SKAMODEL  || ((CEntity  *) ef  . eidAttach ) -> en_RenderType  == CEntity  :: RT_SKAEDITORMODEL ))
#line 263 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
{
#line 264 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
m_bBurningBrush  = TRUE ;
#line 265 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
FLOAT3D vPos  = GetPlacement  () . pl_PositionVector ;
#line 266 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
FLOATplane3D plPlane ;
#line 267 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
FLOAT fDistanceToEdge ;
#line 268 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
FindSectorsAroundEntity  ();
#line 269 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
CBrushPolygon  * pbpo  = NULL ;
#line 270 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
pbpo  = GetNearestPolygon  (vPos  , plPlane  , fDistanceToEdge );
#line 271 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
FLOAT3D vBrushPos  = ((CEntity  *) ef  . eidAttach ) -> GetPlacement  () . pl_PositionVector ;
#line 272 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
FLOATmatrix3D mBrushRotInv  = ! ((CEntity  *) ef  . eidAttach ) -> GetRotationMatrix  ();
#line 273 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
if(pbpo  != NULL  && pbpo  -> bpo_pbscSector  -> bsc_pbmBrushMip  -> bm_pbrBrush  -> br_penEntity  == ef  . eidAttach )
#line 274 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
{
#line 275 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
plPlane  = pbpo  -> bpo_pbplPlane  -> bpl_plAbsolute ;
#line 276 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
m_vPlaneNormal  = (FLOAT3D &) plPlane ;
#line 277 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
m_vPlaneNormal  . Normalize  ();
#line 279 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
FLOAT3D vU  , vV ;
#line 281 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
if(plPlane  (2) < - 0.1f)
#line 282 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
{
#line 283 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
bAllowFlame  = FALSE ;
#line 284 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
}
#line 287 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
if(Abs  (plPlane  (2)) > 0.5){
#line 289 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
vU  = FLOAT3D (1.0f , 0.0f , 0.0f) * m_vPlaneNormal ;
#line 291 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
}else {
#line 293 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
vU  = FLOAT3D (0.0f , 1.0f , 0.0f) * m_vPlaneNormal ;
#line 294 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
}
#line 296 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
vU  . Normalize  ();
#line 298 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
vV  = vU  * m_vPlaneNormal ;
#line 299 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
vV  . Normalize  ();
#line 302 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
m_ctFlames  = 0;
#line 303 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
for(INDEX iTest  = 0;iTest  < 20;iTest  ++)
#line 304 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
{
#line 305 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
FLOAT fA  = FRnd  () * 360.0f;
#line 306 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
FLOAT fR  = FRnd  () * 2.0f;
#line 307 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
FLOAT3D vRndV  = vV  * fR  * SinFast  (fA );
#line 308 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
FLOAT3D vRndU  = vU  * fR  * CosFast  (fA );
#line 309 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
FLOAT3D vRndPos  = vPos ;
#line 310 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
if(iTest  != 0)
#line 311 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
{
#line 312 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
vRndPos  += vRndV  + vRndU ;
#line 313 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
}
#line 315 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
FLOAT3D vProjectedRndPos  = plPlane  . ProjectPoint  (vRndPos );
#line 316 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
if(IsPointInsidePolygon  (vProjectedRndPos  , pbpo ))
#line 317 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
{
#line 318 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
(& m_vPos01 ) [ m_ctFlames  ] = (vProjectedRndPos  - vBrushPos ) * mBrushRotInv ;
#line 319 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
m_ctFlames  ++;
#line 320 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
if(m_ctFlames  == 6){break ;};
#line 321 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
}
#line 322 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
}
#line 323 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
}
#line 324 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
else 
#line 325 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
{
#line 326 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
bAllowFlame  = FALSE ;
#line 327 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
}
#line 328 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
}
#line 330 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
if(bAllowFlame )
#line 331 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
{
#line 332 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
SetupLightSource  ();
#line 333 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
}
#line 335 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
m_bLoop  = bAllowFlame ;
#line 336 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
Jump(STATE_CURRENT,0x01f80004, FALSE, EInternal());return TRUE;}BOOL CFlame::H0x01f80004_Main_03(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x01f80004
if(!(m_bLoop )){ Jump(STATE_CURRENT,0x01f80005, FALSE, EInternal());return TRUE;}
#line 337 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
SetTimerAfter(TM_APPLY_DAMAGE_QUANTUM );
Jump(STATE_CURRENT, 0x01f80002, FALSE, EBegin());return TRUE;}BOOL CFlame::H0x01f80002_Main_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x01f80002
switch(__eeInput.ee_slEvent){case(EVENTCODE_EBegin):{const EBegin&e= (EBegin&)__eeInput;

#line 341 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
if(m_penAttach  == NULL  || (m_penAttach  -> GetFlags  () & ENF_DELETED )){
#line 343 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
m_bLoop  = FALSE ;
#line 344 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
UnsetTimer();Jump(STATE_CURRENT,0x01f80003, FALSE, EInternal());return TRUE;
#line 345 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
}
#line 347 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
const FLOAT fDamageMul  = GetSeriousDamageMultiplier  (m_penOwner );
#line 348 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
FLOAT fDamageToApply  = fDamageMul  * (m_fDamageToApply  / TM_APPLY_WHOLE_DAMAGE  * TM_APPLY_DAMAGE_QUANTUM ) * m_fDamageStep ;
#line 349 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
m_penAttach  -> InflictDirectDamage  (m_penAttach  , m_penOwner  , DMT_BURNING  , fDamageToApply  , 
#line 350 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
GetPlacement  () . pl_PositionVector  , - en_vGravityDir );
#line 351 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
m_fAppliedDamage  += fDamageToApply ;
#line 352 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
return TRUE;
#line 353 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
}ASSERT(FALSE);break;case(EVENTCODE_EFlame):{const EFlame&ef= (EFlame&)__eeInput;

#line 355 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
m_penOwner  = ef  . eidOwner ;
#line 356 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
FLOAT fTimeLeft  = m_tmStart  + TM_APPLY_WHOLE_DAMAGE  - _pTimer  -> CurrentTick  ();
#line 357 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
FLOAT fDamageLeft  = (fTimeLeft  / TM_APPLY_DAMAGE_QUANTUM ) * m_fDamageStep ;
#line 358 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
m_fDamageToApply  = ClampUp  (fDamageLeft  + DAMAGE_AMMOUNT  , 80.0f);
#line 359 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
m_tmStart  = _pTimer  -> CurrentTick  ();
#line 360 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
m_fDamageStep  = m_fDamageToApply  / (TM_APPLY_WHOLE_DAMAGE  / TM_APPLY_DAMAGE_QUANTUM );
#line 361 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
return TRUE;
#line 362 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
}ASSERT(FALSE);break;case(EVENTCODE_EStopFlaming):{const EStopFlaming&esf= (EStopFlaming&)__eeInput;

#line 364 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
if(! esf  . m_bNow )
#line 365 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
{
#line 366 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
m_tmDeathParticlesStart  = _pTimer  -> CurrentTick  ();
#line 367 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
return TRUE;
#line 368 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
}
#line 369 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
else 
#line 370 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
{
#line 371 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
m_bLoop  = FALSE ;
#line 372 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
UnsetTimer();Jump(STATE_CURRENT,0x01f80003, FALSE, EInternal());return TRUE;
#line 373 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
}
#line 374 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
}ASSERT(FALSE);break;case(EVENTCODE_EBrushDestroyed):{const EBrushDestroyed&e= (EBrushDestroyed&)__eeInput;

#line 376 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
m_bLoop  = FALSE ;
#line 377 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
UnsetTimer();Jump(STATE_CURRENT,0x01f80003, FALSE, EInternal());return TRUE;
#line 378 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
}ASSERT(FALSE);break;case(EVENTCODE_ETimer):{const ETimer&e= (ETimer&)__eeInput;
UnsetTimer();Jump(STATE_CURRENT,0x01f80003, FALSE, EInternal());return TRUE;}ASSERT(FALSE);break;default: return FALSE; break;
#line 380 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
}return TRUE;}BOOL CFlame::H0x01f80003_Main_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x01f80003

#line 381 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
if(_pTimer  -> CurrentTick  () > m_tmStart  + TM_APPLY_WHOLE_DAMAGE )
#line 382 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
{
#line 383 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
m_bLoop  = FALSE ;
#line 384 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
}Jump(STATE_CURRENT,0x01f80004, FALSE, EInternal());return TRUE;
#line 385 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
}BOOL CFlame::H0x01f80005_Main_04(const CEntityEvent &__eeInput) {
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x01f80005

#line 388 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
Destroy  (FALSE );
#line 389 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
Return(STATE_CURRENT,EVoid());
#line 389 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Flame.es"
return TRUE; ASSERT(FALSE); return TRUE;};
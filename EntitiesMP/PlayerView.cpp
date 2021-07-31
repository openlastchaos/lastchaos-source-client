/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#line 2 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"

#include "StdH.h"
#include "EntitiesMP/Player.h"
#include "EntitiesMP/PlayerWeapons.h"

#include <EntitiesMP/PlayerView.h>
#include <EntitiesMP/PlayerView_tables.h>
CEntityEvent *EViewInit::MakeCopy(void) { CEntityEvent *peeCopy = new EViewInit(*this); return peeCopy;}
SLONG EViewInit::GetSizeOf(void) { return sizeof(*this);}
EViewInit::EViewInit() : CEntityEvent(EVENTCODE_EViewInit) {;
 ClearToDefault(eidOwner);
 ClearToDefault(eidCamera);
 ClearToDefault(vtView);
 ClearToDefault(bDeathFixed);
};
BOOL EViewInit::CheckIDs(void) { return TRUE && eidOwner.IsValid()&& eidCamera.IsValid(); }
#line 24 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"

#include <Engine/Effect/CCameraEffect.h>
extern ENGINE_API CCameraEffect::CCameraValue g_cvCameraShake;
FLOAT3D g_vOldCameraShake;
FLOAT3D g_vOldCameraPosByCharCoord;

void CPlayerView_Precache(void) 
{
	CDLLEntityClass *pdec = &CPlayerView_DLLClass;
	pdec->PrecacheModel(MODEL_MARKER);
	pdec->PrecacheTexture(TEXTURE_MARKER);
}

static FLOAT m_fLastTime;
static BOOL m_bLockRotate = FALSE;

// FIXME : 임시적으로 만든 변수.
// 추적 모드를 쓸 것인지 기존 방식을 쓸것인지...
static BOOL m_bUseTraceMode	= FALSE;

void CameraEffect(const CPlacement3D &plPlr, CPlacement3D &plCam)
{
	CPlacement3D plNew = plCam;
	//twist 처리
	if(g_cvCameraShake.m_fTwist != 0)
	{
		plNew.pl_OrientationAngle(3) += g_cvCameraShake.m_fTwist;
	}

	//카메라 좌표기준 이동처리
	FLOATmatrix3D m;
	if(g_cvCameraShake.m_fHorizonal != 0
	|| g_cvCameraShake.m_fVertical != 0
	|| g_cvCameraShake.m_fZoom != 0)
	{
		MakeRotationMatrixFast(m, plCam.pl_OrientationAngle);
		FLOAT3D vMoveByCamCoord(g_cvCameraShake.m_fHorizonal
								, g_cvCameraShake.m_fVertical
								, g_cvCameraShake.m_fZoom);
		RotateVector(vMoveByCamCoord, m);
		plNew.pl_PositionVector += vMoveByCamCoord;
	}

	//캐릭터 좌표기준 이동처리
	if(g_cvCameraShake.m_vMoveByCharCoord(1) != 0
	|| g_cvCameraShake.m_vMoveByCharCoord(2) != 0
	|| g_cvCameraShake.m_vMoveByCharCoord(3) != 0)
	{
		MakeRotationMatrixFast(m, plPlr.pl_OrientationAngle);
		FLOAT3D vMoveByChaCoord = g_cvCameraShake.m_vMoveByCharCoord;
		RotateVector(vMoveByChaCoord, m);
		plNew.pl_PositionVector += vMoveByChaCoord;
	}

	plCam = plNew;
}

void CPlayerView::SetDefaultProperties(void) {
  m_penOwner = NULL;
  m_iViewType = 0;
  m_fDistance = 1.0f;
  m_vZLast = FLOAT3D(0 , 0 , 0);
  m_vTargetLast = FLOAT3D(0 , 0 , 0);
  m_bFixed = FALSE ;
  m_bInit = FALSE ;
  m_aCurAngle = 0.0f;
  m_aAccel = 0.0f;
  m_aVelocity = 0.0f;
  m_PrevModelColor = 0.0f;
  m_bPrevModelColor = FALSE ;
  m_penPrediction = NULL;
  CMovableEntity::SetDefaultProperties();
}
  
#line 109 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
void CPlayerView::PreMoving() {}
  
#line 110 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
void CPlayerView::DoMoving() 
#line 111 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 112 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
en_plLastPlacement  = GetPlacement  ();
#line 113 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
  
#line 114 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
void CPlayerView::PostMoving() 
#line 115 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 117 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(m_bUseTraceMode )
#line 118 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 120 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
SetCameraPosition  ();
#line 121 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 123 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
else 
#line 124 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 125 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
SetCameraPositionEx  ();
#line 126 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 127 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
  
#line 128 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
CPlacement3D CPlayerView::GetLerpedPlacement(void)const 
#line 129 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 130 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
FLOAT fLerpFactor ;
#line 131 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
fLerpFactor  = _pTimer  -> GetLerpFactor2  ();
#line 132 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
return LerpPlacementsPrecise  (en_plLastPlacement  , en_plPlacement  , fLerpFactor );
#line 135 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
  
#line 138 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
void CPlayerView::RenderParticles(void) 
#line 139 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 140 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(Particle_GetViewer  () == this ){
#line 141 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
Particles_ViewerLocal  (this );
#line 142 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 143 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
  
#line 146 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
void CPlayerView::UnlockRotate() 
#line 147 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 148 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
m_bLockRotate  = FALSE ;
#line 149 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
  
#line 154 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
void CPlayerView::SetCameraPositionEx() 
#line 155 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 157 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
FLOAT fDistance  = 1.0f;
#line 159 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
FLOAT fDistanceLimit  = 9.5f;
#line 160 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
static FLOAT fDistanceTmp  = 0.0f;
#line 161 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
BOOL bHit  = FALSE ;
#line 186 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
static BOOL bLimitIn  = FALSE ;
#line 187 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
static FLOAT fSmooth  = 0.0f;
#line 188 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
FLOAT zoomMin  = - 2.0f;
#line 191 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(((CPlayer  &) * m_penOwner ) . m_bRide )
#line 192 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 193 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(((CPlayer  &) * m_penOwner ) . m_iRideType  % 2 == 0)
#line 194 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 195 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
zoomMin  = 3.0f;
#line 196 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 197 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
else 
#line 198 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 199 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
zoomMin  = 2.5f;
#line 200 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 201 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 204 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
CPlacement3D pl  = ((CPlayerEntity  &) * m_penOwner ) . en_plViewpoint ;
#line 205 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
BOOL bFollowCrossHair ;
#line 207 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(m_iViewType  == VT_3RDPERSONVIEW  || m_iViewType  == VT_PLAYERDEATH )
#line 208 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 210 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
fDistance  = 5.0f;
#line 212 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
int  WheelDelta  = abs  (_pInput  -> m_WheelPos ) / 120;
#line 213 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(_pInput  -> m_WheelPos  > 0 || _pInput  -> GetInputDevice  (1) -> id_aicControls  [ KID_PAGEUP  ] . ic_fValue  == 1.0f)
#line 214 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 216 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(_pInput  -> GetInputDevice  (1) -> id_aicControls  [ KID_PAGEUP  ] . ic_fValue  == 1.0f)
#line 217 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 218 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
WheelDelta  = 1.0f;
#line 219 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 220 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(fDistanceTmp  > zoomMin )
#line 221 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 222 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(fDistanceTmp  <= - 1.6)
#line 223 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 224 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
fDistanceTmp  -= 0.2f * WheelDelta ;
#line 225 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 226 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
else 
#line 227 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 228 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
fDistanceTmp  -= 2.0f * WheelDelta ;
#line 229 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 231 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(fDistanceTmp  < zoomMin )
#line 232 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 233 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
fDistanceTmp  = zoomMin ;
#line 235 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 237 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
_pInput  -> m_WheelPos  = 0;
#line 238 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 239 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
else 
#line 240 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 241 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(fSmooth  > 0.0f && ! bLimitIn ){fSmooth  -= 0.03f;}
#line 242 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(fSmooth  <= 0.0f){
#line 243 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
fSmooth  = 0.0f;
#line 244 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
_pInput  -> m_WheelPos  = 0;
#line 245 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(WheelDelta  > 1){
#line 246 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
fDistanceTmp  = zoomMin  - 1.0f;
#line 247 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
fSmooth  = 1.0f;
#line 248 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
bLimitIn  = TRUE ;
#line 250 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 251 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 253 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 256 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(((CPlayer  &) * m_penOwner ) . m_bRide  && fDistanceTmp  <= - 1.1)
#line 257 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 258 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(((CPlayer  &) * m_penOwner ) . m_iRideType  % 2 != 0)
#line 259 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 260 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
fDistanceTmp  = - 1.1f;
#line 261 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 262 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 263 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 264 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
else if(_pInput  -> m_WheelPos  < 0 || _pInput  -> GetInputDevice  (1) -> id_aicControls  [ KID_PAGEDOWN  ] . ic_fValue  == 1.0f)
#line 265 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 267 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(bLimitIn ){fDistanceTmp  = zoomMin ;}
#line 268 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
bLimitIn  = FALSE ;
#line 269 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(fSmooth  < 0.9f){fSmooth  += 0.1f;}
#line 270 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
else if(fSmooth  > 0.9f){
#line 271 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
fSmooth  = 0.9f;
#line 272 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 273 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(_pInput  -> GetInputDevice  (1) -> id_aicControls  [ KID_PAGEDOWN  ] . ic_fValue  == 1.0f)
#line 274 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 275 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
WheelDelta  = 1.0f;
#line 276 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 278 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(fDistanceTmp  < fDistanceLimit )
#line 279 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 280 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(fDistanceTmp  <= - 1.6)
#line 281 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 282 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
fDistanceTmp  += 0.2f * WheelDelta ;
#line 283 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 284 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
else 
#line 285 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 286 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
fDistanceTmp  += 0.4f * WheelDelta ;
#line 287 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 290 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
_pInput  -> m_WheelPos  = 0;
#line 291 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 293 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 295 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(fDistanceTmp  > fDistanceLimit )
#line 296 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 297 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
fDistanceTmp  = fDistanceLimit ;
#line 298 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 299 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(fDistanceTmp  < zoomMin  - 1.0f)
#line 300 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 301 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
fDistanceTmp  = zoomMin  - 1.0f;
#line 302 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 305 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
fDistance  = fDistance  + fDistanceTmp ;
#line 307 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
bFollowCrossHair  = TRUE ;
#line 309 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 314 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
CPlacement3D plTmp ;
#line 315 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
plTmp  = m_penOwner  -> GetPlacement  ();
#line 316 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
plTmp  . pl_OrientationAngle  (1) = pl  . pl_OrientationAngle  (1);
#line 318 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
pl  . RelativeToAbsolute  (plTmp );
#line 321 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
FLOAT3D vBase ;
#line 322 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
EntityInfo  * pei  = (EntityInfo  *) (m_penOwner  -> GetEntityInfo  ());
#line 330 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
vBase  = m_penOwner  -> GetPlacement  () . pl_PositionVector ;
#line 332 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
FLOATmatrix3D mRotation ;
#line 334 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
MakeRotationMatrixFast  (mRotation  , m_penOwner  -> GetPlacement  () . pl_OrientationAngle );
#line 335 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
vBase  += FLOAT3D (pei  -> vTargetCenter  [ 0 ] , pei  -> vTargetCenter  [ 1 ] , pei  -> vTargetCenter  [ 2 ]) * mRotation ;
#line 340 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
FLOATmatrix3D m ;
#line 342 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
MakeRotationMatrixFast  (m  , pl  . pl_OrientationAngle );
#line 345 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
FLOAT3D vRight  = m  . GetColumn  (1);
#line 346 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
FLOAT3D vUp  = m  . GetColumn  (2);
#line 347 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
FLOAT3D vFront  = m  . GetColumn  (3);
#line 350 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
FLOAT3D vDest  [ 5 ];
#line 351 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
vDest  [ 0 ] = vBase  + vFront  * fDistance  + vUp  * 1.5f;
#line 352 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
vDest  [ 1 ] = vBase  + vFront  * fDistance  + vRight  * 1.5f;
#line 353 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
vDest  [ 2 ] = vBase  + vFront  * fDistance  - vRight  * 1.5f;
#line 354 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
vDest  [ 3 ] = vBase  + vFront  * fDistance  - vUp  * 1.5f;
#line 355 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
vDest  [ 4 ] = vBase  + vFront  * fDistance ;
#line 357 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
FLOAT fBack  = 0;
#line 359 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 361 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
for(INDEX i  = 0;i  < 5;i  ++)
#line 362 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 364 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
CCastRay  crRay  (m_penOwner  , vBase  , vDest  [ i  ]);
#line 365 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
crRay  . cr_bHitTranslucentPortals  = FALSE ;
#line 369 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
crRay  . cr_ttHitModels  = CCastRay  :: TT_NONE ;
#line 375 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
GetWorld  () -> CastRay  (crRay );
#line 378 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(crRay  . cr_penHit  != NULL  && (! (crRay  . cr_penHit  -> GetFlags  () & ENF_ALIVE ) && ! (crRay  . cr_penHit  -> GetFlags  () & ENF_ITEM )))
#line 379 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 381 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
fDistance  = Min  (fDistance  , crRay  . cr_fHitDistance  - 0.5f);
#line 384 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(crRay  . cr_pbpoBrushPolygon  != NULL )
#line 385 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 387 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
FLOAT3D vDir  = (vDest  [ i  ] - vBase ) . Normalize  ();
#line 390 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
FLOAT fD  = Abs  (FLOAT3D (crRay  . cr_pbpoBrushPolygon  -> bpo_pbplPlane  -> bpl_plAbsolute ) % vDir ) * 0.25f;
#line 391 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
fBack  = Max  (fBack  , fD );
#line 392 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
bHit  = TRUE ;
#line 393 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 394 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 395 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 396 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 399 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(((CPlayer  &) * m_penOwner ) . m_bRide )
#line 400 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 401 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(((CPlayer  &) * m_penOwner ) . m_iRideType  % 2 == 0)
#line 402 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 403 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
fDistance  -= 5.0f;
#line 404 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 405 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
else 
#line 406 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 407 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
fDistance  -= 4.5f;
#line 408 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 409 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 411 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
fDistance  = ClampDn  (fDistance  - fBack  , 0.0f);
#line 413 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(m_fDistance  < fDistance )
#line 414 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 415 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
m_fDistance  += 0.5f;
#line 416 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 418 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(m_fDistance  >= fDistance )
#line 419 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 420 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
m_fDistance  = fDistance ;
#line 421 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 423 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
vBase  += vFront  * m_fDistance ;
#line 424 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
pl  . pl_PositionVector  = vBase ;
#line 429 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(m_fDistance  < 1.0f)
#line 430 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 431 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(! m_bPrevModelColor )
#line 432 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 433 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
m_PrevModelColor  = ((CPlayer  &) * m_penOwner ) . GetModelInstance  () -> GetModelColor  ();
#line 434 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
COLOR tColor  = m_PrevModelColor ;
#line 435 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
tColor  &= 0xFFFFFF00;
#line 436 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
tColor  |= 0x0F;
#line 437 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
((CPlayer  &) * m_penOwner ) . GetModelInstance  () -> mi_colModelColor  = tColor ;
#line 438 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
m_bPrevModelColor  = TRUE ;
#line 439 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 440 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 441 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
else 
#line 442 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 443 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(m_bPrevModelColor )
#line 444 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 445 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
((CPlayer  &) * m_penOwner ) . GetModelInstance  () -> mi_colModelColor  = m_PrevModelColor ;
#line 446 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
m_bPrevModelColor  = FALSE ;
#line 447 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 448 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 453 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(((CPlayer  &) * m_penOwner ) . m_bRide )
#line 454 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 455 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(((CPlayer  &) * m_penOwner ) . m_iRideType  % 2 == 0)
#line 456 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 457 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
vBase  += vFront  * m_fDistance ;
#line 458 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
pl  . pl_PositionVector  = vBase ;
#line 459 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
pl  . pl_PositionVector  (2) += 1.0f;
#line 460 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 461 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
else 
#line 462 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 463 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
vBase  += vFront  * m_fDistance ;
#line 464 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
pl  . pl_PositionVector  = vBase ;
#line 465 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
pl  . pl_PositionVector  (2) += 3.0f;
#line 466 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 467 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 468 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(! _pNetwork  -> m_bSingleMode )
#line 469 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 472 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
pl  . pl_PositionVector  (2) += fSmooth ;
#line 473 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 478 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
SetPlacement_special  (pl  , m  , SPIF_NEAR );
#line 479 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
  
#line 482 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
void CPlayerView::SetCameraPosition() 
#line 483 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 485 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
FLOAT fDistance  = 1.0f;
#line 486 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
static FLOAT fDistanceTmp  = 0.0f;
#line 487 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
static FLOAT fUpTmp  = 0.0f;
#line 489 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
CPlacement3D pl  = ((CPlayerEntity  &) * m_penOwner ) . en_plViewpoint ;
#line 490 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
BOOL bFollowCrossHair ;
#line 493 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
extern  BOOL _bWorldEditorApp ;
#line 497 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
BOOL bBackView  = TRUE ;
#line 500 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(m_iViewType  == VT_3RDPERSONVIEW  || m_iViewType  == VT_PLAYERDEATH )
#line 501 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 503 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(_pNetwork  -> m_bSingleMode )
#line 504 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 505 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
fDistance  = 6.0f;
#line 506 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 507 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
else 
#line 508 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 509 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
fDistance  = 5.0f;
#line 510 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 512 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(_pInput  -> GetInputDevice  (1) -> id_aicControls  [ KID_PAGEUP  ] . ic_fValue  == 1.0f)
#line 513 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 514 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
fUpTmp  -= 10.0f;
#line 515 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
m_bLockRotate  = TRUE ;
#line 516 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 517 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(_pInput  -> GetInputDevice  (1) -> id_aicControls  [ KID_PAGEDOWN  ] . ic_fValue  == 1.0f)
#line 518 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 519 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
fUpTmp  += 10.0f;
#line 520 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
m_bLockRotate  = TRUE ;
#line 521 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 525 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(_pInput  -> m_WheelPos  > 0){
#line 526 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(fDistanceTmp  > - 3.6f){
#line 527 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(fDistanceTmp  <= - 1.6){
#line 528 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
fDistanceTmp  -= 0.2f;
#line 529 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 530 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
else 
#line 531 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 532 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
fDistanceTmp  -= 1.0f;
#line 533 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 535 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
_pInput  -> m_WheelPos  = 0;
#line 536 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 537 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 538 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
else if(_pInput  -> m_WheelPos  < 0){
#line 539 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(fDistanceTmp  < 25.0f){
#line 540 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(fDistanceTmp  <= - 1.6){
#line 541 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
fDistanceTmp  += 0.2f;
#line 542 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 543 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
else 
#line 544 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 545 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
fDistanceTmp  += 1.0f;
#line 546 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 548 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
_pInput  -> m_WheelPos  = 0;
#line 549 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 550 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 552 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(_pNetwork  -> m_bSingleMode )
#line 553 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 554 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
fDistanceTmp  = 0.0f;
#line 555 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 557 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
fDistance  = fDistance  + fDistanceTmp ;
#line 559 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
bFollowCrossHair  = TRUE ;
#line 561 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 563 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
pl  . pl_OrientationAngle  (3) = 0.0f;
#line 568 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
CPlacement3D plTmp ;
#line 569 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
plTmp  = m_penOwner  -> GetPlacement  ();
#line 572 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
pl  . RelativeToAbsolute  (plTmp );
#line 575 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
FLOAT3D vBase ;
#line 576 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
EntityInfo  * pei  = (EntityInfo  *) (m_penOwner  -> GetEntityInfo  ());
#line 579 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(_pNetwork  -> m_bSingleMode  && bBackView )
#line 580 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 581 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
CPlayerWeapons  * ppw  = ((CPlayer  &) * m_penOwner ) . GetPlayerWeapons  ();
#line 582 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
FLOAT m_fCurTime  = _pTimer  -> CurrentTick  ();
#line 584 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(! m_bLockRotate )
#line 585 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 587 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(ppw  -> m_penRayHitTmp )
#line 588 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 589 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
FLOAT3D vDelta  = ppw  -> m_vRayHitTmp  - m_penOwner  -> GetPlacement  () . pl_PositionVector ;
#line 590 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
vDelta  . SafeNormalize  ();
#line 591 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
ANGLE aDelta  = GetRelativeHeading  (vDelta );
#line 592 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
aDelta  = NormalizeAngle  (aDelta );
#line 594 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(! m_bInit )
#line 595 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 596 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
m_bInit  = TRUE ;
#line 597 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
m_fLastTime  = m_fCurTime ;
#line 598 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 599 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
m_aVelocity  = aDelta ;
#line 600 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 601 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
FLOAT m_fDeltaTime  = m_fCurTime  - m_fLastTime ;
#line 602 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
m_aVelocity  = m_aVelocity  + (m_aAccel  * m_fDeltaTime );
#line 603 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
m_aCurAngle  = m_aCurAngle  + (m_aVelocity  * m_fDeltaTime );
#line 604 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 606 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
m_fLastTime  = m_fCurTime ;
#line 608 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
pl  . pl_OrientationAngle  (1) = m_aCurAngle  - fUpTmp ;
#line 609 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 617 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
vBase  = m_penOwner  -> GetPlacement  () . pl_PositionVector ;
#line 619 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
FLOATmatrix3D mRotation ;
#line 621 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
MakeRotationMatrixFast  (mRotation  , m_penOwner  -> GetPlacement  () . pl_OrientationAngle );
#line 622 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
vBase  += FLOAT3D (pei  -> vTargetCenter  [ 0 ] , pei  -> vTargetCenter  [ 1 ] , pei  -> vTargetCenter  [ 2 ]) * mRotation ;
#line 627 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
FLOATmatrix3D m ;
#line 630 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
MakeRotationMatrixFast  (m  , pl  . pl_OrientationAngle );
#line 633 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
FLOAT3D vRight  = m  . GetColumn  (1);
#line 634 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
FLOAT3D vUp  = m  . GetColumn  (2);
#line 635 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
FLOAT3D vFront  = m  . GetColumn  (3);
#line 638 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
FLOAT3D vDest  [ 5 ];
#line 639 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
vDest  [ 0 ] = vBase  + vFront  * fDistance  + vUp  * 1.0f;
#line 640 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
vDest  [ 1 ] = vBase  + vFront  * fDistance  - vUp  * 1.0f;
#line 641 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
vDest  [ 2 ] = vBase  + vFront  * fDistance  + vRight  * 1.0f;
#line 642 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
vDest  [ 3 ] = vBase  + vFront  * fDistance  - vRight  * 1.0f;
#line 643 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
vDest  [ 4 ] = vBase  + vFront  * fDistance ;
#line 645 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
FLOAT fBack  = 0;
#line 647 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
for(INDEX i  = 0;i  < 5;i  ++)
#line 648 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 650 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
CCastRay  crRay  (m_penOwner  , vBase  , vDest  [ i  ]);
#line 651 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
crRay  . cr_bHitTranslucentPortals  = FALSE ;
#line 652 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
crRay  . cr_ttHitModels  = CCastRay  :: TT_COLLISIONBOX ;
#line 653 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
GetWorld  () -> CastRay  (crRay );
#line 656 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(crRay  . cr_penHit  != NULL  && (! (crRay  . cr_penHit  -> GetFlags  () & ENF_ALIVE ) && ! (crRay  . cr_penHit  -> GetFlags  () & ENF_ITEM )))
#line 657 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 659 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
fDistance  = Min  (fDistance  , crRay  . cr_fHitDistance  - 0.5f);
#line 661 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(crRay  . cr_pbpoBrushPolygon  != NULL )
#line 662 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 664 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
FLOAT3D vDir  = (vDest  [ i  ] - vBase ) . Normalize  ();
#line 691 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
FLOAT fD  = Abs  (FLOAT3D (crRay  . cr_pbpoBrushPolygon  -> bpo_pbplPlane  -> bpl_plAbsolute ) % vDir ) * 0.25f;
#line 692 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
fBack  = Max  (fBack  , fD );
#line 695 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 696 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 697 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 699 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
fDistance  = ClampDn  (fDistance  - fBack  , 0.0f);
#line 700 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
m_fDistance  = fDistance ;
#line 701 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
vBase  += vFront  * fDistance ;
#line 738 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
pl  . pl_PositionVector  = vBase ;
#line 753 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
SetPlacement_special  (pl  , m  , SPIF_NEAR );
#line 754 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
BOOL CPlayerView::
#line 757 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
Main(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CPlayerView_Main
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EViewInit, "CPlayerView::Main expects 'EViewInit' as input!");  const EViewInit &eInit = (const EViewInit &)__eeInput;
#line 760 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
ASSERT  (((CEntity  *) eInit  . eidOwner ) != NULL );
#line 761 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
m_penOwner  = eInit  . eidOwner ;
#line 763 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
m_iViewType  = VT_3RDPERSONVIEW ;
#line 764 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
m_bFixed  = eInit  . bDeathFixed ;
#line 765 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
ASSERT  (IsOfClass  (m_penOwner  , & CPlayer_DLLClass ));
#line 768 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
InitAsEditorModel  ();
#line 769 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
SetFlags  (GetFlags  () | ENF_CROSSESLEVELS );
#line 770 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
SetPhysicsFlags  (EPF_MODEL_IMMATERIAL  | EPF_MOVABLE );
#line 771 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
SetCollisionFlags  (ECF_IMMATERIAL );
#line 773 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
SetFlagOff  (ENF_PLACCHANGED  | ENF_PROPSCHANGED );
#line 774 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
SetFlagOff  (ENF_PROPSCHANGED );
#line 777 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
SetModel  (MODEL_MARKER );
#line 778 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
SetModelMainTexture  (TEXTURE_MARKER );
#line 786 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
m_bLockRotate  = FALSE ;
#line 788 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
SendEvent  (EStart  ());
#line 789 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
SetTimerAt(THINKTIME_NEVER);
Jump(STATE_CURRENT, 0x01930001, FALSE, EBegin());return TRUE;}BOOL CPlayerView::H0x01930001_Main_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x01930001
switch(__eeInput.ee_slEvent)
#line 790 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{case(EVENTCODE_EBegin):
#line 792 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{const EBegin&e= (EBegin&)__eeInput;

#line 793 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
return TRUE;
#line 794 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}ASSERT(FALSE);break;case(EVENTCODE_EStart):
#line 796 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{const EStart&e= (EStart&)__eeInput;

#line 800 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
if(m_bUseTraceMode )
#line 801 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 803 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
SetCameraPosition  ();
#line 804 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 806 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
else 
#line 807 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
{
#line 808 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
SetCameraPositionEx  ();
#line 809 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}
#line 810 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
en_plLastPlacement  = GetPlacement  ();
#line 811 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
m_vTargetLast  = ((CPlayer  &) * m_penOwner ) . GetPlayerWeapons  () -> m_vRayHit ;
#line 812 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
return TRUE;
#line 813 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}ASSERT(FALSE);break;case(EVENTCODE_EEnd):{const EEnd&e= (EEnd&)__eeInput;
UnsetTimer();Jump(STATE_CURRENT,0x01930002, FALSE, EInternal());return TRUE;}ASSERT(FALSE);break;default:{return TRUE;}ASSERT(FALSE);break;
#line 816 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
}return TRUE;}BOOL CPlayerView::H0x01930002_Main_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x01930002

#line 818 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
Destroy  (FALSE );
#line 820 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
Return(STATE_CURRENT,EVoid());
#line 820 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerView.es"
return TRUE; ASSERT(FALSE); return TRUE;};
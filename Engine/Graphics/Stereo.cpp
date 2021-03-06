#include "stdh.h"

#include <Engine/Graphics/Stereo.h>

#include <Engine/Math/Projection.h>
#include <Engine/Graphics/GfxLibrary.h>
#include <Engine/Graphics/ViewPort.h>
#include <Engine/Graphics/Gfx_wrapper.h>

extern INDEX gfx_iStereo;
extern INDEX gfx_bStereoInvert;
extern INDEX gfx_iStereoOffset;
extern FLOAT gfx_fStereoSeparation;

// query whether user has turned stereo rendering on
BOOL Stereo_IsEnabled(void)
{
  return gfx_iStereo!=0;
}


// set buffer for stereo rendering left/right/nostereo
void Stereo_SetBuffer(INDEX iEye)
{
  if( gfx_bStereoInvert) gfx_bStereoInvert = 1; 
  const ULONG ulLeftMask  = gfx_bStereoInvert ? CT_BMASK|CT_GMASK : CT_RMASK;
  const ULONG ulRightMask = gfx_bStereoInvert ? CT_RMASK : CT_BMASK|CT_GMASK;

  if( iEye==STEREO_BOTH || gfx_iStereo==0) {
    gfxSetColorMask(CT_RMASK|CT_GMASK|CT_BMASK|CT_AMASK);
  } else if (iEye==STEREO_LEFT) {
    gfxSetColorMask(ulLeftMask);
  } else if (iEye==STEREO_RIGHT) {
    gfxSetColorMask(ulRightMask);
  }
}


// adjust perspective projection stereo rendering left/right/nostereo
void Stereo_AdjustProjection(CProjection3D &pr, INDEX iEye, FLOAT fFactor)
{
  // prepare and clamp
  CPerspectiveProjection3D &ppr = (CPerspectiveProjection3D &)pr;
  gfx_fStereoSeparation = Clamp( gfx_fStereoSeparation, 0.01f, 1.0f);
  gfx_iStereoOffset     = Clamp( gfx_iStereoOffset, -100L, +100L);
  // apply!
  if (iEye==STEREO_BOTH || gfx_iStereo==0) {
    NOTHING;
  } else if (iEye==STEREO_LEFT) {
    pr.ViewerPlacementL().Translate_OwnSystem( FLOAT3D(-gfx_fStereoSeparation*fFactor/2,0,0) );
    ppr.ppr_boxSubScreen  = ppr.pr_ScreenBBox;
    ppr.ppr_boxSubScreen -= FLOAT2D(-gfx_iStereoOffset/2.0f, 0.0f);
  } else if (iEye==STEREO_RIGHT) {
    pr.ViewerPlacementL().Translate_OwnSystem( FLOAT3D(+gfx_fStereoSeparation*fFactor/2,0,0) );
    ppr.ppr_boxSubScreen  = ppr.pr_ScreenBBox;
    ppr.ppr_boxSubScreen -= FLOAT2D(+gfx_iStereoOffset/2.0f, 0.0f);
  }
}


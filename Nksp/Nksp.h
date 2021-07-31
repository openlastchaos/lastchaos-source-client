/*
 *  Serious Sam
 *  Copyright (c) 1998, CroTeam. All rights reserved.
 *
 */

#define APPLICATION_NAME "Nksp"
#include "CDCheck.h"

//extern HINSTANCE _hInstance;		// yjpark
extern BOOL _bRunning, _bQuitScreen;

extern ENGINE_API INDEX sam_bFullScreenActive;
extern ENGINE_API INDEX sam_iScreenSizeI;
extern ENGINE_API INDEX sam_iScreenSizeJ;
extern ENGINE_API INDEX sam_iDisplayDepth;
extern ENGINE_API INDEX sam_iDisplayAdapter;
extern ENGINE_API INDEX sam_iGfxAPI;
extern INDEX sam_iVideoSetup;  // 0==speed, 1==normal, 2==quality, 3==custom
extern BOOL  sam_bAutoAdjustAudio;

extern void StartNewMode( enum GfxAPIType eAPI, INDEX iAdapter, PIX pixSizeI, PIX pixSizeJ,
                          enum DisplayDepth eColorDepth, BOOL bFullScreenMode);
struct KeyData {
  char *kd_strASCII;
};

extern CGame *_pGame;

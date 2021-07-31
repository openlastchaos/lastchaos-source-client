#include "stdh.h"
#include <Engine/Base/Stream.h>
#include <Engine/Terrain/Terrain.h>
#include <Engine/Terrain/Terrain.hpp>
#include <Engine/Terrain/TRGenerate.hpp>
#include <Engine/Terrain/TRLayer.h>
#include <Engine/Terrain/TRLayer.hpp>
#include <Engine/Templates/Stock_CTextureData.h>
// Fill height map with const value
extern void TR_FillHeightMap(CTerrain *ptrTerrain, UWORD uwFill);/*
{
	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain->tr_puwHeightMap!=NULL);
	
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;
	const PIX pixHeightMapWidth  = ptr->tr_pixHeightMapWidth;
	const PIX pixHeightMapHeight = ptr->tr_pixHeightMapHeight;
	const PIX pixHeightMapPixels = pixHeightMapWidth*pixHeightMapHeight;

	UWORD *puwDst = ptr->tr_puwHeightMap;
	for(INDEX ipix=0;ipix<pixHeightMapPixels;ipix++) {
		puwDst[ipix] = uwFill;
	}
}
*/
extern void TR_FillEdgeMap(CTerrain *ptrTerrain, UBYTE ubFill);/*
{
	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain->tr_pubEdgeMap!=NULL);
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	const PIX pixEdgeMapWidth = ptr->tr_pixHeightMapWidth-1;
	const PIX pixEdgeMapHeight = ptr->tr_pixHeightMapHeight-1;
	const SLONG slMapSize = pixEdgeMapWidth * pixEdgeMapHeight * sizeof(UBYTE);

	memset(ptr->tr_pubEdgeMap,ubFill,slMapSize);

	// Upload terrain edge map
	TR_UploadEdgeMap(ptrTerrain);
}*/
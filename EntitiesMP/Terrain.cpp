/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#line 2 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Terrain.es"

#include "StdH.h"

#include <Terrain.h>
#include <Terrain_tables.h>
void CTerrainEntity::SetDefaultProperties(void) {
	CEntity::SetDefaultProperties();
}
BOOL CTerrainEntity::
#line 14 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Terrain.es"
Main(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CTerrainEntity_Main
	ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CTerrainEntity::Main expects 'EVoid' as input!");	const EVoid &e = (const EVoid &)__eeInput;
#line 17 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Terrain.es"
InitAsTerrain  ();
#line 18 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Terrain.es"
TerrainChangeNotify  ();
#line 19 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Terrain.es"
SetCollisionFlags  (ECF_BRUSH );
#line 20 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Terrain.es"
Return(STATE_CURRENT,EVoid());
#line 20 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Terrain.es"
return TRUE; ASSERT(FALSE); return TRUE;};
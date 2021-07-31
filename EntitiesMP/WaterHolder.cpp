/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#line 2 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"

#include "StdH.h"
#define C_WHITE_OPAQUE	(C_WHITE|CT_OPAQUE)

#include <EntitiesMP/WaterHolder.h>
#include <EntitiesMP/WaterHolder_tables.h>
void CWaterHolder::SetDefaultProperties(void) {
  m_fnModel = CTFILENAME("");
  m_fnmWaterTex = CTFILENAME("");
  m_fnmBumpTex = CTFILENAME("");
  m_fStretchAll = 1.0f;
  m_vStretchXYZ = FLOAT3D(1.0f , 1.0f , 1.0f);
  m_bColliding = FALSE ;
  m_bActive = TRUE ;
  m_bWireFrame = FALSE ;
  m_bReflection = TRUE ;
  m_bWave = FALSE ;
  m_fBumpMat11 = 0.03f;
  m_fBumpMat12 = -0.03f;
  m_fBumpMat21 = -0.03f;
  m_fBumpMat22 = 0.03f;
  m_fTexFlowX = 0.001f;
  m_fTexFlowY = 0.001f;
  m_colWater = C_WHITE_OPAQUE ;
  m_strName = "WaterHolder";
  CRationalEntity::SetDefaultProperties();
}
  
#line 38 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
void CWaterHolder::Precache(void) 
#line 39 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
{
#line 40 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
PrecacheSkaModel  (EDITOR_WATER_MODEL );
#line 41 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
}
  
#line 59 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
void CWaterHolder::InitNiceWater() 
#line 60 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
{
#line 61 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
if(m_fnModel  == "")
#line 62 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
{
#line 65 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
m_fnModel  = CTFILENAME  ("Data\\World\\Water\\WaterPlane.smc");
#line 67 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
}
#line 70 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
if(m_fnmWaterTex  == "")
#line 71 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
{
#line 72 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
m_fnmWaterTex  = CTFILENAME  ("Data\\World\\Water\\Texture\\WaterFX.tex");
#line 73 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
}
#line 77 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
if(m_fnmBumpTex  == "")
#line 78 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
{
#line 79 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
m_fnmBumpTex  = CTFILENAME  ("Data\\World\\Water\\Texture\\OceanBump.tex");
#line 80 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
}
#line 90 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
InitAsSkaModel  ();
#line 91 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
SetFlagOn  (ENF_NICEWATER );
#line 93 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
BOOL bLoadOK  = TRUE ;
#line 95 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
try {
#line 96 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
SetSkaModel_t  (m_fnModel );
#line 98 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
}catch  (char  *){
#line 99 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
bLoadOK  = FALSE ;
#line 101 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
}
#line 102 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
if(! bLoadOK )
#line 103 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
{
#line 104 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
SetSkaModel  (CTFILENAME  ("Data\\Models\\Editor\\Ska\\Axis.smc"));
#line 105 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
WarningMessage  (TRANS  ("Cannot load ska model '%s'") , (CTString &) m_fnModel );
#line 106 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
}
#line 108 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
GetModelInstance  () -> SetModelColor  (m_colWater );
#line 111 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
StretchModel  ();
#line 112 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
ModelChangeNotify  ();
#line 114 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
return ;
#line 115 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
}
  
#line 132 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
void CWaterHolder::StretchModel(void) 
#line 133 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
{
#line 135 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
if(Abs  (m_vStretchXYZ  (1)) < 0.01f){m_vStretchXYZ  (1) = 0.01f;}
#line 136 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
if(Abs  (m_vStretchXYZ  (2)) < 0.01f){m_vStretchXYZ  (2) = 0.01f;}
#line 137 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
if(Abs  (m_vStretchXYZ  (3)) < 0.01f){m_vStretchXYZ  (3) = 0.01f;}
#line 138 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
if(m_fStretchAll  < 0.01f){m_fStretchAll  = 0.01f;}
#line 140 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
if(Abs  (m_vStretchXYZ  (1)) > 1000.0f){m_vStretchXYZ  (1) = 1000.0f * Sgn  (m_vStretchXYZ  (1));}
#line 141 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
if(Abs  (m_vStretchXYZ  (2)) > 1000.0f){m_vStretchXYZ  (2) = 1000.0f * Sgn  (m_vStretchXYZ  (2));}
#line 142 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
if(Abs  (m_vStretchXYZ  (3)) > 1000.0f){m_vStretchXYZ  (3) = 1000.0f * Sgn  (m_vStretchXYZ  (3));}
#line 143 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
if(m_fStretchAll  > 1000.0f){m_fStretchAll  = 1000.0f;}
#line 144 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
GetModelInstance  () -> StretchModel  (m_vStretchXYZ  * m_fStretchAll );
#line 145 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
return ;
#line 146 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
}
BOOL CWaterHolder::
#line 149 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
Main(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CWaterHolder_Main
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CWaterHolder::Main expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 151 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
InitNiceWater  ();
#line 152 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
Return(STATE_CURRENT,EVoid());
#line 152 "E:/질산/Korea/Client_USA/EntitiesMP/WaterHolder.es"
return TRUE; ASSERT(FALSE); return TRUE;};
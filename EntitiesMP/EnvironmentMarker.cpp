/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#line 2 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"

#include "StdH.h"

#include <EntitiesMP/EnvironmentMarker.h>
#include <EntitiesMP/EnvironmentMarker_tables.h>
void CEnvironmentMarker::SetDefaultProperties(void) {
  m_fWaitTime = 0.0f;
  m_fRandomTime = 0.0f;
  m_fMarkerRange = 5.0f;
  m_bFixedAnimLength = FALSE ;
  m_bChangeDefaultAnim = FALSE ;
  m_fMoveSpeed = -1.0f;
  m_fRotateSpeed = -1.0f;
  m_fnMdl = CTFILENAME("Data\\Models\\Editor\\Axis.mdl");
  m_iAnim = 0;
  m_moAnimData.SetData(NULL);
m_moAnimData.mo_toTexture.SetData(NULL);
  CMarker::SetDefaultProperties();
}
  
#line 32 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"
BOOL CEnvironmentMarker::MovesByTargetedRoute(CTString & strTargetProperty)const {
#line 33 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"
strTargetProperty  = "Target";
#line 34 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"
return TRUE ;
#line 35 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"
}
  
#line 37 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"
BOOL CEnvironmentMarker::DropsMarker(CTFileName & fnmMarkerClass,CTString & strTargetProperty)const {
#line 38 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"
fnmMarkerClass  = CTFILENAME  ("Classes\\EnvironmentMarker.ecl");
#line 39 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"
strTargetProperty  = "Target";
#line 40 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"
return TRUE ;
#line 41 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"
}
  
#line 43 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"
CAnimData * CEnvironmentMarker::GetAnimData(SLONG slPropertyOffset) {
#line 44 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"
if(slPropertyOffset  == offsetof  (CEnvironmentMarker  , m_iAnim )){
#line 45 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"
return m_moAnimData  . GetData  ();
#line 47 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"
}else {
#line 48 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"
return CEntity  :: GetAnimData  (slPropertyOffset );
#line 49 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"
}
#line 50 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"
}
  
#line 54 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"
void CEnvironmentMarker::SetModelObject(void) {
#line 56 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"
try {
#line 58 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"
m_moAnimData  . SetData_t  (m_fnMdl );
#line 60 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"
}catch  (char  * strError ){
#line 61 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"
strError ;
#line 62 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"
DECLARE_CTFILENAME  (fnmDefault  , "Data\\Models\\Editor\\Axis.mdl");
#line 64 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"
try {
#line 66 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"
m_moAnimData  . SetData_t  (fnmDefault );
#line 68 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"
}catch  (char  * strErrorDefault ){
#line 69 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"
FatalError  (TRANS  ("Cannot load default model '%s':\n%s") , 
#line 70 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"
(CTString &) fnmDefault  , strErrorDefault );
#line 71 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"
}
#line 72 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"
}
#line 73 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"
}
BOOL CEnvironmentMarker::
#line 77 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"
Main(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CEnvironmentMarker_Main
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CEnvironmentMarker::Main expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 78 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"
InitAsEditorModel  ();
#line 79 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"
SetPhysicsFlags  (EPF_MODEL_IMMATERIAL );
#line 80 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"
SetCollisionFlags  (ECF_IMMATERIAL );
#line 83 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"
SetModel  (MODEL_MARKER );
#line 84 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"
SetModelMainTexture  (TEXTURE_MARKER );
#line 87 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"
SetModelObject  ();
#line 89 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"
Return(STATE_CURRENT,EVoid());
#line 89 "E:/질산/Korea/Client_USA/EntitiesMP/EnvironmentMarker.es"
return TRUE; ASSERT(FALSE); return TRUE;};
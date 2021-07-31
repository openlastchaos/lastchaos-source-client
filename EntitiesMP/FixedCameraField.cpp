/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#line 2 "E:/질산/Korea/Client_USA/EntitiesMP/FixedCameraField.es"

#include "StdH.h"
#include "Common/FixedCameraContainer.h"

#include <EntitiesMP/FixedCameraField.h>
#include <EntitiesMP/FixedCameraField_tables.h>
void CFixedCameraField::SetDefaultProperties(void) {
  m_strName = "Fixed Camera Field";
  m_strDescription = "";
  du = 1.0f;
  dv = 1.0f;
  dw = 1.0f;
  m_penMarker = NULL;
  CRationalEntity::SetDefaultProperties();
}
   CFixedCameraField::CFixedCameraField(void) 
#line 35 "E:/질산/Korea/Client_USA/EntitiesMP/FixedCameraField.es"
{
#line 36 "E:/질산/Korea/Client_USA/EntitiesMP/FixedCameraField.es"
InsertCameraField  (this );
#line 37 "E:/질산/Korea/Client_USA/EntitiesMP/FixedCameraField.es"
}
   CFixedCameraField:: ~ CFixedCameraField(void) 
#line 42 "E:/질산/Korea/Client_USA/EntitiesMP/FixedCameraField.es"
{
#line 43 "E:/질산/Korea/Client_USA/EntitiesMP/FixedCameraField.es"
RemoveCameraField  (this );
#line 44 "E:/질산/Korea/Client_USA/EntitiesMP/FixedCameraField.es"
}
  
#line 46 "E:/질산/Korea/Client_USA/EntitiesMP/FixedCameraField.es"
const CTString & CFixedCameraField::GetDescription(void)const 
#line 47 "E:/질산/Korea/Client_USA/EntitiesMP/FixedCameraField.es"
{
#line 48 "E:/질산/Korea/Client_USA/EntitiesMP/FixedCameraField.es"
((CTString &) m_strDescription ) . PrintF  ("-><none>");
#line 49 "E:/질산/Korea/Client_USA/EntitiesMP/FixedCameraField.es"
return m_strDescription ;
#line 50 "E:/질산/Korea/Client_USA/EntitiesMP/FixedCameraField.es"
}
  
#line 51 "E:/질산/Korea/Client_USA/EntitiesMP/FixedCameraField.es"
SLONG CFixedCameraField::GetUsedMemory(void) 
#line 52 "E:/질산/Korea/Client_USA/EntitiesMP/FixedCameraField.es"
{
#line 53 "E:/질산/Korea/Client_USA/EntitiesMP/FixedCameraField.es"
SLONG  slUsedMemory  = sizeof  (CFixedCameraField ) - sizeof  (CRationalEntity ) + CRationalEntity  :: GetUsedMemory  ();
#line 54 "E:/질산/Korea/Client_USA/EntitiesMP/FixedCameraField.es"
slUsedMemory  += m_strName  . Length  ();
#line 55 "E:/질산/Korea/Client_USA/EntitiesMP/FixedCameraField.es"
slUsedMemory  += m_strDescription  . Length  ();
#line 56 "E:/질산/Korea/Client_USA/EntitiesMP/FixedCameraField.es"
return slUsedMemory ;
#line 57 "E:/질산/Korea/Client_USA/EntitiesMP/FixedCameraField.es"
}
BOOL CFixedCameraField::
#line 61 "E:/질산/Korea/Client_USA/EntitiesMP/FixedCameraField.es"
Main(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CFixedCameraField_Main
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CFixedCameraField::Main expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 64 "E:/질산/Korea/Client_USA/EntitiesMP/FixedCameraField.es"
InitAsEditorModel  ();
#line 65 "E:/질산/Korea/Client_USA/EntitiesMP/FixedCameraField.es"
SetPhysicsFlags  (EPF_MODEL_IMMATERIAL );
#line 66 "E:/질산/Korea/Client_USA/EntitiesMP/FixedCameraField.es"
SetCollisionFlags  (ECF_IMMATERIAL );
#line 69 "E:/질산/Korea/Client_USA/EntitiesMP/FixedCameraField.es"
GetModelObject  () -> StretchModel  (FLOAT3D (du  , dv  , dw ));
#line 70 "E:/질산/Korea/Client_USA/EntitiesMP/FixedCameraField.es"
SetModel  (MODEL_DOORCONTROLLER );
#line 71 "E:/질산/Korea/Client_USA/EntitiesMP/FixedCameraField.es"
SetModelMainTexture  (TEXTURE_DOORCONTROLLER );
#line 72 "E:/질산/Korea/Client_USA/EntitiesMP/FixedCameraField.es"
ModelChangeNotify  ();
#line 73 "E:/질산/Korea/Client_USA/EntitiesMP/FixedCameraField.es"
Return(STATE_CURRENT,EVoid());
#line 73 "E:/질산/Korea/Client_USA/EntitiesMP/FixedCameraField.es"
return TRUE; ASSERT(FALSE); return TRUE;};
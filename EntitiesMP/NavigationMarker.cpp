/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#line 2 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"

#include "StdH.h"
#include "EntitiesMP/Common/PathFinding.h"

#define MAX_TARGETS 6

#include <NavigationMarker.h>
#include <NavigationMarker_tables.h>
#line 11 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"

// info structure
static EntityInfo eiMarker = {
  EIBT_ROCK, 10.0f,
  0.0f, 1.0f, 0.0f,     // source (eyes)
  0.0f, 1.0f, 0.0f,     // target (body)
};


void CNavigationMarker::SetDefaultProperties(void) {
	m_strName = "Marker";
	m_fMarkerRange = 1.0f;
	m_penTarget0 = NULL;
	m_penTarget1 = NULL;
	m_penTarget2 = NULL;
	m_penTarget3 = NULL;
	m_penTarget4 = NULL;
	m_penTarget5 = NULL;
	CEntity::SetDefaultProperties();
}
 CNavigationMarker::CNavigationMarker(void) 
#line 47 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
{
#line 48 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
m_ppnNode  = NULL ;
#line 49 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
}
 CNavigationMarker:: ~ CNavigationMarker(void) 
#line 51 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
{
#line 52 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
ASSERT  (m_ppnNode  == NULL );
#line 53 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
}

#line 56 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
void CNavigationMarker::Read_t(CTStream * istr,BOOL bNetwork) 
#line 57 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
{
#line 58 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
CEntity  :: Read_t  (istr  , bNetwork );
#line 59 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
m_ppnNode  = NULL ;
#line 60 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
}

#line 62 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
CEntity * CNavigationMarker::GetTarget(void)const {return m_penTarget0 ;}

#line 65 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
void * CNavigationMarker::GetEntityInfo(void) {
#line 66 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
return & eiMarker ;
#line 67 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
}

#line 70 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
CPathNode * CNavigationMarker::GetPathNode(void) 
#line 71 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
{
#line 72 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
if(m_ppnNode  == NULL ){
#line 73 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
m_ppnNode  = new  CPathNode  (this );
#line 74 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
}
#line 76 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
return m_ppnNode ;
#line 77 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
}

#line 79 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
CEntityPointer & CNavigationMarker::TargetPointer(INDEX i) 
#line 80 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
{
#line 81 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
ASSERT  (i  >= 0 && i  < MAX_TARGETS );
#line 82 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
return (& m_penTarget0 ) [ i  ];
#line 83 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
}

#line 84 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
CNavigationMarker & CNavigationMarker::Target(INDEX i) 
#line 85 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
{
#line 86 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
return (CNavigationMarker  &) * TargetPointer  (i );
#line 87 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
}

#line 90 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
CNavigationMarker * CNavigationMarker::GetLink(INDEX i) 
#line 91 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
{
#line 92 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
for(INDEX iTarget  = 0;iTarget  < MAX_TARGETS ;iTarget  ++){
#line 93 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
CNavigationMarker  * penLink  = & Target  (iTarget );
#line 94 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
if(penLink  == NULL ){
#line 95 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
continue ;
#line 96 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
}
#line 97 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
if(iTarget  == i ){
#line 98 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
return penLink ;
#line 99 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
}
#line 100 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
}
#line 101 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
return NULL ;
#line 102 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
}

#line 104 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
const CTString & CNavigationMarker::GetDescription(void)const {
#line 105 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
return m_strName ;
#line 106 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
}

#line 109 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
BOOL CNavigationMarker::MovesByTargetedRoute(CTString & strTargetProperty)const {
#line 110 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
strTargetProperty  = "Target 0";
#line 111 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
return TRUE ;
#line 112 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
}

#line 114 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
BOOL CNavigationMarker::DropsMarker(CTFileName & fnmMarkerClass,CTString & strTargetProperty)const {
#line 115 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
fnmMarkerClass  = CTFILENAME  ("Classes\\NavigationMarker.ecl");
#line 116 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
strTargetProperty  = "Target 0";
#line 117 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
return TRUE ;
#line 118 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
}
BOOL CNavigationMarker::IsMarker(void)const {
#line 121 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
return TRUE ;
#line 122 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
}
BOOL CNavigationMarker::
#line 125 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
Main(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CNavigationMarker_Main
	ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CNavigationMarker::Main expects 'EVoid' as input!");	const EVoid &e = (const EVoid &)__eeInput;
#line 126 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
InitAsEditorModel  ();
#line 127 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
SetPhysicsFlags  (EPF_MODEL_IMMATERIAL );
#line 128 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
SetCollisionFlags  (ECF_IMMATERIAL );
#line 131 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
const FLOAT fSize  = 0.25f;
#line 132 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
GetModelObject  () -> StretchModel  (FLOAT3D (fSize  , fSize  , fSize ));
#line 133 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
SetModel  (MODEL_MARKER );
#line 134 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
ModelChangeNotify  ();
#line 135 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
SetModelMainTexture  (TEXTURE_MARKER );
#line 138 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
for(INDEX iTarget  = 0;iTarget  < MAX_TARGETS ;iTarget  ++){
#line 139 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
CEntityPointer & penTarget  = TargetPointer  (iTarget );
#line 140 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
if(penTarget  == NULL ){
#line 141 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
continue ;
#line 142 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
}
#line 145 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
if(! IsOfClass  (penTarget  , & CNavigationMarker_DLLClass )){
#line 147 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
penTarget  = NULL ;
#line 148 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
continue ;
#line 149 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
}
#line 151 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
CNavigationMarker  & nmOther  = (CNavigationMarker  &) * penTarget ;
#line 154 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
BOOL bFound  = FALSE ;
#line 155 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
for(INDEX iTarget2  = 0;iTarget2  < MAX_TARGETS ;iTarget2  ++){
#line 156 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
CEntityPointer & penTarget2  = nmOther  . TargetPointer  (iTarget2 );
#line 157 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
if(penTarget2  == this ){
#line 158 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
bFound  = TRUE ;
#line 159 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
break ;
#line 160 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
}
#line 161 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
}
#line 163 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
if(! bFound ){
#line 165 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
for(INDEX iTarget2  = 0;iTarget2  < MAX_TARGETS ;iTarget2  ++){
#line 166 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
CEntityPointer & penTarget2  = nmOther  . TargetPointer  (iTarget2 );
#line 167 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
if(penTarget2  == NULL ){
#line 168 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
penTarget2  = this ;
#line 169 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
break ;
#line 170 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
}
#line 171 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
}
#line 172 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
}
#line 173 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
}
#line 175 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
Return(STATE_CURRENT,EVoid());
#line 175 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/NavigationMarker.es"
return TRUE; ASSERT(FALSE); return TRUE;};
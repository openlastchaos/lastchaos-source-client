/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#line 2 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"

#include "StdH.h"

#include <EntitiesMP/ParticlesHolder.h>
#include <EntitiesMP/ParticlesHolder_tables.h>
CEntityEvent *EParticlesActivate::MakeCopy(void) { CEntityEvent *peeCopy = new EParticlesActivate(*this); return peeCopy;}
SLONG EParticlesActivate::GetSizeOf(void) { return sizeof(*this);}
EParticlesActivate::EParticlesActivate() : CEntityEvent(EVENTCODE_EParticlesActivate) {;
};
BOOL EParticlesActivate::CheckIDs(void) { return TRUE ; }
CEntityEvent *EParticlesDeactivate::MakeCopy(void) { CEntityEvent *peeCopy = new EParticlesDeactivate(*this); return peeCopy;}
SLONG EParticlesDeactivate::GetSizeOf(void) { return sizeof(*this);}
EParticlesDeactivate::EParticlesDeactivate() : CEntityEvent(EVENTCODE_EParticlesDeactivate) {;
};
BOOL EParticlesDeactivate::CheckIDs(void) { return TRUE ; }
void CParticlesHolder::SetDefaultProperties(void) {
  m_phtType = PHT_SPIRAL ;
  m_ptTexture = PT_STAR01 ;
  m_ctCount = 16;
  m_fStretchAll = 1.0f;
  m_fStretchX = 1.0f;
  m_fStretchY = 1.0f;
  m_fStretchZ = 1.0f;
  m_strName = "";
  m_strDescription = "";
  m_bBackground = FALSE ;
  m_bTargetable = FALSE ;
  m_fSize = 0.1f;
  m_fParam1 = 1.0f;
  m_fParam2 = 1.0f;
  m_fParam3 = 1.0f;
  m_bActive = TRUE ;
  m_fActivateTime = 0.0f;
  m_fDeactivateTime = -10000.0f;
  m_fMipFactorDisappear = 8.0f;
  m_cColor = 0xFFFFFFFF;
  CMovableModelEntity::SetDefaultProperties();
}
  
#line 72 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
void CParticlesHolder::RenderParticles(void) 
#line 73 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
{
#line 74 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
if(! m_bActive )
#line 75 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
{
#line 76 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
return ;
#line 77 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
}
#line 78 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
switch(m_phtType )
#line 79 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
{
#line 80 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
case PHT_SPIRAL : 
#line 81 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
Particles_Spiral  (this  , m_fStretchAll  , m_fStretchAll  / 2 , m_ptTexture  , m_ctCount  , m_fParam2 );
#line 82 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
break ;
#line 83 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
case PHT_EMANATE : 
#line 84 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
Particles_Emanate  (this  , m_fStretchAll  , m_fStretchAll  / 2 , m_ptTexture  , m_ctCount  , m_fMipFactorDisappear  , m_cColor  , m_fParam1  , m_fParam2  , m_fParam3 );
#line 85 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
break ;
#line 86 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
case PHT_STARDUST : 
#line 87 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
Particles_Stardust  (this  , m_fStretchAll  , m_fStretchAll  / 2 , m_ptTexture  , m_ctCount );
#line 88 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
break ;
#line 89 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
case PHT_ATOMIC : 
#line 90 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
Particles_Atomic  (this  , m_fStretchAll  , m_fStretchAll  / 2 , m_ptTexture  , m_ctCount );
#line 91 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
break ;
#line 92 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
case PHT_RISING : 
#line 93 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
Particles_Rising  (this  , m_fActivateTime  , m_fDeactivateTime  , m_fStretchAll  , m_fStretchX  , m_fStretchY  , m_fStretchZ  , m_fSize  , m_ptTexture  , m_ctCount );
#line 94 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
break ;
#line 95 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
case PHT_FOUNTAIN : 
#line 96 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
Particles_Fountain  (this  , m_fStretchAll  , m_fStretchAll  / 2 , m_ptTexture  , m_ctCount );
#line 97 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
break ;
#line 98 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
case PHT_SMOKE : 
#line 99 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
Particles_GrenadeTrail  (this );
#line 100 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
break ;
#line 101 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
case PHT_BLOOD : 
#line 102 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
Particles_BloodTrail  (this );
#line 103 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
break ;
#line 104 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
case PHT_EMANATEPLANE : 
#line 105 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
Particles_EmanatePlane  (this  , 
#line 106 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
m_fStretchX  , m_fStretchY  , m_fStretchZ  , m_fSize  , 
#line 107 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
m_fParam1  , m_fParam2  , m_ptTexture  , m_ctCount  , m_fMipFactorDisappear );
#line 108 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
break ;
#line 109 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
case PHT_SANDFLOW : 
#line 110 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
Particles_SandFlow  (this  , m_fStretchAll  , m_fSize  , m_fParam1  , m_fActivateTime  , m_fDeactivateTime  , m_ctCount );
#line 111 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
break ;
#line 112 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
case PHT_WATERFLOW : 
#line 113 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
Particles_WaterFlow  (this  , m_fStretchAll  , m_fSize  , m_fParam1  , m_fActivateTime  , m_fDeactivateTime  , m_ctCount );
#line 114 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
break ;
#line 115 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
case PHT_LAVAFLOW : 
#line 116 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
Particles_LavaFlow  (this  , m_fStretchAll  , m_fSize  , m_fParam1  , m_fActivateTime  , m_fDeactivateTime  , m_ctCount );
#line 117 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
break ;
#line 118 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
case PHT_LAVAERUPTING : 
#line 119 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
Particles_LavaErupting  (this  , m_fStretchAll  , m_fSize  , m_fStretchX  , m_fStretchY  , m_fStretchZ  , m_fActivateTime );
#line 120 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
break ;
#line 121 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
case PHT_WATERFALLFOAM : 
#line 122 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
Particles_WaterfallFoam  (this  , 
#line 123 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
m_fStretchX  , m_fStretchY  , m_fStretchZ  , m_fSize  , 
#line 124 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
m_fParam1  , m_fParam2  , m_fParam3  , m_ctCount );
#line 125 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
break ;
#line 126 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
case PHT_CHIMNEYSMOKE : 
#line 127 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
Particles_ChimneySmoke  (this  , m_ctCount  , m_fStretchAll  , m_fMipFactorDisappear );
#line 128 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
break ;
#line 129 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
case PHT_ROCKETMOTOR : 
#line 130 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
Particles_RocketMotorBurning  (this  , m_ctCount  , 
#line 131 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
FLOAT3D (m_fStretchX  , m_fStretchY  , m_fStretchZ ) , m_fSize  , m_ctCount );
#line 132 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
break ;
#line 144 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
case PHT_WATERFALL : 
#line 145 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
Particles_Waterfall  (this  , m_ctCount  , m_fStretchAll  , m_fStretchX  , m_fStretchY  , m_fStretchZ  , 
#line 146 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
m_fSize  , m_fMipFactorDisappear  , m_fParam1  , m_fParam2  , m_cColor );
#line 147 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
break ;
#line 148 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
}
#line 149 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
}
  
#line 150 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
BOOL CParticlesHolder::IsTargetable(void)const 
#line 151 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
{
#line 152 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
return m_bTargetable ;
#line 153 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
}
  
#line 156 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
void CParticlesHolder::MirrorAndStretch(FLOAT fStretch,BOOL bMirrorX) 
#line 157 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
{
#line 158 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
m_fStretchAll  *= fStretch ;
#line 159 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
if(bMirrorX ){
#line 160 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
m_fStretchX  = - m_fStretchX ;
#line 161 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
}
#line 162 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
}
  
#line 165 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
void CParticlesHolder::StretchModel(void) {
#line 167 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
if(Abs  (m_fStretchX ) < 0.01f){m_fStretchX  = 0.01f;}
#line 168 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
if(Abs  (m_fStretchY ) < 0.01f){m_fStretchY  = 0.01f;}
#line 169 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
if(Abs  (m_fStretchZ ) < 0.01f){m_fStretchZ  = 0.01f;}
#line 170 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
if(m_fStretchAll  < 0.01f){m_fStretchAll  = 0.01f;}
#line 172 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
if(Abs  (m_fStretchX ) > 100.0f){m_fStretchX  = 100.0f * Sgn  (m_fStretchX );}
#line 173 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
if(Abs  (m_fStretchY ) > 100.0f){m_fStretchY  = 100.0f * Sgn  (m_fStretchY );}
#line 174 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
if(Abs  (m_fStretchZ ) > 100.0f){m_fStretchZ  = 100.0f * Sgn  (m_fStretchZ );}
#line 175 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
if(m_fStretchAll  > 100.0f){m_fStretchAll  = 100.0f;}
#line 177 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
GetModelObject  () -> StretchModel  (FLOAT3D (
#line 178 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
m_fStretchAll  * m_fStretchX  , 
#line 179 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
m_fStretchAll  * m_fStretchY  , 
#line 180 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
m_fStretchAll  * m_fStretchZ ));
#line 181 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
ModelChangeNotify  ();
#line 182 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
}
  
#line 184 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
CPlacement3D CParticlesHolder::GetLerpedPlacement(void)const 
#line 185 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
{
#line 186 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
return CEntity  :: GetLerpedPlacement  ();
#line 187 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
}
  
#line 191 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
SLONG CParticlesHolder::GetUsedMemory(void) 
#line 192 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
{
#line 194 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
SLONG  slUsedMemory  = sizeof  (CParticlesHolder ) - sizeof  (CMovableModelEntity ) + CMovableModelEntity  :: GetUsedMemory  ();
#line 196 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
slUsedMemory  += m_strName  . Length  ();
#line 197 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
slUsedMemory  += m_strDescription  . Length  ();
#line 198 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
return slUsedMemory ;
#line 199 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
}
BOOL CParticlesHolder::
#line 206 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
Active(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CParticlesHolder_Active
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CParticlesHolder::Active expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 208 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
m_bActive  = TRUE ;
#line 209 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
Jump(STATE_CURRENT,0x00df0005, FALSE, EInternal());return TRUE;}BOOL CParticlesHolder::H0x00df0005_Active_03(const CEntityEvent &__eeInput)
#line 210 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
{
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00df0005
if(!(TRUE )){ Jump(STATE_CURRENT,0x00df0006, FALSE, EInternal());return TRUE;}
#line 212 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
SetTimerAfter(m_fParam2  + FRnd  () * m_fParam3 );
Jump(STATE_CURRENT, 0x00df0003, FALSE, EBegin());return TRUE;}BOOL CParticlesHolder::H0x00df0003_Active_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00df0003
switch(__eeInput.ee_slEvent)
#line 213 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
{case(EVENTCODE_ETimer):
#line 215 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
{const ETimer&e= (ETimer&)__eeInput;

#line 217 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
if(m_phtType  == PHT_LAVAERUPTING )
#line 218 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
{
#line 220 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
m_fActivateTime  = _pTimer  -> CurrentTick  ();
#line 221 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
}
#line 222 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
UnsetTimer();Jump(STATE_CURRENT,0x00df0004, FALSE, EInternal());return TRUE;
#line 223 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
}ASSERT(FALSE);break;case(EVENTCODE_EBegin):
#line 225 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
{const EBegin&e= (EBegin&)__eeInput;

#line 226 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
return TRUE;
#line 227 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
}ASSERT(FALSE);break;case(EVENTCODE_EDeactivate):
#line 229 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
{const EDeactivate&e= (EDeactivate&)__eeInput;

#line 230 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
if(_pNetwork  -> IsServer  ()){
#line 231 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
SendEvent  (EParticlesDeactivate  () , TRUE );
#line 232 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
}
#line 233 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
return TRUE;
#line 234 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
}ASSERT(FALSE);break;case(EVENTCODE_EParticlesDeactivate):
#line 236 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
{const EParticlesDeactivate&e= (EParticlesDeactivate&)__eeInput;

#line 237 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
m_fDeactivateTime  = _pTimer  -> CurrentTick  ();
#line 238 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
Jump(STATE_CURRENT, STATE_CParticlesHolder_Inactive, TRUE, EVoid());return TRUE;
#line 239 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
}ASSERT(FALSE);break;default: return FALSE; break;
#line 240 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
}return TRUE;}BOOL CParticlesHolder::H0x00df0004_Active_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00df0004
Jump(STATE_CURRENT,0x00df0005, FALSE, EInternal());return TRUE;
#line 241 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
}BOOL CParticlesHolder::H0x00df0006_Active_04(const CEntityEvent &__eeInput) {
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00df0006
 ASSERT(FALSE); return TRUE;};BOOL CParticlesHolder::
#line 245 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
Inactive(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CParticlesHolder_Inactive
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CParticlesHolder::Inactive expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 246 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
m_bActive  = FALSE ;
#line 247 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
SetTimerAt(THINKTIME_NEVER);
Jump(STATE_CURRENT, 0x00df0008, FALSE, EBegin());return TRUE;}BOOL CParticlesHolder::H0x00df0008_Inactive_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00df0008
switch(__eeInput.ee_slEvent)
#line 248 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
{case(EVENTCODE_EBegin):{const EBegin&e= (EBegin&)__eeInput;
return TRUE;}ASSERT(FALSE);break;case(EVENTCODE_EActivate):
#line 251 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
{const EActivate&e= (EActivate&)__eeInput;

#line 252 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
if(_pNetwork  -> IsServer  ()){
#line 253 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
SendEvent  (EParticlesActivate  () , TRUE );
#line 254 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
}
#line 255 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
return TRUE;
#line 256 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
}ASSERT(FALSE);break;case(EVENTCODE_EParticlesActivate):
#line 258 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
{const EParticlesActivate&e= (EParticlesActivate&)__eeInput;

#line 259 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
m_fActivateTime  = _pTimer  -> CurrentTick  ();
#line 260 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
m_fDeactivateTime  = _pTimer  -> CurrentTick  () + 10000.0f;
#line 261 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
Jump(STATE_CURRENT, STATE_CParticlesHolder_Active, TRUE, EVoid());return TRUE;
#line 262 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
}ASSERT(FALSE);break;default: return FALSE; break;
#line 263 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
}return TRUE;}BOOL CParticlesHolder::H0x00df0009_Inactive_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00df0009
 ASSERT(FALSE); return TRUE;};BOOL CParticlesHolder::
#line 266 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
Main(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CParticlesHolder_Main
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CParticlesHolder::Main expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 268 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
InitAsEditorModel  ();
#line 269 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
SetPhysicsFlags  (EPF_MODEL_IMMATERIAL );
#line 270 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
SetCollisionFlags  (ECF_IMMATERIAL );
#line 271 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
SetFlagOn  (ENF_CLIENTHANDLING );
#line 274 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
StretchModel  ();
#line 275 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
SetModel  (MODEL_TELEPORT );
#line 276 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
ModelChangeNotify  ();
#line 277 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
SetModelMainTexture  (TEXTURE_TELEPORT );
#line 279 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
if(m_bBackground ){
#line 280 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
SetFlags  (GetFlags  () | ENF_BACKGROUND );
#line 281 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
}else {
#line 282 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
SetFlags  (GetFlags  () & ~ ENF_BACKGROUND );
#line 283 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
}
#line 285 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
en_fGravityA  = 30.0f;
#line 286 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
GetPitchDirection  (- 90.0f , en_vGravityDir );
#line 288 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
m_fActivateTime  = 0.0f;
#line 289 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
m_fDeactivateTime  = - 10000.0f;
#line 291 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
if(m_bActive ){
#line 292 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
Jump(STATE_CURRENT, STATE_CParticlesHolder_Active, TRUE, EVoid());return TRUE;
#line 293 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
}else {
#line 294 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
Jump(STATE_CURRENT, STATE_CParticlesHolder_Inactive, TRUE, EVoid());return TRUE;
#line 295 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
}
#line 298 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
Return(STATE_CURRENT,EVoid());
#line 298 "E:/질산/Korea/Client_USA/EntitiesMP/ParticlesHolder.es"
return TRUE; ASSERT(FALSE); return TRUE;};
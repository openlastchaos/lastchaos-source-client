/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#ifndef _EnvironmentBase_INCLUDED
#define _EnvironmentBase_INCLUDED 1
#include <EntitiesMP/EnvironmentMarker.h>
#include <EntitiesMP/WatchPlayers.h>
extern "C" DECL_DLL CDLLEntityClass CEnvironmentBase_DLLClass;
class CEnvironmentBase : public CMovableEntity {
public:
virtual const CTString &GetName(void) const { return m_strName; };
virtual BOOL IsTargetable(void) const { return TRUE; };
	DECL_DLL virtual void SetDefaultProperties(void);
	CTString m_strName;
	CTString m_strDescription;
	RANGE m_fDistance;
	FLOAT m_fStretch;
	CEntityPointer m_penTarget;
	CEntityPointer m_penWatcher;
	FLOAT m_fWatcherFrequency;
	FLOAT3D m_vDesiredPosition;
	FLOAT m_fMoveSpeed;
	FLOAT m_fRotateSpeed;
	FLOAT m_fMoveFrequency;
	BOOL m_bUseWatcher;
	BOOL m_bFlying;
	FLOAT m_fWaitTime;
	CTFileName m_fnMdl;
	CTFileName m_fnTex;
	ANIMATION m_iAnim;
	CTFileName m_fnAtt1Mdl;
	CTFileName m_fnAtt1Tex;
	INDEX m_iAtt1Position;
	ANIMATION m_iAtt1Anim;
	CTFileName m_fnAtt2Mdl;
	CTFileName m_fnAtt2Tex;
	INDEX m_iAtt2Position;
	ANIMATION m_iAtt2Anim;
	CTFileName m_fnAtt3Mdl;
	CTFileName m_fnAtt3Tex;
	INDEX m_iAtt3Position;
	ANIMATION m_iAtt3Anim;
   
#line 56 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnvironmentBase.es"
BOOL MovesByTargetedRoute(CTString & strTargetProperty)const;
   
#line 61 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnvironmentBase.es"
BOOL DropsMarker(CTFileName & fnmMarkerClass,CTString & strTargetProperty)const;
   
#line 66 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnvironmentBase.es"
const CTString & GetDescription(void)const;
   
#line 74 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnvironmentBase.es"
CAnimData * GetAnimData(SLONG slPropertyOffset);
   
#line 104 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnvironmentBase.es"
BOOL NextMarker(void);
   
#line 132 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnvironmentBase.es"
void CalcRotation(ANGLE aWantedHeadingRelative,ANGLE3D & aRotation);
   
#line 151 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnvironmentBase.es"
void StopMoving(void);
   
#line 157 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnvironmentBase.es"
void MoveToPosition(void);
   
#line 181 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnvironmentBase.es"
void CalcDestination();
   
#line 190 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnvironmentBase.es"
void MarkerParameters();
   
#line 207 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnvironmentBase.es"
void Initialize(void);
   
#line 259 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnvironmentBase.es"
void InitializeWatcher(FLOAT fWaitTime);
   
#line 280 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnvironmentBase.es"
void PlayDefaultAnim(void);
   
#line 285 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnvironmentBase.es"
void PlayMarkerAnimation(void);
   
#line 292 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnvironmentBase.es"
void ChangeDefaultAnim(void);
   
#line 299 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnvironmentBase.es"
void WaitOnMarker(void);
#define  STATE_CEnvironmentBase_Main 1
	BOOL 
#line 315 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnvironmentBase.es"
Main(const CEntityEvent &__eeInput);
	BOOL H0x03840000_Main_01(const CEntityEvent &__eeInput);
	BOOL H0x03840001_Main_02(const CEntityEvent &__eeInput);
#define  STATE_CEnvironmentBase_MoveToMarker 0x03840002
	BOOL 
#line 336 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnvironmentBase.es"
MoveToMarker(const CEntityEvent &__eeInput);
	BOOL H0x03840003_MoveToMarker_01(const CEntityEvent &__eeInput);
	BOOL H0x03840004_MoveToMarker_02(const CEntityEvent &__eeInput);
	BOOL H0x03840005_MoveToMarker_03(const CEntityEvent &__eeInput);
	BOOL H0x03840006_MoveToMarker_04(const CEntityEvent &__eeInput);
	BOOL H0x03840007_MoveToMarker_05(const CEntityEvent &__eeInput);
#define  STATE_CEnvironmentBase_Activate 0x03840008
	BOOL 
#line 363 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnvironmentBase.es"
Activate(const CEntityEvent &__eeInput);
	BOOL H0x03840009_Activate_01(const CEntityEvent &__eeInput);
	BOOL H0x0384000a_Activate_02(const CEntityEvent &__eeInput);
#define  STATE_CEnvironmentBase_Stop 0x0384000b
	BOOL 
#line 371 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnvironmentBase.es"
Stop(const CEntityEvent &__eeInput);
	BOOL H0x0384000c_Stop_01(const CEntityEvent &__eeInput);
	BOOL H0x0384000d_Stop_02(const CEntityEvent &__eeInput);
#define  STATE_CEnvironmentBase_DoAction 0x0384000e
	BOOL 
#line 380 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnvironmentBase.es"
DoAction(const CEntityEvent &__eeInput);
	BOOL H0x0384000f_DoAction_01(const CEntityEvent &__eeInput);
	BOOL H0x03840010_DoAction_02(const CEntityEvent &__eeInput);
	BOOL H0x03840011_DoAction_03(const CEntityEvent &__eeInput);
	BOOL H0x03840012_DoAction_04(const CEntityEvent &__eeInput);
	BOOL H0x03840013_DoAction_05(const CEntityEvent &__eeInput);
	BOOL H0x03840014_DoAction_06(const CEntityEvent &__eeInput);
	BOOL H0x03840015_DoAction_07(const CEntityEvent &__eeInput);
	BOOL H0x03840016_DoAction_08(const CEntityEvent &__eeInput);
	BOOL H0x03840017_DoAction_09(const CEntityEvent &__eeInput);
	BOOL H0x03840018_DoAction_10(const CEntityEvent &__eeInput);
#define  STATE_CEnvironmentBase_MainLoop 0x03840019
	BOOL 
#line 406 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/EnvironmentBase.es"
MainLoop(const CEntityEvent &__eeInput);
	BOOL H0x0384001a_MainLoop_01(const CEntityEvent &__eeInput);
	BOOL H0x0384001b_MainLoop_02(const CEntityEvent &__eeInput);
};
#endif // _EnvironmentBase_INCLUDED
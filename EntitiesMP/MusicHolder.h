/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#ifndef _EntitiesMP_MusicHolder_INCLUDED
#define _EntitiesMP_MusicHolder_INCLUDED 1
extern DECL_DLL CEntityPropertyEnumType MusicType_enum;
enum MusicType {
  MT_LIGHT = 0,
  MT_MEDIUM = 1,
  MT_HEAVY = 2,
  MT_EVENT = 3,
  MT_CONTINUOUS = 4,
};
DECL_DLL inline void ClearToDefault(MusicType &e) { e = (MusicType)0; } ;
#define EVENTCODE_EChangeMusic 0x00de0000
class DECL_DLL EChangeMusic : public CEntityEvent {
public:
EChangeMusic();
CEntityEvent *MakeCopy(void);
BOOL CheckIDs(void);
SLONG GetSizeOf(void);
enum MusicType mtType;
CTFileName fnMusic;
FLOAT fVolume;
BOOL bForceStart;
};
DECL_DLL inline void ClearToDefault(EChangeMusic &e) { e = EChangeMusic(); } ;
#define EVENTCODE_ENetChangeMusic 0x00de0001
class DECL_DLL ENetChangeMusic : public CEntityEvent {
public:
ENetChangeMusic();
CEntityEvent *MakeCopy(void);
BOOL CheckIDs(void);
SLONG GetSizeOf(void);
enum MusicType mtType;
INDEX iBossID;
INDEX iCounterID;
};
DECL_DLL inline void ClearToDefault(ENetChangeMusic &e) { e = ENetChangeMusic(); } ;
extern "C" DECL_DLL CDLLEntityClass CMusicHolder_DLLClass;
class CMusicHolder : public CRationalEntity {
public:
virtual const CTString &GetName(void) const { return m_strName; };
virtual BOOL IsTargetable(void) const { return TRUE; };
virtual BOOL IsImportant(void) const { return TRUE; };
  DECL_DLL virtual void SetDefaultProperties(void);
  CTString m_strName;
  FLOAT m_fScoreMedium;
  FLOAT m_fScoreHeavy;
  CTFileName m_fnMusic0;
  CTFileName m_fnMusic1;
  CTFileName m_fnMusic2;
  CTFileName m_fnMusic3;
  CTFileName m_fnMusic4;
  FLOAT m_fVolume0;
  FLOAT m_fVolume1;
  FLOAT m_fVolume2;
  FLOAT m_fVolume3;
  FLOAT m_fVolume4;
  CEntityPointer m_penBoss;
  CEntityPointer m_penCounter;
  INDEX m_ctEnemiesInWorld;
  INDEX m_iRespawnMarker;
  INDEX m_ctSecretsInWorld;
  FLOAT m_tmFade;
  enum MusicType m_mtCurrentMusic;
  INDEX m_ctTreasuresInWorld;
  FLOAT m_fCurrentVolume0a;
  FLOAT m_fCurrentVolume0b;
  FLOAT m_fCurrentVolume1a;
  FLOAT m_fCurrentVolume1b;
  FLOAT m_fCurrentVolume2a;
  FLOAT m_fCurrentVolume2b;
  FLOAT m_fCurrentVolume3a;
  FLOAT m_fCurrentVolume3b;
  FLOAT m_fCurrentVolume4a;
  FLOAT m_fCurrentVolume4b;
  CSoundObject m_soMusic0a;
  CSoundObject m_soMusic0b;
  CSoundObject m_soMusic1a;
  CSoundObject m_soMusic1b;
  CSoundObject m_soMusic2a;
  CSoundObject m_soMusic2b;
  CSoundObject m_soMusic3a;
  CSoundObject m_soMusic3b;
  CSoundObject m_soMusic4a;
  CSoundObject m_soMusic4b;
  INDEX m_iSubChannel0;
  INDEX m_iSubChannel1;
  INDEX m_iSubChannel2;
  INDEX m_iSubChannel3;
  INDEX m_iSubChannel4;
  FLOAT m_tmLastUpdate;
  INDEX m_iFirstTime;
CDynamicContainer < CEntity > m_cenFussMakers;
CStaticStackArray < ULONG > m_aulAutoAimables;
CAutoPrecacheSound m_aps0;
CAutoPrecacheSound m_aps1;
CAutoPrecacheSound m_aps2;
   
#line 134 "E:/USA/EntitiesMP/MusicHolder.es"
void Precache(void);
   
#line 148 "E:/USA/EntitiesMP/MusicHolder.es"
void CountEnemies(void);
   
#line 217 "E:/USA/EntitiesMP/MusicHolder.es"
void CheckOldFussMakers(void);
   
#line 240 "E:/USA/EntitiesMP/MusicHolder.es"
INDEX GetFussMakersScore(void);
   
#line 250 "E:/USA/EntitiesMP/MusicHolder.es"
void ChangeMusicChannel(enum MusicType mtType,const CTFileName & fnNewMusic,FLOAT fNewVolume);
   
#line 301 "E:/USA/EntitiesMP/MusicHolder.es"
void FadeOutChannel(INDEX iChannel,INDEX iSubChannel);
   
#line 326 "E:/USA/EntitiesMP/MusicHolder.es"
void FadeInChannel(INDEX iChannel,INDEX iSubChannel);
   
#line 349 "E:/USA/EntitiesMP/MusicHolder.es"
void CrossFadeOneChannel(enum MusicType mtType);
#define  STATE_CMusicHolder_Main 1
  BOOL 
#line 370 "E:/USA/EntitiesMP/MusicHolder.es"
Main(const CEntityEvent &__eeInput);
  BOOL H0x00de0002_Main_01(const CEntityEvent &__eeInput);
  BOOL H0x00de0003_Main_02(const CEntityEvent &__eeInput);
  BOOL H0x00de0004_Main_03(const CEntityEvent &__eeInput);
  BOOL H0x00de0005_Main_04(const CEntityEvent &__eeInput);
  BOOL H0x00de0006_Main_05(const CEntityEvent &__eeInput);
  BOOL H0x00de0007_Main_06(const CEntityEvent &__eeInput);
  BOOL H0x00de0008_Main_07(const CEntityEvent &__eeInput);
  BOOL H0x00de0009_Main_08(const CEntityEvent &__eeInput);
};
#endif // _EntitiesMP_MusicHolder_INCLUDED

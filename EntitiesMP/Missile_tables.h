/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

EP_ENUMBEG(MissileType)
  EP_ENUMVALUE(MLT_NORMALARROW, "NormalArrow"),
  EP_ENUMVALUE(MLT_SOMETHING, "Something"),
  EP_ENUMVALUE(MLT_FALLDOWN, "FallDown"),
  EP_ENUMVALUE(MLT_GOWAITBACK, "Go, Wait, Back"),
  EP_ENUMVALUE(MLT_CONNECT, "Connect"),
EP_ENUMEND(MissileType);

CDLLEntityEvent *CMissile_events[] = {NULL};
#define CMissile_eventsct 0
#define ENTITYCLASS CMissile

CEntityProperty CMissile_properties[] = {
 CEntityProperty(CEntityProperty::EPT_BOOL, NULL, (0x000003ed<<8)+1, offsetof(CMissile, m_bInit), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_ENUM, &MissileType_enum, (0x000003ed<<8)+2, offsetof(CMissile, m_eMissileType), "Missile Type", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_BOOL, NULL, (0x000003ed<<8)+14, offsetof(CMissile, m_bEntityTarget), "Entity Target", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_ENTITYPTR, NULL, (0x000003ed<<8)+3, offsetof(CMissile, m_epTarget), "Target", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_ENTITYPTR, NULL, (0x000003ed<<8)+17, offsetof(CMissile, m_epSource), "Source", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT3D, NULL, (0x000003ed<<8)+15, offsetof(CMissile, m_vTargetPos), "Target Pos", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000003ed<<8)+4, offsetof(CMissile, m_fMoveSpeed), "Move Speed", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000003ed<<8)+5, offsetof(CMissile, m_fBeforeDelay), "Delay Before Fire", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000003ed<<8)+6, offsetof(CMissile, m_fAfterDelay), "Delay After Fire", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_STRING, NULL, (0x000003ed<<8)+7, offsetof(CMissile, m_strHitEffect), "Hit Effect Name", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_STRING, NULL, (0x000003ed<<8)+18, offsetof(CMissile, m_strMoveEffect), "Move Effect Name", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FILENAME, NULL, (0x000003ed<<8)+8, offsetof(CMissile, m_fnModel), "Model file (.smc)", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000003ed<<8)+9, offsetof(CMissile, m_fHorizonalOffset), "Horizonal Offset", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000003ed<<8)+10, offsetof(CMissile, m_fVerticalOffset), "Vertical Offset", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_SOUNDOBJECT, NULL, (0x000003ed<<8)+11, offsetof(CMissile, m_soSound), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_BOOL, NULL, (0x000003ed<<8)+12, offsetof(CMissile, m_bCritical), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_ANGLE, NULL, (0x000003ed<<8)+13, offsetof(CMissile, m_aAngleSpeed), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_BOOL, NULL, (0x000003ed<<8)+16, offsetof(CMissile, m_bDirectTag), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x000003ed<<8)+19, offsetof(CMissile, m_iFireObjectType), "", 0, 0, 0),
};
#define CMissile_propertiesct ARRAYCOUNT(CMissile_properties)

CEntityComponent CMissile_components[] = {
#define SOUND_BOW_BLOW ((0x000003ed<<8)+1)
 CEntityComponent(ECT_SOUND, SOUND_BOW_BLOW, 0, "EFNM" "Data\\Sounds\\Character\\public\\C_blow_bow01.wav"),
#define SOUND_BOW_CRITICAL ((0x000003ed<<8)+2)
 CEntityComponent(ECT_SOUND, SOUND_BOW_CRITICAL, 0, "EFNM" "Data\\Sounds\\Character\\public\\C_critical_blow_bow01.wav"),
#define SOUND_CROSSBOW_BLOW ((0x000003ed<<8)+3)
 CEntityComponent(ECT_SOUND, SOUND_CROSSBOW_BLOW, 0, "EFNM" "Data\\Sounds\\Character\\public\\C_blow_cross01.wav"),
};
#define CMissile_componentsct ARRAYCOUNT(CMissile_components)

CEventHandlerEntry CMissile_handlers[] = {
 {0x03ed0000, -1, CEntity::pEventHandler(&CMissile::
#line 780 "E:/USA/EntitiesMP/Missile.es"
PreBehavior),DEBUGSTRING("CMissile::PreBehavior")},
 {0x03ed0001, -1, CEntity::pEventHandler(&CMissile::H0x03ed0001_PreBehavior_01), DEBUGSTRING("CMissile::H0x03ed0001_PreBehavior_01")},
 {0x03ed0002, -1, CEntity::pEventHandler(&CMissile::H0x03ed0002_PreBehavior_02), DEBUGSTRING("CMissile::H0x03ed0002_PreBehavior_02")},
 {0x03ed0003, -1, CEntity::pEventHandler(&CMissile::H0x03ed0003_PreBehavior_03), DEBUGSTRING("CMissile::H0x03ed0003_PreBehavior_03")},
 {0x03ed0004, -1, CEntity::pEventHandler(&CMissile::H0x03ed0004_PreBehavior_04), DEBUGSTRING("CMissile::H0x03ed0004_PreBehavior_04")},
 {0x03ed0005, -1, CEntity::pEventHandler(&CMissile::
#line 796 "E:/USA/EntitiesMP/Missile.es"
PostBehavior),DEBUGSTRING("CMissile::PostBehavior")},
 {0x03ed0006, -1, CEntity::pEventHandler(&CMissile::H0x03ed0006_PostBehavior_01), DEBUGSTRING("CMissile::H0x03ed0006_PostBehavior_01")},
 {0x03ed0007, -1, CEntity::pEventHandler(&CMissile::H0x03ed0007_PostBehavior_02), DEBUGSTRING("CMissile::H0x03ed0007_PostBehavior_02")},
 {0x03ed0008, -1, CEntity::pEventHandler(&CMissile::H0x03ed0008_PostBehavior_03), DEBUGSTRING("CMissile::H0x03ed0008_PostBehavior_03")},
 {0x03ed0009, -1, CEntity::pEventHandler(&CMissile::H0x03ed0009_PostBehavior_04), DEBUGSTRING("CMissile::H0x03ed0009_PostBehavior_04")},
 {0x03ed000a, -1, CEntity::pEventHandler(&CMissile::
#line 947 "E:/USA/EntitiesMP/Missile.es"
DoBehavior),DEBUGSTRING("CMissile::DoBehavior")},
 {0x03ed000b, -1, CEntity::pEventHandler(&CMissile::H0x03ed000b_DoBehavior_01), DEBUGSTRING("CMissile::H0x03ed000b_DoBehavior_01")},
 {0x03ed000c, -1, CEntity::pEventHandler(&CMissile::H0x03ed000c_DoBehavior_02), DEBUGSTRING("CMissile::H0x03ed000c_DoBehavior_02")},
 {0x03ed000d, -1, CEntity::pEventHandler(&CMissile::H0x03ed000d_DoBehavior_03), DEBUGSTRING("CMissile::H0x03ed000d_DoBehavior_03")},
 {0x03ed000e, -1, CEntity::pEventHandler(&CMissile::H0x03ed000e_DoBehavior_04), DEBUGSTRING("CMissile::H0x03ed000e_DoBehavior_04")},
 {0x03ed000f, -1, CEntity::pEventHandler(&CMissile::
#line 977 "E:/USA/EntitiesMP/Missile.es"
Connect),DEBUGSTRING("CMissile::Connect")},
 {0x03ed0010, -1, CEntity::pEventHandler(&CMissile::H0x03ed0010_Connect_01), DEBUGSTRING("CMissile::H0x03ed0010_Connect_01")},
 {0x03ed0011, -1, CEntity::pEventHandler(&CMissile::H0x03ed0011_Connect_02), DEBUGSTRING("CMissile::H0x03ed0011_Connect_02")},
 {0x03ed0012, -1, CEntity::pEventHandler(&CMissile::H0x03ed0012_Connect_03), DEBUGSTRING("CMissile::H0x03ed0012_Connect_03")},
 {0x03ed0013, -1, CEntity::pEventHandler(&CMissile::H0x03ed0013_Connect_04), DEBUGSTRING("CMissile::H0x03ed0013_Connect_04")},
 {0x03ed0014, -1, CEntity::pEventHandler(&CMissile::H0x03ed0014_Connect_05), DEBUGSTRING("CMissile::H0x03ed0014_Connect_05")},
 {0x03ed0015, -1, CEntity::pEventHandler(&CMissile::H0x03ed0015_Connect_06), DEBUGSTRING("CMissile::H0x03ed0015_Connect_06")},
 {0x03ed0016, -1, CEntity::pEventHandler(&CMissile::H0x03ed0016_Connect_07), DEBUGSTRING("CMissile::H0x03ed0016_Connect_07")},
 {0x03ed0017, -1, CEntity::pEventHandler(&CMissile::H0x03ed0017_Connect_08), DEBUGSTRING("CMissile::H0x03ed0017_Connect_08")},
 {1, -1, CEntity::pEventHandler(&CMissile::
#line 1023 "E:/USA/EntitiesMP/Missile.es"
Main),DEBUGSTRING("CMissile::Main")},
};
#define CMissile_handlersct ARRAYCOUNT(CMissile_handlers)

CEntity *CMissile_New(void) { return new CMissile; };
void CMissile_OnInitClass(void) {};
void CMissile_OnEndClass(void) {};
void CMissile_OnPrecache(CDLLEntityClass *pdec, INDEX iUser) {};
void CMissile_OnWorldEnd(CWorld *pwo) {};
void CMissile_OnWorldInit(CWorld *pwo) {};
void CMissile_OnWorldTick(CWorld *pwo) {};
void CMissile_OnWorldRender(CWorld *pwo) {};
ENTITY_CLASSDEFINITION(CMissile, CRationalEntity, "Missile", "Missile.tbn", 0x000003ed);
DECLARE_CTFILENAME(_fnmCMissile_tbn, "Missile.tbn");

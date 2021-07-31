/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

CDLLEntityEvent *CEnvironmentBase_events[] = {NULL};
#define CEnvironmentBase_eventsct 0
#define ENTITYCLASS CEnvironmentBase

CEntityProperty CEnvironmentBase_properties[] = {
 CEntityProperty(CEntityProperty::EPT_STRING, NULL, (0x00000384<<8)+1, offsetof(CEnvironmentBase, m_strName), "Name", 'N', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_STRING, NULL, (0x00000384<<8)+2, offsetof(CEnvironmentBase, m_strDescription), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_RANGE, NULL, (0x00000384<<8)+3, offsetof(CEnvironmentBase, m_fDistance), "Range", 'R', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000384<<8)+4, offsetof(CEnvironmentBase, m_fStretch), "Stretch", 'S', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_ENTITYPTR, NULL, (0x00000384<<8)+5, offsetof(CEnvironmentBase, m_penTarget), "Target", 'T', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_ENTITYPTR, NULL, (0x00000384<<8)+6, offsetof(CEnvironmentBase, m_penWatcher), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000384<<8)+7, offsetof(CEnvironmentBase, m_fWatcherFrequency), "Watcher frequency", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT3D, NULL, (0x00000384<<8)+8, offsetof(CEnvironmentBase, m_vDesiredPosition), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000384<<8)+10, offsetof(CEnvironmentBase, m_fMoveSpeed), "Move speed", 'V', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000384<<8)+11, offsetof(CEnvironmentBase, m_fRotateSpeed), "Rotate speed", 'B', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000384<<8)+12, offsetof(CEnvironmentBase, m_fMoveFrequency), "Move frequency", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_BOOL, NULL, (0x00000384<<8)+13, offsetof(CEnvironmentBase, m_bUseWatcher), "Use watcher", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_BOOL, NULL, (0x00000384<<8)+14, offsetof(CEnvironmentBase, m_bFlying), "Flying", 'F', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000384<<8)+16, offsetof(CEnvironmentBase, m_fWaitTime), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FILENAME, NULL, (0x00000384<<8)+20, offsetof(CEnvironmentBase, m_fnMdl), "Model", 'M', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FILENAME, NULL, (0x00000384<<8)+21, offsetof(CEnvironmentBase, m_fnTex), "Texture", 'X', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_ANIMATION, NULL, (0x00000384<<8)+22, offsetof(CEnvironmentBase, m_iAnim), "Animation", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FILENAME, NULL, (0x00000384<<8)+25, offsetof(CEnvironmentBase, m_fnAtt1Mdl), "Attachment 1 Model", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FILENAME, NULL, (0x00000384<<8)+26, offsetof(CEnvironmentBase, m_fnAtt1Tex), "Attachment 1 Texture", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00000384<<8)+27, offsetof(CEnvironmentBase, m_iAtt1Position), "Attachment 1 position", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_ANIMATION, NULL, (0x00000384<<8)+28, offsetof(CEnvironmentBase, m_iAtt1Anim), "Attachment 1 animation", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FILENAME, NULL, (0x00000384<<8)+30, offsetof(CEnvironmentBase, m_fnAtt2Mdl), "Attachment 2 Model", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FILENAME, NULL, (0x00000384<<8)+31, offsetof(CEnvironmentBase, m_fnAtt2Tex), "Attachment 2 Texture", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00000384<<8)+32, offsetof(CEnvironmentBase, m_iAtt2Position), "Attachment 2 position", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_ANIMATION, NULL, (0x00000384<<8)+33, offsetof(CEnvironmentBase, m_iAtt2Anim), "Attachment 2 animation", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FILENAME, NULL, (0x00000384<<8)+35, offsetof(CEnvironmentBase, m_fnAtt3Mdl), "Attachment 3 Model", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FILENAME, NULL, (0x00000384<<8)+36, offsetof(CEnvironmentBase, m_fnAtt3Tex), "Attachment 3 Texture", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00000384<<8)+37, offsetof(CEnvironmentBase, m_iAtt3Position), "Attachment 3 position", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_ANIMATION, NULL, (0x00000384<<8)+38, offsetof(CEnvironmentBase, m_iAtt3Anim), "Attachment 3 animation", 0, 0x7F0000FFUL, 0),
};
#define CEnvironmentBase_propertiesct ARRAYCOUNT(CEnvironmentBase_properties)

CEntityComponent CEnvironmentBase_components[] = {
#define CLASS_WATCHPLAYERS ((0x00000384<<8)+1)
 CEntityComponent(ECT_CLASS, CLASS_WATCHPLAYERS, 0, "EFNM" "Classes\\WatchPlayers.ecl"),
};
#define CEnvironmentBase_componentsct ARRAYCOUNT(CEnvironmentBase_components)

CEventHandlerEntry CEnvironmentBase_handlers[] = {
 {0x03840000, -1, CEntity::pEventHandler(&CEnvironmentBase::
#line 318 "E:/USA/EntitiesMP/EnvironmentBase.es"
MoveToMarker),DEBUGSTRING("CEnvironmentBase::MoveToMarker")},
 {0x03840001, -1, CEntity::pEventHandler(&CEnvironmentBase::H0x03840001_MoveToMarker_01), DEBUGSTRING("CEnvironmentBase::H0x03840001_MoveToMarker_01")},
 {0x03840002, -1, CEntity::pEventHandler(&CEnvironmentBase::H0x03840002_MoveToMarker_02), DEBUGSTRING("CEnvironmentBase::H0x03840002_MoveToMarker_02")},
 {0x03840003, -1, CEntity::pEventHandler(&CEnvironmentBase::H0x03840003_MoveToMarker_03), DEBUGSTRING("CEnvironmentBase::H0x03840003_MoveToMarker_03")},
 {0x03840004, -1, CEntity::pEventHandler(&CEnvironmentBase::H0x03840004_MoveToMarker_04), DEBUGSTRING("CEnvironmentBase::H0x03840004_MoveToMarker_04")},
 {0x03840005, -1, CEntity::pEventHandler(&CEnvironmentBase::H0x03840005_MoveToMarker_05), DEBUGSTRING("CEnvironmentBase::H0x03840005_MoveToMarker_05")},
 {0x03840006, -1, CEntity::pEventHandler(&CEnvironmentBase::
#line 345 "E:/USA/EntitiesMP/EnvironmentBase.es"
Activate),DEBUGSTRING("CEnvironmentBase::Activate")},
 {0x03840007, -1, CEntity::pEventHandler(&CEnvironmentBase::H0x03840007_Activate_01), DEBUGSTRING("CEnvironmentBase::H0x03840007_Activate_01")},
 {0x03840008, -1, CEntity::pEventHandler(&CEnvironmentBase::H0x03840008_Activate_02), DEBUGSTRING("CEnvironmentBase::H0x03840008_Activate_02")},
 {0x03840009, -1, CEntity::pEventHandler(&CEnvironmentBase::
#line 353 "E:/USA/EntitiesMP/EnvironmentBase.es"
Stop),DEBUGSTRING("CEnvironmentBase::Stop")},
 {0x0384000a, -1, CEntity::pEventHandler(&CEnvironmentBase::H0x0384000a_Stop_01), DEBUGSTRING("CEnvironmentBase::H0x0384000a_Stop_01")},
 {0x0384000b, -1, CEntity::pEventHandler(&CEnvironmentBase::H0x0384000b_Stop_02), DEBUGSTRING("CEnvironmentBase::H0x0384000b_Stop_02")},
 {0x0384000c, -1, CEntity::pEventHandler(&CEnvironmentBase::
#line 362 "E:/USA/EntitiesMP/EnvironmentBase.es"
DoAction),DEBUGSTRING("CEnvironmentBase::DoAction")},
 {0x0384000d, -1, CEntity::pEventHandler(&CEnvironmentBase::H0x0384000d_DoAction_01), DEBUGSTRING("CEnvironmentBase::H0x0384000d_DoAction_01")},
 {0x0384000e, -1, CEntity::pEventHandler(&CEnvironmentBase::H0x0384000e_DoAction_02), DEBUGSTRING("CEnvironmentBase::H0x0384000e_DoAction_02")},
 {0x0384000f, -1, CEntity::pEventHandler(&CEnvironmentBase::H0x0384000f_DoAction_03), DEBUGSTRING("CEnvironmentBase::H0x0384000f_DoAction_03")},
 {0x03840010, -1, CEntity::pEventHandler(&CEnvironmentBase::H0x03840010_DoAction_04), DEBUGSTRING("CEnvironmentBase::H0x03840010_DoAction_04")},
 {0x03840011, -1, CEntity::pEventHandler(&CEnvironmentBase::H0x03840011_DoAction_05), DEBUGSTRING("CEnvironmentBase::H0x03840011_DoAction_05")},
 {0x03840012, -1, CEntity::pEventHandler(&CEnvironmentBase::H0x03840012_DoAction_06), DEBUGSTRING("CEnvironmentBase::H0x03840012_DoAction_06")},
 {0x03840013, -1, CEntity::pEventHandler(&CEnvironmentBase::H0x03840013_DoAction_07), DEBUGSTRING("CEnvironmentBase::H0x03840013_DoAction_07")},
 {0x03840014, -1, CEntity::pEventHandler(&CEnvironmentBase::H0x03840014_DoAction_08), DEBUGSTRING("CEnvironmentBase::H0x03840014_DoAction_08")},
 {0x03840015, -1, CEntity::pEventHandler(&CEnvironmentBase::H0x03840015_DoAction_09), DEBUGSTRING("CEnvironmentBase::H0x03840015_DoAction_09")},
 {0x03840016, -1, CEntity::pEventHandler(&CEnvironmentBase::H0x03840016_DoAction_10), DEBUGSTRING("CEnvironmentBase::H0x03840016_DoAction_10")},
 {0x03840017, -1, CEntity::pEventHandler(&CEnvironmentBase::
#line 388 "E:/USA/EntitiesMP/EnvironmentBase.es"
MainLoop),DEBUGSTRING("CEnvironmentBase::MainLoop")},
 {0x03840018, -1, CEntity::pEventHandler(&CEnvironmentBase::H0x03840018_MainLoop_01), DEBUGSTRING("CEnvironmentBase::H0x03840018_MainLoop_01")},
 {0x03840019, -1, CEntity::pEventHandler(&CEnvironmentBase::H0x03840019_MainLoop_02), DEBUGSTRING("CEnvironmentBase::H0x03840019_MainLoop_02")},
 {1, -1, CEntity::pEventHandler(&CEnvironmentBase::
#line 400 "E:/USA/EntitiesMP/EnvironmentBase.es"
Main),DEBUGSTRING("CEnvironmentBase::Main")},
 {0x0384001a, -1, CEntity::pEventHandler(&CEnvironmentBase::H0x0384001a_Main_01), DEBUGSTRING("CEnvironmentBase::H0x0384001a_Main_01")},
 {0x0384001b, -1, CEntity::pEventHandler(&CEnvironmentBase::H0x0384001b_Main_02), DEBUGSTRING("CEnvironmentBase::H0x0384001b_Main_02")},
};
#define CEnvironmentBase_handlersct ARRAYCOUNT(CEnvironmentBase_handlers)

CEntity *CEnvironmentBase_New(void) { return new CEnvironmentBase; };
void CEnvironmentBase_OnInitClass(void) {};
void CEnvironmentBase_OnEndClass(void) {};
void CEnvironmentBase_OnPrecache(CDLLEntityClass *pdec, INDEX iUser) {};
void CEnvironmentBase_OnWorldEnd(CWorld *pwo) {};
void CEnvironmentBase_OnWorldInit(CWorld *pwo) {};
void CEnvironmentBase_OnWorldTick(CWorld *pwo) {};
void CEnvironmentBase_OnWorldRender(CWorld *pwo) {};
ENTITY_CLASSDEFINITION(CEnvironmentBase, CMovableEntity, "Environment Base", "Thumbnails\\EnvironmentBase.tbn", 0x00000384);
DECLARE_CTFILENAME(_fnmCEnvironmentBase_tbn, "Thumbnails\\EnvironmentBase.tbn");

/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

CDLLEntityEvent *CCameraMarker_events[] = {NULL};
#define CCameraMarker_eventsct 0
#define ENTITYCLASS CCameraMarker

CEntityProperty CCameraMarker_properties[] = {
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000000e0<<8)+1, offsetof(CCameraMarker, m_fDeltaTime), "Delta time", 'D', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000000e0<<8)+2, offsetof(CCameraMarker, m_fBias), "Bias", 'B', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000000e0<<8)+3, offsetof(CCameraMarker, m_fTension), "Tension", 'E', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000000e0<<8)+4, offsetof(CCameraMarker, m_fContinuity), "Continuity", 'C', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_BOOL, NULL, (0x000000e0<<8)+5, offsetof(CCameraMarker, m_bStopMoving), "Stop moving", 'O', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000000e0<<8)+6, offsetof(CCameraMarker, m_fFOV), "FOV", 'F', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_BOOL, NULL, (0x000000e0<<8)+7, offsetof(CCameraMarker, m_bSkipToNext), "Skip to next", 'S', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_COLOR, NULL, (0x000000e0<<8)+8, offsetof(CCameraMarker, m_colFade), "Fade Color", 'C', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_ENTITYPTR, NULL, (0x000000e0<<8)+9, offsetof(CCameraMarker, m_penTrigger), "Trigger", 'G', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_ENTITYPTR, NULL, (0x000000e0<<8)+10, offsetof(CCameraMarker, m_penViewTarget), "View Target", 'V', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT3D, NULL, (0x000000e0<<8)+11, offsetof(CCameraMarker, m_vPosRatio), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000000e0<<8)+12, offsetof(CCameraMarker, m_fRatioX), "View pos ratio X", 'X', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000000e0<<8)+13, offsetof(CCameraMarker, m_fRatioY), "View pos ratio Y", 'Y', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000000e0<<8)+14, offsetof(CCameraMarker, m_fRatioZ), "View pos ratio Z", 'Z', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_BOOL, NULL, (0x000000e0<<8)+20, offsetof(CCameraMarker, m_bTargetCaller), "_SC Target player", 0, 0x7F0000FFUL, 0),
};
#define CCameraMarker_propertiesct ARRAYCOUNT(CCameraMarker_properties)

CEntityComponent CCameraMarker_components[] = {
#define MODEL_MARKER ((0x000000e0<<8)+1)
 CEntityComponent(ECT_MODEL, MODEL_MARKER, 2, "EFNM" "Data\\Models\\Editor\\CameraMarker.mdl"),
#define TEXTURE_MARKER ((0x000000e0<<8)+2)
 CEntityComponent(ECT_TEXTURE, TEXTURE_MARKER, 2, "EFNM" "Data\\Models\\Editor\\CameraMarker.tex"),
};
#define CCameraMarker_componentsct ARRAYCOUNT(CCameraMarker_components)

CEventHandlerEntry CCameraMarker_handlers[] = {
 {1, -1, CEntity::pEventHandler(&CCameraMarker::
#line 60 "E:/USA/EntitiesMP/CameraMarker.es"
Main),DEBUGSTRING("CCameraMarker::Main")},
};
#define CCameraMarker_handlersct ARRAYCOUNT(CCameraMarker_handlers)

CEntity *CCameraMarker_New(void) { return new CCameraMarker; };
void CCameraMarker_OnInitClass(void) {};
void CCameraMarker_OnEndClass(void) {};
void CCameraMarker_OnPrecache(CDLLEntityClass *pdec, INDEX iUser) {};
void CCameraMarker_OnWorldEnd(CWorld *pwo) {};
void CCameraMarker_OnWorldInit(CWorld *pwo) {};
void CCameraMarker_OnWorldTick(CWorld *pwo) {};
void CCameraMarker_OnWorldRender(CWorld *pwo) {};
ENTITY_CLASSDEFINITION(CCameraMarker, CMarker, "Camera Marker", "Thumbnails\\CameraMarker.tbn", 0x000000e0);
DECLARE_CTFILENAME(_fnmCCameraMarker_tbn, "Thumbnails\\CameraMarker.tbn");

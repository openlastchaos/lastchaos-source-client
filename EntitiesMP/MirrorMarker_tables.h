/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

EP_ENUMBEG(WarpRotation)
  EP_ENUMVALUE(WR_NONE, "none"),
  EP_ENUMVALUE(WR_BANKING, "banking"),
  EP_ENUMVALUE(WR_TWIRLING, "twirling"),
EP_ENUMEND(WarpRotation);

CDLLEntityEvent *CMirrorMarker_events[] = {NULL};
#define CMirrorMarker_eventsct 0
#define ENTITYCLASS CMirrorMarker

CEntityProperty CMirrorMarker_properties[] = {
 CEntityProperty(CEntityProperty::EPT_ENUM, &WarpRotation_enum, (0x000000f5<<8)+1, offsetof(CMirrorMarker, m_wrRotation), "Rotation Type", 'R', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000000f5<<8)+2, offsetof(CMirrorMarker, m_fRotationSpeed), "Rotation Speed", 'S', 0x7F0000FFUL, 0),
};
#define CMirrorMarker_propertiesct ARRAYCOUNT(CMirrorMarker_properties)

CEntityComponent CMirrorMarker_components[] = {
#define MODEL_IN ((0x000000f5<<8)+1)
 CEntityComponent(ECT_MODEL, MODEL_IN, 2, "EFNM" "Data\\Models\\Editor\\WarpEntrance.mdl"),
#define TEXTURE_IN ((0x000000f5<<8)+2)
 CEntityComponent(ECT_TEXTURE, TEXTURE_IN, 2, "EFNM" "Data\\Models\\Editor\\Warp.tex"),
#define MODEL_OUT ((0x000000f5<<8)+3)
 CEntityComponent(ECT_MODEL, MODEL_OUT, 2, "EFNM" "Data\\Models\\Editor\\WarpExit.mdl"),
#define TEXTURE_OUT ((0x000000f5<<8)+4)
 CEntityComponent(ECT_TEXTURE, TEXTURE_OUT, 2, "EFNM" "Data\\Models\\Editor\\Warp.tex"),
};
#define CMirrorMarker_componentsct ARRAYCOUNT(CMirrorMarker_components)

CEventHandlerEntry CMirrorMarker_handlers[] = {
 {1, -1, CEntity::pEventHandler(&CMirrorMarker::
#line 62 "E:/USA/EntitiesMP/MirrorMarker.es"
Main),DEBUGSTRING("CMirrorMarker::Main")},
};
#define CMirrorMarker_handlersct ARRAYCOUNT(CMirrorMarker_handlers)

CEntity *CMirrorMarker_New(void) { return new CMirrorMarker; };
void CMirrorMarker_OnInitClass(void) {};
void CMirrorMarker_OnEndClass(void) {};
void CMirrorMarker_OnPrecache(CDLLEntityClass *pdec, INDEX iUser) {};
void CMirrorMarker_OnWorldEnd(CWorld *pwo) {};
void CMirrorMarker_OnWorldInit(CWorld *pwo) {};
void CMirrorMarker_OnWorldTick(CWorld *pwo) {};
void CMirrorMarker_OnWorldRender(CWorld *pwo) {};
ENTITY_CLASSDEFINITION(CMirrorMarker, CMarker, "Mirror Marker", "Thumbnails\\WarpMarker.tbn", 0x000000f5);
DECLARE_CTFILENAME(_fnmCMirrorMarker_tbn, "Thumbnails\\WarpMarker.tbn");

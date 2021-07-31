/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

EP_ENUMBEG(GravityType)
	EP_ENUMVALUE(LT_PARALLEL, "Parallel"),
	EP_ENUMVALUE(LT_CENTRAL, "Central"),
	EP_ENUMVALUE(LT_CYLINDRICAL, "Cylindirical"),
	EP_ENUMVALUE(LT_TORUS, "Torus"),
EP_ENUMEND(GravityType);

CDLLEntityEvent *CGravityMarker_events[] = {NULL};
#define CGravityMarker_eventsct 0
#define ENTITYCLASS CGravityMarker

CEntityProperty CGravityMarker_properties[] = {
 CEntityProperty(CEntityProperty::EPT_ENUM, &GravityType_enum, (0x000000d4<<8)+1, offsetof(CGravityMarker, m_gtType), "Type", 'Y', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000000d4<<8)+2, offsetof(CGravityMarker, m_fStrength), "Strength", 'S', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_RANGE, NULL, (0x000000d4<<8)+3, offsetof(CGravityMarker, m_rFallOff), "FallOff", 'F', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_RANGE, NULL, (0x000000d4<<8)+4, offsetof(CGravityMarker, m_rHotSpot), "HotSpot", 'H', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_RANGE, NULL, (0x000000d4<<8)+5, offsetof(CGravityMarker, m_rTorusR), "Torus Radius", 'R', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000000d4<<8)+10, offsetof(CGravityMarker, m_fAcc), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000000d4<<8)+11, offsetof(CGravityMarker, m_fSign), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000000d4<<8)+12, offsetof(CGravityMarker, m_fStep), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_ANGLE3D, NULL, (0x000000d4<<8)+20, offsetof(CGravityMarker, m_aForceDir), "Forcefield Direction", 'F', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000000d4<<8)+21, offsetof(CGravityMarker, m_fForceA), "Forcefield Acceleration", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000000d4<<8)+22, offsetof(CGravityMarker, m_fForceV), "Forcefield Velocity", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT3D, NULL, (0x000000d4<<8)+23, offsetof(CGravityMarker, m_vForceDir), "", 0, 0, 0),
};
#define CGravityMarker_propertiesct ARRAYCOUNT(CGravityMarker_properties)

CEntityComponent CGravityMarker_components[] = {
#define MODEL_MARKER ((0x000000d4<<8)+1)
 CEntityComponent(ECT_MODEL, MODEL_MARKER, 2, "EFNM" "Data\\Models\\Editor\\GravityMarker.mdl"),
#define TEXTURE_MARKER ((0x000000d4<<8)+2)
 CEntityComponent(ECT_TEXTURE, TEXTURE_MARKER, 2, "EFNM" "Data\\Models\\Editor\\GravityMarker.tex"),
};
#define CGravityMarker_componentsct ARRAYCOUNT(CGravityMarker_components)

CEventHandlerEntry CGravityMarker_handlers[] = {
 {1, -1, CEntity::pEventHandler(&CGravityMarker::
#line 137 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/GravityMarker.es"
Main),DEBUGSTRING("CGravityMarker::Main")},
};
#define CGravityMarker_handlersct ARRAYCOUNT(CGravityMarker_handlers)

CEntity *CGravityMarker_New(void) { return new CGravityMarker; };
void CGravityMarker_OnInitClass(void) {};
void CGravityMarker_OnEndClass(void) {};
void CGravityMarker_OnPrecache(CDLLEntityClass *pdec, INDEX iUser) {};
void CGravityMarker_OnWorldEnd(CWorld *pwo) {};
void CGravityMarker_OnWorldInit(CWorld *pwo) {};
void CGravityMarker_OnWorldTick(CWorld *pwo) {};
void CGravityMarker_OnWorldRender(CWorld *pwo) {};
ENTITY_CLASSDEFINITION(CGravityMarker, CMarker, "Gravity Marker", "Thumbnails\\GravityMarker.tbn", 0x000000d4);
DECLARE_CTFILENAME(_fnmCGravityMarker_tbn, "Thumbnails\\GravityMarker.tbn");
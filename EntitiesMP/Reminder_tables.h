/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

CEntityEvent *EReminderInit_New(void) { return new EReminderInit; };
CDLLEntityEvent DLLEvent_EReminderInit = {
  0x02bf0000, &EReminderInit_New
};
CDLLEntityEvent *CReminder_events[] = {
  &DLLEvent_EReminderInit,
};
#define CReminder_eventsct ARRAYCOUNT(CReminder_events)
#define ENTITYCLASS CReminder

CEntityProperty CReminder_properties[] = {
 CEntityProperty(CEntityProperty::EPT_ENTITYPTR, NULL, (0x000002bf<<8)+1, offsetof(CReminder, m_penOwner), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000002bf<<8)+2, offsetof(CReminder, m_fWaitTime), "", 0, 0, EPROPF_NETSEND ),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x000002bf<<8)+3, offsetof(CReminder, m_iValue), "", 0, 0, EPROPF_NETSEND ),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x000002bf<<8)+4, offsetof(CReminder, m_iOwner), "", 0, 0, EPROPF_NETSEND ),
};
#define CReminder_propertiesct ARRAYCOUNT(CReminder_properties)

CEntityComponent CReminder_components[] = {
  CEntityComponent()
};
#define CReminder_componentsct 0


CEventHandlerEntry CReminder_handlers[] = {
 {1, -1, CEntity::pEventHandler(&CReminder::
#line 35 "E:/USA/EntitiesMP/Reminder.es"
Main),DEBUGSTRING("CReminder::Main")},
 {0x02bf0001, -1, CEntity::pEventHandler(&CReminder::H0x02bf0001_Main_01), DEBUGSTRING("CReminder::H0x02bf0001_Main_01")},
 {0x02bf0002, -1, CEntity::pEventHandler(&CReminder::H0x02bf0002_Main_02), DEBUGSTRING("CReminder::H0x02bf0002_Main_02")},
 {0x02bf0003, -1, CEntity::pEventHandler(&CReminder::H0x02bf0003_Main_03), DEBUGSTRING("CReminder::H0x02bf0003_Main_03")},
};
#define CReminder_handlersct ARRAYCOUNT(CReminder_handlers)

CEntity *CReminder_New(void) { return new CReminder; };
void CReminder_OnInitClass(void) {};
void CReminder_OnEndClass(void) {};
void CReminder_OnPrecache(CDLLEntityClass *pdec, INDEX iUser) {};
void CReminder_OnWorldEnd(CWorld *pwo) {};
void CReminder_OnWorldInit(CWorld *pwo) {};
void CReminder_OnWorldTick(CWorld *pwo) {};
void CReminder_OnWorldRender(CWorld *pwo) {};
ENTITY_CLASSDEFINITION(CReminder, CRationalEntity, "Reminder", "", 0x000002bf);
DECLARE_CTFILENAME(_fnmCReminder_tbn, "");

/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

CEntityEvent *EPetAttackDamage_New(void) { return new EPetAttackDamage; };
CDLLEntityEvent DLLEvent_EPetAttackDamage = {
  0x4c7b0000, &EPetAttackDamage_New
};
CDLLEntityEvent *CPet_events[] = {
  &DLLEvent_EPetAttackDamage,
};
#define CPet_eventsct ARRAYCOUNT(CPet_events)
#define ENTITYCLASS CPet

CEntityProperty CPet_properties[] = {
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00004c7b<<8)+3, offsetof(CPet, m_fBodyAnimTime), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00004c7b<<8)+5, offsetof(CPet, m_fStretch), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00004c7b<<8)+100, offsetof(CPet, iAnimPet_Walk), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00004c7b<<8)+101, offsetof(CPet, iAnimPet_Idle1), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00004c7b<<8)+102, offsetof(CPet, iAnimPet_Idle2), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00004c7b<<8)+103, offsetof(CPet, iAnimPet_Run), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00004c7b<<8)+104, offsetof(CPet, iAnimPet_Death), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00004c7b<<8)+105, offsetof(CPet, iAnimPet_Wound), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00004c7b<<8)+106, offsetof(CPet, iAnimPet_Attack), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00004c7b<<8)+107, offsetof(CPet, iAnimPet_NormalBox), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00004c7b<<8)+108, offsetof(CPet, iAnimPet_LevelUp), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00004c7b<<8)+109, offsetof(CPet, iAnimPet_Skill01), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00004c7b<<8)+110, offsetof(CPet, iAnimPet_Skill02), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00004c7b<<8)+111, offsetof(CPet, iAnimPet_Skill03), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00004c7b<<8)+112, offsetof(CPet, iAnimPet_Skill04), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00004c7b<<8)+113, offsetof(CPet, iAnimPet_Skill05), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00004c7b<<8)+114, offsetof(CPet, iAnimPet_Skill01_01), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00004c7b<<8)+115, offsetof(CPet, iAnimPet_Skill01_02), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00004c7b<<8)+116, offsetof(CPet, iAnimPet_Pick), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00004c7b<<8)+117, offsetof(CPet, iAnimPetType), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00004c7b<<8)+118, offsetof(CPet, iAnimPetAge), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_STRING, NULL, (0x00004c7b<<8)+200, offsetof(CPet, m_strName), " Name", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_ENUM, &EventType_enum, (0x00004c7b<<8)+247, offsetof(CPet, m_EventType), "Event Type", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_ENTITYPTR, NULL, (0x00004c7b<<8)+248, offsetof(CPet, m_penEventTarget), "Event Target", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_BOOL, NULL, (0x00004c7b<<8)+251, offsetof(CPet, m_bIsRun), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_BOOL, NULL, (0x00004c7b<<8)+252, offsetof(CPet, m_bDie), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00004c7b<<8)+225, offsetof(CPet, m_fActionAnimTime), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_BOOL, NULL, (0x00004c7b<<8)+255, offsetof(CPet, m_bTransform), "", 0, 0, 0),
};
#define CPet_propertiesct ARRAYCOUNT(CPet_properties)

CEntityComponent CPet_components[] = {
  CEntityComponent()
};
#define CPet_componentsct 0


CEventHandlerEntry CPet_handlers[] = {
 {0x4c7b0001, STATE_CPetBase_Fire, CEntity::pEventHandler(&CPet::
#line 885 "E:/USA/EntitiesMP/Pet.es"
Fire),DEBUGSTRING("CPet::Fire")},
 {0x4c7b0002, STATE_CPetBase_Hit, CEntity::pEventHandler(&CPet::
#line 890 "E:/USA/EntitiesMP/Pet.es"
Hit),DEBUGSTRING("CPet::Hit")},
 {0x4c7b0003, -1, CEntity::pEventHandler(&CPet::H0x4c7b0003_Hit_01), DEBUGSTRING("CPet::H0x4c7b0003_Hit_01")},
 {0x4c7b0004, -1, CEntity::pEventHandler(&CPet::H0x4c7b0004_Hit_02), DEBUGSTRING("CPet::H0x4c7b0004_Hit_02")},
 {0x4c7b0005, STATE_CPetBase_AttackTarget, CEntity::pEventHandler(&CPet::
#line 897 "E:/USA/EntitiesMP/Pet.es"
AttackTarget),DEBUGSTRING("CPet::AttackTarget")},
 {0x4c7b0006, -1, CEntity::pEventHandler(&CPet::H0x4c7b0006_AttackTarget_01), DEBUGSTRING("CPet::H0x4c7b0006_AttackTarget_01")},
 {0x4c7b0007, -1, CEntity::pEventHandler(&CPet::H0x4c7b0007_AttackTarget_02), DEBUGSTRING("CPet::H0x4c7b0007_AttackTarget_02")},
 {0x4c7b0008, STATE_CPetBase_SkillingTarget, CEntity::pEventHandler(&CPet::
#line 922 "E:/USA/EntitiesMP/Pet.es"
SkillingTarget),DEBUGSTRING("CPet::SkillingTarget")},
 {0x4c7b0009, -1, CEntity::pEventHandler(&CPet::H0x4c7b0009_SkillingTarget_01), DEBUGSTRING("CPet::H0x4c7b0009_SkillingTarget_01")},
 {0x4c7b000a, -1, CEntity::pEventHandler(&CPet::H0x4c7b000a_SkillingTarget_02), DEBUGSTRING("CPet::H0x4c7b000a_SkillingTarget_02")},
 {0x4c7b000b, STATE_CPetBase_ActionAnimation, CEntity::pEventHandler(&CPet::
#line 943 "E:/USA/EntitiesMP/Pet.es"
ActionAnimation),DEBUGSTRING("CPet::ActionAnimation")},
 {0x4c7b000c, -1, CEntity::pEventHandler(&CPet::H0x4c7b000c_ActionAnimation_01), DEBUGSTRING("CPet::H0x4c7b000c_ActionAnimation_01")},
 {0x4c7b000d, -1, CEntity::pEventHandler(&CPet::H0x4c7b000d_ActionAnimation_02), DEBUGSTRING("CPet::H0x4c7b000d_ActionAnimation_02")},
 {0x4c7b000e, -1, CEntity::pEventHandler(&CPet::
#line 1005 "E:/USA/EntitiesMP/Pet.es"
AttackAnimation),DEBUGSTRING("CPet::AttackAnimation")},
 {0x4c7b000f, -1, CEntity::pEventHandler(&CPet::H0x4c7b000f_AttackAnimation_01), DEBUGSTRING("CPet::H0x4c7b000f_AttackAnimation_01")},
 {0x4c7b0010, -1, CEntity::pEventHandler(&CPet::H0x4c7b0010_AttackAnimation_02), DEBUGSTRING("CPet::H0x4c7b0010_AttackAnimation_02")},
 {0x4c7b0011, -1, CEntity::pEventHandler(&CPet::H0x4c7b0011_AttackAnimation_03), DEBUGSTRING("CPet::H0x4c7b0011_AttackAnimation_03")},
 {0x4c7b0012, -1, CEntity::pEventHandler(&CPet::H0x4c7b0012_AttackAnimation_04), DEBUGSTRING("CPet::H0x4c7b0012_AttackAnimation_04")},
 {0x4c7b0013, -1, CEntity::pEventHandler(&CPet::
#line 1128 "E:/USA/EntitiesMP/Pet.es"
SkillAndMagicAnimation),DEBUGSTRING("CPet::SkillAndMagicAnimation")},
 {0x4c7b0014, -1, CEntity::pEventHandler(&CPet::H0x4c7b0014_SkillAndMagicAnimation_01), DEBUGSTRING("CPet::H0x4c7b0014_SkillAndMagicAnimation_01")},
 {0x4c7b0015, -1, CEntity::pEventHandler(&CPet::H0x4c7b0015_SkillAndMagicAnimation_02), DEBUGSTRING("CPet::H0x4c7b0015_SkillAndMagicAnimation_02")},
 {0x4c7b0016, -1, CEntity::pEventHandler(&CPet::H0x4c7b0016_SkillAndMagicAnimation_03), DEBUGSTRING("CPet::H0x4c7b0016_SkillAndMagicAnimation_03")},
 {0x4c7b0017, -1, CEntity::pEventHandler(&CPet::H0x4c7b0017_SkillAndMagicAnimation_04), DEBUGSTRING("CPet::H0x4c7b0017_SkillAndMagicAnimation_04")},
 {0x4c7b0018, -1, CEntity::pEventHandler(&CPet::
#line 1226 "E:/USA/EntitiesMP/Pet.es"
NormalAttack),DEBUGSTRING("CPet::NormalAttack")},
 {0x4c7b0019, -1, CEntity::pEventHandler(&CPet::H0x4c7b0019_NormalAttack_01), DEBUGSTRING("CPet::H0x4c7b0019_NormalAttack_01")},
 {0x4c7b001a, -1, CEntity::pEventHandler(&CPet::H0x4c7b001a_NormalAttack_02), DEBUGSTRING("CPet::H0x4c7b001a_NormalAttack_02")},
 {0x4c7b001b, -1, CEntity::pEventHandler(&CPet::H0x4c7b001b_NormalAttack_03), DEBUGSTRING("CPet::H0x4c7b001b_NormalAttack_03")},
 {0x4c7b001c, -1, CEntity::pEventHandler(&CPet::H0x4c7b001c_NormalAttack_04), DEBUGSTRING("CPet::H0x4c7b001c_NormalAttack_04")},
 {1, -1, CEntity::pEventHandler(&CPet::
#line 1251 "E:/USA/EntitiesMP/Pet.es"
Main),DEBUGSTRING("CPet::Main")},
};
#define CPet_handlersct ARRAYCOUNT(CPet_handlers)

CEntity *CPet_New(void) { return new CPet; };
void CPet_OnInitClass(void);
void CPet_OnEndClass(void) {};
void CPet_OnPrecache(CDLLEntityClass *pdec, INDEX iUser) {};
void CPet_OnWorldEnd(CWorld *pwo) {};
void CPet_OnWorldInit(CWorld *pwo) {};
void CPet_OnWorldTick(CWorld *pwo) {};
void CPet_OnWorldRender(CWorld *pwo) {};
ENTITY_CLASSDEFINITION(CPet, CPetBase, "Pet", "Mods\\test.tbn", 0x00004c7b);
DECLARE_CTFILENAME(_fnmCPet_tbn, "Mods\\test.tbn");

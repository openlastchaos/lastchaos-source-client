/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#ifndef _EntitiesMP_Trigger_INCLUDED
#define _EntitiesMP_Trigger_INCLUDED 1
extern "C" DECL_DLL CDLLEntityClass CTrigger_DLLClass;
class CTrigger : public CRationalEntity {
public:
virtual const CTString &GetName(void) const { return m_strName; };
virtual BOOL IsTargetable(void) const { return TRUE; };
  DECL_DLL virtual void SetDefaultProperties(void);
  CTString m_strName;
  CEntityPointer m_penTarget1;
  CEntityPointer m_penTarget2;
  CEntityPointer m_penTarget3;
  CEntityPointer m_penTarget4;
  CEntityPointer m_penTarget5;
  CEntityPointer m_penTarget6;
  CEntityPointer m_penTarget7;
  CEntityPointer m_penTarget8;
  CEntityPointer m_penTarget9;
  CEntityPointer m_penTarget10;
  enum EventEType m_eetEvent1;
  enum EventEType m_eetEvent2;
  enum EventEType m_eetEvent3;
  enum EventEType m_eetEvent4;
  enum EventEType m_eetEvent5;
  enum EventEType m_eetEvent6;
  enum EventEType m_eetEvent7;
  enum EventEType m_eetEvent8;
  enum EventEType m_eetEvent9;
  enum EventEType m_eetEvent10;
  CTStringTrans m_strMessage;
  FLOAT m_fMessageTime;
  enum MessageSound m_mssMessageSound;
  FLOAT m_fWaitTime;
  BOOL m_bAutoStart;
  INDEX m_iCount;
  BOOL m_bUseCount;
  BOOL m_bReuseCount;
  BOOL m_bTellCount;
  BOOL m_bActive;
  RANGE m_fSendRange;
  enum EventEType m_eetRange;
  INDEX m_iCountTmp;
  CEntityPointer m_penCaused;
  INDEX m_ctMaxTrigs;
   
#line 65 "E:/USA/EntitiesMP/Trigger.es"
CEntity * GetTarget(void)const;
   
#line 72 "E:/USA/EntitiesMP/Trigger.es"
SLONG GetUsedMemory(void);
#define  STATE_CTrigger_SendEventToTargets 0x00cd0000
  BOOL 
#line 87 "E:/USA/EntitiesMP/Trigger.es"
SendEventToTargets(const CEntityEvent &__eeInput);
  BOOL H0x00cd0001_SendEventToTargets_01(const CEntityEvent &__eeInput);
  BOOL H0x00cd0002_SendEventToTargets_02(const CEntityEvent &__eeInput);
  BOOL H0x00cd0003_SendEventToTargets_03(const CEntityEvent &__eeInput);
#define  STATE_CTrigger_Active 0x00cd0004
  BOOL 
#line 158 "E:/USA/EntitiesMP/Trigger.es"
Active(const CEntityEvent &__eeInput);
  BOOL H0x00cd0005_Active_01(const CEntityEvent &__eeInput);
  BOOL H0x00cd0006_Active_02(const CEntityEvent &__eeInput);
#define  STATE_CTrigger_Inactive 0x00cd0007
  BOOL 
#line 215 "E:/USA/EntitiesMP/Trigger.es"
Inactive(const CEntityEvent &__eeInput);
  BOOL H0x00cd0008_Inactive_01(const CEntityEvent &__eeInput);
  BOOL H0x00cd0009_Inactive_02(const CEntityEvent &__eeInput);
  BOOL H0x00cd000a_Inactive_03(const CEntityEvent &__eeInput);
  BOOL H0x00cd000b_Inactive_04(const CEntityEvent &__eeInput);
  BOOL H0x00cd000c_Inactive_05(const CEntityEvent &__eeInput);
  BOOL H0x00cd000d_Inactive_06(const CEntityEvent &__eeInput);
#define  STATE_CTrigger_Main 1
  BOOL 
#line 236 "E:/USA/EntitiesMP/Trigger.es"
Main(const CEntityEvent &__eeInput);
  BOOL H0x00cd000e_Main_01(const CEntityEvent &__eeInput);
  BOOL H0x00cd000f_Main_02(const CEntityEvent &__eeInput);
};
#endif // _EntitiesMP_Trigger_INCLUDED

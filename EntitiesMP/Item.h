/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#ifndef _EntitiesMP_Item_INCLUDED
#define _EntitiesMP_Item_INCLUDED 1
#define EVENTCODE_EReceiveItem 0x03200000
class DECL_DLL EReceiveItem : public CEntityEvent {
public:
EReceiveItem();
CEntityEvent *MakeCopy(void);
BOOL CheckIDs(void);
SLONG GetSizeOf(void);
};
DECL_DLL inline void ClearToDefault(EReceiveItem &e) { e = EReceiveItem(); } ;
#define EVENTCODE_EMarkPicked 0x03200001
class DECL_DLL EMarkPicked : public CEntityEvent {
public:
EMarkPicked();
CEntityEvent *MakeCopy(void);
BOOL CheckIDs(void);
SLONG GetSizeOf(void);
CEntityID eidPlayer;
};
DECL_DLL inline void ClearToDefault(EMarkPicked &e) { e = EMarkPicked(); } ;
#define EVENTCODE_ESetType 0x03200002
class DECL_DLL ESetType : public CEntityEvent {
public:
ESetType();
CEntityEvent *MakeCopy(void);
BOOL CheckIDs(void);
SLONG GetSizeOf(void);
INDEX iType;
};
DECL_DLL inline void ClearToDefault(ESetType &e) { e = ESetType(); } ;
extern "C" DECL_DLL CDLLEntityClass CItem_DLLClass;
class DECL_DLL  CItem : public CMovableModelEntity {
public:
virtual const CTString &GetName(void) const { return m_strName; };
virtual const CTString &GetDescription(void) const { return m_strDescription; };
virtual BOOL IsTargetable(void) const { return TRUE; };
   virtual void SetDefaultProperties(void);
  CTString m_strName;
  CTString m_strDescription;
  FLOAT m_fValue;
  FLOAT m_fRespawnTime;
  FLOAT m_fCustomRespawnTime;
  BOOL m_bRespawn;
  CEntityPointer m_penTarget;
  BOOL m_bPickupOnce;
  CSoundObject m_soPick;
  FLOAT m_fPickSoundLen;
  BOOL m_bDropped;
  INDEX m_ulPickedMask;
  BOOL m_bFloating;
  INDEX m_ulRecentlyPickedMask;
  
#line 53 "E:/USA/EntitiesMP/Item.es"
virtual void AdjustDifficulty(void);
  
#line 57 "E:/USA/EntitiesMP/Item.es"
virtual void SetItemType(INDEX iType);
   
#line 62 "E:/USA/EntitiesMP/Item.es"
void AdjustMipFactor(FLOAT & fMipFactor);
   
#line 101 "E:/USA/EntitiesMP/Item.es"
BOOL ShowItemParticles(void);
   
#line 118 "E:/USA/EntitiesMP/Item.es"
BOOL MarkRecentlyPickedBy(CEntity * pen);
   
#line 130 "E:/USA/EntitiesMP/Item.es"
BOOL WasRecentlyPickedBy(CEntity * pen);
   
#line 141 "E:/USA/EntitiesMP/Item.es"
BOOL MarkPickedBy(CEntity * pen);
   
#line 153 "E:/USA/EntitiesMP/Item.es"
BOOL WasPickedBy(CEntity * pen);
   
#line 164 "E:/USA/EntitiesMP/Item.es"
BOOL AdjustShadingParameters(FLOAT3D & vLightDirection,COLOR & colLight,COLOR & colAmbient);
   
#line 194 "E:/USA/EntitiesMP/Item.es"
void InitializeItem(void);
   
#line 221 "E:/USA/EntitiesMP/Item.es"
void AddItem(ULONG ulIDModel,ULONG ulIDTexture,
#line 222 "E:/USA/EntitiesMP/Item.es"
ULONG ulIDReflectionTexture,ULONG ulIDSpecularTexture,ULONG ulIDBumpTexture);
   
#line 226 "E:/USA/EntitiesMP/Item.es"
void AddItemSpecial(INDEX iAttachmentPos,ULONG ulIDModel,ULONG ulIDTexture,
#line 227 "E:/USA/EntitiesMP/Item.es"
ULONG ulIDReflectionTexture,ULONG ulIDSpecularTexture,ULONG ulIDBumpTexture);
   
#line 233 "E:/USA/EntitiesMP/Item.es"
void AddItemAttachment(INDEX iAttachment,ULONG ulIDModel,ULONG ulIDTexture,
#line 234 "E:/USA/EntitiesMP/Item.es"
ULONG ulIDReflectionTexture,ULONG ulIDSpecularTexture,ULONG ulIDBumpTexture);
   
#line 240 "E:/USA/EntitiesMP/Item.es"
void SetItemAttachmentAnim(INDEX iAttachment,INDEX iAnim);
   
#line 249 "E:/USA/EntitiesMP/Item.es"
void AddFlare(ULONG ulIDModel,ULONG ulIDTexture,
#line 250 "E:/USA/EntitiesMP/Item.es"
const FLOAT3D & vPos,const FLOAT3D & vStretch);
   
#line 264 "E:/USA/EntitiesMP/Item.es"
void StretchItem(const FLOAT3D & vStretch);
   
#line 273 "E:/USA/EntitiesMP/Item.es"
SLONG GetUsedMemory(void);
#define  STATE_CItem_ItemCollected 0x03200003
  BOOL 
#line 291 "E:/USA/EntitiesMP/Item.es"
ItemCollected(const CEntityEvent &__eeInput);
#define  STATE_CItem_ItemLoop 0x03200004
  BOOL 
#line 299 "E:/USA/EntitiesMP/Item.es"
ItemLoop(const CEntityEvent &__eeInput);
  BOOL H0x03200005_ItemLoop_01(const CEntityEvent &__eeInput);
  BOOL H0x03200006_ItemLoop_02(const CEntityEvent &__eeInput);
  BOOL H0x03200007_ItemLoop_03(const CEntityEvent &__eeInput);
  BOOL H0x03200008_ItemLoop_04(const CEntityEvent &__eeInput);
  BOOL H0x03200009_ItemLoop_05(const CEntityEvent &__eeInput);
  BOOL H0x0320000a_ItemLoop_06(const CEntityEvent &__eeInput);
#define  STATE_CItem_ItemReceived 0x0320000b
  BOOL 
#line 356 "E:/USA/EntitiesMP/Item.es"
ItemReceived(const CEntityEvent &__eeInput);
  BOOL H0x0320000c_ItemReceived_01(const CEntityEvent &__eeInput);
  BOOL H0x0320000d_ItemReceived_02(const CEntityEvent &__eeInput);
  BOOL H0x0320000e_ItemReceived_03(const CEntityEvent &__eeInput);
  BOOL H0x0320000f_ItemReceived_04(const CEntityEvent &__eeInput);
};
#endif // _EntitiesMP_Item_INCLUDED

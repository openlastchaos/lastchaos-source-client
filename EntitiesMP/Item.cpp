/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#line 2 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"

#include "StdH.h"
#include "Models/Items/ItemHolder/ItemHolder.h"
#include "EntitiesMP/Player.h"

#include <EntitiesMP/Item.h>
#include <EntitiesMP/Item_tables.h>
CEntityEvent *EReceiveItem::MakeCopy(void) { CEntityEvent *peeCopy = new EReceiveItem(*this); return peeCopy;}
SLONG EReceiveItem::GetSizeOf(void) { return sizeof(*this);}
EReceiveItem::EReceiveItem() : CEntityEvent(EVENTCODE_EReceiveItem) {;
};
BOOL EReceiveItem::CheckIDs(void) { return TRUE ; }
CEntityEvent *EMarkPicked::MakeCopy(void) { CEntityEvent *peeCopy = new EMarkPicked(*this); return peeCopy;}
SLONG EMarkPicked::GetSizeOf(void) { return sizeof(*this);}
EMarkPicked::EMarkPicked() : CEntityEvent(EVENTCODE_EMarkPicked) {;
 ClearToDefault(eidPlayer);
};
BOOL EMarkPicked::CheckIDs(void) { return TRUE && eidPlayer.IsValid(); }
CEntityEvent *ESetType::MakeCopy(void) { CEntityEvent *peeCopy = new ESetType(*this); return peeCopy;}
SLONG ESetType::GetSizeOf(void) { return sizeof(*this);}
ESetType::ESetType() : CEntityEvent(EVENTCODE_ESetType) {;
 ClearToDefault(iType);
};
BOOL ESetType::CheckIDs(void) { return TRUE ; }
#line 19 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"

// used to render certain entities only for certain players (like picked items, etc.)
extern ULONG _ulPlayerRenderingMask;

void CItem::SetDefaultProperties(void) {
  m_strName = "Item";
  m_strDescription = "";
  m_fValue = 0.0f;
  m_fRespawnTime = 0.0f;
  m_fCustomRespawnTime = 0.0f;
  m_bRespawn = FALSE ;
  m_penTarget = NULL;
  m_bPickupOnce = FALSE ;
  m_soPick.SetOwner(this);
m_soPick.Stop(FALSE);
  m_fPickSoundLen = 0.0f;
  m_bDropped = FALSE ;
  m_ulPickedMask = 0;
  m_bFloating = FALSE ;
  m_ulRecentlyPickedMask = 0;
  CMovableModelEntity::SetDefaultProperties();
}
  void CItem::AdjustDifficulty(void) 
#line 54 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
{
#line 55 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}
  void CItem::SetItemType(INDEX iType) 
#line 58 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
{
#line 59 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}
  
#line 62 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
void CItem::AdjustMipFactor(FLOAT & fMipFactor) 
#line 63 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
{
#line 65 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
CAttachmentModelObject  * pamo  = GetModelObject  () -> GetAttachmentModel  (ITEMHOLDER_ATTACHMENT_FLARE );
#line 66 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
if(pamo  != NULL )
#line 67 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
{
#line 68 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
FLOAT fRatio  = (Clamp  (fMipFactor  , 5.0f , 7.0f) - 5.0f) / 2.0f;
#line 69 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
UBYTE  ubRatio  = UBYTE  (255 * fRatio );
#line 70 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
COLOR colMutiply  = RGBToColor  (ubRatio  , ubRatio  , ubRatio ) | CT_OPAQUE ;
#line 71 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
pamo  -> amo_moModelObject  . mo_colBlendColor  = colMutiply ;
#line 72 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}
#line 75 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
if(m_ulPickedMask  == 0){
#line 77 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
return ;
#line 78 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}
#line 80 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
BOOL bFlare  = TRUE ;
#line 82 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
if(_ulPlayerRenderingMask  & m_ulPickedMask ){
#line 84 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
extern  INDEX plr_bRenderPicked ;
#line 85 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
if(! plr_bRenderPicked ){
#line 87 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
fMipFactor  = UpperLimit  (0.0f);
#line 88 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}
#line 90 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
extern  INDEX plr_bRenderPickedParticles ;
#line 91 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
if(! plr_bRenderPickedParticles ){
#line 93 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
bFlare  = FALSE ;
#line 94 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}
#line 95 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}
#line 98 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}
  
#line 101 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
BOOL CItem::ShowItemParticles(void) 
#line 102 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
{
#line 104 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
if(_ulPlayerRenderingMask  & m_ulPickedMask ){
#line 106 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
extern  INDEX plr_bRenderPickedParticles ;
#line 107 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
if(! plr_bRenderPickedParticles ){
#line 109 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
return FALSE ;
#line 110 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}
#line 111 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}
#line 113 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
return TRUE ;
#line 114 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}
  
#line 118 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
BOOL CItem::MarkRecentlyPickedBy(CEntity * pen) 
#line 119 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
{
#line 120 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
if(! IsOfClass  (pen  , & CPlayer_DLLClass )){
#line 121 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
return FALSE ;
#line 122 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}
#line 123 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
INDEX iPlayer  = ((CPlayerEntity  *) pen ) -> GetMyPlayerIndex  ();
#line 124 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
BOOL bPickedAlready  = (1 << iPlayer ) & m_ulRecentlyPickedMask ;
#line 125 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
m_ulRecentlyPickedMask  |= (1 << iPlayer );
#line 126 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
return bPickedAlready ;
#line 127 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}
  
#line 130 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
BOOL CItem::WasRecentlyPickedBy(CEntity * pen) 
#line 131 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
{
#line 132 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
if(! IsOfClass  (pen  , & CPlayer_DLLClass )){
#line 133 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
return FALSE ;
#line 134 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}
#line 135 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
INDEX iPlayer  = ((CPlayerEntity  *) pen ) -> GetMyPlayerIndex  ();
#line 136 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
BOOL bPickedAlready  = (1 << iPlayer ) & m_ulRecentlyPickedMask ;
#line 137 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
return bPickedAlready ;
#line 138 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}
  
#line 141 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
BOOL CItem::MarkPickedBy(CEntity * pen) 
#line 142 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
{
#line 143 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
if(! IsOfClass  (pen  , & CPlayer_DLLClass )){
#line 144 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
return FALSE ;
#line 145 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}
#line 146 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
INDEX iPlayer  = ((CPlayerEntity  *) pen ) -> GetMyPlayerIndex  ();
#line 147 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
BOOL bPickedAlready  = (1 << iPlayer ) & m_ulPickedMask ;
#line 148 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
m_ulPickedMask  |= (1 << iPlayer );
#line 149 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
return bPickedAlready ;
#line 150 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}
  
#line 153 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
BOOL CItem::WasPickedBy(CEntity * pen) 
#line 154 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
{
#line 155 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
if(! IsOfClass  (pen  , & CPlayer_DLLClass )){
#line 156 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
return FALSE ;
#line 157 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}
#line 158 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
INDEX iPlayer  = ((CPlayerEntity  *) pen ) -> GetMyPlayerIndex  ();
#line 159 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
BOOL bPickedAlready  = (1 << iPlayer ) & m_ulPickedMask ;
#line 160 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
return bPickedAlready ;
#line 161 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}
  
#line 164 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
BOOL CItem::AdjustShadingParameters(FLOAT3D & vLightDirection,COLOR & colLight,COLOR & colAmbient) 
#line 165 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
{
#line 168 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
{
#line 170 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
colLight  = 0x40404040;
#line 171 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
colAmbient  = 0x60606060;
#line 172 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}
#line 183 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
vLightDirection  = FLOAT3D (- 1 , - 1 , - 1);
#line 184 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
vLightDirection  . Normalize  ();
#line 185 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
vLightDirection  *= GetRotationMatrix  ();
#line 188 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
return FALSE ;
#line 189 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}
  
#line 194 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
void CItem::InitializeItem(void) {
#line 195 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
InitAsModel  ();
#line 196 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
SetFlags  (GetFlags  () | ENF_SEETHROUGH );
#line 198 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
if(m_bFloating ){
#line 199 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
SetPhysicsFlags  (EPF_MODEL_FLYING );
#line 200 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}else {
#line 201 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
SetPhysicsFlags  (EPF_MODEL_SLIDING );
#line 202 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}
#line 204 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
SetCollisionFlags  (ECF_ITEM );
#line 205 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
SetFlagOn  (ENF_CLIENTHANDLING );
#line 208 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
en_fDeceleration  = 60.0f;
#line 211 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
SetModel  (MODEL_ITEM );
#line 212 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
SetDesiredTranslation  (FLOAT3D (0 , 0 , 0));
#line 213 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}
  
#line 221 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
void CItem::AddItem(ULONG ulIDModel,ULONG ulIDTexture,
#line 222 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
ULONG ulIDReflectionTexture,ULONG ulIDSpecularTexture,ULONG ulIDBumpTexture) {
#line 223 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
AddAttachmentToModel  (this  , * GetModelObject  () , ITEMHOLDER_ATTACHMENT_ITEM  , ulIDModel  , ulIDTexture  , 
#line 224 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
ulIDReflectionTexture  , ulIDSpecularTexture  , ulIDBumpTexture );
#line 225 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}
  
#line 226 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
void CItem::AddItemSpecial(INDEX iAttachmentPos,ULONG ulIDModel,ULONG ulIDTexture,
#line 227 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
ULONG ulIDReflectionTexture,ULONG ulIDSpecularTexture,ULONG ulIDBumpTexture) {
#line 228 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
AddAttachmentToModel  (this  , * GetModelObject  () , iAttachmentPos  , ulIDModel  , ulIDTexture  , 
#line 229 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
ulIDReflectionTexture  , ulIDSpecularTexture  , ulIDBumpTexture );
#line 230 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}
  
#line 233 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
void CItem::AddItemAttachment(INDEX iAttachment,ULONG ulIDModel,ULONG ulIDTexture,
#line 234 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
ULONG ulIDReflectionTexture,ULONG ulIDSpecularTexture,ULONG ulIDBumpTexture) {
#line 235 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
CModelObject & mo  = GetModelObject  () -> GetAttachmentModel  (ITEMHOLDER_ATTACHMENT_ITEM ) -> amo_moModelObject ;
#line 236 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
AddAttachmentToModel  (this  , mo  , iAttachment  , ulIDModel  , ulIDTexture  , 
#line 237 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
ulIDReflectionTexture  , ulIDSpecularTexture  , ulIDBumpTexture );
#line 238 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}
  
#line 240 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
void CItem::SetItemAttachmentAnim(INDEX iAttachment,INDEX iAnim) 
#line 241 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
{
#line 242 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
CModelObject & mo  = 
#line 243 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
GetModelObject  () -> GetAttachmentModel  (ITEMHOLDER_ATTACHMENT_ITEM ) -> amo_moModelObject  . 
#line 244 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
GetAttachmentModel  (iAttachment ) -> amo_moModelObject ;
#line 245 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
mo  . PlayAnim  (iAnim  , 0);
#line 246 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}
  
#line 249 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
void CItem::AddFlare(ULONG ulIDModel,ULONG ulIDTexture,
#line 250 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
const FLOAT3D & vPos,const FLOAT3D & vStretch) 
#line 251 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
{
#line 253 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
if(! m_bRespawn  && ! m_bDropped )
#line 254 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
{
#line 255 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
AddAttachmentToModel  (this  , * GetModelObject  () , 
#line 256 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
ITEMHOLDER_ATTACHMENT_FLARE  , ulIDModel  , ulIDTexture  , 0 , 0 , 0);
#line 257 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
CAttachmentModelObject  & amo  = * GetModelObject  () -> GetAttachmentModel  (ITEMHOLDER_ATTACHMENT_FLARE );
#line 258 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
amo  . amo_moModelObject  . StretchModel  (vStretch );
#line 259 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
amo  . amo_plRelative  . pl_PositionVector  = vPos ;
#line 260 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}
#line 261 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}
  
#line 264 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
void CItem::StretchItem(const FLOAT3D & vStretch) {
#line 265 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
CModelObject & mo  = GetModelObject  () -> GetAttachmentModel  (ITEMHOLDER_ATTACHMENT_ITEM ) -> amo_moModelObject ;
#line 266 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
mo  . StretchModel  (vStretch );
#line 267 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
ModelChangeNotify  ();
#line 268 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}
  
#line 273 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
SLONG CItem::GetUsedMemory(void) 
#line 274 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
{
#line 276 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
SLONG  slUsedMemory  = sizeof  (CItem ) - sizeof  (CMovableModelEntity ) + CMovableModelEntity  :: GetUsedMemory  ();
#line 278 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
slUsedMemory  += m_strDescription  . Length  ();
#line 279 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
slUsedMemory  += m_strName  . Length  ();
#line 280 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
slUsedMemory  += 1 * sizeof  (CSoundObject);
#line 281 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
return slUsedMemory ;
#line 282 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}
BOOL CItem::
#line 291 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
ItemCollected(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CItem_ItemCollected
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EPass, "CItem::ItemCollected expects 'EPass' as input!");  const EPass &epass = (const EPass &)__eeInput;Return(STATE_CURRENT,EVoid());return TRUE; ASSERT(FALSE); return TRUE;};BOOL CItem::
#line 299 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
ItemLoop(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CItem_ItemLoop
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CItem::ItemLoop expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 301 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
SetFlagOn  (ENF_CLIENTHANDLING );
#line 302 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
m_fCustomRespawnTime  = ClampDn  (m_fCustomRespawnTime  , 0.0f);
#line 304 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
SetTimerAfter(0.1f);
Jump(STATE_CURRENT, 0x03200005, FALSE, EBegin());return TRUE;}BOOL CItem::H0x03200005_ItemLoop_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x03200005
switch(__eeInput.ee_slEvent){case(EVENTCODE_EBegin):{const EBegin&e= (EBegin&)__eeInput;
return TRUE;}ASSERT(FALSE);break;case(EVENTCODE_ESetType):{const ESetType&eSetType= (ESetType&)__eeInput;

#line 307 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
SetItemType  (eSetType  . iType );
#line 308 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
return TRUE;
#line 309 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}ASSERT(FALSE);break;case(EVENTCODE_ETimer):{const ETimer&e= (ETimer&)__eeInput;
UnsetTimer();Jump(STATE_CURRENT,0x03200006, FALSE, EInternal());return TRUE;}ASSERT(FALSE);break;default: return FALSE; break;
#line 311 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}return TRUE;}BOOL CItem::H0x03200006_ItemLoop_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x03200006
;
#line 313 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
AdjustDifficulty  ();
#line 315 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
SetTimerAt(THINKTIME_NEVER);
Jump(STATE_CURRENT, 0x03200007, FALSE, EBegin());return TRUE;}BOOL CItem::H0x03200007_ItemLoop_03(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x03200007
switch(__eeInput.ee_slEvent){case(EVENTCODE_EBegin):{const EBegin&e= (EBegin&)__eeInput;
return TRUE;}ASSERT(FALSE);break;case(EVENTCODE_EPass):{const EPass&epass= (EPass&)__eeInput;

#line 318 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
if(! IsOfClass  (epass  . penOther  , & CPlayer_DLLClass )){
#line 319 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
return FALSE;
#line 320 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}
#line 321 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
if(! (m_bPickupOnce  || m_bRespawn )){
#line 322 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
SendToTarget  (m_penTarget  , EET_TRIGGER  , epass  . penOther );
#line 323 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
m_penTarget  = NULL ;
#line 324 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}
#line 325 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
Call(STATE_CURRENT, STATE_CItem_ItemCollected, TRUE, epass );return TRUE;
#line 326 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
return TRUE;
#line 327 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}ASSERT(FALSE);break;case(EVENTCODE_EMarkPicked):{const EMarkPicked&eMark= (EMarkPicked&)__eeInput;

#line 329 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
MarkPickedBy  ((CEntity  *) eMark  . eidPlayer );
#line 330 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
return TRUE;
#line 331 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}ASSERT(FALSE);break;case(EVENTCODE_EReceiveItem):{const EReceiveItem&e= (EReceiveItem&)__eeInput;

#line 333 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
if(! _pNetwork  -> IsServer  ()){
#line 334 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
Call(STATE_CURRENT, STATE_CItem_ItemReceived, TRUE, EVoid  ());return TRUE;
#line 335 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}
#line 336 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
return TRUE;
#line 337 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}ASSERT(FALSE);break;case(EVENTCODE_ESetType):{const ESetType&eSetType= (ESetType&)__eeInput;

#line 339 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
SetItemType  (eSetType  . iType );
#line 340 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
return TRUE;
#line 341 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}ASSERT(FALSE);break;case(EVENTCODE_EEnd):{const EEnd&e= (EEnd&)__eeInput;
UnsetTimer();Jump(STATE_CURRENT,0x03200008, FALSE, EInternal());return TRUE;}ASSERT(FALSE);break;default: return FALSE; break;
#line 343 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}return TRUE;}BOOL CItem::H0x03200008_ItemLoop_04(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x03200008

#line 345 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
SetTimerAfter(m_fPickSoundLen  + 0.5f);
Jump(STATE_CURRENT, 0x03200009, FALSE, EBegin());return TRUE;}BOOL CItem::H0x03200009_ItemLoop_05(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x03200009
switch(__eeInput.ee_slEvent) {case EVENTCODE_EBegin: return TRUE;case EVENTCODE_ETimer: Jump(STATE_CURRENT,0x0320000a, FALSE, EInternal()); return TRUE;default: return FALSE; }}BOOL CItem::H0x0320000a_ItemLoop_06(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x0320000a
;
#line 347 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
if(m_bDropped ){
#line 348 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
Destroy  ();
#line 349 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}else {
#line 350 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
Destroy  (FALSE );
#line 351 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}
#line 352 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
Return(STATE_CURRENT,EVoid());
#line 352 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
return TRUE; ASSERT(FALSE); return TRUE;};BOOL CItem::
#line 356 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
ItemReceived(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CItem_ItemReceived
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CItem::ItemReceived expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 359 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
SwitchToEditorModel  ();
#line 360 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
if((m_bPickupOnce  || m_bRespawn )){
#line 361 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
SendToTarget  (m_penTarget  , EET_TRIGGER  , NULL );
#line 362 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}
#line 365 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
if(!(m_bRespawn )){ Jump(STATE_CURRENT,0x0320000f, FALSE, EInternal());return TRUE;}
#line 366 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
ASSERT  (m_fRespawnTime  > 0.0f);
#line 369 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
SetTimerAfter(m_fRespawnTime );
Jump(STATE_CURRENT, 0x0320000c, FALSE, EBegin());return TRUE;}BOOL CItem::H0x0320000c_ItemReceived_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x0320000c
switch(__eeInput.ee_slEvent){case(EVENTCODE_EBegin):{const EBegin&e= (EBegin&)__eeInput;
return TRUE;}ASSERT(FALSE);break;case(EVENTCODE_ETimer):{const ETimer&e= (ETimer&)__eeInput;
UnsetTimer();Jump(STATE_CURRENT,0x0320000d, FALSE, EInternal());return TRUE;}ASSERT(FALSE);break;default:{return TRUE;}ASSERT(FALSE);break;
#line 373 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}return TRUE;}BOOL CItem::H0x0320000d_ItemReceived_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x0320000d

#line 375 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
SwitchToModel  ();
#line 376 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
m_ulRecentlyPickedMask  = 0;Jump(STATE_CURRENT,0x0320000e, FALSE, EInternal());return TRUE;}BOOL CItem::H0x0320000f_ItemReceived_04(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x0320000f
{
#line 379 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
Return(STATE_CURRENT,EEnd  ());
#line 379 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
return TRUE;
#line 380 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
}Jump(STATE_CURRENT,0x0320000e, FALSE, EInternal());return TRUE;}
BOOL CItem::H0x0320000e_ItemReceived_03(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x0320000e

#line 381 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
Return(STATE_CURRENT,EVoid());
#line 381 "E:/질산/Korea/Client_USA/EntitiesMP/Item.es"
return TRUE; ASSERT(FALSE); return TRUE;};
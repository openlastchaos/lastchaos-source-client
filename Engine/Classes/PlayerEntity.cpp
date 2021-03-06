/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */


#include "StdH.h"
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Base/Stream.h>

#include <Classes/PlayerEntity.h>
#include <Classes/PlayerEntity_tables.h>
void CPlayerEntity::SetDefaultProperties(void) {
	en_tmPing = 0.0f;
	CMovableModelEntity::SetDefaultProperties();
}

void CPlayerEntity::InitializePlayer(ULONG ulWeaponsID,ULONG ulAnimatorID) 
{
EPlayerInit  ePLInit ;
ePLInit  . ulWeaponsID  = ulWeaponsID ;
ePLInit  . ulAnimatorID  = ulAnimatorID ;
ePLInit  . bCreate  = TRUE ;
Initialize  (ePLInit  , FALSE );
}
void CPlayerEntity::SetWeaponAndAnimator() {
}
BOOL CPlayerEntity::IsPlayer() {
return TRUE ;
}
void CPlayerEntity::DumpToConsole() {
}
void CPlayerEntity::Read_net_Mob(int index,CNetworkMessage * istr) {
}
void CPlayerEntity::Read_net_Character(int type,int sub_type,CNetworkMessage * istr) {
}
void CPlayerEntity::Read_net_Pet(int index,CNetworkMessage * istr) {
}
void CPlayerEntity::Read_net_WildPet(int index,CNetworkMessage * istr) {
}
void CPlayerEntity::Read_net_Summon(int index,CNetworkMessage * istr) {
}
void CPlayerEntity::Read_net_Damage_Character(CNetworkMessage * istr) {
}
void CPlayerEntity::Read_net_Damage(SBYTE sbType,CNetworkMessage * istr) {
}
void CPlayerEntity::Read_net_DissappearEffect(CNetworkMessage * istr) {
}
void CPlayerEntity::Read_net_StatusPC(CNetworkMessage * istr) {
}
void CPlayerEntity::Read_net_StatusNPC(CNetworkMessage * istr) {
}
void CPlayerEntity::Read_net_StatusPet(CNetworkMessage * istr) {
}
void CPlayerEntity::Read_net_StatusElemental(CNetworkMessage * istr) {
}
void CPlayerEntity::ProcessProduce(SBYTE sbChaType,SLONG slChaIndex,SBYTE sbTargetType,SLONG slTargetIndex,SLONG slTargetHP) {
}
void CPlayerEntity::StandDown(BOOL bStand) {
}
void CPlayerEntity::OpenGate(int iGateNumber,BOOL bOpenGate) {
}
void CPlayerEntity::SetMobData(CEntity * penEntity,SLONG hp,SLONG maxHp,int level,BOOL bNpc,int mobIdx) {
}
void CPlayerEntity::SetChaData(int index,int type,ULONG hp,ULONG maxHp,SBYTE hairstyle,SBYTE facestyle,CTString & strName,UWORD state,SBYTE pkTitle,SLONG pkstate) {
}
void CPlayerEntity::SetPetData(CEntity * penEntity,SLONG hp,SLONG maxHP) {
}
void CPlayerEntity::SetWildPetData(CEntity * penEntity,SLONG hp,SLONG maxHP) {
}
void CPlayerEntity::SetElementalData(CEntity * penEntity,SLONG hp,SLONG maxHP) {
}
void CPlayerEntity::SetElementalStatus(CEntity * penEntity,SBYTE sbAttackSpeed,SBYTE sbMagicSpeed,LONG lSkillSpeed,FLOAT fWalkSpeed,FLOAT fRunSpeed,FLOAT fAttackRange) {
}
void CPlayerEntity::SetShopData(INDEX index,SBYTE sbShopType) {
}
void CPlayerEntity::SetImportantValues(FLOAT fWalkspeed,FLOAT fRunspeed,FLOAT fAttackrange,SBYTE sbAttackspeed,SBYTE sbMagicspeed) {
}
FLOAT CPlayerEntity::GetWalkspeed() {
return 0.0f;
}
FLOAT CPlayerEntity::GetRunspeed() {
return 0.0f;
}
FLOAT CPlayerEntity::GetAttackrange() {
return 0.0f;
}
SBYTE CPlayerEntity::GetAttackspeed() {
return 0;
}
SBYTE CPlayerEntity::GetMagicspeed() {
return 0;
}
void CPlayerEntity::SetSkillCancel(BOOL bCanSkillCancel) {
}
void CPlayerEntity::StartCamera(CEntity * pTarget,BOOL bRestart) {
}
void CPlayerEntity::SetWideScreen(CEntity * pTarget,BOOL bWide,FLOAT fFov) {
}
void CPlayerEntity::GetProjection(CAnyProjection3D & apr) {
}
void CPlayerEntity::LostTarget() {
}
void CPlayerEntity::UseSkill(int skillIndex) {
}
void CPlayerEntity::UsePetPickItem() {
}
void CPlayerEntity::UsePetSkill(int skillIndex) {
}
void CPlayerEntity::UseSlaveSkill(CEntity * pEntity,int skillIndex) {
}
void CPlayerEntity::UseWildSkill(CEntity * pEntity,int skillIndex) {
}
void CPlayerEntity::CancelSkill(BOOL bLostTarget,BOOL bAutoAttack,BOOL bSkillError) {
}
void CPlayerEntity::LoadSkillAnimID() {
}
void CPlayerEntity::UseAction(int ActionIndex) {
}
void CPlayerEntity::StopMove() {
}
void CPlayerEntity::ClearMove() {
}
void CPlayerEntity::SetProduct(SLONG slChaIndex,SBYTE slProductType) {
}
void CPlayerEntity::SetConsensus(SLONG slChaIndex) {
}
void CPlayerEntity::WarpTeleport(FLOAT fX,FLOAT fZ,FLOAT fH,FLOAT fR,SBYTE sbLayer) {
}
void CPlayerEntity::SingleModeOn() {
}
void CPlayerEntity::FieldModeOn() {
}
void CPlayerEntity::SetLegit(UBYTE sbType,SLONG chaindex) {
}
void CPlayerEntity::SetRepairLegit(SLONG chaindex,CCharacterTarget * Ct) {
}
BOOL CPlayerEntity::IsLegit() {
return FALSE ;
}
void CPlayerEntity::ClearMultiTargets() {
}
void CPlayerEntity::RidingPet(CEntity * pCharacter,INDEX iPetType) {
}
void CPlayerEntity::LeavingPet(CEntity * pCharacter) {
}
BOOL CPlayerEntity::IsRidePet(CEntity * pCharacter) {
return FALSE ;
}
void CPlayerEntity::RidingWildPet(CEntity * pCharacter,CEntity * pWildPet,CTString strFileName) {
}
void CPlayerEntity::LeavingWildPet(CEntity * pCharacter) {
}
BOOL CPlayerEntity::IsMoving() {
return FALSE ;
}
BOOL CPlayerEntity::IsActionSitting() {
return FALSE ;
}
BOOL CPlayerEntity::IsIdle() {
return FALSE ;
}
const BOOL CPlayerEntity::IsTransform()const {
return FALSE ;
}
void CPlayerEntity::ClearTargetInfo(CEntity * penEntity) {
}
void CPlayerEntity::SetCharacterHairChange(INDEX nSIdx,SBYTE sbType,BOOL IsMe) {
}
void CPlayerEntity::SetCharacterAppearance(CModelInstance * pMI,int iJob,SBYTE sbHairStyle,SBYTE sbFaceStyle) {
}
void CPlayerEntity::ChangeFaceMesh(CModelInstance * pMI,int iJob,SBYTE sbType) {
}
void CPlayerEntity::ChangeHairMesh(CModelInstance * pMI,int iJob,SBYTE sbType) {
}
void CPlayerEntity::CancelProduct() {
}
void CPlayerEntity::DeleteWearingWeapon(BOOL bException,BOOL bMode) {
}
void CPlayerEntity::AppearWearingWeapon(BOOL bSkillEnd) {
}
void CPlayerEntity::DeleteDefaultArmor(int type) {
}
void CPlayerEntity::WearingDefaultArmor(int type) {
}
void CPlayerEntity::DeleteCurrentArmor(int weartype) {
}
void CPlayerEntity::SetChaWearing(class CCharacterTarget * pTarget,CEntity * penEntity,CNetworkMessage * istr) {
}
void CPlayerEntity::SetCharCostumeWearing(class CCharacterTarget * pTarget,CEntity * penEntity,CNetworkMessage * istr) {
}
void CPlayerEntity::WearingCostumeArmor(CNetworkMessage * istr) {
}
void CPlayerEntity::WearingCostumeArmorOneSuit(CNetworkMessage * istr) {
}
BOOL CPlayerEntity::IsHelmet(INDEX iCharIndex) {
return FALSE ;
}
void CPlayerEntity::Rebirth() {
}
void CPlayerEntity::DeathYou() {
}
void CPlayerEntity::PlayerInit(bool bDeath) {
}
BOOL CPlayerEntity::IsAlreadyDie() {
return FALSE ;
}
void CPlayerEntity::SetDie() {
}
void CPlayerEntity::SetTarget(CEntity * penEntity) {
}
void CPlayerEntity::SetTarget(SLONG targetID,SBYTE targetType) {
}
BOOL CPlayerEntity::IsSameTarget(CEntity * penEntity) {
return FALSE ;
}
void CPlayerEntity::SetTargetMe() {
}
void CPlayerEntity::PlayItemSound(BOOL bPick,BOOL bMoney) {
}
void CPlayerEntity::PlayButtonSound() {
}
void CPlayerEntity::PlayJewelDullSound() {
}
void CPlayerEntity::PlayJewelRefinementSound() {
}
void CPlayerEntity::PlayJewelShinySound() {
}
void CPlayerEntity::PlayPetStashEffectSound() {
}
void CPlayerEntity::PlayPetStashCardSelSound() {
}
void CPlayerEntity::PlayHolyWaterSound() {
}
void CPlayerEntity::CommandAttack() {
}
BOOL CPlayerEntity::CommandAction() {
return FALSE ;
}
void CPlayerEntity::PlayBGM(CTFileName fnFileName) {
}
void CPlayerEntity::SearchNearItem() {
}
void CPlayerEntity::SetCharacterState(CEntity * cha_Entity,INDEX ch_state) {
}
BOOL CPlayerEntity::IsSkilling() {
return FALSE ;
}
BOOL CPlayerEntity::IsSitting() {
return FALSE ;
}
BOOL CPlayerEntity::IsAttacking() {
return FALSE ;
}
BOOL CPlayerEntity::IsPetActing() {
return FALSE ;
}
BOOL CPlayerEntity::IsProduct() {
return FALSE ;
}
void CPlayerEntity::SetSpeedUp(BOOL bMe,INDEX index,BOOL bSpeedup) {
}
BOOL CPlayerEntity::CheckEscKey(void) {
return FALSE ;
}
void CPlayerEntity::PlayItemEffect(SLONG itemindex,SBYTE effecttype) {
}
BOOL CPlayerEntity::IsSocialActing() {
return FALSE ;
}
BOOL CPlayerEntity::IsPolymophing() {
return FALSE ;
}
BOOL CPlayerEntity::IsTransforming() {
return FALSE ;
}
BOOL CPlayerEntity::IsChanging() {
return FALSE ;
}
void CPlayerEntity::AppearChaPolymoph(CEntity * pentity,INDEX mobindex) {
}
void CPlayerEntity::TransfromPet(CEntity * pentity,INDEX mobIndex,INDEX mobSize) {
}
void CPlayerEntity::ReturnTransformPet(CEntity * pentity) {
}
void CPlayerEntity::TransformSorcerer(int iType) {
}
void CPlayerEntity::ReturnSorcerer() {
}
void CPlayerEntity::AppearChaTransform(CEntity * pEntity,INDEX iTransformType) {
}
void CPlayerEntity::PlAddAccessoryEffect(CEntity * penEntity,CCharacterTarget * pTarget) {
}
void CPlayerEntity::EnemyTargetSelected_InputTab() {
}
void CPlayerEntity::EvocationStart(LONG lIndex,int nSkillIndex) {
}
void CPlayerEntity::EvocationStop(LONG lIndex) {
}
BOOL CPlayerEntity::CheckChangeCondition(INDEX level,INDEX zone) {
return FALSE ;
}
void CPlayerEntity::SetSummonCommand(CEntity * pEntity,INDEX iCommand) {
}
BOOL CPlayerEntity::CheckNormalAttack(CEntity * pEntity,FLOAT fDist) {
return FALSE ;
}
CModelInstance * CPlayerEntity::GetPlayerModelInstance(CEntity * pEntity) {
return NULL ;
}
BOOL CPlayerEntity::CheckAttackTarget(INDEX iSkillIndex,CEntity * pEntity,FLOAT fDist) {
return FALSE ;
}
BOOL CPlayerEntity::CheckEntityOfTarget(CEntity * pEntity,CEntity * pTarget) {
return FALSE ;
}
void CPlayerEntity::ReturnChange(BOOL bAppear) {
}
void CPlayerEntity::AddDeathItem(CEntity * penEnemy,FLOAT3D pos,class CItemTarget * pItemTarget,class CItemData * pItemData) {
}
void CPlayerEntity::ReceiveTriggerEvent(INDEX iIn,INDEX iOut,INDEX iContinued,CNetworkMessage * iStr) {
}
void CPlayerEntity::ReceiveRaidScene(CNetworkMessage * istr) {
}
void CPlayerEntity::SendTriggerEvent(INDEX iTarget,INDEX eetEventType) {
}
CEntity * CPlayerEntity::GetClickObject(void) {
return NULL ;
}
void CPlayerEntity::DropDeathItem(CEntity * penEnemy) {
}
void CPlayerEntity::HUD_DrawModel(CDrawPort * pdp,FLOAT fMinX,FLOAT fMinY,FLOAT fMaxX,FLOAT fMaxY,INDEX iType,FLOAT fDistance,FLOAT fHeight) {
}
FLOAT CPlayerEntity::GetAnimationTime() {
return 0.0f;
}
void CPlayerEntity::HUD_SetCharModelData(INDEX iJob,INDEX iHairStyle,INDEX iFaceStyle) {
}
void CPlayerEntity::HUD_SetModelData(INDEX iType,INDEX iIndex,INDEX iUIType) {
}
void CPlayerEntity::HUD_WearingModel(INDEX iType,INDEX iJob,INDEX iItem,BOOL bLogin) {
}
void CPlayerEntity::HUD_ChangeHairMesh(INDEX iJob,INDEX iHairStyle) {
}
void CPlayerEntity::HUD_ChangeFaceMesh(INDEX iJob,INDEX iFaceStyle) {
}
void CPlayerEntity::HUD_PlayAnimation(INDEX iJob,INDEX AnimID) {
}
void CPlayerEntity::HUD_PlayAnimation(CTString strAniName) {
}
void CPlayerEntity::HUD_SetModelColor(const COLOR ModelColor) {
}
void CPlayerEntity::HUD_SetTitleEffect(CTString strEffectName) {
}
BOOL CPlayerEntity::IsHudModel_Used() {
return FALSE ;
}
void CPlayerEntity::SetHudModel_Use(BOOL bUse) {
}
void CPlayerEntity::SetDecoModeCamera(FLOAT fDistance,FLOAT fHeight) {
}
void CPlayerEntity::SetAppearanceData(int inde,SBYTE hairstyle,SBYTE facestyle) {
}
CTString CPlayerEntity::GetPlayerName(void) 
{
return en_pcCharacter  . GetNameForPrinting  ();
}
const CTString & CPlayerEntity::GetName(void)const 
{
return en_pcCharacter  . GetName  ();
}
INDEX CPlayerEntity::GetMyPlayerIndex(void) 
{
CEntity  * penMe  = this ;
for(INDEX iPlayer  = 0;
iPlayer  < GetMaxPlayers  ();
iPlayer  ++){
if(GetPlayerEntity  (iPlayer ) == penMe ){
return iPlayer ;
}
}
return 15;
}
void CPlayerEntity::DoMoving(void) 
{
CMovableModelEntity  :: DoMoving  ();
}
void CPlayerEntity::Copy(CEntity & enOther,ULONG ulFlags) 
{
CMovableModelEntity  :: Copy  (enOther  , ulFlags );
CPlayerEntity  * ppenOther  = (CPlayerEntity  *) (& enOther );
en_pcCharacter  = ppenOther  -> en_pcCharacter ;
en_plViewpoint  = ppenOther  -> en_plViewpoint ;
en_plLastViewpoint  = ppenOther  -> en_plLastViewpoint ;
}
void CPlayerEntity::Read_t(CTStream * istr,BOOL bNetwork) 
{
CMovableModelEntity  :: Read_t  (istr  , bNetwork );
(* istr ) >> en_pcCharacter  >> en_plViewpoint ;
en_plLastViewpoint  = en_plViewpoint ;
}
void CPlayerEntity::Write_t(CTStream * ostr,BOOL bNetwork) 
{
CMovableModelEntity  :: Write_t  (ostr  , bNetwork );
(* ostr ) << en_pcCharacter  << en_plViewpoint ;
}
void CPlayerEntity::ApplyAction(const CPlayerAction & pa,FLOAT tmLatency) {
}
void CPlayerEntity::Disconnect(void) {
}
void CPlayerEntity::CharacterChanged(const CPlayerCharacter & pcNew) {
en_pcCharacter  = pcNew ;
}
BOOL CPlayerEntity::
Dummy(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CPlayerEntity_Dummy
	ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CPlayerEntity::Dummy expects 'EVoid' as input!");
	const EVoid &e = (const EVoid &)__eeInput;
 ASSERT(FALSE);
 return TRUE;
}
;

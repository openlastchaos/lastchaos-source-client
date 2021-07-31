#include "stdh.h"

#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Entities/MobData.h>
#include <Engine/Entities/EntityClass.h>
#include <Engine/Entities/EntityProperties.h>
#include <Engine/Ska/StringTable.h>
#include <Engine/World/World.h>
#include <Engine/Network/CNetwork.h> // WSS_DRATAN_SEIGEWARFARE 2007/08/08

#define DEL_AUCTION
/*
 *  Constructor.
 */
CMobData::CMobData(void)
:mob_ulZoneFlag(0), mob_ulExtraFlag(0)
{
	//mob_iArrayIndex	= -1;
	//mob_iAttackType	= -1;
	memset(&mob_Data, 0, sizeof(_npcStat));
	mob_Data.index	= -1;
	mob_bBoss		= FALSE;
	Owner_Index	= -1;
//	Owner_name		= "";
//	Owner_Shop_name	= "";
}

/*
 *  Destructor.
 */
CMobData::~CMobData(void) 
{
	mob_ulZoneFlag	= 0;
	mob_ulExtraFlag	= 0;
}

CTString CMobData::GetAnimAttackNameByID(int iAttackID)
{
	ASSERT(iAttackID > 0 && "Invalid Attack Animation Number");

	// === Attack Animation 규칙 ===
	// Attack##		--> ##는 숫자.
	// 예) Attack01, Attack02...
	CTString strAttackName;
	strAttackName.PrintF(TRANS("%s%02d"), GetAnimAttackName(), iAttackID);
	return strAttackName;
}

int CMobData::LoadNPCDataFromFile(CStaticArray<CMobData> &apMobData, const char* FileName, const char* ZNFileName)
{
	//apMobData.Clear();
	
	FILE *fp		= NULL;
	if ((fp = fopen(FileName, "rb")) == NULL) 
	{
		MessageBox(NULL, "File is not Exist.", "error!", MB_OK);
		return -1;
	}

	int iNumOfNPC	= 0;
	int iLength		= -1;
	int iReadBytes	= 0;
	int iLastIndex	= 0;

	iReadBytes = fread(&iLastIndex, sizeof(int), 1, fp);		// SHOP의 마지막 인덱스.
	apMobData.New(iLastIndex);									// define 으로 한정된 부분을 동적할당
	ASSERT(apMobData.Count() > 0 && "Invalid SHOP Data");		
	ASSERT(iLastIndex > 0 && "Invalid SHOP Data");

	//////////////////////////////////////////////////////////////////////////	
	// MACRO DEFINITION
	//////////////////////////////////////////////////////////////////////////	
#define LOADINT(d)			iReadBytes = fread(&d, sizeof(int), 1, fp);
#define LOADSHORT(d)		iReadBytes = fread(&d, sizeof(short), 1, fp);
#define LOADCHAR(d)			iReadBytes = fread(&d, sizeof(char), 1, fp);
#define LOADFLOAT(d)		iReadBytes = fread(&d, sizeof(float), 1, fp);
#define LOADSTR(d)			{ int iLen; LOADINT(iLen); iReadBytes = fread(&d, iLen, 1, fp); }
	//////////////////////////////////////////////////////////////////////////	

	

	for(int i = 0; i < iLastIndex; ++i)
	{
		int iIndex = -1;
		LOADINT(iIndex);
		if(iReadBytes <= 0)		break;										// EOF

		CMobData& MD		= apMobData[iIndex];				
		_npcStat& NpcData	= MD.mob_Data;
		NpcData.index		= iIndex;

		LOADSTR(NpcData.name);
		LOADINT(NpcData.level);
		LOADINT(NpcData.walkspeed);
		LOADINT(NpcData.runspeed);

		for(int j = 0; j < 5; ++j)
		{
			LOADINT(NpcData.effect[j]);
		}

		LOADINT(NpcData.hp);
		LOADINT(NpcData.mp);
		LOADINT(NpcData.attackSpeed);
		LOADFLOAT(NpcData.AttackDistance);
		LOADCHAR(NpcData.skill);
		LOADCHAR(NpcData.sskillmaster);
		LOADINT(NpcData.type);
		//구매대행 캐릭터 체크를 위한 플래그 추가 
#ifdef DEL_AUCTION
		LOADINT(NpcData.typeSE);
#endif

		LOADFLOAT(NpcData.scale);
		LOADFLOAT(NpcData.size);

		LOADINT(NpcData.skill0_index);
		LOADCHAR(NpcData.skill0_level);
		LOADINT(NpcData.skill1_index);
		LOADCHAR(NpcData.skill1_level);

		LOADSTR(NpcData.motion_Idle);
		LOADSTR(NpcData.motion_Walk);
		LOADSTR(NpcData.motion_Wound);
		LOADSTR(NpcData.motion_Attack);
		LOADSTR(NpcData.motion_Die);
		LOADSTR(NpcData.motion_Run);
		LOADSTR(NpcData.motion_Idle2);
		LOADSTR(NpcData.motion_Attack2);
		LOADSTR(NpcData.fileSMC);
		LOADCHAR(NpcData.attackType);
		LOADCHAR(NpcData.fireDelayCount);
		LOADFLOAT(NpcData.client_fireobjDelay[0]);
		LOADFLOAT(NpcData.client_fireobjDelay[1]);
		LOADFLOAT(NpcData.client_fireobjDelay[2]);
		LOADFLOAT(NpcData.client_fireobjDelay[3]);

		CTString strAttackAnim;
		// FIXME : 죄다 하드 코딩~~~~
		// FIXME : 구울만 공격 애니메이션이 4개여서...
		// FIXME : 매직넘버 제거!!!
		// 구울의 경우.
		if( iIndex == 193 )
		{			
			strAttackAnim.PrintF( "%s_att%02d", "ghoulworker", 3 );
			strcpy( MD.motion_Attack3, strAttackAnim );
			strAttackAnim.PrintF( "%s_att%02d", "ghoulworker", 4 );
			strcpy( MD.motion_Attack4, strAttackAnim );
		}
		else if( iIndex == 194 )
		{
			strAttackAnim.PrintF( "%s_att_%02d", "ghoulsucker", 3 );
			strcpy( MD.motion_Attack3, strAttackAnim );
			strAttackAnim.PrintF( "%s_att_%02d", "ghoulsucker", 4 );
			strcpy( MD.motion_Attack4, strAttackAnim );
		}
		else if( iIndex == 195 )
		{
			strAttackAnim.PrintF( "%s_att%02d", "ghoulanker", 3 );
			strcpy( MD.motion_Attack3, strAttackAnim );
			strAttackAnim.PrintF( "%s_att%02d", "ghoulanker", 4 );
			strcpy( MD.motion_Attack4, strAttackAnim );
		}
		else
		{
			// FIXME : 싱글던젼을 위해서만 필요한 부분....ㅡ.ㅡ
			strcpy( MD.motion_Attack3, NpcData.motion_Attack );
			strcpy( MD.motion_Attack4, NpcData.motion_Attack2 );
		}
		
		// FIXME : 보스 몹인지 인덱스로 확인 후에 플래그를 켜줌...ㅡ.ㅡ		
		// NOTE : 201 -> 싱글던젼에 보스 몹 카미라.
		if( iIndex == 201 )
		{
			MD.mob_bBoss	= TRUE;
		}

		std::vector<float> tempvec;
		for(int a=0; a<NpcData.fireDelayCount; ++a)
		{
			tempvec.push_back(NpcData.client_fireobjDelay[a]);
		}
		std::sort(tempvec.begin(), tempvec.end());
		for(a=0; a<NpcData.fireDelayCount; ++a)
		{
			NpcData.client_fireobjDelay[a] = tempvec[a];
		}
		
		LOADSTR(NpcData.fireEffect0);
		LOADSTR(NpcData.fireEffect1);
		LOADSTR(NpcData.fireEffect2);
		LOADCHAR(NpcData.fireObject);
		LOADFLOAT(NpcData.fireSpeed);

		if(iReadBytes <= 0)
		{
			MessageBox(NULL, "NPC 화일이 올바르지 않습니다.", "Error!", MB_OK);
			fclose(fp);
			return -1;
		}
	}
	fclose(fp);

	// 존 이동 플래그 정보를 화일로부터 읽어들임.
	if(ZNFileName)
	{
		FILE *fp		= NULL;
		if ((fp = fopen(ZNFileName, "rb")) == NULL) 
		{
			MessageBox(NULL, "File is not Exist.", "error!", MB_OK);
			return -1;
		}
		int iIndex = 0;
		LOADINT(iIndex);
		ASSERT(apMobData.Count() > 0 && "Invalid MOB Data");		
		ASSERT(iIndex > 0 && "Invalid MOB Data");

		//for(int i = 0; i <= iIndex; ++i)
		// NPC 데이터 화일의 마지막 인덱스만큼 데이터를 읽어들임.
		for(int i = 0; i <= iLastIndex; ++i)
		{
			CMobData& MD		= apMobData[i];
			iReadBytes = fread(&MD.mob_ulZoneFlag,	sizeof(ULONG), 1, fp);	// Zone Flag
			iReadBytes = fread(&MD.mob_ulExtraFlag,	sizeof(ULONG), 1, fp);	// Extra Flag
		}

		fclose(fp);
	}
//////////////////////////////////////////////////////////////////////////	
#undef LOADINT
#undef LOADCHAR
#undef LOADFLOAT
#undef LOADSTR
	return iLastIndex;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : penEntity - 
//			MD - 
// Output : int
//-----------------------------------------------------------------------------
int CMobData::SetMobDataToNPC(CEntity* penEntity, CMobData& MD, const char* szMobName)
{
	ASSERT(penEntity != NULL && "Invalid Entity Pointer!");
	ASSERT(MD.GetMobIndex() != -1 && "Invalid Mob Index!");

	if(MD.IsPeaceful())
	{
		penEntity->SetFirstExtraFlagOn(ENF_EX1_NPC);
		penEntity->SetFirstExtraFlagOn(ENF_EX1_PEACEFUL);
	}

	if(MD.IsZoneMoving())	
	{
		// WSS_DRATAN_SIEGEWARFARE 2007/10/14 -------------------------------->>
		BOOL bSkip = FALSE;
		if( MD.IsCastleTower() )
		{
			// 워프 타워의 경우 공성측의 경우 일반 몹으로 처리함
			if(!(_pNetwork->MyCharacterInfo.sbJoinFlagDratan == WCJF_OWNER ||
				 _pNetwork->MyCharacterInfo.sbJoinFlagDratan == WCJF_DEFENSE_GUILD) )
			{
				bSkip = TRUE;				
			}
		}
		// -------------------------------------------------------------------<<

		if( !bSkip )
		{
			// 존 이동 장치 플래그가 켜져있는 경우, 존 이동 플래그를 넣어줌.
			penEntity->SetFlagOn(ENF_ZONEMOVING);
			penEntity->SetFirstExtraFlagOn(ENF_EX1_NPC);
			CEntityProperty &epPropertyZoneFlag	= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_FLAGS, 237));		// Zone Flag
			ENTITYPROPERTY( &*penEntity, epPropertyZoneFlag.ep_slOffset, ULONG)		= MD.GetZoneMovingFlag();

			CEntityProperty &epPropertyExtraFlag	= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_FLAGS, 238));	// Extra Flag
			ENTITYPROPERTY( &*penEntity, epPropertyExtraFlag.ep_slOffset, ULONG)	= MD.GetExtraFlag();
		}
	}
	else if(MD.IsResource())							// 생산물품
	{
		// 광물은 반드시 NPC가 아니어야함.
		penEntity->SetFirstExtraFlagOn(ENF_EX1_PRODUCTION);
		penEntity->SetFirstExtraFlagOff(ENF_EX1_NPC);
	}
	else if( MD.IsSkillMaster() || MD.IsSSkillMaster() || MD.IsNPC() )		// NPC
	{
		penEntity->SetFirstExtraFlagOn(ENF_EX1_NPC);
	}
	else
	{
		penEntity->SetFlagOn(ENF_ALIVE);
	}

	CEntityProperty &epPropertyNpcIndex	= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 91));	// Enemy Index
	ENTITYPROPERTY( &*penEntity, epPropertyNpcIndex.ep_slOffset, INDEX)		= MD.GetMobIndex();

	// NOTE : 리젠 타임도 복사해줘야 함(월드 에디터에서만 필요함)
	//CEntityProperty &epPropertyNpcIndex	= pdecDLLClass->dec_aepProperties[3];		// Regen Time
	//ENTITYPROPERTY( &*penEntity, epPropertyNpcIndex.ep_slOffset, INDEX)		= MD.GetIndex();

	CEntityProperty &epPropertyAlready	= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_BOOL, 250));	// Initialized?
	ENTITYPROPERTY( &*penEntity, epPropertyAlready.ep_slOffset, BOOL)	= TRUE;
	
	//-----------------------------------------------------------------------------
	// 애니메이션 정보를 설정함.
	//-----------------------------------------------------------------------------
	CEntityProperty &epPropertyWalk		= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 100));	// Walk Animation
	ENTITYPROPERTY( &*penEntity, epPropertyWalk.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(MD.GetAnimWalkName());
	
	CEntityProperty &epPropertyAttack	= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 101));	// Attack Animation
	ENTITYPROPERTY( &*penEntity, epPropertyAttack.ep_slOffset, INDEX)	= ska_GetIDFromStringTable(MD.GetAnimAttackName());
	
	CEntityProperty &epPropertyIdle		= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 102));	// Idle Animation
	ENTITYPROPERTY( &*penEntity, epPropertyIdle.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(MD.GetAnimDefaultName());
	
	CEntityProperty &epPropertyDeath	= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 103));	// Death Animation
	ENTITYPROPERTY( &*penEntity, epPropertyDeath.ep_slOffset, INDEX)	= ska_GetIDFromStringTable(MD.GetAnimDeathName());
	
	CEntityProperty &epPropertyWound	= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 104));	// Wound Animation
	ENTITYPROPERTY( &*penEntity, epPropertyWound.ep_slOffset, INDEX)	= ska_GetIDFromStringTable(MD.GetAnimWoundName());
	
	CEntityProperty &epPropertyBox		= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 105));	// Normal Box
	ENTITYPROPERTY( &*penEntity, epPropertyBox.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(MD.GetNormalBoxName());

	CEntityProperty &epPropertyRun		= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 107));	// Run Animation
	ENTITYPROPERTY( &*penEntity, epPropertyRun.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(MD.GetAnimRunName());

	CEntityProperty &epPropertyStretch	= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 5));	// Model Stretch Scale
	ENTITYPROPERTY( &*penEntity, epPropertyStretch.ep_slOffset, FLOAT)	= MD.GetScale();

	CEntityProperty &epPropertyAttack2	= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 106));	// Attack Animation 2
	ENTITYPROPERTY( &*penEntity, epPropertyAttack2.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(MD.GetAnimAttack2Name());

	CEntityProperty &epPropertyIdle2		= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 108));	// Idle Animation
	ENTITYPROPERTY( &*penEntity, epPropertyIdle2.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(MD.GetAnimDefault2Name());

	CEntityProperty &epPropertyAttack3	= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 109));	// Attack Animation 2
	ENTITYPROPERTY( &*penEntity, epPropertyAttack3.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(MD.GetAnimAttack3Name());

	CEntityProperty &epPropertyAttack4	= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 110));	// Attack Animation 2
	ENTITYPROPERTY( &*penEntity, epPropertyAttack4.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(MD.GetAnimAttack4Name());

	CEntityProperty &epPropertyName		= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_STRING, 200));	// Mob Name
	ENTITYPROPERTY( &*penEntity, epPropertyName.ep_slOffset, CTString)	= szMobName;

	// FIXME : 싱글던젼에서만 쓰임...ㅡ.ㅡ
	if( MD.IsBoss() )
	{
		CEntityProperty &epPropertyBoss	= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_BOOL, 160));	// Is Boss Mob?
		ENTITYPROPERTY( &*penEntity, epPropertyBoss.ep_slOffset, BOOL)	= MD.IsBoss();
	}
	
	CDLLEntityClass *pdecDLLBaseClass	= penEntity->GetClass()->ec_pdecDLLClass->dec_pdecBase;
	CDLLEntityClass *pdecDLLBaseBaseClass	= pdecDLLBaseClass->dec_pdecBase;

	CEntityProperty &epPropertyWalkSpeed		= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 10);	// NPC Walk Speed
	ENTITYPROPERTY( &*penEntity, epPropertyWalkSpeed.ep_slOffset, FLOAT)			= MD.GetWalkSpeed();	

	CEntityProperty &epPropertyAttackRunSpeed	= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 12);	// NPC Attack Run Speed
	ENTITYPROPERTY( &*penEntity, epPropertyAttackRunSpeed.ep_slOffset, FLOAT)		= MD.GetRunSpeed();

	CEntityProperty &epPropertyCloseRunSpeed	= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 14);	// NPC Close Run Speed
	ENTITYPROPERTY( &*penEntity, epPropertyCloseRunSpeed.ep_slOffset, FLOAT)		= MD.GetRunSpeed();	

	CEntityProperty &epPropertyAttackDistance	= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 20);	// Attack Distance
	ENTITYPROPERTY( &*penEntity, epPropertyAttackDistance.ep_slOffset, FLOAT)		= MD.GetAttackDistance() * 1.5f;

	CEntityProperty &epPropertyHealth	= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 143);	// NPC Health
	ENTITYPROPERTY( &*penEntity, epPropertyHealth.ep_slOffset, FLOAT)		= MD.GetHealth();
	((CLiveEntity*)penEntity)->SetHealth(MD.GetHealth());	

	float fStopDistance		= MD.GetAttackDistance() - 0.1f;
	float fCloseDistance	= MD.GetAttackDistance();
	if(fStopDistance < 0.0f)	fStopDistance	= 0.0f;
	if(fCloseDistance < 0.0f)	fCloseDistance	= 0.0f;

	CEntityProperty &epPropertyStopDistance		= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 24);	// Attack Distance
	ENTITYPROPERTY( &*penEntity, epPropertyStopDistance.ep_slOffset, FLOAT)			= fStopDistance;

	CEntityProperty &epPropertyCloseDistance	= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 21);	// Attack Distance
	ENTITYPROPERTY( &*penEntity, epPropertyCloseDistance.ep_slOffset, FLOAT)		= fCloseDistance;
	
	return -1;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *  Constructor.
 */
CMobName::CMobName(void)
{
	memset(&mob_Name, 0, sizeof(_npcStat));
	mob_Name.index	= -1;
}

/*
 *  Destructor.
 */
CMobName::~CMobName(void) 
{
}

int CMobName::LoadMobNameFromFile(CStaticArray<CMobName> &apMobData, const char* FileName)
{
	//apMobData.Clear();
	
	FILE *fp		= NULL;
	if ((fp = fopen(FileName, "rb")) == NULL) 
	{
		MessageBox(NULL, "File is not Exist.", "error!", MB_OK);
		return -1;
	}

	int iNumOfNPC	= 0;
	int iLength		= -1;
	int iReadBytes	= 0;
	int iLastIndex	= 0;

	iReadBytes = fread(&iLastIndex, sizeof(int), 1, fp);		// SHOP의 마지막 인덱스.
	apMobData.New(iLastIndex);
	ASSERT(apMobData.Count() > 0 && "Invalid SHOP Data");		
	ASSERT(iLastIndex > 0 && "Invalid SHOP Data");

	//////////////////////////////////////////////////////////////////////////	
	// MACRO DEFINITION
	//////////////////////////////////////////////////////////////////////////	
#define LOADINT(d)			iReadBytes = fread(&d, sizeof(int), 1, fp);
#define LOADSHORT(d)		iReadBytes = fread(&d, sizeof(short), 1, fp);
#define LOADCHAR(d)			iReadBytes = fread(&d, sizeof(char), 1, fp);
#define LOADFLOAT(d)		iReadBytes = fread(&d, sizeof(float), 1, fp);
#define LOADSTR(d)			{ int iLen; LOADINT(iLen); iReadBytes = fread(&d, iLen, 1, fp); }
	//////////////////////////////////////////////////////////////////////////	

	//apMobData.New(iNumOfNPC);

	for(int i = 0; i < iLastIndex; ++i)
	{
		int iIndex = -1;
		LOADINT(iIndex);
		if(iReadBytes <= 0)		break;										// EOF

		CMobName& MD		= apMobData[iIndex];				
		_npcStat& NpcName	= MD.mob_Name;
		NpcName.index		= iIndex;

		LOADSTR(NpcName.name);

	/* // Date : 2005-01-15,   By Lee Ki-hwan 대만 로컬라이징
		if(iReadBytes <= 0)
		{
			MessageBox(NULL, "NPC 화일이 올바르지 않습니다.", "Error!", MB_OK);
			fclose(fp);
			return -1;
		}
	*/
	}
	fclose(fp);
//////////////////////////////////////////////////////////////////////////	
#undef LOADINT
#undef LOADCHAR
#undef LOADFLOAT
#undef LOADSTR
	return iLastIndex;
}

// WSS_DRATAN_SEIGEWARFARE 2007/08/08 ---------------------------------------->>
// 드라탄 마스터 타워 관려 수정...
// NPC로 되면 말표시가 되고 관련 창을 생성할 수 있고, NPC가 아니면 
// CheckTarget을 통해 공격여부를 판단할 수 있다 
BOOL CMobData::IsNPC() const
{
	// 제외 사항 설정
	// NPC_CASTLE_TOWER가 설정되어 있으면 기본적으로 NPC로 설정하지 않음
	// 공성의 경우 수성과 공성에 따라 설정값이 다르기 때문에...
	if( (mob_Data.type&NPC_CASTLE_TOWER) )
	{
		int tRes = IsMyQuarter();
		if (tRes != QUARTER_NONE )
		{
			if ( tRes == QUARTER_INSTALL || tRes == QUARTER_MINE) return TRUE;
			else if ( tRes == QUARTER_ATTACK ) return FALSE; 
		}
		if (_pNetwork->MyCharacterInfo.sbJoinFlagDratan!=WCJF_OWNER)
			return FALSE;
		else return TRUE;	
	}		
	else 
		return (mob_Data.type & NPC_MASK);	
}
// ---------------------------------------------------------------------------<<

// WSS_DRATAN_SEIGEWARFARE 2007/08/13------------------------>>
int CMobData::IsMyQuarter() const
{
	std::map<int,int>::iterator tFound,tEnd;
	tFound	= _pNetwork->MyCharacterInfo.mQuarter.find(mob_Data.index);
	tEnd	= _pNetwork->MyCharacterInfo.mQuarter.end();
	
	if( tFound == tEnd ) return QUARTER_NONE; // 부활진지가 아님
	
	int  tIdx = _pNetwork->MyCharacterInfo.mQuarter[mob_Data.index];
	if (tIdx==-1) 
		return QUARTER_INSTALL;	// 설치	
	else if ( tIdx == _pNetwork->MyCharacterInfo.lGuildIndex)
		return QUARTER_MINE;	// 나의 진지

	return QUARTER_ATTACK;		// 공격
}
// ----------------------------------------------------------<<

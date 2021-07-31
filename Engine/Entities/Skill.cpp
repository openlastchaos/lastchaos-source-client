#include "stdh.h"

#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Entities/Skill.h>
#include <Engine/Ska/StringTable.h>
#include <Engine/Base/CTString.h>
#include <Engine/Base/Timer.h>				// yjpark
#include <Engine/Interface/UIManager.h>		// yjpark
#include <Engine/Interface/UISummon.h>
#include <Engine/Interface/UIQuickSlot.h>
#include <Engine/Interface/UIPetInfo.h>

CSkill::CSkill(void) 
{	
	memset(&Skill_Data, 0, sizeof(_SkillData));
	bCanCancel	= FALSE;
	bNeedTarget	= FALSE;
}

CSkill::~CSkill(void) 
{
	if(!m_vectorSkillLevels.empty())
		m_vectorSkillLevels.clear();
}

CSkill::Init(void) 
{
	bCanCancel = FALSE;	
}

// yjpark |<--
void CSkill::SetStartTime()
{
	Skill_Data.Skill_StartTime =_pTimer->GetHighPrecisionTimer().GetSeconds();
	_pUIMgr->GetCharacterInfo()->StartSkillDelay( Skill_Data.index );
	_pUIMgr->GetQuickSlot()->StartSkillDelay( Skill_Data.index );
	_pUIMgr->GetPetInfo()->StartSkillDelay( Skill_Data.index );
	_pUIMgr->GetWildPetInfo()->StartSkillDelay(Skill_Data.index);

	for( int i = UI_SUMMON_START; i <= UI_SUMMON_END; ++i )
	{
		CUISummon* pUISummon = (CUISummon*)_pUIMgr->GetUI(i);
		pUISummon->StartSkillDelay( Skill_Data.index );
	}	
}
// yjpark     -->|

//안태훈 수정 시작	//(Open beta)(2004-11-29)
#include <vector>
#include <algorithm>
//안태훈 수정 끝	//(Open beta)(2004-11-29)
//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &apSkillData - 
//			FileName - 
// Output : int
//-----------------------------------------------------------------------------
int CSkill::LoadSkillDataFromFile(CStaticArray<CSkill> &apSkillData, const char* FileName)
{	
	FILE *fp		= NULL;
	if ((fp = fopen(FileName, "rb")) == NULL) 
	{
		MessageBox(NULL, "File is not Exist.", "error!", MB_OK);
		return -1;
	}

	int iLastSkillIndex	= 0; //스킬 갯수.
	int iLength		= -1;
	int iReadBytes	= 0;

	iReadBytes = fread(&iLastSkillIndex, sizeof(int), 1, fp);			
	apSkillData.New(iLastSkillIndex);
	ASSERT(apSkillData.Count() >= iLastSkillIndex && "Invalid Array Count");//여기서 걸리면 고정된 개수의 스킬수 초과한것임. 더 늘릴것.(스킬은 고정배열 사용)
	ASSERT(iLastSkillIndex > 0 && "Invalid Skill Data");

	//////////////////////////////////////////////////////////////////////////	
	// MACRO DEFINITION
	//////////////////////////////////////////////////////////////////////////	
#define LOADINT(d)			iReadBytes = fread(&d, sizeof(int), 1, fp);
#define LOADSHORT(d)		iReadBytes = fread(&d, sizeof(short), 1, fp);
#define LOADCHAR(d)			iReadBytes = fread(&d, sizeof(char), 1, fp);
#define LOADFLOAT(d)		iReadBytes = fread(&d, sizeof(float), 1, fp);
#define LOADSTR(d)			{ int iLen; LOADINT(iLen); iReadBytes = fread(&d, iLen, 1, fp); }
	//////////////////////////////////////////////////////////////////////////	

	for(int i = 0; i < iLastSkillIndex; i++) //스킬 갯수만큼.
	{
		int iIndex = 1; //스킬번호.	
		LOADINT(iIndex);

		if(iReadBytes <= 0)		break;										// EOF
		ASSERT(iIndex != -1	&& "Invalid Skill Index");

		CSkill& SkillData			= apSkillData[iIndex];	
		_SkillData& SD				= SkillData.Skill_Data;		
		SD.index					= iIndex;

		// 일반
		LOADINT(SD.job);
		LOADINT(SD.job2);
		LOADINT(SD.petindex);
		LOADSTR(SD.name);
		LOADCHAR(SD.type);
		LOADINT(SD.flag);
		LOADINT(SD.sorcerer); 
		LOADCHAR(SD.maxLevel);

		// 거리
		LOADFLOAT(SD.appRange);
		LOADFLOAT(SD.fireRange);
		LOADFLOAT(SD.fireRange2);

		// 타겟
		LOADCHAR(SD.targetType);

		if( SD.targetType == STT_TARGET_ONE || 
			SD.targetType == STT_TARGET_RANGE || 
			SD.targetType == STT_PARTY_ONE ||
			SD.targetType == STT_TARGET_D120 ||
			SD.targetType == STT_TARGET_RECT )
		{
			SkillData.bNeedTarget = TRUE;
		}

		LOADCHAR(SD.targetNum);

		// 사용조건
		LOADINT(SD.useState);
		LOADINT(SD.useWeaponType0);
		LOADINT(SD.useWeaponType1);
		LOADINT(SD.useMagicIndex1);
		LOADCHAR(SD.useMagicLevel1);
		LOADINT(SD.useMagicIndex2);
		LOADCHAR(SD.useMagicLevel2);
		LOADINT(SD.useMagicIndex3);
		LOADCHAR(SD.useMagicLevel3);

		// 적용조건
		LOADINT(SD.appState);

		// 시간
		LOADINT(SD.readyTime);
		LOADINT(SD.waitTime);
		LOADINT(SD.fireTime);
		LOADINT(SD.reuseTime);

		if( SD.reuseTime == 0 )
		{// 기본 3초
			SD.reuseTime = 30; 
		}

		for(INDEX iWeapon=0; iWeapon<WEAPON_COUNT; ++iWeapon)
		{
			// 시전
			LOADSTR(SD.client[iWeapon].readyAni);
			LOADSTR(SD.client[iWeapon].readyEffect1);

			// 정지
			LOADSTR(SD.client[iWeapon].stillAni);

			// 발사
			LOADSTR(SD.client[iWeapon].fireAni);
			LOADSTR(SD.client[iWeapon].fireEffect1);
			LOADSTR(SD.client[iWeapon].fireEffect2);
			LOADSTR(SD.client[iWeapon].fireEffect3);

			// 발사체
			LOADCHAR(SD.client[iWeapon].fireobjType);
			LOADFLOAT(SD.client[iWeapon].fireobjSpeed);
			LOADFLOAT(SD.client[iWeapon].fireobjX);
			LOADFLOAT(SD.client[iWeapon].fireobjZ);
			LOADFLOAT(SD.client[iWeapon].fireobjH);
			LOADCHAR(SD.client[iWeapon].fireobjCoord);
			LOADCHAR(SD.client[iWeapon].fireobjDelayCount);
			LOADFLOAT(SD.client[iWeapon].fireobjDelay[0]);
			LOADFLOAT(SD.client[iWeapon].fireobjDelay[1]);
			LOADFLOAT(SD.client[iWeapon].fireobjDelay[2]);
			LOADFLOAT(SD.client[iWeapon].fireobjDelay[3]);
			//시간 순으로 소팅
			std::vector<float> tempvec;
			for(int a=0; a<SD.client[iWeapon].fireobjDelayCount; ++a)
			{
				tempvec.push_back(SD.client[iWeapon].fireobjDelay[a]);
			}
			std::sort(tempvec.begin(), tempvec.end());
			for(a=0; a<SD.client[iWeapon].fireobjDelayCount; ++a)
			{
				SD.client[iWeapon].fireobjDelay[a] = tempvec[a];
			}
			LOADFLOAT(SD.client[iWeapon].fireobjDestDelay);
		}

		// 설명
		LOADSTR(SD.client_description);
		LOADSTR(SD.client_tooltip);

		// 아이콘
		LOADINT(SD.client_icon_texid);
		LOADINT(SD.client_icon_row);
		LOADINT(SD.client_icon_col);

		for(int j = 0; j < SD.maxLevel;++j)
		{		
			_SkillLevel SL;
			LOADINT(SL.needHP);
			LOADINT(SL.needMP);
			LOADINT(SL.durtime);
			LOADINT(SL.dummyPower);
			LOADINT(SL.needItemIndex1);
			LOADINT(SL.needItemCount1);
			LOADINT(SL.needItemIndex2);
			LOADINT(SL.needItemCount2);
			LOADINT(SL.learnLevel);
			LOADINT(SL.learnSP);
			for( int k = 0; k < 3; k++ )
			{
				LOADINT(SL.learnSkillIndex[k]);
				LOADCHAR(SL.learnSkillLevel[k]);
			}
			for( k = 0; k < 3; k++ )
			{
				LOADINT(SL.learnItemIndex[k]);
				LOADINT(SL.learnItemCount[k]);
			}
			// 050401 edit by cpp2angel
			LOADINT(SL.learnStr);
			LOADINT(SL.learnDex);
			LOADINT(SL.learnInt);
			LOADINT(SL.learnCon);
					
			LOADINT(SL.appMagicIndex1);
			LOADCHAR(SL.appMagicLevel1);
			LOADINT(SL.appMagicIndex2);
			LOADCHAR(SL.appMagicLevel2);
			LOADINT(SL.appMagicIndex3);
			LOADCHAR(SL.appMagicLevel3);
			LOADINT(SL.magicIndex1);
			LOADCHAR(SL.magicLevel1);
			LOADINT(SL.magicIndex2);
			LOADCHAR(SL.magicLevel2);
			LOADINT(SL.magicIndex3);
			LOADCHAR(SL.magicLevel3);			
// WSS_NEW_GUILD_SYSTEM 070716 --------------------->>	
#ifdef NEW_GUILD_SYSTEM 
			LOADINT(SL.learnGP);				
#endif
// -------------------------------------------------<<
			SkillData.m_vectorSkillLevels.push_back(SL);
		}

		if(SkillData.GetType() == ST_MAGIC && 
			( (SkillData.GetJob() == HEALER) || 
			(SkillData.GetJob() == MAGE) || 
			(SkillData.GetJob() == ROGUE) || 
			(SkillData.GetJob() == SORCERER) ))
		{
			SkillData.bCanCancel = TRUE;
		}
		
		for(iWeapon=0; iWeapon<WEAPON_COUNT; ++iWeapon)
		{
			SkillData.idPlayer_Anim_Skill[iWeapon][0] = ska_GetIDFromStringTable(SD.client[iWeapon].readyAni);
			SkillData.idPlayer_Anim_Skill[iWeapon][1] = ska_GetIDFromStringTable(SD.client[iWeapon].stillAni);
			SkillData.idPlayer_Anim_Skill[iWeapon][2] = ska_GetIDFromStringTable(SD.client[iWeapon].fireAni);
		}

		if(iReadBytes <= 0)
		{
			fclose(fp);
			return -1;
		}

	}
	fclose(fp);

//////////////////////////////////////////////////////////////////////////	
#undef LOADINT
#undef LOADCHAR
#undef LOADFLOAT
#undef LOADSTR

	return iLastSkillIndex;
}
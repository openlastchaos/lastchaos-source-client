#include "stdh.h"
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Entities/SpecialSkill.h>

CSpecialSkill::CSpecialSkill()
{
	memset(&SSkill_Data, 0, sizeof(_SSkillData));	
}

CSpecialSkill::~CSpecialSkill()
{
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &apSkillData - 
//			FileName - 
// Output : int
//-----------------------------------------------------------------------------
int CSpecialSkill::LoadSSkillDataFromFile(CStaticArray<CSpecialSkill> &apSkillData, const char* FileName)
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
	ASSERT(apSkillData.Count() >= iLastSkillIndex && "Invalid Array Count");
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

		CSpecialSkill& SkillData	= apSkillData[iIndex];	
		_SSkillData& SD				= SkillData.SSkill_Data;		
		SD.index					= iIndex;
		
		LOADSTR(SD.name);
		LOADINT(SD.type);		
		LOADINT(SD.maxLevel);
		LOADINT(SD.preference);

		for(int j = 0; j < SSKILL_MAX_LEVEL;++j)
		{
			LOADINT(SD.needLevel[j]);
			LOADINT(SD.needSP[j]);
		}

		LOADINT(SD.needSSkill);
		LOADINT(SD.needSSkillLevel);
		
		LOADINT(SD.textureID);
		LOADINT(SD.textureRow);
		LOADINT(SD.textureCol);

		LOADSTR(SD.desc);
/* // Date : 2005-01-15,   By Lee Ki-hwan : 대만 로컬라이징
		if(iReadBytes <= 0)
		{
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

	return iLastSkillIndex;
}
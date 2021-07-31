#include "stdh.h"

#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Entities/MissionCase.h>
//강동민 수정 시작 테스트 클라이언트 작업	06.09
#include <Engine/Entities/EntityClass.h>
#include <Engine/Entities/EntityProperties.h>
#include <Engine/Ska/StringTable.h>
//강동민 수정 끝 테스트 클라이언트 작업		06.09

/*
 *  Constructor.
 */
CMissionCase::CMissionCase(void)
{
	memset(&m_MissionData, 0, sizeof(TMissionData));

}

/*
 *  Destructor.
 */
CMissionCase::~CMissionCase(void) 
{

}
//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &apShopData - 
//			FileName - 
// Output : int
//-----------------------------------------------------------------------------
int CMissionCase::LoadMissionFromFile(CStaticArray<CMissionCase> &apMissiondata, const char* FileName)
{	
	FILE* fMissiondata = NULL;
	if ((fMissiondata = fopen(FileName, "rb")) == NULL) 
	{
		MessageBox(NULL, "MissionCase File is not Exist.", "error!", MB_OK);
		return -1;
	}
	int		nMission_total=0;
	int		iReadBytes	= 0;
		
	// 이벤트 개수 체크
	fread(&nMission_total,sizeof(int),1,fMissiondata);
	apMissiondata.New(nMission_total);
	ASSERT(apMissiondata.Count() > 0 && "Invalid MissionCase Data");		
	ASSERT(nMission_total > 0 && "Invalid MissionCase Data");
	//////////////////////////////////////////////////////////////////////////	
	// MACRO DEFINITION
	//////////////////////////////////////////////////////////////////////////	
#define LOADINT(d)			iReadBytes = fread(&d, sizeof(int), 1, fMissiondata);
#define LOADSHORT(d)		iReadBytes = fread(&d, sizeof(short), 1, fMissiondata);
#define LOADCHAR(d)			iReadBytes = fread(&d, sizeof(char), 1, fMissiondata);
#define LOADFLOAT(d)		iReadBytes = fread(&d, sizeof(float), 1, fMissiondata);
#define LOADSTR(d)			{ int iLen; LOADINT(iLen); iReadBytes = fread(&d, iLen, 1, fMissiondata); }
	//////////////////////////////////////////////////////////////////////////	

	for(int i = 0; i < nMission_total; ++i)
	{
		CMissionCase& MC	= apMissiondata[i];
		TMissionData& MissionData	= MC.m_MissionData;		
		LOADINT(MissionData.nMission_index);
		LOADSTR(MissionData.strMission_name);
		LOADINT(MissionData.price);
		LOADINT(MissionData.iIconTexID);
		LOADINT(MissionData.iIconTexRow);
		LOADINT(MissionData.iIconTexCol);
		LOADCHAR(MissionData.bSkill);		
		LOADINT(MissionData.iPoint);
	}
	fclose(fMissiondata);
		
	#undef LOADINT
	#undef LOADCHAR
	#undef LOADFLOAT
	#undef LOADSTR				

	return nMission_total;
}
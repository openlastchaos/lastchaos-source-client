#include "stdh.h"

#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Entities/WildPetData.h>
#include <Engine/Network/CNetwork.h>

CWildPetData::CWildPetData(void)
{
	memset(&m_WildPetData, 0, sizeof(TWildPetData));	
}
CWildPetData::~CWildPetData(void)
{

}

int CWildPetData::LoadWildPetDataFromFile(CStaticArray<CWildPetData> &apWildPetData, const char* FileName)
{	
	FILE* fPetdata = NULL;
	if ((fPetdata = fopen(FileName, "rb")) == NULL) 
	{
		MessageBox(NULL, "Petdata File is not Exist.", "error!", MB_OK);
		return -1;
	}
	int		nPetData_total=0;
	int		iReadBytes	= 0;
		
	// 이벤트 개수 체크
	fread(&nPetData_total,sizeof(int),1,fPetdata);
	apWildPetData.New(nPetData_total);
	ASSERT(apWildPetData.Count() > 0 && "Invalid PetData Data");		
	ASSERT(nPetData_total > 0 && "Invalid PetData Data");
	//////////////////////////////////////////////////////////////////////////	
	// MACRO DEFINITION
	//////////////////////////////////////////////////////////////////////////	
#define LOADINT(d)			iReadBytes = fread(&d, sizeof(int), 1, fPetdata);
#define LOADSHORT(d)		iReadBytes = fread(&d, sizeof(short), 1, fPetdata);
#define LOADCHAR(d)			iReadBytes = fread(&d, sizeof(char), 1, fPetdata);
#define LOADFLOAT(d)		iReadBytes = fread(&d, sizeof(float), 1, fPetdata);
#define LOADSTR(d)			{ int iLen; LOADINT(iLen); iReadBytes = fread(&d, iLen, 1, fPetdata); }
	//////////////////////////////////////////////////////////////////////////	

	for(int i = 0; i < nPetData_total; ++i)
	{
		CWildPetData& PD	= apWildPetData[i];
		TWildPetData& WildPetData	= PD.m_WildPetData;
		LOADINT(WildPetData.nIndex);
		LOADSTR(WildPetData.strName);
		LOADINT(WildPetData.nType);
		LOADSTR(WildPetData.strFileName);
		
		for (int j = 0; j < WILD_PET_ANIM_TOTAL; j++)
		{
			LOADSTR(*WildPetData.PetAni[j]);
		}
	}
	fclose(fPetdata);
		
	#undef LOADINT
	#undef LOADCHAR
	#undef LOADFLOAT
	#undef LOADSTR				

	return nPetData_total;
}

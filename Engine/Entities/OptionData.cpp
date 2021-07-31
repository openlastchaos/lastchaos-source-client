#include "stdh.h"

#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Entities/OptionData.h>

/*
 *  Constructor.
 */
COptionData::COptionData(void)
{
	memset(&Option_Data, 0, sizeof(_tOptionStat));	
}

/*
 *  Destructor.
 */
COptionData::~COptionData(void) 
{
}

//-----------------------------------------------------------------------------
// Purpose: 아이템 데이터를 파일로부터 읽어들입니다.
// Input  : &apOptionData - 아이템 목록이 저장될 배열.
//			FileName - 파일명.
// Output : 	static int
//-----------------------------------------------------------------------------
int COptionData::LoadOptionDataFromFile(CStaticArray<COptionData> &apOptionData, const char* FileName)
{	
	FILE *fp		= NULL;
	if ((fp = fopen(FileName, "rb")) == NULL) 
	{
		MessageBox(NULL, "File is not Exist.", "error!", MB_OK);
		return -1;
	}

	int iLastOptionIndex	= 0;
	int iLength				= -1;
	int iReadBytes			= 0;

	iReadBytes = fread(&iLastOptionIndex, sizeof(int), 1, fp);				// ITEM 데이터의 마지막 인덱스.
	apOptionData.New(iLastOptionIndex);
	ASSERT(apOptionData.Count() >= iLastOptionIndex && "Invalid Array Count");
	ASSERT(iLastOptionIndex > 0 && "Invalid Option Data");

	//////////////////////////////////////////////////////////////////////////	
	// MACRO DEFINITION
	//////////////////////////////////////////////////////////////////////////	
#define LOADINT(d)			iReadBytes = fread(&d, sizeof(int), 1, fp);
#define LOADSHORT(d)		iReadBytes = fread(&d, sizeof(short), 1, fp);
#define LOADCHAR(d)			iReadBytes = fread(&d, sizeof(char), 1, fp);
#define LOADFLOAT(d)		iReadBytes = fread(&d, sizeof(float), 1, fp);
#define LOADSTR(d)			{ int iLen; LOADINT(iLen); iReadBytes = fread(&d, iLen, 1, fp); }
	//////////////////////////////////////////////////////////////////////////	

	int	nType;

	apOptionData.New(iLastOptionIndex);
	
	for(int i = 0; i < iLastOptionIndex; ++i)
	{
		int iIndex = -1;
		LOADINT(iIndex);
		if(iReadBytes <= 0)		break;										// EOF
		ASSERT(iIndex != -1	&& "Invalid Option Index");

		LOADINT(nType);

		COptionData& OD			= apOptionData[nType];
		_tOptionStat& OptionStat= OD.Option_Data;

		LOADSTR(OptionStat.name);
		iReadBytes = fread(&OptionStat.value,		sizeof(int), OPTION_MAX_LEVEL, fp);

		if(iReadBytes <= 0)
		{
			MessageBox(NULL, "옵션 화일이 올바르지 않습니다.", "Error!", MB_OK);
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
	return iLastOptionIndex;
}
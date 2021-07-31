#include "stdh.h"

#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Entities/NpcHelp.h>
//강동민 수정 시작 테스트 클라이언트 작업	06.09
#include <Engine/Entities/EntityClass.h>
#include <Engine/Entities/EntityProperties.h>
#include <Engine/Ska/StringTable.h>
//강동민 수정 끝 테스트 클라이언트 작업		06.09

/*
 *  Constructor.
 */
CNpcHelp::CNpcHelp(void)
{
	memset(&m_NpcList, 0, sizeof(TNpcList));

}

/*
 *  Destructor.
 */
CNpcHelp::~CNpcHelp(void) 
{

}
//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &apShopData - 
//			FileName - 
// Output : int
//-----------------------------------------------------------------------------
int CNpcHelp::LoadNpcListFromFile(CStaticArray<CNpcHelp> &apNpcList, const char* FileName)
{	
	FILE* fNpclist = NULL;
	if ((fNpclist = fopen(FileName, "rb")) == NULL) 
	{
		MessageBox(NULL, "NpcHelp File is not Exist.", "error!", MB_OK);
		return -1;
	}
	int		nNpc_total=0;
	int		iReadBytes	= 0;
		
	// 이벤트 개수 체크
	fread(&nNpc_total,sizeof(int),1,fNpclist);
	apNpcList.New(nNpc_total);
	ASSERT(apNpcList.Count() > 0 && "Invalid HelpNpc Data");		
	ASSERT(nNpc_total > 0 && "Invalid HelpNpc Data");
	//////////////////////////////////////////////////////////////////////////	
	// MACRO DEFINITION
	//////////////////////////////////////////////////////////////////////////	
#define LOADINT(d)			iReadBytes = fread(&d, sizeof(int), 1, fNpclist);
#define LOADSHORT(d)		iReadBytes = fread(&d, sizeof(short), 1, fNpclist);
#define LOADCHAR(d)			iReadBytes = fread(&d, sizeof(char), 1, fNpclist);
#define LOADFLOAT(d)		iReadBytes = fread(&d, sizeof(float), 1, fNpclist);
#define LOADSTR(d)			{ int iLen; LOADINT(iLen); iReadBytes = fread(&d, iLen, 1, fNpclist); }
	//////////////////////////////////////////////////////////////////////////	

	for(int i = 0; i < nNpc_total; ++i)
	{
		CNpcHelp& NH	= apNpcList[i];
		TNpcList& NpcList	= NH.m_NpcList;		
		LOADINT(NpcList.zone_index);
		LOADINT(NpcList.npc_index);
		LOADSTR(NpcList.npc_name);
		LOADSTR(NpcList.npc_explan);
	}
	fclose(fNpclist);
		
	#undef LOADINT
	#undef LOADCHAR
	#undef LOADFLOAT
	#undef LOADSTR				

	return nNpc_total;
}

#include "stdh.h"

#include <Engine/Entities/TargetInfo.h>

CTargetInfom::CTargetInfom(void)
{	
	Init();
}

CTargetInfom::~CTargetInfom(void)
{

}

void CTargetInfom::Init(void)
{
	fMaxHealth = 0;
	fHealth = 0;
	bIsActive = FALSE;
	iLevel = 0;
	TargetName[0] = NULL;
	pen_pEntity = NULL;
	PkMode = 0;
	PkState = 0;
	PkTitle = 0;
}
/////////////////////////////////////////////////
CTargetInfomReal::CTargetInfomReal(void)
{	
	Init();
}

CTargetInfomReal::~CTargetInfomReal(void)
{

}

void CTargetInfomReal::Init(void)
{
	fMaxHealth = 0;
	fHealth = 0;
	bIsActive = FALSE;
	iLevel = 0;
	TargetName[0] = NULL;
	pen_pEntity = NULL;
	PkMode = 0;
	PkState = 0;
	PkTitle = 0;
	llCount = 0;
	lGuildIndex		= -1;
	strGuildName	= "";
	sbGuildRank = 0;		// WSS_GUILDMASTER 070517
	ubGuildNameColor =0;	//[071123: Su-won] DRATAN_SIEGE_DUNGEON
}
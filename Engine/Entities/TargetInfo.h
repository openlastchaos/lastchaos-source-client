#ifndef INCL_TARGETINFOM_H
#define INCL_TARGETINFOM_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

//#include <Engine/Entities/Entity.h>
#include <Engine/Math/Vector.h>
#include <Engine/Base/CTString.h>

class  CTargetInfom
{
public:
	BOOL	bIsActive;
	SBYTE	TargetType;
	char	TargetName[30];
	FLOAT	fMaxHealth;
	FLOAT	fHealth;
	int		iLevel;
	CEntity	*pen_pEntity;
	int		PkMode;
	int		PkState;
	int     PkTitle;
	int		Legit;
	int		dbIdx;

public:
	CTargetInfom(void);
	~CTargetInfom(void);

	ENGINE_API void	Init();
};

class  CTargetInfomReal
{
public:
	BOOL	bIsActive;
	SBYTE	TargetType;
	char	TargetName[50];
	FLOAT	fMaxHealth;
	FLOAT	fHealth;
	int		iLevel;
	CEntity	*pen_pEntity;
	int		PkMode;
	int		PkState;
	int     PkTitle;
	SQUAD   llCount;
	int		Legit;
	LONG		lGuildIndex;	// 길드 인덱스
	CTString	strGuildName;	// 길드명
	SBYTE	sbGuildRank;		// WSS_GUILDMASTER 070517
	UBYTE	ubGuildNameColor;	//[071123: Su-won] DRATAN_SIEGE_DUNGEON

public:
	CTargetInfomReal(void);
	~CTargetInfomReal(void);

	ENGINE_API void	Init();
};

typedef enum _tagTargetType
{
	CHARACTER,
	MOB,
	ITEM,
	NPC,
	PRODUCTION,
	PET,	
	SUMMON,
	WILDPET,
} TARGET_TYPE;
//extern CTargetInfom _TargetInfo;

#endif
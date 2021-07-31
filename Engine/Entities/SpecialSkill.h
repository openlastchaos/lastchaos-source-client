#ifndef INCL_SPECIALSKILL_H
#define INCL_SPECIALSKILL_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

// Special Skill 타입
#define		SSKILL_MINING		0	// 채굴
#define		SSKILL_GATHERING	1	// 채집
#define		SSKILL_CHARGE		2	// 차지
#define		SSKILL_STONE		3	// 광석정련술
#define		SSKILL_PLANT		4	// 식물가공술
#define		SSKILL_ELEMENT		5	// 원소정제술

#define		SSKILL_MAKE_WEAPON	6	// 무기 제작 기술
#define		SSKILL_MAKE_WEAR	7	// 방어구 제작 기술
#define		SSKILL_MAKE_G_B		8	// 장갑 부츠 제작 기술
#define		SSKILL_MAKE_ARMOR	9	// 갑바 바지 제작 기술
#define		SSKILL_MAKE_H_S		10	// 투구 방패 제작 기술
#define		SSKILL_MAKE_POTINO	11  // 포션 제작 // Date : 2005-01-07,   By Lee Ki-hwan
#define		SSKILL_PROCESS_NPC	12	// 가공 NPC

#define		SSKILL_STONE_0		3	// 스톤 온 정련술
#define		SSKILL_STONE_1		4	// 스톤 리스 정련술
#define		SSKILL_STONE_2		5	// 스톤 웨버 정련술
#define		SSKILL_STONE_3		6	// 스톤 비스트 정련술
#define		SSKILL_STONE_4		7	// 스톤 웰스던 정련술

#define		SSKILL_PLANT_0		8	// 크락의 노란잎 가공술
#define		SSKILL_PLANT_1		9	// 크락의 줄기 가공술
#define		SSKILL_PLANT_2		10	// 크락의 파란잎 가공술
#define		SSKILL_PLANT_3		11	// 크락의 가시 가공술
#define		SSKILL_PLANT_4		12	// 크락의 꽃 가공술

#define		SSKILL_ELEMENT_0	13	// 원소 E등급 정제술
#define		SSKILL_ELEMENT_1	14	// 원소 D등급 정제술
#define		SSKILL_ELEMENT_2	15	// 원소 C등급 정제술
#define		SSKILL_ELEMENT_3	16	// 원소 B등급 정제술
#define		SSKILL_ELEMENT_4	17	// 원소 A등급 정제술

#define		SSKILL_MAX_LEVEL	5	// Special Skill 최대 레벨

class CSpecialSkill
{
public:
	struct _SSkillData
	{
		int		index;
		char	name[50];
		int		type;
		int		maxLevel;
		int		preference;
		
		int		needLevel[SSKILL_MAX_LEVEL];
		int		needSP[SSKILL_MAX_LEVEL];
		int		needSSkill;
		int		needSSkillLevel;
		int		textureID;
		int		textureRow;
		int		textureCol;
		
		char	desc[255];
	};

	CSpecialSkill();
	~CSpecialSkill();

	inline int GetIndex()			const
	{	return SSkill_Data.index;	}

	inline const char* GetName()	const
	{	return SSkill_Data.name;	}

	inline int GetType()			const
	{	return SSkill_Data.type;	}

	inline int GetLearnLevel( int iIndex) const
	{	return SSkill_Data.needLevel[iIndex];	}

	inline int GetLearnSP( int iIndex) const
	{	return SSkill_Data.needSP[iIndex];	}

	inline int GetLearnSkillIndex()		const
	{	return SSkill_Data.needSSkill;		}

	inline int GetLearnSkillLevel()	const
	{	return SSkill_Data.needSSkillLevel;	}

	inline int GetMaxLevel()	const
	{	return SSkill_Data.maxLevel;	}

	inline int GetPreference()	const
	{	return SSkill_Data.preference;	}

	inline int	GetIconTexID()	const { return SSkill_Data.textureID; }
	inline int	GetIconTexRow() const { return SSkill_Data.textureRow; }
	inline int	GetIconTexCol() const { return SSkill_Data.textureCol; }	

	inline const char* GetDescription()	const
	{	return SSkill_Data.desc;	}

	_SSkillData							SSkill_Data;

	static int LoadSSkillDataFromFile(CStaticArray<CSpecialSkill> &apSkillData, const char* FileName);
};
#endif
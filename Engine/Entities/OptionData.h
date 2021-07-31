#ifndef INCL_OPTIONDATA_H
#define INCL_OPTIONDATA_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#define		OPTION_STR_UP				0		// 힘 상승
#define		OPTION_DEX_UP				1		// 민첩 상승
#define		OPTION_INT_UP				2		// 지혜 상승
#define		OPTION_CON_UP				3		// 체질 상승
#define		OPTION_HP_UP				4		// 생명력 상승
#define		OPTION_MP_UP				5		// 마나 상승

#define		OPTION_DAMAGE_UP			6		// 물리 공격력 상승
#define		OPTION_MELEE_DAMAGE_UP		7		// 물리 근거리 공격력 상승
#define		OPTION_RANGE_DAMAGE_UP		8		// 물리 원거리 공격력 상승
#define		OPTION_MELEE_HIT_UP			9		// 물리 근접 명중률 상승
#define		OPTION_RANGE_HIT_UP			10		// 물리 원거리 명중률 상승

#define		OPTION_DEFENSE_UP			11		// 물리 방어력 상승
#define		OPTION_MELEE_DEFENSE_UP		12		// 물리 근거리 방어력 상승
#define		OPTION_RANGE_DEFENSE_UP		13		// 물리 원거리 방어력 상승
#define		OPTION_MELEE_AVOID_UP		14		// 물리 근접 회피율 상승
#define		OPTION_RANGE_AVOID_UP		15		// 물리 원거리 회피율 상승

#define		OPTION_MAGIC_UP				16		// 마법 공격력 상승
#define		OPTION_MAGIC_HIT_UP			17		// 마법 명중률 상승
#define		OPTION_RESIST_UP			18		// 마법 방어력 상승
#define		OPTION_RESIST_AVOID_UP		19		// 마법 회피율 상승

#define		OPTION_ALL_DAMAGE_UP		20		// 전체 공격력 상승 (근접물리, 원격물리, 마법)
#define		OPTION_ALL_HIT_UP			21		// 전체 명중률 상승 (근접물리, 원격물리, 마법)

#define		OPTION_ALL_DEFENSE_UP		22		// 전체 방어력 상승 (근접물리, 원격물리, 마법)
#define		OPTION_ALL_AVOID_UP			23		// 전체 회피율 상승 (근접물리, 원격물리, 마법)

#define		OPTION_MP_STILL				24		// 마나 스틸
#define		OPTION_HP_STILL				25		// 생명력 스틸

#define		OPTION_ATT_FIRE_UP			26		// 화염 속성 확률 상승
#define		OPTION_ATT_WATER_UP			27		// 빙정 속성 확률 상승
#define		OPTION_ATT_WIND_UP			28		// 폭풍 속성 확률 상승
#define		OPTION_ATT_EARTH_UP			29		// 대지 속성 확률 상승
#define		OPTION_ALL_ATT_UP			30		// 모든 속성 확률 상승

#define		OPTION_ATT_FIRE_DOWN		31		// 화염 속성 확률 감소
#define		OPTION_ATT_WATER_DOWN		32		// 빙정 속성 확률 감소
#define		OPTION_ATT_WIND_DOWN		33		// 폭풍 속성 확률 감소
#define		OPTION_ATT_EARTH_DOWN		34		// 대지 속성 확률 감소
#define		OPTION_ALL_ATT_DOWN			35		// 모든 속성 확률 감소

#define		MAX_NUM_OPTION				36		// 옵션 총 숫자
#define		MAX_ITEM_OPTION				6		// 아이템에 붙을 수 있는 최대 옵션 수
#define		OPTION_VALUE_SHIFT			8		// 2byte 값중 왼쪽 1Byte 옵션 번호 오른쪽 1Byte 옵션 레벨
#define		OPTION_VALUE_AND			0x11111111
#define		OPTION_MAX_LEVEL			7		// 옵션 최대 레벨 현재 7

class ENGINE_API COptionData
{
private:
	struct _tOptionStat
	{
		char	name[50];
		int		value[OPTION_MAX_LEVEL];
	};
	_tOptionStat	Option_Data;

public:
	/* Default constructor. */
	COptionData(void);
	/* Destructor. */
	~COptionData(void);	

	inline const char* GetName() const
	{	return Option_Data.name; }

	inline int GetValue(int iLevel) const
	{	return Option_Data.value[iLevel];	}

	//-----------------------------------------------------------------------------
	// Purpose: 옵션 데이터를 화일로부터 읽어들입니다.
	// Input  : &apOptionData - 저장될 배열.
	//			FileName - 파일명
	// Output : 	static int
	//-----------------------------------------------------------------------------
	static int LoadOptionDataFromFile(CStaticArray<COptionData> &apOptionData, const char* FileName);
};
#endif
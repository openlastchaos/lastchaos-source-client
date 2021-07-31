#ifndef INCL_MISSIONCASE_H
#define INCL_MISSIONCASE_H
#ifdef PRAGMA_ONCE
#pragma once
#endif
#include <Engine/Entities/Entity.h>
#include <vector>


class ENGINE_API CMissionCase
{
private:
	typedef struct _tagMissionData
	{
//		int		nMaxCount;			// 미션 케이스 총 갯수
		int		nMission_index;		// 미션 인덱스
		char	strMission_name[50];// 미션 네임
		int		price;				// 입장료
		int		iIconTexID;			// 아이콘 텍스쳐 ID
		int		iIconTexRow;		// 아이콘의 ROW상의 위치(최대 8)
		int		iIconTexCol;		// 아이콘의 COL상의 위치(최대 16)
		BOOL	bSkill;				// 미션 패널티 유무
		int		iPoint;				// 미션 포인트
	
	}TMissionData, *PTMissionData;
	
protected:
	
public:
	CMissionCase();
	~CMissionCase();


	TMissionData			m_MissionData;
	int						m_nMission_index;

	static int	LoadMissionFromFile(CStaticArray<CMissionCase> &apMissiondata, const char* FileName);
};
#endif
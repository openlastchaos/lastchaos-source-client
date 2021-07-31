// TODO. 후견인 시스템이 3가지로 나뉘어져 있기 때문에 유지 보수 하기 쉽도록
// Manager 클래스에서 각 상황에 맞게 관리 하기 위함.

#ifndef __HELPER_MGR_H__
#define __HELPER_MGR_H__

#include <Common/CommonDef.h>

enum eSelection
{
	SEL_HELPER_TEACHER,
	SEL_HELPER_STUDENT
};

// 후견인 신청 제한 레벨.
#define LIMIT_GUARDIAN_APP_LEVEL	(50)

class CHelperManager
{
public:
	CHelperManager();
	~CHelperManager();

	// 대화창 열기.
	void OpenDialog();
	void PrepareOpen();
	void ChangeHelperLevel( LONG lIndex, int iLevel );
	void DelFromHelperList( LONG lIndex );
	void AddToStudentInfoList(LONG index, const char *szFirstDate, const char *szFinalDate);
	void AddToHelperList( LONG lIndex, const CTString& strName, int iLevel, int iJob, int iJob2 );
	void AddToTeacherInfoList(LONG index, SLONG fame, SLONG cntTeaching, SLONG cntComplete, SLONG cntFail, SBYTE StartPlayTime = -1, SBYTE EndPlayTime = -1);

	void ClearHelperList();

	enum eTextInfo
	{
		ING_CNT = 0,
		COMPLETE_CNT,
		FAIL_CNT,
		SUCCESS_CNT,
		TEXT_INFO_MAX
	};

	// internal structure
	struct sHelperMember
	{
		sHelperMember()
		{
			lIndex			= -1;			
			iJob			= -1;
			iJob2			= -1;
			iLevel			= 0;
			strName			= CTString("");
		}

		bool operator<(const sHelperMember &other) const
		{
			if( lIndex < other.lIndex )
				return true;
			return false;
		}
		bool operator>(const sHelperMember &other) const
		{			
			return other.operator < (*this);
		}

		SLONG			lIndex;
		CTString		strName;			// 멤버명?		
		int				iLevel;
		int				iJob;
		int				iJob2;
	};

	// Function Object
	struct FindMember
	{
		SLONG lIndex;
		FindMember(sHelperMember& info) : lIndex(info.lIndex) {};
		bool operator() (const sHelperMember& info)
		{
			if(info.lIndex == lIndex)
			{
				return true;
			}
			else
				return false;
		}
	};

	struct sTeacherInfo
	{
		bool operator<(const sTeacherInfo &other) const
		{
			if( index < other.index )
				return true;
			return false;
		}
		bool operator>(const sTeacherInfo &other) const
		{			
			return other.operator < (*this);
		}
		LONG index;
		SLONG nFame;
		SLONG nCntTeachingStudent;
		SLONG nCntCompleteStudent;
		SLONG nCntFailStudent;
		SBYTE sbStartPlayTime;
		SBYTE sbEndPlayTime;
	};
	struct sStudentInfo
	{
		LONG index;
		char szFirstDate[50];
		char szFinalDate[50];
	};

	typedef std::vector<sHelperMember>	_vec_member;
	typedef std::vector<sStudentInfo>	_vec_sudent_info;
	typedef std::vector<sTeacherInfo>	_vec_teacher_info;
	_vec_member m_vectorHelperList;			// 단원 목록
	_vec_teacher_info m_vectorTeacherInfoList;	// 선생들의 정보 목록
	_vec_sudent_info m_vectorStudentInfoList;	// 학생들의 정보 목록

	sHelperMember* GetHelperMember(int idx)	{ return &m_vectorHelperList[idx];	}
	_vec_member GetHelperList()	{ return m_vectorHelperList;	}
	_vec_teacher_info GetTeacherInfoList()	{ return m_vectorTeacherInfoList;	}

private:
};
#endif // __HELPER_MGR_H__
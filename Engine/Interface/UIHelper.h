// ----------------------------------------------------------------------------
//  File : UIHelper.h
//  Desc : Created by Dongmin
// ----------------------------------------------------------------------------

#ifndef	UIHELPER_H_
#define	UIHELPER_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Interface/UIWindow.h>
#include <Engine/Interface/UIButton.h>
#include <Engine/Interface/UIListBox.h>
#include <Engine/Interface/UIEditBox.h>
#include <vector>

// Define text position
#define	HELPER_TITLE_TEXT_OFFSETX		25
#define	HELPER_TITLE_TEXT_OFFSETY		5

// Define size of Helper
#define	HELPER_WIDTH					250
#define	HELPER_HEIGHT					335

//toop-tip
#define HELPER_MAX_INFO_LINE 20

#define	HELPER_MAX_STUDENTS					(20)		// 최대 가입 신청자.

// ----------------------------------------------------------------------------
// Name : CUIHelper
// Desc :
// ----------------------------------------------------------------------------
class CUIHelper : public CUIWindow
{
protected:	
	// Helper State
	enum eHelperState
	{
		HELPER_MENU_REQ,
		HELPER_MENU_TEACHER,			// 견습생 관리
		HELPER_MENU_STUDENT,			// 후견인 관리
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

		~sHelperMember()
		{	
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
		LONG index;
		SLONG nFame;
		SLONG nCntTeachingStudent;
		SLONG nCntCompleteStudent;
		SLONG nCntFailStudent;
	};
	struct sStudentInfo
	{
		LONG index;
		char szFirstDate[50];
		char szFinalDate[50];
	};

	std::vector<sHelperMember>	m_vectorHelperList;			// 단원 목록
	std::vector<sTeacherInfo>	m_vectorTeacherInfoList;	// 선생들의 정보 목록
	std::vector<sStudentInfo>	m_vectorStudentInfoList;	// 학생들의 정보 목록

	// Controls
	CUIButton				m_btnClose;						// Close button

	CUIButton				m_btnRefreshTeacher;			// 후견인 목록 갱신
	CUIButton				m_btnRegisterTeacher;			// 후견인 등록
	CUIButton				m_btnBeMyTeacher;				// 견습생
	CUIButton				m_btnFireMyTeacher;				// 견습생 포기
	CUIButton				m_btnCancelRegister;			// 후견인 등록 취소

	CUIButton				m_btnExit;						// 닫기

	// Region of each part
	UIRect					m_rcTitle;						// Region of title bar

	// UV of each part
	UIRectUV				m_rtBackTop;					// UV of top background
	UIRectUV				m_rtBackMiddle2;				// UV of middle background
	UIRectUV				m_rtBackManagerTop;				//
	UIRectUV				m_rtBackManagerMiddle;			//
	UIRectUV				m_rtBackManagerBottom;			//
	UIRectUV				m_rtBackBottom;					// UV of bottom background	

	UIRectUV				m_rtTab;						// UV of section tab
	UIRect					m_rcTab;

	CUIListBox				m_lbTeacherList;				// List box of teacher member list
	CUIListBox				m_lbStudentList;				// List box of student list
	
	// Tool Tip	
	UIRectUV				m_rtInfoUL;								// UV of upper left region of information
	UIRectUV				m_rtInfoUM;								// UV of upper middle region of information
	UIRectUV				m_rtInfoUR;								// UV of upper right region of information
	UIRectUV				m_rtInfoML;								// UV of middle left region of information
	UIRectUV				m_rtInfoMM;								// UV of middle middle region of information
	UIRectUV				m_rtInfoMR;								// UV of middle right region of information
	UIRectUV				m_rtInfoLL;								// UV of lower left region of information
	UIRectUV				m_rtInfoLM;								// UV of lower middle region of information
	UIRectUV				m_rtInfoLR;								// UV of lower right region of information

	BOOL					m_bShowInfo;

	UIRect					m_rcInfo;
	int						m_nCurInfoLines;

	CTString				m_strInfo[HELPER_MAX_INFO_LINE];		// information string
	COLOR					m_colInfo[HELPER_MAX_INFO_LINE];		// Color of information string

	//info icon
	UIRectUV				m_rtInfoMarkUV;
	UIRect					m_rcInfoMark[HELPER_MAX_STUDENTS];

protected:
	eHelperState			m_eHelperMenuState;
	BOOL					m_bRegistredTeacher;
	int						m_iLimitLevel;					// 최대 레벨
public:
	CUIHelper();
	~CUIHelper();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();
	void	RenderInfo();

	//tool-tip
	void	ShowInfo(int line, BOOL bRenew);

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Open guild
	void	OpenHelper( );
	void	ResetHelper();	

	// 선생 & 학생 목록 관리	
	void	AddToHelperList( LONG lIndex, const CTString& strName, int iLevel, int iJob, int iJob2 );		// 멤버 목록에 추가
	void	AddToTeacherInfoList(LONG lIndex, SLONG fame, SLONG cntTeaching, SLONG cntComplete, SLONG cntFail);
	void	AddToStudentInfoList(LONG lIndex, const char *szFirstDate, const char *szFinalDate);
	void	DelFromHelperList( LONG lIndex );
	void	ClearHelperList();	
	void	ChangeHelperLevel( LONG lIndex, int iLevel );		

	// Command
	void	RegisterTeacher( );					// 후견인 등록
	void	CancelRegisterTeacher( );			// 후견인 등록 취소
	void	BeMyTeacher( );						// 견습생 신청
	void	FireMyTeacher();					// 견습생 포기	
	void	TeacherAccept( LONG lIndex, const CTString& strStudentName );		// 학생이 후견인이 되어달라고 요청했어요.

	// 후견인으로 등록되어 있는지 여부를 설정함.
	void	SetRegistredTeacher( BOOL bRegistered );

	// 멤버 목록 갱신	
	void	RefreshTeacherList( BOOL bInit = FALSE );
	void	RefreshStudentList( BOOL bInit = FALSE);

	int		GetLimitLevel();

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	MsgBoxLCommand( int nCommandCode, int nResult );

	// Set focus
	void	SetFocus( BOOL bVisible );
};

#endif	// UIHELPER_H_
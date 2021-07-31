#ifndef __HELPER_UI_H__
#define __HELPER_UI_H__

#define	HELPER_MAX_STUDENTS					(20)		// 최대 가입 신청자.

class CUIList;
class CHelperUI : public CUIWindow
{
public:
	CHelperUI();
	~CHelperUI();
	void initialize();
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Open guild
	void	OpenUI( );
	void	CloseUI();	

	void	TeacherAccept( LONG lIndex, const CTString& strStudentName );		// 학생이 후견인이 되어달라고 요청했어요.

	// 후견인으로 등록되어 있는지 여부를 설정함.
	void	SetRegistredTeacher( BOOL bRegistered );

	// 멤버 목록 갱신	
	void	RefreshStudentList( BOOL bInit = FALSE);

	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput, CTString &strSEInput );
	void	MsgBoxLCommand( int nCommandCode, int nResult );
	// Set focus
	void	SetFocus( BOOL bVisible );
	void	SetMyTeachInfo( SLONG CurTeachCnt, SLONG CompleteTeachCnt, SLONG FailTeachCnt );
	void	ChangeOverColor(int idx);
	void	ChangeLeaveColor(int idx);

private:
	// Command
	void	RegisterTeacher( );					// 후견인 등록
	void	CancelRegisterTeacher( );			// 후견인 등록 취소
	void	BeMyTeacher( );						// 견습생 신청
	void	FireMyStudent();
	void	AmendReq();

	CUIButton*	m_pBtnRegistTeacher;
	CUIList*	m_pListDisciple;
	CUIList*	m_pListInfo;

	int			m_nOldMouseOverIdx;
};
#endif // __HELPER_UI_H__
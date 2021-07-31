#ifndef __HELPER_STUDENT_UI_H__
#define __HELPER_STUDENT_UI_H__

class CHelper_StudentUI : public CUIWindow
{
public:
	CHelper_StudentUI();
	~CHelper_StudentUI();

	void initialize();
	void ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	void OpenUI();
	void CloseUI();
	void RefreshTeacherList();
	void MsgBoxLCommand( int nCommandCode, int nResult );
	void MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
private:
	
	CUIList* m_pList;

	void BeMyTeacher();
	void FireMyTeacher();
	void SendRefresh();

	CTString m_strTeacherName;
	SLONG	 m_slIndex;
};
#endif // __HELPER_STUDENT_UI_H__
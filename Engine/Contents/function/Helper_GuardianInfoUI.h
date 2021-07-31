#ifndef __HELPER_GUARDIANINFO_UI_H__
#define __HELPER_GUARDIANINFO_UI_H__

class CHelper_GuardianInfoUI : public CUIWindow
{
public:
	CHelper_GuardianInfoUI();
	~CHelper_GuardianInfoUI();

	void initialize();
	void ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	void OpenUI();
	void CloseUI();

	void MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void MsgBoxLCommand( int nCommandCode, int nResult );
private:

	void FireMyTeacher();
	void setData();
	CUIList* m_pListGuardInfo;
};
#endif // __HELPER_GUARDIANINFO_UI_H__
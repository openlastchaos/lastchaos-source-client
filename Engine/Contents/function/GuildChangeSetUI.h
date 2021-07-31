#ifndef	GUILD_CHANGE_SET_UI_H_
#define	GUILD_CHANGE_SET_UI_H_

class CUIButton;
class CUIEditBox;
class CUICheckButton;
class CUIComboBox;

class CGuildChangeSetUI : public CUIBase
{
public:
	CGuildChangeSetUI();
	~CGuildChangeSetUI();

	enum
	{
		eEDIT_CHANGE_POSITION = 0,
		eEDIT_EXP_MIN,
		eEDIT_EXP_MAX,
		eEDIT_FAME_MIN,
		eEDIT_FAME_MAX,
		eEDIT_MAX
	};

	void PopupUIOff();
	void PressOk();
	void FocuseChange();
	void KillFocusEditBox();

	void AddComboString(CTString strString);
	void SetString(int nEditId, CTString strString);
	void SetFocusEditBox(int nFocusIndex = -1);
	void SetFocusComboBox(BOOL bFocus);
	void SetStashUsable(BOOL bUse);
	void SetComboCurSelIdx(int nSelIdx);

	CTString GetString(int nEditId);
	BOOL GetStashUsable();
	int GetComboCurSelIdx();
	bool IsFocusedComboBox();
	bool IsFocused();
	

	void toggleVisible(BOOL bEnable);
	
	void openUI();
	void closeUI();
	void ResetUI();
	void ResetEditBox();
	void ResetComboBox();
	void ResetCharPostionString();
	void initialize();

	WMSG_RESULT KeyMessage( MSG *pMsg );
	WMSG_RESULT IMEMessage( MSG *pMsg );
	WMSG_RESULT	CharMessage( MSG *pMsg );

	WMSG_RESULT MouseMessage(MSG *pMsg);
private:
	enum
	{
		eBTN_CLOSE = 0,
		eBTN_OK,
		eBTN_CANCEL,
		eBTN_MAX
	};

	CUIButton* m_pBtn[eBTN_MAX];
	CUIEditBox* m_pEditBox[eEDIT_MAX];

	CUIComboBox* m_pCBCorps;
	CUICheckButton* m_pChkStashUsable;
};

#endif	// GUILD_CHANGE_SET_UI_H_


#ifndef	GUILD_DONATE_UI_H_
#define	GUILD_DONATE_UI_H_

class CUIButton;
class CUIEditBox;

enum
{
	eDONATE_UI_SET_ALL = 0,
	eDONATE_UI_CHANGE_INFO,
	eDONATE_UI_TYPE_MAX
};

class CGuildDonateUI : public CUIBase
{
public:
	CGuildDonateUI();
	~CGuildDonateUI();

	enum
	{
		eEDIT_EXP_MIN,
		eEDIT_EXP_MAX,
		eEDIT_EXP,
		eEDIT_FAME_MIN,
		eEDIT_FAME_MAX,
		eEDIT_FAME,
		eEDIT_MAX
	};

	void PopupUIOff();
	void PressOk();
	void FocuseChange();
	void KillFocusEditBox();

	void SetString(int nEditId, CTString strString);
	void SetFocusEditBox(int nFocusIndex = -1);
	CTString GetString(int nEditId);
	bool IsFocused();

	void toggleVisible(BOOL bEnable, int nUIType);
	void openUI();
	void closeUI();
	void updateUI();
	void ResetUI();
	void ResetEditBox();

	void initialize();

	WMSG_RESULT KeyMessage( MSG *pMsg );
	WMSG_RESULT IMEMessage( MSG *pMsg );
	WMSG_RESULT	CharMessage( MSG *pMsg );

	WMSG_RESULT MouseMessage( MSG *pMsg );

private:
	void SetStringLock();

	enum
	{
		eBTN_CLOSE = 0,
		eBTN_OK,
		eBTN_CANCEL,
		eBTN_MAX
	};

	CUIButton* m_pBtn[eBTN_MAX];
	CUIEditBox* m_pEditBox[eEDIT_MAX];
	bool m_bEditLock[eEDIT_MAX];

	int m_nOpenType;
private:
};

#endif	// GUILD_DONATE_UI_H_


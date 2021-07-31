#ifndef		_PARTY_AUTO_REG_BOSS_H_
#define		_PARTY_AUTO_REG_BOSS_H_


class CUIPartyAutoRegBoss : public CUIWindow
{
public:
	CUIPartyAutoRegBoss();

	void	initialize();

	void	open();
	void	close();	

	void	check_callback(int job_idx);

	BOOL	IsEditBoxFocused();
	void	KillFocusEditBox();

private:

	void	send_reg();
	void	check_level();

	DWORD			m_dwJobFlag;
	BOOL			m_bLimit_level;
	CUIEditBox*		m_pEditNotice;
	CUICheckButton* m_pCheckJob[TOTAL_JOB];
	CUICheckButton* m_pCheckLevel;

};

#endif		// _PARTY_AUTO_REG_BOSS_H_
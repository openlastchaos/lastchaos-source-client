#ifndef		GUILD_BATTLE_MATCH_UI_H_
#define		GUILD_BATTLE_MATCH_UI_H_

class CUIList;

class GuildBattleMatchUI : public CUIWindow
{
public:
	GuildBattleMatchUI();

	void	initialize();

	void	open();
	void	close();

	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);

	void	Updatedata();

private:
	void	_cb_sel_callback();
	void	_ls_sel_callback();

	void	_regist();
	void	_challenge();

	void	_press_prev();
	void	_press_next();

	int		m_nSelTime;

	CUIComboBox* m_pCbTime;
	CUIList*	m_pList;
	CUIButton*	m_pBtnRegist;	
};

#endif		// GUILD_BATTLE_MATCH_UI_H_
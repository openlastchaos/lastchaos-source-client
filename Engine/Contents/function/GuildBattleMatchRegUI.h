#ifndef		GUILD_BATTLE_MATCH_REG_H_
#define		GUILD_BATTLE_MATCH_REG_H_

class GuildBattleMatchRegUI : public CUIWindow
{
public:
	GuildBattleMatchRegUI();

	void	initialize();
	
	// 데이터 로딩이 필요하기 때문에 별도로 초기화 한다.
	void	init_zone();

	void	open();
	void	close();

	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);

	BOOL	IsEditBoxFocused();

private:

	enum eCHECK_VALUE
	{
		eCHECK_VALUE_OK = 0,

		eCHECK_VALUE_ERR_NAS_RANGE,
		eCHECK_VALUE_ERR_NAS_UNIT,

		eCHECK_VALUE_ERR_GP_RANGE,
		eCHECK_VALUE_ERR_GP_UNIT,

		eCHECK_VALUE_ERR_NO_SEL_TIME,
		eCHECK_VALUE_ERR_OVER_TIME_RANGE,

		eCHECK_VALUE_ERR_NO_SEL_ZONE,
		eCHECK_VALUE_ERR_OVER_ZONE_RANGE,
	};

	void	clear_value();

	void	_regist();
	int		_check_value();

	__int64			m_nNas;
	int				m_nGP;
	int				m_nTime;
	int				m_nZone;

	CUIEditBox*		m_pEdNas;
	CUIEditBox*		m_pEdGP;
	CUIComboBox*	m_pCbTime;
	CUIComboBox*	m_pCbZone;
};

#endif		// GUILD_BATTLE_MATCH_REG_H_
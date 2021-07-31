
#ifndef		GUILD_TAX_HISTORY_H_
#define		GUILD_TAX_HISTORY_H_

struct STakeInfo
{
	int			m_nMonth;
	int			m_nDay;
	LONGLONG	m_llMoney;

	void SetData( int nMonth, int nDay, LONGLONG llMoney )
	{
		m_nMonth	= nMonth;
		m_nDay		= nDay;
		m_llMoney	= llMoney;
	}
};

typedef std::vector<STakeInfo>	VTakeInfo;
typedef VTakeInfo::iterator		VTake_iter;

class CUIList;

class GuildTaxHistoryUI : public CUIWindow
{
public:
	GuildTaxHistoryUI();
	void initialize();

	void open();
	void close();

	void AddTakeInfo(int nMonth, int nDay, LONGLONG llMoney);
	void ResetTakeInfo();

private:
	void update_list();

	VTakeInfo m_vTakeInfo;
	CUIList* m_pList;
};


#endif		// GUILD_TAX_HISTORY_H_
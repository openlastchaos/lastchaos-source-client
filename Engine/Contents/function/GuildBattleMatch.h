#ifndef		GUILD_BATTLE_MATCH_H_
#define		GUILD_BATTLE_MATCH_H_

#include <Common/Packet/ptype_guild_battle.h>

class GuildBattleMatch
{
public:
	enum {
		time_max = 8,

		NAS_MIN = 1000000,			// 1,000,000
		NAS_MAX = 1000000000,		// 1,000,000,000

		GP_MIN = 1000,				// 1,000
		GP_MAX = 1000000,			// 1,000,000

		LIMIT_LV = 6,
	};

	GuildBattleMatch();
	~GuildBattleMatch();

	void clear_data();

	void init_zone();

	void RecvList(CNetworkMessage* istr);
	void RecvError(CNetworkMessage* istr);	
	void RecvChallenge(CNetworkMessage* istr);
	void RecvChallengeRes(CNetworkMessage* istr);
	void RecvGuildBattleChallengeAgree(CNetworkMessage* istr);

	// 목록 요청
	void SendReqList(int time, int gap);

	// 등록 요청
	void SendReqReg(__int64 nas, int gp, int time, int zone);

	// 등록 취소
	void SendReqRegCancel();

	// 도전 요청
	bool SendReqChallenge();

	// 도전장 응답
	void SendResChallenge(bool bYes);

	ResponseClient::GuildBattleData* get_data(int idx);
	int get_data_cnt()		{ return m_data.size(); }

	bool isNextPage()		{ return m_bNextPage; }
	bool isRegist()			{ return m_bRegist; }
	void set_curpage(int nPage)		{ m_nCurPage = nPage; }
	int get_curpage()		{ return m_nCurPage; }

	// 도전장 보여주기
	void show_challenge(const char* guild_name, int glv, 
		int mem_total, int mem_online, int avg_lv, int avg_lv_online,
		int war_cnt, int win_cnt, __int64 nas, int gp);

	int get_zone_cnt()			{ return m_zone.size(); }
	int get_zone_idx(int idx)	{ 
		if (idx < 0 || idx >= m_zone.size())
			return -1;
		return m_zone[idx]; 
	}

	void set_sel_list(int idx)	{ m_nSelList = idx; }

	static int arr_time[time_max];

private:
	typedef std::vector<ResponseClient::GuildBattleData*> vec_data;
	typedef	std::vector<int> vec_guildbattle_zone;	

	vec_data	m_data;
	vec_guildbattle_zone m_zone;
	
	bool		m_bNextPage;
	bool		m_bRegist;
	int			m_nCurPage;
	int			m_nSelList;
	int			m_nCurTime;
	INDEX		m_nReqCharIdx;
};


#endif		// GUILD_BATTLE_MATCH_H_
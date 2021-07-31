#ifndef		GUILD_BATTLE_MATCH_CHALLENGE_H_
#define		GUILD_BATTLE_MATCH_CHALLENGE_H_

class GuildBattleMatchChallengeUI : public CUIWindow
{
public:
	GuildBattleMatchChallengeUI();

	void	initialize();

	void	open(const char* guild_name,__int64 nas, int gp, int time, int zone);
	void	close();

	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);

private:

	void	_challenge();

	CUIText*	m_pTxtNas;
	CUIText*	m_pTxtGP;
	CUIText*	m_pTxtTime;
	CUIText*	m_pTxtZone;
	CUIText*	m_pTxtChallenge;
};

#endif		// GUILD_BATTLE_MATCH_CHALLENGE_H_
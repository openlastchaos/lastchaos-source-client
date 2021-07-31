#ifndef		_PARTY_AUTO_PARTY_LIST_H_
#define		_PARTY_AUTO_PARTY_LIST_H_

class CUIPartyAutoPartyList : public CUIWindow
{
public:
	CUIPartyAutoPartyList();

	void	initialize();
	void	open();
	void	close();

	void	ReceivePartyData(CNetworkMessage* istr);
	void	ReceivePartyJoinMessage(int nErrorcode, CNetworkMessage* istr);

	void	SendPartyReq(int nNum);

	void	MsgBoxCommand(int nCommandCode, BOOL bOK, CTString& strInput);

private:

	void	init_data();
	void	_join();

	void	PartyMatchJoin(SBYTE sbType, SLONG slBoss, CTString strBossName,
				SLONG slChar, CTString strCharName, SBYTE sbCharJob);

	int			m_nCurrentPageNum;
	std::vector<PartyListData>	m_vectorPartyListData;
	int			m_nCharIndex;

	CUIList*	m_pList;

	CUIImage*	m_pImgClass[TOTAL_JOB];
	CUIImage*	m_pImgType[PT_MAX];
	CUIImage*	m_pImgLevel;

	CUIButton*	m_pBtnPrev;
	CUIButton*	m_pBtnNext;

	CUIComboBox* m_pCbClass;
	CUICheckButton* m_pChkLimit;
	CUIComboBox* m_pCbType;
};


#endif		// _PARTY_AUTO_PARTY_LIST_H_
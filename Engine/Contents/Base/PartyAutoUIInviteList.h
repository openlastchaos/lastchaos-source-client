#ifndef		_PARTY_AUTO_INVITE_LIST_H_
#define		_PARTY_AUTO_INVITE_LIST_H_

class CUIPartyAutoInviteList : public CUIWindow
{
public:
	CUIPartyAutoInviteList();

	void	initialize();
	void	open();
	void	close();

	void	ReceiveMemberData(CNetworkMessage* istr);
	void	ReceivePartyInviteMessage(int nErrorcode, CNetworkMessage* istr);

	void	SendPartyReq(int nNum);

private:	
	void	init_data();
	void	_invite();	

	int			m_nCurrentPageNum;
	std::vector<PartyListData>	m_vectorPartyListData;

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


#endif		// _PARTY_AUTO_INVITE_LIST_H_
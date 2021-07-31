#ifndef __AFFINITY_UI_H__
#define __AFFINITY_UI_H__

class CAffinityUI : public CUIWindow
{
public:
	CAffinityUI();
	~CAffinityUI();

	void	initialize();
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	BOOL	CloseWindowByEsc()		{ CloseDonation();	return TRUE;	}
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	MsgBoxLCommand( int nCommandCode, int nResult );

	// User Definition func.
	ENGINE_API void	OpenAffinity( int iMobIndex, BOOL bHasQuest, FLOAT fX, FLOAT fZ );
	void	ToggleVisible();
	void	OpenDonation();
	void	CloseDonation();
	void	DonationReset();
	void	OpenShop( const int npcIdx );
	void	CloseAllUI();

	const BOOL	IsOpenUI() const;
	const FLOAT	GetNPCPosX() const			{ return m_fNpcX; }
	const FLOAT	GetNPCPosZ() const			{ return m_fNpcZ; }
	const SLONG	GetNPCIndex() const			{ return m_slNPCIdx; }	// [2010/12/07 : Sora] 친화도 개선 2차

	void Donation_Sum(int nSelect);
	void Donation_Sub(int nSelect);
	void Donation_Max(int nSelect);
	void _sendDonateMessage();

protected:
	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);

private:
	struct Item
	{
		INDEX index;
		int count;
		Item(INDEX index)
		{
			this->index = index;
			this->count = 0;
		}
	};
	std::vector<Item> m_vecItem;

	int	m_nAffinityPoint;
	int m_nAffinityIndex;

	bool	m_bDrag;
	int		m_nOldX;
	int		m_nOldY;

	SLONG	m_slNPCIdx;
	SLONG	m_slNPCVIdx;
	FLOAT	m_fNpcZ;
	FLOAT	m_fNpcX;

	CUIList* m_pListAffinity;
	CUIText* m_pWant2DonationPoint;
	CUIText* m_pTextDonationPoint;
	CUIBase* m_pDragArea;

	CUIListItem* GetListItem(int idx);
	Item* GetDonationItem(int idx);
	void UpdateItemCount(int nSelect, int nCnt);
	void UpdateWant2DonationPoint(int nSelect, int nOld, int nCur);
	void UpdateListItem();
	void UpdateDonationPoint();
};
#endif // __AFFINITY_UI_H__
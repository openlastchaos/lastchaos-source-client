#ifndef __COMPOUND_UI_H__
#define __COMPOUND_UI_H__

#define COMPOUND_ITEM_SLOT_COUNT			3

class CCompoundUI : public CUIWindow
{
public:
	CCompoundUI();
	~CCompoundUI();

	void initialize();
	// Open & close refine
	ENGINE_API	void	OpenCompound( int nItemIndex, SWORD nTab, SWORD inven_idx );
	void	CloseCompound();
	void	SetCompoundItem ();
	void	CompoundRep( SBYTE sbResult );
	void	SendCompoundReq();
	void	RemoveItem(int nIndex);
	void	RemoveItem(CUIIcon* pIcon);
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
private:
	BOOL m_bWaitResult;					// If UI wait result from server or not

	int	m_nCurItemSlot;
	int	m_nCurItemCount;

	int	m_nTextItemIndex;					// 조합문서의 정보
	int	m_nTab;
	int	m_nInvenIdx;

	CUIButton* m_pBtnOk;
	CUIIcon* m_pIcon[COMPOUND_ITEM_SLOT_COUNT];

	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	ResetCompoundItem ();
	void	setBtnEnable(bool b)	{ if (m_pBtnOk) m_pBtnOk->SetEnable(b ? TRUE : FALSE);	}
};
#endif // __COMPOUND_UI_H__
#ifndef __INSECT_COLLECT_UI_H__
#define __INSECT_COLLECT_UI_H__

class CInsectCollectUI : public CUIWindow
{
public:
	CInsectCollectUI();
	~CInsectCollectUI();

	void initialize();
	void Init( void );
	void OpenUI( int nTab, int inven_idx );
	void CloseUI();

	void ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );

	void UpDateItem( void );
	void DeleteItem();
	CTString GetInsectName( int n );

	void DropInsectItem( CUIIcon* pIcon );
	void DropItemPos(int nPos);
private:
	void SetBtnItem( int num, int nIndex );

	CUIArray* m_pArr;
	CUIText* m_pTextPoint;
	int	m_nDropPosition;
	int	m_nTab;
	int	m_nInvenIdx;
};
#endif // __INSECT_COLLECT_UI_H__
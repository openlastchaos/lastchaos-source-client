#ifndef __ITEMPRODUCT_UI_H__
#define __ITEMPRODUCT_UI_H__

class CItemProductUI : public CUIWindow
{
public:
	CItemProductUI();

	void initialize();
	void OpenUI();
	void CloseUI();

	void AddData(int idx);

	void ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void SendMakeItemList(int idx);
private:
	CUIList* m_pList;
};

#endif // __ITEMPRODUCT_UI_H__
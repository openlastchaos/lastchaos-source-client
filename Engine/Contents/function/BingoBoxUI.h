#ifndef __BINGOBOX_UI_H__
#define __BINGOBOX_UI_H__

class CBingoBoxUI : public CUIWindow
{
public:
	CBingoBoxUI();
	~CBingoBoxUI();
	
	void initialize();

public:
	void	Init(void);
	void	OpenBingoBox(int nTab, int inven_idx);
	void	ResetPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ);
	void	AdjustPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ);

	void	MsgBoxCommand(int nCommandCode, BOOL bOK, CTString &strInput);

	void	UpDateItem(void);
	void	FindBingo(int num, BOOL bAllSearch=FALSE);
	void	ErrorMessage(CNetworkMessage *istr);
	void	CloseUI();

protected:
	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);

private:
	typedef struct _BingoInfo
	{
		BOOL		bBingoEnable;
		BOOL		bHighlight;
		__int64		tmBingoStart; // using Highlight Start

		_BingoInfo()
		{
			bBingoEnable = FALSE;
			bHighlight = FALSE;
			tmBingoStart = 0;
		}
	}sBingoInfo;

	typedef struct _BingoOfCase
	{
		std::vector<int> vecBingo;
		BOOL	bMultiply;
	}sBingoOfCase;

	int m_nTab;
	int m_nBingo;
	int m_nInvenIdx;
	int	m_nSelectItem;
	int m_nTempItemArray;
	int m_nTempItemIndex;
	int m_nOldX;
	int m_nOldY;
	bool m_bDrag;
	bool m_bBtnDownInItem;
	
	CUIIcon* m_pIconTemp;
	CUIText* m_pTextTitle;
	CUIText* m_pTextScore;
	CUIArray* m_pArrayData;

	BOOL m_bSelectLock;

	CUIIcon* m_pIconInsectItem[9];
	sBingoInfo m_BingoBtnInfo[8];
	sBingoOfCase m_BingoOfCase[9];

	void	SetBtnItem(int num, int nIndex);
	void	UpdateBingo();
	void	UpdateScore(int nScore);
};

#endif // __BINGOBOX_UI_H__
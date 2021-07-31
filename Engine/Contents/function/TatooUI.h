#ifndef	TATOO_UI_H_
#define	TATOO_UI_H_

#include <Engine/Interface/UIWindow.h>

class CUIImage;
class CUIButton;
class CUIArray;

class CTatooUI : public CUIWindow
{
public:
	CTatooUI();
	void OpenTatoo();
	void Close();		// 종료

	void initialize();

	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);

	// Send
	void SendItemUse();	// 선택한 색상 및 문양을 보낸다
private:
	void CheckShowShape();			// 현재 선택한 색상에서 문양을 선택 할 수 있는지 체크	
		 
	int	 GetColorAndShapeIndex();	// 서버로 보낼 문양 인덱스를 얻는다.
	void MakeMessageBox();			// 메시지 박스를 만든다

	int _getSelectIndex(int nArrayType);
	void _setUI();
	enum
	{
		eBTN_CLOSE = 0,
		eBTN_OK,
		eBTN_CANCEL,
		eBTN_MAX
	};

	CUIButton* m_pBtn[eBTN_MAX];

	enum
	{
		eARRAY_COLOR = 0,	// 색상 선택
		eARRAY_PATTERN,		// 문양 선택
		eARRAY_MAX
	};

	CUIArray* m_pArray[eARRAY_MAX];

	enum
	{
		eIMG_HEART = 0,	// 하트패턴
		eIMG_CLOVER,	// 클로버패턴
		eIMG_DIA,		// 다이아패턴
		eIMG_SPADE,		// 스페이드패턴
		eIMG_SKULL,		// 해골패턴
		eIMG_COLORTEMP, // 흰색컬러아이콘
		eIMG_MAX
	};

	CUIImage* m_pImgTemp[eIMG_MAX];
	
	// drag
	CUIImage* m_pMoveArea;
};

#endif	// TATOO_UI_H_


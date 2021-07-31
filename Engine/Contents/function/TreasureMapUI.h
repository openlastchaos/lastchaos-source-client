#ifndef	TREASURE_MAP_UI_H
#define	TREASURE_MAP_UI_H

#include <Engine/Interface/UIWindow.h>

class CUIButton;
class CUIImage;

class CTreasureMapUI : public CUIWindow
{
public:
	CTreasureMapUI();

	void OpenMap(INDEX ZoneIndex, FLOAT fPosX, FLOAT fPosY);
	void Close();

	bool UpdateUI(INDEX ZoneIndex, FLOAT fPosX, FLOAT fPosY);

	void initialize();

	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);
private:
	CUIButton* m_pBtnClose;

	enum
	{
		eIMG_MAP = 0,
		eIMG_MARK,
		eIMG_MAX
	};

	CUIImage* m_pImg[eIMG_MAX];
	// drag
	CUIImage* m_pMoveArea;

	bool m_bDrag;
	int m_nOriX, m_nOriY;
	int m_nMouseX;
	int m_nMouseY;

	INDEX m_nZoneIndex;
	FLOAT m_fposX;
	FLOAT m_fposY;
};

#endif	// TREASURE_MAP_UI_H


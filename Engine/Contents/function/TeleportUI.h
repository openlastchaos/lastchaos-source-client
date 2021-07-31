#ifndef	TELEPORT_UI_H
#define	TELEPORT_UI_H

#include <Engine/Interface/UIWindow.h>

#define	DEF_TELEPORT_EXTEND_MAX_MEMO	15

class CUIText;
class CUIButton;
class CUIImage;
class CUIList;

class CTeleportUI : public CUIWindow
{
public:
	CTeleportUI();

	void ClearTeleportList();
	void ListItemDown();
	void UpdateListItem(int nSlotIdx);

	void PressMoveBtn();
	void PressSaveBtn();

	void ShowTeleportError();
	void SetTeleportInfo( UBYTE ubSlot, SLONG slZone, FLOAT fX, FLOAT fZ, CTString &strComment );
	void NotUseMemEx();

	void MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	// Open & close teleport
	void OpenTeleport();
	void CloseTeleport();

	void initialize();

	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);

	// Set Get
	void SetUseTime(int t);
	int GetUseTime() { return m_useTime; }

private:
	// Internal functions
	void OpenMemorizeMsgBox();

	// Network message functions ( send )
	void SendTeleportWrite();
	void SendTeleportMove();

	void _setUI();
	void _setBtnEnable(int nBtnIdx, BOOL bEnable);
	void _setList();
	void _setListIndex(int nIndex);
	void _setExtendTimeString();
	
	int _getListSelectIndex();
	int _getMaxSlot();
	enum
	{
		eBTN_CLOSE = 0,
		eBTN_MOVE,
		eBTN_SAVE,
		eBTN_CANCEL,
		eBTN_MAX
	};

	CUIButton* m_pBtn[eBTN_MAX];
	CUIList* m_plistPortal;
	CUIText* m_pExtendTime;
	// drag
	CUIImage* m_pMoveArea;

	CTString m_strZone[DEF_TELEPORT_EXTEND_MAX_MEMO];	// Strings of zone name
	CTString m_strCoord[DEF_TELEPORT_EXTEND_MAX_MEMO];	// Strings of coordinate
	CTString m_strMemo[DEF_TELEPORT_EXTEND_MAX_MEMO];	// Strings of memo
	int		 m_nZoneNum[DEF_TELEPORT_EXTEND_MAX_MEMO];	// Numbers of zone

	int m_useTime; // is used memscroll_ex item
};

#endif	// TELEPORT_UI_H


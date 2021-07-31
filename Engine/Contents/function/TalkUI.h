#ifndef _TALK_UI_H_
#define _TALK_UI_H_

#include <Engine/Interface/UIWindow.h>
#include <Engine/Interface/UIGroup.h>

class CUIImage;
class CUIList;
class CUIArray;
class CUIButton;
class CUIImageSplit;
class CUIText;
class CUIEditBox;
class CUICheckButton;
class CUITextBoxEx;

class CTalkUI : public CUIWindow
{
public:
	CTalkUI();
	~CTalkUI();

	void SelectChatColor();
	void DropMessengerItem();
	void AddTalkListString( CTString strName, CTString strMessage, bool bFocus = true, COLOR colDesc = 0xF2F2F2FF );
	void AddTalkListString( CTString &strDesc, COLOR colDesc = 0xF2F2F2FF );
	void AddErrorTalk( CTString &strDesc, COLOR colDesc = 0xE28769FF );
	void AddTalkTarget( CTString strName);
	void DeleteTalkTarget(CTString strName);
	
	void Open( CTString strName );
	void Open( int clientIndex, const CMemberInfo targetInfo );

	void ToggleVisibleMemberList();
	void ToggleVisibleChangeChatColor();
	
	void Close();
	void Clear();

	void initialize();

	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);
	WMSG_RESULT OnKeyMessage(MSG* pMsg);
	WMSG_RESULT OnCharMessage(MSG* pMsg);
	WMSG_RESULT OnIMEMessage(MSG* pMsg);

	// Adjust position
	void ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Network
	void SendChatMessageFiltering( CTString strMessage );
	void SendChatMessage( CTString strMessage );

	// Set Get
	void SetFocus( BOOL bVisible );
	void SetIndex(int nMakeCharIndex, int nChatIndex) { m_nMakeCharIndex = nMakeCharIndex; m_nChatIndex = nChatIndex;};
	void SetUIIndex(int iUI) { m_nWhichUI = iUI; }
	int GetServerIndex() { return m_nChatIndex; }
	int GetTargetIndex() { return m_nTargetCharIndex; }
	int GetTargetSize()	{ return m_vecTarget.size(); }

	bool IsExistTarget( CTString strName );
	BOOL IsEditBoxFocused();
	void KillFocusEditBox();
private:
	int calcMaxStringCount();
	int calcMaxTargetNameLength();
	void updateTargetList();
	void updateTargetListSize();
	void resetTargetList();
	void resetTalkList();

	// Main
	int m_nMakeCharIndex;
	int m_nChatIndex;
	int m_nTargetCharIndex;
	int m_nWhichUI;
	int m_nColIndex;
	// Data	
	std::vector<CTString> m_vecTarget;		// 대화상대 정보 
	std::vector<CTString> m_vecTalkList;	// 대화 내용

	// UI
	CUIImage* m_pMoveArea;
	CUIImage* m_pImgChatColor;
	CUIText* m_pTextMemberInfo;
	CUIButton* m_pBtnClose;
	CUIArray* m_pArrChatColor;
	CUIEditBox* m_pEditInput;
	CUITextBoxEx* m_pChatDesign;
	enum
	{
		eCHK_MEMBER = 0,
		eCHK_COLOR,
		eCHK_MAX
	};

	CUICheckButton* m_pCheckBtn[eCHK_MAX];

	enum
	{
		eIMGSPLIT_BACK = 0,
		eIMGSPLIT_MEMBER,
		eIMGSPLIT_MAX
	};

	CUIImageSplit* m_pImgSplit[eIMGSPLIT_MAX];

	enum
	{
		eLIST_CHAT = 0,
		eLIST_MEMBER,
		eLIST_MAX
	};
	
	CUIList* m_pList[eLIST_MAX];
};

#endif	//	_TALK_UI_H_


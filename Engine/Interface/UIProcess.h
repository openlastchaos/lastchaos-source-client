
// ----------------------------------------------------------------------------
//  File : UIProcess.h
//  Desc : Created by 이기환
// ----------------------------------------------------------------------------

#ifndef	UIProcess_H
#define	UIProcess_H
#ifdef	PRAGMA_ONCE 
	#pragma once
#endif

// 네트워크 메세지를 위한 데이터 구조체 
struct CNeedItems 
{	
	CItemData*	ItemData;
	SWORD		MatTab;
	SWORD		inven_idx;
	__int64		llCount;

	CNeedItems() : ItemData(NULL) {}
};

// ----------------------------------------------------------------------------
// Name : CUIProcess
// Desc : 제조 
// ----------------------------------------------------------------------------
class CUIProcess : public CUIWindow
{
protected:
	
// Controls...
	
	// Skill buttons
	std::vector< CItems* >	m_vecItems;							// 가공 아이템	
	
//	Process Item Info
	int						m_nProcessText;						// 가공 문서 아이템 인덱스
	SWORD					m_nTab;
	SLONG					m_nInvenIdx;

	int						m_nSelectProcessItem;				// 현재 선택된 가공물
	
	BOOL					m_bSatisfied;						// 조건이 충분한가?
	
	CNeedItems				m_NeedItems[MAX_MAKE_ITEM_MATERIAL];// 필요 아이템 정보
		
// Network ...
	BOOL					m_bWaitProcessResult;				// Wait Message

public:
	CUIProcess();
	~CUIProcess();

	void	initialize();
	
// Open & close Process
	void	Clear ();
	void	InitProcess();

	void	OpenProcess( int nItemIndex, int nTab, int inven_idx );
	void	CloseProcess();

// Network message functions ( receive )
	void	ProcessRep( SBYTE sbResult );
	
// Send ...
	void	SendProcessReq();

// etc ...
	void	SelectItem ( int nIndex = -1 );
	void	AddString ( CTString& strText, COLOR colText = 0xffffffff );

	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);

private:
	
	void	clearItems();

	void	pushback_string(CTString& strText, COLOR color);
	void	callback_select();

	CUIText*	m_pTxtSubject;
	CUIList*	m_pListItems;
	CUIList*	m_pListDesc;
	CUIButton*	m_pBtnOK;
};


#endif	// UIProcess



// ----------------------------------------------------------------------------
//  File : UIProcessNPC.h
//  Desc : Created by 이기환
// ----------------------------------------------------------------------------

#ifndef	UIProcessNPC_H_
#define	UIProcessNPC_H_
#ifdef	PRAGMA_ONCE 
#pragma once
#endif

#include <Engine/Entities/ItemData.h>
#include <Engine/Interface/UIProcess.h>

// Define max char and line of strings
#define	MAX_PROCESSNPC_STRING			4
#define	PROCESSNPC_STRING_CHAR_WIDTH	170

#define PROCESSNPC_SLOT_ROW			4
#define PROCESSNPC_SLOT_ROW_TOTAL		30


// ----------------------------------------------------------------------------
// Name : CUIProcessNPC
// Desc : 제조 
// ----------------------------------------------------------------------------
class CUIProcessNPC : public CUIWindow
{
protected:
	// Skill buttons
	typedef std::vector< CItems* >::iterator		vecItems_Iter;
	std::vector< CItems* >	m_vecItems;							// 가공 아이템
	int						m_nProcessItemCount;				// 만들고자 하는 수.
		
	int						m_nSelectProcessNPCItem;			// 현재 선택된 가공물
	
	BOOL					m_bSatisfied;						// 조건이 충분한가?
	
	CNeedItems				m_NeedItems[MAX_MAKE_ITEM_MATERIAL];// 필요 아이템 정보
	
	int						m_nNeedItemCount;					// 필요한 아이템 종류의 수
	
	// Network ...
	BOOL					m_bWaitProcessNPCResult;				// Wait Message

	int						m_iResourceType;
	int						m_iResourceSubType;
	int						m_iLastResourceType;
	int						m_iLastResourceSubType;
	
public:
	CUIProcessNPC();
	~CUIProcessNPC();

	void initialize();
		
	// Open & close ProcessNPC
	void	Clear ();
	void	InitProcessNPC();
	
	ENGINE_API	void	OpenProcessNPC( );
	ENGINE_API	void	OpenProcessList( int iType, int iSubType );
	void				CloseProcessNPC();

	// SpinCtrl 에 입력 받으려면 필요
	BOOL	IsEditBoxFocused() { 
		if (m_nSelectProcessNPCItem >= 0)
			return TRUE;

		return FALSE;
	}
	
	// Network message functions ( receive )
	void	ProcessNPCRep( LONG processItemDBIndex, LONG count, LONG result );
	
	// Send ...
	void	SendProcessNPCReq();
	
	// etc ...
	void	SelectItem ( int nIndex = -1 );
	void	AddString ( CTString& strText, COLOR colText = 0xffffffff );

	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	MsgBoxLCommand( int nCommandCode, int nResult );

private:
	void	clearVecBtnEX();

	void	pushback_string(CTString& strText, COLOR color);
	void	callback_select();

	void	show_spinctrl(int idx, BOOL bhide);

	void	callback_spin();

	CUIList*	m_pListItems;
	CUIList*	m_pListDesc;
	CUIButton*	m_pBtnOK;

	int			m_nNeedTextWidth;		// 필요 아이템 텍스트 넓이
};


#endif	// UIProcessNPC


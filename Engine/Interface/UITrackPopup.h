// ----------------------------------------------------------------------------
//  File : UITrackPopup.h
//  Desc : Created by cpp2angel
// ----------------------------------------------------------------------------

#ifndef	UITRACKPOPUP_H_
#define	UITRACKPOPUP_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Interface/UIListBox.h>

//------------------------------------------------------------------------------
// class CUITrackPopup 
// Explain:  
// Date : 2005-05-16,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
class CUITrackPopup : public CUIListBox
{
protected:
	
public : // 9개나 만들 필요가 ... 구찮다.. 난중에 만들장..~
	// Tool Tip	
	UIRectUV		m_rtBackUL;				// UV of upper left region of Back				
	UIRectUV		m_rtBackUM;				// UV of upper middle region of Back				
	UIRectUV		m_rtBackUR;				// UV of upper right region of Back				
	UIRectUV		m_rtBackML;				// UV of middle left region of Back				
	UIRectUV		m_rtBackMM;				// UV of middle middle region of Back				
	UIRectUV		m_rtBackMR;				// UV of middle right region of Back				
	UIRectUV		m_rtBackLL;				// UV of lower left region of Back				
	UIRectUV		m_rtBackLM;				// UV of lower middle region of Back				
	UIRectUV		m_rtBackLR;				// UV of lower right region of Back				
	
	int				m_nMaxCharCount; 
public:
	CUITrackPopup();
	virtual ~CUITrackPopup();
	void Render();
	
	void AddMenuList( CTString strText, COLOR colText = 0xc2bac5FF );
	void DeleteMenuList( int nCol, int nIndex );
	
	void Resize();
	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	void ResetAllStrings();

	void Hide()	{	SetEnable( FALSE ); SetVisible( FALSE ); SetCurSel( -1 );	}
	void Show()	{	SetEnable( TRUE ); SetVisible( TRUE );		}
	
};


#endif	// UISCROLLBAR_H_

//DEL 
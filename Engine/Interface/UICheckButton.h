// ----------------------------------------------------------------------------
//  File : UICheckButton.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UICHECKBUTTON_H_
#define	UICHECKBUTTON_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Interface/UIWindow.h>


// UI check button state
enum UICheckBtnState
{
	UCBS_NONE			= 0,
	UCBS_CHECK			= 1,
	UCBS_NONE_DISABLE	= 2,
	UCBS_CHECK_DISABLE	= 3,
	UCBS_TOTAL			= 4,
};


// ----------------------------------------------------------------------------
// Name : CUICheckButton
// Desc :
// ----------------------------------------------------------------------------
class CUICheckButton : public CUIWindow
{
protected:
	BOOL			m_bChecked;						// If button is checked
	CTString		m_strText;						// String of button text
	BOOL			m_bValidText;					// If text is valid or not
	COLOR			m_colText[2];					// Color of button text
	int				m_nTextSX, m_nTextSY;			// Position of text
	UIRect			m_rcCheckRegion;				// Checking region
	UIRectUV		m_rtUV[UCBS_TOTAL];				// UV of each button stste

public:
	CUICheckButton();
	~CUICheckButton();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight,
					CTString &strText, BOOL bCheckRegionLeft = TRUE, int nTextSX = 0, int nCheckRegion = 0 );

	// Render
	void	Render();

	// Check state
	void	SetCheck( BOOL bCheck ) { m_bChecked = bCheck; }
	BOOL	IsChecked() const { return m_bChecked; }

	// Text
	void	SetText( CTString &strText );

	// Color
	void	SetTextColor( BOOL bEnableState, COLOR colText )
	{
		bEnableState ? m_colText[1] = colText : m_colText[0] = colText;
	}

	// UV
	void	SetUV( UICheckBtnState bsState,
					FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_rtUV[bsState].SetUV( fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	void	SetUV( UICheckBtnState bsState, UIRectUV &rt)
	{
		m_rtUV[bsState].CopyUV(rt);
	}
	void	CopyUV( UICheckBtnState bsSrcState, UICheckBtnState bsDstState )
	{
		m_rtUV[bsDstState].CopyUV( m_rtUV[bsSrcState] );
	}

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
};


#endif	// UICHECKBUTTON_H_


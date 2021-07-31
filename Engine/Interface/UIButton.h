// ----------------------------------------------------------------------------
//  File : UIButton.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UIBUTTON_H_
#define	UIBUTTON_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


#include <Engine/Interface/UIWindow.h>


// UI button state
enum UIBtnState
{
	UBS_IDLE	= 0,
	UBS_ON,
	UBS_CLICK,
	UBS_DISABLE,
	UBS_TOTAL,
};


// ----------------------------------------------------------------------------
// Name : CUIButton
// Desc :
// ----------------------------------------------------------------------------
class CUIButton : public CUIWindow
{
protected:

	BOOL			m_bLButtonDown;					// If left button is down or not
	BOOL			m_bUseNewType;					// Using for "CUIRectSurface" class
	CTString		m_strText;						// String of button text
	BOOL			m_bValidText;					// If text is valid or not
	BOOL			m_bOnlyText;					// If button has only text
	BOOL			m_bVerticality;
	COLOR			m_colText[UBS_TOTAL];			// Color of button text
	UIBtnState		m_bsState;						// State of UI button
	UIRectUV		m_rtUV[UBS_TOTAL];				// UV of each button stste
	CUIRectSurface	m_rtSurface[UBS_TOTAL];

public:
	CUIButton();
	~CUIButton();

	// Create
	void	Create( CUIWindow *pParentWnd, CTString &strText, int nX, int nY, int nWidth, int nHeight );

	// Render
	// [090709: selo] Render() 함수에 텍스쳐 색상을 지정할 수 있게 함
	void	Render( COLOR textureColor = 0xFFFFFFFF );
	void	RenderHighlight( const COLOR colHighlight );

	// Text
	int		SetText( CTString &strText, BOOL bVerticality = FALSE );
	inline CTString	GetText() {return m_strText;}	// WSS_NEW_GUILD_SYSTEM 070721 

	// Color
	void	SetTextColor( UIBtnState bsState, COLOR colText ) { m_colText[bsState] = colText; }

	// Window state
	void	SetEnable( BOOL bEnable )
	{
		if( bEnable )
		{
			m_dwWndState |= UWS_ENABLE;
			SetBtnState( UBS_IDLE );
		}
		else
		{
			m_dwWndState &= ~UWS_ENABLE;
			SetBtnState( UBS_DISABLE );
		}
	}

	// Button state
	void	SetBtnState( UIBtnState bsState ) { m_bsState = bsState; }
	UIBtnState	GetBtnState() const { return m_bsState; }

	// UV
	void	SetUV( UIBtnState bsState,
					FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_rtUV[bsState].SetUV( fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	void	CopyUV( UIBtnState bsSrcState, UIBtnState bsDstState )
	{
		m_rtUV[bsDstState].CopyUV( m_rtUV[bsSrcState] );
	}
	void	CopyUV( UIBtnState bsState, UIRectUV &rtUV )
	{
		m_rtUV[bsState].CopyUV( rtUV );
	}

	void	SetRTSurface(UIBtnState bsState, UIRect rt, UIRectUV uv)
	{
		m_rtSurface[bsState].AddRectSurface(rt, uv);
	}

	void	CopyRTSurface(UIBtnState Src, UIBtnState Dest)
	{
		m_rtSurface[Dest] = m_rtSurface[Src];
	}

	void	CopyRTSurface(UIBtnState Src, CUIRectSurface& rtSurface)
	{
		m_rtSurface[Src] = rtSurface;
	}

	UIRectUV	&GetUV( UIBtnState bsState ) { return m_rtUV[bsState]; }
	CUIRectSurface	&GetRectSurface(UIBtnState bsState) { return m_rtSurface[bsState]; }

	void	SetNewType(BOOL bTrue)
	{
		m_bUseNewType = bTrue;
	}

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
};


#endif	// UIBUTTON_H_


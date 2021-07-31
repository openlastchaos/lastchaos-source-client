// ----------------------------------------------------------------------------
//  File : UIWindow.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UIWINDOW_H_
#define	UIWINDOW_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#ifndef	WM_MOUSEWHEEL
	#define	WM_MOUSEWHEEL	0x020A
#endif


#ifndef	WHEEL_DELTA
	#define	WHEEL_DELTA		120
#endif

//#include <Engine/Graphics/DrawPort.h>
#include "UIBase.h"

// ----------------------------------------------------------------------------
// Name : CUIWindow
// Desc :
// ----------------------------------------------------------------------------
class CUIWindow : public CUIBase
{
protected:
	CTextureData	*m_ptdBaseTexture;			// Texture of window

public:

    CUIWindow() 
		: /*m_pParentWnd( NULL ),*/ m_ptdBaseTexture( NULL )
		, m_rcTitle(_UIRect(0, 0, 0, 0))
		, m_bTitleClick(false)
    {
		setType(eUI_CONTROL_WINDOW);
	}
	virtual ~CUIWindow() { Destroy(); }

	// Create & destroy window
	virtual void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
	{
		m_pParent = pParentWnd;
		SetPos( nX, nY );
		SetSize( nWidth, nHeight );
	}

	void Destroy()
	{
		CUIBase::Destroy();
		STOCK_RELEASE(m_ptdBaseTexture);
	}

	// Render window
	virtual void	PostRender() {}

	// Leave

	virtual void	FocusLeave() {}

	virtual void	OnRender(CDrawPort* pDraw)
	{
#ifdef UI_TOOL
		if(m_bSelect == FALSE)
			return;
		RenderBorder( pDraw );
#endif //UI_TOOL
	}
	
	virtual BOOL	IsEditBoxFocused() { return FALSE; }
	virtual void	KillFocusEditBox() {}

	

	// Adjust position
	virtual void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ ) {}
	virtual void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ ) {}
	virtual void	ResetSavePosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ ) {} // 유저 정보가 저장되는 UI 리셋용 함수

	// Window size
	void	SetSize( int nWidth, int nHeight ) { m_nWidth = nWidth; m_nHeight = nHeight; }
	void	SetWidth( int nWidth ) { m_nWidth = nWidth; }
	void	SetHeight( int nHeight ) { m_nHeight = nHeight; }

	// Region check
	
	// Rectangle region
	BOOL	IsInsideRect( int nX, int nY, const UIRect& rcRect )
	{
		ConvertToWindow( nX, nY );

		if( nX >= rcRect.Left && nY >= rcRect.Top && nX < rcRect.Right && nY < rcRect.Bottom )
			return TRUE;
		return FALSE;
	}

	// Command functions
	virtual void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput ) {}
	virtual void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput ,CTString &strConfirm) {}
	virtual void	MsgBoxLCommand( int nCommandCode, int nResult ) {}
	virtual void	MsgCommonCommand( int nCommandCode ) {} // 커먼 메시지 커맨드 (타이머 등의 커맨드 호출에 사용하기 위해 만듬) 처리할 내용은 각자의 UI에서 모두 처리한다
	
	// Close Window by Escape Key Event.
	virtual BOOL	CloseWindowByEsc() {	return FALSE;	}

	void Hide( BOOL bHide ) {
		if (bHide == TRUE)
			CloseProc();

		CUIBase::Hide(bHide);
	}

	void setTitleRect(int l, int t, int r, int b) { m_rcTitle.SetRect(l, t, r, b); }

	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y) {
		if (m_bHide)
			return WMSG_OB;

		if (IsInside(x, y) == FALSE)
			return WMSG_OB;

		if (IsInsideRect(x, y, m_rcTitle))
		{
			m_bTitleClick = true;

			ox = x;
			oy = y;
		}
		
		return CUIBase::OnLButtonDown(x, y);
	}

	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg)
	{
		if (m_bHide)
			return WMSG_FAIL;

		if (m_bTitleClick && (pMsg->wParam & MK_LBUTTON))
		{
			int	ndX = x - ox;
			int	ndY = y - oy;

			ox = x;
			oy = y;

			Move(ndX, ndY);

			return WMSG_SUCCESS;
		}

		if (IsInside(x, y) == FALSE)
		{
			m_bTitleClick = false;
			return WMSG_FAIL;
		}

		return CUIBase::OnMouseMove(x, y, pMsg);
	}

	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y)
	{
		if (m_bHide)
			return WMSG_OB;

		m_bTitleClick = false;

		if (IsInside(x, y) == FALSE)
			return WMSG_OB;

		return CUIBase::OnLButtonUp(x, y);
	}

private:
	UINT16		ox, oy;
	bool		m_bTitleClick;
	UIRect		m_rcTitle;
};


#endif	// UIWINDOW_H_
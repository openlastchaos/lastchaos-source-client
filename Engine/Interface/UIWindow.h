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

#include <Engine/Graphics/DrawPort.h>
#include <Engine/Templates/Stock_CTextureData.h>


// Rectangle region of UI
typedef struct _UIRect
{
	_UIRect() : Left(0), Top(0), Right(0), Bottom(0) {}
	_UIRect(int l, int t, int r, int b) { SetRect(l, t, r, b); }
	~_UIRect() {}

	int		Left, Top, Right, Bottom;

	void	SetRect( int nLeft, int nTop, int nRight, int nBottom )
	{
		Left = nLeft; Top = nTop; Right = nRight; Bottom = nBottom;
	}
	void	CopyRect( const _UIRect &rcSrc )
	{
		Left = rcSrc.Left; Top = rcSrc.Top; Right = rcSrc.Right; Bottom = rcSrc.Bottom;
	}
	void	Scale( FLOAT fScale )
	{
		Left *= fScale;	Top *= fScale;	Right *= fScale;	Bottom *= fScale;
	}
	void	Offset( int nX, int nY )
	{
		Left += nX;		Top += nY;		Right += nX;		Bottom += nY;
	}
	int		GetCenterX() { return ( Left + Right ) / 2; }
	int		GetCenterY() { return ( Top + Bottom ) / 2; }
	int		GetWidth() const { return Right - Left; }
	int		GetHeight() const { return Bottom - Top; }

	void operator = (const _UIRect& rcSrc)
	{
		CopyRect(rcSrc);
	}
} UIRect;


// UV Coordinate of UI texture
typedef struct _UIRectUV
{
	_UIRectUV() : U0(0.0f), V0(0.0f), U1(0.0f), V1(0.0f) {}
	_UIRectUV(FLOAT u0, FLOAT v0, FLOAT u1, FLOAT v1, FLOAT w, FLOAT h) { SetUV(u0, v0, u1, v1, w, h); }
	~_UIRectUV() {}

	FLOAT	U0, V0, U1, V1;

	void	SetUV( FLOAT fU0, FLOAT fV0, FLOAT fU1, FLOAT fV1 )
	{
		U0 = fU0; V0 = fV0; U1 = fU1; V1 = fV1;
	}
	void	SetUV( FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		U0 = fTx0 / fTexWidth;		V0 = fTy0 / fTexHeight;
		U1 = fTx1 / fTexWidth;		V1 = fTy1 / fTexHeight;
	}
	void	CopyUV( const _UIRectUV &rtSrc )
	{
		U0 = rtSrc.U0; V0 = rtSrc.V0; U1 = rtSrc.U1; V1 = rtSrc.V1;
	}

	void operator = (const _UIRectUV& rtSrc)
	{
		CopyUV(rtSrc);
	}
} UIRectUV;

// 3 UV Coordinate of UI texture
typedef struct _UIRectUV3
{
	UIRectUV	rtL;
	UIRectUV	rtM;
	UIRectUV	rtR;

	void SetUV3( FLOAT fLU0, FLOAT fLV0, FLOAT fLU1, FLOAT fLV1,
		FLOAT fMU0, FLOAT fMV0, FLOAT fMU1, FLOAT fMV1,
		FLOAT fRU0, FLOAT fRV0, FLOAT fRU1, FLOAT fRV1,
		FLOAT fTexWidth, FLOAT fTexHeight )
	{
		rtL.SetUV( fLU0, fLV0, fLU1, fLV1, fTexWidth, fTexHeight );
		rtM.SetUV( fMU0, fMV0, fMU1, fMV1, fTexWidth, fTexHeight );
		rtR.SetUV( fRU0, fRV0, fRU1, fRV1, fTexWidth, fTexHeight );
	}

	void CopyUV3( const _UIRectUV3 &rtSrc )
	{
		rtL.CopyUV( rtSrc.rtL );
		rtM.CopyUV( rtSrc.rtM );
		rtR.CopyUV( rtSrc.rtR );
	}

	void operator = (const _UIRectUV3& rtSrc)
	{
		CopyUV3(rtSrc);
	}
}UIRectUV3;

class CUIRectEx
{
public:
	UIRect	m_RT;
	UIRectUV m_UV;

public:
	CUIRectEx() {}
	CUIRectEx(const CUIRectEx& rhs)
	{
		(*this) = rhs;
	}

	CUIRectEx(UIRect rt, UIRectUV uv)
	{
		SetRectEx(rt, uv);
	}

	~CUIRectEx() {}

	void SetRectEx(UIRect rt, UIRectUV uv)
	{
		m_RT = rt;
		m_UV = uv;
	}

	void operator = (const CUIRectEx& rhs)
	{
		SetRectEx(rhs.m_RT, rhs.m_UV);
	}
};

class CUIRectSurface
{
public:
	CStaticStackArray<CUIRectEx> m_RectSurfaceArray;
	int PosX, PosY;
public:
	CUIRectSurface() 
	{
		Clear();	
	}

	~CUIRectSurface()
	{
		Clear();
	}

	void operator = (const CUIRectSurface& rhs)
	{
		m_RectSurfaceArray = rhs.m_RectSurfaceArray;
		PosX = rhs.PosX;
		PosY = rhs.PosY;
	}

	void Clear()
	{
		m_RectSurfaceArray.Clear();
		PosX = 0; PosY = 0;
	}

	void SetPos(int x, int y)
	{
		PosX = x; PosY = y;
	}

	BOOL IsEmpty()
	{
		return (m_RectSurfaceArray.Count() == 0);
	}

	void AddRectSurface(UIRect rt, UIRectUV uv)
	{
		m_RectSurfaceArray.Add(CUIRectEx(rt, uv));
	}

	void RenderRectSurface(CDrawPort* p, COLOR col)
	{
		ASSERT(m_RectSurfaceArray.Count() > 0 && "CUIRectSurface::RenderRectSurface()");

		INDEX Num, i;

		Num = m_RectSurfaceArray.Count();

		for (i=0; i<Num; ++i)
		{
			UIRect tmpRT = m_RectSurfaceArray[i].m_RT;
			UIRectUV tmpUV = m_RectSurfaceArray[i].m_UV;

			p->AddTexture(PosX + tmpRT.Left, PosY + tmpRT.Top, PosX + tmpRT.Right, PosY + tmpRT.Bottom,
						tmpUV.U0, tmpUV.V0, tmpUV.U1, tmpUV.V1, col);
		}
	}
	
	void RenderRectSurface(CDrawPort* p, COLOR col, INDEX n)
	{
		ASSERT(m_RectSurfaceArray.Count() > 0 && "CUIRectSurface::RenderRectSurface()");

		UIRect tmpRT = m_RectSurfaceArray[n].m_RT;
		UIRectUV tmpUV = m_RectSurfaceArray[n].m_UV;

		p->AddTexture(PosX + tmpRT.Left, PosY + tmpRT.Top, PosX + tmpRT.Right, PosY + tmpRT.Bottom,
					tmpUV.U0, tmpUV.V0, tmpUV.U1, tmpUV.V1, col);
	}
};

// UI window state flags
#define	UWS_ENABLE		0x00000001
#define	UWS_VISIBLE		0x00000002
#define	UWS_FOCUS		0x00000004
#define	UWS_FLOAT		0x00000008


// Message result of UI window
enum WMSG_RESULT
{
	WMSG_FAIL		= 0,		// Message is not processed
	WMSG_SUCCESS	= 1,		// Message is processed, and that's all
	WMSG_COMMAND	= 2,		// Message is processed, but needs another process
};


// Cursor movement direction
enum MoveDirection
{
	MD_LEFT	= 0,
	MD_RIGHT,
	MD_UP,
	MD_DOWN,
	MD_HOME,
	MD_END,
	MD_PGUP,
	MD_PGDN,
};


// Text align
enum TEXT_ALIGN
{
	TEXT_LEFT = 0,
	TEXT_CENTER,
	TEXT_RIGHT,
};


// Define position of button text
#define	BTN_TEXT_OFFSETY		5


// ----------------------------------------------------------------------------
// Name : CUIWindow
// Desc :
// ----------------------------------------------------------------------------
class CUIWindow
{
protected:
	CUIWindow		*m_pParentWnd;				// Pointer of parent window
	DWORD			m_dwWndState;				// State flags of window
	int				m_nPosX, m_nPosY;			// Position of window
	int				m_nWidth, m_nHeight;		// Size of window
	CTextureData	*m_ptdBaseTexture;			// Texture of window

protected:
	// Convert mouse point
	void	ConvertToWindow( int &nX, int &nY )
	{
		int	nAbsX, nAbsY;
		GetAbsPos( nAbsX, nAbsY );
		nX -= nAbsX;	nY -= nAbsY;
	}

public:
	CUIWindow() : m_pParentWnd( NULL ), m_ptdBaseTexture( NULL ), m_dwWndState( UWS_ENABLE | UWS_VISIBLE ) {}
	virtual ~CUIWindow() { Destroy(); }

	// Create & destroy window
	virtual void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
	{
		m_pParentWnd = pParentWnd;
		SetPos( nX, nY );
		SetSize( nWidth, nHeight );
	}
	virtual void	Destroy()
	{
		if( m_ptdBaseTexture )
		{
			_pTextureStock->Release( m_ptdBaseTexture );
			m_ptdBaseTexture = NULL;
		}
	}

	// Render window
	virtual void	Render() {}

	// Texture for rendering window
	CTextureData	*CreateTexture( CTFileName fnmTexture ) { return _pTextureStock->Obtain_t( fnmTexture ); }

	// Window state
	void	SetWndState( DWORD dwWndState ) { m_dwWndState = dwWndState; }
	virtual void	SetEnable( BOOL bEnable ) { bEnable ? m_dwWndState |= UWS_ENABLE : m_dwWndState &= ~UWS_ENABLE; }
	void	SetVisible( BOOL bVisible ) { bVisible ? m_dwWndState |= UWS_VISIBLE : m_dwWndState &= ~UWS_VISIBLE; }
	virtual void	SetFocus( BOOL bVisible ) { bVisible ? m_dwWndState |= UWS_FOCUS : m_dwWndState &= ~UWS_FOCUS; }
	void	SetFloat( BOOL bFloat ) { bFloat ? m_dwWndState |= UWS_FLOAT : m_dwWndState &= ~UWS_FLOAT; }
	DWORD	GetWndState() const { return m_dwWndState; }
	BOOL	IsEnabled() const { return m_dwWndState & UWS_ENABLE; }
	BOOL	IsVisible() const { return m_dwWndState & UWS_VISIBLE; }
	BOOL	IsFocused() const { return m_dwWndState & UWS_FOCUS; }
	BOOL	IsFloating() const { return m_dwWndState & UWS_FLOAT; }
	virtual BOOL	IsEditBoxFocused() { return FALSE; }
	virtual void	KillFocusEditBox() {}

	// Window position
	void	SetPos( int nX, int nY ) { m_nPosX = nX; m_nPosY = nY; }
	void	SetPosX( int nX ) { m_nPosX = nX; }
	void	SetPosY( int nY ) { m_nPosY = nY; }
	void	Move( int ndX, int ndY ) { m_nPosX += ndX; m_nPosY += ndY; }
	void	GetPos( int &nX, int &nY ) { nX = m_nPosX; nY = m_nPosY; }
	int		GetPosX() const { return m_nPosX; }
	int		GetPosY() const { return m_nPosY; }
	void	GetAbsPos( int &nX, int &nY )
	{
		if( m_pParentWnd ) { m_pParentWnd->GetAbsPos( nX, nY ); nX += m_nPosX; nY += m_nPosY; }
		else { nX = m_nPosX; nY = m_nPosY; }
	}
	int		GetAbsPosX() const
	{
		if( m_pParentWnd ) return m_pParentWnd->GetAbsPosX() + m_nPosX;
		else return m_nPosX;
	}
	int		GetAbsPosY() const
	{
		if( m_pParentWnd ) return m_pParentWnd->GetAbsPosY() + m_nPosY;
		else return m_nPosY;
	}

	// Adjust position
	virtual void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ ) {}
	virtual void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ ) {}

	// Window size
	void	SetSize( int nWidth, int nHeight ) { m_nWidth = nWidth; m_nHeight = nHeight; }
	void	SetWidth( int nWidth ) { m_nWidth = nWidth; }
	void	SetHeight( int nHeight ) { m_nHeight = nHeight; }
	int		GetWidth() const { return m_nWidth; }
	int		GetHeight() const { return m_nHeight; }

	// Region check
	// Window region
	BOOL	IsInside( int nX, int nY )
	{
		ConvertToWindow( nX, nY );

		if( nX >= 0 && nY >= 0 && nX < m_nWidth && nY < m_nHeight )
			return TRUE;
		return FALSE;
	}
	// Rectangle region
	BOOL	IsInsideRect( int nX, int nY, const UIRect& rcRect )
	{
		ConvertToWindow( nX, nY );

		if( nX >= rcRect.Left && nY >= rcRect.Top && nX < rcRect.Right && nY < rcRect.Bottom )
			return TRUE;
		return FALSE;
	}

	// Messages
	virtual WMSG_RESULT	KeyMessage( MSG *pMsg ) { return WMSG_FAIL; }
	virtual WMSG_RESULT CharMessage( MSG *pMsg ) { return WMSG_FAIL; }
	virtual WMSG_RESULT	IMEMessage( MSG *pMsg ) { return WMSG_FAIL; }
	virtual WMSG_RESULT	MouseMessage( MSG *pMsg ) { return WMSG_FAIL; }

	// Command functions
	virtual void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput ) {}
	virtual void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput ,CTString &strConfirm) {}
	virtual void	MsgBoxLCommand( int nCommandCode, int nResult ) {}
};


#endif	// UIWINDOW_H_
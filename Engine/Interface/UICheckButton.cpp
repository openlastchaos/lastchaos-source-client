#include "stdh.h"
#include <Engine/Interface/UICheckButton.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UITextureManager.h>


// ----------------------------------------------------------------------------
// Name : CUICheckButton()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUICheckButton::CUICheckButton()
{
	m_bChecked = FALSE;
	m_strText = CTString( "" );
	m_bValidText = FALSE;
	m_colText[0] = 0xAAAAAAFF;
	m_colText[1] = 0xF2F2F2FF;
}

// ----------------------------------------------------------------------------
// Name : ~CUICheckButton()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUICheckButton::~CUICheckButton()
{
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUICheckButton::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight,
								CTString &strText, BOOL bCheckRegionLeft, int nTextSX, int nCheckRegion )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	int	nLength = strText.Length();
	m_bValidText = nLength > 0;
	m_nTextSY = ( nHeight - _pUIFontTexMgr->GetFontHeight() ) / 2;

	if( m_bValidText )
		m_strText = strText;

	if( bCheckRegionLeft )
	{
		m_nTextSX = -nTextSX;
		m_rcCheckRegion.SetRect( -nCheckRegion, 0, nWidth, nHeight );
	}
	else
	{
		m_nTextSX = nTextSX;
		m_rcCheckRegion.SetRect( 0, 0, nWidth + nCheckRegion, nHeight );
	}
}

// ----------------------------------------------------------------------------
// Name : SetText()
// Desc :
// ----------------------------------------------------------------------------
void CUICheckButton::SetText( CTString &strText )
{
	int	nLength = strText.Length();
	m_bValidText = nLength > 0;

	if( m_bValidText )
		m_strText = strText;
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUICheckButton::Render()
{
	// Get position
	int	nX, nY;
	GetAbsPos( nX, nY );

	// Add render regions
	int	nUVIndex;
	if( m_bChecked )
		nUVIndex = UCBS_CHECK;
	else
		nUVIndex = UCBS_NONE;
	
	if(m_bValidText)
	{
		if( IsEnabled() )
		{
			_pUIMgr->GetDrawPort()->PutTextEx( m_strText, nX + m_nTextSX, nY + m_nTextSY, m_colText[1] );
		}
		else
		{
			_pUIMgr->GetDrawPort()->PutTextEx( m_strText, nX + m_nTextSX, nY + m_nTextSY, m_colText[0] );
			nUVIndex += 2;
		}
	}

	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + m_nWidth, nY + m_nHeight,
										m_rtUV[nUVIndex].U0, m_rtUV[nUVIndex].V0,
										m_rtUV[nUVIndex].U1, m_rtUV[nUVIndex].V1,
										0xFFFFFFFF );
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUICheckButton::MouseMessage( MSG *pMsg )
{
	// If check button is disabled
	if( !IsEnabled() )
		return WMSG_FAIL;

	// Mouse point
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	if( pMsg->message == WM_LBUTTONDOWN )
	{
		if( IsInsideRect( nX, nY, m_rcCheckRegion ) )
		{
			m_bChecked = !m_bChecked;
			return WMSG_SUCCESS;
		}
	}

	return WMSG_FAIL;
}

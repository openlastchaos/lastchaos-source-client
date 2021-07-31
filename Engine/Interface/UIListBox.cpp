#include "stdh.h"
#include <Engine/Interface/UIListBox.h>
#include <Engine/Interface/UITextureManager.h>



// ----------------------------------------------------------------------------
// Name : CUIListBox()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIListBox::CUIListBox()
{
	m_nColumnCount = 0;
	m_nSelectList = -1;
	m_nOverList = -1;
	m_bScrollBar = FALSE;
	m_colSelectList = 0xB8B8B8FF;
	m_colOverList = 0xB8B8B8FF;
}

// ----------------------------------------------------------------------------
// Name : ~CUIListBox()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIListBox::~CUIListBox()
{
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIListBox::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight,
							int nLineHeight, int nSpaceX, int nSpaceY, int nColumn, BOOL bSelectList, BOOL bAtLeastOneSelect )
{
	ASSERT( nColumn > 0 );

	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	m_nLineHeight = nLineHeight;
	m_bSelectList = bSelectList;
	m_bAtLeastOneSelect = bAtLeastOneSelect;

	m_nTextSX = nSpaceX;
	m_nTextSY = nSpaceY;

	int	nOffset = nLineHeight - _pUIFontTexMgr->GetFontHeight();
	if( nOffset < 0 )
		nOffset = 0;
	m_nSelectOffsetY = nOffset / 2;
	m_nLinePerPage = ( nHeight + nOffset - nSpaceY * 2 ) / nLineHeight;

	m_nColumnCount = nColumn;
	m_vecString.reserve( nColumn );
	m_vecColor.reserve( nColumn );
	m_vecColumnSX.reserve( nColumn );
	m_vecAlign.reserve( nColumn );

	for( int iCol = 0; iCol < nColumn; iCol++ )
	{
		sVecString	sVecTempString;
		sVecColor	sVecTempColor;

		m_vecString.push_back( sVecTempString );
		m_vecColor.push_back( sVecTempColor );
		m_vecColumnSX.push_back( 0 );
		m_vecAlign.push_back( TEXT_LEFT );
	}
}

// ----------------------------------------------------------------------------
// Name : CreateScroll()
// Desc :
// ----------------------------------------------------------------------------
void CUIListBox::CreateScroll( BOOL bRight, int nOffsetX, int nOffsetY, int nWidth, int nHeight,
								int nBtnWidth, int nBtnHeight, int nBarWidthGap, int nBarHeightGap,
								int nBarVertEdgeSize )
{
	m_bScrollBar = TRUE;

	if( bRight )
	{
		m_sbScrollBar.Create( this, m_nWidth + nOffsetX, nOffsetY, nWidth, nHeight );
		m_sbScrollBar.SetWheelRect( -( m_nWidth + nOffsetX ), -nOffsetY, m_nWidth, m_nHeight );
	}
	else
	{
		m_sbScrollBar.Create( this, nOffsetX - nWidth, nOffsetY, nWidth, nHeight );
		m_sbScrollBar.SetWheelRect( nWidth - nOffsetX, -nOffsetY, m_nWidth, m_nHeight );
	}

	m_sbScrollBar.CreateButtons( TRUE, nBtnWidth, nBtnHeight, nBarWidthGap, nBarHeightGap, nBarVertEdgeSize );
	m_sbScrollBar.SetScrollPos( 0 );
	m_sbScrollBar.SetItemsPerPage( m_nLinePerPage );
}

// ----------------------------------------------------------------------------
// Name : SetSelBar()
// Desc :
// ----------------------------------------------------------------------------
void CUIListBox::SetSelBar( int nWidth, int nHeight,
							FLOAT fU0, FLOAT fV0, FLOAT fU1, FLOAT fV1, FLOAT fTexWidth, FLOAT fTexHeight )
{
	m_bShowSelectBar = TRUE;
	m_rtSelectOver.SetUV( fU0, fV0, fU1, fV1, fTexWidth, fTexHeight );

	if( nWidth > m_nWidth || nWidth == -1 )
		nWidth = m_nWidth;
	int	nHalfWidth = nWidth / 2;
	int	nLeft = m_nWidth / 2 - nHalfWidth;

	if( nHeight < m_nLineHeight || nHeight == -1 )
		nHeight = m_nLineHeight;

	m_rcSelectOver.SetRect( nLeft, -m_nSelectOffsetY, nLeft + nWidth, nHeight - m_nSelectOffsetY );
}

// ----------------------------------------------------------------------------
// Name : ChangeSize()
// Desc :
// ----------------------------------------------------------------------------
void CUIListBox::ChangeSize( int ndLine )
{
	int	ndHeight = ndLine * m_nLineHeight;

	m_nHeight += ndHeight;
	m_nLinePerPage += ndLine;

	m_sbScrollBar.SetItemsPerPage( m_nLinePerPage );
	m_sbScrollBar.ChangeSize( ndHeight );
}


//------------------------------------------------------------------------------
// CUIListBox::ChangeLineCount
// Explain:  
// Date : 2005-05-19,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIListBox::ChangeLineCount( int nLine )
{
	int	ndHeight = nLine * m_nLineHeight;

	m_nHeight = ndHeight;
	m_nLinePerPage = nLine;

	m_sbScrollBar.SetItemsPerPage( m_nLinePerPage );
	m_sbScrollBar.ChangeSize( ndHeight );

}


// ----------------------------------------------------------------------------
// Name : AddString()
// Desc :
// ----------------------------------------------------------------------------
void CUIListBox::AddString( int nCol, CTString &strText, COLOR colText, BOOL bSelectable )
{
	ASSERT( nCol < m_nColumnCount );

	m_vecString[nCol].vecString.push_back( strText );
	m_vecColor[nCol].vecColor.push_back( colText );
	if(nCol == 0)
	{
		if(bSelectable && m_bAtLeastOneSelect && m_nSelectList == -1)
			m_nSelectList = m_vecSelectable.size();
		m_vecSelectable.push_back( bSelectable );
	}

	if( m_sbScrollBar.GetCurItemCount() < m_vecString[nCol].vecString.size() )
		m_sbScrollBar.SetCurItemCount( m_vecString[nCol].vecString.size() );
}

// ----------------------------------------------------------------------------
// Name : InsertString()
// Desc :
// ----------------------------------------------------------------------------
void CUIListBox::InsertString( int nIndex, int nCol, CTString &strText, COLOR colText, BOOL bSelectable )
{
	ASSERT( nCol < m_nColumnCount );
	ASSERT( nIndex < m_vecString[nCol].vecString.size() );

	std::vector<CTString>::iterator	itText = m_vecString[nCol].vecString.begin();
	itText += nIndex;
	//for( int i = 0; i < nIndex; i++, itText++ );
	m_vecString[nCol].vecString.insert( itText, strText );

	std::vector<COLOR>::iterator	itCol = m_vecColor[nCol].vecColor.begin();
	itCol += nIndex;
	//for( i = 0; i < nIndex; i++, itCol++ );
	m_vecColor[nCol].vecColor.insert( itCol, colText );

	std::vector<BOOL>::iterator	itBool = m_vecSelectable.begin();
	itBool += nIndex;
	//for( i = 0; i < nIndex; i++, itBool++ );
	m_vecSelectable.insert( itBool, bSelectable );

	if( m_sbScrollBar.GetCurItemCount() < m_vecString[nCol].vecString.size() )
		m_sbScrollBar.SetCurItemCount( m_vecString[nCol].vecString.size() );
}

// ----------------------------------------------------------------------------
// Name : RemoveString()
// Desc :
// ----------------------------------------------------------------------------
void CUIListBox::RemoveString( int nIndex, int nCol )
{
	ASSERT( nCol < m_nColumnCount );
	ASSERT( nIndex < m_vecString[nCol].vecString.size() );

	int i;

	std::vector<CTString>::iterator	itText = m_vecString[nCol].vecString.begin();
	itText += nIndex;
	//for( i = 0; i < nIndex; i++, itText++ );
	m_vecString[nCol].vecString.erase( itText );

	std::vector<COLOR>::iterator	itCol = m_vecColor[nCol].vecColor.begin();
	itCol += nIndex;
	//for( i = 0; i < nIndex; i++, itCol++ );
	m_vecColor[nCol].vecColor.erase( itCol );

	if(nCol == 0)
	{
		std::vector<BOOL>::iterator	itBool = m_vecSelectable.begin();
		itBool += nIndex;
		//for( i = 0; i < nIndex; i++, itBool++ );
		m_vecSelectable.erase( itBool );
	}

	int	nMaxCount = 0;
	for( i = 0; i < nCol; i++ )
	{
		if( nMaxCount < m_vecString[nCol].vecString.size() )
			nMaxCount = m_vecString[nCol].vecString.size();
	}
	m_sbScrollBar.SetCurItemCount( nMaxCount );
}

// ----------------------------------------------------------------------------
// Name : MoveString()
// Desc :
// ----------------------------------------------------------------------------
void CUIListBox::MoveString( int nFromIndex, int nToIndex, int nCol )
{
	ASSERT( nCol < m_nColumnCount );
	ASSERT( nFromIndex < m_vecString[nCol].vecString.size() );
	ASSERT( nToIndex < m_vecString[nCol].vecString.size() );

	if( nFromIndex == nToIndex )
		return;

	CTString	strTemp = m_vecString[nCol].vecString[nFromIndex];
	COLOR		colTemp = m_vecColor[nCol].vecColor[nFromIndex];
	BOOL		bTemp = m_vecSelectable[nFromIndex];

	std::vector<CTString>::iterator	itText = m_vecString[nCol].vecString.begin();
	itText += nFromIndex;
	//for( int i = 0; i < nFromIndex; i++, itText++ );
	m_vecString[nCol].vecString.erase( itText );

	std::vector<COLOR>::iterator	itCol = m_vecColor[nCol].vecColor.begin();
	itCol += nFromIndex;
	//for( i = 0; i < nFromIndex; i++, itCol++ );
	m_vecColor[nCol].vecColor.erase( itCol );
	
	std::vector<BOOL>::iterator	itBool = m_vecSelectable.begin();
	itBool += nFromIndex;
	//for( i = 0; i < nFromIndex; i++, itBool++ );
	m_vecSelectable.erase( itBool );


	itText = m_vecString[nCol].vecString.begin();
	itText += nToIndex;
	//for( i = 0; i < nToIndex; i++, itText++ );
	m_vecString[nCol].vecString.insert( itText, strTemp );

	itCol = m_vecColor[nCol].vecColor.begin();
	itCol += nToIndex;
	//for( i = 0; i < nToIndex; i++, itCol++ );
	m_vecColor[nCol].vecColor.insert( itCol, colTemp );

	itBool = m_vecSelectable.begin();
	itBool += nToIndex;
	//for( i = 0; i < nToIndex; i++, itBool++ );
	m_vecSelectable.insert( itBool, bTemp );
}

// ----------------------------------------------------------------------------
// Name : ResetAllStrings()
// Desc :
// ----------------------------------------------------------------------------
void CUIListBox::ResetAllStrings()
{
	m_nSelectList = -1;
	m_nOverList = -1;

	for( int iCol = 0; iCol < m_vecString.size(); iCol++ )
	{
		m_vecString[iCol].vecString.clear();
		m_vecColor[iCol].vecColor.clear();
	}
	m_vecSelectable.clear();

	m_sbScrollBar.SetScrollPos( 0 );
	m_sbScrollBar.SetCurItemCount( 0 );
}

// ----------------------------------------------------------------------------
// Name : SetString()
// Desc :
// ----------------------------------------------------------------------------
void CUIListBox::SetString( int nCol, int nIndex, CTString &strText )
{
	ASSERT( nCol < m_nColumnCount );
	ASSERT( nIndex < m_vecString[nCol].vecString.size() );

	m_vecString[nCol].vecString[nIndex] = strText;
}

// ----------------------------------------------------------------------------
// Name : SetColor()
// Desc :
// ----------------------------------------------------------------------------
void CUIListBox::SetColor( int nCol, int nIndex, COLOR colText )
{
	ASSERT( nCol < m_nColumnCount );
	ASSERT( nIndex < m_vecColor[nCol].vecColor.size() );

	m_vecColor[nCol].vecColor[nIndex] = colText;
}

// ----------------------------------------------------------------------------
// Name : SetColor()
// Desc :
// ----------------------------------------------------------------------------
void CUIListBox::SetSelectable( int nIndex, BOOL bSelectable )
{
	ASSERT( nIndex < m_vecSelectable.size() );

	m_vecSelectable[nIndex] = bSelectable;
}

// ----------------------------------------------------------------------------
// Name : SetColor()
// Desc :
// ----------------------------------------------------------------------------
void CUIListBox::SetAllNotSelectable()
{
	for(int i=0; i<m_vecSelectable.size(); ++i)
	{
		m_vecSelectable[i] = FALSE;
	}
}

// ----------------------------------------------------------------------------
// Name : SetColumnPosX()
// Desc :
// ----------------------------------------------------------------------------
void CUIListBox::SetColumnPosX( int nCol, int nPosX, TEXT_ALIGN eAlign )
{
	ASSERT( nCol < m_nColumnCount );

	m_vecColumnSX[nCol] = nPosX;
	m_vecAlign[nCol] = eAlign;
}


// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIListBox::Render()
{
	// Get position
	int	nX, nY;
	GetAbsPos( nX, nY );

	// Add render regions
	// Selection bar
	if( m_bShowSelectBar && m_nSelectList >= 0 )
	{
		int	nSelLine = m_nSelectList - m_sbScrollBar.GetScrollPos();
		
		if( nSelLine >= 0 )
		{
			if( !m_bScrollBar || nSelLine < m_sbScrollBar.GetScrollRange() )
			{
			
				int	nBarY = nY + m_nTextSY + nSelLine * m_nLineHeight;
				_pUIMgr->GetDrawPort()->AddTexture( nX + m_rcSelectOver.Left, nBarY + m_rcSelectOver.Top,
													nX + m_rcSelectOver.Right, nBarY + m_rcSelectOver.Bottom,
													m_rtSelectOver.U0, m_rtSelectOver.V0,
													m_rtSelectOver.U1, m_rtSelectOver.V1,
													0xFFFFFFFF );
			}
		}
	}

	// Scroll bar
	if( m_bScrollBar )
		m_sbScrollBar.Render();

	// Text in list box
	for( int nCol = 0; nCol < m_vecString.size(); nCol++ )
	{
		int	nRowS = m_sbScrollBar.GetScrollPos();
		int	nRowE = nRowS + m_nLinePerPage;
		if( nRowE > m_vecString[nCol].vecString.size() )
			nRowE = m_vecString[nCol].vecString.size();

		COLOR	colText;
		int		nTextX = nX + m_nTextSX + m_vecColumnSX[nCol];
		int		nTextY = nY + m_nTextSY;
		int		nSelList = -1, nOverList = -1;
		if( m_bSelectList )
		{
			nSelList = m_nSelectList;
			nOverList = nRowS + m_nOverList;
		}

		for( int nList = nRowS; nList < nRowE; nList++ )
		{
			if( m_vecSelectable[nList] )
			{
				if( nList == nSelList )
					colText = m_colSelectList;
				else if( nList == nOverList )
					colText = m_colOverList;
				else
					colText = m_vecColor[nCol].vecColor[nList];
			}
			else
				colText = m_vecColor[nCol].vecColor[nList];

			switch( m_vecAlign[nCol] )
			{
			case TEXT_LEFT:
				_pUIMgr->GetDrawPort()->PutTextEx( m_vecString[nCol].vecString[nList], nTextX, nTextY, colText );
				break;
			case TEXT_CENTER:
				_pUIMgr->GetDrawPort()->PutTextExCX( m_vecString[nCol].vecString[nList], nTextX, nTextY, colText );
				break;
			case TEXT_RIGHT:
				_pUIMgr->GetDrawPort()->PutTextExRX( m_vecString[nCol].vecString[nList], nTextX, nTextY, colText );
				break;
			}

			nTextY += m_nLineHeight;
		}
	}
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIListBox::MouseMessage( MSG *pMsg )
{
	// If list box is disabled
	if( !IsEnabled() )
		return WMSG_FAIL;

	// Mouse point
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( m_bSelectList )
			{
				m_nOverList = -1;

				if( IsInside( nX, nY ) )
				{
					int	nAbsY = GetAbsPosY();
					m_nOverList = ( nY - nAbsY - m_nTextSY + m_nSelectOffsetY ) / m_nLineHeight;

					return WMSG_SUCCESS;
				}
			}

			if( m_bScrollBar )
			{
				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( m_bSelectList )
			{
				if( IsInside( nX, nY ) )
				{
					int	nAbsY = GetAbsPosY();
					m_nOverList = ( nY - nAbsY - m_nTextSY + m_nSelectOffsetY ) / m_nLineHeight;

					int	nRealSel = m_nOverList + m_sbScrollBar.GetScrollPos();
					if( nRealSel >= 0 && nRealSel < m_sbScrollBar.GetCurItemCount()
						&& m_vecSelectable[nRealSel] )
					{
						m_nSelectList = nRealSel;
						return WMSG_COMMAND;
					}
					else if(!m_bAtLeastOneSelect)
					{
						m_nSelectList = -1;
					}

					return WMSG_SUCCESS;
				}
			}

			if( m_bScrollBar )
			{
				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			if( m_bScrollBar )
			{
				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( m_bSelectList )
			{
				if( IsInside( nX, nY ) )
				{
					int	nAbsY = GetAbsPosY();
					m_nOverList = ( nY - nAbsY - m_nTextSY + m_nSelectOffsetY ) / m_nLineHeight;

					int	nRealSel = m_nOverList + m_sbScrollBar.GetScrollPos();
					if( nRealSel >= 0 && nRealSel < m_sbScrollBar.GetCurItemCount()
						&& m_vecSelectable[nRealSel] )
					{
						m_nSelectList = nRealSel;
						return WMSG_COMMAND;
					}

					return WMSG_SUCCESS;
				}
			}

			if( m_bScrollBar )
			{
				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( m_bScrollBar )
			{
				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;
	}

	return WMSG_FAIL;
}
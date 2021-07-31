#include "stdh.h"
#include <Engine/Interface/UIMultList.h>
#include <Engine/Interface/UITextureManager.h>



//------------------------------------------------------------------------------
// CUIMultiList::CUIMultiList
// Explain:  
// Date : 2005-05-17,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CUIMultiList::CUIMultiList()
{
		
}


//------------------------------------------------------------------------------
// CUIMultiList::~CUIMultiList
// Explain:  
// Date : 2005-05-17,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CUIMultiList::~CUIMultiList()
{

}

//------------------------------------------------------------------------------
// CUIMultiList::Create
// Explain:  MultiList생성시 추가 작업 필요로 
// Date : 2005-05-18,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiList::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight,
							int nLineHeight, int nSpaceX, int nSpaceY, int nColumn, BOOL bSelectList )
{
	CUIListBox::Create( pParentWnd, nX, nY, nWidth, nHeight, nLineHeight, nSpaceX, nSpaceY, nColumn, bSelectList );

	// Todo :~ 추가된 데이터 타입에 따른 추가 작업...
	m_vecIcon.reserve( nColumn );
	m_vecConType.reserve( nColumn );
	m_vecAlpha.reserve( nColumn );

	for( int iCol = 0; iCol < nColumn; iCol++ )
	{
		sVecIcon		sVecTempIcon;
		sVecAlpha		sVecTempAlpha;
				
		m_vecIcon.push_back( sVecTempIcon );			
		m_vecConType.push_back( TYPE_TEXT );
		m_vecAlpha.push_back( sVecTempAlpha );
	}
}


//------------------------------------------------------------------------------
// CUIMultiList::SetColumnType
// Explain:  
// Date : 2005-05-18,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiList::SetColumnType( int nColumn, CONTENT_TYPE eContentType )
{
	m_vecConType[nColumn] = eContentType;
}

//------------------------------------------------------------------------------
// CUIMultiList::Render
// Explain:  기본 루틴 그대로 가져옴.. 일부 수정
// Date : 2005-05-18,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiList::Render()
{
	if( !IsVisible() ) return;

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
			//!m_bScrollBar ||	
			if( nSelLine < m_nLinePerPage ) // Date : 2005-05-23(오후 5:26:26), By Lee Ki-hwan
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

	// Text부분만 아니라 Icon부분도 렌더링
	// Text in list box
	for( int nCol = 0; nCol < m_vecString.size(); nCol++ )
	{
		if( m_vecConType[nCol] == TYPE_TEXT )
		{
			RenderString( nCol, nX, nY );
		}
		else if( m_vecConType[nCol] == TYPE_ICON )
		{
			RenderIcon( nCol, nX, nY );
		}
	
	}
}


//------------------------------------------------------------------------------
// CUIMultiList::RenderIcon
// Explain:  
// Date : 2005-05-18,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiList::RenderIcon( int nColumn, int nX, int nY )
{
	int	nRowS = m_sbScrollBar.GetScrollPos();
	int	nRowE = nRowS + m_nLinePerPage;
	if( nRowE > m_vecIcon[nColumn].vecIcon.size() )
		nRowE = m_vecIcon[nColumn].vecIcon.size();

	// m_nTextSX, SY는 위와 좌의 여백임 ... ( TEXT에서만 국한 된는 것이 아님 ~ )
	int		nIconX = nX + m_nTextSX + m_vecColumnSX[nColumn];
	int		nIconY = nY + m_nTextSY;
	
	/* 셀렉션은 사용하지 않음 
	int		nSelList = -1, nOverList = -1;
	if( m_bSelectList )
	{
		nSelList = m_nSelectList;
		nOverList = nRowS + m_nOverList;
	}
	*/

	for( int nList = nRowS; nList < nRowE; nList++ )
	{
		switch( m_vecAlign[nColumn] )
		{
		case TEXT_LEFT:
		
		case TEXT_CENTER:
		
		case TEXT_RIGHT:
			_pUIMgr->GetDrawPort()->AddTexture( nIconX, nIconY, nIconX + ICON_WIDTH, nIconY + ICON_HEIGHT,
											m_vecIcon[nColumn].vecIcon[nList].U0, m_vecIcon[nColumn].vecIcon[nList].V0,
											m_vecIcon[nColumn].vecIcon[nList].U1, m_vecIcon[nColumn].vecIcon[nList].V1,
											m_vecAlpha[nColumn].vecAhpla[nList] );
	
			break;
		}

		nIconY += m_nLineHeight;
	}

}


//------------------------------------------------------------------------------
// CUIMultiList::RenderString
// Explain:  
// Date : 2005-05-18,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiList::RenderString( int nColumn, int nX, int nY )
{
	int	nRowS = m_sbScrollBar.GetScrollPos();
	int	nRowE = nRowS + m_nLinePerPage;
	if( nRowE > m_vecString[nColumn].vecString.size() )
		nRowE = m_vecString[nColumn].vecString.size();

	COLOR	colText;
	int		nTextX = nX + m_nTextSX + m_vecColumnSX[nColumn];
	int		nTextY = nY + m_nTextSY;
	int		nSelList = -1, nOverList = -1;
	if( m_bSelectList )
	{
		nSelList = m_nSelectList;
		nOverList = nRowS + m_nOverList;
	}

	for( int nList = nRowS; nList < nRowE; nList++ )
	{
		if( nList == nSelList )
			colText = m_colSelectList;
		else if( nList == nOverList )
			colText = m_colOverList;
		else
			colText = m_vecColor[nColumn].vecColor[nList];

		switch( m_vecAlign[nColumn] )
		{
		case TEXT_LEFT:
			_pUIMgr->GetDrawPort()->PutTextEx( m_vecString[nColumn].vecString[nList], nTextX, nTextY, colText );
			break;
		case TEXT_CENTER:
			_pUIMgr->GetDrawPort()->PutTextExCX( m_vecString[nColumn].vecString[nList], nTextX, nTextY, colText );
			break;
		case TEXT_RIGHT:
			_pUIMgr->GetDrawPort()->PutTextExRX( m_vecString[nColumn].vecString[nList], nTextX, nTextY, colText );
			break;
		}

		nTextY += m_nLineHeight;
	}

}


//------------------------------------------------------------------------------
// CUIMultiList::AddIcon
// Explain:  
// Date : 2005-05-18,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiList::AddIcon( int nCol, UIRectUV &rtIcon, ALPHA alpIcon, BOOL bSelectable )
{
	ASSERT( m_vecConType[nCol] == TYPE_ICON );
	ASSERT( nCol < m_nColumnCount );
	
	m_vecIcon[nCol].vecIcon.push_back( rtIcon );
	m_vecAlpha[nCol].vecAhpla.push_back( alpIcon );

	// Date : 2005-06-14(오후 3:44:55), By Lee Ki-hwan
	if( nCol == 0 )
	{
		if( bSelectable && m_bAtLeastOneSelect && m_nSelectList == -1 )
			m_nSelectList = m_vecSelectable.size();
		
		m_vecSelectable.push_back( bSelectable );
	}

	// Scroll resizing
	if( m_sbScrollBar.GetCurItemCount() < m_vecIcon[nCol].vecIcon.size() )
		m_sbScrollBar.SetCurItemCount( m_vecIcon[nCol].vecIcon.size() );
}


//------------------------------------------------------------------------------
// CUIMultiList::InsertIcon
// Explain:  
// Date : 2005-05-18,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiList::InsertIcon( int nIndex, int nCol, UIRectUV &rtIcon, ALPHA alpIcon, BOOL bSelectable )
{
	ASSERT( m_vecConType[nCol] == TYPE_ICON );
	ASSERT( nCol < m_nColumnCount );
	ASSERT( nIndex < m_vecIcon[nCol].vecIcon.size() );

	std::vector<UIRectUV>::iterator	itICon = m_vecIcon[nCol].vecIcon.begin();
	for( int i = 0; i < nIndex; i++, itICon++ );
		m_vecIcon[nCol].vecIcon.insert( itICon, rtIcon );

	std::vector<ALPHA>::iterator	itAlp = m_vecAlpha[nCol].vecAhpla.begin();
	for( i = 0; i < nIndex; i++, itAlp++ );
		m_vecColor[nCol].vecColor.insert( itAlp, alpIcon );
	if(nCol == 0)
	{
		// Date : 2005-06-14(오후 3:44:52), By Lee Ki-hwan
		std::vector<BOOL>::iterator	itBool = m_vecSelectable.begin();
		itBool += nIndex;
		m_vecSelectable.insert( itBool, bSelectable );
	}

	// Scroll resizing
	if( m_sbScrollBar.GetCurItemCount() < m_vecIcon[nCol].vecIcon.size() )
		m_sbScrollBar.SetCurItemCount( m_vecIcon[nCol].vecIcon.size() );

}


//------------------------------------------------------------------------------
// CUIMultiList::RemoveIcon
// Explain:  
// Date : 2005-05-18,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiList::RemoveIcon( int nIndex, int nCol )
{
	ASSERT( m_vecConType[nCol] == TYPE_ICON );
	ASSERT( nCol < m_nColumnCount );
	ASSERT( nIndex < m_vecIcon[nCol].vecIcon.size() );

	std::vector<UIRectUV>::iterator	itIcon = m_vecIcon[nCol].vecIcon.begin();
	for( int i = 0; i < nIndex; i++, itIcon++ );
		m_vecIcon[nCol].vecIcon.erase( itIcon );

	std::vector<COLOR>::iterator	itAlp = m_vecAlpha[nCol].vecAhpla.begin();
	for( i = 0; i < nIndex; i++, itAlp++ );
		m_vecAlpha[nCol].vecAhpla.erase( itAlp );
	
	if( nCol == 0 ) 
	{
		std::vector<BOOL>::iterator	itBool = m_vecSelectable.begin();
		itBool += nIndex;
		m_vecSelectable.erase( itBool );
	}

	// Scroll resizing
	int	nMaxCount = 0;
	for( i = 0; i < nCol; i++ )
	{
		if( nMaxCount < m_vecIcon[nCol].vecIcon.size() )
			nMaxCount = m_vecIcon[nCol].vecIcon.size();
	}
	m_sbScrollBar.SetCurItemCount( nMaxCount );

}


//------------------------------------------------------------------------------
// CUIMultiList::MoveIcon
// Explain:  
// Date : 2005-05-18,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiList::MoveIcon( int nFromIndex, int nToIndex, int nCol )
{
	ASSERT( m_vecConType[nCol] == TYPE_ICON );
	ASSERT( nCol < m_nColumnCount );
	ASSERT( nFromIndex < m_vecIcon[nCol].vecIcon.size() );
	ASSERT( nToIndex < m_vecIcon[nCol].vecIcon.size() );

	if( nFromIndex == nToIndex )
		return;

	UIRectUV	strTemp = m_vecIcon[nCol].vecIcon[nFromIndex];
	ALPHA		alpTemp = m_vecAlpha[nCol].vecAhpla[nFromIndex];
	BOOL		bTemp = m_vecSelectable[nFromIndex];

	// erase
	std::vector<UIRectUV>::iterator	itIcon = m_vecIcon[nCol].vecIcon.begin();
	for( int i = 0; i < nFromIndex; i++, itIcon++ );
		m_vecIcon[nCol].vecIcon.erase( itIcon );

	std::vector<COLOR>::iterator	itAlp = m_vecAlpha[nCol].vecAhpla.begin();
	for( i = 0; i < nFromIndex; i++, itAlp++ );
		m_vecAlpha[nCol].vecAhpla.erase( itAlp );

	std::vector<BOOL>::iterator	itBool = m_vecSelectable.begin();
	itBool += nFromIndex;
	//for( i = 0; i < nFromIndex; i++, itBool++ );
	m_vecSelectable.erase( itBool );


	// insert
	itIcon = m_vecIcon[nCol].vecIcon.begin();
	for( i = 0; i < nToIndex; i++, itIcon++ );
		m_vecIcon[nCol].vecIcon.insert( itIcon, strTemp );

	itAlp = m_vecAlpha[nCol].vecAhpla.begin();
	for( i = 0; i < nToIndex; i++, itAlp++ );
		m_vecAlpha[nCol].vecAhpla.insert( itAlp, alpTemp );
	
	// Scroll resizing

}


//------------------------------------------------------------------------------
// CUIMultiList::SetIcon
// Explain:  
// Date : 2005-05-18,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiList::SetIcon( int nCol, int nIndex, UIRectUV &rtIcon )
{
	ASSERT( m_vecConType[nCol] == TYPE_ICON );
	ASSERT( nCol < m_nColumnCount );
	ASSERT( nIndex < m_vecIcon[nCol].vecIcon.size() );

	m_vecIcon[nCol].vecIcon[nIndex] = rtIcon;
}


//------------------------------------------------------------------------------
// CUIMultiList::SetAlpha
// Explain:  
// Date : 2005-05-18,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiList::SetAlpha( int nCol, int nIndex, ALPHA alpIcon )
{
	ASSERT( m_vecConType[nCol] == TYPE_ICON );
	ASSERT( nCol < m_nColumnCount );
	ASSERT( nIndex < m_vecAlpha[nCol].vecAhpla.size() );

	m_vecAlpha[nCol].vecAhpla[nIndex] = alpIcon;
}


//------------------------------------------------------------------------------
// CUIMultiList::ResetAllData
// Explain:  
// Date : 2005-05-18,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiList::ResetAllData()
{
	CUIListBox::ResetAllStrings();
	
	for( int nCol = 0; nCol < m_vecIcon.size(); nCol++ )
	{
		m_vecIcon[nCol].vecIcon.clear();
		m_vecAlpha[nCol].vecAhpla.clear();
	}

}
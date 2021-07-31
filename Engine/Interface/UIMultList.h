
#ifndef	UIMULTILIST_H_
#define	UIMULTILIST_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Interface/UIListBox.h>

#define ICON_WIDTH	18
#define ICON_HEIGHT 15

enum CONTENT_TYPE 
{
	TYPE_TEXT,	// 문자
	TYPE_ICON,	// 이미지 
};

typedef COLOR		ALPHA;
//------------------------------------------------------------------------------
// CUIMultiList
// Explain: 메신저에서 사용하는 사용자 리스트 위해서 작성된 
//			이미지와 TEXT를 담을 수 있는 MultiListBox 
// Date : 2005-05-18,Author: Lee Ki-hwan
// History: 
//------------------------------------------------------------------------------
class CUIMultiList : public CUIListBox
{

protected:
	typedef struct _sVecIcon
	{
		std::vector<UIRectUV>	vecIcon;

	} sVecIcon;

	typedef struct _sVecAlpha
	{
		std::vector<ALPHA>		vecAhpla;
	} sVecAlpha;


protected:
	std::vector<CONTENT_TYPE>	m_vecConType;

	std::vector<sVecIcon>		m_vecIcon;
	std::vector<sVecAlpha>		m_vecAlpha;

public:

	CUIMultiList();
	virtual ~CUIMultiList();


	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight, int nLineHeight,
					int nSpaceX, int nSpaceY, int nColumn, BOOL bSelectList );

	void	Render();
	void	RenderIcon( int nColumn, int nX, int nY );
	void	RenderString( int nColumn, int nX, int nY );

	void	SetColumnType( int nColumn, CONTENT_TYPE eContentType );

	void	ResetAllData();

	
	// Get count of current item
	int		GetCurItemCount( int nCol ) const
	{
		ASSERT( nCol < m_nColumnCount );
		return m_vecString[nCol].vecString.size();
	} 

	// Set Icon
	void	AddIcon( int nCol, UIRectUV &rtIcon, ALPHA alpIcon = 0xFFFFFFFF, BOOL bSelectable = TRUE );
	void	InsertIcon( int nIndex, int nCol, UIRectUV &rtIcon, ALPHA alpIcon = 0xFFFFFFFF, BOOL bSelectable = TRUE );
	void	RemoveIcon( int nIndex, int nCol );
	void	MoveIcon( int nFromIndex, int nToIndex, int nCol );
	void	SetIcon( int nCol, int nIndex, UIRectUV &rtIcon );
	void	SetAlpha( int nCol, int nIndex, ALPHA alpIcon = 0xFFFFFFFF );

	UIRectUV	&GetIcon( int nCol, int nIndex )
	{
		ASSERT( nCol < m_nColumnCount );
		ASSERT( nIndex < m_vecIcon[nCol].vecIcon.size() );
		return m_vecIcon[nCol].vecIcon[nIndex];
	}


};


#endif	// UISCROLLBAR_H_


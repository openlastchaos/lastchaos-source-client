#include "stdh.h"
#include <Engine/Interface/UIMultiEditBox.h>
#include <Engine/Interface/UITextureManager.h>
#include <Engine/Network/Web.h>

// External variables
extern HWND	_hwndMain;

#define WEB_NEXT_LINE	"\r\n"		// Use Web ( New Line Charactor )

//------------------------------------------------------------------------------
// CUIMultiEditBox::CUIMultiEditBox
// Explain: Constructor 
// Date : 2005-01-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CUIMultiEditBox::CUIMultiEditBox()
{
	// m_ebEditBox;				// Do Nothing 
	// m_sbScrollBar;			// Do Nothing 
	m_strTexts.clear();			
	
	m_nCurrentLine		= 0;		
	m_nFontWidth		= 0;		
	m_nFontHeight		= 0;		
	m_nMaxChar			= 0;		
	m_nLineHeight		= 0;
	m_nBlankSpaceTop	= 0;
	m_nBlankSpaceLeft	= 0;
}


//------------------------------------------------------------------------------
// CUIMultiEditBox::~CUIMultiEditBox
// Explain: Destructor
// Date : 2005-01-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CUIMultiEditBox::~CUIMultiEditBox()
{
	Destroy();
}


//------------------------------------------------------------------------------
// CUIMultiEditBox::Destroy
// Explain: Destroy
// Date : 2005-01-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiEditBox::Destroy() 
{
	// Do Nothing
}


//------------------------------------------------------------------------------
// CUIMultiEditBox::Create
// Explain:  
// Date : 2005-01-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiEditBox::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight, int nLineHeight )
{
	// Basic Data Set
	m_pParentWnd = pParentWnd;

	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	m_nFontHeight	= _pUIFontTexMgr->GetFontHeight() + _pUIFontTexMgr->GetLineSpacing();
	m_nFontWidth	= _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing();
	m_nMaxChar		= ( nWidth / m_nFontWidth ) - 2;
	m_nLineHeight	= ( nHeight / m_nFontHeight ) -1;// - 2;
	
	// Texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MessageBox.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// Sctroll Bar
	m_sbScrollBar.Create( this, nWidth - 6, 0, 9, nHeight - 3 );
	m_sbScrollBar.CreateButtons( TRUE, 9, 7, 0, 0, 10 );
	m_sbScrollBar.SetScrollPos( 0 );
	m_sbScrollBar.SetScrollRange( nLineHeight );
	m_sbScrollBar.SetCurItemCount( 0 );
	m_sbScrollBar.SetItemsPerPage( nLineHeight );
	m_sbScrollBar.SetWheelRect ( -nWidth, -1, nWidth, nHeight );
	// UP Button
	m_sbScrollBar.SetUpUV( UBS_IDLE, 230, 16, 239, 23, fTexWidth, fTexHeight );
	m_sbScrollBar.SetUpUV( UBS_CLICK, 240, 16, 249, 23, fTexWidth, fTexHeight );
	m_sbScrollBar.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbScrollBar.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Down Button
	m_sbScrollBar.SetDownUV( UBS_IDLE, 230, 24, 239, 31, fTexWidth, fTexHeight );
	m_sbScrollBar.SetDownUV( UBS_CLICK, 240, 24, 249, 31, fTexWidth, fTexHeight );
	m_sbScrollBar.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbScrollBar.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar Button
	m_sbScrollBar.SetBarTopUV( 219, 16, 228, 26, fTexWidth, fTexHeight );
	m_sbScrollBar.SetBarMiddleUV( 219, 27, 228, 29, fTexWidth, fTexHeight );
	m_sbScrollBar.SetBarBottomUV( 219, 30, 228, 40, fTexWidth, fTexHeight );

	// Edit Box
	m_ebEditBox.Create( this, m_nFontWidth / 2, 0, nWidth, 15, m_nMaxChar );
	m_ebEditBox.SetReadingWindowUV( 146, 46, 163, 62, fTexWidth, fTexHeight );
	m_ebEditBox.SetCandidateUV( 146, 46, 163, 62, fTexWidth, fTexHeight );

	// Blank Space
	m_nBlankSpaceTop	= ( m_ebEditBox.GetHeight() - _pUIFontTexMgr->GetFontHeight() ) / 2;	// 위쪽에 약간의 여백을 
	m_nBlankSpaceLeft	= m_nFontWidth / 2;														// 옆에 여백을

	//!! Change AddString ( "" , 0 );
	for( int i =0; i < nLineHeight; i++ )
	{
		m_strTexts.push_back( CTString ( "" ) );
	}

}


//------------------------------------------------------------------------------
// CUIMultiEditBox::Render
// Explain:  
// Date : 2005-01-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiEditBox::Render()
{
	int nX, nY;
	int nScrollBarPos	= m_sbScrollBar.GetScrollPos();	// 스크롤 바 위치 
	int nEndLine		= m_strTexts.size();

	GetAbsPos( nX, nY );
	
	// Blank Space 
	nX += m_nBlankSpaceLeft;
	nY += m_nBlankSpaceTop;
	
	// 라인수가 
	// 한번에 표현 할 수 있는 최대 라인수를 넘었을 때 
	// 최대 라인수로 제한
	if( m_strTexts.size() > m_nLineHeight )
	{
		nEndLine = m_nLineHeight+1;
	}

	// 문자열 출력
	for( int i = nScrollBarPos; i < nScrollBarPos + nEndLine; i++ )
	{
		_pUIMgr->GetDrawPort()->PutTextEx( m_strTexts[i], nX, nY, 0xFFFFFFFF );
		nY += m_nFontHeight;
	}

	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();

	// Render EditBox
	// EditBox가 화면 안에 표현 될 때 ( m_nCurrentLine의 위치로 판단 )
	if( nScrollBarPos <= m_nCurrentLine )
	{
		int nMaxLine = nScrollBarPos + nEndLine;
		if( nMaxLine >= m_nCurrentLine )
		{
			m_ebEditBox.Render ();
		}
	}
	
	// Set texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	// Render ScrollBar
	m_sbScrollBar.Render();
	
	// Reading window
	if( m_ebEditBox.DoesShowReadingWindow() )
	{
		// Reading window
		m_ebEditBox.RenderReadingWindow();
	}
	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

}


//------------------------------------------------------------------------------
// CUIMultiEditBox::SetFocus
// Explain:  
// Date : 2005-01-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiEditBox::SetFocus( BOOL bVisible )
{
	CUIWindow::SetFocus( bVisible );
	m_ebEditBox.SetFocus( bVisible );
}


//------------------------------------------------------------------------------
// CUIMultiEditBox::IsFocused
// Explain:  
// Date : 2005-01-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
BOOL CUIMultiEditBox::IsFocused()
{
	return m_ebEditBox.IsFocused();
}


//------------------------------------------------------------------------------
// CUIMultiEditBox::KeyMessage
// Explain:  
// Date : 2005-01-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
WMSG_RESULT	CUIMultiEditBox::KeyMessage( MSG *pMsg )
{
	switch( pMsg->wParam )
	{
	case VK_UP:
		MoveCursorUp();
		return WMSG_SUCCESS;
	
	case VK_DOWN:
		MoveCursorDown();
		return WMSG_SUCCESS;
	
	case VK_RETURN: // 글자의 처음이나 중간에서 입력할 때와 글자의 마지막에서 입력할 때 
		{
			if( m_strTexts[m_nCurrentLine].Length() <= m_ebEditBox.GetCursorIndex() )
			{
				m_nCurrentLine++;
				AddString( "", m_nCurrentLine );
				SetStringToEditBox();
				ResizeScrollBar();
				SetScrollBarPos();
			}
			else // 글자 중간 이나 처음 
			{
				// 라인을 2개로 분리 한다.
				SplitLine( m_ebEditBox.GetCursorIndex() );
				MoveCursorFirst();				
			}
		}
		return WMSG_SUCCESS;

	case VK_BACK:
		if( m_ebEditBox.GetCursorIndex () == 0 )
		{
			if ( m_nCurrentLine == 0 ) return WMSG_SUCCESS;

			m_nCurrentLine--;

			m_ebEditBox.SetString ( m_strTexts[m_nCurrentLine].str_String );
			m_ebEditBox.SetCursorIndex ( m_strTexts[m_nCurrentLine].Length() );
			
			//WMSG_RESULT wmsgResult = m_ebEditBox.KeyMessage ( pMsg );
			//m_strTexts[m_nCurrentLine] = m_ebEditBox.GetString ();

			Cutting ();
			ResizeScrollBar ();
			SetScrollBarPos ();
			return WMSG_SUCCESS;

		}
		else
		{
			WMSG_RESULT wmsgResult = m_ebEditBox.KeyMessage ( pMsg );
			m_strTexts[m_nCurrentLine] = m_ebEditBox.GetString ();
			SetScrollBarPos ();
			return wmsgResult;
		}
		return WMSG_FAIL;

	case VK_DELETE :
		/*if ( m_strTexts[m_nCurrentLine].Length() <= 0 ) // 현재 라인의 내용이 하나도 없으면
		{
			if ( m_nCurrentLine != 0 )
			{
				//라인은 삭제
				m_strTexts.erase ( m_strTexts.begin() + m_nCurrentLine );
				
				m_nCurrentLine--;
								
				if ( m_strTexts[m_nCurrentLine].Length() > 0 ) 
					m_ebEditBox.SetString ( m_strTexts[m_nCurrentLine].str_String );
				else
					m_ebEditBox.ResetString();

				m_ebEditBox.SetCursorIndex( 0 );
				
				ReSizeScrollBar ();
				SetScrollBarPos ();
			}
			else if ( m_nCurrentLine == 0 )
			{
				if ( m_strTexts.size () > 1 )
				{
					if ( m_strTexts[1].Length() > 0 ) 
						m_ebEditBox.SetString ( m_strTexts[1].str_String );
					else
						m_ebEditBox.ResetString();

					m_ebEditBox.SetCursorIndex( 0 );
					
					m_strTexts.erase ( m_strTexts.begin() );
										
					ReSizeScrollBar ();
					SetScrollBarPos ();

				}
				
			}
			return WMSG_SUCCESS;
		}
		else 
		*/if ( m_ebEditBox.GetCursorIndex() >= m_strTexts[m_nCurrentLine].Length() ) // 문장의 끝
		{
			if ( m_nCurrentLine >= m_strTexts.size() - 1 ) 	return WMSG_SUCCESS;
			
			if ( m_strTexts[m_nCurrentLine].Length() >= m_nMaxChar ) // 라인의 끝이라면
			{	
				// 다음 라인의 처음으로 옮기고 
				m_nCurrentLine++;
				
				if ( m_strTexts[m_nCurrentLine].Length() > 0 ) 
					m_ebEditBox.SetString ( m_strTexts[m_nCurrentLine].str_String );
				else
					m_ebEditBox.ResetString();
				
				m_ebEditBox.SetCursorIndex ( 0 );

			//	WMSG_RESULT wmsgResult = m_ebEditBox.KeyMessage ( pMsg );
			//	m_strTexts[m_nCurrentLine] = m_ebEditBox.GetString ();
				//m_ebEditBox.SetPos ( m_ebEditBox.GetPosX(), m_nCurrentLine * m_nFontHeight );
				ResizeScrollBar ();
				SetScrollBarPos ();
				return WMSG_SUCCESS;
			}
			
			Cutting ();	// 밑에서 끌어 올려서 자른다.
			ResizeScrollBar ();
			SetScrollBarPos ();
			return WMSG_SUCCESS;

		}
		else // 글자의 중간이라면
		{
			WMSG_RESULT wmsgResult = m_ebEditBox.KeyMessage ( pMsg );
			m_strTexts[m_nCurrentLine] = m_ebEditBox.GetString ();
			
			return wmsgResult;
		}
		return WMSG_FAIL;

	case VK_LEFT :
		{
			if ( m_ebEditBox.GetCursorIndex() <= 0 )
			{
				if ( m_nCurrentLine <= 0 )
				{
					m_nCurrentLine = 0;
					WMSG_RESULT wmsgResult = m_ebEditBox.KeyMessage ( pMsg );
					m_strTexts[m_nCurrentLine] = m_ebEditBox.GetString ();
					SetScrollBarPos ();
					return wmsgResult;
				}
				else 
					m_nCurrentLine--;

				if ( !m_strTexts[m_nCurrentLine].Length() )
				{
					m_ebEditBox.ResetString ();
				}
				else
				{
					m_ebEditBox.SetString ( m_strTexts[m_nCurrentLine].str_String );
					m_ebEditBox.SetCursorIndex ( m_strTexts[m_nCurrentLine].Length() );
				}

				if ( m_nCurrentLine < m_sbScrollBar.GetScrollPos() )
				{
					m_sbScrollBar.SetScrollPos ( m_nCurrentLine );
				}
				m_ebEditBox.SetPos ( m_ebEditBox.GetPosX(), ( m_nCurrentLine - m_sbScrollBar.GetScrollPos() ) * m_nFontHeight );
			
				return WMSG_SUCCESS;
			}

			WMSG_RESULT wmsgResult = m_ebEditBox.KeyMessage ( pMsg );
			m_strTexts[m_nCurrentLine] = m_ebEditBox.GetString ();

			if ( m_nCurrentLine < m_sbScrollBar.GetScrollPos() )
			{
				m_sbScrollBar.SetScrollPos ( m_nCurrentLine );
			}
			m_ebEditBox.SetPos ( m_ebEditBox.GetPosX(), ( m_nCurrentLine - m_sbScrollBar.GetScrollPos() ) * m_nFontHeight );
			return wmsgResult;
		}
	case VK_RIGHT : 
		{
			if ( m_ebEditBox.GetCursorIndex() >= m_strTexts[m_nCurrentLine].Length() ) // 라인의 끝
			{
				
				if ( m_nCurrentLine >= m_strTexts.size() - 1 )
				{
					m_nCurrentLine = m_strTexts.size() - 1;
					SetScrollBarPos ();
					return WMSG_SUCCESS;
				}
				else
				{
					m_nCurrentLine++;	
				}

				if ( !m_strTexts[m_nCurrentLine].Length() )
				{
					m_ebEditBox.ResetString ();
				}
				else
				{
					m_ebEditBox.SetString ( m_strTexts[m_nCurrentLine].str_String );
					m_ebEditBox.SetCursorIndex ( 0 );
					
				}

				//m_ebEditBox.SetPos ( m_ebEditBox.GetPosX(), m_nCurrentLine * m_nFontHeight );
				SetScrollBarPos ();
				return WMSG_SUCCESS;
			}

			WMSG_RESULT wmsgResult = m_ebEditBox.KeyMessage ( pMsg );
			m_strTexts[m_nCurrentLine] = m_ebEditBox.GetString ();
			SetScrollBarPos ();
			return wmsgResult;
		}
	default :
		{
			WMSG_RESULT wmsgResult = m_ebEditBox.KeyMessage ( pMsg );
			m_strTexts[m_nCurrentLine] = m_ebEditBox.GetString ();
			SetScrollBarPos ();
			return wmsgResult;
		}
		
	}

	return WMSG_FAIL;	
}


// ----------------------------------------------------------------------------
// Name : CharMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIMultiEditBox::CharMessage( MSG *pMsg )	
{ 

	if ( m_ebEditBox.GetCursorIndex() >= m_nMaxChar )
	{
		m_nCurrentLine++;
		m_strTexts.insert ( m_strTexts.begin() + m_nCurrentLine, "" );
		
		m_ebEditBox.ResetString ();
		SetScrollBarPos ();
	}
	else 
	{
		PasteNextLine( m_nCurrentLine );	
		m_ebEditBox.SetString ( m_strTexts[m_nCurrentLine].str_String );
	}

	WMSG_RESULT wmsgResult = m_ebEditBox.CharMessage ( pMsg );
	m_strTexts[m_nCurrentLine] = m_ebEditBox.GetString ();
	
	ResizeScrollBar();

	return wmsgResult;
		
}


// ----------------------------------------------------------------------------
// Name : IMEMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT	CUIMultiEditBox::IMEMessage( MSG *pMsg )	
{ 
	if ( m_ebEditBox.GetCursorIndex() >= m_nMaxChar -1 ) // 마지막 위치 
	{
		m_nCurrentLine++;
		m_strTexts.insert ( m_strTexts.begin() + m_nCurrentLine, "" );
		//m_ebEditBox.SetPos ( m_ebEditBox.GetPosX(), m_nCurrentLine * m_nFontHeight );
		m_ebEditBox.ResetString ();
		SetScrollBarPos ();
	}
	else if ( m_ebEditBox.GetCursorIndex() < m_nMaxChar - 3 )
	{
		PasteNextLineKor ( m_nCurrentLine );
		m_ebEditBox.SetString ( m_strTexts[m_nCurrentLine].str_String );

	}

	WMSG_RESULT wmsgResult = m_ebEditBox.IMEMessage ( pMsg );
	m_strTexts[m_nCurrentLine] = m_ebEditBox.GetString ();

	//m_ebEditBox.SetPos ( m_ebEditBox.GetPosX(), ( m_nCurrentLine - m_sbScrollBar.GetScrollPos() )* m_nFontHeight );
	SetScrollBarPos ();
	
	ResizeScrollBar();
	
	return wmsgResult;
}


// ----------------------------------------------------------------------------
// Name : !!MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT	CUIMultiEditBox::MouseMessage( MSG *pMsg )	
{ 
//	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				_pUIMgr->SetMouseCursorInsideUIs();
			
			if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				m_ebEditBox.SetPos ( m_ebEditBox.GetPosX(), ( m_nCurrentLine - m_sbScrollBar.GetScrollPos() )* m_nFontHeight );
				return WMSG_SUCCESS;
			}
			else if (m_ebEditBox.MouseMessage ( pMsg ) != WMSG_FAIL ) 
				return WMSG_SUCCESS;
			
		}
		break;

	case WM_LBUTTONDOWN:
		{	
			if( IsInside( nX, nY ) )
			{
				if( m_strTexts.size() > 0 )
				{
					ConvertToWindow ( nX, nY );
					m_ebEditBox.SetFocus ( TRUE );
					SetFocus( TRUE );
			
					m_nCurrentLine = nY / m_nFontHeight   + m_sbScrollBar.GetScrollPos();					
					 
					if ( m_nCurrentLine >= m_strTexts.size() - 1 )
					{	
						m_nCurrentLine  = m_strTexts.size() - 1;
					}
					
					//Add: Su-won
					if( m_nCurrentLine >=m_sbScrollBar.GetScrollPos() +m_sbScrollBar.GetItemsPerPage() )
					{
						m_ebEditBox.SetFocus ( FALSE);
						SetFocus( FALSE );
						break;
					}
					
					if ( m_strTexts[m_nCurrentLine].Length() > 0 ) 
						m_ebEditBox.SetString ( m_strTexts[m_nCurrentLine].str_String );
					else
						m_ebEditBox.ResetString();
					
					m_ebEditBox.SetPos ( m_ebEditBox.GetPosX(), ( m_nCurrentLine - m_sbScrollBar.GetScrollPos() )* m_nFontHeight );							

					int pos = nX / m_nFontWidth;

					if ( pos >= m_strTexts[m_nCurrentLine].Length() )
						m_ebEditBox.SetCursorIndex ( m_strTexts[m_nCurrentLine].Length() );
					
				}


				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					m_ebEditBox.SetPos ( m_ebEditBox.GetPosX(), ( m_nCurrentLine - m_sbScrollBar.GetScrollPos() )* m_nFontHeight );
					return WMSG_SUCCESS;
				}
				else if (m_ebEditBox.MouseMessage ( pMsg ) != WMSG_FAIL ) 
					return WMSG_SUCCESS;
			}
			m_ebEditBox.SetFocus ( FALSE );
			SetFocus( FALSE );
		}
		break;
	case WM_LBUTTONUP:
		{
			if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					m_ebEditBox.SetPos ( m_ebEditBox.GetPosX(), ( m_nCurrentLine - m_sbScrollBar.GetScrollPos() )* m_nFontHeight );
					return WMSG_SUCCESS;
				}
			}
		}
		break;
	}	
	return WMSG_FAIL;	
}


// ----------------------------------------------------------------------------
// Name : IsDBCS
// Desc : 상위 바이트가 2Byte 문자의 첫 글자 인지 확인
// ----------------------------------------------------------------------------
BOOL CUIMultiEditBox::IsDBCS ( char* strBuf, int nPos )
{
    BOOL bRet = FALSE;
    for(int i = 0; i <= nPos; i++)
	{
        if(isascii(strBuf[i])==0)
        {
            if(nPos == i+1)
                bRet = TRUE;

            i++;
        }
    }
    return bRet;
}


//------------------------------------------------------------------------------
// CUIMultiEditBox::SplitLine
// Explain: 라인을 두개로 분리한다.
// Date : 2005-01-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiEditBox::SplitLine( int nIndex )
{
	CTString strTemp = m_strTexts[m_nCurrentLine];
	CTString strTemp2;
	
	int		nSplitPos;
	BOOL	b2ByteChar = FALSE;	

	if( nIndex == -1 ) // MaxCharactor에서 자른다.
	{
		nSplitPos = m_nMaxChar;
	}
	else
	{
		nSplitPos = nIndex;
	}

	// Check splitting position for 2 byte characters
	for( int iPos = 0; iPos < nSplitPos; iPos++ )
	{
		if( strTemp[iPos] & 0x80 )	
			b2ByteChar = !b2ByteChar;
		else
			b2ByteChar = FALSE;
	}

	if( b2ByteChar )
		nSplitPos--;

	strTemp.Split( nSplitPos, strTemp, strTemp2 );
	
	//SetString ( strTemp.str_String, m_nCurrentLine );			--->
	m_strTexts[m_nCurrentLine] =strTemp.str_String;
	//m_nCurrentLine++;
	if( nIndex ==-1)
		AddString ( strTemp2.str_String, m_nCurrentLine+1);
	else
		AddString ( strTemp2.str_String, ++m_nCurrentLine);
	
	SetStringToEditBox();
		
	ResizeScrollBar();
	SetScrollBarPos();
	
}


// ----------------------------------------------------------------------------
// Name : Cutting
// Desc : 라인을 끌어 올려서 max 아래에서 자른다.
// ----------------------------------------------------------------------------
void  CUIMultiEditBox::Cutting ()
{
	int temp = m_ebEditBox.GetCursorIndex();

	if ( m_strTexts.size() > 1 + m_nCurrentLine )
	{
		// 한 라인으로 합친다.
		m_strTexts[m_nCurrentLine] += m_strTexts[m_nCurrentLine+1];
	
		// 아랫라인은 삭제
		m_strTexts.erase ( m_strTexts.begin() + m_nCurrentLine + 1 );
	}
	
	if ( m_strTexts[m_nCurrentLine].Length() > m_nMaxChar )
	{
		SplitLine ();	
		m_ebEditBox.SetString ( m_strTexts[m_nCurrentLine].str_String );
		m_ebEditBox.SetCursorIndex ( temp );
	}
	else
	{
		m_ebEditBox.SetString ( m_strTexts[m_nCurrentLine].str_String );
		m_ebEditBox.SetCursorIndex ( temp );
	}
	
	SetScrollBarPos ();
//	m_ebEditBox.SetPos ( m_ebEditBox.GetPosX(), ( m_nCurrentLine - m_sbScrollBar.GetScrollPos() )* m_nFontHeight );
	//m_ebEditBox.SetPos ( m_ebEditBox.GetPosX(), m_nCurrentLine * m_nFontHeight );
}


// ----------------------------------------------------------------------------
// Name : PasteNextLine
// Desc : 현재 라인의 길이를 넘는 문자를 다음 라인의 앞에 붙인다.
// ----------------------------------------------------------------------------	
void CUIMultiEditBox::PasteNextLine ( int nCurrentLine, BOOL bHangle )	
{
	CTString strString = m_strTexts[nCurrentLine].str_String;
	int		nLength		= strString.Length(); //현재 문자의 길이

	int		nMaxChar = m_nMaxChar;

	if ( nCurrentLine != m_nCurrentLine ) // 처음 라인이 아니라면 
	{
		if( nLength < ++nMaxChar ) return;
	}
	else 
	{
		if( nLength < nMaxChar ) return;
	}
	
	// Need multi-line
		
	// Check splitting position for 2 byte characters
	int	nSplitPos = nMaxChar-1;
 	
	if ( bHangle )
		nSplitPos--;
	

	BOOL	b2ByteChar	= FALSE;	

	for( int iPos = 0; iPos < nSplitPos; iPos++ )
	{
		if( strString[iPos] & 0x80 )
			b2ByteChar = !b2ByteChar;
		else
			b2ByteChar = FALSE;
	}

	if( b2ByteChar )
		nSplitPos--;

	// Split string
	CTString	strTemp;
	
	strString.Split( nSplitPos,strString, strTemp );
	m_strTexts[nCurrentLine] = strString;

	/*****
	if ( m_strTexts.size() <= 1 + nCurrentLine ) return;

	m_strTexts[nCurrentLine+1] = strTemp + m_strTexts[nCurrentLine+1];

	if ( nCurrentLine >= m_strTexts.size() - 1 ) // 마지막 라인이라면
	{
		m_strTexts.insert ( m_strTexts.begin() + nCurrentLine, "" );
		SetScrollBarPos ();
	}
	*****/
	if ( nCurrentLine >= m_strTexts.size() - 1 ) // 마지막 라인이라면
	{
		m_strTexts.insert ( m_strTexts.begin() + nCurrentLine+1, "" );
		m_strTexts[nCurrentLine+1] = strTemp;
		SetScrollBarPos ();
		return;
	}
	else
		m_strTexts[nCurrentLine+1] = strTemp + m_strTexts[nCurrentLine+1];

	PasteNextLine ( nCurrentLine + 1 );
}


// ----------------------------------------------------------------------------
// Name : PasteNextLineKor
// Desc : 현재 라인의 길이를 넘는 문자를 다음 라인의 앞에 붙인다.
// ----------------------------------------------------------------------------
void CUIMultiEditBox::PasteNextLineKor ( int nCurrentLine )	
{
	CTString strString	= m_strTexts[nCurrentLine].str_String;
	int		nLength		= strString.Length(); //현재 문자의 길이
	int		nMaxChar	= m_nMaxChar;

	if ( nCurrentLine != m_nCurrentLine ) // 처음 라인이 아니라면 
	{
		if( nLength < ++nMaxChar -1 ) return;
	}
	else 
	{
		if( nLength < nMaxChar - 2 ) return;
	}
	
	// Check splitting position for 2 byte characters
	int	nSplitPos = nMaxChar-2;
 	
	BOOL	b2ByteChar	= FALSE;	

	for( int iPos = 0; iPos < nSplitPos; iPos++ )
	{
		if( strString[iPos] & 0x80 )
			b2ByteChar = !b2ByteChar;
		else
			b2ByteChar = FALSE;
	}

	if( b2ByteChar )
		nSplitPos--;

	// Split string
	CTString	strTemp;
	
	strString.Split( nSplitPos,strString, strTemp );
	m_strTexts[nCurrentLine] = strString;
	
	/*****
	if ( m_strTexts.size() <= 1 + nCurrentLine ) return;
	
	m_strTexts[nCurrentLine+1] = strTemp + m_strTexts[nCurrentLine+1];

	if ( nCurrentLine >= m_strTexts.size() - 1 ) // 마지막 라인이라면
	{
		m_strTexts.insert ( m_strTexts.begin() + nCurrentLine, "" );
		SetScrollBarPos ();
		
	}
	*****/
	if ( nCurrentLine >= m_strTexts.size() - 1 ) // 마지막 라인이라면
	{
		m_strTexts.insert ( m_strTexts.begin() + nCurrentLine+1, "" );
		m_strTexts[nCurrentLine+1] = strTemp;
		SetScrollBarPos ();
		return;
	}
	else
		m_strTexts[nCurrentLine+1] = strTemp + m_strTexts[nCurrentLine+1];


	PasteNextLine ( nCurrentLine + 1 );
}


// ----------------------------------------------------------------------------
// Name : !!GetString()
// Desc :
// ----------------------------------------------------------------------------
CTString CUIMultiEditBox::GetString ()
{
	CTString String;

	for ( int i = 0; i < m_strTexts.size(); i++ )
	{
		String += m_strTexts[i] + WEB_NEXT_LINE;
	}

	return String;

}


// ----------------------------------------------------------------------------
// Name : !!ResetString()
// Desc :
// ----------------------------------------------------------------------------
void CUIMultiEditBox::ResetString ()
{
	m_nCurrentLine = 0;
	m_ebEditBox.ResetString();
	m_sbScrollBar.SetScrollPos ( 0 );
	m_sbScrollBar.SetCurItemCount ( 0 );
	m_strTexts.clear ();
	m_ebEditBox.SetPos ( m_ebEditBox.GetPosX(), 0 );
	m_strTexts.push_back ( CTString ( "" ) );
}


//------------------------------------------------------------------------------
// ResizeScrollBar
// Explain:  
// Date : 2005-01-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiEditBox::ResizeScrollBar()
{
	int nLineCount =  m_strTexts.size ();
	
	if( nLineCount > m_nLineHeight )
	{
		int nOverCount = nLineCount - m_nLineHeight - 1; 
		
		if( m_sbScrollBar.GetScrollPos() > nOverCount )
		{
			m_sbScrollBar.SetScrollPos( nOverCount );
		}
		
		nOverCount++; // Index와 Count는 1차이
	}

	//m_sbScrollBar.SetCurItemCount( 0 );
	m_sbScrollBar.SetCurItemCount( nLineCount );
}


//------------------------------------------------------------------------------
// CUIMultiEditBox::SetScrollBarPos
// Explain:  
// Date : 2005-01-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiEditBox::SetScrollBarPos()
{
	int nScrollPos = m_sbScrollBar.GetScrollPos();
	
	// 현재 연결된 라인이 스크롤바 위치 보다 작으면 
	if ( m_nCurrentLine < nScrollPos  ) // 연결된 라인 까지 스크롤 바 이동 
	{
		m_sbScrollBar.SetScrollPos ( m_nCurrentLine );
	}
	else if ( m_nCurrentLine >= m_nLineHeight + m_sbScrollBar.GetScrollPos() ) // 스크롤 바의 위치를 벗어 날 때. (아래로 이동 )
	{
		int nDist = m_nCurrentLine - m_nLineHeight;
		m_sbScrollBar.SetScrollPos ( nDist );
	}
	else 
	{
		// 위, 아래 공간에서는 스크롤바의 위치를 변경할 필요가 없음
	}
	
	m_ebEditBox.SetPos ( m_ebEditBox.GetPosX(), ( m_nCurrentLine - m_sbScrollBar.GetScrollPos() )* m_nFontHeight );
}


// ----------------------------------------------------------------------------
// Name : !!ResetString()
// Desc :
// ----------------------------------------------------------------------------
void CUIMultiEditBox::SetString ( char* strString, char* strHead )
{
	if ( strString == NULL ) return;

	int		len		= strlen ( strString ) - 1;
	int		nFirst	= 0;
	char	string[256];

	ResetString ();
	
	for ( int i = 0; i < len; i++ )
	{
		if ( strncmp ( WEB_NEXT_LINE, strString + i, 2 ) == 0 )
		{
			memset ( string, 0, sizeof ( char )  * 256 ) ;
			
			if ( strHead )	strcpy ( string, strHead );

			strncat ( string, strString + nFirst, i - nFirst ); 
			
			if ( strlen ( string ) > m_nMaxChar ) 
			{
				CTString tempString = string;
				CTString temp;
				int nMaxChar = m_nMaxChar;
				
				if ( tempString.str_String[nMaxChar] & 0x80 )
					nMaxChar--;
									
			
				tempString.Split ( nMaxChar, tempString, temp );
				m_strTexts.push_back ( tempString );
				
				if ( strHead )
				{
					CTString strLeft;
					strLeft += strHead;
					strLeft += temp;
				
					m_strTexts.push_back ( strLeft );
				}

			}
			else
			{
				if (nFirst > 0)
					m_strTexts.push_back ( string );
				else
					m_strTexts[nFirst] = string;
			}

			nFirst = i+2;
			i+=2;
		}
	}
	
	ResizeScrollBar ();
	SetScrollBarPos ();
}


// ----------------------------------------------------------------------------
// Name : !!ResetString()
// Desc :
// ----------------------------------------------------------------------------
void CUIMultiEditBox::SetString ( char* strString, char* Writer, char* strHead )
{
	if ( strString == NULL ) return;

	int		len		= strlen ( strString ) - 1;
	int		nFirst	= 0;
	char	string[256];

	ResetString ();
	m_strTexts.clear ();

	m_strTexts.push_back ( CTString ( "" ) );

	CTString strWriter = "----";
	strWriter += Writer;
	strWriter +=_S( 1939,  "님이 쓰신 글입니다 -----" );	
		
	m_strTexts.push_back ( strWriter );
	
	for ( int i = 0; i < len; i++ )
	{
		if ( strncmp ( WEB_NEXT_LINE, strString + i, 2 ) == 0 )
		{
			memset ( string, 0, sizeof ( char )  * 256 ) ;
			
			if ( strHead )	strcpy ( string, strHead );

			strncat ( string, strString + nFirst, i - nFirst ); 

			if ( strlen ( string ) > m_nMaxChar ) 
			{
				CTString tempString = string;
				CTString temp;
				int nMaxChar = m_nMaxChar;
				
				if ( tempString.str_String[nMaxChar] & 0x80 )
					nMaxChar--;
									
				tempString.Split ( nMaxChar, tempString, temp );
				m_strTexts.push_back ( tempString );
				
				if ( strHead )
				{
					CTString strLeft;
					strLeft += strHead;
					strLeft += temp;
				
					m_strTexts.push_back ( strLeft );
				}

			}
			else
				m_strTexts.push_back ( string );

			nFirst = i+2;
			i+=2;
		}
	}
	
	if ( m_strTexts[m_nCurrentLine].Length() > 0 ) 
		m_ebEditBox.SetString ( m_strTexts[m_nCurrentLine].str_String );
	else
		m_ebEditBox.ResetString();

	ResizeScrollBar ();
	SetScrollBarPos ();

}


//------------------------------------------------------------------------------
// CUIMultiEditBox::MoveCursorUp
// Explain:  
// Date : 2005-01-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiEditBox::MoveCursorUp()
{
	int nCursorIndex = m_ebEditBox.GetCursorIndex();

	if( 0 >= m_nCurrentLine )
	{
		m_nCurrentLine = 0;
		return;
	}
	
	m_nCurrentLine--;
	SetStringToEditBox ();
	SetScrollBarPos ();		// 스크롤 바의 위치 재조정
}


//------------------------------------------------------------------------------
// CUIMultiEditBox::MoveCursorDown
// Explain:  
// Date : 2005-01-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiEditBox::MoveCursorDown()
{
	if ( m_nCurrentLine >= m_strTexts.size() - 1 )
	{
		m_nCurrentLine = m_strTexts.size() - 1;
		return;
	}

	m_nCurrentLine++;
	SetStringToEditBox ();
	SetScrollBarPos (); // 스크롤 바의 위치 재조정
}


//------------------------------------------------------------------------------
// CUIMultiEditBox::MoveCursorFirst
// Explain:  
// Date : 2005-01-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiEditBox::MoveCursorFirst()
{
	m_ebEditBox.SetCursorIndex( 0 );	
}


//------------------------------------------------------------------------------
// CUIMultiEditBox::MoveCursorEnd
// Explain:  
// Date : 2005-01-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiEditBox::MoveCursorEnd()
{
	m_ebEditBox.SetCursorIndex( m_strTexts[m_nCurrentLine].Length() );
}


//------------------------------------------------------------------------------
// CUIMultiEditBox::SetStringToEditBox
// Explain:  
// Date : 2005-01-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiEditBox::SetStringToEditBox()
{
	if( m_nCurrentLine >= m_strTexts.size() )
	{
		m_nCurrentLine = m_strTexts.size() - 1;
		return;
	}

	int nCursorIndex = m_ebEditBox.GetCursorIndex();

	// 현재 문자열이 NULL이면 EditBox에 복사 할 때 에러나는 걸 막기위해 
	if( 0 == m_strTexts[m_nCurrentLine].Length() ) 
	{
		m_ebEditBox.ResetString ();
	}
	else
	{
		m_ebEditBox.SetString ( m_strTexts[m_nCurrentLine].str_String );
		
		// 현재 커서의 위치가 문자열의 길이 보다 크면 커서를 문자열의 마지막으로 옮김
		if ( m_strTexts[m_nCurrentLine].Length() <= m_ebEditBox.GetCursorIndex() )
		{
			MoveCursorEnd ();
		}
		else 
		{
			// 한글 2Byte문자 사이에 커서가 위치 했을 때
			// 한글 문자열 일 때 첫 바이트를 확인해서 Double byte이면 커서의 위치 조정 해줌
			if( IsDBCS( m_strTexts[m_nCurrentLine].str_String, nCursorIndex ) )
			{
				++nCursorIndex;
				m_ebEditBox.SetCursorIndex ( nCursorIndex );
			}
		}
	}
}


//------------------------------------------------------------------------------
// CUIMultiEditBox::SetString
// Explain: 
// Date : 2005-01-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiEditBox::SetString( char* strString, int nPos )
{
	

}


//------------------------------------------------------------------------------
// CUIMultiEditBox::AddString
// Explain:  
// Date : 2005-01-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiEditBox::AddString( char* strString, int nPos )
{
	if ( -1 == nPos ) // Max Line에 추가 
	{
		nPos = m_strTexts.size() - 1;
	}
	else if ( nPos > m_strTexts.size() )
	{
		return;
	}
	
	//m_strTexts.insert ( m_strTexts.begin() + nPos, CTString ( m_ebEditBox.GetString () ) );
	//m_strTexts[m_nCurrentLine] = strString;
	m_strTexts.insert ( m_strTexts.begin() + nPos, CTString ( strString) );
	m_ebEditBox.SetString( strString );
		
	SetScrollBarPos();
}


//------------------------------------------------------------------------------
// CUIMultiEditBox::DeleteString
// Explain:  
// Date : 2005-01-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiEditBox::DeleteString( int nPos )
{


}



//------------------------------------------------------------------------------
// CUIMultiEditBox::GetAllStringLength
// Explain: 현재 박스에 있는 모든 문자열의 길이 리턴
// Date : 2006-06-23,Author: Su-won
//------------------------------------------------------------------------------
int CUIMultiEditBox::GetAllStringLength()
{
	int nLength=0;
	for(int i=0; i<m_strTexts.size(); ++i)
		nLength += m_strTexts[i].Length() +strlen(WEB_NEXT_LINE);
	
	return nLength;
}


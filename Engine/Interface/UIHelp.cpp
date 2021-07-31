#include "stdh.h"
#include <Engine/Interface/UIHelp.h>
#include <Engine/Interface/UIInternalClasses.h>

extern INDEX g_iCountry;
extern INDEX  cli_iCurrentFPS;

CStaticArray<HELP1>* m_pHelp1;

int tListInc;

void InitList( CStaticArray<int>& ilistIndex, int iValue)
{
	for(int i=0; i<ilistIndex.Count() ; i++)
	{
		ilistIndex[i]= iValue;
	};
}
// ----------------------------------------------------------------------------
// Name : CUIHelp()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIHelp::CUIHelp()
{
	m_bSlideImgX		= 0;
	m_iImageIdx			= -1; 
	m_bVisible			= FALSE;
	m_bVisibleImg		= FALSE;
	m_ptdImageTexture	= NULL;
}

// ----------------------------------------------------------------------------
// Name : ~CUIHelp()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIHelp::~CUIHelp()
{
	Destroy();
}

// ------------------------------- ---------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelp::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 600, 22 );	
	m_rcRightView.SetRect( 187, 39, 587, 435 );

	// Create web board texture
	
	m_ptdImageTexture	=  _pTextureStock->Obtain_t ( CTString( "Data\\Interface\\Help.tex" ) );
	m_ptdBaseTexture	= CreateTexture( CTString( "Data\\Interface\\WebBoard.tex" ) );
	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	m_rtTitleUL.SetUV( 0, 0, 49, 28, fTexWidth, fTexHeight );
	m_rtTitleUM.SetUV( 50, 0, 56, 28, fTexWidth, fTexHeight );
	m_rtTitleUR.SetUV( 57, 0, 89, 28, fTexWidth, fTexHeight );

	m_rtBackUL.SetUV( 0, 42, 49, 52, fTexWidth, fTexHeight );
	m_rtBackUM.SetUV( 50, 42, 56, 52, fTexWidth, fTexHeight );
	m_rtBackUR.SetUV( 57, 42, 89, 52, fTexWidth, fTexHeight );
	m_rtBackML.SetUV( 0, 53, 49, 58, fTexWidth, fTexHeight );
	m_rtBackMM.SetUV( 51, 54, 55, 57, fTexWidth, fTexHeight );
	m_rtBackMR.SetUV( 57, 53, 89, 58, fTexWidth, fTexHeight );
	m_rtBackLL.SetUV( 0, 59, 49, 74, fTexWidth, fTexHeight );
	m_rtBackLM.SetUV( 50, 59, 56, 74, fTexWidth, fTexHeight );
	m_rtBackLR.SetUV( 57, 59, 89, 74, fTexWidth, fTexHeight );

	m_rtBackSub.SetUV( 91, 26, 106, 41, fTexWidth, fTexHeight );

	m_rtEditBoxL.SetUV( 90, 0, 93, 16, fTexWidth, fTexHeight );
	m_rtEditBoxM.SetUV( 95, 0, 101, 16, fTexWidth, fTexHeight );
	m_rtEditBoxR.SetUV( 103, 0, 106, 16, fTexWidth, fTexHeight );

	m_rtEditBoxUL.SetUV( 90, 0, 93, 3, fTexWidth, fTexHeight );
	m_rtEditBoxUM.SetUV( 94, 0, 102, 3, fTexWidth, fTexHeight );
	m_rtEditBoxUR.SetUV( 103, 0, 106, 3, fTexWidth, fTexHeight );
	m_rtEditBoxML.SetUV( 90, 4, 93, 12, fTexWidth, fTexHeight );
	m_rtEditBoxMM.SetUV( 96, 5, 101, 11, fTexWidth, fTexHeight );
	m_rtEditBoxMR.SetUV( 103, 4, 106, 12, fTexWidth, fTexHeight );
	m_rtEditBoxLL.SetUV( 90, 13, 93, 16, fTexWidth, fTexHeight );
	m_rtEditBoxLM.SetUV( 94, 13, 102, 16, fTexWidth, fTexHeight );
	m_rtEditBoxLR.SetUV( 103, 13, 106, 16, fTexWidth, fTexHeight );
	
	// List box of list type
	m_lbListContent.Create( this, 15, 42, 160 , 390, _pUIFontTexMgr->GetFontHeight() + 2, 0, 0, 1, TRUE );
	m_lbListContent.SetOverColor( 0x66B5FFFF );
//	m_lbListContent.SetSelectColor( 0x66B5FFFF );
	m_lbListContent.CreateScroll( TRUE, 0, 0, 9, 390, 9, 7, 0, 0, 10 );
	// Scroll bar of list box
	// Up button
	m_lbListContent.SetScrollUpUV( UBS_IDLE, 46, 117, 55, 124, fTexWidth, fTexHeight );
	m_lbListContent.SetScrollUpUV( UBS_CLICK, 56, 117, 65, 124, fTexWidth, fTexHeight );
	m_lbListContent.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbListContent.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbListContent.SetScrollBarTopUV( 90, 43, 99, 53, fTexWidth, fTexHeight );
	m_lbListContent.SetScrollBarMiddleUV( 90, 54, 99, 56, fTexWidth, fTexHeight );
	m_lbListContent.SetScrollBarBottomUV( 90, 57, 99, 67, fTexWidth, fTexHeight );
	// Down button
	m_lbListContent.SetScrollDownUV( UBS_IDLE, 66, 117, 75, 124, fTexWidth, fTexHeight );
	m_lbListContent.SetScrollDownUV( UBS_CLICK, 76, 117, 85, 124, fTexWidth, fTexHeight );
	m_lbListContent.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbListContent.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Adjust column position of list box
	m_lbListContent.SetColumnPosX( 0, 5 , TEXT_LEFT );

	// List box of read type
	m_lbReadContent.Create( this, 190, 42, 385, 390, _pUIFontTexMgr->GetLineHeight() + 2, 0, 0, 1, FALSE );
	m_lbReadContent.CreateScroll( TRUE, 0, 0, 9, 390, 9, 7, 0, 0, 10 );
	m_lbReadContent.SetOverColor( 0x66B5FFFF );	
	// Scroll bar of list box
	// Up button
	m_lbReadContent.SetScrollUpUV( UBS_IDLE, 46, 117, 55, 124, fTexWidth, fTexHeight );
	m_lbReadContent.SetScrollUpUV( UBS_CLICK, 56, 117, 65, 124, fTexWidth, fTexHeight );
	m_lbReadContent.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbReadContent.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbReadContent.SetScrollBarTopUV( 90, 43, 99, 53, fTexWidth, fTexHeight );
	m_lbReadContent.SetScrollBarMiddleUV( 90, 54, 99, 56, fTexWidth, fTexHeight );
	m_lbReadContent.SetScrollBarBottomUV( 90, 57, 99, 67, fTexWidth, fTexHeight );
	// Down button
	m_lbReadContent.SetScrollDownUV( UBS_IDLE, 66, 117, 75, 124, fTexWidth, fTexHeight );
	m_lbReadContent.SetScrollDownUV( UBS_CLICK, 76, 117, 85, 124, fTexWidth, fTexHeight );
	m_lbReadContent.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbReadContent.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Adjust column position of list box
	m_lbReadContent.SetColumnPosX( 0, 5 , TEXT_LEFT );



	// Close Button
	m_btnClose.Create( this, CTString( "" ), 568, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 108, 0, 122, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 108, 15, 122, 29, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Add list
	m_pHelp1 = &_pUIMgr->m_aHelp1Data;
	int tCnt = m_pHelp1->Count();
	m_ilistIndex.New(tCnt);
	InitList(m_ilistIndex,0);
	SetListArrange(1);
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelp::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelp::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : ToggleVisible()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelp::ToggleVisible()
{
	if( IsVisible() )
	{
		CloseHelp();
		m_bVisible = FALSE;
	}	
}

// ----------------------------------------------------------------------------
// Name : OpenHelp()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelp::OpenHelp()
{
	_pUIMgr->RearrangeOrder( UI_HELP, TRUE );
}

// ----------------------------------------------------------------------------
// Name : CloseHelp()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelp::CloseHelp()
{
	_pUIMgr->RearrangeOrder( UI_HELP, FALSE );
}


// ----------------------------------------------------------------------------
// Name : AddListSubjectString()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelp::AddListSubjectString( CTString &strSubject )
{
	CTString	strTemp = strSubject;

	INDEX	nLength = strTemp.Length();
	if( nLength > HELP_LIST_SUBJECT_MAX_CHAR )
	{
		// Check splitting position for 2 byte characters
		int		nSplitPos = HELP_LIST_SUBJECT_MAX_CHAR - 4;
		BOOL	b2ByteChar = FALSE;
		for( int iPos = 0; iPos < nSplitPos; iPos++ )
		{
			if( strTemp[iPos] & 0x80 )
				b2ByteChar = !b2ByteChar;
			else
				b2ByteChar = FALSE;
		}

		if( b2ByteChar )
			nSplitPos--;

		// Trim right
		strTemp.TrimRight( nSplitPos );
		strTemp += CTString( "..." );
	}

	m_lbListContent.AddString( 0, strTemp, 0xFFAD60FF );
}

// ----------------------------------------------------------------------------
// Name : AddReadingString()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelp::AddReadingString( CUIListBox& lbList, CTString &strContent, COLOR colContent, int iMaxChar )
{
	// Get length of string
	INDEX	nLength = strContent.Length();
	if( nLength == 0 )
		return;

	// wooss 051002
	if(g_iCountry == THAILAND){
		// Get length of string
		INDEX	nThaiLen = FindThaiLen(strContent);
		INDEX	nChatMax= (iMaxChar-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
		if( nLength == 0 )
			return;
		// If length of string is less than max char
		if( nThaiLen <= nChatMax )
		{
			// Check line character
			for( int iPos = 0; iPos < nLength; iPos++ )
			{
				if( strContent[iPos] == '\n' || strContent[iPos] == '\r' )
					break;	
			}

			// Not exist
			if( iPos == nLength )
			{
				lbList.AddString( 0, strContent, colContent );
			}
			else
			{
				// Split string
				CTString	strTemp, strTemp2;
				strContent.Split( iPos, strTemp2, strTemp );
				lbList.AddString( 0, strTemp2, colContent );

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddReadingString( lbList, strTemp, colContent );
			}
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = iMaxChar;
			BOOL	b2ByteChar = FALSE;
			for( int iPos = 0; iPos < nLength; iPos++ )
			{
				if(nChatMax < FindThaiLen(strContent,0,iPos))
					break;
			}
			nSplitPos = iPos;

			// Check line character
			for( iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strContent[iPos] == '\n' || strContent[iPos] == '\r' )
					break;
			}

			// Not exist
			if( iPos == nSplitPos )
			{
				// Split string
				CTString	strTemp, strTemp2;
				strContent.Split( nSplitPos, strTemp2, strTemp );
				lbList.AddString( 0, strTemp2, colContent );

				// Trim space
				if( strTemp[0] == ' ' )
				{
					int	nTempLength = strTemp.Length();
					for( iPos = 1; iPos < nTempLength; iPos++ )
					{
						if( strTemp[iPos] != ' ' )
							break;
					}

					strTemp.TrimLeft( strTemp.Length() - iPos );
				}

				AddReadingString( lbList, strTemp, colContent );
			}
			else
			{
				// Split string
				CTString	strTemp, strTemp2;
				strContent.Split( iPos, strTemp2, strTemp );
				lbList.AddString( 0, strTemp2, colContent );

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddReadingString( lbList, strTemp, colContent );
			}

		}
		
	} else {
		// If length of string is less than max char
		if( nLength <= iMaxChar )
		{
			// Check line character
			for( int iPos = 0; iPos < nLength; iPos++ )
			{
				if( strContent[iPos] == '\n' || strContent[iPos] == '\r' )
					break;	
			}

			// Not exist
			if( iPos == nLength )
			{
				lbList.AddString( 0, strContent, colContent );
			}
			else
			{
				// Split string
				CTString	strTemp, strTemp2;
				strContent.Split( iPos, strTemp2, strTemp );
				lbList.AddString( 0, strTemp2, colContent );

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddReadingString( lbList, strTemp, colContent );
			}
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = iMaxChar;
			BOOL	b2ByteChar = FALSE;
			for( int iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strContent[iPos] & 0x80 )
					b2ByteChar = !b2ByteChar;
				else
					b2ByteChar = FALSE;
			}

			if( b2ByteChar )
				nSplitPos--;

			// Check line character
			for( iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strContent[iPos] == '\n' || strContent[iPos] == '\r' )
					break;
			}

			// Not exist
			if( iPos == nSplitPos )
			{
				// Split string
				CTString	strTemp, strTemp2;
				strContent.Split( nSplitPos, strTemp2, strTemp );
				lbList.AddString( 0, strTemp2, colContent );

				// Trim space
				if( strTemp[0] == ' ' )
				{
					int	nTempLength = strTemp.Length();
					for( iPos = 1; iPos < nTempLength; iPos++ )
					{
						if( strTemp[iPos] != ' ' )
							break;
					}

					strTemp.TrimLeft( strTemp.Length() - iPos );
				}

				AddReadingString( lbList, strTemp, colContent );
			}
			else
			{
				// Split string
				CTString	strTemp, strTemp2;
				strContent.Split( iPos, strTemp2, strTemp );
				lbList.AddString( 0, strTemp2, colContent );

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddReadingString( lbList, strTemp, colContent );
			}
		}
	}
}

// Render Help Left List
void CUIHelp::RenderList()
{

	// List box
	m_lbListContent.Render();

}

// Render Help Right Content
void CUIHelp::RenderContent()
{
	m_lbReadContent.Render();
}

void CUIHelp::RenderImage(int iListIndex)
{	
	// Check invalid
	if(iListIndex<=0) return;
	HELP1 tHelp1 = (*m_pHelp1)[iListIndex];
	if( tHelp1.m_width <= 0 ) return;

	FLOAT fTexWidth	= m_ptdImageTemp->GetPixWidth();
	FLOAT fTexHeight	= m_ptdImageTemp->GetPixHeight();	

	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdImageTemp );

	// Add render regions
	int	nX, nY;
	if( m_bSlideImgX < tHelp1.m_width ) 
		{
		m_bSlideImgX+=50000/cli_iCurrentFPS;
	}
	else 
		m_bSlideImgX = tHelp1.m_width;

	nX = m_nPosX + m_rcRightView.Left - 20;
	nY = m_nPosY + 70;

	// Set UV 
	UIRectUV tUV;
	tUV.SetUV(tHelp1.m_uv_x,tHelp1.m_uv_y,tHelp1.m_uv_x+m_bSlideImgX,tHelp1.m_uv_y+tHelp1.m_height,fTexWidth,fTexHeight);

	_pUIMgr->GetDrawPort()->AddTexture(
		nX - m_bSlideImgX, nY, nX , nY + tHelp1.m_height,
		tUV.U0,tUV.V0,tUV.U1,tUV.V1,
		0xFFFFFFFF);

	_pUIMgr->GetDrawPort()->FlushRenderingQueue();
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelp::Render()
{	
	m_bVisible = TRUE;
	// Set web board texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	int	nX, nY, nX2, nY2;
	// Background
	// Upper left
	nX = m_nPosX;
	nY = m_nPosY;
	nX2 = m_nPosX + m_nWidth;
	nY2 = m_nPosY + 28;
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + 49, nY2,
										m_rtTitleUL.U0, m_rtTitleUL.V0, m_rtTitleUL.U1, m_rtTitleUL.V1,
										0xFFFFFFFF );
	// Upper middle
	_pUIMgr->GetDrawPort()->AddTexture( nX + 49, nY, nX2 - 32, nY2,
										m_rtTitleUM.U0, m_rtTitleUM.V0, m_rtTitleUM.U1, m_rtTitleUM.V1,
										0xFFFFFFFF );
	// Upper right
	_pUIMgr->GetDrawPort()->AddTexture( nX2 - 32, nY, nX2, nY2,
										m_rtTitleUR.U0, m_rtTitleUR.V0, m_rtTitleUR.U1, m_rtTitleUR.V1,
										0xFFFFFFFF );

	nY = nY2;
	nY2 = nY2 + 10;

	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + 49, nY2,
										m_rtBackUL.U0, m_rtBackUL.V0, m_rtBackUL.U1, m_rtBackUL.V1,
										0xFFFFFFFF );
	// Upper middle
	_pUIMgr->GetDrawPort()->AddTexture( nX + 49, nY, nX2 - 32, nY2,
										m_rtBackUM.U0, m_rtBackUM.V0, m_rtBackUM.U1, m_rtBackUM.V1,
										0xFFFFFFFF );
	// Upper right
	_pUIMgr->GetDrawPort()->AddTexture( nX2 - 32, nY, nX2, nY2,
										m_rtBackUR.U0, m_rtBackUR.V0, m_rtBackUR.U1, m_rtBackUR.V1,
										0xFFFFFFFF );

	// Middle left
	nY = m_nPosY + m_nHeight - 15;
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY2, nX + 49, nY,
										m_rtBackML.U0, m_rtBackML.V0, m_rtBackML.U1, m_rtBackML.V1,
										0xFFFFFFFF );
	// Middle middle
	_pUIMgr->GetDrawPort()->AddTexture( nX + 49, nY2, nX2 - 32, nY,
										m_rtBackMM.U0, m_rtBackMM.V0, m_rtBackMM.U1, m_rtBackMM.V1,
										0xFFFFFFFF );
	// Middle right
	_pUIMgr->GetDrawPort()->AddTexture( nX2 - 32, nY2, nX2, nY,
										m_rtBackMR.U0, m_rtBackMR.V0, m_rtBackMR.U1, m_rtBackMR.V1,
										0xFFFFFFFF );

	// Lower left
	nY2 = m_nPosY + m_nHeight;
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + 49, nY2,
										m_rtBackLL.U0, m_rtBackLL.V0, m_rtBackLL.U1, m_rtBackLL.V1,
										0xFFFFFFFF );
	// Lower middle
	_pUIMgr->GetDrawPort()->AddTexture( nX + 49, nY, nX2 - 32, nY2,
										m_rtBackLM.U0, m_rtBackLM.V0, m_rtBackLM.U1, m_rtBackLM.V1,
										0xFFFFFFFF );
	// Lower right
	_pUIMgr->GetDrawPort()->AddTexture( nX2 - 32, nY, nX2, nY2,
										m_rtBackLR.U0, m_rtBackLR.V0, m_rtBackLR.U1, m_rtBackLR.V1,
										0xFFFFFFFF );	

	// Close button
	m_btnClose.Render();

	// Text in web board
	// Title
	_pUIMgr->GetDrawPort()->PutTextEx( _S( 1610, "도움말" ), m_nPosX + HELP_TITLE_OFFSETX,		
										m_nPosY + HELP_TITLE_OFFSETY, 0xFFFFFFFF );	

	// Render subject List
	RenderList();
	RenderContent();

	// Flush all render queue
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	_pUIMgr->GetDrawPort()->EndTextEx();	

	// Render Help Image
	if( m_iImageIdx>=0 && m_bVisibleImg )
		RenderImage( m_iImageIdx );	
	
}

int CUIHelp::SetListArrange(int iStart)
{
	int tCnt = m_pHelp1->Count(); 
	if ( iStart >= tCnt || iStart <= 0) return 0;
	CTString strTemp;
	int tCnt2 = iStart+(*m_pHelp1)[iStart].m_subNum;
	int tCnt3 = (*m_pHelp1)[iStart].m_subLevel;
	
	if( tCnt3 == 0 ) 
	{
		// Add Mark
		// 해외로컬 확인 요!!
		if( g_iCountry == GERMANY || g_iCountry == BRAZIL || g_iCountry == SPAIN || g_iCountry == FRANCE || g_iCountry == POLAND || g_iCountry == TURKEY)//FRANCE_SPAIN_CLOSEBETA_NA_20081124
			strTemp+=CTString("+ ");
		else
			strTemp+=CTString("◆");
	}

	for(int j=0;j<tCnt3;j++)
	{
		// Add Space
		strTemp+=CTString("  ");
		if((tCnt3-1)==j)
			// Add Hipon
			strTemp+=CTString("-");
	}
	strTemp += (*m_pHelp1)[iStart].m_strName;

	// if Exist some pic data...ㅇ
	if( (*m_pHelp1)[iStart].m_width > 0)
		strTemp += CTString("*");
	
	AddListSubjectString( strTemp );
	m_ilistIndex[tListInc++]=iStart;
	
	if((*m_pHelp1)[iStart].m_subEnable && (*m_pHelp1)[iStart].m_subNum >0)
	{
		iStart = SetListArrange(iStart+1);
	}
	else
	{
		iStart = tCnt2+1;
	}
	return SetListArrange(iStart);
}

// ----------------------------------------------------------------------------
// Name : SetListContent()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelp::SetListContent( int iIndex )
{
	if ((*m_pHelp1)[m_ilistIndex[iIndex]].m_subNum >0 )
	{
		// Toggle Enable
		(*m_pHelp1)[m_ilistIndex[iIndex]].m_subEnable = ((*m_pHelp1)[m_ilistIndex[iIndex]].m_subEnable)? 0 : 1;
	// Reset list box
	m_lbListContent.ResetAllStrings();
		InitList(m_ilistIndex,0);
		tListInc =0;
		
		SetListArrange(1);
	}
}

// ----------------------------------------------------------------------------
// Name : SetViewContent()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelp::SetViewContent( int iIndex )
{
		//content
	m_lbReadContent.ResetAllStrings();
	m_iImageIdx = m_ilistIndex[iIndex];
	// Set web board texture
	HELP1 tHelp1 = (*m_pHelp1)[m_iImageIdx];
	if(tHelp1.m_strFile!="")
		{
		CTString strFullPath = _fnmApplicationPath.FileDir();
		strFullPath += CTString("Data\\Interface\\") + tHelp1.m_strFile + CTString(".tex");		
		m_ptdImageTemp = CreateTexture( strFullPath );		
		}
		else
		{
		// Set Default
		m_ptdImageTemp = m_ptdImageTexture; 		
	}
	AddReadingString(m_lbReadContent, tHelp1.m_strName, 0x8E94FFFF);
	AddReadingString(m_lbReadContent, tHelp1.m_strDesc, 0xFFE7ADFF);

}

// ----------------------------------------------------------------------------
// Name : KeyMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIHelp::KeyMessage( MSG *pMsg )
{
	// If web board is not focused
	if( !IsFocused() )
		return WMSG_FAIL;

	return WMSG_SUCCESS;
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIHelp::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	if(!m_bVisible) return WMSG_FAIL;
	
	// Mouse message
		switch( pMsg->message )
		{
		case WM_MOUSEMOVE:
			{
				if( IsInside( nX, nY ) )
					_pUIMgr->SetMouseCursorInsideUIs();

			if( IsInsideRect(nX,nY,m_rcRightView))
				m_bVisibleImg = TRUE;
				else
				{
				m_bVisibleImg = FALSE;
				m_bSlideImgX = 0;
				}



			// Move web board
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			// Close button
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
					// List box
			else if( m_lbListContent.MouseMessage( pMsg ) != WMSG_FAIL )
								return WMSG_SUCCESS;
			// List box
					else if( m_lbReadContent.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
							
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				SetFocus ( TRUE );
				_pUIMgr->RearrangeOrder( UI_HELP, TRUE );

				nOldX = nX;		nOldY = nY;

				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Title bar
				else if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
				}				
						// List box
						if( ( wmsgResult = m_lbListContent.MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							if( wmsgResult == WMSG_COMMAND )
							{
						SetViewContent( m_lbListContent.GetCurSel() );
						SetListContent( m_lbListContent.GetCurSel() );
								
							}
						}
				// List box
				else if( ( wmsgResult = m_lbReadContent.MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
					// Do Nothing
						}
			
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			// If holding button doesn't exist
			if( _pUIMgr->GetHoldBtn().IsEmpty() )
			{
				// Title bar
				bTitleBarClick = FALSE;

				// If teleport isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseHelp();

					return WMSG_SUCCESS;
				}
						// List box
				else if( m_lbListContent.MouseMessage( pMsg ) != WMSG_FAIL )
							return WMSG_SUCCESS;
				// List box
						else if( m_lbReadContent.MouseMessage( pMsg ) != WMSG_FAIL )
							return WMSG_SUCCESS;

				
			}
			// If holding button exists
			else
			{
				if( IsInside( nX, nY ) )
				{
					// Reset holding button
					_pUIMgr->ResetHoldBtn();

					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
			{
				// DO NOTTHING
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				// List box of read type
				if( m_lbListContent.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;							
						// List box of read type
						if( m_lbReadContent.MouseMessage( pMsg ) != WMSG_FAIL )
							return WMSG_SUCCESS;
			}
		}
		break;
	}

	return WMSG_FAIL;
}

// [KH_07042] HELP3

CUIHelp3::CUIHelp3()
{
}

// ----------------------------------------------------------------------------
// Name : ~CUIHelp()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIHelp3::~CUIHelp3()
{
	Destroy();
}

// -----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelp3::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 180, 22 );
	
	// Create web board texture
	
	m_ptdImageTexture	=  _pTextureStock->Obtain_t ( CTString( "Data\\Interface\\Help.tex" ) );
	m_ptdBaseTexture	= CreateTexture( CTString( "Data\\Interface\\WebBoard.tex" ) );
	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	m_rtTitleUL.SetUV( 0, 0, 49, 28, fTexWidth, fTexHeight );
	m_rtTitleUM.SetUV( 50, 0, 56, 28, fTexWidth, fTexHeight );
	m_rtTitleUR.SetUV( 57, 0, 89, 28, fTexWidth, fTexHeight );

	m_rtBackUL.SetUV( 0, 42, 49, 52, fTexWidth, fTexHeight );
	m_rtBackUM.SetUV( 50, 42, 56, 52, fTexWidth, fTexHeight );
	m_rtBackUR.SetUV( 57, 42, 89, 52, fTexWidth, fTexHeight );
	m_rtBackML.SetUV( 0, 53, 49, 58, fTexWidth, fTexHeight );
	m_rtBackMM.SetUV( 51, 54, 55, 57, fTexWidth, fTexHeight );
	m_rtBackMR.SetUV( 57, 53, 89, 58, fTexWidth, fTexHeight );
	m_rtBackLL.SetUV( 0, 59, 49, 74, fTexWidth, fTexHeight );
	m_rtBackLM.SetUV( 50, 59, 56, 74, fTexWidth, fTexHeight );
	m_rtBackLR.SetUV( 57, 59, 89, 74, fTexWidth, fTexHeight );

	m_rtBackSub.SetUV( 91, 26, 106, 41, fTexWidth, fTexHeight );

	// List box of read type
	m_lbContent.Create( this, 10, 38, 217, 305, _pUIFontTexMgr->GetLineHeight() + 2, 0, 0, 1, FALSE );
	m_lbContent.CreateScroll( TRUE, 0, 0, 9, 300, 9, 7, 0, 0, 10 );
	m_lbContent.SetOverColor( 0x66B5FFFF );	
	// Scroll bar of list box
	// Up button
	m_lbContent.SetScrollUpUV( UBS_IDLE, 46, 117, 55, 124, fTexWidth, fTexHeight );
	m_lbContent.SetScrollUpUV( UBS_CLICK, 56, 117, 65, 124, fTexWidth, fTexHeight );
	m_lbContent.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbContent.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbContent.SetScrollBarTopUV( 90, 43, 99, 53, fTexWidth, fTexHeight );
	m_lbContent.SetScrollBarMiddleUV( 90, 54, 99, 56, fTexWidth, fTexHeight );
	m_lbContent.SetScrollBarBottomUV( 90, 57, 99, 67, fTexWidth, fTexHeight );
	// Down button
	m_lbContent.SetScrollDownUV( UBS_IDLE, 66, 117, 75, 124, fTexWidth, fTexHeight );
	m_lbContent.SetScrollDownUV( UBS_CLICK, 76, 117, 85, 124, fTexWidth, fTexHeight );
	m_lbContent.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbContent.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Adjust column position of list box
	m_lbContent.SetColumnPosX( 0, 5 , TEXT_LEFT );
	
	// Close Button
	m_btnTClose.Create( this, CTString( "" ), HELP3_WIDTH - 33, 4, 14, 14 );
	m_btnTClose.SetUV( UBS_IDLE, 108, 0, 122, 14, fTexWidth, fTexHeight );
	m_btnTClose.SetUV( UBS_CLICK, 108, 15, 122, 29, fTexWidth, fTexHeight );
	m_btnTClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnTClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	m_btnBClose.Create( this, _S( 870, "닫기" ), 175, HELP3_HEIGHT - 28, 63, 21 );
	m_btnBClose.SetUV( UBS_IDLE, 0, 94, 63, 115, fTexWidth, fTexHeight );
	m_btnBClose.SetUV( UBS_CLICK, 64, 94, 127, 115, fTexWidth, fTexHeight );
	m_btnBClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnBClose.CopyUV( UBS_IDLE, UBS_DISABLE );
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelp3::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( 0, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelp3::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : OpenHelp()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelp3::OpenHelp(CUIWindow *pMammyWnd)
{
// [KH_070426] 도움말 자동꺼지기를 위해 추가
	m_pMammyWnd = pMammyWnd;
	_pUIMgr->RearrangeOrder( UI_HELP3, TRUE );
}

// ----------------------------------------------------------------------------
// Name : CloseHelp()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelp3::CloseHelp()
{
	_pUIMgr->RearrangeOrder( UI_HELP3, FALSE );
}


// ----------------------------------------------------------------------------
// Name : AddReadingString()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelp3::AddHelpString( CTString &strContent, COLOR colContent, int iMaxChar )
{
	// Get length of string
	INDEX	nLength = strContent.Length();
	if( nLength == 0 )
		return;

	// wooss 051002
	if(g_iCountry == THAILAND){
		// Get length of string
		INDEX	nThaiLen = FindThaiLen(strContent);
		INDEX	nChatMax= (iMaxChar-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
		if( nLength == 0 )
			return;
		// If length of string is less than max char
		if( nThaiLen <= nChatMax )
		{
			// Check line character
			for( int iPos = 0; iPos < nLength; iPos++ )
			{
				if( strContent[iPos] == '\n' || strContent[iPos] == '\r' )
					break;	
			}

			// Not exist
			if( iPos == nLength )
			{
				m_lbContent.AddString( 0, strContent, colContent );
			}
			else
			{
				// Split string
				CTString	strTemp, strTemp2;
				strContent.Split( iPos, strTemp2, strTemp );
				m_lbContent.AddString( 0, strTemp2, colContent );

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddHelpString( strTemp, colContent );
			}
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = iMaxChar;
			BOOL	b2ByteChar = FALSE;
			for( int iPos = 0; iPos < nLength; iPos++ )
			{
				if(nChatMax < FindThaiLen(strContent,0,iPos))
					break;
			}
			nSplitPos = iPos;

			// Check line character
			for( iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strContent[iPos] == '\n' || strContent[iPos] == '\r' )
					break;
			}

			// Not exist
			if( iPos == nSplitPos )
			{
				// Split string
				CTString	strTemp, strTemp2;
				strContent.Split( nSplitPos, strTemp2, strTemp );
				m_lbContent.AddString( 0, strTemp2, colContent );

				// Trim space
				if( strTemp[0] == ' ' )
				{
					int	nTempLength = strTemp.Length();
					for( iPos = 1; iPos < nTempLength; iPos++ )
					{
						if( strTemp[iPos] != ' ' )
							break;
					}

					strTemp.TrimLeft( strTemp.Length() - iPos );
				}

				AddHelpString( strTemp, colContent );
			}
			else
			{
				// Split string
				CTString	strTemp, strTemp2;
				strContent.Split( iPos, strTemp2, strTemp );
				m_lbContent.AddString( 0, strTemp2, colContent );

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddHelpString( strTemp, colContent );
			}

		}
		
	} else {
		// If length of string is less than max char
		if( nLength <= iMaxChar )
		{
			// Check line character
			for( int iPos = 0; iPos < nLength; iPos++ )
			{
				if( strContent[iPos] == '\n' || strContent[iPos] == '\r' )
					break;	
			}

			// Not exist
			if( iPos == nLength )
			{
				m_lbContent.AddString( 0, strContent, colContent );
			}
			else
			{
				// Split string
				CTString	strTemp, strTemp2;
				strContent.Split( iPos, strTemp2, strTemp );
				m_lbContent.AddString( 0, strTemp2, colContent );

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddHelpString( strTemp, colContent );
			}
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = iMaxChar;
			BOOL	b2ByteChar = FALSE;
			for( int iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strContent[iPos] & 0x80 )
					b2ByteChar = !b2ByteChar;
				else
					b2ByteChar = FALSE;
			}

			if( b2ByteChar )
				nSplitPos--;

			// Check line character
			for( iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strContent[iPos] == '\n' || strContent[iPos] == '\r' )
					break;
			}

			// Not exist
			if( iPos == nSplitPos )
			{
				// Split string
				CTString	strTemp, strTemp2;
				strContent.Split( nSplitPos, strTemp2, strTemp );
				m_lbContent.AddString( 0, strTemp2, colContent );

				// Trim space
				if( strTemp[0] == ' ' )
				{
					int	nTempLength = strTemp.Length();
					for( iPos = 1; iPos < nTempLength; iPos++ )
					{
						if( strTemp[iPos] != ' ' )
							break;
					}

					strTemp.TrimLeft( strTemp.Length() - iPos );
				}

				AddHelpString( strTemp, colContent );
			}
			else
			{
				// Split string
				CTString	strTemp, strTemp2;
				strContent.Split( iPos, strTemp2, strTemp );
				m_lbContent.AddString( 0, strTemp2, colContent );

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddHelpString( strTemp, colContent );
			}
		}
	}
}

void CUIHelp3::ClearHelpString()
{
	m_lbContent.ResetAllStrings();
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelp3::Render()
{	
	// Set web board texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	int	nX, nY, nX2, nY2;
	// Background
	// Upper left
	nX = m_nPosX;
	nY = m_nPosY;
	nX2 = m_nPosX + m_nWidth;
	nY2 = m_nPosY + 28;
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + 49, nY2,
										m_rtTitleUL.U0, m_rtTitleUL.V0, m_rtTitleUL.U1, m_rtTitleUL.V1,
										0xFFFFFFFF );
	// Upper middle
	_pUIMgr->GetDrawPort()->AddTexture( nX + 49, nY, nX2 - 32, nY2,
										m_rtTitleUM.U0, m_rtTitleUM.V0, m_rtTitleUM.U1, m_rtTitleUM.V1,
										0xFFFFFFFF );
	// Upper right
	_pUIMgr->GetDrawPort()->AddTexture( nX2 - 32, nY, nX2, nY2,
										m_rtTitleUR.U0, m_rtTitleUR.V0, m_rtTitleUR.U1, m_rtTitleUR.V1,
										0xFFFFFFFF );

	nY = nY2;
	nY2 = nY2 + 10;

	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + 49, nY2,
										m_rtBackUL.U0, m_rtBackUL.V0, m_rtBackUL.U1, m_rtBackUL.V1,
										0xFFFFFFFF );
	// Upper middle
	_pUIMgr->GetDrawPort()->AddTexture( nX + 49, nY, nX2 - 32, nY2,
										m_rtBackUM.U0, m_rtBackUM.V0, m_rtBackUM.U1, m_rtBackUM.V1,
										0xFFFFFFFF );
	// Upper right
	_pUIMgr->GetDrawPort()->AddTexture( nX2 - 32, nY, nX2, nY2,
										m_rtBackUR.U0, m_rtBackUR.V0, m_rtBackUR.U1, m_rtBackUR.V1,
										0xFFFFFFFF );

	// Middle left
	nY = m_nPosY + m_nHeight - 38;
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY2, nX + 49, nY,
										m_rtBackML.U0, m_rtBackML.V0, m_rtBackML.U1, m_rtBackML.V1,
										0xFFFFFFFF );
	// Middle middle
	_pUIMgr->GetDrawPort()->AddTexture( nX + 49, nY2, nX2 - 32, nY,
										m_rtBackMM.U0, m_rtBackMM.V0, m_rtBackMM.U1, m_rtBackMM.V1,
										0xFFFFFFFF );
	// Middle right
	_pUIMgr->GetDrawPort()->AddTexture( nX2 - 32, nY2, nX2, nY,
										m_rtBackMR.U0, m_rtBackMR.V0, m_rtBackMR.U1, m_rtBackMR.V1,
										0xFFFFFFFF );

	// Lower left
	nY2 = m_nPosY + m_nHeight;
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + 49, nY2,
										m_rtBackLL.U0, m_rtBackLL.V0, m_rtBackLL.U1, m_rtBackLL.V1,
										0xFFFFFFFF );
	// Lower middle
	_pUIMgr->GetDrawPort()->AddTexture( nX + 49, nY, nX2 - 32, nY2,
										m_rtBackLM.U0, m_rtBackLM.V0, m_rtBackLM.U1, m_rtBackLM.V1,
										0xFFFFFFFF );
	// Lower right
	_pUIMgr->GetDrawPort()->AddTexture( nX2 - 32, nY, nX2, nY2,
										m_rtBackLR.U0, m_rtBackLR.V0, m_rtBackLR.U1, m_rtBackLR.V1,
										0xFFFFFFFF );	

	// Close button
	m_btnTClose.Render();
	m_btnBClose.Render();

	// Text in web board
	// Title
	_pUIMgr->GetDrawPort()->PutTextEx( _S( 1610, "도움말" ), m_nPosX + HELP_TITLE_OFFSETX,
										m_nPosY + HELP_TITLE_OFFSETY, 0xFFFFFFFF );	

	// Render subject List
	m_lbContent.Render();

	// Flush all render queue
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	_pUIMgr->GetDrawPort()->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : KeyMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIHelp3::KeyMessage( MSG *pMsg )
{
	// If web board is not focused
	if( !IsFocused() )
		return WMSG_FAIL;

	return WMSG_SUCCESS;
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIHelp3::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

// [KH_07042]	if(!m_bVisible) return WMSG_FAIL;
	
	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				_pUIMgr->SetMouseCursorInsideUIs();

			// Move web board
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			// Close button
			else if( m_btnTClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnBClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// List box
			else if( m_lbContent.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				SetFocus ( TRUE );
				_pUIMgr->RearrangeOrder( UI_HELP3, TRUE );

				nOldX = nX;		nOldY = nY;

				// Close button
				if( m_btnTClose.MouseMessage( pMsg ) != WMSG_FAIL  || m_btnBClose.MouseMessage( pMsg ) != WMSG_FAIL)
				{
					// Nothing
				}
				// Title bar
				else if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
				}
				// List box
				else if( ( wmsgResult = m_lbContent.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					// Do Nothing
				}
			
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			// If holding button doesn't exist
			if( _pUIMgr->GetHoldBtn().IsEmpty() )
			{
				// Title bar
				bTitleBarClick = FALSE;

				// If teleport isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnTClose.MouseMessage( pMsg ) ) != WMSG_FAIL || ( wmsgResult = m_btnBClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseHelp();

					return WMSG_SUCCESS;
				}
				// List box
				else if( m_lbContent.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
			// If holding button exists
			else
			{
				if( IsInside( nX, nY ) )
				{
					// Reset holding button
					_pUIMgr->ResetHoldBtn();

					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
			{
				// DO NOTTHING
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				// List box of read type
				if( m_lbContent.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;							
			}
		}
		break;
	}

	return WMSG_FAIL;
}
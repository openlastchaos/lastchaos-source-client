#include "stdh.h"
#include <Engine/Interface/UIMix.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Entities/Itemdata.h>
#include <Engine/Interface/UIInventory.h>


static int	_iMaxMsgStringChar = 0;

extern INDEX g_iCountry;
// ----------------------------------------------------------------------------
// Name : CUIMix()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIMix::CUIMix()
{
	m_nStringCount		= 0;
	m_bWaitMixResult	= TRUE;
	m_nCurItemSlot		= -1;
	m_nCurItemCount		= 0;

	m_dwWndState = UWS_ENABLE | UWS_VISIBLE;

	m_nTextItemIndex	= -1;						
	m_nTextRow			= -1;
	m_nTextCol			= -1;
	
}

// ----------------------------------------------------------------------------
// Name : ~CUIMix()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIMix::~CUIMix()
{
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIMix::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	_iMaxMsgStringChar = 187 / ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 216, 22 );
	
	m_rcItemSlot[0].SetRect( 66, 0, 66 + MIX_ITEM_SLOT_SIZE, 0 );
	m_rcItemSlot[1].SetRect( 116, 0, 116 + MIX_ITEM_SLOT_SIZE, 0 );
	m_rcItemSlot[2].SetRect( 91, 0, 91 + MIX_ITEM_SLOT_SIZE, 0 );

	m_rcInsertItem.SetRect( 3, 0, 213, 0 );

	// Create mix texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MessageBox.tex" ) );
	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	m_rtTop.SetUV( 0, 0, 216, 26, fTexWidth, fTexHeight );
	m_rtMiddle1.SetUV( 0, 31, 216, 33, fTexWidth, fTexHeight );
	m_rtMiddle2.SetUV( 0, 35, 216, 37, fTexWidth, fTexHeight );
	m_rtBottom.SetUV( 0, 38, 216, 45, fTexWidth, fTexHeight );
	m_rtItemSlot.SetUV( 0, 68, 34, 102, fTexWidth, fTexHeight );

	// Close button
	m_btnClose.Create( this, CTString( "" ), 184, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 219, 0, 233, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 234, 0, 248, 14, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// OK button
	m_btnOK.Create( this, _S( 191, "확인" ), 36, 154, 63, 21 );					
	m_btnOK.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnOK.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnOK.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOK.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Cancel button
	m_btnCancel.Create( this, _S( 139, "취소" ), 117, 154, 63, 21 );
	m_btnCancel.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Item information region
	m_rtInfoUL.SetUV( 164, 45, 171, 63, fTexWidth, fTexHeight );
	m_rtInfoUM.SetUV( 174, 45, 176, 63, fTexWidth, fTexHeight );
	m_rtInfoUR.SetUV( 179, 45, 186, 63, fTexWidth, fTexHeight );
	
	m_rtInfoML.SetUV( 164, 55, 171, 58, fTexWidth, fTexHeight );
	m_rtInfoMM.SetUV( 174, 55, 176, 58, fTexWidth, fTexHeight );
	m_rtInfoMR.SetUV( 179, 55, 186, 58, fTexWidth, fTexHeight );
	
	m_rtInfoLL.SetUV( 164, 60, 171, 68, fTexWidth, fTexHeight );
	m_rtInfoLM.SetUV( 174, 60, 176, 68, fTexWidth, fTexHeight );
	m_rtInfoLR.SetUV( 179, 60, 186, 68, fTexWidth, fTexHeight );

	// Add string
	AddString( _S( 553, "조합하실 악세사리를 인벤토리에서 선택하여 넣어주십시오." ) );							

	// Set region of slot item & money...
	int	nNewHeight = MIX_DESC_TEXT_SY + ( m_nStringCount + 1 ) * _pUIFontTexMgr->GetLineHeight();
		
	m_rcItemSlot[0].Top = nNewHeight;
	m_rcItemSlot[0].Bottom = m_rcItemSlot[0].Top + MIX_ITEM_SLOT_SIZE;

	m_rcItemSlot[1].Top = nNewHeight;
	m_rcItemSlot[1].Bottom = m_rcItemSlot[1].Top + MIX_ITEM_SLOT_SIZE;

	nNewHeight += 12 + MIX_ITEM_SLOT_SIZE;

	m_rcItemSlot[2].Top = nNewHeight;
	m_rcItemSlot[2].Bottom = m_rcItemSlot[2].Top + MIX_ITEM_SLOT_SIZE;

	nNewHeight += MIX_ITEM_SLOT_SIZE + _pUIFontTexMgr->GetLineHeight();
	m_nTextRegionHeight = nNewHeight - MIX_DESC_TEXT_SY + 8;

	m_rcInsertItem.Top = MIX_DESC_TEXT_SY;
	m_rcInsertItem.Bottom = nNewHeight;

	nNewHeight += 6;
	m_btnOK.SetPosY( nNewHeight );
	m_btnCancel.SetPosY( nNewHeight );

	nNewHeight += m_btnOK.GetHeight() + 7;
	SetHeight( nNewHeight );

	// Slot item button
	for ( int i  = 0; i < MIX_ITEM_SLOT_COUNT; i++ )
	{
		m_btnItemSlot[i].Create( this, m_rcItemSlot[i].Left + 1, m_rcItemSlot[i].Top + 1, BTN_SIZE, BTN_SIZE, UI_MIX, UBET_ITEM );
	}

	m_bShowItemInfo = false;
	m_nCurInfoLines = 0;
	m_bDetailItemInfo = TRUE;
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIMix::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIMix::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : OpenMix()
// Desc :
// ----------------------------------------------------------------------------
void CUIMix::OpenMix ( int nItemIndex, int nRow, int nCol )
{
	// If this is already exist
	if( _pUIMgr->DoesMessageBoxLExist( MSGLCMD_MIX_REQ ) || IsVisible() )
		return;

	// If inventory is already locked
	if( _pUIMgr->GetInventory()->IsLocked() )
	{
		_pUIMgr->GetInventory()->ShowLockErrorMessage();
		return;
	}

	m_nTextItemIndex	= nItemIndex;						
	m_nTextRow			= nRow;
	m_nTextCol			= nCol;

	if( !_pUIMgr->GetInventory()->IsVisible() )
		_pUIMgr->GetInventory()->ToggleVisible();

	_pUIMgr->RearrangeOrder( UI_MIX, TRUE );

	m_bWaitMixResult	= FALSE;
	m_nCurItemSlot		= -1;
	m_nCurItemCount		= 0;
	m_btnOK.SetEnable ( FALSE );
}

// ----------------------------------------------------------------------------
// Name : CloseMix()
// Desc :
// ----------------------------------------------------------------------------
void CUIMix::CloseMix()
{
	// Reset slot item
	for ( int i = 0; i < MIX_ITEM_SLOT_COUNT; i++ )
	{
		m_btnItemSlot[i].InitBtn();
	}

	m_nTextItemIndex	= -1;						
	m_nTextRow			= -1;
	m_nTextCol			= -1;

	// Close Mix
	_pUIMgr->RearrangeOrder( UI_MIX, FALSE );

	// Unlock inventory
	_pUIMgr->GetInventory()->Lock( FALSE, FALSE, LOCK_MIX );

	m_bWaitMixResult = FALSE;
}

// ----------------------------------------------------------------------------
// Name : AddString()
// Desc :
// ----------------------------------------------------------------------------
void CUIMix::AddString( CTString &strDesc )
{
	if( m_nStringCount >= MAX_MIX_STRING )
		return;

	// Get length of string
	INDEX	nLength = strDesc.Length();
	if( nLength == 0 )
		return;

	// wooss 051002
	if(g_iCountry == THAILAND){
		// Get length of string
		INDEX	nThaiLen = FindThaiLen(strDesc);
		INDEX	nChatMax= (_iMaxMsgStringChar-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
		if( nLength == 0 )
			return;
		// If length of string is less than max char
		if( nThaiLen <= nChatMax )
		{
			// Check line character
			for( int iPos = 0; iPos < nLength; iPos++ )
			{
				if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
					break;
			}

			// Not exist
			if( iPos == nLength )
				m_strMixDesc[m_nStringCount++] = strDesc;
			else
			{
				// Split string
				CTString	strTemp;
				strDesc.Split( iPos, m_strMixDesc[m_nStringCount++], strTemp );

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddString( strTemp );
			}
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = _iMaxMsgStringChar;
			BOOL	b2ByteChar = FALSE;
			for( int iPos = 0; iPos < nLength; iPos++ )
			{
				if(nChatMax < FindThaiLen(strDesc,0,iPos))
					break;
			}
			nSplitPos = iPos;

			// Check line character
			for( iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
					break;
			}

			// Not exist
			if( iPos == nSplitPos )
			{
				// Split string
				CTString	strTemp;
				strDesc.Split( nSplitPos, m_strMixDesc[m_nStringCount++], strTemp );

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

				AddString( strTemp );
			}
			else
			{
				// Split string
				CTString	strTemp;
				strDesc.Split( iPos, m_strMixDesc[m_nStringCount++], strTemp );

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddString( strTemp );
			}

		}
		
	} else {
		// If length of string is less than max char
		if( nLength <= _iMaxMsgStringChar )
		{
			// Check line character
			for( int iPos = 0; iPos < nLength; iPos++ )
			{
				if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
					break;
			}

			// Not exist
			if( iPos == nLength )
				m_strMixDesc[m_nStringCount++] = strDesc;
			else
			{
				// Split string
				CTString	strTemp;
				strDesc.Split( iPos, m_strMixDesc[m_nStringCount++], strTemp );

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddString( strTemp );
			}
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = _iMaxMsgStringChar;
			BOOL	b2ByteChar = FALSE;
			for( int iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strDesc[iPos] & 0x80 )
					b2ByteChar = !b2ByteChar;
				else
					b2ByteChar = FALSE;
			}

			if( b2ByteChar )
				nSplitPos--;

			// Check line character
			for( iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
					break;
			}

			// Not exist
			if( iPos == nSplitPos )
			{
				// Split string
				CTString	strTemp;
				strDesc.Split( nSplitPos, m_strMixDesc[m_nStringCount++], strTemp );

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

				AddString( strTemp );
			}
			else
			{
				// Split string
				CTString	strTemp;
				strDesc.Split( iPos, m_strMixDesc[m_nStringCount++], strTemp );

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddString( strTemp );
			}
		}
	}
}

// ----------------------------------------------------------------------------
// Name : !!Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIMix::Render()
{
	// Set mix texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	int	nX, nY;
	// Background
	// Top
	nX = m_nPosX + m_nWidth;
	nY = m_nPosY + 26;
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY, nX, nY,
										m_rtTop.U0, m_rtTop.V0, m_rtTop.U1, m_rtTop.V1,
										0xFFFFFFFF );

	// Middle 1
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, nX, nY + m_nTextRegionHeight,
										m_rtMiddle1.U0, m_rtMiddle1.V0, m_rtMiddle1.U1, m_rtMiddle1.V1,
										0xFFFFFFFF );

	// Middle 2
	nY += m_nTextRegionHeight;
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, nX, m_nPosY + m_nHeight - 7,
										m_rtMiddle2.U0, m_rtMiddle2.V0, m_rtMiddle2.U1, m_rtMiddle2.V1,
										0xFFFFFFFF );

	// Bottom
	nY = m_nPosY + m_nHeight - 7;
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, nX, m_nPosY + m_nHeight,
										m_rtBottom.U0, m_rtBottom.V0, m_rtBottom.U1, m_rtBottom.V1,
										0xFFFFFFFF );

	for ( int i = 0; i < MIX_ITEM_SLOT_COUNT; i++ )
	{
		// Slot item region
		_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcItemSlot[i].Left, m_nPosY + m_rcItemSlot[i].Top,
											m_nPosX + m_rcItemSlot[i].Right, m_nPosY + m_rcItemSlot[i].Bottom,
											m_rtItemSlot.U0, m_rtItemSlot.V0, m_rtItemSlot.U1, m_rtItemSlot.V1,
											0xFFFFFFFF );
	}

	// Close button
	m_btnClose.Render();

	// OK button
	m_btnOK.Render();

	// Cancel button
	m_btnCancel.Render();

	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	// Item
	for ( i = 0; i < MIX_ITEM_SLOT_COUNT; i++ )
	{
		if( !m_btnItemSlot[i].IsEmpty() )
		{
			m_btnItemSlot[i].Render();
			_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_ITEM );
		}
	}

	// Text in mix
	_pUIMgr->GetDrawPort()->PutTextEx( _S( 552, "재활의 상자" ), m_nPosX + MIX_TITLE_TEXT_OFFSETX,			
										m_nPosY + MIX_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );

	nX = m_nPosX + MIX_DESC_TEXT_SX;
	nY = m_nPosY + MIX_DESC_TEXT_SY;
	for( int iDesc = 0; iDesc < m_nStringCount; iDesc++ )
	{
		_pUIMgr->GetDrawPort()->PutTextEx( m_strMixDesc[iDesc], nX , nY, 0xC5C5C5FF );
		nY += _pUIFontTexMgr->GetLineHeight();
	}

	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();

	//_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );
	RenderItemInfo ();
	//_pUIMgr->GetDrawPort()->FlushRenderingQueue();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIMix::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;
	
	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
			{
				_pUIMgr->SetMouseCursorInsideUIs();
			
				if( IsInsideRect( nX, nY, m_rcInsertItem ) )
				{
					bool bShowItem = false;
					for( int iRow = 0; iRow < 3; iRow++ )
					{
						if( m_btnItemSlot[iRow].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							bShowItem = true;
							ShowItemInfo( true, iRow ) ;					
						}
					}

					if ( !bShowItem ) 
						ShowItemInfo( false, -1 );
				}
			}
			else ShowItemInfo( false, -1 );

			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			// Move mix
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			// Close button
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// OK button
			else if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Cancel button
			else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( _pUIMgr->GetHoldBtn().IsEmpty() )
			{
				if( IsInsideRect( nX, nY, m_rcInsertItem ) && ( pMsg->wParam & MK_LBUTTON ) &&
					( ndX != 0 || ndY != 0 ) )
				{
					for ( int i = 0; i < MIX_ITEM_SLOT_COUNT; i++ )
					{
						if ( IsInsideRect( nX, nY, m_rcItemSlot[i] ) )
						{
							m_nCurItemSlot = i;
							
							_pUIMgr->SetHoldBtn( m_btnItemSlot[m_nCurItemSlot] );
							int	nOffset = BTN_SIZE / 2;
							_pUIMgr->GetHoldBtn().SetPos( nX - nOffset,	nY - nOffset );
							return WMSG_SUCCESS;
						}
					}
				}
			}
			else if ( !_pUIMgr->GetHoldBtn().IsEmpty() )
			{
				ShowItemInfo( false, -1 );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
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
				// OK button
				else if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Cancel button
				else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				
				_pUIMgr->RearrangeOrder( UI_MIX, TRUE );
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

				// If Mix isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseMix();

					return WMSG_SUCCESS;
				}
				// OK button
				else if( ( wmsgResult = m_btnOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						SendMixReq();

					return WMSG_SUCCESS;
				}
				// Cancel button
				else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseMix();

					return WMSG_SUCCESS;
				}
			}  
			// If holding button exists
			else
			{
				if( IsInside( nX, nY ) )
				{
					//!! If holding button is item and comes from inventory
					if( _pUIMgr->GetHoldBtn().GetBtnType() == UBET_ITEM &&
						_pUIMgr->GetHoldBtn().GetWhichUI() == UI_INVENTORY ||
						_pUIMgr->GetHoldBtn().GetWhichUI() == UI_MIX )
					{
						if( IsInsideRect( nX, nY, m_rcInsertItem ) )
						{
							/* 슬롯 0번 부터 차례 대로 입력하는 방식 
							for ( int i = 0; i < MIX_ITEM_SLOT_COUNT; i++ )
							{
								if ( IsInsideRect( nX-BTN_SIZE/2, nY-BTN_SIZE/2, m_rcItemSlot[i] ) )
								{
									// Set Mix item
									SetMixItem ( i );
									return WMSG_SUCCESS;
								}
							}
							*/
							SetMixItem ( NearPosition ( nX-BTN_SIZE/2, nY-BTN_SIZE/2 ) );
						}
					}
					return WMSG_SUCCESS;
				}
				else
				{
					ResetMixItem ();
					_pUIMgr->ResetHoldBtn();

					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) && IsInsideRect( nX, nY, m_rcInsertItem ) )
			{		
				m_nCurItemSlot = NearPosition ( nX-BTN_SIZE/2, nY-BTN_SIZE/2 );	
				ResetMixItem ();
				_pUIMgr->ResetHoldBtn();
			}
				return WMSG_SUCCESS;
		}
		break;
	}

	return WMSG_FAIL;
}


// ----------------------------------------------------------------------------
// Name : RenderItemInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIMix::RenderItemInfo ()
{
	// ----------------------------------------------------------------------------
	// Item information ( name and property etc... )
	if( m_bShowItemInfo )
	{
		_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

		// Item information region
		_pUIMgr->GetDrawPort()->AddTexture( m_rcItemInfo.Left, m_rcItemInfo.Top,
											m_rcItemInfo.Left + 7, m_rcItemInfo.Top + 7,
											m_rtInfoUL.U0, m_rtInfoUL.V0, m_rtInfoUL.U1, m_rtInfoUL.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_rcItemInfo.Left + 7, m_rcItemInfo.Top,
											m_rcItemInfo.Right - 7, m_rcItemInfo.Top + 7,
											m_rtInfoUM.U0, m_rtInfoUM.V0, m_rtInfoUM.U1, m_rtInfoUM.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_rcItemInfo.Right - 7, m_rcItemInfo.Top,
											m_rcItemInfo.Right, m_rcItemInfo.Top + 7,
											m_rtInfoUR.U0, m_rtInfoUR.V0, m_rtInfoUR.U1, m_rtInfoUR.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_rcItemInfo.Left, m_rcItemInfo.Top + 7,
											m_rcItemInfo.Left + 7, m_rcItemInfo.Bottom - 7,
											m_rtInfoML.U0, m_rtInfoML.V0, m_rtInfoML.U1, m_rtInfoML.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_rcItemInfo.Left + 7, m_rcItemInfo.Top + 7,
											m_rcItemInfo.Right - 7, m_rcItemInfo.Bottom - 7,
											m_rtInfoMM.U0, m_rtInfoMM.V0, m_rtInfoMM.U1, m_rtInfoMM.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_rcItemInfo.Right - 7, m_rcItemInfo.Top + 7,
											m_rcItemInfo.Right, m_rcItemInfo.Bottom - 7,
											m_rtInfoMR.U0, m_rtInfoMR.V0, m_rtInfoMR.U1, m_rtInfoMR.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_rcItemInfo.Left, m_rcItemInfo.Bottom - 7,
											m_rcItemInfo.Left + 7, m_rcItemInfo.Bottom,
											m_rtInfoLL.U0, m_rtInfoLL.V0, m_rtInfoLL.U1, m_rtInfoLL.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_rcItemInfo.Left + 7, m_rcItemInfo.Bottom - 7,
											m_rcItemInfo.Right - 7, m_rcItemInfo.Bottom,
											m_rtInfoLM.U0, m_rtInfoLM.V0, m_rtInfoLM.U1, m_rtInfoLM.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_rcItemInfo.Right - 7, m_rcItemInfo.Bottom - 7,
											m_rcItemInfo.Right, m_rcItemInfo.Bottom,
											m_rtInfoLR.U0, m_rtInfoLR.V0, m_rtInfoLR.U1, m_rtInfoLR.V1,
											0xFFFFFFFF );

		// Render all elements
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();

		// Render item information
		int	nInfoX = m_rcItemInfo.Left + 12;
		int	nInfoY = m_rcItemInfo.Top + 8;
		for( int iInfo = 0; iInfo < m_nCurInfoLines; iInfo++ )
		{
			_pUIMgr->GetDrawPort()->PutTextEx( m_strItemInfo[iInfo], nInfoX, nInfoY, m_colItemInfo[iInfo] );
			nInfoY += _pUIFontTexMgr->GetLineHeight();
		}

		// Flush all render text queue
		_pUIMgr->GetDrawPort()->EndTextEx();
	}
}

// ----------------------------------------------------------------------------
// Name : AddItemInfoString()
// Desc :
// ----------------------------------------------------------------------------
void CUIMix::AddItemInfoString( CTString &strItemInfo, COLOR colItemInfo )
{
	if( m_nCurInfoLines >= MAX_ITEMINFO_LINE )
		return ;

	// Get length of string
	INDEX	nLength = strItemInfo.Length();
	if( nLength <= 0 )
		return;

	// If length of string is less than max char
	if( nLength <= MAX_ITEMINFO_CHAR )
	{
		m_strItemInfo[m_nCurInfoLines] = strItemInfo;
		m_colItemInfo[m_nCurInfoLines++] = colItemInfo;
	}
	// Need multi-line
	else
	{
		// Check splitting position for 2 byte characters
		int		nSplitPos = MAX_ITEMINFO_CHAR;
		BOOL	b2ByteChar = FALSE;
		for( int iPos = 0; iPos < nSplitPos; iPos++ )
		{
			if( strItemInfo[iPos] & 0x80 )
				b2ByteChar = !b2ByteChar;
			else
				b2ByteChar = FALSE;
		}

		if( b2ByteChar )
			nSplitPos--;

		// Split string
		CTString	strTemp;
		strItemInfo.Split( nSplitPos, m_strItemInfo[m_nCurInfoLines], strTemp );
		m_colItemInfo[m_nCurInfoLines++] = colItemInfo;

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

		AddItemInfoString( strTemp, colItemInfo );
	}
}

// ----------------------------------------------------------------------------
// Name : ShowItemInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIMix::ShowItemInfo( BOOL bShowInfo, int nItemIndex, BOOL bRenew )
{
	static int	nOldBtnID = -1;
	int			nBtnID;

	m_bShowItemInfo = FALSE;

	// Hide item information
	if( !bShowInfo )
	{
		nOldBtnID = -1;
		return;
	}

	BOOL	bUpdateInfo = FALSE;
	int		nInfoWidth, nInfoHeight;
	int		nInfoPosX, nInfoPosY;

	
	if( nItemIndex >= 0 )
	{
		m_bShowItemInfo = TRUE;
		nBtnID = m_btnItemSlot[nItemIndex].GetBtnID();

		// Update item information
		if( nOldBtnID != nBtnID || bRenew )
		{
			bUpdateInfo = TRUE;
			nOldBtnID = nBtnID;
			m_btnItemSlot[nItemIndex].GetAbsPos( nInfoPosX, nInfoPosY );

			// Get item information
			//m_bDetailItemInfo = m_nSelectProcessItem == nItemIndex;
			
			if( !GetItemInfo(  nItemIndex, nInfoWidth, nInfoHeight ) )
				m_bShowItemInfo = FALSE;
		}
	}

	// Update item information box
	if( m_bShowItemInfo && bUpdateInfo )
	{
		nInfoPosX += BTN_SIZE / 2 - nInfoWidth / 2;

		if( nInfoPosX < _pUIMgr->GetMinI() )
			nInfoPosX = _pUIMgr->GetMinI();
		else if( nInfoPosX + nInfoWidth > _pUIMgr->GetMaxI() )
			nInfoPosX = _pUIMgr->GetMaxI() - nInfoWidth;

		if( nInfoPosY - nInfoHeight < _pUIMgr->GetMinJ() )
		{
			nInfoPosY += BTN_SIZE;
			m_rcItemInfo.SetRect( nInfoPosX, nInfoPosY, nInfoPosX + nInfoWidth, nInfoPosY + nInfoHeight );
		}
		else
		{
			m_rcItemInfo.SetRect( nInfoPosX, nInfoPosY - nInfoHeight, nInfoPosX + nInfoWidth, nInfoPosY );
		}
	}

	if( !m_bShowItemInfo )
		nOldBtnID = -1;
}

// ----------------------------------------------------------------------------
// Name : GetItemInfo()
// Desc : 
// ----------------------------------------------------------------------------
BOOL CUIMix::GetItemInfo( int nItemIndex, int &nInfoWidth, int &nInfoHeight )
{
	CTString	strTemp;
	m_nCurInfoLines = 0;

	int			nIndex;
	SBYTE		sbOptionType[MAX_ITEM_OPTION], sbOptionLevel[MAX_ITEM_OPTION];

	nIndex		= m_btnItemSlot[nItemIndex].GetItemIndex();
	SBYTE iRow	= m_btnItemSlot[nItemIndex].GetItemRow ();
	SBYTE iCol	= m_btnItemSlot[nItemIndex].GetItemCol ();

	CItems	&rItems = _pNetwork->MySlotItem[0][iRow][iCol];
		
	for( SBYTE sbOption = 0; sbOption < MAX_ITEM_OPTION; sbOption++ )
	{
		sbOptionType[sbOption] = rItems.GetOptionType( sbOption );
		sbOptionLevel[sbOption] = rItems.GetOptionLevel( sbOption );
	}
	
	if( nIndex < 0 )
		return FALSE;

	CItemData	&rItemData = _pNetwork->GetItemData( nIndex );
	const char* szItemName = _pNetwork->GetItemName( rItemData.GetItemIndex() );

	// Get item name
	strTemp = szItemName;
	AddItemInfoString( strTemp ); // 리스트에 추가 

	// Get item information in detail
	if( m_bDetailItemInfo )
	{
		switch( rItemData.GetType() )
		{
		case CItemData::ITEM_ACCESSORY:		// Accessory
			{
				if(rItems.Item_Used > 0)
				{
					strTemp.PrintF(  _S( 510, "내구도 : %ld" ), rItems.Item_Used);		
					AddItemInfoString( strTemp, 0xDEC05BFF );
				}
			}
			break;
		}

		// Weight
		strTemp.PrintF( _S( 165, "무게 : %d" ), rItemData.GetWeight() );
		AddItemInfoString( strTemp, 0xDEC05BFF );

		// Options
		switch( rItemData.GetType() )
		{
		case CItemData::ITEM_WEAPON:
		case CItemData::ITEM_SHIELD:
		case CItemData::ITEM_ACCESSORY:
			{
				for( SBYTE sbOption = 0; sbOption < MAX_ITEM_OPTION; sbOption++ )
				{
					if( sbOptionType[sbOption] == -1 || sbOptionLevel[sbOption] == 0 )
						break;

					COptionData	&odItem = _pNetwork->GetOptionData( sbOptionType[sbOption] );
					strTemp.PrintF( "%s : %d", odItem.GetName(), odItem.GetValue( sbOptionLevel[sbOption] - 1 ) );
					AddItemInfoString( strTemp, 0x94B7C6FF );
				}
			}
			break;
		}

		// Description
		const char	*pDesc = _pNetwork->GetItemDesc( nIndex );
		if( pDesc[0] != NULL )
		{
			strTemp.PrintF( "%s", pDesc );
			AddItemInfoString( strTemp, 0x9E9684FF );
		}

		nInfoWidth = 27 - _pUIFontTexMgr->GetFontSpacing() + MAX_ITEMINFO_CHAR *
						( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
		nInfoHeight = 19 - _pUIFontTexMgr->GetLineSpacing() + m_nCurInfoLines * _pUIFontTexMgr->GetLineHeight();
	}
	else 
	{

		if(g_iCountry == THAILAND) {
			nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + FindThaiLen(m_strItemInfo[0]);				
		} else
		nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + m_strItemInfo[0].Length() *
						( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
		nInfoHeight = 30;
	}

	return TRUE;
}

// ========================================================================= //
//                             Command functions                             //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : !!SetMixItem()
// Desc :
// ----------------------------------------------------------------------------
void CUIMix::SetMixItem ( int nSlotIndex )
{
	// If this is wearing item
	if( _pUIMgr->GetHoldBtn().GetItemWearType() >= 0 )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 554, "착용한 아이템은 조합이 불가능 합니다." ), SYSMSG_ERROR );	
		_pUIMgr->ResetHoldBtn();
		return;
	}

	// If this is not weapon or armor
	int	nTab = _pUIMgr->GetHoldBtn().GetItemTab();
	int	nRow = _pUIMgr->GetHoldBtn().GetItemRow();
	int	nCol = _pUIMgr->GetHoldBtn().GetItemCol();
	CItems		&rItems = _pNetwork->MySlotItem[nTab][nRow][nCol];
	CItemData	&rItemData = rItems.ItemData;
	
	// 악세서리 아이템인지?
	if( rItemData.GetType() != CItemData::ITEM_ACCESSORY )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 555, "악세사리만 조합이 가능 합니다." ), SYSMSG_ERROR );
		_pUIMgr->ResetHoldBtn();
		return;
	}
	
	ResetMixItem();	// 버튼이 들려 있으면 삭제 하고
	
	// 아이템이 현재 사용중인지 확인 한다.
	for ( int i = 0; i < MIX_ITEM_SLOT_COUNT; i++ )
	{
		if ( !m_btnItemSlot[i].IsEmpty () )
		{
			if ( _pUIMgr->GetHoldBtn().GetItemRow() == m_btnItemSlot[i].GetItemRow() && 
				_pUIMgr->GetHoldBtn().GetItemCol() == m_btnItemSlot[i].GetItemCol() )
			{
				_pUIMgr->GetChatting()->AddSysMessage( _S( 556, "이미 사용중인 아이템 입니다." ), SYSMSG_ERROR );	
				_pUIMgr->ResetHoldBtn();
				return;
			}
		}
	}

	//!! Insert upgrade slot
	if ( m_nCurItemCount < MIX_ITEM_SLOT_COUNT )
	{	
		// 비어 있는 슬롯을 찾는다.
		if ( nSlotIndex == -1 )
		{
			for ( int i = 0; i < MIX_ITEM_SLOT_COUNT; i++ )
			{
				if ( m_btnItemSlot[i].IsEmpty() )
				{
					nSlotIndex = i;
					break;
				}
			}
		}
		
		// 사용중이 아니라면 입력 해 준다.
		if ( m_btnItemSlot[nSlotIndex].IsEmpty() )
		{
			m_nCurItemCount++;	
		}

		m_btnItemSlot[nSlotIndex].Copy( _pUIMgr->GetHoldBtn() );
		
	}
	
	if ( m_nCurItemCount >= MIX_ITEM_SLOT_COUNT )
	{
		m_btnOK.SetEnable ( TRUE );
	}
	else 
	{
		m_btnOK.SetEnable ( FALSE );
	}
	
	_pUIMgr->ResetHoldBtn();

	// Lock arrange of inventory
	_pUIMgr->GetInventory()->Lock( TRUE, TRUE, LOCK_MIX );
}

// ----------------------------------------------------------------------------
// Name : !!ResetMixItem()
// Desc :
// ----------------------------------------------------------------------------
void CUIMix::ResetMixItem ()
{
	if ( m_nCurItemSlot != -1 )
	{
		m_btnItemSlot[m_nCurItemSlot].SetEmpty ( TRUE );
		m_nCurItemSlot = -1;
		m_nCurItemCount--;
		m_btnOK.SetEnable ( FALSE );
	}
}

// ----------------------------------------------------------------------------
// Name : NearPosition()
// Desc : 현재 포인터와 가장 가까운 아이템 슬롯의 포인터를 리턴한다.
// ----------------------------------------------------------------------------
int	CUIMix::NearPosition ( int nX, int nY )
{
	ConvertToWindow( nX, nY );
	
	int nNearPoint = 0xffff;
	int nItemSlotX, nItemSlotY;

	int nDest;
	int nIndex = 0;

	for ( int i = 0; i < MIX_ITEM_SLOT_COUNT; i++ )
	{
		nItemSlotX = m_rcItemSlot[i].Left + m_rcItemSlot[i].GetWidth() / 2;
		nItemSlotY = m_rcItemSlot[i].Top + m_rcItemSlot[i].GetHeight() / 2;
			
		nDest = abs ( nX - nItemSlotX ) + abs ( nY - nItemSlotY );
		
		if ( nNearPoint > nDest )
		{
			nIndex = i;
			nNearPoint = nDest;
		}
	}
	
	return nIndex;
}

// ========================================================================= //
//                           Send message functions                          //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : SendMixReq()
// Desc :
// ----------------------------------------------------------------------------
void CUIMix::SendMixReq()
{
	if( m_bWaitMixResult )
		return;

	if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsSkilling() )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 937, "스킬 사용중에는 재활의 상자를 조합할 수 없습니다." ), SYSMSG_ERROR );		
		return;
	}

	if( _pUIMgr->IsCSFlagOn( CSF_TELEPORT ) )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 938, "순간 이동중에는 재활의 상자를 조합할 수 없습니다." ), SYSMSG_ERROR );	
		return;
	}

	SBYTE	sbRow[MIX_ITEM_SLOT_COUNT];
	SBYTE	sbCol[MIX_ITEM_SLOT_COUNT];
	
	for ( int i = 0; i < MIX_ITEM_SLOT_COUNT; i++ )
	{
		sbRow[i] = m_btnItemSlot[i].GetItemRow();
		sbCol[i] = m_btnItemSlot[i].GetItemCol();
	}

	_pNetwork->MixReq( m_nTextRow, m_nTextCol, sbRow, sbCol );
	m_btnOK.SetEnable ( FALSE );
	m_bWaitMixResult = TRUE;

	// Lock inventory
	_pUIMgr->GetInventory()->Lock( TRUE, FALSE, LOCK_MIX );
}

// ========================================================================= //
//                         Receive message functions                         // 
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : !!MixRep()
// Desc :
// ----------------------------------------------------------------------------
void CUIMix::MixRep( SBYTE sbResult )
{
	// Close message box
	_pUIMgr->CloseMessageBox( MSGCMD_MIX_REP );

	// Show result
	CTString	strMessage;
	switch( sbResult )
	{
	case MSG_MIX_FAIL_SYSTEM: // 있어서는 안되는 오류
		strMessage = _S( 557, "조합에 실패 하였습니다." );			
		break;

	case MSG_MIX_FAIL_REMAKEBOX : // 재활의 상자 정보 오류
		strMessage =  _S( 557, "조합에 실패 하였습니다." );			
		break;

	case MSG_MIX_FAIL_ACCESSORY: // 보낸 액세서리 정보 오류
		strMessage =  _S( 557, "조합에 실패 하였습니다." );			
		break;

	case MSG_MIX_SUCCESS :
		strMessage = _S( 558, "조합에 성공 하였습니다." );			
		break;
	}

	CloseMix ();
	
	m_bWaitMixResult = FALSE;

//	if ( strMessage.Length() )
	{
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 552, "재활의 상자" ), UMBS_OK, UI_MIX, MSGCMD_MIX_REP ); 
		MsgBoxInfo.AddString( strMessage );
		_pUIMgr->CreateMessageBox( MsgBoxInfo );
	}	
}

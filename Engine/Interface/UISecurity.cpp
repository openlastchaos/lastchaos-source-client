#include "stdh.h"
#include <vector>
#include <Engine/Interface/UISecurity.h>
#include <Engine/Interface/UIInternalClasses.h>

#define MIN_PASSWORD				(6)
#define MAX_PASSWORD				(8)
#define MIN_UNLOCK_NUM				(7)
#define MAX_UNLOCK_NUM				(11)
#define MAX_NUMBER					(7)
#define	MAX_SECURITY_DESC_CHAR			28

//define String // Date : 2005-07-05(오후 3:28:09), By Lee Ki-hwan
#define ST_PASSWORD_ERROR	_S( 1777, "암호 설정 오류" )		
#define ST_SECURE_INPUT		_S( 1727, "보안 입력" )		
#define ST_PASSWORD_RANGE 	_S( 1729, "8자(영문 숫자조합).") 

extern INDEX g_iCountry;

// ----------------------------------------------------------------------------
// Name : CUISecurity()
// Desc : Constructorreturn WMSG_FAIL;
// ----------------------------------------------------------------------------
CUISecurity::CUISecurity()
{
	m_bHasPassWord		= FALSE;	
}

// ----------------------------------------------------------------------------
// Name : ~CUISecurity()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUISecurity::~CUISecurity()
{
	Destroy();
}


//------------------------------------------------------------------------------
// CUITalk::SetFocus
// Explain:  
// Date : 2005-05-25,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISecurity::SetFocus( BOOL bVisible )
{
	CUIWindow::SetFocus( bVisible );

	m_ebPersonalNumber.SetFocus( TRUE );
	if( !bVisible )
	{
		m_ebPersonalNumber.SetFocus( FALSE );
	}

}


// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUISecurity::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );
	
	// Region of each part
	m_rcTitle.SetRect( 0, 0, 236, 22 );
	
	// Create skill learn texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MessageBox.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();
	
	// UV Coordinate of each part
	// Background
	m_rtBackTop.SetUV( 0, 0, 216, 26, fTexWidth, fTexHeight );
	m_rtBackMiddle1.SetUV( 0, 31, 216, 33, fTexWidth, fTexHeight );
	m_rtBackMiddle2.SetUV( 0, 35, 216, 37, fTexWidth, fTexHeight );
	m_rtBackBottom.SetUV( 0, 38, 216, 45, fTexWidth, fTexHeight );
	
	// Input box
	m_rtInputBoxL.SetUV( 131, 46, 135, 59, fTexWidth, fTexHeight );
	m_rtInputBoxM.SetUV( 137, 46, 139, 59, fTexWidth, fTexHeight );
	m_rtInputBoxR.SetUV( 141, 46, 145, 59, fTexWidth, fTexHeight );
	
	// Close button
	m_btnClose.Create( this, CTString( "" ), 204, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 219, 0, 233, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 234, 0, 248, 14, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );
	
	// OK button
	m_btnOK.Create( this, _S( 191, "확인" ), 98, 188, 63, 21 );
	m_btnOK.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnOK.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnOK.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOK.CopyUV( UBS_IDLE, UBS_DISABLE );
	
	// Cancel button
	m_btnCancel.Create( this, _S( 139, "취소" ), 165, 188, 63, 21 );
	m_btnCancel.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );
	
	// List box of Security description
	m_lbSecurityDesc.Create( this, 8, 28, 210, 110, _pUIFontTexMgr->GetLineHeight(), 13, 3, 1, FALSE );
	m_lbSecurityDesc.CreateScroll( TRUE, 0, 0, 9, 110, 9, 7, 0, 0, 10 );
	m_lbSecurityDesc.SetSelBar( 210, _pUIFontTexMgr->GetLineHeight(), 187, 46, 204, 61, fTexWidth, fTexHeight );
	// Up button
	m_lbSecurityDesc.SetScrollUpUV( UBS_IDLE, 230, 16, 239, 23, fTexWidth, fTexHeight );
	m_lbSecurityDesc.SetScrollUpUV( UBS_CLICK, 240, 16, 249, 23, fTexWidth, fTexHeight );
	m_lbSecurityDesc.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbSecurityDesc.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_lbSecurityDesc.SetScrollDownUV( UBS_IDLE, 230, 24, 239, 31, fTexWidth, fTexHeight );
	m_lbSecurityDesc.SetScrollDownUV( UBS_CLICK, 240, 24, 249, 31, fTexWidth, fTexHeight );
	m_lbSecurityDesc.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbSecurityDesc.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbSecurityDesc.SetScrollBarTopUV( 219, 16, 228, 26, fTexWidth, fTexHeight );
	m_lbSecurityDesc.SetScrollBarMiddleUV( 219, 27, 228, 29, fTexWidth, fTexHeight );
	m_lbSecurityDesc.SetScrollBarBottomUV( 219, 30, 228, 40, fTexWidth, fTexHeight );
	
	int nTempY = m_nPosY + 126;
	// Password edit box
	
	nTempY += 36;
	m_ebPersonalNumber.Create( this, 122, nTempY, 91, 15, MAX_NUMBER, TRUE );
	m_ebPersonalNumber.SetFocus( TRUE );	
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUISecurity::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUISecurity::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : GetSecurityDesc()
// Desc :
// ----------------------------------------------------------------------------
void CUISecurity::GetSecurityDesc( BOOL bShow )
{
	m_lbSecurityDesc.ResetAllStrings();
	
	if( bShow == FALSE )
		return;
	
	// Make description of Security	
	/*
	AddSecurityDescString( _S( 949, "보관소의 암호를 설정하겠습니다." ) );	
	AddSecurityDescString( _S( 950, "계정 암호와 별도로 된 암호를 입력해 주십시오." ) );			
	AddSecurityDescString( CTString( "  " ) );
	AddSecurityDescString( _S( 951, "보관소의 암호를 잊었을 경우 홈페이지의 창고 암호 찾기 절차를 따라서 처리할 수 있습니다." ), 0xBCBCBCFF );		
	*/
	AddSecurityDescString( _S( 1723, "암호를 분실하셨나요?" ) );		
	AddSecurityDescString( _S( 1724, "계정에 등록된 주민등록 번호 뒷자리 7자리를 입력하여 주십시오." ) );		
	AddSecurityDescString( CTString( "  " ) );
	AddSecurityDescString( _S( 1725,  "성공적으로 입력되면 기존 암호가 삭제됩니다."  ) );	
	AddSecurityDescString( _S( 1726,  "암호 삭제 후 암호를 다시 설정하여 주십시오."  ) );	
}

// ----------------------------------------------------------------------------
// Name : AddSecurityDescString()
// Desc :
// ----------------------------------------------------------------------------
void CUISecurity::AddSecurityDescString( CTString &strDesc, COLOR colDesc )
{
	// Get length of string
	INDEX	nLength = strDesc.Length();
	if( nLength == 0 )
		return;
	
	// wooss 051002
	if(g_iCountry == THAILAND){
		// Get length of string
		INDEX	nThaiLen = FindThaiLen(strDesc);
		INDEX	nChatMax= (MAX_SECURITY_DESC_CHAR-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
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
			{
				m_lbSecurityDesc.AddString( 0, strDesc, colDesc );
			}
			else
			{
				// Split string
				CTString	strTemp, strTemp2;
				strDesc.Split( iPos, strTemp2, strTemp );
				m_lbSecurityDesc.AddString( 0, strTemp2, colDesc );
				
				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );
				
				AddSecurityDescString( strTemp, colDesc );
			}
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = MAX_SECURITY_DESC_CHAR;
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
				CTString	strTemp, strTemp2;
				strDesc.Split( nSplitPos, strTemp2, strTemp );
				m_lbSecurityDesc.AddString( 0, strTemp2, colDesc );
				
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
				
				AddSecurityDescString( strTemp, colDesc );
			}
			else
			{
				// Split string
				CTString	strTemp, strTemp2;
				strDesc.Split( iPos, strTemp2, strTemp );
				m_lbSecurityDesc.AddString( 0, strTemp2, colDesc );
				
				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );
				
				AddSecurityDescString( strTemp, colDesc );
			}

		}
		
	} else {
		// If length of string is less than max char
		if( nLength <= MAX_SECURITY_DESC_CHAR )
		{
			// Check line character
			for( int iPos = 0; iPos < nLength; iPos++ )
			{
				if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
					break;
			}
			
			// Not exist
			if( iPos == nLength )
			{
				m_lbSecurityDesc.AddString( 0, strDesc, colDesc );
			}
			else
			{
				// Split string
				CTString	strTemp, strTemp2;
				strDesc.Split( iPos, strTemp2, strTemp );
				m_lbSecurityDesc.AddString( 0, strTemp2, colDesc );
				
				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );
				
				AddSecurityDescString( strTemp, colDesc );
			}
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = MAX_SECURITY_DESC_CHAR;
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
				CTString	strTemp, strTemp2;
				strDesc.Split( nSplitPos, strTemp2, strTemp );
				m_lbSecurityDesc.AddString( 0, strTemp2, colDesc );
				
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
				
				AddSecurityDescString( strTemp, colDesc );
			}
			else
			{
				// Split string
				CTString	strTemp, strTemp2;
				strDesc.Split( iPos, strTemp2, strTemp );
				m_lbSecurityDesc.AddString( 0, strTemp2, colDesc );
				
				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );
				
				AddSecurityDescString( strTemp, colDesc );
			}
		}
	}
}

// ----------------------------------------------------------------------------
// Name : OpenChangePassWord()
// Desc : 암호 변경.
// ----------------------------------------------------------------------------
void CUISecurity::OpenChangePassWord( BOOL bHasPassWord )
{	
	if( IsVisible() )
		return;
	
	ResetSecurity();	
	
	m_bHasPassWord		= bHasPassWord;	
	
	int nSecurity_ChangePW_Height = SECURITY_CHANGEPW_HEIGHT;
	// Set size & position
	if( g_iCountry == TAIWAN || g_iCountry == TAIWAN2 ) 
		nSecurity_ChangePW_Height += 86;

	SetSize( SECURITY_CHANGEPW_WIDTH, nSecurity_ChangePW_Height );
	CDrawPort	*pdp = _pUIMgr->GetDrawPort();
	int	nX = ( pdp->dp_MinI + pdp->dp_MaxI ) / 2 - GetWidth() / 2;
	int	nY = ( pdp->dp_MinJ + pdp->dp_MaxJ ) / 2 - GetHeight() / 2;
	SetPos( nX, nY );
	m_btnOK.SetPos( 98, nSecurity_ChangePW_Height - 26 );
	m_btnCancel.SetPos( 165, nSecurity_ChangePW_Height - 26 );
	
	GetSecurityDesc( TRUE );
	_pUIMgr->RearrangeOrder( UI_SECURITY, TRUE );
}

// ----------------------------------------------------------------------------
// Name : OpenSecurity()
// Desc : 암호를 물어봄.
// ----------------------------------------------------------------------------
void CUISecurity::OpenSecurity( BOOL bHasPassWord )
{
	// 암호가 설정되어 있지 않다면...
	if( !bHasPassWord )
	{
		_pNetwork->SendWareHouseListReq( "" );
		return;
	}
	
	if( IsVisible() )
		return;
	
	ResetSecurity();
	
	m_bHasPassWord		= bHasPassWord;
	
	_pUIMgr->CloseMessageBox( MSGCMD_SECURITY_PASSWORD );
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 1727, "보안 입력" ), UMBS_OK | UMBS_INPUTPASSWORD, UI_SECURITY, MSGCMD_SECURITY_PASSWORD );		
	CTString	strMessage =  _S( 1728, "암호를 입력하여 주십시오." );	
	MsgBoxInfo.AddStringEx( strMessage, 0, 0, 0xF2F2F2FF, TEXT_CENTER );
	strMessage = ST_PASSWORD_RANGE;				
	MsgBoxInfo.AddStringEx( strMessage, 1, 0, 0xF2F2F2FF, TEXT_CENTER );
	_pUIMgr->CreateMessageBox( MsgBoxInfo );
}

// ----------------------------------------------------------------------------
// Name : ResetSecurity()
// Desc : 
// ----------------------------------------------------------------------------
void CUISecurity::ResetSecurity()
{	
	m_ebPersonalNumber.ResetString();	
	m_lbSecurityDesc.ResetAllStrings();
	
	m_strOldPassWord		= "";
	m_strNewPassWord		= "";
	m_strPersonalNumber	= "";	
	
	m_bHasPassWord			= FALSE;
	
	_pUIMgr->RearrangeOrder( UI_SECURITY, FALSE );
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUISecurity::Render()
{
	// Set skill learn texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );
	
	// Add render regions
	// Background
	int nX = m_nPosX;
	int nY = m_nPosY;
	_pUIMgr->GetDrawPort()->AddTexture( nX, m_nPosY, nX + m_nWidth, nY + 26,
		m_rtBackTop.U0, m_rtBackTop.V0, 
		m_rtBackTop.U1, m_rtBackTop.V1,
		0xFFFFFFFF );
	
	nY += 26;	
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + m_nWidth, nY + 123,
		m_rtBackMiddle1.U0, m_rtBackMiddle1.V0, 
		m_rtBackMiddle1.U1, m_rtBackMiddle1.V1,
		0xFFFFFFFF );
	
	nY += 123;
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + m_nWidth, nY + 9,
		m_rtBackMiddle2.U0, m_rtBackMiddle2.V0, 
		m_rtBackMiddle2.U1, m_rtBackMiddle2.V1,
		0xFFFFFFFF );
	
	nY += 9;
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + m_nWidth, nY + 24,
		m_rtBackMiddle1.U0, m_rtBackMiddle1.V0, 
		m_rtBackMiddle1.U1, m_rtBackMiddle1.V1,
		0xFFFFFFFF );
	
	nY += 24;
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + m_nWidth, m_nPosY + m_nHeight - 7,
		m_rtBackMiddle2.U0, m_rtBackMiddle2.V0, 
		m_rtBackMiddle2.U1, m_rtBackMiddle2.V1,
		0xFFFFFFFF );	
	
	
	_pUIMgr->GetDrawPort()->AddTexture( nX, m_nPosY + m_nHeight - 7, nX + m_nWidth, m_nPosY + m_nHeight,
		m_rtBackBottom.U0, m_rtBackBottom.V0, 
		m_rtBackBottom.U1, m_rtBackBottom.V1,
		0xFFFFFFFF );
	
	nX = m_nPosX + 117;
	nY = m_nPosY + 162;
	int nX2 = m_nPosX + m_nWidth - 20;	
	int nY2 = nY + 16;
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + 4, nY2,
		m_rtInputBoxL.U0, m_rtInputBoxL.V0, m_rtInputBoxL.U1, m_rtInputBoxL.V1,
		0xFFFFFFFF );
	// Lower middle
	_pUIMgr->GetDrawPort()->AddTexture( nX + 4, nY, nX2 - 4, nY2,
		m_rtInputBoxM.U0, m_rtInputBoxM.V0, m_rtInputBoxM.U1, m_rtInputBoxM.V1,
		0xFFFFFFFF );
	// Lower right
	_pUIMgr->GetDrawPort()->AddTexture( nX2 - 4, nY, nX2, nY2,
		m_rtInputBoxR.U0, m_rtInputBoxR.V0, m_rtInputBoxR.U1, m_rtInputBoxR.V1,
		0xFFFFFFFF );
	
	// Close button
	m_btnClose.Render();
	
	// Learn button
	m_btnOK.Render();
	
	// Cancel button
	m_btnCancel.Render();	
	
	m_lbSecurityDesc.Render();
	
	// Password Edit Box		
	m_ebPersonalNumber.Render();			
	
	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();	
	
	// Text in skill learn
	//_pUIMgr->GetDrawPort()->PutTextEx( _S( 952, "보관소 암호 변경 설정" ), m_nPosX + SECURITY_TITLE_TEXT_OFFSETX,	
	_pUIMgr->GetDrawPort()->PutTextEx( _S( 1730, "창고 암호 삭제" ), m_nPosX + SECURITY_TITLE_TEXT_OFFSETX,	
		m_nPosY + SECURITY_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );
	
	nY = m_nPosY + 164;
	_pUIMgr->GetDrawPort()->PutTextEx( _S( 1731, "주민등록번호" ), m_nPosX + SECURITY_TITLE_TEXT_OFFSETX - 3,		
		nY, 0xFFFFFFFF );
	
	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : PressOKBtn()
// Desc :
// ----------------------------------------------------------------------------
void CUISecurity::PressOKBtn()
{
	// 암호 삭제시...
	if( m_ebPersonalNumber.GetString() )
	{		
		m_strPersonalNumber = m_ebPersonalNumber.GetString();
		int tv_len = 7;
		if( g_iCountry == TAIWAN ||g_iCountry == TAIWAN2 ) tv_len = 13;
		
		if( m_strPersonalNumber.Length() < tv_len )
		{
			m_ebPersonalNumber.ResetString();
			_pUIMgr->CloseMessageBox( MSGCMD_SECURITY_NOTIFY );
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 1732, "암호 삭제 오류" ) , UMBS_OK, UI_SECURITY, MSGCMD_SECURITY_NOTIFY );	
			CTString	strMessage = _S( 1733, "주민등록번호를 확인후 다시 시도하여 주십시오." );	
			MsgBoxInfo.AddString( strMessage );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );	
			_pUIMgr->GetSecurity()->ResetSecurity();
			return;
		}
	}		
	
	_pNetwork->SendWareHouseDeletePassWord( m_strPersonalNumber );		
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT	CUISecurity::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;
	
	// Title bar
	static BOOL	bTitleBarClick = FALSE;
	
	// Item clicked
	static BOOL	bLButtonDownInItem = FALSE;
	
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
				_pUIMgr->SetMouseCursorInsideUIs();
			
			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;
			
			// Move shop
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;
				
				Move( ndX, ndY );
				
				return WMSG_SUCCESS;
			}
			
			// Close Button
			if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// OK Button
			else if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Cancel Button
			else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;			
			// List box
			else if( m_lbSecurityDesc.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;			
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
				// OK Button
				else if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				
				// Cancel Button
				else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				
				// Confirm Password
				else if( m_ebPersonalNumber.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing						
					m_ebPersonalNumber.SetFocus(TRUE);
					_pUIMgr->RearrangeOrder( UI_SECURITY, TRUE );
					return WMSG_SUCCESS;
				}
				// List box
				else if( m_lbSecurityDesc.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;				
				
				_pUIMgr->RearrangeOrder( UI_SECURITY, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			bTitleBarClick = FALSE;
			
			// Close button
			if( (wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if(wmsgResult == WMSG_COMMAND)
				{
					ResetSecurity();
				}
				return WMSG_SUCCESS;
			}
			// OK Button
			else if( (wmsgResult = m_btnOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if(wmsgResult == WMSG_COMMAND)
				{
					PressOKBtn();
				}
				return WMSG_SUCCESS;
			}
			// Cancel Button
			else if( (wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if(wmsgResult == WMSG_COMMAND)
				{
					ResetSecurity();
				}
				return WMSG_SUCCESS;
			}			
			// List box
			else if( m_lbSecurityDesc.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;			
		}
		break;
	}
	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : KeyMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT	CUISecurity::KeyMessage( MSG *pMsg )
{	
	if( !IsFocused() )
		return WMSG_FAIL;
	
	if( pMsg->wParam == VK_RETURN )
	{
		PressOKBtn();
		return WMSG_SUCCESS;
	}
	else if( m_ebPersonalNumber.KeyMessage( pMsg ) != WMSG_FAIL )
	{
		return WMSG_SUCCESS;
	}	
	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : IMEMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT	CUISecurity::IMEMessage( MSG *pMsg )
{	
	if( m_ebPersonalNumber.IMEMessage( pMsg ) != WMSG_FAIL )
	{
		return WMSG_SUCCESS;
	}
	
	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : CharMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT	CUISecurity::CharMessage( MSG *pMsg )
{	
	if( m_ebPersonalNumber.CharMessage( pMsg ) != WMSG_FAIL )
	{
		return WMSG_SUCCESS;
	}	
	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUISecurity::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput , CTString &strConfirm)
{
	if( !bOK )
		return;
	
	switch( nCommandCode )
	{
	case MSGCMD_NEW_PASSWORD:
		{
			if( strInput.Length() < MIN_PASSWORD || strInput.Length() > MAX_PASSWORD)
			{	
				_pUIMgr->CloseMessageBox( MSGCMD_SECURITY_NOTIFY );
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo(  ST_PASSWORD_ERROR , UMBS_OK, UI_SECURITY, MSGCMD_SECURITY_NOTIFY );
				CTString	strMessage = _S( 1734, "암호는 6자 이상 8자 이하로 하셔야 합니다." );	
				MsgBoxInfo.AddString( strMessage );
				_pUIMgr->CreateMessageBox( MsgBoxInfo );			
				return;
			}
			if(strInput.IsEqualCaseSensitive(strConfirm)){
				_pUIMgr->CloseMessageBox( MSGCMD_PASSWORD_UNLOCK_A );
				m_strNewPassWord=strConfirm;
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( ST_SECURE_INPUT, UMBS_OK | UMBS_INPUTPASSWORD |UMBS_CONFIRMPASSWORD, UI_SECURITY, MSGCMD_PASSWORD_UNLOCK_A );
				CTString	strMessage =  _S(2270, "잠금 해제 번호를 입력해 주십시오." );	
				MsgBoxInfo.AddStringEx(strMessage, 0, 0, 0xF2F2F2FF, TEXT_CENTER );
				strMessage = _S(2271,"12자 영문 숫자 조합");				
				MsgBoxInfo.AddStringEx( strMessage, 1, 0, 0xF2F2F2FF, TEXT_CENTER );
				_pUIMgr->CreateMessageBox( MsgBoxInfo );

				CUIMessageBox* pMsgBox = _pUIMgr->GetMessageBox( MSGCMD_PASSWORD_UNLOCK_A );						
				ASSERT( pMsgBox != NULL && "Invalid Message Box!!!" );
				pMsgBox->GetInputBox().SetMaxChar(MAX_UNLOCK_NUM);
				pMsgBox->GetConfirmBox().SetMaxChar(MAX_UNLOCK_NUM);
			} else {
				_pUIMgr->CloseMessageBox( MSGCMD_SECURITY_NOTIFY );
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo(  ST_PASSWORD_ERROR , UMBS_OK, UI_SECURITY, MSGCMD_SECURITY_NOTIFY );
				CTString	strMessage = _S(2272, "입력 번호가 일치 하지 않습니다." );	
				MsgBoxInfo.AddString( strMessage );
				_pUIMgr->CreateMessageBox( MsgBoxInfo );			
			}

		}
		break;

	case MSGCMD_PASSWORD_UNLOCK_A:
		{
			if( strInput.Length() < MIN_UNLOCK_NUM || strInput.Length() > MAX_UNLOCK_NUM)
			{	
				_pUIMgr->CloseMessageBox( MSGCMD_SECURITY_NOTIFY );
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo(  ST_PASSWORD_ERROR , UMBS_OK, UI_SECURITY, MSGCMD_SECURITY_NOTIFY );
				CTString	strMessage = _S(2273, "해제 번호는 8자 이상 12자 이하로 하셔야 합니다." );	
				MsgBoxInfo.AddString( strMessage );
				_pUIMgr->CreateMessageBox( MsgBoxInfo );			
				return;
			}

			if(strInput.IsEqualCaseSensitive(strConfirm)){
				
				_pNetwork->SendWareHouseSetupPassWord(m_strNewPassWord,strConfirm);
				
			} else {
				_pUIMgr->CloseMessageBox( MSGCMD_SECURITY_NOTIFY );
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo(  ST_PASSWORD_ERROR , UMBS_OK, UI_SECURITY, MSGCMD_SECURITY_NOTIFY );
				CTString	strMessage = _S(2272, "입력 번호가 일치 하지 않습니다." );	
				MsgBoxInfo.AddString( strMessage );
				_pUIMgr->CreateMessageBox( MsgBoxInfo );			
			}
			

		}
		break;

	}


}

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUISecurity::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	if( !bOK )
		return;
	
	switch( nCommandCode )
	{	
	case MSGCMD_OLD_PASSWORD:
		{
			if( strInput.Length() < MIN_PASSWORD || strInput.Length() > MAX_PASSWORD)
			{	
				_pUIMgr->CloseMessageBox( MSGCMD_SECURITY_NOTIFY );
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo(  ST_PASSWORD_ERROR , UMBS_OK, UI_SECURITY, MSGCMD_SECURITY_NOTIFY );
				CTString	strMessage = _S( 1734, "암호는 6자 이상 8자 이하로 하셔야 합니다." );	
				MsgBoxInfo.AddString( strMessage );
				_pUIMgr->CreateMessageBox( MsgBoxInfo );			
				return;
			}
			
			m_strOldPassWord = strInput;

			_pNetwork->SendWareHouseCheckPassWord(m_strOldPassWord);
			
		}
		break;
	case MSGCMD_NEW_PASSWORD:
		{
			if( strInput.Length() < MIN_PASSWORD || strInput.Length() > MAX_PASSWORD)
			{	
				_pUIMgr->CloseMessageBox( MSGCMD_SECURITY_NOTIFY );
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo(  ST_PASSWORD_ERROR , UMBS_OK, UI_SECURITY, MSGCMD_SECURITY_NOTIFY );
				CTString	strMessage = _S( 1734, "암호는 6자 이상 8자 이하로 하셔야 합니다." );	
				MsgBoxInfo.AddString( strMessage );
				_pUIMgr->CreateMessageBox( MsgBoxInfo );	
				return;
			}

			m_strNewPassWord = strInput;

			_pUIMgr->CloseMessageBox( MSGCMD_CONFIRM_PASSWORD );
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( ST_SECURE_INPUT, UMBS_OK | UMBS_INPUTPASSWORD, UI_SECURITY, MSGCMD_CONFIRM_PASSWORD );
			CTString	strMessage =  _S( 1735, "새 암호를 확인 합니다." );	
			MsgBoxInfo.AddStringEx( strMessage, 0, 0, 0xF2F2F2FF, TEXT_CENTER );
			strMessage = ST_PASSWORD_RANGE;			
			MsgBoxInfo.AddStringEx( strMessage, 1, 0, 0xF2F2F2FF, TEXT_CENTER );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );
			
			CUIMessageBox* pMsgBox = _pUIMgr->GetMessageBox( MSGCMD_CONFIRM_PASSWORD );						
			ASSERT( pMsgBox != NULL && "Invalid Message Box!!!" );
			pMsgBox->GetInputBox().SetMaxChar(MAX_PASSWORD);
		}
		break;
	case MSGCMD_CONFIRM_PASSWORD:
		{
			if( strInput.Length() < MIN_PASSWORD || strInput.Length() > MAX_PASSWORD)
			{
				_pUIMgr->CloseMessageBox( MSGCMD_SECURITY_NOTIFY );
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( ST_PASSWORD_ERROR, UMBS_OK, UI_SECURITY, MSGCMD_SECURITY_NOTIFY );
				CTString	strMessage = _S( 1734, "암호는 6자 이상 8자 이하로 하셔야 합니다." );	
				MsgBoxInfo.AddString( strMessage );
				_pUIMgr->CreateMessageBox( MsgBoxInfo );	
				return;
			}

			if( m_strNewPassWord != strInput)
			{
				_pUIMgr->CloseMessageBox( MSGCMD_SECURITY_NOTIFY );
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( ST_PASSWORD_ERROR, UMBS_OK, UI_SECURITY, MSGCMD_SECURITY_NOTIFY );		
				MsgBoxInfo.AddString( _S( 853, "암호가 올바르지 않습니다." ) );
				_pUIMgr->CreateMessageBox( MsgBoxInfo );
				return;
			}

			m_strConfirmPassWord = strInput;

			// 서버로 메세지 보내기...
			_pNetwork->SendWareHouseSetupPassWord(m_strConfirmPassWord);
		}
		break;
	case MSGCMD_SECURITY_NOTIFY:
		{
		}
		break;
	case MSGCMD_SECURITY_PASSWORD:
		{	
			if( strInput.Length() < MIN_PASSWORD || strInput.Length() > MAX_PASSWORD)
			{	
				_pUIMgr->CloseMessageBox( MSGCMD_SECURITY_NOTIFY );
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo(  _S( 1736, "암호 확인 오류" ) , UMBS_OK, UI_SECURITY, MSGCMD_SECURITY_NOTIFY );	
				MsgBoxInfo.AddString( _S( 853, "암호가 올바르지 않습니다." ) );
				_pUIMgr->CreateMessageBox( MsgBoxInfo );
				return;
			}
			// 암호 확인을 보내고 응답으로 아이템 목록을 받음.
			_pNetwork->SendWareHouseListReq( strInput );			
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : MsgBoxLCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUISecurity::MsgBoxLCommand( int nCommandCode, int nResult )
{
	switch( nCommandCode )
	{
	case MSGLCMD_SECURITY_REQ:
		{
			switch ( nResult ) 
			{
			case SET_PASSWORD:
				{
					// 암호가 이미 설정되어있는 경우.
					if(_pUIMgr->GetWareHouse()->HasPassword())
					{
						_pUIMgr->CloseMessageBox( MSGCMD_OLD_PASSWORD );
						CUIMsgBox_Info	MsgBoxInfo;
						MsgBoxInfo.SetMsgBoxInfo( ST_SECURE_INPUT, UMBS_OK | UMBS_INPUTPASSWORD, UI_SECURITY, MSGCMD_OLD_PASSWORD );
						CTString	strMessage =  _S( 1737, "기존 암호를 입력하여 주십시오." );	
						MsgBoxInfo.AddStringEx( strMessage, 0, 0, 0xF2F2F2FF, TEXT_CENTER );
						strMessage = ST_PASSWORD_RANGE;				
						MsgBoxInfo.AddStringEx( strMessage, 1, 0, 0xF2F2F2FF, TEXT_CENTER );
						_pUIMgr->CreateMessageBox( MsgBoxInfo );
						
						CUIMessageBox* pMsgBox = _pUIMgr->GetMessageBox( MSGCMD_OLD_PASSWORD );						
						ASSERT( pMsgBox != NULL && "Invalid Message Box!!!" );
						pMsgBox->GetInputBox().SetMaxChar(MAX_PASSWORD);
					}
					// 암호가 없는 경우...
					else
					{
						_pUIMgr->CloseMessageBox( MSGCMD_NEW_PASSWORD );
						CUIMsgBox_Info	MsgBoxInfo;
						if(g_iCountry == TAIWAN||g_iCountry == TAIWAN2) 
							MsgBoxInfo.SetMsgBoxInfo( ST_SECURE_INPUT, UMBS_OK | UMBS_INPUTPASSWORD |UMBS_CONFIRMPASSWORD, UI_SECURITY, MSGCMD_NEW_PASSWORD );
						else 
							MsgBoxInfo.SetMsgBoxInfo( ST_SECURE_INPUT, UMBS_OK | UMBS_INPUTPASSWORD , UI_SECURITY, MSGCMD_NEW_PASSWORD );
						CTString	strMessage =  _S( 1738, "새 암호를 입력하여 주십시오." );	
						MsgBoxInfo.AddStringEx( strMessage, 0, 0, 0xF2F2F2FF, TEXT_CENTER );
						strMessage = ST_PASSWORD_RANGE;				
						MsgBoxInfo.AddStringEx( strMessage, 1, 0, 0xF2F2F2FF, TEXT_CENTER );
						_pUIMgr->CreateMessageBox( MsgBoxInfo );

						CUIMessageBox* pMsgBox = _pUIMgr->GetMessageBox( MSGCMD_NEW_PASSWORD );						
						ASSERT( pMsgBox != NULL && "Invalid Message Box!!!" );
						if(g_iCountry == TAIWAN||g_iCountry == TAIWAN2){
							pMsgBox->GetInputBox().SetMaxChar(MAX_PASSWORD);
							pMsgBox->GetConfirmBox().SetMaxChar(MAX_PASSWORD);
						}
						else  pMsgBox->GetInputBox().SetMaxChar(MAX_PASSWORD);
					}
				}
				break;
				
			case UNSET_PASSWORD:
				{
					_pUIMgr->SetCSFlagOff( CSF_WAREHOUSE );
					_pUIMgr->GetSecurity()->OpenChangePassWord( _pUIMgr->GetWareHouse()->HasPassword() );
				}
				break;
			default:
				{
					// Character state flags
					_pUIMgr->SetCSFlagOff( CSF_WAREHOUSE );
				}
				break;
			}
			break;
		}
	}
}


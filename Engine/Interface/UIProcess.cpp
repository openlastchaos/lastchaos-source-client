#include "stdh.h"
#include <Engine/Interface/UIProcess.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Entities/Items.h>
#include <Engine/Interface/UISkillLearn.h>
#include <Engine/Interface/UIInventory.h>

extern INDEX g_iCountry;
// Date : 2005-03-07,   By Lee Ki-hwan
static int	_iMaxMsgStringChar = 0;

// ----------------------------------------------------------------------------
// Name : CUIProcess()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIProcess::CUIProcess()
{
	Clear ();
}

// ----------------------------------------------------------------------------
// Name : ~CUIProcess()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIProcess::~CUIProcess()
{
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcess::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	int diff = SKILLLEARN_HEIGHT - PROCESS_HEIGHT;

	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	_iMaxMsgStringChar = PROCESS_STRING_CHAR_WIDTH / ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 216, 22 );
	m_rcIcons.SetRect( 17, 55, 51, 200 );
	m_rcTab.SetRect( 21, 31, 204, 50 );

	m_rcItem.SetRect ( 13, 33, 218, 203 );
	m_rcDesc.SetRect ( 13, 203, 218, 331 );

	// Process Texture 생성 - SkillLearn Texture 사용
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\SkillLearn.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	m_rtBackgroundTop.SetUV( 0, 0, 216, PROCESS_TOP_HEIGHT, fTexWidth, fTexHeight );
	m_rtBackgroundBtm.SetUV( 0, PROCESS_TOP_HEIGHT + diff, 216, SKILLLEARN_HEIGHT, fTexWidth, fTexHeight );

	m_rtSelOutline.SetUV( 218, 80, 250, 112, fTexWidth, fTexHeight );

// Item information region
	m_rtInfoUL.SetUV( 218, 113, 225, 120, fTexWidth, fTexHeight );
	m_rtInfoUM.SetUV( 228, 113, 230, 120, fTexWidth, fTexHeight );
	m_rtInfoUR.SetUV( 233, 113, 240, 120, fTexWidth, fTexHeight );
	m_rtInfoML.SetUV( 218, 123, 225, 125, fTexWidth, fTexHeight );
	m_rtInfoMM.SetUV( 228, 123, 230, 125, fTexWidth, fTexHeight );
	m_rtInfoMR.SetUV( 233, 123, 240, 125, fTexWidth, fTexHeight );
	m_rtInfoLL.SetUV( 218, 128, 225, 135, fTexWidth, fTexHeight );
	m_rtInfoLM.SetUV( 228, 128, 230, 135, fTexWidth, fTexHeight );
	m_rtInfoLR.SetUV( 233, 128, 240, 135, fTexWidth, fTexHeight );


	// Close button
	m_btnClose.Create( this, CTString( "" ), 184, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 219, 0, 233, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 219, 15, 233, 29, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Process button
	m_btnOK.Create( this, _S( 191, "확인" ), 70, 372-diff, 63, 21 );			
	m_btnOK.SetUV( UBS_IDLE, 0, 403, 63, 424, fTexWidth, fTexHeight );
	m_btnOK.SetUV( UBS_CLICK, 66, 403, 129, 424, fTexWidth, fTexHeight );
	m_btnOK.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOK.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Cancel button
	m_btnCancel.Create( this, _S( 139, "취소" ), 141, 372-diff, 63, 21 );
	m_btnCancel.SetUV( UBS_IDLE, 0, 403, 63, 424, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 66, 403, 129, 424, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Scroll bar of Process Item
	m_sbProcessItem.Create( this, 193, 51, 9, 153 );
	m_sbProcessItem.CreateButtons( TRUE, 9, 7, 0, 0, 10 );
	m_sbProcessItem.SetScrollPos( 0 );
	m_sbProcessItem.SetScrollRange( PROCESS_SLOT_ROW_TOTAL );
	m_sbProcessItem.SetCurItemCount( 0 );
	m_sbProcessItem.SetItemsPerPage( PROCESS_SLOT_ROW );	
	// Up button
	m_sbProcessItem.SetUpUV( UBS_IDLE, 219, 62, 228, 69, fTexWidth, fTexHeight );
	m_sbProcessItem.SetUpUV( UBS_CLICK, 230, 62, 239, 69, fTexWidth, fTexHeight );
	m_sbProcessItem.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbProcessItem.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbProcessItem.SetBarTopUV( 219, 32, 228, 42, fTexWidth, fTexHeight );
	m_sbProcessItem.SetBarMiddleUV( 219, 43, 228, 45, fTexWidth, fTexHeight );
	m_sbProcessItem.SetBarBottomUV( 219, 46, 228, 56, fTexWidth, fTexHeight );
	// Down button
	m_sbProcessItem.SetDownUV( UBS_IDLE, 219, 71, 228, 78, fTexWidth, fTexHeight );
	m_sbProcessItem.SetDownUV( UBS_CLICK, 230, 71, 239, 78, fTexWidth, fTexHeight );
	m_sbProcessItem.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbProcessItem.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Wheel region
	m_sbProcessItem.SetWheelRect( -181, -1, 180, 154 );


	// List box of Precondition description
	m_lbPreconditionDesc.Create( this, 13, 207, 180, 124, _pUIFontTexMgr->GetLineHeight(), 7, 6, 1, FALSE );
	m_lbPreconditionDesc.CreateScroll( TRUE, 0, 0, 9, 124, 9, 7, 0, 0, 10 );
	
	// Up button
	m_lbPreconditionDesc.SetScrollUpUV( UBS_IDLE, 219, 62, 228, 69, fTexWidth, fTexHeight );
	m_lbPreconditionDesc.SetScrollUpUV( UBS_CLICK, 230, 62, 239, 69, fTexWidth, fTexHeight );
	m_lbPreconditionDesc.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbPreconditionDesc.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbPreconditionDesc.SetScrollBarTopUV( 219, 32, 228, 42, fTexWidth, fTexHeight );
	m_lbPreconditionDesc.SetScrollBarMiddleUV( 219, 43, 228, 45, fTexWidth, fTexHeight );
	m_lbPreconditionDesc.SetScrollBarBottomUV( 219, 46, 228, 56, fTexWidth, fTexHeight );
	// Down button
	m_lbPreconditionDesc.SetScrollDownUV( UBS_IDLE, 219, 71, 228, 78, fTexWidth, fTexHeight );
	m_lbPreconditionDesc.SetScrollDownUV( UBS_CLICK, 230, 71, 239, 78, fTexWidth, fTexHeight );
	m_lbPreconditionDesc.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbPreconditionDesc.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );

	m_bSatisfied = FALSE;

	m_bShowItemInfo = false;
	m_nCurInfoLines = 0;
	m_bDetailItemInfo = TRUE;
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcess::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcess::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : Clear()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcess::Clear()
{
	m_nSelectProcessItem	= -1;
	m_bWaitProcessResult	= TRUE;
	m_bSatisfied			= FALSE;

	m_btnProcessItems.size();

	m_nProcessText			= -1;				
	m_nRow					= -1;
	m_nCol					= -1;
	
	m_nNeedItemCount		= 0;

	memset ( m_NeedItems, 0, sizeof ( CNeedItems ) * MAX_MAKE_ITEM_MATERIAL );

	m_StrProcessType.Clear ();

	m_bWaitProcessResult = FALSE;
	m_lbPreconditionDesc.ResetAllStrings();

	for(std::vector<CUIButtonEx>::iterator pos = m_btnProcessItems.begin(); pos != m_btnProcessItems.end(); ++pos)
	{
		(*pos).InitBtn();
	}

	m_btnProcessItems.clear ();
}

// ----------------------------------------------------------------------------
// Name : InitProcess()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcess::InitProcess()
{	
	m_btnProcessItems.clear ();
	CItemData &TextItemData = _pNetwork->GetItemData ( m_nProcessText );
	const char* szItemName	= _pNetwork->GetItemName( m_nProcessText );
	
	// 임시 처리 
	int nProcessType = TextItemData.GetProcessType();
	int nProcessSubType = TextItemData.GetProcessSubType();
	
	// 제조  Type 설정
	m_StrProcessType = szItemName;

	CUIButtonEx UIButtonEx;
	int bit = 0x00000001;

	//!! 제조 문서로 제조 할수 있는 아이템 검색 
	for( int nIndex = 0; nIndex <= _pNetwork->wo_iNumOfItem; nIndex++ )
	{
		CItemData& ItemData = _pNetwork->GetItemData ( nIndex );

		if( ItemData.GetItemIndex() == -1 ) continue;
		
		if ( ItemData.GetFlag() & ITEM_FLAG_MADE )
		{
			if ( ItemData.GetType () == CItemData::ITEM_ETC && 
				ItemData.GetSubType () == CItemData::ITEM_ETC_PROCESS && // 가공품 이라는 얘기
				ItemData.GetProcessType () == nProcessType 
				&& ItemData.GetProcessSubType () == nProcessSubType 
				)			
			{
				UIButtonEx.Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_PROCESS ); 
				UIButtonEx.SetItemInfo ( 0, 0, 0, ItemData.GetItemIndex(), -1, -1 );
				
				m_btnProcessItems.push_back ( UIButtonEx );
			
			}
		}
	}		

	AddString ( _S( 561, "가공품을 선택해 주십시오." ), COLOR_TEXT  );		

	m_sbProcessItem.SetScrollPos( 0 );
	m_sbProcessItem.SetCurItemCount( m_btnProcessItems.size()  );

	m_btnOK.SetEnable ( m_bSatisfied );
}

// ----------------------------------------------------------------------------
// Name : OpenProcess()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcess::OpenProcess( int nItemIndex, int nRow, int nCol )
{
	// If this is already exist
	if( IsVisible() )	return;

	// If inventory is already locked
	if( _pUIMgr->GetInventory()->IsLocked() )
	{
		_pUIMgr->GetInventory()->ShowLockErrorMessage();
		return;
	}

	Clear ();

	if( !_pUIMgr->GetInventory()->IsVisible() )
		_pUIMgr->GetInventory()->ToggleVisible();

	m_nProcessText	= nItemIndex;
	m_nRow			= nRow;
	m_nCol			= nCol;
	

	_pUIMgr->RearrangeOrder( UI_PROCESS, TRUE );
	
	InitProcess ();

	_pUIMgr->GetInventory()->Lock( TRUE, FALSE, LOCK_PROCESS );
}

// ----------------------------------------------------------------------------
// Name : CloseProcess()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcess::CloseProcess()
{
	// Close refine
	_pUIMgr->RearrangeOrder( UI_PROCESS, FALSE );

	// Unlock inventory
	_pUIMgr->GetInventory()->Lock( FALSE, FALSE, LOCK_PROCESS );

	Clear();
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcess::Render()
{

// Set skill learn texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

// Add render regions -----------------------------------------------------------------------------------------------
	// Background up
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + PROCESS_TOP_HEIGHT,
										m_rtBackgroundTop.U0, m_rtBackgroundTop.V0, m_rtBackgroundTop.U1, m_rtBackgroundTop.V1,
										0xFFFFFFFF );

	// Background down 
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY+PROCESS_TOP_HEIGHT, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
										m_rtBackgroundBtm.U0, m_rtBackgroundBtm.V0, m_rtBackgroundBtm.U1, m_rtBackgroundBtm.V1,
										0xFFFFFFFF );

	// Render Title Text
	_pUIMgr->GetDrawPort()->PutTextEx( _S( 560, "가공" ) , m_nPosX + PROCESS_TITLE_TEXT_OFFSETX,				
										m_nPosY + PROCESS_TITLE_TEXT_OFFSETY );

	// Render Process Type
	_pUIMgr->GetDrawPort()->PutTextExCX( m_StrProcessType, m_nPosX + 106, m_nPosY + 35, COLOR_TITLE );
	

	// Process Item Scroll bar
	m_sbProcessItem.Render();

	// List box of Need Item Desc
	m_lbPreconditionDesc.Render();

	// Close button
	m_btnClose.Render();
	
	// Process button
	m_btnOK.Render();

	// Cancel button
	m_btnCancel.Render();

	_pUIMgr->GetDrawPort()->FlushRenderingQueue();


	// Render Process Item
	CTString m_strShortDesc;
	int	nX = SLEARN_SLOT_SX;
	int	nY = SLEARN_SLOT_SY;

	int	iEnd = m_sbProcessItem.GetScrollPos() + PRODUCT_SLOT_ROW;
	if( iEnd > m_btnProcessItems.size() )
		iEnd = m_btnProcessItems.size();

	for ( int i = m_sbProcessItem.GetScrollPos(); i < iEnd; i++ )
	{
		if( !m_btnProcessItems[i].IsEmpty() )
		{
			// render Item
			m_btnProcessItems[i].SetPos ( nX, nY );
			m_btnProcessItems[i].Render();
			
			
			// render Item desc 
			CItemData& ItemData = _pNetwork->GetItemData ( m_btnProcessItems[i].GetIndex() );
			const char* szItemName = _pNetwork->GetItemName ( m_btnProcessItems[i].GetIndex() );
			
			_pUIMgr->GetDrawPort()->PutTextExCX( CTString ( szItemName ), m_nPosX + 122, m_nPosY + nY + 8, 0xC3C3C3ff );
												
			nY += SLEARN_SLOT_OFFSETY;
		}
	}
	_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_ITEM );

	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();

	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	nX = SLEARN_SLOT_SX;
	nY = SLEARN_SLOT_SY;

	// render sel outline
	if ( m_nSelectProcessItem != -1 )
	{
		int BoxX, BoxY;
		
		int nOffset = m_nSelectProcessItem - m_sbProcessItem.GetScrollPos();

		if ( nOffset >= 0 && nOffset < PROCESS_SLOT_ROW )
		{
			BoxX = m_nPosX + nX -1 ; 
			BoxY = m_nPosY + nY + ( SLEARN_SLOT_OFFSETY *  nOffset ) -1;

			
			_pUIMgr->GetDrawPort()->AddTexture( BoxX, BoxY,	BoxX + 34, BoxY + 34,
											m_rtSelOutline.U0, m_rtSelOutline.V0, m_rtSelOutline.U1, m_rtSelOutline.V1,
											0xffffffff );
		}
	}
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	
	RenderItemInfo ();
}

// ----------------------------------------------------------------------------
// Name : RenderItemInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcess::RenderItemInfo ()
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
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIProcess::MouseMessage( MSG *pMsg )
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

				int	iRowS = m_sbProcessItem.GetScrollPos();
				int	iRowE = iRowS + PROCESS_SLOT_ROW;

				if ( m_btnProcessItems.size() < iRowE )
				{
					iRowE = m_btnProcessItems.size();
				}
				
				if ( IsInsideRect ( nX, nY, m_rcItem ) )
				{
					bool bShowItem = false;
					for( int iRow = iRowS; iRow < iRowE; iRow++ )
					{
						if( m_btnProcessItems[iRow].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							bShowItem = true;
							ShowItemInfo( true, iRow ) ;					
						}
					}

					if ( !bShowItem ) 
						ShowItemInfo( false, -1 );
				}
				else
					ShowItemInfo( false, -1 );
			}
			
		
			// Move refine
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
			// OK button
			else if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Cancel button
			else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
				// List box of skill desc
			else if( m_sbProcessItem.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_lbPreconditionDesc.MouseMessage( pMsg ) != WMSG_FAIL )
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
				else if( m_sbProcessItem.MouseMessage( pMsg ) != WMSG_FAIL )
				{
						return WMSG_SUCCESS;
				}

				_pUIMgr->RearrangeOrder( UI_PROCESS, TRUE );
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

				// If refine isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseProcess();

					return WMSG_SUCCESS;
				}
				// OK button
				else if( ( wmsgResult = m_btnOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						SendProcessReq();

					return WMSG_SUCCESS;
				}
				// Cancel button
				else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseProcess();

					return WMSG_SUCCESS;
				}
				else if( m_sbProcessItem.MouseMessage( pMsg ) != WMSG_FAIL )
				{
						return WMSG_SUCCESS;
				}
				else if( IsInsideRect( nX, nY, m_rcIcons ) )
				{
					int	iRowS = m_sbProcessItem.GetScrollPos();
					int	iRowE = iRowS + PROCESS_SLOT_ROW;

					if ( m_btnProcessItems.size() < iRowE )
					{
						iRowE = m_btnProcessItems.size();
					}
		
					for( int iRow = iRowS; iRow < iRowE; iRow++ )
					{
						if( m_btnProcessItems[iRow].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							m_nSelectProcessItem = iRow;

							SelectItem ( m_nSelectProcessItem );
							return WMSG_SUCCESS;
						}
					}
				}
			
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
				return WMSG_SUCCESS;
		}

		break;
		
	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				if ( IsInsideRect ( nX, nY, m_rcItem ) )
				{ 
					return m_sbProcessItem.MouseMessage( pMsg );
				}
				else if ( IsInsideRect ( nX, nY, m_rcDesc ) )
				{
					return m_lbPreconditionDesc.MouseMessage ( pMsg );
				}
		
				return WMSG_SUCCESS;
			}
		}
		break;
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : AddItemInfoString()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcess::AddItemInfoString( CTString &strItemInfo, COLOR colItemInfo )
{
	if( m_nCurInfoLines >= MAX_ITEMINFO_LINE )
		return ;

	// Get length of string
	INDEX	nLength = strItemInfo.Length();
	if( nLength <= 0 )
		return;

	// wooss 051002
	if(g_iCountry == THAILAND){
		// Get length of string
		INDEX	nThaiLen = FindThaiLen(strItemInfo);
		INDEX	nChatMax= (MAX_ITEMINFO_CHAR-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
		if( nLength == 0 )
			return;
		// If length of string is less than max char
		if( nThaiLen <= nChatMax )
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
			for( int iPos = 0; iPos < nLength; iPos++ )
			{
				if(nChatMax < FindThaiLen(strItemInfo,0,iPos))
					break;
			}
			nSplitPos = iPos;

			// Split string
			CTString	strTemp;
			strItemInfo.Split( nSplitPos, m_strItemInfo[m_nCurInfoLines], strTemp );
			m_colItemInfo[m_nCurInfoLines++] = colItemInfo;

			// Trim space
			if( strTemp[0] == ' ' )
			{
				int	nTempLength = strTemp.Length();
				for( iPos = 1; iPos < nTempLength; ++iPos )
				{
					if( strTemp[iPos] != ' ' )
						break;
				}

				strTemp.TrimLeft( strTemp.Length() - iPos );
			}

			AddItemInfoString( strTemp, colItemInfo );

		}
		
	} else {
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
}


// ----------------------------------------------------------------------------
// Name : ShowItemInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcess::ShowItemInfo( BOOL bShowInfo, int nItemIndex, BOOL bRenew )
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
		nBtnID = m_btnProcessItems[nItemIndex].GetBtnID();

		// Update item information
		if( nOldBtnID != nBtnID || bRenew )
		{
			bUpdateInfo = TRUE;
			nOldBtnID = nBtnID;
			m_btnProcessItems[nItemIndex].GetAbsPos( nInfoPosX, nInfoPosY );

			// Get item information
		//	m_bDetailItemInfo = m_nSelectProcessItem == nItemIndex;
			
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
BOOL CUIProcess::GetItemInfo( int nItemIndex, int &nInfoWidth, int &nInfoHeight )
{
	CTString	strTemp;
	m_nCurInfoLines = 0;

	int			nIndex = m_btnProcessItems[nItemIndex].GetItemIndex();

	if( nIndex < 0 )
		return FALSE;

	CItemData	&rItemData = _pNetwork->GetItemData( nIndex );
	const char* szItemName = _pNetwork->GetItemName( nIndex );

	// Get item name
	strTemp = szItemName;
	AddItemInfoString( strTemp ); // 리스트에 추가 

	// Get item information in detail
	if( m_bDetailItemInfo )
	{
		// Weight
		strTemp.PrintF( _S( 165, "무게 : %d" ), rItemData.GetWeight() );
		AddItemInfoString( strTemp, 0xDEC05BFF );

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


// ----------------------------------------------------------------------------
// Name : SelectItem()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcess::SelectItem ( int _nIndex )
{
	if ( _nIndex == -1 )
	{
		_nIndex = m_nSelectProcessItem;
		if ( _nIndex == -1 ) return;
	}

	BOOL bNeedItem			= FALSE;	
	BOOL bNeedItemCount		= FALSE;
	BOOL bNeedSSkill		= FALSE;
	BOOL bNeedSSkillLevel	= FALSE;
		
	m_lbPreconditionDesc.ResetAllStrings();
	
	// 생산 하고자 하는 아이템 모록
	if ( m_btnProcessItems[_nIndex].GetIndex() == -1 ) return;
	CItemData&		ProcessItemData	= _pNetwork-> GetItemData ( m_btnProcessItems[_nIndex].GetIndex() );

	if ( ProcessItemData.GetNeedSSkillIndex () == -1 ) return;
	CItemData&		TextItemData	= _pNetwork->GetItemData ( m_nProcessText );
	
	CSpecialSkill	NeedSSkill		= _pNetwork->GetSSkillData ( ProcessItemData.GetNeedSSkillIndex () );
	int				nSSkillLevel	= ProcessItemData.GetNeedSSkillCount ();

// 조건 검색 
	// 필요 스킬
	if ( NeedSSkill.GetIndex() == -1 ) return;
	if ( nSSkillLevel == -1 ) return;

	bNeedSSkill = _pUIMgr->GetCharacterInfo()->CheckSSkillByIndex( NeedSSkill.GetIndex(), nSSkillLevel, &bNeedSSkillLevel );
	
// 필요 아이템 
	int nIndexTemp = 0;
	CItemData InvenItem;

	// 필요 아이템 종류와 갯수 얻기 
	nIndexTemp = ProcessItemData.GetNeedItemIndex( 0 );

	if( nIndexTemp == -1 ) return;
		
	m_NeedItems[0].ItemData	= _pNetwork->GetItemData ( nIndexTemp );
	m_NeedItems[0].llCount	= ProcessItemData.GetNeedItemCount( 0 ); 


	// 인벤에서 필요 아이템 검사 ( nUniIndex 처리 안했음 )
	for ( int nTab = 0; nTab < INVEN_SLOT_TAB; nTab++ )
	{
		for ( int nRow = 0 ; nRow < INVEN_SLOT_ROW_TOTAL; nRow++ )
		{
			for ( int nCol = 0 ; nCol < INVEN_SLOT_COL; nCol++ )
			{
				InvenItem = _pNetwork->MySlotItem[nTab][nRow][nCol].ItemData;
				
				if ( InvenItem.GetItemIndex() == m_NeedItems[0].ItemData.GetItemIndex() )
				{
					bNeedItem = TRUE;
					
					if ( _pNetwork->MySlotItem[nTab][nRow][nCol].Item_Sum >= m_NeedItems[0].llCount )
					{
						bNeedItemCount = TRUE;
					
						m_NeedItems[0].sbMatTab = nTab;
						m_NeedItems[0].sbMatRow = nRow;
						m_NeedItems[0].sbMatCol = nCol;
					}
					
				}
		
			}		
		}
	}


// 조건 출력 
	CTString strTitle;
	CTString strSpace = "";

// 필요 스킬 출력 
	AddString ( _S( 562, "필요 스킬" ), COLOR_SUB_TITLE ); 
	AddString ( strSpace + NeedSSkill.GetName(), bNeedSSkill? COLOR_TEXT : COLOR_NONE );
	m_bSatisfied = bNeedSSkill;

	AddString ( CTString ( " " ) );

// 필요 물품
	switch ( TextItemData.GetProcessType () )
	{
	case CItemData::PROCESS_DOC_STONE :
		strTitle = _S( 563, "필요 스톤" );				
		break;
	case CItemData::PROCESS_DOC_PLANT :
		strTitle = _S( 564, "필요 크락" );				
		break;
	case CItemData::PROCESS_DOC_ELEMENT :
		strTitle = _S( 565, "필요 원소" );				
		break;
	}
	AddString ( strTitle, COLOR_SUB_TITLE ); 

	
	bNeedItem &= bNeedItemCount;
	const char* szItemName = _pNetwork->GetItemName( m_NeedItems[0].ItemData.GetItemIndex() );
	strTitle.PrintF ( _S( 576, "%s : %d개 " ), strSpace + szItemName, m_NeedItems[0].llCount );

	AddString ( strTitle, bNeedItem? COLOR_TEXT : COLOR_NONE );
	m_bSatisfied &= bNeedItem;

	m_btnOK.SetEnable ( m_bSatisfied );
}


// ----------------------------------------------------------------------------
// Name : AddString()
// Desc : 
// ----------------------------------------------------------------------------
void CUIProcess::AddString ( CTString& strText, COLOR colText )
{
	int nLength =  strText.Length();

	if( nLength >= _iMaxMsgStringChar )
	{
		nLength -= nLength - _iMaxMsgStringChar;
		
		do
		{
			if ( strText[nLength] == ' ' )
			{
				break;
			}

		} while ( nLength-- );

		CTString strTemp2, strTemp;

		strText.Split( nLength, strTemp2, strTemp );
		m_lbPreconditionDesc.AddString ( 0, strTemp2, colText );

		strTemp2.PrintF ( "%s", strTemp );
		
		m_lbPreconditionDesc.AddString ( 0, strTemp2, colText );

	}
	else 
	{
		m_lbPreconditionDesc.AddString ( 0, strText, colText );
	}

}





// ========================================================================= //
//                           Send message functions                          //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : SendProcessReq()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcess::SendProcessReq()
{
	if( m_bWaitProcessResult )
		return;

	if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsSkilling() )
	{
		_pUIMgr->GetChatting()->AddSysMessage(  _S( 941, "스킬 사용중에는 가공을 할 수 없습니다." ) , SYSMSG_ERROR );		
		return;
	}

	if( _pUIMgr->IsCSFlagOn( CSF_TELEPORT ) )
	{
		_pUIMgr->GetChatting()->AddSysMessage(  _S( 942, "순간 이동중에는 가공을 할 수 없습니다." ) , SYSMSG_ERROR );	
		return;
	}

	CItemData& ProcessItemData	= _pNetwork->GetItemData ( m_btnProcessItems[m_nSelectProcessItem].GetIndex() );

	_pNetwork->ProcessReq ( m_nRow, m_nCol, m_btnProcessItems[m_nSelectProcessItem].GetIndex(), 1, 
						m_NeedItems );

	m_bWaitProcessResult = TRUE;
	
	m_btnOK.SetEnable ( !m_bWaitProcessResult );
	//SetEnable ( !m_bWaitProcessResult );
}

// ========================================================================= //
//                         Receive message functions                         // 
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : ProcessRep()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcess::ProcessRep( SBYTE sbResult )
{
	// Close message box
	_pUIMgr->CloseMessageBox( MSGCMD_PROCESS_REP );

	// Show result
	CTString	strMessage;
	
	switch( sbResult )
	{

	case MSG_PROCESS_FAIL_SYSTEM :			// 있어서는 안되는 이유로 실패
		strMessage = _S( 566, "가공에 실패하였습니다." );						
		break;

	case MSG_PROCESS_FAIL_MATERIAL :		// 재료 정보 오류
		strMessage = _S( 567, "가공에 필요한 재료가 부족합니다." );			
		break;

	case MSG_PROCESS_FAIL_NO_SSKILL :		// 가공 스킬 없음
		strMessage = _S( 568, "가공에 필요한 가공 스킬이 없습니다." );				
		break;
	//case MSG_PROCESS_FAIL_SSKILL_LEVEL :	// 가공 스킬 레벨 오류
	//	strMessage = _S( 569, "가공에 필요한 스킬 레벨이 부족합니다." );			
	//	break;
	case MSG_PROCESS_FAIL_OVER_WEIGHT: // 무게가 초과
		strMessage = _S( 705, "무게가 초과하여 가공을 할 수 없습니다." );
		break;
	case MSG_PROCESS_FAIL_PROB :			// 확률 실패
		strMessage = _S( 566, "가공에 실패하였습니다." );							
		break;
	case MSG_PROCESS_SUCCESS :				// 가공 성공
		strMessage = _S( 570, "가공에 성공하였습니다." );							
		break;
	
	}

	CloseProcess();

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 560, "가공" ), UMBS_OK, UI_PROCESS, MSGCMD_PROCESS_REP );		
	MsgBoxInfo.AddString( strMessage );
	_pUIMgr->CreateMessageBox( MsgBoxInfo );

	m_bWaitProcessResult = FALSE;	
}
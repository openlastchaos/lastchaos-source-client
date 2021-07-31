#include "stdh.h"
#include <Engine/Interface/UIProcessNPC.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Entities/Itemdata.h>
#include <Engine/Entities/Items.h>
#include <Engine/Interface/UISkillLearn.h>
#include <Engine/Interface/UIInventory.h>

extern INDEX g_iCountry;
// Date : 2005-03-07,   By Lee Ki-hwan
static int	_iMaxMsgStringChar = 0;

enum eSelProcessNPC
{
	SEL_MINERAL = 0,	// 광석 정련
	SEL_CROP,			// 식물 가공
	SEL_ENERGY,			// 원소 정제
};

enum eSelMineral
{
	SEL_MINERAL_1 = 0,	// 스톤 온
	SEL_MINERAL_2,		// 스톤 리스
	SEL_MINERAL_3,		// 스톤 웨버
	SEL_MINERAL_4,		// 스톤 비스트
	SEL_MINERAL_5,		// 스톤 웰스던
};

enum eSelEnergy
{
	SEL_ENERGY_1 = 0,	// E
	SEL_ENERGY_2,		// D
	SEL_ENERGY_3,		// C
	SEL_ENERGY_4,		// B
	SEL_ENERGY_5,		// A
};

enum eSelCrop
{
	SEL_CROP_1 = 0,		// 노란잎
	SEL_CROP_2,			// 줄기
	SEL_CROP_3,			// 파란잎
	SEL_CROP_4,			// 가시
	SEL_CROP_5,			// 꽃
};

// ----------------------------------------------------------------------------
// Name : CUIProcessNPC()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIProcessNPC::CUIProcessNPC()
{
	Clear ();
}

// ----------------------------------------------------------------------------
// Name : ~CUIProcessNPC()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIProcessNPC::~CUIProcessNPC()
{
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	int diff = SKILLLEARN_HEIGHT - PROCESSNPC_HEIGHT;
	
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );
	
	_iMaxMsgStringChar = PROCESSNPC_STRING_CHAR_WIDTH / ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
	
	// Region of each part
	m_rcTitle.SetRect( 0, 0, 216, 22 );
	m_rcIcons.SetRect( 17, 55, 51, 200 );
	//m_rcTab.SetRect( 21, 31, 204, 50 );
	
	m_rcItem.SetRect ( 13, 33, 218, 203 );
	m_rcDesc.SetRect ( 13, 203, 218, 331 );
	
	// ProcessNPC Texture 생성 - SkillLearn Texture 사용
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\SkillLearn.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();
	
	// UV Coordinate of each part
	m_rtBackgroundTop.SetUV( 0, 0, 216, PROCESSNPC_TOP_HEIGHT, fTexWidth, fTexHeight );
	m_rtBackgroundBtm.SetUV( 0, PROCESSNPC_TOP_HEIGHT + diff, 216, SKILLLEARN_HEIGHT, fTexWidth, fTexHeight );
	
	m_rtSelOutline.SetUV( 218, 80, 250, 112, fTexWidth, fTexHeight );
	m_rtInput.SetUV( 218, 136, 248, 149, fTexWidth, fTexHeight );
	
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
	
	// ProcessNPC button
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

	// Up button
	m_btnUP.Create( this, CTString(""), 141, 372-diff, 7, 7 );
	m_btnUP.SetUV( UBS_IDLE, 219, 62, 228, 69, fTexWidth, fTexHeight );
	m_btnUP.SetUV( UBS_CLICK, 230, 62, 239, 69, fTexWidth, fTexHeight );
	m_btnUP.CopyUV( UBS_IDLE, UBS_ON );
	m_btnUP.CopyUV( UBS_IDLE, UBS_DISABLE );
	m_btnUP.SetEnable(FALSE);

	// Down button
	m_btnDown.Create( this, CTString(""), 141, 372-diff, 7, 7 );
	m_btnDown.SetUV( UBS_IDLE, 219, 71, 228, 78, fTexWidth, fTexHeight );
	m_btnDown.SetUV( UBS_CLICK, 230, 71, 239, 78, fTexWidth, fTexHeight );
	m_btnDown.CopyUV( UBS_IDLE, UBS_ON );
	m_btnDown.CopyUV( UBS_IDLE, UBS_DISABLE );
	m_btnDown.SetEnable(FALSE);
	
	// Scroll bar of ProcessNPC Item
	m_sbProcessNPCItem.Create( this, 193, 51, 9, 153 );
	m_sbProcessNPCItem.CreateButtons( TRUE, 9, 7, 0, 0, 10 );
	m_sbProcessNPCItem.SetScrollPos( 0 );
	m_sbProcessNPCItem.SetScrollRange( PROCESSNPC_SLOT_ROW_TOTAL );
	m_sbProcessNPCItem.SetCurItemCount( 0 );
	m_sbProcessNPCItem.SetItemsPerPage( PROCESSNPC_SLOT_ROW );	
	// Up button
	m_sbProcessNPCItem.SetUpUV( UBS_IDLE, 219, 62, 228, 69, fTexWidth, fTexHeight );
	m_sbProcessNPCItem.SetUpUV( UBS_CLICK, 230, 62, 239, 69, fTexWidth, fTexHeight );
	m_sbProcessNPCItem.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbProcessNPCItem.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbProcessNPCItem.SetBarTopUV( 219, 32, 228, 42, fTexWidth, fTexHeight );
	m_sbProcessNPCItem.SetBarMiddleUV( 219, 43, 228, 45, fTexWidth, fTexHeight );
	m_sbProcessNPCItem.SetBarBottomUV( 219, 46, 228, 56, fTexWidth, fTexHeight );
	// Down button
	m_sbProcessNPCItem.SetDownUV( UBS_IDLE, 219, 71, 228, 78, fTexWidth, fTexHeight );
	m_sbProcessNPCItem.SetDownUV( UBS_CLICK, 230, 71, 239, 78, fTexWidth, fTexHeight );
	m_sbProcessNPCItem.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbProcessNPCItem.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Wheel region
	m_sbProcessNPCItem.SetWheelRect( -181, -1, 180, 154 );
	
	
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
void CUIProcessNPC::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : Clear()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::Clear()
{
	m_iResourceType			= -1;
	m_iResourceSubType		= -1;
	m_nProcessItemCount		= 1;

	m_nSelectProcessNPCItem	= -1;
	m_bWaitProcessNPCResult	= TRUE;
	m_bSatisfied			= FALSE;
	
	m_btnProcessNPCItems.size();
	
	//m_nProcessNPCText			= -1;				
	//m_nRow					= -1;
	//m_nCol					= -1;
	
	m_nNeedItemCount		= 0;
	
	memset ( m_NeedItems, 0, sizeof ( CNeedItems ) * MAX_MAKE_ITEM_MATERIAL );
	
	//m_StrProcessNPCType.Clear ();
	
	m_bWaitProcessNPCResult = FALSE;
	m_lbPreconditionDesc.ResetAllStrings();
	
	for(std::vector<CUIButtonEx>::iterator pos = m_btnProcessNPCItems.begin(); 
		pos != m_btnProcessNPCItems.end(); ++pos)
	{
		(*pos).InitBtn();
	}
	
	m_btnProcessNPCItems.clear ();
}

// ----------------------------------------------------------------------------
// Name : InitProcessNPC()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::InitProcessNPC()
{	
	m_btnProcessNPCItems.clear ();
	//CItemData &TextItemData = _pNetwork->GetItemData ( m_nProcessNPCText );
	//const char* szItemName	= _pNetwork->GetItemName( m_nProcessNPCText );
	
	// 임시 처리 
	int nProcessType = m_iResourceType;
	int nProcessSubType = m_iResourceSubType;
	
	// 제조  Type 설정
	//m_StrProcessNPCType = szItemName;
	
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
				UIButtonEx.Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_PROCESSNPC ); 
				UIButtonEx.SetItemInfo ( 0, 0, 0, ItemData.GetItemIndex(), -1, -1 );
				
				m_btnProcessNPCItems.push_back ( UIButtonEx );				
			}
		}
	}		
	
	AddString ( _S( 561, "가공품을 선택해 주십시오." ), COLOR_TEXT  );		
	
	m_sbProcessNPCItem.SetScrollPos( 0 );
	m_sbProcessNPCItem.SetCurItemCount( m_btnProcessNPCItems.size()  );
	
	m_btnOK.SetEnable ( m_bSatisfied );
}

// ----------------------------------------------------------------------------
// Name : OpenProcessNPC()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::OpenProcessNPC( )
{
	_pUIMgr->CloseMessageBoxL( MSGLCMD_PROCESSNPC_REQ );
	
	_pUIMgr->CreateMessageBoxL( _S( 1786, "가공 NPC"  ) , UI_PROCESSNPC, MSGLCMD_PROCESSNPC_REQ );		
	
	_pUIMgr->AddMessageBoxLString( MSGLCMD_PROCESSNPC_REQ, TRUE, _S( 1787, "무엇을 가공하러 왔는가?" ), -1, 0xA3A1A3FF );		
	_pUIMgr->AddMessageBoxLString( MSGLCMD_PROCESSNPC_REQ, TRUE, _S( 1788, "어떤 생산품이든 맡겨만 주면 내가 완벽하게 가공해 주겠네. 물론 나도 먹고 살아야 하니 무료로는 힘들어." ), -1, 0xA3A1A3FF );		
	_pUIMgr->AddMessageBoxLString( MSGLCMD_PROCESSNPC_REQ, TRUE, _S( 1789, "수수료를 주는 것이 싫다면 어쩔 수 없지만 이 근처에서 나만큼 뛰어난 가공기술을 가지고 있는 사람은 보질 못했어.  잘 생각해 보라구." ), -1, 0xA3A1A3FF );		
	_pUIMgr->AddMessageBoxLString( MSGLCMD_PROCESSNPC_REQ, FALSE, _S( 1790, "광석 정련" ), SEL_MINERAL );	
	_pUIMgr->AddMessageBoxLString( MSGLCMD_PROCESSNPC_REQ, FALSE, _S( 1791, "식물 가공" ), SEL_CROP );	
	_pUIMgr->AddMessageBoxLString( MSGLCMD_PROCESSNPC_REQ, FALSE, _S( 1792, "원소 정제" ), SEL_ENERGY );	
	_pUIMgr->AddMessageBoxLString( MSGLCMD_PROCESSNPC_REQ, FALSE, _S( 1220, "취소한다." ) );	
}

// ----------------------------------------------------------------------------
// Name : OpenProcessList()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::OpenProcessList( int iType, int iSubType )
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
	
	//m_nProcessNPCText	= nItemIndex;	
	m_iResourceType = iType;
	m_iResourceSubType = iSubType;
	
	_pUIMgr->RearrangeOrder( UI_PROCESSNPC, TRUE );
	
	InitProcessNPC ();
	
	//_pUIMgr->GetInventory()->Lock( TRUE, FALSE, LOCK_PROCESSNPC );
}

// ----------------------------------------------------------------------------
// Name : CloseProcessNPC()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::CloseProcessNPC()
{
	// Close refine
	_pUIMgr->RearrangeOrder( UI_PROCESSNPC, FALSE );
	
	// Unlock inventory
	//_pUIMgr->GetInventory()->Lock( FALSE, FALSE, LOCK_PROCESSNPC );
	
	Clear();
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::Render()
{
	
	// Set skill learn texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );
	
	// Add render regions -----------------------------------------------------------------------------------------------
	// Background up
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + PROCESSNPC_TOP_HEIGHT,
		m_rtBackgroundTop.U0, m_rtBackgroundTop.V0, m_rtBackgroundTop.U1, m_rtBackgroundTop.V1,
		0xFFFFFFFF );
	
	// Background down 
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY+PROCESSNPC_TOP_HEIGHT, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
		m_rtBackgroundBtm.U0, m_rtBackgroundBtm.V0, m_rtBackgroundBtm.U1, m_rtBackgroundBtm.V1,
		0xFFFFFFFF );	
	
	// Render Title Text
	_pUIMgr->GetDrawPort()->PutTextEx( _S( 560, "가공" ) , m_nPosX + PROCESSNPC_TITLE_TEXT_OFFSETX,				
		m_nPosY + PROCESSNPC_TITLE_TEXT_OFFSETY );
	
	// Render ProcessNPC Type
	//_pUIMgr->GetDrawPort()->PutTextExCX( m_StrProcessNPCType, m_nPosX + 106, m_nPosY + 35, COLOR_TITLE );
	
	
	// ProcessNPC Item Scroll bar
	m_sbProcessNPCItem.Render();
	
	// List box of Need Item Desc
	m_lbPreconditionDesc.Render();
	
	// Close button
	m_btnClose.Render();
	
	// ProcessNPC button
	m_btnOK.Render();
	
	// Cancel button
	m_btnCancel.Render();
	
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	
	
	// Render ProcessNPC Item
	CTString m_strShortDesc;
	int	nX = SLEARN_SLOT_SX;
	int	nY = SLEARN_SLOT_SY;
	
	int	iEnd = m_sbProcessNPCItem.GetScrollPos() + PRODUCT_SLOT_ROW;
	if( iEnd > m_btnProcessNPCItems.size() )
		iEnd = m_btnProcessNPCItems.size();
	
	for ( int i = m_sbProcessNPCItem.GetScrollPos(); i < iEnd; i++ )
	{
		if( !m_btnProcessNPCItems[i].IsEmpty() )
		{
			// render Item
			m_btnProcessNPCItems[i].SetPos ( nX, nY );
			m_btnProcessNPCItems[i].Render();			
			
			// render Item desc 
			CItemData& ItemData = _pNetwork->GetItemData ( m_btnProcessNPCItems[i].GetIndex() );
			const char* szItemName = _pNetwork->GetItemName ( m_btnProcessNPCItems[i].GetIndex() );
			CTString strOutput = szItemName;
			
			if(i == m_nSelectProcessNPCItem)//현재 선택되어 있다면
			{
				int index = m_nSelectProcessNPCItem - m_sbProcessNPCItem.GetScrollPos();
				char buf[3];
				sprintf(buf, _S( 766, "%d 개" ), m_nProcessItemCount);
				strOutput += buf;
			}
												
			_pUIMgr->GetDrawPort()->PutTextExCX( strOutput, m_nPosX + 114, m_nPosY + nY + 8, 0xC3C3C3ff );

			nY += SLEARN_SLOT_OFFSETY;
		}
	}
	_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_ITEM );
	
	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();
	
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );
	
	nX = SLEARN_SLOT_SX;
	nY = SLEARN_SLOT_SY;

	m_btnDown.SetEnable(FALSE);
	m_btnUP.SetEnable(FALSE);
	
	// render sel outline
	if ( m_nSelectProcessNPCItem != -1 )
	{
		int BoxX, BoxY;
		
		int nOffset = m_nSelectProcessNPCItem - m_sbProcessNPCItem.GetScrollPos();
		
		if ( nOffset >= 0 && nOffset < PROCESSNPC_SLOT_ROW )
		{
			BoxX = m_nPosX + nX -1 ; 
			BoxY = m_nPosY + nY + ( SLEARN_SLOT_OFFSETY *  nOffset ) -1;			
			
			_pUIMgr->GetDrawPort()->AddTexture( BoxX, BoxY,	BoxX + 34, BoxY + 34,
				m_rtSelOutline.U0, m_rtSelOutline.V0, m_rtSelOutline.U1, m_rtSelOutline.V1,
				0xffffffff );
			
			// Input
			_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + 156, BoxY + 19, m_nPosX + 156 + 31, BoxY + 19 + 14, 
				m_rtInput.U0, m_rtInput.V0, m_rtInput.U1, m_rtInput.V1, 0xFFFFFFFF );

			m_btnUP.SetPos( 180, nY + ( SLEARN_SLOT_OFFSETY *  nOffset ) + 19 + 13 - 14);
			m_btnDown.SetPos( 180, nY + ( SLEARN_SLOT_OFFSETY *  nOffset ) + 19 + 13 - 7);

			m_btnUP.SetEnable(TRUE);
			m_btnDown.SetEnable(TRUE);
			m_btnUP.Render();
			m_btnDown.Render();
		}
	}
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	
	RenderItemInfo ();
}

// ----------------------------------------------------------------------------
// Name : RenderItemInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::RenderItemInfo ()
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
WMSG_RESULT CUIProcessNPC::MouseMessage( MSG *pMsg )
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
				
				int	iRowS = m_sbProcessNPCItem.GetScrollPos();
				int	iRowE = iRowS + PROCESSNPC_SLOT_ROW;
				
				if ( m_btnProcessNPCItems.size() < iRowE )
				{
					iRowE = m_btnProcessNPCItems.size();
				}
				
				if ( IsInsideRect ( nX, nY, m_rcItem ) )
				{
					bool bShowItem = false;
					for( int iRow = iRowS; iRow < iRowE; iRow++ )
					{
						if( m_btnProcessNPCItems[iRow].MouseMessage( pMsg ) != WMSG_FAIL )
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
			else if( m_sbProcessNPCItem.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_lbPreconditionDesc.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnUP.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnDown.MouseMessage( pMsg ) != WMSG_FAIL )
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
				else if( m_sbProcessNPCItem.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS;
				}
				else if( m_btnUP.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					++m_nProcessItemCount;
					if(m_nProcessItemCount > 10) m_nProcessItemCount = 10;
					SelectItem ( m_nSelectProcessNPCItem );
					return WMSG_SUCCESS;
				}
				else if( m_btnDown.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					--m_nProcessItemCount;
					if(m_nProcessItemCount < 1) m_nProcessItemCount = 1;
					SelectItem ( m_nSelectProcessNPCItem );
					return WMSG_SUCCESS;
				}
				
				_pUIMgr->RearrangeOrder( UI_PROCESSNPC, TRUE );
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
						CloseProcessNPC();
					
					return WMSG_SUCCESS;
				}
				// OK button
				else if( ( wmsgResult = m_btnOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						SendProcessNPCReq();
					
					return WMSG_SUCCESS;
				}
				// Cancel button
				else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseProcessNPC();
					
					return WMSG_SUCCESS;
				}
				else if( m_sbProcessNPCItem.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS;
				}				
				else if( m_btnUP.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_btnDown.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( IsInsideRect( nX, nY, m_rcIcons ) )
				{
					int	iRowS = m_sbProcessNPCItem.GetScrollPos();
					int	iRowE = iRowS + PROCESSNPC_SLOT_ROW;
					
					if ( m_btnProcessNPCItems.size() < iRowE )
					{
						iRowE = m_btnProcessNPCItems.size();
					}
					
					for( int iRow = iRowS; iRow < iRowE; iRow++ )
					{
						if( m_btnProcessNPCItems[iRow].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							if(m_nSelectProcessNPCItem != iRow) m_nProcessItemCount = 1;
							m_nSelectProcessNPCItem = iRow;
							
							SelectItem ( m_nSelectProcessNPCItem );
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
					return m_sbProcessNPCItem.MouseMessage( pMsg );
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
void CUIProcessNPC::AddItemInfoString( CTString &strItemInfo, COLOR colItemInfo )
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
void CUIProcessNPC::ShowItemInfo( BOOL bShowInfo, int nItemIndex, BOOL bRenew )
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
		nBtnID = m_btnProcessNPCItems[nItemIndex].GetBtnID();
		
		// Update item information
		if( nOldBtnID != nBtnID || bRenew )
		{
			bUpdateInfo = TRUE;
			nOldBtnID = nBtnID;
			m_btnProcessNPCItems[nItemIndex].GetAbsPos( nInfoPosX, nInfoPosY );
			
			// Get item information
			//	m_bDetailItemInfo = m_nSelectProcessNPCItem == nItemIndex;
			
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
BOOL CUIProcessNPC::GetItemInfo( int nItemIndex, int &nInfoWidth, int &nInfoHeight )
{
	CTString	strTemp;
	m_nCurInfoLines = 0;
	
	int			nIndex = m_btnProcessNPCItems[nItemIndex].GetItemIndex();
	
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
void CUIProcessNPC::SelectItem ( int _nIndex )
{
	if ( _nIndex == -1 )
	{
		_nIndex = m_nSelectProcessNPCItem;
		if ( _nIndex == -1 ) return;
	}
	
	BOOL bNeedItem			= FALSE;	
	BOOL bNeedItemCount		= FALSE;
	//BOOL bNeedSSkill		= FALSE;
	//BOOL bNeedSSkillLevel	= FALSE;
	
	m_lbPreconditionDesc.ResetAllStrings();
	
	// 생산 하고자 하는 아이템 모록
	if ( m_btnProcessNPCItems[_nIndex].GetIndex() == -1 ) return;
	CItemData&		ProcessNPCItemData	= _pNetwork-> GetItemData ( m_btnProcessNPCItems[_nIndex].GetIndex() );
	
	//if ( ProcessNPCItemData.GetNeedSSkillIndex () == -1 ) return;
	//CItemData&		TextItemData	= _pNetwork->GetItemData ( m_nProcessNPCText );
	
	//CSpecialSkill	NeedSSkill		= _pNetwork->GetSSkillData ( ProcessNPCItemData.GetNeedSSkillIndex () );
	//int				nSSkillLevel	= ProcessNPCItemData.GetNeedSSkillCount ();
	
	// 조건 검색 
	// 필요 스킬
	//if ( NeedSSkill.GetIndex() == -1 ) return;
	//if ( nSSkillLevel == -1 ) return;
	
	//bNeedSSkill = _pUIMgr->GetCharacterInfo()->CheckSSkillByIndex( NeedSSkill.GetIndex(), nSSkillLevel, &bNeedSSkillLevel );
	
	// 필요 아이템 
	int nIndexTemp = 0;
	CItemData InvenItem;
	
	// 필요 아이템 종류와 갯수 얻기 
	nIndexTemp = ProcessNPCItemData.GetNeedItemIndex( 0 );
	
	if( nIndexTemp == -1 ) return;
	
	m_NeedItems[0].ItemData	= _pNetwork->GetItemData ( nIndexTemp );
	m_NeedItems[0].llCount	= ProcessNPCItemData.GetNeedItemCount( 0 );
	__int64 myMoney = 0;
	
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
				if(_pNetwork->MySlotItem[nTab][nRow][nCol].Item_Index == 19)
				{
					myMoney = _pNetwork->MySlotItem[nTab][nRow][nCol].Item_Sum;
				}
			}		
		}
	}
	
	
	// 조건 출력 
	CTString strTitle;
	CTString strSpace = "";
	
	// 필요 스킬 출력 
	//AddString ( _S( 562, "필요 스킬" ), COLOR_SUB_TITLE ); 
	//AddString ( strSpace + NeedSSkill.GetName(), bNeedSSkill? COLOR_TEXT : COLOR_NONE );
	//m_bSatisfied = bNeedSSkill;
	
	//AddString ( CTString ( " " ) );
	
	// 필요 물품
	switch ( m_iResourceType )
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
	
	
	bNeedItem = bNeedItem && bNeedItemCount;
	const char* szItemName = _pNetwork->GetItemName( m_NeedItems[0].ItemData.GetItemIndex() );
	strTitle.PrintF ( _S( 576, "%s : %d개 " ), strSpace + szItemName
		, m_nProcessItemCount * m_NeedItems[0].llCount );
	
	AddString ( strTitle, bNeedItem ? COLOR_TEXT : COLOR_NONE );
	m_bSatisfied = bNeedItem;

	AddString( CTString(""), COLOR_TEXT );
	AddString( _S( 1793, "수수료" ), COLOR_SUB_TITLE );
	CTString strMoney;
	static int s_iMoney[3][5] = {
		{10, 21, 40, 63, 94}
		, {10, 21, 40, 63, 94}
		, {10, 21, 40, 63, 94}
	};
	__int64 needMoney = s_iMoney[m_iResourceType][m_iResourceSubType] * m_nProcessItemCount * m_NeedItems[0].llCount;
	strMoney.PrintF("%I64d %s", needMoney, _S( 1762, "나스" ) );
	AddString( strMoney, needMoney <= myMoney ? COLOR_TEXT : COLOR_NONE );
	
	m_bSatisfied = m_bSatisfied && needMoney <= myMoney;
	m_btnOK.SetEnable ( m_bSatisfied );
}


// ----------------------------------------------------------------------------
// Name : AddString()
// Desc : 
// ----------------------------------------------------------------------------
void CUIProcessNPC::AddString ( CTString& strText, COLOR colText )
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
// Name : SendProcessNPCReq()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::SendProcessNPCReq()
{
	if( m_bWaitProcessNPCResult )
		return;
	
	if( _pUIMgr->IsCSFlagOn( CSF_TELEPORT ) )
	{
		_pUIMgr->GetChatting()->AddSysMessage(  _S( 942, "순간 이동중에는 가공을 할 수 없습니다." ) , SYSMSG_ERROR );	
		return;
	}
	
	CItemData& ProcessNPCItemData	= _pNetwork->GetItemData ( m_btnProcessNPCItems[m_nSelectProcessNPCItem].GetIndex() );

	//_pNetwork->ProcessReq ( m_nRow, m_nCol, m_btnProcessNPCItems[m_nSelectProcessNPCItem].GetIndex(), 1, m_NeedItems );
	_pNetwork->SendNPCProcessMessage(m_btnProcessNPCItems[m_nSelectProcessNPCItem].GetItemIndex(), m_nProcessItemCount);
	
	m_bWaitProcessNPCResult = TRUE;
	
	m_btnOK.SetEnable ( !m_bWaitProcessNPCResult );
	//SetEnable ( !m_bWaitProcessNPCResult );
}

// ========================================================================= //
//                         Receive message functions                         // 
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : ProcessNPCRep()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::ProcessNPCRep( LONG processItemDBIndex, LONG count, LONG result )
{
	m_bWaitProcessNPCResult = FALSE;	
	// Close message box
	_pUIMgr->CloseMessageBox( MSGCMD_PROCESS_REP );
	
	// Show result
	CTString	strMessage;
	bool bRetProcessNPCList = false;
	CTString strTitle;
	CTString strContent;

	int commandCode = -1;
	
	strTitle = _S( 1794, "가공 실패" );
	
	switch(result)
	{
	case MSG_ITEM_PROCESS_NPC_ERROR_OK:		// 성공
		{
			strTitle = _S( 1795, "가공 성공" );
			strContent = _S( 1796, "물품 가공에 성공하였습니다. 수수료를 지불합니다." );
			commandCode = MSGCMD_PROCESS_REP;
		} break;
	case MSG_ITEM_PROCESS_NPC_ERROR_FAIL:	// 실패
		{
			strContent = _S( 1797, "물품 가공에 실패하였습니다. 수수료는 지불하지 않습니다." );
			commandCode = MSGCMD_PROCESS_REP;
		} break;
	case MSG_ITEM_PROCESS_NPC_ERROR_COUNT:	// 카운트 오류
		{
			strContent = _S( 1798, "가공할 재료의 개수를 입력하여 주십시오." );
			commandCode = MSGCMD_PROCESS_NOTIFY;
		} break;
	case MSG_ITEM_PROCESS_NPC_ERROR_NOHAVE:	// 재료 부족
		{
			strContent = _S( 1799, "재료가 부족합니다. 확인하고 다시 시도하여 주십시오." );
			commandCode = MSGCMD_PROCESS_NOTIFY;
		} break;
	case MSG_ITEM_PROCESS_NPC_ERROR_MONEY:	// 나스 부족
		{
			strContent = _S( 1800, "수수료가 부족합니다. 확인하고 다시 시도하여 주십시오." );
			commandCode = MSGCMD_PROCESS_NOTIFY;
		} break;
	}
	
	m_iLastResourceType = m_iResourceType;
	m_iLastResourceSubType = m_iResourceSubType;
	CloseProcessNPC();

	_pUIMgr->CloseMessageBox(MSGCMD_PROCESS_NOTIFY);
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( strTitle, UMBS_OK, UI_PROCESSNPC, commandCode );
	MsgBoxInfo.AddString( strContent );
	_pUIMgr->CreateMessageBox( MsgBoxInfo );
	
	m_bWaitProcessNPCResult = FALSE;	
}

void CUIProcessNPC::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
	case MSGCMD_PROCESS_NOTIFY:
		{
		} break;
	case MSGCMD_PROCESS_REP:
		{
			OpenProcessList(m_iLastResourceType, m_iLastResourceSubType);
		} break;
	}
}

void CUIProcessNPC::MsgBoxLCommand( int nCommandCode, int nResult )
{
	switch( nCommandCode )
	{
	case MSGLCMD_PROCESSNPC_REQ:
		{
			switch( nResult )
			{
			case SEL_MINERAL:		// 광석 정련
				{
					_pUIMgr->CloseMessageBoxL( MSGLCMD_PROCESSNPC_MINERAL_REQ );
					
					_pUIMgr->CreateMessageBoxL( _S( 1790, "광석 정련"  ) , UI_PROCESSNPC, MSGLCMD_PROCESSNPC_MINERAL_REQ );
					
					_pUIMgr->AddMessageBoxLString( MSGLCMD_PROCESSNPC_MINERAL_REQ, TRUE, _S( 1801, "그래 잘 생각했네." ), -1, 0xA3A1A3FF );		
					_pUIMgr->AddMessageBoxLString( MSGLCMD_PROCESSNPC_MINERAL_REQ, TRUE, _S( 1802, "자 이제 어떤 스톤을 정련할 것인지 한번 골라보라구." ), -1, 0xA3A1A3FF );		
					_pUIMgr->AddMessageBoxLString( MSGLCMD_PROCESSNPC_MINERAL_REQ, TRUE, _S( 1803, "아 나라고 무조건 정련에 성공하는 것은 아니네. 가끔 실패를 하기도 하지. 물론 실패한다면 수수료는 받지 않네." ), -1, 0xA3A1A3FF );		
					_pUIMgr->AddMessageBoxLString( MSGLCMD_PROCESSNPC_MINERAL_REQ, FALSE, _S( 1804, "스톤 온 정련" ), SEL_MINERAL_1 );	
					_pUIMgr->AddMessageBoxLString( MSGLCMD_PROCESSNPC_MINERAL_REQ, FALSE, _S( 1805, "스톤 리스 정련" ), SEL_MINERAL_2 );	
					_pUIMgr->AddMessageBoxLString( MSGLCMD_PROCESSNPC_MINERAL_REQ, FALSE, _S( 1806, "스톤 웨버 정련" ), SEL_MINERAL_3 );	
					//_pUIMgr->AddMessageBoxLString( MSGLCMD_PROCESSNPC_MINERAL_REQ, FALSE, _S( 1807, "스톤 비스트 정련" ), SEL_MINERAL_4 );	
					//_pUIMgr->AddMessageBoxLString( MSGLCMD_PROCESSNPC_MINERAL_REQ, FALSE, _S( 1808, "스톤 웰스던 정련" ), SEL_MINERAL_5 );	
					_pUIMgr->AddMessageBoxLString( MSGLCMD_PROCESSNPC_MINERAL_REQ, FALSE, _S( 1220, "취소한다." ) );
				}
				break;
				
			case SEL_ENERGY:		// 원소 정제
				{
					_pUIMgr->CloseMessageBoxL( MSGLCMD_PROCESSNPC_ENERGY_REQ );
					
					_pUIMgr->CreateMessageBoxL( _S( 1792, "원소 정제"  ) , UI_PROCESSNPC, MSGLCMD_PROCESSNPC_ENERGY_REQ );
					
					_pUIMgr->AddMessageBoxLString( MSGLCMD_PROCESSNPC_ENERGY_REQ, TRUE, _S( 1801, "그래 잘 생각했네." ), -1, 0xA3A1A3FF );		
					_pUIMgr->AddMessageBoxLString( MSGLCMD_PROCESSNPC_ENERGY_REQ, TRUE, _S( 1809, "자 이제 어떤 원소를 정제할 것인지 한번 골라보라구." ), -1, 0xA3A1A3FF );		
					_pUIMgr->AddMessageBoxLString( MSGLCMD_PROCESSNPC_ENERGY_REQ, TRUE, _S( 1810, "아 나라고 무조건 정제에 성공하는 것은 아니네. 가끔 실패를 하기도 하지. 물론 실패한다면 수수료는 받지 않네." ), -1, 0xA3A1A3FF );		
					_pUIMgr->AddMessageBoxLString( MSGLCMD_PROCESSNPC_ENERGY_REQ, FALSE, _S( 1811, "E등급 원소 정제" ), SEL_ENERGY_1 );	
					_pUIMgr->AddMessageBoxLString( MSGLCMD_PROCESSNPC_ENERGY_REQ, FALSE, _S( 1812, "D등급 원소 정제" ), SEL_ENERGY_2 );	
					_pUIMgr->AddMessageBoxLString( MSGLCMD_PROCESSNPC_ENERGY_REQ, FALSE, _S( 1813, "C등급 원소 정제" ), SEL_ENERGY_3 );	
					//_pUIMgr->AddMessageBoxLString( MSGLCMD_PROCESSNPC_ENERGY_REQ, FALSE, _S( 1814, "B등급 원소 정제" ), SEL_ENERGY_4 );	
					//_pUIMgr->AddMessageBoxLString( MSGLCMD_PROCESSNPC_ENERGY_REQ, FALSE, _S( 1815, "A등급 원소 정제" ), SEL_ENERGY_5 );	
					_pUIMgr->AddMessageBoxLString( MSGLCMD_PROCESSNPC_ENERGY_REQ, FALSE, _S( 1220, "취소한다." ) );
				}
				break;
				
			case SEL_CROP:			// 식물 가공.
				{
					_pUIMgr->CloseMessageBoxL( MSGLCMD_PROCESSNPC_CROP_REQ );
					
					_pUIMgr->CreateMessageBoxL( _S( 1791, "식물 가공"  ) , UI_PROCESSNPC, MSGLCMD_PROCESSNPC_CROP_REQ );
					
					_pUIMgr->AddMessageBoxLString( MSGLCMD_PROCESSNPC_CROP_REQ, TRUE, _S( 1801, "그래 잘 생각했네." ), -1, 0xA3A1A3FF );		
					_pUIMgr->AddMessageBoxLString( MSGLCMD_PROCESSNPC_CROP_REQ, TRUE, _S( 1816, "자 이제 크락의 어떤 부분을 가공할 것인지 한번 골라보라구." ), -1, 0xA3A1A3FF );		
					_pUIMgr->AddMessageBoxLString( MSGLCMD_PROCESSNPC_CROP_REQ, TRUE, _S( 1817, "아 나라고 무조건 가공에 성공하는 것은 아니네. 가끔 실패를 하기도 하지. 물론 실패한다면 수수료는 받지 않네." ), -1, 0xA3A1A3FF );		
					_pUIMgr->AddMessageBoxLString( MSGLCMD_PROCESSNPC_CROP_REQ, FALSE, _S( 1818, "크락의 노란잎 가공" ), SEL_CROP_1 );	
					_pUIMgr->AddMessageBoxLString( MSGLCMD_PROCESSNPC_CROP_REQ, FALSE, _S( 1819, "크락의 줄기 가공" ), SEL_CROP_2 );	
					_pUIMgr->AddMessageBoxLString( MSGLCMD_PROCESSNPC_CROP_REQ, FALSE, _S( 1820, "크락의 파란잎 가공" ), SEL_CROP_3 );	
					//_pUIMgr->AddMessageBoxLString( MSGLCMD_PROCESSNPC_CROP_REQ, FALSE, _S( 1821, "크락의 가시 가공" ), SEL_CROP_4 );	
					//_pUIMgr->AddMessageBoxLString( MSGLCMD_PROCESSNPC_CROP_REQ, FALSE, _S( 1822, "크락의 꽃 가공" ), SEL_CROP_5 );	
					_pUIMgr->AddMessageBoxLString( MSGLCMD_PROCESSNPC_CROP_REQ, FALSE, _S( 1220, "취소한다." ) );
				}
				break;
			}			
		}
		break;
	case MSGLCMD_PROCESSNPC_MINERAL_REQ:
		{
			switch( nResult )
			{
			case SEL_MINERAL_1:		// 광석 정련
			case SEL_MINERAL_2:		// 광석 정련
			case SEL_MINERAL_3:		// 광석 정련				
			case SEL_MINERAL_4:		// 광석 정련				
			case SEL_MINERAL_5:		// 광석 정련				
				OpenProcessList( SEL_MINERAL, nResult );				
				break;			
			}
		}
		break;
	case MSGLCMD_PROCESSNPC_ENERGY_REQ:
		{
			switch( nResult )
			{
			case SEL_ENERGY_1:		// 원소 정제
			case SEL_ENERGY_2:		// 원소 정제
			case SEL_ENERGY_3:		// 원소 정제
			case SEL_ENERGY_4:		// 원소 정제
			case SEL_ENERGY_5:		// 원소 정제
				{
					OpenProcessList( SEL_ENERGY, nResult );
				}
				break;
			}
		}
		break;
	case MSGLCMD_PROCESSNPC_CROP_REQ:
		{
			switch( nResult )
			{
			case SEL_CROP_1:		// 크락 가공
			case SEL_CROP_2:		// 크락 가공				
			case SEL_CROP_3:		// 크락 가공
			case SEL_CROP_4:		// 크락 가공
			case SEL_CROP_5:		// 크락 가공
				{
					OpenProcessList( SEL_CROP, nResult );
				}
				break;
			}
		}
		break;
	}
}
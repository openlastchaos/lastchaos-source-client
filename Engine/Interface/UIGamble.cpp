#include "stdh.h"
#include <Engine/Interface/UIGamble.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/Items.h>
#include <Engine/Interface/UIInventory.h>

#include <Engine/Entities/Entity.h>
#include <Engine/Entities/EntityClass.h>
#include <Engine/Entities/EntityProperties.h>
#include <Engine/LocalDefine.h>

#define GAMBLE_NUMBER_WIDTH					10
#define GAMBLE_NUMBER_HEIGHT				15
#define	GAMBLE_TIME_SPAN					0.1 // 0.03 -> 0.1

// [071122 Su-won] NEW_MOONSTONE
INDEX MOONSTONEINDEX[MOONSTONE_COUNT]  = {2545, 2546, 723, 2547, 2548};

// ----------------------------------------------------------------------------
// Name : CUIGamble()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIGamble::CUIGamble()
{
	m_strAccumulate		= CTString("");
	m_iAccumulate		= 0;	
	m_pNpcEntity		= NULL;
	m_bSendedStart		= FALSE;
	m_bSelectedFixed	= FALSE;
	m_bIsCashMoonStone	= FALSE;
	m_bIsReady			= FALSE;
	m_bStart			= FALSE;
	m_selCashItemIdx	= -1;


	m_iUsedMoonStone =0;
	m_rtNewMark = NULL;
	m_iTexID = NULL;
}



// ----------------------------------------------------------------------------
// Name : ~CUIGamble()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIGamble::~CUIGamble()
{
	Destroy();

	if (m_iTexID)
	{
		delete [] m_iTexID;
		m_iTexID = NULL;
	}

	if (m_rtNewMark)
	{
		delete [] m_rtNewMark;
		m_rtNewMark = NULL;
	}
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIGamble::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );	

	m_rcMainTitle.SetRect( 0, 0, GAMBLE_MAIN_WIDTH, 150 );
	m_rcSlot[0].SetRect( 7, 103, 52, 148 );
	m_rcSlot[1].SetRect( 56, 103, 101, 148 );
	m_rcSlot[2].SetRect( 105, 103, 150, 148 );
	m_rcSlot[3].SetRect( 155, 103, 200, 148 );
	m_rcSlot[4].SetRect( 204, 103, 249, 148 );	

	// Create shop texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MoonStone.tex" ) );
	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();
	
	// UV Coordinate of each part
	// Background
	// [071122: Su-won] NEW_MOONSTONE
#ifdef NEW_MOONSTONE
	m_rtBackTop.SetUV( 0, 0, GAMBLE_MAIN_WIDTH, 148, fTexWidth, fTexHeight );
	m_rtBackMiddle.SetUV( 0, 149, GAMBLE_MAIN_WIDTH, 166, fTexWidth, fTexHeight );		// 
	m_rtBackBottom.SetUV( 0, 167, GAMBLE_MAIN_WIDTH, GAMBLE_MAIN_HEIGHT, fTexWidth, fTexHeight );
#else
	m_rtBackTop.SetUV( 0, 0, GAMBLE_MAIN_WIDTH, 44, fTexWidth, fTexHeight );
	m_rtBackMiddle.SetUV( 0, 45, GAMBLE_MAIN_WIDTH, 161, fTexWidth, fTexHeight );		// 
	m_rtBackBottom.SetUV( 0, 162, GAMBLE_MAIN_WIDTH, GAMBLE_MAIN_HEIGHT, fTexWidth, fTexHeight );
#endif

	m_btnStart.Create( this, CTString(""), 8, 167, 113, 29 );
	m_btnStart.SetUV( UBS_IDLE, 0, 374, 112, 402, fTexWidth, fTexHeight ); // 330 -> 374 , 44
	m_btnStart.SetUV( UBS_CLICK, 114, 374, 226, 402, fTexWidth, fTexHeight );
	m_btnStart.CopyUV( UBS_IDLE, UBS_ON );
	m_btnStart.CopyUV( UBS_IDLE, UBS_DISABLE );

	// [071122: Su-won] NEW_MOONSTONE
#ifdef NEW_MOONSTONE
	m_btnStop.Create( this, CTString(""), 8, 167+26, 113, 29 );
#else
	m_btnStop.Create( this, CTString(""), 8, 167, 113, 29 );
#endif
	m_btnStop.SetUV( UBS_IDLE, 0, 404, 112, 432, fTexWidth, fTexHeight );
	m_btnStop.SetUV( UBS_CLICK, 114, 404, 226, 432, fTexWidth, fTexHeight );
	m_btnStop.CopyUV( UBS_IDLE, UBS_ON );
	m_btnStop.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Cancel button of shop
	// [071122: Su-won] NEW_MOONSTONE
#ifdef NEW_MOONSTONE
	m_btnCancel.Create( this, CTString(""), 135, 167+26, 113, 29 );
#else
	m_btnCancel.Create( this, CTString(""), 135, 167, 113, 29 );
#endif
	m_btnCancel.SetUV( UBS_IDLE, 0, 434, 112, 462, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 114, 434, 226, 462, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Button Ready
	m_btnReady.Create( this, CTString(""), 8, 167, 113, 29 );
	m_btnReady.SetUV( UBS_IDLE, 0, 464, 112, 492, fTexWidth, fTexHeight );
	m_btnReady.SetUV( UBS_CLICK, 114, 464, 226, 492, fTexWidth, fTexHeight );
	m_btnReady.CopyUV( UBS_IDLE, UBS_ON );
	m_btnReady.CopyUV( UBS_IDLE, UBS_DISABLE );


	for( int i = 0 ; i < GAMBLE_TOTAL_NUMBER; ++i )
	{
		m_rtSmallNumber[i].SetUV( i * GAMBLE_NUMBER_WIDTH, 358, 
								i * GAMBLE_NUMBER_WIDTH + GAMBLE_NUMBER_WIDTH, 368, fTexWidth, fTexHeight );
	}

	m_rtSelect.SetUV( 92, 262, 137, 307, fTexWidth, fTexHeight );	

// [071122: Su-won] NEW_MOONSTONE
#ifdef NEW_MOONSTONE
	for(i=0; i<5; ++i)
	{
		m_btnMoonStone[i].Create( this, 17+i*49, 158, 25, 25 );
	}

	ReadMoonStoneLOD();
#endif

}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIGamble::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI ) / 2 - GetWidth(), ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIGamble::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : OpenGamble()
// Desc :
// ----------------------------------------------------------------------------
void CUIGamble::OpenGamble( CEntity *pNpcEntity , BOOL bIsCashMoonStone)
{
	ASSERT( pNpcEntity != NULL && "Invalid Npc Entity!" );
	if( IsVisible() || !pNpcEntity )
		return;

	Init();

	// Set position of target npc
	m_pNpcEntity	= pNpcEntity;

	// Set cash moonstone
	m_bIsCashMoonStone = bIsCashMoonStone;

	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();
	
	if(!m_bIsCashMoonStone)
	{
		m_rtMark[0].SetUV( 0, 216, 45, 261, fTexWidth, fTexHeight );
		m_rtMark[1].SetUV( 46, 216, 91, 261, fTexWidth, fTexHeight );
		m_rtMark[2].SetUV( 92, 216, 137, 261, fTexWidth, fTexHeight );
		m_rtMark[3].SetUV( 138, 216, 183, 261, fTexWidth, fTexHeight );
		m_rtMark[4].SetUV( 184, 216, 229, 261, fTexWidth, fTexHeight );
		m_rtMark[5].SetUV( 0, 262, 45, 307, fTexWidth, fTexHeight );
		m_rtMark[6].SetUV( 46, 262, 91, 307, fTexWidth, fTexHeight );
		_pNetwork->SendMoonStoneStartReq();
	}
	else 
	{
		//Reset Slot UV
		m_rtMark[0].SetUV( 138, 262, 183, 307, fTexWidth, fTexHeight );
		m_rtMark[1].SetUV( 184, 262, 229, 307, fTexWidth, fTexHeight );
		m_rtMark[2].SetUV( 0, 308, 45, 352, fTexWidth, fTexHeight );
		m_rtMark[3].SetUV( 46, 308, 91, 352, fTexWidth, fTexHeight );
		m_rtMark[4].SetUV( 92, 308, 137, 352, fTexWidth, fTexHeight );
		m_rtMark[5].SetUV( 138, 308, 183, 352, fTexWidth, fTexHeight );
		m_rtMark[6].SetUV( 184, 308, 229, 352, fTexWidth, fTexHeight );
		_pUIMgr->GetGamble()->SetAccumulate( 0 );
		_pUIMgr->GetGamble()->ShowGamble();
		_pUIMgr->SetCSFlagOn( CSF_MOONSTONE );
	}

// [071122: Su-won] NEW_MOONSTONE
#ifdef NEW_MOONSTONE
	for(int i=0; i<5; ++i)
	{
		m_btnMoonStone[i].SetItemInfo( 0, 0, 0, MOONSTONEINDEX[i], -1, -1);
		
		CItemData	&rItemData = _pNetwork->GetItemData( MOONSTONEINDEX[2] );

		int nTextureID = rItemData.GetIconTexID();
		int	nTexRow = rItemData.GetIconTexRow();
		int	nTexCol = rItemData.GetIconTexCol();
		int nUVSX = BTN_SIZE *nTexCol;
		int nUVSY = BTN_SIZE *nTexRow;

		FLOAT	fTexWidth = _pUIBtnTexMgr->GetTexWidth( UBET_ITEM, nTextureID );
		FLOAT	fTexHeight = _pUIBtnTexMgr->GetTexHeight( UBET_ITEM, nTextureID );
		
		m_rtNewMark[i].SetUV( nUVSX, nUVSY, nUVSX + BTN_SIZE, nUVSY + BTN_SIZE, fTexWidth, fTexHeight );

		m_iTexID[i] =nTextureID;
	}
#endif
}

//////////////////////////////////////////////////////////////////////////
// Name : OpenReady()
// Desc : 
// Date : [6/20/2006] , Wooss
//////////////////////////////////////////////////////////////////////////

void CUIGamble::OpenReady()
{
	if(_pUIMgr->DoesMessageBoxExist(MSGCMD_GAMBLE_READY)) return;
	// Set MessageBox
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S(2909, "캐시 문스톤 미니게임 준비"), UMBS_YESNO|UMBS_BUTTONEX, UI_GAMBLE, MSGCMD_GAMBLE_READY );
	MsgBoxInfo.SetBtnType( UBET_ITEM, -1, -1 );
	
	MsgBoxInfo.AddString( _S(2910,"게임에 사용할 캐시아이템을 넣어 주십시오. 미니 게임의 결과에 따라 다른 캐시 아이템으로 교환 됩니다.") );
	_pUIMgr->CreateMessageBox( MsgBoxInfo );
	_pUIMgr->GetMessageBox(MSGCMD_GAMBLE_READY)->SetPosY(_pUIMgr->GetInventory()->GetPosY());
	// Open Inventory
	_pUIMgr->RearrangeOrder(UI_INVENTORY,TRUE);
	_pUIMgr->GetInventory()->SetPosX(
		_pUIMgr->GetMessageBox(MSGCMD_GAMBLE_READY)->GetAbsPosX()+
		_pUIMgr->GetMessageBox(MSGCMD_GAMBLE_READY)->GetWidth());

	// Invisible CashMoonStone UI
	this->SetVisible(FALSE);

}

//////////////////////////////////////////////////////////////////////////
// Name : OpenReady()
// Desc : 
// Date : [6/20/2006] , Wooss
//////////////////////////////////////////////////////////////////////////

void CUIGamble::OpenReadySelect()
{
	if(_pUIMgr->DoesMessageBoxExist(MSGCMD_GAMBLE_READYSEL)) {
		_pUIMgr->CloseMessageBox(MSGCMD_GAMBLE_READYSEL);
		return;
	}
	// Set MessageBox
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S(2911,"묶음 개수 선택창"), UMBS_SELECTBOX, UI_GAMBLE, MSGCMD_GAMBLE_READYSEL );
	_pUIMgr->CreateMessageBox( MsgBoxInfo );
	int tv_posY=_pUIMgr->GetMessageBox(MSGCMD_GAMBLE_READY)->GetAbsPosY()
		+_pUIMgr->GetMessageBox(MSGCMD_GAMBLE_READY)->GetHeight();
	_pUIMgr->GetMessageBox(MSGCMD_GAMBLE_READYSEL)->SetPosY(tv_posY);

}
//---------------------------------------------------------------------->>
// Name : CheckCashItem()
// Desc :
// Date : [6/25/2006] , Wooss
//----------------------------------------------------------------------<<
BOOL CUIGamble::CheckCashItem(int itemIdx)
{
	BOOL tv_chk = FALSE;

	// 예외 처리....감정된 플래티늄 제련석의 경우 감전전 플래티늄으로 대체 
	// 플래티늄 제련석 하드 코딩....
	if(itemIdx == 1419) itemIdx = 1418;

	OpenReadySelect();
	m_cashItemList.clear();

	for(int i=0 ; i< CASH_SHOP_TOTAL ; i++)
	{
		// exclude set items
		if(i == SET_ITEM ) continue;

		CCashShopData& CD_CHK = _pNetwork->GetCashShopData(i);
		std::vector<CCashShopData::CASH_SHOP_DATA>::iterator tv_begin,tv_end;
		tv_begin = CD_CHK.m_vShopItemArray.begin();
		tv_end = CD_CHK.m_vShopItemArray.end();
		while(tv_begin != tv_end){
			// 리스트가 하나인것 (이미 세트 아이템을 제외 했지만서도...한번 더
			if( tv_begin->m_itemListCnt == 1 )
			{
				if( tv_begin->m_vItemDataArray[0].m_itemIndex == itemIdx) {
					CASHITEMS tv_item;
					tv_item.cashIdx		=	tv_begin->m_shopItemIndex;
					tv_item.cashName	=	tv_begin->m_itemName;
					tv_item.cashCnt		=	tv_begin->m_vItemDataArray[0].m_itemCnt;
					m_cashItemList.push_back(tv_item);
					tv_chk = TRUE;

					// Add context
					CTString tv_str;
					tv_str.PrintF("%s(%d)",tv_item.cashName,tv_item.cashCnt);
					_pUIMgr->GetMessageBox(MSGCMD_GAMBLE_READYSEL)->addSelectContext(tv_str); 
				}
			}
			tv_begin++;
		}
	}
	if(!tv_chk){
		_pUIMgr->CloseMessageBox(MSGCMD_GAMBLE_READYSEL);
	}
	return tv_chk;

}


//////////////////////////////////////////////////////////////////////////
// Name : MsgBoxCommand()
// Desc :
// Date : [6/20/2006] , Wooss
//////////////////////////////////////////////////////////////////////////
void CUIGamble::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput)
{
	// Select Yes/OK
	switch(nCommandCode)
	{
		case MSGCMD_GAMBLE_READY :
		{
			if(bOK)
			{
				if(!m_slotBtn.IsEmpty())
				{
					m_bIsReady = TRUE;
					m_btnStart.SetEnable(TRUE);
				}
			}
			
			_pUIMgr->CloseMessageBox(MSGCMD_GAMBLE_READY);
			_pUIMgr->CloseMessageBox(MSGCMD_GAMBLE_READYSEL);
			
		}
		break;
	}

	// after Click 
	_pUIMgr->RearrangeOrder(UI_GAMBLE,TRUE);
}


// ----------------------------------------------------------------------------
// Name : ShowGamble()
// Desc :
// ----------------------------------------------------------------------------
void CUIGamble::ShowGamble()
{	
	_pUIMgr->RearrangeOrder( UI_GAMBLE, TRUE );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIGamble::Init()
{
	for( int i = 0; i < GAMBLE_TOTAL_SLOT; ++i )
	{
		m_dSlotOld[i]			= 0;
		m_dSlotElapsed[i]		= 0;
		m_iSelectedMarker[i]	= -1;
		m_iCurMarker[i]			= i;	
		m_iStoppedMarker[i]		= FALSE;
	}
	memset( &m_iMarkerScore[0], 0, sizeof(int) * GAMBLE_TOTAL_MARK );	
	
	m_iStopSlot				= 0;
	m_iMaxMarker			= -1;
	Stop();	
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIGamble::ResetGamble()
{
	if(m_bIsCashMoonStone)
	{
		_pUIMgr->CloseMessageBox(MSGCMD_GAMBLE_READY);
		_pUIMgr->CloseMessageBox(MSGCMD_GAMBLE_READYSEL);
		_pUIMgr->RearrangeOrder( UI_INVENTORY, FALSE );	
	}

	if( _pUIMgr->IsCSFlagOn( CSF_MOONSTONE ) )
	{
		_pUIMgr->SetCSFlagOff( CSF_MOONSTONE );
	}
	m_pNpcEntity			= NULL;
	m_bSendedStart			= FALSE;
	m_bIsReady				= FALSE;
	m_bStart				= FALSE;
	m_bIsCashMoonStone		= FALSE;
	_pUIMgr->RearrangeOrder( UI_GAMBLE, FALSE );
	m_cashItemList.clear();
	m_selCashItemIdx	= -1;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIGamble::Start()
{	
	m_bStart		= TRUE;	
	m_bSendedStart	= FALSE;

// [071122: Su-won] NEW_MOONSTONE
#ifdef NEW_MOONSTONE
	for(int i=0; i<m_saReward[m_iUsedMoonStone].Count(); ++i)
	{
		INDEX iItem =m_saReward[m_iUsedMoonStone][i];
		CItemData	&rItemData = _pNetwork->GetItemData( iItem );

		int nTextureID = rItemData.GetIconTexID();
		int	nTexRow = rItemData.GetIconTexRow();
		int	nTexCol = rItemData.GetIconTexCol();
		int nUVSX = BTN_SIZE *nTexCol;
		int nUVSY = BTN_SIZE *nTexRow;

		FLOAT	fTexWidth = _pUIBtnTexMgr->GetTexWidth( UBET_ITEM, nTextureID );
		FLOAT	fTexHeight = _pUIBtnTexMgr->GetTexHeight( UBET_ITEM, nTextureID );
		
		m_rtNewMark[i].SetUV( nUVSX, nUVSY, nUVSX + BTN_SIZE, nUVSY + BTN_SIZE, fTexWidth, fTexHeight );

		m_iTexID[i] =nTextureID;
	}
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIGamble::Stop()
{
	m_bStart	= FALSE;
	m_bBegin	= FALSE;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIGamble::SetSelectedMarker( int iSlot, int iCount )
{
	const int iSelectedNumber	= iSlot;

	for( int t = 0; t < iCount; ++t )
	{
		m_iSelectedMarker[t]	= iSlot;
	}

	m_bSelectedFixed	= (iCount == 1) ? TRUE : FALSE;

	if( iCount != GAMBLE_TOTAL_SLOT )
	{		
		// 나머지 슬롯을 결정함.
		for( int i = iCount; i < GAMBLE_TOTAL_SLOT ; ++i )
		{
			int iNewNumber	= 0;
			do
			{
				iNewNumber = rand() % GAMBLE_TOTAL_MARK;
				
				// 랜덤으로 돌려야 되는 슬롯이 2개 이상있으며...
				if( GAMBLE_TOTAL_SLOT - iCount > 1 )
				{
					// 얻은 랜덤값과 같은 슬롯이 있는지 확인함...
					for( int j = iCount; j < i; ++j )
					{
						if( m_iSelectedMarker[j] == iNewNumber )
						{
							// while문에서 걸리도록 하기 위해서...
							iNewNumber	= iSelectedNumber;
							break;
						}
					}
				}
			}
			while( iSelectedNumber == iNewNumber );
			m_iSelectedMarker[i] = iNewNumber;
		}

		// 뒤섞기~!!!
		// NOTE : 1개일때는 제외!!!
		// NOTE : 1개일 때는 무조건 첫번째값이 표시가 되어야 하기 때문에...
		// NOTE : 4개의 랜덤값을 얻은 이후, 2개씩 교환을 합니다.
		if( iCount != 1 )
		{
			int iRand[4];
			for( int i = 0; i < 4; ++i )
			{
				iRand[i] = rand() % GAMBLE_TOTAL_SLOT;
			}
			
			// Swap
			{
				int iTemp = m_iSelectedMarker[iRand[0]];
				m_iSelectedMarker[iRand[0]] = m_iSelectedMarker[iRand[1]];
				m_iSelectedMarker[iRand[1]]	= iTemp;
			}
			
			// Swap
			{
				int iTemp = m_iSelectedMarker[iRand[2]];
				m_iSelectedMarker[iRand[2]] = m_iSelectedMarker[iRand[3]];
				m_iSelectedMarker[iRand[3]]	= iTemp;
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 누적수치를 셋팅합니다.
//-----------------------------------------------------------------------------
void CUIGamble::SetAccumulate(int iAccumulate)
{
	m_iAccumulate		= iAccumulate;
	m_strAccumulate.PrintF( "%d", m_iAccumulate );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIGamble::ShowPrizeEffect()
{
	ASSERT( m_pNpcEntity != NULL && "Invalid Npc Entity!" );
	if( m_pNpcEntity )
	{
		// 이펙트 표시...
		StartEffectGroup("npcGamblingWin", &m_pNpcEntity->GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
	}
}

// ----------------------------------------------------------------------------
// Name : DrawNumber()
// Desc : 오른쪽 정렬 형태로 출력...
// ----------------------------------------------------------------------------
void CUIGamble::DrawNumber( int x, int y, CTString &strNumber )
{
	// Get length of string
	INDEX	nLength = strNumber.Length();
	if( nLength == 0 )
		return;

	int iStartPos = x - GAMBLE_NUMBER_WIDTH * nLength;

	// Check line character
	for( int iPos = 0; iPos < nLength; iPos++ )
	{
		CTString strTemp;
		strTemp.PrintF("%c", strNumber[iPos] );
		int iNumber = atoi( strTemp );
		DrawNumber( iStartPos + GAMBLE_NUMBER_WIDTH * iPos, y, iNumber );
	}
}

// ----------------------------------------------------------------------------
// Name : DrawNumber()
// Desc :
// ----------------------------------------------------------------------------
void CUIGamble::DrawNumber( int x, int y, int nNumber )
{
	ASSERT( nNumber >= 0 && nNumber <= 9 );
	UIRectUV rtNumber	= m_rtSmallNumber[nNumber];
	int nWidth			= GAMBLE_NUMBER_WIDTH;
	int nHeight			= GAMBLE_NUMBER_HEIGHT;
	
	_pUIMgr->GetDrawPort()->AddTexture( x, y, x + nWidth, y + nHeight,
							rtNumber.U0, rtNumber.V0, rtNumber.U1, rtNumber.V1,
							0xFFFFFFFF );
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIGamble::Render()
{
	// Set shop texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	int	nX, nY, nX2, nY2;

	nX		= m_nPosX;
	nX2		= nX + m_nWidth;

// [071122: Su-won] NEW_MOONSTONE
#ifdef NEW_MOONSTONE
	nY		= m_nPosY;	
	nY2		= nY + 148;

	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX2, nY2,
		m_rtBackTop.U0, m_rtBackTop.V0,
		m_rtBackTop.U1, m_rtBackTop.V1,
		0xFFFFFFFF );
	
	nY		= nY2;
	nY2		= nY +45;

	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX2, nY2,
		m_rtBackMiddle.U0, m_rtBackMiddle.V0,
		m_rtBackMiddle.U1, m_rtBackMiddle.V1,
		0xFFFFFFFF );

	//nY		= nY2;
	//nY2		= nY +19;

	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX2, nY2,
		m_rtBackMiddle.U0, m_rtBackMiddle.V0,
		m_rtBackMiddle.U1, m_rtBackMiddle.V1,
		0xFFFFFFFF );

	nY		= nY2;

	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX2, m_nPosY + m_nHeight+19,
		m_rtBackBottom.U0, m_rtBackBottom.V0,
		m_rtBackBottom.U1, m_rtBackBottom.V1,
		0xFFFFFFFF );
		
#else
	nY		= m_nPosY;	
	nY2		= nY + 44;

	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX2, nY2,
		m_rtBackTop.U0, m_rtBackTop.V0,
		m_rtBackTop.U1, m_rtBackTop.V1,
		0xFFFFFFFF );
	
	nY		= nY2;
	nY2		= m_nPosY + m_nHeight - 51;

	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX2, nY2,
		m_rtBackMiddle.U0, m_rtBackMiddle.V0,
		m_rtBackMiddle.U1, m_rtBackMiddle.V1,
		0xFFFFFFFF );

	nY		= nY2;

	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX2, m_nPosY + m_nHeight,
		m_rtBackBottom.U0, m_rtBackBottom.V0,
		m_rtBackBottom.U1, m_rtBackBottom.V1,
		0xFFFFFFFF );
#endif
	
	if(m_bIsCashMoonStone)
	{
		if(!m_bStart)
		{
			if( !m_bIsReady )
			{
				m_btnStart.SetEnable(FALSE);
				m_btnReady.SetEnable(TRUE);
				m_btnReady.Render();
			}
			else
			{	
				m_btnReady.SetEnable(FALSE);
				m_btnStart.SetEnable(TRUE);
				m_btnStart.Render();
			}
		}
		else
			m_btnStop.Render();

	}
	else if( !m_bStart ){
// [071122: Su-won] NEW_MOONSTONE
#ifdef NEW_MOONSTONE
		m_btnStart.SetEnable(FALSE);
		m_btnStop.Render();
#else
		m_btnStart.SetEnable(TRUE);
		m_btnStart.Render();
#endif
	}
	else
		m_btnStop.Render();
	
	// Cancel button of shop
	m_btnCancel.Render();	

	for( int i = 0; i < GAMBLE_TOTAL_SLOT; ++i )
	{
		if( m_bStart )
		{		
			DOUBLE	dCurTime	= _pTimer->GetHighPrecisionTimer().GetSeconds();
			m_dSlotElapsed[i]	+= dCurTime - m_dSlotOld[i];
			m_dSlotOld[i]		= dCurTime;
			
			if( m_dSlotElapsed[i] > GAMBLE_TIME_SPAN && !m_iStoppedMarker[i] )
			{
				m_iCurMarker[i]++;

// [071122: Su-won] NEW_MOONSTONE
#ifdef NEW_MOONSTONE
				m_iCurMarker[i] = m_iCurMarker[i] % m_saReward[m_iUsedMoonStone].Count();
#else
				m_iCurMarker[i] = m_iCurMarker[i] % GAMBLE_TOTAL_MARK;
#endif
				do
				{
					m_dSlotElapsed[i] -= GAMBLE_TIME_SPAN;
				}
				while( m_dSlotElapsed[i] > GAMBLE_TIME_SPAN );

				if( i == m_iStopSlot )
				{
					if( m_bBegin && m_iCurMarker[i] == m_iSelectedMarker[i] )
					{
						m_iStoppedMarker[i]	= TRUE;
// [071122: Su-won] NEW_MOONSTONE
#ifndef NEW_MOONSTONE
						m_iMarkerScore[m_iSelectedMarker[i]]++;
#endif

						// 마지막 슬롯까지 다 정지했다면...
						if( m_iStopSlot == GAMBLE_TOTAL_SLOT - 1 )
						{
							Stop();
							if(!m_bIsCashMoonStone)
								_pNetwork->SendMoonStoneResultReq();
							else{
								_pNetwork->SendCashMoonStoneReward();
							}
												
							int iMaxScore	= 0;							
							for( int t = 0; t < GAMBLE_TOTAL_MARK; ++t )
							{
								if( m_iMarkerScore[t] > iMaxScore )
								{									
									iMaxScore		= m_iMarkerScore[t];
									m_iMaxMarker	= t;
								}
								else if( m_iMarkerScore[t] == iMaxScore )
								{
									if( m_iMaxMarker <= t )
									{
										iMaxScore		= m_iMarkerScore[t];
										m_iMaxMarker	= t;
									}
								}
							}
						}

						m_iStopSlot++;
					}
				}
			}
		}
		RenderSlot( i, m_iCurMarker[i] );

// [071122: Su-won] NEW_MOONSTONE
#ifndef NEW_MOONSTONE
		RenderSelectedSlot();
#endif

	}
	
	if(m_bIsCashMoonStone)
	{
		CTString tv_str;
		if(m_bIsReady)
			tv_str=CTString("Press Start Button!");
		else
			tv_str=CTString("Insert Item...");
		_pUIMgr->GetDrawPort()->PutTextExCX(tv_str,m_nPosX+129,m_nPosY+80);

	}
	else
	{
		DrawNumber( m_nPosX + 190, m_nPosY + 80, m_strAccumulate );
	}

	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

// [071122: Su-won] NEW_MOONSTONE
#ifdef NEW_MOONSTONE
	m_btnMoonStone[0].Render();
	m_btnMoonStone[1].Render();
	m_btnMoonStone[2].Render();
	m_btnMoonStone[3].Render();
	m_btnMoonStone[4].Render();

	_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_ITEM, PBT_OPAQUE );

	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );
	RenderSelectedSlot();
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();
#endif

	// Render Text
	_pUIMgr->GetDrawPort()->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : RenderSlot()
// Desc :
// ----------------------------------------------------------------------------
void CUIGamble::RenderSlot( int iSlot, int iMarker )
{
	if( iSlot == -1 )
		return;

	if( iMarker == -1 )
		iMarker = 0;

	int iCurSlot	= iSlot % GAMBLE_TOTAL_SLOT;
	int iCurMarker	= iMarker % GAMBLE_TOTAL_MARK;

// [071122: Su-won] NEW_MOONSTONE
#ifdef NEW_MOONSTONE
	iCurMarker	= iMarker % m_saReward[m_iUsedMoonStone].Count();

	_pUIMgr->GetDrawPort()->AddBtnTexture( m_iTexID[iCurMarker],
								m_nPosX + m_rcSlot[iCurSlot].Left, m_nPosY + m_rcSlot[iCurSlot].Top, m_nPosX + m_rcSlot[iCurSlot].Right, m_nPosY + m_rcSlot[iCurSlot].Bottom, 
								m_rtNewMark[iCurMarker].U0, m_rtNewMark[iCurMarker].V0, m_rtNewMark[iCurMarker].U1, m_rtNewMark[iCurMarker].V1, 
								0xFFFFFFFF );
#else
	_pUIMgr->GetDrawPort()->AddTexture( 
		m_nPosX + m_rcSlot[iCurSlot].Left, m_nPosY + m_rcSlot[iCurSlot].Top, m_nPosX + m_rcSlot[iCurSlot].Right, m_nPosY + m_rcSlot[iCurSlot].Bottom, 
		m_rtMark[iCurMarker].U0, m_rtMark[iCurMarker].V0, m_rtMark[iCurMarker].U1, m_rtMark[iCurMarker].V1, 
		0xFFFFFFFF );
#endif

}

// ----------------------------------------------------------------------------
// Name : RenderSelectedSlot()
// Desc :
// ----------------------------------------------------------------------------
void CUIGamble::RenderSelectedSlot( )
{
	if( m_iMaxMarker == -1 )
		return;

// [071122: Su-won] NEW_MOONSTONE
#ifdef NEW_MOONSTONE
	_pUIMgr->GetDrawPort()->AddTexture( 
			m_nPosX + m_rcSlot[2].Left, m_nPosY + m_rcSlot[2].Top, m_nPosX + m_rcSlot[2].Right, m_nPosY + m_rcSlot[2].Bottom, 
			m_rtSelect.U0, m_rtSelect.V0, m_rtSelect.U1, m_rtSelect.V1, 
			0xFFFFFFFF );

#else
	if( m_bSelectedFixed )
	{
		_pUIMgr->GetDrawPort()->AddTexture( 
			m_nPosX + m_rcSlot[0].Left, m_nPosY + m_rcSlot[0].Top, m_nPosX + m_rcSlot[0].Right, m_nPosY + m_rcSlot[0].Bottom, 
			m_rtSelect.U0, m_rtSelect.V0, m_rtSelect.U1, m_rtSelect.V1, 
			0xFFFFFFFF );	
	}
	else
	{
		for( int i = 0; i < GAMBLE_TOTAL_SLOT; ++i )
		{
			if( m_iSelectedMarker[i] == m_iMaxMarker )
			{
				_pUIMgr->GetDrawPort()->AddTexture( 
					m_nPosX + m_rcSlot[i].Left, m_nPosY + m_rcSlot[i].Top, m_nPosX + m_rcSlot[i].Right, m_nPosY + m_rcSlot[i].Bottom, 
					m_rtSelect.U0, m_rtSelect.V0, m_rtSelect.U1, m_rtSelect.V1, 
					0xFFFFFFFF );	
			}
		}
	}
#endif
	
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIGamble::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL	bTitleBarClick = FALSE;

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
			
			// Sell button
			if( !m_bStart && m_btnStart.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_bStart && m_btnStop.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Cancel button
			else if( !m_bStart && m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Ready Button
			else if( !m_bIsReady && m_btnReady.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;

// [071122: Su-won] NEW_MOONSTONE
#ifdef NEW_MOONSTONE
			for(int i=0; i<5; ++i)
			{
				if( !m_bStart && m_btnMoonStone[i].MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
#endif
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				nOldX = nX;		nOldY = nY;
				
				// Title bar
				if( IsInsideRect( nX, nY, m_rcMainTitle ) )
				{
					bTitleBarClick = TRUE;
				}
				else if( !m_bStart && m_btnStart.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( m_bStart && m_btnStop.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Cancel Button of shop
				else if( !m_bStart && m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( !m_bIsReady && m_btnReady.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}

// [071122: Su-won] NEW_MOONSTONE
#ifdef NEW_MOONSTONE
				for(int i=0; i<5; ++i)
				{
					if( !m_bStart && m_btnMoonStone[i].MouseMessage( pMsg ) != WMSG_FAIL )
					{
						// Nothing
					}
				}
#endif

				if(this->IsVisible())
					_pUIMgr->RearrangeOrder( UI_GAMBLE, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			bTitleBarClick = FALSE;

			// If refine isn't focused
		 	if( !IsFocused() )
				return WMSG_FAIL;	
			
			if( !m_bStart && ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult != WMSG_FAIL )
					ResetGamble();
				
				return WMSG_SUCCESS;
			}
			else if( !m_bStart && ( wmsgResult = m_btnStart.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( !m_bSendedStart )
				{
					if(m_bIsCashMoonStone)
					{								
						m_bIsReady = FALSE;
						_pNetwork->SendCashMoonStoneReq(m_slotBtn);
				
					}
					else
					{
						_pNetwork->SendMoonStoneTryReq();
					}
					
					Init();
					m_bSendedStart	= TRUE;						
				}
				return WMSG_SUCCESS;
			}
			else if( m_bStart && ( wmsgResult = m_btnStop.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND )
				{
					m_bBegin	= TRUE;
		
				}
				return WMSG_SUCCESS;
			}
			else if( m_bIsCashMoonStone && !m_bIsReady && ( wmsgResult = m_btnReady.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				Init();
				OpenReady();
			}

// [071122: Su-won] NEW_MOONSTONE
#ifdef NEW_MOONSTONE
			for(int i=0; i<5; ++i)
			{
				if( !m_bStart && m_btnMoonStone[i].MouseMessage( pMsg ) != WMSG_FAIL )
				{
					if( !m_bSendedStart )
					{
						m_iUsedMoonStone =i;

						_pNetwork->SendMoonStoneTryReq();

						Init();
						m_bSendedStart	= TRUE;
					}
					return WMSG_SUCCESS;					
				}
			}
#endif
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
			{					
			}
		}
		break;
	}

	return WMSG_FAIL;
}


/////////////////////////////////////////////////////////////////////////
// [071122: Su-won] NEW_MOONSTONE
void CUIGamble::ReadMoonStoneLOD()
{
	FILE* fMoonStone = NULL;

	CTString strFullPath = _fnmApplicationPath.FileDir();
	CTString tPath = strFullPath + "data\\MoonStone.lod";

	INDEX iMax = 0;

	if( fMoonStone = fopen(tPath.str_String ,"rb") )
	{
		for( int i=0; i<5; ++i)
		{
			int nCount;
			fread(&nCount,sizeof(int),1,fMoonStone);

			m_saReward[i].New(nCount);

			if (nCount > iMax) iMax = nCount; // 최대 갯수

			for(int j=0; j<nCount; ++j)
				fread(&m_saReward[i][j],sizeof(int),1,fMoonStone);
		}
	}

	ASSERT(iMax > 0 && "MoonStone Reward Count 0");

	if (iMax > 0)
	{
		m_iTexID = new INDEX[iMax];
		m_rtNewMark = new UIRectUV[iMax];
	}
}

void CUIGamble::SetSelectedItem( INDEX iRewardItem )
{
	int iRewardCount =m_saReward[m_iUsedMoonStone].Count();

	UBYTE ubReward =0;

	for(int i=0; i<iRewardCount; ++i)
	{
		if( m_saReward[m_iUsedMoonStone][i] == iRewardItem )
		{
			ubReward =i;
			break;
		}
	}

	m_iSelectedMarker[0] = ubReward-2;
	m_iSelectedMarker[1] = ubReward-1;
	m_iSelectedMarker[2] = ubReward;
	m_iSelectedMarker[3] = ubReward+1;
	m_iSelectedMarker[4] = ubReward+2;

	if( m_iSelectedMarker[0] <0 )
		m_iSelectedMarker[0] += iRewardCount;

	if( m_iSelectedMarker[1] <0 )
		m_iSelectedMarker[1] += iRewardCount;

	if( m_iSelectedMarker[3] >=iRewardCount )
		m_iSelectedMarker[3] -= iRewardCount;

	if( m_iSelectedMarker[4] >=iRewardCount )
		m_iSelectedMarker[4] -= iRewardCount;
}

INDEX CUIGamble::GetUsedMoonStoneIndex()
{ 
	return MOONSTONEINDEX[m_iUsedMoonStone]; 
}
// [071122: Su-won] NEW_MOONSTONE
/////////////////////////////////////////////////////////////////////////
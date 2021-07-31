#include "stdh.h"
#include <vector>
#include <Engine/Interface/UIGuildStash.h>
#include <Engine/Interface/UIInternalClasses.h>

#define	GUILDSTASH_TITLE_TEXT_OFFSETX		25
#define	GUILDSTASH_TITLE_TEXT_OFFSETY		5

// Define size of skill learn
#define	GUILDSTASH_WIDTH				243
#define	GUILDSTASH_HEIGHT				214

#define GUILDSTASH_TOP_HEIGHT			26
#define GUILDSTASH_GAP_HEIGHT			26
#define GUILDSTASH_BOTTOM_HEIGHT		7		

#define TABLE_TITLE_HEIGHT				18
#define TABLE_MIDDLE_HEIGHT				149 - 18 - 18
#define TABLE_BOTTOM_HEIGHT				18

#define TALBE_LEFT_GAP_WIDTH			8	
#define TALBE_TITLE_WIDTH				60
#define TALBE_CONTENTS_WIDTH			GUILDSTASH_WIDTH - 8 - 8
#define TALBE_RIGHT_GAP_WIDTH			8

#define SEL_VIEW	0
#define SEL_TAKE	1


//------------------------------------------------------------------------------
// CUIGuildStash::CUIGuildStash
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CUIGuildStash::CUIGuildStash()
{	
	GameState = GSS_NPC;
	m_strTitle = "Stash";
}


//------------------------------------------------------------------------------
// CUIGuildStash::~CUIGuildStash
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CUIGuildStash::~CUIGuildStash()
{
	Destroy();
}


//------------------------------------------------------------------------------
// CUIGuildStash::Create
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildStash::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( GUILDSTASH_WIDTH, GUILDSTASH_HEIGHT );
	
	// Region of each part
	m_rcTitle.SetRect( 0, 0, 236, 22 );
	
	// Create skill learn texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MessageBox.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();
	
	// UV Coordinate of each part
	// Background

	m_rtTopL.SetUV( 0, 0, 40, 26, fTexWidth, fTexHeight );
	m_rtTopM.SetUV( 40, 0, 176, 26, fTexWidth, fTexHeight );
	m_rtTopR.SetUV( 176, 0, 216, 26, fTexWidth, fTexHeight );
	
	m_rtMiddleL.SetUV( 0, 31, 40, 33, fTexWidth, fTexHeight );
	m_rtMiddleM.SetUV( 40, 31, 176, 33, fTexWidth, fTexHeight );
	m_rtMiddleR.SetUV( 176, 31, 216, 33, fTexWidth, fTexHeight );

	m_rtMiddleGapL.SetUV( 0, 35, 40, 37, fTexWidth, fTexHeight );
	m_rtMiddleGapM.SetUV( 40, 35, 176, 37, fTexWidth, fTexHeight );
	m_rtMiddleGapR.SetUV( 176, 35, 216, 37, fTexWidth, fTexHeight );

	m_rtBottomL.SetUV( 0, 38, 40, 45, fTexWidth, fTexHeight );
	m_rtBottomM.SetUV( 40, 38, 176, 45, fTexWidth, fTexHeight );
	m_rtBottomR.SetUV( 176, 38, 216, 45, fTexWidth, fTexHeight );

	m_rtTableTitle.SetUV3( 36, 69, 44, 87, 45, 69, 215, 87, 244, 69, 252, 87, fTexWidth, fTexHeight );
	m_rtTableMiddle.SetUV3( 36, 73, 44, 82, 45, 73, 215, 82, 244, 73, 252, 82, fTexWidth, fTexHeight );	
	m_rtTableBottom.SetUV3( 36, 71, 44, 87, 45, 71, 215, 87, 244, 71, 252, 87, fTexWidth, fTexHeight );

	m_rtTableGrid.SetUV( 43, 69, 44, 87, fTexWidth, fTexHeight );


	// Ok button
	m_btnOk.Create( this, _S( 191, "확인" ), 173, 179, 63, 21 ); 
	m_btnOk.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnOk.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnOk.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOk.CopyUV( UBS_IDLE, UBS_DISABLE );

}

//------------------------------------------------------------------------------
// CUIGuildStash::RenderTable
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildStash::RenderTable()
{
	int nY = m_nPosY + GUILDSTASH_TOP_HEIGHT;
	int nX = m_nPosX;
	
	// Title 
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + TALBE_LEFT_GAP_WIDTH, nY + TABLE_TITLE_HEIGHT,
								m_rtTableTitle.rtL.U0, m_rtTableTitle.rtL.V0, m_rtTableTitle.rtL.U1, m_rtTableTitle.rtL.V1,
								0xFFFFFFFF );
	
	nX += TALBE_LEFT_GAP_WIDTH;

	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + TALBE_CONTENTS_WIDTH, nY + TABLE_TITLE_HEIGHT,
								m_rtTableTitle.rtM.U0, m_rtTableTitle.rtM.V0, m_rtTableTitle.rtM.U1, m_rtTableTitle.rtM.V1,
								0xFFFFFFFF );
	nX += TALBE_CONTENTS_WIDTH;
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + TALBE_RIGHT_GAP_WIDTH, nY + TABLE_TITLE_HEIGHT,
								m_rtTableTitle.rtR.U0, m_rtTableTitle.rtR.V0, m_rtTableTitle.rtR.U1, m_rtTableTitle.rtR.V1,
								0xFFFFFFFF );

	// Middle
	nY += TABLE_TITLE_HEIGHT;
	nX = m_nPosX;
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + TALBE_LEFT_GAP_WIDTH, nY + TABLE_MIDDLE_HEIGHT,
								m_rtTableMiddle.rtL.U0, m_rtTableMiddle.rtL.V0, m_rtTableMiddle.rtL.U1, m_rtTableMiddle.rtL.V1,
								0xFFFFFFFF );
	
	nX += TALBE_LEFT_GAP_WIDTH;

	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + TALBE_CONTENTS_WIDTH, nY + TABLE_MIDDLE_HEIGHT,
								m_rtTableMiddle.rtM.U0, m_rtTableMiddle.rtM.V0, m_rtTableMiddle.rtM.U1, m_rtTableMiddle.rtM.V1,
								0xFFFFFFFF );
	nX += TALBE_CONTENTS_WIDTH;
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + TALBE_RIGHT_GAP_WIDTH, nY + TABLE_MIDDLE_HEIGHT,
								m_rtTableMiddle.rtR.U0, m_rtTableMiddle.rtR.V0, m_rtTableMiddle.rtR.U1, m_rtTableMiddle.rtR.V1,
								0xFFFFFFFF );

	// Bottom
	nY += TABLE_MIDDLE_HEIGHT;
	nX = m_nPosX;
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + TALBE_LEFT_GAP_WIDTH, nY + TABLE_BOTTOM_HEIGHT,
								m_rtTableBottom.rtL.U0, m_rtTableBottom.rtL.V0, m_rtTableBottom.rtL.U1, m_rtTableBottom.rtL.V1,
								0xFFFFFFFF );

	nX += TALBE_LEFT_GAP_WIDTH;

	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + TALBE_CONTENTS_WIDTH, nY + TABLE_BOTTOM_HEIGHT,
								m_rtTableBottom.rtM.U0, m_rtTableBottom.rtM.V0, m_rtTableBottom.rtM.U1, m_rtTableBottom.rtM.V1,
								0xFFFFFFFF );
	nX += TALBE_CONTENTS_WIDTH;
	_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + TALBE_RIGHT_GAP_WIDTH, nY + TABLE_BOTTOM_HEIGHT,
								m_rtTableBottom.rtR.U0, m_rtTableBottom.rtR.V0, m_rtTableBottom.rtR.U1, m_rtTableBottom.rtR.V1,
								0xFFFFFFFF );

	
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + TALBE_TITLE_WIDTH + TALBE_LEFT_GAP_WIDTH, m_nPosY + GUILDSTASH_TOP_HEIGHT, 
										m_nPosX + TALBE_TITLE_WIDTH + TALBE_LEFT_GAP_WIDTH + 1, m_nPosY + GUILDSTASH_TOP_HEIGHT + 149,
								m_rtTableGrid.U0,m_rtTableGrid.V0,m_rtTableGrid.U1,m_rtTableGrid.V1,
								0xFFFFFFFF );
}

//------------------------------------------------------------------------------
// CUIGuildStash::ResetPosition
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildStash::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

//------------------------------------------------------------------------------
// CUIGuildStash::AdjustPosition
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildStash::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

//------------------------------------------------------------------------------
// CUIGuildStash::OpenGuildStash
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildStash::OpenGuildStash()
{	
	if( IsVisible() )
	{
		CloseStash();
		OpenGuildStash();
		return;
	}

	_pUIMgr->CloseMessageBoxL( MSGLCMD_GUILDSTASH_REQ );
	
	_pUIMgr->CreateMessageBoxL( _S( 1905, "재정 관리인" ) , UI_GUILDSTASH, MSGLCMD_GUILDSTASH_REQ );	 
	
	_pUIMgr->AddMessageBoxLString( MSGLCMD_GUILDSTASH_REQ, TRUE, _S( 1906, "길드 재정관리인" ), -1, 0xE18600FF );	 
	_pUIMgr->AddMessageBoxLString( MSGLCMD_GUILDSTASH_REQ, TRUE, CTString( " " ), -1, 0xE18600FF );		
	
	_pUIMgr->AddMessageBoxLString( MSGLCMD_GUILDSTASH_REQ, TRUE, _S( 1907, "길드에서 거두어 들이는 세금이 있으신가요?" ), -1, 0xA3A1A3FF );		 
	_pUIMgr->AddMessageBoxLString( MSGLCMD_GUILDSTASH_REQ, TRUE, _S( 1908, "저는 그 세금을 관리하는 재정관리인입니다." ), -1, 0xA3A1A3FF );		
	_pUIMgr->AddMessageBoxLString( MSGLCMD_GUILDSTASH_REQ, TRUE, _S( 1909, "이곳에서 세금을 출금하고 세금의 입금 목록을 확인할 수 있습니다." ), -1, 0xA3A1A3FF );		
		
	_pUIMgr->AddMessageBoxLString( MSGLCMD_GUILDSTASH_REQ, FALSE, _S( 1910, "세금 수입 확인" ), SEL_VIEW );	
	_pUIMgr->AddMessageBoxLString( MSGLCMD_GUILDSTASH_REQ, FALSE, _S( 1911, "세금 잔액 확인 및 출금" ), SEL_TAKE );	
	_pUIMgr->AddMessageBoxLString( MSGLCMD_GUILDSTASH_REQ, FALSE, _S(1220 , "취소한다." ) );

}

//------------------------------------------------------------------------------
// CUIGuildStash::OpenTake
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildStash::OpenTake( LONGLONG nBalance )
{
	_pUIMgr->CloseMessageBoxL( MSGLCMD_GUILDSTASH_REQ );

	m_llBalance = nBalance;
	
	if( m_llBalance > 0 )
		GameState = GSS_TAKE;

	m_strTitle = _S( 1911 , "세금 잔액 확인 및 출금" ); 
	m_btnOk.SetEnable( FALSE );

	// Button pos reset
	CTString strSysMessage, strNas;
	CUIMsgBox_Info	MsgBoxInfo;

	MsgBoxInfo.SetMsgBoxInfo( m_strTitle, UMBS_USER_12 | UMBS_INPUT_MASK, UI_GUILDSTASH, MSGCMD_GUILDSTASH_TAKE_VIEW, MSGBOX_WIDTH + 20 ); 
	MsgBoxInfo.SetUserBtnName( _S( 1913, "출금" ),_S( 139 ,  "취소" ) ); 
	
	strNas.PrintF( "%I64d", m_llBalance );
	_pUIMgr->InsertCommaToString( strNas );
		
	strSysMessage.PrintF( _S( 1914, "잔액 : %s Nas" ) , strNas); 
	MsgBoxInfo.AddString( strSysMessage, _pUIMgr->GetNasColor( strNas ), TEXT_CENTER );
	
	strSysMessage.PrintF( " " );
	MsgBoxInfo.AddString( strSysMessage, 0xF3F3F3FF, TEXT_CENTER );
	
	strSysMessage.PrintF( _S( 1915, "출금할 금액을 입력하여 주십시오." ) ); 
	MsgBoxInfo.AddString( strSysMessage, 0xF3F3F3FF, TEXT_CENTER );

	strSysMessage.PrintF( " " );  
	MsgBoxInfo.AddString( strSysMessage, 0xF3F3F3FF, TEXT_CENTER );
	
		strSysMessage.PrintF( " " );
	MsgBoxInfo.AddString( strSysMessage, 0xF3F3F3FF, TEXT_CENTER );

	MsgBoxInfo.SetInputBox( 4, 2, 34, 187 );	

	_pUIMgr->CreateMessageBox( MsgBoxInfo );

}

//------------------------------------------------------------------------------
// CUIGuildStash::OpenView
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildStash::OpenView()
{
	_pUIMgr->CloseMessageBoxL( MSGLCMD_GUILDSTASH_REQ );

	GameState = GSS_VIEW;
	
	// Button pos reset
	m_btnOk.SetEnable( TRUE );
	m_strTitle = _S( 1910, "세금 수입 확인" ); 
	_pUIMgr->RearrangeOrder( UI_GUILDSTASH, TRUE );
}

// ----------------------------------------------------------------------------
// Name : ResetGuildStash()
// Desc : 
// ----------------------------------------------------------------------------
void CUIGuildStash::CloseStash()
{
	GameState = GSS_NPC;

	_pUIMgr->RearrangeOrder( UI_GUILDSTASH, FALSE );
}

//------------------------------------------------------------------------------
// CIGuildStash::MsgBoxCommand
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildStash::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
	case MSGCMD_GUILDSTASH_TAKE_VIEW:
		{
			if( !bOK ) 
			{
				OpenGuildStash();
				return;
			}
				
			if( _pNetwork->MyCharacterInfo.lGuildPosition != GUILD_MEMBER_BOSS 
				&& _pNetwork->MyCharacterInfo.lGuildPosition != GUILD_MEMBER_VICE_BOSS )
			{
				// 출금 요청 
				Message( MSGCMD_GUILDSTASH_ERROR, _S( 1917, "세금 출금은 길드장, 길드부장만 할 수 있습니다." ), UMBS_OK );	
				return;
			}

			LONGLONG llReqNas = _atoi64( strInput.str_String );
			_pNetwork->SendGuildStashTakeReq( llReqNas );
			
		}
		break;

	case MSGCMD_GUILDSTASH_TAKE_STASH:
		{
			if( !bOK ) return;
		
		}
		break;

	case MSGCMD_GUILDSTASH_INFO:
		{
			if( !bOK ) return;

			OpenGuildStash();
			return;
		}
		break;

	case MSGCMD_GUILDSTASH_ERROR:
		{	
			switch( GameState )
			{
			case GSS_NPC:
				OpenGuildStash();
				break;
			case GSS_TAKE:
				OpenTake( m_llBalance );
				break;
			case GSS_VIEW:
				OpenView();
				break;
			}
		}
		break;

	}

}

//------------------------------------------------------------------------------
// CUIGuildStash::MsgBoxLCommand
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildStash::MsgBoxLCommand( int nCommandCode, int nResult )
{
	switch( nCommandCode )
	{
	case MSGLCMD_GUILDSTASH_REQ:
		{
			

			if( nResult == SEL_VIEW )
			{
				// 조건 검사 

				// 세금 내용 요청 
				_pNetwork->SendGuildStashHistroyReq();
				// 서버에서 요청을 제대로 받았으면 ui출력 
							
			}
			else if ( nResult == SEL_TAKE )
			{
				//조건 검사 


				// 잔액 요청 
				_pNetwork->SendGuildStashViewReq();
				// 잔액 요청시 재대로 받았으면 출력 
				
			}
			else 
			{
				CloseStash();
			}
		}
		break;
	}

}

//------------------------------------------------------------------------------
// CUIGuildStash::Render
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildStash::Render()
{
	if( GameState == GSS_NPC ) return;
	else if( GameState == GSS_TAKE )
	{
		//RenderTake(); 
		return;
	}
	// else // GSS_VEIW
	// Set skill learn texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );
	
	// Add render regions
	int	nY, nX2;
	nX2 = m_nPosX + m_nWidth;
	
	// Background
	// Top
	nY = m_nPosY + GUILDSTASH_TOP_HEIGHT;
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY, m_nPosX + 40, nY,
										m_rtTopL.U0, m_rtTopL.V0, m_rtTopL.U1, m_rtTopL.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + 40, m_nPosY, nX2 - 40, nY,
										m_rtTopM.U0, m_rtTopM.V0, m_rtTopM.U1, m_rtTopM.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( nX2 - 40, m_nPosY, nX2, nY,
										m_rtTopR.U0, m_rtTopR.V0, m_rtTopR.U1, m_rtTopR.V1,
										0xFFFFFFFF );

	RenderTable();
	nY += TABLE_TITLE_HEIGHT + TABLE_MIDDLE_HEIGHT + TABLE_BOTTOM_HEIGHT;

	// Gap middle
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, m_nPosX + 40, nY + GUILDSTASH_GAP_HEIGHT,
										m_rtMiddleGapL.U0, m_rtMiddleGapL.V0, m_rtMiddleGapL.U1, m_rtMiddleGapL.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + 40, nY, nX2 - 40, nY + GUILDSTASH_GAP_HEIGHT,
										m_rtMiddleGapM.U0, m_rtMiddleGapM.V0, m_rtMiddleGapM.U1, m_rtMiddleGapM.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( nX2 - 40, nY, nX2, nY + GUILDSTASH_GAP_HEIGHT,
										m_rtMiddleGapR.U0, m_rtMiddleGapR.V0, m_rtMiddleGapR.U1, m_rtMiddleGapR.V1,
										0xFFFFFFFF );
	// Bottom
	nY += GUILDSTASH_GAP_HEIGHT;
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, m_nPosX + 40, nY + GUILDSTASH_BOTTOM_HEIGHT,
										m_rtBottomL.U0, m_rtBottomL.V0, m_rtBottomL.U1, m_rtBottomL.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + 40, nY, nX2 - 40, nY + GUILDSTASH_BOTTOM_HEIGHT,
										m_rtBottomM.U0, m_rtBottomM.V0, m_rtBottomM.U1, m_rtBottomM.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( nX2 - 40, nY, nX2, nY + GUILDSTASH_BOTTOM_HEIGHT,
										m_rtBottomR.U0, m_rtBottomR.V0, m_rtBottomR.U1, m_rtBottomR.V1,
										0xFFFFFFFF );

	m_btnOk.Render();

	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();	


	// Render Take List
	static int nOffsetY = 21;
	static int TAKE_LIST_HEIGHT	= 18;
	static int TAKE_LIST_TITLE_OFFSETX	= 18;
	static int TAKE_LIST_CENTENTS_OFFSETX = 	55;

	int nX = m_nPosX;
	nY = m_nPosY + GUILDSTASH_TOP_HEIGHT + nOffsetY;

	VTakeInfo::iterator iterBegin = m_vTakeInfo.begin();
	VTakeInfo::iterator iterEnd = m_vTakeInfo.end();
	VTakeInfo::iterator iter;
	

	static int j = 9;
	static int x = 90;
	static int n = 18;

	_pUIMgr->GetDrawPort()->PutTextEx( _S( 1718, "시간" ), nX + TAKE_LIST_TITLE_OFFSETX + j,	 
		nY - n, 0xFFFFFFFF );

	_pUIMgr->GetDrawPort()->PutTextEx( CTString( "Nas" ), nX + TAKE_LIST_CENTENTS_OFFSETX + x,	
	nY - n, 0xFFFFFFFF );
	
	for( iter = iterBegin; iter != iterEnd; ++iter) 
	{
		CTString strDate, strMoney, strTempMoney;
		if( iter->m_nMonth == 0 ) break;

		strDate.PrintF( "%02d / %02d", iter->m_nMonth, iter->m_nDay );
		strMoney.PrintF( "%I64d", iter->m_llMoney );
		_pUIMgr->InsertCommaToString( strMoney );
		strTempMoney.PrintF( "%25s Nas", strMoney );
		
		_pUIMgr->GetDrawPort()->PutTextEx( strDate, nX + TAKE_LIST_TITLE_OFFSETX,	
		nY, 0xFFFFFFFF );

		_pUIMgr->GetDrawPort()->PutTextEx( strTempMoney, nX + TAKE_LIST_CENTENTS_OFFSETX,	
		nY,  _pUIMgr->GetNasColor( iter->m_llMoney )  );
		
		nY += TAKE_LIST_HEIGHT;
	}
		
	_pUIMgr->GetDrawPort()->PutTextEx( m_strTitle, m_nPosX + GUILDSTASH_TITLE_TEXT_OFFSETX,	
		m_nPosY + GUILDSTASH_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );
	
	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();
}

//------------------------------------------------------------------------------
// CUIGuildStash::RenderTake
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildStash::RenderTake()
{
	
	// Set skill learn texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );
	
	// Add render regions
	int nY, nX2;
	nX2 = m_nPosX + m_nWidth;
	
	// Background
	// Top
	nY = m_nPosY + GUILDSTASH_TOP_HEIGHT;
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY, m_nPosX + 40, nY,
										m_rtTopL.U0, m_rtTopL.V0, m_rtTopL.U1, m_rtTopL.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + 40, m_nPosY, nX2 - 40, nY,
										m_rtTopM.U0, m_rtTopM.V0, m_rtTopM.U1, m_rtTopM.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( nX2 - 40, m_nPosY, nX2, nY,
										m_rtTopR.U0, m_rtTopR.V0, m_rtTopR.U1, m_rtTopR.V1,
										0xFFFFFFFF );

		// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();	

	_pUIMgr->GetDrawPort()->PutTextEx( m_strTitle, m_nPosX + GUILDSTASH_TITLE_TEXT_OFFSETX,	
		m_nPosY + GUILDSTASH_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );
	
	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();

}

//------------------------------------------------------------------------------
// CUIGuildStash::MouseMessage
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
WMSG_RESULT	CUIGuildStash::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult = WMSG_FAIL;
	
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
			// OK button
			else if( m_btnOk.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			

		}
		break;
	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				nOldX = nX;		nOldY = nY;
						
				// Title bar
				if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
				}
				// OK button
				else if( m_btnOk.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				
				
				_pUIMgr->RearrangeOrder( UI_GUILDSTASH, TRUE );
				return WMSG_SUCCESS;
			}
			return WMSG_FAIL;
		}
		break;
	case WM_LBUTTONUP:
		{
			bTitleBarClick = FALSE;

			// Title bar
			
			// If message box isn't focused
			if( !IsFocused() )
				return WMSG_FAIL;

			// OK button
			if( ( wmsgResult = m_btnOk.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND )
					OpenGuildStash();

				return WMSG_SUCCESS;
			}
			
		}
		break;
	}
	return WMSG_FAIL;
}

//------------------------------------------------------------------------------
// CUIGuildStash::AddTakeInfo
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildStash::AddTakeInfo( int nMonth, int nDay, LONGLONG llMoney )
{
	STakeInfo TakeInfo;
	TakeInfo.SetData( nMonth, nDay, llMoney );

	m_vTakeInfo.push_back( TakeInfo );
}

//------------------------------------------------------------------------------
// CUIGuildStash::ResetTakeInfo
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildStash::ResetTakeInfo()
{
	m_vTakeInfo.clear();
}


//------------------------------------------------------------------------------
// CUIMessenger::Message
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildStash::Message( int nCommandCode, CTString strMessage, DWORD dwStyle )
{
	if( _pUIMgr->DoesMessageBoxExist( nCommandCode ) )
	return;

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( m_strTitle, dwStyle, UI_GUILDSTASH, nCommandCode );
	
	MsgBoxInfo.AddString( strMessage );
	_pUIMgr->CreateMessageBox( MsgBoxInfo );
}

//------------------------------------------------------------------------------
// CUIGuildStash::ErrorProc
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildStash::ErrorProc( LONG lErrorCode, UBYTE ubType )
{
	switch( lErrorCode )
	{
	case MSG_GUILD_STASH_ERROR_OK:				// 성공
		{
			// nothing 
		}
		break;
	case MSG_GUILD_STASH_ERROR_NOHISTORY:		// 히스토리 없음
		{
			Message( MSGCMD_GUILDSTASH_ERROR, _S( 1918, "세금에 대한 입금 내역이 없습니다." ), UMBS_OK ); 
		}
		break;
	case MSG_GUILD_STASH_ERROR_NOTBOSS:			// 길드장/부관 아님
		{
			Message( MSGCMD_GUILDSTASH_ERROR, _S( 1917, "세금 출금은 길드장, 길드부장만 할 수 있습니다." ), UMBS_OK ); 
		
		}
		break;
	case MSG_GUILD_STASH_ERROR_NOTENOUGH:		// 잔액 부족
		{
			Message( MSGCMD_GUILDSTASH_ERROR, _S( 1920, "잔액이 부족합니다.  확인하시고 다시 입력하여 주십시오." ), UMBS_OK ); 
		}
		break;
	case MSG_GUILD_STASH_ERROR_FAIL_DB:			// 시스템 오류
		{
			Message( MSGCMD_GUILDSTASH_ERROR, _S( 37, "시스템 오류" ), UMBS_OK ); 
			CloseStash();
		}
		break;
	// 이전 메시지는 헬퍼의 길드 창고 오류와 동일해야함
	case MSG_GUILD_STASH_ERROR_NOSPACE:			// 인벤토리 공간 부족으로 지급 불가
		{
			Message( MSGCMD_GUILDSTASH_ERROR, _S( 1921, "인벤토리가 부족합니다." ), UMBS_OK ); 
		}
		break;
	}

}
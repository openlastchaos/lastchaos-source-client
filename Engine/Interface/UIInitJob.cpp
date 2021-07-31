#include "stdh.h"
#include <Engine/Interface/UIInitJob.h>
#include <Engine/Interface/UIInternalClasses.h>

enum eSelection
{
	INITJOB_OK,
	INITJOB_GIVEUP,
	INITJOB_TALK,
	INITJOB_EVENT,
};

static int	_iMaxMsgStringChar = 0;

#define DESC_LIST_BOX_HEIGHT			(172)

// Define text position
#define	INITJOB_TITLE_TEXT_OFFSETX		25
#define	INITJOB_TITLE_TEXT_OFFSETY		5
#define	INITJOB_DESC_TEXT_SX			15
#define	INITJOB_DESC_TEXT_SY			47
#define CRITERION_LEVEL					(31)
#define WEIGHT_MONEY					(100000)		// 가중치

extern INDEX g_iCountry;
// ----------------------------------------------------------------------------
// Name : CUIInitJob()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIInitJob::CUIInitJob()
{
	m_eInitJobState		= INITJOB_REQ;
	m_nStringCount		= 0;
	m_strInitJobMoney	= CTString( "" );
	m_llInitJobMoney	= 0;
	m_iRestoreStatPoint	= 0;
	m_strRestoreStatPoint	= CTString( "" );

	m_strStrength		= CTString( "" );
	m_strDexterity		= CTString( "" );
	m_strIntelligence	= CTString( "" );
	m_strConstitution	= CTString( "" );

	m_strGivePrice		= CTString( "" );

	m_aswTemp[MSG_STATPOINT_USE_STR]	= 0;
	m_aswTemp[MSG_STATPOINT_USE_DEX]	= 0;
	m_aswTemp[MSG_STATPOINT_USE_INT]	= 0;
	m_aswTemp[MSG_STATPOINT_USE_CON]	= 0;
	m_aswNew[MSG_STATPOINT_USE_STR]		= 0;
	m_aswNew[MSG_STATPOINT_USE_DEX]		= 0;
	m_aswNew[MSG_STATPOINT_USE_INT]		= 0;
	m_aswNew[MSG_STATPOINT_USE_CON]		= 0;
	m_astrTemp[MSG_STATPOINT_USE_STR]	= CTString( "" );
	m_astrTemp[MSG_STATPOINT_USE_DEX]	= CTString( "" );
	m_astrTemp[MSG_STATPOINT_USE_INT]	= CTString( "" );
	m_astrTemp[MSG_STATPOINT_USE_CON]	= CTString( "" );
	m_astrNew[MSG_STATPOINT_USE_STR]	= CTString( "0" );
	m_astrNew[MSG_STATPOINT_USE_DEX]	= CTString( "0" );
	m_astrNew[MSG_STATPOINT_USE_INT]	= CTString( "0" );
	m_astrNew[MSG_STATPOINT_USE_CON]	= CTString( "0" );
}

// ----------------------------------------------------------------------------
// Name : ~CUIInitJob()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIInitJob::~CUIInitJob()
{
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIInitJob::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	_iMaxMsgStringChar = 190 / ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 216, 22 );

	// Create refine texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MessageBox.tex" ) );
	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	m_rtTop.SetUV( 0, 0, 216, 26, fTexWidth, fTexHeight );
	m_rtMiddle1.SetUV( 0, 31, 216, 33, fTexWidth, fTexHeight );
	m_rtMiddle2.SetUV( 0, 35, 216, 37, fTexWidth, fTexHeight );
	m_rtMiddle3.SetUV( 36, 86, 252, 104, fTexWidth, fTexHeight );
	m_rtBottom.SetUV( 0, 38, 216, 45, fTexWidth, fTexHeight );

	// Input box
	m_rtInputBoxL.SetUV( 205, 46, 209, 59, fTexWidth, fTexHeight );
	m_rtInputBoxM.SetUV( 211, 46, 213, 59, fTexWidth, fTexHeight );
	m_rtInputBoxR.SetUV( 215, 46, 219, 59, fTexWidth, fTexHeight );

	// Close button
	m_btnClose.Create( this, CTString( "" ), 184, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 219, 0, 233, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 234, 0, 248, 14, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// OK button
	m_btnOK.Create( this, _S( 191, "확인" ), 78, 223, 63, 21 );
	m_btnOK.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnOK.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnOK.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOK.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Cancel button
	m_btnCancel.Create( this, _S( 139, "취소" ), 146, 223, 63, 21 );
	m_btnCancel.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );

	AddString( _S( 1168, "직업을 포기하면 전직 이후에 배웠던 스킬은 초기화되며, 스킬 습득시 소비했던 sp는 환원되지 않습니다." ) );	
	AddString( _S( 1169, "또한 분배했던 스탯 포인트는 초기화 되며, 해당 클래스의 기본 직업을 기준으로 스탯 포인트가 다시 분배됩니다." ) );	
	AddString( _S( 1170, "직업을 포기한 경우에는 스킬 마스터를 통해 재 전직을 할 수 있습니다."  ) );	

	int nY2 = m_nPosY + 76;	
	// Strength decrease button
	m_btnDPStrength.Create( this, CTString( "" ), 149, nY2, 14, 14 );
	m_btnDPStrength.SetUV( UBS_IDLE, 230, 33, 244, 46, fTexWidth, fTexHeight );
	m_btnDPStrength.SetUV( UBS_CLICK, 230, 48, 244, 62, fTexWidth, fTexHeight );	
	m_btnDPStrength.CopyUV( UBS_IDLE, UBS_ON );
	m_btnDPStrength.SetEnable( TRUE );

	nY2 += 16;
	// Dexterity decrease button
	m_btnDPDexterity.Create( this, CTString( "" ), 149, nY2, 14, 14 );
	m_btnDPDexterity.SetUV( UBS_IDLE, 230, 33, 244, 46, fTexWidth, fTexHeight );
	m_btnDPDexterity.SetUV( UBS_CLICK, 230, 48, 244, 62, fTexWidth, fTexHeight );	
	m_btnDPDexterity.CopyUV( UBS_IDLE, UBS_ON );
	m_btnDPDexterity.SetEnable( TRUE );

	nY2 += 16;
	// Intelligence decrease button
	m_btnDPIntelligence.Create( this, CTString( "" ), 149, nY2, 14, 14 );
	m_btnDPIntelligence.SetUV( UBS_IDLE, 230, 33, 244, 46, fTexWidth, fTexHeight );
	m_btnDPIntelligence.SetUV( UBS_CLICK, 230, 48, 244, 62, fTexWidth, fTexHeight );	
	m_btnDPIntelligence.CopyUV( UBS_IDLE, UBS_ON );
	m_btnDPIntelligence.SetEnable( TRUE );

	nY2 += 16;
	// Constitution decrease button
	m_btnDPConstitution.Create( this, CTString( "" ), 149, nY2, 14, 14 );
	m_btnDPConstitution.SetUV( UBS_IDLE, 230, 33, 244, 46, fTexWidth, fTexHeight );
	m_btnDPConstitution.SetUV( UBS_CLICK, 230, 48, 244, 62, fTexWidth, fTexHeight );	
	m_btnDPConstitution.CopyUV( UBS_IDLE, UBS_ON );
	m_btnDPConstitution.SetEnable( TRUE );
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIInitJob::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2 - GetWidth(), ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIInitJob::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : OpenInitJob()
// Desc :
// ----------------------------------------------------------------------------
void CUIInitJob::OpenInitJob(int iMobIndex, BOOL bHasQuest, FLOAT fX, FLOAT fZ )
{
	// If this is already exist
	if( _pUIMgr->DoesMessageBoxLExist( MSGLCMD_INITJOB_REQ ) || IsVisible() )
		return;

	// Set position of target npc
	m_fNpcX = fX;
	m_fNpcZ = fZ;

	_pUIMgr->CloseMessageBox( MSGCMD_INITJOB_GIVEUP );
	_pUIMgr->CloseMessageBox( MSGCMD_INITJOB_STAT );
	_pUIMgr->CloseMessageBox( MSGCMD_INITJOB_NOTIFY );

	CMobData& MD = _pNetwork->GetMobData(iMobIndex);

	// Create refine message box
	_pUIMgr->CreateMessageBoxL( _S( 1171, "초기화" ), UI_INITJOB, MSGLCMD_INITJOB_REQ );		

	CTString	strNpcName = _pNetwork->GetMobName(iMobIndex);
	_pUIMgr->AddMessageBoxLString( MSGLCMD_INITJOB_REQ, TRUE, strNpcName, -1, 0xE18600FF );

	_pUIMgr->AddMessageBoxLString( MSGLCMD_INITJOB_REQ, TRUE, _S( 1172,  "잘못된 훈련으로 올바르게 성장하지 못한 사람들에게 다시 새로운 생명을 주는 것이 제가 맡고 있는 일입니다.\n"  ), -1, 0xA3A1A3FF );	
	_pUIMgr->AddMessageBoxLString( MSGLCMD_INITJOB_REQ, TRUE, _S( 1173,  "캐릭터를 잘못 키웠거나 당신이 현재 가지고 있는 직업이 마음에 들지 않으신다면 제가 도와 드리지요."  ), -1, 0xA3A1A3FF );		

	CTString strMessage;
	//int i = 1;

	// 직업을 갖고 있을 경우...
	if( _pNetwork->MyCharacterInfo.job2 > 0 )
	{
		strMessage.PrintF( _S( 1174, "스탯환원" ) );		
		_pUIMgr->AddMessageBoxLString( MSGLCMD_INITJOB_REQ, FALSE, strMessage, INITJOB_OK );
		
		strMessage.PrintF( _S( 1175, "직업포기" ) );		
		_pUIMgr->AddMessageBoxLString( MSGLCMD_INITJOB_REQ, FALSE, strMessage, INITJOB_GIVEUP );
	}
	
	if( bHasQuest )
	{
#ifdef	NEW_QUESTBOOK
		// 2009. 05. 27 김정래
		// 이야기한다 변경 처리
		CUIQuestBook::AddQuestListToMessageBoxL(MSGLCMD_INITJOB_REQ);				
#else
		strMessage.PrintF( _S( 1053, "이야기한다." ) );	
		_pUIMgr->AddMessageBoxLString( MSGLCMD_INITJOB_REQ, FALSE, strMessage, INITJOB_TALK );
#endif
	}
	
	if( MD.IsEvent() )
	{
		strMessage.PrintF( _S( 100, "이벤트" ) );			
		_pUIMgr->AddMessageBoxLString( MSGLCMD_INITJOB_REQ, FALSE, strMessage, INITJOB_EVENT );
	}

	strMessage.PrintF( _S( 1220, "취소한다." ) );			
	_pUIMgr->AddMessageBoxLString( MSGLCMD_INITJOB_REQ, FALSE, strMessage );	

}

// ----------------------------------------------------------------------------
// Name : CloseInitJob()
// Desc :
// ----------------------------------------------------------------------------
void CUIInitJob::CloseInitJob()
{
	// Close refine
	_pUIMgr->RearrangeOrder( UI_INITJOB, FALSE );

	m_iRestoreStatPoint		= 0;
	m_llInitJobMoney		= 0;

	for( int i = 0; i < 4; ++i )
	{
		m_aswTemp[i]		= 0;
		m_aswNew[i]			= 0;
		m_astrTemp[i].Clear();
		m_astrNew[i]		= "0";
	}

	m_strInitJobMoney.Clear();
	m_strRestoreStatPoint.Clear();
}

// ----------------------------------------------------------------------------
// Name : AddString()
// Desc :
// ----------------------------------------------------------------------------
void CUIInitJob::AddString( CTString &strDesc )
{
	if( m_nStringCount >= MAX_INITJOB_STRING )
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
				m_strInitJobDesc[m_nStringCount++] = strDesc;
			else
			{
				// Split string
				CTString	strTemp;
				strDesc.Split( iPos, m_strInitJobDesc[m_nStringCount++], strTemp );

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
				strDesc.Split( nSplitPos, m_strInitJobDesc[m_nStringCount++], strTemp );

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
				strDesc.Split( iPos, m_strInitJobDesc[m_nStringCount++], strTemp );

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
				m_strInitJobDesc[m_nStringCount++] = strDesc;
			else
			{
				// Split string
				CTString	strTemp;
				strDesc.Split( iPos, m_strInitJobDesc[m_nStringCount++], strTemp );

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
				strDesc.Split( nSplitPos, m_strInitJobDesc[m_nStringCount++], strTemp );

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
				strDesc.Split( iPos, m_strInitJobDesc[m_nStringCount++], strTemp );

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
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIInitJob::Render()
{
	// Check distance
	FLOAT	fDiffX = _pNetwork->MyCharacterInfo.x - m_fNpcX;
	FLOAT	fDiffZ = _pNetwork->MyCharacterInfo.z - m_fNpcZ;
	if( fDiffX * fDiffX + fDiffZ * fDiffZ > UI_VALID_SQRDIST )
		CloseInitJob();

	// Set refine texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	int	nX, nY, nX2, nY2;
	
	// Background
	// Top
	nX = m_nPosX + m_nWidth;
	nY = m_nPosY + 26;
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY, nX, nY,
		m_rtTop.U0, m_rtTop.V0,
		m_rtTop.U1, m_rtTop.V1,
		0xFFFFFFFF );

	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, nX, nY + 18,
		m_rtMiddle3.U0, m_rtMiddle3.V0,
		m_rtMiddle3.U1, m_rtMiddle3.V1,
		0xFFFFFFFF );

	nY += 18;
	
	// Not Manager
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, nX, nY + DESC_LIST_BOX_HEIGHT,
		m_rtMiddle1.U0, m_rtMiddle1.V0,
		m_rtMiddle1.U1, m_rtMiddle1.V1,
		0xFFFFFFFF );
	
	nY += DESC_LIST_BOX_HEIGHT;
	
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, nX, m_nPosY + m_nHeight - 7,
		m_rtMiddle2.U0, m_rtMiddle2.V0,
		m_rtMiddle2.U1, m_rtMiddle2.V1,
		0xFFFFFFFF );
	
	// Bottom
	nY = m_nPosY + m_nHeight - 7;
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, nX, m_nPosY + m_nHeight,
		m_rtBottom.U0, m_rtBottom.V0,
		m_rtBottom.U1, m_rtBottom.V1,
		0xFFFFFFFF );

	if( m_eInitJobState == INITJOB_STAT )
	{
		nX	= m_nPosX + 114;
		nY	= m_nPosY + 178;
		nX2	= nX + 86;
		nY2	= nY + 13;

		// Point
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

		nY	= m_nPosY + 196;
		nY2	= nY + 13;

		// Money
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
		
		nX	= m_nPosX + 74;
		nY	= m_nPosY + 78;
		nX2 = nX + 33;
		nY2 = nY + 13;
		for( int i = 0; i < 4; ++i )
		{
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

			nY += 16;
			nY2 = nY + 13;
		}

		nX	= m_nPosX + 111;
		nY	= m_nPosY + 78;
		nX2 = nX + 33;
		nY2 = nY + 13;
		for( i = 0; i < 4; ++i )
		{
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

			nY += 16;
			nY2 = nY + 13;
		}

		// Render decrease point buttons
		m_btnDPStrength.Render();
		m_btnDPDexterity.Render();
		m_btnDPIntelligence.Render();
		m_btnDPConstitution.Render();
	}
	else if( m_eInitJobState == INITJOB_GIVEUP )
	{
		nX	= m_nPosX + 110;
		nY	= m_nPosY + 195;
		nX2	= nX + 80;
		nY2	= nY + 13;

		// Money
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
	}
	
	// Close button
	m_btnClose.Render();

	// OK button
	m_btnOK.Render();

	// Cancel button
	m_btnCancel.Render();

	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	
	if( m_eInitJobState == INITJOB_STAT )
	{		
		_pUIMgr->GetDrawPort()->PutTextEx( _S( 1179, "스탯 환원" ), m_nPosX + INITJOB_TITLE_TEXT_OFFSETX,		
			m_nPosY + INITJOB_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );
		
		int nX1 = m_nPosX + 30;		
		nY = m_nPosY + 29;
		_pUIMgr->GetDrawPort()->PutTextEx( _pNetwork->MyCharacterInfo.name, nX1, nY, 0xFFFFFFFF );

		nY = m_nPosY + 78;
		
		nX1 = m_nPosX + 44;
		nX2 = m_nPosX + 105;
		_pUIMgr->GetDrawPort()->PutTextEx( _S( 80, "힘" ), nX1, nY, 0xABABABFF );
		_pUIMgr->GetDrawPort()->PutTextExRX( m_strStrength, nX2, nY, 0xE18600FF );
		_pUIMgr->GetDrawPort()->PutTextExRX( m_astrNew[MSG_STATPOINT_USE_STR], nX2 + 34, nY, 0xE18600FF );
		nY += 16;
		_pUIMgr->GetDrawPort()->PutTextEx( _S( 81, "민첩" ), nX1, nY, 0xABABABFF );
		_pUIMgr->GetDrawPort()->PutTextExRX( m_strDexterity, nX2, nY, 0xE18600FF );
		_pUIMgr->GetDrawPort()->PutTextExRX( m_astrNew[MSG_STATPOINT_USE_DEX], nX2 + 34, nY, 0xE18600FF );
		nY += 16;
		_pUIMgr->GetDrawPort()->PutTextEx( _S( 82, "지혜" ), nX1, nY, 0xABABABFF );
		_pUIMgr->GetDrawPort()->PutTextExRX( m_strIntelligence, nX2, nY, 0xE18600FF );
		_pUIMgr->GetDrawPort()->PutTextExRX( m_astrNew[MSG_STATPOINT_USE_INT], nX2 + 34, nY, 0xE18600FF );
		nY += 16;
		_pUIMgr->GetDrawPort()->PutTextEx( _S( 83, "체질" ), nX1, nY, 0xABABABFF );
		_pUIMgr->GetDrawPort()->PutTextExRX( m_strConstitution, nX2, nY, 0xE18600FF );
		_pUIMgr->GetDrawPort()->PutTextExRX( m_astrNew[MSG_STATPOINT_USE_CON], nX2 + 34, nY, 0xE18600FF );

		nY	= m_nPosY + 178;
		nX2 = m_nPosX + 46;
		_pUIMgr->GetDrawPort()->PutTextEx( _S( 1180, "환원 포인트" ), nX2, nY, 0xE18600FF );		
		_pUIMgr->GetDrawPort()->PutTextEx( m_strRestoreStatPoint, m_nPosX + 114, nY );

		nY += 20;
		_pUIMgr->GetDrawPort()->PutTextEx( _S( 1181, "필요 나스" ), nX2, nY, 0xE18600FF );		
		_pUIMgr->GetDrawPort()->PutTextEx( m_strInitJobMoney, m_nPosX + 114, nY );
	}
	else if( m_eInitJobState == INITJOB_GIVEUP )
	{
		CTString strMessage;
		strMessage.PrintF( _S( 1113, "[%s] 직업 포기" ), JobInfo().GetName( _pNetwork->MyCharacterInfo.job, _pNetwork->MyCharacterInfo.job2 ) ); 

		int nX1 = m_nPosX + 30;		
		nY		= m_nPosY + 29;
		_pUIMgr->GetDrawPort()->PutTextEx( strMessage, nX1, nY, 0xFFFFFFFF );

		_pUIMgr->GetDrawPort()->PutTextEx( _S( 1182, "직업 포기" ), m_nPosX + INITJOB_TITLE_TEXT_OFFSETX,		
			m_nPosY + INITJOB_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );
		
		nX = m_nPosX + INITJOB_DESC_TEXT_SX;
		nY = m_nPosY + INITJOB_DESC_TEXT_SY;
		for( int iDesc = 0; iDesc < m_nStringCount; iDesc++ )
		{
			_pUIMgr->GetDrawPort()->PutTextEx( m_strInitJobDesc[iDesc], nX , nY, 0xC5C5C5FF );
			nY += _pUIFontTexMgr->GetLineHeight();
		}

		nY	= m_nPosY + 195;
		nX2	= m_nPosX + STATUS_NAME_SX;
		_pUIMgr->GetDrawPort()->PutTextEx( _S( 1181, "필요 나스" ), nX2, nY, 0xE18600FF );		
		_pUIMgr->GetDrawPort()->PutTextEx( m_strGivePrice, m_nPosX + 112, nY );
	}

	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIInitJob::MouseMessage( MSG *pMsg )
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
				_pUIMgr->SetMouseCursorInsideUIs();

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
			else if( m_eInitJobState == INITJOB_STAT )
			{
				// Strength decrease point button
				if( m_btnDPStrength.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				// Dexterity decrease point button
				else if( m_btnDPDexterity.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				// Intelligence decrease point button
				else if( m_btnDPIntelligence.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				// Constitution decrease point button
				else if( m_btnDPConstitution.MouseMessage( pMsg ) != WMSG_FAIL )
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
				else if( m_eInitJobState == INITJOB_STAT )
				{
					// Strength decrease point button
					if( m_btnDPStrength.MouseMessage( pMsg ) != WMSG_FAIL )
					{
					}
					// Dexterity decrease point button
					else if( m_btnDPDexterity.MouseMessage( pMsg ) != WMSG_FAIL )
					{
					}
					// Intelligence decrease point button
					else if( m_btnDPIntelligence.MouseMessage( pMsg ) != WMSG_FAIL )
					{
					}
					// Constitution decrease point button
					else if( m_btnDPConstitution.MouseMessage( pMsg ) != WMSG_FAIL )
					{
					}
				}

				_pUIMgr->RearrangeOrder( UI_INITJOB, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
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
					CloseInitJob();
				
				return WMSG_SUCCESS;
			}
			// OK button
			else if( ( wmsgResult = m_btnOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND )
				{
					if( m_eInitJobState == INITJOB_GIVEUP )
					{
						// 포기 확인~~~!!!						
						_pUIMgr->CloseMessageBox( MSGCMD_INITJOB_GIVEUP );
						
						CTString	strMessage;
						CUIMsgBox_Info	MsgBoxInfo;
						MsgBoxInfo.SetMsgBoxInfo( _S( 1182, "직업 포기" ), UMBS_OKCANCEL, UI_INITJOB, MSGCMD_INITJOB_GIVEUP );	
						strMessage.PrintF( CTString( _S( 1183, "[%s] 직업을 포기 하시겠습니까?" ) ), JobInfo().GetName( _pNetwork->MyCharacterInfo.job, _pNetwork->MyCharacterInfo.job2 ) );			
						MsgBoxInfo.AddString( strMessage );
						_pUIMgr->CreateMessageBox( MsgBoxInfo );
					}
					else if( m_eInitJobState == INITJOB_STAT )
					{
						// 내가 갖고 있는 돈이 필요한 돈 보다 적은경우...
						if( m_llInitJobMoney > _pNetwork->MyCharacterInfo.money )
						{
							_pUIMgr->GetChatting()->AddSysMessage( _S( 1184,  "나스가 부족합니다."  ), SYSMSG_ERROR ); 
							return WMSG_SUCCESS;
						}
						else if( m_iRestoreStatPoint > 0 )
						{
							// 포기 확인~~~!!!						
							_pUIMgr->CloseMessageBox( MSGCMD_INITJOB_STAT );
							
							CTString	strMessage;
							CUIMsgBox_Info	MsgBoxInfo;
							MsgBoxInfo.SetMsgBoxInfo( _S( 1179, "스탯 환원" ), UMBS_OKCANCEL, UI_INITJOB, MSGCMD_INITJOB_STAT );	
							strMessage.PrintF( _S( 1185, "총 %d 스탯을 환원 하시겠습니까?" ), m_iRestoreStatPoint );			
							MsgBoxInfo.AddString( strMessage );
							_pUIMgr->CreateMessageBox( MsgBoxInfo );
						}
					}
				}
				//	SendInitJobReq();
				
				return WMSG_SUCCESS;
			}
			// Cancel button
			else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND )
					CloseInitJob();
				
				return WMSG_SUCCESS;
			}
			else if( m_eInitJobState == INITJOB_STAT )
			{
				// Strength decrease point button
				if( m_btnDPStrength.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					RestoreStatPoint( MSG_STATPOINT_USE_STR );
					return WMSG_SUCCESS;
				}
				// Dexterity decrease point button
				else if( m_btnDPDexterity.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					RestoreStatPoint( MSG_STATPOINT_USE_DEX );
					return WMSG_SUCCESS;
				}
				// Intelligence decrease point button
				else if( m_btnDPIntelligence.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					RestoreStatPoint( MSG_STATPOINT_USE_INT );
					return WMSG_SUCCESS;
				}
				// Constitution decrease point button
				else if( m_btnDPConstitution.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					RestoreStatPoint( MSG_STATPOINT_USE_CON );
					return WMSG_SUCCESS;
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
	}

	return WMSG_FAIL;
}


// ========================================================================= //
//                             Command functions                             //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUIInitJob::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
	case MSGCMD_INITJOB_GIVEUP:		// 직업 포기 확인
		{
			 if( !bOK )
				 return;
			 _pNetwork->ChangeJobGiveUp();
		}
		break;
	case MSGCMD_INITJOB_STAT:		// 스탯 환원 확인
		{
			if( !bOK )
				 return;
			int iStr = m_aswNew[MSG_STATPOINT_USE_STR];
			int iDex = m_aswNew[MSG_STATPOINT_USE_DEX];
			int iInt = m_aswNew[MSG_STATPOINT_USE_INT];
			int iCon = m_aswNew[MSG_STATPOINT_USE_CON];
			_pNetwork->StatReset( iStr, iDex, iInt, iCon );
		}
		break;
	case MSGCMD_INITJOB_NOTIFY:		// 에러 메세지
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUIInitJob::MsgBoxLCommand( int nCommandCode, int nResult )
{
	switch( nCommandCode )
	{
	case MSGLCMD_INITJOB_REQ:
		if( nResult == INITJOB_OK )					// 스탯 환원
		{
			m_eInitJobState = INITJOB_STAT;

			int	iStrength					= _pNetwork->MyCharacterInfo.str - _pNetwork->MyCharacterInfo.opt_str;
			int	iDexterity					= _pNetwork->MyCharacterInfo.dex - _pNetwork->MyCharacterInfo.opt_dex;
			int	iIntelligence				= _pNetwork->MyCharacterInfo.intel - _pNetwork->MyCharacterInfo.opt_intel;
			int	iConstitution				= _pNetwork->MyCharacterInfo.con - _pNetwork->MyCharacterInfo.opt_con;

			m_aswTemp[MSG_STATPOINT_USE_STR]	= iStrength;
			m_aswTemp[MSG_STATPOINT_USE_DEX]	= iDexterity;
			m_aswTemp[MSG_STATPOINT_USE_INT]	= iIntelligence;
			m_aswTemp[MSG_STATPOINT_USE_CON]	= iConstitution;

			m_strStrength.PrintF( "%3d", iStrength );
			m_strDexterity.PrintF( "%3d", iDexterity );
			m_strIntelligence.PrintF( "%3d", iIntelligence );
			m_strConstitution.PrintF( "%3d", iConstitution );
			
			_pUIMgr->RearrangeOrder( UI_INITJOB, TRUE );
		}
		else if( nResult == INITJOB_GIVEUP )		// 직업 포기
		{
			if( !_pUIMgr->GetCharacterInfo()->IsVisible() )
				_pUIMgr->GetCharacterInfo()->ToggleVisibleStatus();
			
			int iTotalStat = 0;
			iTotalStat += ( _pNetwork->MyCharacterInfo.str - _pNetwork->MyCharacterInfo.opt_str );
			iTotalStat += ( _pNetwork->MyCharacterInfo.dex - _pNetwork->MyCharacterInfo.opt_dex );
			iTotalStat += ( _pNetwork->MyCharacterInfo.intel - _pNetwork->MyCharacterInfo.opt_intel );
			iTotalStat += ( _pNetwork->MyCharacterInfo.con - _pNetwork->MyCharacterInfo.opt_con );

			SQUAD llPrice = CalculateGiveUpPrice( iTotalStat, _pNetwork->MyCharacterInfo.level );
			m_strGivePrice.PrintF( "%I64d", llPrice );
			
			m_eInitJobState = INITJOB_GIVEUP;
			_pUIMgr->RearrangeOrder( UI_INITJOB, TRUE );
		}
		else if( nResult == INITJOB_TALK )			// 이야기하기.
		{
			//TODO : NewQuestSystem
			// 퀘스트 창 띄우기
			CUIQuestBook::TalkWithNPC();
			//_pUIMgr->GetQuest()->OpenQuest( _pUIMgr->GetCharacterInfo()->GetMobIndex(), m_fNpcX, m_fNpcZ );
		}
		else if( nResult == INITJOB_EVENT )			// Event
		{
		}
		// [090527: selo] 확장팩 퀘스트 수정
		else if( ciQuestClassifier < nResult )	
		{
			// 선택한 퀘스트에 대해 수락 또는 보상 창을 연다.
			CUIQuestBook::SelectQuestFromMessageBox( nResult );
		}
		else
		{
			_pUIMgr->RearrangeOrder( UI_INITJOB, FALSE );
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : CalculateGiveUpPrice()
// Desc :
// ----------------------------------------------------------------------------
SQUAD CUIInitJob::CalculateGiveUpPrice( int iTotalStat, int iCharLevel )
{
	SQUAD llMoney	= 0;
	llMoney			= iTotalStat * 25000 * iCharLevel / CRITERION_LEVEL + WEIGHT_MONEY;
	return llMoney;
}

// ----------------------------------------------------------------------------
// Name : CalculateStatPrice()
// Desc :
// ----------------------------------------------------------------------------
SQUAD CUIInitJob::CalculateStatPrice( int iStatPoint )
{
	SQUAD llMoney	= 0;
	llMoney			= iStatPoint * 20000;
	return llMoney;
}

// ----------------------------------------------------------------------------
// Name : RestoreStatPoint()
// Desc :
// ----------------------------------------------------------------------------
void CUIInitJob::RestoreStatPoint( SBYTE sbStatType )
{
	// 스탯 값이 0보다 작지 않은지 확인.
	if( m_aswTemp[sbStatType] <= 0 )
		return;

	m_aswTemp[sbStatType]--;
	m_aswNew[sbStatType]++;
	m_astrTemp[sbStatType].PrintF( "%d", m_aswTemp[sbStatType] );
	m_astrNew[sbStatType].PrintF( "%d", m_aswNew[sbStatType] );

	m_iRestoreStatPoint++;

	m_strRestoreStatPoint.PrintF( "%d", m_iRestoreStatPoint );

	m_llInitJobMoney = CalculateStatPrice( m_iRestoreStatPoint );		// 필요 나스
	m_strInitJobMoney.PrintF( "%I64d", m_llInitJobMoney );
}
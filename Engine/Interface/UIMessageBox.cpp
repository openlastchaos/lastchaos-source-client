#include "stdh.h"
#include <Engine/Interface/UIMessageBox.h>
#include <Engine/Petinfo.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIPetInfo.h>
#include <Engine/Interface/UITextureManager.h>
#include <Engine/LocalDefine.h>

#define MESSAGEBOX_CHAR_WIDTH	( MSGBOX_WIDTH - 14 - MSGBOX_MESSAGE_OFFSETX )
static int	_iMaxMsgStringChar = 0;

extern INDEX g_iCountry;

// ----------------------------------------------------------------------------
// Name : ISBoxInit() wooss 050805
// Desc : MsgCommand roop에서 다시 메시지 박스를 만들 경우 
//		  ReturnCommand()에서 새로만든 메시지 박스가 사라지는 것을 방지
// ----------------------------------------------------------------------------
BOOL	IsBoxInit(int cmdCode)
{

	if(MSGCMD_USE_RECOVER_ITEM_HEXP==cmdCode) return FALSE; 
	if(MSGCMD_USE_RECOVER_ITEM_AEXP==cmdCode) return FALSE; 
	if(MSGCMD_USE_RECOVER_ITEM_HEXP_LUCKY==cmdCode) return FALSE; 
	if(MSGCMD_USE_RECOVER_ITEM_AEXP_LUCKY==cmdCode) return FALSE; 
	if(MSGCMD_USE_REBIRTH_ITEM==cmdCode) return FALSE;
	if(MSGCMD_USE_CHANGE_MY_NAME_ITEM==cmdCode) return FALSE;
	if(MSGCMD_USE_CHANGE_GUILD_NAME_ITEM==cmdCode) return FALSE;
	if(MSGCMD_PET_MOUNT_CANCEL_CARD==cmdCode) return FALSE;
	//if(MSGLCMD_EVENT_XMAS_2006_CHANGE==cmdCode) return FALSE;
	if(MSGCMD_USE_PACKING_PAPER==cmdCode) return FALSE;
	if(MSGCMD_USE_SMALL_CORD==cmdCode) return FALSE;
	if(MSGCMD_GUILDMASTER_1==cmdCode) return FALSE;
	if(MSGCMD_MOONSTONE_MIX==cmdCode) return FALSE;
	if(MSGCMD_SIEGE_DUNGEON_ENVIRONMENT_CONTROL==cmdCode) return FALSE;
	if(MSGCMD_SIEGE_DUNGEON_MONSTER_CONTROL==cmdCode) return FALSE;
	if(MSGCMD_SIEGE_DUNGEON_ADMISSION_FEE_CONTROL==cmdCode) return FALSE;
	if(MSGCMD_SIEGE_DUNGEON_HUNTING_FEE_CONTROL==cmdCode) return FALSE;
	if(MSGCMD_CUBE_STATE==cmdCode) return FALSE;
	
	return TRUE;	
}

/////////////////////////////////////////////////////////////////////////////////////////
/////								Class: CUIColorBox								/////
CUIColorBox::CUIColorBox()
{
	m_nColorBoxCount =0;
	m_ColorBoxInfo =NULL;
}

CUIColorBox::~CUIColorBox()
{
	Reset();
}

void CUIColorBox::Create(int iCount, CTextureData *ptdBaseTexture)
{
	m_nColorBoxCount =iCount;

	// Color Box
	for(int i=0; i<9; ++i)
	{
#ifdef TEXTURE_MSGBOX_CHANGE_NA_20080902
		m_dbColorBox[i].SetBoxUV(ptdBaseTexture,2,WRect( 208+(i%3*10)+3, 98+(i/3*10)+3, 208+(i%3*10)+7, 98+(i/3*10)+7));
#else
		m_dbColorBox[i].SetBoxUV(ptdBaseTexture,2,WRect( 328+(i%3*10)+3, 177+(i/3*10)+3, 328+(i%3*10)+7, 177+(i/3*10)+7));
#endif
	}

	m_ColorBoxInfo =new COLOR_BOX_INFO[iCount];
	memset(m_ColorBoxInfo, 0, sizeof(COLOR_BOX_INFO)*iCount);
}

void CUIColorBox::Start(int index, double dDurationTime)
{
	m_ColorBoxInfo[index].m_dStartTime =_pTimer->GetHighPrecisionTimer().GetSeconds();
	m_ColorBoxInfo[index].m_dDurationTime =dDurationTime;
}

void CUIColorBox::Render(int PosX, int PosY, int nCommandCode)
{
	for(int i=0; i<m_nColorBoxCount; ++i)
	{
		int nColorBoxType =m_ColorBoxInfo[i].m_nColorBoxType;
		WRect rcBox =m_ColorBoxInfo[i].m_rcColorBox;

		if( m_ColorBoxInfo[i].m_dStartTime >0 )
		{
			double dCurTime =_pTimer->GetHighPrecisionTimer().GetSeconds();
			double dElapsedTime = dCurTime -m_ColorBoxInfo[i].m_dStartTime;

			if( dElapsedTime > m_ColorBoxInfo[i].m_dDurationTime )
				dElapsedTime =m_ColorBoxInfo[i].m_dDurationTime;

			double dProgressRate =dElapsedTime /m_ColorBoxInfo[i].m_dDurationTime;

			rcBox.Right = rcBox.Left + (rcBox.Right -rcBox.Left)*dProgressRate;
		}
			
		m_dbColorBox[nColorBoxType].SetBoxPos(rcBox);
		m_dbColorBox[nColorBoxType].Render( PosX, PosY);

		if( IsComplete(i) )
			MsgBoxCommand(i, nCommandCode);
	}
}

BOOL CUIColorBox::IsComplete(int index)
{
	if( m_ColorBoxInfo[index].m_dStartTime >0)
	{
		double dCurTime	= _pTimer->GetHighPrecisionTimer().GetSeconds();
		double dElapsedTime = dCurTime -m_ColorBoxInfo[index].m_dStartTime;
		return (dElapsedTime >m_ColorBoxInfo[index].m_dDurationTime);
	}
	else
		return FALSE;
}

void CUIColorBox::Reset()
{
	m_nColorBoxCount =0;

	if( m_ColorBoxInfo != NULL )
	{
		delete[] m_ColorBoxInfo;
		m_ColorBoxInfo =NULL;
	}
}

void CUIColorBox::MsgBoxCommand(int index, int nCommandCode)
{
	switch(nCommandCode)
	{
	case MSGCMD_MOONSTONE_MIX:
		{
			int nTab, nRow, nCol, nUniIndex;
			_pUIMgr->GetMessageBox(MSGCMD_MOONSTONE_MIX)->GetBtnUseItemInfo( nTab, nRow, nCol, nUniIndex );
			
			int iMoonStoneBox =_pNetwork->MySlotItem[nTab][nRow][nCol].Item_Index;

			int iMoonStone =_pUIMgr->GetMessageBox(MSGCMD_MOONSTONE_MIX)->GetBtnEx(0).GetItemIndex();
			
			_pNetwork->SendMoonStoneMix( iMoonStone, iMoonStoneBox );

			_pUIMgr->CloseMessageBox(MSGCMD_MOONSTONE_MIX);
		}
		break;
	}
}
/////								Class: CUIColorBox								/////
/////////////////////////////////////////////////////////////////////////////////////////


// ----------------------------------------------------------------------------
// Name : CUIMsgBox_Info()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIMsgBox_Info::CUIMsgBox_Info()
{
	Reset();
}

// ----------------------------------------------------------------------------
// Name : ~CUIMsgBox_Info()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIMsgBox_Info::~CUIMsgBox_Info()
{
}

// ----------------------------------------------------------------------------
// Name : Reset()
// Desc :
// ----------------------------------------------------------------------------
void CUIMsgBox_Info::Reset()
{
	m_dwStyle = 0;
	m_nWhichUI = -1;
	m_nCommandCode = -1;

	m_nCurStringCount = 0;
	m_nMaxRow = 0;
	m_strTitle = CTString( "" );
	for( int i = 0; i < MAX_MSGSTRING; i++ )
	{
		m_strMessage[i] = CTString( "" );
		m_nPosX[i] = MSGBOX_MESSAGE_OFFSETX;
		m_nPosY[i] = -1;
		m_taAlign[i] = TEXT_LEFT;
	}

	m_nInputPosX	= -1;
	m_nInputPosY	= -1;
	m_nInputMaxChar = 0;
	m_nWidth		= -1;
	m_nInputWidth	= MSGBOX_INPUT_WIDTH;

	m_nSEInputPosX	= -1;
	m_nSEInputPosY	= -1;
	m_nSEInputMaxChar = 0;
	m_nSEInputWidth = MSGBOX_INPUT_WIDTH;

	m_nBtnCount =0;

	m_nColorBoxCount =0;

	m_nSpinButtonPosX =-1;
	m_nSpinButtonPosY =-1;
	m_nSpinButtonWidth =-1;
	m_strSpinButtonTitle =CTString("");
}

// ----------------------------------------------------------------------------
// Name : SetMsgBoxInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIMsgBox_Info::SetMsgBoxInfo( CTString &strTitle, DWORD dwStyle, int nWhichUI, int nCommandCode, int nWidth, FLOAT fNPCPosX, FLOAT fNPCPosZ )
{
	m_strTitle = strTitle;
	m_dwStyle = dwStyle;
	m_nWhichUI = nWhichUI;
	m_nCommandCode = nCommandCode;
	m_fNPCPosX = fNPCPosX;
	m_fNPCPosZ = fNPCPosZ;

	if( nWidth == -1 )
	{
		m_nWidth = MSGBOX_WIDTH;
	}
	else
	{
		m_nWidth = nWidth;
	}
	
	_iMaxMsgStringChar = ( m_nWidth - 14 - MSGBOX_MESSAGE_OFFSETX ) / ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
}

// ----------------------------------------------------------------------------
// Name : AddString()
// Desc :
// ----------------------------------------------------------------------------
void CUIMsgBox_Info::AddString( CTString &strMessage, COLOR colMessage, TEXT_ALIGN eAlign )
{
	if( m_nCurStringCount >= MAX_MSGSTRING )
		return;

	// Get length of string
	INDEX	nLength = strMessage.Length();
	if( nLength == 0 )
		return;

	// wooss 051002
	if(g_iCountry == THAILAND){
		// Get length of string
		INDEX	nThaiLen = FindThaiLen(strMessage);
		INDEX	nChatMax= (_iMaxMsgStringChar-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
		if( nLength == 0 )
			return;
		// If length of string is less than max char
		if( nThaiLen <= nChatMax )
		{
			// Check line character
			for( int iPos = 0; iPos < nLength; iPos++ )
			{
				if( strMessage[iPos] == '\n' || strMessage[iPos] == '\r' )
					break;
			}

			// Not exist
			if( iPos == nLength )
			{
				m_strMessage[m_nCurStringCount] = strMessage;
				m_colMessage[m_nCurStringCount] = colMessage;
				m_taAlign[m_nCurStringCount] = eAlign;

/*				if( eAlign == TEXT_CENTER )
				{
					int nLength = strMessage.Length()/2;
					m_nPosX[m_nCurStringCount] = ( MESSAGEBOX_CHAR_WIDTH + MSGBOX_MESSAGE_OFFSETX ) / 2 - nLength * ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
				}*/

				m_nPosY[m_nCurStringCount++] = MSGBOX_MESSAGE_OFFSETY + m_nMaxRow * _pUIFontTexMgr->GetLineHeight();
				m_nMaxRow++;
			}
			else
			{
				// Split string
				CTString	strTemp;
				strMessage.Split( iPos, m_strMessage[m_nCurStringCount], strTemp );
				m_colMessage[m_nCurStringCount] = colMessage;
				m_taAlign[m_nCurStringCount] = eAlign;
/*				if( eAlign == TEXT_CENTER )
				{
					int nLength = strTemp.Length()/2;
					
					m_nPosX[m_nCurStringCount] = ( MESSAGEBOX_CHAR_WIDTH + MSGBOX_MESSAGE_OFFSETX ) / 2 - nLength * ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
				}*/
				m_nPosY[m_nCurStringCount++] = MSGBOX_MESSAGE_OFFSETY + m_nMaxRow * _pUIFontTexMgr->GetLineHeight();
				m_nMaxRow++;

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddString( strTemp, colMessage, eAlign );
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
				if(nChatMax < FindThaiLen(strMessage,0,iPos))
					break;
			}
			nSplitPos = iPos;

			// Check line character
			for( iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strMessage[iPos] == '\n' || strMessage[iPos] == '\r' )
					break;
			}

			// Not exist
			if( iPos == nSplitPos )
			{
				// Split string
				CTString	strTemp;
				strMessage.Split( nSplitPos, m_strMessage[m_nCurStringCount], strTemp );
				m_colMessage[m_nCurStringCount] = colMessage;
				m_taAlign[m_nCurStringCount] = eAlign;
				m_nPosY[m_nCurStringCount++] = MSGBOX_MESSAGE_OFFSETY + m_nMaxRow * _pUIFontTexMgr->GetLineHeight();
				m_nMaxRow++;

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

				AddString( strTemp, colMessage, eAlign );
			}
			else
			{
				// Split string
				CTString	strTemp;
				strMessage.Split( iPos, m_strMessage[m_nCurStringCount], strTemp );
				m_colMessage[m_nCurStringCount] = colMessage;
				m_taAlign[m_nCurStringCount] = eAlign;
				m_nPosY[m_nCurStringCount++] = MSGBOX_MESSAGE_OFFSETY + m_nMaxRow * _pUIFontTexMgr->GetLineHeight();
				m_nMaxRow++;

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddString( strTemp, colMessage, eAlign );
			}

		}
		
	} else {
		// If length of string is less than max char
		if( nLength <= _iMaxMsgStringChar )
		{
			// Check line character
			for( int iPos = 0; iPos < nLength; iPos++ )
			{
				if( strMessage[iPos] == '\n' || strMessage[iPos] == '\r' )
					break;
			}

			// Not exist
			if( iPos == nLength )
			{
				m_strMessage[m_nCurStringCount] = strMessage;
				m_colMessage[m_nCurStringCount] = colMessage;
				m_taAlign[m_nCurStringCount] = eAlign;

/*				if( eAlign == TEXT_CENTER )
				{
					int nLength = strMessage.Length()/2;
					
					
					m_nPosX[m_nCurStringCount] = ( MESSAGEBOX_CHAR_WIDTH + MSGBOX_MESSAGE_OFFSETX ) / 2 - nLength * ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
				}*/
				

				m_nPosY[m_nCurStringCount++] = MSGBOX_MESSAGE_OFFSETY + m_nMaxRow * _pUIFontTexMgr->GetLineHeight();
				m_nMaxRow++;
			}
			else
			{
				// Split string
				CTString	strTemp;
				strMessage.Split( iPos, m_strMessage[m_nCurStringCount], strTemp );
				m_colMessage[m_nCurStringCount] = colMessage;
				m_taAlign[m_nCurStringCount] = eAlign;
/*				if( eAlign == TEXT_CENTER )
				{
					int nLength = strTemp.Length()/2;
					
					m_nPosX[m_nCurStringCount] = ( MESSAGEBOX_CHAR_WIDTH + MSGBOX_MESSAGE_OFFSETX ) / 2 - nLength * ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
				}*/
				m_nPosY[m_nCurStringCount++] = MSGBOX_MESSAGE_OFFSETY + m_nMaxRow * _pUIFontTexMgr->GetLineHeight();
				m_nMaxRow++;

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddString( strTemp, colMessage, eAlign );
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
				if( strMessage[iPos] & 0x80 )
					b2ByteChar = !b2ByteChar;
				else
					b2ByteChar = FALSE;
			}

			if( b2ByteChar )
				nSplitPos--;

			// Check line character
			for( iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strMessage[iPos] == '\n' || strMessage[iPos] == '\r' )
					break;
			}

			// Not exist
			if( iPos == nSplitPos )
			{
				// Split string
				CTString	strTemp;
#ifdef LINE_CHANGE_BY_WORD
				if( strMessage[nSplitPos] != ' ' )
				{
					for(int i=iPos; i>=0; --i)
					{
						if( strMessage[i] == ' ' )
						{
							nSplitPos =i;
							break;
						}
					}
				}
#endif
				strMessage.Split( nSplitPos, m_strMessage[m_nCurStringCount], strTemp );
				m_colMessage[m_nCurStringCount] = colMessage;
				m_taAlign[m_nCurStringCount] = eAlign;
				m_nPosY[m_nCurStringCount++] = MSGBOX_MESSAGE_OFFSETY + m_nMaxRow * _pUIFontTexMgr->GetLineHeight();
				m_nMaxRow++;

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

				AddString( strTemp, colMessage, eAlign );
			}
			else
			{
				// Split string
				CTString	strTemp;
				strMessage.Split( iPos, m_strMessage[m_nCurStringCount], strTemp );
				m_colMessage[m_nCurStringCount] = colMessage;
				m_taAlign[m_nCurStringCount] = eAlign;
				m_nPosY[m_nCurStringCount++] = MSGBOX_MESSAGE_OFFSETY + m_nMaxRow * _pUIFontTexMgr->GetLineHeight();
				m_nMaxRow++;

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddString( strTemp, colMessage, eAlign );
			}
		}
	}
}

// ----------------------------------------------------------------------------
// Name : AddStringEx()
// Desc :
// ----------------------------------------------------------------------------
void CUIMsgBox_Info::AddStringEx( CTString &strMessage, int nRow, int nCol, COLOR colMessage, TEXT_ALIGN eAlign )
{
	if( m_nCurStringCount >= MAX_MSGSTRING )
		return;

	// Get length of string
	INDEX	nLength = strMessage.Length();
	if( nLength == 0 )
		return;

	m_strMessage[m_nCurStringCount] = strMessage;
	m_colMessage[m_nCurStringCount] = colMessage;
	m_taAlign[m_nCurStringCount] = eAlign;
	m_nPosX[m_nCurStringCount] = MSGBOX_MESSAGE_OFFSETX +
									nCol * ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
	m_nPosY[m_nCurStringCount++] = MSGBOX_MESSAGE_OFFSETY + nRow * _pUIFontTexMgr->GetLineHeight();
	

	if( nRow > m_nMaxRow )
		m_nMaxRow = nRow;
}

// ----------------------------------------------------------------------------
// Name : SetInputBox()
// Desc :
// ----------------------------------------------------------------------------
void CUIMsgBox_Info::SetInputBox( int nRow, int nCol, int nMaxChar, int nInputWidth )
{
	m_nInputPosX = MSGBOX_MESSAGE_OFFSETX +
					nCol * ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
	m_nInputPosY = MSGBOX_MESSAGE_OFFSETY + nRow * _pUIFontTexMgr->GetLineHeight() -
					( 13 - _pUIFontTexMgr->GetFontHeight() ) / 2;		// 13은 입력 상자 이미지의 높이
	m_nInputMaxChar = nMaxChar;

	if( nInputWidth <= -1 )
	{
		m_nInputWidth = MSGBOX_INPUT_WIDTH;
		return;
	}	
	else
	{
		m_nInputWidth = nInputWidth;
	}
}

// ----------------------------------------------------------------------------
// Name : SetSEInputBox()
// Desc :
// ----------------------------------------------------------------------------
void CUIMsgBox_Info::SetSEInputBox( int nRow, int nCol, int nMaxChar, int nInputWidth /* = -1 */, int nGapX /* = -1  */ )
{
	if( nGapX < 0 )
	{
		m_nSEInputPosX = MSGBOX_MESSAGE_OFFSETX +
					nCol * ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
	}
	else
	{
		m_nSEInputPosX = m_nInputPosX + m_nInputWidth + nGapX;
	}

	m_nSEInputPosY = MSGBOX_MESSAGE_OFFSETY + nRow * _pUIFontTexMgr->GetLineHeight() -
					( 13 - _pUIFontTexMgr->GetFontHeight() ) / 2;		// 13은 입력 상자 이미지의 높이

	m_nSEInputMaxChar = nMaxChar;

	if( nInputWidth < 0 )
	{
		m_nSEInputWidth = MSGBOX_INPUT_WIDTH;
	}
	else
	{
		m_nSEInputWidth = nInputWidth;
	}
}


//------------------------------------------------------------------------------
// CUIMessageBox::SetUserBtmName
// Explain:  
// Date : 2005-05-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMsgBox_Info::SetUserBtnName( CTString strBtnName1, CTString strBtnName2 )
{
	m_strUserBtn1 = strBtnName1;
	m_strUserBtn2 = strBtnName2;
}

void CUIMsgBox_Info::SetSpinButton(int nRow, int nCol, int nWidth, CTString strTitle)
{

	m_nSpinButtonPosX = MSGBOX_MESSAGE_OFFSETX +
					nCol * ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
	m_nSpinButtonPosY = MSGBOX_MESSAGE_OFFSETY + nRow * _pUIFontTexMgr->GetLineHeight() -
					( 13 - _pUIFontTexMgr->GetFontHeight() ) / 2;		// 13은 입력 상자 이미지의 높이

	m_nSpinButtonWidth =nWidth;
	m_strSpinButtonTitle =strTitle;
}

// ----------------------------------------------------------------------------
// Name : CUIMessageBox()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIMessageBox::CUIMessageBox()
{
	m_nMaxChar = -1;
	m_dwStyle = 0;
	m_nWhichUI = -1;
	m_nCommandCode = -1;
	m_nCurStringCount = 0;
	m_bTitleBarClick = FALSE;

	m_btnSlot =NULL;
	m_rcBtnSlot =NULL;
	m_rcBtnInsertSlot =NULL;
	m_nUniIndex = 0;
	m_nBtnCount =0;
}

// ----------------------------------------------------------------------------
// Name : ~CUIMessageBox()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIMessageBox::~CUIMessageBox()
{
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIMessageBox::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	_iMaxMsgStringChar = 187 / ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 216, 22 );
	m_rcInputBox.SetRect( 46, 0, 157, 13 );
	m_rcSEInputBox.SetRect( 46, 0, 157, 13 );
	m_rcConfirmBox.SetRect( 46, 0, 157, 13 );

#ifdef TEXTURE_MSGBOX_CHANGE_NA_20080902
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\CommonBtn.tex" ) );
	//m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MessageBox.tex" ) );
#else
	// Create message box texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MessageBox.tex" ) );
#endif
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight(); 

	// UV Coordinate of each part
	// Background
#ifdef TEXTURE_MSGBOX_CHANGE_NA_20080902
	m_rtBackTopL.SetUV( 0, 136, 22, 136+27, fTexWidth, fTexHeight );
	m_rtBackTopM.SetUV( 23, 136, 23+10, 136+27, fTexWidth, fTexHeight );
	m_rtBackTopR.SetUV( 117, 136, 117+26, 136+27, fTexWidth, fTexHeight );

	m_rtBackMiddle1L.SetUV( 0, 175, 22, 175+17, fTexWidth, fTexHeight );
	m_rtBackMiddle1M.SetUV( 23, 175, 23+10, 175+17, fTexWidth, fTexHeight );
	m_rtBackMiddle1R.SetUV( 117, 175, 117+26, 175+17, fTexWidth, fTexHeight );

	m_rtBackBottomL.SetUV( 0, 208, 22, 208+20, fTexWidth, fTexHeight );
	m_rtBackBottomM.SetUV( 23, 208, 23+10, 208+20, fTexWidth, fTexHeight );
	m_rtBackBottomR.SetUV( 117, 208, 117+26, 208+20, fTexWidth, fTexHeight );
#else
	m_rtBackTopL.SetUV( 0, 0, 20, 26, fTexWidth, fTexHeight );
	m_rtBackTopM.SetUV( 21, 0, 25, 26, fTexWidth, fTexHeight );
	m_rtBackTopR.SetUV( 196, 0, 216, 26, fTexWidth, fTexHeight );
	
	m_rtBackMiddle1L.SetUV( 0, 31, 20, 33, fTexWidth, fTexHeight );
	m_rtBackMiddle1M.SetUV( 21, 31, 25, 33, fTexWidth, fTexHeight );
	m_rtBackMiddle1R.SetUV( 196, 31, 216, 33, fTexWidth, fTexHeight );

	m_rtBackMiddle2L.SetUV( 0, 35, 20, 37, fTexWidth, fTexHeight );
	m_rtBackMiddle2M.SetUV( 21, 35, 25, 37, fTexWidth, fTexHeight );
	m_rtBackMiddle2R.SetUV( 196, 35, 216, 37, fTexWidth, fTexHeight );

	m_rtBackBottomL.SetUV( 0, 38, 20, 45, fTexWidth, fTexHeight );
	m_rtBackBottomM.SetUV( 21, 38, 25, 45, fTexWidth, fTexHeight );
	m_rtBackBottomR.SetUV( 196, 38, 216, 45, fTexWidth, fTexHeight );
#endif//TEXTURE_MSGBOX_CHANGE_NA_20080902

#ifdef TEXTURE_MSGBOX_CHANGE_NA_20080902
	FLOAT	fTextureWid = CreateTexture(CTString("Data\\Interface\\MessageBox.tex"))->GetPixWidth();
	FLOAT	fTextureHeight = CreateTexture(CTString("Data\\Interface\\MessageBox.tex"))->GetPixHeight();

	// Input box
	m_rtInputBoxL.SetUV( 131, 46, 135, 59, fTextureWid, fTextureHeight );
	m_rtInputBoxM.SetUV( 137, 46, 139, 59, fTextureWid, fTextureHeight );
	m_rtInputBoxR.SetUV( 141, 46, 145, 59, fTextureWid, fTextureHeight );

	// SE Input box
	m_rtSEInputBoxL.SetUV( 131, 46, 135, 59, fTextureWid, fTextureHeight );
	m_rtSEInputBoxM.SetUV( 137, 46, 139, 59, fTextureWid, fTextureHeight );
	m_rtSEInputBoxR.SetUV( 141, 46, 145, 59, fTextureWid, fTextureHeight );

	// Btn Slot
	m_rtBtnSlot.SetUV( 14, 410, 48, 444, 
					   CreateTexture( CTString( "Data\\Interface\\NewCharacterInfo.tex" ) )->GetPixWidth(), 
					   CreateTexture( CTString( "Data\\Interface\\NewCharacterInfo.tex" ) )->GetPixHeight() );
#else
	// Input box
	m_rtInputBoxL.SetUV( 131, 46, 135, 59, fTexWidth, fTexHeight );
	m_rtInputBoxM.SetUV( 137, 46, 139, 59, fTexWidth, fTexHeight );
	m_rtInputBoxR.SetUV( 141, 46, 145, 59, fTexWidth, fTexHeight );

	// SE Input box
	m_rtSEInputBoxL.SetUV( 131, 46, 135, 59, fTexWidth, fTexHeight );
	m_rtSEInputBoxM.SetUV( 137, 46, 139, 59, fTexWidth, fTexHeight );
	m_rtSEInputBoxR.SetUV( 141, 46, 145, 59, fTexWidth, fTexHeight );

	// Btn Slot
	m_rtBtnSlot.SetUV( 0, 68, 34, 102, fTexWidth, fTexHeight );
#endif

	// Ok button
#ifdef TEXTURE_MSGBOX_CHANGE_NA_20080902
	m_btnOk.Create( this, CTString( "" ), 0, 0, 63, 21 );
	m_btnOk.SetUV( UBS_IDLE, 112, 0, 112+71, 23, fTexWidth, fTexHeight );
	m_btnOk.SetUV( UBS_CLICK, 185, 0, 185+71, 23, fTexWidth, fTexHeight );
	m_btnOk.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOk.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Cancel button
	m_btnCancel.Create( this, CTString( "" ), 0, 0, 63, 21 );
	m_btnCancel.SetUV( UBS_IDLE, 112, 0, 112+71, 23, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 185, 0, 185+71, 23, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Close button 
	m_btnClose.Create( this, CTString( "" ), 0, 0, 15, 15 );
	m_btnClose.SetUV( UBS_IDLE, 210, 32, 210+18, 32+18, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 228, 32, 228+18, 32+18, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );
#else
	m_btnOk.Create( this, CTString( "" ), 0, 0, 63, 21 );
	m_btnOk.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnOk.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnOk.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOk.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Cancel button
	m_btnCancel.Create( this, CTString( "" ), 0, 0, 63, 21 );
	m_btnCancel.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );

	//WSS_GUILD_MASTER 070411 ----------------------->>
	// Close button 
	m_btnClose.Create( this, CTString( "" ), 0, 0, 15, 15 );
	m_btnClose.SetUV( UBS_IDLE, 218, 0, 233, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 234, 0, 249, 14, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );
	// ----------------------------------------------<<
#endif//TEXTURE_MSGBOX_CHANGE_NA_20080902

#ifdef TEXTURE_MSGBOX_CHANGE_NA_20080902
	// Input box
	m_ebInput.Create( this, 54, 0, 105, 13, 17, TRUE );
	m_ebInput.SetReadingWindowUV( 146, 46, 163, 62, fTextureWid, fTextureHeight );
	m_ebInput.SetCandidateUV( 146, 46, 163, 62, fTextureWid, fTextureHeight );

	// SE Input box
	m_ebSEInput.Create( this, 54, 0, 105, 13, 17, TRUE );
	m_ebSEInput.SetReadingWindowUV( 146, 46, 163, 62, fTextureWid, fTextureHeight );
	m_ebSEInput.SetCandidateUV( 146, 46, 163, 62, fTextureWid, fTextureHeight );

	// Confirm box
	m_ebConfirm.Create( this, 54, 0, 105, 13, 17, TRUE );
	m_ebConfirm.SetReadingWindowUV( 146, 46, 163, 62, fTextureWid, fTextureHeight );
	m_ebConfirm.SetCandidateUV( 146, 46, 163, 62, fTextureWid, fTextureHeight );

	// Combo Box
// 	m_cbList.Create( this, 100, 118, 127, 15, 115, 1, 13, 13,
// 						5, _pUIFontTexMgr->GetFontHeight() + 4, 4, 4 );
// 	m_cbList.SetBackUV( 5, 160, 5+100, 160+30, fTextureWid, fTextureHeight );
// 	m_cbList.SetDownBtnUV( 157, 48, 157+9, 48+7, fTextureWid, fTextureHeight );//13 / 12
// 	m_cbList.SetUpBtnUV( 170, 48, 170+9, 48+7, fTextureWid, fTextureHeight );//156, 45+12 ,156+12, 45	
// 	m_cbList.SetDropListUV( 100, 40, 100+4, 40+4, fTextureWid, fTextureHeight );
// 	// scroll bar
// 	m_cbList.CreateScroll(TRUE,10,9,0,0,10);
// 	// Up button
// 	m_cbList.SetScrollUpUV( UBS_IDLE, 157, 35, 157+9, 35+7, fTextureWid, fTextureHeight );
// 	m_cbList.SetScrollUpUV( UBS_CLICK, 170, 35, 170+9, 35+7, fTextureWid, fTextureHeight );
// 	m_cbList.CopyScrollUpUV( UBS_IDLE, UBS_ON );
// 	m_cbList.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
// 	// Bar button
// 	m_cbList.SetScrollBarTopUV( 185, 32, 185+10, 32+9, fTextureWid, fTextureHeight );//밝은
// 	m_cbList.SetScrollBarMiddleUV( 185, 32+9, 185+10, 62, fTextureWid, fTextureHeight );
// 	m_cbList.SetScrollBarBottomUV( 185, 62, 185+10, 62+9, fTextureWid, fTextureHeight );
// 	m_cbList.SetScrollBarTopUV( 198, 30, 198+10, 30+9, fTextureWid, fTextureHeight );//어두운
// 	m_cbList.SetScrollBarMiddleUV( 198, 30+9, 198+10, 69, fTextureWid, fTextureHeight );
// 	m_cbList.SetScrollBarBottomUV( 198, 69, 198+10, 69+9, fTextureWid, fTextureHeight );
// 	// Down button
// 	m_cbList.SetScrollDownUV( UBS_IDLE, 157, 48, 157+9, 48+7, fTextureWid, fTextureHeight );
// 	m_cbList.SetScrollDownUV( UBS_CLICK, 170, 48, 170+9, 48+7, fTextureWid, fTextureHeight );
// 	m_cbList.CopyScrollDownUV( UBS_IDLE, UBS_ON );
// 	m_cbList.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
// 	m_cbList.SetCurSel( 0 );

	// Combo Box
	m_cbList.Create( this, 100, 118, 127, 15, 115, 1, 13, 13,
						5, _pUIFontTexMgr->GetFontHeight() + 4, 4, 4 );
	m_cbList.SetBackUV( 153, 216, 252, 245, fTextureWid, fTextureHeight ); 
	m_cbList.SetDownBtnUV( 232, 150, 244, 161, fTextureWid, fTextureHeight );
	m_cbList.SetUpBtnUV( 232, 161 , 244, 150, fTextureWid, fTextureHeight );	
	m_cbList.SetDropListUV( 149,92, 152, 95, fTextureWid, fTextureHeight );
	// scroll bar
	m_cbList.CreateScroll(TRUE,9,7,0,0,10);
	// Up button
	m_cbList.SetScrollUpUV( UBS_IDLE, 231, 18, 240, 25, fTextureWid, fTextureHeight );
	m_cbList.SetScrollUpUV( UBS_CLICK, 241, 18, 250, 25, fTextureWid, fTextureHeight );
	m_cbList.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_cbList.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_cbList.SetScrollBarTopUV( 220, 17, 230, 21, fTextureWid, fTextureHeight );
	m_cbList.SetScrollBarMiddleUV( 220, 21, 230, 35, fTextureWid, fTextureHeight );
	m_cbList.SetScrollBarBottomUV( 220, 35, 230, 39, fTextureWid, fTextureHeight );
	// Down button
	m_cbList.SetScrollDownUV( UBS_IDLE, 231, 27, 240, 31, fTextureWid, fTextureHeight );
	m_cbList.SetScrollDownUV( UBS_CLICK,  241, 27, 250, 31, fTextureWid, fTextureHeight );
	m_cbList.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_cbList.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	m_cbList.SetCurSel( 0 );

	//m_btnSlot.Create( this, 0, 0, BTN_SIZE, BTN_SIZE );

	//Spin Button
	int iIncrease = 5;
	m_sbtnSpin.Create( this, 0, 0, 60, 15+2, CTString(""), 37 );
	m_sbtnSpin.SetDataBackUV3( 138, 74, 138+iIncrease, 74+(iIncrease*3),  138+iIncrease+1, 74, 138+(iIncrease*2), 74+(iIncrease*3), 
							138+(iIncrease*2)+1, 74, 138+(iIncrease*3), 74+(iIncrease*3), fTexWidth, fTexHeight );
	
	m_sbtnSpin.SetUpUV( UBS_IDLE, 156, 33, 156+10, 33+10, fTexWidth, fTexHeight );
	m_sbtnSpin.SetUpUV( UBS_CLICK, 168, 33, 168+10, 33+10, fTexWidth, fTexHeight );
	m_sbtnSpin.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbtnSpin.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	
	m_sbtnSpin.SetDownUV( UBS_IDLE, 156, 45, 156+10, 45+10, fTexWidth, fTexHeight );
	m_sbtnSpin.SetDownUV( UBS_CLICK, 168, 45, 168+10, 45+10, fTexWidth, fTexHeight );
	m_sbtnSpin.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbtnSpin.CopyDownUV( UBS_IDLE, UBS_DISABLE );

	m_sbtnSpin.CreateButtons( 11, 8 );
	m_sbtnSpin.SetWheelRect( -10, -10, 20, 20 );
	m_sbtnSpin.SetEnable(FALSE);
#else
	// Input box
	m_ebInput.Create( this, 54, 0, 105, 13, 17, TRUE );
	m_ebInput.SetReadingWindowUV( 146, 46, 163, 62, fTexWidth, fTexHeight );
	m_ebInput.SetCandidateUV( 146, 46, 163, 62, fTexWidth, fTexHeight );

	// SE Input box
	m_ebSEInput.Create( this, 54, 0, 105, 13, 17, TRUE );
	m_ebSEInput.SetReadingWindowUV( 146, 46, 163, 62, fTexWidth, fTexHeight );
	m_ebSEInput.SetCandidateUV( 146, 46, 163, 62, fTexWidth, fTexHeight );

	// Confirm box
	m_ebConfirm.Create( this, 54, 0, 105, 13, 17, TRUE );
	m_ebConfirm.SetReadingWindowUV( 146, 46, 163, 62, fTexWidth, fTexHeight );
	m_ebConfirm.SetCandidateUV( 146, 46, 163, 62, fTexWidth, fTexHeight );

	// Combo Box
	m_cbList.Create( this, 100, 118, 127, 15, 115, 1, 13, 13,
						5, _pUIFontTexMgr->GetFontHeight() + 4, 4, 4 );
	m_cbList.SetBackUV( 153, 216, 252, 245, fTexWidth, fTexHeight );
	m_cbList.SetDownBtnUV( 232, 150, 244, 161, fTexWidth, fTexHeight );
	m_cbList.SetUpBtnUV( 232, 161 , 244, 150, fTexWidth, fTexHeight );	
	m_cbList.SetDropListUV( 149,92, 152, 95, fTexWidth, fTexHeight );
	// scroll bar
	m_cbList.CreateScroll(TRUE,9,7,0,0,10);
	// Up button
	m_cbList.SetScrollUpUV( UBS_IDLE, 231, 18, 240, 25, fTexWidth, fTexHeight );
	m_cbList.SetScrollUpUV( UBS_CLICK, 241, 18, 250, 25, fTexWidth, fTexHeight );
	m_cbList.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_cbList.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_cbList.SetScrollBarTopUV( 220, 17, 230, 21, fTexWidth, fTexHeight );
	m_cbList.SetScrollBarMiddleUV( 220, 21, 230, 35, fTexWidth, fTexHeight );
	m_cbList.SetScrollBarBottomUV( 220, 35, 230, 39, fTexWidth, fTexHeight );
	// Down button
	m_cbList.SetScrollDownUV( UBS_IDLE, 231, 27, 240, 31, fTexWidth, fTexHeight );
	m_cbList.SetScrollDownUV( UBS_CLICK,  241, 27, 250, 31, fTexWidth, fTexHeight );
	m_cbList.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_cbList.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	m_cbList.SetCurSel( 0 );

	//m_btnSlot.Create( this, 0, 0, BTN_SIZE, BTN_SIZE );

	//Spin Button
	int black = 74; // black 를 0으로 놓으면 회색 바탕의 Edit 박스 생성
	m_sbtnSpin.Create( this, 0, 0, 60, 15, CTString(""), 37 );
	m_sbtnSpin.SetDataBackUV3( 131+black, 46, 135+black, 59, 136+black, 46, 140+black, 59, 
							141 + black, 46, 145 + black, 59, fTexWidth, fTexHeight );
	
	m_sbtnSpin.SetUpUV( UBS_IDLE, 230, 16, 239, 23, fTexWidth, fTexHeight );
	m_sbtnSpin.SetUpUV( UBS_CLICK, 240, 16, 249, 23, fTexWidth, fTexHeight );
	m_sbtnSpin.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbtnSpin.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	
	m_sbtnSpin.SetDownUV( UBS_IDLE, 230, 24, 239, 31, fTexWidth, fTexHeight );
	m_sbtnSpin.SetDownUV( UBS_CLICK, 240, 24, 249, 31, fTexWidth, fTexHeight );
	m_sbtnSpin.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbtnSpin.CopyDownUV( UBS_IDLE, UBS_DISABLE );

	m_sbtnSpin.CreateButtons( 9, 7 );
	m_sbtnSpin.SetWheelRect( -10, -10, 20, 20 );
	m_sbtnSpin.SetEnable(FALSE);
#endif//TEXTURE_MSGBOX_CHANGE_NA_20080902
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIMessageBox::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIMessageBox::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : CreateMessageBox()
// Desc :
// ----------------------------------------------------------------------------
void CUIMessageBox::CreateMessageBox( CUIMsgBox_Info &rMsgBoxInfo, int nPosX, int nPosY )
{
	SetWidth( rMsgBoxInfo.m_nWidth );

	int	nHeight = 0;
	int	nX, nY;
	// Message box setting
	if( nPosX != -1 && nPosX != -1 )
	{
		SetPos( nPosX, nPosY );
	}
	else
	{
		nX = ( _pUIMgr->GetMaxI() - _pUIMgr->GetMinI() - GetWidth() ) / 2;
		nY = ( _pUIMgr->GetMaxJ() - _pUIMgr->GetMinJ() - GetHeight() ) / 5;
		SetPos( nX, nY );
	}
	m_dwStyle = rMsgBoxInfo.m_dwStyle;
	m_nWhichUI = rMsgBoxInfo.m_nWhichUI;
	m_nCommandCode = rMsgBoxInfo.m_nCommandCode;
	m_strTitle = rMsgBoxInfo.m_strTitle;
	m_nCurStringCount = rMsgBoxInfo.m_nCurStringCount;

	m_fNPCPosX = rMsgBoxInfo.m_fNPCPosX;
	m_fNPCPosZ = rMsgBoxInfo.m_fNPCPosZ;

	for( int i = 0; i < m_nCurStringCount; i++ )
	{
		m_strMessage[i] = rMsgBoxInfo.m_strMessage[i];
		m_colMessage[i] = rMsgBoxInfo.m_colMessage[i];
		m_nStrPosX[i] = rMsgBoxInfo.m_nPosX[i];
		m_nStrPosY[i] = rMsgBoxInfo.m_nPosY[i];
		m_taAlign[i] = rMsgBoxInfo.m_taAlign[i];
	}

	//WSS_GUILD_MASTER 070411 ----------------------->>
	//LIST BOX
	//List box of read type
	if( m_dwStyle & UMBS_LISTBOX )
	{
		int fTexWidth	= rMsgBoxInfo.m_listBoxPlacement.width;
		int fTexHeight	= rMsgBoxInfo.m_listBoxPlacement.height;
		int fColumnCnt	= rMsgBoxInfo.m_listBoxPlacement.columnCnt;
		int ilineCab	= rMsgBoxInfo.m_listBoxPlacement.lineCab;	
		m_lbListBox.Create( this, 
			rMsgBoxInfo.m_listBoxPlacement.pos_x,
			rMsgBoxInfo.m_listBoxPlacement.pos_y,
			fTexWidth,fTexHeight,
			_pUIFontTexMgr->GetLineHeight() + ilineCab, 0, 0, fColumnCnt, FALSE );
		m_lbListBox.CreateScroll( TRUE, 0, 0, 9, fTexHeight, 9, 7, 0, 0, 10 );
		m_lbListBox.SetOverColor( 0x66B5FFFF );	

#ifdef TEXTURE_MSGBOX_CHANGE_NA_20080902
 		FLOAT	fTexListWidth = CreateTexture(CTString("Data\\Interface\\MessageBox.tex"))->GetPixWidth();
 		FLOAT	fTexListHeight = CreateTexture(CTString("Data\\Interface\\MessageBox.tex"))->GetPixHeight();
		// Scroll bar of list box
		// Up button
		m_lbListBox.SetScrollUpUV( UBS_IDLE, 231, 18, 240, 25, fTexListWidth, fTexListHeight );
		m_lbListBox.SetScrollUpUV( UBS_CLICK, 241, 18, 250, 25, fTexListWidth, fTexListHeight );
		m_lbListBox.CopyScrollUpUV( UBS_IDLE, UBS_ON );
		m_lbListBox.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
		// Bar button
		m_lbListBox.SetScrollBarTopUV( 220, 17, 230, 21, fTexListWidth, fTexListHeight );
		m_lbListBox.SetScrollBarMiddleUV( 220, 21, 230, 35, fTexListWidth, fTexListHeight );
		m_lbListBox.SetScrollBarBottomUV( 220, 35, 230, 39, fTexListWidth, fTexListHeight );
		// Down button
		m_lbListBox.SetScrollDownUV( UBS_IDLE, 231, 27, 240, 31, fTexListWidth, fTexListHeight );
		m_lbListBox.SetScrollDownUV( UBS_CLICK,  241, 27, 250, 31, fTexListWidth, fTexListHeight );
		m_lbListBox.CopyScrollDownUV( UBS_IDLE, UBS_ON );
		m_lbListBox.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );	
#else
		// Scroll bar of list box
		fTexWidth = m_ptdBaseTexture->GetPixWidth();
		fTexHeight = m_ptdBaseTexture->GetPixHeight();
		// Up button
		m_lbListBox.SetScrollUpUV( UBS_IDLE, 231, 18, 240, 25, fTexWidth, fTexHeight );
		m_lbListBox.SetScrollUpUV( UBS_CLICK, 241, 18, 250, 25, fTexWidth, fTexHeight );
		m_lbListBox.CopyScrollUpUV( UBS_IDLE, UBS_ON );
		m_lbListBox.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
		// Bar button
		m_lbListBox.SetScrollBarTopUV( 220, 17, 230, 21, fTexWidth, fTexHeight );
		m_lbListBox.SetScrollBarMiddleUV( 220, 21, 230, 35, fTexWidth, fTexHeight );
		m_lbListBox.SetScrollBarBottomUV( 220, 35, 230, 39, fTexWidth, fTexHeight );
		// Down button
		m_lbListBox.SetScrollDownUV( UBS_IDLE, 231, 27, 240, 31, fTexWidth, fTexHeight );
		m_lbListBox.SetScrollDownUV( UBS_CLICK,  241, 27, 250, 31, fTexWidth, fTexHeight );
		m_lbListBox.CopyScrollDownUV( UBS_IDLE, UBS_ON );
		m_lbListBox.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );		
// #endif
#endif
	}

	// ----------------------------------------------<<

	// Input box
	int	nInputBoxHeight = 0;
	int	nMaxRow = rMsgBoxInfo.m_nMaxRow;
	if( nMaxRow < 3 ) nMaxRow = 3;
	if( m_dwStyle & UMBS_INPUT_MASK )
	{
		if( m_dwStyle & UMBS_INPUTBOX )
		{
			m_ebInput.SetPassWord(FALSE);

			if( m_dwStyle & UMBS_SECOND_INPUTBOX )
				m_ebSEInput.SetPassWord( FALSE );
		}
		else if( m_dwStyle & UMBS_INPUTPASSWORD )
		{
			m_ebInput.SetPassWord(TRUE, TRUE);
			if(m_dwStyle & UMBS_CONFIRMPASSWORD ){
				m_ebConfirm.SetPassWord(TRUE, FALSE);
				m_ebConfirm.SetWidth( rMsgBoxInfo.m_nInputWidth );
				m_ebConfirm.ResetShowCount();
				m_ebConfirm.SetEnable( TRUE );
				m_ebConfirm.SetFocus( FALSE );
			}
		}
		
		m_ebInput.SetWidth( rMsgBoxInfo.m_nInputWidth );
		m_ebInput.ResetShowCount();
		m_ebInput.SetEnable( TRUE );
		m_ebInput.SetFocus( TRUE );

		if( rMsgBoxInfo.m_nInputMaxChar > 0 )
			m_ebInput.SetMaxChar( rMsgBoxInfo.m_nInputMaxChar );
		else
			m_ebInput.SetMaxChar( 17 ); // 기본 최대 캐릭터 길이 

		if( m_dwStyle & UMBS_SECOND_INPUTBOX )		// Date : 2006-06-01(오후 4:39:49), By eons
		{
			m_ebSEInput.SetWidth( rMsgBoxInfo.m_nSEInputWidth );
			m_ebSEInput.ResetShowCount();
			m_ebSEInput.SetEnable( TRUE );
			m_ebSEInput.SetFocus( FALSE );

			if( rMsgBoxInfo.m_nSEInputMaxChar > 0 )
				m_ebSEInput.SetMaxChar( rMsgBoxInfo.m_nSEInputMaxChar );
			else
				m_ebSEInput.SetMaxChar( 17 ); // 기본 캐릭터 길이
		}

		if( rMsgBoxInfo.m_nInputPosX < 0 )
		{
			int	nOffsetY = 5 - ( 13 - _pUIFontTexMgr->GetFontHeight() ) / 2;
			nX = MSGBOX_INPUT_DEFAULT_POSX;
			nY = MSGBOX_MESSAGE_OFFSETY + nMaxRow * _pUIFontTexMgr->GetLineHeight() + nOffsetY;
#ifdef TEXTURE_MSGBOX_CHANGE_NA_20080902
			const int iadd = 3;
			m_ebInput.SetPos( nX + 4, nY+iadd );
			m_rcInputBox.SetRect( nX, nY+iadd, nX + rMsgBoxInfo.m_nInputWidth, nY + 13+iadd );
#else
			m_ebInput.SetPos( nX + 4, nY );
			m_rcInputBox.SetRect( nX, nY, nX + rMsgBoxInfo.m_nInputWidth, nY + 13 );
#endif
			if( m_dwStyle & UMBS_CONFIRMPASSWORD )
			{
#ifdef TEXTURE_MSGBOX_CHANGE_NA_20080902
				m_ebInput.SetPos( nX + 14, nY - 13+iadd);
				m_ebConfirm.SetPos( nX + 14, nY +5+iadd);
				m_rcConfirmBox.SetRect( nX+10, nY + 5+iadd, nX + rMsgBoxInfo.m_nInputWidth +10, nY + 18+iadd );
#else
				m_ebInput.SetPos( nX + 14, nY - 13);
				m_ebConfirm.SetPos( nX + 14, nY +5);
				m_rcConfirmBox.SetRect( nX+10, nY + 5, nX + rMsgBoxInfo.m_nInputWidth +10, nY + 18 );
#endif
			}
			nInputBoxHeight = m_ebInput.GetHeight() + nOffsetY + _pUIFontTexMgr->GetLineSpacing();
		}
		else
		{
			int	nOffsetY = ( 13 - _pUIFontTexMgr->GetFontHeight() ) / 2;
			m_ebInput.SetPos( rMsgBoxInfo.m_nInputPosX + 4, rMsgBoxInfo.m_nInputPosY );
			m_rcInputBox.SetRect( rMsgBoxInfo.m_nInputPosX, rMsgBoxInfo.m_nInputPosY,
									rMsgBoxInfo.m_nInputPosX + rMsgBoxInfo.m_nInputWidth, rMsgBoxInfo.m_nInputPosY + 13 );
			if( m_dwStyle & UMBS_CONFIRMPASSWORD )
			{
				m_ebInput.SetPos( rMsgBoxInfo.m_nInputPosX + 14, rMsgBoxInfo.m_nInputPosY -13);
				m_ebConfirm.SetPos( rMsgBoxInfo.m_nInputPosX + 14, rMsgBoxInfo.m_nInputPosY +5);
				m_rcConfirmBox.SetRect( rMsgBoxInfo.m_nInputPosX +10, rMsgBoxInfo.m_nInputPosY +5,
									rMsgBoxInfo.m_nInputPosX +rMsgBoxInfo.m_nInputWidth +10 , rMsgBoxInfo.m_nInputPosY + 18 );
			}

			if( m_dwStyle & UMBS_SECOND_INPUTBOX )		// Date : 2006-06-01(오후 4:39:49), By eons
			{
				m_ebSEInput.SetPos( rMsgBoxInfo.m_nSEInputPosX + 4, rMsgBoxInfo.m_nSEInputPosY );
				m_rcSEInputBox.SetRect( rMsgBoxInfo.m_nSEInputPosX, rMsgBoxInfo.m_nSEInputPosY,
								rMsgBoxInfo.m_nSEInputPosX + rMsgBoxInfo.m_nSEInputWidth, rMsgBoxInfo.m_nSEInputPosY + 13 );
			}

			nY = MSGBOX_MESSAGE_OFFSETY + nMaxRow * _pUIFontTexMgr->GetLineHeight() - nOffsetY;
			if( rMsgBoxInfo.m_nInputPosY + m_ebInput.GetHeight() > nY )
				nInputBoxHeight = nY - ( rMsgBoxInfo.m_nInputPosY + m_ebInput.GetHeight() ) +
									 _pUIFontTexMgr->GetLineSpacing();
		}
	}
	// List Box
	if( m_dwStyle & UMBS_LISTBOX )
	{
		m_nTextRegionHeight = m_lbListBox.GetHeight();
		m_nWidth  = m_lbListBox.GetWidth() + 30;
		m_rcTitle.SetRect( 0, 0, m_nWidth - 35, 22 );
		//nHeight += 50;
		nHeight += m_lbListBox.GetHeight() +20;
	}
	else 
	{
		m_nTextRegionHeight = nMaxRow * _pUIFontTexMgr->GetLineHeight() + nInputBoxHeight +
							16 - _pUIFontTexMgr->GetLineSpacing();
	}

	// Combo Box
	if( m_dwStyle & UMBS_COMBOBOX )
	{
		m_cbList.SetEnable( TRUE );
#ifdef TEXTURE_MSGBOX_CHANGE_NA_20080902
		m_cbList.SetPos( m_nWidth / 2 - m_cbList.GetWidth()/2 , nHeight + ( m_cbList.GetMaxLine()) * _pUIFontTexMgr->GetLineHeight() + 10);
#else
		m_cbList.SetPos( m_nWidth / 2 - m_cbList.GetWidth()/2 , nHeight + ( m_cbList.GetMaxLine()) * _pUIFontTexMgr->GetLineHeight());
#endif		
		m_nTextRegionHeight += (nMaxRow+m_cbList.GetMaxLine()) * _pUIFontTexMgr->GetLineHeight() ;
		nHeight += (nMaxRow+ m_cbList.GetMaxLine()) * _pUIFontTexMgr->GetLineHeight() ;
	}

	nHeight += MSGBOX_MESSAGE_OFFSETY + nMaxRow * _pUIFontTexMgr->GetLineHeight() + nInputBoxHeight +
				8 - _pUIFontTexMgr->GetLineSpacing();	

	///////////////////////////////////////////////////////////////////////////////
	//SpinButton
	//{
	if( m_dwStyle & UMBS_SPINBUTTON )
	{
		m_sbtnSpin.SetEnable(TRUE);

		int nPosX, nPosY;

		if( rMsgBoxInfo.m_nSpinButtonPosX < 0 )
			nPosX = MSGBOX_MESSAGE_OFFSETX;
		else
			nPosX =rMsgBoxInfo.m_nSpinButtonPosX;

		if( rMsgBoxInfo.m_nSpinButtonPosY < 0 )
			nPosY = nHeight;
		else
			nPosY =rMsgBoxInfo.m_nSpinButtonPosY;

		m_sbtnSpin.SetPos( nPosX, nPosY );

		if( rMsgBoxInfo.m_nSpinButtonWidth > 0)
			m_sbtnSpin.SetWidth( rMsgBoxInfo.m_nSpinButtonWidth );

		if( rMsgBoxInfo.m_strSpinButtonTitle != CTString("") )
			m_sbtnSpin.SetTitle( rMsgBoxInfo.m_strSpinButtonTitle );

		m_nTextRegionHeight +=20;
		nHeight +=20;
	}
	//}
	////////////////////////////////////////////////////////////////////////////////

	// Btn slot
	if( m_dwStyle & UMBS_BUTTONEX )
	{
		m_nBtnType = rMsgBoxInfo.m_nBtnType;
		m_nItemType = rMsgBoxInfo.m_nItemType;
		m_nItemSubType = rMsgBoxInfo.m_nItemSubType;
		m_nBtnCount =rMsgBoxInfo.m_nBtnCount;

		m_btnSlot =new CUIButtonEx[m_nBtnCount];
		m_rcBtnSlot = new UIRect[m_nBtnCount];
		m_rcBtnInsertSlot = new UIRect[m_nBtnCount];

		for(int i=0; i<m_nBtnCount; ++i)
		{
			m_btnSlot[i].Create( this, 0, 0, BTN_SIZE, BTN_SIZE );
			m_btnSlot[i].InitBtn();

			m_btnSlot[i].SetPos( (m_nWidth/(m_nBtnCount+1))*(i+1) - ( BTN_SIZE / 2 ), nHeight + 6 );

			//m_rcBtnSlot[i].SetRect( ( m_nWidth / 2 ) - 17, nHeight+5, ( m_nWidth / 2 ) + 17, nHeight + 39 );
			m_rcBtnSlot[i].SetRect( m_btnSlot[i].GetPosX()-1, m_btnSlot[i].GetPosY()-1, 
									m_btnSlot[i].GetPosX()-1+ BTN_SIZE+2, m_btnSlot[i].GetPosY()-1+ BTN_SIZE+2);
			m_rcBtnInsertSlot[i].SetRect( m_rcBtnSlot[i].Left-10, m_rcBtnSlot[i].Top-10, m_rcBtnSlot[i].Right+10, m_rcBtnSlot[i].Bottom+10);
		}

		nHeight += 49;
	}

	int m_nBtn1Width = 0;
	int m_nBtn2Width = 0;

	#define BTN_2BTN_SPACE	3
	#define BTN_OFFSET		10
	// Buttons
	nHeight += 10;
	// wooss 050804 
	// select box
	if(m_dwStyle & UMBS_SELECTBOX)
	{
		switch(m_nCommandCode)
		{
			case MSGCMD_USE_RECOVER_ITEM_HEXP:
			case MSGCMD_USE_RECOVER_ITEM_AEXP:
			case MSGCMD_USE_RECOVER_ITEM_HEXP_LUCKY:
			case MSGCMD_USE_RECOVER_ITEM_AEXP_LUCKY:
			case MSGCMD_USE_REBIRTH_ITEM:
			
				addSelectContext( _S( 2144, "가까운 시작지점에서 부활" ) ); 
				addSelectContext( _S( 2145, "사망장소에서 부활" ) ); 
				break;

			case MSGCMD_USE_SLOT_ITEM:
				addSelectContext(_S( 2112,  "확장슬롯 1" )); 
				addSelectContext( _S( 2113, "확장슬롯 2" )); 
				break;
			
			default:
				break;
		}
	
	}
	else if( m_dwStyle & UMBS_ONEBTN_MASK )
	{
		// 버튼 위치 조정 
		if( m_dwStyle & UMBS_ALIGN_LEFT )
		{
			m_nBtn1Width = BTN_OFFSET;
		}
		else if( m_dwStyle & UMBS_ALIGN_RIGHT )
		{
			m_nBtn1Width = m_nWidth - BTN_OFFSET - m_btnOk.GetWidth();
		}
		else // UMBS_ALIGN_CENTER
		{
			m_nBtn1Width = m_nWidth / 2 - m_btnOk.GetWidth() / 2;
		}

		if( m_dwStyle & UMBS_OKBTN_MASK )
		{
			m_btnOk.SetEnable( TRUE );
			m_btnOk.SetPos( m_nBtn1Width, nHeight );
		}
		else
		{
			m_btnCancel.SetEnable( TRUE );
			m_btnCancel.SetPos( m_nBtn1Width, nHeight );
		}

		if( m_dwStyle & UMBS_OK )
			m_btnOk.SetText( _S( 191, "확인" ) );
		else if( m_dwStyle & UMBS_CANCEL )
			m_btnCancel.SetText( _S( 139, "취소" ) );
		else if( m_dwStyle & UMBS_YES )
			m_btnOk.SetText( _S( 192, "예" ) );
		else if( m_dwStyle & UMBS_NO )
			m_btnCancel.SetText( _S( 193, "아니오" ) );
		else if( m_dwStyle & UMBS_USER_1 )
			m_btnOk.SetText( rMsgBoxInfo.m_strUserBtn1 );
		else if( m_dwStyle & UMBS_USER_2 )
			m_btnCancel.SetText( rMsgBoxInfo.m_strUserBtn1 );
	}
	else
	{
		// 버튼 위치 조정 
		if( m_dwStyle & UMBS_ALIGN_LEFT )
		{
			m_nBtn1Width = BTN_OFFSET;
			m_nBtn2Width = m_nBtn1Width + m_btnOk.GetWidth() + BTN_2BTN_SPACE;
		}
		else if( m_dwStyle & UMBS_ALIGN_RIGHT )
		{
			m_nBtn1Width = m_nWidth - BTN_OFFSET - m_btnOk.GetWidth() * 2 - BTN_2BTN_SPACE;
			m_nBtn2Width = m_nBtn1Width + m_btnOk.GetWidth() + BTN_2BTN_SPACE;
		}
		else // UMBS_ALIGN_CENTER
		{
			m_nBtn1Width = m_nWidth / 2 - m_btnOk.GetWidth();
			m_nBtn2Width = m_nWidth / 2 + BTN_2BTN_SPACE;
		}

		if( m_dwStyle & UMBS_BUTTONEX )
			m_btnOk.SetEnable( FALSE );
		else
			m_btnOk.SetEnable( TRUE );

		m_btnCancel.SetEnable( TRUE );
		m_btnOk.SetPos( m_nBtn1Width, nHeight );
		m_btnCancel.SetPos( m_nBtn2Width, nHeight );

		if( m_dwStyle & UMBS_OKCANCEL )
		{
			m_btnOk.SetText( _S( 191, "확인" ) );
			m_btnCancel.SetText( _S( 139, "취소" ) );
		}
		else if( m_dwStyle & UMBS_YESNO )
		{
			m_btnOk.SetText( _S( 192, "예" ) );
			m_btnCancel.SetText( _S( 193, "아니오" ) );
		}
		else if( m_dwStyle & UMBS_USER_12 )
		{
			m_btnOk.SetText( rMsgBoxInfo.m_strUserBtn1  );
			m_btnCancel.SetText( rMsgBoxInfo.m_strUserBtn2 );
		}
	}

	nHeight += m_btnOk.GetHeight() + 7;
	SetHeight( nHeight );

	// WSS_GUILD_MASTER 070411 ---------------------->>
	// Close Button Position
	m_btnClose.SetPos(m_nWidth-35,4);	
	// ----------------------------------------------<<

	if( rMsgBoxInfo.m_nColorBoxCount >0 )
		m_cboxColorBox.Create(rMsgBoxInfo.m_nColorBoxCount, m_ptdBaseTexture);

	_pUIMgr->RearrangeOrder( m_nUIIndex, TRUE );
}

// ----------------------------------------------------------------------------
// Name : InitMessageBox()
// Desc :
// ----------------------------------------------------------------------------
void CUIMessageBox::InitMessageBox()
{
	m_dwStyle = 0;
	m_nWhichUI = -1;
	m_nCommandCode = -1;
	m_nCurStringCount = 0;

	m_btnOk.SetEnable( FALSE );
	m_btnCancel.SetEnable( FALSE );
	
	//wooss 050805
	//추가된 select button을 제거한다
	for(;m_pBtnlist.size()>0;) m_pBtnlist.pop_back();

	m_ebInput.ResetString();
	m_ebInput.SetFocus( FALSE );
	m_ebInput.SetEnable( FALSE );
	m_ebInput.SetInValidEditBox(FALSE);

	m_ebSEInput.ResetString();
	m_ebSEInput.SetFocus( FALSE );
	m_ebSEInput.SetEnable( FALSE );
	m_ebSEInput.SetInValidEditBox(FALSE);

	m_ebConfirm.ResetString();
	m_ebConfirm.SetFocus( FALSE );
	m_ebConfirm.SetEnable( FALSE );
	m_ebConfirm.SetInValidEditBox(FALSE);

	//m_btnSlot.InitBtn();
	if( m_btnSlot != NULL)
	{
		delete[] m_btnSlot;
		m_btnSlot =NULL;
	}

	if( m_rcBtnSlot != NULL)
	{
		delete[] m_btnSlot;
		m_rcBtnSlot =NULL;
	}

	if( m_rcBtnInsertSlot != NULL)
	{
		delete[] m_btnSlot;
		m_rcBtnInsertSlot =NULL;
	}

	m_sbtnSpin.ResetAllDatas();

	m_cboxColorBox.Reset();

	_pUIMgr->RearrangeOrder( m_nUIIndex, FALSE );
}

// ----------------------------------------------------------------------------
// Name : addSelectContext() 
// Desc : wooss 050803
// ----------------------------------------------------------------------------
void CUIMessageBox::addSelectContext(CTString str)
{
	if( m_dwStyle == UMBS_SELECTBOX)
	{
		CTString tv_str=str;
		CUIButton new_btn;
		
		FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
		FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();
		
// 	FLOAT	fTexWidth = CreateTexture(CTString("Data\\Interface\\MessageBox.tex"))->GetPixWidth();
// 	FLOAT	fTexHeight = CreateTexture(CTString("Data\\Interface\\MessageBox.tex"))->GetPixHeight();
		
#ifdef TEXTURE_MSGBOX_CHANGE_NA_20080902
		new_btn.Create( this, tv_str, 8+10, 33+10+(m_pBtnlist.size())*30,180,25 );
		new_btn.SetUV( UBS_IDLE, 129, 0, 128+39, 21, fTexWidth, fTexHeight );
		new_btn.SetUV( UBS_CLICK, 203, 0, 202+39, 21, fTexWidth, fTexHeight );
#else
		new_btn.Create( this, tv_str, 8, 33+(m_pBtnlist.size())*30,200,25 );
		new_btn.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
		new_btn.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
#endif
		new_btn.CopyUV( UBS_IDLE, UBS_ON );
		new_btn.CopyUV( UBS_IDLE, UBS_DISABLE );

		new_btn.SetEnable(TRUE);
//		new_btn.SetVisible(TRUE);
		m_pBtnlist.push_back(new_btn);				
	}
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIMessageBox::Render()
{

	if(m_fNPCPosX >= 0.0f && m_fNPCPosZ >= 0.0f)
	{
		FLOAT	fDiffX = _pNetwork->MyCharacterInfo.x - m_fNPCPosX;
		FLOAT	fDiffZ = _pNetwork->MyCharacterInfo.z - m_fNPCPosZ;
		if( fDiffX * fDiffX + fDiffZ * fDiffZ > UI_VALID_SQRDIST )
		{
			_pUIMgr->CloseMessageBox(m_nCommandCode);
		}
	}

	// Set texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	int	nX, nY, nX2, nY2;
	// Background
	// Top
	nX = m_nPosX + m_nWidth;
	nY = m_nPosY + 26;

#ifndef TEXTURE_MSGBOX_CHANGE_NA_20080902//if not
	//WSS_GUILD_MASTER 070411 ----------------------->>
	if(m_dwStyle & UMBS_LISTBOX)
	{
		m_lbListBox.Render();
	}	
	// ----------------------------------------------<<
#endif

	if(m_dwStyle & UMBS_SELECTBOX)
	{
	int tv_height = nY + (m_pBtnlist.size()+1)*25;
	
	// Title Bar
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY, m_nPosX + 20, nY,
										m_rtBackTopL.U0, m_rtBackTopL.V0,
										m_rtBackTopL.U1, m_rtBackTopL.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + 20, m_nPosY, nX - 20, nY,
										m_rtBackTopM.U0, m_rtBackTopM.V0,
										m_rtBackTopM.U1, m_rtBackTopM.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( nX - 20, m_nPosY, nX, nY,
										m_rtBackTopR.U0, m_rtBackTopR.V0,
										m_rtBackTopR.U1, m_rtBackTopR.V1,
										0xFFFFFFFF );

	// Middle 1
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, m_nPosX+20, tv_height,
										m_rtBackMiddle1L.U0, m_rtBackMiddle1L.V0,
										m_rtBackMiddle1L.U1, m_rtBackMiddle1L.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX+20, nY, nX-20, tv_height,
										m_rtBackMiddle1M.U0, m_rtBackMiddle1M.V0,
										m_rtBackMiddle1M.U1, m_rtBackMiddle1M.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( nX-20, nY, nX, tv_height,
										m_rtBackMiddle1R.U0, m_rtBackMiddle1R.V0,
										m_rtBackMiddle1R.U1, m_rtBackMiddle1R.V1,
										0xFFFFFFFF );

#ifdef TEXTURE_MSGBOX_CHANGE_NA_20080902
	// Bottom
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, tv_height, m_nPosX+20, tv_height + 20,
										m_rtBackBottomL.U0, m_rtBackBottomL.V0,
										m_rtBackBottomL.U1, m_rtBackBottomL.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX+20, tv_height, nX-20, tv_height + 20,
										m_rtBackBottomM.U0, m_rtBackBottomM.V0,
										m_rtBackBottomM.U1, m_rtBackBottomM.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture(  nX-20, tv_height, nX, tv_height + 20,
										m_rtBackBottomR.U0, m_rtBackBottomR.V0,
										m_rtBackBottomR.U1, m_rtBackBottomR.V1,
										0xFFFFFFFF );
#else
	// Bottom
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, tv_height, m_nPosX+20, tv_height + 7,
										m_rtBackBottomL.U0, m_rtBackBottomL.V0,
										m_rtBackBottomL.U1, m_rtBackBottomL.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX+20, tv_height, nX-20, tv_height + 7,
										m_rtBackBottomM.U0, m_rtBackBottomM.V0,
										m_rtBackBottomM.U1, m_rtBackBottomM.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture(  nX-20, tv_height, nX, tv_height + 7,
										m_rtBackBottomR.U0, m_rtBackBottomR.V0,
										m_rtBackBottomR.U1, m_rtBackBottomR.V1,
										0xFFFFFFFF );
#endif
		std::list<CUIButton>::iterator iList,iListEnd;
		iList=m_pBtnlist.begin();
		iListEnd=m_pBtnlist.end();
		while(iList!=iListEnd)
		{
			iList->Render();
			iList++;
		}

	}//m_dwStyle & UMBS_SELECTBOX
	else
	{
	// Title Bar
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY, m_nPosX + 20, nY,
										m_rtBackTopL.U0, m_rtBackTopL.V0,
										m_rtBackTopL.U1, m_rtBackTopL.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + 20, m_nPosY, nX - 20, nY,
										m_rtBackTopM.U0, m_rtBackTopM.V0,
										m_rtBackTopM.U1, m_rtBackTopM.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( nX - 20, m_nPosY, nX, nY,
										m_rtBackTopR.U0, m_rtBackTopR.V0,
										m_rtBackTopR.U1, m_rtBackTopR.V1,
										0xFFFFFFFF );

#ifdef TEXTURE_MSGBOX_CHANGE_NA_20080902
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, m_nPosX+20, (m_nPosY + m_nHeight - 20),
										m_rtBackMiddle1L.U0, m_rtBackMiddle1L.V0,
										m_rtBackMiddle1L.U1, m_rtBackMiddle1L.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX+20, nY, nX-20, (m_nPosY + m_nHeight - 20),
										m_rtBackMiddle1M.U0, m_rtBackMiddle1M.V0,
										m_rtBackMiddle1M.U1, m_rtBackMiddle1M.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( nX-20, nY, nX, (m_nPosY + m_nHeight - 20),
										m_rtBackMiddle1R.U0, m_rtBackMiddle1R.V0,
										m_rtBackMiddle1R.U1, m_rtBackMiddle1R.V1,
										0xFFFFFFFF );
	// Bottom
	nY = m_nPosY + m_nHeight - 20;
#else
	// Middle 1
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, m_nPosX+20, nY + m_nTextRegionHeight,
										m_rtBackMiddle1L.U0, m_rtBackMiddle1L.V0,
										m_rtBackMiddle1L.U1, m_rtBackMiddle1L.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX+20, nY, nX-20, nY + m_nTextRegionHeight,
										m_rtBackMiddle1M.U0, m_rtBackMiddle1M.V0,
										m_rtBackMiddle1M.U1, m_rtBackMiddle1M.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( nX-20, nY, nX, nY + m_nTextRegionHeight,
										m_rtBackMiddle1R.U0, m_rtBackMiddle1R.V0,
										m_rtBackMiddle1R.U1, m_rtBackMiddle1R.V1,
										0xFFFFFFFF );

	// Middle 2
	nY += m_nTextRegionHeight;
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, m_nPosX+20, m_nPosY + m_nHeight - 7,
										m_rtBackMiddle2L.U0, m_rtBackMiddle2L.V0,
										m_rtBackMiddle2L.U1, m_rtBackMiddle2L.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX+20, nY, nX-20, m_nPosY + m_nHeight - 7,
										m_rtBackMiddle2M.U0, m_rtBackMiddle2M.V0,
										m_rtBackMiddle2M.U1, m_rtBackMiddle2M.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( nX-20, nY, nX, m_nPosY + m_nHeight - 7,
										m_rtBackMiddle2R.U0, m_rtBackMiddle2R.V0,
										m_rtBackMiddle2R.U1, m_rtBackMiddle2R.V1,
										0xFFFFFFFF );
	// Bottom
	nY = m_nPosY + m_nHeight - 7;
#endif//TEXTURE_MSGBOX_CHANGE_NA_20080902
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, m_nPosX+20, m_nPosY + m_nHeight,
										m_rtBackBottomL.U0, m_rtBackBottomL.V0,
										m_rtBackBottomL.U1, m_rtBackBottomL.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX+20, nY, nX-20, m_nPosY + m_nHeight,
										m_rtBackBottomM.U0, m_rtBackBottomM.V0,
										m_rtBackBottomM.U1, m_rtBackBottomM.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( nX-20, nY, nX, m_nPosY + m_nHeight,
										m_rtBackBottomR.U0, m_rtBackBottomR.V0,
										m_rtBackBottomR.U1, m_rtBackBottomR.V1,
										0xFFFFFFFF );


	// Input box

	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	_pUIMgr->GetDrawPort()->InitTextureData(CreateTexture(CTString("Data\\Interface\\MessageBox.tex")));

	if( m_dwStyle & UMBS_INPUT_MASK )
	{
		nX = m_nPosX + m_rcInputBox.Left;
		nX2 = m_nPosX + m_rcInputBox.Right;
		nY = m_nPosY + m_rcInputBox.Top;
		nY2 = m_nPosY + m_rcInputBox.Bottom;

		if(m_dwStyle & UMBS_CONFIRMPASSWORD)
		{			
			nY = m_nPosY + m_rcInputBox.Top -13;
			nY2 = m_nPosY + m_rcInputBox.Bottom -13;

			_pUIMgr->GetDrawPort()->AddTexture( nX+10, nY, nX + 14, nY2,
											m_rtInputBoxL.U0, m_rtInputBoxL.V0,
											m_rtInputBoxL.U1, m_rtInputBoxL.V1,
											0xFFFFFFFF );
			_pUIMgr->GetDrawPort()->AddTexture( nX + 14, nY, nX2 + 6, nY2,
												m_rtInputBoxM.U0, m_rtInputBoxM.V0,
												m_rtInputBoxM.U1, m_rtInputBoxM.V1,
												0xFFFFFFFF );
			_pUIMgr->GetDrawPort()->AddTexture( nX2 + 6, nY, nX2+10, nY2,
												m_rtInputBoxR.U0, m_rtInputBoxR.V0,
												m_rtInputBoxR.U1, m_rtInputBoxR.V1,
												0xFFFFFFFF );
	
			nY = m_nPosY + m_rcConfirmBox.Top;
			nY2 = m_nPosY + m_rcConfirmBox.Bottom;
			_pUIMgr->GetDrawPort()->AddTexture( nX+10, nY, nX + 14, nY2,
												m_rtInputBoxL.U0, m_rtInputBoxL.V0,
												m_rtInputBoxL.U1, m_rtInputBoxL.V1,
												0xFFFFFFFF );
			_pUIMgr->GetDrawPort()->AddTexture( nX + 14, nY, nX2 +6, nY2,
												m_rtInputBoxM.U0, m_rtInputBoxM.V0,
												m_rtInputBoxM.U1, m_rtInputBoxM.V1,
												0xFFFFFFFF );
			_pUIMgr->GetDrawPort()->AddTexture( nX2 + 6, nY, nX2+10, nY2,
												m_rtInputBoxR.U0, m_rtInputBoxR.V0,
												m_rtInputBoxR.U1, m_rtInputBoxR.V1,
												0xFFFFFFFF );
#ifdef TEXTURE_MSGBOX_CHANGE_NA_20080902
			_pUIMgr->GetDrawPort()->PutTextEx( _S(2278, "입력" ), m_nPosX + 25, m_nPosY + 64+2 );		
			_pUIMgr->GetDrawPort()->PutTextEx( _S(191, "확인" ), m_nPosX + 25, m_nPosY + 83+2 );
#else
			_pUIMgr->GetDrawPort()->PutTextEx( _S(2278, "입력" ), m_nPosX + 25, m_nPosY + 64 );		
			_pUIMgr->GetDrawPort()->PutTextEx( _S(191, "확인" ), m_nPosX + 25, m_nPosY + 83 );
#endif
		}
		else 
		{		
			_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + 4, nY2,
												m_rtInputBoxL.U0, m_rtInputBoxL.V0,
												m_rtInputBoxL.U1, m_rtInputBoxL.V1,
												0xFFFFFFFF );
			_pUIMgr->GetDrawPort()->AddTexture( nX + 4, nY, nX2 - 4, nY2,
												m_rtInputBoxM.U0, m_rtInputBoxM.V0,
												m_rtInputBoxM.U1, m_rtInputBoxM.V1,
												0xFFFFFFFF );
			_pUIMgr->GetDrawPort()->AddTexture( nX2 - 4, nY, nX2, nY2,
												m_rtInputBoxR.U0, m_rtInputBoxR.V0,
												m_rtInputBoxR.U1, m_rtInputBoxR.V1,
												0xFFFFFFFF );

			nX = m_nPosX + m_rcSEInputBox.Left;
			nX2 = m_nPosX + m_rcSEInputBox.Right;
			nY = m_nPosY + m_rcSEInputBox.Top;
			nY2 = m_nPosY + m_rcSEInputBox.Bottom;			

			if(m_dwStyle & UMBS_SECOND_INPUTBOX )
			{
				_pUIMgr->GetDrawPort()->AddTexture( nX, nY, nX + 4, nY2,
													m_rtSEInputBoxL.U0, m_rtSEInputBoxL.V0,
													m_rtSEInputBoxL.U1, m_rtSEInputBoxL.V1,
													0xFFFFFFFF );
				_pUIMgr->GetDrawPort()->AddTexture( nX + 4, nY, nX2 - 4, nY2,
													m_rtSEInputBoxM.U0, m_rtSEInputBoxM.V0,
													m_rtSEInputBoxM.U1, m_rtSEInputBoxM.V1,
													0xFFFFFFFF );
				_pUIMgr->GetDrawPort()->AddTexture( nX2 - 4, nY, nX2, nY2,
													m_rtSEInputBoxR.U0, m_rtSEInputBoxR.V0,
													m_rtSEInputBoxR.U1, m_rtSEInputBoxR.V1,
													0xFFFFFFFF );				
			}
		}
	}

	_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

#ifndef TEXTURE_MSGBOX_CHANGE_NA_20080902//if not
	// combo box 
	if( m_dwStyle & UMBS_COMBOBOX )
		m_cbList.Render();
#endif
	
	//Spint Button
	if( m_dwStyle & UMBS_SPINBUTTON )
		m_sbtnSpin.Render();

	// Ok button
	if( m_dwStyle & UMBS_OKBTN_MASK )
		m_btnOk.Render();

	// Cancel button
	if( m_dwStyle & UMBS_CANCELBTN_MASK )
		m_btnCancel.Render();

	// Input box
	if( m_dwStyle & UMBS_INPUT_MASK )
	{
		m_ebInput.Render();
		if( m_dwStyle & UMBS_CONFIRMPASSWORD )
			m_ebConfirm.Render();

		if( m_dwStyle & UMBS_SECOND_INPUTBOX )
			m_ebSEInput.Render();
	}

	// WSS_GUILD_MASTER 070411 ---------------------->>
	// Render Close button

	if( m_nWhichUI != UI_AUCTION )
		m_btnClose.Render();
	// ------------------------------------------------<<
	}

	if( m_dwStyle & UMBS_BUTTONEX )
	{

		_pUIMgr->GetDrawPort()->FlushRenderingQueue();

		_pUIMgr->GetDrawPort()->InitTextureData(CreateTexture( CTString( "Data\\Interface\\NewCharacterInfo.tex" )));

		for(int i=0; i<m_nBtnCount; ++i)
		{
			_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcBtnSlot[i].Left, m_nPosY + m_rcBtnSlot[i].Top,
												m_nPosX + m_rcBtnSlot[i].Right, m_nPosY + m_rcBtnSlot[i].Bottom,
												m_rtBtnSlot.U0, m_rtBtnSlot.V0, m_rtBtnSlot.U1, m_rtBtnSlot.V1,
												0xFFFFFFFF );
		}
		
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();
		
		_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );
	}

	//ColorBox
	int iAbsPosX, iAbsPosY;
	this->GetAbsPos(iAbsPosX, iAbsPosY);
	m_cboxColorBox.Render(iAbsPosX, iAbsPosY, m_nCommandCode);

	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	if( m_dwStyle & UMBS_BUTTONEX )
	{
		for(int i=0; i<m_nBtnCount; ++i)
		{
			if( !m_btnSlot[i].IsEmpty())
			{
				m_btnSlot[i].Render();
				_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( m_nBtnType );
			}
		}
	}

	// Title
#ifdef TEXTURE_MSGBOX_CHANGE_NA_20080902
	_pUIMgr->GetDrawPort()->PutTextEx(m_strTitle, m_nPosX+MSGBOX_TITLE_TEXT_OFFSETX,
										m_nPosY+MSGBOX_TITLE_TEXT_OFFSETY+5, 0x42FFD8FF, 0, TRUE);
#else
	_pUIMgr->GetDrawPort()->PutTextEx( m_strTitle, m_nPosX + MSGBOX_TITLE_TEXT_OFFSETX,
										m_nPosY + MSGBOX_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );
#endif
	// Message strings
	for( int iMsg = 0; iMsg < m_nCurStringCount; iMsg++ )
	{
		switch(m_taAlign[iMsg])
		{
		case TEXT_LEFT:
			{
#ifdef TEXTURE_MSGBOX_CHANGE_NA_20080902
				_pUIMgr->GetDrawPort()->PutTextEx( m_strMessage[iMsg], m_nPosX + m_nStrPosX[iMsg],
					m_nPosY + m_nStrPosY[iMsg] + 5, m_colMessage[iMsg] );
#else
				_pUIMgr->GetDrawPort()->PutTextEx( m_strMessage[iMsg], m_nPosX + m_nStrPosX[iMsg],
					m_nPosY + m_nStrPosY[iMsg], m_colMessage[iMsg] );
#endif
			}
			break;
		case TEXT_CENTER:
			{
#ifdef TEXTURE_MSGBOX_CHANGE_NA_20080902
				_pUIMgr->GetDrawPort()->PutTextExCX( m_strMessage[iMsg], m_nPosX + ((m_nWidth)/2),
					m_nPosY + m_nStrPosY[iMsg] + 5, m_colMessage[iMsg] );
#else
				_pUIMgr->GetDrawPort()->PutTextExCX( m_strMessage[iMsg], m_nPosX + ((m_nWidth)/2),
					m_nPosY + m_nStrPosY[iMsg], m_colMessage[iMsg] );
#endif
			}
			break;
		case TEXT_RIGHT:
			{
#ifdef TEXTURE_MSGBOX_CHANGE_NA_20080902
				_pUIMgr->GetDrawPort()->PutTextExRX( m_strMessage[iMsg], m_nPosX + m_nWidth -m_nStrPosX[iMsg],
					m_nPosY + m_nStrPosY[iMsg] + 5, m_colMessage[iMsg] );
#else
				_pUIMgr->GetDrawPort()->PutTextExRX( m_strMessage[iMsg], m_nPosX + m_nWidth -m_nStrPosX[iMsg],
					m_nPosY + m_nStrPosY[iMsg], m_colMessage[iMsg] );
#endif
			}
			break;
		}		
	}

	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();
	// Reading window
	if( m_dwStyle & UMBS_INPUT_MASK )
	{
		if( m_ebInput.DoesShowReadingWindow() )
		{
			// Set texture
#ifdef TEXTURE_MSGBOX_CHANGE_NA_20080902
			_pUIMgr->GetDrawPort()->InitTextureData(CreateTexture(CTString("Data\\Interface\\MessageBox.tex")));
#else
			_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );
#endif
			// Reading window
			m_ebInput.RenderReadingWindow();
			// Render all elements
			_pUIMgr->GetDrawPort()->FlushRenderingQueue();
			// Flush all render text queue
			_pUIMgr->GetDrawPort()->EndTextEx();
		}
		
		if( m_dwStyle & UMBS_SECOND_INPUTBOX )
		{
			if( m_ebSEInput.DoesShowReadingWindow() )
			{
				// Set texture
#ifdef TEXTURE_MSGBOX_CHANGE_NA_20080902
			_pUIMgr->GetDrawPort()->InitTextureData(CreateTexture(CTString("Data\\Interface\\MessageBox.tex")));
#else
				_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );
#endif
				// Reading window
				m_ebSEInput.RenderReadingWindow();
				// Render all elements
				_pUIMgr->GetDrawPort()->FlushRenderingQueue();
				// Flush all render text queue
				_pUIMgr->GetDrawPort()->EndTextEx();
			}
		}
	}
#ifdef TEXTURE_MSGBOX_CHANGE_NA_20080902
	// combo box 
	if( (m_dwStyle & UMBS_COMBOBOX) )
	{
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();
		_pUIMgr->GetDrawPort()->InitTextureData(CreateTexture(CTString("Data\\Interface\\MessageBox.tex")));
		m_cbList.Render();		
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();
		_pUIMgr->GetDrawPort()->EndTextEx();
	}
#endif	
#ifdef TEXTURE_MSGBOX_CHANGE_NA_20080902
	//WSS_GUILD_MASTER 070411 ----------------------->>
	if(m_dwStyle & UMBS_LISTBOX /*|| m_dwStyle & UMBS_SELECTBOX*/)
	{
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();
		_pUIMgr->GetDrawPort()->InitTextureData(CreateTexture(CTString("Data\\Interface\\MessageBox.tex")));
		m_lbListBox.Render();

// 		std::list<CUIButton>::iterator iList,iListEnd;
// 		iList=m_pBtnlist.begin();
// 		iListEnd=m_pBtnlist.end();
// 		while(iList!=iListEnd)
// 		{
// 			iList->Render();
// 			iList++;
// 		}

		_pUIMgr->GetDrawPort()->FlushRenderingQueue();
		_pUIMgr->GetDrawPort()->EndTextEx();
	}	
	// ----------------------------------------------<<
#endif
}

// ----------------------------------------------------------------------------
// Name : ReturnCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUIMessageBox::ReturnCommand( BOOL bOK )
{
	CTString	strInput = CTString( "" );
	CTString	strSEInput = CTString( "" );
	CTString	strConfirm = CTString( "" );

	if( bOK )
	{
		// Get input string
		if( m_ebInput.GetString() )
			strInput = m_ebInput.GetString();
	
		if( m_ebSEInput.GetString() )
			strSEInput = m_ebSEInput.GetString();

		if( m_ebConfirm.GetString() )
			strConfirm = m_ebConfirm.GetString();
		
		// Return message box command ( ok )
		if( m_nWhichUI < 0 )
		{
			if( m_dwStyle & UMBS_SECOND_INPUTBOX )
				_pUIMgr->MsgBoxCommand( m_nCommandCode, bOK, strInput, strSEInput );
			else
				_pUIMgr->MsgBoxCommand( m_nCommandCode, bOK, strInput );
		}
		else
		{
			if(m_dwStyle & UMBS_CONFIRMPASSWORD) 
				_pUIMgr->GetUI( m_nWhichUI )->MsgBoxCommand( m_nCommandCode, bOK, strInput , strConfirm);
			else if(m_dwStyle & UMBS_SECOND_INPUTBOX) 
				_pUIMgr->GetUI( m_nWhichUI )->MsgBoxCommand( m_nCommandCode, bOK, strInput , strSEInput);
			else 
				_pUIMgr->GetUI( m_nWhichUI )->MsgBoxCommand( m_nCommandCode, bOK, strInput );
		}
	}
	else
	{
		// Return message box command ( cancel )
		if( m_nWhichUI < 0 )
			_pUIMgr->MsgBoxCommand( m_nCommandCode, bOK, strInput );
		else
			_pUIMgr->GetUI( m_nWhichUI )->MsgBoxCommand( m_nCommandCode, bOK, strInput );
	}

	//wooss 050805
	if(IsBoxInit(m_nCommandCode)) InitMessageBox();
}

// ----------------------------------------------------------------------------
// Name : KeyMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIMessageBox::KeyMessage( MSG *pMsg )
{
	// If message box is not focused
	if( !IsFocused() )
		return WMSG_FAIL;

	if( pMsg->wParam == VK_RETURN )
	{
		if( m_dwStyle & ( UMBS_CANCEL | UMBS_NO ) )
			ReturnCommand( FALSE );
		else if( m_btnOk.IsEnabled() )
			ReturnCommand( TRUE );

		return WMSG_SUCCESS;
	}
	else if( m_ebInput.KeyMessage( pMsg ) != WMSG_FAIL )
	{
		return WMSG_SUCCESS;
	}
	else if( m_ebSEInput.KeyMessage( pMsg ) != WMSG_FAIL )
	{
		return WMSG_SUCCESS;
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : CharMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIMessageBox::CharMessage( MSG *pMsg )
{
	if( m_ebInput.CharMessage( pMsg ) != WMSG_FAIL )
	{
		return WMSG_SUCCESS;
	}
	else if( m_ebSEInput.CharMessage( pMsg ) != WMSG_FAIL )
	{
		return WMSG_SUCCESS;
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : IMEMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIMessageBox::IMEMessage( MSG *pMsg )
{
	// 이기환 수정 (11.17) : IME 처리 루틴 변경
	if( m_dwStyle & UMBS_SECOND_INPUTBOX )
	{
		WMSG_RESULT wmsgResult;

		if( ( wmsgResult = m_ebInput.IMEMessage( pMsg ) ) != WMSG_FAIL )
		{
			return wmsgResult;
		}
		else if( ( wmsgResult = m_ebSEInput.IMEMessage( pMsg ) ) != WMSG_FAIL )
		{
			return wmsgResult;
		}
	}

	return m_ebInput.IMEMessage( pMsg );
	// 이기환 수정 끝 
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIMessageBox::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

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

			// If message box isn't focused
			if( !IsFocused() )
				return WMSG_FAIL;

			// Move message box
			if( m_bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			// OK button
			else if( m_btnOk.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Cancel button
			else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			//Combo Box
			else if( m_cbList.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			//WSS_GUILD_MASTER 070411 ----------------------->><<
			else if( m_lbListBox.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_sbtnSpin.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				nOldX = nX;		nOldY = nY;

				//Combo Box
				if(m_dwStyle & UMBS_COMBOBOX)
				{
					if(m_cbList.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
				// Input box
				if( ( m_dwStyle & UMBS_INPUT_MASK ) && m_ebInput.MouseMessage( pMsg ) != WMSG_FAIL && 
					( ~m_dwStyle & UMBS_SECOND_INPUTBOX ) )
				{
					_pUIMgr->RearrangeOrder( m_nUIIndex, TRUE );
					return WMSG_SUCCESS;
				}
				
				// Title bar
				if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					m_bTitleBarClick = TRUE;
				}
				// OK button
				else if( m_btnOk.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Cancel button
				else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// WSS_GUILD_MASTER 070411 ----------------------->>
				// ListBox
				else if( m_lbListBox.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS;
				}
				// Close button
				else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// -----------------------------------------------<<
				
				//Spin Button
				else if( m_sbtnSpin.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS;
				}			
				
				// Input box
				else  if(  m_dwStyle & UMBS_INPUT_MASK ){
					if(m_ebInput.MouseMessage( pMsg ) != WMSG_FAIL) {
						m_ebConfirm.SetFocus( FALSE );
						m_ebSEInput.SetFocus( FALSE );
						_pUIMgr->RearrangeOrder( m_nUIIndex, TRUE );
						return WMSG_SUCCESS;
					} 
					else if( m_ebSEInput.MouseMessage( pMsg ) != WMSG_FAIL && ( m_dwStyle & UMBS_SECOND_INPUTBOX ) ) {
						m_ebInput.SetFocus( FALSE );
						m_ebConfirm.SetFocus( FALSE );
						_pUIMgr->RearrangeOrder( m_nUIIndex, TRUE );
						return WMSG_SUCCESS;
					}
					else if(m_ebConfirm.MouseMessage( pMsg ) != WMSG_FAIL) {
						m_ebInput.SetFocus( FALSE );
						m_ebSEInput.SetFocus( FALSE );
						_pUIMgr->RearrangeOrder( m_nUIIndex, TRUE );
						return WMSG_SUCCESS;
					}
				}
				else
				{	//wooss 050805 추가된 기능 버튼 처리
					m_iBtnlist=m_pBtnlist.begin();
					for(;m_iBtnlist!=m_pBtnlist.end();m_iBtnlist++)
					{
						if(m_iBtnlist->MouseMessage(pMsg) != WMSG_FAIL)
						{
							//TODO ::  

						}
					}
				}

				_pUIMgr->RearrangeOrder( m_nUIIndex, TRUE );
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
				m_bTitleBarClick = FALSE;

				// If message box isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				//Combo Box
				if(m_dwStyle & UMBS_COMBOBOX)
				{
					if(m_cbList.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
				// OK button
				if( ( wmsgResult = m_btnOk.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						ReturnCommand( TRUE );

					return WMSG_SUCCESS;
				}
				// Cancel button
				else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						ReturnCommand( FALSE );

					return WMSG_SUCCESS;
				}
				//List box
				else if( ( wmsgResult = m_lbListBox.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS;
				}
				// Close button
				else if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( m_nWhichUI > 0 )
							_pUIMgr->CloseMessageBox(m_nCommandCode);					
					return WMSG_SUCCESS;
				}
				if( ( wmsgResult = m_sbtnSpin.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( m_nCommandCode == MSGCMD_SIEGE_DUNGEON_ENVIRONMENT_CONTROL )
					{
						int nData =m_sbtnSpin.GetDataToNumber();

						CTString str;
						str.PrintF( _s("%s: %d %s%s"), _S(1058, "필요나스"), nData*50000, _S(1762, "나스"), _s("   ") );
						this->SetString(m_nCurStringCount-1, str);
					}

					if( m_nCommandCode == MSGCMD_SIEGE_DUNGEON_MONSTER_CONTROL )
					{
						int nData =m_sbtnSpin.GetDataToNumber();

						CTString str;
						str.PrintF( _s("%s: %d %s%s"), _S(1058, "필요나스"), nData*100000, _S(1762, "나스"), _s("   ") );
						this->SetString(m_nCurStringCount-1, str);
					}
					return WMSG_SUCCESS;
				}
			
				else
				{	//wooss 050805 추가된 기능 버튼 처리
					CTString	strInput = CTString( "" );
					m_iBtnlist=m_pBtnlist.begin();
					for(int btnNum=0;m_iBtnlist!=m_pBtnlist.end();m_iBtnlist++,btnNum++)
					{ 
						if((wmsgResult = (m_iBtnlist->MouseMessage(pMsg))) != WMSG_FAIL)
						{
							if(wmsgResult == WMSG_COMMAND) 
								_pUIMgr->MsgBoxBtnCommand( m_nCommandCode, btnNum, strInput );

							return WMSG_SUCCESS;	
						}
					}
				}
			}
			// If holding button exists
			else
			{
				if( IsInside( nX, nY ) )
				{
					// If holding button is item and comes from inventory
/*					if( m_dwStyle & UMBS_BUTTONEX &&
						_pUIMgr->GetHoldBtn().GetBtnType() == m_nBtnType &&
						_pUIMgr->GetHoldBtn().GetWhichUI() == UI_INVENTORY )*/
// wooss Fix - call me later ^^;					
					if( m_dwStyle & UMBS_BUTTONEX &&
						_pUIMgr->GetHoldBtn().GetWhichUI() == UI_INVENTORY )
					{
						for(int i=0; i<m_nBtnCount; ++i)
						{
							if( IsInsideRect( nX, nY, m_rcBtnInsertSlot[i] ) )
								SetBtnSlot(i);
						}
					}

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
				if( m_dwStyle & UMBS_BUTTONEX )
				{
					for(int i=0; i<m_nBtnCount; ++i)
					{
						if( IsInsideRect( nX, nY, m_rcBtnSlot[i] ) )
						{
							m_btnSlot[i].InitBtn();
							m_btnOk.SetEnable( FALSE );

							////////////////////////////////////////////////////
							// [070807: Su-won] EVENT_ADULT_OPEN
							// 연금술 이벤트 관련 장비교환창일 때는 필요재료 스트링 출력하지 않음...
							if( m_nCommandCode == MSGCMD_ADULTEVENT_ALCHEMIST)
							{
								this->SetString(3, CTString(" ") );
								this->SetString(4, CTString(" ") );
							}
							// [070807: Su-won] EVENT_ADULT_OPEN
							////////////////////////////////////////////////////
							
							// [070824: Su-won] REFINE_PROTECT
							if(m_nCommandCode == MSGCMD_INVEN_UPGRADE_PROTECT_REQ)
								m_btnOk.SetEnable( TRUE );
						}
					}
				}

				return WMSG_SUCCESS;
			}
		}
		break;
	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				if( m_lbListBox.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : SetBtnSlot()
// Desc :
// ----------------------------------------------------------------------------
void CUIMessageBox::SetBtnSlot(int nBtn)
{
	int nTab = _pUIMgr->GetHoldBtn().GetItemTab();
	int nRow = _pUIMgr->GetHoldBtn().GetItemRow();
	int nCol = _pUIMgr->GetHoldBtn().GetItemCol();
	
	CItems		&TempItem = _pNetwork->MySlotItem[nTab][nRow][nCol];
	CItemData	&rItemData = TempItem.ItemData;

	// 버튼 초기화
	m_btnSlot[nBtn].InitBtn();
	
	// 공통 체크 사항
	if( TempItem.Item_Wearing != -1 )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 2898, "장착 하지 않은 아이템을 올려 주세요" ), SYSMSG_ERROR );		
		return;	
	}
		
	if( m_nCommandCode == MSGLCMD_EVENT_XMAS_2006_CHANGE ) // 2006 X-Mas event [12/12/2006 Theodoric]
	{
		m_nUniIndex = TempItem.Item_UniIndex;

		int nItemType = rItemData.GetType();
		int nItemIndex = rItemData.GetItemIndex();

		if( nItemIndex != 1975 ) // 케이크 1975
		{
			_pUIMgr->GetChatting()->AddSysMessage( _S(3181, "교환 가능한 아이템이 아닙니다." ), SYSMSG_ERROR );
			return;
		}

		if( _pUIMgr->GetMessageBox(MSGLCMD_EVENT_XMAS_2006_COUNT) == NULL)
		{
			CUIMsgBox_Info	MsgBoxInfo;
			CTString strSysMessage;
			strSysMessage.PrintF( _S(150,"몇 개의 %s<를> 옮기시겠습니까?"), rItemData.GetName() );
			MsgBoxInfo.AddString( strSysMessage );
			MsgBoxInfo.SetMsgBoxInfo( _S(127, "교환" ), UMBS_OK|UMBS_INPUTBOX , UI_NONE, MSGLCMD_EVENT_XMAS_2006_COUNT);
			_pUIMgr->CreateMessageBox( MsgBoxInfo );
		}
	}	
	else if(m_nCommandCode == MSGCMD_PET_MOUNT_CANCEL_CARD)
	{	// 펫 훈련 취소 카드의 메시지 박스일 경우
		m_nUniIndex = TempItem.Item_UniIndex;

		int nItemType = rItemData.GetType();

		if( nItemType != m_nItemType || rItemData.GetSubType() != m_nItemSubType )
		{// 현재 악세사리( 펫 아이템 )에 관한 버튼 슬롯만 검사
			if( m_nItemSubType == CItemData::ACCESSORY_PET )	// 펫 아이템
			{
				_pUIMgr->GetChatting()->AddSysMessage( _S( 2445, "펫 아이템만 가능합니다." ), SYSMSG_ERROR );
				return;
			}
		}

		CNetworkLibrary::sPetInfo	TempPet;
		TempPet.lIndex				= _pNetwork->MySlotItem[nTab][nRow][nCol].Item_Plus; // 펫 식별인덱스
		
		std::vector<CNetworkLibrary::sPetInfo>::iterator iter = 
			std::find_if(_pNetwork->m_vectorPetList.begin(), _pNetwork->m_vectorPetList.end(), CNetworkLibrary::FindPet(TempPet) );
		
		if( iter != _pNetwork->m_vectorPetList.end() )
		{
			INDEX iPetType	= -1;
			INDEX iPetAge	= -1;
			
			_pNetwork->CheckPetType( (*iter).sbPetTypeGrade, iPetType, iPetAge );				

			const BOOL bPetRide = PetInfo().IsRide(iPetType, iPetAge);
			
			if( !bPetRide || !_pUIMgr->GetPetInfo()->IsClearSkills( (*iter).lIndex ) )
			{
				_pUIMgr->GetChatting()->AddSysMessage( _S( 2899, "스킬이 초기화 된 탈 것(마운트) 상태의 애완 동물을 올려주세요." ), SYSMSG_ERROR );
				return;
			}
		}
		m_btnOk.SetEnable( TRUE );
	}
	else if(m_nCommandCode == MSGCMD_GAMBLE_READY)
	{
		// 이전 사용된 선택창을 없앤다
		_pUIMgr->CloseMessageBox(MSGCMD_GAMBLE_READYSEL);

		// 캐쉬 문스톤용 아이템이 아니면...
		// 결합 주문서 하드 코딩 ( 플러스 수치가 0이 아니면 감정전 결합 주문서로... )
		int tv_idx = rItemData.GetItemIndex();
		if(tv_idx == 1421 && TempItem.Item_Plus > 0) tv_idx = 1420;

		if(!rItemData.IsFlag(ITEM_FLAG_CASHMOON)
			|| !_pUIMgr->GetGamble()->CheckCashItem(tv_idx))
		{
			_pUIMgr->GetChatting()->AddSysMessage( _S(2907, "이용 가능한 아이템이 아닙니다." ), SYSMSG_ERROR );
			return;
		}
		_pUIMgr->GetGamble()->SetSlotBtn(_pUIMgr->GetHoldBtn());
	}
	else if( m_nCommandCode == MSGCMD_RARE_IDENTIFY)
	{
		m_nUniIndex = TempItem.Item_UniIndex;

		//레어 아이템이 아니거나 감정된 레어 아이템이면...
		if( !(rItemData.GetFlag() & ITEM_FLAG_RARE) ||
			( (rItemData.GetFlag() & ITEM_FLAG_RARE) && TempItem.Item_RareIndex>0 ) )
		{
			_pUIMgr->GetChatting()->AddSysMessage( _S(3162, "옵션 감정을 할 수 없는 아이템입니다." ), SYSMSG_ERROR );		
			return;
		}
		else
			m_btnOk.SetEnable(TRUE);
	}
	else if(m_nCommandCode == MSGCMD_USE_PACKING_PAPER || m_nCommandCode == MSGCMD_USE_SMALL_CORD)
	{
		int nItemType = rItemData.GetType();
		int nItemIndex = rItemData.GetItemIndex();
		
		if ( nItemIndex < 2043 && nItemIndex > 2045 && m_nCommandCode == MSGCMD_USE_PACKING_PAPER ) // 초코렛 아이템 , 복주머니 아이템
		{
			_pUIMgr->GetChatting()->AddSysMessage( _S(3201, "초코렛 아이템을 올려주세요" ), SYSMSG_ERROR );
			return;
		}
		else if (nItemIndex < 2629 && nItemIndex > 2631 && m_nCommandCode == MSGCMD_USE_SMALL_CORD)
		{
			_pUIMgr->GetChatting()->AddSysMessage( _S(3995, "복주머니 아이템을 올려주세요" ), SYSMSG_ERROR );
			return;
		}

		m_nUniIndex = TempItem.Item_UniIndex;
		m_btnOk.SetEnable( TRUE );
	}
	else if(m_nCommandCode == MSGCMD_OK_EXCHANGE_CHOCOBOX || m_nCommandCode == MSGCMD_OK_EXCHANGE_LUCKYBOX)
	{
		int nItemType = rItemData.GetType();
		int nItemIndex = rItemData.GetItemIndex();
		
		if ( nItemIndex != 2047 && m_nCommandCode == MSGCMD_OK_EXCHANGE_CHOCOBOX ) // 초코렛 아이템
		{
			_pUIMgr->GetChatting()->AddSysMessage( _S(3202, "초코수집상자 아이템을 올려주세요" ), SYSMSG_ERROR );
			return;
		}

		if ( nItemIndex != 2633 && m_nCommandCode == MSGCMD_OK_EXCHANGE_LUCKYBOX ) // 새해소망상자 아이템
		{
			_pUIMgr->GetChatting()->AddSysMessage( _S(4009, "새해소망상자 아이템을 올려주세요" ), SYSMSG_ERROR );
			return;
		}

		m_nUniIndex = TempItem.Item_UniIndex;
		m_btnOk.SetEnable( TRUE );
	}
	// [070705: Su-won] EVENT_SUMMER_2007
	else if( m_nCommandCode == MSGCMD_SUMMER2007_PAPERFOLDING
			||m_nCommandCode == MSGCMD_SUMMER2007_PAPERBLENDING )
	{
		int nItemIndex = rItemData.GetItemIndex();

		if( !(nItemIndex >=2367 && nItemIndex<=2370) )
		{
			_pUIMgr->GetChatting()->AddSysMessage( _S(2657, "다른 종류의 아이템입니다." ), SYSMSG_ERROR );
			return;
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// [070807: Su-won] EVENT_ADULT_OPEN
	else if( m_nCommandCode == MSGCMD_ADULTEVENT_ALCHEMIST)
	{
		if( !(rItemData.GetType() == CItemData::ITEM_WEAPON || rItemData.GetType() == CItemData::ITEM_SHIELD) )
		{
			_pUIMgr->GetChatting()->AddSysMessage( _S(2657, "다른 종류의 아이템입니다." ), SYSMSG_ERROR );
			return;
		}

		int iLevel = rItemData.GetLevel();

		if( iLevel<1 ||																		//아이템 레벨이 0일 때,
			rItemData.IsFlag(ITEM_FLAG_CASH) ||												//캐쉬 아이템일 때,
			( 1228 <= rItemData.GetItemIndex() && rItemData.GetItemIndex() <=1278) ||		//일본 의상 아이템일 때는
			( 1978 <= rItemData.GetItemIndex() && rItemData.GetItemIndex() <=2031) )		//조합 시도할 수 없음.
		{
			_pUIMgr->GetChatting()->AddSysMessage( _S(1899, "조합가능한 아이템이 아닙니다." ), SYSMSG_ERROR );
			return;
		}

		// 아이템 레벨이 33이상이면 조합 시도할 수 없음.
		if( iLevel >33)
		{
			_pUIMgr->CloseMessageBox( MSGCMD_NULL );
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo(   _S( 169, "이벤트" ), UMBS_OK, UI_NONE, MSGCMD_NULL);
			MsgBoxInfo.AddString( _S(3645, "조합할 수 없는 아이템입니다. 33레벨 이하의 방어구 및 무기를 올려주세요.") );
			_pUIMgr->CreateMessageBox( MsgBoxInfo );

			m_btnOk.SetEnable( FALSE );
			this->SetString(3, CTString(" ") );
			this->SetString(4, CTString(" ") );

			return;
		}

		INDEX iReqMaterial[2] ={0,0};		// 필요 재료(index)
		INDEX iReqCount[2] ={0,0};			// 필요 개수

		if( iLevel>=1 && iLevel<=5 )
		{
			iReqMaterial[0] =113;	iReqCount[0] =2;	//여우가죽
			iReqMaterial[1] =114;	iReqCount[1] =2;	//코드이빨
		}
		else if( iLevel>=6 && iLevel<=10 )
		{
			iReqMaterial[0] =119;	iReqCount[0] =3;	//재규어 이빨
			iReqMaterial[1] =110;	iReqCount[1] =3;	//사슴뿔
		}
		else if( iLevel>=11 && iLevel<=15 )
		{
			iReqMaterial[0] =127;	iReqCount[0] =5;	//고디스의 털
			iReqMaterial[1] =123;	iReqCount[1] =3;	//버려진 주문서

		}
		else if( iLevel>=16 && iLevel<=20 )
		{
			iReqMaterial[0] =116;	iReqCount[0] =5;	//부러진 나뭇가지
			iReqMaterial[1] =133;	iReqCount[1] =5;	//좀비의 껍질
		}
		else if( iLevel>=21 && iLevel<=25 )
		{
			iReqMaterial[0] =124;	iReqCount[0] =7;	//무딘칼날
			iReqMaterial[1] =140;	iReqCount[1] =5;	//썩은 붕대조각
		}
		else if( iLevel>=26 && iLevel<=30 )
		{
			iReqMaterial[0] =129;	iReqCount[0] =7;	//부러진 칼날
			iReqMaterial[1] =134;	iReqCount[1] =7;	//녹슨 방패조각
		}
		else if( iLevel>=31 && iLevel<=35 )
		{
			iReqMaterial[0] =118;	iReqCount[0] =9;	//원목
			iReqMaterial[1] =128;	iReqCount[1] =9;	//강화비늘
		}

		// 가지고 있는 재료 개수
		INDEX iHaveCount[2];	
		iHaveCount[0] =_pUIMgr->GetInventory()->GetItemCount(iReqMaterial[0]);
		iHaveCount[1] =_pUIMgr->GetInventory()->GetItemCount(iReqMaterial[1]);

		CTString strTemp;
		strTemp.PrintF( _S(3646, "%s (%d/%d)개"), _pNetwork->GetItemName(iReqMaterial[0]), iHaveCount[0], iReqCount[0]);
		this->SetString(3, strTemp, iHaveCount[0] >=iReqCount[0] ? 0xFFFFFFFF : 0xB3B3B3FF);
		strTemp.PrintF( _S(3646, "%s (%d/%d)개"), _pNetwork->GetItemName(iReqMaterial[1]), iHaveCount[1], iReqCount[1]);
		this->SetString(4, strTemp, iHaveCount[1] >=iReqCount[1] ? 0xFFFFFFFF : 0xB3B3B3FF);

		//재료가 부족하면 슬롯에 아이템을 올려놓고 확인 버튼 비활성화 상태에서 리턴
		if( iHaveCount[0]<iReqCount[0] || iHaveCount[1]<iReqCount[1] )
		{
			m_btnSlot[nBtn].Copy( _pUIMgr->GetHoldBtn() );
			return;
		}
	}
	// [070807: Su-won] EVENT_ADULT_OPEN
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	else if (m_nCommandCode == MSGCMD_TG2007_1000DAYS_CAP_UPGRADE) // 1000일 기념모자 업그레이드
	{
		int nItemIndex = rItemData.GetItemIndex();

		if (nItemIndex < 2423 && nItemIndex > 2434) // 기념모자 아이템이 아니라면
		{
			_pUIMgr->GetChatting()->AddSysMessage( _S(3831, "라스트카오스 1000일 기념모자만 해당됩니다 확인하고 다시 시도하여 주십시오"), SYSMSG_ERROR );
			return;
		}
	}
	
	// [070820: Su-won] REFINE_PROTECT
	else if( m_nCommandCode == MSGCMD_INVEN_UPGRADE_PROTECT_REQ)
	{
		if( rItemData.GetItemIndex() != 2394 && rItemData.GetItemIndex() != 2395)
		{
			_pUIMgr->GetChatting()->AddSysMessage( _S(2657, "다른 종류의 아이템입니다." ), SYSMSG_ERROR );
			return;
		}

		SLONG slItemPlus =_pUIMgr->GetInventory()->GetUpgradeItemPlus();

		//하급제련보호석
		if( rItemData.GetItemIndex() == 2394 )
		{
			if( !(3<=slItemPlus && slItemPlus<=9) )
			{
				_pUIMgr->GetChatting()->AddSysMessage( _S(3663, "+3부터 +9까지의 장비에만 사용할 수 있습니다." ), SYSMSG_ERROR );
				return;
			}
		}

		//상급제련보호석
		if( rItemData.GetItemIndex() == 2395 )
		{
			if( !(10<=slItemPlus && slItemPlus<=14) )
			{
				_pUIMgr->GetChatting()->AddSysMessage( _S(3664, "+10부터 +14까지의 장비에만 사용할 수 있습니다." ), SYSMSG_ERROR );
				return;
			}
		}

		m_btnSlot[nBtn].Copy( _pUIMgr->GetHoldBtn() );
	}
	// [071122: Su-won] NEW_MOONSTONE
	else if( m_nCommandCode == MSGCMD_MOONSTONE_MIX)
	{
		if( !((2545<=rItemData.GetItemIndex() && rItemData.GetItemIndex() <=2548) || rItemData.GetItemIndex()==723) )
		{
			_pUIMgr->GetChatting()->AddSysMessage( _S(2657, "다른 종류의 아이템입니다." ), SYSMSG_ERROR );
			return;
		}
	}
	else if( m_nCommandCode == MSGCMD_EVENT_CHAOSBALL)
	{
		if( rItemData.GetItemIndex() != 2843 )
		{
			_pUIMgr->GetChatting()->AddSysMessage( _S(2657, "다른 종류의 아이템입니다." ), SYSMSG_ERROR );
			return;
		}
	}

	// 그외의 경우 일반적으로 해당 파일에서 자체 처리 

	// 예외 사항이 아니라면...
	m_btnSlot[nBtn].Copy( _pUIMgr->GetHoldBtn() );

	// 빈 버튼이 없으면 OK버튼 활성화
	m_btnOk.SetEnable( TRUE );
	for(int i=0; i<m_nBtnCount; ++i)
	{
		if( m_btnSlot[i].IsEmpty() )
		{
			m_btnOk.SetEnable( FALSE );
			break;
		}
	}
}

// ----------------------------------------------------------------------------
// Name : GetBtnUseItemInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIMessageBox::GetBtnUseItemInfo( int &nTab, int &nRow, int &nCol, int &nUniIndex )
{
	nTab = m_nBtnTab;
	nRow = m_nBtnRow;
	nCol = m_nBtnCol;
	nUniIndex = m_nUniIndex;
}

//============================================================================================================
// Name : SetInvenUseItemInfo()
// Desc :
//============================================================================================================
void CUIMessageBox::SetInvenUseItemInfo( int nTab, int nRow, int nCol )
{
	m_nBtnTab = nTab;
	m_nBtnRow = nRow;
	m_nBtnCol = nCol;
}

void CUIMessageBox::SetColorBox(INDEX i, int nType, WRect rcDrawBox) 
{ 
	if( i >= m_cboxColorBox.m_nColorBoxCount )
		return;

	m_cboxColorBox.m_ColorBoxInfo[i].m_nColorBoxType =nType;
	m_cboxColorBox.m_ColorBoxInfo[i].m_rcColorBox =rcDrawBox; 		
}

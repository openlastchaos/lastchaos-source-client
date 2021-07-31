#include "stdh.h"
#include <string>
#include <vector>
#include <algorithm>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/InternalClasses.h>

CUIFiltering _UIFiltering;
CUIFiltering _UIFilteringCharacter;
CUIFiltering _UICharacterChatFilter;

extern INDEX g_iCountry;

#define is_space(c)		(c == 0x20 || (c >= 0x09 && c <= 0x0D))

// comments : key change string to hexa code.
#pragma warning( disable: 4305)		// disable truncation warning.
#pragma warning( disable: 4309)		// disable truncation warning.
const char	szKey_brz[32] = {0x8D,0x8F,0x81,0x90,0x9D,0x8D,0x90,0x9D,0x81,0x81,0x9D,0x8F,0x8D,0x8D,0x81,0x90,0x81,0x8D,0x90,0x8F,
	0x8D,0x8F,0x9D,0x8F,0x9D,0x8D,0x90,0x9D,0x81,0x90,0x81,0x9D};

// Explain:  비교시 소문자로 변환 
void makeLowCase(char* tmpChar)
{
	if( *tmpChar >= 65 && *tmpChar <= 90)
		*tmpChar += 32;
}

bool lessLength(const FILTER& f1, const FILTER& f2)
{
	return f1.strString < f2.strString;
}

void CUIFiltering::Clear()
{
	m_vList.clear();

	memset ( m_strFileName, 0, sizeof ( m_strFileName ) );

	memset ( m_strRefineWord, 0, sizeof ( m_strRefineWord ) );

	strcpy ( m_strRefineWord, "!@d#$w35%^vb&s*(!@d#$w35%^vb&s*(" );

	m_nListCount = 0;
}

BOOL CUIFiltering::Read( char* file_name , bool charfilter /*= false*/ )
{
	FILE* fp;	

	char szReadBuffer[MAX_STR_LENGTH];
	char szEndCodingBuffer[MAX_STR_LENGTH];

	fp = fopen(file_name, "rb");

	if ( fp == NULL )
	{
		return FALSE;
	}

	// 파일 버젼 체크
	fread ( szReadBuffer, MAX_STR_LENGTH, 1, fp );
	EnDecoding ( szEndCodingBuffer, szReadBuffer );

	if ( strcmp ( szEndCodingBuffer, m_strRefineWord ) != 0 )
	{
		fclose(fp);
		return FALSE;	
	}

	// 파일 개수 체크 
	fread ( szReadBuffer, MAX_STR_LENGTH, 1, fp );
	m_nListCount = atoi ( szReadBuffer );

	int nRefindWordLen = strlen ( m_strRefineWord ) / 2;

	char szString[MAX_STR_LENGTH];
	char szRep[MAX_STR_LENGTH];

	for( int i = 0; i < m_nListCount; i++ ) 
	{	
		fread( szReadBuffer, MAX_STR_LENGTH, 1, fp );
		EnDecoding ( szString, szReadBuffer );

		memcpy( szRep, m_strRefineWord+(rand()%nRefindWordLen), strlen( szString ) );		

		szRep[strlen( szString )] = NULL;
		FILTER	tempFilter( szString, szRep );
		m_vList.push_back ( tempFilter );
	}

	fclose(fp);			

	std::stable_sort ( m_vList.begin(), m_vList.end(), lessLength );

	m_bCharacterfilter = charfilter;
	return TRUE;
}

BOOL CUIFiltering::Filtering( char* syntax )
{
	int space_array[256];	// 띄어쓰기가 있는 위
	int space_cnt = 0;		// 띄어쓰기가 
	int i, pos;
	
	int len = strlen ( syntax );
	if ( len <= 0 )
	{
		return FALSE;		
	}

	// add new temp val 060419--------------------<<
	//char *tv_syntax = new(char[len+1]);
	//strcpy(tv_syntax,syntax);
	char chComp[256];
	memcpy(chComp, syntax, len);
	chComp[len] = '\0';
	// ------------------------------------------->>

	// space 를 모두 없애고, space 가 있던 
	for ( i = 0; i < len; i++ ) 
	{
		if ( is_space ( chComp[i] ) ) 
		{
			for ( pos = i; pos < len; pos++ ) 
			{
				chComp[pos] = chComp[pos+1];
			}

			space_array[space_cnt++] = i;
			len--;
			i--;
		}
		else {
			//[ttos_2009_8_6]: 채팅무시는 캐릭터이름으로 비교하므로 스트링 변환을 시키지 않는다
			// 영문자 소문자로 비교 060403
			if(!m_bCharacterfilter) 
				makeLowCase( &chComp[i] );
		}
	}

	// 필터링
	i = 0;
	BOOL bFount;
	int mid, left, right;
	BOOL bAllFount = FALSE;

	while ( i < len ) 
	{
		bFount = FALSE;
		left = 0;
		right =  m_nListCount - 1;	// Date : 2005-01-17,   By Lee Ki-hwan : 버그 수정

		while ( right >= left ) 
		{
			mid = ( right + left ) / 2;
			
			int Res;
			if (!m_bCharacterfilter)
			{
				Res = m_vList[mid].strString.compare ( 0, m_vList[mid].nString, chComp+i, 0, m_vList[mid].nString );
			}else
			{
				Res = m_vList[mid].strString.compare ( 0, m_vList[mid].nString, chComp+i, 0, len);
			}

			
			//strncmp ( m_vList[mid].strString, &syntax[i], m_vList[mid].nString );
					
			if ( Res < 0 ) 
			{
				left = mid + 1;	  // check_buffer이 더 크면 
			} 
			else if ( Res > 0 ) 
			{
				right = mid - 1; //  check_buffer이 더 작으면
			} 
			else // 찾았다.
			{
				if (!m_bCharacterfilter || (m_bCharacterfilter && m_vList[mid].nString == len))
				{
					memcpy ( &chComp[i], m_vList[mid].strReword.c_str(), m_vList[mid].nReword );
					bFount = TRUE;
				}
				break;
			}
		}


		if ( bFount ) 
		{
			i += m_vList[mid].nString;
			bAllFount = TRUE;
			break;
		} 
		else 
		{
			if ( chComp[i] < 0 || chComp[i] > 127 )  //2바이트
			{
				i += 2;
			} 
			else 
			{
				i++;
			}
		}

	} // while ( i < len ) 
/*
	// 띄어 쓰기 복구 
	for ( i = space_cnt - 1; i >= 0; i-- ) 
	{
		for ( pos = len; pos >= space_array[i]; pos-- ) 
		{
			syntax[pos+1] = syntax[pos];
		}

		syntax[space_array[i]] = ' ';
		len++;
	}
*/
	return bAllFount;
}

BOOL CUIFiltering::Filtering( char* syntax, int* szReturn )
{
	int i, pos;

	int len = strlen ( syntax );
	if ( len <= 0 )
	{
		return FALSE;		
	}

	const int max_filter = 256;

	if (len > max_filter)
		len = max_filter - 1;

	// add new temp val 060419--------------------<<
//	char *tv_syntax = new(char[len+1]);
//	strcpy(tv_syntax,syntax);
	char chComp[max_filter];
	memcpy(chComp, syntax, len);
	chComp[len] = '\0';
	// ------------------------------------------->>

	// space 를 모두 없애고, space 가 있던 
	for ( i = 0; i < len; i++ ) 
	{
		if ( is_space ( chComp[i] ) ) 
		{
			if (g_iCountry != USA)
			{
				for ( pos = i; pos < len; pos++ ) 
				{
					chComp[pos] = chComp[pos+1];
				}

				len--;
				i--;
			}
		}
		else {
			//[ttos_2009_8_6]: 채팅무시는 캐릭터이름으로 비교하므로 스트링 변환을 시키지 않는다
			// 영문자 소문자로 비교 060403 
			if(!m_bCharacterfilter) 
				makeLowCase( &chComp[i] );
		}
	}

	// 필터링
	i = 0;
	BOOL bFount;
	int mid, left, right;
	BOOL bAllFount = FALSE;
	int nCountIndex = 1;

	while ( i < len ) 
	{
		bFount = FALSE;
		left = 0;
		right =  m_nListCount-1;
	

		while ( right >= left ) 
		{
			mid = ( right + left ) / 2;

			//int Res = strncmp ( m_vList[mid].strString.c_str(), &syntax[i], m_vList[mid].nString );
			int Res = m_vList[mid].strString.compare ( 0, m_vList[mid].nString, chComp+i, 0, m_vList[mid].nString );
				
			if ( Res < 0 ) 
			{
				left = mid + 1;	  // check_buffer이 더 크면 
			} 
			else if ( Res > 0 ) 
			{
				right = mid - 1; //  check_buffer이 더 작으면
			} 
			else // 찾았다.
			{ 
				szReturn[nCountIndex++] = mid;
				
				bFount = TRUE;
				break;
			}
		}


		if ( bFount ) 
		{
			i += m_vList[mid].nString;
			bAllFount = TRUE;
		} 
		else 
		{
			if ( chComp[i] < 0 || chComp[i] > 127 )  //2바이트
			{
				i += 2;
			} 
			else 
			{
				i++;
			}
		}

	} // while ( i < len ) 
	szReturn[0] = nCountIndex-1;

	return bAllFount;
}


//------------------------------------------------------------------------------
// CUIFiltering::GetString 
// Explain:  
// Date : 2005-01-13,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
char* CUIFiltering::GetString ( int nIndex ) const 
{
	return (char*)m_vList[nIndex].strString.c_str();
}

char* CUIFiltering::GetKey() const
{
	return (char*)szKey_brz;
}

void CUIFiltering::EnDecoding( char* szResult, const char* szData )
{
	char* TempKey = GetKey(); 
	int nKeyLeng = strlen ( TempKey);
	int nDataLen = strlen ( szData );
	int nKeyIndex = 0;

	int i;
	for ( i = 0; i < nDataLen; i++ )
	{
		szResult[i] = szData[i] ^ TempKey[nKeyIndex];
		if( ++nKeyIndex > nKeyLeng ) nKeyIndex = 0;
	}

	szResult[i] = NULL;
}

char* CUIFiltering::GetRefineWord() const
{
	return (char*) (m_strRefineWord);
}

void CUIFiltering::SetFileName( char* szData )
{
	strcpy ( m_strFileName, szData );
}

char* CUIFiltering::GetFileName() const
{
	return (char*) m_strFileName;
}

CChatBlockListUI::CChatBlockListUI()
	: m_pListBlock(NULL)
	, m_nOldSelect(-1)
{
	setInherit(false);
	m_strSelName.Clear();
}

CChatBlockListUI::~CChatBlockListUI()
{
	
}

void CChatBlockListUI::initialize()
{
#ifndef		WORLD_EDITOR

	// 드래그 영역 설정.
	if (CUIBase* pDrag = findUI("base_drag"))
	{
		int l = pDrag->GetPosX();
		int t = pDrag->GetPosY();
		int r = l + pDrag->GetWidth();
		int b = t + pDrag->GetHeight();

		setTitleRect(l, t, r, b);
	}

	m_pListBlock = (CUIList*)findUI("list_block");

	if (CUIBase* pClose = findUI("btn_close"))
		pClose->SetCommandFUp(boost::bind(&CChatBlockListUI::CloseChatFilter, this));

	if (CUIBase* pAdd = findUI("btn_add"))
		pAdd->SetCommandFUp(boost::bind(&CChatBlockListUI::OpenAddPopup, this));

	if (CUIBase* pDel = findUI("btn_del"))
		pDel->SetCommandFUp(boost::bind(&CChatBlockListUI::DelCharName, this));

#endif		// WORLD_EDITOR
}

void CChatBlockListUI::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

void CChatBlockListUI::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

void CChatBlockListUI::OpenChatFilter()
{
	if( IsVisible() || GetHide() == FALSE)
		return;

	Hide(FALSE);

	ListUpdate();
	CUIManager::getSingleton()->RearrangeOrder( UI_CHAT_FILTER, TRUE );
}

void CChatBlockListUI::CloseChatFilter()
{
	m_pListBlock->DeleteAllListItem();
	m_strSelName.Clear();
	
	Hide(TRUE);
	CUIManager::getSingleton()->RearrangeOrder( UI_CHAT_FILTER, FALSE );
}

void CChatBlockListUI::ListUpdate()
{
#ifndef		WORLD_EDITOR

	_UICharacterChatFilter.Clear();
	CTString	strFullPath = _fnmApplicationPath.FileDir();
	strFullPath += "Data\\CharacterChatFilter.dat"; 
	_UICharacterChatFilter.Read(strFullPath.str_String, true);
	//m_lbCharName.ResetAllStrings();
	m_pListBlock->DeleteAllListItem();

	int count = _UICharacterChatFilter.GetListCount();
	CTString strTem;

	CUIBase* pItemTmp = m_pListBlock->GetListItemTemplate();
	CUIBase* pItem = NULL;
	CUIText* pText = NULL;

	if (pItemTmp == NULL)
		return;

	for (int i = 0; i < count; i++)
	{
		strTem.PrintF("%s",_UICharacterChatFilter.GetString(i)); 
		m_pListBlock->AddListItem(pItemTmp->Clone());

		pItem = m_pListBlock->GetListItem(i);

		if (pItem == NULL)
			continue;

		pText = (CUIText*)pItem->findUI("text_name");

		pItem->SetCommandFUp(boost::bind(&CChatBlockListUI::ChangeColor, this));

		if (pText == NULL)
			continue;

		pText->SetText(strTem);
	}

	m_pListBlock->UpdateScroll(m_pListBlock->getListItemCount());
	m_pListBlock->UpdateList();

	m_pListBlock->SetCommandFUp(boost::bind(&CChatBlockListUI::ChangeColor, this));
	m_nOldSelect = -1;

#endif		// WORLD_EDITOR
}

void CChatBlockListUI::SaveFile()
{
	FILE* fp;
	CTString	strFullPath = _fnmApplicationPath.FileDir();
	strFullPath += "Data\\CharacterChatFilter.dat";

	fp = fopen ( strFullPath, "wb" );

	if ( fp == NULL )
	{
		MessageBox(NULL, "File is not Exist.", "error!", MB_OK);
		return;
	}
	//	_UICharacterChatFilter.Clear();
	int nCount = m_pListBlock->getListItemCount();

	char szBuffer[MAX_STR_LENGTH]; 
	memset ( szBuffer, 0, sizeof ( szBuffer ) );

	_UICharacterChatFilter.EnDecoding ( szBuffer, _UICharacterChatFilter.GetRefineWord() );
	fwrite ( szBuffer, MAX_STR_LENGTH, 1, fp );


	wsprintf ( szBuffer, "%d", nCount );
	fwrite ( szBuffer, sizeof ( szBuffer ), 1, fp );	

	CUIText* pText = NULL;

	for ( int nIndex = 0; nIndex < nCount; nIndex++ )
	{
		// save data...
		pText = (CUIText*)m_pListBlock->GetListItem(nIndex)->findUI("text_name");

		if (pText == NULL)
			continue;

		_UICharacterChatFilter.EnDecoding ( szBuffer, pText->getText());//m_lbCharName.GetString(0, nIndex) );
		fwrite ( szBuffer, sizeof ( szBuffer ), 1, fp );	
	}

	fclose ( fp );
}

void CChatBlockListUI::AddCharName(char* strCharName)
{
	int len = strlen ( strCharName );
	if ( len <= 0 )
		return;		

	CTString strTemp;

	strTemp.PrintF("%s",strCharName);

	CUIBase* pItemTmp = m_pListBlock->GetListItemTemplate();
	m_pListBlock->AddListItem(pItemTmp->Clone());

	CUIBase* pItem = m_pListBlock->GetListItem(m_pListBlock->getListItemCount() - 1);

	if (pItem == NULL)
		return;

	CUIText* pText = (CUIText*)pItem->findUI("text_name");

	if (pText == NULL)
		return;

	pText->SetText(strTemp);
	//m_lbCharName.AddString(0, strTemp);

	SaveFile();
	ListUpdate();
}

void CChatBlockListUI::DelCharName()
{
	if (m_nOldSelect < 0)
		return;

	CUIText* pText = (CUIText*)m_pListBlock->GetListItem(m_nOldSelect)->findUI("text_name");

	if (pText == NULL)
		return;

	CTString strName = pText->getText();

	if (strName.IsEmpty() == TRUE)
		return;

	// 필터리스트에서 해제 되면 스패머 맵에서도 해제 added by sam 11/03/02
	CUIManager::getSingleton()->GetChattingUI()->SpamerLift( strName );

	//m_lbCharName.RemoveString(m_nSelIndex,0);
	m_pListBlock->deleteListItem(m_nOldSelect);

	m_strSelName.Clear();
	SaveFile();
	ListUpdate();
}

void CChatBlockListUI::Message( int nCommandCode, CTString strTitle, CTString strMessage, DWORD dwStyle )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist( nCommandCode ) )
		return;

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( strTitle, dwStyle, UI_CHAT_FILTER, nCommandCode ); 

	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

void CChatBlockListUI::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch ( nCommandCode )
	{
	case MSGCMD_CHAT_BLOCK_REQ:
		{
			if( bOK )
			{
				CTString strMessage;
				strMessage.PrintF(_S(3005, "%s님을 차단하시겠습니까?"), strInput);
				m_strSelName = strInput;
				Message( MSGCMD_CHAT_BLOCK, _S(3004, "캐릭터 차단"), strMessage, UMBS_YESNO );		
			}
		}
		break;
	case MSGCMD_CHAT_BLOCK:
		{
			if( bOK)
			{
				if( m_strSelName.Length() < 2 || m_strSelName.Length() > 16 )
				{
					Message( MSGCMD_CHAT_ERROR, _S(3008, "차단 에러"), _S(3009, "차단하고자 하는 이름이 올바르지 않습니다"), UMBS_OK);		
					return;
				}
				if( _UICharacterChatFilter.Filtering((char *)((const char *)m_strSelName)))
				{
					Message( MSGCMD_CHAT_ERROR, _S(3008, "차단 에러"), _S(3011, "이미 차단된 캐릭입니다."), UMBS_OK);
					return;
				}
				AddCharName(m_strSelName.str_String);	
			}else
			{
				m_strSelName.Clear();
			}
		}
		break;
	case MSGCMD_CHAT_ERROR:
		break;
	}
}

void	CChatBlockListUI::CharacterFilterInSimplePopup( CTString strCharName )
{
	CTString strMessage;
	strMessage.PrintF(_S(3005, "%s님을 차단하시겠습니까?"), strCharName);
	m_strSelName = strCharName;
	Message( MSGCMD_CHAT_BLOCK, _S(3004, "캐릭터 차단"), strMessage, UMBS_YESNO );	
}

void CChatBlockListUI::OpenAddPopup()
{
	Message( MSGCMD_CHAT_BLOCK_REQ, _S(3004, "캐릭터 차단"), _S(3007, "차단할 캐릭명을 입력하여 주십시오." ), UMBS_OKCANCEL | UMBS_INPUTBOX );
}

void CChatBlockListUI::ChangeColor()
{
	if (m_pListBlock == NULL)
		return;

	if (m_nOldSelect >= 0)
	{
		CUIText* pOld = (CUIText*)m_pListBlock->GetListItem(m_nOldSelect)->findUI("text_name");

		if (pOld != NULL)
		{
			pOld->setFontColor(DEF_UI_COLOR_WHITE);
			m_nOldSelect = -1;
		}
	}

	int nCurSel = m_pListBlock->getCurSel();

	if (nCurSel < 0)
		return;

	CUIText* pText = (CUIText*)m_pListBlock->GetListItem(nCurSel)->findUI("text_name");

	if (pText == NULL)
		return;

	pText->setFontColor(0xF0A769FF);
	m_nOldSelect = nCurSel;
}

#include "stdh.h"
#include <Engine/Interface/UIIME.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIMessageBox.h>

HKL			_hKL		= NULL;
BOOL		_bIMEProc	= false;

// 국가 설정 ( 하드 코딩 )
DWORD _dwLocalLanguage	 =   LANG_KOREAN;		// 한국 
							// LANG_CHT;		// 대만 
							// LANG_CHS;		// 중국 (PRC)
							// LANG_JAPANESE;	// 일본

// ----------------------------------------------------------------------------
// Name : GetNationName()
// Desc :
// ----------------------------------------------------------------------------
char* GetNationName ( DWORD dwNations )
{
	switch ( PRIMARYLANGID ( dwNations ) )
	{
	case LANG_CHINESE :
		switch ( SUBLANGID ( dwNations ) )
		{
		case SUBLANG_CHINESE_SIMPLIFIED :
			return "중국(PRC)";
							
		case SUBLANG_CHINESE_TRADITIONAL :
			return "대만";
		}
		return "중국";

	case LANG_KOREAN :
		return "한국";

	case LANG_JAPANESE :
		return "일본";
	}
	return "Unknow";
}
	
// ----------------------------------------------------------------------------
// Name : CheckInputLocal()
// Desc :
// ----------------------------------------------------------------------------
bool CheckInputLocal ()
{
	_hKL = GetKeyboardLayout ( 0 );

	if ( _dwLocalLanguage != GetPrimaryLang ()  )
	{
		_pUIMgr->CloseMessageBox( MSGCMD_LOGIN_ERROR );

		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( CTString( "Language setting" ), UMBS_OK, UI_LOGIN, MSGCMD_LOGIN_ERROR );

		CTString strMessage = CTString( "Setting country is wrong.\n\n" );
				
		strMessage += "Current version : ";
		strMessage += GetNationName ( _dwLocalLanguage );
		strMessage += "\nCurrent input local : ";
		strMessage += GetNationName ( GetLanguage () );

		MsgBoxInfo.AddString( strMessage );
		_pUIMgr->CreateMessageBox( MsgBoxInfo, 350, 300 ); // 임의 설정 

	}

	// 나라별 Cand 출력 형태 지정 ( 가로, 세로 )
	bool bVerticalCand = false;
/*
	switch ( _dwLocalLanguage )
	{
	case LANG_CHINESE :
		switch ( GetSubLang () )
		{
		case SUBLANG_CHINESE_SIMPLIFIED :
			bVerticalCand = true;
		
		case SUBLANG_CHINESE_TRADITIONAL :
			bVerticalCand = true;
		}
		break;	

	case LANG_KOREAN :
		bVerticalCand = false;
		break;
		
	case LANG_JAPANESE :
		bVerticalCand = true;
		break;
	}
*/

	return bVerticalCand;
}



//------------------------------------------------------------------------------
// SetEngMode
// Explain: 키보드 입력을 영어로 전환 하는 함수
// Date : 2005-01-17,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void SetEngMode ( HWND hWnd )
{
	HIMC hImc = ImmGetContext ( hWnd );
	DWORD dwConv, dwSent;
	DWORD dwTemp;

	ImmGetConversionStatus ( hImc, &dwConv, &dwSent );

	dwTemp = dwConv & ~IME_CMODE_LANGUAGE;
	dwTemp |= IME_CMODE_ALPHANUMERIC;
	
	dwConv = dwTemp;

	ImmSetConversionStatus ( hImc, dwConv, dwSent );
	ImmReleaseContext ( hWnd, hImc );
}
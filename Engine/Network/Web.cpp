//안태훈 수정 시작	//(5th Closed beta)(0.2)
#include "StdH.h"

#include "Web.h"
#include <WinInet.h>
#include <assert.h>

#include "ThreadWrapper.h"
#include <Engine/Interface/UIManager.h>

void WebAddressInit()
{
	// Date : 2005-01-19,   By Lee Ki-hwan : string 으로 변경
	WebAddressRegister("help_list",		_S( 117, "http://222.122.55.241/game/help/game_help_list.asp" ) );
	WebAddressRegister("help_view",		_S( 118, "http://222.122.55.241/game/help/game_help_view.asp" ) );

	WebAddressRegister("notice_list",	_S( 119, "http://222.122.55.241/game/notice/game_notice_list.asp" ) );
	WebAddressRegister("notice_view",	_S( 120, "http://222.122.55.241/game/notice/game_notice_view.asp" ) );

	WebAddressRegister("freebbs_list",	_S( 121, "http://222.122.55.241/game/bbs/game_bbs_list.asp" ) );
	WebAddressRegister("freebbs_view",	_S( 122, "http://222.122.55.241/game/bbs/game_bbs_view.asp" ) );
	WebAddressRegister("freebbs_modify",_S( 123, "http://222.122.55.241/game/bbs/game_bbs_modify.asp" ) );

	WebAddressRegister("guildbbs_list",	_S( 1008, "http://lastchaos.2pan4pan.com/game/guildbbs/game_bbs_list.asp" ) ); // wooss 웹주소 수정 
	WebAddressRegister("guildbbs_view",	_S( 1009, "http://lastchaos.2pan4pan.com/game/guildbbs/game_bbs_view.asp" ) );
	WebAddressRegister("guildbbs_modify", _S( 1010, "http://lastchaos.2pan4pan.com/game/guildbbs/game_bbs_modify.asp" ) );
}

/**
리턴값
0 : 이상 없음
1 : 자체 복구가 가능한 수준의 오류
2 : 유저에게 알려야할 오류
3 : 개발자 수준의 오류
**/
//DWORD WINAPI WebThreadFunction(void *parameter)
UINT WINAPI WebThreadFunction(void *parameter)
{
	ASSERT(parameter != NULL);
	if(parameter == NULL) return 3;
	cSharedWebData &shared = *(cSharedWebData*)parameter;

	///internet 연결을 초기화한다.
	HINTERNET hInternet = InternetOpen( "Web Board Browser", 
										INTERNET_OPEN_TYPE_PRECONFIG, // Use registry settings. 
										NULL, // Proxy name. NULL indicates use default.
										NULL, // List of local servers. NULL indicates default. 
										0 );
	if(hInternet == NULL)	//internet 연결을 여는데 실패했다. IE가 없나?
	{
		return 3;
	}
	DWORD exitCode = 0;

	for(;;)
	{
		///입력이 들어오기를 무한정 기다림.
		if(WAIT_OBJECT_0 != WaitForSingleObject(shared.GetSendReadEventHandle(), INFINITE)) {exitCode = 3; break;}
		if(shared.GetExit()) break;

		///URL을 저장하고 Send Buffer를 지우고 Send Write Event를 Set한다.
		if(shared.GetSendMsgBuffer() == NULL) continue;
		std::string strURL = shared.GetSendMsgBuffer();
		shared.FreeSendMsgBuffer();
		SetEvent(shared.GetSendWriteEventHandle());

		///http연결을 만든다.
		HINTERNET hHttpFile = InternetOpenUrl(	hInternet
												, strURL.c_str()
												, NULL
												, 0
												, 0
												, 0 );
		if(hHttpFile == NULL) //url을 찾을 수 없습니다.
		{
			DWORD dwError = GetLastError();
			//exitCode = 2;
			//break;
			continue;
		}


		///http file의 사이즈를 얻어온다.
		char szSizeBuffer[128] = {0};
		DWORD dwLengthSizeBuffer = sizeof(szSizeBuffer);
		BOOL bQuery = HttpQueryInfo( hHttpFile
									, HTTP_QUERY_CONTENT_LENGTH
									, szSizeBuffer
									, &dwLengthSizeBuffer
									, NULL );
		DWORD dwFileSize = 0;
		if(bQuery)
		{
			//http file을 저장할 적당한 크기의 메모리를 할당한다.
			dwFileSize = atol(szSizeBuffer);
		}
		else
		{
			//적당한 크기를 잡는다.
			dwFileSize = 1024*1024; //1 mega bytes
		}
		ResetEvent(shared.GetReceiveWriteEventHandle());
		ResetEvent(shared.GetReceiveReadEventHandle());
		shared.AllocReceiveMsgBuffer(dwFileSize+1);
		char *szReceiveBuffer = shared.GetReceiveMsgBuffer();

		///http file을 web에서 읽어온다.
		DWORD dwBytesRead = 0;	//읽힌 파일의 크기
		BOOL bRead = InternetReadFile( hHttpFile
									, szReceiveBuffer
									, dwFileSize
									, &dwBytesRead );
    
		if(bRead)
		{
			szReceiveBuffer[dwBytesRead] = '\0';
			SetEvent(shared.GetReceiveReadEventHandle());
		}
		else	//web page를 읽는데 실패했다.
		{
			shared.FreeReceiveMsgBuffer();
			//exitCode = 3;
			//break;
			InternetCloseHandle(hHttpFile);
			continue;
		}

		///http 연결을 닫는다.
		InternetCloseHandle(hHttpFile);
	}

	///internet 연결을 종료한다.
	InternetCloseHandle(hInternet);

	return exitCode;
}

std::string ConvertStringToWebParameter(const char *szParam)
{
	if(szParam == NULL) return "";
	int strLen = strlen(szParam);
	if(strLen == 0) return "";

	std::string strRet;
	for(int i=0; i<strLen; ++i)
	{
		if(szParam[i] == '\n') strRet += "%0a";
		else if(szParam[i] == '\r') strRet += "%0d";
		else if(szParam[i] == '+') strRet += "%2b";
		else if(szParam[i] == '&') strRet += "error_andchar";
		else if(szParam[i] == ' ') strRet += "%20";
		else strRet += szParam[i];
	}
	return strRet;
}

//-------------- cWebAddress --------------//
cWebAddress cWebAddress::m_instance;

cWebAddress::cWebAddress()		{}
cWebAddress::~cWebAddress()		{}

BOOL cWebAddress::RegisterAddress(const char *alias, const char *address)
{
	web_map::value_type temp(alias, address);
	return m_mapAddressAlias.insert(temp).second;
}

const char *cWebAddress::Address(const char *alias)
{
	return m_mapAddressAlias[std::string(alias)].c_str();
}

//-------------- cWeb --------------//
cWeb::cWeb()
: m_eStatus( WS_PREBEGIN )
{
	m_sharedData.ResetAll();
	m_pThread = new cThreadWrapper(WebThreadFunction);
}

cWeb::~cWeb()
{
	if(m_pThread) delete m_pThread;
}

BOOL cWeb::Begin()
{
	if(!IsPreBegin()) return FALSE;
	if(m_pThread->Start(&m_sharedData))
	{
		m_eStatus = WS_READYREQUEST;
		return TRUE;
	}
	return FALSE;
}

BOOL cWeb::End()
{
	if(!IsBegin()) return TRUE;
	m_sharedData.SetExit(TRUE);
	SetEvent(m_sharedData.GetSendReadEventHandle());
	Sleep(0);
	m_sharedData.ResetAll();
	m_eStatus = WS_PREBEGIN;
	return TRUE;
}

/*
BOOL cWeb::Refresh()
{
	if(m_pThread->GetExitCode() == STILL_ACTIVE)
	{
		return TRUE;
	}
	else if(m_pThread->GetExitCode() == 3)
	{
		return TRUE;
	}
	else if(m_pThread->GetExitCode() == 0xFFFFFFFF)
	{
		return TRUE;
	}
	else
	{
		End();
		Begin();
		return TRUE;
	}
}
*/

void cWeb::Request(const char *szURL)	//web page 요청
{
	ResetEvent(m_sharedData.GetSendWriteEventHandle());
	int lenURL = strlen(szURL);
	m_sharedData.AllocSendMsgBuffer(lenURL+1);
	char *sendBuf = m_sharedData.GetSendMsgBuffer();
	memcpy(sendBuf, szURL, lenURL+1);
	sendBuf[lenURL] = '\0';
	SetEvent(m_sharedData.GetSendReadEventHandle());
}

BOOL cWeb::Read(std::string &strContent, std::string &strError)		//요청했던 web page 읽기
{
	DWORD waitRet = WaitForSingleObject(m_sharedData.GetReceiveReadEventHandle(), 0);
	if(WAIT_OBJECT_0 == waitRet)
	{
		if(m_sharedData.GetReceiveMsgBuffer() != NULL)
		{
			strContent = m_sharedData.GetReceiveMsgBuffer();
			m_sharedData.FreeReceiveMsgBuffer();
		}
		if(m_sharedData.GetErrorMsgBuffer() != NULL)
		{
			strError = m_sharedData.GetErrorMsgBuffer();
			m_sharedData.FreeErrorMsgBuffer();
		}
		SetEvent(m_sharedData.GetReceiveWriteEventHandle());
		return TRUE;
	}
	return FALSE;
}

//-------------- cSharedWebData --------------//
cSharedWebData::cSharedWebData()
: m_hSendWriteEvent( NULL )
, m_hSendReadEvent( NULL )
, m_szSendMsgBuffer( NULL )
, m_hReceiveWriteEvent( NULL )
, m_hReceiveReadEvent( NULL )
, m_szReceiveMsgBuffer( NULL )
, m_szErrorMsgBuffer( NULL )
{
	m_hSendWriteEvent = CreateEvent(NULL, TRUE, TRUE, "Web Thread Send Buffer Write");
	m_hSendReadEvent = CreateEvent(NULL, FALSE, FALSE, "Web Thread Send Buffer Read");
	m_hReceiveWriteEvent = CreateEvent(NULL, TRUE, TRUE, "Web Thread Receive Buffer Write");
	m_hReceiveReadEvent = CreateEvent(NULL, FALSE, FALSE, "Web Thread Receive Buffer Read");
}

cSharedWebData::~cSharedWebData()
{
	CloseHandle(m_hSendWriteEvent);
	CloseHandle(m_hSendReadEvent);
	CloseHandle(m_hReceiveWriteEvent);
	CloseHandle(m_hReceiveReadEvent);

	FreeSendMsgBuffer();
	FreeReceiveMsgBuffer();
}

///초기상태로 복구. 보통 오류가 났을 경우 초기상태로 되돌리기 위한 수단임.
void cSharedWebData::ResetAll()
{
	ResetEvent(m_hSendReadEvent);
	ResetEvent(m_hSendWriteEvent);
	FreeSendMsgBuffer();
	SetEvent(m_hSendWriteEvent);

	ResetEvent(m_hReceiveReadEvent);
	ResetEvent(m_hReceiveWriteEvent);
	FreeReceiveMsgBuffer();
	SetEvent(m_hReceiveWriteEvent);
}

void cSharedWebData::AllocSendMsgBuffer(int size)
{
	FreeSendMsgBuffer();
	m_szSendMsgBuffer = new char[size];
}

void cSharedWebData::FreeSendMsgBuffer()
{
	delete[] m_szSendMsgBuffer;
	m_szSendMsgBuffer = NULL;
}

void cSharedWebData::AllocReceiveMsgBuffer(int size)
{
	FreeReceiveMsgBuffer();
	m_szReceiveMsgBuffer = new char[size];
}

void cSharedWebData::FreeReceiveMsgBuffer()
{
	delete[] m_szReceiveMsgBuffer;
	m_szReceiveMsgBuffer = NULL;
}

void cSharedWebData::AllocErrorMsgBuffer(int size)
{
	FreeErrorMsgBuffer();
	m_szErrorMsgBuffer = new char[size];
}

void cSharedWebData::FreeErrorMsgBuffer()
{
	delete[] m_szErrorMsgBuffer;
	m_szErrorMsgBuffer = NULL;
}

//안태훈 수정 끝	//(5th Closed beta)(0.2)

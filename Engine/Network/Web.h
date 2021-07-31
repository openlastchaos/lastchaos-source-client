//안태훈 수정 시작	//(5th Closed beta)(0.2)
#ifndef __WEB_H__
#define __WEB_H__

#include <Windows.h>
#include <string>
#include <vector>
#include <map>

class cSharedWebData;
class cThreadWrapper;

ENGINE_API void WebAddressInit();
//DWORD WINAPI WebThreadFunction(void *parameter);
UINT WINAPI WebThreadFunction(void *parameter);
std::string ConvertStringToWebParameter(const char *szParam);

//-------------- cWebAddress --------------//
class cWebAddress
{
private:
	typedef std::map<std::string, std::string> web_map;
public:
	~cWebAddress();
	inline static cWebAddress &Instance()	{ return m_instance;	}

	BOOL RegisterAddress(const char *alias, const char *address);
	const char *Address(const char *alias);
private:
	cWebAddress();
private:
	static cWebAddress m_instance;
	std::map<std::string, std::string> m_mapAddressAlias;
};

inline BOOL WebAddressRegister(const char *szAlias, const char *szAddress)
{
	return cWebAddress::Instance().RegisterAddress(szAlias, szAddress);
}

inline const char *WebAddress(const char *szAlias)
{
	return cWebAddress::Instance().Address(szAlias);
}

//-------------- cSharedWebData --------------//
///Web thread와 Game thread간 공유될 객체의 class
class cSharedWebData
{
public:
	cSharedWebData();
	~cSharedWebData();
	
	void ResetAll();

	HANDLE GetSendWriteEventHandle()	{ return m_hSendWriteEvent;		}
	HANDLE GetSendReadEventHandle()		{ return m_hSendReadEvent;		}
	HANDLE GetReceiveWriteEventHandle()	{ return m_hReceiveWriteEvent;	}
	HANDLE GetReceiveReadEventHandle()	{ return m_hReceiveReadEvent;	}

	void AllocSendMsgBuffer(int size);
	void FreeSendMsgBuffer();
	char *GetSendMsgBuffer()			{ return m_szSendMsgBuffer;		}

	void AllocReceiveMsgBuffer(int size);
	void FreeReceiveMsgBuffer();
	char *GetReceiveMsgBuffer()			{ return m_szReceiveMsgBuffer;	}

	void SetExit(BOOL bExit)			{ m_bExit = bExit;				}
	BOOL GetExit()						{ return m_bExit;				}

	void AllocErrorMsgBuffer(int size);
	void FreeErrorMsgBuffer();
	char *GetErrorMsgBuffer()			{ return m_szReceiveMsgBuffer;	}

private:
	HANDLE	m_hSendWriteEvent;		//Send Message Buffer를 쓰기 위해 접근할 때 확인할 event
	HANDLE	m_hSendReadEvent;		//           〃         읽기            〃
	char	*m_szSendMsgBuffer;
	HANDLE	m_hReceiveWriteEvent;	//Receive Message Buffer를 쓰기 위해 접근할 때 확인할 event
	HANDLE	m_hReceiveReadEvent;	//           〃         읽기            〃
	char	*m_szReceiveMsgBuffer;
	BOOL	m_bExit;
	char	*m_szErrorMsgBuffer;	//접근 권한은 Receive Message Buffer와 공유
};


//-------------- cWeb --------------//
class cWeb
{
private:
	enum WEB_STATUS
	{
		WS_PREBEGIN = 0,
		WS_READYREQUEST,
		WS_REQUESTED,
		WS_READYREAD,
	};
public:
	cWeb();
	~cWeb();

	//control function
	ENGINE_API BOOL Begin();
	ENGINE_API BOOL End();
	//BOOL Refresh();

	//web function
	void Request(const char *szURL);	//web page 요청
	BOOL Read(std::string &strContent, std::string &strError);		//요청했던 web page 읽기.

	//status function
	BOOL IsBegin()			{ return m_eStatus != WS_PREBEGIN;	}
	BOOL IsPreBegin()		{ return m_eStatus == WS_PREBEGIN;	}
	BOOL IsRequest()		{ return m_eStatus == WS_REQUESTED;		}
	BOOL IsReadyRequest()	{ return m_eStatus == WS_READYREQUEST;	}
	//BOOL IsReadyRead()		{ return m_eStatus == WS_READYREAD;		}
private:
	WEB_STATUS		m_eStatus;
	cSharedWebData	m_sharedData;
	cThreadWrapper	*m_pThread;
};


#endif //__WEB_H__
//안태훈 수정 끝	//(5th Closed beta)(0.2)

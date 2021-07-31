#ifndef __LOGIN_H__
#define __LOGIN_H__
#include <Engine/LocalDefine.h>

enum eLOGO_TYPE
{
	eLOGO_NORMAL = 0,
	eLOGO_HOLLOWEEN,
	eLOGO_XMAS,
	eLOGO_MAX,
};

class ENGINE_API LoginNew
{
public:
	LoginNew(void);
	virtual ~LoginNew(void);
	
	BOOL	ConnectToLoginServer();

	BOOL		GetSaveID()				{ return m_bSave; }
	CTString	GetUserId()				{ return m_strUserID; }
	CTString	GetPassword()			{ return m_strUserPW; }

	void	SetSaveID(BOOL bSave);
	void	SetUserId(CTString strId)		{ m_strUserID = strId; }
	void	SetPassword(CTString strPw)		{ m_strUserPW = strPw; }
	
	void	SetLogoType(int nType);
	int		GetLogoType()			{ return m_nLogoType; }
private:
	void	SetVersion();

	BOOL	m_bSave;
	CTString m_strUserID;						// User ID
	CTString m_strUserPW;						// User Password
	int		m_nLogoType;
};



#endif	//	__LOGIN_H__
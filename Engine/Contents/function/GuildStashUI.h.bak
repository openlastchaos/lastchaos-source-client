
#ifndef	__GUILDSTASH_UI_H__
#define	__GUILDSTASH_UI_H__

struct STakeInfo
{
	int			m_nMonth;
	int			m_nDay;
	LONGLONG	m_llMoney;

	void SetData( int nMonth, int nDay, LONGLONG llMoney )
	{
		m_nMonth	= nMonth;
		m_nDay		= nDay;
		m_llMoney	= llMoney;

	}

};

typedef std::vector<STakeInfo>	VTakeInfo;


enum eGuilStashState
{
	GSS_NPC,
	GSS_VIEW,
	GSS_TAKE,
};

class CGuildStashUI : public CUIWindow
{
public:
	CGuildStashUI();
	~CGuildStashUI();

	void initialize();
	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	MsgBoxLCommand( int nCommandCode, int nResult );

	ENGINE_API void Message( int nCommandCode, CTString strMessage, DWORD dwStyle );

	// Open
	ENGINE_API void	OpenGuildStash();

	void OpenTake( LONGLONG nBalance );
	void CloseStash();

	void AddTakeInfo( int nMonth, int nDay, LONGLONG llMoney );
	void ResetTakeInfo();

	void ErrorProc( LONG lErrorCode, UBYTE ubType );
	void ReceiveGuildStashMessage( UBYTE ubType, CNetworkMessage* istr );
	void SendGuildStashListReq();
	void SendGuildStashKeepReq();
	void SendGuildStashTakeReq();

	//20 거
	void InNas();
	void OutNas();
	void SendGuildStashListReqNas();

	//	 
	void GuildStashDoesMessageBoxExist();
	void AddItemCallback();
	void AddItemToBasket();
	void DelItemInBasket(CUIIcon* pIcon);

	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
private:
	BOOL					m_bIsTake;								//보관인지 찾는것인지 결정 T 가 찾는것 
	enum __tagGuildStashMode
	{
		GSMODE_DUTY,
		GSMODE_KEEP,
		GSMODE_TAKE,
		GSMODE_LOG,	
	};

	enum __tagGuildStashItem
	{
		GSITEM_COL = 5,
		GSITEM_ROW = 4,
	};

	enum __tagGuildStashLogCulumnType
	{
		GSLOG_COLUMN_TIME,
		GSLOG_COLUMN_USERNAME,
		GSLOG_COLUMN_ACTION,
		GSLOG_COLUMN_ITEMNAME,
		GSLOG_COLUMN_ITEMCOUNT,
	};

	enum __tagGuildStashNetError
	{
		GSERR_SUCCESS,
		GSERR_NO_GUILD,
		GSERR_INVEN_FULL,
		GSERR_WEIGHT_FULL,
		GSERR_USED,
		GSERR_DONT_KEEP_ITEM,
		GSERR_NO_PERMISSION,
		GSERR_NO_GUILD_SKILL,
		GSERR_CREATE_FAIL,
		GSERR_STASH_FULL,	// [2012/06/01 : Sora] ITS 9436 길드창고 공간 부족 메시지 추가
	};

	CTString m_strTitle;

	// Properties
	eGuilStashState		GameState;
	CUIButton	m_btnOk;
	LONGLONG	m_llBalance;		// Balance;
	VTakeInfo	m_vTakeInfo;
	
	CTString	m_strTotalNas;	//보관 금액
	CTString	m_strInNas;	//입금 금액
	CTString	m_strOutNas;	//출금 금액

	CUIButton*	m_pBtnProcess;

	CUIButton*	m_pBtnNas;		//입금 

	CUIArray*	m_pArrKeep;
	CUIArray*	m_pArrTake;

	CUIIcon*	m_pIconsTakeItem[GSITEM_COL];
	CItems*		m_pItemsDummy;

	CUIText*	m_pTxtInOutNas;
	CUIText*	m_pTxtInOutNasCnt;
	CUIText*	m_pTxtKeepNasCnt;
	CUIText*	m_pTxtTitle;
	CUIImageSplit* m_pImgKeepNas_bg;

	__tagGuildStashMode	m_enMode;

	// data of stash part.
	CTString m_strLeftUsedTime;
	INDEX	 m_iStashCapacity;

	//2013/04/05 jeil 나스 아이템 제거
	__int64	m_nInNas;
	__int64	m_nOutNas;
	__int64	m_nTotalNas;
	void _openStashView( __tagGuildStashMode _mode = GSMODE_TAKE );
	void _openDutyView();
	void _closeStashView();
		 
	void _initStashUI();
	void _initTakeItemBtn();
		 
	void _createStashUI();

	const bool _isVaildateData();

	void _enableProcessButton(BOOL bEnable)									
	{ m_pBtnProcess->SetEnable(bEnable);	}

	WMSG_RESULT _mouseStashViewMsg( MSG *pMsg );
	WMSG_RESULT _mouseStashLogMsg( MSG *pMsg );
	WMSG_RESULT _mouseStashDummyMsg( MSG *pMsg ) { return WMSG_FAIL;		}

	void _showNetErrorMsgBox( int _error );
	void _showErrorMsgBox( int _error );

	void _addItemToBasket( CItems* pItem);
	void _copyItemToBasket();
	CItems* GetStashItem(int idx);
};

#endif	// __GUILDSTASH_UI_H__



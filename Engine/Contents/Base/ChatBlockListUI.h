#ifndef __CHAT_BLOCK_LIST_UI_H__
#define __CHAT_BLOCK_LIST_UI_H__

#define MAX_SPACE_ARRAY_NUM		80
#define MAX_STR_LENGTH          64

typedef struct _tagFILTER 
{
	std::string strString;
	std::string strReword;

	int		nString;
	int		nReword;

	//------------------------------------------------------------------------------
	// SFILTER
	// Explain: 持失切~
	// Date : 2005-01-28(神板 7:31:52) Lee Ki-hwan
	//------------------------------------------------------------------------------
	_tagFILTER ( const char* _szString = NULL, const char* _szReword = NULL )
	{
		strString = _szString;
		strReword = _szReword;

		nString = strString.length();
		nReword = strReword.length();
	}

} FILTER;

typedef std::vector<FILTER> FILTER_VECTOR;

class CChatBlockListUI : public CUIWindow
{
public:
	CChatBlockListUI();
	~CChatBlockListUI();

	void initialize();
	void ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void OpenChatFilter();
	void CloseChatFilter();
	void ListUpdate();
	void SaveFile();
	void AddCharName(char* strCharName);
	void DelCharName();
	void Message( int nCommandCode, CTString strTitle, CTString strMessage, DWORD dwStyle );
	void MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );

	void CharacterFilterInSimplePopup( CTString strCharName );

	void OpenAddPopup();
	void ChangeColor();
private:
	CUIList* m_pListBlock;
	CTString m_strSelName;
	int		 m_nOldSelect;
};

class CUIFiltering
{
	FILTER_VECTOR	m_vList;	

	char	m_strRefineWord[MAX_STR_LENGTH];
	int		m_nListCount;
	char	m_strFileName[1024];
	bool	m_bCharacterfilter;

public :
	CUIFiltering()		{ Clear();	}	
	~CUIFiltering()		{ Clear();	}

	void Clear();
	BOOL Read( char* file_name , bool charfilter = false);

	BOOL Filtering( char* syntax );
	BOOL Filtering( char* syntax, int* szReturn );

	void EnDecoding( char* szResult, const char* szData );

	char* GetString( int nIndex ) const;
	char* GetKey() const;
	char* GetRefineWord() const;

	void SetFileName( char* szData );
	char* GetFileName() const;
	int GetListCount()	{ return m_nListCount; }
};

extern CUIFiltering _UIFiltering;
extern CUIFiltering _UIFilteringCharacter;
extern CUIFiltering _UICharacterChatFilter;	

#endif // __CHAT_BLOCK_LIST_UI_H__
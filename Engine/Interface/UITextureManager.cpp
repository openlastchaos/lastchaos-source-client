#include "stdh.h"
#include <Engine/Interface/UITextureManager.h>
#include <Engine/Interface/UIManager.h>


// ----------------------------------------------------------------------------
// Name : CUIFontTextureManager()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIFontTextureManager::CUIFontTextureManager()
{
	// Font texture
	for( INDEX iTex = 0; iTex < FONT_MAX; iTex++ )
		m_aptdFont[iTex] = NULL;

	memset(m_aFontWidthThai,0,sizeof(int[6][16]));

}

// ----------------------------------------------------------------------------
// Name : ~CUITextTextureManager()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIFontTextureManager::~CUIFontTextureManager()
{
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIFontTextureManager::Create()
{
	INDEX		iTex;
	CTString	strFullPath;
	CTString	strDirectory = CTString( "\\Data\\Interface\\" );
	CTString	strFileName;

	extern INDEX	g_iCountry;
	switch( g_iCountry )
	{
	case KOREA:
		{
			m_nLanguage = FONT_KOREAN;
			for( iTex = 0; iTex < TEXCT_FONT_KOREAN; iTex++ )
			{
				strFileName.PrintF( "FontKorean%d.tex", iTex );
				strFullPath = strDirectory + strFileName;
				m_aptdFont[iTex] = _pTextureStock->Obtain_t( strFullPath );
			}
			m_nFontTextureCount = TEXCT_FONT_KOREAN;
			m_nFontSpacing = 0;
			m_nLineSpacing = 1;
		}
		break;

	case TAIWAN:
	case TAIWAN2:
		{
			m_nLanguage = FONT_CHINESE_T;
			for( iTex = 0; iTex < TEXCT_FONT_CHINESE_T; iTex++ )
			{
				strFileName.PrintF( "FontChineseT%d.tex", iTex );
				strFullPath = strDirectory + strFileName;
				m_aptdFont[iTex] = _pTextureStock->Obtain_t( strFullPath );
			}
			m_nFontTextureCount = TEXCT_FONT_CHINESE_T;
			m_nFontSpacing = 1;
			m_nLineSpacing = 2;
		}
		break;

	case CHINA:		// China
		{
			m_nLanguage = FONT_CHINESE_S;
			for( iTex = 0; iTex < TEXCT_FONT_CHINESE_S; iTex++ )
			{
				strFileName.PrintF( "FontChineseS%d.tex", iTex );
				strFullPath = strDirectory + strFileName;
				m_aptdFont[iTex] = _pTextureStock->Obtain_t( strFullPath );
			}
			m_nFontTextureCount = TEXCT_FONT_CHINESE_S;
			m_nFontSpacing = 1;
			m_nLineSpacing = 2;
		}
		break;

	case THAILAND:
		{
			m_nLanguage = FONT_THAILAND;
			for( iTex = 0; iTex < TEXCT_FONT_THAILAND; iTex++ )
			{
				//test 050910
				strFileName="FontThailand0.tex";
				strFullPath = strDirectory + strFileName;
				m_aptdFont[iTex] = _pTextureStock->Obtain_t( strFullPath );
			}
			m_nFontTextureCount = TEXCT_FONT_THAILAND;
			m_nFontSpacing = 1;
			m_nLineSpacing = 2;

			// Size of font texture
			m_nFontTexWidth = m_aptdFont[0]->GetPixWidth();
			m_nFontTexHeight = m_aptdFont[0]->GetPixHeight();

			// Font information
			m_nFontWidth = 6;
			m_nFontWidth2Byte = 12;
			m_nFontHeight = 12;
			m_nLineHeight = m_nFontHeight + m_nLineSpacing;
			
			// wooss 059030 for non-fixed font
			int fontWidthThai[6][16]={
						6,	7,	7,	8,	7,	7,	8,	5,	6,	7,	8,	9, 10, 10,	7,	7,
						6, 10, 10, 10,	7,	7,	7,	8,	6,	7,	7,	7,	7,  7,	8,	8,
						7,	7,	7,	5,	6,	7,	7,	5,	8, 10,	8,	7,	9,	7,	8,	6,
						5,	7,	5,	9,	7,	7,	7,	7,	7,	7,	7,	6,	6,	6,	6,	6,
						3,	6,	5,	5,	6,	5,	5,	7, 	7,	7,	7,	7,	7,	7,	7,	7,
						7,	7,	8,	8,	10,	10,	7,	9,	9,	9,	7,	12,	6,	6,	6,	6		
			};

			memcpy(m_aFontWidthThai,fontWidthThai,sizeof(fontWidthThai));

			return;
		}
		break;
	case JAPAN :		// JAPAN
	{
		// wooss japan font test 051021			
		m_nLanguage = FONT_JAPANESE;
		for( iTex = 0; iTex < TEXCT_FONT_JAPAN; iTex++ )
		{
			strFileName.PrintF( "FontJapanese%d.tex", iTex );
			strFullPath = strDirectory + strFileName;
			m_aptdFont[iTex] = _pTextureStock->Obtain_t( strFullPath );
		}
		m_nFontTextureCount = TEXCT_FONT_JAPAN;
		m_nFontSpacing = 0;
		m_nLineSpacing = 2;
	}
	break;
	case MALAYSIA:
	case HONGKONG:
		{
			m_nLanguage = FONT_CHINESE_T;
			for( iTex = 0; iTex < TEXCT_FONT_CHINESE_T; iTex++ )
			{
				strFileName.PrintF( "FontChineseT%d.tex", iTex );
				strFullPath = strDirectory + strFileName;
				m_aptdFont[iTex] = _pTextureStock->Obtain_t( strFullPath );
			}
			m_nFontTextureCount = TEXCT_FONT_CHINESE_T;
			m_nFontSpacing = 0;
			m_nLineSpacing = 2;
		}
		break;
	case USA:
		{
			m_nLanguage = FONT_CHINESE_T;

			for( iTex = 0; iTex < TEXCT_FONT_USA; iTex++ )
			{
				//strFileName.PrintF( "FontUSA%d.tex", iTex);
				strFileName.PrintF( "FontChineseT%d.tex", iTex );
				strFullPath = strDirectory + strFileName;
				m_aptdFont[iTex] = _pTextureStock->Obtain_t( strFullPath );
			}

			m_nFontTextureCount = TEXCT_FONT_USA;
			m_nFontSpacing = 0;
			m_nLineSpacing = 2;
		}
		break;
	case BRAZIL:
		{
			m_nLanguage = FONT_PROTUGES;

			for( iTex = 0; iTex < TEXCT_FONT_BRZ; iTex++ )
			{
				strFileName.PrintF( "FontBrazil%d.tex", iTex );
				strFullPath = strDirectory + strFileName;
				m_aptdFont[iTex] = _pTextureStock->Obtain_t( strFullPath );
			}

			m_nFontTextureCount = TEXCT_FONT_BRZ;
			m_nFontSpacing = 0;
			m_nLineSpacing = 2;
		}
		break;		
	case GERMANY:
		{
			m_nLanguage = FONT_GERMAN;

			for( iTex = 0; iTex < TEXCT_FONT_GERMAN; iTex++ )
			{
				strFileName.PrintF( "FontGerman%d.tex", iTex );
				strFullPath = strDirectory + strFileName;
				m_aptdFont[iTex] = _pTextureStock->Obtain_t( strFullPath );
			}

			m_nFontTextureCount = TEXCT_FONT_GERMAN;
			m_nFontSpacing = 0;
			m_nLineSpacing = 2;
		}
		break;
	case SPAIN://FRANCE_SPAIN_CLOSEBETA_NA_20081124
		{
			m_nLanguage = FONT_SPAIN;
			for (iTex = 0; iTex < TEXCT_FONT_SPAIN; iTex++)
			{
				strFileName.PrintF("FontSpain%d.tex", iTex);
				strFullPath = strDirectory + strFileName;
				m_aptdFont[iTex] = _pTextureStock->Obtain_t(strFullPath);
			}
			m_nFontTextureCount = TEXCT_FONT_SPAIN;
			m_nFontSpacing = 0; 
			m_nLineSpacing = 2;
		}
		break;
	case FRANCE:
		{
			m_nLanguage = FONT_FRANCE;
			for (iTex = 0; iTex < TEXCT_FONT_FRANCE; iTex++)
			{
				strFileName.PrintF("FontFrance%d.tex", iTex);
				strFullPath = strDirectory + strFileName;
				m_aptdFont[iTex] = _pTextureStock->Obtain_t(strFullPath);
			}
			m_nFontTextureCount = TEXCT_FONT_FRANCE;
			m_nFontSpacing = 0; 
			m_nLineSpacing = 2;
		}
		break;
	case POLAND:
		{
			m_nLanguage = FONT_POLAND;
			for (iTex = 0; iTex < TEXCT_FONT_POLAND; iTex++)
			{
				strFileName.PrintF("FontPOLAND%d.tex", iTex);
				strFullPath = strDirectory + strFileName;
				m_aptdFont[iTex] = _pTextureStock->Obtain_t(strFullPath);
			}
			m_nFontTextureCount = TEXCT_FONT_POLAND;
			m_nFontSpacing = 0; 
			m_nLineSpacing = 2;
		}
		break;
		
	case TURKEY:
		{
			m_nLanguage = FONT_TURKEY;
			for (iTex = 0; iTex < TEXCT_FONT_TURKEY; iTex++)
			{
				strFileName.PrintF("FontTURKEY%d.tex", iTex);
				strFullPath = strDirectory + strFileName;
				m_aptdFont[iTex] = _pTextureStock->Obtain_t(strFullPath);
			}
			m_nFontTextureCount = TEXCT_FONT_TURKEY;
			m_nFontSpacing = 0; 
			m_nLineSpacing = 2;
		}
		break;
	};

	// Size of font texture
	m_nFontTexWidth = m_aptdFont[0]->GetPixWidth();
	m_nFontTexHeight = m_aptdFont[0]->GetPixHeight();

	// Font information
	m_nFontWidth = 6;
	m_nFontWidth2Byte = 12;
	m_nFontHeight = 12;
	m_nLineHeight = m_nFontHeight + m_nLineSpacing;
}

// ----------------------------------------------------------------------------
// Name : Destroy()
// Desc :
// ----------------------------------------------------------------------------
void CUIFontTextureManager::Destroy()
{
	// Font texture
	for( INDEX iTex = 0; iTex < FONT_MAX; iTex++ )
	{
		if( m_aptdFont[iTex] != NULL )
		{
			_pTextureStock->Release( m_aptdFont[iTex] );
			m_aptdFont[iTex] = NULL;
		}
	}
}





// ----------------------------------------------------------------------------
// Name : CUIButtonTextureManager()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIButtonTextureManager::CUIButtonTextureManager()
{
	m_aptdButton[UBET_ITEM] = NULL;
	m_aptdButton[UBET_SKILL] = NULL;
	m_aptdButton[UBET_ACTION] = NULL;
	m_aptdButton[TEXCT_QUEST] = NULL;
	m_aptdButton[TEXCT_EVENT] = NULL;
}

// ----------------------------------------------------------------------------
// Name : ~CUIButtonTextureManager()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIButtonTextureManager::~CUIButtonTextureManager()
{
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIButtonTextureManager::Create()
{
	INDEX	iTex;
	CTString	strFullPath;
	CTString	strDirectory = CTString( "\\Data\\Interface\\" );
	CTString	strFileName;

	// Item texture
	m_aptdButton[UBET_ITEM] = new CTextureData *[TEXCT_ITEM];
	for( iTex = 0; iTex < TEXCT_ITEM; iTex++ )
	{
		strFileName.PrintF( "ItemBtn%d.tex", iTex );
		strFullPath = strDirectory + strFileName;
		m_aptdButton[UBET_ITEM][iTex] = _pTextureStock->Obtain_t( strFullPath );
	}

	// Skill texture
	m_aptdButton[UBET_SKILL] = new CTextureData *[TEXCT_SKILL];
	for( iTex = 0; iTex < TEXCT_SKILL; iTex++ )
	{
		strFileName.PrintF( "SkillBtn%d.tex", iTex );
		strFullPath = strDirectory + strFileName;
		m_aptdButton[UBET_SKILL][iTex] = _pTextureStock->Obtain_t( strFullPath );
	}

	// Action texture
	m_aptdButton[UBET_ACTION] = new CTextureData *[TEXCT_ACTION];
	for( iTex = 0; iTex < TEXCT_ACTION; iTex++ )
	{
		strFileName.PrintF( "ActionBtn%d.tex", iTex );
		strFullPath = strDirectory + strFileName;
		m_aptdButton[UBET_ACTION][iTex] = _pTextureStock->Obtain_t( strFullPath );
	}

	// Quest texture
	m_aptdButton[UBET_QUEST] = new CTextureData *[TEXCT_QUEST];
	for( iTex = 0; iTex < TEXCT_QUEST; iTex++ )
	{
		strFileName.PrintF( "QuestBtn%d.tex", iTex );
		strFullPath = strDirectory + strFileName;
		m_aptdButton[UBET_QUEST][iTex] = _pTextureStock->Obtain_t( strFullPath );
	}

	// Event texture
	m_aptdButton[UBET_EVENT] = new CTextureData *[TEXCT_EVENT];
	for( iTex = 0; iTex < TEXCT_EVENT; iTex++ )
	{
		strFileName.PrintF( "EventBtn%d.tex", iTex );
		strFullPath = strDirectory + strFileName;
		m_aptdButton[UBET_EVENT][iTex] = _pTextureStock->Obtain_t( strFullPath );
	}

	// Remission texture
	m_aptdButton[UBET_REMISSION] = new CTextureData *[TEXCT_REMISSION];
	for( iTex = 0; iTex < TEXCT_REMISSION; iTex++ )
	{
		strFileName.PrintF( "RemissionBtn%d.tex", iTex );
		strFullPath = strDirectory + strFileName;
		m_aptdButton[UBET_REMISSION][iTex] = _pTextureStock->Obtain_t( strFullPath );
	}

	// MissionCase texture
	m_aptdButton[UBET_COMBO] = new CTextureData *[TEXCT_COMBO];
	for( iTex = 0; iTex < TEXCT_COMBO; iTex++ )
	{
		strFileName.PrintF( "ComboBtn%d.tex", iTex );
		strFullPath = strDirectory + strFileName;
		m_aptdButton[UBET_COMBO][iTex] = _pTextureStock->Obtain_t( strFullPath );
	}


	m_aptdButton[UBET_AUCTION] = new CTextureData *[TEXCT_AUCTION];
	
	strFileName.PrintF( "Auction.tex");
	strFullPath = strDirectory + strFileName;
	m_aptdButton[UBET_AUCTION][0] = _pTextureStock->Obtain_t( strFullPath );
}

// ----------------------------------------------------------------------------
// Name : Destroy()
// Desc :
// ----------------------------------------------------------------------------
void CUIButtonTextureManager::Destroy()
{
	INDEX	iTex;

	// Item texture
	if( m_aptdButton[UBET_ITEM] != NULL )
	{
		for( iTex = 0; iTex < TEXCT_ITEM; iTex++ )
		{
			if( m_aptdButton[UBET_ITEM][iTex] != NULL )
			{
				_pTextureStock->Release( m_aptdButton[UBET_ITEM][iTex] );
				m_aptdButton[UBET_ITEM][iTex] = NULL;
			}
		}

		delete [] m_aptdButton[UBET_ITEM];
		m_aptdButton[UBET_ITEM] = NULL;
	}

	// Skill texture
	if( m_aptdButton[UBET_SKILL] != NULL )
	{
		for( iTex = 0; iTex < TEXCT_SKILL; iTex++ )
		{
			if( m_aptdButton[UBET_SKILL][iTex] != NULL )
			{
				_pTextureStock->Release( m_aptdButton[UBET_SKILL][iTex] );
				m_aptdButton[UBET_SKILL][iTex] = NULL;
			}
		}

		delete [] m_aptdButton[UBET_SKILL];
		m_aptdButton[UBET_SKILL] = NULL;
	}

	// Action texture
	if( m_aptdButton[UBET_ACTION] != NULL )
	{
		for( iTex = 0; iTex < TEXCT_ACTION; iTex++ )
		{
			if( m_aptdButton[UBET_ACTION][iTex] != NULL )
			{
				_pTextureStock->Release( m_aptdButton[UBET_ACTION][iTex] );
				m_aptdButton[UBET_ACTION][iTex] = NULL;
			}
		}

		delete [] m_aptdButton[UBET_ACTION];
		m_aptdButton[UBET_ACTION] = NULL;
	}

	// Quest texture
	if( m_aptdButton[UBET_QUEST] != NULL )
	{
		for( iTex = 0; iTex < TEXCT_QUEST; iTex++ )
		{
			if( m_aptdButton[UBET_QUEST][iTex] != NULL )
			{
				_pTextureStock->Release( m_aptdButton[UBET_QUEST][iTex] );
				m_aptdButton[UBET_QUEST][iTex] = NULL;
			}
		}

		delete [] m_aptdButton[UBET_QUEST];
		m_aptdButton[UBET_QUEST] = NULL;
	}

	// Event texture
	if( m_aptdButton[UBET_EVENT] != NULL )
	{
		for( iTex = 0; iTex < TEXCT_EVENT; iTex++ )
		{
			if( m_aptdButton[UBET_EVENT][iTex] != NULL )
			{
				_pTextureStock->Release( m_aptdButton[UBET_EVENT][iTex] );
				m_aptdButton[UBET_EVENT][iTex] = NULL;
			}
		}

		delete [] m_aptdButton[UBET_EVENT];
		m_aptdButton[UBET_EVENT] = NULL;
	}

	// Remission texture
	if( m_aptdButton[UBET_REMISSION] != NULL )
	{
		for( iTex = 0; iTex < TEXCT_REMISSION; iTex++ )
		{
			if( m_aptdButton[UBET_REMISSION][iTex] != NULL )
			{
				_pTextureStock->Release( m_aptdButton[UBET_REMISSION][iTex] );
				m_aptdButton[UBET_REMISSION][iTex] = NULL;
			}
		}

		delete [] m_aptdButton[UBET_REMISSION];
		m_aptdButton[UBET_REMISSION] = NULL;
	}

	// MissionCase texture
	if( m_aptdButton[UBET_COMBO] != NULL )
	{
		for( iTex = 0; iTex < TEXCT_COMBO; iTex++ )
		{
			if( m_aptdButton[UBET_COMBO][iTex] != NULL )
			{
				_pTextureStock->Release( m_aptdButton[UBET_COMBO][iTex] );
				m_aptdButton[UBET_COMBO][iTex] = NULL;
			}
		}

		delete [] m_aptdButton[UBET_COMBO];
		m_aptdButton[UBET_COMBO] = NULL;
	}

	if( m_aptdButton[UBET_AUCTION] != NULL )
	{
		if( m_aptdButton[UBET_AUCTION][0] != NULL )
		{
			_pTextureStock->Release( m_aptdButton[UBET_AUCTION][0] );
			m_aptdButton[UBET_AUCTION][0] = NULL;
		}

		delete [] m_aptdButton[UBET_AUCTION];
		m_aptdButton[UBET_AUCTION] = NULL;
	}
}

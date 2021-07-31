#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "News_Web_UI.h"
#include <Engine/Network/Web.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/function/News.h>
#include <Engine/Contents/Base/Notice.h>
#include <Engine/Contents/Base/UINoticeNew.h>

#ifndef		WORLD_EDITOR
#include <3rdparty/OpenAES/inc/oaes_lib.h>
#endif		// WORLD_EDITOR

extern cWeb			g_web;
extern HWND			_hDlgWeb;

class CmdNews_Web_Close : public Command
{
public:
	CmdNews_Web_Close() : m_pWnd(NULL) {}
	void setData(CNewsWebUI* pWnd)	{ m_pWnd = pWnd;	}
	void execute()
	{
		if (m_pWnd != NULL)
			m_pWnd->UpdateUrl();
	}

private:
	CNewsWebUI* m_pWnd;
};

CNewsWebUI::CNewsWebUI()
	: m_pWebBase(NULL)
	, m_nCurPage(0)
	, m_strDefUrl("")
{
	setInherit(false);
	LoadUrl();
}

CNewsWebUI::~CNewsWebUI()
{

}

void CNewsWebUI::initialize()
{
	CUIButton* pBtn = (CUIButton*)findUI("btn_close");

	if (pBtn != NULL)
	{
		CmdNews_Web_Close* pCmd = new CmdNews_Web_Close;
		pCmd->setData(this);
		pBtn->SetCommandUp(pCmd);
	}

	m_pWebBase = findUI("base_web");
}

void CNewsWebUI::OpenUI()
{
	if (IsVisible() == TRUE || GetHide() == FALSE)
		return;

	SetVisible(TRUE);
	Hide(FALSE);

	UIMGR()->RearrangeOrder(UI_NEWS_WEB, TRUE);

	if (m_pWebBase != NULL)
	{
		int nAbsX, nAbsY;
		m_pWebBase->GetAbsPos(nAbsX, nAbsY);
		g_web.SetSize(m_pWebBase->GetWidth(), m_pWebBase->GetHeight());
		g_web.SetPos(nAbsX, nAbsY);
		g_web.SendWebPageOpenMsg(TRUE);
		m_nCurPage = 0;
		UpdateUrl();
		UIMGR()->GetNotice()->SetIconBlink(Notice::NOTICE_NEWS_WEB, false);
	}
}

void CNewsWebUI::CloseUI()
{
	if (g_web.GetWebHandle())
		g_web.CloseWebPage(g_web.GetWebHandle());

	SetVisible(FALSE);
	Hide(TRUE);
	UIMGR()->RearrangeOrder(UI_NEWS_WEB, FALSE);

	m_nCurPage = 0;
}

bool CNewsWebUI::UpdateUrl()
{
	CNews* pNews = GAMEDATAMGR()->GetNews();

	if (pNews == NULL)
		return false;

	int nUrl = pNews->GetWebUrl(m_nCurPage);

	if (nUrl < 0)
	{
		CloseUI();
		return false;
	}

	updateParam();

	CTString strTemp;
	CTString strFull;

	strTemp.PrintF("popups/popup_%03d.htm", nUrl);
	strFull.PrintF("%s%s?v=%s", m_strDefUrl.c_str(), strTemp, m_strParam);
	g_web.SetWebUrl(std::string(strFull.str_String));
	++m_nCurPage;

	return true;
}

void CNewsWebUI::LoadUrl()
{
	CTString connectFile = _fnmApplicationPath.FileDir() + "lccnct.dta";

	FILE *fp = fopen(connectFile, "rb");
	if (!fp) 
	{
		return;
	}

	unsigned char cNum;
	int iNum, iNumTemp;

	int i, ret;
	for ( i = 0; i < 6; i++) 
	{		
		ret = fread(&cNum, 1, 1, fp);
		if( ret == 0 )
		{
			fclose(fp);
			return;
		}
	}
	fread(&iNum, sizeof(int), 1, fp);
	fread(&cNum, 1, 1, fp);

	int svrAddrNum;
	fread(&svrAddrNum, sizeof(int), 1, fp); // 서버 정보는 한개만 있어야 한다.
	iNumTemp = svrAddrNum;
	svrAddrNum -= iNum;
	iNum = iNumTemp;

	int size = 0;
	fread(&size, sizeof(int), 1, fp);		
	iNumTemp = size;
	size -= iNum;
	iNum = iNumTemp;

	unsigned char tmp;
	char data[256];
	for(i =0; i<size;i++)
	{
		ret = fread(&tmp, 1, 1, fp);
		if( ret == 0 )
		{
			fclose(fp);
			return;
		}

		data[i] = tmp - cNum;
		cNum = tmp;
	}
	data[size] = '\0';

	m_strDefUrl = data;

	fclose(fp);
}

BOOL CNewsWebUI::CloseWindowByEsc()
{
	return UpdateUrl() ? FALSE : TRUE;
}

void CNewsWebUI::updateParam()
{
#ifndef		WORLD_EDITOR

	if (m_strParam.empty() == false)
		return;

	std::string plain = boost::str( boost::format("%08x%08x%08x%08x") % _pNetwork->MyCharacterInfo.userIndex %
		_pNetwork->MyCharacterInfo.index % _pNetwork->m_iServerGroup % _pNetwork->m_iServerCh );


	// init
	int		i;
	OAES_CTX *ctx = NULL;
	ctx = oaes_alloc();

	// key
	{
		// key - EFF2E20F AD6F4099 5B1FD89D 1D678D62 C474818E 2BCF4870 6BA86F0B 59A17C7B
		uint32_t	key03 = 0x628D671D;
		uint32_t	key06 = 0x0B6FA86B;
		uint32_t	key04 = 0x8E8174C4;
		uint32_t	key00 = 0x0FE2F2EF;
		uint32_t	key05 = 0x7048CF2B;
		uint32_t	key02 = 0x9DD81F5B;		
		uint32_t	key01 = 0x99406FAD;
		uint32_t	key07 = 0x7B7CA159;		

		size_t key_size = 32; // AES256
		uint8_t* data = (uint8_t*) calloc(key_size, sizeof(uint8_t));

		memcpy(&data[0], &key00, sizeof(uint32_t));
		memcpy(&data[4], &key01, sizeof(uint32_t));
		memcpy(&data[8], &key02, sizeof(uint32_t));
		memcpy(&data[12], &key03, sizeof(uint32_t));
		memcpy(&data[16], &key04, sizeof(uint32_t));
		memcpy(&data[20], &key05, sizeof(uint32_t));
		memcpy(&data[24], &key06, sizeof(uint32_t));
		memcpy(&data[28], &key07, sizeof(uint32_t));

		if (OAES_RET_SUCCESS != oaes_key_import_data(ctx, data, key_size)) 
		{
			//printf("Error: Failed to generate OAES 256 bit key.\n");
			return;
		}

		free(data);
	}

	// encrypt
	{
		uint8_t _pad = 1;
		uint8_t _iv[OAES_BLOCK_SIZE] = "";
		uint8_t* enbuf;
		size_t out_size = plain.size();

		// iv - 7A868B3C 3D7725D1 B1E4F648 C4A54F44
		uint32_t iv02 = 0x48F6E4B1;
		uint32_t iv03 = 0x444FA5C4;
		uint32_t iv00 = 0x3C8B867A;		
		uint32_t iv01 = 0xD125773D;		

		enbuf = (uint8_t*) calloc(out_size, sizeof(uint8_t));

		// add salt
		memcpy(&_iv[0], &iv00, sizeof(uint32_t));
		memcpy(&_iv[4], &iv01, sizeof(uint32_t));
		memcpy(&_iv[8], &iv02, sizeof(uint32_t));
		memcpy(&_iv[12], &iv03, sizeof(uint32_t));

		oaes_encrypt(ctx, (const uint8_t*)plain.c_str(), plain.size(), enbuf, &out_size, _iv, &_pad);

		for (i = 0; i < out_size; ++i)
			m_strParam += boost::str( boost::format("%02x") % (int)enbuf[i] );

		free(enbuf);
	}

	// release
	oaes_free(&ctx);

#endif		// WORLD_EDITOR
}

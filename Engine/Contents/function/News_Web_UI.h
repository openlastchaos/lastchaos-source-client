#ifndef __NEWS_WEB_UI_H__
#define __NEWS_WEB_UI_H__

class CNewsWebUI : public CUIWindow
{
public:
	CNewsWebUI();
	~CNewsWebUI();

	void initialize();
	void OpenUI();
	void CloseUI();

	bool UpdateUrl();
	void LoadUrl();
	BOOL CloseWindowByEsc();

	void clear_param()	{ m_strParam = ""; }
private:

	void updateParam();

	CUIBase* m_pWebBase;
	int		m_nCurPage;
	std::string m_strDefUrl;
	std::string m_strParam;
};

#endif // __NEWS_WEB_UI_H__
#ifndef __GUILDSTASH_USE_RECORD_UI_H__
#define __GUILDSTASH_USE_RECORD_UI_H__

class CGuildStashUseRecordUI : public CUIWindow
{
public:
	CGuildStashUseRecordUI();
	~CGuildStashUseRecordUI();

	void initialize();
	void OpenUI(CTString& strGuildName);
	void CloseUI();
	void GSUseRecordListRecv(CNetworkMessage* istr);
	void GSUseRecordListReq();
private:
	CUIList* m_pListRecord;
	CUIText* m_pTitle;
	int m_nLastLogIdx;

};
#endif // __GUILDSTASH_USE_RECORD_UI_H__
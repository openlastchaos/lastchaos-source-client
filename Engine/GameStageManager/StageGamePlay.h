
#ifndef		_STAGE_PLAY_H_
#define		_STAGE_PLAY_H_

class CStageGamePlay : public CStage
{
public:
	CStageGamePlay();

	void Init();	
	void Release();
	void Run();

private:
	DWORD m_dwRemainTime;
	bool m_bSendRestart;
	bool m_bFirst;
};

#endif		// _STAGE_PLAY_H_
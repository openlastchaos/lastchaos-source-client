#ifndef __COMMAND_AFFINITY_H__
#define __COMMAND_AFFINITY_H__

class Command;

class CmdAffinityClose : public Command
{
public:
	CmdAffinityClose() : m_pWnd(NULL) {}
	void setData(CAffinityUI* pWnd) { m_pWnd = pWnd;	}
	void execute()
	{
		if (m_pWnd != NULL)
			m_pWnd->CloseDonation();
	}

private:
	CAffinityUI* m_pWnd;
};

class CmdAffinityConfirm : public Command
{
public:
	CmdAffinityConfirm() : m_pWnd(NULL)	{}
	void setData(CAffinityUI* pWnd)	{ m_pWnd = pWnd;	}
	void execute()
	{
		if (m_pWnd != NULL)
		{
			m_pWnd->_sendDonateMessage();
		}
	}
private:
	CAffinityUI* m_pWnd;
};

class CmdAffinitySum : public Command
{
public:
	CmdAffinitySum() : m_pWnd(NULL), m_nIdx(-1)	{}
	void setData(CAffinityUI* pWnd, int nIdx)	{ m_pWnd = pWnd;	m_nIdx = nIdx;}
	void execute()
	{
		if (m_pWnd != NULL)
		{
			m_pWnd->Donation_Sum(m_nIdx);
		}
	}
private:
	CAffinityUI* m_pWnd;
	int m_nIdx;
};

class CmdAffinitySub : public Command
{
public:
	CmdAffinitySub() : m_pWnd(NULL), m_nIdx(-1)	{}
	void setData(CAffinityUI* pWnd, int nIdx)	{ m_pWnd = pWnd;	m_nIdx = nIdx; }
	void execute()
	{
		if (m_pWnd != NULL)
		{
			m_pWnd->Donation_Sub(m_nIdx);
		}
	}
private:
	CAffinityUI* m_pWnd;
	int m_nIdx;
};

class CmdAffinityMax : public Command
{
public:
	CmdAffinityMax() : m_pWnd(NULL), m_nIdx(-1)	{}
	void setData(CAffinityUI* pWnd, int nIdx)	{ m_pWnd = pWnd;	m_nIdx = nIdx;	}
	void execute()
	{
		if (m_pWnd != NULL)
		{
			m_pWnd->Donation_Max(m_nIdx);
		}
	}
private:
	CAffinityUI* m_pWnd;
	int m_nIdx;
};

#endif // __COMMAND_AFFINITY_H__
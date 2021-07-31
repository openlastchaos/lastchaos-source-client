#ifndef __INPUTBUFFER_H__
#define __INPUTBUFFER_H__

#include "CPacket.h"

#define HTONL(n)		{ n = htonl(n); }
#define HTONS(n)		{ n = htons(n); }
#define NTOHL(n)		{ n = ntohl(n); }
#define NTOHS(n)		{ n = ntohs(n); }

class CInputBuffer  
{
public:
	unsigned char*	m_buf;
	int				m_size;
	int				m_ptr;
	unsigned short	m_id;

	CInputBuffer(int size = 40*MAX_PACKET_SIZE)
	{
		m_size = size + 100;
		m_buf = new unsigned char[m_size];
		Clear();
	}

	virtual ~CInputBuffer()
	{
		delete [] m_buf;
		m_buf = NULL;
		m_size = 0;
		m_ptr = 0;
	}

	void Clear()
	{
		memset(m_buf, 0, m_size);
		m_ptr = 0;
	}

	unsigned char* GetPoint()
	{
		ASSERT( m_ptr <= m_size && "BOF" );
		return m_buf + m_ptr;
	}

	int GetRemain()
	{
		ASSERT( m_size - m_ptr >= 0 && "Not enough buffer..." );
		return m_size - m_ptr;
	}

	void SetPoint(int dval)
	{
		m_ptr += dval;
	}

	CPacket* GetPacket()
	{
		if (m_ptr < MAX_HEADER_SIZE) 
			return NULL;
		unsigned short reliable;
		unsigned long seq;
		unsigned short id;
		unsigned long txSize;
		unsigned char* p = m_buf;
		memcpy(&reliable, p, sizeof(reliable));		p += sizeof(reliable);
		memcpy(&seq, p, sizeof(seq));				p += sizeof(seq);
		memcpy(&id, p, sizeof(id));					p += sizeof(id);
		memcpy(&txSize, p, sizeof(txSize));			p += sizeof(txSize);

		//0524 kwon
		NTOHS(reliable);
		NTOHL(seq);
		NTOHS(id);
		NTOHL(txSize);

		ASSERT(txSize <= MAX_PACKET_SIZE && txSize > 0);

		if ((int)(txSize + MAX_HEADER_SIZE) > m_ptr)
			return NULL;
		//CPacket* ret = new CPacket;
		//ret->Write(p, txSize, seq, id);
		CPacket* ret;
		ret = CreatePacket();
		ret->WriteToPacketRaw(m_buf,txSize + MAX_HEADER_SIZE);
		p += txSize;
		m_ptr -= (MAX_HEADER_SIZE + txSize);
		if (m_ptr)
		{
			unsigned char* tmp = new unsigned char[m_size];
			memset(tmp, 0, m_size);
			memcpy(tmp, p, m_ptr);
			delete [] m_buf;
			m_buf = tmp;
		}
		return ret;
	}
};

#endif

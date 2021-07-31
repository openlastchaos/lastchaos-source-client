#include <string.h>
#include <stdlib.h>
#include "LCCrypt.h"
#include "LCCrypt_Internel.h"

// 16비트 CRC 생성
static inline unsigned short LCCNM_CalcCRC(unsigned char* pData, int nLen)
{
	unsigned short crc = 0;
	int i;

	while (nLen-- > 0)
	{
		crc = crc ^ ((int)*pData++ << 8);
		i = 8;
		while (i-- > 0)
		{
			if (crc & 0x8000)
				crc = (crc << 1) ^ LCCNM_CRC;
			else
				crc <<= 1;
		}
	}

	return crc;
}

// return value		암호화된 길이, 오류시 -1
// pSrc				원본 데이터
// nLenSrc			원본 길이
// nKey				암호키
// pDest			암호 데이터 버퍼
int LCCrypt_Crypt(const unsigned char* pSrc, int nLenSrc, unsigned int nKey, unsigned char* pDest)
{
	// 변환 테이블
	#include "LCCrypt_TransTable"

	int nLenDest = nLenSrc + (LCCNM_BEGIN_SIZE + LCCNM_END_SIZE + LCCNM_CHECKSUM_SIZE + LCCNM_DUMMY_SIZE);
	unsigned short nCheckSum = 0;	// 체크섬
	int nIndexSrc = 0;				// pSrc 버퍼 인덱스
	int nIndexDest = 0;				// pDest 버퍼 인덱스
	unsigned char btTrans;			// 변환 테이블용
	unsigned char btKey[4];			// 키를 바이트로 분리

	if (nLenSrc < 0)
		return -1;

	btKey[0] = LCCNM_BYTE_3(nKey);
	btKey[1] = LCCNM_BYTE_2(nKey);
	btKey[2] = LCCNM_BYTE_1(nKey);
	btKey[3] = LCCNM_BYTE_0(nKey);

	// 더미 삽입
	pDest[nIndexDest] = LCCNM_DUMMY_TYPE;
	nIndexDest++;

	// 시작마크 삽입
	pDest[nIndexDest] = LCCNM_BEGIN_MARK;
	nIndexDest++;

	// 데이터 복사
	while (nIndexSrc < nLenSrc)
	{
		pDest[nIndexDest] = pSrc[nIndexSrc];
		nIndexDest++;
		nIndexSrc++;
	}

	// 끝 마크 삽입
	pDest[nIndexDest] = LCCNM_END_MARK;
	nIndexDest++;

	// 체크섬 생성 : 더미 제외
	nCheckSum = LCCNM_CalcCRC(pDest + LCCNM_DUMMY_SIZE, nIndexDest - LCCNM_DUMMY_SIZE);

	// 체크섬 삽입
	pDest[nIndexDest] = LCCNM_BYTE_1(nCheckSum);
	nIndexDest++;
	pDest[nIndexDest] = LCCNM_BYTE_0(nCheckSum);;
	nIndexDest++;

	// 데이터 암호화 : 더미 제외
	nIndexDest = LCCNM_DUMMY_SIZE;
	while (nIndexDest < nLenDest)
	{
		// 치환
		btTrans = LCCNM_TransTable[pDest[nIndexDest]];

		// 키와 XOR
		btTrans ^= btKey[nIndexDest % 4];

		// 오른쪽으로 Rotate
		btTrans = LCCNM_ROTATE_RIGHT(btTrans);

		// 키 변경
		btKey[nIndexDest % 4] = btTrans;

		// 값 저장
		pDest[nIndexDest] = btTrans;

		nIndexDest++;
	}

	return nLenDest;
}

// return value		복호된 길이, 오류시 -1
// pSrc				복호 데이터
// nLenSrc			복호 길이
// nKey				암호키
// pDest			평문 데이터 버퍼
// pTmpBuf			변환 임시 버퍼
int LCCrypt_Decrypt(const unsigned char* pSrc, int nLenSrc, unsigned int nKey, unsigned char* pDest, unsigned char* pTmpBuf)
{
	// 변환 테이블
	#include "LCCrypt_TransTable_Reverse"

	int nLenDest = nLenSrc - (LCCNM_BEGIN_SIZE + LCCNM_END_SIZE + LCCNM_CHECKSUM_SIZE + LCCNM_DUMMY_SIZE);
	unsigned short nCheckSum = 0;	// 체크섬
	unsigned char btKey[4];			// 키를 바이트로 분리
	int nIndexSrc = 0;				// pSrc 버퍼 인덱스
	unsigned char btTrans;			// 변환 테이블용

	//(*pDest) = NULL;

	if (nLenDest < 0)
		return -1;

	btKey[0] = LCCNM_BYTE_3(nKey);
	btKey[1] = LCCNM_BYTE_2(nKey);
	btKey[2] = LCCNM_BYTE_1(nKey);
	btKey[3] = LCCNM_BYTE_0(nKey);

	//pTmpBuf = new unsigned char[nLenSrc];

	// 데이터 복호화 : 더미 제외
	nIndexSrc = LCCNM_DUMMY_SIZE;
	while (nIndexSrc < nLenSrc)
	{
		// 현재 복호값을 저장
		btTrans = pSrc[nIndexSrc];

		// 왼쪽으로 Rotate
		btTrans = LCCNM_ROTATE_LEFT(btTrans);

		// 키와 XOR
		btTrans ^= btKey[nIndexSrc % 4];

		// 치환
		btTrans = LCCNM_RTransTable[btTrans];

		// 키변경
		btKey[nIndexSrc % 4] = pSrc[nIndexSrc];

		// 값 저장
		pTmpBuf[nIndexSrc] = btTrans;

		nIndexSrc++;
	}

	// 체크섬 검사 : 더미 제외
	nCheckSum = LCCNM_MAKEWORD(pTmpBuf[nLenSrc - 1], pTmpBuf[nLenSrc - 2]);
	if (nCheckSum != LCCNM_CalcCRC(pTmpBuf + LCCNM_DUMMY_SIZE, nLenSrc - LCCNM_CHECKSUM_SIZE - LCCNM_DUMMY_SIZE))
	{
		return -1;
	}

	// 시작마크 검사
	if (pTmpBuf[LCCNM_DUMMY_SIZE] != LCCNM_BEGIN_MARK)
	{
		return -1;
	}

	// 끝마크 검사
	if (pTmpBuf[nLenSrc - LCCNM_CHECKSUM_SIZE - 1] != LCCNM_END_MARK)
	{
		return -1;
	}

	//(*pDest) = new unsigned char[nLenDest];
	memcpy(pDest, pTmpBuf + LCCNM_BEGIN_SIZE + LCCNM_DUMMY_SIZE, sizeof(unsigned char) * nLenDest);

	return nLenDest;
}

#ifndef __PERSISTENTSYMBOLSENCODE_H__
#define __PERSISTENTSYMBOLSENCODE_H__

void EncodePersistentSymbols(unsigned char *buf, const unsigned char *bufSrc, int len)
{
	char *seed = ";!k.az\"MAEjhgasbube18340-fZ,;asAOJM.joqwAsefFsFjd";
	int seedLen = strlen(seed);
	for(int i=0; i<len; ++i)
	{
		int shift = (len - i) % 8 + 1;
		int val = bufSrc[i];
		val = val << shift;
		val = (val & 0x000000FF) | ((val & 0x0000FF00) >> 8);
		buf[i] = unsigned char(val) ^ seed[(len + 23*i) % seedLen];
	}
}

void DecodePersistentSymbols(unsigned char *buf, const unsigned char *bufSrc, int len)
{
	char *seed = ";!k.az\"MAEjhgasbube18340-fZ,;asAOJM.joqwAsefFsFjd";
	int seedLen = strlen(seed);
	for(int i=0; i<len; ++i)
	{
		int shift = 8 - ((len - i) % 8 + 1);
		int val = bufSrc[i] ^ seed[(len + 23*i) % seedLen];
		val = (val << 16) << shift;
		val = (val & 0x00FF0000) | ((val & 0xFF000000) >> 8);
		buf[i] = unsigned char(val >> 16);
	}
}
#endif //__PERSISTENTSYMBOLSENCODE_H__
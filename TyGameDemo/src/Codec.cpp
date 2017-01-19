#include<time.h> 
#include "Codec.h"

namespace tyGame{

#define SEED_MAX 128

	CCodec::CCodec()
	{
		m_cSeed = SEED_MAX;
	}

	CCodec::~CCodec()
	{
	}

	void CCodec::setSeed(char seed)
	{
		m_cSeed = seed;
	}

	void CCodec::resetSeed()
	{
		srand((unsigned)time(nullptr));
		m_cSeed = random(SEED_MAX);
	}

	int CCodec::getSeed()
	{
		return m_cSeed;
	}

	void CCodec::encoder(char* pBuffer)
	{
		char* pSrc = pBuffer;
		while (*pSrc != '\0')
		{
			*pSrc = (*pSrc) ^ m_cSeed;
			pSrc++;
		}
	}

	void CCodec::decoder(char* pBuffer)
	{
		char* pSrc = pBuffer;
		while (*pSrc != '\0')
		{
			*pSrc = (*pSrc) ^ m_cSeed;
			pSrc++;
		}
	}

}

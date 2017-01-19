/*
��Ϣ���ܺͽ���
*/
#pragma once
#include<stdlib.h>

namespace tyGame {
#define random(x) (rand() % x)
	class CCodec
	{
	private:
		char m_cSeed;

	public:
		CCodec();
		~CCodec();

		void setSeed(char seed);
		void resetSeed();
		int getSeed();

		void encoder(char* pBuffer);
		void decoder(char* pBuffer);
	};
}


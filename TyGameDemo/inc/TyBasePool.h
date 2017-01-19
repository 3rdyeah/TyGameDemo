/*
≥ÿ¿‡–Õƒ£∞Â
*/
#pragma once
#include <vector>
#include "Logger.h"

using std::vector;

namespace tyGame {
	template<typename pooltype>
	class TTyBasePool
	{
	private:
		int m_nPoolSizeMin;
		int m_nPoolSizeMax;
		int m_nUsedCountMax;
		vector<pooltype*> m_Pool;
	public:
		TTyBasePool() {};
		~TTyBasePool() {};

		void setPoolSizeMin(int nSize)
		{
			m_nPoolSizeMin = nSize;
		}
		int getPoolSizeMin()
		{
			return m_nPoolSizeMin;
		}

		void setPoolSizeMax(int nSize)
		{
			m_nPoolSizeMax = nSize;
		}
		int getPoolSizeMax()
		{
			return m_nPoolSizeMax;
		}

		void setUsedCountMax(int nCount)
		{
			m_nUsedCountMax = nCount;
		}
		int getUsedCountMax()
		{
			return m_nUsedCountMax;
		}
		
		vector<pooltype*>* getPool()
		{
			return &m_Pool;
		}

		CLogger* logger;
	};
}


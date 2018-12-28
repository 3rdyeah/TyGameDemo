/*
动态内存申请方法封装
*/
#pragma once

#include <memory>

using std::malloc;
using std::free;

static int g_nMallocCnt = 0;

template<typename T>
T* tyMalloc(T** pData, size_t nSize)
{
	*pData = (T*)malloc(nSize);
	memset(*pData, 0, nSize);
	g_nMallocCnt++;

	return *pData;
}

template<typename T>
void tyFree(T* pData)
{
	if (pData != NULL)
	{
		free(pData);
		pData = NULL;
		g_nMallocCnt--;
	}
}


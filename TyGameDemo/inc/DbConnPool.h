/*
���ݿ����ӳ�
*/
#pragma once
#include "TyBasePool.h"

namespace tyGame
{
	class CDbConnPool : public TTyBasePool<void>
	{
	public:
		CDbConnPool();
		~CDbConnPool();
	};
}

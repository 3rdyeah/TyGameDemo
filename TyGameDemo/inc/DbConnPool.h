/*
数据库连接池
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

/*
���ݿ����ӳ�
*/
#pragma once
#include "TyBasePool.h"

namespace tyGame
{
	class CDbConnPool : virtual public TTyBasePool<void>
	{
	public:
		CDbConnPool();
		~CDbConnPool();
	};
}

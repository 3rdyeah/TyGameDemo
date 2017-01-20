#pragma once

namespace tyGame {
	class CReactor
	{
	private:
		static CReactor* m_Inst;

		CReactor();
	public:
		~CReactor();
		 CReactor* getInstance();


	};
}


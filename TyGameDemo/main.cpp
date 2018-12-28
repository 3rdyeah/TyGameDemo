#include "Codec.h"
#include "Logger.h"
#include "Config.h"
#include "SocketMgr.h"
#include "DatabaseMgr.h"

using namespace tyGame;

int main(int argc, char const *argv[])
{
	//创建main函数logger
	CLogger* logger = new CLogger("main");
	if (!logger->InitDir())
	{
		logger->print(LOG_TYPE_ERROR, "init log file failed");
		return 1;
	}

	CConfig* config = CConfig::getInstance();
	config->init();

	CDatabaseMgr* dbMgr = CDatabaseMgr::getInstance();
	dbMgr->init();
	CMysqlConnPool* connPool = dbMgr->getInstance()->getConnPool();

	//test mysql begin
	char* sql = "select player_id, account_id, nickname, icon from player_info order by player_id limit %d";
	CDbConnection* pConn = connPool->getConnect();
	MYSQL* pMysql = &pConn->m_Conn;
	
	DbResult dbRes;
	pConn->excuteQuery(dbRes, sql, 5);
	pConn->excute("insert into test values(1, 'xiaoming')");

	connPool->releaseConnect(pConn);
	//test mysql end


	GAME_CONFIG_MAP cfgHello;
	config->analysisConfigFile("./etc/hello.txt", cfgHello);

	/*配置表读取测试*/
	const char* value1 = (char*)CFG_GETVALUE(cfgHello, "2", "hello");
	const char* value2 = (char*)CFG_GETVALUE(cfgHello, 2, "hello");

	char testvalue[] = "hello world"; //const_cast<char*>(value1);
	CCodec* coder = new CCodec();
	coder->resetSeed();
	coder->encoder(testvalue);
	coder->decoder(testvalue);

	CSocketMgr* socketMgr = CSocketMgr::getInstance();
	socketMgr->start();

	cfgHello.clear();
	delete socketMgr;
	delete config;
	delete logger;

	return 0;
}
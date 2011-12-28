/************************************************************************/
/* 寫入log機制的檔案，外面對它的操作主要透過3個macro MDebug MTrace MAssert                                                                     
/************************************************************************/
#ifndef _HISTORY_LOGGER_H_
#define _HISTORY_LOGGER_H_

#include <cassert>
#include <iostream>

#include <boost/date_time.hpp>
#include <boost/thread.hpp>

using namespace std;

class Logger{
public:
	~Logger();
	static Logger* getInstance();

	void log(string fileName,string funcName,int lineNumber);
	void log(string text,string fileName,string funcName,int lineNumber);
	void debug(string text,string fileName,string funcName,int lineNumber);
protected:
	Logger();
	static Logger* singleton;

	ofstream* afs;
	ofstream* dfs;
	boost::mutex fmtx;

	const static string ASSERTS;
	const static string MESSAGE;
	const static string EXCEPTS;
};

/** MAssert , 如果在debug模式下，直接assert ，如果在release模式下，則將此訊息寫入log中 */ 
#ifdef _DEBUG
#define MASSERT(x) if(!(x)){ assert(x);}
#else
#define MASSERT(x) if(!(x)){ Logger::getInstance()->log(__FILE__,__FUNCTION__,__LINE__);}
#endif

/** MTrack , 將指定訊息直接寫入log中(shell.txt)*/
#define MTRACK(x) Logger::getInstance()->log(x,__FILE__,__FUNCTION__,__LINE__);
/** MDebug , 將指定訊息直接寫入log中(debug.txt)，會和以上做區分是因為 shell.txt含的內容
    包山包海的，有時後只是想看單一訊息就透過mdebug來看*/
#define MDEBUG(x) Logger::getInstance()->debug(x,__FILE__,__FUNCTION__,__LINE__);

#endif

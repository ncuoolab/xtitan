#include "util/Logger.h"

#include "commonsetting/Config.h"

const string Logger::ASSERTS = "Asserts";
const string Logger::MESSAGE = "Message";

Logger* Logger::singleton = NULL;
Logger::Logger(){
	afs = new ofstream(string(Config::OUTPUTDIR+"/log/shell.txt").data());
	dfs = new ofstream(string(Config::OUTPUTDIR+"/log/debug.txt").data());
}

Logger* Logger::getInstance(){
	if(singleton == NULL)
		singleton = new Logger();
	return singleton;
}

Logger::~Logger(){
	delete afs;
	delete dfs;
}

void Logger::log(string fileName, string funcName, int lineNumber){
	boost::unique_lock<boost::mutex> lock(fmtx);
	boost::posix_time::ptime nowtime = boost::posix_time::microsec_clock::local_time();
	string timestream = boost::posix_time::to_simple_string(nowtime);

	(*afs) << timestream << " ["<<ASSERTS << "] : " << funcName.data()<<" - (" << lineNumber << ")\r\n";
	afs->flush();
}

void Logger::log(string text,string fileName,string funcName,int lineNumber){
	boost::unique_lock<boost::mutex> lock(fmtx);
	boost::posix_time::ptime nowtime = boost::posix_time::microsec_clock::local_time();
	string timestream = boost::posix_time::to_simple_string(nowtime);

	(*afs) << timestream << " ["<<MESSAGE << "] : " << funcName.data()<<" - (" << lineNumber << ") >> "<< text.data() << "\r\n";
	afs->flush();
}

void Logger::debug(string text,string fileName,string funcName,int lineNumber){
	boost::unique_lock<boost::mutex> lock(fmtx);
	boost::posix_time::ptime nowtime = boost::posix_time::microsec_clock::local_time();
	string timestream = boost::posix_time::to_simple_string(nowtime);

	(*dfs) << timestream << " ["<<ASSERTS << "] : " << funcName.data()<<" - (" << lineNumber << ") >> "<< text.data() << "\r\n";
	dfs->flush();
}

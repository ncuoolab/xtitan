#include "commonsetting/Config.h"
//boost header
#include <boost/algorithm/string.hpp>

Config* Config::singleton = NULL;

string Config::INPUTDIR ="";
string Config::OUTPUTDIR = "";

Config::Config(){
	this->parseConfigFile(Config::INPUTDIR+"/config/config.ini");	
}

Config::~Config(){
	singleton = NULL;
}

Config* Config::getInstance(){
	if(singleton == NULL)
		singleton = new Config();
	return singleton;
}

void Config::parseConfigFile(string fileName){
	locale loc = locale::global(locale(""));
	cfgFile.open(fileName.c_str());
	string buffer;
	
	while(getline(cfgFile,buffer)){
		vector<string> strVector;
		boost::algorithm::split(strVector,buffer,boost::algorithm::is_any_of("="));
		if(strVector.size() == 2){
			string key = strVector[0];
			string value = strVector[1];
			boost::algorithm::trim(key);
			boost::algorithm::trim(value);
			cfgMap[key] = value;
		}
	}
	cfgFile.close();
	locale::global(loc);
}

string Config::getProperty(const string key){
	return cfgMap[key];
}
void Config::setProperty(const string key,const string value){
	cfgMap[key] = value;
}


#ifndef _CONFIG_H_
#define _CONFIG_H_

//standard header
#include <iostream>
#include <map>
#include <vector>
#include <fstream>

#include "commonsetting/ConfigTag.h"

using namespace std;

class Config{
public:
	virtual ~Config();
	static Config* getInstance();

	static string INPUTDIR;
	static string OUTPUTDIR;

	string getProperty(const string key);
	void setProperty(const string key,const string value);
protected:
	Config();
	static Config* singleton;

	void parseConfigFile(string fileName);

	ifstream cfgFile;
	map<string,string> cfgMap;
};
#endif

#ifndef _CONFIG_TAG_H_
#define _CONFIG_TAG_H_

#include <iostream>

using namespace std;

class ConfigTag{
public:
	static const string ESN_PATH;
	static const string SOUND_PATH;
	static const string LOG_PATH;
	static const string HISTORY_PATH;

	static const string ESN_FILE;
	static const string AMITY_FILE;
	static const string CUSTOMIZE_FILE;
	static const string DEBUG_FILE;
	static const string STYLE_FILE;
	static const string COLOUR_FILE;

	static const string LOGIN_SERVER_ADDR;
	static const string PTC_SERVER_ADDR;
	static const string PTC_SERVER_PORT;
	static const string PTC_SERVER_TIMEOUT;
	static const string SBX_SERVER_TIMEOUT;

	static const string WEB_SERVER_PORT;

	static const string PIPE_NAME;

	static const string SENSOR_SENSITIVE;

	static const string LANGUAGE;

	static const string FPS;
};

#endif
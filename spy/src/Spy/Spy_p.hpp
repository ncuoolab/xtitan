#ifndef XTITAN_CLIENT_SPYMODULE_CONTROLLER_SPY_PRIVATE_HPP
#define XTITAN_CLIENT_SPYMODULE_CONTROLLER_SPY_PRIVATE_HPP

#include "Spy.hpp"

#include <memory>

#include <QtScript/QScriptEngine>


namespace xtitan {

class SpyModel;

class Spy::Private {
public:
	static std::shared_ptr< Spy > self;
	static void destory( Spy * );

	Private();

	//model
	std::shared_ptr< SpyModel > model;
	//engine
	std::shared_ptr< QScriptEngine > engine;
	bool isTestable;
	QString filename;
	QMap< QString, QObject * > tokens;
};

}

#endif

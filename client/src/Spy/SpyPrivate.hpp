#ifndef XTITAN_CLIENT_SPYMODULE_CONTROLLER_SPY_PRIVATE_HPP
#define XTITAN_CLIENT_SPYMODULE_CONTROLLER_SPY_PRIVATE_HPP

#include "xTitanClient/Spy/Spy.hpp"

#include <QtScript/QScriptEngine>

#include <memory>

namespace xtitan {
	namespace spy {

		class SpyModel;

		class Spy::Private {
		public:
			static std::shared_ptr< Spy > self;
			static void destory( Spy * );

			Private();

			void check( const QString & label, const QString & msg );
			void recordInput( const QString &, const QString &, const QStringList & );
			void recordInput( const QObject *, const QString &, const QString &, const QStringList & );
			void recordScript( const QString &, const QString & );

		public:
			//model 
			std::shared_ptr< SpyModel > model;
			//engine
			std::shared_ptr< QScriptEngine > engine;
			bool isTestable;
			QString filename;
			QMap< QString, QObject * > tokens;
		};

	}
}

#endif

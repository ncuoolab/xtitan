#ifndef XTITAN_CLIENT_SPYMODULE_CONTROLLER_SPY_HPP
#define XTITAN_CLIENT_SPYMODULE_CONTROLLER_SPY_HPP

#include "xTitanClient/Utility/Config.hpp"

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

#include <memory>

namespace xtitan {
	namespace spy {

		class XTITAN_CLIENT_DLL Spy : public QObject {
			Q_OBJECT
		public:
			static Spy & getInstance();	

			void activate();
			bool isTesting() const;
			void finish();

			Q_INVOKABLE void registerObject( QObject * obj );
			QObject * getObject( const QString & key ) const;

			Q_INVOKABLE void sendRawCommand( const QString & tag, const QVariant & data );

			Q_INVOKABLE void recordInput( const QString & objectName, const QString & id );
			Q_INVOKABLE void recordInput( const QString & objectName, const QString & id, const QVariant & a1 );
			Q_INVOKABLE void recordInput( const QString & objectName, const QString & id, const QVariant & a1, const QVariant & a2 );
			Q_INVOKABLE void recordInput( const QString & objectName, const QString & id, const QVariant & a1, const QVariant & a2, const QVariant & a3 );
			Q_INVOKABLE void recordInput( const QString & objectName, const QString & id, const QVariant & a1, const QVariant & a2, const QVariant & a3, const QVariant & a4 );
			Q_INVOKABLE void recordInput( const QString & objectName, const QString & id, const QVariant & a1, const QVariant & a2, const QVariant & a3, const QVariant & a4, const QVariant & a5 );
			Q_INVOKABLE void recordInput( const QString & objectName, const QString & id, const QVariant & a1, const QVariant & a2, const QVariant & a3, const QVariant & a4, const QVariant & a5, const QVariant & a6 );
			Q_INVOKABLE void recordInput( const QObject * object, const QString & id, const QString & method );
			Q_INVOKABLE void recordInput( const QObject * object, const QString & id, const QString & method, const QVariant & a1 );
			Q_INVOKABLE void recordInput( const QObject * object, const QString & id, const QString & method, const QVariant & a1, const QVariant & a2 );
			Q_INVOKABLE void recordInput( const QObject * object, const QString & id, const QString & method, const QVariant & a1, const QVariant & a2, const QVariant & a3 );
			Q_INVOKABLE void recordInput( const QObject * object, const QString & id, const QString & method, const QVariant & a1, const QVariant & a2, const QVariant & a3, const QVariant & a4 );
			Q_INVOKABLE void recordInput( const QObject * object, const QString & id, const QString & method, const QVariant & a1, const QVariant & a2, const QVariant & a3, const QVariant & a4, const QVariant & a5 );
			Q_INVOKABLE void recordInput( const QObject * object, const QString & id, const QString & method, const QVariant & a1, const QVariant & a2, const QVariant & a3, const QVariant & a4, const QVariant & a5, const QVariant & a6 );

			Q_INVOKABLE void encodeCheck( const QString & method, const QString & id, const QString & value );
			Q_INVOKABLE void encodeCheck( const QString & method, const QString & id, bool value );
			Q_INVOKABLE void encodeCheck( const QString & method, const QString & id, int value );
			Q_INVOKABLE void encodeCheck( const QString & method, const QString & id, double value );

			Q_INVOKABLE void executeScript( const QString & script );

		signals:
			void ready();

		private:
			Spy();
			virtual ~Spy();

			class Private;
			friend class Private;
			std::shared_ptr< Private > p_;
		};

		XTITAN_CLIENT_DLL void recordInput( const QString & objectName, const QString & id );
		XTITAN_CLIENT_DLL void recordInput( const QString & objectName, const QString & id, const QVariant & a1 );
		XTITAN_CLIENT_DLL void recordInput( const QString & objectName, const QString & id, const QVariant & a1, const QVariant & a2 );
		XTITAN_CLIENT_DLL void recordInput( const QString & objectName, const QString & id, const QVariant & a1, const QVariant & a2, const QVariant & a3 );
		XTITAN_CLIENT_DLL void recordInput( const QString & objectName, const QString & id, const QVariant & a1, const QVariant & a2, const QVariant & a3, const QVariant & a4 );
		XTITAN_CLIENT_DLL void recordInput( const QString & objectName, const QString & id, const QVariant & a1, const QVariant & a2, const QVariant & a3, const QVariant & a4, const QVariant & a5 );
		XTITAN_CLIENT_DLL void recordInput( const QString & objectName, const QString & id, const QVariant & a1, const QVariant & a2, const QVariant & a3, const QVariant & a4, const QVariant & a5, const QVariant & a6 );
		XTITAN_CLIENT_DLL void recordInput( const QObject * object, const QString & id, const QString & method );
		XTITAN_CLIENT_DLL void recordInput( const QObject * object, const QString & id, const QString & method, const QVariant & a1 );
		XTITAN_CLIENT_DLL void recordInput( const QObject * object, const QString & id, const QString & method, const QVariant & a1, const QVariant & a2 );
		XTITAN_CLIENT_DLL void recordInput( const QObject * object, const QString & id, const QString & method, const QVariant & a1, const QVariant & a2, const QVariant & a3 );
		XTITAN_CLIENT_DLL void recordInput( const QObject * object, const QString & id, const QString & method, const QVariant & a1, const QVariant & a2, const QVariant & a3, const QVariant & a4 );
		XTITAN_CLIENT_DLL void recordInput( const QObject * object, const QString & id, const QString & method, const QVariant & a1, const QVariant & a2, const QVariant & a3, const QVariant & a4, const QVariant & a5 );
		XTITAN_CLIENT_DLL void recordInput( const QObject * object, const QString & id, const QString & method, const QVariant & a1, const QVariant & a2, const QVariant & a3, const QVariant & a4, const QVariant & a5, const QVariant & a6 );
		XTITAN_CLIENT_DLL void encodeCheck( const QString & method, const QString & value );
		XTITAN_CLIENT_DLL void encodeCheck( const QString & method, bool value );
		XTITAN_CLIENT_DLL void encodeCheck( const QString & method, int value );
		XTITAN_CLIENT_DLL void encodeCheck( const QString & method, double value );
		XTITAN_CLIENT_DLL void encodeCheck( const QString & method, const QString & id, const QString & value );
		XTITAN_CLIENT_DLL void encodeCheck( const QString & method, const QString & id, bool value );
		XTITAN_CLIENT_DLL void encodeCheck( const QString & method, const QString & id, int value );
		XTITAN_CLIENT_DLL void encodeCheck( const QString & method, const QString & id, double value );

	}
}

#endif

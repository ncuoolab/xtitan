#ifndef XTITAN_CLIENT_SPYMODULE_CONTROLLER_SPY_HPP
#define XTITAN_CLIENT_SPYMODULE_CONTROLLER_SPY_HPP

#include "xTitanClient/Utility/Config.hpp"

#include <QtCore/QObject>
#include <QtCore/QStringList>

#include <memory>

namespace xtitan {
	namespace spy {

		class XTITAN_CLIENT_DLL Spy : public QObject {
			Q_OBJECT
		public:
			static Spy & getInstance();	

			void activate();
			bool isTesting();

			void registerObject( QObject * obj );
			QObject * getObject( const QString & key ) const;

			void recordInput( const QString & objectName, const QString & id );
			void recordInput( const QString & objectName, const QString & id, const QVariant & a1 );
			void recordInput( const QString & objectName, const QString & id, const QVariant & a1, const QVariant & a2 );
			void recordInput( const QString & objectName, const QString & id, const QVariant & a1, const QVariant & a2, const QVariant & a3 );
			void recordInput( const QString & objectName, const QString & id, const QVariant & a1, const QVariant & a2, const QVariant & a3, const QVariant & a4 );
			void recordInput( const QString & objectName, const QString & id, const QVariant & a1, const QVariant & a2, const QVariant & a3, const QVariant & a4, const QVariant & a5 );
			void recordInput( const QString & objectName, const QString & id, const QVariant & a1, const QVariant & a2, const QVariant & a3, const QVariant & a4, const QVariant & a5, const QVariant & a6 );
			void recordInput( const QObject * object, const QString & id, const QString & method );
			void recordInput( const QObject * object, const QString & id, const QString & method, const QVariant & a1 );
			void recordInput( const QObject * object, const QString & id, const QString & method, const QVariant & a1, const QVariant & a2 );
			void recordInput( const QObject * object, const QString & id, const QString & method, const QVariant & a1, const QVariant & a2, const QVariant & a3 );
			void recordInput( const QObject * object, const QString & id, const QString & method, const QVariant & a1, const QVariant & a2, const QVariant & a3, const QVariant & a4 );
			void recordInput( const QObject * object, const QString & id, const QString & method, const QVariant & a1, const QVariant & a2, const QVariant & a3, const QVariant & a4, const QVariant & a5 );
			void recordInput( const QObject * object, const QString & id, const QString & method, const QVariant & a1, const QVariant & a2, const QVariant & a3, const QVariant & a4, const QVariant & a5, const QVariant & a6 );

			void finish();

			void encodeCheck( const QString & method, const QString & value );
			void encodeCheck( const QString & method, bool value );
			void encodeCheck( const QString & method, int value );
			void encodeCheck( const QString & method, double value );
			void encodeCheck( const QString & method, const QString & id, const QString & value );
			void encodeCheck( const QString & method, const QString & id, bool value );
			void encodeCheck( const QString & method, const QString & id, int value );
			void encodeCheck( const QString & method, const QString & id, double value );

			void executeScript( const QString & script );

		signals:
			void ready();

		private:
			Spy();
			virtual ~Spy();

			class Private;
			friend class Private;
			std::shared_ptr< Private > p_;
		};

	}
}

#endif // QX_SPY_H

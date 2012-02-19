#ifndef XTITAN_CLIENT_SPY_SPYMODEL_HPP 
#define XTITAN_CLIENT_SPY_SPYMODEL_HPP

#include <QtCore/QObject>

namespace xtitan {
	namespace spy {

		class SpyModel : public QObject {
			Q_OBJECT
		public :
			explicit SpyModel();
			virtual ~SpyModel();

			void connectToHost( const QString & name );

			void stop();
			void check( const QString & label, const QString & value );
			void input( const QString & label, const QString & script, qint64 waitTime );
			void raw( const QString & tag, const QVariant & data );

		signals:
			void ready();

		private:
			class Private;
			Private * p_;
		};

	}
}

#endif

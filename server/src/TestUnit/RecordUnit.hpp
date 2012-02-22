#ifndef XTITAN_TESTUNIT_RECORDUNIT_HPP
#define XTITAN_TESTUNIT_RECORDUNIT_HPP

#include "AbstractUnit.hpp"

#include <memory>

namespace xtitan {
	namespace testunit {

		class RecordUnit : public AbstractUnit {
		public:
			explicit RecordUnit( QObject * parent );

		protected:
			virtual void doOpen();
			virtual void doTest();
			virtual void doClose();
			virtual void doCheck( testcase::CheckPoint * point );
			virtual void doInput( testcase::InputPoint * point );
			virtual network::SimpleSocket::Packet onCheck( int id, const QString & label, const QString & value );
			virtual network::SimpleSocket::Packet onInput( int id, const QString & label, const QString & script, qint64 timestamp );

		private:
			class Private;
			std::shared_ptr< Private > p_;
		};

	}
}

#endif

#ifndef XTITAN_TESTUNIT_UPDATEUNIT_HPP
#define XTITAN_TESTUNIT_UPDATEUNIT_HPP

#include "AbstractUnit.hpp"
#include "TestCase/TestCase.hpp"

namespace xtitan {
	namespace testunit {

		class UpdateUnit : public AbstractUnit {
		public:
			explicit UpdateUnit( QObject * parent );

		protected:
			virtual void doOpen();
			virtual void doTest();
			virtual void doClose();
			virtual void doCheck( testcase::CheckPoint * point );
			virtual void doInput( testcase::InputPoint * point );
			virtual network::SimpleSocket::Packet onCheck( int id, const QString & label, const QString & value );
			virtual network::SimpleSocket::Packet onInput( int id, const QString & label, const QString & script, qint64 waitTime );

		private:
			class Private;
			std::shared_ptr< Private > p_;
		};

	}
}

#endif

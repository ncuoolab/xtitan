#ifndef XTITAN_SERVER_TESTCASE_SOCKETPOINT_HPP
#define XTITAN_SERVER_TESTCASE_SOCKETPOINT_HPP

#include "AbstractPoint.hpp"

namespace xtitan {
	namespace testcase {

		class SocketPoint : public AbstractPoint {
		public:
			SocketPoint( int id, const QString & message, int line );

			const QString & getMessage() const;

			virtual QString toString() const;
			virtual void execute( testunit::AbstractUnit * unit );

		private:
			class Private;
			std::shared_ptr< Private > p_;
		};

	}
}

#endif

#ifndef XTITAN_SERVER_TESTCASE_SOCKETPOINT_PRIVATE_HPP
#define XTITAN_SERVER_TESTCASE_SOCKETPOINT_PRIVATE_HPP

#include "SocketPoint.hpp"

namespace xtitan {
	namespace testcase {

		class SocketPoint::Private {
		public:
			explicit Private( const QString & );

			QString message;
		};

	}
}

#endif

#ifndef XTITAN_TESTCASE_INPUTPOINT_PRIVATE_HPP
#define XTITAN_TESTCASE_INPUTPOINT_PRIVATE_HPP

#include "InputPoint.hpp"

#include <QtCore/QTimer>

namespace xtitan {
	namespace testcase {

		class InputPoint::Private {
		public:
			Private( const QString & script, qint64 waitTime );

			QString script;
			QTimer timer;
			qint64 waitTime;
		};

	}
}

#endif

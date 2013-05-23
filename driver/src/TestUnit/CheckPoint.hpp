#ifndef XTITAN_DRIVER_TESTUNIT_CHECKPOINT_HPP
#define XTITAN_DRIVER_TESTUNIT_CHECKPOINT_HPP

#include <QtCore/QStringList>


namespace xtitan {

struct CheckPoint {
	bool operator ==( const CheckPoint & that ) const;

	QString file;
	int line;
	QString id;
	QStringList args;
};

struct AsyncCheckPoint {
	bool operator ==( const AsyncCheckPoint & that ) const;

	QString file;
	int line;
	QString id;
	QString pre;
	QStringList args;
};

}

#endif

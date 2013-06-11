#ifndef XTITAN_DRIVER_TESTUNIT_CHECKPOINT_HPP
#define XTITAN_DRIVER_TESTUNIT_CHECKPOINT_HPP

#include <QtCore/QVariant>


namespace xtitan {

struct InputPoint {
	QString object;
	QString method;
	QVariantList args;
	qlonglong timestamp;
};

struct CheckPoint {
	bool operator ==( const CheckPoint & that ) const;

	QString file;
	int line;
	QString id;
	QVariantList args;
};

struct AsyncCheckPoint {
	bool operator ==( const AsyncCheckPoint & that ) const;

	QString file;
	int line;
	QString id;
	QString pre;
	QVariantList args;
};

}

#endif

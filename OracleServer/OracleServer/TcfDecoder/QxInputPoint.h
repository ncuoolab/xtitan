#ifndef QXINPUTPOINT_H
#define QXINPUTPOINT_H

#include "QxAbstractPoint.h"
#include <QtCore/QTimer>

class QxInputPoint : public QxAbstractPoint {
public:
	QxInputPoint( int id, const QString & label, const QString & script, qint64 waitTime, int line );

	const QString & getScript() const;
	qint64 getWaitTime() const;

	virtual QString toString() const;
	virtual void execute( QxAbstractUnit * unit );

private:
	QxInputPoint( const QxInputPoint & );
	QxInputPoint & operator =( const QxInputPoint & );

	QString script_;
	QTimer timer_;
	qint64 waitTime_;
};

#endif

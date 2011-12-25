#ifndef QXCHECKPOINT_H
#define QXCHECKPOINT_H

#include "QxAbstractPoint.h"

class QxCheckPoint : public QxAbstractPoint {
public:
	QxCheckPoint( int id, const QString & label, const QString & value, int line );

	const QString & getValue() const;
	void setChecked( bool checked );
	bool isChecked() const;

	virtual QString toString() const;
	virtual void execute( QxAbstractUnit * unit );

private:
	QString value_;
	bool checked_;
};

#endif

#ifndef QXRECORDUNIT_H
#define QXRECORDUNIT_H

#include "QxAbstractUnit.h"

class QxRecordUnit : public QxAbstractUnit {
public:
	explicit QxRecordUnit( QObject * parent );

protected:
	virtual void doOpen();
	virtual void doTest();
	virtual void doClose();
	virtual void doCheck( QxCheckPoint * point );
	virtual void doInput( QxInputPoint * point );
	virtual QxPacket onCheck( int id, const QString & label, const QString & value );
	virtual QxPacket onInput( int id, const QString & label, const QString & script, qint64 timestamp );

private:
	qint64 lastTick_;
	bool ready_;
};

#endif

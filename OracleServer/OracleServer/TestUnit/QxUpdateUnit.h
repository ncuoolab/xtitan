#ifndef QXUPDATEUNIT_H
#define QXUPDATEUNIT_H

#include "QxAbstractUnit.h"
#include "TcfDecoder/QxTestCase.h"

class QxUpdateUnit : public QxAbstractUnit {
public:
	explicit QxUpdateUnit( QObject * parent );

protected:
	virtual void doOpen();
	virtual void doTest();
	virtual void doClose();
	virtual void doCheck( QxCheckPoint * point );
	virtual void doInput( QxInputPoint * point );
	virtual QxPacket onCheck( int id, const QString & label, const QString & value );
	virtual QxPacket onInput( int id, const QString & label, const QString & script, qint64 waitTime );

private:
	QxTestCase inputCase_;
};

#endif

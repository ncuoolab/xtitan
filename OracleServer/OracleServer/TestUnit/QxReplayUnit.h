#ifndef QXREPLAYUNIT_H
#define QXREPLAYUNIT_H

#include "QxAbstractUnit.h"

#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QPair>

class QxReplayUnit : public QxAbstractUnit {
	Q_OBJECT
public:
	explicit QxReplayUnit( QObject * parent );

protected:
	virtual void doOpen();
	virtual void doTest();
	virtual void doClose();
	virtual void doCheck( QxCheckPoint * point );
	virtual void doInput( QxInputPoint * point );
	virtual QxPacket onCheck( int id, const QString & label, const QString & value );
	virtual QxPacket onInput( int id, const QString & label, const QString & script, qint64 waitTime );

signals:
	void releaseWaiting();

private slots:
	void onError_( const QString & );

private:
	QxCheckPoint * waitingPoint_;
	QList< QPair< QString, QString > > errorMessage_;
};

#endif

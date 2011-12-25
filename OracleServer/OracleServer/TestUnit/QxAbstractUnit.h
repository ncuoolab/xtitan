#ifndef QXABSTRACTUNIT_H
#define QXABSTRACTUNIT_H

#include "TCP_Connection/QxSimpleSocket.h"

#include <QtCore/QRunnable>

class QxCheckPoint;
class QxInputPoint;
class QxTestCase;

class QxAbstractUnit : public QObject, public QRunnable {
	Q_OBJECT

public:
	virtual ~QxAbstractUnit();

	void addClient( QxSimpleSocket * socket );
	const QString & getName() const;
	void setName( const QString & name );
	int getClients() const;
	void setClients( int nClients );
	void setCanceled( bool canceled );
	bool isCancled() const;
	const QxTestCase & getTestCase() const;
	void check( QxCheckPoint * point );
	void input( QxInputPoint * point );
	virtual void run();

protected:
	explicit QxAbstractUnit( QObject * parent );

	QxTestCase & testCase();
	void sendMessage( int id, const QString & command, const QVariant & data );

	virtual void doOpen() = 0;
	virtual void doClose() = 0;
	virtual void doTest() = 0;
	virtual void doCheck( QxCheckPoint * point ) = 0;
	virtual void doInput( QxInputPoint * point ) = 0;
	virtual QxPacket onCheck( int id, const QString & label, const QString & value ) = 0;
	virtual QxPacket onInput( int id, const QString & label, const QString & script, qint64 waitTime ) = 0;

signals:
	void log( const QString & type, const QString & message );
	void error( const QString & type, const QString & errorString );
	void canceled();
	void finished();

private:
	class Private;
	Private * p_;
};

#endif

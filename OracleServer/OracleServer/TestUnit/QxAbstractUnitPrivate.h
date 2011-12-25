#ifndef QXABSTRACTUNITPRIVATE_H
#define QXABSTRACTUNITPRIVATE_H

#include "QxAbstractUnit.h"
#include "TcfDecoder/QxTestCase.h"

#include <QtCore/QProcess>

#include <boost/tr1/functional.hpp>

class QxAbstractUnit::Private : public QObject {
	Q_OBJECT

public:
	typedef std::map< QString, std::tr1::function< QxPacket ( QxSimpleSocket *, const QVariant & ) > > CommandMap;

	explicit Private( QxAbstractUnit * parent );

public slots:
	void onSocketReadyRead();
	void onSocketError( QLocalSocket::LocalSocketError socketError );
	void onSocketDisconnected();

signals:
	void readyForTest();
	void log( const QString &, const QString & );
	void error( const QString &, const QString & );

public:
	void addClient( QxSimpleSocket * socket );
	void spawnChiwawa();
	void restartServer();

	QxPacket onCheck( QxSimpleSocket * sender, const QVariant & data );
	QxPacket onInput( QxSimpleSocket * sender, const QVariant & data );
	QxPacket onJoin( QxSimpleSocket * sender, const QVariant & data );

	QxAbstractUnit * parent_;
	QList< QxSimpleSocket * > sockets;
	QList< QProcess * > children_;
	QxTestCase testCase;
	CommandMap commands;
	bool canceled;
};

#endif

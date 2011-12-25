#ifndef TEST_CLIENT_H
#define TEST_CLIENT_H

#include <QString>
#include <QtNetwork/QLocalSocket>

#include <boost/tr1/functional.hpp>

class QxTestUnit;
class QxSimpleSocket;
class QxInputPoint;

class TestClient : public QObject
{
	Q_OBJECT
public:
	TestClient( QxSimpleSocket * socket );
	virtual ~TestClient();

	void setTestUnit( QxTestUnit * u );
	void disconnectFromHost();

	void setID( int id );
	int getID() const;
	QString getIP() const;
	QxTestUnit* getTestUnit() const;

	void AckNextInput( const QxInputPoint & input ) const;
	void AckQuit();

signals:
	void disconnected();
	void error( const QString & );

private slots:
	void onMessageReceived_( const QString &, const QVariant & );
	void onSocketError_( QLocalSocket::LocalSocketError );
	
private:
	typedef std::tr1::function< void ( const QVariant & ) > Command;
	typedef QMap< QString, Command > CommandTable;

	void doReqJoinUnit_( const QVariant & );

	void doCheckCmd_( const QVariant & );
	void doInputCmd_( const QVariant & );

	QxTestUnit * unit_;
	QxSimpleSocket * socket_;
	CommandTable commands_;
	int id_;
	bool closed_;
};

#endif	//_TEST_CLIENT_H_

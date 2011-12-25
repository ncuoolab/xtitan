#ifndef QX_SPY_SOCKET_H
#define QX_SPY_SOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QMutex>

class QxSpySocket :public QObject
{
	Q_OBJECT
public:
	QxSpySocket(QString ip, int port , QObject *parent = 0);
	QxSpySocket(QTcpSocket* sck);
	~QxSpySocket();

	
	void sendMsg(QString msg);

	void SetConnect(QString ip, int port );
	void Connect();

	bool isConnect(){	return _isConnect;	}
	QString getIP(){	return _ip;			}
	int getPort(){		return _port;		}

	QTcpSocket* getSocket(){	return connection;	}

signals:
	void sig_ConnectionUpdate(QString);
	void sig_getMsg(QString);

public slots:
	void slot_connectOK();
	void slot_readMsg();
	void slot_displayError(QAbstractSocket::SocketError socketError);

private:
	QTcpSocket* connection;
	QString cache;
	QString _ip;
	int _port;

	QDataStream readStream;
	quint16 blockSize;
	bool _isConnect;

	QMutex mutex;
};

#endif // QX_SPY_SOCKET_H

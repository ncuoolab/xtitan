#ifndef QX_AUTO_SPY_MODEL_H 
#define QX_AUTO_SPY_MODEL_H

#include "QxTesting/SpyModule/Model/QxSpyModel.h"
#include "QxTesting/TCPConnection/QxTcpMessageComposer.h"
#include "QxTesting/TcpConnection/QxSimpleSocket.h"

class QxAutoSpyModel : public QxSpyModel {
	Q_OBJECT
public :
	QxAutoSpyModel( QScriptEngine * eng );
	virtual ~QxAutoSpyModel();

	void connectToHost( const QString & name );

	virtual void Stop();
	virtual void check( const QString & label, const QString & value );
	virtual void input( const QString & label, const QString & script, qint64 waitTime );

private slots:
	void onConnected_();
	void onReadyRead_();

private:
	void send_( const QxPacket & );
	QxSimpleSocket * socket_;
};

#endif	//QX_AUTO_SPY_MODEL_H

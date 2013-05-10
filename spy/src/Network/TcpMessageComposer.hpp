#ifndef QX_TCPMESSAGE_COMPOSER_H
#define QX_TCPMESSAGE_COMPOSER_H

#include <QtCore/QString>
#include <QtCore/QPair>
#include <QtCore/QVariant>


typedef QPair< QString, QVariant > QxPacket;

/*
This is class is about to encode the tcp message which is sent to the oracle server.
*/
class TcpMessageComposer {
public:
	static QxPacket Check( const QString & label, const QString & value );

	static QxPacket Input( const QString & object, const QString & method, const QStringList & args, qint64 timeStamp );
};

#endif	//QX_TCPMESSAGE_COMPOSER_H

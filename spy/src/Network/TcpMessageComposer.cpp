#include "Network/TcpMessageComposer.hpp"

#include "Network/TCPMessage.hpp"

#include <QtCore/QRegExp>


QxPacket TcpMessageComposer::Check( const QString & label, const QString & value ) {
	QVariantMap data;
	data.insert( "label", label );
	data.insert( "value", value );
	return QxPacket( TCPMessage::Check, data );
}

QxPacket TcpMessageComposer::Input( const QString & object, const QString & method, const QStringList & args, qint64 timeStamp ) {
	QVariantMap data;
	data.insert( "object", object );
	data.insert( "method", method );
	data.insert( "args", args );
	data.insert( "timestamp", timeStamp );
	return QxPacket( TCPMessage::Input, data );
}

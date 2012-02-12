#include "Network/TcpMessageComposer.hpp"
#include "Network/TCPMessage.hpp"
#include <QtCore/QRegExp>

QxPacket TcpMessageComposer::Check( const QString & label, const QString & value ) {
	QString sValue( value );
	sValue.remove( QRegExp( "[\r\n]" ) );
	QVariantMap data;
	data.insert( "label", label );
	data.insert( "value", sValue );
	return QxPacket( TCPMessage::Check, data );
}

QxPacket TcpMessageComposer::Input( const QString & label, const QString & script, qint64 waitTime ) {
	QVariantMap data;
	data.insert( "label", label );
	data.insert( "script", script );
	data.insert( "waitTime", waitTime );
	return QxPacket( TCPMessage::Input, data );
}

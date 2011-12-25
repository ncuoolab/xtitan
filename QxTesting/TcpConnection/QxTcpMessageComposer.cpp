#include "QxTesting/TCPConnection/QxTcpMessageComposer.h"
#include "QxTesting/TCPConnection/QxTCPMessage.h"
#include <QtCore/QRegExp>

QxPacket QxTcpMessageComposer::JoinUnit( const QString & name ) {
	return QxPacket( QxTCPMessage::ReqJoinUnit, name );
}

QxPacket QxTcpMessageComposer::Check( const QString & label, const QString & value ) {
	QString sValue( value );
	sValue.remove( QRegExp( "[\r\n]" ) );
	QVariantMap data;
	data.insert( "label", label );
	data.insert( "value", sValue );
	return QxPacket( QxTCPMessage::Check, data );
}

QxPacket QxTcpMessageComposer::Input( const QString & label, const QString & script, qint64 waitTime ) {
	QVariantMap data;
	data.insert( "label", label );
	data.insert( "script", script );
	data.insert( "waitTime", waitTime );
	return QxPacket( QxTCPMessage::Input, data );
}

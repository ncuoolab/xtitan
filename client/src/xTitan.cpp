#include "xTitanClient/xTitan.hpp"
#include "xTitan/Utility/Setting.hpp"
#include "xTitan/Utility/Logger.hpp"
#include "xTitanClient/Command/MessageEncoder.hpp"
#include "Network/TCPMessage.hpp"

#include <QtCore/QRegExp>

void xRawCommand( const QString & tag, const QVariant & data ) {
	QMetaObject::invokeMethod( &xtitan::spy::Spy::getInstance(), "sendRawCommand", Q_ARG( const QString &, tag ), Q_ARG( const QVariant &, data ) );
}

void xRegisterObject( QObject * object, const QString & id /* = QString */ ) {
	using namespace xtitan::spy;
	if( object->objectName().isEmpty() && id.isEmpty() ) {
		return;
	}

//	MASSERT( !object->objectName().isEmpty() || !id.isEmpty() || !"OBJECT NAME IS EMPTY" );
	object->setObjectName( object->objectName() + id );
	QMetaObject::invokeMethod( &Spy::getInstance(), "registerObject", Q_ARG( QObject *, object ) );
}

QObject * xGetObject( const QString & token ) {
	using namespace xtitan::spy;
	return Spy::getInstance().getObject( token );
}

/**
 * @note Please call this function in the main thread.
 */
void xTryTestAutomation() {
	// initialize setting
	xtitan::utilities::Setting::getInstance();

	// initialize logger
	xtitan::utilities::Logger::getInstance();

	// initialize message encoder
	xtitan::command::MessageEncoder::getInstance().registerEncoder( "check", []( const QVariant & data )->xtitan::network::SimpleSocket::Packet {
		QVariantMap args( data.toMap() );
		QString sValue( args["value"].toString() );
		sValue.remove( QRegExp( "[#\r\n]" ) );
		args["value"] = sValue;
		return xtitan::network::SimpleSocket::Packet( TCPMessage::Check, args );
	} );
	xtitan::command::MessageEncoder::getInstance().registerEncoder( "input", []( const QVariant & data )->xtitan::network::SimpleSocket::Packet {
		QVariantMap args( data.toMap() );
		QString sScript( args["script"].toString() );
		sScript.remove( QRegExp( "[#\r\n]" ) );
		args["script"] = sScript;
		return xtitan::network::SimpleSocket::Packet( TCPMessage::Input, args );
	} );

	// initialize spy
	xtitan::spy::Spy::getInstance().activate();
}

bool xIsTesting() {
	using namespace xtitan::spy;
	return Spy::getInstance().isTesting();
}

QString xMakeSafeName( const QString & objName ) {
	QString sName( objName );
	return sName.replace( QRegExp( "[^a-zA-Z0-9_]" ), "" );
}

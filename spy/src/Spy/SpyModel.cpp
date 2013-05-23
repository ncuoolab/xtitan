#include "Spy/SpyModel_p.hpp"

#include "xTitan/Exception/NetworkError.hpp"
#include "Spy.hpp"

namespace {

xtitan::SimpleSocket::Packet makeAsyncCheck( const QString & file, int line, const QString & id, const QString & pre, const QStringList & args ) {
	QVariantMap data;
	data.insert( "file", file );
	data.insert( "line", line );
	data.insert( "id", id );
	data.insert( "pre", pre );
	data.insert( "args", args );
	return xtitan::SimpleSocket::Packet( "<AsyncCheck>", data );
}

xtitan::SimpleSocket::Packet makeCheck( const QString & file, int line, const QString & id, const QStringList & args ) {
	QVariantMap data;
	data.insert( "file", file );
	data.insert( "line", line );
	data.insert( "id", id );
	data.insert( "args", args );
	return xtitan::SimpleSocket::Packet( "<Check>", data );
}

xtitan::SimpleSocket::Packet makeInput( const QString & object, const QString & method, const QStringList & args, qint64 timeStamp ) {
	QVariantMap data;
	data.insert( "object", object );
	data.insert( "method", method );
	data.insert( "args", args );
	data.insert( "timestamp", timeStamp );
	return xtitan::SimpleSocket::Packet( "<Input>", data );
}

}


using xtitan::SpyModel;
using xtitan::SimpleSocket;


SpyModel::Private::Private( SpyModel * host ):
QObject( host ),
socket( new SimpleSocket( this ) ),
commands() {
	this->commands.insert( std::make_pair( "<Input>", [this]( const QVariant & data )->void {
		auto kwargs = data.toMap();
		auto object = kwargs.value( "object" ).toString();
		auto method = kwargs.value( "method" ).toString();
		auto args = kwargs.value( "args" ).toList();

		QStringList sArgs;
		for( auto it = args.begin(); it != args.end(); ++it ) {
			if( it->type() == QVariant::Bool ) {
				auto b = it->toBool();
				sArgs.append( b ? "true" : "false" );
			} else if( it->type() == QVariant::Int ) {
				auto i = it->toInt();
				sArgs.append( QString::number( i ) );
			} else if( it->type() == QVariant::Double ) {
				auto d = it->toDouble();
				sArgs.append( QString::number( d ) );
			} else if( it->type() == QVariant::String ) {
				auto s = it->toString();
				sArgs.append( QString( "\'%1\'" ).arg( s ) );
			} else {
				throw NetworkError( QObject::tr( "unknown data type: %1" ).arg( it->typeName() ) );
			}
		}

		auto script = QString( "%1.%2(%3)" ).arg( object ).arg( method ).arg( sArgs.join( "," ) );

		emit this->scriptReceived( script );
	} ) );

	this->connect( this->socket, SIGNAL( connected() ), SLOT( onConnected() ) );
	this->connect( this->socket, SIGNAL( readyRead() ), SLOT( onReadyRead() ) );
}

void SpyModel::Private::send( const SimpleSocket::Packet & pkt ) {
	if( this->socket->state() != QLocalSocket::ConnectedState ) {
		throw NetworkError( "disconnected from server, send failed" );
	}
	this->socket->write( pkt.first, pkt.second );
}

void SpyModel::Private::onConnected() {
	emit this->ready();
}

void SpyModel::Private::onReadyRead() {
	while( !this->socket->atEnd() ) {
		auto packet = this->socket->read();
		auto it = this->commands.find( packet.first );
		if( it == this->commands.end() ) {
			// TODO report error
			return;
		}
		it->second( packet.second );
	}
}

SpyModel::SpyModel():
QObject(),
p_( new Private( this ) ) {
	this->connect( this->p_, SIGNAL( ready() ), SIGNAL( ready() ) );
	this->connect( this->p_, SIGNAL( scriptReceived( const QString & ) ), SIGNAL( scriptReceived( const QString & ) ) );
}

SpyModel::~SpyModel(){
	this->p_->socket->disconnectFromServer();
}

void SpyModel::asyncCheck( const QString & file, int line, const QString & id, const QString & pre, const QStringList & args ) {
	auto msg = makeAsyncCheck( file, line, id, pre, args );
	this->p_->socket->write( msg.first, msg.second );
}

void SpyModel::connectToHost( const QString & name ) {
	this->p_->socket->connectToServer( name );
}

void SpyModel::check( const QString & file, int line, const QString & id, const QStringList & args ) {
	auto msg = makeCheck( file, line, id, args );
	this->p_->socket->write( msg.first, msg.second );
}

void SpyModel::input( const QString & object, const QString & method, const QStringList & args, qint64 timeStamp ) {
	if( this->p_->socket->state() == QLocalSocket::ConnectedState ) {
		this->p_->send( makeInput( object, method, args, timeStamp ) );
	}
}

void SpyModel::stop() {
	this->p_->socket->disconnectFromServer();
}

#include "MessageEncoderPrivate.hpp"

#include <cassert>

using namespace xtitan::command;
using xtitan::network::SimpleSocket;

std::shared_ptr< MessageEncoder > MessageEncoder::Private::self;

void MessageEncoder::Private::destory( MessageEncoder * self ) {
	delete self;
}

MessageEncoder::Private::Private():
lock(),
encoders() {
}

/**
 * @warning This function is NOT thread-safe
 */
MessageEncoder & MessageEncoder::getInstance() {
	if( !Private::self ) {
		Private::self.reset( new MessageEncoder, Private::destory );
	}
	return *Private::self;
}

MessageEncoder::MessageEncoder():
p_( new Private ) {
}

MessageEncoder::~MessageEncoder() {
}

bool MessageEncoder::registerEncoder( const QString & tag, const Encoder & encoder ) {
	QWriteLocker locker( &this->p_->lock );
	std::map< QString, Encoder >::const_iterator it = this->p_->encoders.find( tag );
	if( it != this->p_->encoders.end() ) {
		return false;
	}
	this->p_->encoders.insert( std::make_pair( tag, encoder ) );
	return true;
}

SimpleSocket::Packet MessageEncoder::encode( const QString & tag, const QVariant & data ) const {
	QReadLocker locker( &this->p_->lock );
	std::map< QString, Encoder >::const_iterator it = this->p_->encoders.find( tag );
	if( it != this->p_->encoders.end() ) {
		// TODO exception class
		assert( !"undefined message tag" );
	}
	return it->second( data );
}

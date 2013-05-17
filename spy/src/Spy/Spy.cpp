#include "Spy/Spy_p.hpp"

#include <QtCore/QDateTime>
#include <QtNetwork/QHostAddress>

#include "xTitan/Utility/Logger.hpp"
#include "Spy/SpyModel.hpp"
//#include "Util/SignalSpy/Q4puGenericSignalSpy.hpp"


using xtitan::Spy;
using xtitan::SpyModel;


std::shared_ptr< Spy > Spy::Private::self;

void Spy::Private::destory( Spy * host ) {
	delete host;
}

Spy::Private::Private():
model(),
engine( new QScriptEngine ),
isTestable( false ),
filename(),
tokens() {
}

/**
 * @warning This function is NOT thread safe!
 */
Spy & Spy::instance() {
	if( !Private::self ) {
		Private::self.reset( new Spy, Private::destory );
	}
	return *Private::self;
}

Spy::Spy():
QObject(),
p_( new Private ) {
}

Spy::~Spy(){
}

void Spy::activate() {
	this->p_->isTestable = true;

	// TODO spawn from a factory, for user library
	SpyModel * m = new SpyModel;
	this->connect( m, SIGNAL( ready() ), SIGNAL( ready() ) );
	m->connectToHost( "OracleServer" );

	this->p_->model.reset( m );
}

bool Spy::isTesting() const {
	return this->p_->isTestable;
}

void Spy::encodeCheck( const QString & id, const QString & value ) {
	if( !this->p_->isTestable ) {
		return;
	}

	if( this->p_->model ){
		this->p_->model->check( id, value );
	}
}

void Spy::registerObject( QObject * obj ){
	if( !this->p_->isTestable ) {
		return;
	}

	QString objName = obj->objectName();
	QRegExp exp( "[^a-zA-Z0-9_]" );
	objName = objName.replace( exp, "" );
	obj->setObjectName( objName );
	QScriptValue scriptObj;
	scriptObj = this->p_->engine->newQObject( obj );
	this->p_->engine->globalObject().setProperty( objName , scriptObj);
	this->p_->tokens.insert( objName, obj );
}

QObject * Spy::getObject( const QString & key ) const {
	QMap< QString, QObject * >::const_iterator it = this->p_->tokens.find( key );
	if( it == this->p_->tokens.end() ) {
		return NULL;
	} else {
		return it.value();
	}
}

void Spy::recordInput( const QString & objectName, const QString & methodName, const QStringList & args ) {
	if( !this->p_->isTestable ) {
		return;
	}

	if( this->p_->model ) {
		this->p_->model->input( objectName, methodName, args, QDateTime::currentMSecsSinceEpoch() );
	}
}

void Spy::executeScript( const QString & script ) {
	if( this->p_->engine->canEvaluate( script ) ){
		this->p_->engine->evaluate( script );
	} else {
		this->p_->engine->clearExceptions();
		// TODO send warning
	}
}

void Spy::finish(){
	if( this->p_->model ) {
		this->p_->model->deleteLater();
		this->p_->model = nullptr;
	}
}

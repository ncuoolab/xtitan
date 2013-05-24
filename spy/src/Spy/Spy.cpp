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

void Spy::Private::executeScript( const QString & script ) {
	if( this->engine->canEvaluate( script ) ){
		this->engine->evaluate( script );
	} else {
		this->engine->clearExceptions();
		// TODO send warning
	}
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
	this->p_->connect( m, SIGNAL( scriptReceived( const QString & ) ), SLOT( executeScript( const QString & ) ) );
	this->connect( m, SIGNAL( ready() ), SIGNAL( ready() ) );
	m->connectToHost( "OracleServer" );

	this->p_->model.reset( m );
}

bool Spy::isTesting() const {
	return this->p_->isTestable;
}

void Spy::encodeCheck( const QString & file, int line, const QString & id, const QStringList & args ) {
	if( !this->p_->isTestable ) {
		return;
	}

	if( this->p_->model ){
		this->p_->model->check( file, line, id, args );
	}
}

void Spy::encodeAsyncCheck( const QString & file, int line, const QString & id, const QString pre, const QStringList & args ) {
	if( !this->p_->isTestable ) {
		return;
	}

	if( this->p_->model ){
		this->p_->model->asyncCheck( file, line, id, pre, args );
	}
}

void Spy::registerObject( QObject * object, const QString & id ){
	if( !this->p_->isTestable ) {
		return;
	}

	QString objName = id;
	QRegExp exp( "[^a-zA-Z0-9_\'\"]" );
	objName = objName.replace( exp, "" );
	QScriptValue scriptObj;
	scriptObj = this->p_->engine->newQObject( object );
	this->p_->engine->globalObject().setProperty( objName , scriptObj );
	this->p_->tokens.insert( objName, object );
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

void Spy::finish() {
	if( this->p_->model ) {
		this->p_->model->deleteLater();
		this->p_->model = nullptr;
	}
}

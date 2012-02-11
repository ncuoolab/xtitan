#include "Spy/SpyPrivate.hpp"
#include "Spy/SpyModel.hpp"
//#include "Util/SignalSpy/Q4puGenericSignalSpy.hpp"
#include "CommandParser/CommandParser.hpp"

#include "xTitan/Utility/Logger.hpp"

#include <QtNetwork/QHostAddress>
#include <QtCore/QDateTime>

#include <cassert>

using namespace xtitan::spy;

namespace {

	QString fromValue( const QVariant & value ) {
		switch( value.type() ) {
		case QVariant::Bool:
			return ( value.toBool() ? "true" : "false" );
		case QVariant::Int:
			return QString::number( value.toInt() );
		case QVariant::Double:
			return QString::number( value.toDouble() );
		case QVariant::String:
			return QString( "\'%1\'" ).arg( value.toString().replace( "\\", "\\\\" ).replace( "\'", "\\\'" ) );
		default:
			assert( !"Unknown Arguments Type" );
			return QString();
		}
	}

	QString toScript( const QString & objectName, const QString & signature, const QStringList & args ) {
		QString methodName = signature.section( "::", -1 );
		return QString( "%1.%2(%3)" ).arg( objectName ).arg( methodName ).arg( args.join( "," ) );
	}

}

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

void Spy::Private::check( const QString & label, const QString & msg ) {
	if( this->model ){
		this->model->check( label, msg );
	}
}

void Spy::Private::recordInput( const QObject * object, const QString & id, const QString & method, const QStringList & args ) {
	this->recordScript( id, QString( "%1.%2(%3)" ).arg( object->objectName() ).arg( method ).arg( args.join( "," ) ) );
}

void Spy::Private::recordInput( const QString & objectName, const QString & id, const QStringList & args ) {
	this->recordScript( id, toScript( objectName, id, args ) );
}

void Spy::Private::recordScript( const QString & id, const QString & script ) {
	if( !this->isTestable ) {
		return;
	}

	if( this->model ) {
		this->model->input( id, script, QDateTime::currentMSecsSinceEpoch() );
	}
}

/**
 * @warning This function is NOT thread safe!
 */
Spy & Spy::getInstance() {
	if( !Private::self ) {
		Private::self.reset( new Spy, Private::destory );
	}
	return *Private::self;
}

Spy::Spy():
QObject(),
p_( new Private ) {
	// NOTE: this connection **MUST** be Qt::QueuedConnection, or you will suffer shit of threads
//	this->connect( this, SIGNAL( scriptProxy_( const QString & ) ), SLOT( executeScript_( const QString & ) ), Qt::QueuedConnection );
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

void Spy::encodeCheck( const QString & method, const QString & value ){
	this->encodeCheck( method, method, value );
}

void Spy::encodeCheck( const QString & method, bool value ){
	this->encodeCheck( method, method, value );
}

void Spy::encodeCheck( const QString & method, int value ){
	this->encodeCheck( method, method, value );
}

void Spy::encodeCheck( const QString & method, double value ){
	this->encodeCheck( method, method, value );
}

void Spy::encodeCheck( const QString & method, const QString & id, const QString & value ){
	if( !this->p_->isTestable ) {
		return;
	}

	QString label = method + "_" + id;
	QString msg = value;
	this->p_->check( label, msg );
}

void Spy::encodeCheck( const QString & method, const QString & id, bool expr ){
	if( !this->p_->isTestable ) {
		return;
	}

	QString label = method + "_" + id;
	QString msg = expr ? "true" : "false";
	this->p_->check( label, msg );
}

void Spy::encodeCheck( const QString & method, const QString & id, int value ){
	if( !this->p_->isTestable ) {
		return;
	}

	QString lable = method + "_" + id;
	QString msg = QString::number( value );
	this->p_->check( lable, msg );
}

void Spy::encodeCheck( const QString & method, const QString & id, double value ){
	if( !this->p_->isTestable ) {
		return;
	}

	QString label = method + "_" + id;
	QString msg = QString::number( value );
	this->p_->check( label, msg );
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

void Spy::recordInput( const QString & objectName, const QString & id ) {
	this->p_->recordInput( objectName, id, QStringList() );
}

void Spy::recordInput( const QString & objectName, const QString & id, const QVariant & v1 ) {
	QStringList args;
	args.append( fromValue( v1 ) );
	this->p_->recordInput( objectName, id, args );
}

void Spy::recordInput( const QString & objectName, const QString & id, const QVariant & v1, const QVariant & v2 ) {
	QStringList args;
	args.append( fromValue( v1 ) );
	args.append( fromValue( v2 ) );
	this->p_->recordInput( objectName, id, args );
}

void Spy::recordInput( const QString & objectName, const QString & id, const QVariant & v1, const QVariant & v2, const QVariant & v3 ) {
	QStringList args;
	args.append( fromValue( v1 ) );
	args.append( fromValue( v2 ) );
	args.append( fromValue( v3 ) );
	this->p_->recordInput( objectName, id, args );
}

void Spy::recordInput( const QString & objectName, const QString & id, const QVariant & v1, const QVariant & v2, const QVariant & v3, const QVariant & v4 ) {
	QStringList args;
	args.append( fromValue( v1 ) );
	args.append( fromValue( v2 ) );
	args.append( fromValue( v3 ) );
	args.append( fromValue( v4 ) );
	this->p_->recordInput( objectName, id, args );
}

void Spy::recordInput( const QString & objectName, const QString & id, const QVariant & v1, const QVariant & v2, const QVariant & v3, const QVariant & v4, const QVariant & v5 ) {
	QStringList args;
	args.append( fromValue( v1 ) );
	args.append( fromValue( v2 ) );
	args.append( fromValue( v3 ) );
	args.append( fromValue( v4 ) );
	args.append( fromValue( v5 ) );
	this->p_->recordInput( objectName, id, args );
}

void Spy::recordInput( const QString & objectName, const QString & id, const QVariant & v1, const QVariant & v2, const QVariant & v3, const QVariant & v4, const QVariant & v5, const QVariant & v6 ) {
	QStringList args;
	args.append( fromValue( v1 ) );
	args.append( fromValue( v2 ) );
	args.append( fromValue( v3 ) );
	args.append( fromValue( v4 ) );
	args.append( fromValue( v5 ) );
	args.append( fromValue( v6 ) );
	this->p_->recordInput( objectName, id, args );
}

void Spy::recordInput( const QObject * object, const QString & id, const QString & method ) {
	this->p_->recordInput( object, id, method, QStringList() );
}

void Spy::recordInput( const QObject * object, const QString & id, const QString & method, const QVariant & v1 ) {
	QStringList args;
	args.append( fromValue( v1 ) );
	this->p_->recordInput( object, id, method, args );
}

void Spy::recordInput( const QObject * object, const QString & id, const QString & method, const QVariant & v1, const QVariant & v2 ) {
	QStringList args;
	args.append( fromValue( v1 ) );
	args.append( fromValue( v2 ) );
	this->p_->recordInput( object, id, method, args );
}

void Spy::recordInput( const QObject * object, const QString & id, const QString & method, const QVariant & v1, const QVariant & v2, const QVariant & v3 ) {
	QStringList args;
	args.append( fromValue( v1 ) );
	args.append( fromValue( v2 ) );
	args.append( fromValue( v3 ) );
	this->p_->recordInput( object, id, method, args );
}

void Spy::recordInput( const QObject * object, const QString & id, const QString & method, const QVariant & v1, const QVariant & v2, const QVariant & v3, const QVariant & v4 ) {
	QStringList args;
	args.append( fromValue( v1 ) );
	args.append( fromValue( v2 ) );
	args.append( fromValue( v3 ) );
	args.append( fromValue( v4 ) );
	this->p_->recordInput( object, id, method, args );
}

void Spy::recordInput( const QObject * object, const QString & id, const QString & method, const QVariant & v1, const QVariant & v2, const QVariant & v3, const QVariant & v4, const QVariant & v5 ) {
	QStringList args;
	args.append( fromValue( v1 ) );
	args.append( fromValue( v2 ) );
	args.append( fromValue( v3 ) );
	args.append( fromValue( v4 ) );
	args.append( fromValue( v5 ) );
	this->p_->recordInput( object, id, method, args );
}

void Spy::recordInput( const QObject * object, const QString & id, const QString & method, const QVariant & v1, const QVariant & v2, const QVariant & v3, const QVariant & v4, const QVariant & v5, const QVariant & v6 ) {
	QStringList args;
	args.append( fromValue( v1 ) );
	args.append( fromValue( v2 ) );
	args.append( fromValue( v3 ) );
	args.append( fromValue( v4 ) );
	args.append( fromValue( v5 ) );
	args.append( fromValue( v6 ) );
	this->p_->recordInput( object, id, method, args );
}

void Spy::executeScript( const QString & script ) {
	if( this->p_->engine->canEvaluate( script ) ){
		this->p_->engine->evaluate( script );
	} else {
		this->p_->engine->clearExceptions();
		assert( !"INVALID SCRIPT" );
	}
}

void Spy::finish(){
	if( this->p_->model ) {
		this->p_->model->deleteLater();
		this->p_->model = NULL;
	}
}

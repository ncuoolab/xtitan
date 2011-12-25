#include "QxTesting/SpyModule/Controller/QxSpy.h"
#include "QxTesting/QxTestDefinition.h"
#include "QxTesting/SpyModule/Model/QxAutoSpyModel.h"
#include "QxTesting/Util/SignalSpy/Q4puGenericSignalSpy.h"
#include "QxTesting/CommandParser/QxCommandParser.h"
#include <cassert>

#include "util/Logger.h"

#include <QtNetwork/QHostAddress>
#include <QtCore/QDateTime>

QString QxSpy::fromValue_( bool value ) {
	return value ? "true" : "false";
}

QString QxSpy::fromValue_( int value ) {
	return QString::number( value );
}

QString QxSpy::fromValue_( double value ) {
	return QString::number( value );
}

QString QxSpy::fromValue_( const QString & value ) {
	QString sValue( value );
	return "\'" + sValue.replace( "\\", "\\\\" ).replace( "\'", "\\\'" ) + "\'";
}

QString toScript( const QString & objectName, const QString & signature, const QStringList & args ) {
	QString methodName = signature.section( "::", -1 );
	return QString( "%1.%2(%3)" ).arg( objectName ).arg( methodName ).arg( args.join( "," ) );
}

QxSpy* QxSpy::_singleton = NULL;

QxSpy* QxSpy::getInstance(){
	if(_singleton == NULL)
		_singleton = new QxSpy();
	return _singleton;
}

QxSpy::QxSpy():
QObject(),
model( NULL ),
engine( new QScriptEngine( this ) ),
isTestable( false ) {
	// NOTE: this connection **MUST** be Qt::QueuedConnection, or you will suffer shit of threads
	this->connect( this, SIGNAL( scriptProxy_( const QString & ) ), SLOT( executeScript_( const QString & ) ), Qt::QueuedConnection );
}

QxSpy::~QxSpy(){
	if(model)
		delete model;
}

void QxSpy::tryAutomation() {
	QStringList args( QApplication::arguments() );

	qDebug() << args;

	if( args.size() <= 1 ) {
		return;
	}

	// argument format: -t testunitname
	int index = args.indexOf( "-t" );
	if( index < 0 ) {
		return;
	}
	this->isTestable = true;
	QxAutoSpyModel * m = new QxAutoSpyModel( this->engine );
	this->connect( m, SIGNAL( ready() ), SLOT( onModelReady_() ) );
	m->connectToHost( "OracleServer" );

	this->model = m;
}

void QxSpy::encodeCheck(QString method, QString id, QString value ){
	if (!isTestable)	return;
	QString label = method+"_"+id;
	QString msg = value;
	this->Check(label,msg);
}

void QxSpy::encodeCheck(QString method, QString id, bool expr ){
	if (!isTestable)	return;
	QString label = method+"_"+id;
	QString msg = expr ? "true":"false";
	this->Check(label,msg);
}

void QxSpy::encodeCheck(QString method, QString id, int val ){
	if (!isTestable)	return;
	QString lable = method+"_"+id;
	QString msg = QString::number(val);
	this->Check(lable,msg);
}

void QxSpy::encodeCheck(QString method, QString id, double val ){
	if (!isTestable)	return;
	QString label = method+"_"+id;
	QString msg = QString::number(val);
	this->Check(label,msg);
}

void QxSpy::encodeCheck( const std::string & method, const std::string & id, const std::string & msg ){
	this->encodeCheck(QString::fromUtf8(method.c_str()),QString::fromUtf8(id.c_str()),QString::fromUtf8(msg.c_str()));
}
void QxSpy::encodeCheck( const std::string & method, const std::string & id, bool expr ){
	this->encodeCheck(QString::fromUtf8(method.c_str()),QString::fromUtf8(id.c_str()),expr);
}
void QxSpy::encodeCheck( const std::string & method, const std::string & id, int val ){
	this->encodeCheck(QString::fromUtf8(method.c_str()),QString::fromUtf8(id.c_str()),val);
}
void QxSpy::encodeCheck( const std::string & method, const std::string & id, double val ){
	this->encodeCheck(QString::fromUtf8(method.c_str()),QString::fromUtf8(id.c_str()),val);
}
void QxSpy::encodeCheck( const std::string & method, const std::string & id, const Checkable * checkable ){
	this->encodeCheck(QString::fromUtf8(method.c_str()),QString::fromUtf8(id.c_str()),QString::fromUtf8(checkable->toCheckString().c_str()));
}

void QxSpy::Check(QString label, QString msg){
	if( this->model ){
		this->model->check(label,msg );
	}
}

void QxSpy::registerObject(QObject* obj){
	if( !this->isTestable ) {
		return;
	}

	QString objName = obj->objectName();
	QRegExp exp("[^a-zA-Z0-9_]");
	objName = objName.replace(exp,"");
	obj->setObjectName(objName);
	QScriptValue scriptObj;
	scriptObj= engine->newQObject( obj );
	engine->globalObject().setProperty( objName , scriptObj);
	this->tokens_.insert( objName, obj );
}

QObject * QxSpy::getObject( const QString & key ) const {
	QMap< QString, QObject * >::const_iterator it = this->tokens_.find( key );
	if( it == this->tokens_.end() ) {
		return NULL;
	} else {
		return it.value();
	}
}

void QxSpy::RecordInput( const QString & objectName, const QString & id ) {
	this->recordInput_( objectName, id, QStringList() );
}

void QxSpy::recordInput_( const QString & objectName, const QString & id, const QStringList & args ) {
	this->recordScript_( id, toScript( objectName, id, args ) );
}

void QxSpy::RecordInput( const QObject * object, const QString & id, const QString & method ) {
	this->recordInput_( object, id, method, QStringList() );
}

void QxSpy::recordInput_( const QObject * object, const QString & id, const QString & method, const QStringList & args ) {
	this->recordScript_( id, QString( "%1.%2(%3)" ).arg( object->objectName() ).arg( method ).arg( args.join( "," ) ) );
}

void QxSpy::recordScript_( const QString & id, const QString & script ) {
	MTRACK( script.toUtf8().data() );

	if( !this->isTestable ) {
		return;
	}

	if( this->model ) {
		this->model->input( id, script, QDateTime::currentMSecsSinceEpoch() );
	}
}

void QxSpy::onModelReady_() {
}

void QxSpy::executeScript( const QString & script ) {
	emit this->scriptProxy_( script );
}

void QxSpy::executeScript_( const QString & script ) {
	if( this->engine->canEvaluate( script ) ){
		this->engine->evaluate( script );
	} else {
		this->engine->clearExceptions();
		assert( !"INVALID SCRIPT" );
	}
}

void QxSpy::finish(){
	if( this->model ) {
		this->model->deleteLater();
		this->model = NULL;
	}
}

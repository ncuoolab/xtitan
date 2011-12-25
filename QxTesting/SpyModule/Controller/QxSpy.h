#ifndef QX_SPY_H
#define QX_SPY_H

#include <time.h>
#include <ctime>
#include <windows.h>
#include <QObject>
#include <QtScript/QScriptEngine>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#include "util/Spy.h"

class QxSpyModel;

class QxSpy : public QObject , public Spy {
	Q_OBJECT
public:
	static QxSpy* getInstance();	

	void tryAutomation();
	bool isTesting(){	return isTestable;	}

	void registerObject( QObject * obj );
	QObject * getObject( const QString & key ) const;
	void Check(QString label, QString msg);

	void RecordInput( const QString & objectName, const QString & id );
	template< typename T1 >
	void RecordInput( const QString & objectName, const QString & id, const T1 & a1 ) {
		QStringList argt;
		argt.append( fromValue_( a1 ) );
		this->recordInput_( objectName, id, argt );
	}
	template< typename T1, typename T2 >
	void RecordInput( const QString & objectName, const QString & id, const T1 & a1, const T2 & a2 ) {
		QStringList argt;
		argt.append( fromValue_( a1 ) );
		argt.append( fromValue_( a2 ) );
		this->recordInput_( objectName, id, argt );
	}
	template< typename T1, typename T2, typename T3 >
	void RecordInput( const QString & objectName, const QString & id, const T1 & a1, const T2 & a2, const T3 & a3 ) {
		QStringList argt;
		argt.append( fromValue_( a1 ) );
		argt.append( fromValue_( a2 ) );
		argt.append( fromValue_( a3 ) );
		this->recordInput_( objectName, id, argt );
	}
	template< typename T1, typename T2, typename T3, typename T4 >
	void RecordInput( const QString & objectName, const QString & id, const T1 & a1, const T2 & a2, const T3 & a3, const T4 & a4 ) {
		QStringList argt;
		argt.append( fromValue_( a1 ) );
		argt.append( fromValue_( a2 ) );
		argt.append( fromValue_( a3 ) );
		argt.append( fromValue_( a4 ) );
		this->recordInput_( objectName, id, argt );
	}
	template< typename T1, typename T2, typename T3, typename T4, typename T5 >
	void RecordInput( const QString & objectName, const QString & id, const T1 & a1, const T2 & a2, const T3 & a3, const T4 & a4, const T5 & a5 ) {
		QStringList argt;
		argt.append( fromValue_( a1 ) );
		argt.append( fromValue_( a2 ) );
		argt.append( fromValue_( a3 ) );
		argt.append( fromValue_( a4 ) );
		argt.append( fromValue_( a5 ) );
		this->recordInput_( objectName, id, argt );
	}
	template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6 >
	void RecordInput( const QString & objectName, const QString & id, const T1 & a1, const T2 & a2, const T3 & a3, const T4 & a4, const T5 & a5, const T6 & a6 ) {
		QStringList argt;
		argt.append( fromValue_( a1 ) );
		argt.append( fromValue_( a2 ) );
		argt.append( fromValue_( a3 ) );
		argt.append( fromValue_( a4 ) );
		argt.append( fromValue_( a5 ) );
		argt.append( fromValue_( a6 ) );
		this->recordInput_( objectName, id, argt );
	}

	void RecordInput( const QObject * object, const QString & id, const QString & method );
	template< typename T1 >
	void RecordInput( const QObject * object, const QString & id, const QString & method, const T1 & a1 ) {
		QStringList argt;
		argt.append( fromValue_( a1 ) );
		this->recordInput_( object, id, method, argt );
	}
	template< typename T1, typename T2 >
	void RecordInput( const QObject * object, const QString & id, const QString & method, const T1 & a1, const T2 & a2 ) {
		QStringList argt;
		argt.append( fromValue_( a1 ) );
		argt.append( fromValue_( a2 ) );
		this->recordInput_( object, id, method, argt );
	}
	template< typename T1, typename T2, typename T3 >
	void RecordInput( const QObject * object, const QString & id, const QString & method, const T1 & a1, const T2 & a2, const T3 & a3 ) {
		QStringList argt;
		argt.append( fromValue_( a1 ) );
		argt.append( fromValue_( a2 ) );
		argt.append( fromValue_( a3 ) );
		this->recordInput_( object, id, method, argt );
	}
	template< typename T1, typename T2, typename T3, typename T4 >
	void RecordInput( const QObject * object, const QString & id, const QString & method, const T1 & a1, const T2 & a2, const T3 & a3, const T4 & a4 ) {
		QStringList argt;
		argt.append( fromValue_( a1 ) );
		argt.append( fromValue_( a2 ) );
		argt.append( fromValue_( a3 ) );
		argt.append( fromValue_( a4 ) );
		this->recordInput_( object, id, method, argt );
	}
	template< typename T1, typename T2, typename T3, typename T4, typename T5 >
	void RecordInput( const QObject * object, const QString & id, const QString & method, const T1 & a1, const T2 & a2, const T3 & a3, const T4 & a4, const T5 & a5 ) {
		QStringList argt;
		argt.append( fromValue_( a1 ) );
		argt.append( fromValue_( a2 ) );
		argt.append( fromValue_( a3 ) );
		argt.append( fromValue_( a4 ) );
		argt.append( fromValue_( a5 ) );
		this->recordInput_( object, id, method, argt );
	}
	template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6 >
	void RecordInput( const QObject * object, const QString & id, const QString & method, const T1 & a1, const T2 & a2, const T3 & a3, const T4 & a4, const T5 & a5, const T6 & a6 ) {
		QStringList argt;
		argt.append( fromValue_( a1 ) );
		argt.append( fromValue_( a2 ) );
		argt.append( fromValue_( a3 ) );
		argt.append( fromValue_( a4 ) );
		argt.append( fromValue_( a5 ) );
		argt.append( fromValue_( a6 ) );
		this->recordInput_( object, id, method, argt );
	}

	void finish();

	void encodeCheck(QString method, QString id, QString msg);
	void encodeCheck(QString method, QString id, bool expr);
	void encodeCheck(QString method, QString id, int val);
	void encodeCheck(QString method, QString id, double val);

	virtual void encodeCheck( const std::string & method, const std::string & id, const std::string & msg );
	virtual void encodeCheck( const std::string & method, const std::string & id, bool expr );
	virtual void encodeCheck( const std::string & method, const std::string & id, int val );
	virtual void encodeCheck( const std::string & method, const std::string & id, double val );
	virtual void encodeCheck( const std::string & method, const std::string & id, const Checkable * checkable );

	void executeScript( const QString & script );

signals:
	void connected();
	void scriptProxy_( const QString & script );

private slots:
	void onModelReady_();
	void executeScript_( const QString & );

protected:
	//model 
	QxSpyModel* model;
	//engine
	QScriptEngine* engine;

private:
	static QString fromValue_( const QString & );
	static QString fromValue_( bool );
	static QString fromValue_( int );
	static QString fromValue_( double );

	QxSpy();
	virtual ~QxSpy();
	static QxSpy* _singleton;

	void recordInput_( const QString &, const QString &, const QStringList & );
	void recordInput_( const QObject *, const QString &, const QString &, const QStringList & );
	void recordScript_( const QString &, const QString & );

	bool isTestable;
	QString filename;
	QMap< QString, QObject * > tokens_;
};

#endif // QX_SPY_H

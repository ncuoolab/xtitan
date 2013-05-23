#ifndef KRAPTURE_SIKULICLIENT_PRIVATE_HPP
#define KRAPTURE_SIKULICLIENT_PRIVATE_HPP

#include "SikuliClient.hpp"

#include <functional>

#include <QtCore/QProcess>
#include <QtCore/QTextStream>
#include <QtCore/QTimer>
#include <QtScript/QScriptEngine>


namespace xtitan {

class SikuliClient::Private : public QObject {
	Q_OBJECT

public:
	typedef std::function< void () > Command;
	typedef std::vector< std::tuple< QString, QString > > BundleList;
	Private();

	void onActionTimeout( const QString & );
	void onActivationTimeout();
	void readBundle();
	void readCapture();
	void readExecute();
	void readReadyToken();
	void nextBundle();

	QVariantMap decode( const QString & );
	QString encode( const QScriptValue & );

public slots:
	void onFinished( int, QProcess::ExitStatus );
	void onReadyRead();
	void onReadyReadStandardError();
	void onTimeout();

signals:
	void bundlesExecuted();
	void captured( const QString & );
	void error( const QString & );
	void executed( bool, const QString & );
	void checkRequired( int id, const xtitan::CheckPoint & cp );
	void asyncCheckRequired( int id, const xtitan::AsyncCheckPoint & acp );
	void inputRequired( int id, const QString & object, const QString & method, const QVariantList & args );
	void log( const QString & );
	void ready();
	void taskCompleted( const QString &, bool, const QString & );

public:
	Command doFailure;
	Command doSuccess;
	bool opening;
	QProcess * server;
	QProcess * client;
	QScriptEngine * engine;
	QTextStream sio;
	QString clientPath;
	QString clientWD;
	BundleList bundles;
	BundleList::size_type currentBundle;
};

}

#endif

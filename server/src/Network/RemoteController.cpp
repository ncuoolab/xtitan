#include "RemoteController.hpp"
#include "xTitan/Utility/Setting.hpp"

#include <QtCore/QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QEventLoop>
#include <QtCore/QTimer>
#include <QtCore/QThread>
#include <QtNetwork/QNetworkAccessManager>

#include <cassert>

using xtitan::utilities::Setting;
using namespace xtitan::network;

/*
void RemoteController::restartServer() {
	QUrl uri( QString( "http://%1/server/restart.php" ).arg( QxSetting::Instance().get( "ServerAddress" ) ) );
	QNetworkRequest request( uri );
	QNetworkReply * reply = this->manager_->get( request );
	this->connect( reply, SIGNAL( finished() ), SLOT( onFinished_() ) );
	reply->connect( this, SIGNAL( finished() ), SLOT( deleteLater() ) );
}
*/

bool RemoteController::importDatabase( QIODevice * data ) {
	QUrl uri( QString( "http://%1/server/import.php" ).arg( Setting::getInstance().get( "ServerAddress" ) ) );
	QNetworkRequest request( uri );
	request.setHeader( QNetworkRequest::ContentTypeHeader, "text/x-sql" );
	if( !data->open( QIODevice::ReadOnly ) ) {
		assert( !"BUFFER OPEN ERROR" );
	}
	request.setHeader( QNetworkRequest::ContentLengthHeader, data->size() );
	QNetworkAccessManager manager;
	QNetworkReply * reply = manager.put( request, data );
	// timeout in 10 seconds
	QTimer timer;
	timer.setInterval( 10000 );
	timer.setSingleShot( true );
	QEventLoop wait;
	wait.connect( &timer, SIGNAL( timeout() ), SLOT( quit() ) );
	wait.connect( reply, SIGNAL( finished() ), SLOT( quit() ) );
	timer.start();
	wait.exec();
	timer.stop();
	data->close();
	bool finished = reply->isFinished();
	reply->deleteLater();
	return finished;
}

bool RemoteController::exportDatabase( QIODevice * data ) {
	QUrl uri( QString( "http://%1/server/export.php" ).arg( Setting::getInstance().get( "ServerAddress" ) ) );
	QNetworkRequest request( uri );
	if( !data->open( QIODevice::WriteOnly ) ) {
		assert( !"BUFFER OPEN ERROR" );
	}
	QNetworkAccessManager manager;
	QNetworkReply * reply = manager.get( request );
	// timeout in 10 seconds
	QTimer timer;
	timer.setInterval( 10000 );
	timer.setSingleShot( true );
	QEventLoop wait;
	wait.connect( &timer, SIGNAL( timeout() ), SLOT( quit() ) );
	wait.connect( reply, SIGNAL( finished() ), SLOT( quit() ) );
	timer.start();
	wait.exec();
	timer.stop();
	bool finished = reply->isFinished();
	if( finished ) {
		data->write( reply->readAll() );
	}
	reply->deleteLater();
	data->close();
	return finished;
}

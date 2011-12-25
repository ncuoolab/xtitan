#ifndef  _TEST_UNIT_MANAGER_H_
#define _TEST_UNIT_MANAGER_H_

#include <QObject>
#include <QString>
#include <QStringList>
#include <map>
#include <QMap>
#include "TCP_Connection/QxSimpleServer.h"

#include <QtCore/QDateTime>

class QxAbstractUnit;
class TestClient;
class QxTestCase;

class TestUnitManager  : public QObject
{
	Q_OBJECT
public:
	explicit TestUnitManager( QObject * parent );
	virtual ~TestUnitManager();

	void RegressionTest();
	void SmokeTest();
	void specificTest( const QStringList & tests, bool killing );
	void stopTest();

	void startRecord( int nClients );
	void startUpdate( const QString & name );
	QxTestCase stopRecord();

	bool isOneShot() const;
	void setOneShot( bool oneShot );

signals:
	void log( const QString & type, const QString & message );
	void error( const QString & type, const QString & errorString );
	void finished();
	void finished( const QxTestCase & testCase );

private slots:
	void onNewConnection_();
	void onUnitError_( const QString &, const QString & );
	void onReplayFinished_();
	void onRecordFinished_();
	void onUpdateFinished_();

private:
	void makeServerReport_();
	void createLogDir_();
	void setupReplay_( bool );
	void nextReplay_();

	QxSimpleServer * server_;
	QxAbstractUnit * currentUnit_;

	bool testing_;
	bool oneShot_;
	int currentTestCount_;
	QStringList testList_;
	QStringList errorList_;
};

#endif

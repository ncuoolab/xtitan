#ifndef QXTESTUNIT_H
#define QXTESTUNIT_H

#include <QObject>
#include <QString>
#include "TCP_Connection/TestClient.h"

#include <QtCore/QMutex>
#include <QtCore/QMap>
#include <QtCore/QProcess>
#include <QtCore/QTimer>

#include <boost/tr1/memory.hpp>

class QxTestUnitView;
class QxRemoteController;
class QxAbstractPoint;
class QxCheckPoint;

class QxTestUnit  : public QObject
{
	Q_OBJECT
public :
	enum UnitState {
		INIT	= 0,
		PLAY	= 1,
		REC		= 2,
		FIN		= 4
	};
	Q_DECLARE_FLAGS( UnitStates, UnitState )

	QxTestUnit( const QString & name );
	~QxTestUnit();

	//getter :
	QWidget * getView() const;
	const QString & getName() const;
	void appendLog( const QString & msg );

	void addClient( TestClient * c );
	TestClient * getClient( int id ) const;
	QList< TestClient * > getClientList() const;
	int getClientCount() const;
	UnitStates getState() const;
	void setKill( bool killing );
	void wait( int msec );
	void wait( QxCheckPoint * point );

	/////////////////////////////////////////////////////

	void startRecord( int nClients );
	void record( int id, const QString & label, const QString & script, qint64 timestamp );
	void record( int id, const QString & label, const QString & value );
	void stopRecord();
	bool saveRecord( const QString & name, const QString & creator, const QString & description );

	bool startReplay( const QString & testName );
	void stopReplay();

	bool startUpdate( const QString & testName );

	void check( int id, const QString & label, const QString & value );

	void closeClients();
	void finish();

protected :
	void Setup();
	void teardown();
	void generateReport( const QString & errorMessage );
	bool saveScript( const QString & testName );
	void saveResult( const QString & msg );
	void setState( UnitStates state );

public slots:
	void executeNextInput();

signals:
	void ready();
	void requestDestroy();
	void requestSave( const QString & id );
	void error( const QString & message );
	void log( const QString & id, const QString & message );

private slots:
	void onClientError_( const QString & );
	void onClientDisconnected_();
	void onTimeout_();

private:
	void loadScript_( const QString & filePath );
	void spawnChiwawa_();
	void exportDatabase_( const QString & );
	void importDatabase_( const QString & );
	void restartServer_();

	QxRemoteController * link_;

	QString name_;
	QxTestUnitView * view;
	QTimer * timer_;
	QTimer * timeout_;
	UnitStates currentState_;
	QMap< int, TestClient * > clients_;
	int nClients_;
	QList< QProcess * > childs_;
	QString databasePath_;
	bool killing_;
	//script
	int currentInput_;
	QList< std::tr1::shared_ptr< QxAbstractPoint > > points_;
	QMap< QPair< int, QString >, QList< QxCheckPoint * > > checkPoints_;
	QxCheckPoint * waitingCheck_;
	QStringList recordedScirpt_;
	QList< QxCheckPoint * > ignoredCheckPoints_;
	bool checkPointUnordered_;
	qint64 lastTick_;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QxTestUnit::UnitStates)

#endif	//_TEST_UNIT_

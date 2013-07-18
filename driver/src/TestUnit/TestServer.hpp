/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#ifndef XTITAN_TESTUNIT_TESTSERVER_HPP
#define XTITAN_TESTUNIT_TESTSERVER_HPP

#include <memory>

#include <QtCore/QObject>
#include <QtCore/QVariant>

namespace xtitan {

struct AsyncCheckPoint;
struct CheckPoint;
struct InputPoint;

class TestServer: public QObject {
	Q_OBJECT
public:
	explicit TestServer( QObject * parent );

	bool check() const;
	QString errorString() const;
	bool isRecording() const;
	bool listen( const QString & name );
	void recordAsyncOracle( int id, const AsyncCheckPoint & acp );
	void recordOracle( int id, const CheckPoint & cp );
	void sendInput( int id, const InputPoint & ip );
	void startRecording();
	void stopRecording();

signals:
	void asyncCheckReceived( int id, const xtitan::AsyncCheckPoint & acp );
	void checkReceived( int id, const xtitan::CheckPoint & cp );
	void inputReceived( int id, int delay, const xtitan::InputPoint & ip );

private:
	class Private;
	std::shared_ptr< Private > p_;
};

}

#endif

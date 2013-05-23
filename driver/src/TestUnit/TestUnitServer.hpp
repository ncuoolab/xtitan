#ifndef XTITAN_TESTUNIT_TESTUNITSERVER_HPP
#define XTITAN_TESTUNIT_TESTUNITSERVER_HPP

#include <memory>

#include <QtCore/QObject>
#include <QtCore/QVariant>

namespace xtitan {

struct AsyncCheckPoint;
struct CheckPoint;

class TestUnitServer: public QObject {
	Q_OBJECT
public:
	explicit TestUnitServer( QObject * parent );

	bool check() const;
	QString errorString() const;
	bool isRecording() const;
	bool listen( const QString & name );
	void recordAsyncOracle( int id, const AsyncCheckPoint & acp );
	void recordOracle( int id, const CheckPoint & cp );
	void sendInput( int id, const QString & object, const QString & method, const QVariantList & args );
	void startRecording();
	void stopRecording();

signals:
	void asyncCheckReceived( int id, const xtitan::AsyncCheckPoint & acp );
	void checkReceived( int id, const xtitan::CheckPoint & cp );
	void inputReceived( int id, int delay, const QString & object, const QString & method, const QStringList & args );

private:
	class Private;
	std::shared_ptr< Private > p_;
};

}

#endif

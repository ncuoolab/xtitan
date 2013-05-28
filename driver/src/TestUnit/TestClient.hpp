#ifndef XTITAN_TESTUNIT_TESTCLIENT_HPP
#define XTITAN_TESTUNIT_TESTCLIENT_HPP

#include <memory>

#include <QtCore/QObject>
#include <QtCore/QVariant>

namespace xtitan {

struct AsyncCheckPoint;
struct CheckPoint;
class SimpleSocket;
class TestServer;

class TestClient: public QObject {
	Q_OBJECT
public:
	TestClient( int id, SimpleSocket * socket, TestServer * server, QObject * parent );

	bool check() const;
	void recordOracle( const CheckPoint & cp );
	void recordAsyncOracle( const AsyncCheckPoint & acp );
	void sendInput( const QString & object, const QString & method, const QVariantList & args );

signals:
	void disconnected();

private:
	class Private;
	std::shared_ptr< Private > p_;
};

}

#endif

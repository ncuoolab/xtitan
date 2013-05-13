#ifndef XTITAN_TESTUNIT_TESTUNIT_HPP
#define XTITAN_TESTUNIT_TESTUNIT_HPP

#include <memory>

#include <QtCore/QObject>
#include <QtCore/QVariant>

namespace xtitan {

class SimpleSocket;

class TestUnitServer;

class TestUnit: public QObject {
	Q_OBJECT
public:
	TestUnit( int id, SimpleSocket * socket, TestUnitServer * server, QObject * parent );

	bool check() const;
	void recordOracle( const QString & label, const QString & value );
	void sendInput( const QString & object, const QString & method, const QVariantList & args );

signals:
	void disconnected();

private:
	class Private;
	std::shared_ptr< Private > p_;
};

}

#endif

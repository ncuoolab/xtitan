#ifndef _TEST_UNIT_MANAGER_H_
#define _TEST_UNIT_MANAGER_H_

#include <QObject>
#include <QVector>
#include <QString>
#include <map>
#include <QTcpServer>

class OracleServerView;
class TestUnit;
typedef QVector<TestUnit*> TestList;

class TestUnitManager  : public QObject
{
	Q_OBJECT
public:
	static TestUnitManager* getInstance();

	void createTcpConnection(int port = 8888);
	void destroyTcpConnection();

	void createTestUnit(QTcpSocket* sck);
	void destroyTestUnit();

	TestUnit* getTestUnit();

	void appendLog(QString msg);

public slots:
	void slot_acceptConnection();

protected:
	static TestUnitManager* singleton;
	TestUnitManager();

private:
	TestList unitList;
	QTcpServer* tcpServer;

	OracleServerView* view;
};

#endif
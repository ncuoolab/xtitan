#ifndef XTITAN_CLIENT_SPY_SPYMODEL_HPP
#define XTITAN_CLIENT_SPY_SPYMODEL_HPP

#include <QtCore/QObject>


namespace xtitan {

class SpyModel : public QObject {
	Q_OBJECT
public :
	explicit SpyModel();
	virtual ~SpyModel();

	void connectToHost( const QString & name );

	void stop();
	void check( const QString & file, int line, const QString & id, const QStringList & args );
	void input( const QString & object, const QString & method, const QStringList & args, qint64 timeStamp );
	void asyncCheck( const QString & file, int line, const QString & id, const QString & pre, const QStringList & args );

signals:
	void ready();
	void scriptReceived( const QString & script );

private:
	class Private;
	Private * p_;
};

}

#endif

/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#ifndef XTITAN_CLIENT_SPY_SPYMODEL_HPP
#define XTITAN_CLIENT_SPY_SPYMODEL_HPP

#include <QtCore/QObject>
#include <QtCore/QVariant>


namespace xtitan {

class SpyModel : public QObject {
	Q_OBJECT
public :
	explicit SpyModel();
	virtual ~SpyModel();

	void connectToHost( const QString & name );

	void stop();
	void check( const QString & file, int line, const QString & id, const QVariantList & args );
	void input( const QString & object, const QString & method, const QVariantList & args, qint64 timeStamp );
	void asyncCheck( const QString & file, int line, const QString & id, const QString & pre, const QVariantList & args );

signals:
	void ready();
	void scriptReceived( const QString & script );

private:
	class Private;
	Private * p_;
};

}

#endif

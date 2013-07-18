/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#ifndef XTITAN_CLIENT_SPYMODULE_CONTROLLER_SPY_HPP
#define XTITAN_CLIENT_SPYMODULE_CONTROLLER_SPY_HPP

#include <memory>

#include <QtCore/QObject>
#include <QtCore/QVariant>

#include "xTitan/Utility/SpyConfig.hpp"


namespace xtitan {

class XTITAN_SPY_DLL Spy: public QObject {
	Q_OBJECT
public:
	static Spy & instance();

	void activate();
	Q_INVOKABLE void check( const QString & file, int line, const QString & id, const QVariantList & args );
	Q_INVOKABLE void asyncCheck( const QString & file, int line, const QString & id, const QString pre, const QVariantList & args );
	void finish();
	QString getToken( QObject * object ) const;
	QObject * getObject( const QString & key ) const;
	bool isTesting() const;
	Q_INVOKABLE void input( const QString & objectName, const QString & id, const QVariantList & args );
	Q_INVOKABLE void registerObject( QObject * object, const QString & id );

signals:
	void ready();

private:
	Spy();
	virtual ~Spy();

	class Private;
	friend class Private;
	std::shared_ptr< Private > p_;
};

}

#endif

#ifndef XTITAN_CLIENT_SPYMODULE_CONTROLLER_SPY_HPP
#define XTITAN_CLIENT_SPYMODULE_CONTROLLER_SPY_HPP

#include <memory>

#include <QtCore/QObject>
#include <QtCore/QStringList>

#include "xTitan/Utility/SpyConfig.hpp"


namespace xtitan {

class XTITAN_SPY_DLL Spy: public QObject {
	Q_OBJECT
public:
	static Spy & instance();

	void activate();
	Q_INVOKABLE void encodeCheck( const QString & id, const QString & feature, const QString & value );
	void finish();
	QObject * getObject( const QString & key ) const;
	bool isTesting() const;
	Q_INVOKABLE void recordInput( const QString & objectName, const QString & id, const QStringList & args );
	Q_INVOKABLE void registerObject( QObject * obj );

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

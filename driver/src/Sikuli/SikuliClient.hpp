#ifndef KRAPTURE_SIKULICLIENT_HPP
#define KRAPTURE_SIKULICLIENT_HPP

#include <memory>
#include <tuple>
#include <vector>

#include <QtCore/QObject>
#include <QtCore/QVariant>

namespace xtitan {

class SikuliClient : public QObject {
	Q_OBJECT

public:
	SikuliClient();
	virtual ~SikuliClient();

	void close();
	bool isOpen() const;
	void open();

	void capture();
	void execute( const QString & script, const QString & programOptions );
	void executeBundles( const std::vector< std::tuple< QString, QString > > & bundles );

signals:
	void bundlesExecuted();
	void captured( const QString & path );
	void error( const QString & message );
	void executed( bool success, const QString & response );
	void checkRequired( int id, const QString & value );
	void asyncCheckRequired( int id, const QString & value );
	void inputRequired( int id, const QString & object, const QString & method, const QVariantList & args );
	void log( const QString & message );
	void ready();
	void taskCompleted( const QString & name, bool success, const QString & response );

private:
	SikuliClient( const SikuliClient & );
	SikuliClient & operator =( const SikuliClient & );

	class Private;
	std::shared_ptr< Private > p_;
};

}

#endif

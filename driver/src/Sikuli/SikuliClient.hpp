/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#ifndef XTITAN_SIKULICLIENT_HPP
#define XTITAN_SIKULICLIENT_HPP

#include <memory>
#include <tuple>
#include <vector>

#include <QtCore/QObject>
#include <QtCore/QVariant>

namespace xtitan {

struct AsyncCheckPoint;
struct CheckPoint;
struct InputPoint;

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
	void checkRequired( int id, const xtitan::CheckPoint & cp );
	void asyncCheckRequired( int id, const xtitan::AsyncCheckPoint & acp );
	void inputRequired( int id, const xtitan::InputPoint & ip );
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

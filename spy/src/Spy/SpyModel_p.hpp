#ifndef XTITAN_CLIENT_SPY_SPYMODEL_PRIVATE_HPP
#define XTITAN_CLIENT_SPY_SPYMODEL_PRIVATE_HPP

#include <functional>

#include "Spy/SpyModel.hpp"
#include "xTitan/Network/SimpleSocket.hpp"

namespace xtitan {

class SpyModel::Private : public QObject {
	Q_OBJECT
public:
	Private( SpyModel * );

	void send( const SimpleSocket::Packet & );

public slots:
	void onConnected();
	void onReadyRead();

signals:
	void ready();
	void scriptReceived( const QString & );

public:
	SimpleSocket * socket;
	std::map< QString, std::function< void ( const QVariant & ) > > commands;
};

}

#endif

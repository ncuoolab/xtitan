#ifndef XTITAN_CLIENT_COMMAND_ABSTRACTCOMMAND_HPP
#define XTITAN_CLIENT_COMMAND_ABSTRACTCOMMAND_HPP

#include <QtCore/QVariant>

namespace xtitan {
namespace command {

class AbstractCommand {
public:
	virtual void execute( const QVariant & data ) = 0;
};

}
}

#endif

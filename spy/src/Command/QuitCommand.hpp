#ifndef XTITAN_CLIENT_COMMAND_QUITCOMMAND_HPP
#define XTITAN_CLIENT_COMMAND_QUITCOMMAND_HPP

#include "AbstractCommand.hpp"

namespace xtitan {
namespace command {

class QuitCommand : public AbstractCommand {
public:
	virtual void execute( const QVariant & data );
};

}
}

#endif

#ifndef XTITAN_CLIENT_COMMAND_COMMANDPARSER_PRIVATE_HPP
#define XTITAN_CLIENT_COMMAND_COMMANDPARSER_PRIVATE_HPP

#include "CommandParser.hpp"
#include "AbstractCommand.hpp"

#include <QtCore/QMap>

namespace xtitan {
namespace command {

class CommandParser::Private {
public:
	Private();

	typedef QMap< QString, AbstractCommand * > CommandMap;
	CommandMap commandList;
};

}
}

#endif

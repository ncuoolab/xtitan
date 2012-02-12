#ifndef XTITAN_CLIENT_COMMAND_INPUTCOMMAND_HPP
#define XTITAN_CLIENT_COMMAND_INPUTCOMMAND_HPP

#include "AbstractCommand.hpp"

namespace xtitan {
	namespace command {

		class InputCommand : public AbstractCommand {
		public:
			virtual void execute( const QVariant & data );
		};

	}
}

#endif

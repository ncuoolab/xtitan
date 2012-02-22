#ifndef XTITAN_CLIENT_COMMAND_FAILCOMMAND_HPP
#define XTITAN_CLIENT_COMMAND_FAILCOMMAND_HPP

#include "AbstractCommand.hpp"

namespace xtitan {
	namespace command {

		class FailCommand : public AbstractCommand {
		public:
			virtual void execute( const QVariant & data );
		};

	}
}

#endif

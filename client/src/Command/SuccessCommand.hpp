#ifndef XTITAN_CLIENT_COMMAND_SUCCESSCOMMAND_HPP
#define XTITAN_CLIENT_COMMAND_SUCCESSCOMMAND_HPP

#include "AbstractCommand.hpp"

namespace xtitan {
	namespace command {

		class SuccessCommand : public AbstractCommand {
		public:
			virtual void execute( const QVariant & data );
		};

	}
}

#endif

#ifndef XTITAN_CLIENT_COMMAND_COMMANDPARSER_HPP
#define XTITAN_CLIENT_COMMAND_COMMANDPARSER_HPP

#include <QtCore/QString>
#include <QtCore/QVariant>

#include <memory>

namespace xtitan {
namespace command {

class CommandParser {
public:
	CommandParser();
	void parse( const QString & header, const QVariant & data );

private:
	CommandParser( const CommandParser & );
	CommandParser & operator =( const CommandParser & );

	class Private;
	std::shared_ptr< Private > p_;
};

}
}

#endif

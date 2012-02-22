#include "CommandParserPrivate.hpp"
#include "Network/TCPMessage.hpp"

#include "InputCommand.hpp"
#include "SuccessCommand.hpp"
#include "FailCommand.hpp"
#include "QuitCommand.hpp"

#include <cassert>

using namespace xtitan::command;

CommandParser::Private::Private():
commandList() {
	//response :
	commandList[TCPMessage::Succeess		] = new SuccessCommand(); 
	commandList[TCPMessage::Failure			] = new FailCommand();

	commandList[TCPMessage::Input			] = new InputCommand();

	commandList[TCPMessage::QuitApplication	] = new QuitCommand();
}

CommandParser::CommandParser():
p_( new Private ) {
}

void CommandParser::parse( const QString & header, const QVariant & data ) {
	Private::CommandMap::iterator it = this->p_->commandList.find( header );
	if( it != this->p_->commandList.end() ) {
		it.value()->execute( data );
	} else {
		//throw an exception
		assert( !"WRONG PROTOCOL COMMAND!" );
	}
}

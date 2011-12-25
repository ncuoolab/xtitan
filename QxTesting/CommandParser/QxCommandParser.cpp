#include "QxTesting/CommandParser/QxCommandParser.h"
#include "QxTesting/TCPConnection/QxTCPMessage.h"

#include "QxTesting/Commands/QxRecvInputCmd.h"
#include "QxTesting/Commands/QxSuccessCmd.h"
#include "QxTesting/Commands/QxFailCmd.h"
#include "QxTesting/Commands/QxEndCmd.h"
#include "QxTesting/Commands/QxQuitCmd.h"

#include <cassert>

QxCommandParser* QxCommandParser::_singleton = 0;

QxCommandParser* QxCommandParser::getInstance(){
	if (_singleton == NULL)
		_singleton = new QxCommandParser();
	return _singleton;
}

void QxCommandParser::parse( const QString & header, const QVariant & data ) {
	CommandMap::iterator it = commandList.find( header );
	if(it != commandList.end()){
		it.value()->Execute( data );
	} else {
		//throw an exception
		assert( !"WRONG PROTOCOL COMMAND!" );
	}
}

QxCommandParser::QxCommandParser(){

	//response :
	commandList[QxTCPMessage::Succeess			] = new QxSuccessCmd(); 
	commandList[QxTCPMessage::Failure			] = new QxFailCmd();

	commandList[QxTCPMessage::Input				] = new QxRecvInputCmd();

	commandList[QxTCPMessage::End				] = new QxEndCmd();
	commandList[QxTCPMessage::QuitApplication	] = new QxQuitCmd();

}
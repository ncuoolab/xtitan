#include "QxTesting/TCPConnection/QxTCPMessage.h"



//Request to server
const QString QxTCPMessage::ReqJoinUnit			= "<ReqJoinUnit>";

//response from server
const QString QxTCPMessage::Succeess			= "<Success>";
const QString QxTCPMessage::Failure				= "<Failure>";

//================================================//

//Send command
const QString QxTCPMessage::Check				= "<Check>";
const QString QxTCPMessage::Input				= "<Input>";

//Received command
const QString QxTCPMessage::End					= "<End>";
const QString QxTCPMessage::QuitApplication		= "<Quit>";

#include "Network/TCPMessage.hpp"


//Request to server
const QString TCPMessage::ReqJoinUnit			= "<ReqJoinUnit>";

//response from server
const QString TCPMessage::Succeess			= "<Success>";
const QString TCPMessage::Failure				= "<Failure>";

//================================================//

//Send command
const QString TCPMessage::Check				= "<Check>";
const QString TCPMessage::Input				= "<Input>";

//Received command
const QString TCPMessage::End					= "<End>";
const QString TCPMessage::QuitApplication		= "<Quit>";

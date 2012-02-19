#include "Network/TCPMessage.hpp"

//request
const QString TCPMessage::ReqJoinUnit			= "<ReqJoinUnit>";

//response to client
const QString TCPMessage::Success				= "<Success>";
const QString TCPMessage::Fail					= "<Fail>";


//received command
const QString TCPMessage::Check					= "<Check>";
const QString TCPMessage::Input					= "<Input>";
const QString TCPMessage::Socket				= "<Socket>";


//send command
const QString TCPMessage::QuitApplicationTag	= "<Quit>";

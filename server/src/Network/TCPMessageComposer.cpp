#include "Network/TCPMessageComposer.hpp"
#include "Network/TCPMessage.hpp"

QString TCPMessageComposer::QuitApplication(){
	return TCPMessage::QuitApplicationTag;
}

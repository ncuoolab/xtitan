#include "TCP_Connection/TCPMessageComposer.h"
#include "TCP_Connection/TCPMessage.h"

QString TCPMessageComposer::QuitApplication(){
	return TCPMessage::QuitApplicationTag;
}

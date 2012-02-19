#ifndef _TCP_MESSAGE_H_
#define _TCP_MESSAGE_H_

#include <QtCore/QString>

class TCPMessage {
public:

	//request from client:
	const static QString ReqJoinUnit;

	//response to client :
	const static QString Success;
	const static QString Fail;


	//received command 
	const static QString Check;
	const static QString Input;
	const static QString Socket;
	
	//send command
	const static QString QuitApplicationTag;
	
};

#endif

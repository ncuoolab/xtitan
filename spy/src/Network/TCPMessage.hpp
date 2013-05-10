#ifndef _QX_TCP_MESSAGE_H_
#define _QX_TCP_MESSAGE_H_

#include <QtCore/QString>


class TCPMessage {
public:
	//Request to server
	const static QString ReqJoinUnit;

	//response from server
	const static QString Succeess;
	const static QString Failure;

	//================================================//

	//Send command
	const static QString Check;
	const static QString Input;

	//Received command
	const static QString End;
	const static QString QuitApplication;
};

#endif

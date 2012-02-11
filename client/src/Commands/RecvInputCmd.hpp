#ifndef _QX_RECV_INPUT_CMD_H
#define _QX_RECV_INPUT_CMD_H

#include "QxTesting/Commands/QxAbstractCommand.h"

class QxRecvInputCmd : public QxAbstractCommand {

	void Execute( const QVariant & data );

};

#endif	//_QX_RECV_INPUT_CMD_H

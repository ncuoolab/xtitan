#ifndef _QX_QUIT_CMD_H
#define _QX_QUIT_CMD_H

#include "QxTesting/Commands/QxAbstractCommand.h"

class QxQuitCmd : public QxAbstractCommand {
	void Execute( const QVariant & data );
};

#endif	//_QX_QUIT_CMD_H

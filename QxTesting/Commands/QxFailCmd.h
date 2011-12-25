#ifndef _QX_FAIL_CMD_H
#define _QX_FAIL_CMD_H

#include "QxTesting/Commands/QxAbstractCommand.h"

class QxFailCmd : public QxAbstractCommand {
	void Execute( const QVariant & data );
};

#endif	//_QX_FAIL_CMD_H

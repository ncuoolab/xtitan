#ifndef _QX_ABSTRACT_COMMAND_H_
#define _QX_ABSTRACT_COMMAND_H_

#include <QtCore/QVariant>

class QxAbstractCommand {
public:
	virtual void Execute( const QVariant & data ) = 0;

};

#endif	//_ABSTRACT_COMMAND_H_

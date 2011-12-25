#include "QxTesting/Commands/QxRecvInputCmd.h"
#include "QxTesting/SpyModule/Controller/QxSpy.h"

// format: script
void QxRecvInputCmd::Execute( const QVariant & data ) {
	QxSpy::getInstance()->executeScript( data.toString() );	// FIXME: thread problems
}

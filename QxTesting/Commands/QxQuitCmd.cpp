#include "QxTesting/Commands/QxQuitCmd.h"
#include "QxMaster/MsnShell.h"
#include "QxTesting/SpyModule/Controller/QxSpy.h"

void QxQuitCmd::Execute( const QVariant & /*data*/ ) {
	QxSpy::getInstance()->finish();
//	MsnShell::getInstance()->slot_QuitMsn();
	_exit( 0 );
}

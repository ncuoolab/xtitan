#include "QxTesting/Commands/QxEndCmd.h"
#include "QxTesting/SpyModule/Controller/QxSpy.h"

void QxEndCmd::Execute( const QVariant & /*data*/ ) {
	QxSpy::getInstance()->finish();
}

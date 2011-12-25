#include "TestUnitManager/OracleServerView.h"
#include "OutputLogger/QxLogger.h"
#include "Setting/Setting.h"

#include <QtGui/QApplication>
#include <QtCore/QStringList>

int main(int argc, char *argv[])
{
	QApplication a( argc, argv );

	QStringList args( a.arguments() );

	OracleServerView view;
	view.show();

	if( a.arguments().size() > 1 ) {
		int index  = args.indexOf( "-o" );
		if( index != -1 && index + 1 < args.size() ) {
			QString filename( args.at( index + 1 ) );
			QxLogger::getInstance()->setFileName( filename );
		}

		index = args.indexOf( "-i" );
		if( index != -1 && index + 1 < args.size() ) {
			QString fileName( args.at( index + 1 ) );
			QxSetting::Instance().setFilePath( fileName );
		}

		index = args.indexOf( "-regression" );
		if( index != -1 ) {
			view.setSingleShot( true );
			view.slot_startRegression();
		} else {
			index = args.indexOf( "-smoke" );
			if( index != -1 ) {
				view.setSingleShot( true );
				view.slot_startSmokeTest();
			}
		}
	}

	return a.exec();
}

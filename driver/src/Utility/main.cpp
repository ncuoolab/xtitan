#include <QtCore/QTextCodec>
#include <QtGui/QApplication>

#include "Widget/MainWindow.hpp"
#include "xTitan/DriverConfig.h"
#include "xTitan/Utility/Setting.hpp"
#include "TestCase/TestCaseManager.hpp"

#define XTITAN_STRINGIZE_(x) #x
#define XTITAN_STRINGIZE(x) XTITAN_STRINGIZE_(x)

int main( int argc, char * argv[] ) {
	QApplication app( argc, argv );

	QApplication::setApplicationName( "xTitanDriver" );
	QApplication::setApplicationVersion( XTITAN_STRINGIZE( XTITAN_DRIVER_VERSION ) );
	QApplication::setOrganizationDomain( "tw.edu.ncu" );
	QApplication::setOrganizationName( "oolab" );
	QTextCodec::setCodecForCStrings( QTextCodec::codecForName( "UTF-8" ) );
	QTextCodec::setCodecForTr( QTextCodec::codecForName( "UTF-8" ) );

	xtitan::Setting::initialize();
	xtitan::TestCaseManager::initialize();

	xtitan::MainWindow window;
	window.show();
	window.reopenTestCaseManager();
	window.restartSikuliServer();

	return app.exec();
}

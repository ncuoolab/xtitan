#include "Widget/MainWindow.hpp"
#include "xTitan/Utility/Logger.hpp"
#include "xTitan/Utility/Setting.hpp"
#include "TestCase/TestCaseManager.hpp"

#include <QtGui/QApplication>
#include <QtCore/QStringList>
#include <QtCore/QTextCodec>

int main( int argc, char * argv[] ) {
	using xtitan::utilities::Setting;
	using xtitan::utilities::Logger;
	using xtitan::testcase::TestCaseManager;
	using xtitan::widget::MainWindow;

	QApplication a( argc, argv );
	QTextCodec::setCodecForCStrings( QTextCodec::codecForName( "UTF-8" ) );
	QTextCodec::setCodecForTr( QTextCodec::codecForName( "UTF-8" ) );
	Setting::getInstance();
	Logger::getInstance();
	TestCaseManager::getInstance();

	QStringList args( a.arguments() );

	MainWindow view;
	view.show();

	if( a.arguments().size() > 1 ) {
		int index  = args.indexOf( "-o" );
		if( index != -1 && index + 1 < args.size() ) {
			QString filename( args.at( index + 1 ) );
			Logger::getInstance().setFileName( filename );
		}

		index = args.indexOf( "-i" );
		if( index != -1 && index + 1 < args.size() ) {
			QString fileName( args.at( index + 1 ) );
			Setting::getInstance().setFilePath( fileName );
		}

		index = args.indexOf( "-regression" );
		if( index != -1 ) {
			view.setSingleShot( true );
			view.startRegression();
		} else {
			index = args.indexOf( "-smoke" );
			if( index != -1 ) {
				view.setSingleShot( true );
				view.startSmokeTest();
			}
		}
	}

	return a.exec();
}

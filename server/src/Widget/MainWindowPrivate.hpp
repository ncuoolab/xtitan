#ifndef ORACLESERVER_CONTROLLER_VIEW_MAINWINDOW_PRIVATE_HPP
#define ORACLESERVER_CONTROLLER_VIEW_MAINWINDOW_PRIVATE_HPP

#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "TestUnit/TestUnitManager.hpp"
#include "Widget/SelectTestDialog.hpp"
#include "Widget/SaveDialog.hpp"
#include "Widget/PreferenceDialog.hpp"
#include "TestCase/TestCase.hpp"

namespace xtitan {
	namespace widget {

		class MainWindow::Private : public QObject {
			Q_OBJECT
		public:
			explicit Private( MainWindow * host );

			void lockReplayUI( bool );
			void lockRecordUI( bool );
			void showSaveDialog( xtitan::testcase::TestCase );

		public slots:
			void error( const QString & type, const QString & errorString );
			void log( const QString & type, const QString & message );
			void onStartRecord();
			void onStopRecord();
			void onStopReplay();
			void onTestSpecific();
			void onRequestSave( const xtitan::testcase::TestCase & );
			void onUpdate();
			void onFinished();

		public:
			MainWindow * host;
			Ui::MainWindow ui;
			SelectTestDialog * selectDialog;
			SaveDialog * saveDialog;
			PreferenceDialog * preference;
			testunit::TestUnitManager * manager;
		};

	}
}

#endif

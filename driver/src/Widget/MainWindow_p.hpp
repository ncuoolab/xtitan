/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#ifndef XTITAN_MAINWINDOW_HPP_
#define XTITAN_MAINWINDOW_HPP_

#include "MainWindow.hpp"

#include <functional>
#include <map>

#include <QtCore/QSignalMapper>
#include <QtCore/QTimer>
#include <QtCore/QUrl>

#include "OpeningDialog.hpp"
#include "PreferenceDialog.hpp"
#include "SavingDialog.hpp"
#include "Sikuli/SikuliClient.hpp"
#include "TestCase/TestCase.hpp"
#include "TestUnit/TestServer.hpp"
#include "TestUnit/CheckPoint.hpp"

#include "ui_MainWindow.h"

namespace xtitan {

class MainWindow::Private : public QObject {
	Q_OBJECT

public:
	typedef std::function< void () > Command;
	typedef std::map< QString, Command > CommandTable;

	explicit Private( MainWindow * );

	void lockSikuliUI( bool );
	void lockTestCaseUI( bool );
	void beginSikuliAction();
	void endSikuliAction();
	void updateTitle();

	void doCapture();
	void doSingle( const QString & );

	MainWindow * host;
	Ui::MainWindow ui;
	OpeningDialog * opening;
	PreferenceDialog * preference;
	SavingDialog * saving;
	QSignalMapper * mapper;
	TestServer * tuServer;
	std::shared_ptr< SikuliClient > client;
	QTimer * buttonTimer;
	CommandTable commands;
	Command doButtonCallback;
	std::function< void ( const QString & ) > doCaptureCallback;
	std::shared_ptr< TestCase > testcase;
	bool modified;

public slots:
	void onConfigure();
	void onRun();
	void onRunRegression();
	void onNew();
	void onOpen();
	void onSave();
	void onSaveAs();
	void onButtonTimeout();
	void onButtonPressed( const QString & );
	void onEFRChanged( int );
	void onPOEdited( const QString & );
	void onTextChanged();
	void onTypeClicked();
	void onSikuliClientError( const QString & );
	void onSikuliClientLog( const QString & );
	void onSikuliClientReady();
	void onSikuliClientCaptured( const QString & );
	void onSikuliClientExecuted( bool, const QString & );
	void onSikuliClientTaskCompleted( const QString &, bool, const QString & );
	void onSikuliClientBundlesExecuted();
	void onSikuliClientCheck( int id, const xtitan::CheckPoint & cp );
	void onSikuliClientAsyncCheck( int id, const xtitan::AsyncCheckPoint & acp );
	void onSikuliClientInput( int id, const xtitan::InputPoint & ip );
	void onSpawnClientClicked();
	void onTUServerAsyncCheck( int id, const xtitan::AsyncCheckPoint & acp );
	void onTUServerCheck( int id, const xtitan::CheckPoint & cp );
	void onTUServerInput( int id, int delay, const xtitan::InputPoint & ip );
	void onRestartSikuliServer();
	void onReopenTestCaseManager();
	void onRecreateTestCaseDatabase();
	void showAbout();
};

}

#endif

/*
Copyright (c) 2011 Wei Cheng Pan <legnaleurc@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef KRAPTURE_MAINWINDOW_PRIVATE_HPP
#define KRAPTURE_MAINWINDOW_PRIVATE_HPP

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
#include "TestUnit/TestUnitServer.hpp"

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
	TestUnitServer * tuServer;
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
	void onSikuliClientCheck( int id, const QString & label, const QString & value );
	void onSikuliClientInput( int id, const QString & object, const QString & method, const QVariantList & args );
	void onSpyInputClicked();
	void onTUServerCheck( int id, const QString & label, const QString & value );
	void onTUServerInput( int id, int delay, const QString & object, const QString & method, const QStringList & args );
	void onRestartSikuliServer();
	void onReopenTestCaseManager();
	void onRecreateTestCaseDatabase();
	void showAbout();
};

}

#endif

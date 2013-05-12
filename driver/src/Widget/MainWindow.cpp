#include "MainWindow_p.hpp"

#include <algorithm>
#include <cassert>

#include <QtGui/QApplication>
#include <QtGui/QImage>
#include <QtGui/QMessageBox>

#include "xTitan/Exception/IOError.hpp"
#include "xTitan/Exception/KeyError.hpp"
#include "xTitan/Utility/Setting.hpp"
#include "TestCase/TestCaseManager.hpp"


namespace i18n {

	const QString aboutWidgetTitle = QObject::tr( "About Krapture" );
	const QString author = QObject::tr( "Author" );
	const QString errorOnSaving = QObject::tr( "Error while saving" );
	const QString license = QObject::tr( "License" );
	const QString version = QObject::tr( "Version" );

}


using xtitan::MainWindow;
using xtitan::Setting;
using xtitan::BasicError;
using xtitan::IOError;
using xtitan::KeyError;


MainWindow::Private::Private( MainWindow * host ):
QObject(),
host( host ),
ui(),
opening( new OpeningDialog( host ) ),
preference( new PreferenceDialog( host ) ),
saving( new SavingDialog( host ) ),
mapper( new QSignalMapper( this ) ),
tuServer( new TestUnitServer( this ) ),
client( new SikuliClient ),
buttonTimer( new QTimer( this ) ),
commands(),
doButtonCallback(),
doCaptureCallback(),
testcase( new TestCase ),
modified( false ) {
	this->ui.setupUi( host );

	this->lockSikuliUI( true );
	this->lockTestCaseUI( true );

	this->buttonTimer->setSingleShot( true );
	this->buttonTimer->setInterval( Setting::instance().get( "capture_waiting", 2000 ).toInt() );

	this->onConfigure();
	this->updateTitle();

	this->ui.textEdit->setTestCase( this->testcase );

	this->connect( this->client.get(), SIGNAL( error( const QString & ) ), SLOT( onSikuliClientError( const QString & ) ) );
	this->connect( this->client.get(), SIGNAL( log( const QString & ) ), SLOT( onSikuliClientLog( const QString & ) ) );
	this->connect( this->client.get(), SIGNAL( ready() ), SLOT( onSikuliClientReady() ) );
	this->connect( this->client.get(), SIGNAL( captured( const QString & ) ), SLOT( onSikuliClientCaptured( const QString & ) ) );
	this->connect( this->client.get(), SIGNAL( executed( bool, const QString & ) ), SLOT( onSikuliClientExecuted( bool, const QString & ) ) );
	this->connect( this->client.get(), SIGNAL( taskCompleted( const QString &, bool, const QString & ) ), SLOT( onSikuliClientTaskCompleted( const QString &, bool, const QString & ) ) );
	this->connect( this->client.get(), SIGNAL( bundlesExecuted() ), SLOT( onSikuliClientBundlesExecuted() ) );
	this->connect( this->client.get(), SIGNAL( checkRequired( int, const QString &, const QString & ) ), SLOT( onSikuliClientCheck( int, const QString &, const QString & ) ) );
	this->connect( this->client.get(), SIGNAL( inputRequired( int, const QString &, const QString &, const QVariantList & ) ), SLOT( onSikuliClientInput( int, const QString &, const QString &, const QVariantList & ) ) );

	this->connect( this->buttonTimer, SIGNAL( timeout() ), SLOT( onButtonTimeout() ) );

	this->connect( this->ui.action_About, SIGNAL( triggered() ), SLOT( showAbout() ) );
	qApp->connect( this->ui.actionAbout_Qt, SIGNAL( triggered() ), SLOT( aboutQt() ) );
	this->connect( this->ui.action_Run, SIGNAL( triggered() ), SLOT( onRun() ) );
	this->connect( this->ui.actionRun_Regression, SIGNAL( triggered() ), SLOT( onRunRegression() ) );
	this->preference->connect( this->ui.action_Preference, SIGNAL( triggered() ), SLOT( show() ) );
	this->connect( this->preference, SIGNAL( accepted() ), SLOT( onConfigure() ) );
	this->connect( this->ui.actionNew, SIGNAL( triggered() ), SLOT( onNew() ) );
	this->connect( this->ui.action_Open, SIGNAL( triggered() ), SLOT( onOpen() ) );
	this->connect( this->ui.action_Save, SIGNAL( triggered() ), SLOT( onSave() ) );
	this->connect( this->ui.actionSave_As, SIGNAL( triggered() ), SLOT( onSaveAs() ) );
	this->connect( this->ui.actionRestart_Sikuli_Server, SIGNAL( triggered() ), SLOT( onRestartSikuliServer() ) );
	this->connect( this->ui.actionReopen_Test_Case_Manager, SIGNAL( triggered() ), SLOT( onReopenTestCaseManager() ) );
	this->connect( this->ui.actionRecreate_Test_Case_Database, SIGNAL( triggered() ), SLOT( onRecreateTestCaseDatabase() ) );
	this->connect( this->ui.programOptions, SIGNAL( textEdited( const QString & ) ), SLOT( onPOEdited( const QString & ) ) );
	this->connect( this->ui.activated, SIGNAL( stateChanged( int ) ), SLOT( onEFRChanged( int ) ) );
	this->connect( this->ui.textEdit, SIGNAL( textChanged() ), SLOT( onTextChanged() ) );
	this->connect( this->ui.type, SIGNAL( clicked() ), SLOT( onTypeClicked() ) );
	this->connect( this->ui.spyInput, SIGNAL( clicked() ), SLOT( onSpyInputClicked() ) );

	// Capture-related buttons behaves similar
	this->connect( this->mapper, SIGNAL( mapped( const QString & ) ), SLOT( onButtonPressed( const QString & ) ) );

	this->mapper->connect( this->ui.capture, SIGNAL( clicked() ), SLOT( map() ) );
	this->mapper->setMapping( this->ui.capture, "capture" );
	this->mapper->connect( this->ui.click, SIGNAL( clicked() ), SLOT( map() ) );
	this->mapper->setMapping( this->ui.click, "click" );
	this->mapper->connect( this->ui.doubleClick, SIGNAL( clicked() ), SLOT( map() ) );
	this->mapper->setMapping( this->ui.doubleClick, "doubleclick" );
	this->mapper->connect( this->ui.dragDrop, SIGNAL( clicked() ), SLOT( map() ) );
	this->mapper->setMapping( this->ui.dragDrop, "draganddrop" );
	this->mapper->connect( this->ui.hover, SIGNAL( clicked() ), SLOT( map() ) );
	this->mapper->setMapping( this->ui.hover, "hover" );
	this->mapper->connect( this->ui.rightClick, SIGNAL( clicked() ), SLOT( map() ) );
	this->mapper->setMapping( this->ui.rightClick, "rightclick" );
	this->mapper->connect( this->ui.wait, SIGNAL( clicked() ), SLOT( map() ) );
	this->mapper->setMapping( this->ui.wait, "wait" );
	this->mapper->connect( this->ui.waitVanish, SIGNAL( clicked() ), SLOT( map() ) );
	this->mapper->setMapping( this->ui.waitVanish, "waitvanish" );

	// For those actions need time interval
	this->commands.insert( std::make_pair( "capture", std::bind( &Private::doCapture, this ) ) );
	this->commands.insert( std::make_pair( "click", std::bind( &Private::doSingle, this, "click" ) ) );
	this->commands.insert( std::make_pair( "doubleclick", std::bind( &Private::doSingle, this, "doubleClick" ) ) );
	this->commands.insert( std::make_pair( "draganddrop", std::bind( &Private::doSingle, this, "dragAndDrop" ) ) );
	this->commands.insert( std::make_pair( "hover", std::bind( &Private::doSingle, this, "hover" ) ) );
	this->commands.insert( std::make_pair( "rightclick", std::bind( &Private::doSingle, this, "rightClick" ) ) );
	this->commands.insert( std::make_pair( "wait", std::bind( &Private::doSingle, this, "wait" ) ) );
	this->commands.insert( std::make_pair( "waitvanish", std::bind( &Private::doSingle, this, "waitVanish" ) ) );

	// Focus should be on edit area
	this->ui.textEdit->setFocus();

	this->connect( this->tuServer, SIGNAL( checkReceived( int, const QString &, const QString & ) ), SLOT( onTUServerCheck( int, const QString &, const QString & ) ) );
	this->connect( this->tuServer, SIGNAL( inputReceived( int, int, const QString &, const QString &, const QStringList & ) ), SLOT( onTUServerInput( int, int, const QString &, const QString &, const QStringList & ) ) );
	if( !this->tuServer->listen( "OracleServer" ) ) {
		QMessageBox::critical( this->host, "", this->tuServer->errorString() );
	}
}

void MainWindow::Private::lockSikuliUI( bool lock ) {
	this->ui.action_Run->setDisabled( lock );
	this->ui.actionRun_Regression->setDisabled( lock );
	this->ui.capture->setDisabled( lock );
	this->ui.click->setDisabled( lock );
	this->ui.doubleClick->setDisabled( lock );
	this->ui.dragDrop->setDisabled( lock );
	this->ui.hover->setDisabled( lock );
	this->ui.rightClick->setDisabled( lock );
	this->ui.type->setDisabled( lock );
	this->ui.wait->setDisabled( lock );
	this->ui.waitVanish->setDisabled( lock );
}

void MainWindow::Private::lockTestCaseUI( bool lock ) {
	this->ui.actionSave_As->setDisabled( lock );
	this->ui.action_Open->setDisabled( lock );
	this->ui.action_Save->setDisabled( lock );
}

void MainWindow::Private::updateTitle() {
	bool hasName = this->testcase && !this->testcase->getName().isEmpty();
	this->host->setWindowTitle( QString( "Krapture - %1" ).arg( hasName ? this->testcase->getName() : QObject::tr( "untitled" ) ) );
}

void MainWindow::Private::onRecreateTestCaseDatabase() {
	QString title( QObject::tr( "Database alert" ) );
	QString content( QObject::tr( "This action will replace database file in %1, all data will be lost and can not reverse, are you sure?" ) );
	content = content.arg( Setting::instance().get( "test_cases_path" ).toString() );
	if( QMessageBox::Yes != QMessageBox::warning( this->host, title, content, QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) ) {
		return;
	}

	try {
		TestCaseManager::instance().create();
	} catch( BasicError & e ) {
		title = QObject::tr( "Database creation failure" );
		QMessageBox::critical( this->host, title, e.getMessage() );
	}
}

void MainWindow::Private::onReopenTestCaseManager() {
	this->host->reopenTestCaseManager();
}

void MainWindow::Private::onRestartSikuliServer() {
	this->host->restartSikuliServer();
}

void MainWindow::Private::onSikuliClientError( const QString & message ) {
	// because beginSikuliAction must called before
	this->endSikuliAction();

	this->ui.textBrowser->append( "[ERROR] " + message );

	if( !this->client->isOpen() ) {
		this->lockSikuliUI( true );
	}
}

void MainWindow::Private::onSikuliClientLog( const QString & message ) {
	this->ui.textBrowser->append( "[LOG] " + message );
}

void MainWindow::Private::onSikuliClientReady() {
	this->lockSikuliUI( false );
}

void MainWindow::Private::onSikuliClientExecuted( bool success, const QString & response ) {
	this->endSikuliAction();

	QString name( ( this->testcase && !this->testcase->getName().isEmpty() ) ? this->testcase->getName() : QObject::tr( "untitled" ) );
	if( success ) {
		// tell all test units to check check points
		if( this->tuServer->check() ) {
			this->ui.textBrowser->append( QObject::tr( "[%1][SUCCEED]" ).arg( name ) );
		} else {
			this->ui.textBrowser->append( QObject::tr( "[%1][FAILED] check points missmatched" ).arg( name ) );
		}
	} else {
		this->ui.textBrowser->append( QObject::tr( "[%1][FAILED] %2" ).arg( name ).arg( response ) );
	}

	if( !this->client->isOpen() ) {
		this->lockSikuliUI( true );
	}
}

void MainWindow::Private::onSikuliClientTaskCompleted( const QString & name, bool success, const QString & response ) {
	if( success ) {
		this->ui.textBrowser->append( QObject::tr( "[%1][SUCCEED]" ).arg( name ) );
	} else {
		this->ui.textBrowser->append( QObject::tr( "[%1][FAILED] %2" ).arg( name ).arg( response ) );
	}
}

void MainWindow::Private::onSikuliClientBundlesExecuted() {
	this->endSikuliAction();
	if( !this->client->isOpen() ) {
		this->lockSikuliUI( true );
	}
}

void MainWindow::Private::onConfigure() {
	QTextDocument * document = this->ui.textEdit->document();
	document->setDefaultFont( Setting::instance().get( "font", QFont( "Lucida Console", 12 ) ).value< QFont >() );
}

void MainWindow::Private::onButtonTimeout() {
	this->doButtonCallback();
	this->doButtonCallback = nullptr;
}

void MainWindow::Private::onRun() {
	this->beginSikuliAction();
	QString script( this->ui.textEdit->dumpScriptWithMapping().join( "\n" ) );
	this->client->execute( script, this->ui.programOptions->text() );
}

void MainWindow::Private::onRunRegression() {
	if( this->modified ) {
		QMessageBox::StandardButton ret = QMessageBox::warning( this->host, QObject::tr( "Modification detected" ), QObject::tr( "Current change is not saved yet, are you sure you want to run regression?" ), QMessageBox::Ok | QMessageBox::Cancel );
		if( ret != QMessageBox::Ok ) {
			return;
		}
	}

	auto testcases( TestCaseManager::instance().getActivated() );

	this->beginSikuliAction();
	this->client->executeBundles( testcases );
}

void MainWindow::Private::onNew() {
	if( this->modified ) {
		QMessageBox::StandardButton ret = QMessageBox::warning( this->host, QObject::tr( "Modification detected" ), QObject::tr( "Current change is not saved yet, are you sure you want to open new test case?" ), QMessageBox::Ok | QMessageBox::Cancel );
		if( ret != QMessageBox::Ok ) {
			return;
		}
	}

	this->testcase.reset( new TestCase );
	this->ui.textEdit->setTestCase( this->testcase );
	this->ui.textEdit->clear();
	this->ui.activated->setChecked( this->testcase->isActivated() );

	this->updateTitle();
	this->modified = false;
}

void MainWindow::Private::onOpen() {
	if( this->opening->exec() != QDialog::Accepted ) {
		return;
	}

	if( this->modified ) {
		QMessageBox::StandardButton ret = QMessageBox::warning( this->host, QObject::tr( "Modification detected" ), QObject::tr( "Current change is not saved yet, are you sure you want to open new test case?" ), QMessageBox::Ok | QMessageBox::Cancel );
		if( ret != QMessageBox::Ok ) {
			return;
		}
	}

	try {
		this->testcase = TestCaseManager::instance().load( this->opening->getSelected() );
	} catch( BasicError & e ) {
		QMessageBox::critical( this->host, QObject::tr( "Can not open test case" ), e.getMessage() );
		return;
	}
	this->ui.textEdit->setTestCase( this->testcase );

	this->ui.textEdit->setHtml( this->testcase->getHTML() );

	this->ui.programOptions->setText( this->testcase->getProgramOptions() );

	this->ui.activated->setChecked( this->testcase->isActivated() );

	this->updateTitle();
	this->modified = false;
}

void MainWindow::Private::onSave() {
	if( this->testcase->getName().isEmpty() ) {
		this->onSaveAs();
		return;
	}

	this->testcase->setHTML( this->ui.textEdit->toHtml() );
	this->testcase->setProgramOptions( this->ui.programOptions->text() );
	this->testcase->setScript( this->ui.textEdit->dumpScript().join( "\n" ) );
	this->testcase->setActivated( this->ui.activated->checkState() == Qt::Checked );

	try {
		TestCaseManager::instance().save( this->testcase );
	} catch( BasicError & e ) {
		QMessageBox::critical( this->host, i18n::errorOnSaving, e.getMessage() );
		return;
	}

	this->updateTitle();
	this->modified = false;
}

void MainWindow::Private::onSaveAs() {
	if( this->saving->exec() != QDialog::Accepted ) {
		return;
	}
	this->testcase->setName( this->saving->getName() );
	this->testcase->setCreator( this->saving->getCreator() );
	this->testcase->setLevel( this->saving->getLevel() );
	this->testcase->setDescription( this->saving->getDescription() );
	this->testcase->setHTML( this->ui.textEdit->toHtml() );
	this->testcase->setProgramOptions( this->saving->getProgramOptions() );
	this->testcase->setScript( this->ui.textEdit->dumpScript().join( "\n" ) );
	this->testcase->setActivated( this->ui.activated->checkState() == Qt::Checked );

	try {
		TestCaseManager::instance().save( this->testcase );
	} catch( BasicError & e ) {
		QMessageBox::critical( this->host, i18n::errorOnSaving, e.getMessage() );
		return;
	}

	this->updateTitle();
	this->modified = false;
}

void MainWindow::Private::onButtonPressed( const QString & command ) {
	if( this->buttonTimer->isActive() ) {
		// there is another command
		return;
	}

	CommandTable::const_iterator it = this->commands.find( command );
	if( it == this->commands.end() ) {
		QMessageBox::critical( this->host, QObject::tr( "Internal Error" ), QObject::tr( "The `%1\' action is invalid" ).arg( command ) );
		return;
	}
	this->doButtonCallback = it->second;

	this->beginSikuliAction();
	this->buttonTimer->start();
}

void MainWindow::Private::onEFRChanged( int ) {
	this->onTextChanged();
}

void MainWindow::Private::onPOEdited( const QString & ) {
	this->onTextChanged();
}

void MainWindow::Private::onTextChanged() {
	if( !this->modified ) {
		this->modified = true;
		this->host->setWindowTitle( this->host->windowTitle() + " *" );
	}
}

void MainWindow::Private::onTypeClicked() {
	this->ui.textEdit->insertTypeCommand();
	this->ui.textEdit->setFocus();
}

void MainWindow::Private::onSpyInputClicked() {
	if( this->tuServer->isRecording() ) {
		this->tuServer->stopRecording();
	} else {
		this->tuServer->startRecording();
	}
}

void MainWindow::Private::onSikuliClientCaptured( const QString & path ) {
	this->doCaptureCallback( path );
	this->doCaptureCallback = nullptr;
	this->endSikuliAction();
}

void MainWindow::Private::doCapture() {
	this->doCaptureCallback = std::bind( &TextEdit::insertImage, this->ui.textEdit, std::placeholders::_1 );
	this->client->capture();
}

void MainWindow::Private::doSingle( const QString & name ) {
	this->doCaptureCallback = std::bind( &TextEdit::insertSingleCommand, this->ui.textEdit, name, std::placeholders::_1 );
	this->client->capture();
}

void MainWindow::Private::beginSikuliAction() {
	this->lockSikuliUI( true );

	// hide the main window
	if( !this->host->isMinimized() ) {
		this->host->setWindowState( this->host->windowState() | Qt::WindowMinimized );
	}
}

void MainWindow::Private::endSikuliAction() {
	// restore window
	if( this->host->isMinimized() ) {
		this->host->setWindowState( this->host->windowState() ^ Qt::WindowMinimized );
		this->host->activateWindow();
	}

	this->lockSikuliUI( false );
}

void MainWindow::Private::showAbout() {
	static const QString aboutWidgetBody( QString( "<table>"
		"<tr><th>%1</th><td>Wei Cheng Pan &lt;legnaleurc@gmail.com&gt;</td></tr>"
		"<tr><th>%2</th><td>%4</td></tr>"
		"<tr><th>%3</th><td>MIT</td></tr>"
		"</table>" ).arg( i18n::author ).arg( i18n::version ).arg( i18n::license ).arg( qApp->applicationVersion() ) );
	QMessageBox::information( this->host, i18n::aboutWidgetTitle, aboutWidgetBody );
}

void MainWindow::Private::onTUServerCheck( int id, const QString & label, const QString & value ) {
	this->ui.textEdit->insertSpyCheck( id, label, value );
}

void MainWindow::Private::onTUServerInput( int id, int delay, const QString & object, const QString & method, const QStringList & args ) {
	this->ui.textEdit->insertSpyInput( id, delay, object, method, args );
}

void MainWindow::Private::onSikuliClientCheck( int id, const QString & label, const QString & value ) {
	this->tuServer->recordOracle( id, label, value );
}

void MainWindow::Private::onSikuliClientInput( int id, const QString & object, const QString & method, const QVariantList & args ) {
	this->tuServer->sendInput( id, object, method, args );
}

MainWindow::MainWindow():
QMainWindow(),
p_( new Private( this ) ) {
	this->connect( this->p_->ui.action_Exit, SIGNAL( triggered() ), SLOT( close() ) );
}

void MainWindow::restartSikuliServer() {
	if( this->p_->client->isOpen() ) {
		this->p_->client->close();
	}
	try {
		this->p_->client->open();
	} catch( IOError & e ) {
		QMessageBox::critical( this, QObject::tr( "SikuliServer stop running" ), e.getMessage() );
	} catch( KeyError & e ) {
		QMessageBox::critical( this, QObject::tr( "configuration error" ), e.getMessage() );
	}
}

void MainWindow::reopenTestCaseManager() {
	if( TestCaseManager::instance().isOpen() ) {
		TestCaseManager::instance().close();
	}
	try {
		TestCaseManager::instance().open();
		this->p_->lockTestCaseUI( false );
	} catch( BasicError & e ) {
		QMessageBox::critical( this, QObject::tr( "Can not start test case manager" ), e.getMessage() );
		this->p_->lockTestCaseUI( true );
	}
}

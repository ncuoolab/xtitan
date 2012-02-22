#include "MainWindowPrivate.hpp"
#include "xTitan/Utility/Setting.hpp"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QTextCodec>
#include <QtGui/QMessageBox>

using namespace xtitan::widget;
using xtitan::testunit::TestUnitManager;
using xtitan::testcase::TestCase;
using xtitan::utilities::Setting;

MainWindow::Private::Private( MainWindow * host ):
host( host ),
ui(),
selectDialog( new SelectTestDialog( this->host ) ),
saveDialog( new SaveDialog( this->host ) ),
preference( new PreferenceDialog( this->host ) ),
manager( new TestUnitManager( this->host ) ) {
	this->ui.setupUi( this->host );

	this->connect( this->manager, SIGNAL( finished( const xtitan::testcase::TestCase & ) ), SLOT( onRequestSave( const xtitan::testcase::TestCase & ) ) );
	this->connect( this->manager, SIGNAL( log( const QString &, const QString & ) ), SLOT( log( const QString &, const QString & ) ) );
	this->connect( this->manager, SIGNAL( error( const QString &, const QString & ) ), SLOT( error( const QString &, const QString & ) ) );
	this->connect( this->manager, SIGNAL( finished() ), SLOT( onFinished() ) );

	this->connect( this->ui.startRecord, SIGNAL( clicked() ), SLOT( onStartRecord() ) );
	this->connect( this->ui.stopRecord, SIGNAL( clicked() ), SLOT( onStopRecord() ) );
	this->connect( this->ui.testSpecific, SIGNAL( clicked() ), SLOT( onTestSpecific() ) );
	this->connect( this->ui.stopReplay, SIGNAL( clicked() ), SLOT( onStopReplay() ) );
	this->connect( this->ui.update, SIGNAL( clicked() ), SLOT( onUpdate() ) );
	this->preference->connect( this->ui.action_Preference, SIGNAL( triggered() ), SLOT( exec() ) );
}

void MainWindow::Private::log( const QString & type, const QString & message ) {
	QTreeWidgetItem * item = new QTreeWidgetItem( this->ui.log );
	item->setText( 0, type );
	item->setText( 1, message );
	this->ui.log->addTopLevelItem( item );
	this->ui.log->scrollToItem( item );

	if( this->ui.log->model()->rowCount() > 300 ) {
		item = this->ui.log->takeTopLevelItem( 0 );
		delete item;
	}
}

void MainWindow::Private::error( const QString & type, const QString & errorString ) {
	QTreeWidgetItem * item = new QTreeWidgetItem( this->ui.log );
	item->setText( 0, type );
	item->setText( 1, errorString );
	item->setTextColor( 0, QColor::fromRgb( 255, 0, 0 ) );
	item->setTextColor( 1, QColor::fromRgb( 255, 0, 0 ) );
	this->ui.log->addTopLevelItem( item );
	this->ui.log->scrollToItem( item );

	if( this->ui.log->model()->rowCount() > 300 ) {
		item = this->ui.log->takeTopLevelItem( 0 );
		delete item;
	}
}

void MainWindow::Private::onStartRecord() {
	this->lockRecordUI( true );

	this->manager->startRecord( this->ui.clientNumber->value() );
}

void MainWindow::Private::onUpdate() {
	int ret = this->selectDialog->exec();
	if( ret == QDialog::Rejected ) {
		return;
	}
	QStringList tests( this->selectDialog->getCheckedTestCase() );
	if( tests.isEmpty() ) {
		return;
	}

	this->lockRecordUI( true );
	this->manager->startUpdate( tests.at( 0 ) );
}

void MainWindow::Private::onStopRecord() {
	TestCase tc( this->manager->stopRecord() );

	this->showSaveDialog( tc );

}

void MainWindow::Private::onStopReplay() {
	this->manager->stopTest();

	this->ui.stopReplay->setEnabled( false );
}

void MainWindow::Private::onTestSpecific() {
	int ret = this->selectDialog->exec();
	if( ret == QDialog::Rejected ) {
		return;
	}
	this->lockReplayUI( true );
	this->manager->specificTest( this->selectDialog->getCheckedTestCase(), !this->ui.dontKillMe->isChecked() );
}

void MainWindow::Private::onRequestSave( const TestCase & tc ) {
	this->showSaveDialog( tc );
	this->lockRecordUI( false );
}

void MainWindow::Private::onFinished() {
	this->lockRecordUI( false );
	this->lockReplayUI( false );
}

void MainWindow::Private::lockReplayUI( bool lock ) {
	this->ui.testSpecific->setDisabled( lock );
	this->ui.regBtn->setDisabled( lock );
	this->ui.smokeBtn->setDisabled( lock );
	this->ui.stopReplay->setEnabled( lock );
	this->ui.toolBox->setItemEnabled( 0, !lock );
}

void MainWindow::Private::lockRecordUI( bool lock ) {
	this->ui.dontKillMe->setDisabled( lock );
	this->ui.update->setDisabled( lock );
	this->ui.startRecord->setDisabled( lock );
	this->ui.stopRecord->setEnabled( lock );
	this->ui.toolBox->setItemEnabled( 1, !lock );
}

void MainWindow::Private::showSaveDialog( TestCase testCase ) {
	if( this->saveDialog->exec() != QDialog::Accepted ) {
		return;
	}

	QString name( this->saveDialog->getName() );
	QString creator( this->saveDialog->getCreator() );
	QString description( this->saveDialog->getDescription() );

	testCase.setName( name );
	testCase.setCreator( creator );
	testCase.setDescription( description );

	if( !testCase.save() ) {
		QMessageBox::critical( this->host, QObject::tr( "Saving failed" ), QObject::tr( "An unknown error occured." ) );
		return;
	}

	QDir root( Setting::getInstance().get( "TestCasePath" ) );
	QFile fout( root.filePath( QString( "level%1.txt" ).arg( this->saveDialog->getLevel() ) ) );
	fout.open( QIODevice::Text | QIODevice::WriteOnly | QIODevice::Append );
	QTextStream sout( &fout );
	sout.setCodec( QTextCodec::codecForName( "UTF-8" ) );
	sout << name << endl;
	fout.close();
}

MainWindow::MainWindow():
QMainWindow( 0, 0 ),
p_( new Private( this ) ) {
}

void MainWindow::setSingleShot( bool singleShot ) {
	this->p_->manager->setOneShot( singleShot );
}

void MainWindow::startRegression() {
	this->p_->lockReplayUI( true );
	this->p_->manager->regressionTest();
}

void MainWindow::startSmokeTest() {
	this->p_->lockReplayUI( true );
	this->p_->manager->smokeTest();
}

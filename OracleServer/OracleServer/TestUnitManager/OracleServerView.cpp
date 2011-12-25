#include "OracleServerView.h"
#include "ui_oracleserver.h"
#include "TestUnitManager/TestUnitManager.h"
#include "View/QxSelectTestDialog.h"
#include "View/QxSaveDialog.h"
#include "View/QxPreferenceDialog.h"
#include "Setting/Setting.h"
#include "TcfDecoder/QxTestCase.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QTextCodec>
#include <QtGui/QMessageBox>

OracleServerView::OracleServerView()
	: QMainWindow( 0, 0 )
	, ui_( new Ui::OracleServerClass )
	, selectDialog_( new QxSelectTestDialog( this ) )
	, saveDialog_( new QxSaveDialog( this ) )
	, preference_( new QxPreferenceDialog( this ) )
	, manager_( new TestUnitManager( this ) )
{
	this->ui_->setupUi( this );

	this->connect( this->manager_, SIGNAL( finished( const QxTestCase & ) ), SLOT( onRequestSave_( const QxTestCase & ) ) );
	this->connect( this->manager_, SIGNAL( log( const QString &, const QString & ) ), SLOT( log( const QString &, const QString & ) ) );
	this->connect( this->manager_, SIGNAL( error( const QString &, const QString & ) ), SLOT( error( const QString &, const QString & ) ) );
	this->connect( this->manager_, SIGNAL( finished() ), SLOT( onFinished_() ) );

	this->connect( this->ui_->startRecord, SIGNAL( clicked() ), SLOT( onStartRecord_() ) );
	this->connect( this->ui_->stopRecord, SIGNAL( clicked() ), SLOT( onStopRecord_() ) );
	this->connect( this->ui_->testSpecific, SIGNAL( clicked() ), SLOT( onTestSpecific_() ) );
	this->connect( this->ui_->stopReplay, SIGNAL( clicked() ), SLOT( onStopReplay_() ) );
	this->connect( this->ui_->update, SIGNAL( clicked() ), SLOT( onUpdate_() ) );
	this->preference_->connect( this->ui_->action_Preference, SIGNAL( triggered() ), SLOT( exec() ) );
}

void OracleServerView::setSingleShot( bool singleShot ) {
	this->manager_->setOneShot( singleShot );
}

void OracleServerView::log( const QString & type, const QString & message ) {
	QTreeWidgetItem * item = new QTreeWidgetItem( this->ui_->log );
	item->setText( 0, type );
	item->setText( 1, message );
	this->ui_->log->addTopLevelItem( item );
	this->ui_->log->scrollToItem( item );

	if( this->ui_->log->model()->rowCount() > 300 ) {
		item = this->ui_->log->takeTopLevelItem( 0 );
		delete item;
	}
}

void OracleServerView::error( const QString & type, const QString & errorString ) {
	QTreeWidgetItem * item = new QTreeWidgetItem( this->ui_->log );
	item->setText( 0, type );
	item->setText( 1, errorString );
	item->setTextColor( 0, QColor::fromRgb( 255, 0, 0 ) );
	item->setTextColor( 1, QColor::fromRgb( 255, 0, 0 ) );
	this->ui_->log->addTopLevelItem( item );
	this->ui_->log->scrollToItem( item );

	if( this->ui_->log->model()->rowCount() > 300 ) {
		item = this->ui_->log->takeTopLevelItem( 0 );
		delete item;
	}
}

void OracleServerView::addView(QWidget* widget,QString name){
	int index = this->ui_->logTab->addTab(widget, name );
	this->ui_->logTab->setCurrentWidget(widget);	
}

void OracleServerView::removeView(QWidget* widget ){
	int index = this->ui_->logTab->indexOf(widget);
	this->ui_->logTab->removeTab(index);
}

void OracleServerView::onStartRecord_() {
	this->lockRecordUI_( true );

	this->manager_->startRecord( this->ui_->clientNumber->value() );
}

void OracleServerView::onUpdate_() {
	int ret = this->selectDialog_->exec();
	if( ret == QDialog::Rejected ) {
		return;
	}
	QStringList tests( this->selectDialog_->getCheckedTestCase() );
	if( tests.isEmpty() ) {
		return;
	}

	this->lockRecordUI_( true );
	this->manager_->startUpdate( tests.at( 0 ) );
}

void OracleServerView::onStopRecord_() {
	QxTestCase tc( this->manager_->stopRecord() );

	this->showSaveDialog_( tc );

}

void OracleServerView::onStopReplay_() {
	this->manager_->stopTest();

	this->ui_->stopReplay->setEnabled( false );
}

void OracleServerView::onTestSpecific_() {
	int ret = this->selectDialog_->exec();
	if( ret == QDialog::Rejected ) {
		return;
	}
	this->lockReplayUI_( true );
	this->manager_->specificTest( this->selectDialog_->getCheckedTestCase(), !this->ui_->dontKillMe->isChecked() );
}

void OracleServerView::onRequestSave_( const QxTestCase & tc ) {
	this->showSaveDialog_( tc );
	this->lockRecordUI_( false );
}

void OracleServerView::slot_startRegression() {
	this->lockReplayUI_( true );
	this->manager_->RegressionTest();
}

void OracleServerView::slot_startSmokeTest() {
	this->lockReplayUI_( true );
	this->manager_->SmokeTest();
}

void OracleServerView::onFinished_() {
	this->lockRecordUI_( false );
	this->lockReplayUI_( false );
}

void OracleServerView::lockReplayUI_( bool lock ) {
	this->ui_->testSpecific->setDisabled( lock );
	this->ui_->regBtn->setDisabled( lock );
	this->ui_->smokeBtn->setDisabled( lock );
	this->ui_->stopReplay->setEnabled( lock );
	this->ui_->toolBox->setItemEnabled( 0, !lock );
}

void OracleServerView::lockRecordUI_( bool lock ) {
	this->ui_->dontKillMe->setDisabled( lock );
	this->ui_->update->setDisabled( lock );
	this->ui_->startRecord->setDisabled( lock );
	this->ui_->stopRecord->setEnabled( lock );
	this->ui_->toolBox->setItemEnabled( 1, !lock );
}

void OracleServerView::showSaveDialog_( QxTestCase testCase ) {
	if( this->saveDialog_->exec() != QDialog::Accepted ) {
		return;
	}

	QString name( this->saveDialog_->getName() );
	QString creator( this->saveDialog_->getCreator() );
	QString description( this->saveDialog_->getDescription() );

	testCase.setName( name );
	testCase.setCreator( creator );
	testCase.setDescription( description );

	if( !testCase.save() ) {
		QMessageBox::critical( this, QObject::tr( "Saving failed" ), QObject::tr( "An unknown error occured." ) );
		return;
	}

	QDir root( QxSetting::Instance().get( "TestCasePath" ) );
	QFile fout( root.filePath( QString( "level%1.txt" ).arg( this->saveDialog_->getLevel() ) ) );
	fout.open( QIODevice::Text | QIODevice::WriteOnly | QIODevice::Append );
	QTextStream sout( &fout );
	sout.setCodec( QTextCodec::codecForName( "UTF-8" ) );
	sout << name << endl;
	fout.close();
}

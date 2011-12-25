#include "QxPreferenceDialog.h"
#include "ui_QxPreferenceDialog.h"
#include "Setting/Setting.h"

#include <QtGui/QFileDialog>

QxPreferenceDialog::QxPreferenceDialog( QWidget * parent ):
QDialog( parent ),
ui_( new Ui::QxPreferenceDialog ) {
	this->ui_->setupUi( this );

	this->connect( this->ui_->buttonBox, SIGNAL( clicked( QAbstractButton * ) ), SLOT( onButtonPressed_( QAbstractButton * ) ) );
	this->connect( this->ui_->browseRender, SIGNAL( clicked() ), SLOT( onBrowseRender_() ) );
	this->connect( this->ui_->browseShell, SIGNAL( clicked() ), SLOT( onBrowseShell_() ) );
	this->connect( this->ui_->browseConfig, SIGNAL( clicked() ), SLOT( onBrowseConfig_() ) );
	this->connect( this->ui_->browsePortal, SIGNAL( clicked() ), SLOT( onBrowsePortal_() ) );
	this->connect( this->ui_->browseTestcase, SIGNAL( clicked() ), SLOT( onBrowseTestcase_() ) );
}

void QxPreferenceDialog::setVisible( bool visible ) {
	this->QDialog::setVisible( visible );
	if( visible ) {
		// show state
		this->ui_->shellPath->setText( QxSetting::Instance().get( "MsnShellProgram" ) );
		this->ui_->renderPath->setText( QxSetting::Instance().get( "MsnRenderProgram" ) );
		this->ui_->configPath->setText( QxSetting::Instance().get( "ConfigurationPath" ) );
		this->ui_->portalServer->setText( QxSetting::Instance().get( "PortalServer" ) );
		this->ui_->testcasePath->setText( QxSetting::Instance().get( "TestCasePath" ) );
		this->ui_->serverAddress->setText( QxSetting::Instance().get( "ServerAddress" ) );
	} else {
		// hide state
	}
}

void QxPreferenceDialog::onButtonPressed_( QAbstractButton * button ) {
	switch( this->ui_->buttonBox->buttonRole( button ) ) {
		case QDialogButtonBox::AcceptRole:
			this->save_();
			break;
		case QDialogButtonBox::ApplyRole:
			this->save_();
			break;
		case QDialogButtonBox::RejectRole:
			break;
		default:
			;
	}
}

void QxPreferenceDialog::save_() {
	QxSetting::Instance().set( "MsnShellProgram", this->ui_->shellPath->text() );
	QxSetting::Instance().set( "MsnRenderProgram", this->ui_->renderPath->text() );
	QxSetting::Instance().set( "ConfigurationPath", this->ui_->configPath->text() );
	QxSetting::Instance().set( "PortalServer", this->ui_->portalServer->text() );
	QxSetting::Instance().set( "TestCasePath", this->ui_->testcasePath->text() );
	QxSetting::Instance().set( "ServerAddress", this->ui_->serverAddress->text() );
	QxSetting::Instance().save();
}

void QxPreferenceDialog::onBrowseRender_() {
	QString path( QFileDialog::getOpenFileName( this, QObject::tr( "Select MsnRender" ), QDir::currentPath(), QObject::tr( "Executables (*.exe *.bat)" ) ) );
	if( !path.isEmpty() ) {
		this->ui_->renderPath->setText( path );
	}
}

void QxPreferenceDialog::onBrowseShell_() {
	QString path( QFileDialog::getOpenFileName( this, QObject::tr( "Select MsnShell" ), QDir::currentPath(), QObject::tr( "Executables (*.exe *.bat)" ) ) );
	if( !path.isEmpty() ) {
		this->ui_->shellPath->setText( path );
	}
}

void QxPreferenceDialog::onBrowseConfig_() {
	QString path( QFileDialog::getExistingDirectory( this, QObject::tr( "Select a directory which contains configuration" ), QDir::currentPath() ) );
	if( !path.isEmpty() ) {
		this->ui_->configPath->setText( path );
	}
}

void QxPreferenceDialog::onBrowsePortal_() {
	QString path( QFileDialog::getOpenFileName( this, QObject::tr( "Select Portal Server" ), QDir::currentPath(), QObject::tr( "Executables (*.exe *.bat)" ) ) );
	if( !path.isEmpty() ) {
		this->ui_->portalServer->setText( path );
	}
}

void QxPreferenceDialog::onBrowseTestcase_() {
	QString path( QFileDialog::getExistingDirectory( this, QObject::tr( "Select a directory which contains testcases" ), QDir::currentPath() ) );
	if( !path.isEmpty() ) {
		this->ui_->testcasePath->setText( path );
	}
}

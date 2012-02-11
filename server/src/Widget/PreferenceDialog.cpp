#include "PreferenceDialogPrivate.hpp"
#include "xTitan/Utility/Setting.hpp"

#include <QtGui/QFileDialog>

using xtitan::utilities::Setting;
using namespace xtitan::widget;

PreferenceDialog::Private::Private( PreferenceDialog * host ):
QObject( host ),
host( host ),
ui() {
	this->ui.setupUi( host );

	this->connect( this->ui.buttonBox, SIGNAL( clicked( QAbstractButton * ) ), SLOT( onButtonPressed_( QAbstractButton * ) ) );
	this->connect( this->ui.browseRender, SIGNAL( clicked() ), SLOT( onBrowseRender_() ) );
	this->connect( this->ui.browseShell, SIGNAL( clicked() ), SLOT( onBrowseShell_() ) );
	this->connect( this->ui.browseConfig, SIGNAL( clicked() ), SLOT( onBrowseConfig_() ) );
	this->connect( this->ui.browsePortal, SIGNAL( clicked() ), SLOT( onBrowsePortal_() ) );
	this->connect( this->ui.browseTestcase, SIGNAL( clicked() ), SLOT( onBrowseTestcase_() ) );
}

void PreferenceDialog::Private::onButtonPressed( QAbstractButton * button ) {
	switch( this->ui.buttonBox->buttonRole( button ) ) {
		case QDialogButtonBox::AcceptRole:
			this->save();
			break;
		case QDialogButtonBox::ApplyRole:
			this->save();
			break;
		case QDialogButtonBox::RejectRole:
			break;
		default:
			;
	}
}

void PreferenceDialog::Private::save() {
	Setting::getInstance().set( "MsnShellProgram", this->ui.shellPath->text() );
	Setting::getInstance().set( "MsnRenderProgram", this->ui.renderPath->text() );
	Setting::getInstance().set( "ConfigurationPath", this->ui.configPath->text() );
	Setting::getInstance().set( "PortalServer", this->ui.portalServer->text() );
	Setting::getInstance().set( "TestCasePath", this->ui.testcasePath->text() );
	Setting::getInstance().set( "ServerAddress", this->ui.serverAddress->text() );
	Setting::getInstance().save();
}

void PreferenceDialog::Private::onBrowseRender() {
	QString path( QFileDialog::getOpenFileName( this->host, QObject::tr( "Select MsnRender" ), QDir::currentPath(), QObject::tr( "Executables (*.exe *.bat)" ) ) );
	if( !path.isEmpty() ) {
		this->ui.renderPath->setText( path );
	}
}

void PreferenceDialog::Private::onBrowseShell() {
	QString path( QFileDialog::getOpenFileName( this->host, QObject::tr( "Select MsnShell" ), QDir::currentPath(), QObject::tr( "Executables (*.exe *.bat)" ) ) );
	if( !path.isEmpty() ) {
		this->ui.shellPath->setText( path );
	}
}

void PreferenceDialog::Private::onBrowseConfig() {
	QString path( QFileDialog::getExistingDirectory( this->host, QObject::tr( "Select a directory which contains configuration" ), QDir::currentPath() ) );
	if( !path.isEmpty() ) {
		this->ui.configPath->setText( path );
	}
}

void PreferenceDialog::Private::onBrowsePortal() {
	QString path( QFileDialog::getOpenFileName( this->host, QObject::tr( "Select Portal Server" ), QDir::currentPath(), QObject::tr( "Executables (*.exe *.bat)" ) ) );
	if( !path.isEmpty() ) {
		this->ui.portalServer->setText( path );
	}
}

void PreferenceDialog::Private::onBrowseTestcase() {
	QString path( QFileDialog::getExistingDirectory( this->host, QObject::tr( "Select a directory which contains testcases" ), QDir::currentPath() ) );
	if( !path.isEmpty() ) {
		this->ui.testcasePath->setText( path );
	}
}

PreferenceDialog::PreferenceDialog( QWidget * parent ):
QDialog( parent ),
p_( new Private( this ) ) {
}

void PreferenceDialog::setVisible( bool visible ) {
	this->QDialog::setVisible( visible );
	if( visible ) {
		// show state
		this->p_->ui.shellPath->setText( Setting::getInstance().get( "MsnShellProgram" ) );
		this->p_->ui.renderPath->setText( Setting::getInstance().get( "MsnRenderProgram" ) );
		this->p_->ui.configPath->setText( Setting::getInstance().get( "ConfigurationPath" ) );
		this->p_->ui.portalServer->setText( Setting::getInstance().get( "PortalServer" ) );
		this->p_->ui.testcasePath->setText( Setting::getInstance().get( "TestCasePath" ) );
		this->p_->ui.serverAddress->setText( Setting::getInstance().get( "ServerAddress" ) );
	} else {
		// hide state
	}
}

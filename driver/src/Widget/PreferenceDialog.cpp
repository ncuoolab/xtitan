/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#include "PreferenceDialog_p.hpp"

#include <QtGui/QFileDialog>
#include <QtGui/QFontDialog>

#include "xTitan/Utility/Setting.hpp"


using xtitan::PreferenceDialog;


PreferenceDialog::Private::Private( PreferenceDialog * host ):
QObject(),
host( host ),
ui(),
font() {
	this->ui.setupUi( host );

	this->connect( this->ui.browseSSP, SIGNAL( clicked() ), SLOT( onBrowseSSP() ) );
	this->connect( this->ui.browseTCP, SIGNAL( clicked() ), SLOT( onBrowseTCP() ) );
	this->connect( this->ui.browseTPP, SIGNAL( clicked() ), SLOT( onBrowseTPP() ) );
	this->connect( this->ui.changeFont, SIGNAL( clicked() ), SLOT( onChangeFont() ) );
	this->connect( this->ui.buttonBox, SIGNAL( clicked( QAbstractButton * ) ), SLOT( onFinished( QAbstractButton * ) ) );

	this->load();
}

void PreferenceDialog::Private::onBrowseSSP() {
	QString path( QFileDialog::getOpenFileName( this->host, QObject::tr( "Select SikuliServer.jar" ), QString(), QObject::tr( "Java Runnable JAR (*.jar)" ) ) );
	if( path.isEmpty() ) {
		return;
	}
	this->ui.sikuliserverPath->setText( path );
}

void PreferenceDialog::Private::onBrowseTCP() {
	QString path( QFileDialog::getExistingDirectory( this->host, QObject::tr( "Select Test Cases Path" ) ) );
	if( path.isEmpty() ) {
		return;
	}
	path = QDir::fromNativeSeparators( path );
	this->ui.testCasesPath->setText( path );
}

void PreferenceDialog::Private::onBrowseTPP() {
	// TODO filter executables for other OS
	QString path = QFileDialog::getOpenFileName( this->host, QObject::tr( "Select Program to be testing" ), QString() );
	if( path.isEmpty() ) {
		return;
	}
	this->ui.testingPath->setText( path );
}

void PreferenceDialog::Private::onChangeFont() {
	bool ok = false;
	QFont font( QFontDialog::getFont( &ok, this->font, this->host ) );
	if( !ok ) {
		return;
	}
	this->font = font;
	this->ui.font->setText( this->font.toString() );
}

void PreferenceDialog::Private::onFinished( QAbstractButton * button ) {
	switch( this->ui.buttonBox->buttonRole( button ) ) {
	case QDialogButtonBox::AcceptRole:
	case QDialogButtonBox::ApplyRole:
		this->save();
		break;
	case QDialogButtonBox::RejectRole:
		this->load();
		break;
	default:
		;
	}
}

void PreferenceDialog::Private::load() {
	QDir appDir = QCoreApplication::instance()->applicationDirPath();

	this->ui.captureWaiting->setValue( Setting::instance().get( "capture_waiting", 2000 ).toInt() );
	this->font = Setting::instance().get( "font", QFont( "Lucida Console", 12 ) ).value< QFont >();
	this->ui.font->setText( this->font.toString() );
	this->ui.sikuliserverPath->setText( Setting::instance().get( "sikuliserver_path", appDir.filePath( "SikuliServer.jar" ) ).toString() );
	this->ui.testCasesPath->setText( Setting::instance().get( "test_cases_path", appDir.filePath( "TestCases" ) ).toString() );
	this->ui.testingPath->setText( Setting::instance().get( "testing_program_path", QString() ).toString() );
	this->ui.testCaseInterval->setValue( Setting::instance().get( "test_case_interval", 1500 ).toInt() );
}

void PreferenceDialog::Private::save() {
	Setting::instance().set( "capture_waiting", this->ui.captureWaiting->value() );
	Setting::instance().set( "font", this->font );
	Setting::instance().set( "sikuliserver_path", this->ui.sikuliserverPath->text() );
	Setting::instance().set( "test_cases_path", this->ui.testCasesPath->text() );
	Setting::instance().set( "testing_program_path", this->ui.testingPath->text() );
	Setting::instance().set( "test_case_interval", this->ui.testCaseInterval->value() );
}

PreferenceDialog::PreferenceDialog( QWidget * parent ):
QDialog( parent ),
p_( new Private( this ) ) {
}

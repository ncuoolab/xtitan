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

	this->connect( this->ui.buttonBox, SIGNAL( clicked( QAbstractButton * ) ), SLOT( onButtonPressed( QAbstractButton * ) ) );
	this->connect( this->ui.browseSoftware, SIGNAL( clicked() ), SLOT( onBrowseSoftware() ) );
	this->connect( this->ui.browseTestcase, SIGNAL( clicked() ), SLOT( onBrowseTestcase() ) );
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
	Setting::getInstance().set( "SoftwarePath", this->ui.softwarePath->text() );
	Setting::getInstance().set( "TestCasePath", this->ui.testcasePath->text() );
	Setting::getInstance().save();
}

void PreferenceDialog::Private::onBrowseSoftware() {
	QString path( QFileDialog::getOpenFileName( this->host, QObject::tr( "Select testing software" ), QDir::currentPath(), QObject::tr( "Executables (*.exe *.bat)" ) ) );
	if( !path.isEmpty() ) {
		this->ui.softwarePath->setText( path );
	}
}

void PreferenceDialog::Private::onBrowseTestcase() {
	QString path( QFileDialog::getExistingDirectory( this->host, QObject::tr( "Select a directory which contains testcases" ), QDir::currentPath() ) );
	if( !path.isEmpty() ) {
		this->ui.testcasePath->setText( QDir::fromNativeSeparators( path ) );
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
		this->p_->ui.softwarePath->setText( Setting::getInstance().get( "SoftwarePath" ).toString() );
		this->p_->ui.testcasePath->setText( Setting::getInstance().get( "TestCasePath" ).toString() );
	} else {
		// hide state
	}
}

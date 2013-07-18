/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#include "SavingDialog_p.hpp"

#include <QtGui/QPushButton>


using xtitan::SavingDialog;


SavingDialog::Private::Private( SavingDialog * host ):
QObject(),
ui() {
	this->ui.setupUi( host );

	this->connect( this->ui.name, SIGNAL( textEdited( const QString & ) ), SLOT( onNameEdited( const QString & ) ) );
}

void SavingDialog::Private::onNameEdited( const QString & /*text*/ ) {
	QPushButton * ok = this->ui.buttonBox->button( QDialogButtonBox::Ok );
	bool empty = this->ui.name->text().isEmpty();
	if( empty && ok->isEnabled() ) {
		ok->setEnabled( false );
	}
	if( !empty && !ok->isEnabled() ) {
		ok->setEnabled( true );
	}
}

SavingDialog::SavingDialog( QWidget * parent ):
QDialog( parent ),
p_( new Private( this ) ) {
}

void SavingDialog::setVisible( bool visible ) {
	if( visible ) {
		// clean up last input
		this->p_->ui.name->clear();
		this->p_->ui.creator->clear();
		this->p_->ui.level->setValue( 0 );
		this->p_->ui.programOptions->clear();
		this->p_->ui.description->clear();
		// empty dialog should not be submitable
		this->p_->ui.buttonBox->button( QDialogButtonBox::Ok )->setEnabled( false );
	}
	this->QDialog::setVisible( visible );
}

QString SavingDialog::getCreator() const {
	return this->p_->ui.creator->text();
}

QString SavingDialog::getDescription() const {
	return this->p_->ui.description->toPlainText();
}

int SavingDialog::getLevel() const {
	return this->p_->ui.level->value();
}

QString SavingDialog::getName() const {
	return this->p_->ui.name->text();
}

QString SavingDialog::getProgramOptions() const {
	return this->p_->ui.programOptions->text();
}

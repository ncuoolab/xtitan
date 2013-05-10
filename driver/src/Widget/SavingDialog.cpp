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

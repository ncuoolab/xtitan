#include "QxSaveDialog.h"
#include "ui_QxSaveDialog.h"

QxSaveDialog::QxSaveDialog( QWidget * parent ):
QDialog( parent ),
ui_( new Ui::QxSaveDialog ) {
	this->ui_->setupUi( this );
}

QString QxSaveDialog::getName() const {
	return this->ui_->name->text();
}

QString QxSaveDialog::getCreator() const {
	return this->ui_->creator->text();
}

QString QxSaveDialog::getDescription() const {
	return this->ui_->description->toPlainText();
}

int QxSaveDialog::getLevel() const {
	return this->ui_->level->currentIndex();
}

void QxSaveDialog::setVisible( bool visible ) {
	if( visible ) {
		this->ui_->name->clear();
		this->ui_->creator->clear();
		this->ui_->description->clear();
	}
	this->QDialog::setVisible( visible );
}

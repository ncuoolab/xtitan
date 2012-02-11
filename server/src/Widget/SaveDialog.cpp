#include "SaveDialogPrivate.hpp"

using namespace xtitan::widget;

SaveDialog::Private::Private( SaveDialog * host ):
host( host ),
ui() {
	this->ui.setupUi( host );
}

SaveDialog::SaveDialog( QWidget * parent ):
QDialog( parent ),
p_( new Private( this ) ) {
}

QString SaveDialog::getName() const {
	return this->p_->ui.name->text();
}

QString SaveDialog::getCreator() const {
	return this->p_->ui.creator->text();
}

QString SaveDialog::getDescription() const {
	return this->p_->ui.description->toPlainText();
}

int SaveDialog::getLevel() const {
	return this->p_->ui.level->currentIndex();
}

void SaveDialog::setVisible( bool visible ) {
	if( visible ) {
		this->p_->ui.name->clear();
		this->p_->ui.creator->clear();
		this->p_->ui.description->clear();
	}
	this->QDialog::setVisible( visible );
}

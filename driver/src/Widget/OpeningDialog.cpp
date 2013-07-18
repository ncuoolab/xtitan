/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#include "OpeningDialog_p.hpp"

#include "xTitan/Exception/IOError.hpp"
#include "TestCase/TestCaseManager.hpp"


using xtitan::OpeningDialog;
using xtitan::IOError;


OpeningDialog::Private::Private( OpeningDialog * host ):
QObject( host ),
host( host ),
ui() {
	this->ui.setupUi( host );

	this->ui.treeView->setModel( &TestCaseManager::instance() );

	this->connect( this->ui.deleteButton, SIGNAL( clicked() ), SLOT( onDeletePressed() ) );
}

void OpeningDialog::Private::onDeletePressed() {
	QModelIndex index( this->ui.treeView->currentIndex() );
	if( !index.isValid() ) {
		return;
	}
	TestCaseManager::instance().deleteByIndex( index );
}

OpeningDialog::OpeningDialog( QWidget * parent ):
QDialog( parent ),
p_( new Private( this ) ) {
}

QString OpeningDialog::getSelected() const {
	QModelIndex index( this->p_->ui.treeView->currentIndex() );
	if( !index.isValid() ) {
		throw IOError( QObject::tr( "index out of range" ) );
	}
	return TestCaseManager::instance().getName( index );
}

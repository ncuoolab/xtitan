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

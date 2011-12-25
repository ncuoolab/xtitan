#include "QxSelectTestDialog.h"
#include "ui_QxSelectTestDialog.h"

#include "Setting/Setting.h"

#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QTextStream>

#include <QtCore/QtDebug>

namespace {
	QStringList readFile( const QString & filePath ) {
		QFile fin( filePath );
		fin.open( QIODevice::ReadOnly | QIODevice::Text );
		QTextStream sin( &fin );
		QStringList result;
		while( !sin.atEnd() ) {
			result.append( sin.readLine() );
		}
		fin.close();
		return result;
	}
}

QxSelectTestDialog::QxSelectTestDialog( QWidget * parent ):
QDialog( parent ),
ui_( new Ui::QxSelectTestDialog ) {
	this->ui_->setupUi( this );

	this->connect( this->ui_->listWidget, SIGNAL( itemDoubleClicked( QListWidgetItem * ) ), SLOT( onDoubleClicked_( QListWidgetItem * ) ) );
}

void QxSelectTestDialog::setVisible( bool visible ) {
	this->QDialog::setVisible( visible );
	if( !visible ) {
		return;
	}
	this->ui_->listWidget->clear();

	QDir tcDir( QxSetting::Instance().get( "TestCasePath" ) );
	QStringList files;
	files << readFile( tcDir.filePath( "level0.txt" ) );
	files << readFile( tcDir.filePath( "level1.txt" ) );
	files << readFile( tcDir.filePath( "level2.txt" ) );
	foreach( QString file, files ) {
		QListWidgetItem * item = new QListWidgetItem( file, this->ui_->listWidget );
		item->setCheckState( Qt::Unchecked );
		item->setData( Qt::UserRole, file );
		this->ui_->listWidget->addItem( item );
	}
}

QStringList QxSelectTestDialog::getCheckedTestCase() const {
	QStringList result;
	for( int i = 0; i < this->ui_->listWidget->count(); ++i ) {
		QListWidgetItem * item = this->ui_->listWidget->item( i );
		if( item->checkState() == Qt::Checked ) {
			result.append( item->data( Qt::UserRole ).toString() );
		}
	}
	return result;
}

void QxSelectTestDialog::onDoubleClicked_( QListWidgetItem * item ) {
	if( item->checkState() != Qt::Unchecked ) {
		item->setCheckState( Qt::Unchecked );
	} else {
		item->setCheckState( Qt::Checked );
	}
}

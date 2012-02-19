#include "SelectTestDialogPrivate.hpp"

#include "xTitan/Utility/Setting.hpp"

#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QTextStream>

#include <QtCore/QtDebug>

using xtitan::utilities::Setting;
using namespace xtitan::widget;

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

SelectTestDialog::Private::Private( SelectTestDialog * host ):
QObject( host ),
host( host ) {
	this->ui.setupUi( host );

	this->connect( this->ui.listWidget, SIGNAL( itemDoubleClicked( QListWidgetItem * ) ), SLOT( onDoubleClicked( QListWidgetItem * ) ) );
}

void SelectTestDialog::Private::onDoubleClicked( QListWidgetItem * item ) {
	if( item->checkState() != Qt::Unchecked ) {
		item->setCheckState( Qt::Unchecked );
	} else {
		item->setCheckState( Qt::Checked );
	}
}

SelectTestDialog::SelectTestDialog( QWidget * parent ):
QDialog( parent ),
p_( new Private( this ) ) {
}

void SelectTestDialog::setVisible( bool visible ) {
	this->QDialog::setVisible( visible );
	if( !visible ) {
		return;
	}
	this->p_->ui.listWidget->clear();

	QDir tcDir( Setting::getInstance().get( "TestCasePath" ).toString() );
	QStringList files;
	files << readFile( tcDir.filePath( "level0.txt" ) );
	files << readFile( tcDir.filePath( "level1.txt" ) );
	files << readFile( tcDir.filePath( "level2.txt" ) );
	foreach( QString file, files ) {
		QListWidgetItem * item = new QListWidgetItem( file, this->p_->ui.listWidget );
		item->setCheckState( Qt::Unchecked );
		item->setData( Qt::UserRole, file );
		this->p_->ui.listWidget->addItem( item );
	}
}

QStringList SelectTestDialog::getCheckedTestCase() const {
	QStringList result;
	for( int i = 0; i < this->p_->ui.listWidget->count(); ++i ) {
		QListWidgetItem * item = this->p_->ui.listWidget->item( i );
		if( item->checkState() == Qt::Checked ) {
			result.append( item->data( Qt::UserRole ).toString() );
		}
	}
	return result;
}

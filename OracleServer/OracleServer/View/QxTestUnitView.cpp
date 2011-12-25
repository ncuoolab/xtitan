#include "QxTestUnitView.h"
#include "ui_QxTestUnitView.h"
#include "TcfDecoder/QxInputPoint.h"
#include "TcfDecoder/QxCheckPoint.h"
#include "TCP_Connection/TestClient.h"

QxTestUnitView::QxTestUnitView():
QWidget( 0 ),
ui_( new Ui::QxTestUnitView ) {
	this->ui_->setupUi( this );
}

void QxTestUnitView::appendLog( const QString & msg ) {
	this->ui_->log->append( msg );
}

void QxTestUnitView::appendInput( const QxInputPoint & input ) {
	QTreeWidgetItem * item = new QTreeWidgetItem( this->ui_->input );
	item->setText( 0, QString::number( input.getID() ) );
	item->setText( 1, input.getLabel() );
	item->setText( 2, input.getScript() );
	item->setText( 3, QString::number( input.getWaitTime() ) );
	this->ui_->input->addTopLevelItem( item );
	this->ui_->input->setCurrentItem( item );
}

void QxTestUnitView::setCurrentLine( int line ) {
	this->ui_->input->setCurrentItem( this->ui_->input->topLevelItem( line ) );
}

#include "TextEdit_p.hpp"

#include <algorithm>

#include <QtGui/QContextMenuEvent>
#include <QtGui/QMenu>
#include <QtGui/QTextDocument>
#include <QtGui/QTextCursor>
#include <QtGui/QTextFrame>
#include <QtGui/QTextBlock>
#include <QtGui/QInputDialog>
#include <QtGui/QMessageBox>
#include <QtCore/QUrl>
#include <QtCore/QDir>


using xtitan::TestCase;
using xtitan::TextEdit;


TextEdit::Private::Private( TextEdit * host ):
QObject( 0 ),
host( host ),
rename( new QAction( QObject::tr( "Rename Image" ), host ) ),
testCase(),
currentCursor() {
	this->connect( this->rename, SIGNAL( triggered() ), SLOT( onRenameTriggered() ) );
}

QPair< QString, QSize > TextEdit::Private::addImageResource( const QString & path ) {
	QFileInfo info( path );
	QUrl uri( QUrl::fromLocalFile( path ) );

	this->testCase->addImageMapping( info.fileName(), uri );
	return this->addImageResource( info.fileName(), uri );
}

QStringList TextEdit::Private::traverse( std::function< QString ( const QString & ) > mapping ) const {
	QStringList script;
	QTextBlock block( this->host->document()->begin() );
	while( block.isValid() ) {
		QString line;
		for( QTextBlock::iterator it = block.begin(); !it.atEnd(); ++it ) {
			QTextFragment fragment( it.fragment() );
			if( !fragment.isValid() ) {
				continue;
			}
			if( fragment.charFormat().isImageFormat() ) {
				QString name( fragment.charFormat().toImageFormat().name() );
				line.append( "\'" + mapping( name ) + "\'" );
			} else {
				line.append( fragment.text() );
			}
		}
		script.append( line );
		block = block.next();
	}
	return script;
}

void TextEdit::Private::replaceImage( std::map< QString, QUrl > & mapping, const QFileInfo & oldP, const QFileInfo & newP ) {
	QVariant r( this->host->document()->resource( QTextDocument::ImageResource, QUrl( oldP.fileName() ) ) );
	this->host->document()->addResource( QTextDocument::ImageResource, QUrl( oldP.fileName() ), QVariant() );
	this->host->document()->addResource( QTextDocument::ImageResource, QUrl( newP.fileName() ), r );

	this->currentCursor.deletePreviousChar();
	QTextImageFormat format;
	format.setName( newP.fileName() );
	format.setToolTip( newP.fileName() );
	this->currentCursor.insertImage( format );

	mapping.erase( oldP.fileName() );
	mapping.insert( std::make_pair( newP.fileName(), QUrl::fromLocalFile( newP.absoluteFilePath() ) ) );
	this->testCase->setImageMapping( mapping );
}

/*
QTextEdit::cursorForPosition returns a cursor before the image if clicked in
left-half, so we need some additional check.
*/
bool TextEdit::Private::imageForPosition( const QPoint & pos ) {
	bool isImage = false;
	QTextCursor cursor( this->host->cursorForPosition( pos ) );
	if( cursor.charFormat().isImageFormat() ) {
		// here has two posibility: clicked on right-half or
		// clicked on left-half but there is a image at left.
		// check next cursor and determine nearest image.
		int w = cursor.charFormat().toImageFormat().width();
		int x1 = this->host->cursorRect( cursor ).x();
		int x2 = cursor.atStart() ? x1 + w : x1 - w;
		if( x1 > x2 ) {
			std::swap( x1, x2 );
		}
		if( x1 < pos.x() && pos.x() < x2 ) {
			isImage = true;
		}
	}
	if( !isImage ) {
		bool ok = cursor.movePosition( QTextCursor::NextCharacter );
		if( ok && cursor.charFormat().isImageFormat() ) {
			isImage = true;
		}
	}
	if( isImage ) {
		this->currentCursor = cursor;
	}
	return isImage;
}

void TextEdit::Private::onRenameTriggered() {
	QTextImageFormat format( this->currentCursor.charFormat().toImageFormat() );
	std::map< QString, QUrl > mapping( this->testCase->getImageMapping() );
	std::map< QString, QUrl >::iterator it = mapping.find( format.name() );
	QFileInfo picture( it->second.toLocalFile() );

	QString newName( QInputDialog::getText( this->host->parentWidget(), QObject::tr( "Rename" ), QObject::tr( "Rename %1 to:" ).arg( picture.completeBaseName() ) ) );
	if( newName.isEmpty() || newName == picture.completeBaseName() ) {
		return;
	}

	QFileInfo newPicture( picture.dir().filePath( newName + ".png" ) );
	bool ok = QFile::rename( picture.filePath(), newPicture.filePath() );
	if( ok ) {
		this->replaceImage( mapping, picture, newPicture );
		return;
	}
	QMessageBox::StandardButton b = QMessageBox::warning( this->host->parentWidget(), QObject::tr( "Rename Problem" ), QObject::tr( "This name will overwrite an exist one. Are you sure?" ) );
	if( b != QMessageBox::Ok ) {
		this->onRenameTriggered();
		return;
	}
	QFile::remove( newPicture.filePath() );
	QFile::rename( picture.filePath(), newPicture.filePath() );
	this->replaceImage( mapping, picture, newPicture );
}

TextEdit::TextEdit( QWidget * parent ):
QTextEdit( parent ),
p_( new Private( this ) ) {
}

QPair< QString, QSize > TextEdit::Private::addImageResource( const QString & fileName, const QUrl & uri ) {
	QTextDocument * document = this->host->document();
	QImage image( QImage( uri.toLocalFile() ).scaled( 50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation ) );
	document->addResource( QTextDocument::ImageResource, QUrl( fileName ), image );

	return qMakePair( fileName, image.size() );
}

void TextEdit::insertImage( const QString & path ) {
	QPair< QString, QSize > imgKey( this->p_->addImageResource( path ) );

	QTextCursor cursor( this->textCursor() );

	QTextImageFormat format;
	format.setName( imgKey.first );
	format.setToolTip( imgKey.first );
	format.setWidth( imgKey.second.width() );
	format.setHeight( imgKey.second.height() );
	cursor.insertImage( format );

	this->setTextCursor( cursor );
}

void TextEdit::insertSingleCommand( const QString & name, const QString & path ) {
	QPair< QString, QSize > imgKey( this->p_->addImageResource( path ) );

	QTextCursor cursor( this->textCursor() );

	cursor.insertText( name );
	cursor.insertText( "( " );
	QTextImageFormat format;
	format.setName( imgKey.first );
	format.setToolTip( imgKey.first );
	format.setWidth( imgKey.second.width() );
	format.setHeight( imgKey.second.height() );
	cursor.insertImage( format );
	cursor.insertText( " )\n" );

	this->setTextCursor( cursor );
}

void TextEdit::insertSpyCheck( int id, const QString & feature, const QString & value ) {
	auto cursor = this->textCursor();
	QStringList tmp;
	QString tpl = "\'%1\'";

	tmp.append( QString::number( id ) );
//	tmp.append( tpl.arg( label ) );
	tmp.append( tpl.arg( value ) );

	cursor.insertText( feature );
	cursor.insertText( "( " );
	cursor.insertText( tmp.join( ", " ) );
	cursor.insertText( " )\n" );
}

void TextEdit::insertSpyInput( int id, int delay, const QString & object, const QString & method, const QStringList & args ) {
	auto cursor = this->textCursor();
	QStringList tmp;
	QString tpl = "\'%1\'";

	tmp.append( QString::number( id ) );
	tmp.append( QString::number( delay ) );
	tmp.append( tpl.arg( object ) );
	tmp.append( tpl.arg( method ) );
	tmp.append( args );

	cursor.insertText( "spyInput" );
	cursor.insertText( "( " );
	cursor.insertText( tmp.join( ", " ) );
	cursor.insertText( " )\n" );
}

void TextEdit::insertTypeCommand() {
	QTextCursor cursor( this->textCursor() );

	cursor.insertText( "type( u\'\' )\n" );
	cursor.movePosition( QTextCursor::Left, QTextCursor::MoveAnchor, 4 );

	this->setTextCursor( cursor );
}

void TextEdit::setTestCase( std::shared_ptr< TestCase > testCase ) {
	this->p_->testCase = testCase;

	const std::map< QString, QUrl > & images( testCase->getImageMapping() );
	std::for_each( images.begin(), images.end(), [this]( const std::pair< QString, QUrl > & mapping ) {
		this->p_->addImageResource( mapping.first, mapping.second );
	} );
}

QStringList TextEdit::dumpScriptWithMapping() const {
	return this->p_->traverse( [this]( const QString & name )->QString {
		std::map< QString, QUrl >::const_iterator it = this->p_->testCase->getImageMapping().find( name );
		if( it != this->p_->testCase->getImageMapping().end() ) {
			return it->second.toLocalFile();
		}
		return name;
	} );
}

QStringList TextEdit::dumpScript() const {
	return this->p_->traverse( []( const QString & name )->QString {
		return name;
	} );
}

void TextEdit::contextMenuEvent( QContextMenuEvent * event ) {
	if( !this->p_->imageForPosition( event->pos() ) ) {
		this->QTextEdit::contextMenuEvent( event );
		return;
	}

	std::shared_ptr< QMenu > menu( this->createStandardContextMenu() );
	menu->addSeparator();
	menu->addAction( this->p_->rename );
	menu->exec( event->globalPos() );
}

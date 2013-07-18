/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#ifndef XTITAN_TEXTEDIT_HPP_
#define XTITAN_TEXTEDIT_HPP_

#include "TextEdit.hpp"

#include <functional>

#include <QtGui/QAction>
#include <QtCore/QFileInfo>

#include "TestCase/TestCase.hpp"


namespace xtitan {

class TextEdit::Private : public QObject {
	Q_OBJECT
public:
	explicit Private( TextEdit * host );

	QPair< QString, QSize > addImageResource( const QString & );
	QPair< QString, QSize > addImageResource( const QString & key, const QUrl & path );

	QStringList traverse( std::function< QString ( const QString & ) > ) const;
	void replaceImage( std::map< QString, QUrl > & mapping, const QFileInfo & oldP, const QFileInfo & newP );
	bool imageForPosition( const QPoint & pos );

public slots:
	void onRenameTriggered();

public:
	TextEdit * host;
	QAction * rename;
	std::shared_ptr< TestCase > testCase;
	QTextCursor currentCursor;
};

}

#endif

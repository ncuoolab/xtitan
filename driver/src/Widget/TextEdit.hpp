/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#ifndef XTITAN_TEXTEDIT_HPP
#define XTITAN_TEXTEDIT_HPP

#include <memory>

#include <QtGui/QTextEdit>


namespace xtitan {

struct AsyncCheckPoint;
struct CheckPoint;
struct InputPoint;
class TestCase;

class TextEdit : public QTextEdit {
public:
	explicit TextEdit( QWidget * parent );

	void insertImage( const QString & path );
	void insertSingleCommand( const QString & name, const QString & path );
	void insertSpyAsyncCheck( int id, const AsyncCheckPoint & acp );
	void insertSpyCheck( int id, const CheckPoint & cp );
	void insertSpyInput( int id, int delay, const InputPoint & ip );
	void insertTypeCommand();
	void setTestCase( std::shared_ptr< TestCase > testCase );
	QStringList dumpScript() const;
	QStringList dumpScriptWithMapping() const;

protected:
	virtual void contextMenuEvent( QContextMenuEvent * event );

private:
	class Private;
	std::shared_ptr< Private > p_;
};

}

#endif

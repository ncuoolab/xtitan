#ifndef KRAPTURE_WIDGET_TEXTEDIT_HPP
#define KRAPTURE_WIDGET_TEXTEDIT_HPP

#include <memory>

#include <QtGui/QTextEdit>


namespace xtitan {

class TestCase;

class TextEdit : public QTextEdit {
public:
	explicit TextEdit( QWidget * parent );

	void insertImage( const QString & path );
	void insertSingleCommand( const QString & name, const QString & path );
	void insertSpyCheck( int id, const QString & label, const QString & value );
	void insertSpyInput( int id, int delay, const QString & object, const QString & method, const QStringList & args );
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

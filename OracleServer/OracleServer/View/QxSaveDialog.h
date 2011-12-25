#ifndef QXSAVEDIALOG_H
#define QXSAVEDIALOG_H

#include <QtGui/QDialog>

#include <boost/tr1/memory.hpp>

namespace Ui {
	class QxSaveDialog;
}

class QxSaveDialog : public QDialog {
public:
	QxSaveDialog( QWidget * parent );

	QString getName() const;
	QString getCreator() const;
	QString getDescription() const;
	int getLevel() const;

	virtual void setVisible( bool visible );

private:
	std::tr1::shared_ptr< Ui::QxSaveDialog > ui_;
};

#endif

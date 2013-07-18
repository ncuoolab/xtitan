/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#ifndef XTITAN_OPENINGDIALOG_HPP
#define XTITAN_OPENINGDIALOG_HPP


#include <QtGui/QDialog>


namespace xtitan {

class OpeningDialog : public QDialog {
public:
	explicit OpeningDialog( QWidget * parent );

	QString getSelected() const;

private:
	class Private;
	Private * p_;
};

}

#endif

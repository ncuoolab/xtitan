/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#ifndef XTITAN_OPENINGDIALOG_HPP_
#define XTITAN_OPENINGDIALOG_HPP_

#include "OpeningDialog.hpp"
#include "ui_OpeningDialog.h"


namespace xtitan {

class OpeningDialog::Private : public QObject {
	Q_OBJECT
public:
	explicit Private( OpeningDialog * );

public slots:
	void onDeletePressed();

public:
	OpeningDialog * host;
	Ui::OpeningDialog ui;
};

}

#endif

/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#ifndef XTITAN_SAVINGDIALOG_HPP_
#define XTITAN_SAVINGDIALOG_HPP_

#include "SavingDialog.hpp"

#include "ui_SavingDialog.h"


namespace xtitan {

class SavingDialog::Private : public QObject {
	Q_OBJECT

public:
	explicit Private( SavingDialog * );

	Ui::SavingDialog ui;

public slots:
	void onNameEdited( const QString & );
};

}

#endif

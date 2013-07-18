/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#ifndef XTITAN_PREFERENCEDIALOG_HPP_
#define XTITAN_PREFERENCEDIALOG_HPP_

#include "PreferenceDialog.hpp"

#include <QtGui/QAbstractButton>

#include "ui_PreferenceDialog.h"


namespace xtitan {

class PreferenceDialog::Private : public QObject {
	Q_OBJECT

public:
	explicit Private( PreferenceDialog * );

	void load();
	void save();

public slots:
	void onBrowseSSP();
	void onBrowseTCP();
	void onBrowseTPP();
	void onChangeFont();
	void onFinished( QAbstractButton * );

public:
	PreferenceDialog * host;
	Ui::PreferenceDialog ui;
	QFont font;
};

}

#endif

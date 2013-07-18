/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#ifndef XTITAN_PREFERENCEDIALOG_HPP
#define XTITAN_PREFERENCEDIALOG_HPP

#include <memory>

#include <QtGui/QDialog>

namespace xtitan {

class PreferenceDialog : public QDialog {
public:
	explicit PreferenceDialog( QWidget * parent );

private:
	class Private;
	std::shared_ptr< Private > p_;
};

}

#endif

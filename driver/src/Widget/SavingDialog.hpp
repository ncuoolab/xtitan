/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#ifndef XTITAN_SAVINGDIALOG_HPP
#define XTITAN_SAVINGDIALOG_HPP

#include <memory>

#include <QtGui/QDialog>


namespace xtitan {

class SavingDialog : public QDialog {
public:
	explicit SavingDialog( QWidget * parent );

	virtual void setVisible( bool visible );

	QString getDescription() const;
	QString getCreator() const;
	int getLevel() const;
	QString getName() const;
	QString getProgramOptions() const;

private:
	class Private;
	std::shared_ptr< Private > p_;
};

}

#endif

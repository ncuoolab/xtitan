#ifndef QXPREFERENCEDIALOG_H
#define QXPREFERENCEDIALOG_H

#include <QtGui/QDialog>

#include <boost/tr1/memory.hpp>

class QAbstractButton;
namespace Ui {
	class QxPreferenceDialog;
}

class QxPreferenceDialog : public QDialog {
	Q_OBJECT
public:
	QxPreferenceDialog( QWidget * parent );

	virtual void setVisible( bool visible );

private slots:
	void onButtonPressed_( QAbstractButton * );
	void onBrowseRender_();
	void onBrowseShell_();
	void onBrowseConfig_();
	void onBrowsePortal_();
	void onBrowseTestcase_();

private:
	void save_();

	std::tr1::shared_ptr< Ui::QxPreferenceDialog > ui_;
};

#endif

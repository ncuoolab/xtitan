#include <QtGui/QDialog>

#include <boost/tr1/memory.hpp>

namespace Ui {
	class QxSelectTestDialog;
}
class QListWidgetItem;

class QxSelectTestDialog : public QDialog {
	Q_OBJECT
public:
	QxSelectTestDialog( QWidget * parent );

	QStringList getCheckedTestCase() const;

	virtual void setVisible( bool visible );

private slots:
	void onDoubleClicked_( QListWidgetItem * );

private:
	std::tr1::shared_ptr< Ui::QxSelectTestDialog > ui_;
};

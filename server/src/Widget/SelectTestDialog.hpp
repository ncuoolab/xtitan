#ifndef XTITAN_WIDGET_SELECTTESTDIALOG_HPP
#define XTITAN_WIDGET_SELECTTESTDIALOG_HPP

#include <QtGui/QDialog>

namespace xtitan {
	namespace widget {

	class SelectTestDialog : public QDialog {
		public:
			explicit SelectTestDialog( QWidget * parent );

			QStringList getCheckedTestCase() const;

			virtual void setVisible( bool visible );

		private:
			class Private;
			Private * p_;
		};

	}
}

#endif

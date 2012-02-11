#ifndef XTITAN_WIDGET_PREFERENCEDIALOG_HPP
#define XTITAN_WIDGET_PREFERENCEDIALOG_HPP

#include <QtGui/QDialog>

namespace xtitan {
	namespace widget {

		class PreferenceDialog : public QDialog {
		public:
			explicit PreferenceDialog( QWidget * parent );

			virtual void setVisible( bool visible );

		private:
			class Private;
			Private * p_;
		};

	}
}

#endif

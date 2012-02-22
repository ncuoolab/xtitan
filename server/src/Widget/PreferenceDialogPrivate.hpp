#ifndef XTITAN_WIDGET_PREFERENCEDIALOG_PRIVATE_HPP
#define XTITAN_WIDGET_PREFERENCEDIALOG_PRIVATE_HPP

#include "PreferenceDialog.hpp"
#include "ui_PreferenceDialog.h"

namespace xtitan {
	namespace widget {

		class PreferenceDialog::Private : public QObject {
			Q_OBJECT
		public:
			explicit Private( PreferenceDialog * );

			void save();

			PreferenceDialog * host;
			Ui::PreferenceDialog ui;

		public slots:
			void onButtonPressed( QAbstractButton * );
			void onBrowseSoftware();
			void onBrowseTestcase();
		};

	}
}

#endif

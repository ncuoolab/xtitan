#ifndef XTITAN_WIDGET_SELECTTESTDIALOG_PRIVATE_HPP
#define XTITAN_WIDGET_SELECTTESTDIALOG_PRIVATE_HPP

#include "SelectTestDialog.hpp"
#include "ui_SelectTestDialog.h"

namespace xtitan {
	namespace widget {

		class SelectTestDialog::Private : public QObject {
			Q_OBJECT
		public:
			explicit Private( SelectTestDialog * );

			SelectTestDialog * host;
			Ui::SelectTestDialog ui;

		public slots:
			void onDoubleClicked( QListWidgetItem * );
		};

	}
}

#endif

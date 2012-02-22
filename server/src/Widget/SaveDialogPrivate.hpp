#ifndef XTITAN_WIDGET_SAVEDIALOG_PRIVATE_HPP
#define XTITAN_WIDGET_SAVEDIALOG_PRIVATE_HPP

#include "SaveDialog.hpp"
#include "ui_SaveDialog.h"

namespace xtitan {
	namespace widget {

		class SaveDialog::Private {
		public:
			explicit Private( SaveDialog * );

			SaveDialog * host;
			Ui::SaveDialog ui;
		};

	}
}

#endif

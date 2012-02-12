#ifndef XTITAN_WIDGET_SAVEDIALOG_HPP
#define XTITAN_WIDGET_SAVEDIALOG_HPP

#include <QtGui/QDialog>

#include <memory>

namespace xtitan {
	namespace widget {

		class SaveDialog : public QDialog {
		public:
			explicit SaveDialog( QWidget * parent );

			QString getName() const;
			QString getCreator() const;
			QString getDescription() const;
			int getLevel() const;

			virtual void setVisible( bool visible );

		private:
			class Private;
			std::shared_ptr< Private > p_;
		};

	}
}

#endif

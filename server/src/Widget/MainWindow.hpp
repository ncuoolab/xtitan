#ifndef ORACLESERVER_CONTROLLER_VIEW_MAINWINDOW_HPP
#define ORACLESERVER_CONTROLLER_VIEW_MAINWINDOW_HPP

#include <QtGui/QMainWindow>
#include <QtCore/QString>

#include <memory>

namespace xtitan {
	namespace widget {

		class MainWindow : public QMainWindow {
			Q_OBJECT
		public:
			MainWindow();

			void setSingleShot( bool singleShot );

		public slots:
			void startRegression();
			void startSmokeTest();

		private:
			class Private;
			std::shared_ptr< Private > p_;
		};

	}
}

#endif

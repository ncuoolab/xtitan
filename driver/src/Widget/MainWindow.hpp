#ifndef KRAPTURE_MAINWINDOW_HPP
#define KRAPTURE_MAINWINDOW_HPP

#include <memory>

#include <QtGui/QMainWindow>


namespace xtitan {

class MainWindow : public QMainWindow {
public:
	MainWindow();

	void restartSikuliServer();
	void reopenTestCaseManager();

private:
	class Private;
	std::shared_ptr< Private > p_;
};

}

#endif

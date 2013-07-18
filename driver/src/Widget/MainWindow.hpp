/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#ifndef XTITAN_MAINWINDOW_HPP
#define XTITAN_MAINWINDOW_HPP

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

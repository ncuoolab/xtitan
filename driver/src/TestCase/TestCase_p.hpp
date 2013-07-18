/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#ifndef XTITAN_TESTCASE_HPP_
#define XTITAN_TESTCASE_HPP_

#include "TestCase.hpp"

namespace xtitan {

class TestCase::Private {
public:
	Private();

	bool activated;
	QString creator;
	QString description;
	QString html;
	int level;
	std::map< QString, QUrl > mapping;
	QDateTime mtime;
	QString name;
	QString options;
	QString script;
};

}

#endif

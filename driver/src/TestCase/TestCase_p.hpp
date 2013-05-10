#ifndef KRAPTURE_TESTCASEPRIVATE_HPP
#define KRAPTURE_TESTCASEPRIVATE_HPP

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

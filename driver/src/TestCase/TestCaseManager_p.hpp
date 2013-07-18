/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#ifndef XTITAN_TESTCASEMANAGER_HPP_
#define XTITAN_TESTCASEMANAGER_HPP_

#include "TestCaseManager.hpp"

#include <QtSql/QSqlDatabase>

namespace xtitan {

	class TestCaseManager::Private : public QObject {
		Q_OBJECT

	public:
		static std::shared_ptr< TestCaseManager > self;
		static void destroy( TestCaseManager * );

		Private();

		void fixup();
		void refresh();

	public slots:
		void close();
	};

}

#endif

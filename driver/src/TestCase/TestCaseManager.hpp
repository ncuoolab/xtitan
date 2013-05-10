#ifndef KRAPTURE_TESTCASEMANAGER_HPP
#define KRAPTURE_TESTCASEMANAGER_HPP

#include <QtCore/QString>
#include <QtSql/QSqlQueryModel>

#include <map>
#include <memory>
#include <tuple>
#include <vector>

namespace xtitan {

	class TestCase;

	class TestCaseManager : public QSqlQueryModel {
	public:
		static void initialize();
		static TestCaseManager & instance();
		
		void close();
		void create();
		bool isOpen() const;
		void open();

		void deleteByIndex( const QModelIndex & index );
		std::vector< std::tuple< QString, QString > > getActivated() const;
		QString getName( const QModelIndex & index ) const;
		std::shared_ptr< TestCase > load( const QString & name ) const;
		void save( std::shared_ptr< TestCase > testcase );

	private:
		TestCaseManager();
		virtual ~TestCaseManager();
		TestCaseManager( const TestCaseManager & );
		TestCaseManager & operator =( const TestCaseManager & );

		class Private;
		friend class Private;
		std::unique_ptr< Private > p_;
	};

}

#endif

/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#ifndef XTITAN_TESTCASE_HPP
#define XTITAN_TESTCASE_HPP

#include <map>
#include <memory>

#include <QtCore/QDateTime>
#include <QtCore/QString>
#include <QtCore/QUrl>

namespace xtitan {

class TestCase {
public:
	TestCase();

	const QString & getCreator() const;
	const QString & getDescription() const;
	const QString & getHTML() const;
	const std::map< QString, QUrl > & getImageMapping() const;
	int getLevel() const;
	const QDateTime & getModifyTime() const;
	const QString & getName() const;
	const QString & getProgramOptions() const;
	const QString & getScript() const;
	bool isActivated() const;

	void addImageMapping( const QString & name, const QUrl & uri );
	void setActivated( bool activated );
	void setCreator( const QString & creator );
	void setDescription( const QString & description );
	void setHTML( const QString & html );
	void setImageMapping( const std::map< QString, QUrl > & mapping );
	void setLevel( int level );
	void setModifyTime( const QDateTime & timestamp );
	void setName( const QString & name );
	void setProgramOptions( const QString & options );
	void setScript( const QString & script );

private:
	TestCase( const TestCase & );
	TestCase & operator =( const TestCase & );

	class Private;
	std::shared_ptr< Private > p_;
};

}

#endif

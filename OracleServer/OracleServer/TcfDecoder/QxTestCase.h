#ifndef QXTESTCASE_H
#define QXTESTCASE_H

#include "QxCheckPoint.h"
#include "QxInputPoint.h"

#include <QtCore/QStringList>
#include <QtCore/QPair>
#include <QtCore/QByteArray>

#include <boost/tr1/memory.hpp>

#include <map>

class QxTestCase {
public:
	typedef QPair< int, QString > CheckKey;
	typedef std::map< CheckKey, QList< QxCheckPoint * > > CheckMap;

	QxTestCase();

	bool load();
	bool save();
	void writeLine( const QString & line );

	void setName( const QString & name );
	const QString & getName() const;
	void setCreator( const QString & creator );
	const QString & getCreator() const;
	void setDescription( const QString & description );
	const QString & getDescription() const;

	void backupRemoteDatabase();
	void restoreRemoteDatabase();
	void importDatabase();

	int getClients() const;
	void setClients( int nClients );
	const QList< QxPoint > & getPoints() const;
	const CheckMap & getCheckPoints() const;

private:
	int nClients_;
	QString name_;
	QString creator_;
	QString description_;
	QList< QxPoint > points_;
	CheckMap checkPoints_;
	QByteArray remoteDatabase_;
	QByteArray database_;
	QString databasePath_;
	QStringList recordedScript_;
};

#endif

#ifndef QXABSTRACTPOINT_H
#define QXABSTRACTPOINT_H

#include <boost/tr1/memory.hpp>

#include <QtCore/QString>

class QxAbstractUnit;

class QxAbstractPoint {
public:
	int getID() const;
	const QString & getLabel() const;
	int getLine() const;

	virtual QString toString() const = 0;
	virtual void execute( QxAbstractUnit * unit ) = 0;

protected:
	QxAbstractPoint( int id, const QString & label, int line );

private:
	QxAbstractPoint( const QxAbstractPoint & );
	QxAbstractPoint & operator =( const QxAbstractPoint & );

	int id_;
	QString label_;
	int line_;
};

typedef std::tr1::shared_ptr< QxAbstractPoint > QxPoint;

#endif

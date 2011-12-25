#ifndef QX_SPY_MODEL_H
#define QX_SPY_MODEL_H

#include <QtScript/QScriptEngine>
#include <QStringList>
#include <QObject>
#include "QxTesting/SpyModule/Controller/QxSpy.h"
#include <QApplication>

class QxSpyModel : public QObject {
	Q_OBJECT
public :
	QxSpyModel( QObject * parent );

	virtual void Stop() = 0;
	virtual void input( const QString & label, const QString & script, qint64 waitTime ) = 0;
	virtual void check( const QString & label, const QString & value ) = 0;

signals:
	void ready();
};

#endif	//QX_SPY_MODEL_H

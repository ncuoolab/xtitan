#ifndef QX_TEST_H
#define QX_TEST_H

#include <QString>
#include "QxTesting/SpyModule/Controller/QxSpy.h"
#include "util/Logger.h"

#define xCheck(id,x)					QxSpy::getInstance()->encodeCheck(QString::fromUtf8(__FUNCTION__),id, x );
#define xCheck(x)						QxSpy::getInstance()->encodeCheck(QString::fromUtf8(__FUNCTION__),QString::fromUtf8(__FUNCTION__),x);
#define xInput(...)						QxSpy::getInstance()->RecordInput(this->objectName(),__FUNCTION__,__VA_ARGS__);
#define xCustomInput(object,method,...)	QxSpy::getInstance()->RecordInput(object,__FUNCTION__,method,__VA_ARGS__);

void xRegisterObject( QObject * object, const QString & id = QString() );

QString xGetToken( QObject * object );
QObject * xGetObject( const QString & token );

void xTryTestAutomation();

bool xIsTesting();

QString xMakeSafeName(QString objName);

#endif

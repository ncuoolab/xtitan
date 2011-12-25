#ifndef _QX_LOGGER_H_
#define _QX_LOGGER_H_

#include <QFile>
#include <QString>
#include <QTextStream>

class QxLogger{
public :
	static QxLogger* getInstance();
	
	void setFileName(QString filename);

	void Log(QString msg);
	
private:
	static QxLogger* singleton;
	QxLogger();
	~QxLogger();

	QString fileName;
	QFile* file;
	QTextStream stream;
};

#endif
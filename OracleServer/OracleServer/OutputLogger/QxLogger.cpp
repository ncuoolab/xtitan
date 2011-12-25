#include "OutputLogger/QxLogger.h"
#include <QDateTime>
#include "Setting/Setting.h"

QxLogger* QxLogger::singleton = NULL;

QxLogger* QxLogger::getInstance(){
	if (singleton == NULL)
		singleton = new QxLogger();
	return singleton;
}

QxLogger::QxLogger(){
	this->fileName = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")+".log";
	QString path = QxSetting::Instance().get("TestCasePath");
	file = new QFile(path+this->fileName);	
}

QxLogger::~QxLogger(){
	int fsize = file->size();
	file->close();	
	delete file;
	if(fsize == 0){
		QString path = QxSetting::Instance().get("TestCasePath");		
		QFile::remove(path+this->fileName);
	}	
}

void QxLogger::setFileName(QString name){
	this->fileName = name;
	file->close();
	delete file;	
	QString path = QxSetting::Instance().get("TestCasePath");	
	file = new QFile(path+this->fileName);
}

void QxLogger::Log(QString msg){
	if (!file->isOpen()){		
		bool result = file->open(QFile::WriteOnly | QFile::Text);
		assert(result);
		stream.setDevice(file);
	}
	stream << msg << "\n";	
	stream.flush();
}

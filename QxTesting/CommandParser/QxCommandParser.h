#ifndef _QX_COMMAND_PARSER_H_
#define _QX_COMMAND_PARSER_H_

#include <QMap>
#include "QxTesting/Commands/QxAbstractCommand.h"
#include <QString>

typedef QMap<QString,QxAbstractCommand*> CommandMap;


class QxCommandParser {
public:
	static QxCommandParser* getInstance();
	void parse( const QString & header, const QVariant & data );

private:
	static QxCommandParser* _singleton;
	QxCommandParser();
	CommandMap commandList;
};

#endif

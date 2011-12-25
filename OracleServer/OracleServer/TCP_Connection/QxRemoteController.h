#ifndef QXREMOTECONTROLLER_H
#define QXREMOTECONTROLLER_H

#include <QtCore/QIODevice>

class QxRemoteController {
public:
	static bool exportDatabase( QIODevice * data );
	static bool importDatabase( QIODevice * data );
	static void restartServer();
};

#endif

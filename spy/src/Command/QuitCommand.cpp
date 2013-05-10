#include "QuitCommand.hpp"

#include <QtCore/QCoreApplication>
#include <QtCore/QMetaObject>

using namespace xtitan::command;

void QuitCommand::execute( const QVariant & /*data*/ ) {
	QMetaObject::invokeMethod( QCoreApplication::instance(), "quit" );
}

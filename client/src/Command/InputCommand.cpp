#include "InputCommand.hpp"
#include "xTitanClient/Spy/Spy.hpp"

#include <QtCore/QMetaObject>

using namespace xtitan::command;
using xtitan::spy::Spy;

// format: script
void InputCommand::execute( const QVariant & data ) {
	// FIXME: thread problems
	QMetaObject::invokeMethod( &Spy::getInstance(), "executeScript", Q_ARG( const QString &, data.toString() ) );
}

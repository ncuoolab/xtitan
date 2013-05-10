#include "InputCommand.hpp"

#include <QtCore/QMetaObject>

#include "xTitan/Exception/NetworkError.hpp"
#include "Spy/Spy.hpp"


using namespace xtitan::command;
using xtitan::Spy;


// format: script
void InputCommand::execute( const QVariant & data ) {
	auto kwargs = data.toMap();
	auto object = kwargs.value( "object" ).toString();
	auto method = kwargs.value( "method" ).toString();
	auto args = kwargs.value( "args" ).toList();

	QStringList sArgs;
	for( auto it = args.begin(); it != args.end(); ++it ) {
		if( it->type() == QVariant::Bool ) {
			auto b = it->toBool();
			sArgs.append( b ? "true" : "false" );
		} else if( it->type() == QVariant::Int ) {
			auto i = it->toInt();
			sArgs.append( QString::number( i ) );
		} else if( it->type() == QVariant::Double ) {
			auto d = it->toDouble();
			sArgs.append( QString::number( d ) );
		} else if( it->type() == QVariant::String ) {
			auto s = it->toString();
			sArgs.append( QString( "\'%1\'" ).arg( s ) );
		} else {
			throw NetworkError( QObject::tr( "unknown data type: %1" ).arg( it->typeName() ) );
		}
	}

	auto script = QString( "%1.%2(%3)" ).arg( object ).arg( method ).arg( sArgs.join( "," ) );

	// FIXME: thread problems
	QMetaObject::invokeMethod( &Spy::instance(), "executeScript", Q_ARG( const QString &, script ) );
}

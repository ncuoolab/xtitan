/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#ifndef XTITAN_UTILITIES_LOGGER_HPP
#define XTITAN_UTILITIES_LOGGER_HPP

#include <memory>

#include <QtCore/QString>

#include "xTitan/Utility/CoreConfig.hpp"


namespace xtitan {

class XTITAN_CORE_DLL Logger {
public :
	static void initialize();
	static Logger & instance();

	void setFileName( const QString & filename );

	void log( const QString & msg );

private:
	class Private;

	Logger();
	~Logger();

	std::shared_ptr< Private > p_;
};

}

#endif

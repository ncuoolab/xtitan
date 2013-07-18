/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#ifndef XTITAN_SPY_SPYASYNCCHECK_HPP
#define XTITAN_SPY_SPYASYNCCHECK_HPP

#include <memory>
#include <string>

#include "xTitan/Utility/SpyConfig.hpp"


namespace xtitan {
namespace detail {

class XTITAN_SPY_DLL SpyAsyncCheck {
public:
	SpyAsyncCheck( const char * file, int line );
	~SpyAsyncCheck();

	SpyAsyncCheck & id( const std::string & s );
	SpyAsyncCheck & id( const std::wstring & ws );
	SpyAsyncCheck & pre( const std::string & s );
	SpyAsyncCheck & pre( const std::wstring & ws );

	SpyAsyncCheck & operator %( bool b );
	SpyAsyncCheck & operator %( int i );
	SpyAsyncCheck & operator %( double d );
	SpyAsyncCheck & operator %( const char * s );
	SpyAsyncCheck & operator %( const std::string & s );
	SpyAsyncCheck & operator %( const wchar_t * ws );
	SpyAsyncCheck & operator %( const std::wstring & ws );

private:
	SpyAsyncCheck( const SpyAsyncCheck & );
	SpyAsyncCheck & operator =( const SpyAsyncCheck & );

	class Private;
	std::shared_ptr< Private > p_;
};

}
}

#endif

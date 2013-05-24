#ifndef XTITAN_SPY_SPYCHECK_HPP
#define XTITAN_SPY_SPYCHECK_HPP

#include <memory>
#include <string>

#include "xTitan/Utility/SpyConfig.hpp"


namespace xtitan {
namespace detail {

class XTITAN_SPY_DLL SpyCheck {
public:
	SpyCheck( const char * file, int line );
	~SpyCheck();

	SpyCheck & id( const std::string & s );
	SpyCheck & id( const std::wstring & ws );

	SpyCheck & operator %( bool b );
	SpyCheck & operator %( int i );
	SpyCheck & operator %( double d );
	SpyCheck & operator %( const char * s );
	SpyCheck & operator %( const std::string & s );
	SpyCheck & operator %( const wchar_t * ws );
	SpyCheck & operator %( const std::wstring & ws );

private:
	SpyCheck( const SpyCheck & );
	SpyCheck & operator =( const SpyCheck & );

	class Private;
	std::shared_ptr< Private > p_;
};

}
}

#endif

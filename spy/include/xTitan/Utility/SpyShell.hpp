#ifndef XTITAN_CLIENT_SPYMODULE_CONTROLLER_SPYINPUT_HPP
#define XTITAN_CLIENT_SPYMODULE_CONTROLLER_SPYINPUT_HPP

#include <memory>
#include <string>

#include "xTitan/Utility/SpyConfig.hpp"


namespace xtitan {

class XTITAN_SPY_DLL SpyInput {
public:
	SpyInput( const char * objectName, const char * signature );
	~SpyInput();

	SpyInput & operator %( bool b );
	SpyInput & operator %( int i );
	SpyInput & operator %( double d );
	SpyInput & operator %( const char * s );
	SpyInput & operator %( const std::string & s );
	SpyInput & operator %( const wchar_t * ws );
	SpyInput & operator %( const std::wstring & ws );

private:
	SpyInput( const SpyInput & );
	SpyInput & operator =( const SpyInput & );

	class Private;
	std::shared_ptr< Private > p_;
};

XTITAN_SPY_DLL void encodeCheck( const std::string & signature, const std::string & feature, bool value );
XTITAN_SPY_DLL void encodeCheck( const std::string & signature, const std::string & feature, int value );
XTITAN_SPY_DLL void encodeCheck( const std::string & signature, const std::string & feature, double value );
XTITAN_SPY_DLL void encodeCheck( const std::string & signature, const std::string & feature, const char * value );
XTITAN_SPY_DLL void encodeCheck( const std::string & signature, const std::string & feature, const std::string & value );
XTITAN_SPY_DLL void encodeCheck( const std::string & signature, const std::string & feature, const wchar_t * value );
XTITAN_SPY_DLL void encodeCheck( const std::string & signature, const std::string & feature, const std::wstring & value );

}

#endif

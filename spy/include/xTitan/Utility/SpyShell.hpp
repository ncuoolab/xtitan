#ifndef XTITAN_CLIENT_SPYMODULE_CONTROLLER_SPYINPUT_HPP
#define XTITAN_CLIENT_SPYMODULE_CONTROLLER_SPYINPUT_HPP

#include <memory>
#include <string>

#include "xTitan/Utility/SpyConfig.hpp"


namespace xtitan {

class XTITAN_SPY_DLL SpyInput {
public:
	explicit SpyInput( const char * objectName, const char * signature );
	~SpyInput();

private:
	XTITAN_SPY_DLL friend SpyInput & operator %( SpyInput & si, bool b );
	XTITAN_SPY_DLL friend SpyInput & operator %( SpyInput & si, int i );
	XTITAN_SPY_DLL friend SpyInput & operator %( SpyInput & si, double d );
	XTITAN_SPY_DLL friend SpyInput & operator %( SpyInput & si, const std::string & s );
	XTITAN_SPY_DLL friend SpyInput & operator %( SpyInput & si, const std::wstring & ws );

	SpyInput( const SpyInput & );
	SpyInput & operator =( const SpyInput & );

	class Private;
	std::shared_ptr< Private > p_;
};

XTITAN_SPY_DLL SpyInput & operator %( SpyInput & si, bool b );
XTITAN_SPY_DLL SpyInput & operator %( SpyInput & si, int i );
XTITAN_SPY_DLL SpyInput & operator %( SpyInput & si, double d );
XTITAN_SPY_DLL SpyInput & operator %( SpyInput & si, const std::string & s );
XTITAN_SPY_DLL SpyInput & operator %( SpyInput & si, const std::wstring & ws );

XTITAN_SPY_DLL void encodeCheck( const std::string & signature, bool value );
XTITAN_SPY_DLL void encodeCheck( const std::string & signature, int value );
XTITAN_SPY_DLL void encodeCheck( const std::string & signature, double value );
XTITAN_SPY_DLL void encodeCheck( const std::string & signature, const std::string & value );
XTITAN_SPY_DLL void encodeCheck( const std::string & signature, const std::wstring & value );

}

#endif

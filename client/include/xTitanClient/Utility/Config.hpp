#ifndef XTITAN_CLIENT_UTILITY_CONFIG_HPP
#define XTITAN_CLIENT_UTILITY_CONFIG_HPP

#ifdef _MSC_VER
# ifdef XTITAN_CLIENT_LIBRARY
#  define XTITAN_CLIENT_DLL __declspec(dllexport)
# else
#  define XTITAN_CLIENT_DLL __declspec(dllimport)
# endif
#else
# define XTITAN_CLIENT_DLL
#endif

#endif

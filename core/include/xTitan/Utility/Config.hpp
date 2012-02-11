#ifndef XTITAN_CORE_UTILITY_CONFIG_HPP
#define XTITAN_CORE_UTILITY_CONFIG_HPP

#ifdef _MSC_VER
# ifdef XTITAN_CORE_LIBRARY
#  define XTITAN_CORE_DLL __declspec(dllexport)
# else
#  define XTITAN_CORE_DLL __declspec(dllimport)
# endif
#else
# define XTITAN_CORE_DLL
#endif

#endif

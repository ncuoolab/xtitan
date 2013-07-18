/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#ifndef XTITAN_SPY_UTILITY_CONFIG_HPP
#define XTITAN_SPY_UTILITY_CONFIG_HPP

#ifdef _MSC_VER
# ifdef XTITAN_SPY_LIBRARY
#  define XTITAN_SPY_DLL __declspec(dllexport)
# else
#  define XTITAN_SPY_DLL __declspec(dllimport)
# endif
#else
# define XTITAN_SPY_DLL
#endif

#endif

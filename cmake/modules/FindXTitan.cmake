# Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>

if(XTitan_FOUND)
    return()
endif()

set(XTITAN_HOME "$ENV{XTITAN_HOME}")
file(TO_CMAKE_PATH "${XTITAN_HOME}" XTITAN_HOME)

find_path(XTITAN_INCLUDE_DIRS "xTitan/Utility/CoreConfig.hpp" HINTS "${XTITAN_HOME}/include")

find_library(XTITAN_CORE_LIBRARY_RELEASE NAMES xTitanCore HINTS "${XTITAN_HOME}/lib")
find_library(XTITAN_CORE_LIBRARY_DEBUG NAMES xTitanCored HINTS "${XTITAN_HOME}/lib")
find_library(XTITAN_SPY_LIBRARY_RELEASE NAMES xTitanSpy HINTS "${XTITAN_HOME}/lib")
find_library(XTITAN_SPY_LIBRARY_DEBUG NAMES xTitanSpyd HINTS "${XTITAN_HOME}/lib")

include(SelectLibraryConfigurations)
select_library_configurations(XTITAN_CORE)
select_library_configurations(XTITAN_SPY)
set(XTITAN_LIBRARIES ${XTITAN_CORE_LIBRARY} ${XTITAN_SPY_LIBRARY})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(XTitan DEFAULT_MSG XTITAN_INCLUDE_DIRS XTITAN_LIBRARIES)

mark_as_advanced(XTITAN_INCLUDE_DIRS XTITAN_LIBRARIES)

include(FindPackageHandleStandardArgs)

set(XTITAN_HOME "$ENV{XTITAN_HOME}")

find_path(XTITAN_INCLUDE_DIRS "xTitan/Utility/Config.hpp" HINTS "${XTITAN_HOME}/include")

find_library(XTITAN_LIBRARY_RELEASE NAMES xTitan HINTS "${XTITAN_HOME}/lib")
find_library(XTITAN_LIBRARY_DEBUG NAMES xTitand HINTS "${XTITAN_HOME}/lib")
find_library(XTITAN_CLIENT_LIBRARY_RELEASE NAMES xTitanClient HINTS "${XTITAN_HOME}/lib")
find_library(XTITAN_CLIENT_LIBRARY_DEBUG NAMES xTitanClientd HINTS "${XTITAN_HOME}/lib")

set(XTITAN_LIBRARIES "debug" ${XTITAN_LIBRARY_DEBUG} "optimized" ${XTITAN_LIBRARY_RELEASE} "debug" ${XTITAN_CLIENT_LIBRARY_DEBUG} "optimized" ${XTITAN_CLIENT_LIBRARY_RELEASE})

find_package_handle_standard_args(XTitan DEFAULT_MSG XTITAN_INCLUDE_DIRS XTITAN_LIBRARIES)

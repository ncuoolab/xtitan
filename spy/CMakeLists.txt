# Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>

# spy

include(xTitanUtility)

# hack MSVC path
if(MSVC)
    set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
else()
    set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
endif()

add_definitions(-DXTITAN_SPY_LIBRARY)

file(GLOB_RECURSE XTITAN_SPY_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.hpp)
file(GLOB_RECURSE XTITAN_SPY_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)

group_sources("include")
group_sources("src")

include_directories(
    "${CMAKE_SOURCE_DIR}/core/include"
    "include"
    "src"
    ${CMAKE_CURRENT_BINARY_DIR})

set(XTITAN_SPY_COMPILE_UNIT ${XTITAN_SPY_HEADERS} ${XTITAN_SPY_SOURCES})
set(XTITAN_SPY_LINK_UNIT xTitanCore ${QT_QTCORE_LIBRARY} ${QT_QTSCRIPT_LIBRARY})
add_library(xTitanSpy SHARED ${XTITAN_SPY_COMPILE_UNIT})
add_dependencies(xTitanSpy xTitanCore)
target_link_libraries(xTitanSpy ${XTITAN_SPY_LINK_UNIT})
set_target_properties(xTitanSpy
    PROPERTIES
        VERSION ${XTITAN_VERSION}
        SOVERSION ${XTITAN_VERSION})

install(TARGETS xTitanSpy
    RUNTIME DESTINATION "bin"
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib")
install(DIRECTORY "include/xTitan" DESTINATION "include")
install(FILES "${CMAKE_SOURCE_DIR}/cmake/modules/FindXTitan.cmake" DESTINATION "share/cmake/modules")

# xTitanClient

# hack MSVC path
if(MSVC)
    set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
else()
    set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
endif()

add_definitions(-DXTITAN_CLIENT_LIBRARY)

file(GLOB_RECURSE XTITAN_CLIENT_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.hpp)
file(GLOB_RECURSE XTITAN_CLIENT_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)
set(XTITAN_CLIENT_MOC_HEADERS "")
foreach(header ${XTITAN_CLIENT_HEADERS})
    file(STRINGS ${header} moc_token REGEX "Q_OBJECT")
    if(moc_token)
        list(APPEND XTITAN_CLIENT_MOC_HEADERS ${header})
    endif()
endforeach()

include_directories(
    "${CMAKE_SOURCE_DIR}/core/include"
    "include"
    "src"
    ${CMAKE_CURRENT_BINARY_DIR})

qt4_wrap_cpp(XTITAN_CLIENT_MOC_SOURCES ${XTITAN_CLIENT_MOC_HEADERS})

source_group("Generated Files" FILES ${XTITAN_CLIENT_MOC_SOURCES})

set(XTITAN_CLIENT_COMPILE_UNIT ${XTITAN_CLIENT_HEADERS} ${XTITAN_CLIENT_SOURCES} ${XTITAN_CLIENT_MOC_SOURCES})
set(XTITAN_CLIENT_LINK_UNIT xTitan ${QT_QTCORE_LIBRARY} ${QT_QTSCRIPT_LIBRARY})
add_library(xTitanClient SHARED ${XTITAN_CLIENT_COMPILE_UNIT})
add_dependencies(xTitanClient xTitan)
target_link_libraries(xTitanClient ${XTITAN_CLIENT_LINK_UNIT})
set_target_properties(xTitanClient
    PROPERTIES
        VERSION ${XTITAN_VERSION}
        SOVERSION ${XTITAN_VERSION})

install(TARGETS xTitanClient
    RUNTIME DESTINATION "bin"
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib")
install(DIRECTORY "include/xTitanClient" DESTINATION "include")

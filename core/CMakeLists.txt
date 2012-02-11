# xTitan

# hack MSVC path
if(MSVC)
    set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
else()
    set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
endif()

add_definitions(-DXTITAN_CORE_LIBRARY)

file(GLOB_RECURSE XTITAN_CORE_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.hpp)
file(GLOB_RECURSE XTITAN_CORE_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)
set(XTITAN_CORE_MOC_HEADERS "")
foreach(header ${XTITAN_CORE_HEADERS})
    file(STRINGS ${header} moc_token REGEX "Q_OBJECT")
    if(moc_token)
        list(APPEND XTITAN_CORE_MOC_HEADERS ${header})
    endif()
endforeach()

include_directories(
    "include"
    "src"
    ${CMAKE_CURRENT_BINARY_DIR})

qt4_wrap_cpp(XTITAN_CORE_MOC_SOURCES ${XTITAN_CORE_MOC_HEADERS})

source_group("Generated Files" FILES ${XTITAN_CORE_MOC_SOURCES})

set(XTITAN_CORE_COMPILE_UNIT ${XTITAN_CORE_HEADERS} ${XTITAN_CORE_SOURCES} ${XTITAN_CORE_MOC_SOURCES})
set(XTITAN_CORE_LINK_UNIT ${QT_QTCORE_LIBRARY} ${QT_QTNETWORK_LIBRARY})
add_library(xTitan SHARED ${XTITAN_CORE_COMPILE_UNIT})
target_link_libraries(xTitan ${XTITAN_CORE_LINK_UNIT})
set_target_properties(xTitan
    PROPERTIES
        VERSION ${XTITAN_VERSION}
        SOVERSION ${XTITAN_VERSION})

if(WIN32)
    install(TARGETS xTitan
        LIBRARY DESTINATION "."
        ARCHIVE DESTINATION "lib")
else()
    install(TARGETS xTitan
        LIBRARY DESTINATION "."
        ARCHIVE DESTINATION "lib")
endif()

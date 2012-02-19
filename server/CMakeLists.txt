# xTitanServer

# hack MSVC path
if(MSVC)
    set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
else()
    set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
endif()

file(GLOB_RECURSE XTITAN_SERVER_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.hpp)
file(GLOB_RECURSE XTITAN_SERVER_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)
file(GLOB_RECURSE XTITAN_SERVER_FORMS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.ui)
set(XTITAN_SERVER_MOC_HEADERS "")
foreach(header ${XTITAN_SERVER_HEADERS})
    file(STRINGS ${header} moc_token REGEX "Q_OBJECT")
    if(moc_token)
        list(APPEND XTITAN_SERVER_MOC_HEADERS ${header})
    endif()
endforeach()

include_directories(
    "${CMAKE_SOURCE_DIR}/core/include"
    "src"
    ${CMAKE_CURRENT_BINARY_DIR})

qt4_wrap_cpp(XTITAN_SERVER_MOC_SOURCES ${XTITAN_SERVER_MOC_HEADERS})
qt4_wrap_ui(XTITAN_SERVER_UIC_HEADERS ${XTITAN_SERVER_FORMS})

source_group("Generated Files" FILES ${XTITAN_SERVER_MOC_SOURCES} ${XTITAN_SERVER_RCC_SOURCES} ${XTITAN_SERVER_UIC_HEADERS})
source_group("Form Files" REGULAR_EXPRESSION .*\\.ui)

set(XTITAN_SERVER_COMPILE_UNIT ${XTITAN_SERVER_HEADERS} ${XTITAN_SERVER_SOURCES} ${XTITAN_SERVER_MOC_SOURCES} ${XTITAN_SERVER_UIC_HEADERS})
set(XTITAN_SERVER_LINK_UNIT xTitan ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTSQL_LIBRARY})
if(MSVC)
    add_executable(xTitanServer WIN32 ${XTITAN_SERVER_COMPILE_UNIT})
    target_link_libraries(xTitanServer ${QT_QTMAIN_LIBRARY} ${XTITAN_SERVER_LINK_UNIT})
else()
    add_executable(xTitanServer ${XTITAN_SERVER_COMPILE_UNIT})
    target_link_libraries(xTitanServer ${XTITAN_SERVER_LINK_UNIT})
endif()
add_dependencies(xTitanServer xTitan)
set_target_properties(xTitanServer PROPERTIES VERSION ${XTITAN_VERSION})

install(TARGETS xTitanServer DESTINATION "bin")
install(FILES "configuration.ini" DESTINATION "bin")

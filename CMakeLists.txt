# root

cmake_minimum_required(VERSION 2.6)

project(xTitan)
set(XTITAN_VERSION_MAJOR "1")
set(XTITAN_VERSION_MINOR "0")
set(XTITAN_VERSION_PATCH "0")
set(XTITAN_VERSION "${XTITAN_VERSION_MAJOR}.${XTITAN_VERSION_MINOR}.${XTITAN_VERSION_PATCH}")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

find_package(Qt4 4.7 COMPONENTS QtNetwork QtScript QtSql REQUIRED)

include(${QT_USE_FILE})

add_definitions(-DXTITAN_VERSION=${XTITAN_VERSION} -DXTITAN_VERSION_MAJOR=${XTITAN_VERSION_MAJOR} -DXTITAN_VERSION_MINOR=${XTITAN_VERSION_MINOR} -DXTITAN_VERSION_PATCH=${XTITAN_VERSION_PATCH})
foreach(type "RELEASE" "MINSIZEREL")
    set(CMAKE_CXX_FLAGS_${type} "${CMAKE_CXX_FLAGS_${type}} -DQT_NO_DEBUG_OUTPUT -DQT_NO_WARNING_OUTPUT")
endforeach()
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -pedantic -Wextra -Wall -pipe")
elseif(MSVC)
    # general flags
    add_definitions("/D__STDC_CONSTANT_MACROS")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:wchar_t- /wd4127 /wd4819 /MP /nologo")
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
    # release flags
    foreach(type "RELEASE" "MINSIZEREL")
        if(CMAKE_CXX_FLAGS_${type} MATCHES "/O[0-3d]")
            string(REGEX REPLACE "/O[0-3d]" "/Ox" CMAKE_CXX_FLAGS_${type} "${CMAKE_CXX_FLAGS_${type}}")
        else()
            set(CMAKE_CXX_FLAGS_${type} "${CMAKE_CXX_FLAGS_${type}} /Ox")
        endif()
    endforeach()
endif()
set(CMAKE_DEBUG_POSTFIX d)

add_subdirectory(core)
add_subdirectory(server)
add_subdirectory(client)

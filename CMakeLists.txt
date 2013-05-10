# root

cmake_minimum_required(VERSION 2.6)

project(xTitan)
set(XTITAN_VERSION_MAJOR "2")
set(XTITAN_VERSION_MINOR "0")
set(XTITAN_VERSION_PATCH "0")
set(XTITAN_VERSION "${XTITAN_VERSION_MAJOR}.${XTITAN_VERSION_MINOR}.${XTITAN_VERSION_PATCH}")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

find_package(Qt4 4.7 COMPONENTS QtNetwork QtScript REQUIRED)

include(${QT_USE_FILE})

add_definitions(-DXTITAN_VERSION=${XTITAN_VERSION} -DXTITAN_VERSION_MAJOR=${XTITAN_VERSION_MAJOR} -DXTITAN_VERSION_MINOR=${XTITAN_VERSION_MINOR} -DXTITAN_VERSION_PATCH=${XTITAN_VERSION_PATCH})
foreach(type "RELEASE" "MINSIZEREL")
    set(CMAKE_CXX_FLAGS_${type} "${CMAKE_CXX_FLAGS_${type}} -DQT_NO_DEBUG_OUTPUT -DQT_NO_WARNING_OUTPUT")
endforeach()
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++ -pedantic -Wextra -Wall -pipe")
elseif(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -pedantic -Wextra -Wall -pipe")
elseif(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:wchar_t- /wd4127 /wd4819 /MP /nologo")
endif()
set(CMAKE_DEBUG_POSTFIX d)

add_subdirectory(core)
add_subdirectory(driver)
add_subdirectory(spy)

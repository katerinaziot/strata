###
# Copyright (c) 2009, Paul Gideon Dann
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
###

cmake_minimum_required(VERSION 3.5)

# Default configuration values. These must be before the project command or
# they won't work in Windows.
# If no build type is specified, default to "Release"
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "None Debug Release RelWithDebInfo MinSizeRel" FORCE)
endif()
# Install to "dist" directory in Windows for testing and as a staging directory
# for the installer.
if (WIN32 AND NOT CMAKE_INSTALL_PREFIX)
    set(CMAKE_INSTALL_PREFIX dist CACHE STRING "Install path prefix.")
endif()

project(strata)
set(PROJECT_LONGNAME "Strata")

execute_process(
    COMMAND git rev-parse --short HEAD
    OUTPUT_VARIABLE PROJECT_GITHASH
    OUTPUT_STRIP_TRAILING_WHITESPACE)
set(PROJECT_VERSION "0.9.0")

# Global CMake options
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# Enable advanced features like:
#  - frequency-dependent moduli
option(ADVANCED_FEATURES "Compile with advanced features" ON)


if(APPLE AND EXISTS /usr/local/opt/qt5)
	# Homebrew installs Qt5 (up to at least 5.9.1) in
	# /usr/local/opt/qt5, ensure it can be found by CMake since
	# it is not in the default /usr/local prefix.
	list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/qt5")
endif()

# Configure libraries
find_package(Qt5PrintSupport REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Xml REQUIRED)
# find_package(Qt5Test REQUIRED)
find_package(GSL REQUIRED)
find_package(Qwt REQUIRED)

include_directories(${GSL_INCLUDE_DIRS} ${QWT_INCLUDE_DIRS})
set(LIBS ${LIBS} ${GSL_LIBRARIES} ${QWT_LIBRARIES})

# Enable the C++14 standard
set(CMAKE_CXX_STANDARD 14)

if (ADVANCED_FEATURES)
    add_definitions(-DADVANCED_FEATURES)
endif()

if(MSVC)
    # Add GSL DLL definition for MSVC. Add define for math constants
    add_definitions(-DGSL_DLL -D_USE_MATH_DEFINES)
    # Optimize for speed and link dynamically "-MD"
    set(CMAKE_CXX_FLAGS_RELEASE "-O2")
    # Dynamic linking
    set(CMAKE_CXX_FLAGS_RELEASE "/MD")
else()
    # Add optimization for GCC
    # SET(CMAKE_CXX_FLAGS_DEBUG "-O0")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()


# Testing configuration
enable_testing()
set(TEST_LINK_LIBRARIES Qt5::Test)

add_subdirectory(resources)
add_subdirectory(source)
add_subdirectory(tests)

find_program(PDFLATEX_PATH pdflatex)
if (PDFLATEX_PATH)
    add_subdirectory(manual)
endif()

if (WIN32)
    add_subdirectory(win)
endif()

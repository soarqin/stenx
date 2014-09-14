
cmake_minimum_required(VERSION 2.8)

# define a macro that helps defining an option
macro(set_option_default var default type docstring)
    if(NOT DEFINED ${var})
        set(${var} ${default})
    endif()
    set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)
endmacro()

# set a default build type if none was provided
# this has to be done before the project() instruction!
set_option_default(CMAKE_BUILD_TYPE Release STRING "Choose the type of build (Debug or Release)")

list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_SOURCE_DIR}/cmake")

# project name
project(StenX)

# include the configuration file
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/SFMLConfig.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/AddPch.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/AutoFiles.cmake)

# setup version numbers
set(MAJOR_VERSION 0)
set(MINOR_VERSION 1)
set(PATCH_VERSION 0)

# add the SFML header path
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Set some useful compile flags.
if( SFML_COMPILER_GCC )

    if( NOT CMAKE_CXX_FLAGS )
        set( CMAKE_CXX_FLAGS "-Wall -Wextra -Wshadow -Wno-long-long -pedantic -std=c++11" CACHE STRING "C++ compiler flags" FORCE )
        set( CMAKE_C_FLAGS "-Wall -Wextra -Wshadow -Wno-long-long -pedantic" CACHE STRING "C compiler flags" FORCE )
    endif()

elseif ( SFML_COMPILER_CLANG )

    if( NOT CMAKE_CXX_FLAGS )
        set( CMAKE_CXX_FLAGS "-Wall -Wextra -Wshadow -Wno-long-long -pedantic -std=c++11" CACHE STRING "C++ compiler flags" FORCE )
        set( CMAKE_C_FLAGS "-Wall -Wextra -Wshadow -Wno-long-long -pedantic" CACHE STRING "C compiler flags" FORCE )

        # On mac, clang needs another parameter
        if (SFML_OS_MACOSX)
            set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++" CACHE STRING "C++ compiler flags" FORCE )
        endif()
    endif()

endif()

# add an option for choosing the build type (shared or static)
if(NOT (SFML_OS_IOS OR SFML_OS_ANDROID))
    set_option_default(BUILD_SHARED_LIBS TRUE BOOL "TRUE to build SFML as shared libraries, FALSE to build it as static libraries")
else()
    if(SFML_OS_IOS)
        set(BUILD_SHARED_LIBS FALSE)
    elseif(SFML_OS_ANDROID)
        set(BUILD_SHARED_LIBS TRUE)
    endif()
endif()

# Android options
if(SFML_OS_ANDROID)
    # force usage of the STL port
    set(ANDROID_USE_STLPORT TRUE)

    # make sure there's the android library available
    if (${ANDROID_NATIVE_API_LEVEL} LESS 9)
        message(FATAL_ERROR "API level must be equal or greater than 9")
    endif()

    # this is a workaround to compile sfml-activity without stlport_shared as dependency
    # we save the original compilation command line to restore it later in Macro.cmake
    set(CMAKE_CXX_CREATE_SHARED_LIBRARY_WITH_STLPORT ${CMAKE_CXX_CREATE_SHARED_LIBRARY})
    set(CMAKE_CXX_CREATE_SHARED_LIBRARY_WITHOUT_STLPORT "<CMAKE_CXX_COMPILER> <CMAKE_SHARED_LIBRARY_CXX_FLAGS> <LANGUAGE_COMPILE_FLAGS> <LINK_FLAGS> <CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS> <SONAME_FLAG><TARGET_SONAME> -o <TARGET> <OBJECTS> <LINK_LIBRARIES>")
endif()

# define SFML_STATIC if the build type is not set to 'shared'
if(NOT BUILD_SHARED_LIBS)
    add_definitions(-DSFML_STATIC)
    set(SFML_STATIC_LIBRARIES TRUE)
endif()

# define an option for choosing between static and dynamic C runtime (Windows only)
if(SFML_OS_WINDOWS)
    # remove SL security warnings with Visual C++
    if(SFML_COMPILER_MSVC)
        add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    endif()

    set_option_default(USE_STATIC_STD_LIBS FALSE BOOL "TRUE to statically link to the standard libraries, FALSE to use them as DLLs")

    # the following combination of flags is not valid
    if (BUILD_SHARED_LIBS AND USE_STATIC_STD_LIBS)
        message(FATAL_ERROR "BUILD_SHARED_LIBS and USE_STATIC_STD_LIBS cannot be used together")
    endif()

    # for VC++, we can apply it globally by modifying the compiler flags
    if(SFML_COMPILER_MSVC AND USE_STATIC_STD_LIBS)
        foreach(flag
                CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
                CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
            if(${flag} MATCHES "/MD")
                string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
            endif()
        endforeach()
    endif()
endif()

# setup Mac OS X stuff
if(SFML_OS_MACOSX)
    # configure Xcode templates
    if(CMAKE_OSX_ARCHITECTURES)
        # maybe multiple arches are present in CMAKE_OSX_ARCHITECTURES
        # we simply need to replace ';' by ' ' (space) and store the result in XCODE_TEMPLATES_ARCH
        string(REPLACE ";" " " XCODE_TEMPLATES_ARCH "${CMAKE_OSX_ARCHITECTURES}")
    else()
        # no arch was provided to cmake, so we use the default one
        set(XCODE_TEMPLATES_ARCH "\$(NATIVE_ARCH_ACTUAL)")
    endif()
endif()

# enable project folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

# Find sfml (also look for the main component when using Visual Studio)
if (SFML_OS_WINDOWS AND SFML_COMPILER_MSVC)
    find_package( SFML 2 COMPONENTS main graphics window system )
else()
    find_package( SFML 2 COMPONENTS graphics window system )
endif()

# FindSFML couldn't find SFML.
if( NOT SFML_FOUND )
    set( SFML_ROOT "" CACHE PATH "SFML root directory" )
    message( FATAL_ERROR "CMake couldn't find SFML. Set the SFML_ROOT entry to SFML's root directory (containing \"include\" and \"lib\" directories)." )
endif()

find_package(TGUI 0.7 REQUIRED)

# FindTGUI couldn't find TGUI.
if( NOT TGUI_FOUND )
    set( TGUI_ROOT "" CACHE PATH "TGUI root directory" )
    message( FATAL_ERROR "CMake couldn't find TGUI. Set the TGUI_ROOT entry to TGUI's root directory (containing \"include\" and \"lib\" directories)." )
endif()

find_package(OpenGL REQUIRED)

# Set the path for the libraries
set( LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib" )

add_subdirectory(Engine)
add_subdirectory(MapEditor)

project(Engine)

AutoFiles(ENGINE_SRC "." "src" "\\.(cpp|hpp|c|h|inl)$")
AddPch("config.h" "config.cpp" ENGINE_SRC)

# add the sfml include directories
include_directories( "." ${SFML_INCLUDE_DIR} ${TGUI_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} )

# Determine library suffixes depending on static/shared configuration
if(BUILD_SHARED_LIBS)
    add_library(Engine SHARED ${ENGINE_SRC})
    set_target_properties( Engine PROPERTIES DEBUG_POSTFIX -d )
    set_target_properties( Engine PROPERTIES VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION} )

    # the library should be linked to sfml, unless you are on linux
    if (NOT SFML_OS_LINUX)
        target_link_libraries( Engine ${TGUI_LIBRARY} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES} ${OPENGL_gl_LIBRARY} )

        # on Windows/gcc get rid of "lib" prefix for shared libraries,
        # and transform the ".dll.a" suffix into ".a" for import libraries
        if (SFML_OS_WINDOWS AND SFML_COMPILER_GCC)
            set_target_properties(Engine PROPERTIES PREFIX "")
            set_target_properties(Engine PROPERTIES IMPORT_SUFFIX ".a")
        endif()
    endif()

else()
    add_definitions(-DSFML_STATIC)
    add_library(Engine STATIC ${ENGINE_SRC})
    set_target_properties(Engine PROPERTIES DEBUG_POSTFIX -s-d)
    set_target_properties(Engine PROPERTIES RELEASE_POSTFIX -s)
endif()

# for gcc >= 4.0 on Windows, apply the USE_STATIC_STD_LIBS option if it is enabled
if(SFML_OS_WINDOWS AND SFML_COMPILER_GCC AND NOT SFML_GCC_VERSION VERSION_LESS "4")
    if(USE_STATIC_STD_LIBS AND NOT SFML_COMPILER_GCC_TDM)
        set_target_properties(Engine PROPERTIES LINK_FLAGS "-static-libgcc -static-libstdc++")
    elseif(NOT USE_STATIC_STD_LIBS AND SFML_COMPILER_GCC_TDM)
        set_target_properties(Engine PROPERTIES LINK_FLAGS "-shared-libgcc -shared-libstdc++")
    endif()
endif()

# if using gcc >= 4.0 or clang >= 3.0 on a non-Windows platform, we must hide public symbols by default
# (exported ones are explicitely marked)
if(NOT SFML_OS_WINDOWS AND ((SFML_COMPILER_GCC AND NOT SFML_GCC_VERSION VERSION_LESS "4") OR (SFML_COMPILER_CLANG AND NOT SFML_CLANG_VERSION VERSION_LESS "3")))
    set_target_properties(Engine PROPERTIES COMPILE_FLAGS -fvisibility=hidden)
endif()

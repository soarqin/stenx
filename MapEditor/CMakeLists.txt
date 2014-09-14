project(MapEditor)

AutoFiles(MAPEDITOR_SRC "." "src" "\\.(cpp|hpp|c|h|inl)$")
AddPch("../Engine/config.h" "../Engine/config.cpp" MAPEDITOR_SRC)

if(NOT BUILD_SHARED_LIBS)
    add_definitions(-DSFML_STATIC)
endif()

# add the sfml include directories
include_directories( "." ${SFML_INCLUDE_DIR} ${TGUI_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} "../Engine" )

add_executable(MapEditor ${MAPEDITOR_SRC})
target_link_libraries(MapEditor Engine ${TGUI_LIBRARY} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES} ${OPENGL_gl_LIBRARY} )
set_target_properties(MapEditor PROPERTIES VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})

# for gcc >= 4.0 on Windows, apply the USE_STATIC_STD_LIBS option if it is enabled
if(SFML_OS_WINDOWS AND SFML_COMPILER_GCC AND NOT SFML_GCC_VERSION VERSION_LESS "4")
    if(USE_STATIC_STD_LIBS AND NOT SFML_COMPILER_GCC_TDM)
        set_target_properties(MapEditor PROPERTIES LINK_FLAGS "-static-libgcc -static-libstdc++")
    elseif(NOT USE_STATIC_STD_LIBS AND SFML_COMPILER_GCC_TDM)
        set_target_properties(MapEditor PROPERTIES LINK_FLAGS "-shared-libgcc -shared-libstdc++")
    endif()
endif()

# if using gcc >= 4.0 or clang >= 3.0 on a non-Windows platform, we must hide public symbols by default
# (exported ones are explicitely marked)
if(NOT SFML_OS_WINDOWS AND ((SFML_COMPILER_GCC AND NOT SFML_GCC_VERSION VERSION_LESS "4") OR (SFML_COMPILER_CLANG AND NOT SFML_CLANG_VERSION VERSION_LESS "3")))
    set_target_properties(MapEditor PROPERTIES COMPILE_FLAGS -fvisibility=hidden)
endif()

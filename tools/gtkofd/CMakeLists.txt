PROJECT(gtkofd)

#EXEC_PROGRAM(glib-compile-resources ARGS "--generate --generate-source --target gtkofd_resources.c --sourcedir ./ui gtkofd.gresource.xml")

ADD_CUSTOM_TARGET(
    Compile_gtkofd_resources ALL DEPENDS gtkofd.gresource.xml 
)

ADD_CUSTOM_COMMAND(TARGET Compile_gtkofd_resources
    PRE_BUILD
    COMMAND /usr/local/bin/glib-compile-resources --generate --generate-source --target ${CMAKE_CURRENT_SOURCE_DIR}/gtkofd_resources.c --sourcedir ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/gtkofd.gresource.xml
    COMMAND /usr/local/bin/glib-compile-resources --generate --generate-header --target ${CMAKE_CURRENT_SOURCE_DIR}/gtkofd_resources.h --sourcedir ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/gtkofd.gresource.xml
    COMMAND /usr/local/bin/glib-compile-resources --generate-dependencies --sourcedir ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/gtkofd.gresource.xml
)

AUX_SOURCE_DIRECTORY(. SRC_LIST)
ADD_EXECUTABLE(gtkofd ${SRC_LIST})

#SET_TARGET_PROPERTIES(gtkofd PROPERTIES LINK_SEARCH_START_STATIC 1)
#SET_TARGET_PROPERTIES(gtkofd PROPERTIES LINK_SEARCH_END_STATIC 1)
#SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")

#SET(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

INCLUDE(FindPkgConfig)

# -------- Gtk3 --------
FIND_PACKAGE(GTK3 REQUIRED)
INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})

# -------- Cairo --------
FIND_PACKAGE(Cairo REQUIRED)
INCLUDE_DIRECTORIES(${CAIRO_INCLUDE_DIRS})

# -------- libSDL2 --------
FIND_PACKAGE(SDL2 REQUIRED)
INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})
#LINK_LIBRARIES(${SDL2_LIBRARY_DIRS})

# -------- GLFW --------
FIND_PACKAGE(glfw3 REQUIRED)
INCLUDE_DIRECTORIES(${GLFW_INCLUDE_DIRS})
#LINK_LIBRARIES(${GLFW_LIBRARY_DIRS})

# -------- OpenGL --------
FIND_PACKAGE(OpenGL REQUIRED)
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIRS})

# -------- GLEW --------
FIND_PACKAGE(GLEW REQUIRED)
INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIRS})
#LINK_LIBRARIES(${GLEW_LIBRARY_DIRS})

SET(LIBOFD_LIBRARY ofd)
#SET(LIBOFD_LIBRARY libofd.a)
#FIND_LIBRARY(Static_libofd libofd.a)
#IF(Static_libofd)
    #SET(LIBOFD_LIBRARY ${Static_libofd})
#ENDIF(Static_libofd) 
TARGET_LINK_LIBRARIES(gtkofd ${LIBOFD_LIBRARY} ${CAIRO_LIBRARIES} ${POPPLER_LIBRARIES} ${GTK3_LIBRARIES} ${SDL2_LIBRARIES})

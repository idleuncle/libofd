PROJECT(gtkofd)

#INCLUDE_DIRECTORIES(
    #/usr/include/gtk-3.0
    #/usr/include/gdk-pixbuf-2.0
    #/usr/include/pango-1.0
    #/usr/include/atk-1.0
#)

#EXEC_PROGRAM(glib-compile-resources ARGS "--generate --generate-source --target gtkofd_resources.c --sourcedir ./ui gtkofd.gresource.xml")

ADD_CUSTOM_TARGET(
    Compile_gtkofd_resources ALL DEPENDS gtkofd.gresource.xml 
)

ADD_CUSTOM_COMMAND(TARGET Compile_gtkofd_resources
    PRE_BUILD
    COMMAND glib-compile-resources --generate --generate-source --target ${CMAKE_CURRENT_SOURCE_DIR}/gtkofd_resources.c --sourcedir ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/gtkofd.gresource.xml
    COMMAND glib-compile-resources --generate --generate-header --target ${CMAKE_CURRENT_SOURCE_DIR}/gtkofd_resources.h --sourcedir ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/gtkofd.gresource.xml
    COMMAND glib-compile-resources --generate-dependencies --sourcedir ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/gtkofd.gresource.xml
)

AUX_SOURCE_DIRECTORY(. SRC_LIST)
ADD_EXECUTABLE(gtkofd ${SRC_LIST})

#SET_TARGET_PROPERTIES(gtkofd PROPERTIES LINK_SEARCH_START_STATIC 1)
#SET_TARGET_PROPERTIES(gtkofd PROPERTIES LINK_SEARCH_END_STATIC 1)
#SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")

#SET(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

INCLUDE(FindPkgConfig)

## -------- Gtk3 --------
FIND_PACKAGE(GTK3 REQUIRED)
INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
# -------- GTK --------
#FIND_PACKAGE(GTK REQUIRED)
#INCLUDE_DIRECTORIES(${GTK_INCLUDE_DIR})

# -------- Glib --------
FIND_PACKAGE(GLIB REQUIRED)
INCLUDE_DIRECTORIES(${GLIB_INCLUDE_DIRS})

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
#TARGET_LINK_LIBRARIES(gtkofd ${LIBOFD_LIBRARY} utils ${CAIRO_LIBRARIES} ${POPPLER_LIBRARIES} ${GTK_LIBRARIES} gtk-3 gdk-3 pangocairo-1.0 pango-1.0 atk-1.0 gdk_pixbuf-2.0 gobject-2.0 gio-2.0 ${GLIB_LIBRARIES} ${SDL2_LIBRARIES})
TARGET_LINK_LIBRARIES(gtkofd ${LIBOFD_LIBRARY} utils ${CAIRO_LIBRARIES} ${POPPLER_LIBRARIES} ${GTK3_LIBRARIES} ${GLIB_LIBRARIES} ${SDL2_LIBRARIES})

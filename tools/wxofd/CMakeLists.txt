PROJECT(wxofd)

#ADD_CUSTOM_TARGET(
    #Compile_wxofd_icons ALL DEPENDS icons/toxpm.sh icons/fixxpm.sh 
#)

#ADD_CUSTOM_COMMAND(TARGET Compile_wxofd_icons
    #PRE_BUILD
    #COMMAND glib-compile-resources --generate --generate-source --target ${CMAKE_CURRENT_SOURCE_DIR}/gtkofd_resources.c --sourcedir ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/gtkofd.gresource.xml
#)

AUX_SOURCE_DIRECTORY(. SRC_LIST)
ADD_EXECUTABLE(wxofd ${SRC_LIST})

# -------- wxWidgets --------
FIND_PACKAGE(wxWidgets REQUIRED core base aui html)
INCLUDE(${wxWidgets_USE_FILE})
#INCLUDE_DIRECTORIES(${wxWidgets_INCLUDE_DIRS})

# -------- Cairo --------
FIND_PACKAGE(Cairo REQUIRED)
INCLUDE_DIRECTORIES(${CAIRO_INCLUDE_DIRS})

# -------- GFlags --------
FIND_PACKAGE(GFlags REQUIRED)
INCLUDE_DIRECTORIES(${GFLAGS_INCLUDE_DIRS})

TARGET_LINK_LIBRARIES(wxofd ofdreader ofd utils ${wxWidgets_LIBRARIES} ${POPPLER_LIBRARIES} ${CAIRO_LIBRARIES} )

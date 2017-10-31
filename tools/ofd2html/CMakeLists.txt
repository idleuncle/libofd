PROJECT(ofd2html)

AUX_SOURCE_DIRECTORY(. SRC_LIST)
AUX_SOURCE_DIRECTORY(HTMLRenderer SRC_LIST)
AUX_SOURCE_DIRECTORY(BackgroundRenderer SRC_LIST)
AUX_SOURCE_DIRECTORY(util SRC_LIST)
AUX_SOURCE_DIRECTORY(3rdparty/poppler/git SRC_LIST)

ADD_EXECUTABLE(ofd2html ${SRC_LIST})

SET(ENABLE_SVG 1)
SET(OFD2HTML_SOURCE_DIR ${CMAKE_SOURCE_DIR}/tools/ofd2html)

configure_file (${OFD2HTML_SOURCE_DIR}/pdf2htmlEX-config.h.in ${OFD2HTML_SOURCE_DIR}/pdf2htmlEX-config.h)
#configure_file (${OFD2HTML_SOURCE_DIR}/pdf2htmlEX.1.in ${OFD2HTML_SOURCE_DIR}/pdf2htmlEX.1)

include(${OFD2HTML_SOURCE_DIR}/css_class_names.cmakelists.txt)
configure_file (${OFD2HTML_SOURCE_DIR}/util/css_const.h.in ${OFD2HTML_SOURCE_DIR}/util/css_const.h)
configure_file (${OFD2HTML_SOURCE_DIR}/share/pdf2htmlEX.js.in ${OFD2HTML_SOURCE_DIR}/share/pdf2htmlEX.js)
configure_file (${OFD2HTML_SOURCE_DIR}/share/base.css.in ${OFD2HTML_SOURCE_DIR}/share/base.css)
configure_file (${OFD2HTML_SOURCE_DIR}/share/fancy.css.in ${OFD2HTML_SOURCE_DIR}/share/fancy.css)


add_custom_target(pdf2htmlEX_resources ALL DEPENDS
    ${OFD2HTML_SOURCE_DIR}/share/base.min.css
    ${OFD2HTML_SOURCE_DIR}/share/fancy.min.css
    ${OFD2HTML_SOURCE_DIR}/share/pdf2htmlEX.min.js
    )

add_custom_command(OUTPUT ${OFD2HTML_SOURCE_DIR}/share/pdf2htmlEX.min.js
    COMMAND ${OFD2HTML_SOURCE_DIR}/share/build_js.sh
    DEPENDS
        ${OFD2HTML_SOURCE_DIR}/share/build_js.sh
        ${OFD2HTML_SOURCE_DIR}/share/pdf2htmlEX.js
    )

add_custom_command(OUTPUT
        ${OFD2HTML_SOURCE_DIR}/share/base.min.css
        ${OFD2HTML_SOURCE_DIR}/share/fancy.min.css

    COMMAND ${OFD2HTML_SOURCE_DIR}/share/build_css.sh
    DEPENDS
        ${OFD2HTML_SOURCE_DIR}/share/build_css.sh
        ${OFD2HTML_SOURCE_DIR}/share/base.css
        ${OFD2HTML_SOURCE_DIR}/share/fancy.css
    )

INCLUDE_DIRECTORIES(
    ./
    ../
)

# -------- Gtk3 --------
FIND_PACKAGE(GTK3 REQUIRED)
INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})

# -------- Cairo --------
FIND_PACKAGE(Cairo REQUIRED)
INCLUDE_DIRECTORIES(${CAIRO_INCLUDE_DIRS})

## -------- Poppler --------
FIND_PACKAGE(Poppler REQUIRED)
INCLUDE_DIRECTORIES(${POPPLER_INCLUDE_DIRS})

# -------- Fontforge --------
FIND_PACKAGE(Fontforge REQUIRED)
INCLUDE_DIRECTORIES(${FONTFORGE_INCLUDE_DIRS})

## -------- GFlags --------
#FIND_PACKAGE(GFlags REQUIRED)
#INCLUDE_DIRECTORIES(${GFLAGS_INCLUDE_DIRS})

TARGET_LINK_LIBRARIES(ofd2html ofd utils ${GTK3_LIBRARIES} ${FONTFORGE_LIBRARIES}  ${POPPLER_LIBRARIES} ${CAIRO_LIBRARIES} )

PROJECT(libofd)

CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

INCLUDE_DIRECTORIES(
    ./
    ./include
    ./tools/libjpeg-turbo
    ./tools/poppler
    ./tools/poppler/poppler
    ./tools/fontforge/inc
    ./tools/fontforge/fontforge
)

LINK_DIRECTORIES(
    ./build/lib
    ./tools/libjpeg-turbo/.libs
    ./tools/poppler/build
    ./tools/poppler/build/glib
    ./tools/fontforge/fontforge/.libs
    /usr/local/lib
)

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

INCLUDE(FindPkgConfig)

# -------- Cairo --------
FIND_PACKAGE(Cairo REQUIRED)
INCLUDE_DIRECTORIES(${CAIRO_INCLUDE_DIRS})

# -------- Freetype --------
FIND_PACKAGE(Freetype REQUIRED)
INCLUDE_DIRECTORIES(${FREETYPE_INCLUDE_DIRS})

# -------- HarfBuzz --------
FIND_PACKAGE(Harfbuzz REQUIRED)
INCLUDE_DIRECTORIES(${HARFBUZZ_INCLUDE_DIRS})

## -------- Poppler --------
#FIND_PACKAGE(Poppler REQUIRED)
#INCLUDE_DIRECTORIES(${POPPLER_INCLUDE_DIRS})

# -------- Darwin Libraries --------
if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    SET(LIBDARWIN "-framework Cocoa -framework IOKit")
endif ()

# -------- CXX Compile Options --------
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -Wall -Werror -Wno-attributes -Wno-c++11-extensions")
SET(LIBOFD_DEPENDCIES ${FREETYPE_LIBRARIES} ${HARFBUZZ_LIBRARIES} ${CAIRO_LIBRARIES} harfbuzz-icu fontforge poppler poppler-glib stdc++)

# -------- LD_FLAGS Options --------
# -lbfd  backward add line and code snipts.
SET(CMAKE_LD_FLAGS "${CMAKE_LD_FLAGS} -lbfd")

# -std=c++11
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

# cmake policy CMP0042
# ``MACOSX_RPATH`` is enabled by default
SET(CMAKE_MACOSX_RPATH 0)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

ADD_SUBDIRECTORY(utils)
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(tests)
ADD_SUBDIRECTORY(tools)

ADD_TEST(ofdtest ${PROJECT_BINARY_DIR}/bin/ofdtest)
ENABLE_TESTING()

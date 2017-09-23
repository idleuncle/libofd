PKG_CHECK_MODULES(PC_SDL2 SDL2>=2.0)
FIND_PATH(SDL2_INCLUDE_DIRS NAMES SDL.h
    HINTS ${PC_SDL2_INCLUDE_DIRS} ${PC_SDL2_INCLUDEDIR}
)

FIND_LIBRARY(SDL2_LIBRARIES NAMES SDL2
    HINTS ${PC_SDL2_LIBRARY_DIRS} ${PC_SDL2_LIBDIR}
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SDL2 DEFAULT_MSG SDL2_INCLUDE_DIRS SDL2_LIBRARIES)

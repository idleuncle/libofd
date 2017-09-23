PKG_CHECK_MODULES(GLIB "glib-2.0>=2.0")

FIND_PATH(GLIB_INCLUDE_DIRS NAMES glib.h
    HINTS ${PC_GLIB_INCLUDE_DIRS} ${PC_GLIB_INCLUDEDIR} 
)

FIND_LIBRARY(GLIB_LIBRARIES NAMES glib
    HINTS ${PC_GLIB_LIBRARY_DIRS} ${PC_GLIB_LIBDIR} 
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLIB DEFAULT_MSG GLIB_INCLUDE_DIRS GLIB_LIBRARIES)

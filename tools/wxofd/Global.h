#ifndef __WXOFD_GLOBAL_H__
#define __WXOFD_GLOBAL_H__

enum {
    ID_USER_BEGIN = wxID_HIGHEST+1,

    ID_CustomizeToolbar,
    ID_CreatePerspective,

    ID_FILE_OPEN,
    ID_FILE_NEW,
    ID_FILE_NEW_FROM_FILE,
    ID_FILE_NEW_FROM_SCANNER,

    ID_SampleItem, // must be last

    ID_FirstPerspective = ID_CreatePerspective+1000
};

#endif // __WXOFD_GLOBAL_H__

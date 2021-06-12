#include "cursor.h"

/* ----------------------------------------- Windows implementation ------------------------- */

#ifdef CSFML_SYSTEM_WINDOWS
#include <windows.h>

// load and set the OS cursor to the specified one
int cursor_set(cursor_enum cursor){
    HCURSOR cursor_handle;

    switch(cursor){
        case cursor_arrow:
            cursor_handle = LoadCursorA(NULL, IDC_ARROW);
        break;

        case cursor_text:
            cursor_handle = LoadCursorA(NULL, IDC_IBEAM);
        break;

        case cursor_blocked:
            cursor_handle = LoadCursorA(NULL, IDC_NO);
        break;

        case cursor_question_mark:
            cursor_handle = LoadCursorA(NULL, IDC_HELP);
        break;

        case cursor_loading:
            cursor_handle = LoadCursorA(NULL, IDC_WAIT);
        break;

        case cursor_hand:
            cursor_handle = LoadCursorA(NULL, IDC_HAND);
        break;

        case cursor_move:
            cursor_handle = LoadCursorA(NULL, IDC_SIZEALL);
        break;

        case cursor_resize_vertical:
            cursor_handle = LoadCursorA(NULL, IDC_SIZENS);
        break;

        case cursor_resize_horizontal:
            cursor_handle = LoadCursorA(NULL, IDC_SIZEWE);
        break;

        case cursor_resize_45_deg:
            cursor_handle = LoadCursorA(NULL, IDC_SIZENESW);
        break;

        case cursor_resize_135_deg:
            cursor_handle = LoadCursorA(NULL, IDC_SIZENWSE);
        break;

        default:
            cursor_handle = NULL;
        break;
    }

    if(cursor_handle != NULL){
        SetCursor(cursor_handle);
        return 1;
    }
    else{
        return 0;
    }
}

/* ----------------------------------------- Linux implementation --------------------------- */

#elif defined(CSFML_SYSTEM_LINUX)
#include <X11/cursorfont.h>
#include <X11/Xlib.h>
#error Linux cursor capabilities are not implemented yet

/* ----------------------------------------- MacOS implementation --------------------------- */

#elif defined(CSFML_SYSTEM_MACOS)
#error MacOS cursor capabilities are not implemented yet

/* non suported OS */    
#else
    #error The current OS is not supported
#endif // OS function
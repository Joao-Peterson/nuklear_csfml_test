#ifndef _CURSOR_HEADER_
#define _CURSOR_HEADER_

#include <SFML/System.h>

/* ----------------------------------------- Enumerators ------------------------------------ */

// types of cursors
typedef enum{
    cursor_arrow,
    cursor_text,
    cursor_hand,
    cursor_blocked,
    cursor_question_mark,
    cursor_loading,
    cursor_move,
    cursor_resize_vertical,
    cursor_resize_horizontal,
    cursor_resize_45_deg,
    cursor_resize_135_deg,
}cursor_enum;

/* ----------------------------------------- Functions -------------------------------------- */

// load and set the OS cursor to the specified one
int cursor_set(cursor_enum cursor);


#endif // _CURSOR_HEADER_
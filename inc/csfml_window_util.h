#ifndef _CSFML_WINDOW_UTIL_HEADER_
#define _CSFML_WINDOW_UTIL_HEADER_

#include <SFML/Graphics.h>

/* ----------------------------------------- Macros ----------------------------------------- */

// check if number is inside a range
#define IS_INSIDE_RANGE(x, a, b) ((a) <= (x) && (x) < (b))

// check if a coordenate is inside a box defined by a top left position and its width and height
#define IS_INSIDE_BOX(px, py, x0, y0, x1, y1) (IS_INSIDE_RANGE(px,x0,x1) && IS_INSIDE_RANGE(py,y0,y1))

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

// positions of the cursor on screen
typedef enum{
    cursor_pos_border_nw = 1,
    cursor_pos_border_n,
    cursor_pos_border_ne,
    cursor_pos_border_e,
    cursor_pos_border_se,
    cursor_pos_border_s,
    cursor_pos_border_sw,
    cursor_pos_border_w,
    cursor_pos_topbar
}cursor_pos_enum;

/* ----------------------------------------- Functions -------------------------------------- */

// load and set the OS cursor to the specified one
int csfml_cursor_set(cursor_enum cursor);

// check where the cursor is based on a clearence, change the cursor based on that and return the position
cursor_pos_enum csfml_window_scan(sfRenderWindow *window, sfEvent event, int border_clearence, int topbar_x0, int topbar_y0, int topbar_x1, int topbar_y1);

// transfor the window position and size if the conditions are met
void csfml_window_transform(sfRenderWindow *window, sfEvent event, cursor_pos_enum resize_dir, int *drag_anchor_x, int *drag_anchor_y, int clip_x, int clip_y);

// minimize window
void csfml_window_minimize(sfRenderWindow *window);

// maximize window
void csfml_window_maximize(sfRenderWindow *window);

// float window
void csfml_window_float(sfRenderWindow *window);

#endif
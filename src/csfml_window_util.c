#include "csfml_window_util.h"
#include <stdbool.h>

/* ----------------------------------------- Macros ----------------------------------------- */

// check if number is inside a range
#define IS_INSIDE_RANGE(x, a, b) ((a) <= (x) && (x) < (b))

// check if a coordenate is inside a box defined by a top left position and its width and height
#define IS_INSIDE_BOX(px, py, x0, y0, x1, y1) (IS_INSIDE_RANGE(px,x0,x1) && IS_INSIDE_RANGE(py,y0,y1))

/* ----------------------------------------- Windows implementation ------------------------- */

#ifdef CSFML_SYSTEM_WINDOWS
#include <windows.h>

// load and set the OS cursor to the specified one
int csfml_cursor_set(cursor_enum cursor){
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

// check where the cursor is based on a clearence, change the cursor based on that and return the region where it is.
// there are 8 cardinal regions based on the window size and a offset, and 1 more region wich resigns below the north most region, inside the defined topbar,
// wich is defined actually the title portion of the topbar, excluding the menu buttons to the left and window buttons to the right. 
cursor_pos_enum csfml_window_scan(sfRenderWindow *window, sfEvent event, int border_clearence, int topbar_x0, int topbar_y0, int topbar_x1, int topbar_y1){
    sfVector2u window_size = sfRenderWindow_getSize(window);
    cursor_pos_enum pos = 0;

    if(IS_INSIDE_BOX(event.mouseMove.x, event.mouseMove.y, border_clearence, topbar_y1, window_size.x - border_clearence, window_size.y - border_clearence)){ // outside any region
        pos = 0;
    }
    else if(IS_INSIDE_RANGE(event.mouseMove.x, 0, border_clearence)){                                       // left side
        if(IS_INSIDE_RANGE(event.mouseMove.y, 0, border_clearence)){                                        // north west
            pos = cursor_pos_border_nw;
        }
        else if(IS_INSIDE_RANGE(event.mouseMove.y, border_clearence, window_size.y - border_clearence)){    // west
            pos = cursor_pos_border_w ;
        }
        else{                                                                                               // south west
            pos = cursor_pos_border_sw;
        }
    }
    else if(IS_INSIDE_RANGE(event.mouseMove.x, window_size.x - border_clearence, window_size.x)){           // right side
        if(IS_INSIDE_RANGE(event.mouseMove.y, 0, border_clearence)){                                        // north east
            pos = cursor_pos_border_ne;
        }
        else if(IS_INSIDE_RANGE(event.mouseMove.y, border_clearence, window_size.y - border_clearence)){    // east
            pos = cursor_pos_border_e;
        }
        else{                                                                                               // south east
            pos = cursor_pos_border_se;
        }
    }
    else{                                                                                                   // middle
        if(IS_INSIDE_RANGE(event.mouseMove.y, 0, border_clearence)){                                        // north
            pos = cursor_pos_border_n;
        }
        else if(IS_INSIDE_BOX(event.mouseMove.x, event.mouseMove.y, topbar_x0, topbar_y0 + border_clearence, topbar_x1, topbar_y1)){      
            pos = cursor_pos_topbar;                                                                        // top bar, for moving the window
        }
        else if(IS_INSIDE_RANGE(event.mouseMove.y, window_size.y - border_clearence, window_size.y)){       // south
            pos = cursor_pos_border_s;
        }
        else{                                                                                               // outside all regions
            pos = 0;
        }
    }

    switch(pos){                                                                    // set cursor sprite
        case cursor_pos_border_n:
        case cursor_pos_border_s:
            csfml_cursor_set(cursor_resize_vertical);
        break;

        case cursor_pos_border_w:    
        case cursor_pos_border_e:
            csfml_cursor_set(cursor_resize_horizontal);
        break;
    
        case cursor_pos_border_nw:
        case cursor_pos_border_se:
            csfml_cursor_set(cursor_resize_135_deg);
        break;
    
        // case cursor_pos_border_ne:
        case cursor_pos_border_sw:
            csfml_cursor_set(cursor_resize_45_deg);
        break;

        case cursor_pos_topbar:
            // csfml_cursor_set(cursor_move);
        break;

        default:
            csfml_cursor_set(cursor_arrow);
        break;
    }

    return pos;
}

// resize the window if the conditions are met and in the desired direction
void csfml_window_transform(sfRenderWindow *window, sfEvent event, cursor_pos_enum resize_dir, sfVector2i *drag_anchor, int clip_x, int clip_y){
    sfVector2u window_size = sfRenderWindow_getSize(window);
    sfVector2i window_pos  = sfRenderWindow_getPosition(window);
    struct {int x; int y;} delta;

    switch(resize_dir){
        case cursor_pos_border_n:
            csfml_cursor_set(cursor_resize_vertical);
            delta.y = drag_anchor->y - event.mouseMove.y;
            delta.y /= 2;
            window_size.y += delta.y;
            window_pos.y -= delta.y;
            drag_anchor->y = event.mouseMove.y / 2;
        break;

        case cursor_pos_border_s:
            csfml_cursor_set(cursor_resize_vertical);
            delta.y = drag_anchor->y - event.mouseMove.y;
            // delta.y /= 2;
            window_size.y -= delta.y;
            // window_pos.y -= delta.y;
            drag_anchor->y = event.mouseMove.y;
        break;

        case cursor_pos_border_w:
            csfml_cursor_set(cursor_resize_horizontal);
            delta.x = drag_anchor->x - event.mouseMove.x;
            delta.x /= 2;
            window_size.x += delta.x;
            window_pos.x -= delta.x;
            drag_anchor->x = event.mouseMove.x / 2;
        break;

        case cursor_pos_border_e:
            csfml_cursor_set(cursor_resize_horizontal);
            delta.x = drag_anchor->x - event.mouseMove.x;
            // delta.x /= 2;
            window_size.x -= delta.x;
            // window_pos.x -= delta.x;
            drag_anchor->x = event.mouseMove.x;
        break;

        case cursor_pos_border_nw:
            csfml_cursor_set(cursor_resize_135_deg);
            delta.x = drag_anchor->x - event.mouseMove.x;
            delta.x /= 2;
            window_size.x += delta.x;
            window_pos.x -= delta.x;
            drag_anchor->x = event.mouseMove.x / 2;
            delta.y = drag_anchor->y - event.mouseMove.y;
            delta.y /= 2;
            window_size.y += delta.y;
            window_pos.y -= delta.y;
            drag_anchor->y = event.mouseMove.y / 2;
        break;

        case cursor_pos_border_ne:
            csfml_cursor_set(cursor_resize_45_deg);
            delta.y = drag_anchor->y - event.mouseMove.y;
            delta.y /= 2;
            window_size.y += delta.y;
            window_pos.y -= delta.y;
            drag_anchor->y = event.mouseMove.y / 2;
            delta.x = drag_anchor->x - event.mouseMove.x;
            // delta.x /= 2;
            window_size.x -= delta.x;
            // window_pos.x -= delta.x;
            drag_anchor->x = event.mouseMove.x;
        break;

        case cursor_pos_border_sw:
            csfml_cursor_set(cursor_resize_45_deg);
            delta.y = drag_anchor->y - event.mouseMove.y;
            // delta.y /= 2;
            window_size.y -= delta.y;
            // window_pos.y -= delta.y;
            drag_anchor->y = event.mouseMove.y;
            delta.x = drag_anchor->x - event.mouseMove.x;
            delta.x /= 2;
            window_size.x += delta.x;
            window_pos.x -= delta.x;
            drag_anchor->x = event.mouseMove.x / 2;
        break;

        case cursor_pos_border_se:
            csfml_cursor_set(cursor_resize_135_deg);
            delta.y = drag_anchor->y - event.mouseMove.y;
            // delta.y /= 2;
            window_size.y -= delta.y;
            // window_pos.y -= delta.y;
            drag_anchor->y = event.mouseMove.y;
            delta.x = drag_anchor->x - event.mouseMove.x;
            // delta.x /= 2;
            window_size.x -= delta.x;
            // window_pos.x -= delta.x;
            drag_anchor->x = event.mouseMove.x;
        break;

        case cursor_pos_topbar:
            csfml_cursor_set(cursor_move);
            window_pos.x -= (drag_anchor->x - event.mouseMove.x);
            window_pos.y -= (drag_anchor->y - event.mouseMove.y);
        break;

        default:
            return;
        break;
    }

    window_size.x = (window_size.x < clip_x) ? clip_x : window_size.x; 
    window_size.y = (window_size.y < clip_y) ? clip_y : window_size.y; 

    sfRenderWindow_setPosition(window, window_pos);
    sfRenderWindow_setSize(window, window_size);
}

// fullscreen window
void csfml_window_fullscreen(sfRenderWindow *window){
    ShowWindow(sfRenderWindow_getSystemHandle(window), SW_SHOWMAXIMIZED);
}

// maximize window
void csfml_window_maximize(sfRenderWindow *window){
    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    sfVector2u window_size;
    sfVector2i window_pos;
    sfRenderWindow_setSize(window, (sfVector2u){.x = desktop.right, .y = desktop.bottom});    
    sfRenderWindow_setPosition(window, (sfVector2i){.x = 0, .y = 0});
}

// minimize window
void csfml_window_minimize(sfRenderWindow *window){
    ShowWindow(sfRenderWindow_getSystemHandle(window), SW_MINIMIZE);
}

// float window
void csfml_window_float(sfRenderWindow *window){
    ShowWindow(sfRenderWindow_getSystemHandle(window), SW_SHOWNORMAL);
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
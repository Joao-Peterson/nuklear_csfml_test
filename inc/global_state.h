#ifndef _GLOBAL_STATE_HEADER_
#define _GLOBAL_STATE_HEADER_

#include <stdbool.h>
#include "cursor.h"
#include "csfml_resize.h"

/* ----------------------------------------- Structs ---------------------------------------- */

// simple point struct
typedef struct{
    int x;
    int y;
}mouse_pos_t;

// global state
typedef struct{
    bool sfml_running;                      /**< state of SFML window, true if running, when set to false the window is killed*/
    char *window_title;                     /**< the title of the SFML window*/
    char *topbar_title;                     /**< the title to be displayed on the topbar of the nuklear window*/
    cursor_pos_enum cursor_pos;             /**< the cursor position based on SFML event mouse_moved, used rule calls on mouse clicks*/
    struct mouse_button_held_t{             /**< stores the state of mouse buttons, 1 if held down*/
        mouse_pos_t anchor;
        bool mouse_left;       
        bool mouse_right;      
        bool mouse_middle;       
        bool mouse_xButton1;    
        bool mouse_xButton2;    
    }mouse_button_held;
}state_t;

/* ----------------------------------------- Globals ---------------------------------------- */

state_t global_state;

#endif
#ifndef _GLOBAL_STATE_HEADER_
#define _GLOBAL_STATE_HEADER_

#include <stdbool.h>
#include <SFML/Graphics.h>
#include "csfml_window_util.h"
#include "nk_mygui.h"

/* ----------------------------------------- Structs ---------------------------------------- */

// global state
typedef struct{
    bool sfml_running;                      /**< state of SFML window, true if running, when set to false the window is killed*/
    bool first_render_loop;                 /**< true if first time of render loop*/
    window_mode_enum sfml_window_mode_flag; /**< flags SFML to change the window mode based on window_mode_enum enumerator*/
    window_mode_enum sfml_window_mode;      /**< indicates what mode/state the window is based on window_mode_enum enumerator*/
    char *window_title;                     /**< the title of the SFML window*/
    char *topbar_title;                     /**< the title to be displayed on the topbar of the nuklear window*/
    int topbar_menus_width;                 /**< width of the topbar menus, after wich the title coes on the x direction*/
    int topbar_title_width;                 /**< width of the topbar title, where the move cursor/cmd should be possible*/
    cursor_pos_enum cursor_pos;             /**< the cursor position based on SFML event mouse_moved, used rule calls on mouse clicks*/
    struct mouse_button_held_t{             /**< stores the state of mouse buttons, 1 if held down*/
        sfVector2i anchor;                      /**< stores the postion where the cursor has last clicked */
        bool mouse_left;       
        bool mouse_right;      
        bool mouse_middle;       
        bool mouse_xButton1;    
        bool mouse_xButton2;    
    }mouse_button_held;
    texture_t *texture;                     /**< pointer to a texture struct containing nk_image for nuklear widgets*/
}state_t;

/* ----------------------------------------- Globals ---------------------------------------- */

extern state_t global_state;

#endif
#ifndef _CSFML_RESIZE_HEADER_
#define _CSFML_RESIZE_HEADER_

#include <stdbool.h>
#include <SFML/Window.h>
#include "cursor.h"

/* ----------------------------------------- Macros ----------------------------------------- */

// check if number is inside a range
#define IS_INSIDE_RANGE(x, a, b) ((a) <= (x) && (x) < (b))

// check if a coordenate is inside a box defined by a top left position and its width and height
#define IS_INSIDE_BOX(px, py, x0, y0, x1, y1) (IS_INSIDE_RANGE(px,x0,x1) && IS_INSIDE_RANGE(py,y0,y1))

/* ----------------------------------------- Enumerators ------------------------------------ */

typedef enum{
    cursor_pos_border_nw = 1,
    cursor_pos_border_n,
    cursor_pos_border_ne,
    cursor_pos_border_e,
    cursor_pos_border_se,
    cursor_pos_border_s,
    cursor_pos_border_sw,
    cursor_pos_border_w
}cursor_pos_enum;

/* ----------------------------------------- Functions -------------------------------------- */

// check where the cursor is based on a clearence, change the cursor based on that and return the position
cursor_pos_enum csfml_resize_scan(sfRenderWindow *window, sfEvent event, int clearence){
    sfVector2u window_size = sfRenderWindow_getSize(window);
    cursor_pos_enum pos;

    if(IS_INSIDE_BOX(event.mouseMove.x, event.mouseMove.y, clearence, clearence, window_size.x - clearence, window_size.y - clearence)){    // is inner than the borders
        pos = 0;
    }
    else if(IS_INSIDE_RANGE(event.mouseMove.x, 0, clearence)){                                          // left side
        if(IS_INSIDE_RANGE(event.mouseMove.y, 0, clearence)){
            pos = cursor_pos_border_nw;
        }
        else if(IS_INSIDE_RANGE(event.mouseMove.y, clearence, window_size.y - clearence)){
            pos = cursor_pos_border_w ;
        }
        else{
            pos = cursor_pos_border_sw;
        }
    }
    else if(IS_INSIDE_RANGE(event.mouseMove.x, window_size.x - clearence, window_size.x)){              // right side
        if(IS_INSIDE_RANGE(event.mouseMove.y, 0, clearence)){
            pos = cursor_pos_border_ne;
        }
        else if(IS_INSIDE_RANGE(event.mouseMove.y, clearence, window_size.y - clearence)){
            pos = cursor_pos_border_e;
        }
        else{
            pos = cursor_pos_border_se;
        }
    }
    else{                                                                                               // middle
        if(IS_INSIDE_RANGE(event.mouseMove.y, 0, clearence)){
            pos = cursor_pos_border_n;
        }
        else if(IS_INSIDE_RANGE(event.mouseMove.y, window_size.y - clearence, window_size.y)){
            pos = cursor_pos_border_s;
        }
        else{
            pos = 0;
        }
    }

    switch(pos){                                                                    // set cursor sprite
        case cursor_pos_border_n:
        case cursor_pos_border_s:
            cursor_set(cursor_resize_vertical);
        break;

        case cursor_pos_border_w:    
        case cursor_pos_border_e:
            cursor_set(cursor_resize_horizontal);
        break;
    
        case cursor_pos_border_nw:
        case cursor_pos_border_se:
            cursor_set(cursor_resize_135_deg);
        break;
    
        case cursor_pos_border_ne:
        case cursor_pos_border_sw:
            cursor_set(cursor_resize_45_deg);
        break;

        default:
            cursor_set(cursor_arrow);
        break;
    }

    return pos;
}

// resize the window if the conditions are met and in the desired direction
void csfml_resize(sfRenderWindow *window, sfEvent event, cursor_pos_enum resize_dir, int *drag_anchor_x, int *drag_anchor_y, int clip_x, int clip_y){
    sfVector2u window_size = sfRenderWindow_getSize(window);
    sfVector2i window_pos  = sfRenderWindow_getPosition(window);
    struct {int x; int y;} delta;

    switch(resize_dir){
        case cursor_pos_border_n:
            cursor_set(cursor_resize_vertical);
            delta.y = *drag_anchor_y - event.mouseMove.y;
            delta.y /= 2;
            window_size.y += delta.y;
            window_pos.y -= delta.y;
            *drag_anchor_y = event.mouseMove.y / 2;
        break;

        case cursor_pos_border_s:
            cursor_set(cursor_resize_vertical);
            delta.y = *drag_anchor_y - event.mouseMove.y;
            // delta.y /= 2;
            window_size.y -= delta.y;
            // window_pos.y -= delta.y;
            *drag_anchor_y = event.mouseMove.y;
        break;

        case cursor_pos_border_w:
            cursor_set(cursor_resize_horizontal);
            delta.x = *drag_anchor_x - event.mouseMove.x;
            delta.x /= 2;
            window_size.x += delta.x;
            window_pos.x -= delta.x;
            *drag_anchor_x = event.mouseMove.x / 2;
        break;

        case cursor_pos_border_e:
            cursor_set(cursor_resize_horizontal);
            delta.x = *drag_anchor_x - event.mouseMove.x;
            // delta.x /= 2;
            window_size.x -= delta.x;
            // window_pos.x -= delta.x;
            *drag_anchor_x = event.mouseMove.x;
        break;

        case cursor_pos_border_nw:
            cursor_set(cursor_resize_135_deg);
            delta.x = *drag_anchor_x - event.mouseMove.x;
            delta.x /= 2;
            window_size.x += delta.x;
            window_pos.x -= delta.x;
            *drag_anchor_x = event.mouseMove.x / 2;
            delta.y = *drag_anchor_y - event.mouseMove.y;
            delta.y /= 2;
            window_size.y += delta.y;
            window_pos.y -= delta.y;
            *drag_anchor_y = event.mouseMove.y / 2;
        break;

        case cursor_pos_border_ne:
            cursor_set(cursor_resize_45_deg);
            delta.y = *drag_anchor_y - event.mouseMove.y;
            delta.y /= 2;
            window_size.y += delta.y;
            window_pos.y -= delta.y;
            *drag_anchor_y = event.mouseMove.y / 2;
            delta.x = *drag_anchor_x - event.mouseMove.x;
            // delta.x /= 2;
            window_size.x -= delta.x;
            // window_pos.x -= delta.x;
            *drag_anchor_x = event.mouseMove.x;
        break;

        case cursor_pos_border_sw:
            cursor_set(cursor_resize_45_deg);
            delta.y = *drag_anchor_y - event.mouseMove.y;
            // delta.y /= 2;
            window_size.y -= delta.y;
            // window_pos.y -= delta.y;
            *drag_anchor_y = event.mouseMove.y;
            delta.x = *drag_anchor_x - event.mouseMove.x;
            delta.x /= 2;
            window_size.x += delta.x;
            window_pos.x -= delta.x;
            *drag_anchor_x = event.mouseMove.x / 2;
        break;

        case cursor_pos_border_se:
            cursor_set(cursor_resize_135_deg);
            delta.y = *drag_anchor_y - event.mouseMove.y;
            // delta.y /= 2;
            window_size.y -= delta.y;
            // window_pos.y -= delta.y;
            *drag_anchor_y = event.mouseMove.y;
            delta.x = *drag_anchor_x - event.mouseMove.x;
            // delta.x /= 2;
            window_size.x -= delta.x;
            // window_pos.x -= delta.x;
            *drag_anchor_x = event.mouseMove.x;
        break;
    }

    window_size.x = (window_size.x < clip_x) ? clip_x : window_size.x; 
    window_size.y = (window_size.y < clip_y) ? clip_y : window_size.y; 

    sfRenderWindow_setPosition(window, window_pos);
    sfRenderWindow_setSize(window, window_size);
    glViewport(0, 0, window_size.x, window_size.y);
}

#endif
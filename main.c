#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <SFML/Graphics.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#include <nuklear.h>

#include "inc/nuklear_csfml.h"
#include "inc/mygui.h"
#include "inc/doc.h"
#include "inc/doc_json.h"
#include "inc/global_state.h"
#include "inc/csfml_window_util.h"

#define CFG_FILE "cfg.json"

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

int main(int argc, char **argv){

    /* global state */
    if(global_state_init(CFG_FILE)){
        return 0;
    }

    /* csfml */
    sfVideoMode mode;
    mode.width  = state.settings.parameters.main_window.size.w;
    mode.height = state.settings.parameters.main_window.size.h;
    mode.bitsPerPixel = 24;
    sfRenderWindow *window = sfRenderWindow_create(
        mode,
        state.window_title,
        0,
        NULL
    );
    sfRenderWindow_setVerticalSyncEnabled(window, sfTrue);
    sfRenderWindow_setActive(window, sfTrue);


    /* opengl viewport */
    if(!gladLoadGL()) {                                                             // load opengl extensions
        printf("Failed to load OpenGL extensions!\n");
        return -1;
    }


    /* nuklear */
    struct nk_font_atlas *atlas;
    nk_csfml_font_stash_begin(&atlas);                                              // fonts
    struct nk_font *font = nk_font_atlas_add_from_file(atlas, state.settings.parameters.text.font.file, state.settings.parameters.text.font.height, NULL);
    if(font == NULL){
        printf("Missing or invalid font.\n");
        return 0;
    }
    nk_csfml_font_stash_end();

    struct nk_context *context;                                                     // nuklear context
    context = nk_csfml_init(window, &font->handle);
    
    /* video settings */
    mygui_styles(context);                                                            // textures and styles
    sfVector2u window_size;
    sfVector2i window_pos;
    window_size.x = state.settings.parameters.main_window.size.w;
    window_size.y = state.settings.parameters.main_window.size.h;
    window_pos.x  = state.settings.parameters.main_window.size.x;
    window_pos.y  = state.settings.parameters.main_window.size.y;
    sfRenderWindow_setSize(window, window_size);
    sfRenderWindow_setPosition(window, window_pos);

    state.sfml_window_mode = window_mode_float;
    if(state.settings.parameters.main_window.size.fullscreen){
        state.sfml_window_mode_flag = window_mode_fullscreen;
    }
    else if(state.settings.parameters.main_window.size.maximized){
        state.sfml_window_mode_flag = window_mode_maximized;
    }

    /* render loop */
    state.sfml_running = sfRenderWindow_isOpen(window);
    int clearence = state.settings.parameters.main_window.resize.clearence;
    int minx = state.settings.parameters.main_window.resize.min.x;
    int miny = state.settings.parameters.main_window.resize.min.y;
    int topbar_height = state.settings.parameters.main_window.topbar.height;
    state.first_render_loop = true;
    while(state.sfml_running){ 
        sfEvent event;

        nk_input_begin(context);
        /* handle events loop */
        while(sfRenderWindow_pollEvent(window, &event)){
            switch(event.type){                                                     // sfml events
                case sfEvtClosed:
                    state.sfml_running = false;
                    sfRenderWindow_close(window);
                break;

                case sfEvtResized:                                                  // on window resize
                    glViewport(0, 0, event.size.width, event.size.height);
                break;

                case sfEvtMouseMoved:                                               // check mouse on the sides of window for resize
                    // only apply window transform logic when not in fulscreen/maximized mode
                    if(state.sfml_window_mode != window_mode_fullscreen && state.sfml_window_mode != window_mode_maximized){
                        if(state.mouse_button_held.mouse_left){              // moving while helding the mouse left down
                            csfml_window_transform(
                                window, 
                                event, 
                                state.cursor_pos, 
                                &state.mouse_button_held.anchor, 
                                minx, 
                                miny
                            );
                        }
                        else{                                                       // checking mouse pos for resize boundaries and cursor sprite 
                            state.cursor_pos = csfml_window_scan(
                                window, 
                                event, 
                                clearence,
                                state.topbar_menus_width, 
                                0, 
                                state.topbar_menus_width + state.topbar_title_width, 
                                topbar_height
                            );
                        }
                    }
                break;

                case sfEvtMouseButtonPressed:
                    switch(event.mouseButton.button){                               
                        case sfMouseLeft:
                            state.mouse_button_held.mouse_left = true;
                            state.mouse_button_held.anchor.x = event.mouseButton.x;
                            state.mouse_button_held.anchor.y = event.mouseButton.y;
                        break;
                    }
                break;

                case sfEvtMouseButtonReleased:
                    switch(event.mouseButton.button){
                        case sfMouseLeft:
                            state.mouse_button_held.mouse_left = false;
                        break;
                    }
                break;

                case sfEvtLostFocus:                                                // reset held mouse buttons if they fly off the window
                    state.mouse_button_held.mouse_left = false;
                    state.mouse_button_held.mouse_right = false;
                    state.mouse_button_held.mouse_middle = false;
                    state.mouse_button_held.mouse_xButton1 = false;
                    state.mouse_button_held.mouse_xButton2 = false;
                break;

                case sfEvtKeyPressed:
                    if(event.key.code == state.settings.keys.fullscreen){           // fullscreen key
                        if(state.sfml_window_mode == window_mode_fullscreen){
                            state.sfml_window_mode_flag = window_mode_float;
                        }
                        else{
                            state.sfml_window_mode_flag = window_mode_fullscreen;
                        }
                    }
                break;
            }

            nk_csfml_handle_event(&event);                                          // nuklear events
        }
        nk_input_end(context);

        /* check global state flags */
        if(!state.sfml_running) break;                                       // close program

        switch(state.sfml_window_mode_flag){                                 // change window mode based on flag

            /**
             * make a decision on what whatever mode is flag/requested based on the current state of the window
             */
            case window_mode_float:                                                 // float
                window_size.x = state.settings.parameters.main_window.size.w;
                window_size.y = state.settings.parameters.main_window.size.h;
                window_pos.x  = state.settings.parameters.main_window.size.x;
                window_pos.y  = state.settings.parameters.main_window.size.y;
            
                switch(state.sfml_window_mode){

                    case window_mode_maximized:                                     // float from maximized, just change in size
                        sfRenderWindow_setSize(window, window_size);
                        sfRenderWindow_setPosition(window, window_pos);
                        glViewport(0, 0, window_size.x, window_size.y);
                    break;

                    case window_mode_fullscreen:                                    // float from fullscreen, call to OS api function
                        csfml_window_float(window);
                        sfRenderWindow_setSize(window, window_size);
                        sfRenderWindow_setPosition(window, window_pos);
                    break;
                }

                state.sfml_window_mode = window_mode_float;
                state.sfml_window_mode_flag = 0;
            break;
                                     
            case window_mode_minimized:                                             // minimize
                state.sfml_window_mode = window_mode_minimized;
                state.sfml_window_mode_flag = 0;
                csfml_window_minimize(window);
            break;

            case window_mode_maximized:        
                switch(state.sfml_window_mode){
                    case window_mode_float:
                        window_size = sfRenderWindow_getSize(window);
                        window_pos = sfRenderWindow_getPosition(window);

                        state.settings.parameters.main_window.size.fullscreen = (bool)(state.sfml_window_mode == window_mode_fullscreen);
                        state.settings.parameters.main_window.size.maximized = (bool)(state.sfml_window_mode == window_mode_maximized);
                        state.settings.parameters.main_window.size.x = window_pos.x;
                        state.settings.parameters.main_window.size.y = window_pos.y;
                        state.settings.parameters.main_window.size.w = window_size.x;
                        state.settings.parameters.main_window.size.h = window_size.y;                   // save floating window size/pos before going fullscreen/maximized
                    break;
                }

                state.sfml_window_mode = window_mode_maximized;
                state.sfml_window_mode_flag = 0;
                csfml_window_maximize(window);
            break;

            case window_mode_fullscreen:                                            // fullscreen
                switch(state.sfml_window_mode){
                    case window_mode_float:
                        window_size = sfRenderWindow_getSize(window);
                        window_pos = sfRenderWindow_getPosition(window);

                        state.settings.parameters.main_window.size.fullscreen = (bool)(state.sfml_window_mode == window_mode_fullscreen);
                        state.settings.parameters.main_window.size.maximized = (bool)(state.sfml_window_mode == window_mode_maximized);
                        state.settings.parameters.main_window.size.x = window_pos.x;
                        state.settings.parameters.main_window.size.y = window_pos.y;
                        state.settings.parameters.main_window.size.w = window_size.x;
                        state.settings.parameters.main_window.size.h = window_size.y;                   // save floating window size/pos before going fullscreen/maximized
                    break;
                }

                state.sfml_window_mode = window_mode_fullscreen;
                state.sfml_window_mode_flag = 0;
                csfml_window_fullscreen(window);
            break;

            default:                                                                // no flag / invalid flag 
            break;
        }

        /* GUI */
        mygui(context, window);

        /* Draw nuklear (opengl) */
        sfRenderWindow_setActive(window, sfTrue);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0, 0, 0, 255);

        /** 
         * IMPORTANT: `nk_csfml_render` modifies some global OpenGL state
         * with blending, scissor, face culling and depth test and defaults everything
         * back into a default state. Make sure to either save and restore or
         * reset your own state after drawing rendering the UI. 
         */
        nk_csfml_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
        sfRenderWindow_display(window);
        state.first_render_loop = false;
    }

    /* save data */
    if(state.sfml_window_mode != window_mode_fullscreen && state.sfml_window_mode != window_mode_maximized){
        window_size = sfRenderWindow_getSize(window);
        window_pos  = sfRenderWindow_getPosition(window);
    }

    state.settings.parameters.main_window.size.fullscreen = (bool)(state.sfml_window_mode == window_mode_fullscreen);
    state.settings.parameters.main_window.size.maximized = (bool)(state.sfml_window_mode == window_mode_maximized);
    state.settings.parameters.main_window.size.x = window_pos.x;
    state.settings.parameters.main_window.size.y = window_pos.y;
    state.settings.parameters.main_window.size.w = window_size.x;
    state.settings.parameters.main_window.size.h = window_size.y;                   // save floating window size/pos before going fullscreen/maximized

    global_state_end();

    /* exit */
    free(state.texture);
    nk_csfml_shutdown();
    return 0;
}
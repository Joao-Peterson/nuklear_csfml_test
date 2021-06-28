#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <SFML/Graphics.h>

#include "nuklear_def.h"
#include "nuklear_csfml.h"
#include "mygui.h"
#include "doc.h"
#include "doc_json.h"
#include "settings.h"
#include "csfml_window_util.h"

#define CFG_FILE "cfg.json"

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024


void key_logger(sfEvent event){

    switch(event.key.code){
        case sfKeyUnknown:
            printf("Keyboard press: key Unknown\n");
        break;

        case sfKeyA:
            printf("Keyboard press: key A\n");
        break;            

        case sfKeyB:
            printf("Keyboard press: key B\n");
        break;            

        case sfKeyC:
            printf("Keyboard press: key C\n");
        break;            

        case sfKeyD:
            printf("Keyboard press: key D\n");
        break;            

        case sfKeyE:
            printf("Keyboard press: key E\n");
        break;            

        case sfKeyF:
            printf("Keyboard press: key F\n");
        break;            

        case sfKeyG:
            printf("Keyboard press: key G\n");
        break;            

        case sfKeyH:
            printf("Keyboard press: key H\n");
        break;            

        case sfKeyI:
            printf("Keyboard press: key I\n");
        break;            

        case sfKeyJ:
            printf("Keyboard press: key J\n");
        break;            

        case sfKeyK:
            printf("Keyboard press: key K\n");
        break;            

        case sfKeyL:
            printf("Keyboard press: key L\n");
        break;            

        case sfKeyM:
            printf("Keyboard press: key M\n");
        break;            

        case sfKeyN:
            printf("Keyboard press: key N\n");
        break;            

        case sfKeyO:
            printf("Keyboard press: key O\n");
        break;            

        case sfKeyP:
            printf("Keyboard press: key P\n");
        break;            

        case sfKeyQ:
            printf("Keyboard press: key Q\n");
        break;            

        case sfKeyR:
            printf("Keyboard press: key R\n");
        break;            

        case sfKeyS:
            printf("Keyboard press: key S\n");
        break;            

        case sfKeyT:
            printf("Keyboard press: key T\n");
        break;            

        case sfKeyU:
            printf("Keyboard press: key U\n");
        break;            

        case sfKeyV:
            printf("Keyboard press: key V\n");
        break;            

        case sfKeyW:
            printf("Keyboard press: key W\n");
        break;            

        case sfKeyX:
            printf("Keyboard press: key X\n");
        break;            

        case sfKeyY:
            printf("Keyboard press: key Y\n");
        break;            

        case sfKeyZ:
            printf("Keyboard press: key Z\n");
        break;            

        case sfKeyNum0:
            printf("Keyboard press: key Num0\n");
        break;         

        case sfKeyNum1:
            printf("Keyboard press: key Num1\n");
        break;         

        case sfKeyNum2:
            printf("Keyboard press: key Num2\n");
        break;         

        case sfKeyNum3:
            printf("Keyboard press: key Num3\n");
        break;         

        case sfKeyNum4:
            printf("Keyboard press: key Num4\n");
        break;         

        case sfKeyNum5:
            printf("Keyboard press: key Num5\n");
        break;         

        case sfKeyNum6:
            printf("Keyboard press: key Num6\n");
        break;         

        case sfKeyNum7:
            printf("Keyboard press: key Num7\n");
        break;         

        case sfKeyNum8:
            printf("Keyboard press: key Num8\n");
        break;         

        case sfKeyNum9:
            printf("Keyboard press: key Num9\n");
        break;         

        case sfKeyEscape:
            printf("Keyboard press: key Escape\n");
        break;       

        case sfKeyLControl:
            printf("Keyboard press: key LControl\n");
        break;     

        case sfKeyLShift:
            printf("Keyboard press: key LShift\n");
        break;       

        case sfKeyLAlt:
            printf("Keyboard press: key LAlt\n");
        break;         

        case sfKeyLSystem:
            printf("Keyboard press: key LSystem\n");
        break;      

        case sfKeyRControl:
            printf("Keyboard press: key RControl\n");
        break;     

        case sfKeyRShift:
            printf("Keyboard press: key RShift\n");
        break;       

        case sfKeyRAlt:
            printf("Keyboard press: key RAlt\n");
        break;         

        case sfKeyRSystem:
            printf("Keyboard press: key RSystem\n");
        break;      

        case sfKeyMenu:
            printf("Keyboard press: key Menu\n");
        break;         

        case sfKeyLBracket:
            printf("Keyboard press: key LBracket\n");
        break;     

        case sfKeyRBracket:
            printf("Keyboard press: key RBracket\n");
        break;     

        case sfKeySemicolon:
            printf("Keyboard press: key Semicolon\n");
        break;    

        case sfKeyComma:
            printf("Keyboard press: key Comma\n");
        break;        

        case sfKeyPeriod:
            printf("Keyboard press: key Period\n");
        break;       

        case sfKeyQuote:
            printf("Keyboard press: key Quote\n");
        break;        

        case sfKeySlash:
            printf("Keyboard press: key Slash\n");
        break;        

        case sfKeyBackslash:
            printf("Keyboard press: key Backslash\n");
        break;    

        case sfKeyTilde:
            printf("Keyboard press: key Tilde\n");
        break;        

        case sfKeyEqual:
            printf("Keyboard press: key Equal\n");
        break;        

        case sfKeyHyphen:
            printf("Keyboard press: key Hyphen\n");
        break;       

        case sfKeySpace:
            printf("Keyboard press: key Space\n");
        break;        

        case sfKeyEnter:
            printf("Keyboard press: key Enter\n");
        break;        

        case sfKeyBackspace:
            printf("Keyboard press: key Backspace\n");
        break;    

        case sfKeyTab:
            printf("Keyboard press: key Tab\n");
        break;          

        case sfKeyPageUp:
            printf("Keyboard press: key PageUp\n");
        break;       

        case sfKeyPageDown:
            printf("Keyboard press: key PageDown\n");
        break;     

        case sfKeyEnd:
            printf("Keyboard press: key End\n");
        break;          

        case sfKeyHome:
            printf("Keyboard press: key Home\n");
        break;         

        case sfKeyInsert:
            printf("Keyboard press: key Insert\n");
        break;       

        case sfKeyDelete:
            printf("Keyboard press: key Delete\n");
        break;       

        case sfKeyAdd:
            printf("Keyboard press: key Add\n");
        break;          

        case sfKeySubtract:
            printf("Keyboard press: key Subtract\n");
        break;     

        case sfKeyMultiply:
            printf("Keyboard press: key Multiply\n");
        break;     

        case sfKeyDivide:
            printf("Keyboard press: key Divide\n");
        break;       

        case sfKeyLeft:
            printf("Keyboard press: key Left\n");
        break;         

        case sfKeyRight:
            printf("Keyboard press: key Right\n");
        break;        

        case sfKeyUp:
            printf("Keyboard press: key Up\n");
        break;           

        case sfKeyDown:
            printf("Keyboard press: key Down\n");
        break;         

        case sfKeyNumpad0:
            printf("Keyboard press: key Numpad0\n");
        break;      

        case sfKeyNumpad1:
            printf("Keyboard press: key Numpad1\n");
        break;      

        case sfKeyNumpad2:
            printf("Keyboard press: key Numpad2\n");
        break;      

        case sfKeyNumpad3:
            printf("Keyboard press: key Numpad3\n");
        break;      

        case sfKeyNumpad4:
            printf("Keyboard press: key Numpad4\n");
        break;      

        case sfKeyNumpad5:
            printf("Keyboard press: key Numpad5\n");
        break;      

        case sfKeyNumpad6:
            printf("Keyboard press: key Numpad6\n");
        break;      

        case sfKeyNumpad7:
            printf("Keyboard press: key Numpad7\n");
        break;      

        case sfKeyNumpad8:
            printf("Keyboard press: key Numpad8\n");
        break;      

        case sfKeyNumpad9:
            printf("Keyboard press: key Numpad9\n");
        break;      

        case sfKeyF1:
            printf("Keyboard press: key F1\n");
        break;           

        case sfKeyF2:
            printf("Keyboard press: key F2\n");
        break;           

        case sfKeyF3:
            printf("Keyboard press: key F3\n");
        break;           

        case sfKeyF4:
            printf("Keyboard press: key F4\n");
        break;           

        case sfKeyF5:
            printf("Keyboard press: key F5\n");
        break;           

        case sfKeyF6:
            printf("Keyboard press: key F6\n");
        break;           

        case sfKeyF7:
            printf("Keyboard press: key F7\n");
        break;           

        case sfKeyF8:
            printf("Keyboard press: key F8\n");
        break;           

        case sfKeyF9:
            printf("Keyboard press: key F9\n");
        break;           

        case sfKeyF10:
            printf("Keyboard press: key F10\n");
        break;          

        case sfKeyF11:
            printf("Keyboard press: key F11\n");
        break;          

        case sfKeyF12:
            printf("Keyboard press: key F12\n");
        break;          

        case sfKeyF13:
            printf("Keyboard press: key F13\n");
        break;          

        case sfKeyF14:
            printf("Keyboard press: key F14\n");
        break;          

        case sfKeyF15:
            printf("Keyboard press: key F15\n");
        break;          

        case sfKeyPause:
            printf("Keyboard press: key Pause\n");
        break;        

    }
}


int main(int argc, char **argv){

    /* settings */
    settings_t *settings = settings_init(CFG_FILE);
    if(settings == NULL){
        printf("Failed to load settings file!\n");
        return 0;
    }


    /* csfml */
    sfVideoMode mode;
    mode.width  = settings->parameters.main_window.size.w;
    mode.height = settings->parameters.main_window.size.h;
    mode.bitsPerPixel = 24;
    sfRenderWindow *window = sfRenderWindow_create(
        mode,
        settings->parameters.main_window.title,
        0,
        NULL
    );
    sfRenderWindow_setVerticalSyncEnabled(window, sfTrue);
    sfRenderWindow_setActive(window, sfTrue);
    mouse_info_t mouse = {.cursor_pos = 0, .mouse_button_held = {}};


    /* opengl viewport */
    if(!gladLoadGL()) {                                                             // load opengl extensions
        printf("Failed to load OpenGL extensions!\n");
        return -1;
    }


    /* mygui init */
    mygui_interface_t *interface = mygui_init(settings);
    if(interface == NULL){
        printf("Failed to initialize mygui interface!\n");
        return 0;
    }


    /* nuklear */
    struct nk_context *context;                                                     // nuklear context
    context = nk_csfml_init(window, mygui_get_active_font(interface));
    if(context == NULL){
        printf("Failed to initialize nuklear context!\n");
        return 0;
    }
    
    /* video settings */
    mygui_styles(context, interface);                                               // textures and styles
    sfVector2u window_size;
    sfVector2i window_pos;
    window_size.x = settings->parameters.main_window.size.w;
    window_size.y = settings->parameters.main_window.size.h;
    window_pos.x  = settings->parameters.main_window.size.x;
    window_pos.y  = settings->parameters.main_window.size.y;
    sfRenderWindow_setSize(window, window_size);
    sfRenderWindow_setPosition(window, window_pos);


    /* render loop */
    int clearence       = settings->parameters.main_window.resize.clearence;
    int minx            = settings->parameters.main_window.resize.min.x;
    int miny            = settings->parameters.main_window.resize.min.y;
    int topbar_height   = settings->parameters.main_window.topbar.height;
    interface->running_flag = true;
    while(interface->running_flag){ 

        sfEvent event;
        nk_input_begin(context);

        /* handle events loop */
        while(sfRenderWindow_pollEvent(window, &event)){
            switch(event.type){                                                     // sfml events

                case sfEvtClosed:                                                   // ON CLOSE
                    interface->running_flag = false;
                    sfRenderWindow_close(window);
                break;


                case sfEvtResized:                                                  // ON WINDOW RESIZE
                    glViewport(0, 0, event.size.width, event.size.height);
                break;


                case sfEvtMouseMoved:                                               // ON MOUSE MOVE

                    // only apply window transform logic when not in fulscreen/maximized mode
                    if(interface->window_mode != window_mode_fullscreen && interface->window_mode != window_mode_maximized){
                        if(mouse.mouse_button_held.mouse_left){                     // moving while helding the mouse left down
                            csfml_window_transform(
                                window, 
                                event, 
                                mouse.cursor_pos, 
                                &mouse.mouse_button_held.anchor, 
                                minx, 
                                miny
                            );
                        }
                        else{                                                       // checking mouse pos for resize boundaries and cursor sprite 
                            mouse.cursor_pos = csfml_window_scan(
                                window, 
                                event, 
                                clearence,
                                interface->topbar_menus_width, 
                                0, 
                                interface->topbar_menus_width + interface->topbar_title_width, 
                                topbar_height
                            );
                        }
                    }
                break;


                case sfEvtMouseButtonPressed:                                       // ON MOUSE BUTTON PRESS
                    switch(event.mouseButton.button){                               
                        case sfMouseLeft:
                            mouse.mouse_button_held.mouse_left = true;
                            mouse.mouse_button_held.anchor.x = event.mouseButton.x;
                            mouse.mouse_button_held.anchor.y = event.mouseButton.y;
                        break;
                    }
                break;


                case sfEvtMouseButtonReleased:                                      // ON MOUSE BUTTON RELEASE
                
                    switch(event.mouseButton.button){
                        case sfMouseLeft:
                            mouse.mouse_button_held.mouse_left = false;
                        break;
                    }
                break;


                // case sfEvtMouseLeft:                                                // ON MOUSE LEFT THE WINDOW
                // case sfEvtLostFocus:                                                // ON WINDOW LOST FOCUS                                                
                //     // reset held mouse buttons if they fly off the window
                //     mouse.mouse_button_held.mouse_left = false;
                //     mouse.mouse_button_held.mouse_right = false;
                //     mouse.mouse_button_held.mouse_middle = false;
                //     mouse.mouse_button_held.mouse_xButton1 = false;
                //     mouse.mouse_button_held.mouse_xButton2 = false;
                // break;


                case sfEvtKeyPressed:                                               // ON KEY PRESS
                    // TODO REMOVE
                    // key_logger(event);

                    if(check_key_combo(event.key, settings->keys.fullscreen)){ // fullscreen key
                        if(interface->window_mode == window_mode_fullscreen){
                            interface->window_mode_flag = window_mode_float;
                        }
                        else{
                            interface->window_mode_flag = window_mode_fullscreen;
                        }
                    }
                    
                                                                                    // scale up mygui 
                    if(check_key_combo(event.key, settings->keys.up_scale_plus) || check_key_combo(event.key, settings->keys.up_scale_equal)){                                                              
                        interface->scale_up_flag = true;      
                    }

                                                                                    // scale down mygui 
                    if(check_key_combo(event.key, settings->keys.down_scale_minus) || check_key_combo(event.key, settings->keys.down_scale_hyphen)){                                                              
                        interface->scale_down_flag = true;      
                    }
                    
                break;

            }

            nk_csfml_handle_event(&event);                                          // nuklear events
        }
        nk_input_end(context);

        /* check global state flags */
        if(!interface->running_flag) break;                                         // close program
        switch(interface->window_mode_flag){                                        // change window mode based on flag

            /**
             * make a decision on what whatever mode is flag/requested based on the current state of the window
             */
            case window_mode_float:                                                 // float
                window_size.x = settings->parameters.main_window.size.w;
                window_size.y = settings->parameters.main_window.size.h;
                window_pos.x  = settings->parameters.main_window.size.x;
                window_pos.y  = settings->parameters.main_window.size.y;
            
                switch(interface->window_mode){

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

                interface->window_mode = window_mode_float;
                interface->window_mode_flag = 0;
            break;
                                     
            case window_mode_minimized:                                             // minimize
                interface->window_mode = window_mode_minimized;
                interface->window_mode_flag = 0;
                csfml_window_minimize(window);
            break;

            case window_mode_maximized:                                             // maximize
                switch(interface->window_mode){
                    case window_mode_float:
                        window_size = sfRenderWindow_getSize(window);
                        window_pos = sfRenderWindow_getPosition(window);

                        settings->parameters.main_window.size.fullscreen = (bool)(interface->window_mode == window_mode_fullscreen);
                        settings->parameters.main_window.size.maximized = (bool)(interface->window_mode == window_mode_maximized);
                        settings->parameters.main_window.size.x = window_pos.x;
                        settings->parameters.main_window.size.y = window_pos.y;
                        settings->parameters.main_window.size.w = window_size.x;
                        settings->parameters.main_window.size.h = window_size.y;    // save floating window size/pos before going fullscreen/maximized
                    break;
                }

                interface->window_mode = window_mode_maximized;
                interface->window_mode_flag = 0;
                csfml_window_maximize(window);
            break;

            case window_mode_fullscreen:                                            // fullscreen
                switch(interface->window_mode){
                    case window_mode_float:
                        window_size = sfRenderWindow_getSize(window);
                        window_pos = sfRenderWindow_getPosition(window);

                        settings->parameters.main_window.size.fullscreen = (bool)(interface->window_mode == window_mode_fullscreen);
                        settings->parameters.main_window.size.maximized = (bool)(interface->window_mode == window_mode_maximized);
                        settings->parameters.main_window.size.x = window_pos.x;
                        settings->parameters.main_window.size.y = window_pos.y;
                        settings->parameters.main_window.size.w = window_size.x;
                        settings->parameters.main_window.size.h = window_size.y;    // save floating window size/pos before going fullscreen/maximized
                    break;
                }

                interface->window_mode = window_mode_fullscreen;
                interface->window_mode_flag = 0;
                csfml_window_fullscreen(window);
            break;

            default:                                                                // no flag / invalid flag 
            break;
        }

        /* GUI */
        sfVector2u real_window_size = sfRenderWindow_getSize(window);
        interface->window_w = real_window_size.x;
        interface->window_h = real_window_size.y;
        mygui(context, interface);

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
    }

    /* save data */
    if(interface->window_mode != window_mode_fullscreen && interface->window_mode != window_mode_maximized){
        window_size = sfRenderWindow_getSize(window);
        window_pos  = sfRenderWindow_getPosition(window);
    }
    settings->parameters.main_window.size.fullscreen = (bool)(interface->window_mode == window_mode_fullscreen);
    settings->parameters.main_window.size.maximized = (bool)(interface->window_mode == window_mode_maximized);
    settings->parameters.main_window.size.x = window_pos.x;
    settings->parameters.main_window.size.y = window_pos.y;
    settings->parameters.main_window.size.w = window_size.x;
    settings->parameters.main_window.size.h = window_size.y;                   // save floating window size/pos before going fullscreen/maximized
    settings_end(settings);
    mygui_end(interface);

    /* exit */
    nk_csfml_shutdown();
    return 0;
}
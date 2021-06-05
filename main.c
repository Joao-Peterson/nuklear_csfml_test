#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <SFML/Graphics.h>

#define NK_IMPLEMENTATION
#define NK_PRIVATE
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include <nuklear.h>

#define NK_CSFML_IMPLEMENTATION
#include "inc/nuklear_csfml.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

int main(int argc, char **argv){
    
    /* csfml */
    sfVideoMode mode = {.height = WINDOW_HEIGHT, .width = WINDOW_WIDTH, .bitsPerPixel = 24};
    sfRenderWindow *window = sfRenderWindow_create(
        mode,
        "Nuklear and CSFML!",
        sfDefaultStyle,
        NULL
    );

    sfRenderWindow_setVerticalSyncEnabled(window, sfTrue);
    sfRenderWindow_setActive(window, sfTrue);

    /* opengl viewport */
    if(!gladLoadGL()) {                                                             // load opengl extensions
        printf("Failed to load OpenGL extensions!\n");
        return -1;
    }

    sfVector2u window_size = sfRenderWindow_getSize(window);
    // glViewport(0, 0, window_size.x, window_size.y);

    /* nuklear */
    struct nk_font_atlas *atlas;
    nk_csfml_font_stash_begin(&atlas);
    struct nk_font *roboto = nk_font_atlas_add_default(atlas, 14, NULL);
    nk_csfml_font_stash_end();

    struct nk_context *context;
    context = nk_csfml_init(window, &roboto->handle);

    struct nk_colorf bg;
    bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;

    /* render loop */
    while(sfRenderWindow_isOpen(window)){ 
        sfEvent event;

        nk_input_begin(context);
        /* handle events loop */
        while(sfRenderWindow_pollEvent(window, &event)){
            switch(event.type){                                                     // sfml events
                case sfEvtClosed:
                    sfRenderWindow_close(window);
                break;

                case sfEvtResized:
                    glViewport(0, 0, event.size.width, event.size.height);
                break;
            }

            nk_csfml_handle_event(&event);                                          // nuklear events
            // sfRenderWindow_clear(window, sfBlack);
            // sfRenderWindow_display(window);
        }
        nk_input_end(context);

        /* GUI */
        window_size = sfRenderWindow_getSize(window);
        if (nk_begin(context, "Demo", nk_rect(0, 0, window_size.x, window_size.y),
            NK_WINDOW_TITLE))
        {
            static int property = 20;

            nk_layout_row_static(context, 30, 80, 1);
            if (nk_button_label(context, "button"))
                fprintf(stdout, "button pressed\n");

            struct nk_command_buffer *canvas = nk_window_get_canvas(context);
            struct nk_rect rect;
            nk_widget(&rect, context);
            rect.w = 250;
            rect.h = 250;
            nk_fill_rect(canvas, rect, 2, nk_rgb(0, 127, 127));

            nk_layout_row_dynamic(context, 25, 1);
            nk_property_int(context, "Compression:", 0, &property, 100, 10, 1);
        }
        nk_end(context);

        /* Draw nuklear (opengl) */
        sfRenderWindow_setActive(window, sfTrue);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(bg.r, bg.g, bg.b, bg.a);
        /* IMPORTANT: `nk_sfml_render` modifies some global OpenGL state
        * with blending, scissor, face culling and depth test and defaults everything
        * back into a default state. Make sure to either save and restore or
        * reset your own state after drawing rendering the UI. */
        nk_csfml_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
        nk_clear(context);
        // sfRenderWindow_display(window);

        /* sfml draw (graphics module) */
        // sfRenderWindow_pushGLStates(window);
        
        // struct sfCircleShape *circle = sfCircleShape_create();
        // sfCircleShape_setRadius(circle, 300);
        // sfCircleShape_setFillColor(circle, sfGreen);

        // sfRenderWindow_drawCircleShape(window, circle, NULL);
        // sfCircleShape_destroy(circle);

        // sfRenderWindow_popGLStates(window);

        sfRenderWindow_display(window);
    }

    nk_csfml_shutdown();
    return 0;
}
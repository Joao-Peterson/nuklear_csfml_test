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

    if(!gladLoadGL()) {                                                             // load opengl extensions
        printf("Failed to load OpenGL extensions!\n");
        return -1;
    }
    sfVector2u window_size = sfRenderWindow_getSize(window);
    glViewport(0, 0, window_size.x, window_size.y);

    /* nuklear */
    struct nk_font_atlas *atlas;
    nk_csfml_font_stash_begin(&atlas);
    struct nk_font *roboto = nk_font_atlas_add_default(atlas, 14, NULL);

    struct nk_context *ctx;
    ctx = nk_csfml_init(window, &roboto->handle);

    struct nk_colorf bg;
    bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;

    while(sfRenderWindow_isOpen(window)){ 
        sfEvent event;

        nk_input_begin(ctx);
        while(sfRenderWindow_pollEvent(window, &event)){
            switch(event.type){
                case sfEvtClosed:
                    sfRenderWindow_close(window);
                break;

                case sfEvtResized:
                    glViewport(0, 0, event.size.width, event.size.height);
                break;
            }

            nk_csfml_handle_event(&event);
            // sfRenderWindow_clear(window, sfBlack);
            // sfRenderWindow_display(window);
        }
        nk_input_end(ctx);

         /* GUI */
        if (nk_begin(ctx, "Demo", nk_rect(50, 50, 230, 250),
            NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
            NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
        {
            enum {EASY, HARD};
            static int op = EASY;
            static int property = 20;
            nk_layout_row_static(ctx, 30, 80, 1);
            if (nk_button_label(ctx, "button"))
                fprintf(stdout, "button pressed\n");

            nk_layout_row_dynamic(ctx, 30, 2);
            if (nk_option_label(ctx, "easy", op == EASY)) op = EASY;
            if (nk_option_label(ctx, "hard", op == HARD)) op = HARD;

            nk_layout_row_dynamic(ctx, 25, 1);
            nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);

            nk_layout_row_dynamic(ctx, 20, 1);
            nk_label(ctx, "background:", NK_TEXT_LEFT);
            nk_layout_row_dynamic(ctx, 25, 1);
            if (nk_combo_begin_color(ctx, nk_rgb_cf(bg), nk_vec2(nk_widget_width(ctx),400))) {
                nk_layout_row_dynamic(ctx, 120, 1);
                bg = nk_color_picker(ctx, bg, NK_RGBA);
                nk_layout_row_dynamic(ctx, 25, 1);
                bg.r = nk_propertyf(ctx, "#R:", 0, bg.r, 1.0f, 0.01f,0.005f);
                bg.g = nk_propertyf(ctx, "#G:", 0, bg.g, 1.0f, 0.01f,0.005f);
                bg.b = nk_propertyf(ctx, "#B:", 0, bg.b, 1.0f, 0.01f,0.005f);
                bg.a = nk_propertyf(ctx, "#A:", 0, bg.a, 1.0f, 0.01f,0.005f);
                nk_combo_end(ctx);
            }
        }
        nk_end(ctx);

        /* Draw */
        sfRenderWindow_setActive(window, sfTrue);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(bg.r, bg.g, bg.b, bg.a);
        /* IMPORTANT: `nk_sfml_render` modifies some global OpenGL state
        * with blending, scissor, face culling and depth test and defaults everything
        * back into a default state. Make sure to either save and restore or
        * reset your own state after drawing rendering the UI. */
        nk_csfml_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
        sfRenderWindow_display(window);
    }

    nk_csfml_shutdown();
    return 0;
}
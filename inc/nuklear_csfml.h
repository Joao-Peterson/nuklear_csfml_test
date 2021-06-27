#ifndef NK_CSFML_H_
#define NK_CSFML_H_

#include "glad/glad.h"  
#include <assert.h>
#include <SFML/Graphics.h>

#include "nuklear_def.h"

NK_API struct nk_context   *nk_csfml_init(sfRenderWindow *window, const struct nk_user_font *font);
NK_API void                 nk_csfml_font_stash_begin(struct nk_font_atlas **atlas);
NK_API void                 nk_csfml_font_stash_end(void);
NK_API int                  nk_csfml_handle_event(sfEvent *event);
NK_API void                 nk_csfml_render(enum nk_anti_aliasing, int max_vertex_buffer, int max_element_buffer);
NK_API void                 nk_csfml_shutdown(void);
NK_API void                 nk_csfml_device_create(void);
NK_API void                 nk_csfml_device_destroy(void);

#endif

#ifdef NK_CSFML_IMPLEMENTATION

#include <string.h>

struct nk_csfml_device {
    struct nk_buffer cmds;
    struct nk_draw_null_texture null;
    GLuint vbo, vao, ebo;
    GLuint prog;
    GLuint vert_shdr;
    GLuint frag_shdr;
    GLint attrib_pos;
    GLint attrib_uv;
    GLint attrib_col;
    GLint uniform_tex;
    GLint uniform_proj;
    GLuint font_tex;
};

struct nk_csfml_vertex {
    float position[2];
    float uv[2];
    nk_byte col[4];
};

struct nk_csfml{
    sfRenderWindow *window;
    struct nk_csfml_device ogl;
    struct nk_context ctx;
    struct nk_font_atlas atlas;
}csfml;

#ifdef __APPLE__
  #define NK_SHADER_VERSION "#version 150\n"
#else
  #define NK_SHADER_VERSION "#version 300 es\n"
#endif

NK_API void nk_csfml_device_create(void){
    GLint status;
    static const GLchar* vertex_shader =
        NK_SHADER_VERSION
        "uniform mat4 ProjMtx;\n"
        "in vec2 Position;\n"
        "in vec2 TexCoord;\n"
        "in vec4 Color;\n"
        "out vec2 Frag_UV;\n"
        "out vec4 Frag_Color;\n"
        "void main() {\n"
        "   Frag_UV = TexCoord;\n"
        "   Frag_Color = Color;\n"
        "   gl_Position = ProjMtx * vec4(Position.xy, 0, 1);\n"
        "}\n";
    static const GLchar *fragment_shader =
        NK_SHADER_VERSION
        "precision mediump float;\n"
        "uniform sampler2D Texture;\n"
        "in vec2 Frag_UV;\n"
        "in vec4 Frag_Color;\n"
        "out vec4 Out_Color;\n"
        "void main(){\n"
        "   Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
        "}\n";

    struct nk_csfml_device* dev = &csfml.ogl;
    nk_buffer_init_default(&dev->cmds);

    dev->prog = glCreateProgram();
    dev->vert_shdr = glCreateShader(GL_VERTEX_SHADER);
    dev->frag_shdr = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(dev->vert_shdr, 1, &vertex_shader, 0);
    glShaderSource(dev->frag_shdr, 1, &fragment_shader, 0);
    glCompileShader(dev->vert_shdr);
    glCompileShader(dev->frag_shdr);

    glGetShaderiv(dev->vert_shdr, GL_COMPILE_STATUS, &status);
    assert(status == GL_TRUE);

    glGetShaderiv(dev->frag_shdr, GL_COMPILE_STATUS, &status);
    assert(status == GL_TRUE);

    glAttachShader(dev->prog, dev->vert_shdr);
    glAttachShader(dev->prog, dev->frag_shdr);
    glLinkProgram(dev->prog);

    glGetProgramiv(dev->prog, GL_LINK_STATUS, &status);
    assert(status == GL_TRUE);

    dev->uniform_tex = glGetUniformLocation(dev->prog, "Texture");
    dev->uniform_proj = glGetUniformLocation(dev->prog, "ProjMtx");
    dev->attrib_pos = glGetAttribLocation(dev->prog, "Position");
    dev->attrib_uv = glGetAttribLocation(dev->prog, "TexCoord");
    dev->attrib_col = glGetAttribLocation(dev->prog, "Color");
    {
        /* buffer setup */
        GLsizei vs = sizeof(struct nk_csfml_vertex);
        size_t vp = NK_OFFSETOF(struct nk_csfml_vertex, position);
        size_t vt = NK_OFFSETOF(struct nk_csfml_vertex, uv);
        size_t vc = NK_OFFSETOF(struct nk_csfml_vertex, col);

        glGenBuffers(1, &dev->vbo);
        glGenBuffers(1, &dev->ebo);
        glGenVertexArrays(1, &dev->vao);

        glBindVertexArray(dev->vao);
        glBindBuffer(GL_ARRAY_BUFFER, dev->vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dev->ebo);

        glEnableVertexAttribArray((GLuint)dev->attrib_pos);
        glEnableVertexAttribArray((GLuint)dev->attrib_uv);
        glEnableVertexAttribArray((GLuint)dev->attrib_col);

        glVertexAttribPointer((GLuint)dev->attrib_pos, 2, GL_FLOAT, GL_FALSE, vs, (void*)vp);
        glVertexAttribPointer((GLuint)dev->attrib_uv, 2, GL_FLOAT, GL_FALSE, vs, (void*)vt);
        glVertexAttribPointer((GLuint)dev->attrib_col, 4, GL_UNSIGNED_BYTE, GL_TRUE, vs, (void*)vc);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

NK_API void nk_csfml_device_destroy(void){
    struct nk_csfml_device* dev = &csfml.ogl;

    glDetachShader(dev->prog, dev->vert_shdr);
    glDetachShader(dev->prog, dev->frag_shdr);
    glDeleteShader(dev->vert_shdr);
    glDeleteShader(dev->vert_shdr);
    glDeleteProgram(dev->prog);
    glDeleteTextures(1, &dev->font_tex);
    glDeleteBuffers(1, &dev->vbo);
    glDeleteBuffers(1, &dev->ebo);
    nk_buffer_free(&dev->cmds);
}

NK_INTERN void nk_csfml_device_upload_atlas(const void* image, int width, int height){
    struct nk_csfml_device* dev = &csfml.ogl;
    glGenTextures(1, &dev->font_tex);
    glBindTexture(GL_TEXTURE_2D, dev->font_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)width, (GLsizei)height, 0,
                GL_RGBA, GL_UNSIGNED_BYTE, image);
}

NK_API void nk_csfml_render(enum nk_anti_aliasing AA, int max_vertex_buffer, int max_element_buffer){
    /* setup global state */
    struct nk_csfml_device* dev = &csfml.ogl;
    sfVector2u window_size = sfRenderWindow_getSize(csfml.window);
    GLfloat ortho[4][4] = {
        {2.0f, 0.0f, 0.0f, 0.0f},
        {0.0f,-2.0f, 0.0f, 0.0f},
        {0.0f, 0.0f,-1.0f, 0.0f},
        {-1.0f,1.0f, 0.0f, 1.0f},
    };
    ortho[0][0] /= (GLfloat)window_size.x;
    ortho[1][1] /= (GLfloat)window_size.y;

    glViewport(0, 0, window_size.x, window_size.y);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
    glActiveTexture(GL_TEXTURE0);

    /* setup program */
    glUseProgram(dev->prog);
    glUniform1i(dev->uniform_tex, 0);
    glUniformMatrix4fv(dev->uniform_proj, 1, GL_FALSE, &ortho[0][0]);
    {
        /* convert from command queue into draw list and draw to screen */
        const struct nk_draw_command *cmd;
        void *vertices, *elements;
        const nk_draw_index *offset = NULL;

        /* allocate vertex and element buffer */
        glBindVertexArray(dev->vao);
        glBindBuffer(GL_ARRAY_BUFFER, dev->vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dev->ebo);

        glBufferData(GL_ARRAY_BUFFER, max_vertex_buffer, NULL, GL_STREAM_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, max_element_buffer, NULL, GL_STREAM_DRAW);

        /* load vertices/elements directly into vertex/element buffer */
        vertices = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        elements = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
        {
            /* fill convert configuration */
            struct nk_convert_config config;
            static const struct nk_draw_vertex_layout_element vertex_layout[] =  {
                {NK_VERTEX_POSITION, NK_FORMAT_FLOAT, NK_OFFSETOF(struct nk_csfml_vertex, position)},
                {NK_VERTEX_TEXCOORD, NK_FORMAT_FLOAT, NK_OFFSETOF(struct nk_csfml_vertex, uv)},
                {NK_VERTEX_COLOR, NK_FORMAT_R8G8B8A8, NK_OFFSETOF(struct nk_csfml_vertex, col)},
                {NK_VERTEX_LAYOUT_END}
            };

            NK_MEMSET(&config, 0, sizeof(config));
            config.vertex_layout = vertex_layout;
            config.vertex_size = sizeof(struct nk_csfml_vertex);
            config.vertex_alignment = NK_ALIGNOF(struct nk_csfml_vertex);
            config.null = dev->null;
            config.circle_segment_count = 22;
            config.curve_segment_count = 22;
            config.arc_segment_count = 22;
            config.global_alpha = 1.0f;
            config.shape_AA = AA;
            config.line_AA = AA;

            /* setup buffers to load vertices and elements */
            struct nk_buffer vbuf, ebuf;
            nk_buffer_init_fixed(&vbuf, vertices, (nk_size)max_vertex_buffer);
            nk_buffer_init_fixed(&ebuf, elements, (nk_size)max_element_buffer);
            nk_convert(&csfml.ctx, &dev->cmds, &vbuf, &ebuf, &config);
        }
        glUnmapBuffer(GL_ARRAY_BUFFER);
        glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

        /* iterate over and execute each draw command */
        nk_draw_foreach(cmd, &(csfml.ctx), &dev->cmds)
        {
            if (!cmd->elem_count) continue;
            glBindTexture(GL_TEXTURE_2D, (GLuint)cmd->texture.id);
            glScissor(
                (GLint)(cmd->clip_rect.x),
                (GLint)((window_size.y - (GLint)(cmd->clip_rect.y + cmd->clip_rect.h))),
                (GLint)(cmd->clip_rect.w),
                (GLint)(cmd->clip_rect.h));
            glDrawElements(GL_TRIANGLES, (GLsizei)cmd->elem_count, GL_UNSIGNED_SHORT, offset);
            offset += cmd->elem_count;
        }
        nk_clear(&csfml.ctx);
        nk_buffer_clear(&dev->cmds);
    }
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDisable(GL_BLEND);
    glDisable(GL_SCISSOR_TEST);
}

static void nk_csfml_clipboard_paste(nk_handle usr, struct nk_text_edit* edit){
#if 0
    /* Not Implemented in SFML */
    (void)usr;
    sf::Clipboard clipboard(sfml.window);
    const char* text = clipboard.getText();
    if(text) nk_textedit_paste(edit, text, nk_strlen(text));
#endif
}

static void nk_csfml_clipboard_copy(nk_handle usr, const char* text, int len){
#if 0
    char* str = 0;
    (void)usr;
    if(!len) return;
    str = (char*)malloc((size_t)len+1);
    if(!str) return;
    memcpy(str, text, (size_t)len);
    str[len] = '\0';

    /* Not Implemented in SFML */
    sf::Clipboard clipboard(sfml.window);
    clipboard.setText(str);
    free(str);
#endif
}

NK_API struct nk_context *nk_csfml_init(sfRenderWindow *window, const struct nk_user_font *font){
    csfml.window = window;
    nk_init_default(&csfml.ctx, font);
    csfml.ctx.clip.copy = nk_csfml_clipboard_copy;
    csfml.ctx.clip.paste = nk_csfml_clipboard_paste;
    csfml.ctx.clip.userdata = nk_handle_ptr(0);
    nk_csfml_device_create();
    return &csfml.ctx;
}

NK_API void nk_csfml_font_stash_begin(struct nk_font_atlas** atlas){
    nk_font_atlas_init_default(&csfml.atlas);
    nk_font_atlas_begin(&csfml.atlas);
    *atlas = &csfml.atlas;
}

NK_API void nk_csfml_font_stash_end(){
    const void* image;
    int w, h;
    image = nk_font_atlas_bake(&csfml.atlas, &w, &h, NK_FONT_ATLAS_RGBA32);
    nk_csfml_device_upload_atlas(image, w, h);
    nk_font_atlas_end(&csfml.atlas, nk_handle_id((int)csfml.ogl.font_tex), &csfml.ogl.null);
    if(csfml.atlas.default_font)
        nk_style_set_font(&csfml.ctx, &csfml.atlas.default_font->handle);
}

NK_API int nk_csfml_handle_event(sfEvent *evt){
    struct nk_context* ctx = &csfml.ctx;
    /* optional grabbing behavior */
    if(ctx->input.mouse.grab)
        ctx->input.mouse.grab = 0;
    else if(ctx->input.mouse.ungrab) {
        sfVector2i mpos;
        mpos.x = (int)ctx->input.mouse.prev.x;
        mpos.y = (int)ctx->input.mouse.prev.y;
        sfMouse_setPositionRenderWindow(mpos, csfml.window);
        ctx->input.mouse.ungrab = 0;
    }
    if(evt->type == sfEvtKeyReleased || evt->type == sfEvtKeyPressed)
    {
        int down = evt->type == sfEvtKeyPressed;
        sfKeyCode key = evt->key.code;
        if(key == sfKeyRShift || key == sfKeyLShift)
            nk_input_key(ctx, NK_KEY_SHIFT, down);
        else if(key == sfKeyDelete)
            nk_input_key(ctx, NK_KEY_DEL, down);
        else if(key == sfKeyReturn)
            nk_input_key(ctx, NK_KEY_ENTER, down);
        else if(key == sfKeyTab)
            nk_input_key(ctx, NK_KEY_TAB, down);
        else if(key == sfKeyBackspace)
            nk_input_key(ctx, NK_KEY_BACKSPACE, down);
        else if(key == sfKeyHome) {
            nk_input_key(ctx, NK_KEY_TEXT_START, down);
            nk_input_key(ctx, NK_KEY_SCROLL_START, down);
        } else if(key == sfKeyEnd) {
            nk_input_key(ctx, NK_KEY_TEXT_END, down);
            nk_input_key(ctx, NK_KEY_SCROLL_END, down);
        } else if(key == sfKeyPageDown)
            nk_input_key(ctx, NK_KEY_SCROLL_DOWN, down);
        else if(key == sfKeyPageUp)
            nk_input_key(ctx, NK_KEY_SCROLL_DOWN, down);
        else if(key == sfKeyZ)
            nk_input_key(ctx, NK_KEY_TEXT_UNDO, down && sfKeyboard_isKeyPressed(sfKeyLControl));
        else if(key == sfKeyR)
            nk_input_key(ctx, NK_KEY_TEXT_REDO, down && sfKeyboard_isKeyPressed(sfKeyLControl));
        else if(key == sfKeyC)
            nk_input_key(ctx, NK_KEY_COPY, down && sfKeyboard_isKeyPressed(sfKeyLControl));
        else if(key == sfKeyV)
            nk_input_key(ctx, NK_KEY_PASTE, down && sfKeyboard_isKeyPressed(sfKeyLControl));
        else if(key == sfKeyX)
            nk_input_key(ctx, NK_KEY_CUT, down && sfKeyboard_isKeyPressed(sfKeyLControl));
        else if(key == sfKeyB)
            nk_input_key(ctx, NK_KEY_TEXT_LINE_START, down && sfKeyboard_isKeyPressed(sfKeyLControl));
        else if(key == sfKeyE)
            nk_input_key(ctx, NK_KEY_TEXT_LINE_END, down && sfKeyboard_isKeyPressed(sfKeyLControl));
        else if(key == sfKeyUp)
            nk_input_key(ctx, NK_KEY_UP, down);
        else if(key == sfKeyDown)
            nk_input_key(ctx, NK_KEY_DOWN, down);
        else if(key == sfKeyLeft) {
            if(sfKeyboard_isKeyPressed(sfKeyLControl))
                nk_input_key(ctx, NK_KEY_TEXT_WORD_LEFT, down);
            else nk_input_key(ctx, NK_KEY_LEFT, down);
        } else if(key == sfKeyRight) {
            if(sfKeyboard_isKeyPressed(sfKeyLControl))
                nk_input_key(ctx, NK_KEY_TEXT_WORD_RIGHT, down);
            else nk_input_key(ctx, NK_KEY_RIGHT, down);
        } else return 0;
        return 1;
    } else if(evt->type == sfEvtMouseButtonPressed || evt->type == sfEvtMouseButtonReleased) {
        int down = evt->type == sfEvtMouseButtonPressed;
        const int x = evt->mouseButton.x, y = evt->mouseButton.y;
        if(evt->mouseButton.button == sfMouseLeft)
            nk_input_button(ctx, NK_BUTTON_LEFT, x, y, down);
        if(evt->mouseButton.button == sfMouseMiddle)
            nk_input_button(ctx, NK_BUTTON_MIDDLE, x, y, down);
        if(evt->mouseButton.button == sfMouseRight)
            nk_input_button(ctx, NK_BUTTON_RIGHT, x, y, down);
        else return 0;
        return 1;
    } else if(evt->type == sfEvtMouseMoved) {
        nk_input_motion(ctx, evt->mouseMove.x, evt->mouseMove.y);
        return 1;
    } else if(evt->type == sfEvtTouchBegan || evt->type == sfEvtTouchEnded) {
        int down = evt->type == sfEvtTouchBegan;
        const int x = evt->touch.x, y = evt->touch.y;
        nk_input_button(ctx, NK_BUTTON_LEFT, x, y, down);
        return 1;
    } else if(evt->type == sfEvtTouchMoved) {
        if(ctx->input.mouse.grabbed) {
            int x = (int)ctx->input.mouse.prev.x;
            int y = (int)ctx->input.mouse.prev.y;
            nk_input_motion(ctx, x + evt->touch.x, y + evt->touch.y);
        } else nk_input_motion(ctx, evt->touch.x, evt->touch.y);
        return 1;
    } else if(evt->type == sfEvtTextEntered) {
		/* 8 ~ backspace */
		if (evt->text.unicode != 8) {  
			nk_input_unicode(ctx, evt->text.unicode);
		}
        return 1;
    } else if(evt->type == sfEvtMouseWheelScrolled) {
        nk_input_scroll(ctx, nk_vec2(0,evt->mouseWheelScroll.delta));
        return 1;
    }
    return 0;
}

NK_API void nk_csfml_shutdown(){
    nk_font_atlas_clear(&csfml.atlas);
    nk_free(&csfml.ctx);
    nk_csfml_device_destroy();
    memset(&csfml, 0, sizeof(csfml));
}

#endif

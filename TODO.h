/** Todo:
 * TODO: remove the little space between the combobox buttons and the combobox window to the right side 
 * TODO: add shortcut customization option 
 * TODO: second font color on cfg.json 
 */


// =======================================================================
//                        Scroll wheel
// =======================================================================

// static int
// ui_piemenu(struct nk_context *ctx, struct nk_vec2 pos, float radius,
//             struct nk_image *icons, int item_count)
// {
//     int ret = -1;
//     struct nk_rect total_space;
//     struct nk_rect bounds;
//     int active_item = 0;

//     /* pie menu popup */
//     struct nk_color border = ctx->style.window.border_color;
//     struct nk_style_item background = ctx->style.window.fixed_background;
//     ctx->style.window.fixed_background = nk_style_item_hide();
//     ctx->style.window.border_color = nk_rgba(0,0,0,0);

//     total_space  = nk_window_get_content_region(ctx);
//     ctx->style.window.spacing = nk_vec2(0,0);
//     ctx->style.window.padding = nk_vec2(0,0);

//     if (nk_popup_begin(ctx, NK_POPUP_STATIC, "piemenu", NK_WINDOW_NO_SCROLLBAR,
//         nk_rect(pos.x - total_space.x - radius, pos.y - radius - total_space.y,
//         2*radius,2*radius)))
//     {
//         int i = 0;
//         struct nk_command_buffer* out = nk_window_get_canvas(ctx);
//         const struct nk_input *in = &ctx->input;

//         total_space = nk_window_get_content_region(ctx);
//         ctx->style.window.spacing = nk_vec2(4,4);
//         ctx->style.window.padding = nk_vec2(8,8);
//         nk_layout_row_dynamic(ctx, total_space.h, 1);
//         nk_widget(&bounds, ctx);

//         /* outer circle */
//         nk_fill_circle(out, bounds, nk_rgb(50,50,50));
//         {
//             /* circle buttons */
//             float step = (2 * 3.141592654f) / (float)(MAX(1,item_count));
//             float a_min = 0; float a_max = step;

//             struct nk_vec2 center = nk_vec2(bounds.x + bounds.w / 2.0f, bounds.y + bounds.h / 2.0f);
//             struct nk_vec2 drag = nk_vec2(in->mouse.pos.x - center.x, in->mouse.pos.y - center.y);
//             float angle = (float)atan2(drag.y, drag.x);
//             if (angle < -0.0f) angle += 2.0f * 3.141592654f;
//             active_item = (int)(angle/step);

//             for (i = 0; i < item_count; ++i) {
//                 struct nk_rect content;
//                 float rx, ry, dx, dy, a;
//                 nk_fill_arc(out, center.x, center.y, (bounds.w/2.0f),
//                     a_min, a_max, (active_item == i) ? nk_rgb(45,100,255): nk_rgb(60,60,60));

//                 /* separator line */
//                 rx = bounds.w/2.0f; ry = 0;
//                 dx = rx * (float)cos(a_min) - ry * (float)sin(a_min);
//                 dy = rx * (float)sin(a_min) + ry * (float)cos(a_min);
//                 nk_stroke_line(out, center.x, center.y,
//                     center.x + dx, center.y + dy, 1.0f, nk_rgb(50,50,50));

//                 /* button content */
//                 a = a_min + (a_max - a_min)/2.0f;
//                 rx = bounds.w/2.5f; ry = 0;
//                 content.w = 30; content.h = 30;
//                 content.x = center.x + ((rx * (float)cos(a) - ry * (float)sin(a)) - content.w/2.0f);
//                 content.y = center.y + (rx * (float)sin(a) + ry * (float)cos(a) - content.h/2.0f);
//                 nk_draw_image(out, content, &icons[i], nk_rgb(255,255,255));
//                 a_min = a_max; a_max += step;
//             }
//         }
//         {
//             /* inner circle */
//             struct nk_rect inner;
//             inner.x = bounds.x + bounds.w/2 - bounds.w/4;
//             inner.y = bounds.y + bounds.h/2 - bounds.h/4;
//             inner.w = bounds.w/2; inner.h = bounds.h/2;
//             nk_fill_circle(out, inner, nk_rgb(45,45,45));

//             /* active icon content */
//             bounds.w = inner.w / 2.0f;
//             bounds.h = inner.h / 2.0f;
//             bounds.x = inner.x + inner.w/2 - bounds.w/2;
//             bounds.y = inner.y + inner.h/2 - bounds.h/2;
//             nk_draw_image(out, bounds, &icons[active_item], nk_rgb(255,255,255));
//         }
//         nk_layout_space_end(ctx);
//         if (!nk_input_is_mouse_down(&ctx->input, NK_BUTTON_RIGHT)) {
//             nk_popup_close(ctx);
//             ret = active_item;
//         }
//     } else ret = -2;
//     ctx->style.window.spacing = nk_vec2(4,4);
//     ctx->style.window.padding = nk_vec2(8,8);
//     nk_popup_end(ctx);

//     ctx->style.window.fixed_background = background;
//     ctx->style.window.border_color = border;
//     return ret;
// }
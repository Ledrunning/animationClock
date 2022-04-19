#include "painter.h"
#include <stddef.h>
#include <stdio.h>
#include "utils/utils.h"



static const uint16_t sin_table[] = {
    0x477, 0x8ef, 0xd65, 0x11db, 0x164f, 0x1ac2, 0x1f32, 0x23a0, 0x280c,
    0x2c74, 0x30d8, 0x3539, 0x3996, 0x3dee, 0x4241, 0x4690, 0x4ad8, 0x4f1b, 0x5358,
    0x578e, 0x5bbe, 0x5fe6, 0x6406, 0x681f, 0x6c30, 0x7039, 0x7438, 0x782f, 0x7c1c,
    0x7fff, 0x83d9, 0x87a8, 0x8b6d, 0x8f27, 0x92d5, 0x9679, 0x9a10, 0x9d9b, 0xa11b,
    0xa48d, 0xa7f3, 0xab4c, 0xae97, 0xb1d5, 0xb504, 0xb826, 0xbb39, 0xbe3e, 0xc134,
    0xc41b, 0xc6f3, 0xc9bb, 0xcc73, 0xcf1b, 0xd1b3, 0xd43b, 0xd6b3, 0xd919, 0xdb6f,
    0xddb3, 0xdfe7, 0xe208, 0xe419, 0xe617, 0xe803, 0xe9de, 0xeba6, 0xed5b, 0xeeff,
    0xf08f, 0xf20d, 0xf378, 0xf4d0, 0xf615, 0xf746, 0xf865, 0xf970, 0xfa67, 0xfb4b,
    0xfc1c, 0xfcd9, 0xfd82, 0xfe17, 0xfe98, 0xff06, 0xff60, 0xffa6, 0xffd8, 0xfff6,
};


err_t painter_init(painter_t* painter, graphics_t* graphics)
{
    if(graphics == NULL) return E_NULL_POINTER;

    painter->graphics = graphics;
    painter->mode = PAINTER_MODE_SET;
    painter->pen = PAINTER_PEN_SOLID;
    painter->brush = PAINTER_BRUSH_NONE;
    painter->font = NULL;
    painter->fill_mode = PAINTER_FILL_MODE_ALL;
    painter->source_image_mode = PAINTER_SOURCE_IMAGE_MODE_NORMAL;
    painter->pen_color = GRAPHICS_COLOR_BLACK;
    painter->brush_color = GRAPHICS_COLOR_BLACK;
    painter->fill_color = GRAPHICS_COLOR_BLACK;
    painter->fill_target_color = GRAPHICS_COLOR_BLACK;
    painter->transparent_color = GRAPHICS_COLOR_BLACK;
    painter->transparent_color_enabled = false;
    painter->custom_pen_graphics = NULL;
    painter->custom_brush_graphics = NULL;
    rect_init(&painter->scissor_rect);
    painter->scissor_enabled = false;
    point_init(&painter->offset_point);
    painter->offset_enabled = false;

    return E_NO_ERROR;
}

bool painter_set_graphics(painter_t* painter, graphics_t* graphics)
{
    if(graphics == NULL) return false;

    painter->graphics = graphics;

    return true;
}

static ALWAYS_INLINE void painter_put_pixel(painter_t* painter, graphics_pos_t x, graphics_pos_t y, graphics_color_t color)
{
    if(painter->transparent_color_enabled && color == painter->transparent_color) return;
    if(painter->offset_enabled)
        { x += point_x(&painter->offset_point); y += point_y(&painter->offset_point); }
    if(painter->scissor_enabled && !rect_contains(&painter->scissor_rect, x, y)) return;
    switch(painter->mode){
        default:
        case PAINTER_MODE_SET:
            graphics_set_pixel(painter->graphics, x, y, color);
            break;
        case PAINTER_MODE_OR:
            graphics_or_pixel(painter->graphics, x, y, color);
            break;
        case PAINTER_MODE_XOR:
            graphics_xor_pixel(painter->graphics, x, y, color);
            break;
        case PAINTER_MODE_AND:
            graphics_and_pixel(painter->graphics, x, y, color);
            break;
    }
}

bool painter_flush(painter_t* painter)
{
#ifdef USE_GRAPHICS_VIRTUAL_BUFFER
    return graphics_flush(painter_graphics(painter));
#else
    return false;
#endif
}

void painter_fill(painter_t* painter)
{
    painter_draw_fillrect(painter, 0, 0, graphics_width(painter_graphics(painter)) - 1,
                                         graphics_height(painter_graphics(painter)) - 1);
}

void painter_draw_point(painter_t* painter, graphics_pos_t x, graphics_pos_t y)
{
    painter_put_pixel(painter, x, y, painter->pen_color);
}

void painter_put_line_pixel(painter_t* painter, graphics_pos_t x, graphics_pos_t y, size_t pixel_number)
{
    switch(painter->pen){
        default:
        case PAINTER_PEN_NONE:
            break;
        case PAINTER_PEN_SOLID:
            painter_put_pixel(painter, x, y, painter->pen_color);
            break;
        case PAINTER_PEN_DOT:
            if(!(pixel_number & 0x1)) painter_put_pixel(painter, x, y, painter->pen_color);
            break;
        case PAINTER_PEN_DASH:
            if(!(pixel_number & 0x2)) painter_put_pixel(painter, x, y, painter->pen_color);
            break;
        case PAINTER_PEN_DASH2:
            if(!(pixel_number & 0x4)) painter_put_pixel(painter, x, y, painter->pen_color);
            break;
        case PAINTER_PEN_DASH_DOT:
            if((pixel_number & 0x5) != 0x5) painter_put_pixel(painter, x, y, painter->pen_color);
            break;
        case PAINTER_PEN_CUSTOM:{
            if(painter->custom_pen_graphics == NULL) break;

            graphics_color_t color = graphics_get_pixel(
                                                painter->custom_pen_graphics,
                                                pixel_number % graphics_width(painter->custom_pen_graphics), 0
                                            );

            switch(painter->source_image_mode){
                default:
                case PAINTER_SOURCE_IMAGE_MODE_NORMAL:
                    color = graphics_convert_color(
                                            graphics_format(painter->graphics),
                                            graphics_format(painter->custom_pen_graphics),
                                            color
                                        );
                    painter_put_pixel(painter, x, y, color);
                    break;
                case PAINTER_SOURCE_IMAGE_MODE_MASK:{
                    color = graphics_apply_mask(
                                            graphics_format(painter->graphics),
                                            painter_get_pixel(painter, x, y),
                                            graphics_format(painter->custom_pen_graphics),
                                            color
                                        );
                    painter_put_pixel(painter, x, y, color);
                    }break;
                case PAINTER_SOURCE_IMAGE_MODE_MAP:
                    color = graphics_apply_mask(
                                            graphics_format(painter->graphics),
                                            painter->pen_color,
                                            graphics_format(painter->custom_pen_graphics),
                                            color
                                        );
                    painter_put_pixel(painter, x, y, color);
                    break;
                case PAINTER_SOURCE_IMAGE_MODE_BITMAP:
                    if(color) painter_put_pixel(painter, x, y, painter->pen_color);
                    break;
                case PAINTER_SOURCE_IMAGE_MODE_BITMASK:
                    color = (color) ? painter->pen_color : painter->brush_color;
                    painter_put_pixel(painter, x, y, color);
                    break;
            }

            }break;
    }
}

void painter_draw_vline(painter_t* painter, graphics_pos_t x, graphics_pos_t y0, graphics_pos_t y1)
{
    if(x < 0 || x >= (graphics_pos_t)graphics_width(painter->graphics)) return;
    
    size_t pixel_number = 0;

    if(y1 < y0){
        graphics_pos_t tmp;
        SWAP(y0, y1, tmp);
    }

    for(;y0 <= y1; y0 ++){
        painter_put_line_pixel(painter, x, y0, pixel_number);
        pixel_number ++;
    }
}

void painter_draw_hline(painter_t* painter, graphics_pos_t y, graphics_pos_t x0, graphics_pos_t x1)
{
    if(y < 0 || y >= (graphics_pos_t)graphics_height(painter->graphics)) return;
    
    size_t pixel_number = 0;

    if(x1 < x0){
        graphics_pos_t tmp;
        SWAP(x0, x1, tmp);
    }

    for(; x0 <= x1; x0 ++){
        painter_put_line_pixel(painter, x0, y, pixel_number);
        pixel_number ++;
    }
}

void painter_draw_line(painter_t* painter, graphics_pos_t x0, graphics_pos_t y0, graphics_pos_t x1, graphics_pos_t y1)
{
    if(x0 == x1){
        painter_draw_vline(painter, x0, y0, y1);
        return;
    }
    if(y0 == y1){
        painter_draw_hline(painter, y0, x0, x1);
        return;
    }
    
    size_t pixel_number = 0;

    graphics_pos_t dx = x1 - x0;
    graphics_pos_t dy = y1 - y0;

    graphics_pos_t x_step = (dx > 0) ? 1 : -1;
    graphics_pos_t y_step = (dy > 0) ? 1 : -1;
    
    dx = ABS(dx);
    dy = ABS(dy);
    
    graphics_pos_t err = dx - dy;
    graphics_pos_t err2 = 0;
    
    while(x0 != x1 || y0 != y1){
        painter_put_line_pixel(painter, x0, y0, pixel_number ++);
        
        err2 = err << 1;
        
        if(err2 > -dy){
            err -= dy;
            x0 += x_step;
        }
        
        if(err2 < dx){
            err += dx;
            y0 += y_step;
        }
    }
    
    painter_put_line_pixel(painter, x1, y1, pixel_number);
}

void painter_fill_back_put_pixel(painter_t* painter, graphics_pos_t x_first, graphics_pos_t y_first, graphics_pos_t x, graphics_pos_t y)
{
    graphics_pos_t dx = x - x_first;
    graphics_pos_t dy = y - y_first;

    dx = ABS(dx);
    dy = ABS(dy);

    switch(painter->brush){
        default:
        case PAINTER_BRUSH_NONE:
            break;
        case PAINTER_BRUSH_SOLID:
            painter_put_pixel(painter, x, y, painter->brush_color);
            break;
        case PAINTER_BRUSH_DOT:
            if(!(dx & 0x1 || dy & 0x1)) painter_put_pixel(painter, x, y, painter->brush_color);
            break;
        case PAINTER_BRUSH_DOT2:
            if(!((dx & 0x3) || (dy & 0x3))) painter_put_pixel(painter, x, y, painter->brush_color);
            break;
        case PAINTER_BRUSH_DOT_DIAG:
            if((dx + dy) & 0x1) painter_put_pixel(painter, x, y, painter->brush_color);
            break;
        case PAINTER_BRUSH_DOT2_DIAG:
            dx &= 0x3; dy &= 0x3;
            if((dx != dy) && ((dx | dy) == 0x2)) painter_put_pixel(painter, x, y, painter->brush_color);
            break;
        case PAINTER_BRUSH_DOT3_DIAG:
            dx &= 0x7; dy &= 0x7;
            if((dx != dy) && ((dx | dy) == 0x4)) painter_put_pixel(painter, x, y, painter->brush_color);
            break;
        case PAINTER_BRUSH_DASH_VER:
            if(dx & 0x1) painter_put_pixel(painter, x, y, painter->brush_color);
            break;
        case PAINTER_BRUSH_DASH2_VER:
            if((dx & 0x3) == 0) painter_put_pixel(painter, x, y, painter->brush_color);
            break;
        case PAINTER_BRUSH_DASH_HOR:
            if(dy & 0x1) painter_put_pixel(painter, x, y, painter->brush_color);
            break;
        case PAINTER_BRUSH_DASH2_HOR:
            if((dy & 0x3) == 0) painter_put_pixel(painter, x, y, painter->brush_color);
            break;
        case PAINTER_BRUSH_DASH_CROSS:
            if((dx & 0x1 || dy & 0x1)) painter_put_pixel(painter, x, y, painter->brush_color);
            break;
        case PAINTER_BRUSH_DASH2_CROSS:
            if((dx & 0x3) == 0 || (dy & 0x3) == 0) painter_put_pixel(painter, x, y, painter->brush_color);
            break;
        case PAINTER_BRUSH_DASH_DIAG_VER:
            if((dx & 0x3) == (dy & 0x3)) painter_put_pixel(painter, x, y, painter->brush_color);
            break;
        case PAINTER_BRUSH_DASH2_DIAG_VER:
            if((dx & 0x7) == (dy & 0x7)) painter_put_pixel(painter, x, y, painter->brush_color);
            break;
        case PAINTER_BRUSH_DASH_DIAG_HOR:
            if(((dx & 0x3) + (dy & 0x3)) == 0x3) painter_put_pixel(painter, x, y, painter->brush_color);
            break;
        case PAINTER_BRUSH_DASH2_DIAG_HOR:
            if(((dx & 0x7) + (dy & 0x7)) == 0x7) painter_put_pixel(painter, x, y, painter->brush_color);
            break;
        case PAINTER_BRUSH_DASH_DIAG_CROSS:
            dx &= 0x3; dy &= 0x3;
            if((dy == (dx + 1)) || ((dx + dy) == 0x3)) painter_put_pixel(painter, x, y, painter->brush_color);
            break;
        case PAINTER_BRUSH_DASH2_DIAG_CROSS:
            dx &= 0x7; dy &= 0x7;
            if((dy == (dx + 1)) || ((dx + dy) == 0x7)) painter_put_pixel(painter, x, y, painter->brush_color);
            break;
        case PAINTER_BRUSH_CUSTOM:{
            if(painter->custom_brush_graphics == NULL) break;

            graphics_color_t color = graphics_get_pixel(
                                                painter->custom_brush_graphics,
                                                dx % graphics_width(painter->custom_brush_graphics),
                                                dy % graphics_height(painter->custom_brush_graphics)
                                            );

            switch(painter->source_image_mode){
                default:
                case PAINTER_SOURCE_IMAGE_MODE_NORMAL:
                    color = graphics_convert_color(
                                            graphics_format(painter->graphics),
                                            graphics_format(painter->custom_brush_graphics),
                                            color
                                        );
                    painter_put_pixel(painter, x, y, color);
                    break;
                case PAINTER_SOURCE_IMAGE_MODE_MASK:{
                    color = graphics_apply_mask(
                                            graphics_format(painter->graphics),
                                            painter_get_pixel(painter, x, y),
                                            graphics_format(painter->custom_brush_graphics),
                                            color
                                        );
                    painter_put_pixel(painter, x, y, color);
                    }break;
                case PAINTER_SOURCE_IMAGE_MODE_MAP:
                    color = graphics_apply_mask(
                                            graphics_format(painter->graphics),
                                            painter->brush_color,
                                            graphics_format(painter->custom_brush_graphics),
                                            color
                                        );
                    painter_put_pixel(painter, x, y, color);
                    break;
                case PAINTER_SOURCE_IMAGE_MODE_BITMAP:
                    if(color) painter_put_pixel(painter, x, y, painter->brush_color);
                    break;
                case PAINTER_SOURCE_IMAGE_MODE_BITMASK:
                    color = (color) ? painter->pen_color : painter->brush_color;
                    painter_put_pixel(painter, x, y, color);
                    break;
            }

            }break;
    }
}

static void painter_fill_back(painter_t* painter, graphics_pos_t x_first, graphics_pos_t y_first, graphics_pos_t y_cur, graphics_pos_t x_from, graphics_pos_t x_to)
{
    if(painter->brush == PAINTER_BRUSH_NONE) return;

    if(x_from > x_to){
        graphics_pos_t tmp;
        SWAP(x_from, x_to, tmp);
    }

    for(; x_from <= x_to; x_from ++){
        painter_fill_back_put_pixel(painter, x_first, y_first, x_from, y_cur);
    }
}

#ifdef USE_GRAPHICS_VIRTUAL_BUFFER
static bool painter_fast_fillrect_impl(painter_t* painter, graphics_pos_t left, graphics_pos_t top, graphics_pos_t right, graphics_pos_t bottom)
{
    if(painter->brush != PAINTER_BRUSH_SOLID) return false;
    
    if(painter_offset_enabled(painter)){
        left   += painter->offset_point.x;
        right  += painter->offset_point.x;
        top    += painter->offset_point.y;
        bottom += painter->offset_point.y;
    }
    if(painter_scissor_enabled(painter)){
        if(left   < painter->scissor_rect.left)   left   = painter->scissor_rect.left;
        if(right  > painter->scissor_rect.right)  right  = painter->scissor_rect.right;
        if(top    < painter->scissor_rect.top)    top    = painter->scissor_rect.top;
        if(bottom > painter->scissor_rect.bottom) bottom = painter->scissor_rect.bottom;
    }
    
    if(left > right || top > bottom) return true;
    
    return graphics_fast_fillrect(painter_graphics(painter), left, top,
                                  right, bottom, painter_brush_color(painter));
}
#endif

void painter_draw_rect(painter_t* painter, graphics_pos_t left, graphics_pos_t top, graphics_pos_t right, graphics_pos_t bottom)
{
    if(left == right){
        painter_draw_vline(painter, left, top, bottom);
        return;
    }

    if(top == bottom){
        painter_draw_hline(painter, top, left, right);
        return;
    }

    if(left > right){
        graphics_pos_t tmp;
        SWAP(left, right, tmp);
    }

    if(top > bottom){
        graphics_pos_t tmp;
        SWAP(top, bottom, tmp);
    }
    
    if(left >= (graphics_pos_t)graphics_width(painter->graphics) || right < 0) return;
    if(top >= (graphics_pos_t)graphics_height(painter->graphics) || bottom < 0) return;

    painter_draw_hline(painter, top, left, right - 1);
    painter_draw_vline(painter, left, top + 1, bottom);
    painter_draw_hline(painter, bottom, left + 1, right);
    painter_draw_vline(painter, right, top, bottom - 1);

    /*painter_draw_hline(painter, top, left, right);
    painter_draw_vline(painter, left, top + 1, bottom - 1);
    painter_draw_hline(painter, bottom, left, right);
    painter_draw_vline(painter, right, top + 1, bottom - 1);*/

    if(painter->brush == PAINTER_BRUSH_NONE) return;

    if(bottom - top > 1 && right - left > 1){

        left ++;
        top ++;
        right --;
        bottom --;

#ifdef USE_GRAPHICS_VIRTUAL_BUFFER
        if(painter_fast_fillrect_impl(painter, left, top, right, bottom)){
            return;
        }
#endif
        
        graphics_pos_t x_first = left;
        graphics_pos_t y_first = top;

        for(; top <= bottom; top ++){
            painter_fill_back(painter, x_first, y_first, top, left, right);
        }
    }
}

void painter_draw_fillrect(painter_t* painter, graphics_pos_t left, graphics_pos_t top, graphics_pos_t right, graphics_pos_t bottom)
{
    if(painter->brush == PAINTER_BRUSH_NONE) return;
    
    if(left > right){
        graphics_pos_t tmp;
        SWAP(left, right, tmp);
    }

    if(top > bottom){
        graphics_pos_t tmp;
        SWAP(top, bottom, tmp);
    }

    if(left >= (graphics_pos_t)graphics_width(painter->graphics) || right < 0) return;
    if(top >= (graphics_pos_t)graphics_height(painter->graphics) || bottom < 0) return;

#ifdef USE_GRAPHICS_VIRTUAL_BUFFER
    if(painter_fast_fillrect_impl(painter, left, top, right, bottom)){
        return;
    }
#endif

    graphics_pos_t x_first = left;
    graphics_pos_t y_first = top;

    for(; top <= bottom; top ++){
        painter_fill_back(painter, x_first, y_first, top, left, right);
    }
}

void painter_draw_circle(painter_t* painter, graphics_pos_t center_x, graphics_pos_t center_y, graphics_pos_t radius)
{
    if(center_x + radius < 0 || center_x - radius >= 
        (graphics_pos_t)graphics_width(painter->graphics)) return;
    if(center_y + radius < 0 || center_y - radius >= 
        (graphics_pos_t)graphics_height(painter->graphics)) return;
    
    size_t pixel_number = 0;

    graphics_pos_t x = -radius;
    graphics_pos_t y = 0;

    graphics_pos_t r2 = radius * radius;

    graphics_pos_t err_x = 0;
    graphics_pos_t err_y = 0;

    graphics_pos_t x1 = x;
    graphics_pos_t y1 = y;

    graphics_pos_t x_first = center_x - radius + 1;
    graphics_pos_t y_first = center_y - radius + 1;
    graphics_pos_t x_fill_from = 0;
    graphics_pos_t x_fill_to = 0;
    graphics_pos_t old_x = 0;
    graphics_pos_t old_y = 0;

    //printf("circle\n");

    for(; x < 0;){

        old_y = y;
        old_x = x;

        painter_put_line_pixel(painter, center_x + x, center_y + y, pixel_number);
        painter_put_line_pixel(painter, center_x - x, center_y - y, pixel_number);
        painter_put_line_pixel(painter, center_x + y, center_y - x, pixel_number);
        painter_put_line_pixel(painter, center_x - y, center_y + x, pixel_number);

        //printf("(%d, %d) -> ", x, y);

        pixel_number ++;

        x1 = x + 1;
        y1 = y + 1;

        err_x = x1 * x1 + y * y - r2;
        err_x = ABS(err_x);

        err_y = x * x + y1 * y1 - r2;
        err_y = ABS(err_y);

        if(err_x < err_y){
            x ++;
        }else if(err_y < err_x){
            y ++;
        }else{
            x ++;
            y ++;
        }
        //printf("(%d, %d)\n", x, y);

        if(painter->brush != PAINTER_BRUSH_NONE){

            if(y != old_y){
                x_fill_from = center_x + old_x + 1;
                x_fill_to = center_x - old_x - 1;

                painter_fill_back(painter, x_first, y_first, center_y + old_y, x_fill_from, x_fill_to);
                if(old_y != 0) painter_fill_back(painter, x_first, y_first, center_y - old_y, x_fill_from, x_fill_to);
            }
        }
    }
}

void painter_draw_ellipse(painter_t* painter, graphics_pos_t center_x, graphics_pos_t center_y, graphics_pos_t a, graphics_pos_t b)
{
    if(center_x + a < 0 || center_x - a >= 
        (graphics_pos_t)graphics_width(painter->graphics)) return;
    if(center_y + b < 0 || center_y - b >= 
        (graphics_pos_t)graphics_height(painter->graphics)) return;
    
    size_t pixel_number = 0;

    graphics_pos_t x = -a;
    graphics_pos_t y = 0;

    graphics_pos_t a2 = a * a;
    graphics_pos_t b2 = b * b;
    graphics_pos_t a2b2 = a2 * b2;

    graphics_pos_t err_x = 0;
    graphics_pos_t err_y = 0;

    graphics_pos_t x1 = x;
    graphics_pos_t y1 = y;

    graphics_pos_t x_first = center_x - a + 1;
    graphics_pos_t y_first = center_y - b + 1;
    graphics_pos_t x_fill_from = 0;
    graphics_pos_t x_fill_to = 0;
    graphics_pos_t old_x = 0;
    graphics_pos_t old_y = 0;

    //printf("ellipse\n");

    for(; x <= 0;){

        old_y = y;
        old_x = x;

        painter_put_line_pixel(painter, center_x + x, center_y + y, pixel_number);
        if(x != 0) painter_put_line_pixel(painter, center_x - x, center_y + y, pixel_number);
        if(y != 0){
            painter_put_line_pixel(painter, center_x + x, center_y - y, pixel_number);
            if(x != 0) painter_put_line_pixel(painter, center_x - x, center_y - y, pixel_number);
        }

        //printf("(%d, %d) -> ", x, y);

        pixel_number ++;

        x1 = x + 1;
        y1 = y + 1;

        err_x = x1 * x1 * b2 + y * y * a2 - a2b2;
        err_x = ABS(err_x);

        err_y = x * x * b2 + y1 * y1 * a2 - a2b2;
        err_y = ABS(err_y);

        if(err_x < err_y){
            x ++;
        }else if(err_y < err_x){
            y ++;
        }else{
            x ++;
            y ++;
        }
        //printf("(%d, %d)\n", x, y);

        if(painter->brush != PAINTER_BRUSH_NONE){

            if(y != old_y){
                x_fill_from = center_x + old_x + 1;
                x_fill_to = center_x - old_x - 1;

                painter_fill_back(painter, x_first, y_first, center_y + old_y, x_fill_from, x_fill_to);
                if(old_y != 0) painter_fill_back(painter, x_first, y_first, center_y - old_y, x_fill_from, x_fill_to);
            }
        }
    }
}

void painter_draw_triangle(painter_t* painter, graphics_pos_t x0, graphics_pos_t y0,
                                               graphics_pos_t x1, graphics_pos_t y1,
                                               graphics_pos_t x2, graphics_pos_t y2)
{
    /*printf("draw triangle (%d, %d),(%d, %d),(%d, %d)...\n",
           x0, y0, x1, y1, x2, y2);*/
    
    if(x0 == x1 && x1 == x2){
        painter_draw_vline(painter, x0, y0, y2);
        return;
    }
    
    if(y0 == y1 && y1 == y2){
        painter_draw_hline(painter, y0, x0, x2);
        return;
    }
    
    graphics_pos_t tmp;
    
    if(y1 < y0){
        SWAP(y0, y1, tmp);
        SWAP(x0, x1, tmp);
    }
    if(y2 < y0){
        SWAP(y0, y2, tmp);
        SWAP(x0, x2, tmp);
    }
    if(y2 < y1){
        SWAP(y1, y2, tmp);
        SWAP(x1, x2, tmp);
    }
    
    if(y0 == y1){
        if(x1 < x0){
            SWAP(x0, x1, tmp);
        }
    }
    if(y1 == y2){
        if(x2 < x1){
            SWAP(x1, x2, tmp);
        }
    }
    if(y0 == y2){
        if(x2 < x0){
            SWAP(x0, x2, tmp);
        }
    }
    
    if(x0 >= graphics_width(painter_graphics(painter)) || x2 < 0) return;
    if(y0 >= graphics_height(painter_graphics(painter)) || y2 < 0) return;
    
    //printf("%d %d %d\n", y0, y1, y2);
    
    size_t i_pixels_count = 0;
    size_t j_pixels_count = 0;
    
    graphics_pos_t x_first = x0, y_first = y0;
    
    if(painter_brush(painter) != PAINTER_BRUSH_NONE){
        if(x1 < x_first) x_first = x1;
        if(x2 < x_first) x_first = x2;
    }
    
    const graphics_pos_t y_step = 1;
    graphics_pos_t i_step, j_step;
    graphics_pos_t i_step_old, j_step_old;
    
    graphics_pos_t i_err, j_err, err2;
    
    graphics_pos_t i, j, i_y, j_y;
    
    graphics_pos_t i_dx, i_dy, j_dx, j_dy;
    
    graphics_pos_t i_old, j_old, i_y_old, j_y_old;
    
    graphics_pos_t fill_from, fill_to;
    
    i = x0;
    j = x0;
    
    i_y = y0;
    j_y = y0;
    
    i_dx = x1 - x0;
    i_step = (i_dx >= 0) ? 1 : -1;
    i_dx = ABS(i_dx);
    
    i_dy = y1 - y0;
    i_dy = ABS(i_dy);
    
    j_dx = x2 - x0;
    j_step = (j_dx >= 0) ? 1 : -1;
    j_dx = ABS(j_dx);
    
    j_dy = y2 - y0;
    j_dy = ABS(j_dy);
    
    i_err = (i_dx - i_dy);
    j_err = (j_dx - j_dy);
    
    i_old = i; j_old = j; i_y_old = i_y; j_y_old = j_y;
    
    while((i_y != y2 || i != x2) || (j_y != y2 || j != x2)){
        
        if(i_y != i_y_old){
            i_old = i;
        }
        if(j_y != j_y_old){
            j_old = j;
        }
        i_y_old = i_y; j_y_old = j_y;
        
        i_step_old = i_step;
        j_step_old = j_step;
        
        while(i_y <= j_y){
            if(i_y == y2 && i == x2) break;
            painter_put_line_pixel(painter, i, i_y, i_pixels_count);

            i_pixels_count ++;

            err2 = i_err * 2;

            if(err2 > -i_dy){
                i_err -= i_dy;
                i += i_step;
            }

            if(err2 < i_dx){
                i_err += i_dx;
                i_y += y_step;
            }
        
            if(i_y == y1 && i == x1){
                i_dx = x2 - x1;
                i_step = (i_dx >= 0) ? 1 : -1;
                i_dx = ABS(i_dx);
                i_dy = y2 - y1;
                i_dy = ABS(i_dy);
                i_err = (i_dx - i_dy);
            }
        }
        
        while(j_y < i_y || i_y == y2){
            if(j_y == y2 && j == x2) break;
            painter_put_line_pixel(painter, j, j_y, j_pixels_count);
            
            j_pixels_count ++;
            
            err2 = j_err * 2;

            if(err2 > -j_dy){
                j_err -= j_dy;
                j += j_step;
            }

            if(err2 < j_dx){
                j_err += j_dx;
                j_y += y_step;
            }
        }
        
        //fill.
        if(painter_brush(painter) != PAINTER_BRUSH_NONE){
            if(i < j){
                if(i_step_old < 0){
                    fill_from = i_old - i_step_old;
                }else{
                    fill_from = i;
                }
                if(j_step_old < 0){
                    fill_to = j;
                }else{
                    fill_to = j_old - j_step_old;
                }
            }else{
                if(i_step_old < 0){
                    fill_to = i;
                }else{
                    fill_to = i_old - i_step_old;
                }
                if(j_step_old < 0){
                    fill_from = j_old - j_step_old;
                }else{
                    fill_from = j;
                }
            }
            
            if(i_y_old == y1){
                if(i < j){
                    if(fill_from < i){
                        fill_from = i;
                    }
                }else{
                    if(fill_to > i){
                        fill_to = i;
                    }
                }
            }
            
            //printf("%d -> %d [%d];  ", fill_from, fill_to, i_y_old);
            if(fill_to >= fill_from && i_y != i_y_old){
                //printf("~ ");
                painter_fill_back(painter, x_first, y_first, i_y_old, fill_from, fill_to);
            }
        }
        //printf("i (%d, %d) - j (%d, %d)\n", (int)i, (int)i_y, (int)j, (int)j_y);
    }
    
    painter_put_line_pixel(painter, x2, y2, i_pixels_count ++);
    
    //printf("done.\n");
}

void painter_bitblt(painter_t* painter, graphics_pos_t dst_x, graphics_pos_t dst_y,
                    const graphics_t* src_graphics, graphics_pos_t src_x, graphics_pos_t src_y,
                    graphics_size_t src_width, graphics_size_t src_height)
{
    if(src_graphics == NULL) return;
    if(dst_x >= (graphics_pos_t)graphics_width(painter->graphics) ||
       dst_y >= (graphics_pos_t)graphics_height(painter->graphics)) return;
    if(src_x >= (graphics_pos_t)graphics_width(src_graphics) ||
       src_y >= (graphics_pos_t)graphics_height(src_graphics)) return;
    if((dst_x + (graphics_pos_t)graphics_width(src_graphics)) < 0 ||
       (dst_y + (graphics_pos_t)graphics_height(src_graphics)) < 0) return;

    graphics_pos_t cur_dst_x = 0;
    graphics_pos_t cur_dst_y = 0;

    graphics_pos_t cur_src_x = 0;
    graphics_pos_t cur_src_y = 0;

    graphics_pos_t src_right = src_x + src_width;
    graphics_pos_t src_bottom = src_y + src_height;

    graphics_color_t color;

    for(cur_src_y = src_y, cur_dst_y = dst_y;; cur_src_y ++, cur_dst_y ++){

        if(cur_src_y >= (graphics_pos_t)graphics_height(src_graphics) ||
           cur_src_y >= src_bottom) break;
        if(cur_dst_y >= (graphics_pos_t)graphics_height(painter->graphics)) 
            break;

        cur_src_x = src_x;
        cur_dst_x = dst_x;

        for(;; cur_src_x ++, cur_dst_x ++){

            if(cur_src_x >= (graphics_pos_t)graphics_width(src_graphics) ||
               cur_src_x >= src_right) break;
            if(cur_dst_x >= (graphics_pos_t)graphics_width(painter->graphics)) 
                break;

            color = graphics_get_pixel(src_graphics, cur_src_x, cur_src_y);


            switch(painter->source_image_mode){
                default:
                case PAINTER_SOURCE_IMAGE_MODE_NORMAL:
                    color = graphics_convert_color(
                                            graphics_format(painter->graphics),
                                            graphics_format(src_graphics),
                                            color
                                        );
                    painter_put_pixel(painter, cur_dst_x, cur_dst_y, color);
                    break;
                case PAINTER_SOURCE_IMAGE_MODE_MASK:
                    color = graphics_apply_mask(
                                            graphics_format(painter->graphics),
                                            painter_get_pixel(painter, cur_dst_x, cur_dst_y),
                                            graphics_format(src_graphics),
                                            color
                                        );
                    painter_put_pixel(painter, cur_dst_x, cur_dst_y, color);
                    break;
                case PAINTER_SOURCE_IMAGE_MODE_MAP:
                    color = graphics_apply_mask(
                                            graphics_format(painter->graphics),
                                            painter->pen_color,
                                            graphics_format(src_graphics),
                                            color
                                        );
                    painter_put_pixel(painter, cur_dst_x, cur_dst_y, color);
                    break;
                case PAINTER_SOURCE_IMAGE_MODE_BITMAP:
                    if(color) painter_put_pixel(painter, cur_dst_x, cur_dst_y, painter->pen_color);
                    break;
                case PAINTER_SOURCE_IMAGE_MODE_BITMASK:
                    color = (color) ? painter->pen_color : painter->brush_color;
                    painter_put_pixel(painter, cur_dst_x, cur_dst_y, color);
                    break;
            }

        }
    }
}

int32_t painter_normalize_angle(int32_t angle)
{
    if(angle >= 360 || angle <= -360) angle %= 360;
    if(angle < 0) angle += 360;

    return angle;
}

fixed32_t painter_sin(int32_t angle)
{
    angle = painter_normalize_angle(angle);

    if(angle < 90){
        if(angle == 0) return 0;
        return (fixed32_t)sin_table[angle - 1];
    }else if(angle < 180){
        if(angle == 90) return fixed32_make_from_int(1);
        return (fixed32_t)sin_table[179 - angle];
    }else if(angle < 270){
        if(angle == 180) return 0;
        return -(fixed32_t)sin_table[angle - 181];
    }else if(angle < 360){
        if(angle == 270) return -fixed32_make_from_int(1);
        return -(fixed32_t)sin_table[359 - angle];
    }
    return 0; // 360.
}

fixed32_t painter_cos(int32_t angle)
{
    angle = painter_normalize_angle(angle);

    if(angle < 90){
        if(angle == 0) return fixed32_make_from_int(1);
        return (fixed32_t)sin_table[89 - angle];
    }else if(angle < 180){
        if(angle == 90) return 0;
        return -(fixed32_t)sin_table[angle - 91];
    }else if(angle < 270){
        if(angle == 180) return -fixed32_make_from_int(1);
        return -(fixed32_t)sin_table[269 - angle];
    }else if(angle < 360){
        if(angle == 270) return 0;
        return (fixed32_t)sin_table[angle - 271];
    }
    return fixed32_make_from_int(1); // 360.
}

graphics_pos_t painter_rotate_x(graphics_pos_t x, int32_t angle)
{
    fixed32_t res = painter_cos(angle) * x;
    res = fixed32_round(res);
    return fixed32_get_int(res);
}

graphics_pos_t painter_rotate_y(graphics_pos_t y, int32_t angle)
{
    fixed32_t res = painter_sin(angle) * y;
    res = fixed32_round(res);
    return fixed32_get_int(res);
}

void painter_rotate(graphics_pos_t* x, graphics_pos_t* y, int32_t angle)
{
    graphics_pos_t vx = 0;
    graphics_pos_t vy = 0;
    
    if(x) vx = *x;
    if(y) vy = *y;
    
    if(x) *x = painter_rotate_x(vx, angle) + painter_rotate_y(vy, angle);
    if(y) *y = painter_rotate_y(vx, angle) + painter_rotate_x(vy, angle);
}

void painter_draw_arc(painter_t* painter, graphics_pos_t center_x, graphics_pos_t center_y,
                      graphics_pos_t radius, int32_t from_angle, int32_t to_angle)
{
    if(center_x + radius < 0 || center_x - radius >= 
        (graphics_pos_t)graphics_width(painter->graphics)) return;
    if(center_y + radius < 0 || center_y - radius >= 
        (graphics_pos_t)graphics_height(painter->graphics)) return;
    
    //printf("\n\nArc!\n\n");

    if(to_angle - from_angle >= 360){
        painter_draw_circle(painter, center_x, center_y, radius);
        return;
    }

    size_t pixel_number = 0;

    from_angle = painter_normalize_angle(from_angle);
    to_angle = painter_normalize_angle(to_angle);

    graphics_pos_t x = painter_rotate_x(radius, from_angle);
    graphics_pos_t y = painter_rotate_y(radius, from_angle);

    graphics_pos_t x_to = painter_rotate_x(radius, to_angle);
    graphics_pos_t y_to = painter_rotate_y(radius, to_angle);

    graphics_pos_t dx = 0;
    graphics_pos_t dy = 0;

    if(from_angle < 90){
        dx = -1;
        dy = 1;
    }else if(from_angle < 180){
        dx = -1;
        dy = -1;
    }else if(from_angle < 270){
        dx = 1;
        dy = -1;
    }else{
        dx = 1;
        dy = 1;
    }

    graphics_pos_t r2 = radius * radius;

    graphics_pos_t err_x = 0;
    graphics_pos_t err_y = 0;

    graphics_pos_t x1 = 0;
    graphics_pos_t y1 = 0;

    //printf("from (%d, %d) to (%d, %d)\n", x, y, x_to, y_to);

    for(;;){
        //printf("(%d, %d) : (%d, %d)\n", x, y, dx, dy);

        painter_put_line_pixel(painter, center_x + x, center_y - y, pixel_number);

        if(x == x_to && y == y_to) break;

        pixel_number ++;

        x1 = x + dx;
        y1 = y + dy;

        err_x = x1 * x1 + y * y - r2;
        err_x = ABS(err_x);

        err_y = x * x + y1 * y1 - r2;
        err_y = ABS(err_y);

        if(err_x < err_y){
            x += dx;
        }else if(err_y < err_x){
            y += dy;
        }else{
            x += dx;
            y += dy;
        }

        if(x == 0) dy = -dy;
        if(y == 0) dx = -dx;

        /*if(pixel_number >= 360){
            //printf("Overfill!");
            break;
        }*/
    }
}

void painter_draw_ellipse_arc(painter_t* painter, graphics_pos_t center_x, graphics_pos_t center_y,
                             graphics_pos_t a, graphics_pos_t b, int32_t from_angle, int32_t to_angle)
{
    if(center_x + a < 0 || center_x - a >= 
        (graphics_pos_t)graphics_width(painter->graphics)) return;
    if(center_y + b < 0 || center_y - b >= 
        (graphics_pos_t)graphics_height(painter->graphics)) return;
    
    //printf("\n\nElliplse Arc!\n\n");

    if(to_angle - from_angle >= 360){
        painter_draw_ellipse(painter, center_x, center_y, a, b);
        return;
    }

    size_t pixel_number = 0;

    from_angle = painter_normalize_angle(from_angle);
    to_angle = painter_normalize_angle(to_angle);

    graphics_pos_t x = painter_rotate_x(a, from_angle);
    graphics_pos_t y = painter_rotate_y(b, from_angle);

    graphics_pos_t x_to = painter_rotate_x(a, to_angle);
    graphics_pos_t y_to = painter_rotate_y(b, to_angle);

    graphics_pos_t dx = 0;
    graphics_pos_t dy = 0;

    if(from_angle < 90){
        dx = -1;
        dy = 1;
    }else if(from_angle < 180){
        dx = -1;
        dy = -1;
    }else if(from_angle < 270){
        dx = 1;
        dy = -1;
    }else{
        dx = 1;
        dy = 1;
    }

    graphics_pos_t a2 = a * a;
    graphics_pos_t b2 = b * b;
    graphics_pos_t a2b2 = a2 * b2;

    graphics_pos_t err_x = 0;
    graphics_pos_t err_y = 0;

    graphics_pos_t x1 = 0;
    graphics_pos_t y1 = 0;

    //printf("from (%d, %d) to (%d, %d)\n", x, y, x_to, y_to);

    for(;;){
        //printf("(%d, %d) : (%d, %d)\n", x, y, dx, dy);

        painter_put_line_pixel(painter, center_x + x, center_y - y, pixel_number);

        if(x == x_to && y == y_to) break;

        pixel_number ++;

        x1 = x + dx;
        y1 = y + dy;

        err_x = x1 * x1 * b2 + y * y * a2 - a2b2;
        err_x = ABS(err_x);

        err_y = x * x * b2 + y1 * y1 * a2 - a2b2;
        err_y = ABS(err_y);

        if(err_x < err_y){
            x += dx;
        }else if(err_y < err_x){
            y += dy;
        }else{
            x += dx;
            y += dy;
        }

        if(x == 0) dy = -dy;
        if(y == 0) dx = -dx;

        /*if(pixel_number >= 360){
            //printf("Overfill!");
            break;
        }*/
    }
}

static graphics_pos_t painter_fill_all_impl(painter_t* painter, graphics_pos_t x, graphics_pos_t y)
{
    //painter_fill_back_put_pixel
    //painter_put_pixel(painter, x, y, painter->fill_color)

    if(x < 0 || y < 0) return x;
    if(x >= (graphics_pos_t)graphics_width(painter->graphics) ||
       y >= (graphics_pos_t)graphics_height(painter->graphics)) return x;

    graphics_color_t color = 0;

    color = painter_get_pixel(painter, x, y);
    if(color == painter->pen_color || color == painter->fill_color) return x;

    painter_put_pixel(painter, x, y, painter->fill_color);

    graphics_pos_t x_left = x - 1;
    graphics_pos_t x_right = x + 1;

    for(; x_left >= 0; x_left --){
        color = painter_get_pixel(painter, x_left, y);
        if(color == painter->pen_color || color == painter->fill_color) break;

        painter_put_pixel(painter, x_left, y, painter->fill_color);
    }

    for(; x_right < (graphics_pos_t)graphics_width(painter->graphics); x_right ++){
        color = painter_get_pixel(painter, x_right, y);
        if(color == painter->pen_color || color == painter->fill_color) break;

        painter_put_pixel(painter, x_right, y, painter->fill_color);
    }

    if(y < (graphics_pos_t)graphics_height(painter->graphics)){
        for(x = x_left + 1; x < x_right;){

            color = painter_get_pixel(painter, x, y + 1);

            if(color != painter->pen_color && color != painter->fill_color){
                x = painter_fill_all_impl(painter, x, y + 1) + 1;
            }else{
                x ++;
            }

            if(x >= (graphics_pos_t)graphics_width(painter->graphics)) break;
        }
    }

    if(y > 0){
        for(x = x_left + 1; x < x_right;){

            color = painter_get_pixel(painter, x, y - 1);

            if(color != painter->pen_color && color != painter->fill_color){
                x = painter_fill_all_impl(painter, x, y - 1) + 1;
            }else{
                x ++;
            }

            if(x >= (graphics_pos_t)graphics_width(painter->graphics)) break;
        }
    }

    //printf("%d, ", x);

    return x_right;
}

static graphics_pos_t painter_fill_target_impl(painter_t* painter, graphics_pos_t x, graphics_pos_t y)
{
    //painter_fill_back_put_pixel
    //painter_put_pixel(painter, x, y, painter->fill_color)

    if(x < 0 || y < 0) return x;
    if(x >= (graphics_pos_t)graphics_width(painter->graphics) ||
       y >= (graphics_pos_t)graphics_height(painter->graphics)) return x;

    graphics_color_t color = 0;

    color = painter_get_pixel(painter, x, y);
    if(color != painter->fill_target_color) return x;

    painter_put_pixel(painter, x, y, painter->fill_color);

    graphics_pos_t x_left = x - 1;
    graphics_pos_t x_right = x + 1;

    for(; x_left >= 0; x_left --){
        color = painter_get_pixel(painter, x_left, y);
        if(color != painter->fill_target_color) break;

        painter_put_pixel(painter, x_left, y, painter->fill_color);
    }

    for(; x_right < (graphics_pos_t)graphics_width(painter->graphics); x_right ++){
        color = painter_get_pixel(painter, x_right, y);
        if(color != painter->fill_target_color) break;

        painter_put_pixel(painter, x_right, y, painter->fill_color);
    }

    if(y < (graphics_pos_t)graphics_height(painter->graphics)){
        for(x = x_left + 1; x < x_right;){

            color = painter_get_pixel(painter, x, y + 1);

            if(color == painter->fill_target_color){
                x = painter_fill_target_impl(painter, x, y + 1) + 1;
            }else{
                x ++;
            }

            if(x >= (graphics_pos_t)graphics_width(painter->graphics)) break;
        }
    }

    if(y > 0){
        for(x = x_left + 1; x < x_right;){

            color = painter_get_pixel(painter, x, y - 1);

            if(color == painter->fill_target_color){
                x = painter_fill_target_impl(painter, x, y - 1) + 1;
            }else{
                x ++;
            }

            if(x >= (graphics_pos_t)graphics_width(painter->graphics)) break;
        }
    }

    //printf("%d, ", x);

    return x_right;
}


void painter_flood_fill(painter_t* painter, graphics_pos_t x, graphics_pos_t y)
{
    switch(painter->fill_mode){
        default:
        case PAINTER_FILL_MODE_ALL:
            painter_fill_all_impl(painter, x, y);
            break;
        case PAINTER_FILL_MODE_TARGET_COLOR:
            painter_fill_target_impl(painter, x, y);
            break;
    }
}

static bool painter_draw_char_impl(painter_t* painter, graphics_pos_t x, graphics_pos_t y, font_char_t c, rect_t* char_rect, point_t* char_offset)
{
    if(painter->font == NULL) return false;
    
    const font_bitmap_t* font_bitmap = NULL;
    rect_t rect;
    point_t offset;
    
    if(!font_get_char_bitmap_position(painter->font, c, &font_bitmap, &rect, &offset)) return false;
    
    x += point_x(&offset);
    y += point_y(&offset);
    
    if(x >= (graphics_pos_t)graphics_width(painter->graphics)) return true;
    if(y >= (graphics_pos_t)graphics_height(painter->graphics)) return true;
    
    if(x + (graphics_pos_t)rect_width(&rect) < 0) return true;
    if(y + (graphics_pos_t)rect_height(&rect) < 0) return true;

    painter_bitblt(painter, x, y, font_bitmap_graphics(font_bitmap),
            rect_left(&rect), rect_top(&rect), rect_width(&rect), rect_height(&rect));
    
    if(char_rect) rect_copy(char_rect, &rect);
    if(char_offset) point_copy(char_offset, &offset);
    
    return true;
}

size_t painter_draw_char(painter_t* painter, graphics_pos_t x, graphics_pos_t y, font_char_t c)
{
    return painter_draw_char_impl(painter, x, y, c, NULL, NULL) ? 1 : 0;
}

size_t painter_draw_string(painter_t* painter, graphics_pos_t x, graphics_pos_t y, const char* s)
{
    if(painter->font == NULL || s == NULL) return 0;
    
    if(x >= (graphics_pos_t)graphics_width(painter->graphics) ||
       y >= (graphics_pos_t)graphics_height(painter->graphics)) return 0;

    size_t tabs_count = 0;
    graphics_pos_t orig_x = x;
    font_char_t c = 0;
    size_t c_size = 0;
    size_t count = 0;
    rect_t char_rect;
    point_t char_offset;

    while(*s){
        switch(*s){
            case '\r':
                x = orig_x;
                s ++;
                count ++;
                break;
            case '\n':
                if(font_get_char_position(painter->font, 0x20, &char_rect, &char_offset)){
                    x = orig_x;
                    y += rect_height(&char_rect) + point_y(&char_offset) + font_vspace(painter->font);
                    count ++;
                }
                s ++;
                break;
            case '\t':
                if(painter_draw_char_impl(painter, x, y, 0x20, &char_rect, &char_offset)){
                    x += rect_width(&char_rect) + point_x(&char_offset) + font_hspace(painter->font);
                }else{
                    s ++;
                    break;
                }
                if(++ tabs_count == FONT_TAB_SIZE){
                    tabs_count = 0;
                    s ++;
                    count ++;
                }
                break;
            default:
                c = font_utf8_decode(s, &c_size);
                s += c_size;
                if(painter_draw_char_impl(painter, x, y, c, &char_rect, &char_offset)){
                    x += rect_width(&char_rect) + point_x(&char_offset) + font_hspace(painter->font);
                    count ++;
                }
                break;
        }
        if(y >= (graphics_pos_t)graphics_height(painter->graphics)) break;
    }
    
    return count;
}

void painter_string_size(painter_t* painter, const char* s, graphics_size_t* width, graphics_size_t* height)
{
    if(painter->font == NULL || s == NULL) return;
    if(width == NULL && height == NULL) return;
    
    graphics_size_t cur_x = 0;
    graphics_size_t x = 0;
    graphics_size_t y = 0;
    font_char_t c = 0;
    graphics_size_t orig_x = cur_x;
    size_t c_size = 0;
    
    rect_t char_rect;
    point_t char_offset;

    while(*s){
        switch(*s){
            case '\r':
                cur_x = orig_x;
                s ++;
                break;
            case '\n':
                if(font_get_char_position(painter->font, 0x20, &char_rect, &char_offset)){
                    cur_x = orig_x;
                    y += rect_height(&char_rect) + point_y(&char_offset) + font_vspace(painter->font);
                }
                s ++;
                break;
            case '\t':
                if(font_get_char_position(painter->font, 0x20, &char_rect, &char_offset)){
                    cur_x += (rect_width(&char_rect) + point_x(&char_offset) + font_hspace(painter->font)) * FONT_TAB_SIZE;
                }
                s ++;
                break;
            default:
                c = font_utf8_decode(s, &c_size);
                s += c_size;
                if(font_get_char_position(painter->font, c, &char_rect, &char_offset)){
                    cur_x += rect_width(&char_rect) + point_x(&char_offset) + font_hspace(painter->font);
                }
                break;
        }
        if(cur_x > x) x = cur_x;
    }
    
    if(cur_x != 0){
        if(font_get_char_position(painter->font, 0x20, &char_rect, &char_offset)){
            y += rect_height(&char_rect) + point_y(&char_offset);
        }
    }
    
    if(width) *width = x;
    if(height) *height = y;
}

size_t painter_draw_string_wrap(painter_t* painter, graphics_pos_t x, graphics_pos_t y, const char* s, graphics_size_t width)
{
    if(painter->font == NULL || s == NULL) return 0;
    
    if(x >= (graphics_pos_t)graphics_width(painter->graphics) ||
       y >= (graphics_pos_t)graphics_height(painter->graphics)) return 0;

    size_t tabs_count = 0;
    graphics_pos_t orig_x = x;
    font_char_t c = 0;
    size_t c_size = 0;
    size_t count = 0;
    rect_t char_rect;
    point_t char_offset;
    graphics_pos_t char_size;
    
    graphics_pos_t right = x + (graphics_pos_t)width;

    while(*s){
        
        c = font_utf8_decode(s, &c_size);
        if(font_get_char_position(painter->font, c, &char_rect, &char_offset)){
            char_size = rect_width(&char_rect) + point_x(&char_offset);
            
            if(x + char_size > right){
                if(x == orig_x){
                    break;
                }
                if(font_get_char_position(painter->font, 0x20, &char_rect, &char_offset)){
                    y += rect_height(&char_rect) + point_y(&char_offset) + font_vspace(painter->font);
                }else{
                    y += font_char_height(painter->font) + font_vspace(painter->font);
                }
                x = orig_x;
                
                if(y < (graphics_pos_t)graphics_height(painter->graphics)) continue;
            }
        }
        
        if(y >= (graphics_pos_t)graphics_height(painter->graphics)) break;
        
        switch(c){
            case '\r':
                x = orig_x;
                s ++;
                count ++;
                break;
            case '\n':
                if(font_get_char_position(painter->font, 0x20, &char_rect, &char_offset)){
                    x = orig_x;
                    y += rect_height(&char_rect) + point_y(&char_offset) + font_vspace(painter->font);
                    count ++;
                }
                s ++;
                break;
            case '\t':
                if(painter_draw_char_impl(painter, x, y, 0x20, &char_rect, &char_offset)){
                    x += rect_width(&char_rect) + point_x(&char_offset) + font_hspace(painter->font);
                }else{
                    s ++;
                    break;
                }
                if(++ tabs_count == FONT_TAB_SIZE){
                    tabs_count = 0;
                    s ++;
                    count ++;
                }
                break;
            default:
                s += c_size;
                if(painter_draw_char_impl(painter, x, y, c, &char_rect, &char_offset)){
                    x += rect_width(&char_rect) + point_x(&char_offset) + font_hspace(painter->font);
                    count ++;
                }
                break;
        }
    }
    
    return count;
}

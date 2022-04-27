#include "clock_gui.h"
#include "graphics/graphics.h"
#include "graphics/painter.h"
#include "graphics/font_5x8_utf8.h"
#include "gui/gui.h"
#include "gui/gui_widget.h"
#include "gui/gui_anim_bitmap.h"
#include "defs/defs.h"
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>


#define CLOCK_GUI_WIDTH 320
#define CLOCK_GUI_HEIGHT 240

#define CLOCK_GUI_ANIM_ITEM_MARGIN 1
#define CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH 5
#define CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT 7
#define CLOCK_GUI_ANIM_ITEMS_COUNT ((CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH) * (CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT))
#define CLOCK_GUI_GET_FONT_DATA(PTR, CHAR, FIRST) ((uint8_t*)PTR + CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH * ((unsigned int)CHAR - FIRST))
#define CLOCK_GUI_DIGITS_BITMAPS_COUNT 10

static const graphics_t clock_gui_digits_bitmaps[CLOCK_GUI_DIGITS_BITMAPS_COUNT] = {
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part0_data, '0', 0x20), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part0_data, '1', 0x20), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part0_data, '2', 0x20), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part0_data, '3', 0x20), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part0_data, '4', 0x20), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part0_data, '5', 0x20), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part0_data, '6', 0x20), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part0_data, '7', 0x20), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part0_data, '8', 0x20), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part0_data, '9', 0x20), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V)
};

#define CLOCK_GUI_WEEKDAY_CHARS_BITMAPS_COUNT 9
// 0 - П
// 1 - Н
// 2 - В
// 3 - Т
// 4 - С
// 5 - Р
// 6 - Ч
// 7 - Г
// 8 - Б
static const graphics_t clock_gui_weekday_chars_bitmaps[CLOCK_GUI_WEEKDAY_CHARS_BITMAPS_COUNT] = {
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part2_data, 0x41F, 0x400), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part2_data, 0x41D, 0x400), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part2_data, 0x412, 0x400), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part2_data, 0x422, 0x400), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part2_data, 0x421, 0x400), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part2_data, 0x420, 0x400), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part2_data, 0x427, 0x400), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part2_data, 0x413, 0x400), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part2_data, 0x411, 0x400), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V)
};

#define CLOCK_WEEKDAYS_COUNT 7
#define CLOCK_WEEKDAY_LEN 2
static const size_t clock_gui_weekday_indexes[CLOCK_WEEKDAYS_COUNT][CLOCK_WEEKDAY_LEN] = {
    {0, 1},
    {2, 3},
    {4, 5},
    {6, 7},
    {0, 3},
    {4, 8},
    {2, 4}
};


#define CLOCK_GUI_MONTH_NAME_CHARS_BITMAPS_COUNT 24
// 0 - Я
// 1 - н
// 2 - в

// 3 - Ф
// 4 - е

// 5 - М
// 6 - а
// 7 - р

// 8 - А
// 9 - п

// 10 - й

// 11 - И
// 12 - ю

// 13 - л

// 14 - г

// 15 - С

// 16 - О
// 17 - к
// 18 - т

// 19 - Н
// 20 - о
// 21 - я

// 22 - Д

static const graphics_t clock_gui_month_name_chars_bitmaps[CLOCK_GUI_MONTH_NAME_CHARS_BITMAPS_COUNT] = {
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part2_data, 0x42F, 0x400), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part2_data, 0x43D, 0x400), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part2_data, 0x432, 0x400), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part2_data, 0x424, 0x400), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part2_data, 0x435, 0x400), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part2_data, 0x41C, 0x400), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part2_data, 0x430, 0x400), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part2_data, 0x440, 0x400), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part2_data, 0x410, 0x400), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part2_data, 0x43F, 0x400), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part2_data, 0x439, 0x400), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part2_data, 0x418, 0x400), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part2_data, 0x44E, 0x400), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part2_data, 0x43B, 0x400), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part2_data, 0x433, 0x400), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part2_data, 0x421, 0x400), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part2_data, 0x41E, 0x400), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part2_data, 0x43A, 0x400), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part2_data, 0x442, 0x400), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part2_data, 0x41D, 0x400), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part2_data, 0x43E, 0x400), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part2_data, 0x44F, 0x400), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_graphics(CLOCK_GUI_GET_FONT_DATA(font_5x8_utf8_part2_data, 0x414, 0x400), CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V)
};

#define CLOCK_MONTH_NAMES_COUNT 12
#define CLOCK_MONTH_NAME_LEN 3
static const size_t clock_gui_month_name_indexes[CLOCK_MONTH_NAMES_COUNT][CLOCK_MONTH_NAME_LEN] = {
    {0, 1, 2}, // Янв
    {3, 4, 2}, // Фев
    {5, 6, 7}, // Мар
    {8, 9, 7}, // Апр
    {5, 6, 10}, // Май
    {11, 12, 1}, // Июн
    {11, 12, 13}, // Июл
    {8, 2, 14}, // Авг
    {15, 4, 1}, // Сен
    {16, 17, 18}, // Окт
    {19, 20, 21}, // Ноя
    {22, 4, 17} // Дек
};


// Пустой сепаратор.
#define CLOCK_GUI_CLOCK_EMPTY_SEP_WIDTH 1
#define CLOCK_GUI_CLOCK_EMPTY_SEP_ANIM_ITEMS_COUNT ((CLOCK_GUI_CLOCK_EMPTY_SEP_WIDTH) * (CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT))
static const uint8_t clock_gui_clock_empty_sep_data[CLOCK_GUI_CLOCK_EMPTY_SEP_WIDTH] = {
    0x00
};
static const graphics_t clock_gui_clock_empty_sep_bitmap = make_graphics(clock_gui_clock_empty_sep_data, CLOCK_GUI_CLOCK_EMPTY_SEP_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V);

// Сепаратор часов и минут (:).
#define CLOCK_GUI_CLOCK_SEP_WIDTH 1
#define CLOCK_GUI_CLOCK_SEP_ANIM_ITEMS_COUNT ((CLOCK_GUI_CLOCK_SEP_WIDTH) * (CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT))
static const uint8_t clock_gui_clock_sep_data[CLOCK_GUI_CLOCK_SEP_WIDTH] = {
    0x22
};
static const graphics_t clock_gui_clock_sep_bitmap = make_graphics(clock_gui_clock_sep_data, CLOCK_GUI_CLOCK_SEP_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V);

// Сепаратор дробной части температуры (.).
#define CLOCK_GUI_TEMP_DOT_WIDTH 1
#define CLOCK_GUI_TEMP_DOT_ANIM_ITEMS_COUNT ((CLOCK_GUI_TEMP_DOT_WIDTH) * (CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT))
static const uint8_t clock_gui_temp_dot_data[CLOCK_GUI_TEMP_DOT_WIDTH] = {
    0x40
};
static const graphics_t clock_gui_temp_dot_bitmap = make_graphics(clock_gui_temp_dot_data, CLOCK_GUI_TEMP_DOT_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V);

// Знак температуры - минус.
#define CLOCK_GUI_TEMP_SIGN_MINUS_WIDTH 3
#define CLOCK_GUI_TEMP_SIGN_MINUS_ANIM_ITEMS_COUNT ((CLOCK_GUI_TEMP_SIGN_MINUS_WIDTH) * (CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT))
static const uint8_t clock_gui_temp_sign_minus_data[CLOCK_GUI_TEMP_SIGN_MINUS_WIDTH] = {
    0x8, 0x8, 0x8
};

static const graphics_t clock_gui_temp_sign_minus_bitmap = make_graphics(clock_gui_temp_sign_minus_data, CLOCK_GUI_TEMP_SIGN_MINUS_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V);

// Знак температуры - плюс (пробел).
#define CLOCK_GUI_TEMP_SIGN_PLUS_WIDTH 3
#define CLOCK_GUI_TEMP_SIGN_PLUS_ANIM_ITEMS_COUNT ((CLOCK_GUI_TEMP_SIGN_PLUS_WIDTH) * (CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT))
static const uint8_t clock_gui_temp_sign_plus_data[CLOCK_GUI_TEMP_SIGN_PLUS_WIDTH] = {
    0x0, 0x0, 0x0
};
static const graphics_t clock_gui_temp_sign_plus_bitmap = make_graphics(clock_gui_temp_sign_plus_data, CLOCK_GUI_TEMP_SIGN_PLUS_WIDTH, CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT, GRAPHICS_FORMAT_BW_1_V);

#define BITMAP_SIZE_BYTES(arg_width, arg_height) ((arg_width * arg_height + 7) / 8)
#define ANIM_ITEMS_COUNT(arg_width, arg_height) ((arg_width * arg_height * 2) / 3)

#define DECLARE_ANIM_BITMAP_DATA(arg_name, arg_width, arg_height, arg_format) \
        static uint8_t CONCAT(arg_name, _data)[BITMAP_SIZE_BYTES(arg_width, arg_height)];\
        static graphics_t CONCAT(arg_name, _graphics) = make_graphics(CONCAT(arg_name, _data), arg_width, arg_height, arg_format);\
        static gui_anim_bitmap_item_t CONCAT(arg_name, _items)[ANIM_ITEMS_COUNT(arg_width, arg_height)];\

#define DECLARE_ANIM_BITMAP_DATA_COUNT(arg_name, arg_items_count, arg_width, arg_height, arg_format) \
        static uint8_t CONCAT(arg_name, _data)[BITMAP_SIZE_BYTES(arg_width, arg_height)];\
        static graphics_t CONCAT(arg_name, _graphics) = make_graphics(CONCAT(arg_name, _data), arg_width, arg_height, arg_format);\
        static gui_anim_bitmap_item_t CONCAT(arg_name, _items)[arg_items_count];\

#define DECLARE_NOANIM_BITMAP_DATA(arg_name, arg_width, arg_height, arg_format) \
        static uint8_t CONCAT(arg_name, _data)[BITMAP_SIZE_BYTES(arg_width, arg_height)];\
        static graphics_t CONCAT(arg_name, _graphics) = make_graphics(CONCAT(arg_name, _data), arg_width, arg_height, arg_format);

#define CLOCK_GUI_TIME_ITEM_BITMAP_WIDTH CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH
#define CLOCK_GUI_TIME_ITEM_BITMAP_HEIGHT CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT
#define CLOCK_GUI_TIME_SEP_BITMAP_WIDTH CLOCK_GUI_CLOCK_SEP_WIDTH
#define CLOCK_GUI_TIME_SEP_BITMAP_HEIGHT CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT

#define CLOCK_GUI_TEMP_ITEM_BITMAP_WIDTH CLOCK_GUI_ANIM_ITEMS_COUNT_WIDTH
#define CLOCK_GUI_TEMP_ITEM_BITMAP_HEIGHT CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT
#define CLOCK_GUI_TEMP_SIGN_BITMAP_WIDTH CLOCK_GUI_TEMP_SIGN_MINUS_WIDTH
#define CLOCK_GUI_TEMP_SIGN_BITMAP_HEIGHT CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT
#define CLOCK_GUI_TEMP_SEP_BITMAP_WIDTH CLOCK_GUI_TEMP_DOT_WIDTH
#define CLOCK_GUI_TEMP_SEP_BITMAP_HEIGHT CLOCK_GUI_ANIM_ITEMS_COUNT_HEIGHT

#define CLOCK_GUI_DIGIT_ANIM_ITEMS 21
#define CLOCK_GUI_TIME_SEP_ANIM_ITEMS 2
#define CLOCK_GUI_TEMP_SIGN_ANIM_ITEMS 3

DECLARE_ANIM_BITMAP_DATA_COUNT(monthday_tens, CLOCK_GUI_DIGIT_ANIM_ITEMS, CLOCK_GUI_TIME_ITEM_BITMAP_WIDTH, CLOCK_GUI_TIME_ITEM_BITMAP_HEIGHT, GRAPHICS_FORMAT_BW_1_V);
DECLARE_ANIM_BITMAP_DATA_COUNT(monthday_ones, CLOCK_GUI_DIGIT_ANIM_ITEMS, CLOCK_GUI_TIME_ITEM_BITMAP_WIDTH, CLOCK_GUI_TIME_ITEM_BITMAP_HEIGHT, GRAPHICS_FORMAT_BW_1_V);
DECLARE_ANIM_BITMAP_DATA_COUNT(month_first, CLOCK_GUI_DIGIT_ANIM_ITEMS, CLOCK_GUI_TIME_ITEM_BITMAP_WIDTH, CLOCK_GUI_TIME_ITEM_BITMAP_HEIGHT, GRAPHICS_FORMAT_BW_1_V);
DECLARE_ANIM_BITMAP_DATA_COUNT(month_second, CLOCK_GUI_DIGIT_ANIM_ITEMS, CLOCK_GUI_TIME_ITEM_BITMAP_WIDTH, CLOCK_GUI_TIME_ITEM_BITMAP_HEIGHT, GRAPHICS_FORMAT_BW_1_V);
DECLARE_ANIM_BITMAP_DATA_COUNT(month_third, CLOCK_GUI_DIGIT_ANIM_ITEMS, CLOCK_GUI_TIME_ITEM_BITMAP_WIDTH, CLOCK_GUI_TIME_ITEM_BITMAP_HEIGHT, GRAPHICS_FORMAT_BW_1_V);
DECLARE_ANIM_BITMAP_DATA_COUNT(weekday_tens, CLOCK_GUI_DIGIT_ANIM_ITEMS, CLOCK_GUI_TIME_ITEM_BITMAP_WIDTH, CLOCK_GUI_TIME_ITEM_BITMAP_HEIGHT, GRAPHICS_FORMAT_BW_1_V);
DECLARE_ANIM_BITMAP_DATA_COUNT(weekday_ones, CLOCK_GUI_DIGIT_ANIM_ITEMS, CLOCK_GUI_TIME_ITEM_BITMAP_WIDTH, CLOCK_GUI_TIME_ITEM_BITMAP_HEIGHT, GRAPHICS_FORMAT_BW_1_V);

DECLARE_ANIM_BITMAP_DATA_COUNT(hours_tens, CLOCK_GUI_DIGIT_ANIM_ITEMS, CLOCK_GUI_TIME_ITEM_BITMAP_WIDTH, CLOCK_GUI_TIME_ITEM_BITMAP_HEIGHT, GRAPHICS_FORMAT_BW_1_V);
DECLARE_ANIM_BITMAP_DATA_COUNT(hours_ones, CLOCK_GUI_DIGIT_ANIM_ITEMS, CLOCK_GUI_TIME_ITEM_BITMAP_WIDTH, CLOCK_GUI_TIME_ITEM_BITMAP_HEIGHT, GRAPHICS_FORMAT_BW_1_V);
DECLARE_ANIM_BITMAP_DATA_COUNT(hours_mins_sep, CLOCK_GUI_TIME_SEP_ANIM_ITEMS, CLOCK_GUI_TIME_SEP_BITMAP_WIDTH, CLOCK_GUI_TIME_SEP_BITMAP_HEIGHT, GRAPHICS_FORMAT_BW_1_V);
DECLARE_ANIM_BITMAP_DATA_COUNT(mins_tens, CLOCK_GUI_DIGIT_ANIM_ITEMS, CLOCK_GUI_TIME_ITEM_BITMAP_WIDTH, CLOCK_GUI_TIME_ITEM_BITMAP_HEIGHT, GRAPHICS_FORMAT_BW_1_V);
DECLARE_ANIM_BITMAP_DATA_COUNT(mins_ones, CLOCK_GUI_DIGIT_ANIM_ITEMS, CLOCK_GUI_TIME_ITEM_BITMAP_WIDTH, CLOCK_GUI_TIME_ITEM_BITMAP_HEIGHT, GRAPHICS_FORMAT_BW_1_V);

DECLARE_ANIM_BITMAP_DATA_COUNT(ind_temp_sign, CLOCK_GUI_TEMP_SIGN_ANIM_ITEMS, CLOCK_GUI_TEMP_SIGN_BITMAP_WIDTH, CLOCK_GUI_TEMP_SIGN_BITMAP_HEIGHT, GRAPHICS_FORMAT_BW_1_V);
DECLARE_ANIM_BITMAP_DATA_COUNT(ind_temp_tens, CLOCK_GUI_DIGIT_ANIM_ITEMS, CLOCK_GUI_TEMP_ITEM_BITMAP_WIDTH, CLOCK_GUI_TEMP_ITEM_BITMAP_HEIGHT, GRAPHICS_FORMAT_BW_1_V);
DECLARE_ANIM_BITMAP_DATA_COUNT(ind_temp_ones, CLOCK_GUI_DIGIT_ANIM_ITEMS, CLOCK_GUI_TEMP_ITEM_BITMAP_WIDTH, CLOCK_GUI_TEMP_ITEM_BITMAP_HEIGHT, GRAPHICS_FORMAT_BW_1_V);
DECLARE_NOANIM_BITMAP_DATA(ind_temp_dot, CLOCK_GUI_TEMP_SEP_BITMAP_WIDTH, CLOCK_GUI_TEMP_SEP_BITMAP_HEIGHT, GRAPHICS_FORMAT_BW_1_V);
DECLARE_ANIM_BITMAP_DATA_COUNT(ind_temp_frac, CLOCK_GUI_DIGIT_ANIM_ITEMS, CLOCK_GUI_TEMP_ITEM_BITMAP_WIDTH, CLOCK_GUI_TEMP_ITEM_BITMAP_HEIGHT, GRAPHICS_FORMAT_BW_1_V);

DECLARE_ANIM_BITMAP_DATA_COUNT(year_first_digit, CLOCK_GUI_DIGIT_ANIM_ITEMS, CLOCK_GUI_TEMP_ITEM_BITMAP_WIDTH, CLOCK_GUI_TEMP_ITEM_BITMAP_HEIGHT, GRAPHICS_FORMAT_BW_1_V);
DECLARE_ANIM_BITMAP_DATA_COUNT(year_second_digit, CLOCK_GUI_DIGIT_ANIM_ITEMS, CLOCK_GUI_TEMP_ITEM_BITMAP_WIDTH, CLOCK_GUI_TEMP_ITEM_BITMAP_HEIGHT, GRAPHICS_FORMAT_BW_1_V);
DECLARE_ANIM_BITMAP_DATA_COUNT(year_third_digit, CLOCK_GUI_DIGIT_ANIM_ITEMS, CLOCK_GUI_TEMP_ITEM_BITMAP_WIDTH, CLOCK_GUI_TEMP_ITEM_BITMAP_HEIGHT, GRAPHICS_FORMAT_BW_1_V);
DECLARE_ANIM_BITMAP_DATA_COUNT(year_fourth_digit, CLOCK_GUI_DIGIT_ANIM_ITEMS, CLOCK_GUI_TEMP_ITEM_BITMAP_WIDTH, CLOCK_GUI_TEMP_ITEM_BITMAP_HEIGHT, GRAPHICS_FORMAT_BW_1_V);

typedef struct _YearGui{
	  gui_anim_bitmap_t year_first_digit;
    gui_anim_bitmap_t year_second_digit;
	  gui_anim_bitmap_t year_third_digit;
    gui_anim_bitmap_t year_fourth_digit;
} year_gui_t;

typedef struct _Date_Gui {
    gui_anim_bitmap_t monthday_tens;
    gui_anim_bitmap_t monthday_ones;
    gui_anim_bitmap_t month_first;
    gui_anim_bitmap_t month_second;
    gui_anim_bitmap_t month_third;
    gui_anim_bitmap_t weekday_tens;
    gui_anim_bitmap_t weekday_ones;
} date_gui_t;

typedef struct _Time_Gui {
    gui_anim_bitmap_t hours_tens;
    gui_anim_bitmap_t hours_ones;
    gui_anim_bitmap_t hours_mins_sep;
    gui_anim_bitmap_t mins_tens;
    gui_anim_bitmap_t mins_ones;
} time_gui_t;

typedef struct _Temp_Gui {
    gui_anim_bitmap_t temp_sign;
    gui_anim_bitmap_t temp_tens;
    gui_anim_bitmap_t temp_ones;
    gui_anim_bitmap_t temp_dot;
    gui_anim_bitmap_t temp_fract;
} temp_gui_t;

typedef struct _Clock_Gui {
	  year_gui_t year;
    date_gui_t date;
    time_gui_t time;
    temp_gui_t ind_temp;
} clock_gui_t;


static clock_gui_t clock_gui;

static gui_widget_t root_widget;
static gui_widget_t date_monthday_widget;
static gui_widget_t date_month_widget;
static gui_widget_t date_weekday_widget;
static gui_widget_t time_widget;
static gui_widget_t temp_ind_widget;
static gui_widget_t year_widget;

static void clock_gui_init_anim_bitmap(gui_anim_bitmap_t* anim_bitmap, gui_t* gui, gui_widget_t* parent,
                                       graphics_t* anim_graphics, gui_anim_bitmap_item_t* anim_items, size_t anim_items_count,
                                       graphics_pos_t x, graphics_pos_t y, graphics_pos_t width, graphics_pos_t height)
{
    graphics_clear(anim_graphics);
    
    gui_anim_bitmap_init_parent(anim_bitmap, gui, parent);
    gui_anim_bitmap_set_bitmap(anim_bitmap, anim_graphics);
    
    if(anim_items != NULL){
        gui_anim_bitmap_set_anim_items(anim_bitmap, anim_items, anim_items_count);
    }
    
    gui_anim_bitmap_set_effect_add_type(anim_bitmap, GUI_ANIM_BITMAP_EFFECT_GRAVITY);
    gui_anim_bitmap_set_effect_del_type(anim_bitmap, GUI_ANIM_BITMAP_EFFECT_GRAVITY);
    
    gui_anim_bitmap_set_max_steps(anim_bitmap, 5);
    
    gui_anim_bitmap_set_margin(anim_bitmap, CLOCK_GUI_ANIM_ITEM_MARGIN);
    
    gui_widget_move(GUI_WIDGET(anim_bitmap), x, y);
    gui_widget_resize(GUI_WIDGET(anim_bitmap), width, height);
    
    //gui_widget_set_border(GUI_WIDGET(anim_bitmap), GUI_BORDER_SOLID);
    //gui_widget_set_back_color(GUI_WIDGET(anim_bitmap), gui->theme->back_color);
    gui_widget_set_visible(GUI_WIDGET(anim_bitmap), true);
}

#define DATE_ITEM_WIDTH 8
#define DATE_ITEM_HEIGHT 8
#define DATE_DIGIT_WIDTH (CLOCK_GUI_TIME_ITEM_BITMAP_WIDTH * DATE_ITEM_WIDTH)
#define DATE_DIGIT_HEIGHT (CLOCK_GUI_TIME_ITEM_BITMAP_HEIGHT * DATE_ITEM_HEIGHT)
#define DATE_SPACE_WIDTH 2
#define DATE_SPACE_HEIGHT 2

#define DATE_WIDGET_WIDTH (DATE_DIGIT_WIDTH * 2 + DATE_SPACE_WIDTH * 1)
#define DATE_WIDGET_HEIGHT DATE_DIGIT_HEIGHT

#define MONTH_WIDGET_WIDTH (DATE_DIGIT_WIDTH * 3 + DATE_SPACE_WIDTH * 2)
#define MONTH_WIDGET_HEIGHT DATE_DIGIT_HEIGHT

#define DATE_MONTHDAY_LEFT 2
#define DATE_MONTHDAY_TOP 2

#define DATE_MONTH_LEFT ((320 - MONTH_WIDGET_WIDTH) / 2)
#define DATE_MONTH_TOP 2

#define DATE_WEEKDAY_LEFT (320 - DATE_MONTHDAY_LEFT - DATE_WIDGET_WIDTH)
#define DATE_WEEKDAY_TOP 2

static void clock_gui_init_date(gui_t* gui)
{
    clock_gui_init_anim_bitmap(&clock_gui.date.monthday_tens, gui, &date_monthday_widget, &monthday_tens_graphics, monthday_tens_items, CLOCK_GUI_DIGIT_ANIM_ITEMS,
                               0, 0, DATE_DIGIT_WIDTH, DATE_DIGIT_HEIGHT);
    clock_gui_init_anim_bitmap(&clock_gui.date.monthday_ones, gui, &date_monthday_widget, &monthday_ones_graphics, monthday_ones_items, CLOCK_GUI_DIGIT_ANIM_ITEMS,
                               DATE_DIGIT_WIDTH + DATE_SPACE_WIDTH, 0, DATE_DIGIT_WIDTH, DATE_DIGIT_HEIGHT);
    
    clock_gui_init_anim_bitmap(&clock_gui.date.month_first, gui, &date_month_widget, &month_first_graphics, month_first_items, CLOCK_GUI_DIGIT_ANIM_ITEMS,
                               0, 0, DATE_DIGIT_WIDTH, DATE_DIGIT_HEIGHT);
    clock_gui_init_anim_bitmap(&clock_gui.date.month_second, gui, &date_month_widget, &month_second_graphics, month_second_items, CLOCK_GUI_DIGIT_ANIM_ITEMS,
                               DATE_DIGIT_WIDTH + DATE_SPACE_WIDTH, 0, DATE_DIGIT_WIDTH, DATE_DIGIT_HEIGHT);
    clock_gui_init_anim_bitmap(&clock_gui.date.month_third, gui, &date_month_widget, &month_third_graphics, month_third_items, CLOCK_GUI_DIGIT_ANIM_ITEMS,
                               DATE_DIGIT_WIDTH * 2 + DATE_SPACE_WIDTH * 2, 0, DATE_DIGIT_WIDTH, DATE_DIGIT_HEIGHT);
    
    clock_gui_init_anim_bitmap(&clock_gui.date.weekday_tens, gui, &date_weekday_widget, &weekday_tens_graphics, weekday_tens_items, CLOCK_GUI_DIGIT_ANIM_ITEMS,
                               0, 0, DATE_DIGIT_WIDTH, DATE_DIGIT_HEIGHT);
    clock_gui_init_anim_bitmap(&clock_gui.date.weekday_ones, gui, &date_weekday_widget, &weekday_ones_graphics, weekday_ones_items, CLOCK_GUI_DIGIT_ANIM_ITEMS,
                               DATE_DIGIT_WIDTH + DATE_SPACE_WIDTH, 0, DATE_DIGIT_WIDTH, DATE_DIGIT_HEIGHT);
}

#define CLOCK_ITEM_WIDTH 14
#define CLOCK_ITEM_HEIGHT 14
#define CLOCK_DIGIT_WIDTH (CLOCK_GUI_TIME_ITEM_BITMAP_WIDTH * CLOCK_ITEM_WIDTH)
#define CLOCK_DIGIT_HEIGHT (CLOCK_GUI_TIME_ITEM_BITMAP_HEIGHT * CLOCK_ITEM_HEIGHT)
#define CLOCK_SEP_WIDTH (CLOCK_GUI_TIME_SEP_BITMAP_WIDTH * CLOCK_ITEM_WIDTH)
#define CLOCK_SEP_HEIGHT (CLOCK_GUI_TIME_SEP_BITMAP_HEIGHT * CLOCK_ITEM_HEIGHT)
#define CLOCK_SEP_SPACE_WIDTH (CLOCK_SEP_WIDTH * 2 / 3)
#define CLOCK_SPACE_WIDTH 2
#define CLOCK_SPACE_HEIGHT 16

#define CLOCK_LEFT 2
#define CLOCK_TOP (DATE_WEEKDAY_TOP + DATE_DIGIT_HEIGHT + CLOCK_SPACE_HEIGHT)

#define CLOCK_TIME_WIDGET_WIDTH (CLOCK_DIGIT_WIDTH * 3 + CLOCK_SEP_WIDTH + CLOCK_SPACE_WIDTH * 2 + CLOCK_SEP_SPACE_WIDTH * 2 + CLOCK_DIGIT_WIDTH)
#define CLOCK_TIME_WIDGET_HEIGHT CLOCK_DIGIT_HEIGHT

static void clock_gui_init_time(gui_t* gui)
{
    clock_gui_init_anim_bitmap(&clock_gui.time.hours_tens, gui, &time_widget, &hours_tens_graphics, hours_tens_items, CLOCK_GUI_DIGIT_ANIM_ITEMS,
                               0, 0, CLOCK_DIGIT_WIDTH, CLOCK_DIGIT_HEIGHT);
    clock_gui_init_anim_bitmap(&clock_gui.time.hours_ones, gui, &time_widget, &hours_ones_graphics, hours_ones_items, CLOCK_GUI_DIGIT_ANIM_ITEMS,
                               CLOCK_DIGIT_WIDTH + CLOCK_SPACE_WIDTH, 0, CLOCK_DIGIT_WIDTH, CLOCK_DIGIT_HEIGHT);
    clock_gui_init_anim_bitmap(&clock_gui.time.hours_mins_sep, gui, &time_widget, &hours_mins_sep_graphics, hours_mins_sep_items, CLOCK_GUI_TIME_SEP_ANIM_ITEMS,
                               CLOCK_DIGIT_WIDTH * 2 + CLOCK_SPACE_WIDTH + CLOCK_SEP_SPACE_WIDTH,
                               0, CLOCK_SEP_WIDTH, CLOCK_SEP_HEIGHT);
    clock_gui_init_anim_bitmap(&clock_gui.time.mins_tens, gui, &time_widget, &mins_tens_graphics, mins_tens_items, CLOCK_GUI_DIGIT_ANIM_ITEMS,
                               CLOCK_DIGIT_WIDTH * 2 + CLOCK_SEP_WIDTH + CLOCK_SPACE_WIDTH + CLOCK_SEP_SPACE_WIDTH * 2,
                               0, CLOCK_DIGIT_WIDTH, CLOCK_DIGIT_HEIGHT);
    clock_gui_init_anim_bitmap(&clock_gui.time.mins_ones, gui, &time_widget, &mins_ones_graphics, mins_ones_items, CLOCK_GUI_DIGIT_ANIM_ITEMS,
                               CLOCK_DIGIT_WIDTH * 3 + CLOCK_SEP_WIDTH + CLOCK_SPACE_WIDTH * 2 + CLOCK_SEP_SPACE_WIDTH * 2,
                               0, CLOCK_DIGIT_WIDTH, CLOCK_DIGIT_HEIGHT);
}

#define TEMP_ITEM_WIDTH 7
#define TEMP_ITEM_HEIGHT 7
#define TEMP_DIGIT_WIDTH (CLOCK_GUI_TIME_ITEM_BITMAP_WIDTH * TEMP_ITEM_WIDTH)
#define TEMP_DIGIT_HEIGHT (CLOCK_GUI_TIME_ITEM_BITMAP_HEIGHT * TEMP_ITEM_HEIGHT)
#define TEMP_SIGN_WIDTH (CLOCK_GUI_TEMP_SIGN_BITMAP_WIDTH * TEMP_ITEM_WIDTH)
#define TEMP_SIGN_HEIGHT (CLOCK_GUI_TEMP_SIGN_BITMAP_HEIGHT * TEMP_ITEM_HEIGHT)
#define TEMP_DOT_WIDTH (CLOCK_GUI_TEMP_SEP_BITMAP_WIDTH * TEMP_ITEM_WIDTH)
#define TEMP_DOT_HEIGHT (CLOCK_GUI_TEMP_SEP_BITMAP_HEIGHT * TEMP_ITEM_HEIGHT)
#define TEMP_DOT_SPACE_WIDTH (TEMP_DOT_WIDTH * 2 / 3)
#define TEMP_SPACE_WIDTH 2
#define TEMP_SPACE_HEIGHT 16

#define TEMP_IND_LEFT 172//2
#define TEMP_IND_TOP (CLOCK_TOP + CLOCK_DIGIT_HEIGHT + TEMP_SPACE_HEIGHT)
#define YEAR_LEFT 2//170
#define YEAR_TOP TEMP_IND_TOP

#define CLOCK_TEMP_WIDGET_WIDTH (TEMP_IND_LEFT + TEMP_SIGN_WIDTH + TEMP_DIGIT_WIDTH * 2 + TEMP_DOT_WIDTH + TEMP_SPACE_WIDTH * 2 + TEMP_DOT_SPACE_WIDTH * 2 + TEMP_DIGIT_WIDTH)
#define CLOCK_TEMP_WIDGET_HEIGHT TEMP_DIGIT_HEIGHT

static void clock_gui_init_temp_ind(gui_t* gui)
{
    clock_gui_init_anim_bitmap(&clock_gui.ind_temp.temp_sign, gui, &temp_ind_widget, &ind_temp_sign_graphics, ind_temp_sign_items, CLOCK_GUI_TEMP_SIGN_ANIM_ITEMS,
                               0, 0, TEMP_SIGN_WIDTH, TEMP_SIGN_HEIGHT);
    clock_gui_init_anim_bitmap(&clock_gui.ind_temp.temp_tens, gui, &temp_ind_widget, &ind_temp_tens_graphics, ind_temp_tens_items, CLOCK_GUI_DIGIT_ANIM_ITEMS,
                               TEMP_SIGN_WIDTH + TEMP_SPACE_WIDTH,
                               0, TEMP_DIGIT_WIDTH, TEMP_DIGIT_HEIGHT);
    clock_gui_init_anim_bitmap(&clock_gui.ind_temp.temp_ones, gui, &temp_ind_widget, &ind_temp_ones_graphics, ind_temp_ones_items, CLOCK_GUI_DIGIT_ANIM_ITEMS,
                               TEMP_SIGN_WIDTH + TEMP_DIGIT_WIDTH + TEMP_SPACE_WIDTH * 2,
                               0, TEMP_DIGIT_WIDTH, TEMP_DIGIT_HEIGHT);
    clock_gui_init_anim_bitmap(&clock_gui.ind_temp.temp_dot, gui, &temp_ind_widget, &ind_temp_dot_graphics, NULL, 0,//ind_temp_dot_items,
                               TEMP_SIGN_WIDTH + TEMP_DIGIT_WIDTH * 2 + TEMP_SPACE_WIDTH * 2 + TEMP_DOT_SPACE_WIDTH,
                               0, TEMP_DOT_WIDTH, TEMP_DOT_HEIGHT);
    clock_gui_init_anim_bitmap(&clock_gui.ind_temp.temp_fract, gui, &temp_ind_widget, &ind_temp_frac_graphics, ind_temp_frac_items, CLOCK_GUI_DIGIT_ANIM_ITEMS,
                               TEMP_SIGN_WIDTH + TEMP_DIGIT_WIDTH * 2 + TEMP_DOT_WIDTH + TEMP_SPACE_WIDTH * 2 + TEMP_DOT_SPACE_WIDTH * 2,
                               0, TEMP_DIGIT_WIDTH, TEMP_DIGIT_HEIGHT);
}

void clock_gui_init_year(gui_t* gui)
{
    clock_gui_init_anim_bitmap(&clock_gui.year.year_first_digit, gui, &year_widget, &year_first_digit_graphics, year_first_digit_items, CLOCK_GUI_DIGIT_ANIM_ITEMS,
                               0, 0, TEMP_DIGIT_WIDTH, TEMP_DIGIT_HEIGHT); 
    clock_gui_init_anim_bitmap(&clock_gui.year.year_second_digit, gui, &year_widget, &year_second_digit_graphics, year_second_digit_items, CLOCK_GUI_DIGIT_ANIM_ITEMS,
                               TEMP_DIGIT_WIDTH + TEMP_SPACE_WIDTH, 0, TEMP_DIGIT_WIDTH, TEMP_DIGIT_HEIGHT);
    clock_gui_init_anim_bitmap(&clock_gui.year.year_third_digit, gui, &year_widget, &year_third_digit_graphics, year_third_digit_items, CLOCK_GUI_DIGIT_ANIM_ITEMS,
                               TEMP_DIGIT_WIDTH * 2 + TEMP_SPACE_WIDTH, 0, TEMP_DIGIT_WIDTH, TEMP_DIGIT_HEIGHT);
    clock_gui_init_anim_bitmap(&clock_gui.year.year_fourth_digit, gui, &year_widget, &year_fourth_digit_graphics, year_fourth_digit_items, CLOCK_GUI_DIGIT_ANIM_ITEMS,
                               TEMP_DIGIT_WIDTH * 3 + TEMP_SPACE_WIDTH, 0, TEMP_DIGIT_WIDTH, TEMP_DIGIT_HEIGHT); 
}

err_t clock_gui_init(gui_t* gui)
{
    if(gui == NULL) return E_NULL_POINTER;
    
    gui_widget_init(&root_widget, gui);
    gui_widget_move(&root_widget, 0, 0);
    gui_widget_resize(&root_widget, CLOCK_GUI_WIDTH, CLOCK_GUI_HEIGHT);
    
    gui_widget_init_parent(&date_monthday_widget, gui, &root_widget);
    gui_widget_move(&date_monthday_widget, DATE_MONTHDAY_LEFT, DATE_MONTHDAY_TOP);
    gui_widget_resize(&date_monthday_widget, DATE_WIDGET_WIDTH, DATE_WIDGET_HEIGHT);
    gui_widget_set_visible(&date_monthday_widget, true);
    
    gui_widget_init_parent(&date_month_widget, gui, &root_widget);
    gui_widget_move(&date_month_widget, DATE_MONTH_LEFT, DATE_MONTH_TOP);
    gui_widget_resize(&date_month_widget, MONTH_WIDGET_WIDTH, MONTH_WIDGET_HEIGHT);
    gui_widget_set_visible(&date_month_widget, true);
    
    gui_widget_init_parent(&date_weekday_widget, gui, &root_widget);
    gui_widget_move(&date_weekday_widget, DATE_WEEKDAY_LEFT, DATE_WEEKDAY_TOP);
    gui_widget_resize(&date_weekday_widget, DATE_WIDGET_WIDTH, DATE_WIDGET_HEIGHT);
    gui_widget_set_visible(&date_weekday_widget, true);
    
    gui_widget_init_parent(&time_widget, gui, &root_widget);
    gui_widget_move(&time_widget, CLOCK_LEFT, CLOCK_TOP);
    gui_widget_resize(&time_widget, CLOCK_TIME_WIDGET_WIDTH, CLOCK_TIME_WIDGET_HEIGHT);
    gui_widget_set_visible(&time_widget, true);
    
    gui_widget_init_parent(&temp_ind_widget, gui, &root_widget);
    gui_widget_move(&temp_ind_widget, TEMP_IND_LEFT, TEMP_IND_TOP);
    gui_widget_resize(&temp_ind_widget, CLOCK_TEMP_WIDGET_WIDTH, CLOCK_TEMP_WIDGET_HEIGHT);
    gui_widget_set_visible(&temp_ind_widget, true);
    
    gui_widget_init_parent(&year_widget, gui, &root_widget);
    gui_widget_move(&year_widget, YEAR_LEFT, YEAR_TOP);
    gui_widget_resize(&year_widget, CLOCK_TEMP_WIDGET_WIDTH, CLOCK_TEMP_WIDGET_HEIGHT);
    gui_widget_set_visible(&year_widget, true);
    
    clock_gui_init_date(gui);
    clock_gui_init_time(gui);
    clock_gui_init_temp_ind(gui);
		clock_gui_init_year(gui);
    
    gui_set_root_widget(gui, &root_widget);
    gui_widget_set_visible(&root_widget, true);
    
    return E_NO_ERROR;
}

static void clock_gui_start_animation(gui_anim_bitmap_t* anim_bitmap, const graphics_t* bitmap)
{
    static gui_anim_bitmap_effect_t effects[] = {
        GUI_ANIM_BITMAP_EFFECT_GRAVITY,
        GUI_ANIM_BITMAP_EFFECT_MOVE,
        GUI_ANIM_BITMAP_EFFECT_RESIZE,
    };
    static gui_anim_bitmap_effect_t effect = 0;
    
    if(!gui_anim_bitmap_animation_done(anim_bitmap)){
        gui_anim_bitmap_animation_flush(anim_bitmap);
        gui_widget_repaint(GUI_WIDGET(anim_bitmap), NULL);
    }
    
    gui_anim_bitmap_set_effect_add_type(anim_bitmap, effects[effect]);
    gui_anim_bitmap_set_effect_del_type(anim_bitmap, effects[effect]);
    
    gui_anim_bitmap_set_target_bitmap(anim_bitmap, bitmap);
    // Если анимаця не запущена (нет анимированных элементов).
    if(gui_anim_bitmap_animation_start(anim_bitmap)){
        // Перерисуем обновлённый виджет.
        gui_widget_repaint(GUI_WIDGET(anim_bitmap), NULL);
    }
    
    if(++ effect > 2) effect = 0;
}

static void clock_gui_start_sep_animation(gui_anim_bitmap_t* anim_bitmap, const graphics_t* bitmap)
{
    if(!gui_anim_bitmap_animation_done(anim_bitmap)){
        gui_anim_bitmap_animation_flush(anim_bitmap);
        gui_widget_repaint(GUI_WIDGET(anim_bitmap), NULL);
    }
    
    gui_anim_bitmap_set_effect_add_type(anim_bitmap, GUI_ANIM_BITMAP_EFFECT_RESIZE);
    gui_anim_bitmap_set_effect_del_type(anim_bitmap, GUI_ANIM_BITMAP_EFFECT_RESIZE);
    
    gui_anim_bitmap_set_target_bitmap(anim_bitmap, bitmap);
    // Если анимаця не запущена (нет анимированных элементов).
    if(gui_anim_bitmap_animation_start(anim_bitmap)){
        // Перерисуем обновлённый виджет.
        gui_widget_repaint(GUI_WIDGET(anim_bitmap), NULL);
    }
}

void clock_gui_set_monthday(uint32_t monthday)
{
    while(monthday > 99) monthday -= 99;
    
    uint32_t monthday_tens = monthday / 10;
    uint32_t monthday_ones = monthday % 10;
    
    clock_gui_start_animation(&clock_gui.date.monthday_tens, &clock_gui_digits_bitmaps[monthday_tens]);
    clock_gui_start_animation(&clock_gui.date.monthday_ones, &clock_gui_digits_bitmaps[monthday_ones]);
}

void clock_gui_set_month(uint32_t month)
{
    while(month > 12) month -= 12;
    
    clock_gui_start_animation(&clock_gui.date.month_first, &clock_gui_month_name_chars_bitmaps[clock_gui_month_name_indexes[month][0]]);
    clock_gui_start_animation(&clock_gui.date.month_second, &clock_gui_month_name_chars_bitmaps[clock_gui_month_name_indexes[month][1]]);
    clock_gui_start_animation(&clock_gui.date.month_third, &clock_gui_month_name_chars_bitmaps[clock_gui_month_name_indexes[month][2]]);
}

void clock_gui_set_weekday(uint32_t weekday)
{
    while(weekday > 6) weekday -= 6;
    
    clock_gui_start_animation(&clock_gui.date.weekday_tens, &clock_gui_weekday_chars_bitmaps[clock_gui_weekday_indexes[weekday][0]]);
    clock_gui_start_animation(&clock_gui.date.weekday_ones, &clock_gui_weekday_chars_bitmaps[clock_gui_weekday_indexes[weekday][1]]);
}

void clock_gui_set_time(uint32_t hours, uint32_t mins)
{
    while(hours > 99) hours -= 99;
    while(mins > 99) mins -= 99;
    
    uint32_t hours_ones = hours % 10;
    uint32_t hours_tens = hours / 10;
    uint32_t mins_ones = mins % 10;
    uint32_t mins_tens = mins / 10;
    
    clock_gui_start_animation(&clock_gui.time.hours_tens, &clock_gui_digits_bitmaps[hours_tens]);
    clock_gui_start_animation(&clock_gui.time.hours_ones, &clock_gui_digits_bitmaps[hours_ones]);
    clock_gui_start_animation(&clock_gui.time.mins_tens, &clock_gui_digits_bitmaps[mins_tens]);
    clock_gui_start_animation(&clock_gui.time.mins_ones, &clock_gui_digits_bitmaps[mins_ones]);
}

void clock_gui_set_temp_impl(temp_gui_t* temp_gui, fixed16_t temp)
{
    bool temp_neg = temp < 0;
    
    temp = fixed_abs(temp);
    
    uint32_t temp_int = fixed16_get_int(temp);
    uint32_t temp_fract = fixed16_get_fract_by_denom(temp, 10);
    
    if(temp_int > 99) temp_int = 99;
    
    uint32_t temp_tens = temp_int / 10;
    uint32_t temp_ones = temp_int % 10;
    
    clock_gui_start_sep_animation(&temp_gui->temp_sign,
            temp_neg ? &clock_gui_temp_sign_minus_bitmap : &clock_gui_temp_sign_plus_bitmap);
    clock_gui_start_animation(&temp_gui->temp_tens, &clock_gui_digits_bitmaps[temp_tens]);
    clock_gui_start_animation(&temp_gui->temp_ones, &clock_gui_digits_bitmaps[temp_ones]);
    clock_gui_start_sep_animation(&temp_gui->temp_dot, &clock_gui_temp_dot_bitmap);
    clock_gui_start_animation(&temp_gui->temp_fract, &clock_gui_digits_bitmaps[temp_fract]);
} 


void clock_gui_set_temp_ind(fixed16_t temp)
{
    clock_gui_set_temp_impl(&clock_gui.ind_temp, temp);
} 

void clock_gui_toggle_time_sep(void)
{
    const graphics_t* bitmap = NULL;
    if(gui_anim_bitmap_target_bitmap(&clock_gui.time.hours_mins_sep) == &clock_gui_clock_empty_sep_bitmap){
        bitmap = &clock_gui_clock_sep_bitmap;
    }else{
        bitmap = &clock_gui_clock_empty_sep_bitmap;
    }
    clock_gui_start_sep_animation(&clock_gui.time.hours_mins_sep, bitmap);
}

static bool clock_gui_animation_step_impl(gui_anim_bitmap_t* anim_bitmap)
{
    if(!gui_anim_bitmap_animation_done(anim_bitmap)){
        gui_anim_bitmap_animation_step(anim_bitmap);
        gui_widget_repaint(GUI_WIDGET(anim_bitmap), NULL);
    }
    return gui_anim_bitmap_animation_done(anim_bitmap);
}

bool clock_gui_animation_step(void)
{
    bool res = true;
    
    res &= clock_gui_animation_step_impl(&clock_gui.date.monthday_tens);
    res &= clock_gui_animation_step_impl(&clock_gui.date.monthday_ones);
    res &= clock_gui_animation_step_impl(&clock_gui.date.month_first);
    res &= clock_gui_animation_step_impl(&clock_gui.date.month_second);
    res &= clock_gui_animation_step_impl(&clock_gui.date.month_third);
    res &= clock_gui_animation_step_impl(&clock_gui.date.weekday_tens);
    res &= clock_gui_animation_step_impl(&clock_gui.date.weekday_ones);
	      
    res &= clock_gui_animation_step_impl(&clock_gui.time.hours_tens);
    res &= clock_gui_animation_step_impl(&clock_gui.time.hours_ones);
    res &= clock_gui_animation_step_impl(&clock_gui.time.hours_mins_sep);
    res &= clock_gui_animation_step_impl(&clock_gui.time.mins_tens);
    res &= clock_gui_animation_step_impl(&clock_gui.time.mins_ones);
    
    res &= clock_gui_animation_step_impl(&clock_gui.ind_temp.temp_sign);
    res &= clock_gui_animation_step_impl(&clock_gui.ind_temp.temp_tens);
    res &= clock_gui_animation_step_impl(&clock_gui.ind_temp.temp_ones);
    res &= clock_gui_animation_step_impl(&clock_gui.ind_temp.temp_dot);
    res &= clock_gui_animation_step_impl(&clock_gui.ind_temp.temp_fract);
    
		/* Year */
    res &= clock_gui_animation_step_impl(&clock_gui.year.year_first_digit);
    res &= clock_gui_animation_step_impl(&clock_gui.year.year_second_digit);
    res &= clock_gui_animation_step_impl(&clock_gui.year.year_third_digit);
    res &= clock_gui_animation_step_impl(&clock_gui.year.year_fourth_digit);
        
    return res;
}

void clock_gui_set_back_color(graphics_color_t color)
{
    gui_widget_set_visible(&root_widget, false);
    
    gui_widget_set_back_color(&root_widget, color);
    gui_widget_set_back_color(&date_monthday_widget, color);
    gui_widget_set_back_color(&date_month_widget, color);
    gui_widget_set_back_color(&date_weekday_widget, color);
    gui_widget_set_back_color(&time_widget, color);
    gui_widget_set_back_color(&temp_ind_widget, color);
    gui_widget_set_back_color(&year_widget, color);
    
    gui_widget_set_back_color(GUI_WIDGET(&clock_gui.date.monthday_tens), color);
    gui_widget_set_back_color(GUI_WIDGET(&clock_gui.date.monthday_ones), color);
    gui_widget_set_back_color(GUI_WIDGET(&clock_gui.date.month_first), color);
    gui_widget_set_back_color(GUI_WIDGET(&clock_gui.date.month_second), color);
    gui_widget_set_back_color(GUI_WIDGET(&clock_gui.date.month_third), color);
    gui_widget_set_back_color(GUI_WIDGET(&clock_gui.date.weekday_tens), color);
    gui_widget_set_back_color(GUI_WIDGET(&clock_gui.date.weekday_ones), color);
    
    gui_widget_set_back_color(GUI_WIDGET(&clock_gui.time.hours_tens), color);
    gui_widget_set_back_color(GUI_WIDGET(&clock_gui.time.hours_ones), color);
    gui_widget_set_back_color(GUI_WIDGET(&clock_gui.time.hours_mins_sep), color);
    gui_widget_set_back_color(GUI_WIDGET(&clock_gui.time.mins_tens), color);
    gui_widget_set_back_color(GUI_WIDGET(&clock_gui.time.mins_ones), color);
    
    gui_widget_set_back_color(GUI_WIDGET(&clock_gui.ind_temp.temp_sign), color);
    gui_widget_set_back_color(GUI_WIDGET(&clock_gui.ind_temp.temp_tens), color);
    gui_widget_set_back_color(GUI_WIDGET(&clock_gui.ind_temp.temp_ones), color);
    gui_widget_set_back_color(GUI_WIDGET(&clock_gui.ind_temp.temp_dot), color);
    gui_widget_set_back_color(GUI_WIDGET(&clock_gui.ind_temp.temp_fract), color);
    
    gui_widget_set_back_color(GUI_WIDGET(&clock_gui.year.year_first_digit), color);
    gui_widget_set_back_color(GUI_WIDGET(&clock_gui.year.year_second_digit), color);
    gui_widget_set_back_color(GUI_WIDGET(&clock_gui.year.year_third_digit), color);
    gui_widget_set_back_color(GUI_WIDGET(&clock_gui.year.year_fourth_digit), color);
    
    gui_widget_set_visible(&root_widget, true);
}

void clock_gui_set_monthday_color(graphics_color_t color)
{
    gui_widget_set_visible(&date_monthday_widget, false);
    
    gui_anim_bitmap_set_front_color(&clock_gui.date.monthday_tens, color);
    gui_anim_bitmap_set_front_color(&clock_gui.date.monthday_ones, color);
    
    gui_widget_set_visible(&date_monthday_widget, true);
}

void clock_gui_set_month_color(graphics_color_t color)
{
    gui_widget_set_visible(&date_month_widget, false);
    
    gui_anim_bitmap_set_front_color(&clock_gui.date.month_first, color);
    gui_anim_bitmap_set_front_color(&clock_gui.date.month_second, color);
    gui_anim_bitmap_set_front_color(&clock_gui.date.month_third, color);
    
    gui_widget_set_visible(&date_month_widget, true);
}

void clock_gui_set_weekday_color(graphics_color_t color)
{
    gui_widget_set_visible(&date_weekday_widget, false);
    
    gui_anim_bitmap_set_front_color(&clock_gui.date.weekday_tens, color);
    gui_anim_bitmap_set_front_color(&clock_gui.date.weekday_ones, color);
    
    gui_widget_set_visible(&date_weekday_widget, true);
}

void clock_gui_set_time_color(graphics_color_t color)
{
    gui_widget_set_visible(&time_widget, false);
    
    gui_anim_bitmap_set_front_color(&clock_gui.time.hours_tens, color);
    gui_anim_bitmap_set_front_color(&clock_gui.time.hours_ones, color);
    gui_anim_bitmap_set_front_color(&clock_gui.time.hours_mins_sep, color);
    gui_anim_bitmap_set_front_color(&clock_gui.time.mins_tens, color);
    gui_anim_bitmap_set_front_color(&clock_gui.time.mins_ones, color);
    
    gui_widget_set_visible(&time_widget, true);
}

void clock_gui_set_time_hour_color(graphics_color_t color)
{
    gui_widget_set_visible(&time_widget, false);
    
    gui_anim_bitmap_set_front_color(&clock_gui.time.hours_tens, color);
    gui_anim_bitmap_set_front_color(&clock_gui.time.hours_ones, color);
    
    gui_widget_set_visible(&time_widget, true);
}

void clock_gui_set_time_mins_color(graphics_color_t color)
{
    gui_widget_set_visible(&time_widget, false);
    
    gui_anim_bitmap_set_front_color(&clock_gui.time.mins_tens, color);
    gui_anim_bitmap_set_front_color(&clock_gui.time.mins_ones, color);
    
    gui_widget_set_visible(&time_widget, true);
}

void clock_gui_set_temp_ind_color(graphics_color_t color)
{
    gui_widget_set_visible(&temp_ind_widget, false);
    
    gui_anim_bitmap_set_front_color(&clock_gui.ind_temp.temp_sign, color);
    gui_anim_bitmap_set_front_color(&clock_gui.ind_temp.temp_tens, color);
    gui_anim_bitmap_set_front_color(&clock_gui.ind_temp.temp_ones, color);
    gui_anim_bitmap_set_front_color(&clock_gui.ind_temp.temp_dot, color);
    gui_anim_bitmap_set_front_color(&clock_gui.ind_temp.temp_fract, color);
    
    gui_widget_set_visible(&temp_ind_widget, true);
}

void clock_gui_set_year_color(graphics_color_t color)
{
    gui_widget_set_visible(&year_widget, false);
    
    gui_anim_bitmap_set_front_color(&clock_gui.year.year_first_digit, color);
    gui_anim_bitmap_set_front_color(&clock_gui.year.year_second_digit, color);
    gui_anim_bitmap_set_front_color(&clock_gui.year.year_third_digit, color);
    gui_anim_bitmap_set_front_color(&clock_gui.year.year_fourth_digit, color);
        
    gui_widget_set_visible(&year_widget, true);
}

/* взять из 4х значного числа по одной цифре */
void clock_gui_set_year_impl(year_gui_t* year_gui, uint32_t year)
{   
	  /* взять из 4х значного числа по одной цифре для каждого фрактала */	
    uint8_t temp_first = year / 1000;
    uint8_t temp_second = (year / 100) % 10;
	  uint8_t temp_third = (year % 100) / 10;
	  uint8_t temp_fourth = year % 10;
	
    clock_gui_start_animation(&clock_gui.year.year_first_digit, &clock_gui_digits_bitmaps[temp_first]);
    clock_gui_start_animation(&clock_gui.year.year_second_digit, &clock_gui_digits_bitmaps[temp_second]);
	  clock_gui_start_animation(&clock_gui.year.year_third_digit, &clock_gui_digits_bitmaps[temp_third]);
    clock_gui_start_animation(&clock_gui.year.year_fourth_digit, &clock_gui_digits_bitmaps[temp_fourth]);
}

void clock_gui_set_year(uint32_t year)
{   
	 clock_gui_set_year_impl(&clock_gui.year, year);
}
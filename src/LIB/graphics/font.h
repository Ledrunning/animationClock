/**
 * @file font.h
 * Библиотека для работы со шрифтами в виде битовых карт.
 */
#ifndef FONT_H
#define	FONT_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "graphics.h"
#include "defs/defs.h"
#include "errors/errors.h"
#include "rect.h"
#include "point.h"

// ютф8!

//! Тип символа.
typedef uint32_t font_char_t;

/**
 * Тип описания символа не моноширинного шрифта.
 * Число дескрипторов обязано совпадать
 * с числом символов в битовой карте.
 */
typedef struct _FontCharDescr {
    uint16_t x; //!< Координата X символа в битовой карте.
    uint16_t y; //!< Координата Y символа в битовой карте.
    uint8_t width; //!< Ширина символа.
    uint8_t height; //!< Высота символа.
    uint8_t offset_x; //!< Смещение символа по X.
    uint8_t offset_y; //!< Смещение символа по Y.
} font_char_descr_t;

/**
 * Структура битовой карты шрифта.
 */
typedef struct _FontBitmap {
    //! Код первого символа в битовой карте.
    font_char_t first_char;
    //! Код последнего символа в битовой карте.
    font_char_t last_char;
    //! Битовая карта.
    const graphics_t graphics;
    //! Таблица дескрипторов символов.
    const font_char_descr_t* char_descrs;
} font_bitmap_t;

/**
 * Структура шрифта.
 */
typedef struct _Font {
    //! Ширина символа (моноширинный шрифт, табуляция).
    graphics_size_t char_width;
    //! Высота символа (моноширинный шрифт, перенос строки).
    graphics_size_t char_height;
    //! Число битовых карт.
    size_t bitmaps_count;
    /**
     * Битовые карты.
     * Должны быть упорядочены по кодам символов.
     */
    const font_bitmap_t* bitmaps;
    //! Расстояние между символами в тексте по горизонтали.
    graphics_pos_t hspace;
    //! Расстояние между символами в тексте по вертикали.
    graphics_pos_t vspace;
    //! Символ по-умолчанию для отсутствующих символов.
    font_char_t default_char;
} font_t;


//! Размер табуляции.
#define FONT_TAB_SIZE 4


/**
 * Заполняет структуру битовой маски шрифта по месту объявления.
 */
#define make_font_bitmap(arg_first_char, arg_last_char, arg_data, arg_width, arg_height, arg_format)\
                        {.first_char = arg_first_char, .last_char = arg_last_char,\
                         .graphics = make_graphics(arg_data, arg_width, arg_height, arg_format),\
                         .char_descrs = NULL}

/**
 * Заполняет структуру битовой маски шрифта с дескрипторами по месту объявления.
 */
#define make_font_bitmap_descrs(arg_first_char, arg_last_char, arg_data, arg_width, arg_height, arg_format, arg_descrs)\
                               {.first_char = arg_first_char, .last_char = arg_last_char,\
                                .graphics = make_graphics(arg_data, arg_width, arg_height, arg_format),\
                                .char_descrs = arg_descrs}

/**
 * Инициализирует битовую карту шрифта.
 * @param font_bitmap Битовая карта шрифта.
 * @param first_char Начальный символ.
 * @param last_char Конечный символ.
 * @param data Графические данные.
 * @param width Ширина битовой карты.
 * @param height Высота битовой карты.
 * @param format Формат битовой карты.
 * @param char_descrs Дескрипторы символов.
 * @return Код ошибки.
 */
EXTERN err_t font_bitmap_init(font_bitmap_t* font_bitmap, font_char_t first_char, font_char_t last_char,
                                const uint8_t* data, graphics_size_t width, graphics_size_t height,
                                graphics_format_t format, const font_char_descr_t* char_descrs);

/**
 * Получает начальный символ битовой карты.
 */
static ALWAYS_INLINE font_char_t font_bitmap_first_char(const font_bitmap_t* font_bitmap)
{
    return font_bitmap->first_char;
}

/**
 * Получает конечный символ битовой карты.
 */
static ALWAYS_INLINE font_char_t font_bitmap_last_char(const font_bitmap_t* font_bitmap)
{
    return font_bitmap->last_char;
}

/**
 * Получает графические данные битовой карты.
 */
static ALWAYS_INLINE const graphics_t* font_bitmap_graphics(const font_bitmap_t* font_bitmap)
{
    return &font_bitmap->graphics;
}

/**
 * Заполняет структуру шрифта по месту объявления.
 */
#define make_font(arg_bitmaps, arg_bitmaps_count, arg_char_width, arg_char_height, arg_hspace, arg_vspace)\
                 {.bitmaps = arg_bitmaps, .bitmaps_count = arg_bitmaps_count, .char_width = arg_char_width,\
                  .char_height = arg_char_height, .hspace = arg_hspace, .vspace = arg_vspace, .default_char = 0}

/**
 * Заполняет структуру шрифта по месту объявления с указанием символа по-умолчанию.
 */
#define make_font_defchar(arg_bitmaps, arg_bitmaps_count, arg_char_width, arg_char_height, arg_hspace, arg_vspace, arg_def_char)\
                         {.bitmaps = arg_bitmaps, .bitmaps_count = arg_bitmaps_count, .char_width = arg_char_width,\
                          .char_height = arg_char_height, .hspace = arg_hspace, .vspace = arg_vspace, .default_char = arg_def_char}

/**
 * Инициализирует шрифт.
 * @param font Шрифт.
 * @param bitmaps Битовые карты.
 * Должны быть упорядочены по кодам символов.
 * @param bitmaps_count Число битовых карт.
 * @param char_width Ширина символа.
 * @param char_height Высота символа.
 * @param hspace Расстояние между символами в тексте по горизонтали.
 * @param vspace Расстояние между символами в тексте по вертикали.
 * @return Код ошибки.
 */
EXTERN err_t font_init(font_t* font, const font_bitmap_t* bitmaps, size_t bitmaps_count,
                        graphics_size_t char_width, graphics_size_t char_height,
                        graphics_pos_t hspace, graphics_pos_t vspace);

/**
 * Получает битовую карту, содержащую заданный символ.
 * @param font Шрифт.
 * @param c Символ.
 * @return Битовую карту, либо NULL, если
 * ни одна битовая карта не содержит заданный символ.
 */
EXTERN const font_bitmap_t* font_bitmap_by_char(const font_t* font, font_char_t c);

/**
 * Получает положение символа в битовой карте.
 * @param font Шрифт.
 * @param font_bitmap Битовая карта шрифта.
 * @param c Символ.
 * @param rect Прямоугольная область символа.
 * @param offset Смещение символа.
 * @return true в случае успеха, иначе false.
 */
EXTERN bool font_bitmap_get_char_position(const font_t* font, const font_bitmap_t* font_bitmap, font_char_t c, rect_t* rect, point_t* offset);

/**
 * Получает положение символа.
 * @param font Шрифт.
 * @param c Символ.
 * @param rect Прямоугольная область символа.
 * @param offset Смещение символа.
 * @return true в случае успеха, иначе false.
 */
EXTERN bool font_get_char_position(const font_t* font, font_char_t c, rect_t* rect, point_t* offset);

/**
 * Получает битовую карту и положение символа.
 * @param font Шрифт.
 * @param c Символ.
 * @param font_bitmap Битовая карта.
 * @param rect Прямоугольная область символа.
 * @param offset Смещение символа.
 * @return true в случае успеха, иначе false.
 */
EXTERN bool font_get_char_bitmap_position(const font_t* font, font_char_t c, const font_bitmap_t** font_bitmap, rect_t* rect, point_t* offset);

/**
 * Получает ширину символа шрифта.
 * @param font Шрифт.
 * @return Ширину символа.
 */
static ALWAYS_INLINE graphics_size_t font_char_width(const font_t* font)
{
    return font->char_width;
}

/**
 * Получает высоту символа шрифта.
 * @param font Шрифт.
 * @return Высоту символа.
 */
static ALWAYS_INLINE graphics_size_t font_char_height(const font_t* font)
{
    return font->char_height;
}

/**
 * Устанавливает расстояние между символами в тексте по горизонтали.
 * @param font Шрифт.
 * @param hspace Расстояние между символами в тексте по горизонтали.
 */
static ALWAYS_INLINE void font_set_hspace(font_t* font, graphics_pos_t hspace)
{
    font->hspace = hspace;
}

/**
 * Получает расстояние между символами в тексте по горизонтали.
 * @param font Шрифт.
 * @return Расстояние между символами в тексте по горизонтали.
 */
static ALWAYS_INLINE graphics_pos_t font_hspace(const font_t* font)
{
    return font->hspace;
}

/**
 * Устанавливает расстояние между символами в тексте по вертикали.
 * @param font Шрифт.
 * @param vspace Расстояние между символами в тексте по вертикали.
 */
static ALWAYS_INLINE void font_set_vspace(font_t* font, graphics_pos_t vspace)
{
    font->vspace = vspace;
}

/**
 * Получает расстояние между символами в тексте по вертикали.
 * @param font Шрифт.
 * @return Расстояние между символами в тексте по вертикали.
 */
static ALWAYS_INLINE graphics_pos_t font_vspace(const font_t* font)
{
    return font->vspace;
}

/**
 * Устанавливает символ по-умолчанию для отсутствующих символов.
 * @param font Шрифт.
 * @param def_char Символ по-умолчанию для отсутствующих символов.
 */
static ALWAYS_INLINE void font_set_default_char(font_t* font, font_char_t def_char)
{
    font->default_char = def_char;
}

/**
 * Получает символ по-умолчанию для отсутствующих символов.
 * @param font Шрифт.
 * @return Символ по-умолчанию для отсутствующих символов.
 */
static ALWAYS_INLINE font_char_t font_default_char(const font_t* font)
{
    return font->default_char;
}

/**
 * Получает размер символа utf8.
 * @param c Символ ютф8.
 * @return Размер символа utf8.
 */
EXTERN size_t font_utf8_size(const char* c);

/**
 * Декодирует символ utf8.
 * @param c Символ ютф8.
 * @param c_size Получаемый размер символа. Может быть NULL, если размер не нужен.
 * @return Декодированный символ utf8.
 */
EXTERN font_char_t font_utf8_decode(const char* c, size_t* c_size);

#endif	/* FONT_H */


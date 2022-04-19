/**
 * @file key_layout_ru.h Русская раскладка клавиатуры (UTF-8).
 */

#ifndef KEY_LAYOUT_RU_H
#define KEY_LAYOUT_RU_H

#include "key_input.h"

#ifndef KEY_LAYOUT_RU_ID
#define KEY_LAYOUT_RU_ID 1
#endif

static const key_layout_t key_layout_ru = {
    .name = "ru",
    .id = KEY_LAYOUT_RU_ID,
    .map_normal = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x00 - 0x07: 
    0x00, 0x09, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x08 - 0x0f: TAB, ENTER
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x10 - 0x17: 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x18 - 0x1f: 
    0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44D, // 0x20 - 0x27: ' ', 'э'
    0x00, 0x00, 0x00, 0x00, 0x431, 0x2d, 0x44E, 0x2f, // 0x28 - 0x2f: 'б', '-', 'ю', '/'
    0x30, 0x31, 0x32, 0x33 , 0x34, 0x35, 0x36, 0x37, // 0x30 - 0x37: '0', '1', '2', '3', '4', '5', '6', '7'
    0x38, 0x39, 0x00, 0x436, 0x00, 0x3d, 0x00, 0x00, // 0x38 - 0x3f: '8', '9', 'ж', '=',
    0x00 , 0x444, 0x438, 0x441, 0x432, 0x443, 0x430, 0x43F, // 0x40 - 0x47: 'ф', 'и', 'с', 'в', 'у', 'а', 'п'
    0x440, 0x448, 0x43E, 0x43B, 0x434, 0x44C, 0x442, 0x449, // 0x48 - 0x4f: 'р', 'ш', 'о', 'л', 'д', 'ь', 'т', 'щ'
    0x437, 0x439, 0x43A, 0x44B, 0x435, 0x433, 0x43C, 0x446, // 0x50 - 0x57: 'з', 'й', 'к', 'ы', 'е', 'г', 'м', 'ц'
    0x447, 0x43D, 0x44F, 0x445, 0x5c , 0x44A, 0x00 , 0x00 , // 0x58 - 0x5f: 'ч', 'н', 'я', 'х', '\', 'ъ',
    0x451, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x60 - 0x67: 'ё'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x68 - 0x6f: 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x70 - 0x77: 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x78 - 0x7f: 
    0x2e, 0x2a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x80 - 0x87: '.', '*'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x2d, 0x2b, 0x0a, // 0x88 - 0x8f: '-', '+', ENTER
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x90 - 0x97: 
    0x00, 0x00, 0x00, 0x00                          // 0x98 - 0x9b: 
    },

    .map_shifted = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x00 - 0x07: 
    0x00, 0x09, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x08 - 0x0f: TAB, ENTER
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x10 - 0x17: 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x18 - 0x1f: 
    0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42D, // 0x20 - 0x27: ' ', 'Э'
    0x00, 0x00, 0x00, 0x00, 0x411, 0x5f, 0x42E, 0x2c, // 0x28 - 0x2f: 'Б', '_', 'Ю', ','
    0x29, 0x21, 0x40, 0x23 , 0x24, 0x25, 0x5e, 0x26, // 0x30 - 0x37: ')', '!', '@', '#', '$', '%', '^', '&'
    0x2a, 0x28, 0x00, 0x416, 0x00, 0x2b, 0x00, 0x00, // 0x38 - 0x3f: '*', '(', 'Ж', '+',
    0x00 , 0x424, 0x418, 0x421, 0x412, 0x423, 0x410, 0x41F, // 0x40 - 0x47: 'Ф', 'И', 'С', 'В', 'У', 'А', 'П'
    0x420, 0x428, 0x41E, 0x41B, 0x414, 0x42C, 0x422, 0x429, // 0x48 - 0x4f: 'Р', 'Ш', 'О', 'Л', 'Д', 'Ь', 'Т', 'Щ'
    0x417, 0x419, 0x41A, 0x42B, 0x415, 0x413, 0x41C, 0x426, // 0x50 - 0x57: 'З', 'Й', 'К', 'Ы', 'Е', 'Г', 'М', 'Ц'
    0x427, 0x41D, 0x42F, 0x425, 0x7c , 0x42A, 0x00 , 0x00 , // 0x58 - 0x5f: 'Ч', 'Н', 'Я', 'Х', '|', 'Ъ',
    0x401, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x60 - 0x67: 'Ё'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x68 - 0x6f: 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x70 - 0x77: 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x78 - 0x7f: 
    0x2f, 0x2a, 0x00, 0x30, 0x31, 0x32, 0x33, 0x34, // 0x80 - 0x87: '/', '*', '0', '1', '2', '3', '4'
    0x35, 0x36, 0x37, 0x38, 0x39, 0x2d, 0x2b, 0x0a, // 0x88 - 0x8f: '5', '6', '7', '8', '9', '-', '+', ENTER
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x90 - 0x97: 
    0x00, 0x00, 0x00, 0x00                          // 0x98 - 0x9b: 
    }
};

#endif  //KEY_LAYOUT_RU_H

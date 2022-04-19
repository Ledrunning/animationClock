/**
 * @file clock_gui.h Графический интерфейс часов.
 */

#ifndef CLOCK_GUI_H
#define	CLOCK_GUI_H

#include <stdint.h>
#include <stdbool.h>
#include "errors/errors.h"
#include "fixed/fixed16.h"
#include "graphics/graphics.h"

typedef struct _Gui gui_t;

/**
 * Инициализирует графический интерфейс часов.
 * @param gui Графический интерфейс.
 * @return Код ошибки.
 */
extern err_t clock_gui_init(gui_t* gui);

/**
 * Устанавливает значение числа месяца.
 * @param monthday Число месяца.
 */
extern void clock_gui_set_monthday(uint32_t monthday);

/**
 * Устанавливает значение месяца.
 * @param monthday Значение месяца.
 */
extern void clock_gui_set_month(uint32_t month);

/**
 * Устанавливает значение дня недели.
 * @param weekday День недели.
 */
extern void clock_gui_set_weekday(uint32_t weekday);

/**
 * Устанавливает значение времени.
 * @param hours Часы.
 * @param mins Минуты.
 */
extern void clock_gui_set_time(uint32_t hours, uint32_t mins);

/**
 * Устанавливает значение температуры дома.
 * @param temp Температура.
 * @return Код ошибки.
 */
extern void clock_gui_set_temp_ind(fixed16_t temp);

extern void clock_gui_set_p(fixed16_t temp);

/**
 * Устанавливает значение температуры на улице.
 * @param temp Температура.
 * @return Код ошибки.
 */
extern void clock_gui_set_temp_outd(fixed16_t temp);

/**
 * Меняет разделитель часов и минут.
 */
extern void clock_gui_toggle_time_sep(void);

/**
 * Делает очередной шаг анимации.
 * @return Флаг завершения анимации.
 */
extern bool clock_gui_animation_step(void);

/**
 * Устанавливает цвет фона.
 * @param color Цвет фона.
 */
extern void clock_gui_set_back_color(graphics_color_t color);

/**
 * Устанавливает цвет числа месяца.
 * @param color Цвет числа месяца.
 */
extern void clock_gui_set_monthday_color(graphics_color_t color);

/**
 * Устанавливает цвет месяца.
 * @param color Цвет месяца.
 */
extern void clock_gui_set_month_color(graphics_color_t color);

/**
 * Устанавливает цвет дня недели.
 * @param color Цвет дня недели.
 */
extern void clock_gui_set_weekday_color(graphics_color_t color);

/**
 * Устанавливает цвет времени.
 * @param color Цвет времени.
 */
extern void clock_gui_set_time_color(graphics_color_t color);
extern void clock_gui_set_time_hour_color(graphics_color_t color);
extern void clock_gui_set_time_mins_color(graphics_color_t color);
/**
 * Устанавливает цвет температуры в доме.
 * @param color Цвет температуры в доме.
 */
extern void clock_gui_set_temp_ind_color(graphics_color_t color);

/**
 * Устанавливает цвет температуры на улице.
 * @param color Цвет температуры на улице.
 */
extern void clock_gui_set_temp_outd_color(graphics_color_t color);

/**
 * Устанавливает цвет.
 * @param color Цвет.
 */
//extern void clock_gui_set__color(graphics_color_t color);

#endif	/* CLOCK_GUI_H */


/**
 * @file future.h
 * Библиотека для работы с ещё не наступившем событием.
 */

#ifndef FUTURE_H
#define	FUTURE_H

#include <stdbool.h>
#include "defs/defs.h"

/**
 * Структура будущего.
 */
typedef struct _Future {
    //! Результат.
    void * volatile result;
    //! Флаг окончания.
    volatile bool done;
    //! Флаг выполнения.
    volatile bool running;
}future_t;

/**
 * Инициализирует будущее.
 * Незавершённое, невыполняющееся, с нулевым результатом.
 * @param future Будущее.
 */
EXTERN void future_init(future_t* future);

/**
 * Получает результат будущего.
 * @param future Будущее.
 * @return Результат.
 */
EXTERN void* future_result(const future_t* future);

/**
 * Устанавливает результат.
 * @param future Будущее.
 * @param result Результат.
 */
EXTERN void future_set_result(future_t* future, void* result);

/**
 * Получает флаг завершения.
 * @param future Будущее.
 * @return Флаг завершения.
 */
EXTERN bool future_done(const future_t* future);

/**
 * Устанавливает флаг завершения.
 * @param future Будущее.
 * @param done Флаг завершения.
 */
EXTERN void future_set_done(future_t* future, bool done);

/**
 * Получает флаг выполнения.
 * @param future Будущее.
 * @return Флаг выполнения.
 */
EXTERN bool future_running(const future_t* future);

/**
 * Устанавливает флаг выполнения.
 * @param future Будущее.
 * @param running Флаг выполнения.
 */
EXTERN void future_set_running(future_t* future, bool running);

/**
 * Запускает выполнение будущего (done = false; running = true;).
 * @param future Будущее.
 */
EXTERN void future_start(future_t* future);

/**
 * Завершает выполнение будущего (done = true; running = false;).
 * @param future Будущее.
 * @param result Результат.
 */
EXTERN void future_finish(future_t* future, void* result);

/**
 * Ждёт окончания выполнения будущего.
 * @param future Будущее.
 */
EXTERN void future_wait(const future_t* future);

#endif	/* FUTURE_H */


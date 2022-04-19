#ifndef _USART2_H
#define	_USART2_H

#include "stm32f10x.h "
#define Buff_size 256														//	Буфер для SD карты
extern uint8_t Buff_Uart[Buff_size];														//	Буфер для SD карты

//	Отправить байт в UART
 void send_Uart(USART_TypeDef* USARTx, unsigned char c);

//	Получить байт из UART
 unsigned char getch_Uart(USART_TypeDef* USARTx);

//	Отправка строки в USTR
 void send_Uart_str(USART_TypeDef* USARTx, unsigned char *s);

 void buff_clear(void);															//	очищаем буфер

//	выводим число в UART. Максимальная длина числа 6 цифр
 void send_int_Uart(USART_TypeDef* USARTx,unsigned long c);

//	Читаем число (в ASCII) с UART. Максимальной длиной в 6 цифр. 
//	Завершение ввода по вводу любого не цифрового символа
 unsigned long read_int_uart(USART_TypeDef* USARTx);

//	читаем текстовую строку с UART, до ввода <enter>. Максимальная длина строки 512Байт
 void read_str_uart(unsigned char* s);

void USART_init(void);

#endif	/* _USART_H */


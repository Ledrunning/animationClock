#ifndef _USART2_H
#define	_USART2_H

#include "stm32f10x.h "
#define Buff_size 256														//	����� ��� SD �����
extern uint8_t Buff_Uart[Buff_size];														//	����� ��� SD �����

//	��������� ���� � UART
 void send_Uart(USART_TypeDef* USARTx, unsigned char c);

//	�������� ���� �� UART
 unsigned char getch_Uart(USART_TypeDef* USARTx);

//	�������� ������ � USTR
 void send_Uart_str(USART_TypeDef* USARTx, unsigned char *s);

 void buff_clear(void);															//	������� �����

//	������� ����� � UART. ������������ ����� ����� 6 ����
 void send_int_Uart(USART_TypeDef* USARTx,unsigned long c);

//	������ ����� (� ASCII) � UART. ������������ ������ � 6 ����. 
//	���������� ����� �� ����� ������ �� ��������� �������
 unsigned long read_int_uart(USART_TypeDef* USARTx);

//	������ ��������� ������ � UART, �� ����� <enter>. ������������ ����� ������ 512����
 void read_str_uart(unsigned char* s);

void USART_init(void);

#endif	/* _USART_H */


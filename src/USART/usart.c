#include "stm32f10x.h"
#include "usart.h "
#include "web_esp8266.h" 


//	Задержка
void delay(unsigned long p)
{
	while(p>0){p--;}
}

//	Отправить байт в UART
void send_Uart(USART_TypeDef* USARTx, unsigned char c)
{
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE)== RESET){}
		USART_SendData(USARTx, c);
}

//	Получить байт из UART
unsigned char getch_Uart(USART_TypeDef* USARTx)
{
	while((USART_GetFlagStatus(USARTx,USART_FLAG_RXNE) == RESET)&&(TIM4->CR1 & TIM_CR1_CEN)){}
	return USART_ReceiveData(USARTx);
}

//	Отправка строки в USTR
void send_Uart_str(USART_TypeDef* USARTx, unsigned char *s)
{
  while (*s != 0) 
    send_Uart(USARTx, *s++);
}

uint8_t Buff_Uart[Buff_size];														//	Буфер 

void buff_clear()															//	очищаем буфер
{
	int i;
	for(i=0;i<Buff_size;i++)
	{
		Buff_Uart[i]=0;
	}
}

//	выводим число в UART. Максимальная длина числа 6 цифр
void send_int_Uart(USART_TypeDef* USARTx,unsigned long c)
{
	unsigned long d=10000000;
	do
	{
		c=c%d;
		d=d/10;
		send_Uart(USARTx,c/d+'0');
	}
	while(d>1);
}
 
//	Читаем число (в ASCII) с UART. Максимальной длиной в 6 цифр. 
//	Завершение ввода по вводу любого не цифрового символа
unsigned long read_int_uart(USART_TypeDef* USARTx)
{
	unsigned char temp=0,index=0;
	unsigned long value=0;
	while(index<8)
	{
		value=value*10+temp;
		temp=getch_Uart(USARTx);
		index++;
		if((47<temp)&&(temp<58))
		{
			temp=temp-48;
		}
		else
		{
			index=255;
		}
	}
	return value;
}

//	читаем текстовую строку с UART, до ввода <enter>. Максимальная длина строки 512Байт
void read_str_uart(unsigned char* s)
{
	unsigned char temp;
	unsigned int index=0;
	while((index<Buff_size) && (TIM4->CR1 & TIM_CR1_CEN))
	{
		temp=getch_Uart(USART2);
		if((temp!=13) )
		{
			*s++=temp;
		}
		else
		{
			index=Buff_size;
		}
		index++;
		
	}
}

	
void USART_init(void){
    GPIO_InitTypeDef gpio_tx =
        {.GPIO_Pin = GPIO_Pin_2, .GPIO_Speed = GPIO_Speed_50MHz, .GPIO_Mode = GPIO_Mode_AF_PP};
    GPIO_InitTypeDef gpio_rx =
        {.GPIO_Pin = GPIO_Pin_3, .GPIO_Speed = GPIO_Speed_50MHz, .GPIO_Mode = GPIO_Mode_IN_FLOATING};
    GPIO_Init(GPIOA, &gpio_tx);
    GPIO_Init(GPIOA, &gpio_rx);
    
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
				
    USART_InitTypeDef usart_is =
        {.USART_BaudRate = 115200, .USART_WordLength = USART_WordLength_8b, .USART_StopBits = USART_StopBits_1,
         .USART_Parity = USART_Parity_No, .USART_Mode = USART_Mode_Rx | USART_Mode_Tx, .USART_HardwareFlowControl = USART_HardwareFlowControl_None};
    USART_Init(USART2, &usart_is);
    USART_Cmd(USART2, ENABLE);
				 
		 
	/* TIM4 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;			 
	  /* Time Base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = 24000 - 1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period =  2000 ; // Чтоб прерывание случалось раз в секунду
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);			 
	  /* TIM4 counter enable */
  TIM_Cmd(TIM4, ENABLE);			 
				 
				 
	   
}
	
	
	

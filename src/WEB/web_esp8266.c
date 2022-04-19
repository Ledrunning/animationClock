#include "stm32f10x.h"
#include "web_esp8266.h" 
#include "usart.h "
#include "string.h"
#include "utils/delay.h"
#include "rtc/rtc.h"
#include "ds18x20.h"
#include "tft9341/tft9341.h"

uint16_t x_print = 300;
uint16_t y_print = 255;

uint8_t ret_ESP;
uint8_t send_OK;
uint8_t hard_reset;
uint8_t len_data;

unsigned char arr_data[256];
unsigned char array [256]; 
unsigned char mac [18]; 

uint8_t MY_SSID[16]= "wifi";
uint8_t MY_SSID_PWD[16]= "0123456789";

   /*------------Замена цвета---------------*/
   #define TRANS           0xFFFE	 
   #define BLACK           0x0000      /*   0,   0,   0 */
   #define NAVY            0x000F      /*   0,   0, 128 */
   #define DGREEN          0x03E0      /*   0, 128,   0 */
   #define DCYAN           0x03EF      /*   0, 128, 128 */
   #define MAROON          0x7800      /* 128,   0,   0 */
   #define PURPLE          0x780F      /* 128,   0, 128 */
   #define OLIVE           0x7BE0      /* 128, 128,   0 */
   #define LGRAY           0xC618      /* 192, 192, 192 */
   //#define DGRAY           0x7BEF      /* 128, 128, 128 */
   #define BLUE            0x001F      /*   0,   0, 255 */
   #define BLUE2			     0x051D
   #define GREEN           0x07E0      /*   0, 255,   0 */
   #define GREEN2		       0xB723
   #define GREEN3		       0x8000
   #define CYAN            0x07FF      /*   0, 255, 255 */
   #define RED             0xF800      /* 255,   0,   0 */
   #define MAGENTA         0xF81F      /* 255,   0, 255 */
   #define YELLOW          0xFFE0      /* 255, 255,   0 */
   #define WHITE           0xFFFF      /* 255, 255, 255 */
   #define ORANGE          0xFD20      /* 255, 165,   0 */
   #define GREENYELLOW     0xAFE5      /* 173, 255,  47 */
   #define BROWN 			     0XBC40 //
   #define BRRED 			     0XFC07 //
   #define LIGHTBLUE	     0X7D7C 
	 #define GRAY			       0X8430
   #define DGRAY			     0X2104
	 #define LIGHTRED	       0xFA08

void setSSID(uint8_t *ssid, uint8_t *pas){
		strcpy(MY_SSID,ssid);	
		strcpy(MY_SSID_PWD,pas);
	  //LogOut(OTSTUP_SLEVA_ERROR, OTSTUP_SVERHU_ERROR+5, MY_SSID , &Font_7x10, WHITE, BLACK);	
}

extern volatile uint8_t refresh_lcd;
typedef struct {
	uint8_t FontWidth;    /*!< Font width in pixels */
	uint8_t FontHeight;   /*!< Font height in pixels */
	const uint8_t *data; /*!< Pointer to data font data array */
} FontDef8_t;

FontDef8_t font_log;

 void LogOut_wifi(uint16_t x, uint16_t y, char *str, FontDef8_t *font, uint16_t foreground, uint16_t background){
//	 refresh_lcd=1; 	 
//	 cycl_multimetr(); 
//   ILI9341_Puts_pixel	 (x,y, str,  &font_log, GREEN, TRANS);
//	 //ILI9341_Puts(OTSTUP_SLEVA_ERROR, OTSTUP_SVERHU_ERROR+5, str , &Font_7x10, WHITE, TRANS);  
//	 delay_ms(250);		 
 }
 uint8_t ret_ESP_max;
 
 uint8_t connect_to_AP()
{	 
	if (ret_ESP>ret_ESP_max)ret_ESP_max=ret_ESP;
	
	 switch (ret_ESP)
		{ 			
			case 0  : 
			LogOut_wifi(x_print,   y_print, "ESP8266, response AT", &font_log, GREEN, BLACK);
			response_AT     ();
			break; // проверка ответа от АР
			
			case 1  : 
			LogOut_wifi(x_print,   y_print, "ESP8266, reset", &font_log, GREEN, BLACK);
			reset_AT        (); 
			break; // программный сброс
			
			case 2  : 
			LogOut_wifi(x_print,   y_print, "ESP8266, get mode", &font_log, GREEN, BLACK);
			mode_AT         (); 
			break; // проверка режима точка доступа или клиент?
			
			case 3  : 
			LogOut_wifi(x_print,   y_print, "ESP8266, select client", &font_log, GREEN, BLACK);
			select_clnt_AT  ();
			break; // переключение в режим клиента
			
		  case 4  : 
			LogOut_wifi(x_print,   y_print, "ESP8266, read connect", &font_log, GREEN, BLACK);
			rd_connect_AT   (); 
			break; // проверка подключения к базовой станции
			
			case 5  : 
			LogOut_wifi(x_print,   y_print, "ESP8266, connect wifi", &font_log, GREEN, BLACK);
			connect_AT      (); 
			break; // соединение с базовой станцией
			
			case 6  : 
			LogOut_wifi(x_print,   y_print, "ESP8266, read MAC", &font_log, GREEN, BLACK);
			read_MAC_AT     (); 
			break; // чтение IP и MAC
			
			case 7  : 
			LogOut_wifi(x_print,   y_print, "ESP8266, cipmux", &font_log, GREEN, BLACK);
			cipmux_AT       (); 
			break; // одиночные подключения
			
			case 8  : 
			LogOut_wifi(x_print,   y_print, "ESP8266, read sensors", &font_log, GREEN, BLACK);
			lead_data_sensor(); 
			break; // получение данных с датчиков
			
			
			case 9  : 
			LogOut_wifi(x_print,   y_print, "ESP8266, connect narodmon", &font_log, GREEN, BLACK);
			connect_serv_AT (); 
			break; // подключение к серверу
			
			case 10 : 
			//LogOut_wifi(x_print,   y_print, "ESP8266, send data", &font_log, GREEN, BLACK);
			send_data_AT    (); 
			break; // отправкка данных на сервер
			
			case 11 : 
			//LogOut_wifi(x_print,   y_print, "ESP8266, ret server", &font_log, GREEN, BLACK);
			read_ret_server (); 
			break; // получение ответа от сервера
			
			case 12 :
			LogOut_wifi(x_print,   y_print, "ESP8266, closed connect", &font_log, GREEN, BLACK);
			 closed_connect  (); 
			break; // закрытие соединения
			

			case 13 :  
			LogOut_wifi(x_print,   y_print, "ESP8266, connect serv time", &font_log, GREEN, BLACK);
			connect_serv_AT_time ();
			break; // подключение к серверу
			
			case 14 :  
			//LogOut_wifi(x_print,   y_print, "ESP8266, send time", &font_log, GREEN, BLACK);
			send_data_AT_time    ();
			break; // отправкка данных на сервер
			
			case 15 : 
			//LogOut_wifi(x_print,   y_print, "ESP8266, get time", &font_log, GREEN, BLACK);
			read_ret_server_time (); 
			break; // получение ответа от сервера
			
			case 16 : 
			LogOut_wifi(x_print,   y_print, "ESP8266, closed connect time", &font_log, GREEN, BLACK);
			closed_connect_time  (); 
			break; // закрытие соединения			
			
						
			case 17 : 
			//LogOut_wifi(x_print,   y_print, "END", &font_log, GREEN, BLACK);				
			return send_OK = 100;
			break;

			default:
			return send_OK = 0;				
			break;	
		}
	 return send_OK;
 }
 
 

//********************************************************************************************
//
//   ****          ТАЙМЕР СБРОСА ПРИ НЕОТВЕТЕ
//
//********************************************************************************************
void error_check (unsigned int delay_1)
 {
	 	 //HAL_IWDG_Refresh(&hiwdg);
     TIM4->PSC = 36000-1;
     TIM4->ARR = delay_1;
     TIM4->EGR |= TIM_EGR_UG;
     TIM4->CR1 |= TIM_CR1_CEN|TIM_CR1_OPM;
 }
 
//********************************************************************************************
//
// 0  ****          ПРОВЕРКА ОТВЕТА ОТ МОДУЛЯ
//
//********************************************************************************************
uint8_t response_AT (void) // проверка ответа
{
		//LCD_Fill(BLACK);
		error_check (1000); 
		arr_data_clear();
		array_clear();	
	
	  send_Uart_str(USART2,"ATE0\r\n"); // Проверка ответа от модуля, "перекличка"
	  do{
			read_str_uart(Buff_Uart);
		}
		while((!strstr(Buff_Uart,"OK")) && (!strstr(Buff_Uart,"ERROR")) && (TIM4->CR1 & TIM_CR1_CEN));
		TIM4->EGR |= TIM_EGR_UG;  TIM4->CR1 &= ~TIM_CR1_CEN;     // stop таймера	 
		
		if (strstr(Buff_Uart,"OK"))
		{
			//WriteString( 0, 0, "Ответ ESP8266,OK" , YELLOW);
			LogOut_wifi(x_print,   y_print, "AT response", &font_log, GREEN, BLACK);	
			//delay_ms (200);
			
		     error_check (1000); 
		     arr_data_clear();
		     array_clear();	
	
	       send_Uart_str(USART2,"AT+GMR\r\n"); // Проверка ответа от модуля, "перекличка"
	       do{
			     read_str_uart(Buff_Uart);
		     }
		     while((!strstr(Buff_Uart,"OK")) && (!strstr(Buff_Uart,"ERROR")) && (TIM4->CR1 & TIM_CR1_CEN));
		     TIM4->EGR |= TIM_EGR_UG;  TIM4->CR1 &= ~TIM_CR1_CEN;     // stop таймера	 			
			
				 
				 
			
			return ret_ESP = 2;          // переход дальше			
		}
		else if (strstr(Buff_Uart,"ERROR"))
		{
			//WriteString( 0, 0, "ERROR", RED);	
			LogOut_wifi(x_print,   y_print, "AT ERROR", &font_log, GREEN, BLACK);	
			//delay_ms (200);			
			return ret_ESP = 0;						// повтор
		}
		else if (strstr(Buff_Uart,"Busy is"))
		{
			//WriteString( 0, 0, "busy is" , RED);
			LogOut_wifi(x_print,   y_print, "AT busy", &font_log, GREEN, BLACK);
			//delay_ms (3000);
			return ret_ESP = 0;				// повтор
		}
		else if ((Buff_Uart)||(!Buff_Uart))
		{
			//WriteString( 0, 0, "time out, return" , WHITE);
			LogOut_wifi(x_print,   y_print, "Time out 0", &font_log, GREEN, BLACK);			
			//delay_ms (500);
			return ret_ESP = 1; 			// сброс
		}	
		return ret_ESP = 2; 		
}
//********************************************************************************************
//
//  1 ****          ПРОГРАММНЫЙ или ЖЕЛЕЗНЫЙ СБРОС
//
//********************************************************************************************
uint8_t reset_AT (void)  // программный ресет
{
	
	static uint8_t repite;
	
		//if (hard_reset++ == 5)
    // {
		 //LCD_Fill(BLACK);
	   //WriteString( 0, 5, "Перезапуск ESP8266" , RED);
		 //delay_ms (100);
		 //WriteString( 1, 5, "Стоп ESP8266" , YELLOW);
		 //GPIOA->BSRR =  GPIO_BSRR_BR11;  //GPIOA.11=0
		 //delay_ms (1000);
		 //WriteString( 2, 5, "Включение ESP8266" , WHITE);
		 //GPIOA->BSRR =  GPIO_BSRR_BS11;  //GPIOA.11=1
		 //delay_ms (1000);
		// }
		//LCD_Fill(BLACK);
		//WriteString( 0, 0, "Сброс ESP8266" , WHITE);
  	//LogOut_wifi(x_print,   y_print, "Reset ESP8266", &font_log, GREEN, BLACK);		
		//delay_ms (200);
		//WriteString( 1, 0, "ожидание..." , YELLOW);
	
		GPIO_ResetBits(GPIOB, GPIO_Pin_3);		
    delay_ms(100);			
		GPIO_SetBits(GPIOB, GPIO_Pin_3);
	
	
		error_check (3000); 
		arr_data_clear();
		array_clear();	
	
	  send_Uart_str(USART2,"AT+RST\r\n"); // Reset
		buff_clear();
	  do{
			read_str_uart(Buff_Uart);
		}
		while((!strstr(Buff_Uart,"ready")) && (!strstr(Buff_Uart,"ERROR"))&& (!strstr(Buff_Uart,"busy")) && (TIM4->CR1 & TIM_CR1_CEN));
		TIM4->EGR |= TIM_EGR_UG;  TIM4->CR1 &= ~TIM_CR1_CEN;     // stop таймера	  
			
		if (strstr(Buff_Uart,"ready"))
		{
			//WriteString( 2, 0, "чтение..." , YELLOW);
			//delay_ms (200);
			//WriteString( 3, 0, "Успешно!" , GREEN);
			LogOut_wifi(x_print,   y_print, "Reset OK", &font_log, GREEN, BLACK);		
			//delay_ms (1000);
			return ret_ESP = 0;						
		}
		else if (strstr(Buff_Uart,"ERROR"))
		{
			//WriteString( 2, 0, "ERROR", RED);
			LogOut_wifi(x_print,   y_print, "Reset ERROR", &font_log, GREEN, BLACK);		
			//delay_ms (200);			
			return ret_ESP = 1;						
		}
		else if (strstr(Buff_Uart,"busy"))
		{
			//WriteString( 2, 0, "busy is" , RED);
			LogOut_wifi(x_print,   y_print, "Busy is", &font_log, GREEN, BLACK);		
			//delay_ms (3000);
			return ret_ESP = 1;			
		}
		else if ((Buff_Uart)||(!Buff_Uart))
		{
			//WriteString( 2, 0, "time out, return" , WHITE);
			LogOut_wifi(x_print,   y_print, "ESP8266 not detected", &font_log, GREEN, BLACK);		
			//delay_ms (500);
			
		  if (repite<3){
				repite++;
		  	return ret_ESP = 1;				
			}
			else {
				repite=0;				
			  return ret_ESP = 99;
			}
			
			
			
		}	
		return ret_ESP = 0;	
}


//********************************************************************************************
//
// 2  ****          ПРОВЕРКА РЕЖИМА, КЛИЕНТ/ТОЧКА ДОСТУПА
//
//********************************************************************************************
unsigned char mode_AT (void)  //
{

		error_check (10000); 
		arr_data_clear();
		array_clear();	
	
	  send_Uart_str(USART2,"AT+CWMODE?\r\n"); // Проверка режима работы
	  do{
			read_str_uart(Buff_Uart);
		}
		while((!strstr(Buff_Uart,"MODE:1")) && (!strstr(Buff_Uart,"MODE:2")) && (!strstr(Buff_Uart,"MODE:3"))&& (!strstr(Buff_Uart,"busy")) && (TIM4->CR1 & TIM_CR1_CEN));
		TIM4->EGR |= TIM_EGR_UG;  TIM4->CR1 &= ~TIM_CR1_CEN;     // stop таймера	  
				
		if (strstr(Buff_Uart,"MODE:1"))
		{
			//WriteString( 1, 0, "Режим клиент, ОК" , GREEN);
			LogOut_wifi(x_print,   y_print, "AP, OK", &font_log, GREEN, BLACK);		
			//delay_ms (200);
			return ret_ESP = 4;						
		}
		else if (strstr(Buff_Uart,"MODE:2"))
		{
			//WriteString( 1, 0, "станция, NO", RED);	
			LogOut_wifi(x_print,   y_print, "ST -> Switching", &font_log, GREEN, BLACK);		
			//delay_ms (200);
			//WriteString( 2, 0, "переключение", RED);			
			//delay_ms (500);
			return ret_ESP = 3;						
		}
		else if (strstr(Buff_Uart,"MODE:3"))
		{
			//WriteString( 1, 0, "станция+клиент, NO", RED);	
			LogOut_wifi(x_print,   y_print, "ST+AP -> Switching", &font_log, GREEN, BLACK);		
			//delay_ms (200);
			//WriteString( 2, 0, "переключение", RED);	
							
			//delay_ms (500);
			return ret_ESP = 3;						
		}		
		else if (strstr(Buff_Uart,"busy"))
		{
			//WriteString( 1, 0, "busy is" , RED);
			LogOut_wifi(x_print,   y_print, "Busy is", &font_log, GREEN, BLACK);		
			//delay_ms (600);
			return ret_ESP = 2;			
		}
		else if ((Buff_Uart)||(!Buff_Uart))
		{
			//WriteString( 1, 0, "time out, return" , WHITE);
			LogOut_wifi(x_print,   y_print, "Time out, return 2", &font_log, GREEN, BLACK);		
			//delay_ms (500);
			return ret_ESP = 2;
		}	
		return ret_ESP = 4;		
}

//********************************************************************************************
//
//  3 ****          ПЕРЕКЛЮЧЕНИЕ В РЕЖИМ КЛИЕНТА //+ сервер
//
//********************************************************************************************
unsigned char select_clnt_AT (void)
{
	  
		error_check (10000); 
		arr_data_clear();
		array_clear();	
	
	  send_Uart_str(USART2,"AT+CWMODE=1\r\n"); // Проверка режима работы
	  do{
			read_str_uart(Buff_Uart);
		}
		while((!strstr(Buff_Uart,"OK")) && (!strstr(Buff_Uart,"ERROR"))&& (!strstr(Buff_Uart,"busy")) && (TIM4->CR1 & TIM_CR1_CEN));
		TIM4->EGR |= TIM_EGR_UG;  TIM4->CR1 &= ~TIM_CR1_CEN;     // stop таймера	 	 
		
		if (strstr(Buff_Uart,"OK"))
		{
			//WriteString( 3, 0, "клиент, ОК" , WHITE);	
      LogOut_wifi(x_print,   y_print, "Client, OK", &font_log, GREEN, BLACK);					
		}
		else if (strstr(Buff_Uart,"ERROR"))
		{
			//WriteString( 3, 0, "Ошибка", RED);
			LogOut_wifi(x_print,   y_print, "Client, error", &font_log, GREEN, BLACK);		
		}
		else if (strstr(Buff_Uart,"busy"))
		{
			//WriteString( 3, 0, "busy is" , RED);  			//delay_ms (2600);		
			LogOut_wifi(x_print,   y_print, "Busy is", &font_log, GREEN, BLACK);		
		}
		else if ((Buff_Uart)||(!Buff_Uart))
		{
			//WriteString( 3, 0, "time out, return" , WHITE);
			LogOut_wifi(x_print,   y_print, "Time out, return 3", &font_log, GREEN, BLACK);		
		}	
		//delay_ms (200);
		return ret_ESP = 1;	
}


//********************************************************************************************
//
// 4  ****         ПРОВЕРКА      ПОДКЛЮЧЕНИЯ К БАЗОВОЙ СТАНЦИИ
//
//********************************************************************************************
unsigned char rd_connect_AT(void)
{	
		error_check (10000); 
		arr_data_clear();
		array_clear();	
	
	  send_Uart_str(USART2,"AT+CWJAP?\r\n");
	  do{
			read_str_uart(Buff_Uart);
		}
		while((!strstr(Buff_Uart,"WJAP:")) && (!strstr(Buff_Uart,"No")) && (TIM4->CR1 & TIM_CR1_CEN));
		TIM4->EGR |= TIM_EGR_UG;  TIM4->CR1 &= ~TIM_CR1_CEN;     // stop таймера	 
			
		if (strstr(Buff_Uart,"CWJAP:"))
		{						
	  	//sprintf (array, "%s, ОК", Buff_Uart+8);
			//WriteString( 2, 0, "Подключено      ", GREEN);	
			LogOut_wifi(x_print,   y_print, "Wifi connected", &font_log, GREEN, BLACK);		
			//delay_ms (200);
			return ret_ESP = 6;						
		}
		else if (strstr(Buff_Uart,"No"))
		{
			//WriteString( 2, 0, "Нет подключения" , RED);
			LogOut_wifi(x_print,   y_print, "Not connect wifi", &font_log, GREEN, BLACK);		
			//delay_ms (200);
			return ret_ESP = 5;						
		}
		else if ((Buff_Uart)||(!Buff_Uart))
		{
			//WriteString( 2, 0, "time out, return" , WHITE);
			LogOut_wifi(x_print,   y_print, "Time out, return 4", &font_log, GREEN, BLACK);		
			//delay_ms (500);
			return ret_ESP = 1;
		}	
		return ret_ESP = 6;	
	
}

//********************************************************************************************
//
// 5  ****         ПОДКЛЮЧЕНИЕ К БАЗОВОЙ СТАНЦИИ
//
//********************************************************************************************
unsigned char connect_AT(void)
{
	static uint8_t repite;
	
	  //WriteString( 2, 0, "Подключение!      " , YELLOW);
	  //LogOut_wifi(x_print,   y_print, "Connect!      ", &font_log, GREEN, BLACK);		
//		delay_ms (200);
		error_check (10000); 	  	
	  arr_data_clear();
		array_clear();	
	
	  strcat (arr_data,"AT+CWJAP=");	
	  strcat (arr_data,"\"");   	
	  strcat (arr_data, MY_SSID);
	  strcat (arr_data,"\"");   	
	  strcat (arr_data,","); 
	  strcat (arr_data,"\"");  	
		strcat (arr_data, MY_SSID_PWD);
	  strcat (arr_data,"\"");  	
	  strcat (arr_data,"\r\n");  	

	  send_Uart_str(USART2, arr_data);//"AT+CWJAP_CUR=""Kurochka"",""sosolala""\r\n" AT+CWJAP="имя Вашей домашней сети wi-fi","пароль к ней"
	
	  //sprintf(array,"#H1#%2.2f#Home_Humidity\n", bmp280_hf ); 
		//strcat (arr_data, array);
	
	  do{
			read_str_uart(Buff_Uart);
		}
		while((!strstr(Buff_Uart,"OK")) && (!strstr(Buff_Uart,"FAIL"))&& (!strstr(Buff_Uart,"busy")) && (TIM4->CR1 & TIM_CR1_CEN));
		TIM4->EGR |= TIM_EGR_UG;  TIM4->CR1 &= ~TIM_CR1_CEN;     // stop таймера	  
		
		if (strstr(Buff_Uart,"OK"))
		{
			//WriteString( 2, 0, "Подключено!     " , GREEN);
			LogOut_wifi(x_print,   y_print, "Wifi connect", &font_log, GREEN, BLACK);		
			//delay_ms (200);
			return ret_ESP = 6;						
		}
		else if (strstr(Buff_Uart,"CONNECTED"))
		{
			//WriteString( 2, 0, "Подключено!     " , GREEN);
			LogOut_wifi(x_print,   y_print, "Wifi connect", &font_log, GREEN, BLACK);		
			//delay_ms (200);
			return ret_ESP = 6;						
		}		
		
		
		else if (strstr(Buff_Uart,"FAIL"))
		{
			//WriteString( 2, 0, "Подключено!     " , GREEN);
			LogOut_wifi(x_print,   y_print, "Wifi fail", &font_log, GREEN, BLACK);		
			//delay_ms (200);
			return ret_ESP = 5;						
		}		
		else if (strstr(Buff_Uart,"busy"))
		{
			//WriteString( 2, 0, "Подключено!     " , GREEN);
			LogOut_wifi(x_print,   y_print, "Wifi busy", &font_log, GREEN, BLACK);		
			//delay_ms (200);
			return ret_ESP = 5;						
		}				
		else if (strstr(Buff_Uart,"WJAP:3"))
		{
			//WriteString( 2, 0, "Неверный логин  ", RED);	
			LogOut_wifi(x_print,   y_print, "Error login", &font_log, GREEN, BLACK);		
			//delay_ms (200);
			return ret_ESP = 17;						
		}
		else if (strstr(Buff_Uart,"WJAP:1"))
		{
			//WriteString( 2, 0, "Неверный пароль ", RED);	
			LogOut_wifi(x_print,   y_print, "Error pasvord", &font_log, GREEN, BLACK);		
			//delay_ms (200);
			return ret_ESP = 17;						
		}
		else if ((Buff_Uart)||(!Buff_Uart))
		{
			//WriteString( 2, 0, "time out, return" , WHITE);
			LogOut_wifi(x_print,   y_print, "Time out, return 5", &font_log, GREEN, BLACK);		
			//delay_ms (500);
			if (repite<3){
				repite++;
		  	return ret_ESP = 5;				
			}
			else {
				repite=0;				
			  return ret_ESP = 1;
			}
		}	
		return ret_ESP = 6;	
	
}

//********************************************************************************************
//
// 6  ****         ПОЛУЧЕНИЕ IP & MAC АДРЕСА
//
//********************************************************************************************
uint8_t is;
unsigned char read_MAC_AT(void)
{	  
	  static uint8_t repite;
	
		error_check (2000); 
		arr_data_clear();
		array_clear();	
	
	  send_Uart_str(USART2,"AT+CIFSR\r\n");
	  do{			
			
			read_str_uart(Buff_Uart);
		}
		while((!strstr(Buff_Uart,"STAMAC")) && (!strstr(Buff_Uart,"ERROR"))&& (!strstr(Buff_Uart,"busy")) && (TIM4->CR1 & TIM_CR1_CEN));
		TIM4->EGR |= TIM_EGR_UG;  TIM4->CR1 &= ~TIM_CR1_CEN;     // stop таймера	  
		
//		if (strstr(Buff_Uart,"STAIP"))
//		{
//			arr_data_clear();
//			strncpy (arr_data, (Buff_Uart+15), ((strlen(Buff_Uart))-16));
//			sprintf (Buff_Uart, "IP: %s", arr_data);
//			//WriteString( 3, 0, Buff_Uart, GREEN);	
//			//delay_ms (200);
//      error_check (2000); 
//	    send_Uart_str(USART2,"AT+CIFSR\r\n");
//	    do{
//		  	read_str_uart(Buff_Uart);
//		    }
//		  while((!strstr(Buff_Uart,"MAC")) && (!strstr(Buff_Uart,"ERROR"))&& (!strstr(Buff_Uart,"busy")) && (TIM4->CR1 & TIM_CR1_CEN));
//		  TIM4->EGR |= TIM_EGR_UG;  TIM4->CR1 &= ~TIM_CR1_CEN;     // stop таймера	 			
//		}
		if (strstr(Buff_Uart,"STAMAC"))
		{
			uint8_t mac_c=0;
			arr_data_clear();
			array_clear();
			strcpy (arr_data,"#");
			for (is=16; is < 32; is=is+3) {
			  strncat(arr_data, (Buff_Uart+is), 2);
				if (mac_c<5){
					mac_c++;
			    strncat(arr_data, ":", 1);
				}
			}	
			sprintf (mac, "%s", arr_data+1);
			//WriteString( 4, 0, array, GREEN);	
			//LogOut_wifi(x_print,   y_print, array, &font_log, GREEN, BLACK);		
			LogOut_wifi(x_print,   y_print, "MAC OK", &font_log, GREEN, BLACK);		
			//delay_ms (200);
			return ret_ESP = 7;	
		}		
		else if (strstr(Buff_Uart,"ERROR"))
		{
			//WriteString( 2, 0, "Ошибка", RED);
      LogOut_wifi(x_print,   y_print, "MAC ERROR", &font_log, GREEN, BLACK);					
			//delay_ms (200);
			return ret_ESP = 1;						
		}
		else if ((Buff_Uart)||(!Buff_Uart))
		{
			//WriteString( 2, 0, "time out, return" , WHITE);
			LogOut_wifi(x_print,   y_print, "Time out, return 6", &font_log, GREEN, BLACK);		
			//delay_ms (500);
		
			
			if (repite<3){
				repite++;
		  	return ret_ESP = 6;				
			}
			else {
				repite=0;				
			  return ret_ESP = 1;
			}
			
			
		}	
}

//********************************************************************************************
//
// 7  ****         ПРОВЕРКА МНОЖЕСТВЕННЫХ ИЛИ ОДИНОЧНЫХ ПОДКЛЮЧЕНИЙ, необходимо одиночные.
//
//********************************************************************************************
unsigned char cipmux_AT(void)
{	
		error_check (10000); 
	  arr_data_clear();
		array_clear();	
	
	  send_Uart_str(USART2,"AT+CIPMUX?\r\n");
	  do{
			read_str_uart(Buff_Uart);
		}
		while((!strstr(Buff_Uart,"MUX:1")) && (!strstr(Buff_Uart,"MUX:0"))&& (!strstr(Buff_Uart,"ERROR")) && (TIM4->CR1 & TIM_CR1_CEN));
		TIM4->EGR |= TIM_EGR_UG;  TIM4->CR1 &= ~TIM_CR1_CEN;     // stop таймера	  
		
		if (strstr(Buff_Uart,"MUX:0"))
		{
			//WriteString( 5, 0, "MUX:0, OK" , GREEN);
			LogOut_wifi(x_print,   y_print, "MUX 0, OK" , &font_log, GREEN, BLACK);		
			//delay_ms (200);
			return ret_ESP = 8;						
		}
		else if (strstr(Buff_Uart,"MUX:1"))
		{
			//WriteString( 5, 0, "multi, изменение", RED);
			LogOut_wifi(x_print,   y_print, "multi", &font_log, GREEN, BLACK);		
			//******** контрольное закрытие соединения
			error_check (10000); 
  	  send_Uart_str(USART2,"AT+CIPCLOSE\r\n");
  	  do{
	  		read_str_uart(Buff_Uart);
	    	}
  		while((!strstr(Buff_Uart,"MUX=1")) && (!strstr(Buff_Uart,"CLOSED"))&& (!strstr(Buff_Uart,"ERROR")) && (TIM4->CR1 & TIM_CR1_CEN));
	  	TIM4->EGR |= TIM_EGR_UG;  TIM4->CR1 &= ~TIM_CR1_CEN;     // stop таймера			
			//******** изменение режима на одиночные подключения
			error_check (10000); 
  	  send_Uart_str(USART2,"AT+CIPMUX=0\r\n");
  	  do{
	  		read_str_uart(Buff_Uart);
	    	}
  		while((!strstr(Buff_Uart,"MUX:1")) && (!strstr(Buff_Uart,"OK"))&& (!strstr(Buff_Uart,"ERROR")) && (TIM4->CR1 & TIM_CR1_CEN));
	  	TIM4->EGR |= TIM_EGR_UG;  TIM4->CR1 &= ~TIM_CR1_CEN;     // stop таймера
			
			//delay_ms (200);			
			return ret_ESP = 7;		// возврат в эту-же функцию в начало				
		}
		else if (strstr(Buff_Uart,"ERROR"))
		{
			//WriteString( 5, 0, "Ошибка", RED);	
			LogOut_wifi(x_print,   y_print, "ERROR", &font_log, GREEN, BLACK);		
			//delay_ms (200);
			return ret_ESP = 1;			// сброс			
		}
		else if ((Buff_Uart)||(!Buff_Uart))
		{
			//WriteString( 5, 0, "time out, return" , WHITE);
			LogOut_wifi(x_print,   y_print, "Time out, return 7", &font_log, GREEN, BLACK);		
			//delay_ms (500);
			return ret_ESP = 1; // сброс
		}		
		return ret_ESP = 8;	
}

//********************************************************************************************
//
// 8  ****         ПОЛУЧЕНИЕ ДАННЫХ С ДАТЧИКОВ, ФОРМИРОВАНИЕ СТРОКИ ОТПРАВКИ
//
//********************************************************************************************
extern float bmp280_t;
extern float bmp280_hf;
extern float bmp280_p;
extern int err_AM2320;
extern float am2320_h;
extern err_t err_ds18b20;
extern float ds18b20_t;

unsigned char lead_data_sensor(void)
{
	  uint8_t er=1;
	
		arr_data_clear();
		array_clear();
	
	  strcat (arr_data,"#");	
	  strcat (arr_data, mac);
	  strcat (arr_data,"#GFXclock\n");
	
	  if (err_ds18b20==E_NO_ERROR){
    	sprintf(array, "#T1#%2.2f#Home_Temperature\n", ds18b20_t );
		  strcat (arr_data, array);
			er=0;
	  }
		
//	  if (err_AM2320==0){
//	    sprintf(array,"#H1#%2.2f#Home_Humidity\n", am2320_h ); 
//		  strcat (arr_data, array);
//			er=0;			
//		}
		
		if(!get_BME280_Error()){
	    sprintf(array,"#H1#%2.2f#Home_Humidity\n", (float)bmp280_hf/10 ); 
		  strcat (arr_data, array);	
			er=0;			
		}		
		
		if(!get_BME280_Error()){
	    sprintf(array,"#P1#%2.2f#Home_Pressure\n", bmp280_p ); 
		  strcat (arr_data, array);	
			er=0;			
		}
		
		strcat (arr_data, "##");
	
		len_data = 0;
		len_data = strlen (arr_data);
	
		if (er==1){
			LogOut_wifi(x_print,   y_print, "Sensors not work!", &font_log, GREEN, BLACK);
			return ret_ESP = 13;			
		}
	  else {
			LogOut_wifi(x_print,   y_print, "Sensors OK", &font_log, GREEN, BLACK);			
			return ret_ESP = 9;
		}
}


//********************************************************************************************
//
// 9  ****         ПОДКЛЮЧЕНИЕ К СЕРВЕРУ
//
//********************************************************************************************
unsigned char connect_serv_AT(void)
{ static uint8_t rst;
	static uint8_t repite;
	
		error_check (10000); 
//	  arr_data_clear();
//		array_clear();	
	
	  send_Uart_str(USART2,"AT+CIPSTART=\"TCP\",\"narodmon.ru\",8283\r\n");

	  do{
			read_str_uart(Buff_Uart);
		}
		while((!strstr(Buff_Uart,"OK"))&& (!strstr(Buff_Uart,"CONNECT")) && (!strstr(Buff_Uart,"CLOSED"))&& (!strstr(Buff_Uart,"ERROR")) && (TIM4->CR1 & TIM_CR1_CEN));
		TIM4->EGR |= TIM_EGR_UG;  TIM4->CR1 &= ~TIM_CR1_CEN;     // stop таймера	  
		
		if ((strstr(Buff_Uart,"OK"))||(strstr(Buff_Uart,"CONNECT")))
		{
			//WriteString( 6, 0, "Соединен!            " , GREEN);
			LogOut_wifi(x_print,   y_print, "Narodmon connected", &font_log, GREEN, BLACK);
			rst = 0;
			//delay_ms (200);
			return ret_ESP = 10;						
		}
		else if ((strstr(Buff_Uart,"CLOSED"))||(strstr(Buff_Uart,"ERROR")) )
		{
			sprintf (array, "CLOSED %u repite", rst);
			if (!rst){
				//WriteString( 6, 0, array, RED);	
				LogOut_wifi(x_print,   y_print, array, &font_log, GREEN, BLACK);
				//delay_ms (200);
				}
			else {
				//WriteString( 6, 0, array, YELLOW);
				LogOut_wifi(x_print,   y_print, array, &font_log, GREEN, BLACK);		
				//delay_ms (200);
			}
			//******** контрольное закрытие соединения
			error_check (10000); 
  	  send_Uart_str(USART2,"AT+CIPCLOSE\r\n");
  	  do{
	  		read_str_uart(Buff_Uart);
	    	}
  		while((!strstr(Buff_Uart,"MUX=1")) && (!strstr(Buff_Uart,"CLOSED"))&& (!strstr(Buff_Uart,"ERROR")) && (TIM4->CR1 & TIM_CR1_CEN));
	  	TIM4->EGR |= TIM_EGR_UG;  TIM4->CR1 &= ~TIM_CR1_CEN;     // stop таймера	
			//delay_ms (200);
				if(rst++ > 4){
					rst = 0; 
					//WriteString( 7, 0, "Сброс", YELLOW); 
					LogOut_wifi(x_print,   y_print, "Reset", &font_log, GREEN, BLACK);		
					//delay_ms (200); 
				  return ret_ESP = 1; 
				}
				else  {
				  return ret_ESP = 9; 
				}
		}
		else if ((Buff_Uart)||(!Buff_Uart))
		{
			//WriteString( 7, 0, "time out, return" , WHITE);
			LogOut_wifi(x_print,   y_print, "Time out, return 9", &font_log, GREEN, BLACK);		
			//delay_ms (500);
			if (repite<3){
				repite++;
		  	return ret_ESP = 9;				
			}
			else {
				repite=0;				
			  return ret_ESP = 1;
			}
		}		
}
//********************************************************************************************
//
// 10 ****         ОТПРАВКА ДАННЫХ НА СЕРВЕР narodmon.ru
//
//********************************************************************************************
unsigned char send_data_AT (void)
{
	static uint8_t repite;
	
		//WriteString( 7, 0, "Отправка данных!" , GREEN);
		//delay_ms (50);
		error_check (10000); 
//	  arr_data_clear();
//		array_clear();	
	
		sprintf( array,"AT+CIPSEND=%u\r\n", len_data); 

	  send_Uart_str(USART2,array);
	  do{
			read_str_uart(Buff_Uart);
		}
		while((!strstr(Buff_Uart,">")) && (!strstr(Buff_Uart,"ERROR"))&& (!strstr(Buff_Uart,"link is not"))&& (TIM4->CR1 & TIM_CR1_CEN));
		TIM4->EGR |= TIM_EGR_UG;  TIM4->CR1 &= ~TIM_CR1_CEN;     // stop таймера	  
		
		if (strstr(Buff_Uart,">"))
		{
			error_check (10000); 
	    send_Uart_str(USART2,arr_data);
			hard_reset = 0;   // т.к. успешно отправлено, сбрасуем флаг "железного" ресета (через вывод МС)
			send_OK = 10; // флаг отправки данных
	    do{
			  read_str_uart(Buff_Uart);
		  }
   		while((!strstr(Buff_Uart,"SEND")) && (!strstr(Buff_Uart,"ERROR"))&& (TIM4->CR1 & TIM_CR1_CEN));
		  TIM4->EGR |= TIM_EGR_UG;  TIM4->CR1 &= ~TIM_CR1_CEN;     // stop таймера	
			
			if (strstr(Buff_Uart,"SEND"))
  		{			
	  			//WriteString( 8, 0, "Успешно         ", MAGENTA);
				  //LogOut_wifi(x_print,   y_print, "Send OK, narodmon", &font_log, GREEN, BLACK);		
					buff_clear ();
		  	  return ret_ESP = 11;	
		  }
			else if (strstr(Buff_Uart,"ERROR"))			
			{
					//WriteString( 8, 0, "Ошибка, отмена  ", RED);	
          LogOut_wifi(x_print,   y_print, "Narodmon send error,", &font_log, GREEN, BLACK);						
			    return ret_ESP = 12;		
			}
		}
		else if (strstr(Buff_Uart,"link is not"))
		{
			//WriteString( 8, 0, "Нет подключения", RED);	
			LogOut_wifi(x_print,   y_print, "Not connect", &font_log, GREEN, BLACK);		
			//delay_ms (100);
			return ret_ESP = 9;										
		}
		else if (strstr(Buff_Uart,"ERROR"))
		{
			//WriteString( 8, 0, "Ошибка, возврат", RED);	
			LogOut_wifi(x_print,   y_print, "Error, return", &font_log, GREEN, BLACK);		
			//delay_ms (100);
			return ret_ESP = 1;										
		}
		else if ((Buff_Uart)||(!Buff_Uart))
		{
			//WriteString( 8, 0, "time out, return" , WHITE);
			LogOut_wifi(x_print,   y_print, "Time out, return 10", &font_log, GREEN, BLACK);		
			//delay_ms (500); 
			if (repite<3){
				repite++;
		  	return ret_ESP = 5;				
			}
			else {
				repite=0;				
			  return ret_ESP = 1;
			}
		}		
}

//********************************************************************************************
//
// 11 ****         ПОЛУЧЕНИЕ ОТВЕТА ОТ СЕРВЕРА
//
//********************************************************************************************
unsigned char read_ret_server (void)
{
	  error_check (2000); 
	  arr_data_clear();
		array_clear();	
	
	  do{
			read_str_uart(Buff_Uart);
		}
		while((!strstr(Buff_Uart,"OK")) && (!strstr(Buff_Uart,"CLOSED"))&& (TIM4->CR1 & TIM_CR1_CEN));
		TIM4->EGR |= TIM_EGR_UG;  TIM4->CR1 &= ~TIM_CR1_CEN;     // stop таймера	  
		//LCD_Fill(BLACK);
		if ((strstr(Buff_Uart,"OK"))||(strstr(Buff_Uart,"CLOSED")))
		{			  
	  	//WriteString( 0, 0, "Получено        ", MAGENTA);	
			LogOut_wifi(x_print,   y_print, "Narodmon send end", &font_log, GREEN, BLACK);		
	   	//delay_ms (200);
		}
		else if ((Buff_Uart)||(!Buff_Uart))
		{
			//WriteString( 0, 0, "Время вышло" , WHITE);
			LogOut_wifi(x_print,   y_print,  "Time out 11", &font_log, GREEN, BLACK);		
			//delay_ms (500);
		 //	return ret_ESP = 1;
		}	
			//WriteString( 1, 0, (Buff_Uart+1), BLUE);	
		//LogOut_wifi(x_print,   y_print+18, (Buff_Uart+1), &font_log, GREEN, BLACK);		
		//delay_ms (1000);
		return ret_ESP = 12;	
}

//********************************************************************************************
//
// 12 ****         ЗАКРЫТИЕ СОЕДИНЕНИЯ
//
//********************************************************************************************
unsigned char closed_connect  (void)
{
		error_check (1000); 
	  arr_data_clear();
		array_clear();	
	
	  send_Uart_str(USART2,"AT+CIPCLOSE\r\n");
	  do{
			read_str_uart(Buff_Uart);
		}
		while((!strstr(Buff_Uart,"CLOSED")) && (!strstr(Buff_Uart,"ERROR"))&& (!strstr(Buff_Uart,"link is not"))&& (TIM4->CR1 & TIM_CR1_CEN));
		TIM4->EGR |= TIM_EGR_UG;  TIM4->CR1 &= ~TIM_CR1_CEN;     // stop таймера	  
		//LCD_Fill(BLACK);
		if (strstr(Buff_Uart,"CLOSED"))
		{
	  	//WriteString( 2, 0, "Закрытие", YELLOW);
       LogOut_wifi(x_print,   y_print, "Narodmon closed", &font_log, GREEN, BLACK);					
	   	//delay_ms (200);
		}
		else if ((strstr(Buff_Uart,"ERROR"))|| (strstr(Buff_Uart,"link is not")))
		{
			//WriteString( 2, 0, "Закрыто, возврат", YELLOW);	
			LogOut_wifi(x_print,   y_print, "Closed, return", &font_log, GREEN, BLACK);		
			//delay_ms (200);						
		}
		else if ((Buff_Uart)||(!Buff_Uart))
		{
			//WriteString( 2, 0, "time out, return" , WHITE);
			LogOut_wifi(x_print,   y_print, "Time out, return 12", &font_log, GREEN, BLACK);		
			//delay_ms (500);
		}	
		return ret_ESP = 13;		
}



//  uart_send_string("AT+CIPSTART,\"www.yandex.ru\",80\r\n");    // с какого сайта хотим скачать
//  uart_send_string("AT+CIPSEND,\"GET /\r\n\"");    // какую страницу
// "<dif id=\"current_time"

//********************************************************************************************
//
// 13  ****         ПОДКЛЮЧЕНИЕ К СЕРВЕРУ
//
//********************************************************************************************
unsigned char connect_serv_AT_time(void)
{ static uint8_t rst;
  static uint8_t repite;
	
		error_check (10000); 
	  arr_data_clear();
		array_clear();	
	
	  send_Uart_str(USART2,"AT+CIPSTART=\"UDP\",\"europe.pool.ntp.org\",123\r\n"); //130.102.128.23 

	  do{
			read_str_uart(Buff_Uart);
		}
		while((!strstr(Buff_Uart,"OK"))&& (!strstr(Buff_Uart,"CONNECT")) && (!strstr(Buff_Uart,"CLOSED"))&& (!strstr(Buff_Uart,"ERROR")) && (TIM4->CR1 & TIM_CR1_CEN));
		TIM4->EGR |= TIM_EGR_UG;  TIM4->CR1 &= ~TIM_CR1_CEN;     // stop таймера	  
		
		if ((strstr(Buff_Uart,"OK"))||(strstr(Buff_Uart,"CONNECT")))
		{
			//WriteString( 6, 0, "Соединен!            " , GREEN);
			LogOut_wifi(x_print,   y_print, "Connect server time", &font_log, GREEN, BLACK);		
			rst = 0;
			//delay_ms (200);
			return ret_ESP = 14;						
		}
		else
		if ((strstr(Buff_Uart,"CLOSED"))||(strstr(Buff_Uart,"ERROR")) )
		{
			sprintf (array, "CLOSED %u repite", rst);
			if (!rst){
				//WriteString( 6, 0, array, RED);	
				LogOut_wifi(x_print,   y_print, array, &font_log, GREEN, BLACK);		
				//delay_ms (200);
				}
			else {
				//WriteString( 6, 0, array, YELLOW);
				LogOut_wifi(x_print,   y_print, array, &font_log, GREEN, BLACK);		
				//delay_ms (200);
			}
			//******** контрольное закрытие соединения
			error_check (10000); 
  	  send_Uart_str(USART2,"AT+CIPCLOSE\r\n");
  	  do{
	  		read_str_uart(Buff_Uart);
	    	}
  		while((!strstr(Buff_Uart,"MUX=1")) && (!strstr(Buff_Uart,"CLOSED"))&& (!strstr(Buff_Uart,"ERROR")) && (TIM4->CR1 & TIM_CR1_CEN));
	  	TIM4->EGR |= TIM_EGR_UG;  TIM4->CR1 &= ~TIM_CR1_CEN;     // stop таймера	
			//delay_ms (200);
				if(rst++ > 4){
					rst = 0; 
					//WriteString( 7, 0, "Сброс", YELLOW); 
					LogOut_wifi(x_print,   y_print, "Error", &font_log, GREEN, BLACK);		
					//delay_ms (200); 
			if (repite<3){
				repite++;
		  	return ret_ESP = 12;				
			}
			else {
				repite=0;				
			  return ret_ESP = 1;
			}
				}
				else  {
				  return ret_ESP = 13; 
				}
		}
		else //if ((Buff_Uart)||(!Buff_Uart))
		{
			//WriteString( 7, 0, "time out, return" , WHITE);
			LogOut_wifi(x_print,   y_print, "Time out, return 13", &font_log, GREEN, BLACK);	
			send_Uart_str(USART2,"AT+CIPCLOSE\r\n");
			//delay_ms (500);
			return ret_ESP = 13;
		}		
}
//********************************************************************************************
//
// 14 ****         ОТПРАВКА ДАННЫХ НА СЕРВЕР
//
//********************************************************************************************
const int NTP_PACKET_SIZE= 48; // NTP time stamp is in the first 48 bytes of the message	
uint8_t packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

unsigned char send_data_AT_time (void)
{
static uint8_t repite;    
	  
		for(uint8_t i =0; i<NTP_PACKET_SIZE;i++){
			  packetBuffer[i]=0;
		}
    // Initialize values needed to form NTP request
    // (see URL above for details on the packets)
    packetBuffer[0] = 0xE3;   // LI, Version, Mode
    packetBuffer[1] = 0;     // Stratum, or type of clock
    packetBuffer[2] = 6;     // Polling Interval
    packetBuffer[3] = 0xEC;  // Peer Clock Precision
	  // 8 bytes of zero for Root Delay & Root Dispersion
    packetBuffer[12]  = 49;
    packetBuffer[13]  = 0x4E;
    packetBuffer[14]  = 49;
    packetBuffer[15]  = 52;
	
		array_clear ();
		sprintf( array,"AT+CIPSEND=%u\r\n", NTP_PACKET_SIZE); 
		error_check (20000); 
	  send_Uart_str(USART2,array);
	
	  do{
			read_str_uart(Buff_Uart);
		}
		while((!strstr(Buff_Uart,">")) && (!strstr(Buff_Uart,"ERROR"))&& (!strstr(Buff_Uart,"link is not"))&& (TIM4->CR1 & TIM_CR1_CEN));
		TIM4->EGR |= TIM_EGR_UG;  TIM4->CR1 &= ~TIM_CR1_CEN;     // stop таймера	  
		
		if (strstr(Buff_Uart,">"))
		{
			error_check (20000); 
			for(uint8_t i =0; i<NTP_PACKET_SIZE;i++){
			  send_Uart(USART2,packetBuffer[i]);
			}
	    //send_Uart_str(USART2,packetBuffer);
			hard_reset = 0;   // т.к. успешно отправлено, сбрасуем флаг "железного" ресета (через вывод МС)
			send_OK = 10; // флаг отправки данных
	    do{
			  read_str_uart(Buff_Uart);
		  }
   		while((!strstr(Buff_Uart,"SEND")) && (!strstr(Buff_Uart,"ERROR"))&& (TIM4->CR1 & TIM_CR1_CEN));
		  TIM4->EGR |= TIM_EGR_UG;  TIM4->CR1 &= ~TIM_CR1_CEN;     // stop таймера	
			
			if (strstr(Buff_Uart,"SEND"))
  		{			
	  			//WriteString( 8, 0, "Успешно         ", MAGENTA);
				  //LogOut_wifi(x_print,   y_print, "Send to time server OK", &font_log, GREEN, BLACK);		
					//buff_clear ();
		  	  return ret_ESP = 15;	
		  }
			else if (strstr(Buff_Uart,"ERROR"))			
			{
					//WriteString( 8, 0, "Ошибка, отмена  ", RED);	
		    	LogOut_wifi(x_print,   y_print, "Error, cancel", &font_log, GREEN, BLACK);				
			    return ret_ESP = 14;		
			}
		}
		else if (strstr(Buff_Uart,"link is not"))
		{
			//WriteString( 8, 0, "Нет подключения", RED);	
			LogOut_wifi(x_print,   y_print, "Not connect", &font_log, GREEN, BLACK);
			//delay_ms (100);
			return ret_ESP = 13;										
		}
		else if (strstr(Buff_Uart,"ERROR"))
		{
			//WriteString( 8, 0, "Ошибка, возврат", RED);	
			LogOut_wifi(x_print,   y_print, "Error, return", &font_log, GREEN, BLACK);
			//delay_ms (100);
			return ret_ESP = 13;										
		}
		else if ((Buff_Uart)||(!Buff_Uart))
		{
			//WriteString( 8, 0, "time out, return" , WHITE);
			LogOut_wifi(x_print,   y_print, "Time out, return 14", &font_log, GREEN, BLACK);
			//delay_ms (500); 
			if (repite<5){
				repite++;
		  	return ret_ESP = 13;				
			}
			else {
				repite=0;				
			  return ret_ESP = 1;
			}
		}		
}

//********************************************************************************************
//
// 15 ****         ПОЛУЧЕНИЕ ОТВЕТА ОТ СЕРВЕРА
//
//********************************************************************************************
RTC_DateTimeTypeDef RTC_DateTime_ntp;
extern RTC_DateTimeTypeDef RTC_DateTime, RTC_DateTime_old;
extern uint32_t RTC_Counter ;

#define MAKE_RGB565(r, g, b) TFT9341_MAKE_RGB565(r, g, b)
#define CLOCK_TIME_COLOR MAKE_RGB565(0x00, 0xff, 0x00)


unsigned char read_ret_server_time (void)
{
static uint8_t repite;   
	
	  int acksize = NTP_PACKET_SIZE + 1 + 2 + 8; // ESP8266 adds a space, a CRLF and starts with "+IPD,48:"
	
	  error_check (20000); 
	  arr_data_clear();
		array_clear();	
		
	  do{
			read_str_uart(Buff_Uart);
		}
		while((!strstr(Buff_Uart,"+IPD,48")) && (TIM4->CR1 & TIM_CR1_CEN));
		TIM4->EGR |= TIM_EGR_UG;  TIM4->CR1 &= ~TIM_CR1_CEN;     // stop таймера	  
		
		if (strstr(Buff_Uart,"+IPD,48"))
		{			
      for(uint8_t i=0; i<48; i++){
				packetBuffer[i]=Buff_Uart[i+9];
			}
		 unsigned long highWord = (packetBuffer[40]<<8) + packetBuffer[41];
     unsigned long lowWord = (packetBuffer[42]<<8) + packetBuffer[43];  
     // combine the four bytes (two words) into a long integer
     // this is NTP time (seconds since Jan 1 1900):
     unsigned long secsSince1900 = highWord << 16 | lowWord;
		  // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
     const unsigned long seventyYears  = 2208988800UL;
     // subtract seventy years:
     unsigned long epoch = secsSince1900 - seventyYears; 
     unsigned long DST = 60*60*3; // adjust to your GMT+DST 
     unsigned long timestamp = epoch + DST;		
		 RTC_GetDateTime(timestamp, &RTC_DateTime_ntp);
		 RTC_Counter = RTC_GetCounter();
	   RTC_GetDateTime(RTC_Counter, &RTC_DateTime);
		 RTC_DateTime_old=RTC_DateTime;
			
		 LogOut_wifi(x_print,   y_print, "Get time OK", &font_log, GREEN, BLACK);
			
		 if ((RTC_DateTime_ntp.RTC_Year>=2019)
			 &&(RTC_DateTime_ntp.RTC_Month<13)
			 &&(RTC_DateTime_ntp.RTC_Date<32)
		   &&(RTC_DateTime_ntp.RTC_Minutes<60)
		   &&(RTC_DateTime_ntp.RTC_Minutes>0)
//		 	 &&(RTC_DateTime.RTC_Year==RTC_DateTime_ntp.RTC_Year)
			 &&(RTC_DateTime.RTC_Month== RTC_DateTime_ntp.RTC_Month)
			 &&(RTC_DateTime.RTC_Date== RTC_DateTime_ntp.RTC_Date)	
		 ){

       clock_gui_set_time_color(MAKE_RGB565(0xff, 0xff, 0xff));						

			 
			 if ((RTC_DateTime.RTC_Year!=RTC_DateTime_ntp.RTC_Year) ||    
			  (RTC_DateTime.RTC_Month!= RTC_DateTime_ntp.RTC_Month) ||  
			  (RTC_DateTime.RTC_Date!= RTC_DateTime_ntp.RTC_Date))		{
					clock_gui_set_monthday(RTC_DateTime.RTC_Date);
					clock_gui_set_month(RTC_DateTime.RTC_Month-1);
					clock_gui_set_weekday(RTC_DateTime.RTC_Wday);
				}
			 
			 if ((RTC_DateTime.RTC_Hours!=RTC_DateTime_ntp.RTC_Hours)||		  
			  (RTC_DateTime.RTC_Minutes!=RTC_DateTime_ntp.RTC_Minutes)) clock_gui_set_time(RTC_DateTime.RTC_Hours, RTC_DateTime.RTC_Minutes);   
		
			 
			 
			 RTC_DateTime.RTC_Year=RTC_DateTime_ntp.RTC_Year;
			 RTC_DateTime.RTC_Month= RTC_DateTime_ntp.RTC_Month;
			 RTC_DateTime.RTC_Date= RTC_DateTime_ntp.RTC_Date;		
			 
			 RTC_DateTime.RTC_Hours=RTC_DateTime_ntp.RTC_Hours;			 
			 RTC_DateTime.RTC_Minutes=RTC_DateTime_ntp.RTC_Minutes;
			 RTC_DateTime.RTC_Seconds=RTC_DateTime_ntp.RTC_Seconds;
			 
		   RTC_SetCounter(timestamp);//RTC_GetRTC_Counter(&RTC_DateTime) 

			 delay_ms(500);
			 
        clock_gui_set_time_color(CLOCK_TIME_COLOR);			 
		 }
			
		}		
		else if ((Buff_Uart)||(!Buff_Uart))
		{
			LogOut_wifi(x_print,   y_print, "Get time error", &font_log, GREEN, BLACK);
			
			if (repite<3){
				repite++;
		  	return ret_ESP = 14;				
			}
			else {
				repite=0;				
			  return ret_ESP = 16;
			}
		}	
		return ret_ESP = 16;	
}

//********************************************************************************************
//
// 16 ****         ЗАКРЫТИЕ СОЕДИНЕНИЯ
//
//********************************************************************************************
unsigned char closed_connect_time  (void)
{
		error_check (1000); 
	  arr_data_clear();
		array_clear();	
	
	  send_Uart_str(USART2,"AT+CIPCLOSE\r\n");
	  do{
			read_str_uart(Buff_Uart);
		}
		while((!strstr(Buff_Uart,"CLOSED")) && (!strstr(Buff_Uart,"ERROR"))&& (!strstr(Buff_Uart,"link is not"))&& (TIM4->CR1 & TIM_CR1_CEN));
		TIM4->EGR |= TIM_EGR_UG;  TIM4->CR1 &= ~TIM_CR1_CEN;     // stop таймера	  
		//LCD_Fill(BLACK);
		if (strstr(Buff_Uart,"CLOSED"))
		{
	  	//WriteString( 2, 0, "Закрытие", YELLOW);	
			LogOut_wifi(x_print,   y_print, "Server time closed", &font_log, GREEN, BLACK);
	   	//delay_ms (200);
		}
		else if ((strstr(Buff_Uart,"ERROR"))|| (strstr(Buff_Uart,"link is not")))
		{
			//WriteString( 2, 0, "Закрыто, возврат", YELLOW);	
			LogOut_wifi(x_print,   y_print, "Closed, return", &font_log, GREEN, BLACK);
			//delay_ms (200);						
		}
		else if ((Buff_Uart)||(!Buff_Uart))
		{
			//WriteString( 2, 0, "time out, return" , WHITE);
			LogOut_wifi(x_print,   y_print, "Time out, return 16", &font_log, GREEN, BLACK);
			//delay_ms (500);
		}	
		return ret_ESP = 17;		
}



void arr_data_clear(void)															//	очищаем буфер
{
	uint16_t i;
	
	for(i=0;i<256;i++)
	{
		arr_data[i]=0;
	}
}
void array_clear(void)															//	очищаем буфер
{
	uint16_t i;
	
	for(i=0;i<256;i++)
	{
		array[i]=0;
	}

}


#ifndef __WEB_ESP8266_H
#define	__WEB_ESP8266_H
//#include "stm32f10x.h" 

extern unsigned char send_OK;
extern unsigned char ret_ESP;
extern unsigned char err_rst;
extern unsigned char stop_run;

void error_check (unsigned int delay_1);
void stop_error_check (void);
unsigned char send_OK_print (void);

void arr_sum_clear(void);	//	очищаем массив
void arrays_clear(void);	//	очищаем массив

unsigned char connect_to_AP(void);
char * strstr(const char *str, const char *substr); 

unsigned char response_AT (void);
unsigned char reset_AT (void);
unsigned char mode_AT (void);
unsigned char read_MAC_AT(void);
unsigned char select_clnt_AT (void);
unsigned char connect_AT(void);
unsigned char rd_connect_AT(void);
unsigned char cipmux_AT(void);
unsigned char lead_data_sensor(void);
unsigned char connect_serv_AT(void);
unsigned char send_data_AT (void);
unsigned char read_ret_server (void);
unsigned char closed_connect  (void);
unsigned char connect_serv_AT_time(void);
unsigned char send_data_AT_time (void);
unsigned char read_ret_server_time (void);
unsigned char closed_connect_time  (void);

void arr_data_clear(void);															//	очищаем буфер
void array_clear(void);

#endif	/* __WEB_ESP8266_H */


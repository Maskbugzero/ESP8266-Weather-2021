#ifndef __WIFISTA_H
#define __WIFISTA_H

#include "sys.h"

void atk_8266_init(void);
void atk_8266_at_response(u8 mode);
u8* atk_8266_check_cmd(u8 *str);
u8 atk_8266_send_data(u8 *data,u8 *ack,u16 waittime);
u8 atk_8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime);
u8 atk_8266_quit_trans(void);
u8 atk_8266_consta_check(void);
void atk_8266_get_wanip(u8* ipbuf);
void atk_8266_get_ip(u8 x,u8 y);
u8 atk_8266_wifi_config(void);

#endif




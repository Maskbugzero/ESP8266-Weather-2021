#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h" 		 	 
#include "lcd.h"  
#include "key.h"     
#include "malloc.h"
#include "sdio_sdcard.h"  
#include "w25qxx.h"    
#include "ff.h"  
#include "exfuns.h"   
#include "text.h"	
#include "touch.h"		
#include "usart3.h"
#include "wifista.h"
#include "weather.h"
#include "cJSON.h" 
#include "picture.h"
#include "rtc.h"
#include "dht11.h"


//static void create_complex(void);
void show_weather(void);
void show_time(void);

 int main(void)
 {	 
	u8 key,fontok=0; 
	u16 t=0; 
	u8 min;	 
	u8 hour;
	u8 constate=0;	//����״̬			    
	u8 temperature;  	    
	u8 humidity;
	delay_init();	    	 //��ʱ������ʼ��	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	
 	LED_Init();		  			//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();					//��ʼ������
	LCD_Init();			   		//��ʼ��LCD   
	RTC_Init();
 	while(DHT11_Init())	//DHT11��ʼ��	
	{
	  printf("DHT11 ����\r\n");
 		delay_ms(200);
	}
	W25QXX_Init();				//��ʼ��W25Q128
	tp_dev.init();				//��ʼ��������
	usart3_init(115200);		//��ʼ������3 
 	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
	exfuns_init();				//Ϊfatfs��ر��������ڴ�  
 	f_mount(fs[0],"0:",1); 		//����SD�� 
 	f_mount(fs[1],"1:",1); 		//����FLASH.
	key=KEY_Scan(0);  
	if(key==KEY0_PRES&&((tp_dev.touchtype&0X80)==0))//ǿ��У׼
	{
		LCD_Clear(WHITE);		//����0
		TP_Adjust();  			//��ĻУ׼ 
		TP_Save_Adjdata();	  
		LCD_Clear(WHITE);		//����
	}
	fontok=font_init();			//����ֿ��Ƿ�OK
	if(fontok||key==KEY1_PRES)	//��Ҫ�����ֿ�				 
	{
		LCD_Clear(WHITE);		//����
 		POINT_COLOR=WHITE;		//��������Ϊ��ɫ	   	   	  
		LCD_ShowString(60,50,200,16,16,"ALIENTEK STM32");
		while(SD_Init())		//���SD��
		{
			LCD_ShowString(60,70,200,16,16,"SD Card Failed!");
			delay_ms(200);
			LCD_Fill(60,70,200+60,70+16,WHITE);
			delay_ms(200);		    
		}								 						    
		LCD_ShowString(60,70,200,16,16,"SD Card OK");
		LCD_ShowString(60,90,200,16,16,"Font Updating...");
		key=update_font(20,110,16,"0:");//��SD������
		while(key)//����ʧ��		
		{			 		  
			LCD_ShowString(60,110,200,16,16,"Font Update Failed!");
			delay_ms(200);
			LCD_Fill(20,110,200+20,110+16,WHITE);
			delay_ms(200);		       
		} 		  
		LCD_ShowString(60,110,200,16,16,"Font Update Success!");
		delay_ms(1500);	
		LCD_Clear(WHITE);//����	       
	} 
	

	delay_ms(200);
	LED0 = 0;
	show_weather();
	POINT_COLOR = WHITE;
	delay_ms(1000);
	delay_ms(1000);
	get_current_weather();
	delay_ms(200);
	get_3days_weather();
	delay_ms(200);
	get_beijing_time();
	
	
	min = calendar.min;
	hour = calendar.hour;
	while(1)
	{
		key = KEY_Scan(0); 
		if(key==KEY0_PRES)
		{
	   atk_8266_wifi_config();
	   printf("wifi ok!\r\n");		
		}
		else if(key==KEY1_PRES)
		{
			get_current_weather();
			delay_ms(200);
			get_3days_weather();
		}
		else if(key==WKUP_PRES)
		{
			get_beijing_time();
		}
		delay_ms(10);
		t++;
		if(t==1000)
		{
			t=0;
		}
		else if((t%100)==0)
		{
			LED1 = !LED1;
			show_time();
		}
		
		if(min!=calendar.min)  //ÿ���ӻ�ȡ��ǰ����	
		{
			get_current_weather();			
			min = calendar.min; 
		}
		if(hour!=calendar.hour)  //ÿСʱ��ȡ3��������У׼ʱ��	
		{
			get_3days_weather(); 
			get_beijing_time();
			hour = calendar.hour;
		}

		atk_8266_at_response(1);
		
		if(t%10==0)			//ÿ100ms��ȡһ��
		{									  
 	   	DHT11_Read_Data(&temperature,&humidity);	//��ȡ��ʪ��ֵ					    
			LCD_ShowNum(220+72,65,temperature,2,16);	//��ʾ�¶�	   		   
			LCD_ShowNum(220+72,85,humidity,2,16);		//��ʾʪ��	
		}				   
		
	}
}
  
void show_weather(void)
{
	u8 res;
	file = mymalloc(SRAMIN,sizeof(FIL));
	res=f_open(file,(const TCHAR*)APP_ASCII_5427,FA_READ);//���ļ�
	if(res==FR_OK)
	{
		asc2_5427 = mymalloc(SRAMIN,file->fsize);
		if(asc2_5427 != NULL)
		{
			res = f_read(file,asc2_5427,file->fsize,&br);
		}
		f_close(file);
	}
	LCD_Clear(BLACK);
	POINT_COLOR = WHITE;
	BACK_COLOR = BLACK;
	Show_Str(0,0,lcddev.width,lcddev.height,"�й�",16,0);
	LCD_ShowString(160,0,lcddev.width,lcddev.height,16,"0000-00-00");
	LCD_ShowString(255,0,lcddev.width,lcddev.height,16,"00:00:00");
//	POINT_COLOR = RED;
	LCD_Draw_Picture(30,5,130,105,(u8 *)gImage_sunny);
	gui_show_num(140,22,2,WHITE,54,0,0x80);
//	POINT_COLOR = WHITE;
	Show_Str(200,25,lcddev.width,lcddev.height,"��",24,0);
//	POINT_COLOR = GBLUE;
	Show_Str(220,25,lcddev.width,lcddev.height,"����",16,0);
	Show_Str(220,45,lcddev.width,lcddev.height,"����",16,0);
  Show_Str(220,65,lcddev.width,lcddev.height,"�����¶�:  C",16,0);
	Show_Str(220,85,lcddev.width,lcddev.height,"����ʪ��:  %",16,0);
//  LCD_ShowString(220,65,20,20,16,(u8 *)gbkstr);                    //��ʾ����
//	Show_Str(220,85,lcddev.width,lcddev.height,"����  0Km/h",16,0);
	
//	POINT_COLOR = WHITE;
	LCD_DrawLine(0,105,lcddev.width,105);
	LCD_DrawLine(105,130,105,220);
	LCD_DrawLine(215,130,215,220);
	LCD_Draw_Picture(0,140,100,240,(u8 *)gImage_sunny);
	LCD_Draw_Picture(110,140,210,240,(u8 *)gImage_sunny);
	LCD_Draw_Picture(220,140,320,240,(u8 *)gImage_sunny);
//	POINT_COLOR = YELLOW;
	Show_Str(30,118,lcddev.width,lcddev.height,"����",12,0);

	LCD_ShowString(30,130,lcddev.width,lcddev.height,12,"00/00");
//	POINT_COLOR = GREEN;
	Show_Str(140,118,lcddev.width,lcddev.height,"����",12,0);

	LCD_ShowString(140,130,lcddev.width,lcddev.height,12,"00/00");
//	POINT_COLOR = MAGENTA;
	Show_Str(250,118,lcddev.width,lcddev.height,"����",12,0);	

	LCD_ShowString(250,130,lcddev.width,lcddev.height,12,"00/00");
	
	myfree(SRAMIN,file);
	myfree(SRAMIN,asc2_5427);
}


void show_time(void)
{
	LCD_ShowNum(160,0,calendar.w_year,4,16);
	LCD_ShowxNum(200,0,calendar.w_month,2,16,0x80);
	LCD_ShowxNum(224,0,calendar.w_date,2,16,0x80);	
	LCD_ShowxNum(255,0,calendar.hour,2,16,0x80);
	LCD_ShowxNum(279,0,calendar.min,2,16,0x80);	
	LCD_ShowxNum(303,0,calendar.sec,2,16,0x80);	
}


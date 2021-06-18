实验器材
========
战舰V3\精英STM32F103开发板
	
硬件资源
========
1,DS0(连接在PB5)<br>
2,串口1(波特率:115200,PA9/PA10连接在板载USB转串口芯片CH340上面)<br>
3,串口3(波特率:115200，PB10/PB11)<br>
4,ALIENTEK 2.8/3.5/4.3/7寸TFTLCD模块(通过FSMC驱动,FSMC_NE4接LCD片选/A10接RS)<br>
5,按键KEY0(PE4)/KEY1(PE3)/KEY_UP(PA0,也称之为WK_UP)<br>
6,SD卡,通过SDIO(SDIO_D0~D4(PC8~PC11),SDIO_SCK(PC12),SDIO_CMD(PD2))连接 <br>
7,W25Q128(SPI FLASH芯片,连接在SPI2上)<br>
8,24C02<br>
9,ESP8266-12S WIFI模块1个.<br>
10,DHT11模块1个.(Data连接在PE11上)<br>
	
实现功能
========
1,微信小程序配网<br>
2,室内温湿度测量及显示<br>
3,室外天气数据获取及显示<br>
4,时间显示及网络校准<br>

连接方式
========
模块与带有无线网卡的电脑或其他wifi设备连接：采用wifi连接<br>
模块与开发板连接（TTL串口方式）：<br>

ATK-ESP8266 WIFI模块与精英板连接方式
-----------------------------------
TXD<------------>PB11<br>
RXD<------------>PB10<br>
GND<------------>GND<br>
VCC<------------>3.3V\5V<br>

DHT11模块与精英板连接方式
-------------------------
Data<------------>PE11<br>

实验现象
========
[![RSEhTJ.png](https://z3.ax1x.com/2021/06/18/RSEhTJ.png)](https://imgtu.com/i/RSEhTJ)<br>
[![RSE5k9.jpg](https://z3.ax1x.com/2021/06/18/RSE5k9.jpg)](https://imgtu.com/i/RSE5k9)<br>
[![RSEIYR.jpg](https://z3.ax1x.com/2021/06/18/RSEIYR.jpg)](https://imgtu.com/i/RSEIYR)<br>

注意事项
========
1,4.3寸和7寸屏需要比较大电流,USB供电可能不足,请用外部电源适配器(推荐外接12V 1A电源).<br>
2,本例程在LCD_Init函数里面(在ILI93xx.c),用到了printf,如果不初始化串口1,将导致液晶无法显示!! <br>
3,字库更新时,需自备标准SD卡一张(即大卡,也可以用TF卡+卡套)，并拷贝光盘:5，SD卡根目录文件里面的所有内容到SD卡根目录,然后将SD卡插到开发板.<br>
4,对于战舰V3开发板,P8需要用跳线短接:PB10(TX)与GBC_RX，PB11(RX)与GBC_TX<br>
5,如果触摸屏不准，请按住KEY0不放，然后按复位，松开复位，进入触摸屏校准。此时松开KEY0，执行校准，即可对屏幕进行校准。 <br>

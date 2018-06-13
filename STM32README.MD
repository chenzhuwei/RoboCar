# 智创stm32报告
---
## 流程简介
```flow
a=>operation: 传感器测得数据
b=>operation: 将数据传输给stm32
c=>operation: STM32进行数据解析
d=>operation: STM32通过串口、SPI等方式将处理结果输出
a->b->c->d->e
```
---
# GPS功能


## 代码
###1. main函数
```c
#include "sys.h"
#include "delay.h"  
#include "usart.h"      
#include "usmart.h"			
#include "usart3.h"
#include "gps.h"
u8 USART1_TX_BUF[USART3_MAX_RECV_LEN]; 				
nmea_msg gpsx; 										
__align(4) u8 dtbuf[50];   							
const u8*fixmode_tbl[4]={"Fail","Fail"," 2D "," 3D "};	
	  
void Gps_Msg_Show(void)                         //将解析好的数据发送给串口1
{
 	float tp;		   	 
	tp=gpsx.longitude;	   
	sprintf((char *)dtbuf,"Longitude:%.5f %1c   ",tp/=100000,gpsx.ewhemi);	
	printf("\r\n%s\r\n",dtbuf);	   
	tp=gpsx.latitude;	   
	sprintf((char *)dtbuf,"Latitude:%.5f %1c   ",tp/=100000,gpsx.nshemi);
	printf("%s\r\n",dtbuf); 	 
	tp=gpsx.altitude;	   
 	sprintf((char *)dtbuf,"Altitude:%.1fm     ",tp/=10);	    		
    printf("%s\r\n",dtbuf);			   
	tp=gpsx.speed;	   
 	sprintf((char *)dtbuf,"Speed:%.3fkm/h     ",tp/=1000);		    	
    printf("%s\r\n",dtbuf);					     	   
	sprintf((char *)dtbuf,"UTC Date:%04d/%02d/%02d   ",gpsx.utc.year,gpsx.utc.month,gpsx.utc.date);	
	printf("%s\r\n",dtbuf);	    
	sprintf((char *)dtbuf,"UTC Time:%02d:%02d:%02d   ",gpsx.utc.hour,gpsx.utc.min,gpsx.utc.sec);	
	printf("%s\r\n",dtbuf);	  
}

int main(void)
{        
	u16 i,rxlen;
	u16 lenx;
	u8 key=0XFF;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);  
	uart_init(115200);		
	usart3_init(384200);  
	usmart_dev.init(168);	
	if(SkyTra_Cfg_Rate(5)!=0)	
	{
		do                                                   //得到串口3的数据
		{
			usart3_init(9600);		
	  	    SkyTra_Cfg_Prt(3);		
			usart3_init(38400);		
            key=SkyTra_Cfg_Tp(100000);
		}while(SkyTra_Cfg_Rate(5)!=0&&key!=0);
		delay_ms(500);
	}
	while(1)                                                 //将得到的数据通过协议进行分析
	{		
		delay_ms(1);
		if(USART3_RX_STA&0X8000)	
		{
			rxlen=USART3_RX_STA&0X7FFF;	
			for(i=0;i<rxlen;i++)USART1_TX_BUF[i]=USART3_RX_BUF[i];	   
 			USART3_RX_STA=0;		   	
			USART1_TX_BUF[i]=0;		
			GPS_Analysis(&gpsx,(u8*)USART1_TX_BUF);
			Gps_Msg_Show();			
 		}
		lenx++;	
	}
}
```
###NMEA-0183协议
NMEA 0183 是美国国家海洋电子协会（National Marine Electronics Association）为海用电子设备制定的标准格式。目前业已成了 GPS/北斗导航设备统一的 RTCM（Radio Technical Commission for Maritime services）标准协议。 
NMEA-0183 协议采用 ASCII 码来传递 GPS 定位信息，我们称之为帧。 
帧格式形如：\$aaccc,ddd,ddd,…,ddd*hh(CR)(LF) 
1. “\$”：帧命令起始位 
2. aaccc：地址域，前两位为识别符（aa），后三位为语句名（ccc） 
3. ddd…ddd：数据 
4. “\*”：校验和前缀（也可以作为语句数据结束的标志） 
5. hh：校验和（check sum），\$与*之间所有字符 ASCII 码的校验和（各字节做异或运算，得到校验和后，再转换 16 进制格式的 ASCII 字符） 
6. (CR)(LF)：帧结束，回车和换行符 
 	NMEA-0183 常用命令如表 2.2.1.1 所示： 
|序号 	|命令 	|说明 	|最大帧长|
| :----: | :----: | :----: | :----: |
|1 |\$GNGGA |GPS/北斗定位信息 |72| 
|2 |\$GNGSA |当前卫星信息 |65| 
|3 |\$GPGSV |可见 GPS 卫星信息 	|210| 
|4 |\$BDGSV |可见北斗卫星信息 	|210| 
|5 |\$GNRMC |推荐定位信息 	|70| 
|6 |\$GNVTG |地面速度信息 	|34| 
|7 |\$GNGLL |大地坐标信息 	|--| 
|8 |\$GNZDA |当前时间(UTC1)信息|--| 
####注 
1： 即协调世界时，相当于本初子午线(0 度经线)上的时间，北京时间比 UTC 早 8 个小时。
####接下来我们分别介绍这些命令。 
1. \$GNGGA（GPS 定位信息，Global Positioning System Fix Data） 
\$GNGGA 语句的基本格式如下（其中 M 指单位 M，hh 指校验和，CR 和 LF 代表回车换行，下同）： 
\$GNGGA,(1),(2),(3),(4),(5),(6),(7),(8),(9),M,(10),M,(11),(12)*hh(CR)(LF) 
(1)UTC 时间，格式为 hhmmss.ss； 
(2)纬度，格式为 ddmm.mmmmm（度分格式）； 
(3)纬度半球，N 或 S（北纬或南纬）； 
(4)经度，格式为 dddmm.mmmmm（度分格式）； 
(5)经度半球，E 或 W（东经或西经）； 
(6)GPS 状态，0=未定位，1=非差分定位，2=差分定位； 
(7)正在使用的用于定位的卫星数量（00~12） 
(8)HDOP 水平精确度因子（0.5~99.9） 
(9)海拔高度（-9999.9 到 9999.9 米） 
(10)大地水准面高度（-9999.9 到 9999.9 米） 
(11)差分时间（从最近一次接收到差分信号开始的秒数，非差分定位，此项为空） (12) 差分参考基站标号（0000 到 1023，首位 0 也将传送，非差分定位，此项为空) 举例如下： 
\$GNGGA,095528.000,2318.1133,N,11319.7210,E,1,06,3.7,55.1,M,-5.4,M,,0000*69 
2. \$GNGSA（当前卫星信息） 
\$GNGSA 语句的基本格式如下： 
\$GNGSA,(1),(2),(3),(3),(3),(3),(3),(3),(3),(3),(3),(3),(3),(3),(4),(5),(6)*hh(CR)(LF) 
(1)模式，M = 手动，A = 自动。 
(2)定位类型，1=未定位，2=2D 定位，3=3D 定位。 
(3)正在用于定位的卫星号（01~32） 
(4)PDOP 综合位置精度因子（0.5-99.9） 
(5)HDOP 水平精度因子 1（0.5-99.9） 
(6)VDOP 垂直精度因子（0.5-99.9）举例如下： 
\$GNGSA,A,3,14,22,24,12,,,,,,,,,4.2,3.7,2.1*2D 
\$GNGSA,A,3,209,214,,,,,,,,,,,4.2,3.7,2.1*21 注 1： 精度因子值越小，则准确度越高。 
 
3. \$GPGSV（可见卫星数，GPS Satellites in View） 
\$GPGSV 语句的基本格式如下：  
\$GPGSV, (1),(2),(3),(4),(5),(6),(7),...,(4),(5),(6),(7)*hh(CR)(LF) 
(1)GSV 语句总数。 
(2)本句 GSV 的编号。 
(3)可见卫星的总数（00~12，前面的 0 也将被传输）。 
(4)卫星编号（01~32，前面的 0 也将被传输）。 
(5)卫星仰角（00~90 度，前面的 0 也将被传输）。 
(6)卫星方位角（000~359 度，前面的 0 也将被传输）
(7)  信噪比（00~99dB，没有跟踪到卫星时为空）。 
注：每条 GSV 语句最多包括四颗卫星的信息，其他卫星的信息将在下一条\$GPGSV 语句中输出。  
举例如下： 
\$GPGSV,3,1,11,18,73,129,19,10,71,335,40,22,63,323,41,25,49,127,06*78 \$GPGSV,3,2,11,14,41,325,46,12,36,072,34,31,32,238,22,21,23,194,08*76 \$GPGSV,3,3,11,24,21,039,40,20,08,139,07,15,08,086,03*45 
4. \$BDGSV（可见卫星数，GPS Satellites in View） 
\$BDGSV 语句的基本格式如下：  
\$BDGSV, (1),(2),(3),(4),(5),(6),(7),...,(4),(5),(6),(7)*hh(CR)(LF) 
(1)GSV 语句总数。 
(2)本句 GSV 的编号。 
(3)可见卫星的总数（00~12，前面的 0 也将被传输）。 
(4)卫星编号（01~32，前面的 0 也将被传输）。 
(5)卫星仰角（00~90 度，前面的 0 也将被传输）。 
(6)卫星方位角（000~359 度，前面的 0 也将被传输） (7)  信噪比（00~99dB，没有跟踪到卫星时为空）。 
注：每条 GSV 语句最多包括四颗卫星的信息，其他卫星的信息将在下一条\$BDGSV 语句中输出。  举例如下： 
\$BDGSV,1,1,02,209,64,354,40,214,05,318,40*69 
5. \$GNRMC（推荐定位信息，Recommended Minimum Specific GPS/Transit Data） 
\$GNRMC 语句的基本格式如下：  
\$GNRMC,(1),(2),(3),(4),(5),(6),(7),(8),(9),(10),(11),(12)*hh(CR)(LF) 
(1)UTC 时间，hhmmss（时分秒） 
(2)定位状态，A=有效定位，V=无效定位 
(3)纬度 ddmm.mmmmm（度分） 
(4)纬度半球 N（北半球）或 S（南半球） 
(5)经度 dddmm.mmmmm（度分） 
(6)经度半球 E（东经）或 W（西经） 
(7)地面速率（000.0~999.9 节） 
(8)地面航向（000.0~359.9 度，以真北方为参考基准） 
(9)UTC 日期，ddmmyy（日月年） 
(10)磁偏角（000.0~180.0 度，前导位数不足则补 0） 
(11)磁偏角方向，E（东）或 W（西） 
(12)模式指示（A=自主定位，D=差分，E=估算，N=数据无效） 
举例如下： 
\$GNRMC,095554.000,A,2318.1327,N,11319.7252,E,000.0,005.7,081215,,,A*73 
6. \$GNVTG（地面速度信息，Track Made Good and Ground Speed） 
\$GNVTG 语句的基本格式如下：   
\$GNVTG,(1),T,(2),M,(3),N,(4),K,(5)*hh(CR)(LF) 
(1)以真北为参考基准的地面航向（000~359 度，前面的 0 也将被传输） 
(2)以磁北为参考基准的地面航向(000~359 度，前面的 0 也将被传输) 
(3)地面速率(000.0~999.9 节，前面的 0 也将被传输) 
(4)地面速率(0000.0~1851.8 公里/小时，前面的 0 也将被传输)  (5)  模式指示（A=自主定位，D=差分，E=估算，N=数据无效） 
举例如下： 
 	 	\$GNVTG,005.7,T,,M,000.0,N,000.0,K,A*11 
7. \$GNGLL（定位地理信息，Geographic Position） 
\$GNGLL 语句的基本格式如下： 
\$GNGLL,(1),(2),(3),(4),(5),(6),(7)*hh(CR)(LF)  
(1)纬度 ddmm.mmmmm（度分） 
(2)纬度半球 N（北半球）或 S（南半球） 
(3)经度 dddmm.mmmmm（度分） 
(4)经度半球 E（东经）或 W（西经） 
(5)UTC 时间：hhmmss（时分秒） 
(6)定位状态，A=有效定位，V=无效定位   
(7)模式指示（A=自主定位，D=差分，E=估算，N=数据无效） 
举例如下： 
\$GNGLL,2318.1330,N,11319.7250,E,095556.000,A,A*4F 
8. \$GNZDA（当前时间信息） 
\$GNZDA 语句的基本格式如下：   
\$GNZDA,(1),(2),(3),(4), (5), (6)*hh(CR)(LF)  
(1)UTC 时间：hhmmss（时分秒） 
(2)日 (3)  月 
(4)年 
(5)本地区域小时（NEO-6M 未用到，为 00） 
(6)本地区域分钟（NEO-6M 未用到，为 00） 
举例如下： 
\$GNZDA,095555.000,08,12,2015,00,00*4C 
NMEA-0183协议命令帧部分就介绍到这里，接下来我们看看NMEA-0183协议的校验，通过前面的介绍，我们知道每一帧最后都有一个 hh 的校验和，该校验和是通过计算\$与*之间所有字符 ASCII 码的异或运算得到，将得到的结果以 ASCII 字符表示就是该校验（hh）。 
例如语句：\$GNZDA,095555.000,08,12,2015,00,00*4C，校验和（红色部分参与计算）计算方法为： 
0X47 xor 0X4E xor 0X5A xor 0X44 xor 0X41 xor 0X2C xor 0X30 xor 0X39 xor 0X35 xor 0X35 xor 0X35 xor 0X35 xor 0X2E xor 0X30 xor 0X30 xor 0X30 xor 0X2C xor 0X30 xor 0X38 xor 0X2C xor 0X31 xor 0X32 xor 0X2C xor 0X32 xor 0X30 xor 0X31 xor 0X35 xor 0X2C xor 
0X30 xor 0X30 xor 0X2C xor 0X30 xor 0X30 
 	得到的结果就是 0X4C，用 ASCII 表示就是 4C。 

###代码
```c
#include "gps.h" 
#include "led.h" 
#include "delay.h" 								   
#include "usart3.h" 								   
#include "stdio.h"	 
#include "stdarg.h"	 
#include "string.h"	 
#include "math.h"

const u32 BAUD_id[9]={4800,9600,19200,38400,57600,115200,230400,460800,921600};
u8 NMEA_Comma_Pos(u8 *buf,u8 cx)
{	 		    
	u8 *p=buf;
	while(cx)
	{		 
		if(*buf=='*'||*buf<' '||*buf>'z')return 0XFF;
		if(*buf==',')cx--;
		buf++;
	}
	return buf-p;	 
}
u32 NMEA_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}
int NMEA_Str2num(u8 *buf,u8*dx)
{
	u8 *p=buf;
	u32 ires=0,fres=0;
	u8 ilen=0,flen=0,i;
	u8 mask=0;
	int res;
	while(1) 
	{
		if(*p=='-'){mask|=0X02;p++;}
		if(*p==','||(*p=='*'))break;
		if(*p=='.'){mask|=0X01;p++;}
		else if(*p>'9'||(*p<'0'))	
		{	
			ilen=0;
			flen=0;
			break;
		}	
		if(mask&0X01)flen++;
		else ilen++;
		p++;
	}
	if(mask&0X02)buf++;	
	for(i=0;i<ilen;i++)	
	{  
		ires+=NMEA_Pow(10,ilen-1-i)*(buf[i]-'0');
	}
	if(flen>5)flen=5;
	*dx=flen;	 		
	for(i=0;i<flen;i++)	
	{  
		fres+=NMEA_Pow(10,flen-1-i)*(buf[ilen+1+i]-'0');
	} 
	res=ires*NMEA_Pow(10,flen)+fres;
	if(mask&0X02)res=-res;		   
	return res;
}	  							 
void NMEA_GPGSV_Analysis(nmea_msg *gpsx,u8 *buf)
{
	u8 *p,*p1,dx;
	u8 len,i,j,slx=0;
	u8 posx;   	 
	p=buf;
	p1=(u8*)strstr((const char *)p,"$GPGSV");
	len=p1[7]-'0';							
	posx=NMEA_Comma_Pos(p1,3); 				
	if(posx!=0XFF)gpsx->svnum=NMEA_Str2num(p1+posx,&dx);
	for(i=0;i<len;i++)
	{	 
		p1=(u8*)strstr((const char *)p,"$GPGSV");  
		for(j=0;j<4;j++)
		{	  
			posx=NMEA_Comma_Pos(p1,4+j*4);
			if(posx!=0XFF)gpsx->slmsg[slx].num=NMEA_Str2num(p1+posx,&dx);
			else break; 
			posx=NMEA_Comma_Pos(p1,5+j*4);
			if(posx!=0XFF)gpsx->slmsg[slx].eledeg=NMEA_Str2num(p1+posx,&dx);
			else break;
			posx=NMEA_Comma_Pos(p1,6+j*4);
			if(posx!=0XFF)gpsx->slmsg[slx].azideg=NMEA_Str2num(p1+posx,&dx);
			else break; 
			posx=NMEA_Comma_Pos(p1,7+j*4);
			if(posx!=0XFF)gpsx->slmsg[slx].sn=NMEA_Str2num(p1+posx,&dx);
			else break;
			slx++;	   
		}   
 		p=p1+1;
	}   
}
void NMEA_BDGSV_Analysis(nmea_msg *gpsx,u8 *buf)
{
	u8 *p,*p1,dx;
	u8 len,i,j,slx=0;
	u8 posx;   	 
	p=buf;
	p1=(u8*)strstr((const char *)p,"$BDGSV");
	len=p1[7]-'0';							
	posx=NMEA_Comma_Pos(p1,3); 				
	if(posx!=0XFF)gpsx->beidou_svnum=NMEA_Str2num(p1+posx,&dx);
	for(i=0;i<len;i++)
	{	 
		p1=(u8*)strstr((const char *)p,"$BDGSV");  
		for(j=0;j<4;j++)
		{	  
			posx=NMEA_Comma_Pos(p1,4+j*4);
			if(posx!=0XFF)gpsx->beidou_slmsg[slx].beidou_num=NMEA_Str2num(p1+posx,&dx);	
			else break; 
			posx=NMEA_Comma_Pos(p1,5+j*4);
			if(posx!=0XFF)gpsx->beidou_slmsg[slx].beidou_eledeg=NMEA_Str2num(p1+posx,&dx);
			else break;
			posx=NMEA_Comma_Pos(p1,6+j*4);
			if(posx!=0XFF)gpsx->beidou_slmsg[slx].beidou_azideg=NMEA_Str2num(p1+posx,&dx);
			else break; 
			posx=NMEA_Comma_Pos(p1,7+j*4);
			if(posx!=0XFF)gpsx->beidou_slmsg[slx].beidou_sn=NMEA_Str2num(p1+posx,&dx);	
			else break;
			slx++;	   
		}   
 		p=p1+1;
	}   
}
void NMEA_GNGGA_Analysis(nmea_msg *gpsx,u8 *buf)
{
	u8 *p1,dx;			 
	u8 posx;    
	p1=(u8*)strstr((const char *)buf,"$GNGGA");
	posx=NMEA_Comma_Pos(p1,6);							
	if(posx!=0XFF)gpsx->gpssta=NMEA_Str2num(p1+posx,&dx);	
	posx=NMEA_Comma_Pos(p1,7);							
	if(posx!=0XFF)gpsx->posslnum=NMEA_Str2num(p1+posx,&dx); 
	posx=NMEA_Comma_Pos(p1,9);						
	if(posx!=0XFF)gpsx->altitude=NMEA_Str2num(p1+posx,&dx);  
}
void NMEA_GNGSA_Analysis(nmea_msg *gpsx,u8 *buf)
{
	u8 *p1,dx;			 
	u8 posx; 
	u8 i;   
	p1=(u8*)strstr((const char *)buf,"$GNGSA");
	posx=NMEA_Comma_Pos(p1,2);							
	if(posx!=0XFF)gpsx->fixmode=NMEA_Str2num(p1+posx,&dx);	
	for(i=0;i<12;i++)									
	{
		posx=NMEA_Comma_Pos(p1,3+i);					 
		if(posx!=0XFF)gpsx->possl[i]=NMEA_Str2num(p1+posx,&dx);
		else break; 
	}				  
	posx=NMEA_Comma_Pos(p1,15);							
	if(posx!=0XFF)gpsx->pdop=NMEA_Str2num(p1+posx,&dx);  
	posx=NMEA_Comma_Pos(p1,16);							
	if(posx!=0XFF)gpsx->hdop=NMEA_Str2num(p1+posx,&dx);  
	posx=NMEA_Comma_Pos(p1,17);							
	if(posx!=0XFF)gpsx->vdop=NMEA_Str2num(p1+posx,&dx);  
}
void NMEA_GNRMC_Analysis(nmea_msg *gpsx,u8 *buf)
{
	u8 *p1,dx;			 
	u8 posx;     
	u32 temp;	   
	float rs;  
	p1=(u8*)strstr((const char *)buf,"$GNRMC");.
	posx=NMEA_Comma_Pos(p1,1);							
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx)/NMEA_Pow(10,dx);	 
		gpsx->utc.hour=temp/10000;
		gpsx->utc.min=(temp/100)%100;
		gpsx->utc.sec=temp%100;	 	 
	}	
	posx=NMEA_Comma_Pos(p1,3);							
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);		 	 
		gpsx->latitude=temp/NMEA_Pow(10,dx+2);
		rs=temp%NMEA_Pow(10,dx+2);				 
		gpsx->latitude=gpsx->latitude*NMEA_Pow(10,5)+(rs*NMEA_Pow(10,5-dx))/60;
	}
	posx=NMEA_Comma_Pos(p1,4);							
	if(posx!=0XFF)gpsx->nshemi=*(p1+posx);					 
 	posx=NMEA_Comma_Pos(p1,5);							
	if(posx!=0XFF)
	{												  
		temp=NMEA_Str2num(p1+posx,&dx);		 	 
		gpsx->longitude=temp/NMEA_Pow(10,dx+2);	
		rs=temp%NMEA_Pow(10,dx+2);					 
		gpsx->longitude=gpsx->longitude*NMEA_Pow(10,5)+(rs*NMEA_Pow(10,5-dx))/60;
	}
	posx=NMEA_Comma_Pos(p1,6);							
	if(posx!=0XFF)gpsx->ewhemi=*(p1+posx);		 
	posx=NMEA_Comma_Pos(p1,9);							
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);		 			
		gpsx->utc.date=temp/10000;
		gpsx->utc.month=(temp/100)%100;
		gpsx->utc.year=2000+temp%100;	 	 
	} 
}
void NMEA_GNVTG_Analysis(nmea_msg *gpsx,u8 *buf)
{
	u8 *p1,dx;			 
	u8 posx;    
	p1=(u8*)strstr((const char *)buf,"$GNVTG");							 
	posx=NMEA_Comma_Pos(p1,7);							
	if(posx!=0XFF)
	{
		gpsx->speed=NMEA_Str2num(p1+posx,&dx);
		if(dx<3)gpsx->speed*=NMEA_Pow(10,3-dx);	 	 	
	}
}  
void GPS_Analysis(nmea_msg *gpsx,u8 *buf)
{
	NMEA_GPGSV_Analysis(gpsx,buf);
	NMEA_BDGSV_Analysis(gpsx,buf);
	NMEA_GNGGA_Analysis(gpsx,buf);		
	NMEA_GNGSA_Analysis(gpsx,buf);	
	NMEA_GNRMC_Analysis(gpsx,buf);
	NMEA_GNVTG_Analysis(gpsx,buf);	
}
u8 SkyTra_Cfg_Ack_Check(void)
{			 
	u16 len=0,i;
	u8 rval=0;
	while((USART3_RX_STA&0X8000)==0 && len<100)
	{
		len++;
		delay_ms(5);
	}		 
	if(len<100)   	//³¬Ê±´íÎó.
	{
		len=USART3_RX_STA&0X7FFF;
		for(i=0;i<len;i++)
		{
			if(USART3_RX_BUF[i]==0X83)break;
			else if(USART3_RX_BUF[i]==0X84)
			{
				rval=3;
				break;
			}
		}
		if(i==len)rval=2;					
	}else rval=1;							
    USART3_RX_STA=0;					
	return rval;  
}

{
	SkyTra_baudrate *cfg_prt=(SkyTra_baudrate *)USART3_TX_BUF;
	cfg_prt->sos=0XA1A0;	
	cfg_prt->PL=0X0400;			
	cfg_prt->id=0X05;		    
	cfg_prt->com_port=0X00;		
	cfg_prt->Baud_id=baud_id;	 	
	cfg_prt->Attributes=1; 		 
	cfg_prt->CS=cfg_prt->id^cfg_prt->com_port^cfg_prt->Baud_id^cfg_prt->Attributes;
	cfg_prt->end=0X0A0D;       
	SkyTra_Send_Date((u8*)cfg_prt,sizeof(SkyTra_baudrate));
	delay_ms(200);				
	usart3_init(BAUD_id[baud_id]);
	return SkyTra_Cfg_Ack_Check();
} 
u8 SkyTra_Cfg_Tp(u32 width)
{
	u32 temp=width;
	SkyTra_pps_width *cfg_tp=(SkyTra_pps_width *)USART3_TX_BUF;
	temp=(width>>24)|((width>>8)&0X0000FF00)|((width<<8)&0X00FF0000)|((width<<24)&0XFF000000);
	cfg_tp->sos=0XA1A0;		 
	cfg_tp->PL=0X0700;       
	cfg_tp->id=0X65	;			  
	cfg_tp->Sub_ID=0X01;		
	cfg_tp->width=temp;		 
	cfg_tp->Attributes=0X01; 
	cfg_tp->CS=cfg_tp->id^cfg_tp->Sub_ID^(cfg_tp->width>>24)^(cfg_tp->width>>16)&0XFF^(cfg_tp->width>>8)&0XFF^cfg_tp->width&0XFF^cfg_tp->Attributes;   
	cfg_tp->end=0X0A0D;     
	SkyTra_Send_Date((u8*)cfg_tp,sizeof(SkyTra_pps_width));
	return SkyTra_Cfg_Ack_Check();
}
u8 SkyTra_Cfg_Rate(u8 Frep)
{
	SkyTra_PosRate *cfg_rate=(SkyTra_PosRate *)USART3_TX_BUF;
 	cfg_rate->sos=0XA1A0;	  
	cfg_rate->PL=0X0300;		
	cfg_rate->id=0X0E;	      
	cfg_rate->rate=Frep;	 	 
	cfg_rate->Attributes=0X01;	   
	cfg_rate->CS=cfg_rate->id^cfg_rate->rate^cfg_rate->Attributes;
	cfg_rate->end=0X0A0D;     
	SkyTra_Send_Date((u8*)cfg_rate,sizeof(SkyTra_PosRate));
	return SkyTra_Cfg_Ack_Check();
}
void SkyTra_Send_Date(u8* dbuf,u16 len)
{
	u16 j;
	for(j=0;j<len;j++)
	{
		while((USART3->SR&0X40)==0);
		USART3->DR=dbuf[j];  
	}	
}
```

<div STYLE="page-break-after: always;"></div>
---
#温湿度传感器DHT11

## **概述**

### &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;DHT11 数字温湿度传感器是一款含有已校准数字信号输出的温湿度复合传感器。它应用专用的数字模块采集技术和温湿度传感技术，确保产品具有极高的可靠性与卓越的长期稳定性。传感器包括一个电阻式感湿元件和一个NTC测温元件，并与一个高性能8 位单片机相连接。因此该产品具有品质卓越、超快响应、抗干扰能力强、性价比极高等优点。每个DHT11 传感器都在极为精确的湿度校验室中进行校准。校准系数以程序的形式储存在OTP 内存中，传感器内部在检测信号的处理过程中要调用这些校准系数。单线制串行接口，使系统集成变得简易快捷。超小的体积、极低的功耗，信号传输距离可达20 米以上，使其成为各类应用甚至最为苛刻的应用场合的最佳选则。

### &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;DHT11 数字温湿度传感器模块为4 针PH2.0 封装。连接方便。如下图所示。

![6345333318371.jpg-23.6kB][1]

## **性能描述**
### 1． 供电电压：3-5.5V
### 2． 供电电流：最大2.5mA
### 3． 温度范围：0-50℃ 误差±2℃
### 4． 湿度范围：20-90%RH 误差±5%RH
### 5． 响应时间: 1/e(63%) 6-30s
### 6． 测量分辨率分别为 8bit（温度）、8bit（湿度）
### 7． 采样周期间隔不得低于1 秒钟
### 8． 模块尺寸：30x20mm 

![6345333318371.jpg-15.8kB][2]

### 注意：连接线长度短于20米时使用5K上拉电阻，大于20米时根据实际情况使用合适的上拉电阻。

## **时序**
## &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;DHT11DATA 用于微处理器与 DHT11之间的通讯和同步,采用单总线数据格式,一次通讯时间4ms左右,数据分小数部分和整数部分,具体格式在下面说明,当前小数部分用于以后扩展,现读出为零.
### 一次完整的数据传输为40bit,高位先出。
### **数据格式**
### 8bit湿度整数数据+8bit湿度小数数据
### +8bi温度整数数据+8bit温度小数数据
### +8bit校验和
### 数据传送正确时校验和数据等于“8bit湿度整数数据+8bit湿度小数数据+8bi温度整数数据+8bit温度小数数据”所得结果的末8位。

### **操作流程如下**

### &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;用户MCU发送一次开始信号后,DHT11从低功耗模式转换到高速模式,等待主机开始信号结束后,DHT11发送响应信号,送出40bit的数据,并触发一次信号采集,用户可选择读取部分数据.从模式下,DHT11接收到开始信号触发一次温湿度采集,如果没有接收到主机发送开始信号,DHT11不会主动进行温湿度采集.采集数据后转换到低速模式。
### **通讯过程如图**
![6345333318371.jpg-23.7kB][3]
### &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;总线空闲状态为高电平,主机把总线拉低等待DHT11响应,主机把总线拉低必须大于18毫秒,保证DHT11能检测到起始信号。DHT11接收到主机的开始信号后,等待主机开始信号结束,然后发送80us低电平响应信号.主机发送开始信号结束后,延时等待20-40us后, 读取DHT11的响应信号,主机发送开始信号后,可以切换到输入模式,或者输出高电平均可, 总线由上拉电阻拉高。
![6345333318371.jpg-21.5kB][4]

### &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 总线为低电平,说明DHT11发送响应信号,DHT11发送响应信号后,再把总线拉高80us,准备发送数据,每一bit数据都以50us低电平时隙开始,高电平的长短定了数据位是0还是1.格式见下面图示.如果读取响应信号为高电平,则DHT11没有响应,请检查线路是否连接正常.当最后一bit数据传送完毕后，DHT11拉低总线50us,随后总线由上拉电阻拉高进入空闲状态。 

## **代码**

```c
/**
**文件名称：DHT11.c
**文件版本：V1.0
**创建时间：2012/08/06		
**创建人员：林木泉
**文件说明：文件为温湿度传感器DHT11的驱动程序
**/
#include "../drive/drive.h"

void GPIO_DHT_Out_Mode(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Pin = DHT11_PORT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; //开漏输出
  GPIO_Init(GPIOA,&GPIO_InitStructure);
}
void GPIO_DHT_Input_Mode(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Pin = DHT11_PORT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA,&GPIO_InitStructure);
}
//----------------------------------------------------------------------------------------------
//--- name	:	DHT11WriteStart	
//--- 功能	:	向DHT11写入一个读取数据的引导码
//----------------------------------------------------------------------------------------------
void DHT11WriteStart()
{
	GPIO_DHT_Out_Mode();
	P10 = 1;
	P10 = 0;
	Delay_ms(25);//拉低电平至少18ms
	P10 = 1;
	Delay_us(30);				
}
//----------------------------------------------------------------------------------------------
//--- name		:	DHT11ReadByte
//--- 功能		:	从DHT11中读取到一个字节
//--- 返回值	:	读取到一个字节的数据
//----------------------------------------------------------------------------------------------
u8 DHT11ReadByte(void)
{
	u8 temp=0,i,j=0;
	for(i=0;i<8;i++)
	{
		temp<<=1;
		while(0 == DHT11);//等待变高电平
		while(1 == DHT11)//计算高电平时长
		{
			Delay_us(1);
			j++;
		}
		if(j>=30)        //超过30us确认为1
		{
			temp = temp|0x01;
			j=0;
		}
		j=0;		
	}
	return temp;
}
//----------------------------------------------------------------------------------------------
//--- name		:	DHT11Read(u8 *RH_temp,u8 *RL_temp,u8 *TH_temp,u8 *TL_temp,u8 *CK_temp)
//--- 功能		:	从DHT11中读取数据
//--- 说明		:	测试过程中发现温度数值不变，小数值都是零，此模块未测试成功！
//----------------------------------------------------------------------------------------------
void DHT11Read(u8 *RH_temp,u8 *RL_temp,u8 *TH_temp,u8 *TL_temp,u8 *CK_temp)
{
	//uchar TH_temp,TL_temp,RH_temp,RL_temp,CK_temp;
	//uchar TL_temp,RL_temp,CK_temp;
	//u8 untemp;
	while(1)
	{
		DHT11WriteStart();//给读取前导信号
		GPIO_DHT_Input_Mode();//设置端口为输入状态
		if(!DHT11)   
		{
			while(0 == DHT11);//低电平的响应信号，80us
			while(1 == DHT11);//紧接着是80us的高电平数据准备信号
// 			*CK_temp = DHT11ReadByte();
// 			*TL_temp = DHT11ReadByte();
// 			*TH_temp = DHT11ReadByte(); 
// 			*RL_temp = DHT11ReadByte(); 
// 			*RH_temp = DHT11ReadByte(); 
			
			*RH_temp = DHT11ReadByte();//湿度高8位
			*RL_temp = DHT11ReadByte();//湿度低8位
			*TH_temp = DHT11ReadByte();//温度高8位
			*TL_temp = DHT11ReadByte();//温度低8位
			*CK_temp = DHT11ReadByte();//校验和
			GPIO_DHT_Out_Mode();
			P10=1;   
			//数据校验 
			//untemp= *RH_temp+RL_temp+*TH_temp+TL_temp;
			return;
		}
		DriveDelay(0x3ff);
	}
}
/*********************************************************************************************
***	代码段	：	DHT11__DEBUG
***	说明  	：	该代码段是用于测试温湿度传感器DHT11所用，正常情况下不加入编译。
***				如果用户要测试该模块，可以将#undef改为#define(在文件头处)
*********************************************************************************************/
#undef	DHT11__DEBUG

#define	DHT11__DEBUG

#ifdef	DHT11__DEBUG

int main(void)
{
	u8 TH_temp,TL_temp,RH_temp,RL_temp,CK_temp;
	char DisBuf[20];
	#ifdef DEBUG
	  debug();
	#endif
	SysInit();
// 	SysTickInit();
	InitLcd();
	LcdDisText(0x80,"hello world!!!");

	while(1)
	{
		DHT11Read(&RH_temp,&RL_temp,&TH_temp,&TL_temp,&CK_temp);
		sprintf(DisBuf,"%d-%d-%d-%d-%d",RH_temp,RL_temp,TH_temp,TL_temp,CK_temp);
// 		Delay_ms(500);
		LcdDisText(0x80+0x40,(u8 *)DisBuf);
		LcdDisText(0x80+0x40+15,"h");
	}
}
#endif
/**************************************end of  DHT11__DEBUG*********************************/
/****************************************end of file ****************************************/
```

  [1]: http://static.zybuluo.com/BJK/rw5isqz30qwuo3y4vczo52iy/6345333318371.jpg
  [2]: http://static.zybuluo.com/BJK/7qphjuz05omsof6tew9wolcg/6345333318371.jpg
  [3]: http://static.zybuluo.com/BJK/wtv1du2rg0gi7c00cr1xtn30/6345333318371.jpg
  [4]: http://static.zybuluo.com/BJK/w8m1e5refe9zxw1xfslozzwe/6345333318371.jpg
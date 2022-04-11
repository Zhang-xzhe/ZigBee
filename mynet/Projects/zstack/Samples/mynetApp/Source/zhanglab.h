#include<iocc2530.h>

void RFSend(char *pstr,char len);
void halRfInit(void);
void RevRFProc();

void lightupD1();//对D1的操作
void lightoffD1();
void lightchangeD1();

void lightupD2();//对D2的操作
void lightoffD2();
void lightchangeD2();

void lightupD3();//对D3的操作
void lightoffD3();
void lightchangeD3();

void delay();//延时函数
void delays();

void configurekey1();//按键中断初始化
void configurekey2();

void set32Mclock();//时钟切换函数

void uart();//串口配置，以及串口中断打开

void LCD_coordinator();//显示协调器


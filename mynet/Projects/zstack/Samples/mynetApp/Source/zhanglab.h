#include<iocc2530.h>

void RFSend(char *pstr,char len);
void halRfInit(void);
void RevRFProc();

void lightupD1();//��D1�Ĳ���
void lightoffD1();
void lightchangeD1();

void lightupD2();//��D2�Ĳ���
void lightoffD2();
void lightchangeD2();

void lightupD3();//��D3�Ĳ���
void lightoffD3();
void lightchangeD3();

void delay();//��ʱ����
void delays();

void configurekey1();//�����жϳ�ʼ��
void configurekey2();

void set32Mclock();//ʱ���л�����

void uart();//�������ã��Լ������жϴ�

void LCD_coordinator();//��ʾЭ����


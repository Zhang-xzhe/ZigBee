#include<iocc2530.h>
#include"zhanglab.h"

//char SendPacket[]={0x19,0x61,0x88,0x00,0x07,0x20,0xEF,0xBE,0x20,0x50,'h','e','l','l','o',' ','l','a','o',' ','d','a','\r','\n'};
//��һ���ֽ�0x0C���壬����Լ����滹��12���ֽ�Ҫ����
//��5 6���ֽڱ�ʾ����PANID
//��7 8���ֽ�������ģ��Ŀ���豸�������ַ 0xBEEF
//��9 10���Ǳ���ģ��������ַ
//11 ���ֽ����������õ�����
// CRC�� 12 13���ֽ� ��Ӳ���Զ�׷��
void halRfInit(void)
{
    EA=0;
    FRMCTRL0 |= 0x60;
    // Recommended RX settings  
    TXFILTCFG = 0x09;
    AGCCTRL1 = 0x15;
    FSCAL1 = 0x00;
    // enable RXPKTDONE interrupt  
    RFIRQM0 |= 0x40;//����Ƶ�����жϴ�
    // enable general RF interrupts
    IEN2 |= 0x01;
    
    FREQCTRL =(11+(25-11)*5);//(MIN_CHANNEL + (channel - MIN_CHANNEL) * CHANNEL_SPACING);   
                     //�����ز�Ϊ2475M
    PAN_ID0=0x07;
    PAN_ID1=0x20; //0x2007   
//halRfRxInterruptConfig(basicRfRxFrmDoneIsr);    
    RFST = 0xEC;//����ջ�����
    RFST = 0xE3;//��������ʹ�� 
    EA=1;    
}



void RFSend(char *pstr,char len)
{
  char i;
    RFST = 0xEC; //ȷ�������ǿյ�
    RFST = 0xE3; //����ձ�־λ
    while (FSMSTAT1 & 0x22);//�ȴ���Ƶ����׼����
    RFST = 0xEE;//ȷ�����Ͷ����ǿ�
    RFIRQF1 &= ~0x02;//�巢�ͱ�־λ
//Ϊ���ݷ�������׼������
    for(i=0;i<len;i++)
    {
       RFD=pstr[i];
    }  //ѭ���������ǰ�����Ҫ���͵�����ȫ��ѹ�����ͻ���������
    RFST = 0xE9; //����Ĵ���һ��������Ϊ0xE9,���ͻ����������ݾͱ����ͳ�ȥ
    while(!(RFIRQF1 & 0x02) );//�ȴ��������
    RFIRQF1 = ~0x02;//�巢����ɱ�־
}




void lightupD1()
{
  P1SEL &=0xFE;//1111 1110
  P1DIR |=0x01;//0000 0001
  P1_0=0;
}

void lightoffD1()
{
  P1SEL &=0xFE;//1111 1110
  P1DIR |=0x01;//0000 0001
  P1_0=1;
}

void lightchangeD1()
{
  P1SEL &=0xFE;//1111 1110
  P1DIR |=0x01;//0000 0001
  P1_0^=1;
}

void lightupD2()
{
  P1SEL &=0xFD;//1111 1101
  P1DIR |=0x02;//0000 0010
  P1_1=0;
}

void lightoffD2()
{
  P1SEL &=0xFD;//1111 1101
  P1DIR |=0x02;//0000 0010
  P1_1=1;
}

void lightchangeD2()
{
  P1SEL &=0xFD;//1111 1101
  P1DIR |=0x02;//0000 0010
  P1_1^=1;
}

void lightupD3()
{
  P0SEL &=0xEF;//1110 1111
  P0DIR |=0x10;//0001 0000
  P0_4=0;
}

void lightoffD3()
{
  P0SEL &=0xEF;//1110 1111
  P0DIR |=0x10;//0001 0000
  P0_4=1;
}

void lightchangeD3()
{
  P0SEL &=0xEF;//1110 1111
  P0DIR |=0x10;//0001 0000
  P0_4^=1;
}

void delay()
{
  int i;
  for(i=0;i<30000;i++){};
  for(i=0;i<30000;i++){};
  for(i=0;i<30000;i++){};
  for(i=0;i<30000;i++){};
  for(i=0;i<30000;i++){};
  for(i=0;i<30000;i++){};
  for(i=0;i<30000;i++){};
  for(i=0;i<30000;i++){};
}

void delays()
{
  int i;
  for(i=0;i<30000;i++){};
  for(i=0;i<30000;i++){};
  for(i=0;i<30000;i++){};
}

void configurekey1()
{
  P0IFG=0; //���������жϳ�ȥ��ʱ��Ҫ��0��������������P0�����Ծ���P0IFG
  P0IF=0;
  P0SEL &=0xFD;// 1111 1101 //P0_1�����������P0_1Ϊ��ͨIO
  P0DIR &=0xFD;// 1111 1101 //P0_1��������
  P0INP &=0xFD;// 1111 1101 //P0_1������
  P2INP &=0xDF;// 1101 1111 //P0_1����
  EA=1;//�����ж�
  P0IE=1;//PO���жϴ�
  P0IEN |=0X02;// 0000 0010 PO_1�жϴ�
  PICTL |=0x01;//��P0��һ�����ó��½��ش���
}

void configurekey2()
{
  P2IFG=0; //���������жϳ�ȥ��ʱ��Ҫ��0��������������P0�����Ծ���P0IFG 
  P2IF=0;
  P2SEL &=0xFE;// 1111 1110 P2_0 �����������P2_0Ϊ��ͨIO
  P2DIR &=0xFE;// 1111 1110 P2_0��������
  P2INP &=0xFE;// 1111 1110 //P2_0������
  P2INP &=0x7F;// 0111 1111 //P2_0����
  EA=1;//�����ж�
  IEN2 |=0x02;//0000 0010 P2���жϴ�
  P2IEN |=0x01;//0000 0001 P2_0�жϴ�
  PICTL |=0x08;//0000 1000 ��P2_0��P2_4����Ϊ�½��ش���
}

void set32Mclock()
{
  SLEEPCMD &=0xFB;//1111 1011����������Ƶʱ��Դ
  while((SLEEPSTA&0x40)==0){}//0100 0000ֱ��ʱ��Դ�ȶ�
  delays();//��ʱһ���������63us
  CLKCONCMD &=0xF8;//1111 1000����Ƶ
  CLKCONCMD &=0xBF;//1011 1111 ��ʱ���л���32M
  while(CLKCONSTA&0x40){}//ֱ���л����
}

void uart()
{
  PERCFG &=0xFE;//1111 1110ѡ�п������ϵ���Ӧ�������ſ�P0_2 P0_3
  P0SEL |=0x0C;//0000 1100���������������ó�Ƭ�������ģʽ
  U0CSR |=0xC0;//8����λ��1ֹͣλ�������أ���У��
  
  U0GCR =11;//���ò�����Ϊ115200
  U0BAUD =216;
  
  URX0IE=1;//���жϴ�
  EA=1;
}

//void LCD_coordinator()
//{
//    int i=0;
//    for(i=0; i<4; i++)
//    {
//      LCD_P16x16Ch(16*(i+2), 0, i);
//    }
//    for(i=0; i<7; i++)
//    {
//      LCD_P16x16Ch(8+16*i, 2, 4+i);
//    }
//    //LCD_P8x16Str(16, 4, "QQ:2357481431");
//    //LCD_P8x16Str(32, 6, "2014-10-01");
//}

/////////////////////////////////////////////////////////////////�����жϺ���



//#pragma vector=URX0_VECTOR
//__interrupt void chuankou()
//{
//  char ch;
//  URX0IF=0;//����־λ����
//  ch=U0DBUF;//�����ݴӼĴ����ж�����
//  
//  U0DBUF=ch;//�����ݴӼĴ����з���ȥ
//  while(UTX0IF==0){}//һֱ�ȵ����ݷ������
//  
//  UTX0IF=0;
//  lightupD1();
//  delay();
//}




///////////////////////////////////////////////////////////////����1�жϺ���





//#pragma vector=P0INT_VECTOR//���ǹ̶�ģʽ���������0�飬������P0INT
//__interrupt void key1() //����1�����ж�
//{
//  if(P0IFG & 0x02)// 0010 0000���P0_5�������жϣ�P0IFG����1������if���
//  {//P0��ĵ�1λ P0_1�������ⲿ�ж�
//    //����Ҫ����
//  }
//  P0IFG=0; //���������жϳ�ȥ��ʱ��Ҫ��0��������������P0�����Ծ���P0IFG
//  P0IF=0;//���������жϳ�ȥ��ʱ��Ҫ��0��������������P0�����Ծ���P0IF
//}




////////////////////////////////////////////////////////////////����2�жϺ���





//#pragma vector=P2INT_VECTOR//���ǹ̶�ģʽ���������0�飬������P0INT
//__interrupt void key2() //����1�����ж�
//{
//  if(P2IFG & 0x01)// 0000 0001���P2_0�������жϣ�P0IFG����1������if���
//  {//P0��ĵ�1λ P0_1�������ⲿ�ж�
//    //����Ҫ����
//  }
//  P2IFG=0; //���������жϳ�ȥ��ʱ��Ҫ��0��������������P0�����Ծ���P0IFG
//  P2IF=0;//���������жϳ�ȥ��ʱ��Ҫ��0��������������P0�����Ծ���P0IF
//}



/////////////////////////////////////////////////////////////��Ƶ���պ���
//#pragma vector=RF_VECTOR
//__interrupt void RF_IRQ(void)
//{//�������Ƶ�жϺ�������С��ģ����յ�����ģ�鷢����������ʱ��С��ģ���CPU�ͻ�����жϺ���ִ��
//    EA=0;
//    if( RFIRQF0 & 0x40 )
//    {            
//        RFIRQF0&= ~0x40;   // Clear RXPKTDONE interrupt
//    }
//    S1CON= 0;                   // Clear general RF interrupt flag
//    RFST = 0xEC;//����ջ�����
//    RFST = 0xE3;//��������ʹ�� 
//    EA=1;
//}
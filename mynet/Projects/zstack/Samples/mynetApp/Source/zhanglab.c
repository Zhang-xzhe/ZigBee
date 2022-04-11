#include<iocc2530.h>
#include"zhanglab.h"

//char SendPacket[]={0x19,0x61,0x88,0x00,0x07,0x20,0xEF,0xBE,0x20,0x50,'h','e','l','l','o',' ','l','a','o',' ','d','a','\r','\n'};
//第一个字节0x0C含义，这个自己后面还有12个字节要发送
//第5 6个字节表示的是PANID
//第7 8个字节是无线模块目标设备的网络地址 0xBEEF
//第9 10就是本地模块的网络地址
//11 个字节是我们有用的数据
// CRC码 12 13个字节 是硬件自动追加
void halRfInit(void)
{
    EA=0;
    FRMCTRL0 |= 0x60;
    // Recommended RX settings  
    TXFILTCFG = 0x09;
    AGCCTRL1 = 0x15;
    FSCAL1 = 0x00;
    // enable RXPKTDONE interrupt  
    RFIRQM0 |= 0x40;//把射频接收中断打开
    // enable general RF interrupts
    IEN2 |= 0x01;
    
    FREQCTRL =(11+(25-11)*5);//(MIN_CHANNEL + (channel - MIN_CHANNEL) * CHANNEL_SPACING);   
                     //设置载波为2475M
    PAN_ID0=0x07;
    PAN_ID1=0x20; //0x2007   
//halRfRxInterruptConfig(basicRfRxFrmDoneIsr);    
    RFST = 0xEC;//清接收缓冲器
    RFST = 0xE3;//开启接收使能 
    EA=1;    
}



void RFSend(char *pstr,char len)
{
  char i;
    RFST = 0xEC; //确保接收是空的
    RFST = 0xE3; //清接收标志位
    while (FSMSTAT1 & 0x22);//等待射频发送准备好
    RFST = 0xEE;//确保发送队列是空
    RFIRQF1 &= ~0x02;//清发送标志位
//为数据发送做好准备工作
    for(i=0;i<len;i++)
    {
       RFD=pstr[i];
    }  //循环的作用是把我们要发送的数据全部压到发送缓冲区里面
    RFST = 0xE9; //这个寄存器一旦被设置为0xE9,发送缓冲区的数据就被发送出去
    while(!(RFIRQF1 & 0x02) );//等待发送完成
    RFIRQF1 = ~0x02;//清发送完成标志
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
  P0IFG=0; //这两条在中断出去的时候都要清0，这次我们配的是P0组所以就是P0IFG
  P0IF=0;
  P0SEL &=0xFD;// 1111 1101 //P0_1这个就是配置P0_1为普通IO
  P0DIR &=0xFD;// 1111 1101 //P0_1配置输入
  P0INP &=0xFD;// 1111 1101 //P0_1上下拉
  P2INP &=0xDF;// 1101 1111 //P0_1上拉
  EA=1;//开总中断
  P0IE=1;//PO组中断打开
  P0IEN |=0X02;// 0000 0010 PO_1中断打开
  PICTL |=0x01;//把P0这一组配置成下降沿触发
}

void configurekey2()
{
  P2IFG=0; //这两条在中断出去的时候都要清0，这次我们配的是P0组所以就是P0IFG 
  P2IF=0;
  P2SEL &=0xFE;// 1111 1110 P2_0 这个就是配置P2_0为普通IO
  P2DIR &=0xFE;// 1111 1110 P2_0配置输入
  P2INP &=0xFE;// 1111 1110 //P2_0上下拉
  P2INP &=0x7F;// 0111 1111 //P2_0上拉
  EA=1;//开总中断
  IEN2 |=0x02;//0000 0010 P2组中断打开
  P2IEN |=0x01;//0000 0001 P2_0中断打开
  PICTL |=0x08;//0000 1000 把P2_0到P2_4配置为下降沿触发
}

void set32Mclock()
{
  SLEEPCMD &=0xFB;//1111 1011开启两个高频时钟源
  while((SLEEPSTA&0x40)==0){}//0100 0000直到时钟源稳定
  delays();//延时一会儿（超过63us
  CLKCONCMD &=0xF8;//1111 1000不分频
  CLKCONCMD &=0xBF;//1011 1111 将时钟切换到32M
  while(CLKCONSTA&0x40){}//直到切换完成
}

void uart()
{
  PERCFG &=0xFE;//1111 1110选中开发板上的相应串口引脚口P0_2 P0_3
  P0SEL |=0x0C;//0000 1100将这两个引脚配置成片上外设的模式
  U0CSR |=0xC0;//8数据位，1停止位，无流控，无校验
  
  U0GCR =11;//配置波特率为115200
  U0BAUD =216;
  
  URX0IE=1;//把中断打开
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

/////////////////////////////////////////////////////////////////串口中断函数



//#pragma vector=URX0_VECTOR
//__interrupt void chuankou()
//{
//  char ch;
//  URX0IF=0;//将标志位清零
//  ch=U0DBUF;//将数据从寄存器中读出来
//  
//  U0DBUF=ch;//将数据从寄存器中发出去
//  while(UTX0IF==0){}//一直等到数据发送完毕
//  
//  UTX0IF=0;
//  lightupD1();
//  delay();
//}




///////////////////////////////////////////////////////////////按键1中断函数





//#pragma vector=P0INT_VECTOR//这是固定模式，咱们配第0组，所以是P0INT
//__interrupt void key1() //按键1引发中断
//{
//  if(P0IFG & 0x02)// 0010 0000如果P0_5发生了中断，P0IFG会变成1→进入if语句
//  {//P0组的第1位 P0_1引发了外部中断
//    //还需要消抖
//  }
//  P0IFG=0; //这两条在中断出去的时候都要清0，这次我们配的是P0组所以就是P0IFG
//  P0IF=0;//这两条在中断出去的时候都要清0，这次我们配的是P0组所以就是P0IF
//}




////////////////////////////////////////////////////////////////按键2中断函数





//#pragma vector=P2INT_VECTOR//这是固定模式，咱们配第0组，所以是P0INT
//__interrupt void key2() //按键1引发中断
//{
//  if(P2IFG & 0x01)// 0000 0001如果P2_0发生了中断，P0IFG会变成1→进入if语句
//  {//P0组的第1位 P0_1引发了外部中断
//    //还需要消抖
//  }
//  P2IFG=0; //这两条在中断出去的时候都要清0，这次我们配的是P0组所以就是P0IFG
//  P2IF=0;//这两条在中断出去的时候都要清0，这次我们配的是P0组所以就是P0IF
//}



/////////////////////////////////////////////////////////////射频接收函数
//#pragma vector=RF_VECTOR
//__interrupt void RF_IRQ(void)
//{//这个是射频中断函数，当小灯模块接收到开关模块发送来的数据时，小灯模块的CPU就会进入中断函数执行
//    EA=0;
//    if( RFIRQF0 & 0x40 )
//    {            
//        RFIRQF0&= ~0x40;   // Clear RXPKTDONE interrupt
//    }
//    S1CON= 0;                   // Clear general RF interrupt flag
//    RFST = 0xEC;//清接收缓冲器
//    RFST = 0xE3;//开启接收使能 
//    EA=1;
//}
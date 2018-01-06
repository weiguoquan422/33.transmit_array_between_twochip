/*
 * @Author: Guoquan Wei 1940359148@qq.com 
 * @Date: 2017-12-27 21:43:56 
 * @Last Modified by: Guoquan Wei
 * @Last Modified time: 2017-12-27 23:18:06
 */

/* transmit part */


#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int
sbit dula=P2^6;
sbit wela=P2^7;

uchar key;
uchar a[]={0,1,2};
uchar n=3;
int num2;

uchar datable[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};//定义段选
uchar watable[]={0x3f,0x1f,0x2f,0x37,0x3b,0x3d,0x3e};    //定义位选，位选六位，最高两位初始化定为零

void delayms(int x)
// 延时函数
{
	int i, j;
	for (i = 0; i < x; i++)
		for (j = 0; j < 110; j++)
			;
}

void display()
//display函数：显示一个三位数组
{
dula=1;
P0=datable[a[0]];
dula=0;
P0=0xff;
wela=1;
P0=watable[1];
wela=0;
delayms(1);

dula=1;
P0=datable[a[1]];
dula=0;
P0=0xff;
wela=1;
P0=watable[2];
wela=0;
delayms(1);

dula=1;
P0=datable[a[2]];
dula=0;
P0=0xff;
wela=1;
P0=watable[3];
wela=0;
delayms(1);
}


void send(uchar key_num)
// 发送一个字节的信息
{
	SBUF = key_num;
	while (!TI)
	{
		;
	}
	TI=0;
}

void sendarray(uchar *p,uchar n)
// 发送n位字节的信息
{
	int i;
	uchar temp;
	send(n);
	for (i=0 ; i<n ; i++ )
	{
		temp=*(p+i);
		send(temp);
	}
}

void init(void)
{
	SCON = 0x40; // 串行口工作方式1,禁止接收
	PCON = 0x00; // 波特率不倍增
	TMOD = 0x21; // 定时器1工作于8位自动重载模式, 用于产生波特率;定时器0为工作方式1
	IP=0x02;

	TH0 = (65536 - 45872) / 256;
	TL0 = (65536 - 45872) % 256;
	TH1 = 0xfd;
	TL1 = 0xfd; // T1赋初值,11.0592M晶振，波特率2400
	TR1 = 1;	// 启动定时器1
	ET0=1;//开定时器0中断
	TR0=1;//启动定时器0
	ES=0;
	EA = 1;
}

void arrayadd(uchar *p,uchar n)
//对一个数组的所有元素加一，数组元素不能自己参与运算！
{
	uchar temp;
	uchar i;
	for (i=0 ; i<n ; i++ )
	{
		temp=*(p+i);
		temp++;
		if (temp>15)
		{
			temp=0;
		}
		*(p+i)=temp;
	}
}

void main()
{
	init();

	while (1)
	{
		display();
		// send(5);
		// sendarray(a,n);
		// send函数必须写到while里面，否则不稳定
	}
}


void t1_time() interrupt 1

{
	TH0 = (65536 - 45872) / 256;
	TL0 = (65536 - 45872) % 256;
	num2++;
	if (num2 == 30)
	{
		num2 = 0;
		arrayadd(a,3);
		sendarray(a,3);
	}
}
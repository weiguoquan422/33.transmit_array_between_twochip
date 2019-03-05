/*
 * @Author: Guoquan Wei 1940359148@qq.com 
 * @Date: 2017-12-27 22:02:35 
 * @Last Modified by: Guoquan Wei
 * @Last Modified time: 2017-12-27 22:37:57
 */

/* receive part */

#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int

uchar a[10];
int i = 0;
int n = 0;

sbit dula = P2 ^ 6;
sbit wela = P2 ^ 7;

uchar datable[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71}; //定义段选
uchar watable[] = {0x3f, 0x1f, 0x2f, 0x37, 0x3b, 0x3d, 0x3e};														//定义位选，位选六位，最高两位初始化定为零

void delayms(int x)
{
	int i, j;
	for (i = 0; i < x; i++)
		for (j = 0; j < 110; j++)
			;
}

void display()
//display函数：显示一个三位数组
{
	dula = 1;
	P0 = datable[a[0]];
	dula = 0;
	P0 = 0xff;
	wela = 1;
	P0 = watable[1];
	wela = 0;
	delayms(1);

	dula = 1;
	P0 = datable[a[1]];
	dula = 0;
	P0 = 0xff;
	wela = 1;
	P0 = watable[2];
	wela = 0;
	delayms(1);

	dula = 1;
	P0 = datable[a[2]];
	dula = 0;
	P0 = 0xff;
	wela = 1;
	P0 = watable[3];
	wela = 0;
	delayms(1);
}

void init(void)
{
	SCON = 0x50; // 串行口工作方式1,允许接收
	PCON = 0x00; // 波特率不倍增
	TMOD = 0x20; // 定时器1工作于8位自动重载模式, 用于产生波特率
	TH1 = 0xfd;
	TL1 = 0xfd; // T1赋初值,波特率2400
	TR1 = 1;	// 启动定时器1
	EA = 1;
	ES = 1;
}

void main()
{
	init();
	while (1)
	{
		display();
	}
}

void ser() interrupt 4
{
	if (RI == 1)
	{
		if (n == 0)//如果n=0，则说明n还没有被赋值，现在寄存器里面存在的是传输的是第一个数字，第一个数字代表数组长度，所以把该值赋值给n
		{
			RI = 0;
			n = SBUF;
		}
		//如果n已经被赋值不是0了，则从a[0]开始记录传过来的数据；如果i==n，说明数组传输完毕，i和n初始化为0
		else
		{
			RI = 0;
			a[i] = SBUF;
			i++;
			if (i == n)
			{
				i = 0;
				n = 0;
			}
		}
	}
}

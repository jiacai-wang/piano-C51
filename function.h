sbit beep=P1^0;     //连接扬声器
sbit up=P1^1;       //升调键
sbit down=P1^2;     //降调键
sbit play=P3^2;     //播放键，中断
sbit led=P1^3;      //指示灯

unsigned char CurFreHigh=0x00;      //当前频率对应的定时器值-高字节
unsigned char CurFreLow=0x00;       //当前频率对应的定时器值-低字节
unsigned char temp=0x00;            //存储当前按键值，无变化则不重装定时器
unsigned char fre, len;             //播放时的音高，音长
unsigned char PlayIndex;            //用于中断切歌
unsigned char Playing;              //当前播放音乐的序号
unsigned char key=0;	            //获取按键值


void delay(unsigned char i)
{
	unsigned char j,k;
	while(i--)
	{
		j=0xFF;
		while(j--)
		{
			k=0x20;
			while(k--) if(PlayIndex!=Playing) return;
		}
	}
}


void Play(unsigned char index)
{
	unsigned int i=0;       //播放第一曲
	if(index==2) i+=68;     //第二曲
	if(index==3) i+=218;        //第三曲
	if(index==4) i+=606;        //第四曲
	if(index==5) i+=684;        //第五曲
	if(index==6)                //进入待机
	{
		led=0;
		delay(1);
		led=1;
		delay(2);
		led=0;
		delay(1);
		led=1;
		delay(2);
		led=0;
		beep=1;
		PCON=0x02;
	}
	while(1)
	{
		if(index!=PlayIndex) break;
		fre=music[i++];
		len=music[i++];
		CurFreHigh=FreqTabHigh[fre];
		CurFreLow=FreqTabLow[fre];
		TH0=CurFreHigh;
		TL0=CurFreLow;
		TR0=1;
		if(len==0x00)
		{
//			PlayIndex=0;
//			Playing=0;
			break;
		}
		delay(len);
		TR0=0;
	}
	TR0=0;
}

void InitInt()      //初始化中断
{
	TMOD=0x01;
	IT0=1;
	IE=0x83;
}
void sound(unsigned char i)     //判断是否升降调并播放按键对应的音阶
{
	up=down=1;
	if((up)&&(!down)) i-=12;
	if((!up)&&(down)) i+=12;
	CurFreHigh=FreqTabHigh[i];
	CurFreLow=FreqTabLow[i];
	TH0=CurFreHigh;
	TL0=CurFreLow;
	TR0=1;
}

void Int0() interrupt 0     //播放键按下时
{
	delay(5);
	led=1;
	PlayIndex=(PlayIndex+1)%7;
}

void Timer0() interrupt 1
{
	TH0=CurFreHigh;
	TL0=CurFreLow;
	if(CurFreLow)	beep=~beep;
}

void keydown()      //扫描按键值
{
	P2=0xC0;
	key=0;
	if(P2!=0xC0)
	{
		P2=0xC0;
		switch(P2)
		{
			case(0x40): key=1; break;
			case(0x80): key=2; break;
		}
		P2=0x3F;
		switch(P2)
		{
			case(0x1F): break;
			case(0x2F): key+=2;break;
			case(0x37): key+=4;break;
			case(0x3B): key+=6;break;
			case(0x3D): key+=8;break;
			case(0x3E): key+=10;break;
		}
		
	}
}

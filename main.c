/*
 * Coded by 王家才
 * For Miss TL.
*/


#include"reg52.h"
#include"freqtab.h"
#include"music.h"
#include"function.h"

void main()
{
	led=1;
	PlayIndex=0;
	InitInt();
	beep=1;
	while(1)
	{
		keydown();
		temp=key;
		switch (key)		//由按键排列推出key对应的音阶
		{
			case 8:sound(13);break;
			case 12:sound(14);break;
			case 11:sound(15);break;
			case 10:sound(16);break;
			case 9:sound(17);break;
			case 7:sound(18);break;
			case 6:sound(19);break;
			case 5:sound(20);break;
			case 4:sound(21);break;
			case 3:sound(22);break;
			case 2:sound(23);break;
			case 1:sound(24);break;
			case 0 :beep=1;TR0=0;break;		//无按键不发声，beep置1避免扬声器烧坏
			default: ;
		}
		while(key==temp)
		{
			keydown();
			if(PlayIndex)
			{
				Playing=PlayIndex;
				Play(PlayIndex);
				Playing=0;
			}
		}
	}
}

/*************************************************************
*项目名称：倒计时数字钟（矩阵按键可调）
*项目平台：Keil5 + 普中科技51单片机
*项目时间：2020-11-18
*作者：云嘉轩
**************************************************************/

#include "smg.h"

void main(){
	Init();
	while(1){
		KeyDown();
		Keypross();
		Datapross();
		if(flag==1)
			Flash(flag1);
		else
			Showsmg();
	}
}

//数码管显示函数
void Showsmg(){      
	u8 i;
	for(i=0;i<8;i++)
	{
	   switch(7-i)	  //位选
	   {
		  case 0: LSA=0;LSB=0;LSC=0;break;
		  case 1: LSA=1;LSB=0;LSC=0;break;
		  case 2: LSA=0;LSB=1;LSC=0;break;
		  case 3: LSA=1;LSB=1;LSC=0;break;
		  case 4: LSA=0;LSB=0;LSC=1;break;
		  case 5: LSA=1;LSB=0;LSC=1;break;
		  case 6: LSA=0;LSB=1;LSC=1;break;
		  case 7: LSA=1;LSB=1;LSC=1;break;
	   }
	  P0=Display[i];
	  Delay(1);
	  P0=0x00;
	}
}
//延时函数
void Delay(u16 i)   //11.0592 1ms
{
	while(i--){
		unsigned char i, j;

		_nop_();
		i = 2;
		j = 199;
		do
		{
			while (--j);
		} while (--i);
	}
} 
//初始化函数
void Init(){
	//参数初始化
	hour=0;
	minute=0;
	second=0;
	t0=0;
	flag=0;
	flag1=0;
	flag2=0;
	//定时器0初始化
	TMOD=0x01;
	TH0=(65536-9174)/256;   //10ms
	TL0=(65536-9174)%256;
	EA=1;
	ET0=1;
	TR0=0;
}
//按键检测
void KeyDown(){
	char a=0;
	GPIO_KEY=0x0f;
	if(GPIO_KEY!=0x0f){
		Delay(10);
		if(GPIO_KEY!=0x0f){
			GPIO_KEY=0x0f;
			switch(GPIO_KEY){
				case 0x07 : KeyVal=0;flag3=1;break;
				case 0x0b : KeyVal=1;flag3=1;break;
				case 0x0d : KeyVal=2;flag3=1;break;
				case 0x0e : KeyVal=3;flag3=1;break;
			}
			GPIO_KEY=0xf0;
			switch(GPIO_KEY){
				case 0x70 : KeyVal=KeyVal;break;
				case 0xb0 : KeyVal=KeyVal+4;break;
				case 0xd0 : KeyVal=KeyVal+8;break;
				case 0xe0 : KeyVal=KeyVal+12;break;
			}
		}
		while((a<150) && (GPIO_KEY!=0xf0)){
			Delay(1);
			a++;
		}
	}
}
//按键处理
void Keypross(){
	if(flag3==1){
		flag3=0;
		switch(KeyVal){
			case 0:Numpross(7);break;
			case 1:Numpross(8);break;
			case 2:Numpross(9);break;
			case 3:Uppross();break;
			case 4:Numpross(4);break;
			case 5:Numpross(5);break;
			case 6:Numpross(6);break;
			case 7:Downpross();break;
			case 8:Numpross(1);break;
			case 9:Numpross(2);break;
			case 10:Numpross(3);break;
			case 11:Funcpross();break;
			case 12:Numpross(0);break;
			case 13:break;
			case 14:Backpross();break;
			case 15:Enterpross();break;
			
		}
	}
}
//数据处理
void Datapross(){
	if(flag2==1){
		flag2=0;
		second--;
		if(second>59){
			if(minute==0){
				if(hour==23){
					second=0;
					minute=0;
					hour=0;
					TR0=0;
					return;
				}
				else{
					hour--;
					minute=59;
					second=59;
				}
			}
			else{
				minute--;
				second=59;
			}	
		}	
	}
	Display[0]=smgduan[hour/10];
	Display[1]=smgduan[hour%10];
	Display[2]=0x40;
	Display[3]=smgduan[minute/10];
	Display[4]=smgduan[minute%10];
	Display[5]=0x40;
	Display[6]=smgduan[second/10];
	Display[7]=smgduan[second%10];
}
//按键数字处理
void Numpross(u8 dat){
	u8 tp;
	if(flag==1){
		switch(flag1){
			case 0:tp=hour%10;hour=tp+dat*10;break;
			case 1:tp=hour/10;hour=tp*10+dat;break;
			case 3:tp=minute%10;minute=tp+dat*10;break;
			case 4:tp=minute/10;minute=tp*10+dat;break;
			case 6:tp=second%10;second=tp+dat*10;break;
			case 7:tp=second/10;second=tp*10+dat;break;
		}
	}
}
//Up功能处理
void Uppross(){
	if(flag==1){
		flag1++;
		if(flag1>7){
			flag1=0;
		}
		if((flag1==5)||(flag1==2)){
			flag1++;
		}
	}
}
//Down功能处理
void Downpross(){
	if(flag==1){
		flag1--;
		if(flag1<0)
			flag1=7;
		if((flag1==5)||(flag1==2)){
			flag1--;
		}
	}
}
//Func功能处理
void Funcpross(){
	TR0=0;	//停止计时
	flag=1; //允许设置时间
}
//Enter功能处理
void Enterpross(){
	flag=0;		//清零，不允许设置时间
	flag1=0;
	if(hour>23)
		hour=23;
	if(minute>59)
		minute=59;
	if(second>59)
		second=59;
	TR0=1;		//开始计时
}
//Back功能处理
void Backpross(){
//暂无功能
}
//某一位数码管闪烁
void Flash(unsigned char t)				
{	
	unsigned char i;
	num%=50;
	for(i=0;i<8;i++)
	{
		switch(7-i)	  //位选
	   {
		  case 0: LSA=0;LSB=0;LSC=0;break;
		  case 1: LSA=1;LSB=0;LSC=0;break;
		  case 2: LSA=0;LSB=1;LSC=0;break;
		  case 3: LSA=1;LSB=1;LSC=0;break;
		  case 4: LSA=0;LSB=0;LSC=1;break;
		  case 5: LSA=1;LSB=0;LSC=1;break;
		  case 6: LSA=0;LSB=1;LSC=1;break;
		  case 7: LSA=1;LSB=1;LSC=1;break;
	   }
		if((i==t)&&(num<25)){
			P0=0x00;
			Delay(1);
			P0=0x00;
//			continue;
		}
		else
		{
			P0=Display[i];
			Delay(1);
			P0=0x00;
			
		}
	}
	num++;
}
void Timer0() interrupt 1{
	TH0=(65536-9174)/256;   //重置
	TL0=(65536-9174)%256;
	t0++;
	if(t0==100){
		t0=0;
		flag2=1;
	}
}

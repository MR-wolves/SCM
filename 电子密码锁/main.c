/*************************************************************
*项目名称: 智能门锁
*项目平台：Keil5 + 普中科技51单片机
*项目时间：2020-12-21
*作者：云嘉轩
**************************************************************/

/*
项目要求：见PDF文件
因为引脚冲突，直流电机模仿继电器取消。改为P2^0引脚开关
*/

#include "reg52.h"
#include "intrins.h"
#include "at24c02.h"
//按键
sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;
//指示灯
sbit led1=P2^5;
sbit led7=P2^6;
sbit led8=P2^7;
//直流电机
sbit moto=P2^0;
//标志位声明
bit flag;
u8 PassFlag;	//密码标志
u8 PassPriFlag;	//密码自加标志
u8 DoLapFlag;	//门或灯状态标志
u8 PrintFlag;	//输入结束标志
u8 PassRE;		//密码正确错误标志
u8 ModFlag;		//修改密码标志
u8 ModEntFlag;	//进入修改密码标志
u8 tt,time;
u8 UserPassword[6];		//用户输入的密码数组
u8 AdminPassword[6];	//系统保存的密码数组
//u8 AdminPassword[6]={8,8,8,8,8,8};
u8 smgduan[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
				   0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x73,0x37};
u8 DisPlay[8];		//数码管显示数组
#define GPIO_KEY P1
u8 KeyVal;
void Delay(u16 i)     //11.0592MHz  1ms
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
//定时器0初始化
void Time0Init(){
	TMOD=0x01;
	TH0=(65536-9174)/256;	//10ms
	TL0=(65536-9174)%256;
	EA=1;
	ET0=1;
	TR0=0;
}
//数码管显示
void Showsmg(){
	u8 i;
	for(i=0;i<8;i++){
		switch(i){
			case 0: LSA=0;LSB=0;LSC=0;break;
			case 1: LSA=1;LSB=0;LSC=0;break;
			case 2: LSA=0;LSB=1;LSC=0;break;
			case 3: LSA=1;LSB=1;LSC=0;break;
			case 4: LSA=0;LSB=0;LSC=1;break;
			case 5: LSA=1;LSB=0;LSC=1;break;
			case 6: LSA=0;LSB=1;LSC=1;break;
			case 7: LSA=1;LSB=1;LSC=1;break;
		}
		P0=DisPlay[i];
		Delay(1);
		P0=0x00;
	}
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
				case 0x07:KeyVal=0;flag=1;break;
				case 0x0b:KeyVal=1;flag=1;break;
				case 0x0d:KeyVal=2;flag=1;break;
				case 0x0e:KeyVal=3;flag=1;break;
			}
			GPIO_KEY=0xf0;
			switch(GPIO_KEY){
				case 0x70:KeyVal=KeyVal;break;
				case 0xb0:KeyVal=KeyVal+4;break;
				case 0xd0:KeyVal=KeyVal+8;break;
				case 0xe0:KeyVal=KeyVal+12;break;
			}
		}while((a<150)&&(GPIO_KEY!=0xf0)){
			Delay(1);
			a++;
		}
	}
}

//数字按键处理
void Numpross(u8 dat){
	u8 i,j;
	KeyVal=0xff;
	if(PassFlag==1){
		UserPassword[PassPriFlag]=dat;
		for(i=PassPriFlag;i>0;i--){
			DisPlay[i]=DisPlay[i-1];
		}
		DisPlay[i]=smgduan[dat];
		PassPriFlag++;
		if(PassPriFlag==6){
			PassPriFlag=0;
			PassFlag=0;
			PrintFlag=1;
		}
	}
	if(ModFlag==1){
		AdminPassword[PassPriFlag]=dat;
		for(i=PassPriFlag;i>0;i--){
			DisPlay[i]=DisPlay[i-1];
		}
		DisPlay[i]=smgduan[dat];
		PassPriFlag++;
		if(PassPriFlag==6){
			PassPriFlag=0;
			ModFlag=0;
			//写入AT24C02
			for(j=0;j<6;j++)
				Write(j,AdminPassword[j]);
			DoLapFlag=1;	//重新计算时间
			TR0=1;
			moto=0;			//开启电机
			for(j=0;j<8;j++)
				DisPlay[j]=0x00;
			led8=1;			//关闭修改密码指示灯
		}
	}
}
//输入功能处理
void Pripross(){
	u8 j;
	KeyVal=0xff;
	TR0=0;			//关闭定时器
	time=0;
	DisPlay[7]=0x40;	// 显示“-”
	for(j=0;j<7;j++)
		DisPlay[j]=0x00;
	PassFlag=1;	//允许输入密码
	led7=0;
	ModFlag=0;	//不允许修改密码
	led8=1;
	
}
//清除功能处理
void Delpross(){
	u8 k;
	KeyVal=0xff;
	if(PassFlag==1){		//输入密码模式下允许清除
		//重新恢复输入密码状态
		Pripross();
		PassPriFlag=0;	//重新统计密码位数
	}
	if(ModFlag==1){		//修改密码模式下允许清除
		for(k=0;k<=6;k++){
			AdminPassword[k]=0;
			DisPlay[k]=0x00;
		}
		PassPriFlag=0;	//重新统计密码位数
		DisPlay[7]=smgduan[12];
	}
}
//修改功能处理
void Modpross(){
	u8 j;
	KeyVal=0xff;
	if((PassRE==1)){
		PassRE=0;
		DisPlay[7]=smgduan[12];
		for(j=0;j<7;j++)
			DisPlay[j]=0x00;
		ModFlag=1;		//允许修改密码
		led8=0;
		PassFlag=0;		//不允许输入密码
		led7=1;
		DoLapFlag=0;	
		TR0=0;			//关闭定时器
		time=0;
	}
}
//初始化函数
void Init(){
	u8 j;
	//数码管全灭
	for(j=0;j<8;j++)
		DisPlay[j]=0x00;
	//指示灯全灭
	led1=1;
	led7=1;
	led8=1;
	//电机关闭
	moto=1;
	//参数初始化
	PassPriFlag=0;
	DoLapFlag=0;
	PassFlag=0;	
	ModFlag=0;	
	//定时器0初始化
	Time0Init();
	//AT24C02初始化
	At24c02Init();
}
//按键处理
void Keypross(){
	if(flag==1){
		switch(KeyVal){
			case 0:Numpross(0);break;
			case 1:Numpross(1);break;
			case 2:Numpross(2);break;
			case 3:Numpross(3);break;
			case 4:Numpross(4);break;
			case 5:Numpross(5);break;
			case 6:Numpross(6);break;
			case 7:Numpross(7);break;
			case 8:Numpross(8);break;
			case 9:Numpross(9);break;
			case 10:break;
			case 11:break;
			case 12:break;
			case 13:Delpross();break;
			case 14:Modpross();break;
			case 15:Pripross();break;
		}
	}
}
void main(){
	u8 i;
	Init();
//	for(i=0;i<6;i++)
//		Write(i,AdminPassword[i]);
	
	//读取最新密码
	for(i=0;i<6;i++){
		AdminPassword[i]=Read(i);
//		DisPlay[i]=smgduan[AdminPassword[i]];
	}
	while(1){
		KeyDown();
		Keypross();
		Showsmg();
		flag=0;		//检测是否有按键按下
		if(PrintFlag==1){		//输入结束开始判断密码是否正确
			PrintFlag=0;
			for(i=0;i<6;i++){
				if(UserPassword[i]==AdminPassword[i]){
					PassRE=1;
				}
				else{
					PassRE=2;
					break;	//密码错误退出
				}
			}
			TR0=1;
			if(PassRE==1){
				moto=0;		//开启电机
				DisPlay[7]=smgduan[0];		//第一位显示O
				DisPlay[6]=0x00;
				DisPlay[5]=0x00;
				DisPlay[4]=0x00;
				DisPlay[3]=smgduan[0];		//显示O
				DisPlay[2]=smgduan[16];		//显示P
				DisPlay[1]=smgduan[14];		//显示E
				DisPlay[0]=smgduan[17];		//显示N
				DoLapFlag=1;
			}
			if(PassRE==2){
				led1=0;		//L1指示灯亮
				DoLapFlag=2;
			}
		}	

		if(DoLapFlag==1){
			if(time==5){
				if(flag==0){
					time=0;
					TR0=0;
					Init();		//进入初始化状态
				}
				else{
					flag=0;		//重新检测按键
					time=0;
				}
			}
		}
		if(DoLapFlag==2){
			if(time==5){
				time=0;
				TR0=0;
				led1=1;
				Init();		//进入初始化状态
			}
		}
	}
}
void Time0() interrupt 1{
	TH0=(65536-9174)/256;	//10ms
	TL0=(65536-9174)%256;
	tt++;
	if(tt==100){
		tt=0;
		time++;
	}
}

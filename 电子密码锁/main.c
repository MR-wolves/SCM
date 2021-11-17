/*************************************************************
*��Ŀ����: ��������
*��Ŀƽ̨��Keil5 + ���пƼ�51��Ƭ��
*��Ŀʱ�䣺2020-12-21
*���ߣ��Ƽ���
**************************************************************/

/*
��ĿҪ�󣺼�PDF�ļ�
��Ϊ���ų�ͻ��ֱ�����ģ�¼̵���ȡ������ΪP2^0���ſ���
*/

#include "reg52.h"
#include "intrins.h"
#include "at24c02.h"
//����
sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;
//ָʾ��
sbit led1=P2^5;
sbit led7=P2^6;
sbit led8=P2^7;
//ֱ�����
sbit moto=P2^0;
//��־λ����
bit flag;
u8 PassFlag;	//�����־
u8 PassPriFlag;	//�����Լӱ�־
u8 DoLapFlag;	//�Ż��״̬��־
u8 PrintFlag;	//���������־
u8 PassRE;		//������ȷ�����־
u8 ModFlag;		//�޸������־
u8 ModEntFlag;	//�����޸������־
u8 tt,time;
u8 UserPassword[6];		//�û��������������
u8 AdminPassword[6];	//ϵͳ�������������
//u8 AdminPassword[6]={8,8,8,8,8,8};
u8 smgduan[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
				   0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x73,0x37};
u8 DisPlay[8];		//�������ʾ����
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
//��ʱ��0��ʼ��
void Time0Init(){
	TMOD=0x01;
	TH0=(65536-9174)/256;	//10ms
	TL0=(65536-9174)%256;
	EA=1;
	ET0=1;
	TR0=0;
}
//�������ʾ
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
//�������
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

//���ְ�������
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
			//д��AT24C02
			for(j=0;j<6;j++)
				Write(j,AdminPassword[j]);
			DoLapFlag=1;	//���¼���ʱ��
			TR0=1;
			moto=0;			//�������
			for(j=0;j<8;j++)
				DisPlay[j]=0x00;
			led8=1;			//�ر��޸�����ָʾ��
		}
	}
}
//���빦�ܴ���
void Pripross(){
	u8 j;
	KeyVal=0xff;
	TR0=0;			//�رն�ʱ��
	time=0;
	DisPlay[7]=0x40;	// ��ʾ��-��
	for(j=0;j<7;j++)
		DisPlay[j]=0x00;
	PassFlag=1;	//������������
	led7=0;
	ModFlag=0;	//�������޸�����
	led8=1;
	
}
//������ܴ���
void Delpross(){
	u8 k;
	KeyVal=0xff;
	if(PassFlag==1){		//��������ģʽ���������
		//���»ָ���������״̬
		Pripross();
		PassPriFlag=0;	//����ͳ������λ��
	}
	if(ModFlag==1){		//�޸�����ģʽ���������
		for(k=0;k<=6;k++){
			AdminPassword[k]=0;
			DisPlay[k]=0x00;
		}
		PassPriFlag=0;	//����ͳ������λ��
		DisPlay[7]=smgduan[12];
	}
}
//�޸Ĺ��ܴ���
void Modpross(){
	u8 j;
	KeyVal=0xff;
	if((PassRE==1)){
		PassRE=0;
		DisPlay[7]=smgduan[12];
		for(j=0;j<7;j++)
			DisPlay[j]=0x00;
		ModFlag=1;		//�����޸�����
		led8=0;
		PassFlag=0;		//��������������
		led7=1;
		DoLapFlag=0;	
		TR0=0;			//�رն�ʱ��
		time=0;
	}
}
//��ʼ������
void Init(){
	u8 j;
	//�����ȫ��
	for(j=0;j<8;j++)
		DisPlay[j]=0x00;
	//ָʾ��ȫ��
	led1=1;
	led7=1;
	led8=1;
	//����ر�
	moto=1;
	//������ʼ��
	PassPriFlag=0;
	DoLapFlag=0;
	PassFlag=0;	
	ModFlag=0;	
	//��ʱ��0��ʼ��
	Time0Init();
	//AT24C02��ʼ��
	At24c02Init();
}
//��������
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
	
	//��ȡ��������
	for(i=0;i<6;i++){
		AdminPassword[i]=Read(i);
//		DisPlay[i]=smgduan[AdminPassword[i]];
	}
	while(1){
		KeyDown();
		Keypross();
		Showsmg();
		flag=0;		//����Ƿ��а�������
		if(PrintFlag==1){		//���������ʼ�ж������Ƿ���ȷ
			PrintFlag=0;
			for(i=0;i<6;i++){
				if(UserPassword[i]==AdminPassword[i]){
					PassRE=1;
				}
				else{
					PassRE=2;
					break;	//��������˳�
				}
			}
			TR0=1;
			if(PassRE==1){
				moto=0;		//�������
				DisPlay[7]=smgduan[0];		//��һλ��ʾO
				DisPlay[6]=0x00;
				DisPlay[5]=0x00;
				DisPlay[4]=0x00;
				DisPlay[3]=smgduan[0];		//��ʾO
				DisPlay[2]=smgduan[16];		//��ʾP
				DisPlay[1]=smgduan[14];		//��ʾE
				DisPlay[0]=smgduan[17];		//��ʾN
				DoLapFlag=1;
			}
			if(PassRE==2){
				led1=0;		//L1ָʾ����
				DoLapFlag=2;
			}
		}	

		if(DoLapFlag==1){
			if(time==5){
				if(flag==0){
					time=0;
					TR0=0;
					Init();		//�����ʼ��״̬
				}
				else{
					flag=0;		//���¼�ⰴ��
					time=0;
				}
			}
		}
		if(DoLapFlag==2){
			if(time==5){
				time=0;
				TR0=0;
				led1=1;
				Init();		//�����ʼ��״̬
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

#include "at24c02.h"

void Delayus(){     //10us
	unsigned char i;

	i = 2;
	while (--i);
}

//起始信号
void Start(){
	SDA=1;
	Delayus();
	SCL=1;
	Delayus();
	SDA=0;
	Delayus();
}

//停止信号
void Stop(){
	SDA=0;
	Delayus();
	SCL=1;
	Delayus();
	SDA=1;
	Delayus();
}

//应答信号
void Respons(){
	unsigned char i=0;
	SCL=1;
	Delayus();
	while((SDA==1)&&(i<250))
		i++;
	Delayus();
	SCL=0;
	Delayus();
}
//初始化
void At24c02Init(){
	SCL=1;
	Delayus();
	SDA=1;
	Delayus();
}

//写入字节
void WriteByte(unsigned char dat){
	unsigned char i,temp;
	temp=dat;
	for(i=0;i<8;i++){     //循环过程中，要想写入SDA数据，先拉低SCL,写完拉高SCL稳定数据
		temp<<=1;
		SCL=0;
		Delayus();
		SDA=CY;
		Delayus();
		SCL=1;
	}
	SCL=0;
	Delayus();
	SDA=1;
	Delayus();
}

//读出字节
unsigned char ReadByte(){
	unsigned char i,temp;
	SCL=0;
	Delayus();
	SDA=1;
	Delayus();
	for(i=0;i<8;i++){   //要想读出数据，先拉高SCL稳定数据，再把SDA数据传出，SCL拉低，让SDA数据改变
		SCL=1;
		Delayus();
		temp=(temp<<1|SDA);
		Delayus();
		SCL=0;
		Delayus();
	}
	return temp;
}

//字节写入方式
void Write(unsigned char address,unsigned char date){
	Start();
	WriteByte(0xa0);
	Respons();
	WriteByte(address);
	Respons();
	WriteByte(date);
	Respons();
	Stop();
}

//字节读出操作
unsigned char Read(unsigned char address){
	unsigned char date;
	Start();
	WriteByte(0xa0);
	Respons();
	WriteByte(address);
	Respons();
	Start();
	WriteByte(0xa1);
	Respons();
	date=ReadByte();
	Stop();
	return date;
}


#include "at24c02.h"

void Delayus(){     //10us
	unsigned char i;

	i = 2;
	while (--i);
}

//��ʼ�ź�
void Start(){
	SDA=1;
	Delayus();
	SCL=1;
	Delayus();
	SDA=0;
	Delayus();
}

//ֹͣ�ź�
void Stop(){
	SDA=0;
	Delayus();
	SCL=1;
	Delayus();
	SDA=1;
	Delayus();
}

//Ӧ���ź�
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
//��ʼ��
void At24c02Init(){
	SCL=1;
	Delayus();
	SDA=1;
	Delayus();
}

//д���ֽ�
void WriteByte(unsigned char dat){
	unsigned char i,temp;
	temp=dat;
	for(i=0;i<8;i++){     //ѭ�������У�Ҫ��д��SDA���ݣ�������SCL,д������SCL�ȶ�����
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

//�����ֽ�
unsigned char ReadByte(){
	unsigned char i,temp;
	SCL=0;
	Delayus();
	SDA=1;
	Delayus();
	for(i=0;i<8;i++){   //Ҫ��������ݣ�������SCL�ȶ����ݣ��ٰ�SDA���ݴ�����SCL���ͣ���SDA���ݸı�
		SCL=1;
		Delayus();
		temp=(temp<<1|SDA);
		Delayus();
		SCL=0;
		Delayus();
	}
	return temp;
}

//�ֽ�д�뷽ʽ
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

//�ֽڶ�������
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


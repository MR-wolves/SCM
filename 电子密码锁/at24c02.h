#ifndef _AT24C02_H
#define _AT24C02_H

#include "reg52.h"
#include "at24c02.h"
#include "intrins.h"

#ifndef u8
#define u8 unsigned char
#endif
#ifndef u16
#define u16 unsigned int
#endif

sbit SDA=P2^0;
sbit SCL=P2^1;

void Delayus();
//��ʼ�ź�
void Start();
//ֹͣ�ź�
void Stop();
//Ӧ���ź�
void Respons();
//��ʼ��
void At24c02Init();
//д���ֽ�
void WriteByte(unsigned char dat);
//�����ֽ�
unsigned char ReadByte();
//�ֽ�д�뷽ʽ
void Write(unsigned char address,unsigned char date);
//�ֽڶ�������
unsigned char Read(unsigned char address);



#endif
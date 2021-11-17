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
//起始信号
void Start();
//停止信号
void Stop();
//应答信号
void Respons();
//初始化
void At24c02Init();
//写入字节
void WriteByte(unsigned char dat);
//读出字节
unsigned char ReadByte();
//字节写入方式
void Write(unsigned char address,unsigned char date);
//字节读出操作
unsigned char Read(unsigned char address);



#endif
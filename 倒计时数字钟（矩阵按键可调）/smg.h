/*************************************************************
*��ĿҪ��
    ����ʱ�����ӹ���:��ʵ�ֵ���ʱ�����ӹ���,��ֵ�����á�Ҫ��:
��ֵΪ00-00-00������ʱ�����������ʱ��ʵʱ��ʾ����ʱʱ�䣨��ʾ
��ʽΪXX-XX-XX����Сʱ-����-�룩����ʱΪ0ʱֹͣ��������ʱ����
������������������FUNC���ɽ���ʱ�����ã������õ����ִ�������
��6��������DOWN��/UP������6�����ּ��л���ѡ��Ҫ���õ�λ�ã�
��ѡ��λ�õ����ֲ�������仯���������ּ�0~9�������ã�������
���ٰ���DOWN��/UP�����Լ������ñ��λ�����ENTER���������
��������ʱ�� ���󰴼���Ӧ���֣�
7    8    9       UP
4    5    6       DOWN
1    2    3       FUNC
0    .    BACK    ENTER
***************************************************************/


#include "reg52.h"
#include "intrins.h"

typedef unsigned int u16;
typedef unsigned char u8;
#define GPIO_KEY P1
u8 smgduan[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};

u8 t0,KeyVal,Display[8],flag,flag1,flag2,flag3,num;
u16 hour,minute,second;				   
//38������					 
sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;

//�������ʾ����
void Showsmg();
//��ʱ����
void Delay(u16 i);
//��ʼ������
void Init();
//�������
void KeyDown();
//��������
void Keypross();
//���ݴ���
void Datapross();
//�������ִ���
void Numpross(u8 dat);
//Up���ܴ���
void Uppross();
//Down���ܴ���
void Downpross();
//Func���ܴ���
void Funcpross();
//Enter���ܴ���
void Enterpross();
//Back���ܴ���
void Backpross();
//ĳһλ�������˸
void Flash(unsigned char t);		
/*************************************************************
*项目要求：
    倒计时数字钟功能:可实现倒计时数字钟功能,初值可设置。要求:
初值为00-00-00，设置时间后启动倒计时，实时显示倒计时时间（显示
格式为XX-XX-XX，即小时-分钟-秒），计时为0时停止（再设置时间能
够重新启动）；按下FUNC键可进行时间设置，需设置的数字从左至右
共6个，按下DOWN键/UP键可在6个数字间切换以选择要设置的位置，
被选中位置的数字不断亮灭变化，按下数字键0~9进行设置，设置完
后再按下DOWN键/UP键可以继续设置别的位，最后按ENTER键完成设置
启动倒计时； 矩阵按键对应名字：
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
//38译码器					 
sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;

//数码管显示函数
void Showsmg();
//延时函数
void Delay(u16 i);
//初始化函数
void Init();
//按键检测
void KeyDown();
//按键处理
void Keypross();
//数据处理
void Datapross();
//按键数字处理
void Numpross(u8 dat);
//Up功能处理
void Uppross();
//Down功能处理
void Downpross();
//Func功能处理
void Funcpross();
//Enter功能处理
void Enterpross();
//Back功能处理
void Backpross();
//某一位数码管闪烁
void Flash(unsigned char t);		
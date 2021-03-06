/****************************************************************************
* 擅长单片机开发，蓝牙相关开发，承接项目，共赢未来!
* 项目合作电话:   18588220515
* 项目合作QQ:     11940507   (请注明合作意向) 
* 公司地址:       深圳市宝安区电子数码城4F10
* 淘宝店铺:       http://amomcu.taobao.com/
* 论坛支持:       www.amomcu.com

* 文 件 名: main.c
* 作    者: Amo [ www.amoMcu.com 阿莫单片机]
* 修    订: 2014-04-08
* 版    本: 1.0
* 描    述: 通过按键S1产生外部中断改变LED1状态
****************************************************************************/
#include <ioCC2540.h>
#include "amomcu_buffer.h"
#include "OSAL.h"
#include "simpleBLE.h"
#include "hal_types.h"

typedef unsigned char uchar;
typedef unsigned int  uint;

uint8 vibra_value[2]={0};

#define LED1 P1_0       // P1.0口控制LED1
#define LED2 P1_1       // P1.1口控制LED2
#define KEY1 P0_1       // P0.1口控制S1


/****************************************************************************
* 名    称: DelayMS()
* 功    能: 以毫秒为单位延时，系统时钟不配置时默认为16M(用示波器测量相当精确)
* 入口参数: msec 延时参数，值越大，延时越久
* 出口参数: 无
****************************************************************************/
void DelayMS(uint msec)
{ 
    uint i,j;
    
    for (i=0; i<msec; i++)
        for (j=0; j<535; j++);
}

/****************************************************************************
* 名    称: InitLed()
* 功    能: 设置LED灯相应的IO口
* 入口参数: 无
* 出口参数: 无
****************************************************************************/
void InitLed(void)
{
    //P1DIR |= 0x01;   //P1.0定义为输出口    ,P1.0   我们的纸尿裤没用
   // LED2 = 0;        //LED2灯灭 ,P1.1   我们的纸尿裤没用
}

/****************************************************************************
* 名    称: InitKey()
* 功    能: 设置KEY相应的IO口,采用中断方式 
* 入口参数: 无
* 出口参数: 无
****************************************************************************/
void InitKey(void)
{
			
    P0IEN |= 0x1;    // P0.0 设置为中断方式 1：中断使能
    PICTL |= 0x2;    //下降沿触发   
    //IEN1 |= 0x20;    //允许P0口中断;   在蓝牙断开和连接的地方进行中断的使能，降功耗
    P0IFG = 0x00;    //初始化中断标志位
    EA = 1;          //打开总中断
}



//void clear_vibra(void)
//{
//    vibra_value=0;
//}

/****************************************************************************
* 名    称: P0_ISR(void) 中断处理函数 
* 描    述: #pragma vector = 中断向量，紧接着是中断处理程序
****************************************************************************/
#pragma vector = P0INT_VECTOR    

__interrupt void P0_ISR(void) 
{     
    // 启动事件，然后在事件中再启动定时器定时检测数据并发送到网络
    vibra_value[0]=0xFF;
    vibra_value[1]=0xFF;
    qq_write(vibra_value,2);
    
//    osal_set_event(simpleBLETaskId, SBP_VABRATIVE_EVT);  
    osal_set_event(simpleBLETaskId, SBP_DATA_EVT); 
    //osal_mem_free(&buffer);
//    clear_vibra();
    //DelayMS(10);     //延时去抖（此处加上延时会造成蓝牙连接的不稳定）
   // LED2 = ~LED2;    //改变LED2状态
    P0IFG = 0;       //清中断标志 
    P0IF = 0;        //清中断标志 
} 





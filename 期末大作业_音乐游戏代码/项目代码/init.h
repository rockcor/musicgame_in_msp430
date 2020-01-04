// 这里是各种初始化函数，如果没有特殊情况的话应该不需要改动

#ifndef INIT_H_
#define INIT_H_

#include <msp430f6638.h>

void initClock()                            //初始化时钟系统（UCS）
{
  while(BAKCTL & LOCKIO) // Unlock XT1 pins for operation
    BAKCTL &= ~(LOCKIO);
  UCSCTL6 &= ~XT1OFF; //启动XT1
  P7SEL |= BIT2 + BIT3; //XT2引脚功能选择
  UCSCTL6 &= ~XT2OFF; //启动XT2
  while (SFRIFG1 & OFIFG) { //等待XT1、XT2与DCO稳定
    UCSCTL7 &= ~(DCOFFG+XT1LFOFFG+XT2OFFG);
    SFRIFG1 &= ~OFIFG;
  }

  UCSCTL4 = SELA__XT2CLK + SELS__XT2CLK + SELM__XT2CLK; //避免DCO调整中跑飞

  UCSCTL1 = DCORSEL_0;
  UCSCTL2 = 7; //XT2频率较高，分频后作为基准可获得更高的精度
  UCSCTL3 = SELREF__REFOCLK + FLLREFDIV__1; //XT2进行16分频后作为基准
  while (SFRIFG1 & OFIFG) { //等待XT1、XT2与DCO稳定
    UCSCTL7 &= ~(DCOFFG+XT1LFOFFG+XT2OFFG);
    SFRIFG1 &= ~OFIFG;
  }
  UCSCTL5 = DIVA__1 + DIVS__1 + DIVM__1; //设定几个CLK的分频
  UCSCTL4 = SELA__DCOCLK + SELS__XT2CLK + SELM__XT2CLK; //设定几个CLK的时钟源                   理论上，ACLK使用的目前DCOLCK的频率应该是32768*8Hz
}

void initAudio()                         //初始化音频模块
{
    //配置REF
    REFCTL0 |= REFON; //默认参数1.5V

    //配置DAC
    DAC12_0CTL0 &= ~DAC12ENC;
    DAC12_0CTL0 = DAC12OPS + DAC12SREF_0 + DAC12LSEL_0 + DAC12AMP_7 + DAC12IR; //P7.6, VREF+, 12bit, 最高速模式, 1倍增益
    DAC12_0CTL1 = DAC12OG; //无DAC12IR时2倍增益
    DAC12_0CTL0 |= DAC12ENC;
    DAC12_0DAT = 0;
}

void initCapTouch()
{
  //Vref加到+极
  CBCTL2 = CBREF14+CBREF13+CBREF02;
  CBCTL1 = CBON + CBF;
  CBCTL2 |= CBRS_1;
  CBCTL3 = ALL_PORT;
}

#endif /* INIT_H_ */

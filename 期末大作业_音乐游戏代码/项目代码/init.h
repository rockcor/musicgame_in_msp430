// �����Ǹ��ֳ�ʼ�����������û����������Ļ�Ӧ�ò���Ҫ�Ķ�

#ifndef INIT_H_
#define INIT_H_

#include <msp430f6638.h>

void initClock()                            //��ʼ��ʱ��ϵͳ��UCS��
{
  while(BAKCTL & LOCKIO) // Unlock XT1 pins for operation
    BAKCTL &= ~(LOCKIO);
  UCSCTL6 &= ~XT1OFF; //����XT1
  P7SEL |= BIT2 + BIT3; //XT2���Ź���ѡ��
  UCSCTL6 &= ~XT2OFF; //����XT2
  while (SFRIFG1 & OFIFG) { //�ȴ�XT1��XT2��DCO�ȶ�
    UCSCTL7 &= ~(DCOFFG+XT1LFOFFG+XT2OFFG);
    SFRIFG1 &= ~OFIFG;
  }

  UCSCTL4 = SELA__XT2CLK + SELS__XT2CLK + SELM__XT2CLK; //����DCO�������ܷ�

  UCSCTL1 = DCORSEL_0;
  UCSCTL2 = 7; //XT2Ƶ�ʽϸߣ���Ƶ����Ϊ��׼�ɻ�ø��ߵľ���
  UCSCTL3 = SELREF__REFOCLK + FLLREFDIV__1; //XT2����16��Ƶ����Ϊ��׼
  while (SFRIFG1 & OFIFG) { //�ȴ�XT1��XT2��DCO�ȶ�
    UCSCTL7 &= ~(DCOFFG+XT1LFOFFG+XT2OFFG);
    SFRIFG1 &= ~OFIFG;
  }
  UCSCTL5 = DIVA__1 + DIVS__1 + DIVM__1; //�趨����CLK�ķ�Ƶ
  UCSCTL4 = SELA__DCOCLK + SELS__XT2CLK + SELM__XT2CLK; //�趨����CLK��ʱ��Դ                   �����ϣ�ACLKʹ�õ�ĿǰDCOLCK��Ƶ��Ӧ����32768*8Hz
}

void initAudio()                         //��ʼ����Ƶģ��
{
    //����REF
    REFCTL0 |= REFON; //Ĭ�ϲ���1.5V

    //����DAC
    DAC12_0CTL0 &= ~DAC12ENC;
    DAC12_0CTL0 = DAC12OPS + DAC12SREF_0 + DAC12LSEL_0 + DAC12AMP_7 + DAC12IR; //P7.6, VREF+, 12bit, �����ģʽ, 1������
    DAC12_0CTL1 = DAC12OG; //��DAC12IRʱ2������
    DAC12_0CTL0 |= DAC12ENC;
    DAC12_0DAT = 0;
}

void initCapTouch()
{
  //Vref�ӵ�+��
  CBCTL2 = CBREF14+CBREF13+CBREF02;
  CBCTL1 = CBON + CBF;
  CBCTL2 |= CBRS_1;
  CBCTL3 = ALL_PORT;
}

#endif /* INIT_H_ */

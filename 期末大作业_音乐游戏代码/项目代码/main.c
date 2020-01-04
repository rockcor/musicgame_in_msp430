#include <msp430f6638.h>
#include <stdint.h>
#include "dr_tft.h"
#include "fw_public.h"
#include <stdio.h>
//��������Լ������ͷ�ļ�
#include "IO.h"
#include "config.h"
#include "SongData.h"
#include "init.h"
#include "picture.h"

uint16_t noteDataLen, musicDataLen1, musicDataLen2;
uint16_t noteIndex, musicIndex1, musicIndex2;

int frame = 0;                              //����������,������¼��ǰ��֡��

uint16_t track[4][10] = {0};                //��¼��Ļ������Щ������������,���нṹ,�Ƚ��ȳ�,���ڿռ�����,ʮ��λ����ѭ��ʹ�õ�
uint8_t trackHead[4] = {0};                 //��¼����ͷ��β��λ��,��һ֡���ж���ֻ������ͷ��Ӧ�ķ�����miss��good��perfect�������ж�����,
uint8_t trackTail[4] = {0};                 //�������β��¼����ʵ�Ƕ��������һ��Ԫ�ص���һ��λ��,Ҳ���������ķ���Ӧ�÷��ڵ�λ��

uint16_t currentAudioOutput = 0;            //������ǰ��Ƶ��������Ǿ���ֵ,���Ƿ�ֵ,�������Ͳ����������,ֻ�����һ���㶨���źţ�
uint8_t musicState1 = 0;                    //������¼���1Ŀǰ�Ǵ��ڷ����ĸߵ�ƽ���ǵ͵�ƽ
uint8_t musicState2 = 0;

uint8_t keyState[5] = {0};                  //�ĸ�������״̬,

uint16_t x1, x2, y1, y2;                    //AreaSet������
uint16_t frameDelta;

const uint16_t perfectpoint=3;        //perfect��3��
const uint16_t goodpoint=1;      //good��1��


float point=0;
int k=1;                        //����ѡ�����
int i=0;                        //ͨ�õ�ѭ������,����û�ж���Ϊuint16_t����ԭ���






/*
void choose_song(void)  //ѡ�躯������ѯ�����ı䷽��λ�ú�k��ֵ
{
    if(P4IN&BIT1)
    {
       k-=1;
       _delay_cycles(6600);
    }
    if(P4IN&BIT3)
    {
      k+=1;
      _delay_cycles(6600);
    }
}
*/

/*void squaremove(int k)                //����ѡ����ʾ����
{
   if(k==0)
   {
       etft_AreaSet(0, 0, 20, 20, 0);
       etft_AreaSet(20,20, 40, 40, 65535);
       k=3;
       return;
   }
   if(k==1)
   {
       etft_AreaSet(0, 0, 0, 0, 0);
       etft_AreaSet(0, 0, 0, 0, 65535);
       return;
   }
   if(k==2)
     {
         etft_AreaSet(0, 0, 0, 0, 0);
         etft_AreaSet(0, 0, 0, 0, 65535);
         return;
     }
   if(k==3)
     {
         etft_AreaSet(0, 0, 0, 0, 0);
         etft_AreaSet(0, 00, 0, 0, 65535);
         return;
     }
   if(k==4)
     {
         etft_AreaSet(0, 0, 0, 0, 0);
         etft_AreaSet(0, 0, 0, 0, 65535);
         k=0;
         return;
     }
}*/

    void tftpoint(float slope)  //�����������洢���ַ�����
    {
        int temp,i,j;
        unsigned char buffer[7];
        temp = (int)slope;//ȡ��������
        for(i=0;temp!=0;i++)//�����������ֵ�λ��
        temp /=10;
        temp =(int)slope;
        for(j=i;j>0;j--)//����������ת�����ַ�����
        {
            buffer[j-1] = temp%10+'0';
            temp /=10;
        }
        buffer[i] = '.';
        slope -=(int)slope;
        for(i=i+1;i<6;i++)//��С������ת�����ַ�����
        {
            slope*=10;
            buffer[i]=(int)slope+'0';
            slope-=(int)slope;
        }
        buffer[5] = '%';
        buffer[6] = '\0';
        etft_DisplayString(buffer,130,120,65535,0);
    }



void jiesuan()         //�������
{
	etft_AreaSet(0, 0, 319, 239, 0);
    uint16_t length;
    uint16_t x;
    uint16_t y;
    float per=0;
    per=point/Point;
    per*=100;    //ת��Ϊ�ٷ���
  // if(per>0.99)
 //  {
	   length = sizeof(lsss) / sizeof(coordinate);

	           for (i = 0; i < length; i++)
	           {
	               x = lsss[i].x;
	               y = lsss[i].y;
	               etft_AreaSet(x, y, x, y, 65535);

	           }
	           tftpoint(per);
	           __delay_cycles(MCLK_FREQ*30);

       return;
 //  }

/*
   else if(per>0.95)
   {
	   length = sizeof(lss) /  sizeof(coordinate);

	   	           for (i = 0; i < length; i++)
	   	           {
	   	               x = lss[i].x;
	   	               y = lss[i].y;
	   	               etft_AreaSet(x, y, x, y, 65535);

	   	           }
	   	           __delay_cycles(MCLK_FREQ*3); //ss
   //    tftpoint(per);
       return;
   }
   else if(per>0.9)
     {
	   length = sizeof(ls) / sizeof(coordinate);

	   	           for (i = 0; i < length; i++)
	   	           {
	   	               x = ls[i].x;
	   	               y = ls[i].y;
	   	               etft_AreaSet(x, y, x, y, 65535);

	   	           }
	   	           __delay_cycles(MCLK_FREQ*3); //s
    //     tftpoint(per);
         return;
     }
   else if(per>0.8)
     {
	   length = sizeof(la) / sizeof(coordinate);

	   	           for (i = 0; i < length; i++)
	   	           {
	   	               x = la[i].x;
	   	               y = la[i].y;
	   	               etft_AreaSet(x, y, x, y, 65535);

	   	           }
	   	           __delay_cycles(MCLK_FREQ*3);  //a
    //     tftpoint(per);
         return;
     }
   else if(per>0.7)
     {
	   length = sizeof(lb) /  sizeof(coordinate);

	   	           for (i = 0; i < length; i++)
	   	           {
	   	               x = lb[i].x;
	   	               y = lb[i].y;
	   	               etft_AreaSet(x, y, x, y, 65535);

	   	           }
	   	           __delay_cycles(MCLK_FREQ*3);  //b
     //    tftpoint(per);
         return;
     }

   else

	   length = sizeof(lc) /  sizeof(coordinate);

	   	           for (i = 0; i < length; i++)
	   	           {
	   	               x = lc[i].x;
	   	               y = lc[i].y;
	   	               etft_AreaSet(x, y, x, y, 65535);

	   	           }
	   	           __delay_cycles(MCLK_FREQ*3);  //c
     //    tftpoint(per);
         return;
*/

}








void CapTouch_ReadChannel(void)             //������������ĸ�pad��״̬����¼��keyState������ pad2��Ӧkeystate[3],pad3��Ӧkeystate[2],pad4��Ӧkeystate[1],pad5��Ӧkeystate[0]
{
  uint16_t cpu_cnt = 0;
  uint16_t osc_cnt = 0;

  for (i = 4; i > 0; i--)
  {
      cpu_cnt = 0;
      osc_cnt = 0;
      CBCTL0 = CBIMEN + (i << 8); //�ⲿ�źżӵ�����
      P6OUT &= ~ALL_PORT;
      P6DIR |= ALL_PORT & ~(1 << i);
      CBCTL3 = 1 << i;

      while(1)
      {
        if(CBCTL1 & CBOUT)
          P6OUT |= ALL_PORT;
        else
          P6OUT &= ~ALL_PORT;

        if(CBINT & CBIFG)
        {
          CBINT &= ~CBIFG;
          osc_cnt++;
        }

        if(osc_cnt >= OSC_CYCLES)
          break;

        cpu_cnt++;
      }

      if (cpu_cnt > captouch_max) keyState[i - 1] = 1;
      else keyState[i - 1] = 0;
  }

  CBCTL3 = ALL_PORT;
  P6DIR &= ~ALL_PORT;
}

void assessing(int assessType)                                          //�ж�����miss����������У��������Ϊ0����perfect��Ϊ1����good
{
    if (assessType == 0)
    {
        point += perfectpoint;
        P1OUT |= BIT5;
    }
    if (assessType == 1)
     {
        point += goodpoint;
        P1OUT |= BIT5;
     }

}




/*uint16_t  noteData[500]={0};
uint16_t  musicData1[500]={0};
uint16_t  musicData2[500]={0};
uint16_t  musicFrame1[500]={0};
uint16_t  musicFrame2[500]={0};
*/
int main()
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer


    initClock();
    initAudio();
    initTFT();
    initCapTouch();
    etft_AreaSet(0, 0, 319, 239, 0); //����
    P1DIR |= BIT5;                                                      //�������˿ڷ�����Ϊ���
    P4REN |= BIT1+BIT2+BIT3;
    P4OUT |= BIT1+BIT2+BIT3;


    TA2CCTL0 = CCIE;                                                    //�ж�ʹ��
    TA2CCR0 = 8738;                                                     //32768 * 8 / 30 = 8738,30fps
    TA2CTL = TASSEL_1 + MC_1 + TACLR;                                   //ѡ��ACLK,������ʽ,�������
    TA1CCTL0 = TA0CCTL0 = CCIE;
    TA1CCR0 = TA0CCR0 = 65535;                                          //���ڻ�û��ʼ�������������Լ�����ֵ�ܴ�,���������Ƶ�����������ź�
    TA1CTL = TA0CTL = (TASSEL_1 + MC_1 + TACLR);


     uint16_t length=0;
     uint16_t x;
     uint16_t y;

     length = sizeof(welcome) / sizeof(coordinate);

        for (i = 0; i < length; i++)
        {
            x = welcome[i].x;
            y = welcome[i].y;
            etft_AreaSet(x, y, x, y, 65535);

        }
        __delay_cycles(10000);

        while(P4IN&BIT2)          //ȷ�ϼ�û������һֱΪ1
           {
        //       choose_song();
       //        squaremove(k);
           }
        etft_AreaSet(0, 0, 319, 239, 0); //����

     /*   for(i=0;i<500;++i)    //���ݲ˵�ѡ��ĸ����������鸳ֵ
        {
          noteData[i]=notedata[k][i];
          musicData1[i]=musicdata1[k][i];
          musicData2[i]=musicdata2[k][i];
          musicFrame1[i]=musicdata1[k][i];
          musicFrame2[i]=musicdata2[k][i];
        }

*/
            noteDataLen = sizeof(noteData) / sizeof(noteData[0]);               //�����Щ����ĳ���,��������Ҫ��Щ��������ֹ����Խ�磨��Index�������鳤��ʱ�Ͳ��ٸ����ж��Ͳ���״̬��
            musicDataLen1 = sizeof(musicData1) / sizeof(musicData1[1]);
            musicDataLen2 = sizeof(musicData2) / sizeof(musicData2[1]);
            noteIndex = musicIndex1 = musicIndex2 = 0;                          //�Խ�Ҫʹ�õ������������൱�ڹ���ָ�룩���г�ʼ��

            for(i = 0; i < 5; ++i)
                *LED_GPIO[i] -> PxDIR |= LED_PORT[i]; //���ø�LED�����ڶ˿�Ϊ�������
            __bis_SR_register(GIE);
            return 0;
}

#pragma vector = TIMER2_A0_VECTOR
__interrupt void Timer2_A0 (void)
{
    __bis_SR_register(GIE);                                             //��һ�����������ж�Ƕ��
    uint16_t tempData;                                                  //��θ����noteData��Ӧ�ÿ�ʼ����ķ���Ž�track����
    tempData = noteData[noteIndex];


    while ((frame >= (tempData & 0x3FFF)) && (noteIndex < noteDataLen))
    {
        uint8_t trackNumber;
        trackNumber = ((tempData & 0x8000) != 0) * 2 + ((tempData & 0x4000) != 0);
        track[trackNumber][trackTail[trackNumber]] = tempData;
        trackTail[trackNumber] ++;
        trackTail[trackNumber] %= 10;
        noteIndex ++;
        tempData = noteData[noteIndex];

    }

    for (i = 0; i < 4; i++)                                             //��һ��ˢ�¶����еķ�����TFT�ϵ���ʾ(�������ж���
    {
        uint8_t tempIndex;
        tempIndex = trackHead[i];

        while (tempIndex != trackTail[i])
        {
            frameDelta = frame - track[i][tempIndex] & 0x3FFF;

            x1 = 80 * i + 20;               //ֱ��Ӳ�����ȥ��
            y1 = (frameDelta - 1) * (noteEnd - noteStart) / noteFallingFrame + noteStart;
            x2 = x1 + 40;
            y2 = y1 + noteHeight;

            if (frameDelta) etft_AreaSet(x1, y1, x2, y2, 0); //�Ȳ�����һ֡��ʾ�ľ���

            if (frameDelta <= noteFallingFrame + missWindow)    //�����������perfectframe+misswindow��֡����������һ֡���Ļ��Լ��ͻ���ʧ��û�м�ʱ�����
            {
                y1 = frameDelta * (noteEnd - noteStart) / noteFallingFrame + noteStart;
                y2 = y1 + noteHeight;
                etft_AreaSet(x1, y1, x2, y2, 65535); //����ʾ�µľ���
            }
            else
            {
                assessing(2);                        //û�м�ʱ�����miss��
                trackHead[i] ++;
                trackHead[i] %= 10;
            }

            tempIndex ++;
            tempIndex %= 10;
        }
    }

    if ((frame >= musicFrame1[musicIndex1]) && (musicIndex1 < musicDataLen1))       //����޸ķ�����ʱ����Ƶ�ʣ�Ҳ���Ǹ���ı䷢��������Ƶ��
    {
        TA0CCR0 = musicData1[musicIndex1];
        TA0R = 0;
        musicIndex1 ++;
        if(musicData1[musicIndex1]==0)
        {
                _DINT();
        		jiesuan();
        		_delay_cycles(30000);
        		etft_AreaSet(0, 0, 319, 239, 0);
        }
    }
    if ((frame >= musicFrame2[musicIndex2]) && (musicIndex2 < musicDataLen2))
    {
        TA1CCR0 = musicData2[musicIndex2];
        TA1R = 0;
        musicIndex2 ++;
    }

    P1OUT &= ~BIT5;             //�ط�����
    CapTouch_ReadChannel();     //��ȡ�ĸ�pad��״̬
    for (i = 3; i >= 0; i--)    //��ν����ж�
    {
        if (keyState[i])
        {
            *LED_GPIO[i] -> PxOUT |= LED_PORT[i];       //�����һ֡pad������,������Ӧ��led��
            if (trackHead[3 - i] != trackTail[3 - i])
            {
                int tempAssessData, perfectDelta;
                tempAssessData = (track[3 - i][trackHead[3 - i]] & 0x3FFF);
                frameDelta = frame - tempAssessData;
                perfectDelta = frameDelta - noteFallingFrame;
                if (perfectDelta < 0) perfectDelta = -perfectDelta;
                if (perfectDelta < assessWindow)
                {
                    if (perfectDelta < goodWindow) {assessing(0);}        //���������perfect
                    else
                    {
                        if (perfectDelta < missWindow) {assessing(1);}    //���������good
                        else {assessing(2);}                      //�������miss��
                    }
                    x1 = 80 * (3 - i) + 20;
                    y1 = frameDelta * (noteEnd - noteStart) / noteFallingFrame + noteStart;
                    x2 = x1 + 40;
                    y2 = y1 + noteHeight;
                    etft_AreaSet(x1, y1, x2, y2, 0);             //�Ѿ��ж���������
                    trackHead[3 - i] ++;
                    trackHead[3 - i] %= 10;
                }
            }
        }
        else *LED_GPIO[i] -> PxOUT &= ~LED_PORT[i];             //�����һ֡padû������,Ϩ���Ӧ��led��
    }
    etft_AreaSet(10, noteEnd, 309,  noteEnd, 65535);            //���ж���
    frame ++;

}

#pragma vector = TIMER0_A0_VECTOR                   //���������ź�  A0��A1�����ȼ���A2��,��Ҫ�����ȼ���ߵ��жϷ���������źŵ����,���������������
__interrupt void Timer0_A0 (void)
{
    if (musicState1)
    {
        currentAudioOutput -= musicVolume1;
        musicState1 = 0;
    }
    else
    {
        currentAudioOutput += musicVolume1;
        musicState1 = 1;
    }
    DAC12_0DAT = currentAudioOutput;
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_A0 (void)
{
    if (musicState2)
    {
        currentAudioOutput -= musicVolume2;
        musicState2 = 0;
    }
    else
    {
        currentAudioOutput += musicVolume2;
        musicState2 = 1;
    }
    DAC12_0DAT = currentAudioOutput;
}

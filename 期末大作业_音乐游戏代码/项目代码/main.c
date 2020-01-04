#include <msp430f6638.h>
#include <stdint.h>
#include "dr_tft.h"
#include "fw_public.h"
#include <stdio.h>
//后面的是自己定义的头文件
#include "IO.h"
#include "config.h"
#include "SongData.h"
#include "init.h"
#include "picture.h"

uint16_t noteDataLen, musicDataLen1, musicDataLen2;
uint16_t noteIndex, musicIndex1, musicIndex2;

int frame = 0;                              //计数器变量,用来记录当前的帧数

uint16_t track[4][10] = {0};                //记录屏幕上有哪些方块正在下落,队列结构,先进先出,由于空间有限,十个位置是循环使用的
uint8_t trackHead[4] = {0};                 //记录队列头和尾的位置,在一帧的判定中只看队列头对应的方块是miss、good、perfect还是在判定区外,
uint8_t trackTail[4] = {0};                 //这个队列尾记录的其实是队列中最后一个元素的下一个位置,也就是新来的方块应该放在的位置

uint16_t currentAudioOutput = 0;            //用来当前音频的输出（是绝对值,不是幅值,如果不变就不会产生波形,只会产生一个恒定的信号）
uint8_t musicState1 = 0;                    //用来记录轨道1目前是处在方波的高电平还是低电平
uint8_t musicState2 = 0;

uint8_t keyState[5] = {0};                  //四个按键的状态,

uint16_t x1, x2, y1, y2;                    //AreaSet的坐标
uint16_t frameDelta;

const uint16_t perfectpoint=3;        //perfect计3分
const uint16_t goodpoint=1;      //good计1分


float point=0;
int k=1;                        //歌曲选择变量
int i=0;                        //通用的循环变量,这里没有定义为uint16_t是有原因的






/*
void choose_song(void)  //选歌函数，查询按键改变方块位置和k的值
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

/*void squaremove(int k)                //歌曲选择提示方块
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

    void tftpoint(float slope)  //浮点型数，存储的字符数组
    {
        int temp,i,j;
        unsigned char buffer[7];
        temp = (int)slope;//取整数部分
        for(i=0;temp!=0;i++)//计算整数部分的位数
        temp /=10;
        temp =(int)slope;
        for(j=i;j>0;j--)//将整数部分转换成字符串型
        {
            buffer[j-1] = temp%10+'0';
            temp /=10;
        }
        buffer[i] = '.';
        slope -=(int)slope;
        for(i=i+1;i<6;i++)//将小数部分转换成字符串型
        {
            slope*=10;
            buffer[i]=(int)slope+'0';
            slope-=(int)slope;
        }
        buffer[5] = '%';
        buffer[6] = '\0';
        etft_DisplayString(buffer,130,120,65535,0);
    }



void jiesuan()         //结算界面
{
	etft_AreaSet(0, 0, 319, 239, 0);
    uint16_t length;
    uint16_t x;
    uint16_t y;
    float per=0;
    per=point/Point;
    per*=100;    //转化为百分数
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








void CapTouch_ReadChannel(void)             //这个函数读出四个pad的状态并记录到keyState数组中 pad2对应keystate[3],pad3对应keystate[2],pad4对应keystate[1],pad5对应keystate[0]
{
  uint16_t cpu_cnt = 0;
  uint16_t osc_cnt = 0;

  for (i = 4; i > 0; i--)
  {
      cpu_cnt = 0;
      osc_cnt = 0;
      CBCTL0 = CBIMEN + (i << 8); //外部信号加到负极
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

void assessing(int assessType)                                          //判定不是miss则蜂鸣器鸣叫，传入参数为0代表perfect，为1代表good
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
    etft_AreaSet(0, 0, 319, 239, 0); //清屏
    P1DIR |= BIT5;                                                      //蜂鸣器端口方向设为输出
    P4REN |= BIT1+BIT2+BIT3;
    P4OUT |= BIT1+BIT2+BIT3;


    TA2CCTL0 = CCIE;                                                    //中断使能
    TA2CCR0 = 8738;                                                     //32768 * 8 / 30 = 8738,30fps
    TA2CTL = TASSEL_1 + MC_1 + TACLR;                                   //选择ACLK,计数方式,清计数器
    TA1CCTL0 = TA0CCTL0 = CCIE;
    TA1CCR0 = TA0CCR0 = 65535;                                          //现在还没开始播放声音，所以计数终值很大,会产生极低频，听不到的信号
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

        while(P4IN&BIT2)          //确认键没被按下一直为1
           {
        //       choose_song();
       //        squaremove(k);
           }
        etft_AreaSet(0, 0, 319, 239, 0); //清屏

     /*   for(i=0;i<500;++i)    //根据菜单选择的歌曲进行数组赋值
        {
          noteData[i]=notedata[k][i];
          musicData1[i]=musicdata1[k][i];
          musicData2[i]=musicdata2[k][i];
          musicFrame1[i]=musicdata1[k][i];
          musicFrame2[i]=musicdata2[k][i];
        }

*/
            noteDataLen = sizeof(noteData) / sizeof(noteData[0]);               //求出这些数组的长度,程序中需要这些长度来防止数组越界（当Index大于数组长度时就不再更新判定和播放状态）
            musicDataLen1 = sizeof(musicData1) / sizeof(musicData1[1]);
            musicDataLen2 = sizeof(musicData2) / sizeof(musicData2[1]);
            noteIndex = musicIndex1 = musicIndex2 = 0;                          //对将要使用的数组索引（相当于工作指针）进行初始化

            for(i = 0; i < 5; ++i)
                *LED_GPIO[i] -> PxDIR |= LED_PORT[i]; //设置各LED灯所在端口为输出方向
            __bis_SR_register(GIE);
            return 0;
}

#pragma vector = TIMER2_A0_VECTOR
__interrupt void Timer2_A0 (void)
{
    __bis_SR_register(GIE);                                             //这一句用来允许中断嵌套
    uint16_t tempData;                                                  //这段负责把noteData中应该开始下落的方块放进track队列
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

    for (i = 0; i < 4; i++)                                             //这一段刷新队列中的方块在TFT上的显示(不进行判定）
    {
        uint8_t tempIndex;
        tempIndex = trackHead[i];

        while (tempIndex != trackTail[i])
        {
            frameDelta = frame - track[i][tempIndex] & 0x3FFF;

            x1 = 80 * i + 20;               //直接硬编码进去了
            y1 = (frameDelta - 1) * (noteEnd - noteStart) / noteFallingFrame + noteStart;
            x2 = x1 + 40;
            y2 = y1 + noteHeight;

            if (frameDelta) etft_AreaSet(x1, y1, x2, y2, 0); //先擦掉上一帧显示的矩形

            if (frameDelta <= noteFallingFrame + missWindow)    //方块最多下落perfectframe+misswindow的帧数，超过这一帧数的话自己就会消失（没有及时点击）
            {
                y1 = frameDelta * (noteEnd - noteStart) / noteFallingFrame + noteStart;
                y2 = y1 + noteHeight;
                etft_AreaSet(x1, y1, x2, y2, 65535); //再显示新的矩形
            }
            else
            {
                assessing(2);                        //没有及时点击，miss了
                trackHead[i] ++;
                trackHead[i] %= 10;
            }

            tempIndex ++;
            tempIndex %= 10;
        }
    }

    if ((frame >= musicFrame1[musicIndex1]) && (musicIndex1 < musicDataLen1))       //这段修改方波定时器的频率，也就是负责改变发出声音的频率
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

    P1OUT &= ~BIT5;             //关蜂鸣器
    CapTouch_ReadChannel();     //读取四个pad的状态
    for (i = 3; i >= 0; i--)    //这段进行判定
    {
        if (keyState[i])
        {
            *LED_GPIO[i] -> PxOUT |= LED_PORT[i];       //如果这一帧pad被按下,点亮对应的led灯
            if (trackHead[3 - i] != trackTail[3 - i])
            {
                int tempAssessData, perfectDelta;
                tempAssessData = (track[3 - i][trackHead[3 - i]] & 0x3FFF);
                frameDelta = frame - tempAssessData;
                perfectDelta = frameDelta - noteFallingFrame;
                if (perfectDelta < 0) perfectDelta = -perfectDelta;
                if (perfectDelta < assessWindow)
                {
                    if (perfectDelta < goodWindow) {assessing(0);}        //这个音符是perfect
                    else
                    {
                        if (perfectDelta < missWindow) {assessing(1);}    //这个音符是good
                        else {assessing(2);}                      //这个音符miss了
                    }
                    x1 = 80 * (3 - i) + 20;
                    y1 = frameDelta * (noteEnd - noteStart) / noteFallingFrame + noteStart;
                    x2 = x1 + 40;
                    y2 = y1 + noteHeight;
                    etft_AreaSet(x1, y1, x2, y2, 0);             //已经判定过，擦除
                    trackHead[3 - i] ++;
                    trackHead[3 - i] %= 10;
                }
            }
        }
        else *LED_GPIO[i] -> PxOUT &= ~LED_PORT[i];             //如果这一帧pad没被按下,熄灭对应的led灯
    }
    etft_AreaSet(10, noteEnd, 309,  noteEnd, 65535);            //画判定线
    frame ++;

}

#pragma vector = TIMER0_A0_VECTOR                   //产生方波信号  A0、A1的优先级比A2高,需要将优先级最高的中断分配给方波信号的输出,否则声音会很难听
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

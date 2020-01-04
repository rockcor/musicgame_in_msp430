/*
 * config.h
 *
 *  Created on: 2019年6月30日
 *      Author: Tsong X1
 */

#ifndef CONFIG_H_
#define CONFIG_H_

const uint8_t noteStart = 10;               //方块下落的起始高度
const uint8_t noteEnd = 160;                //判定线位置
const uint8_t noteFallingFrame = 30;        //对应fps为30
const uint8_t noteHeight = 1;               //音符矩形的宽度
const uint8_t missWindow = 3;
const uint8_t goodWindow = 2;               //8帧以内的都是perfect了
const uint8_t assessWindow = 4;

const uint8_t captouch_max = 20;         //按键检测阈值
const uint8_t OSC_CYCLES = 5;              //这个参数可以简单理解为测定信号的时间，时间越长，精度越高，修改这个，上面的阈值也需要跟着进行调整

const uint16_t musicVolume1 = 1000;         //轨道1产生方波的幅值
const uint16_t musicVolume2 = 1000;

const uint32_t fullPoint = 100;

#endif /* CONFIG_H_ */

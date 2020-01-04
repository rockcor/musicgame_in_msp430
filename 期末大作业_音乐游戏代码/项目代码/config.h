/*
 * config.h
 *
 *  Created on: 2019��6��30��
 *      Author: Tsong X1
 */

#ifndef CONFIG_H_
#define CONFIG_H_

const uint8_t noteStart = 10;               //�����������ʼ�߶�
const uint8_t noteEnd = 160;                //�ж���λ��
const uint8_t noteFallingFrame = 30;        //��ӦfpsΪ30
const uint8_t noteHeight = 1;               //�������εĿ��
const uint8_t missWindow = 3;
const uint8_t goodWindow = 2;               //8֡���ڵĶ���perfect��
const uint8_t assessWindow = 4;

const uint8_t captouch_max = 20;         //���������ֵ
const uint8_t OSC_CYCLES = 5;              //����������Լ����Ϊ�ⶨ�źŵ�ʱ�䣬ʱ��Խ��������Խ�ߣ��޸�������������ֵҲ��Ҫ���Ž��е���

const uint16_t musicVolume1 = 1000;         //���1���������ķ�ֵ
const uint16_t musicVolume2 = 1000;

const uint32_t fullPoint = 100;

#endif /* CONFIG_H_ */

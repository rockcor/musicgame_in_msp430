#ifndef __FW_PUBLIC_H_
#define __FW_PUBLIC_H_

//�����ٽ�״̬
//����ǰ��GIE����temp���������ر��ж�
#define _ECRIT(temp) \
{ \
	temp = __get_SR_register() & GIE; \
	__disable_interrupt(); \
}

//�뿪�ٽ�״̬
//��temp�лָ�֮ǰ��GIE
#define _LCRIT(temp) \
{ \
	__bis_SR_register(temp); \
}

#endif

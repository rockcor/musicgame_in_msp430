#ifndef __FW_PUBLIC_H_
#define __FW_PUBLIC_H_

//进入临界状态
//将当前的GIE存入temp变量，并关闭中断
#define _ECRIT(temp) \
{ \
	temp = __get_SR_register() & GIE; \
	__disable_interrupt(); \
}

//离开临界状态
//从temp中恢复之前的GIE
#define _LCRIT(temp) \
{ \
	__bis_SR_register(temp); \
}

#endif

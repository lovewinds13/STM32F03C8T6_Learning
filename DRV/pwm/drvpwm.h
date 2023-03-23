#ifndef __DRVPWM_H__
#define __DRVPWM_H__

#include "stm32f10x.h"

void pwm_init_timer1(uint16_t tim_psc, uint16_t tim_arr);
void pwm_set_duty(uint16_t duty_val1, uint16_t duty_val2);

#endif	//__DRVPWM_H__

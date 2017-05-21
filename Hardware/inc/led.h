#ifndef __LED_H
#define __LED_H

#define LED1ON GPIO_SetBits(GPIOB,GPIO_Pin_12)
#define LED2ON GPIO_SetBits(GPIOB,GPIO_Pin_13)
#define LED3ON GPIO_SetBits(GPIOB,GPIO_Pin_14)
#define LED1OFF GPIO_ResetBits(GPIOB,GPIO_Pin_12)
#define LED2OFF GPIO_ResetBits(GPIOB,GPIO_Pin_13)
#define LED3OFF GPIO_ResetBits(GPIOB,GPIO_Pin_14)

void LED_GPIO_Configuration(void);

#endif

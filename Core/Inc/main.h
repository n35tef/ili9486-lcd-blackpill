#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32f4xx_hal.h"

    void Error_Handler(void);
    void core_init(void);

#define USER_LED_Pin GPIO_PIN_13
#define USER_LED_GPIO_Port GPIOC
#define USER_KEY_Pin GPIO_PIN_0
#define USER_KEY_GPIO_Port GPIOB
#define SPI2_CS_Pin GPIO_PIN_3
#define SPI2_CS_GPIO_Port GPIOB
#define LCD_DATA_CTL_Pin GPIO_PIN_12
#define LCD_DATA_CTL_GPIO_Port GPIOB
#define LCD_RST_Pin GPIO_PIN_5
#define LCD_RST_GPIO_Port GPIOB
#define INIT_TRIGGER_Pin GPIO_PIN_0
#define INIT_TRIGGER_GPIO_Port GPIOB

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

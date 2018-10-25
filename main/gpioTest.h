/*Exemple from https://github.com/espressif/esp-idf/blob/822de0d038fd273a731ce9ce76addadfc16c1761/examples/peripherals/gpio/main/gpio_example_main.c*/

#ifndef GPIOTEST_H
#define GPIOTEST_H

#define GPIO_OUTPUT_IO_0    18
#define GPIO_OUTPUT_IO_1    19
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_OUTPUT_IO_0) | (1ULL<<GPIO_OUTPUT_IO_1))
#define GPIO_INPUT_IO_0     4
#define GPIO_INPUT_IO_1     5
#define GPIO_INPUT_PIN_SEL  ((1ULL<<GPIO_INPUT_IO_0) | (1ULL<<GPIO_INPUT_IO_1))
#define ESP_INTR_FLAG_DEFAULT 0



void GPIO_vidInit();
void GPIO_vidToggleLed();

#endif /*GPIOTEST_H*/

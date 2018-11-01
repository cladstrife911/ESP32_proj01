/*
 * gpioTest.c
 *
 *  Created on: 25 oct. 2018
 *      Author: toine
 */

#include "gpioTest.h"
#include "driver/gpio.h"
#include "esp_log.h"

/************ LOCAL VARIABLES ***************/
static int cnt = 0;

static const char *LOC_sTag = "GPIO";

/************ GLOBAL FUNCTIONS ***************/
void GPIO_vidInit()
{
	gpio_config_t io_conf;
	//disable interrupt
	io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
	//set as output mode
	io_conf.mode = GPIO_MODE_OUTPUT;
	//bit mask of the pins that you want to set,e.g.GPIO18/19
	io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
	//disable pull-down mode
	io_conf.pull_down_en = 0;
	//disable pull-up mode
	io_conf.pull_up_en = 0;
	//configure GPIO with the given settings
	gpio_config(&io_conf);

}


void GPIO_vidToggleLed()
{
	cnt++;
	cnt%=2;
	gpio_set_level(GPIO_OUTPUT_IO_1, cnt);
    ESP_LOGI(LOC_sTag, "Toggle %d!", cnt);
}


/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
//#include "esp_spi_flash.h"

#include "esp_bt.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "gatt_server.h"

#include "gpioTest.h"

/********** LOCAL FUNCTION DEFINITION **************/
static void ble_init();
static void ble_write_callback(uint8_t* buffer, uint16_t buffLen);

/****************** LOCAL VAR *********************/
static const char *LOC_sTag = "MAIN_APP";

uint8_t mac_addr[8] = { 0xC0, 0xFF, 0xEE, 0xC0, 0xFF, 0xEE };

/****************** LOCAL FUNCTIONS *********************/
void app_main()
{
    printf("[INFO] app_main start!\n");

    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is ESP32 chip with %d CPU cores, WiFi%s%s, ",
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d \n", chip_info.revision);

    /*Start BLE*/
    ble_init();

    /*Init GPIO*/
    GPIO_vidInit();

    while(1)
    {
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		//GPIO_vidToggleLed();
    }


    fflush(stdout);
    esp_restart();
}


static void ble_init()
{
	esp_err_t ret;
	esp_bt_controller_config_t bt_cfg =	BT_CONTROLLER_INIT_CONFIG_DEFAULT();

	/* Initialize NVS — it is used to store PHY calibration data */
	ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
	        ESP_ERROR_CHECK(nvs_flash_erase());
	        ret = nvs_flash_init();
	}

	/*disable bluetooth classic mode*/
	ret = esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);
	if (ret) {
	        ESP_LOGI(LOC_sTag, "Bluetooth controller release classic bt memory failed: %s", esp_err_to_name(ret));
	}else
	{
		ESP_LOGI(LOC_sTag, "esp_bt_controller_mem_release OK");
		esp_base_mac_addr_set(mac_addr);
		 if ((ret = esp_bt_controller_init(&bt_cfg)) != ESP_OK) {
		        ESP_LOGI(LOC_sTag, "Bluetooth controller initialize failed: %s", esp_err_to_name(ret));
		}
		 else
		 {
			 ESP_LOGI(LOC_sTag, "esp_bt_controller_init OK");

			 if ((ret = esp_bt_controller_enable(ESP_BT_MODE_BLE)) != ESP_OK) {
			        ESP_LOGI(LOC_sTag, "Bluetooth controller enable failed: %s", esp_err_to_name(ret));
			 }
			 else
			 {
				 /*Start the GATT server using Bluedroid stack*/
				 gatt_server_Init();

                 /*Register the callback to be notifyed when a BLE message is received*/
				 gatt_server_registerCbk(&ble_write_callback);

				 /*
				 ESP_LOGI(LOC_sTag, "esp_bt_controller_enable OK");
				 ESP_LOGI(LOC_sTag, "Bluetooth controller ready, Start bleAdbTask");
				 if(xTaskCreate(&bleAdvtTask, "bleAdvtTask", 2048, NULL, 5, NULL) != pdPASS)
				 {
					 ESP_LOGI(LOC_sTag, "Fail to create bleAdbTask");
				 }
				 */
			 }

		 }
	}

}

static void ble_write_callback(uint8_t* buffer, uint16_t buffLen)
{
	ESP_LOGI(LOC_sTag, "ble_write_callback");
	if(buffLen>0)
	{
		GPIO_vidToggleLed();
		esp_log_buffer_hex(LOC_sTag, buffer, buffLen);
	}
}



/******************** EOF ********************/

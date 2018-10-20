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

#include "hci_user.h"

/********** LOCAL FUNCTION DEFINITION **************/
void ble_init();
void bleAdvtTask(void *pvParameters);
static void controller_rcv_pkt_ready(void);
static int host_rcv_pkt(uint8_t *data, uint16_t len);

/****************** LOCAL VAR *********************/
static const char *LOC_sTag = "BLE_ADV";
static const char *LOC_sTagTask = "BLE_TASK";
static const char *LOC_sTagMain = "MAIN_TASK";
static const char *LOC_sTagHostRX = "HOST_RX";

uint8_t mac_addr[8] = { 0xC0, 0xFF, 0xEE, 0xC0, 0xFF, 0xEE };

static uint8_t u8ControllerReady=0;

static esp_vhci_host_callback_t vhci_host_cb = {
    controller_rcv_pkt_ready,
    host_rcv_pkt
};

/****************** LOCAL FUNCTIONS *********************/
void app_main()
{
	int i=0;

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

    while(1)
    {
		vTaskDelay(1000 / portTICK_PERIOD_MS);
    }


    fflush(stdout);
    esp_restart();
}


void ble_init()
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
				 ESP_LOGI(LOC_sTag, "esp_bt_controller_enable OK");
				 ESP_LOGI(LOC_sTag, "Bluetooth controller ready, Start bleAdbTask");
				 if(xTaskCreate(&bleAdvtTask, "bleAdvtTask", 2048, NULL, 5, NULL) != pdPASS)
				 {
					 ESP_LOGI(LOC_sTag, "Fail to create bleAdbTask");
				 }
			 }
		 }
	}

}

void bleAdvtTask(void *pvParameters)
{
    int cmd_cnt = 0;
    bool send_avail = false;
    esp_vhci_host_register_callback(&vhci_host_cb);
    ESP_LOGI(LOC_sTagTask, "BLE adv task start\n");
    while (1) {
//		ESP_LOGI(LOC_sTagTask, "bleAdvtTask tick\n");
		vTaskDelay(100 / portTICK_PERIOD_MS);
        send_avail = esp_vhci_host_check_send_available();
        if (send_avail) {
            switch (cmd_cnt) {
				case 0: hci_cmd_send_reset(); ++cmd_cnt; break;
				case 1: hci_cmd_send_ble_set_adv_param(); ++cmd_cnt; break;
				case 2: hci_cmd_send_ble_set_adv_data(); ++cmd_cnt; break;
				case 3: hci_cmd_send_ble_adv_start(); ++cmd_cnt; break;
				default:
            	break;
            }
        }
    }

    ESP_LOGI(LOC_sTagTask, "BLE adv task End\n");
}


/*
 * @brief: BT controller callback function, used to notify the upper layer that
 *         controller is ready to receive command
 */
static void controller_rcv_pkt_ready(void)
{
	u8ControllerReady=1;
}

/*
 * @brief: BT controller callback function, to transfer data packet to upper
 *         controller is ready to receive command
 */
static int host_rcv_pkt(uint8_t *data, uint16_t len)
{

    ESP_LOGI(LOC_sTagHostRX, "");
    for (uint16_t i = 0; i < len; i++) {
        printf("%02x", data[i]);
    }
    printf("\n");
    return 0;
}




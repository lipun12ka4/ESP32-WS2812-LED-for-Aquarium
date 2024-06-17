/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
 
 #include "Common_Header.h"

TaskHandle_t uart0_comms_task;
TaskHandle_t data_rx_state_machine;

enum data_rx_states data_rx = IDLE;



static void data_rx_task()
{

	while (1) {

		switch (data_rx)
		{
			case GO_TO_IDLE_STATE:
				data_rx = IDLE;
				printf("Tool is in Idle State\n");
				break;
				
			case FILL_LED_STRIP_COLOR:
				//extract the Desired Pixel Colors from the command
				set_led_strip_clour_fill(red_amount, green_amount, blue_amount);
				
				printf("LED Strip Color Set\n");
				data_rx = IDLE;
				break;
			
			
			default:
				break;
			
		}
		
		vTaskDelay(50 / portTICK_PERIOD_MS); //give time to other tasks
	}//end of while loop
}//end of state machine



void say_hello(){
	printf("Hello world!\n");

    /* Print chip information */
    esp_chip_info_t chip_info;
    uint32_t flash_size;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), %s%s%s%s, ",
           CONFIG_IDF_TARGET,
           chip_info.cores,
           (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi/" : "",
           (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "BLE" : "",
           (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : "");

    unsigned major_rev = chip_info.revision / 100;
    unsigned minor_rev = chip_info.revision % 100;
    printf("silicon revision v%d.%d, ", major_rev, minor_rev);
    if(esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
        printf("Get flash size failed");
        return;
    }

    printf("%" PRIu32 "MB %s flash\n", flash_size / (uint32_t)(1024 * 1024),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());
	
}

void app_main(void)
{
    
	test_led_strip();
	vTaskDelay(100/portTICK_PERIOD_MS);
	//fill_led_strip();
    //vTaskDelay(100/portTICK_PERIOD_MS);
    
    Uart0_Init();			//Uart0 Initialization for debugging purpose.
	xTaskCreate(uart0_event_task, "uart0_event_task", 8192, NULL, 5, &uart0_comms_task);
    
    
    xTaskCreate(data_rx_task, "data_rx_task", 16000, NULL, 20, &data_rx_state_machine);
	ESP_LOGI("RTOS", "data_rx_task Created");
	
	
}

/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
 
 #include "Common_Header.h"
#include "esp_log.h"

TaskHandle_t uart0_comms_task;
TaskHandle_t data_rx_state_machine;
TaskHandle_t http_server_task;

enum data_rx_states data_rx = IDLE;

char firmware_version[100] = "ESP32-Aquarium-LED-Version-01.04";

nvs_handle_t my_handle;


//Function is used to read saved LED parameters from NVS
esp_err_t rd_led_params() {
	
	esp_err_t err;

	err = nvs_open("storage", NVS_READONLY, &my_handle);
	if (err != ESP_OK) {
		printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
		return ESP_FAIL;
	} else {
		printf("Opened NVS\n");
		
		err = nvs_get_u8(my_handle, "red_amount",&red_amount);
		err = nvs_get_u8(my_handle, "green_amount",&green_amount);
		err = nvs_get_u8(my_handle, "blue_amount",&blue_amount);
		
		switch (err) {
		case ESP_OK:
			printf("Done\n");
			printf("Obtained Red_amount from NVS: %u\n", red_amount);
			printf("Obtained Green_amount from NVS: %u\n", green_amount);
			printf("Obtained Blue_amount from NVS: %u\n", blue_amount);
			break;
		case ESP_ERR_NVS_NOT_FOUND:
			printf("The value is not initialized yet!\n");
			break;
		default:
			printf("Error (%s) reading!\n", esp_err_to_name(err));
		}
		nvs_close(my_handle);
	}
	
	if(err != ESP_OK){
		return ESP_FAIL;
	}

	return ESP_OK;
}

//Function is used to write/save LED parameters from NVS
esp_err_t wr_led_params() {
	
	esp_err_t err;
	
	err = nvs_open("storage", NVS_READWRITE, &my_handle);
	
	if (err != ESP_OK) {
		printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
	} else {

		//strcpy(serial_no, es_data); //11 characters

		//red_amount, green_amount, blue_amount
		
		nvs_set_u8(my_handle, "red_amount", red_amount);
		nvs_set_u8(my_handle, "green_amount", green_amount);
		nvs_set_u8(my_handle, "blue_amount", blue_amount);
		
		err = nvs_commit(my_handle);
        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");

		//size_t buf_len = sizeof(serial_no)/sizeof(serial_no[0]);
		red_amount = 0;
		green_amount = 0;
		blue_amount = 0;

		err = nvs_get_u8(my_handle, "red_amount",&red_amount);
		err = nvs_get_u8(my_handle, "green_amount",&green_amount);
		err = nvs_get_u8(my_handle, "blue_amount",&blue_amount);

		switch (err) {
		case ESP_OK:
			printf("Done\n");
			printf("Obtained Red_amount from NVS: %u\n", red_amount);
			printf("Obtained Green_amount from NVS: %u\n", green_amount);
			printf("Obtained Blue_amount from NVS: %u\n", blue_amount);
			break;
		case ESP_ERR_NVS_NOT_FOUND:
			printf("The value is not initialized yet!\n");
			break;
		default:
			printf("Error (%s) reading!\n", esp_err_to_name(err));
		}
		nvs_close(my_handle);
	}

	
	update_status( "WR_SUCCESS");
	fflush(stdout);
	
	if(err != ESP_OK){
		return ESP_FAIL;
	}
	

	return ESP_OK;
}





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
				
			case OTA_UPDATE:
				OTA_Update_app_main();			
				data_rx = IDLE;
				break;
				
			case SAVE_LED_COLORS_TO_NVS:
				wr_led_params();
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
    
    printf("Running Firmware: %s\n", firmware_version);
    
    
    //Initialize NVS    
    ESP_ERROR_CHECK(nvs_flash_init());
    
	//test_led_strip();
	//vTaskDelay(100/portTICK_PERIOD_MS);
	//fill_led_strip();
    //vTaskDelay(100/portTICK_PERIOD_MS);
    
    Uart0_Init();			//Uart0 Initialization for debugging purpose.
	xTaskCreate(uart0_event_task, "uart0_event_task", 8192, NULL, 5, &uart0_comms_task);
    
    
    xTaskCreate(data_rx_task, "data_rx_task", 16000, NULL, 20, &data_rx_state_machine);
	ESP_LOGI("RTOS", "data_rx_task Created");
	
		
	//apply previously set LED Colors
	if(rd_led_params() == ESP_OK){
		set_led_strip_clour_fill(red_amount, green_amount, blue_amount);				
		printf("LED Strip Color Set to Previously Configured Colors\n");		
	}
	
	
	xTaskCreate(http_server_app_main, "http_server_task", 64000, NULL, 15, &http_server_task);
	ESP_LOGI("RTOS", "http_server_task Created");
	
	
}

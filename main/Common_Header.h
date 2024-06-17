#ifndef _COMMON_HEADER_H_
#define _COMMON_HEADER_H_

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"

//Include other common headers
#include <stdint.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
//#include "tinyusb.h"
//#include "tusb_cdc_acm.h"
#include "sdkconfig.h"
#include "esp_event.h"
#include "driver/uart.h" //include UART Drivers
#include "dirent.h"
//#include "esp_littlefs.h"
#include "esp_idf_version.h"
#include "esp_flash.h"
#include "sys/stat.h"
#include "driver/gpio.h"
#include "cJSON.h"
#include <sys/unistd.h>
//#include "hexString.h"
#include "esp_err.h"
#include "esp_system.h"			//This inclusion configures the peripherals in the ESP system.It as system initialization
#include "nvs_flash.h"   		//included header file for NonVolatile storage library
#include "nvs.h"     			//included header file for NonVolatile storage library
#include "esp_task_wdt.h"


#include "mbedtls/ripemd160.h"
#include "mbedtls/des.h"


#include "nvs.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "esp_timer.h"

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"

/* \copyright 2023 Zorxx Software. All rights reserved.
 * \license This file is released under the MIT License. See the LICENSE file for details.
 * \brief ESP32 Neopixel Driver Library Example Application
 */
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_log.h"
#include "driver/gpio.h"
#include "neopixel.h"


//Function to print Hello
void say_hello(void);

//Headers for the WS2812B NeoPixel LED Library
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_log.h"
#include "driver/gpio.h"
#include "neopixel.h"

//ALL LED Blinking functions here
void test_neopixel_led(void);
void test_set_color_based_on_status();

//Enums for different LED Colors
enum led_colors{
	Red,
	Green,
	Blue,
	Off,
};

extern enum led_colors led_color;

//function to set color of the RGB LED
void set_led_color(enum led_colors led_color);
void test_set_led_color();


//UART Stuffs
#include "driver/gpio.h"
#include "driver/uart.h"
#include "string.h"


extern bool BT_AT_COMMAND_REPLY_MODE;
extern TaskHandle_t uart0_comms_task;
extern TaskHandle_t data_rx_state_machine;

extern FILE *firmware_file;
extern FILE *log_file;


extern int64_t data_rx_TimeStamp;



extern char serial_no[13];


//State Machine Enums
enum data_rx_states{
	IDLE,
	GO_TO_IDLE_STATE,
	FILL_LED_STRIP_COLOR,
};

extern enum data_rx_states data_rx; //data rx state

extern bool UART0_DEBUG_MODE_ACTIVE;

void update_status(char message[]);

esp_err_t parse_commands(char *cmd_rx);

extern uint32_t file_size;

//UART 0
extern bool FILE_TRANSFER_MODE;
extern bool UART_COMMAND_DEBUG_ACTIVE;

#define ENABLE_UART0_RECEIVE_DEBUG
void Uart0_Init(void);
void uart0_event_task(void *pvParameters);

//RTOS Tasks







//Command Response Communication Stuffs
extern char es_data[1000];


//LED Strip Functions
void fill_led_strip();
void test_led_strip(void);

extern uint16_t red_amount, green_amount, blue_amount;
void set_led_strip_clour_fill(uint8_t red, uint8_t green, uint8_t blue);


#endif



/*
 * LED_Strip_Functions.c
 *
 *  Created on: Jun 17, 2024
 *      Author: Debashish
 */
#include "Common_Header.h"
#include "neopixel.h"

enum led_colors led_color = Off;

uint8_t red_amount, green_amount, blue_amount = 50;

#define TAG "neopixel_test"
#define PIXEL_COUNT  256
#define NEOPIXEL_PIN GPIO_NUM_23

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

static void test1(uint32_t iterations)
{
   tNeopixelContext neopixel = neopixel_Init(PIXEL_COUNT, NEOPIXEL_PIN);
   tNeopixel pixel[] =
   {
       { 0, NP_RGB(50, 0,  0) }, /* red */
       { 0, NP_RGB(0,  50, 0) }, /* green */
       { 0, NP_RGB(0,  0, 50) }, /* blue */
       { 0, NP_RGB(0,  0,  0) }, /* off */
   };

   ESP_LOGI(TAG, "[%s] Starting", __func__);
   for(int iter = 0; iter < iterations; ++iter)
   {
      for(int i = 0; i < ARRAY_SIZE(pixel); ++i)
      {
         neopixel_SetPixel(neopixel, &pixel[i], 1);
         vTaskDelay(1000/portTICK_PERIOD_MS);
      }
   }
   ESP_LOGI(TAG, "[%s] Finished", __func__);

   neopixel_Deinit(neopixel);
   vTaskDelay(100/portTICK_PERIOD_MS);
}



void test_led_strip(void)
{
   
      test1(1);
   
}

void fill_led_strip(){
	tNeopixelContext neopixel = neopixel_Init(PIXEL_COUNT, NEOPIXEL_PIN);
	uint16_t i = 0;
	tNeopixel pixel[] =
	   {
	   { i, NP_RGB(255, 0,  0) }, /* red */
       { i, NP_RGB(0,  255, 0) }, /* green */
       { i, NP_RGB(0,  0, 255) }, /* blue */
       { i, NP_RGB(0,  0,  0) }, /* off */ 
	   };
	
	for(uint16_t indx = 0; indx <64; indx++){
		pixel[0].index = indx;
		neopixel_SetPixel(neopixel, &pixel[0], 1);
		vTaskDelay(10/portTICK_PERIOD_MS);
	}
	
	
	neopixel_Deinit(neopixel);
   	vTaskDelay(100/portTICK_PERIOD_MS);
}


void set_led_strip_clour_fill(uint8_t red, uint8_t green, uint8_t blue){
	tNeopixelContext neopixel = neopixel_Init(PIXEL_COUNT, NEOPIXEL_PIN);
	uint16_t i = 0;
	tNeopixel pixel[] =
	   {
       { i, NP_RGB(0,  0,  0) }, /* off */ 
	   };
	
	for(uint16_t indx = 0; indx <64; indx++){
		pixel[0].index = indx;
		pixel[0].rgb = NP_RGB(red, green, blue);
		neopixel_SetPixel(neopixel, &pixel[0], 1);
		vTaskDelay(10/portTICK_PERIOD_MS);
	}
	
	
	neopixel_Deinit(neopixel);
   	vTaskDelay(100/portTICK_PERIOD_MS);
}

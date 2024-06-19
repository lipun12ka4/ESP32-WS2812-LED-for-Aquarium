/*
 * UART0_Communication_Functions.c
 *
 *  Created on: Jun 10, 2024
 *      Author: Debashish
 */

#include "Common_Header.h"

bool UART_COMMAND_DEBUG_ACTIVE = 0;

static QueueHandle_t uart0_queue;
bool UART0_DEBUG_MODE_ACTIVE = 0;

static const int UART0_RX_BUF_SIZE = 1024;

//ESP32
#define ECHO_TEST_TXD0  (GPIO_NUM_1)
#define ECHO_TEST_RXD0  (GPIO_NUM_3)


#define ECHO_TEST_RTS  (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS  (UART_PIN_NO_CHANGE)

char UART0_RX_DATA[1500];
uint16_t UART0_RX_Index;
uint16_t UART0_RX_Size;
uint16_t UART0_rx_size;

static uint32_t wr_bytes;

static uint32_t dack_count=0;
static char dack_count_string[50] = "";

#define UART0_DEBUG_DATA_RX_SPEED
static int64_t data_event_TimeStamp;
static uint32_t bytes_written_stamp;
int64_t data_rx_TimeStamp;

static uint32_t bytes_written_to_file;

//Function for initializing the UART0 (Debugging)
void Uart0_Init(void) {
	uart_config_t uart_config = {
			.baud_rate = 115200,
			.data_bits = UART_DATA_8_BITS,
			.parity = UART_PARITY_DISABLE,
			.stop_bits = UART_STOP_BITS_1,
			.flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
			.source_clk = UART_SCLK_DEFAULT,
	};

	uart_param_config(UART_NUM_0, &uart_config);
	uart_set_pin(UART_NUM_0, ECHO_TEST_TXD0, ECHO_TEST_RXD0, ECHO_TEST_RTS,	ECHO_TEST_CTS);
	uart_driver_install(UART_NUM_0, UART0_RX_BUF_SIZE * 2, 0, 20, &uart0_queue, 0);
}





static const char* UART0_TAG = "UART0";
//There is no callback for UART, so have to put it inside an infinite loop
void uart0_event_task(void *pvParameters)
{
	uart_event_t event;
	uint8_t* dtmp = (uint8_t*) malloc(UART0_RX_BUF_SIZE);

	for(;;) {
		//Waiting for UART event.
		if(xQueueReceive(uart0_queue, (void * )&event, (TickType_t)portMAX_DELAY)) {
			//bzero(dtmp, UART0_RX_BUF_SIZE);
			//	ESP_LOGI(TAG, "uart[%d] event:", UART_NUM_1);
			switch(event.type) {
			//Event of UART receiving data
			//We'd better handler data event fast, there would be much more data events than
			//other types of events. If we take too much time on data event, the queue might
			//be full.
			case UART_DATA:
				//heap_trace_start();
				//ESP_ERROR_CHECK( heap_trace_start(HEAP_TRACE_LEAKS) );
				//ESP_LOGI(TAG, "[UART_DATA]: %d", event.size);
				uart_read_bytes(UART_NUM_0, dtmp, event.size, portMAX_DELAY);
				//dtmp[event.size]='\r';		//See if this does not break VCI Tool Stuffs
#ifdef UART0_DEBUG_EX
				ESP_LOGI(UART0_TAG, "[DATA_EVT]: %s",dtmp);
#endif

				UART_COMMAND_DEBUG_ACTIVE = 1;


					//parse_commands(dtmp, event.size);
					for(uint16_t i = 0; i < event.size ; i++){
						if(dtmp[i] == ';'){//is a semicolon
							//printf("BUF[%d] = %d",i, buf[i]);
							//End Reached
							UART0_RX_DATA[UART0_RX_Index + event.size] = '\0';

#ifdef UART0_DEBUG
							printf("Received Total %u bytes of Data \n", UART0_RX_Index);
							printf("The Received Data String is : %s \n", UART0_RX_DATA);
#endif
							//Should call process command function from here
							UART0_DEBUG_MODE_ACTIVE = 1;
							vTaskDelay(10/portTICK_PERIOD_MS);
							parse_commands(UART0_RX_DATA);//one ; and one line feed character
							vTaskDelay(10/portTICK_PERIOD_MS);
							printf("Free-RAM :%lu \n", esp_get_free_heap_size() );

							// Reset the buffer
							memset(UART0_RX_DATA, 0, 1500); //Can we skip this ? ? TODO
							// Reset the Indexes
							UART0_RX_Size = 0;
							UART0_RX_Index = 0;
							//exit from here, so that the next line wont be executed
							break;
						}else{
							//printf("BUF[%d] = %d",i, buf[i]);
							UART0_RX_DATA[UART0_RX_Index] = dtmp[i]; //copy/assign the received bytes into the Larger Buffer
							UART0_RX_Index ++;
#ifdef UART0_SERIAL_DEBUG_EX
							printf("UART0_RX_Index: %u \n", UART0_RX_Index);
#endif
							//printf("CDC_RX_DATA: %s \n", CDC_RX_DATA);
						}
					}
			
				//
				//memset(dtmp, 0, UART0_RX_BUF_SIZE);
				//heap_trace_stop();
				//ESP_ERROR_CHECK( heap_trace_stop() );
				//heap_trace_dump();
				break;
				//Event of HW FIFO overflow detected
			case UART_FIFO_OVF:
				ESP_LOGI(UART0_TAG, "HW FIFO Overflow");
				// If fifo overflow happened, you should consider adding flow control for your application.
				// The ISR has already reset the rx FIFO,
				// As an example, we directly flush the rx buffer here in order to read more data.
				uart_flush_input(UART_NUM_0);
				xQueueReset(uart0_queue);
				break;
				//Event of UART ring buffer full
			case UART_BUFFER_FULL:
				ESP_LOGI(UART0_TAG, "Ring Buffer Full");
				// If buffer full happened, you should consider increasing your buffer size
				// As an example, we directly flush the rx buffer here in order to read more data.
				uart_flush_input(UART_NUM_0);
				xQueueReset(uart0_queue);
				break;
				//Event of UART RX break detected
			case UART_BREAK:
				ESP_LOGI(UART0_TAG, "UART RX Break");
				uart_flush_input(UART_NUM_0);
				xQueueReset(uart0_queue);
				data_rx = IDLE;
				break;
				//Event of UART parity check error
			case UART_PARITY_ERR:
				ESP_LOGI(UART0_TAG, "UART Parity Error");
				break;
				//Event of UART frame error
			case UART_FRAME_ERR:
				ESP_LOGI(UART0_TAG, "UART Frame Error");
				uart_flush_input(UART_NUM_0);
				xQueueReset(uart0_queue);
				break;
				//UART_PATTERN_DET
			case UART_PATTERN_DET:
				break;
				//Others
			default:
				ESP_LOGI(UART0_TAG, "UART Event Type: %d", event.type);
				break;
			}
		}
		//this is the inside for loop

	}
	free(dtmp);
	dtmp = NULL;
	vTaskDelete(NULL);
}


static char UART_Message[1000];
char global_http_response[1000];


void update_status(char message[]){

	if(UART0_DEBUG_MODE_ACTIVE){
		strcpy(UART_Message, message);
		strcat(UART_Message, "\n");
		uart_write_bytes(UART_NUM_0,UART_Message, strlen(UART_Message));
	}
	
	//memset(global_http_response,0, 1000);
	strcpy(global_http_response, message);
	

}


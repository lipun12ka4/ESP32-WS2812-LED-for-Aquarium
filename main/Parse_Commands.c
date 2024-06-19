/*
 * Parse_Commands.c
 *
 *  Created on: Jun 10, 2024
 *      Author: Debashish
 */

#include "Common_Header.h"


char es_data[1000];


esp_err_t parse_commands(char *cmd_rx){

	bool Received_A_Command = 0;
	char *command;
	vTaskDelay(10/portTICK_PERIOD_MS);

	cJSON *root = cJSON_Parse(cmd_rx);
	int blocks;
	cJSON *block = NULL;
	bool JSON_Contains_Square_Braket = false;

	if (cmd_rx[0]=='['){
		JSON_Contains_Square_Braket = true;
		blocks = cJSON_GetArraySize(root);
		printf("No of Blocks in JSON Array are %d \n", blocks);
		block = cJSON_GetArrayItem(root, 0);
		if (cJSON_GetObjectItem(block, "Command") ) {
			printf("Received A Command \n");
			Received_A_Command = 1;
			command = cJSON_GetObjectItem(block, "Command")->valuestring;
		}
	}

	if (cmd_rx[0]=='{'){
			//cJSON *root = cJSON_Parse(cmd_rx);
			if (cJSON_GetObjectItem(root, "Command") ) {
				Received_A_Command = 1;
				printf("Received A Command \n");
				command = cJSON_GetObjectItem(root, "Command")->valuestring;
			}
	}

	if (Received_A_Command == 1 ) {
		//printf("Received Command %s \n", command);
		//char *command = cJSON_GetObjectItem(root, "Command")->valuestring;


		printf("Received Command: %s \n", command);

		//Example Command to Test if Android to Tool Communication is working
		//Receive: Hello, Send: Hi There
		if (!strcmp(command,"Hello"))
		{	
			printf("Received Hello inside parse_commands \n");
			update_status("Hi There;");
		}
		
		else if(!strcmp(command,"IDLE")){
			data_rx = GO_TO_IDLE_STATE;
		}
		
		else if(!strcmp(command,"FILL LED STRIP COLOR")){
			
			char color_amount[5];
			strcpy(color_amount, cJSON_GetObjectItem(root, "Red")->valuestring);
			sscanf(color_amount, "%hhu", &red_amount);

			strcpy(color_amount, cJSON_GetObjectItem(root, "Green")->valuestring);
			sscanf(color_amount, "%hhu", &green_amount);
			
			strcpy(color_amount, cJSON_GetObjectItem(root, "Blue")->valuestring);
			sscanf(color_amount, "%hhu", &blue_amount);
			
			
			data_rx = FILL_LED_STRIP_COLOR;
		}
		
		else if(!strcmp(command,"OTA Update")){
			strcpy(OTA_URL, cJSON_GetObjectItem(root, "URL")->valuestring);			
			data_rx = OTA_UPDATE;
		}
		
		else if(!strcmp(command,"Save LED Colors")){	
			data_rx = SAVE_LED_COLORS_TO_NVS;
		}
		
		

		
		else{
			printf("Unknown/Unsupported Command Received\n");
			update_status("Unknown/Unsupported Command Received");
		}
		

	}
	return ESP_OK;
}

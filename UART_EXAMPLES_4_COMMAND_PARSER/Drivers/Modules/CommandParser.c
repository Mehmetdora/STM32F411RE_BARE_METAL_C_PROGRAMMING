/*
 * CommandParser.c
 *
 *  Created on: 16 Mar 2026
 *      Author: mehmet_dora
 */


#include <stm32f4xx.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>


#include "GPIODriver.h"
#include "CommandParser.h"
#include "UARTDriver.h"
#include "RingBuffer.h"


#define Control_Char_Start '<'
#define Control_Char_End   '>'

typedef void (*function_t)(void);
typedef void (*function_param_t)(uint32_t);

typedef struct {
	const char* command;
	function_t function;
}Command_t;

typedef struct {
	const char* command;
	const uint32_t min;
	const uint32_t max;
	function_param_t function;
}CommandWithParam_t;

Command_t commands[] = {
	{"OFF", GPIODriver_led_off},
	{"ON" , GPIODriver_led_on},
	{"TOGGLE", GPIODriver_led_toggle},
	{NULL,NULL}
};

CommandWithParam_t commands_params[] = {
		{"LED", 1 , 10 , GPIODriver_led_toggle_for_1_sn},
		{NULL,NULL,NULL,NULL}
};

char parsed_data[64];
uint8_t cmd_ready = 0;


void CommandParser_RX_CMD_Complete_callback(RingBuffer_t* rx_rb){

	if(cmd_ready == 1){			// Eğer bu karakter geldiyse bu bir komut sonudur

		memset(parsed_data, 0, sizeof(parsed_data));
		if(rx_rb->buffer[rx_rb->write_index - 1] == Control_Char_End){
			strncpy(parsed_data, &rx_rb->buffer[rx_rb->read_index], (rx_rb->write_index - rx_rb->read_index) - 1);

			for(int i = 0; commands[i].command != NULL; i++){
				if(strcmp(parsed_data,commands[i].command) == 0){
					commands[i].function();
					break;
				}
			}


			if(strchr(parsed_data,':') != NULL){ 		// parsed_data içindeki string : göre parçalanır, okuma pointer ı en başı gösterir
				char* parameter1 = strtok(parsed_data, ":");			// ilk parametre
				char* parameter2 = strtok(NULL, ":");					// ikinci parametre
				uint32_t parameter2_int = atoi(parameter2);

				// Gelen parametreler istenen şekilde fonksiyonların çağrılması için kontrol edilecek
				// Gelen parametrelerin doğruluğu kontrol edilmeli, parametreler tanımlanma aralığında mı ?



				// Eğer parametreler doğru aralıkta tanımlanmışsa ilgili handler çalışsın
				uint8_t is_valid = parameter_control(parameter1, parameter2_int);
				if(is_valid == 1){

					for(int i = 0; commands_params[i].command != NULL; i++){
						if(strcmp(parameter1,commands_params[i].command) == 0){
							commands_params[i].function(parameter2_int);
							break;
						}
					}


				}
			}

			ring_buffer_clear(rx_rb);

		}

		cmd_ready = 0;
	}

}



uint8_t parameter_control(char* param1, int param2){


	// Parametre 1 control
	uint8_t commands_params_len = sizeof(commands_params) / sizeof(CommandWithParam_t);
	for(int i = 0; i < commands_params_len; i++){

		if(strcmp(commands_params[i].command,param1) == 0){		// string karşılaştırma yaparken direkt karşılaştırmak yerine strcmp() kullan

			// Parametre 2 control , max-min kontrolü
			if(commands_params[i].min <= (uint32_t)param2 && commands_params[i].max >= (uint32_t)param2){
				//Tüm parametreler uygun
				return 1;
			}
		}
	}

	return 0;



}









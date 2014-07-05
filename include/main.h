#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

const uint16_t USER_BUTTON = GPIO_Pin_0;

const uint16_t LEDS = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
const uint16_t LED[4] = {GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15};

void init();

typedef struct command_for_ROS{
	char command[8];
	uint8_t index;
}ROS_CMD_T;

ROS_CMD_T ros_cmd;


int get_int_from_char(uint8_t mode)
{
	int tem = 0;
	if(mode == 1){
		tem += (ros_cmd.command[1] - '0') * 100;
		tem += (ros_cmd.command[2] - '0') * 10;
		tem += (ros_cmd.command[3] - '0');
	}else{
		tem += (ros_cmd.command[5] - '0') * 100;
		tem += (ros_cmd.command[6] - '0') * 10;
		tem += (ros_cmd.command[7] - '0');
	}
	return tem;
}

void command_motor()
{
	int val = get_int_from_char(1);
	if(val > 500){
		val = 500;
	}
	if(ros_cmd.command[0] == 'f'){
		TIM4->CCR1 = val;
		TIM4->CCR2 = 0;
	}else if (ros_cmd.command[0] == 'b'){
		TIM4->CCR1 = 0;
		TIM4->CCR2 = val;
	}
	val = get_int_from_char(2);
	if(val > 500){
		val = 500;
	}
	if(ros_cmd.command[4] == 'f'){
		TIM4->CCR3 = val;
		TIM4->CCR4 = 0;
	}else if (ros_cmd.command[4] == 'b'){
		TIM4->CCR3 = 0;
		TIM4->CCR4 = val;
	}
	
}

//int timer_period = (SystemCoreClock / 5000 ) - 1;

//volatile uint16_t flagOfLed = 0;



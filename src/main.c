#include "main.h"

int main(void) {
    init();
	
	while(1){}
}

void init_NVIC_Config()
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
    NVIC_InitStructure.NVIC_IRQChannel  = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
    NVIC_Init(&NVIC_InitStructure);
}

void init_Led_PWM_Pin() 
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    
	GPIO_InitTypeDef GPIO_InitStructure;
	
    GPIO_InitStructure.GPIO_Pin = LEDS;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);
}

void initForUsart(uint32_t baudrate)
{
	GPIO_InitTypeDef GPIO_InitStruct; 
	USART_InitTypeDef USART_InitStruct; 


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);


	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; 			

	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;			
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;			
	GPIO_Init(GPIOB, &GPIO_InitStruct);					


	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1); 
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);


	USART_InitStruct.USART_BaudRate = baudrate;				
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;	
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	
	USART_Init(USART1, &USART_InitStruct);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	USART_Cmd(USART1, ENABLE);
}

void USART1_IRQHandler(void)
{
    char c;
    if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==SET){
        c = USART_ReceiveData(USART1);
		if( c != '$'){
			ros_cmd.command[ros_cmd.index++] = c;
		}else{
			command_motor();
			ros_cmd.index = 0;
			USART_SendData(USART1,'#');
		}
		/*int a = c - '0';
		if(a > -1 && a < 4){
			if(a%2 == 0){
				if(TIM4->CCR1 > 500){
					TIM4->CCR1 = 0;
				}else{
					TIM4->CCR1 += 100;
				}
				
			}else{
				if(TIM4->CCR1 == 0){
					TIM4->CCR1 = 500;
				}else{
					TIM4->CCR1 -= 100;
				}
				
			}
			
		}*/
        //USART_SendData(USART1, ((TIM4->CCR1 / 100 ) + '0'));
    }
}

void init_TIM_Config()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);


    //u32 PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 21000000) - 1;

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    TIM_TimeBaseStructure.TIM_Period = 500 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = 21 - 1 ;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
    TIM_OCInitTypeDef TIM_OCInitStructure;

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OC1Init(TIM4, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

    TIM_OC2Init(TIM4, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

    TIM_Cmd(TIM4, ENABLE);
}

void init()
{
	init_Led_PWM_Pin();
	initForUsart(9600);
	init_NVIC_Config();
	init_TIM_Config();
	ros_cmd.index = 0;
}


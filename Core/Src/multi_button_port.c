#include "multi_button.h"
#include "gpio.h"
#include "stm32f4xx_hal.h"
#include "stdio.h"

struct Button btn0;
struct Button btn1;
struct Button btn2;

static uint8_t read_button0_GPIO()
{
	return HAL_GPIO_ReadPin(KEY_RIGHT_GPIO_Port, KEY_RIGHT_Pin);
}

static uint8_t read_button1_GPIO()
{
	return HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port, KEY_DOWN_Pin);
}

static uint8_t read_button2_GPIO()
{
	return HAL_GPIO_ReadPin(KEY_LEFT_GPIO_Port, KEY_LEFT_Pin);
}


// ----------------------------------------------------------------------------
// button 0
// ----------------------------------------------------------------------------
static void BTN0_PRESS_DOWN_Handler(void* btn)
{
    printf("---> btn0_press_down!\r\n");
}

static void BTN0_PRESS_UP_Handler(void* btn)
{
    printf("---> btn0_press_up!\r\n");
}

static void BTN0_PRESS_REPEAT_Handler(void* btn)
{
    printf("---> btn0_press_repeat!\r\n");
}

static void BTN0_SINGLE_Click_Handler(void* btn)
{
    printf("---> btn0_single_click!\r\n");
}

static void BTN0_DOUBLE_Click_Handler(void* btn)
{
    printf("---> btn0_double_click!\r\n");
}

static void BTN0_LONG_PRESS_START_Handler(void* btn)
{
    printf("---> btn0_long_press_start!\r\n");
}

static void BTN0_LONG_PRESS_HOLD_Handler(void* btn)
{
    printf("---> btn0_long_press_hold!\r\n");
}

// ----------------------------------------------------------------------------
// button 1
// ----------------------------------------------------------------------------
static void BTN1_PRESS_DOWN_Handler(void *btn)
{
    printf("---> btn1_press_down!\r\n");
}

static void BTN1_PRESS_UP_Handler(void *btn)
{
    printf("---> btn1_press_up!\r\n");
}

static void BTN1_PRESS_REPEAT_Handler(void *btn)
{
    printf("---> btn1_press_repeat!\r\n");
}

static void BTN1_SINGLE_Click_Handler(void *btn)
{
    printf("---> btn1_single_click!\r\n");
}

static void BTN1_DOUBLE_Click_Handler(void *btn)
{
    printf("---> btn1_double_click!\r\n");
}

static void BTN1_LONG_PRESS_START_Handler(void *btn)
{
    printf("---> btn1_long_press_start!\r\n");
}

static void BTN1_LONG_PRESS_HOLD_Handler(void *btn)
{
    printf("---> btn1_long_press_hold!\r\n");
}


// ----------------------------------------------------------------------------
// button 2
// ----------------------------------------------------------------------------
static void BTN2_PRESS_DOWN_Handler(void* btn)
{
    printf("---> btn2_press_down!\r\n");
}

static void BTN2_PRESS_UP_Handler(void* btn)
{
    printf("---> btn2_press_up!\r\n");
}

static void BTN2_PRESS_REPEAT_Handler(void* btn)
{
    printf("---> btn2_press_repeat!\r\n");
}

static void BTN2_SINGLE_Click_Handler(void* btn)
{
    printf("---> btn2_single_click!\r\n");
}

static void BTN2_DOUBLE_Click_Handler(void* btn)
{
    printf("---> btn2_double_click!\r\n");
}

static void BTN2_LONG_PRESS_START_Handler(void* btn)
{
    printf("---> btn2_long_press_start!\r\n");
}

static void BTN2_LONG_PRESS_HOLD_Handler(void* btn)
{
    printf("---> btn2_long_press_hold!\r\n");
}




void user_button_init(void)
{
    button_init(&btn0, read_button0_GPIO, GPIO_PIN_RESET);
	button_init(&btn1, read_button1_GPIO, GPIO_PIN_RESET);
	button_init(&btn2, read_button2_GPIO, GPIO_PIN_RESET);

    button_attach(&btn0, PRESS_DOWN,       BTN0_PRESS_DOWN_Handler);
	button_attach(&btn0, PRESS_UP,         BTN0_PRESS_UP_Handler);
	button_attach(&btn0, PRESS_REPEAT,     BTN0_PRESS_REPEAT_Handler);
	button_attach(&btn0, SINGLE_CLICK,     BTN0_SINGLE_Click_Handler);
	button_attach(&btn0, DOUBLE_CLICK,     BTN0_DOUBLE_Click_Handler);
	button_attach(&btn0, LONG_PRESS_START, BTN0_LONG_PRESS_START_Handler);
	button_attach(&btn0, LONG_PRESS_HOLD,  BTN0_LONG_PRESS_HOLD_Handler);

	button_attach(&btn1, PRESS_DOWN,       BTN1_PRESS_DOWN_Handler);
	button_attach(&btn1, PRESS_UP,         BTN1_PRESS_UP_Handler);
	button_attach(&btn1, PRESS_REPEAT,     BTN1_PRESS_REPEAT_Handler);
	button_attach(&btn1, SINGLE_CLICK,     BTN1_SINGLE_Click_Handler);
	button_attach(&btn1, DOUBLE_CLICK,     BTN1_DOUBLE_Click_Handler);
	button_attach(&btn1, LONG_PRESS_START, BTN1_LONG_PRESS_START_Handler);
	button_attach(&btn1, LONG_PRESS_HOLD,  BTN1_LONG_PRESS_HOLD_Handler);

	button_attach(&btn2, PRESS_DOWN,       BTN2_PRESS_DOWN_Handler);
	button_attach(&btn2, PRESS_UP,         BTN2_PRESS_UP_Handler);
	button_attach(&btn2, PRESS_REPEAT,     BTN2_PRESS_REPEAT_Handler);
	button_attach(&btn2, SINGLE_CLICK,     BTN2_SINGLE_Click_Handler);
	button_attach(&btn2, DOUBLE_CLICK,     BTN2_DOUBLE_Click_Handler);
	button_attach(&btn2, LONG_PRESS_START, BTN2_LONG_PRESS_START_Handler);
	button_attach(&btn2, LONG_PRESS_HOLD,  BTN2_LONG_PRESS_HOLD_Handler);

    button_start(&btn0);
    button_start(&btn1);
    button_start(&btn2);
}


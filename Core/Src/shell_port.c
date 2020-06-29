/**
 * @file shell_port.c
 * @author Letter (NevermindZZT@gmail.com)
 * @brief
 * @version 0.1
 * @date 2019-02-22
 *
 * @Copyright (c) 2019 Unicook
 *
 */

#include "shell.h"
#include "stm32f4xx_hal.h"
#include "usart.h"
#include "i2c.h"
#include "stdio.h"
#include "string.h"


Shell shell;
char shellBuffer[512];

/**
 * @brief 用户shell写
 *
 * @param data 数据
 */
void userShellWrite(char data)
{
    HAL_UART_Transmit(&huart1, (uint8_t*)&data, 1, HAL_MAX_DELAY);
}


#if 0
/**
 * @brief 用户shell读
 *
 * @param data 数据
 * @return char 状态
 */
signed char userShellRead(char *data)
{
    if (serialReceive(&debugSerial, (uint8_t *)data, 1, 0) == 1)
    {
        return 0;
    }
    else
    {
        return -1;
    }

}
#endif


/**
 * @brief 用户shell初始化
 *
 */
void userShellInit(void)
{
    shell.write = userShellWrite;
    // shell.read = userShellRead;
    shellInit(&shell, shellBuffer, 512);
}


// both RESET and BOOT0 are connected and influenced by CH340G.
// can't reset through this USART port.
#if 0
static void reset(void)
{
    // HAL_NVIC_DisableIRQ(USART1_IRQn);
    // __disable_irq();
    HAL_NVIC_SystemReset();
}

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), reset, reset, reset MCU);
#endif


// ported from i2c-tools/tools/i2cdetect.c
static void i2cdetect(uint8_t bus_num)
{
	unsigned int i, j;
    uint8_t ret;
    I2C_HandleTypeDef *phi2c;

    switch (bus_num)
    {
    case 0:
    case 1:
        phi2c = &hi2c1;
        break;
#if 0
    case 2:
        phi2c = &hi2c2;
        break;
    case 3:
        phi2c = &hi2c3;
        break;
#endif
    default:
        phi2c = &hi2c1;
        break;
    }

    printf("usage:\n\ti2cdetect [bus_num], start from 1, 7-bit address\n");
    printf("i2c scan bus %d\n\n", bus_num);
    printf("       0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");

    for (i = 0; i < 128; i += 16)
    {
        printf("0x%02X: ", i);
        for (j = 0; j < 16; j++)
        {
            ret = HAL_I2C_IsDeviceReady(phi2c, (i+j)<<1, 3, 1000);

            if (ret == HAL_OK)
                printf("%02X ", i + j);
            else
                printf("-- ");
        }
        printf("\n");
    }
    printf("\n");
}

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), i2cdetect, i2cdetect, ported from i2c-tools/tools/i2cdetect.c);


#define POS_SLAVE_ADDR 0
#define POS_NETFN      1
#define POS_CHECKSUM_1 2


// 8-bit addr
static void ipmi_get_device_id(uint8_t addr)
{
    uint8_t ret;

    static uint8_t ipmi_recv_buf[100];
    static uint8_t ipmi_send_buf[] = {0x00, 0x18, 0x00, 0x20, 0x00, 0x01, 0xDF};

    ipmi_send_buf[POS_SLAVE_ADDR] = addr;
    ipmi_send_buf[POS_CHECKSUM_1] = 0x100 - addr - ipmi_send_buf[POS_NETFN];

    ret = HAL_I2C_Master_Transmit(&hi2c1, addr, &ipmi_send_buf[1], sizeof(ipmi_send_buf) - 1, 1000);
    if (ret == HAL_OK)
    {
        printf("HAL_I2C_Master_Transmit ok.\n");
#if 1
        ret = HAL_I2C_Slave_Receive(&hi2c1, ipmi_recv_buf, sizeof(ipmi_recv_buf), 10000);
        if ((ret == HAL_OK) || (hi2c1.XferSize != sizeof(ipmi_recv_buf)))
        {
            print_buff("HAL_I2C_Slave_Receive", ipmi_recv_buf, sizeof(ipmi_recv_buf) - hi2c1.XferSize);
        }
        else
        {
            printf("HAL_I2C_Slave_Receive error, ret = %d.\n", ret);
        }

        memset(ipmi_recv_buf, 0, sizeof(ipmi_recv_buf));
#endif
    }
    else
    {
        printf("HAL_I2C_Master_Transmit error, ret = %d.\n", ret);
    }
}

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), ipmi_get_device_id, ipmi_get_device_id, ipmi_get_device_id 8-bit addr);




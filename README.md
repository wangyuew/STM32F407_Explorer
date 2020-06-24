# STM32F407_Explorer
正点原子探索者开发板 STM32F407 Explorer

目标：将开发板作为通用工具，用于调试I2C、IPMB、IPMI、SPI、232、485等接口和协议。

开发环境：

* STM32CubeIDE v1.3.1
* STM32F4 Cube 1.25.0

### 前置条件

1. 已移植 [letter-shell 3.0](https://github.com/NevermindZZT/letter-shell)。串口轮询发送，中断接收。
2. 已移植 [MultiButton](https://github.com/0x1abin/MultiButton)。`HAL_SYSTICK_Callback`中以5ms间隔调用`button_ticks`。

### 硬件配置

1. **I2C1**通过PB6、PB7引出。开发板上默认通过PB8、PB9引出，用于连接板上的EEPROM、MPU6050等设备，但只在接线端子P4引出。这样配置的原因，一是避免被板上其它设备影响，二是PB6、PB7在接线端子P8上引出，此时可将示波器、协议分析仪连接至接线端子P4，方便调试。
2. **USART1**为letter-shell的调试串口，对应接线端子USB_232，通过板载CH340转换电平。

### 注意事项

1. STM32CubeMX配置界面中的i2c地址为**7位**，HAL库函数中为**8位**。另外要注意区分十进制、十六进制。
2. STM32CubeMX v5.x 为避免`HAL_SYSTICK_Callback()`执行时间超过1ms或产生循环依赖时出现阻塞，默认不调用`HAL_SYSTICK_IRQHandler()`，并不是bug。。需要在 `SysTick_Handler()` 中的用户段手动增加 `HAL_SYSTICK_IRQHandler()` 。

### 实现功能

1. 将printf重定向至USART1。
2. 通过MultiButton注册按键KEY0-2，并使能所有事件。
3. `i2cdetect`命令。类似于[i2c-tools](https://i2c.wiki.kernel.org/index.php/I2C_Tools)中的i2cdetect命令，用于扫描指定i2c总线上的从设备，打印7位地址。循环调用 `HAL_I2C_IsDeviceReady` 函数实现。
4. `ipmi_get_device_id`命令。对指定的8位从设备地址发送`IPMI Get Device ID`命令，打印返回信息。主设备轮询发送，然后切换为从设备轮询接收。


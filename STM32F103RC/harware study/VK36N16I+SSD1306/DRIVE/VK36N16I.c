#include "VK36N16I.h"
#include "I2C.h"

static void i2c_setting_init_cb(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12; //SDA PC11 SCL PC12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; //开漏输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
}
static void i2c_sda_write_cb(uint8_t level)
{
    if (level == I2C_PIN_HIGH) GPIOC->ODR |= 1<<11; //GPIO_SetBits(GPIOC, GPIO_Pin_11);
    else GPIOC->ODR &= ~(1<<11); //GPIO_ResetBits(GPIOC, GPIO_Pin_11);
	Systick_us(5);
}
static uint8_t i2c_sda_read_cb(void)
{
	Systick_us(5);
    return GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11);
}
static void i2c_scl_write_cb(uint8_t level)
{
    if (level == I2C_PIN_HIGH) GPIOC->ODR |= 1<<12; //GPIO_SetBits(GPIOC, GPIO_Pin_12);
    else GPIOC->ODR &= ~(1<<12); //GPIO_ResetBits(GPIOC, GPIO_Pin_12);
	Systick_us(5);
}
static void i2c_delay_us_cb(uint32_t us)
{
    Systick_us(us);
}
/* 初始化：调用原来的 I2C_GPIO_Init 并注册回调 */
void VK36N16I_init(I2C_T *i2c)
{
    /* 初始化并注册回调（会在内部调用 i2c_setting_init_cb） */
	i2c->I2C_SETTING_INIT = i2c_setting_init_cb;
	i2c->SDA_READ = i2c_sda_read_cb;	
	i2c->SDA_WRITE = i2c_sda_write_cb;
	i2c->SCL_WRITE = i2c_scl_write_cb;
	i2c->DELAY_US = i2c_delay_us_cb;
	
	i2c->I2C_SETTING_INIT();
}
/* 使用新的 I2C 接口重写 Key_Read/Key_return/Key_Scan */
uint16_t Key_Read(I2C_T *i2c)
{
    uint16_t low_data = 0, high_data = 0, return_data;

    I2C_START(i2c);
    I2C_WRITE_BYTE(i2c, 0XCB);
    I2C_ACK(i2c);
    low_data = I2C_READ_BYTE(i2c);
    I2C_ACK(i2c);
    high_data = I2C_READ_BYTE(i2c);
    I2C_STOP(i2c);

    return_data = high_data;
    return_data = return_data << 8;
    return_data |= low_data;
    return return_data;
}

uint16_t Key_return(I2C_T *i2c)
{
    uint16_t data;
    data = Key_Read(i2c);
    return data;
}

uint8_t Key_Scan(I2C_T *i2c)
{
    uint16_t key = 0x00;
    uint8_t return_key = 0x00;
    key = Key_return(i2c);
    switch (key)
    {
        case 0x0001:return_key = 1;break;
        case 0x0002:return_key = 4;break;
        case 0x0004:return_key = 7;break;
        case 0x0008:return_key = 15;break;

        case 0x0010:return_key = 2;break;
        case 0x0020:return_key = 5;break;
        case 0x0040:return_key = 8;break;
        case 0x0080:return_key = 0;break;

        case 0x0100:return_key = 3;break;
        case 0x0200:return_key = 6;break;
        case 0x0400:return_key = 9;break;
        case 0x0800:return_key = 14;break;

        case 0x1000:return_key = 10;break;
        case 0x2000:return_key = 11;break;
        case 0x4000:return_key = 12;break;
        case 0x8000:return_key = 13;break;
        default:return_key = 255;break;
    }
    return return_key;
}










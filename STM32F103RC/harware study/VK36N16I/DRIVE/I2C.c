#include "I2C.h"
/* START: SDA 1->0 while SCL high */
void I2C_START(I2C_T *i2c)
{
    i2c->SCL_WRITE(I2C_PIN_HIGH);
    i2c->SDA_WRITE(I2C_PIN_HIGH);
    i2c->SDA_WRITE(I2C_PIN_LOW);
    i2c->SCL_WRITE(I2C_PIN_LOW);
}

/* STOP: SDA 0->1 while SCL high */
void I2C_STOP(I2C_T *i2c)
{
    i2c->SDA_WRITE(I2C_PIN_LOW);
    i2c->SCL_WRITE(I2C_PIN_HIGH);
    i2c->SDA_WRITE(I2C_PIN_HIGH);
    i2c->SCL_WRITE(I2C_PIN_LOW);
}

/* ACK: 主机拉SDA为0并脉冲SCL（用于发送ACK） */
void I2C_ACK(I2C_T *i2c)
{
    i2c->SCL_WRITE(I2C_PIN_LOW);
    i2c->SDA_WRITE(I2C_PIN_LOW);
    i2c->SCL_WRITE(I2C_PIN_HIGH);
    i2c->SCL_WRITE(I2C_PIN_LOW);
}

/* 写一个字节（MSB 先） */
void I2C_WRITE_BYTE(I2C_T *i2c, uint8_t data)
{
    for (int i = 0; i < 8; i++) {
        i2c->SCL_WRITE(I2C_PIN_LOW);
        if (data & 0x80)
            i2c->SDA_WRITE(I2C_PIN_HIGH);
        else
            i2c->SDA_WRITE(I2C_PIN_LOW);
        data <<= 1;
        i2c->SCL_WRITE(I2C_PIN_HIGH);
    }
    i2c->SCL_WRITE(I2C_PIN_LOW);
}

/* 读一个字节（MSB 先），SDA 需被释放为输入/高电平 */
uint8_t I2C_READ_BYTE(I2C_T *i2c)
{
    uint8_t redata = 0;
    i2c->SDA_WRITE(I2C_PIN_HIGH); /* release SDA (pattern: write high or GPIO input via callback) */
    for (int i = 0; i < 8; i++) {
        i2c->SCL_WRITE(I2C_PIN_LOW);
        i2c->SCL_WRITE(I2C_PIN_HIGH);
        redata <<= 1;
        if (i2c->SDA_READ())
            redata |= 0x01;
    }
    i2c->SCL_WRITE(I2C_PIN_LOW);
    return redata;
}

/* 发送简易写事务： device_address(R/W=0), reg, data */
void I2C_SEND(I2C_T *i2c, uint8_t device_address, uint8_t reg, uint8_t data)
{
    I2C_START(i2c);
    I2C_WRITE_BYTE(i2c, device_address & 0xFE); /* write address */
    /* 用户可在回调内检测 ACK；此处仅按原流程发出 ACK 操作 */
    I2C_ACK(i2c);
    I2C_WRITE_BYTE(i2c, reg);
    I2C_ACK(i2c);
    I2C_WRITE_BYTE(i2c, data);

    I2C_STOP(i2c);
}









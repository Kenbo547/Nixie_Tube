#include "i2c.h"
#define F_CPU 8000000UL
#define F_SCL 100000UL // SCL frequency
#define Prescaler 1
#define TWBR_val ((((F_CPU / F_SCL) / Prescaler) - 16 ) / 2)

void i2c_init(void)
{
	//SCL frequency = CPU Clock Frequency/(16 + 2(TWBR) * Prescaler Value)
	//15 KHZ
	//TWSR = 0x00;	//prescale SCL frequency
	//TWBR = 0xFF;	//bit rate register
	TWBR = (uint8_t)TWBR_val;
}	

void i2c_start_condition(void)
{
	TWCR = 0;
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while ((TWCR & (1 << TWINT)) == 0);
}

void i2c_stop_condition(void)
{
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void i2c_send_byte(unsigned char byte)
{
	TWDR = byte;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while ((TWCR & (1 << TWINT)) == 0);
	
}

void i2c_send_packet(unsigned char value, unsigned char address)
{
	i2c_start_condition();
	i2c_send_byte(address);
	i2c_send_byte(value);
	i2c_stop_condition();
}

unsigned char i2c_recv_byte(void)
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    while ((TWCR & (1 << TWINT)) == 0);
    return TWDR;
}

unsigned char i2c_recv_last_byte(void)
{
	TWCR = (1 << TWINT) | (1 << TWEN);
    while ((TWCR & (1 << TWINT)) == 0);
    return TWDR;
}

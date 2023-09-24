
#ifndef I2C_RTC_H_					
#define I2C_RTC_H_
												
#include <avr/io.h>								
#include <util/delay.h>							
#include <math.h>								
#define SCL_CLK 100000L							
#define BITRATE(TWSR)	((F_CPU/SCL_CLK)-16)/(2*pow(4,(TWSR&((1<<TWPS0)|(1<<TWPS1))))) /* Define bit rate */

void I2C_init();								
uint8_t  I2C_Start(char write_address);			
uint8_t  I2C_Repeated_Start(char read_address);	
void I2C_Stop();								
void I2C_Start_Wait(char write_address);		/* I2C start wait function */
uint8_t  I2C_Write(char data);					/* I2C write function */
int I2C_Read_Ack();								/* I2C read ack function */
int I2C_Read_Nack();							/* I2C read nack function */


#endif											
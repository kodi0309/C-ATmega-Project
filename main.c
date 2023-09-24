#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "LCD/LCD_4bit.h"
#include "add.h"
//#include "LED/LED_segmenty.h"
#include "I2C/I2C_RTC.h"

#define F_CPU 8000000UL

#define LED1 (1<<PA2)
#define LED1_OFF PORTA &=~LED1
#define LED1_ON PORTA |=LED1
#define LED2 (1<<PA3)
#define LED2_OFF PORTA &=~LED2
#define LED2_ON PORTA |=LED2
#define LED3 (1<<PA4)
#define LED3_OFF PORTA &=~LED3
#define LED3_ON PORTA |=LED3
#define LED4 (1<<PA5)
#define LED4_OFF PORTA &=~LED4
#define LED4_ON PORTA |=LED4
#define LED4_ZM PORTA ^=LED4
#define LED5 (1<<PA6)
#define LED5_OFF PORTA &=~LED5
#define LED5_ON PORTA |=LED5
#define LED5_ZM PORTA ^=LED5
#define LED6 (1<<PA7)
#define LED6_OFF PORTA &=~LED6
#define LED6_ON PORTA |=LED6
#define LED6_ZM PORTA ^=LED6


#define SW0 (1<<PD0)
#define SW1 (1<<PD1)
#define SW2 (1<<PD2)
#define SW3 (1<<PD3)

int aktOdleglosc=400;
uint8_t nocG=21;
uint8_t dzienG=6;
uint16_t ostOdleglosc;
uint8_t tryb=1;
uint8_t PWM_V;
void DIP_init()
{
	DDRA|=LED1; //dioda 1
	DDRA|=LED2;
	DDRA|=LED3; 
	DDRA|=LED4;
	DDRA|=LED5;
	DDRA|=LED6;
	DDRD&= ~(SW0); //przycisk 1
	DDRD&= ~(SW1); 
	DDRD&= ~(SW2); 
	DDRD&= ~(SW3); 
}

void miganieLED1()
{
	LED1_ON;
	_delay_ms(500);
	LED1_OFF;
}

int main(void)
{	
	//--- Inicjalizacje ---
	DIP_init(); //diody i przyciski
	LCD_init(); //lcd 
	I2C_init(); //zegar
	ADC_init(); //temperatura
	USS_init(); //czujnik ultradzwiekowy}
	PWM_init();	
	
	//LED_init();
	//sei();
	
	lcd_print_xy(0,0,"HEJ!");
	_delay_ms(5000);
	lcd_clear();

	_delay_ms(20);
	
	while (1)
	{
		int aktGodzina=odczytDatyiCzasu();
		int aktTemperatura=odczytTemperatury();
		

			aktOdleglosc=odczytOdleglosci();
			if (aktOdleglosc<200)
			{
				LED2_ON;
			}
			else
			{
				LED2_OFF;
			}
		
			if(!(PIND & (SW2))) //(aktTemperatura>25 || aktGodzina>nocG || aktGodzina<dzienG || 
			{
				PWM_V = 100;	
			}
			else
			{
				PWM_V = 50;	
			}
			if(!(PIND & (SW3))) 
			{
				OCR2=PWM_V;
				_delay_ms(200);
			}
			
		
		//zegarLED(R_godzina, R_minuta);

		if (ostOdleglosc!=aktOdleglosc) //wykrywanie ruchu
		{
			LED3_ON;
		}
		else
		{
			LED3_OFF;
		}
		if (aktGodzina<dzienG || aktGodzina>nocG )
		{
			if (ostOdleglosc!=aktOdleglosc)
			{
				LED4_ON;
				LED5_ON;
				LED6_ON;
				_delay_ms(1000);
				LED4_OFF;
				LED5_OFF;
				LED6_OFF;
			}
		}
		
		if(!(PIND & (SW1)))
		{
			LED4_ZM;
			LED5_ZM;
			LED6_ZM;
			_delay_ms(500);
		}
		
		if(!(PIND & (SW0)))
		{
			miganieLED1();
			lcd_print_xy(1,6,"   ");
			temperaturaLCD();
			
			odlegloscLCD();
			
		}
		else
		{
			dataiczasLCD();		
		}
		
		ostOdleglosc=aktOdleglosc;
	}
}

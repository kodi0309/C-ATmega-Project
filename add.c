#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <util/delay.h>

#include "avr/pgmspace.h"
#include "add.h"
#include "LCD/LCD_4bit.h"
#include "I2C/I2C_RTC.h"
//#include "SEG/LED_segmenty.h"

//data i czas
//zmienne odczytane z RTC
int R_sekunda,R_minuta,R_godzina,R_dzienTyg,R_dzien,R_miesiac,R_rok;
//tablice dla zapisania daty i czasu
char data[20], czas[20];
//tekstowe dni tygodnia
char* dniTygodnia[7]= {"Ndz","Pon","Wt","Sr","Czw","Pt","Sob"};

////zmienne dla konwersji do LED - segmenty
//char A_minuta[3], A_godzina[3];
////skonwertowane liczby do LED - segmenty
//int I_minuta, I_godzina;
////podzial dla wyswietlenia na LED - segmenty
//int DD0_godzina, DD1_godzina, DD2_minuta, DD3_minuta;

//--- RTC ---
#define RTC_Ares_Zapisu		0xD0
#define RTC_Ares_Odczytu	0xD1

//--- USS ---
#define PRIN_TRIG PD4

int Timer_Ovf = 0;
char S_odleglosc[10];
long zliczanie;  
double D_odleglosc;

//--- ADC ---
#define stopien 0xdf

char teperatura[10];
float teperatura_StC;

//--- funcje RTC ---
void RTC_OdczytajCzas(char adresCzasu)
{
	I2C_Start(RTC_Ares_Zapisu);
	I2C_Write(adresCzasu);
	I2C_Repeated_Start(RTC_Ares_Odczytu);

	R_sekunda = I2C_Read_Ack();
	R_minuta = I2C_Read_Ack();
	R_godzina = I2C_Read_Nack();
	I2C_Stop();
}

void RTC_OdczytajDate(char adresDaty)
{
	I2C_Start(RTC_Ares_Zapisu);
	I2C_Write(adresDaty);
	I2C_Repeated_Start(RTC_Ares_Odczytu);

	R_dzienTyg = I2C_Read_Ack();
	R_dzien = I2C_Read_Ack();
	R_miesiac = I2C_Read_Ack();
	R_rok = I2C_Read_Nack();
	I2C_Stop();
}

void zegarLED(int R_godzina, int R_minuta)
{
	//sprintf(A_godzina,"%02x ",R_godzina);
	//sprintf(A_minuta,"%02x ",R_minuta);
	//
	//I_godzina=atoi(A_godzina);
	//I_minuta=atoi(A_minuta);
	//
	//DD0_godzina=I_godzina/10;
	//DD1_godzina=I_godzina%10;
	//DD2_minuta=I_minuta/10;
	//DD3_minuta=I_minuta%10;
	//
	//
	//nrLED_SD[0]=DD0_godzina;
	//nrLED_SD[1]=DD1_godzina;
	//nrLED_SD[2]=DD2_minuta;
	//nrLED_SD[3]=DD3_minuta;
}

int odczytDatyiCzasu(void)
{
	RTC_OdczytajCzas(0);
	sprintf(czas, "%02x:%02x:%02x               .", (R_godzina & 0b00011111), R_minuta, R_sekunda);
	
	RTC_OdczytajDate(3);
	sprintf(data, "%02x/%02x/%02x %3s          .", R_dzien, R_miesiac, R_rok,dniTygodnia[R_dzienTyg-1]);
	
	return (int)R_godzina;
}

void dataiczasLCD()
{
	lcd_print_xy(0,0,czas);
	lcd_print_xy(1,0,data);
}


//--- funkcje ADC ---
void ADC_init(){
	DDRA &= (0<<0);	        
	ADCSRA = 0x87;    //ADC wlaczone, 128p      
	ADMUX = 0x40;    // Vref Avcc, na ADC0     
}

int ADC_odczyt(char kanal)
{
	ADMUX = 0x40 | (kanal & 0x07);	    //wlacz odczyt		
	ADCSRA |= (1<<ADSC);				//rozp konwersje			
	while (!(ADCSRA & (1<<ADIF)));		//zaczekaj 			
	ADCSRA |= (1<<ADIF);				//wyczysc flage przerwania
	_delay_ms(1);									
	return ADCW;						
}

int odczytTemperatury()
{
	teperatura_StC = ((ADC_odczyt(0)*0.32)-40);
	sprintf(teperatura,"Temp: %d%cC    ", (int)teperatura_StC, stopien);               
	
	return (int)teperatura_StC;
};

void temperaturaLCD()
{
	lcd_print_xy(0,0,teperatura);
	memset(teperatura,0,10);
}


//--- funkcje USS ---
ISR(TIMER1_OVF_vect)
{
	Timer_Ovf++;	
}

void USS_init()
{
	DDRD |= (1<<4);		
	PORTD |= (1<<6);		
	sei();			
	TIMSK = (1 << TOIE1);	//w?aczenie dla timera 1 zezwolenia na przepelnienie
	TCCR1A = 0;		
}

int odczytOdleglosci()
{
	PORTD |= (1 << PRIN_TRIG);  //impuls nadawanie
	_delay_us(10);
	PORTD &= (~(1 << PRIN_TRIG));
	
	TCNT1 = 0;	//wyczyszczenie
	TCCR1B = 0x41;	//zbocze narastajace, bez preskalera
	TIFR = 1<<ICF1;	//wyczysc flage wejsciowa przechwytywania
	TIFR = 1<<TOV1;	//wyczy?? flage przepelnienia zegara
	
	while ((TIFR & (1 << ICF1)) == 0); //czekanie na zbocze narastajace
	TCNT1 = 0;	//wyczysc TC
	TCCR1B = 0x01;	//zbocze opadajace, bez preskalera
	TIFR = 1<<ICF1;	//
	TIFR = 1<<TOV1;	
	Timer_Ovf = 0;

	while ((TIFR & (1 << ICF1)) == 0);  //zbocze opadajace
	zliczanie = ICR1 + (65535 * Timer_Ovf);	 
	D_odleglosc = (double)zliczanie/58;
	
	return (int)D_odleglosc;
}

void odlegloscLCD()
{
	char odleglosc1[10];
	int I_odleglosc1=(int)D_odleglosc;
	sprintf(odleglosc1, "%d", I_odleglosc1);
	lcd_print_xy(1,0,"Odl = ");
	lcd_print_xy(1,6,odleglosc1);
	lcd_print_xy(1,9," cm   ");
}


void PWM_init()
{
	DDRD |= (1<<7);								
	TCCR2 = 0x6A; //6C
}



////obsluga wyswietlacza 4-segmentowego led, wykorzystywanego do wyswietlania godziny
//
//#include "avr/io.h"
//#include "avr/pgmspace.h"
//#include "avr/interrupt.h"
//
//#include "LED_segmenty.h"
//
//const uint8_t ukladBK[] PROGMEM = {
	//~(segA|segB|segC|segD|segE|segF), 
	//~(segB|segC), 
	//~(segA|segB|segD|segE|segG), 
	//~(segA|segB|segC|segD|segG), 
	//~(segB|segC|segF|segG), 
	//~(segA|segC|segD|segF|segG), 
	//~(segA|segC|segD|segE|segF|segG), 
	//~(segA|segB|segC|segF), 
	//~(segA|segB|segC|segD|segE|segF|segG), 
	//~(segA|segB|segC|segD|segF|segG), 
	////0xFF 
//};
//
////const uint8_t ukladZK[] PROGMEM = {
	////~(segA|segB|segC|segD|segE|segF|segDP), 
	////~(segB|segC|segDP), 
	////~(segA|segB|segD|segE|segG|segDP), 
	////~(segA|segB|segC|segD|segG|segDP), 
	////~(segB|segC|segF|segG|segDP), 
	////~(segA|segC|segD|segF|segG|segDP), 
	////~(segA|segC|segD|segE|segF|segG|segDP), 
	////~(segA|segB|segC|segF|segDP), 
	////~(segA|segB|segC|segD|segE|segF|segG|segDP), 
	////~(segA|segB|segC|segD|segF|segG|segDP), 
	////0xFF
////};
//
//
//uint8_t nrLED_SD[4];
//
//
//void LED_init(void)
//{
	//ddrSEG |= (1<<1) | (1<<2) | (1<<3) | (1<<4)| (1<<5) | (1<<6) | (1<<7); //wyjscie na 1-7
	//portSEG |= (1<<1) | (1<<2) | (1<<3) | (1<<4)| (1<<5) | (1<<6) | (1<<7);
	//
	//ddrSD |= LED_SD1 | LED_SD2 |LED_SD3 | LED_SD4;
	//portSD |= LED_SD1 | LED_SD2 | LED_SD3 | LED_SD4;
	//
		////Ustawienia TIMER0
		//TCCR0 |=(1<<WGM01); //CTC
		//TCCR0 |=(1<<CS02);
		//OCR0 = 10;
		//TIMSK |= (1<<OCIE0);
//}
//
//ISR(TIMER0_COMP_vect)
//{
	//static uint8_t nr;
	//
	//portSD=(portSD | maskaSD);
	//
	//if(nr==1)
	//{
		//portSEG = pgm_read_byte (&ukladZK[nrLED_SD[nr]] );
	//}
	//else
	//{
		//portSEG = pgm_read_byte (&ukladBK[nrLED_SD[nr]] );
	//};
	//
	//portSD=(portSD & ~maskaSD) | (~(1<<nr) & maskaSD);
	//
	//nr++;
	//if(nr>3) nr=0;
	//
//}
#ifndef add_H_

#define add_H_

#define RTC_Ares_Zapisu		0xD0
#define RTC_Ares_Odczytu	0xD1


int R_minuta,R_godzina;

char data[20], czas[20];



void DIP_init();
void USS_init();
void ADC_init();
void PWM_init();
int odczytDatyiCzasu(void);
//void zegarLED(int R_godzina, int R_minuta);
void dataiczasLCD();

int odczytTemperatury();
int odczytOdleglosci();
void temperaturaLCD();
void odlegloscLCD();

#endif

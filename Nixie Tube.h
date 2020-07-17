/*
 * Nixie_Tube.h
 *
 * Created: 16/01/2020 18:05:17
 *  Author: KENBO
 */ 
#define F_CPU 8000000UL
#include "rtc3231.h"
#include "ds18b20.h"
#include "mcp4017.h"

#ifndef NIXIE TUBE_H_
#define NIXIE TUBE_H_

#define Cathode_0 PORTC2
#define Cathode_1 PORTC1
#define Cathode_2 PORTB2
#define Cathode_3 PORTE0
#define Cathode_4 PORTB0
#define Cathode_5 PORTD7
#define Cathode_6 PORTD6
#define Cathode_7 PORTD4
#define Cathode_8 PORTD3
#define Cathode_9 PORTC3

#define Anode_1 PORTE1
#define Anode_2 PORTB6
#define Anode_3 PORTB7
#define Anode_4 PORTD5
#define Anode_5 PORTE2
#define Anode_6 PORTE3

#define PWM_DImming PORTD5
#define Mode_Button PORTB3
#define Up_Button PORTB4
#define Down_Button PORTB5
#define PIR_Sensor PORTD2
#define SHDWN_Power_170V PORTD0
#define Double_Point PORTC0
#define Temp_OneWire PORTD1
void Loop();
void Read_Time();
void Write_Time();
void Clear_Cathodes();
void Blanking();
void Read_Temperature();
void Digit_To_Cathode(uint8_t Digit);








#endif /* NIXIE TUBE_H_ */